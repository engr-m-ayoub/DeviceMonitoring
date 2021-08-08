#include "monitoring.h"
#include <map>
#include <signal.h>
#include <algorithm>

using namespace std;

int sock_s = 0, sock_c[MAX_DEVICES_ALLOWED];
map<string, long> dev_msgs; //store message count for each device.
long total_messages = 0;

// signal handling callback
void signal_handler(int signum)
{
    cout << endl << "Stopping Monitoring service after SIGNAL#: " << signum << endl;

    for (int i = 0; i < MAX_DEVICES_ALLOWED; i++)
    {
        if (sock_c[i] > 0)
            close(sock_c[i]);
    }

    close(sock_s);

    cout << "Connection with all devices has been closed." << endl
         << endl
         << endl;

    cout << "*********************************************************************" << endl;
    cout << "********************* GoodBye Monitoring Server *********************" << endl;
    cout << "*********************************************************************" << endl;

    cout << "Total Messages Received ::" << total_messages << endl;
    cout << "Total Devices Connected ::" << dev_msgs.size() << endl;

    cout << endl << "Device Messages Count Summary:" << endl;
    for (auto it = dev_msgs.begin(); it != dev_msgs.end(); it++)
    {
        cout << "Device Name:: " << it->first << " - Messages Count::" << it->second << endl;
    }

    exit(signum);
}

int main(int argc, char *argv[])
{
    cout << endl
         << endl;
    cout << "*********************************************************************" << endl;
    cout << "**************** Welcome to Device Monitoring Server ****************" << endl;
    cout << "*********************************************************************" << endl
         << endl;

    int sock_max;
    struct sockaddr_in addr_s, addr_c;
    fd_set fds;
    md_data_t data;
    memset(&data, 0, sizeof(data));

    // clear all client sockets before using
    memset(sock_c, 0, sizeof(sock_c));

    // handle ^C signal
    signal(SIGINT, signal_handler);

    // create socket
    if ((sock_s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cerr << "Failed to create socket for server." << endl;
        return -1;
    }

    //make the socket reuseable
    int option = 1;
    if (setsockopt(sock_s, SOL_SOCKET, SO_REUSEPORT | SO_REUSEPORT, (const void *)&option, sizeof(option)) < 0)
    {
        cerr << "Failed to set socket port/addr reuseable option." << endl;
        return -2;
    }

    // fill IP/Port and bind socket
    memset(&addr_s, 0, sizeof(addr_s));
    addr_s.sin_family = AF_INET;
    addr_s.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_s.sin_port = SERVICE_PORT;

    if (bind(sock_s, (struct sockaddr *)&addr_s, sizeof(addr_s)) < 0)
    {
        cerr << "Failed to bind server socket." << endl;
        return -3;
    }

    cout << "Device Monitoring service is ready for connections ..." << endl;

    // listen for incoming connections from devices
    if (listen(sock_s, LISTEN_QUEUE_SIZE) < 0)
    {
        cerr << "Failed to listen on server socket." << endl;
        return -4;
    }

    while (1)
    {
        // reset all sockets fds
        FD_ZERO(&fds);

        // add server and clients sockets to fd set
        sock_max = sock_s;
        FD_SET(sock_s, &fds);

        for (int i = 0; i < MAX_DEVICES_ALLOWED; i++)
        {
            //update maximum fd value
            if (sock_max < sock_c[i])
                sock_max = sock_c[i];

            // add client sock to fd set
            if (sock_c[i] > 0)
                FD_SET(sock_c[i], &fds);
        }

        // wait for read activity on any of clients or server socket
        // timeout is NULL to waut indefinite
        int ret = select(sock_max + 1, &fds, NULL, NULL, NULL);

        if (ret < 0) // error case
        {
            cerr << "Failed to select due to an error." << endl;
            continue;
        }

        // check message on server sock_s
        if (FD_ISSET(sock_s, &fds))
        {
            // accept incoming connection from device
            socklen_t addr_c_len = sizeof(addr_c);
            int sock = 0;
            if ((sock = accept(sock_s, (struct sockaddr *)&addr_c, &addr_c_len)) > 0)
            {
                cout << "A New Device connected.\n"
                     << endl;
                for (int i = 0; i < MAX_DEVICES_ALLOWED; i++)
                {
                    if (sock_c[i] == 0)
                    {
                        sock_c[i] = sock;
                        break;
                    }
                }
            }
        }

        // check message on all client sockets
        for (int i = 0; i < MAX_DEVICES_ALLOWED; i++)
        {
            if (sock_c[i] == 0)
                continue;

            if (FD_ISSET(sock_c[i], &fds))
            {
                //clear data
                memset(&data, 0, sizeof(data));

                // handle device message and close client sock if needed
                int read_ret = read(sock_c[i], &data, sizeof(data));
                if (read_ret == 0)
                {
                    sock_c[i] = 0;
                    close(sock_c[i]);
                    continue;
                }
                int counter = 0;
                auto it = dev_msgs.find(string(data.device_name));
                if (it == dev_msgs.end())
                {
                    dev_msgs[string(data.device_name)] = 1;
                    counter = 1;
                }
                else
                {
                    (*it).second++;
                    counter = (*it).second;
                }

                total_messages++;
                cout << "[Total Messages: " << total_messages << "] [" << counter << " ] [" << data.print_data() << "]" << endl;
            }
        }
    }

    //close server socket
    close(sock_s);
    return 0;
}