#include "Client.hpp"
#include <iostream>
#include <string>

int main()
{
	//we give the client the ip = 127.0.0.1 and the port=1111 on which he will try to connect to a server available
	Client myClient("127.0.0.1", 1111);        

	if (!myClient.Connect())     
	{
		std::cout << "Failed to connect to server" << std::endl;
		system("pause");
	}
	//here we process the input recieved from the command line and we send it to the server we are connected to
	std::string userinput;
	while (true)
	{
		std::getline(std::cin, userinput);          
		if (!myClient.sendString(myClient.getSocket(), userinput))          
			break;                   
	}
}