#include <map>
#include <iostream>
#include <string>
#include "Chatbot.hpp"
#include "Channel.hpp"
#include <time.h>

ChatBot::ChatBot()
{
	// 넣고 싶은거 알아서 넣음 됨
	chatbot_command_list.insert(std::make_pair("!hi", "안녕하세요1"));
	chatbot_command_list.insert(std::make_pair("!hello", "안녕하세요2"));

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

	chatbot_command_list.insert(std::make_pair("!now", now));
}

bool	ChatBot::CheckChatBotCommand(std::string param, std::string &ret)
{
	ret = "";
    std::map<std::string, std::string>::iterator 
    iter = chatbot_command_list.find(param.substr(1));
	if (iter != chatbot_command_list.end())
	{
		ret = chatbot_command_list[param.substr(1)];
		return (true);
	}
	return (false);
}

void	BroadCast(std::string server_name)
{
	server_name = "";
}

ChatBot::~ChatBot()
{
	chatbot_command_list.clear();
}
