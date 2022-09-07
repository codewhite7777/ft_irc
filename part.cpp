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
        // if (c_iter != iter)
        // {
            msg = getUserInfo(iter->second->getNickName(), iter->second->getUserName(), "bar.example.com ") \
                + command + " " + param + " :Bye\r\n"; 
            std::cout << msg << '\n';
            insertSendBuffer(c_iter->second, msg);

        // }
    }
}


void Server::partTest(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
    // 검색
    std::string channelName = param;
    std::map<std::string, Channel *>::iterator channelIter = chann_map_.find(channelName);
    if (channelIter == chann_map_.end())
    {
        std::cout << "tq";
        exit(0);
    }
    int partSocket = iter->second->getSocket();


    std::cout << "PART TEST START\n";
    std::cout << "param :  [" << channelName << "]\n";
    std::cout << "partSocket :  [" << partSocket << "]\n";
    
    std::vector<Client*> ClientList = channelIter->second->getUsers();
    std::cout << ClientList.size() << '\n';
    //channel에서 삭제
    for (unsigned int i = 0 ; i < ClientList.size() ; ++i)
    {
        std::cout << i << '\n';
        // 삭제 로직
        if (partSocket == ClientList[i]->getSocket())
        {
            std::cout << "Fount it\n";
            channelIter->second->eraseUser(i);   
            break;
        }
    }
    //oper 삭제
    channelIter->second->eraseOper(partSocket);
    
    // 나가기
    requestPartMsg(iter , command, param);
}