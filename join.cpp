#include "server.hpp"
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sstream>
#include <vector>

void	Server::requestJoin(std::map<SOCKET, Client*>::iterator &client_iter, \
						std::string& command, std::string& param)
{
	// find channel name in chann_map
	for (std::map<std::string, Channel*>::iterator chann_iter = chann_map_.begin()\
		; chann_iter != chann_map_.end()\
		; ++chann_iter)
	{
		if (chann_iter->first == param)
		{
			if (!chann_iter->second->getUsers().size())
				chann_iter->second->assignOper(client_iter->second);
			std::cout << "already channel name\n";
			chann_iter->second->assignUser(client_iter->second);
			client_iter->second->addChannel(chann_iter->second);
			return ;
		}
	}
	std::cout << "make channel\n";
	Channel* new_chann = new Channel(param);
	chann_map_.insert(std::make_pair(param, new_chann));
	std::cout << "new channel created, chann name: [" << param << "]\n"; // test

	new_chann->assignOper(client_iter->second);
	new_chann->assignUser(client_iter->second);
	
	client_iter->second->addChannel(new_chann);
	(void)command;
}
