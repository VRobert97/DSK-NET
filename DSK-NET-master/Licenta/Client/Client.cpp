#include "Client.hpp"
//a static pointer to the instance of the object
static Client* clientPtr;
//winsock initialization when we create the new Client object
Client::Client(std::string IP, int PORT, bool administrator)
{
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);

	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	addr_.sin_addr.s_addr = inet_addr(IP.c_str());      
	addr_.sin_port = htons(PORT);    
	addr_.sin_family = AF_INET; 
	admin=administrator; 
	clientPtr = this;             
}
//the connect function: we try to connect to a server
//upon failurie we exit and throw an error
//upon being succesuful we create a thread and start the comunication later
bool Client::Connect()
{
	connection_ = socket(AF_INET, SOCK_STREAM, NULL);   
	if (connect(connection_, (SOCKADDR*)&addr_, sizeOfAddr_) != 0)
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	if(admin==FALSE)
	{
		std::cout << "Connected as guest!" << std::endl;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientThread, NULL, NULL, NULL);  
	}
	else
	{
		std::cout << "Connected as admin!"<<std::endl;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)adminThread, NULL, NULL, NULL);
	}           
	return true;
}

SOCKET* Client::getSocket()
{
	return &connection_;
}
//the guest thread itself, with which we try to recieve messages or we try to send them to the Server we are connected on
void Client::clientThread()
{
	Packet PacketType;
	while (true)
	{
		if (!clientPtr->GetPacketType(&(clientPtr->connection_), PacketType))   
			break;            
		if (!clientPtr->processPacket(clientPtr->connection_, PacketType))    
			break;     
		
	}
	std::cout << "Lost connection to the server." << std::endl;
	if (clientPtr->closeConnection())           
		std::cout << "Socket to the server was closed successfuly." << std::endl;
	else             
		std::cout << "Socket was not able to be closed." << std::endl;
}
//the admin thread for us to send admin specific commands
void Client::adminThread()
{
	Packet PacketType;
	while (true)
	{
		if (!clientPtr->GetPacketType(&(clientPtr->connection_), PacketType))   
			break;            
		if (!clientPtr->processPacket(clientPtr->connection_, PacketType))    
			break;     
		
	}
	std::cout << "Lost connection to the server." << std::endl;
	if (clientPtr->closeConnection())           
		std::cout << "Socket to the server was closed successfuly." << std::endl;
	else             
		std::cout << "Socket was not able to be closed." << std::endl;
}
bool Client::closeConnection()
{
	if (closesocket(connection_) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK)                     
			return true;        

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}
//here is the function that proceses messages
//so far it can only process strings
bool Client::processPacket(SOCKET& source, Packet _packettype)
{
	std::string Message;
	switch (_packettype)
	{
		case P_ChatMessage:       
		{
			if (!GetString(source, Message))          
				return false;    
			std::cout << Message << std::endl;  
			break;
		}
		case P_IdentifyRequest:
		{
			if(admin==TRUE)
			{
				Message="Administrator";
				sendString(source, Message);
			}
			else
			{
				Message="Guest";
				sendString(source, Message);
			}
			break;
		}
		default:       
			std::cout << "Unrecognized packet: " << _packettype << std::endl;      
			break;
	}
	return true;
}