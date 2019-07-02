#include "Resource.hpp"
static Resource* resourcePtr;

Resource::Resource(int PORT);
{
    //winsock init
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if(WSAStartup(DllVersion, &wsaData)!=0)
    {
        MessageBox(NULL,"WinSock startup failed","Error",MB_OK|MB_ICONERROR);
        exit(1);
    }
    //we initialize it on localhost
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
//upon succesufully connecting we create a thread for the client
bool Resource::listenForNewConnection()
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
        CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)resourceThread,(LPVOID)(totalconnections_),NULL,NULL);
        ++totalConnections_;
        return true;
    }
}
//here we process the request (messages) we recieved
void Resource::resourceThread(int ID)
{
    RequestType requesttype;
    while(true)
    {
        if(!serverPtr->getPacketType((serverPtr->connections_+ID),requesttype))
            break;
        if(!serverPtr->processPacket(ID, requesttype))
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
//the helper function for the request processing
bool Resource::processRequest(int ID,ResourceType _resourcetype)
{
    switch (_resourcetype)
	{
        case R_Discard:    
        {
            char* Message="Request discarded";       
            if (!getString(connections_[ID], Message))  
            {        
                return false;     
            }      
            std::cout << "Processed chat message packet from user ID: " << ID << std::endl;
            /*
            TO DO: some sort of verification of the message
            Since this is the discard branch 
            */
            if (!sendString((connections_ + i), Message))             
                {
                    std::cout << "Failed to send message from client ID: " << ID << " to client ID: " << i << std::endl;
                }
            break;
            /*
            TO DO: add more types of messages
            */
        }

        default:       
        {
            std::cout << "Unrecognized packet: " << _resourceType << std::endl;      
            break;
        }
	}
	return true;
}
