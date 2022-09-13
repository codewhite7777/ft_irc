#include "server.hpp"
#include <fcntl.h>
#include <netinet/tcp.h>
#include <sstream>
#include <vector>
#define VECTOR std::vector
#define MAP std::map
void	Server::requestJoin(std::map<SOCKET, Client*>::iterator &client_iter, \
						std::string& command, std::string& param)
{
	VECTOR<STRING> server_split = split(param, ',');
	
	// ,으로 구분해주고 일일이 넣어줘야한다.
	for (unsigned int i = 0 ; i < server_split.size() ; ++i)
	{
		std::cout << "TEST tq[" << server_split[i] << "]\n";
		MAP<STRING, Channel*>::iterator chan_iter = chann_map_.find(server_split[i]);
		if (chan_iter == chann_map_.end())
		{
			std::cout << "make channel\n";
			Channel* new_chann = new Channel(server_split[i]);
			chann_map_.insert(std::make_pair(server_split[i], new_chann));
			std::cout << "new channel created, chann name: [" << param << "]\n"; // test

			new_chann->assignOper(client_iter->second->getNickName(), client_iter->second);
			new_chann->assignUser(client_iter->second->getNickName(), client_iter->second);
		}
		// 찾았을 경우
		else
		{
			std::cout << "already channel exist\n";
			Channel* channel_info = chan_iter->second;
			if (!channel_info->getUsers_().size())
				channel_info->assignOper(client_iter->second->getNickName(), client_iter->second);
			channel_info->assignUser(client_iter->second->getNickName(), client_iter->second);
		}
	}
	(void)command;
}
