#include "Server.hpp"
#include "Client.hpp"
#include "irc_protocol.hpp"
#include "Message.hpp"
#define RPL_CHANNELMODEIS "324"
#define VECTOR std::vector
#define STRING std::string
#define print std::cout
#include "util.h"
void		Server::requestMode(
	std::map<SOCKET, Client*>::iterator &iter,
	std::string& command,
	std::string& param
)
{
	std::string	channelName;
	std::string	option;
	std::string	user_info = ":" + iter->second->getNickName();
	//BUF확인 하기

	VECTOR<STRING> paramSplitV = split(param, ' ');
	print << "size:" << paramSplitV.size() << '\n';
	if (paramSplitV.size() <= 1)
	{
		// 461    ERR_NEEDMOREPARAMS "<command> :Not enough parameters"
		insertSendBuffer(iter->second, buildErrPacket(ERR_NEEDMOREPARAMS, iter->second->getUserName(), "<MODE> :Not enough parameters\r\n"));
		return ;
	}
	if (paramSplitV[1].size() == 1 || paramSplitV[1][0] == '+')
		return ;
	for (unsigned int i = 1 ; i < paramSplitV[1].size() ; ++i)
		if (
			!(paramSplitV[1][i] == 's' || 
			paramSplitV[1][i] == 'n' ||
			paramSplitV[1][i] == 't' ||
			paramSplitV[1][i] == 'o')
		)
		{
			// 472    ERR_UNKNOWNMODE "<char> :is unknown mode char to me for <channel>"
			Message replyMessage(user_info, "472", ":is unknown mode char to me for <channel>");
			insertSendBuffer(iter->second, replyMessage.getReplyMessage());
		}
			
	channelName = paramSplitV[0];
	option = paramSplitV[1];
	STRING nick;
	if (paramSplitV.size() > 1)
		nick = paramSplitV[2];
	// 권한 검색

	std::cout << "target : " << channelName << std::endl;
	std::cout << "option : " << option << std::endl;

    std::map<std::string, Channel*>::iterator chanIter = chann_map_.find(channelName);
    if (chanIter == chann_map_.end())
    {
        std::cout << "not found\n";
		return ;
    }
    else {
        // 오퍼레이터인지 검증이 필요함. 필수
		bool flag = false;
		VECTOR<Client*> operList = chanIter->second->getOpers_();
		for (unsigned int i = 0 ; i < operList.size() ; ++i)
		{
			if (operList[i]->getNickName() == iter->second->getNickName())
			{
				flag = true;
				break;
			}
		}
		if (!flag)
		{
			// 482    ERR_CHANOPRIVSNEEDED "<channel> :You're not channel operator"
			Message replyMsg(":" + iter->second->getNickName(), "482", "<channel> :You're not channel operator");
			insertSendBuffer(iter->second, replyMsg.getReplyMessage());
			return ;
		}
        // 검증 후 전체한테 돌려서 테스트를 해봅시다.
		std::vector<std::string> spl = split(option, ' ');

		
		Message replyMsg(":" + iter->second->getNickName(), RPL_CHANNELMODEIS, channelName, spl[0]);
		std::cout << "replyMSG[" << replyMsg.getReplyMessage() <<"]\n";
		insertSendBuffer(iter->second, replyMsg.getReplyMessage());

		std::vector<Client*> users = chanIter->second->getUsers();
		if (spl.size() > 1)
		{
			bool flag = false;
			for (std::vector<Client*>::iterator userIter = users.begin() ; userIter != users.end() ; ++userIter)
			{
				if ((*userIter)->getNickName() == spl[1])
				{
					flag = true;
					break;
				}
			}
			
			if (!flag)
			{
				// 441    ERR_USERNOTINCHANNEL "<nick> <channel> :They aren't on that channel"
				Message replyMsg(":" + iter->second->getNickName(), "441", "<nick> <channel> :They aren't on that channel");
				insertSendBuffer(iter->second, replyMsg.getReplyMessage());
				return ;
			}

			for (std::vector<Client*>::iterator userIter = users.begin() ; userIter != users.end() ; ++userIter)
			{
				// 324    RPL_CHANNELMODEIS "<channel> <mode> <mode params>"
				std::string sendMsg;
				sendMsg = ":" + iter->second->getNickName() + " MODE " + channelName + " " + spl[0] + " " + spl[1] + "\r\n";
				std::cout <<"send[" << sendMsg << "]\n";
				std::cout << "sendTEST[" << command + " " << param << "]\n";
				insertSendBuffer(*userIter, sendMsg);
			}	
		}
		else
		{
			for (std::vector<Client*>::iterator userIter = users.begin() ; userIter != users.end() ; ++userIter)
			{
				
				std::string sendMsg;
				sendMsg = ":" + iter->second->getNickName() + " MODE " + channelName + " " + spl[0] + "\r\n";
				
				std::cout <<"send[" << sendMsg << "]\n";
				std::cout << "sendTEST[" << command + " " << param << "]\n";
				insertSendBuffer(*userIter, sendMsg);
			}
		}
    }
	return ;
}