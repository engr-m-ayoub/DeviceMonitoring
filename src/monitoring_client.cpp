#include "monitoring.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " [deviceName] [serverip]" << endl;
        return -1;
    }

    cout << endl
         << endl;
    cout << "****************************************************************************" << endl;
    cout << "************************* Device Monitoring Client *************************" << endl;
    cout << "****************************************************************************" << endl
         << endl;

    int sock_c;
    struct sockaddr_in addr_s;

    md_data_t data;
    memset(&data, 0, sizeof(data));
    strncpy(data.device_name, argv[1], sizeof(data.device_name));

    // create socket
    if ((sock_c = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cerr << "Failed to create socket." << endl;
        return -1;
    }

    // fill IP/Port and connect
    memset(&addr_s, 0, sizeof(addr_s));
    addr_s.sin_family = AF_INET;
    addr_s.sin_addr.s_addr = inet_addr(argv[2]);
    addr_s.sin_port = SERVICE_PORT;

    if (connect(sock_c, (struct sockaddr *)&addr_s, sizeof(addr_s)) < 0)
    {
        cerr << "Failed to connect to Monitoring Server @." << string(argv[2]) << endl;
        return -2;
    }

    int counter = 0;
    while (1)
    {
        data.mt1 = rand() % 100;
        data.mt2 = (double)(rand() % 100) + (double)((rand() % 10) / 10.0);

        if (write(sock_c, &data, sizeof(data)) <= 0)
        {
            cerr << "Failed to write data." << endl;
            break;
        }
        //read(sock_c, &data, sizeof(data));
        cout << "[" << ++counter << " ] [ " << data.print_data() << " ]" << endl;
        sleep(5);
    }
    close(sock_c);

    cout << endl
         << endl;
    cout << "************************************************************************************" << endl;
    cout << "************************* Goodbye Device Monitoring Client *************************" << endl;
    cout << "************************************************************************************" << endl;

    return 0;
}
