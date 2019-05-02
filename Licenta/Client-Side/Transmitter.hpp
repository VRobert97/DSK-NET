#ifndef Transmitter_FILE_HPP_
#define Transmitter_FILE_HPP_

#pragma comment(lib,"ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>

#include "Packet.hpp"

class SentMessage {
protected:
	bool sendInt(SOCKET&, int);
	bool sendPacketType(SOCKET&, Packet);
	bool sendString(SOCKET*, std::string&);
};

#endif // Transmitter_HPP_