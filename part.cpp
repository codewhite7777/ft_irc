# include <iostream>
# include "client.hpp"
# include <map>
# include "channel.hpp"
#include "server.hpp"
#include "utils.hpp"

void	Server::requestPartMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
    // nick!user@host
    std::string msg = "";
    for (std::map<SOCKET, Client*>::iterator c_iter = client_map_.begin(); c_iter != client_map_.end(); c_iter++)
    {
        msg = getUserInfo(iter->second->getNickName(), iter->second->getUserName(), "bar.example.com ") \
            + command + " " + param + " :Bye\r\n"; 
        std::cout << msg << '\n';
        insertSendBuffer(c_iter->second, msg);
    }
}


void Server::partTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
    // #체널 + 메세지 올 경우    
    std::vector<std::string> splitted_param = split(param, ' ');

    std::string channelName = splitted_param[0];
    std::map<std::string, Channel *>::iterator channel_iter = chann_map_.find(channelName);

    // 없다면 part할 필요가 없음
    if (channel_iter == chann_map_.end())
        return ;
    
    // 나갈 친구의 fd
    STRING partNick = iter->second->getNickName();

    std::cout << "PART TEST START\n";
    std::cout << "param :  [" << channelName << "]\n";
    std::cout << "partSocket :  [" << partNick << "]\n";
    
    MAP<STRING, Client*> client_map = channel_iter->second->getUsers_();
    
    MAP<STRING, Client*>::iterator client_iter = client_map.find(partNick);
    // 내가 찾는 친구가 없네? 그럼 끝
    if (client_iter == client_map.end())
        return ;
    // 나가기
    channel_iter->second->eraseUser(partNick);
    channel_iter->second->eraseOper(partNick);
    
    // 명령어 요청
    requestPartMsg(iter , command, param);
    // todo: part leaving message
}