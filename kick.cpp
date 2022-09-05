# include <iostream>
# include "client.hpp"
# include <map>
# include "channel.hpp"
#include "server.hpp"
#include <sstream>

void	Server::requestKickMsg(std::map<SOCKET, Client*>::iterator &iter, \
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

std::vector<std::string> split(std::string input, char delimiter) {
    std::vector<std::string> answer;
    std::stringstream ss(input);
    std::string temp;
 
    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }
 
    return answer;
}

void Server::kickTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
    // 검색
    std::vector<std::string> splitVector = split(param, ' ');
    if (splitVector.size() != 2)
    {
        //error;
    }
    else {
        // 권한 검색 필요
        std::string channelName = splitVector[0];
        std::string kickNick = splitVector[1];

        
        std::map<std::string, Channel *>::iterator channelIter = chann_map_.find(channelName);
        if (channelIter != chann_map_.end())
        //(void)channelIter;
        //channelIter->second->

        requestKickMsg(iter , command, param);
    }
    
}