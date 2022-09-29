#include "Client.hpp"
#include "Server.hpp"
#include "utils.hpp"

void	Server::requestPrivateMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	std::cout << command << ' ' << param << '\n';

	std::string	tar_nick = param.substr(0, param.find(' '));
	std::string	msg = param.substr(param.find(' ') + 1) + "\r\n";

	std::cout << tar_nick << '\n';
	// 채널 메시지
	// 루프 -> 유효한 방 -> 리스트 명단 -> 보내기
	if (tar_nick[0] == '#')
	{
		std::map<std::string, Channel*>::iterator chanIter = chann_map_.find(tar_nick);
		if (chanIter == chann_map_.end())
		{
			std::cout << "not found \n";
		}
		else {
            MAP<STRING, Client*> client_map = chanIter->second->getUsers_();
			
			
			for (MAP<STRING, Client*>::iterator client_iter = client_map.begin()
                ; client_iter != client_map.end() 
                ; ++client_iter)
			{
                Client * client_info = client_iter->second;
                
				// std::cout << (*it)->getNickName() << ' ' << iter->second->getNickName() << '\n';
                // 나 자신을 뺀 나머지에게 보내야 한다.
				if (client_info->getNickName() != iter->second->getNickName())
				{
					std::string message = getUserInfo(iter->second->getNickName(), iter->second->getUserName(), iter->second->getName()) \
						+ " PRIVMSG " + tar_nick + " :" + msg;
					message += "\r\n";
					std::cout << message << '\n';
					insertSendBuffer(client_info, message);
				}
			}
		}
		
	}
	else {
		for (std::map<SOCKET, Client*>::iterator c_iter = client_map_.begin(); c_iter != client_map_.end(); c_iter++)
		{
			std::cout << "name Test: " << c_iter->second->getNickName() <<' ' << tar_nick << '\n';
			if (c_iter->second->getNickName() == tar_nick)
			{
				msg = getUserInfo(iter->second->getNickName(), iter->second->getUserName(), iter->second->getName()) \
					+ " PRIVMSG " + tar_nick + " :" + msg;
				msg += "\r\n";
				std::cout << msg << '\n';
				insertSendBuffer(c_iter->second, msg);
				return ;
			}
		}
	}
}