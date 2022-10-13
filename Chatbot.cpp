#include <map>
#include <iostream>
#include <string>
#include "Chatbot.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include <time.h>
#include <vector>
#include "utils.hpp"
using namespace std;
std::string		ChatBot::CommandList()
{
	std::string list = "";
	for (std::map< std::string, std::string>::iterator iter = chatbot_command_list.begin()
		; iter != chatbot_command_list.end()
		; ++iter)
	{
		list += iter->first + ' ';
	}
	return list;
}

void			ChatBot::AddCommand(std::string &contents)
{
	std::cout << "IN ADDCOMMAND\n";
	std::vector<std::string> v = split(contents, ',');
	std::cout <<"["<< v[0] <<"], [" << v[1] << "]\n";
	if (v[0][0] == '!' && v[0] != "!add" && v[0] != "!delete" && v[0] != "!list")
	{
		std::cout << "size:[" << chatbot_command_list.size()<< "]\n";
		chatbot_command_list[v[0]] = v[1];
		std::cout << "size:[" << chatbot_command_list.size()<< "]\n";
	}
}

void			ChatBot::DeleteCommand(std::string &command)
{
	if (command[0] == '!' && command != "!add" && command != "!delete" && command != "!list")
		chatbot_command_list.erase(command);
}

std::string		ChatBot::GetNowTime()
{
	std::string	now = "";
	time_t timer;
	struct tm* t;

	timer = time(NULL);
	t = localtime(&timer);

	now += std::to_string(t->tm_year + 1900) + "/";
	now += std::to_string(t->tm_mon + 1) + "/";
	now += std::to_string(t->tm_mday) + "/";
	now += std::to_string(t->tm_hour) + ":";
	now += std::to_string(t->tm_min)+ ":";
	now += std::to_string(t->tm_sec);

	return now;
}

ChatBot::ChatBot()
{
	// !hi 
	chatbot_command_list.insert(std::make_pair("!hi", "안녕하세요"));

	// !now
	chatbot_command_list.insert(std::make_pair("!now", GetNowTime()));

	// !hi 
	chatbot_command_list.insert(std::make_pair("!mgo", "mgo는 코딩 버스기사입니다."));

	// !hi 
	chatbot_command_list.insert(std::make_pair("!hena", "미래의 청소부입니다."));
}

bool	ChatBot::CheckChatBotCommand(std::string param, std::string &ret)
{
    std::map<std::string, std::string>::iterator 
    iter = chatbot_command_list.find(param);
	if (iter != chatbot_command_list.end())
	{
		ret = chatbot_command_list[param];
		return (true);
	}
	return (false);
}

bool	ChatBot::ChatBotCommand(std::string& param, std::string &bot_msg)
{
	std::cout << "CHAT BOT TEST START\n";
	unsigned int ret = param.find(' ');
	if ((int)ret != -1)
	{
		std::string chat_command = param.substr(1, ret - 1);
		std::string chat_param = param.substr(ret + 1);
		
		if (chat_command == "!add")
			AddCommand(chat_param);
		else if (chat_command == "!delete")
			DeleteCommand(chat_param);
		return true;
	}
	else
	{
		std::string chat_command = param.substr(1);
		if (chat_command == "!list")
			bot_msg = CommandList();
		else if (chat_command == "!now")
			chatbot_command_list[chat_command] =  GetNowTime();
		CheckChatBotCommand(chat_command, bot_msg);
		return true;
	}
	return false;
}

ChatBot::~ChatBot()
{
	chatbot_command_list.clear();
}

std::string		ChatBot::getPrefix()
{
	return ":NIGHTBOT!night@localhost ";
}