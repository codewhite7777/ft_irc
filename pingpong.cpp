
#include "server.hpp"

void	Server::requestPing(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command)
{
    if (command != "PING")
        std::cout << "NO PING\n";

	//채널 메시지
	//루프 -> 유효한 방 -> 리스트 명단 -> 보내기

	//개인 메시지
	for (std::map<SOCKET, Client*>::iterator c_iter = client_map_.begin(); c_iter != client_map_.end(); c_iter++)
	{
		std::string msg = getUserInfo(iter->second->getNickName(), iter->second->getUserName(), iter->second->getHostName()) \
				+ " PONG " + iter->second->getNickName();
        insertSendBuffer(c_iter->second, msg);
        return ;
	}
}