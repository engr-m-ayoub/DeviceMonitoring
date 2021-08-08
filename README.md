# DeviceMonitoring
TCP/IP device monitoring


# Project Hierarchy 
DeviceMonitoring
    |- build
    |
    |- inc
        |- monitoring.h
    |
    |- src
        |- monioring_server.cpp
        |- monioring_client.cpp
    |
    |- Makefile
    |- README.md
    
# How to build
    Makefile has been added to the system which support clean & build targets. Run following command to build
    
    $ make clean
    $ make build

    or (run make only, it will automatically clean and build for you.)
    $ make 

    built server and client binaries will be available in 'build' folder.

# How to run
    There are two components of this project, client and server. Follow these steps to run:

    1- run server
    $ ./build/monitoring_server
    
    2- run client and pass device name as argument
    $ ./build/monitoring_client [device_name] [server_ip]

    e.g ./build/monitoring_client "device_iot_1" "127.0.01"

# Features
1- server counts total messages from all the devices
2- serve keeps track of messages count for each device and diffrentiate a device with its name.
3- client send dummy data to server but device name remains same throughout session
4- on client restart over a device, server will retain its messages count.
5- on server restart, all clients get disconnected and server loose messages count for all devices


# Testing
This client and server were deployed on same machine and tested for given features.
