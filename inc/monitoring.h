#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <errno.h>


#define MAX_DEVICES_ALLOWED 10
#define SERVICE_PORT 8000
#define LISTEN_QUEUE_SIZE MAX_DEVICES_ALLOWED

typedef struct md_data
{
    char device_name[1024];
    int mt1;
    double mt2;

    std::string print_data()
    {
        return std::string(device_name) + " :: mt1: " + std::to_string(mt1) + ", mt2: " + std::to_string(mt2) ;
    }
} md_data_t;