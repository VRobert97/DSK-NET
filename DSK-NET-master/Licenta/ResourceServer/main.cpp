#include "Resource.hpp"
#include<iostream>
int main()
{
    Resource ResourceServer(1111);
    for (int i = 0; i < NUMBER_OF_CONNECTIONS; ++i)
		ResourceServer.listenForNewConnection();        

	std::cin.get();


    return 0;
}