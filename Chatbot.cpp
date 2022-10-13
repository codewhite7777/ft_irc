/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chatbot.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 13:26:15 by hena              #+#    #+#             */
/*   Updated: 2022/10/13 13:29:44 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chatbot.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <map>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

std::string		ChatBot::CommandList(void)
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
	std::vector<std::string> v = split(contents, ',');
	if (v[0][0] == '!' && v[0] != "!add" && v[0] != "!delete" \
		&& v[0] != "!list" && v[0] != "!info")
	{
		chatbot_command_list[v[0]] = v[1];
	}
}

void			ChatBot::DeleteCommand(std::string &command)
{
	if (command[0] == '!' && command != "!add" && command != "!delete" \
		&& command != "!list" && command != "!info")
		chatbot_command_list.erase(command);
}

std::string		ChatBot::GetNowTime()
{
	std::string	now = "";
	time_t timer;

	timer = time(NULL);
	now += ctime(&timer);
	return now;
}

ChatBot::ChatBot(void)
{
	// !hi 
	chatbot_command_list.insert(std::make_pair("!hi", "안녕하세요"));

	// !now
	chatbot_command_list.insert(std::make_pair("!now", GetNowTime()));

	// !mgo
	chatbot_command_list.insert(std::make_pair("!mgo", "나는 항상 배가 고프다"));

	// !hena
	chatbot_command_list.insert(std::make_pair("!hena", "뀨 저는 귀요미에욤"));

	// !info
	chatbot_command_list.insert(std::make_pair("!info", "기본 봇 명령어: !add, !delete, !list"));
}

bool	ChatBot::CheckList(std::string param, std::string &ret)
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
		CheckList(chat_command, bot_msg);
		return true;
	}
	return false;
}

ChatBot::~ChatBot(void)
{
	chatbot_command_list.clear();
}

std::string		ChatBot::getPrefix(void)
{
	return (":NIGHTBOT!night@localhost ");
}
