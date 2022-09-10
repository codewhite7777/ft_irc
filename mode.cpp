#include "server.hpp"
#include "mode.hpp"
#include "client.hpp"
#include "irc_protocol.hpp"
void		Server::requestMode(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	std::string	cp_param;
	std::string	tar_nick;
	std::string	option;
	size_t		pos;

	cp_param = param;
	pos = cp_param.find(' ');
	if (pos == std::string::npos)
	{
		std::cout << "tqtqtqtqtqtq\n";
		insertSendBuffer(iter->second, buildErrPacket(ERR_NEEDMOREPARAMS, iter->second->getUserName(), ":info) Not enough parameter\r\n"));
		return ;
	}
	tar_nick = param.substr(0, pos);
	option = cp_param.erase(0, pos + 1);

	std::cout << "target : " << tar_nick << std::endl;
	std::cout << "option : " << option << std::endl;
    //324     RPL_CHANNELMODEIS "<channel> <mode> <mode params>"
    
    std::map<std::string, Channel*>::iterator chanIter = chann_map_.find(tar_nick);

// 	Command: MODE
//    Parameters: <nickname>
//                *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )

    if (chanIter == chann_map_.end())
    {
        std::cout << "not found\n";
    }
    else {
        // 오퍼레이터인지 검증이 필요함. 필수
		

		
        // 검증 후 전체한테 돌려서 테스트를 해봅시다.
		std::string msg = ":" + iter->second->getNickName() + " 324 ";
        std::vector<Client*> users = chanIter->second->getUsers();
        std::cout << "msg test[" << msg << "\n";
		std::vector<std::string> spl = split(option, ' ');
		msg += tar_nick + " ";
			
		msg += spl[0] + "\r\n";
		
		std::cout << msg <<'\n';
		
        for (std::vector<Client*>::iterator userIter = users.begin() ; userIter != users.end() ; ++userIter)
        {
			
			std::string	user_info = (*userIter)->getNickName();
							// + "!" + (*userIter)->getUserName() \
							// + "@" + (*userIter)->getHostName();

			std::string sendMsg = ":" + user_info + " MODE " + tar_nick + " " + spl[0] + "\r\n";
			std::cout <<"send[" << sendMsg << "]\n";
			std::cout << "sendTEST[" << command + " " << param << "]\n";
            insertSendBuffer(*userIter, sendMsg);
        }
    }
    


	//OPER <Name> <Pass>
	(void)command;
	(void)iter;
	(void)param;
	return ;
}