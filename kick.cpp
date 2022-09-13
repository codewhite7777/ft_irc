# include <iostream>
# include "client.hpp"
# include <map>
# include "channel.hpp"
#include "server.hpp"
#include <sstream>
#include "utils.hpp"
#define VECTOR std::vector

void	Server::requestKickMsg(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
    VECTOR<STRING> split_param = split(param, ' ');
    if (split_param.size() > 1)
    {
        STRING channel_list = split_param[0];
        STRING user_list = split_param[1];

        // TODO:: 지워야하네?ㅋ ㅋ
        VECTOR<STRING> channels = split(channel_list, ',');
        for (unsigned int i = 0 ; i < channels.size() ; ++i)
        {
            STRING channel = channels[i];
            std::cout <<"chan name:" <<  channel << '\n';
            MAP<STRING, Channel*>::iterator chan_iter = chann_map_.find(channel);
            if (chan_iter == chann_map_.end())
            {
                std::cout << "tq 없어\n";
                ;//ERR_NOSUCHCHANNEL
            }
            else 
            {
                VECTOR<STRING> users = split(user_list, ',');
                MAP<STRING, Client*> &user_map = chan_iter->second->getUsers_();
                std::cout << "size:" << user_map.size() << '\n';


                int cnt = 0;
                std::string msg = "";
                std::cout << "2번째 체크: " << user_map.size() << '\n';
                for (MAP<STRING, Client*>::iterator user_iter = user_map.begin()
                    ; user_iter != user_map.end()
                    ; ++user_iter
                )
                {
                    std::cout << "cnt" << cnt << '\n';
                    // if (user_iter->second->getNickName() != iter->second->getNickName())
                    // {
                    msg = getUserInfo(iter->second->getNickName(), iter->second->getUserName(), iter->second->getHostName()) \
                        +" " + command + " " + param + "\r\n";
                    std::cout << "msg:" << msg << "\n";
                    std::cout << user_iter->second->getNickName() << "\n";
                    insertSendBuffer(user_iter->second, msg);
                    // } 
                }

                for (unsigned int i = 0 ; i < users.size() ; ++i)
                {
                    STRING user = users[i];
                    std::cout << "user란:" << user << '\n';
                    MAP<STRING, Client*>::iterator user_iter = user_map.find(user);
                    std::cout << user_iter->second->getNickName() << "asdf\n";
                    if (user_iter == user_map.end())
                    {
                        std::cout << "tq  사람이 없어\n";
                        ;
                    }
                    else 
                    {
                        chan_iter->second->eraseOper(user);
                        chan_iter->second->eraseUser(user);
                    }
                }
                 
                
                
            }
        }
        //comment 일단 무시
    }
}

// std::vector<std::string> split(std::string input, char delimiter) {
//     std::vector<std::string> answer;
//     std::stringstream ss(input);
//     std::string temp;
 
//     while (getline(ss, temp, delimiter)) {
//         answer.push_back(temp);
//     }
 
//     return answer;
// }

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