#include "Server.hpp"
//a static pointer to the instance (object) of the server we are creating
static Server* serverPtr;
Server::Server(int PORT, bool BroadcastPublically);
{
    //winsock initialization
    WSAData wsaData;
   	WORD DllVersion = MAKEWORD(2, 1);
    if(WSAStartup(DllVersion, &wsaData)!=0)
    {
        MessageBoxA(NULL,"WinSock startup failed","Error",MB_OK|MB_ICONERROR);
        exit(1);
    }
    //if we broadcast publically it means we will try to initialise on any adress, hence the INADDR_ANY
    //otherwise we initialize the server on the localhost
    //so far it will always go on the else branch since broadcastPublically is always false
    if (BroadcastPublically)
		addr_.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr_.sin_addr.s_addr = inet_addr("127.0.0.1");

	addr_.sin_port = htons(PORT);
	addr_.sin_family = AF_INET;
	sListen_ = socket(AF_INET, SOCK_STREAM, NULL);
    //we try to bind the adress to our listening socket, and avfterwards we try to listen on that socket for any event (messages)
	if (bind(sListen_, (SOCKADDR*)&addr_, sizeof(addr_)) == SOCKET_ERROR)
	{
		std::string errorMessage = "Failed to bind the addres to our listening socket. Winsock Error: "
			+ std::to_string(WSAGetLastError());
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(sListen_, SOMAXCONN) == SOCKET_ERROR)
	{
		std::string errorMessage = "Failed to listen on listening socket. Winsock Error: "
			+ std::to_string(WSAGetLastError());
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	serverPtr = this;
}
//here we listen for connections
//upon succesufuly connecting a Client to our Server, we create a thread for him
bool Server::listenForNewConnection()
{
    SOCKET newConnection = accept(sListen_,(SOCKADDR)*&addr_,&addrLen_);
    if(newConnection == 0)
    {
        std::cout<<"Failed to accept the client's connection. "<<std::endl;
        return false;
    }
    else
    {
        std::cout<<"Client Connected! Total number of connections: "<<totalConnections_+1<<std::endl;
        connections_[totalConnections_]=newConnection;
        CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)clientHandlerThread,(LPVOID)(totalconnections_),NULL,NULL);
        ++totalConnections_;
        return true;
    }
}
//here we try to process messages recieved in the thread of the client
//if everything goes well we broadcast the client messages to all the other users that are connected to us.
void Server::clientHandlerThread(int ID)
{
    Packet PacketType;
    while(true)
    {
        If(!serverPtr->getPacketType((serverPtr->connections_+ID),PacketType))
            break;
        if(!serverPtr->processPacket(ID, PacketType))
            break;
    }
    std::cout<<"Lost connection to client ID "<<ID<<std::endl;
    closesocket(serverPtr->connections_[ID]);
    /*
	If we lost connection to a client, we close the socket we last used for his connection and delete him
	*/
	if (ID == serverPtr->totalConnections_) {
		serverPtr->totalConnections_ = serverPtr->totalConnections_ - 1;
	}
	else
	{
		for (int i = ID; i < serverPtr->totalConnections_-1; i++) {
			serverPtr->connections_[i] = serverPtr->connections_[i + 1];
		}
		serverPtr->totalConnections_= serverPtr->totalConnections_-1;

	}

}
//here we process the message
//TO DO: implement other types of messages
bool Server::processPacket(int ID, Packet _packettype)
{
	switch (_packettype)
	{
        case P_ChatMessage:    
        {
            std::string Message;       
            if (!getString(connections_[ID], Message))          
                return false;           
            for (int i = 0; i < totalConnections_; i++)
            {
                if (i == ID) 
                {        
                    continue; 
                }                   
                if (!sendString((connections_ + i), Message))             
                {
                    std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << i << std::endl;
                }
            }
            std::cout << "Processed chat message packet from user ID: " << ID << std::endl;
            break;
        }

        default:       
        {
            std::cout << "Unrecognized packet: " << _packettype << std::endl;      
            break;
        }
	}
	return true;
}