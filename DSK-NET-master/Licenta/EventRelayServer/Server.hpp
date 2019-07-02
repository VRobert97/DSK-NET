#ifndef SERVER_FILE_HPP_
#define SERVER_FILE_HPP_

#include "Receiver.hpp"
#include "Transmitter.hpp"

const int NUMBER_OF_CONNECTIONS = 100;
const int NUMBER_OF_ADMINS = 10;

class Server : public ReceiveMessage, public SentMessage {
public:
	Server(int, bool BroadcastPublically = false);
	bool listenForNewConnection();
private:
	static void clientHandlerThread(int ID);
	static void adminHandlerThread(int ID);
	bool processAdminCommand(int ID, std::string)
	bool processPacket(int, Packet);

private:
	int totalConnections_ = 0;
	SOCKET connections_[NUMBER_OF_CONNECTIONS]; 
	SOCKADDR_IN addr_;         
	int addrLen_ = sizeof(addr_);
	SOCKET adminConnections_[NUMBER_OF_ADMINS];
	int totalAdminConnections_=0;
	SOCKET sListen_;
};

#endif // SERVER_FILE_HPP_