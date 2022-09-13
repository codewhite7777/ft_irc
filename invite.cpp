#include "server.hpp"
#include "utils.hpp"
#define VECTOR std::vector
void Server::inviteTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
    std::cout << "Invite TEST\n";
    VECTOR<STRING> param_split = split(param, ' ');
    STRING target_nick;
    STRING target_channel;
    if (param_split.size() == 2)
    {
        target_nick = param_split[0];
        target_channel = param_split[1];
    }
    else {
        return ;
    }
    STRING msg = ":" + iter->second->getNickName() + "!" +
                iter->second->getUserName() + "@" +
                iter->second->getHostName() + " ";
    msg += command + " " + param + "\r\n";
    for (std::map<SOCKET, Client*>::iterator client_iter = client_map_.begin()
        ; client_iter != client_map_.end() 
        ; ++client_iter
    )
    {
        if (client_iter->second->getNickName() == target_nick)
        {
            insertSendBuffer(client_iter->second, msg);
            STRING reply = ":bar.example.com 341 " + target_channel + " " + "test\r\n" ;
            //TODO need change 
            insertSendBuffer(iter->second, reply);
            break;
        }
    }
    (void)iter;
    (void)command;
    (void)param;
}