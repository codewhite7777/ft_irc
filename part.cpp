# include <iostream>
# include "client.hpp"
# include <map>
# include "channel.hpp"
#include "server.hpp"

void	Server::requestPartMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
    // nick!user@host
    std::string msg = "";
    for (std::map<SOCKET, Client*>::iterator c_iter = client_map_.begin(); c_iter != client_map_.end(); c_iter++)
    {
        if (c_iter != iter)
        {
            msg = getUserInfo(iter->second->getNickName(), iter->second->getUserName(), "bar.example.com ") \
                + command + param + " :Bye"; 
            insertSendBuffer(c_iter->second, msg);
            return ;
        }
    }
}


void Server::partTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
    // 검색
    std::string channelName = param;

    std::map<std::string, Channel *>::iterator channelIter = chann_map_.find(channelName);
    
    // 없을 경우가 없음.
    (void)channelIter;
    // todo: channel.sendMsgOtherUsers()
    // channelIter->second->벡터값에서 빼기
    // iter->second->
    
    // 나가기
    requestPartMsg(iter , command, param);
}