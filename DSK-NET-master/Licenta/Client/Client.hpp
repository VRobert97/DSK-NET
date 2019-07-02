#ifndef CLIENT_FILE_HPP_
#define CLIENT_FILE_HPP_

#include "Receiver.hpp"
#include "Transmitter.hpp"

class Client : public ReceiveMessage, public SentMessage {
public:
	Client(std::string, int, bool);
	bool Connect();
	bool closeConnection();
	SOCKET* getSocket();
private:
	bool processPacket(SOCKET&, Packet);
	static void clientThread();
	static void adminThread();

private:
	SOCKET connection_;     
	SOCKADDR_IN addr_;        
	int sizeOfAddr_ = sizeof(addr_);  
	bool admin=false;    
};

#endif // CLIENT_FILE_HPP_