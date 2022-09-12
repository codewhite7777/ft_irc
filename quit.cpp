#include "client.hpp"
#include "server.hpp"
void		Server::quitTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{

    int quit_fd = iter->second->getSocket();
    std::cout << "socket NUM :" << quit_fd << "\n";
    for (MAP<STRING, Channel*>::iterator channer_iter = chann_map_.begin()
        ; channer_iter != chann_map_.end()
        ; ++channer_iter
    )
    {
        Channel * channel_info = channer_iter->second;  
        channel_info->eraseOper(quit_fd);
        channel_info->eraseUser(quit_fd);
        MAP<SOCKET, Client*>::iterator iter = client_map_.find(quit_fd);
        if (iter != client_map_.end())
            client_map_.erase(iter);
    }
    (void)command;
    (void)param;
}