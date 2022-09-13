#include "client.hpp"
#include "server.hpp"
void		Server::quitTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{

    STRING quit_nick = iter->second->getNickName();
    std::cout << "socket NICK :" << quit_nick << "\n";
    for (MAP<STRING, Channel*>::iterator channer_iter = chann_map_.begin()
        ; channer_iter != chann_map_.end()
        ; ++channer_iter
    )
    {
        Channel * channel_info = channer_iter->second;  
        channel_info->eraseOper(quit_nick);
        channel_info->eraseUser(quit_nick);
        MAP<SOCKET, Client*>::iterator client_iter = client_map_.find(iter->second->getSocket());
        if (client_iter != client_map_.end())
            client_map_.erase(client_iter);
    }
    (void)command;
    (void)param;
}