#include "Transmitter.hpp"
//similar to the Reciever, here we have an auxiliary class that will take care of the sending process
//we have a function for every type of message we are gonna send (so far PacketType/int/String)
bool Transmitter::sendPacketType(SOCKET& source, Packet _packettype)
{
	int RetnCheck = send(source, (char*)&_packettype, sizeof(Packet), NULL);   
	if (RetnCheck == SOCKET_ERROR)         
		return false;    

	return true;     
}

bool Transmitter::sendInt(SOCKET& source, int _int)
{
	int RetnCheck = send(source, (char*)&_int, sizeof(int), NULL);   
	if (RetnCheck == SOCKET_ERROR)         
		return false;    

	return true;     
}

bool Transmitter::sendString(SOCKET* source, std::string & _string)
{
	if (!sendPacketType(*source, P_ChatMessage))          
		return false;

	int bufferlength = _string.size();    
	if (!sendInt(*source, bufferlength))          
		return false; 

	int RetnCheck = send(*source, _string.c_str(), bufferlength, NULL);   
	if (RetnCheck == SOCKET_ERROR)      
		return false;      

	return true;     
}
