#include "Client.hpp"
#include <iostream>
#include <string>

int main()
{
	std::string userinput;
	bool admin_priviledge=FALSE;
	std::cout<"For guest session provite as input the word guest."<<std::endl;
	std::cout<<"For admin session please provide as input the word admin. You will also need to provide the password afterwards."<<std::endl;
	std::cout<<"If you provide anything else the program will close."<<std::endl;
	std::getline(std::cin, userinput);
	if( std::strcmp(userinput,"admin")==0)
	{
		do{
			std::cout<<"Please provide the password,or 0 if you would like to exit."<<std::endl;
			std::getline(std::cin, userinput);
			if(std::strcmp(userinput,"0")==0)
			{
				std::cout<<"Closing.";
				return 0;
			}
			if(std::strcmp(userinput,"licenta2019")==0)
			{
				admin_priviledge=TRUE;
			}
		}while(std::strcmp(userinput,"admin")!=0);
	}
	else
	{
		if(std::strcmp(userinput,"guest")!=0)
		{
			return 0;
		}

	}
	
	//we give the client the ip = 127.0.0.1 and the port=1111,and the priviledge on which he will try to connect to a server available
	Client myClient("127.0.0.1", 1111, admin_priviledge);        

	if (!myClient.Connect())     
	{
		std::cout << "Failed to connect to server" << std::endl;
		system("pause");
		return 1;
	}
	//here we process the input recieved from the command line and we send it to the server we are connected to
	while (true)
	{
		std::getline(std::cin, userinput);          
		if (!myClient.sendString(myClient.getSocket(), userinput))          
			break;                   
	}
	return 0;
}