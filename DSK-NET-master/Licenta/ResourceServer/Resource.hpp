#ifndef RESOURCE_FILE_HPP_
#define RESOURCE_FILE_HPP_

#include "Resource.hpp"
#include "Receiver.hpp"
#include "Transmitter.hpp"

const int NUMBER_OF_RESOURCE_CONNECTIONS = 100;

class Resource {
    public:
        //server initializing, here we also initialize and start WinSock
        Resource(int);
        //listening for connections
        bool listenForNewConnection();
    private:
        static void resourceThread(int ID);
        bool processRequest(int, RequestType);

    private:
        int totalConnections_=0;
        SOCKET connections_ [NUMBER_OF_RESOURCE_CONNECTIONS];
        SOCKADDR_IN addr_;
        int addrLen_ = sizeof(addr_);
        SOCKET sListen_;

};

#endif //RESOURCE_FILE_HPP_