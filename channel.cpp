/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 18:32:38 by alee              #+#    #+#             */
/*   Updated: 2022/09/26 18:16:02 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <string>
#include <map>

Channel::Channel(std::string name)
	: name_(name)
	, curr_user_count_(0) {}

Channel::~Channel(void) {}

bool	Channel::isAlreadyIn(Client* clnt)
{
	std::map<std::string, Client*>::iterator clnt_it;
	
	clnt_it = users_.find(clnt->getNickname());
	if (clnt_it == users_.end())
		return (false);
	return (true);
}

std::size_t		Channel::getCurrUserCount() const
{
	return curr_user_count_;
}

void	Channel::assignAsUser(Client* clnt)
{
	users_.insert(std::make_pair(clnt->getNickname(), clnt));
	++curr_user_count_;
}

void	Channel::assignAsOperator(Client* clnt)
{
	opers_.insert(std::make_pair(clnt->getNickname(), clnt));
}

const std::string&	Channel::getName(void) const
{
	return name_;
}

std::string		Channel::getUserListStr(void)
{
	std::string	ret("");

	for (std::map<std::string, Client*>::iterator user_it = users_.begin()
		; user_it != users_.end()
		; ++user_it)
	{
		if (isOperator(user_it->second))
			ret += "@";
		ret += user_it->second->getNickname() + " ";
	}
	return ret;
}

bool			Channel::isOperator(Client* clnt)
{
	for (std::map<std::string, Client*>::iterator oper_it = opers_.begin()
		; oper_it != opers_.end()
		; ++oper_it)
	{
		if (oper_it->second == clnt)
		{
			return true;
		}
	}
	return false;
}

void		Channel::sendToAll(std::string msg)
{
	for (std::map<std::string, Client*>::iterator user_it = users_.begin()
		; user_it != users_.end()
		; ++user_it)
	{
		user_it->second->appendToSendBuf(msg);
	}
}

/*
void Channel::eraseUser(STRING nick)
{
	MAP<STRING, Client*>::iterator iter = users_.find(nick);
	if (iter != users_.end())
		users_.erase(iter);
}

void Channel::eraseOper(STRING nick)
{
	MAP<STRING, Client*>::iterator iter = opers_.find(nick);
	if (iter != opers_.end())
		opers_.erase(iter);
}
*/
