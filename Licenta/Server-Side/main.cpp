#include <iostream>

#include "Server.hpp"

int main()
{
	//we specify a port to the server side server on which it will listen for connections
	//we don't specify a ip adress since it will comnnect to localhost
	Server MyServer(1111);   

	for (int i = 0; i < NUMBER_OF_CONNECTIONS; ++i)
		MyServer.listenForNewConnection();        

	std::cin.get();
}