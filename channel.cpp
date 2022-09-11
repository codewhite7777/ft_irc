/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 18:32:38 by alee              #+#    #+#             */
/*   Updated: 2022/08/28 18:32:39 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"
#include "mode.hpp"
#include <iterator>

Channel::Channel(std::string name) : users_(), opers_(), name_(name),mode(CHANNEL_MODE) {}

Channel::Channel(void) : users_(), opers_(), mode(CHANNEL_MODE) {}

Channel::~Channel(void) {}

bool	Channel::isUserAlreadyIn(Client* user)
{
	for (std::vector<Client*>::iterator user_it = users_.begin()
		; user_it != users_.end()
		; ++user_it)
	{
		if ((*user_it)->getNickName() == user->getNickName())
			return true;
	}
	return false;
}

void	Channel::assignUser(Client* new_user)
{
	#ifdef DEBUG
	std::cout << "assign new client user in channel\n"; // test
	#endif
	if (isUserAlreadyIn(new_user))
		return ;
	users_.push_back(new_user);
	new_user->addChannel(this); // in client session
	//"":nickName!userName@hostName JOIN #channName\r\n"
	std::string	user_info = ":" + new_user->getNickName() \
							+ "!" + new_user->getUserName() \
							+ "@" + new_user->getHostName();
	std::string	proto_join = " JOIN " + name_ + "\r\n";
	std::string proto_to_send;
	proto_to_send = user_info + proto_join;
	/*
	std::cout << "send msg [" << proto_to_send << "]\n";
	std::cout << "memeber size: [" << users_.size() << "]\n";
	*/
	for (std::vector<Client*>::iterator user_it = users_.begin()
		; user_it != users_.end()
		; ++user_it)
	{
		std::cout << "memeber: [" << (*user_it)->getNickName() << "]\n";
		(*user_it)->getSendBuf().append(proto_to_send);
	}
	/*
	new_user->getSendBuf().append(tmp1);
	new_user->getSendBuf().append(tmp2);
	#ifdef DEBUG
	std::cout << "Channel: send JOIN protocol to users in the channel\n"; // test
	#endif
	*/
	
	std::string host = ":bar.example.com ";
	std::cout << "tq:" << topic_.size() << '\n';
	if (topic_.size() > 0)
	{
		std::string re1 = host + "332 " + new_user->getNickName()+ ' ' + name_ + " :" + topic_ + "\r\n";
		new_user->getSendBuf().append(re1);
		std::cout << re1 << '\n'; // todo: remove
	}
	std::string re2 = host + "353 " + new_user->getNickName()+ " = " + name_ + " :";
	for (std::vector<Client*>::iterator user_it = users_.begin(); user_it != users_.end(); ++user_it)
	{
		if (opers_.find(((*user_it)->getNickName())) != opers_.end())
			re2 += "@";
		re2 += (*user_it)->getNickName() + " ";
	}
	re2 += "\r\n";
	new_user->getSendBuf().append(re2);
	std::string re3 = host + "366 " + new_user->getNickName()+ " " + name_ + " :End of NAMES list\r\n";
	new_user->getSendBuf().append(re3);
	std::cout << re2 << '\n'; // todo: remove
	std::cout << re3 << '\n'; // todo: remove
	#ifdef DEBUG
	std::cout << "Channel: send JOIN protocol to users in the channel\n"; // test
	#endif
}

void	Channel::assignOper(Client* user)
{
	std::string nick = user->getNickName();
	//std::map<std::string, Client*>::iterator iter = opers_.find(nick);
	opers_[nick] = user;

	/*
	if (iter ==  opers_.end())
	{

	}
	else {
		// opers_.insert({nick, user});
		opers_.insert(make_pair(nick, user));
	}
	*/

}

void	Channel::setName(std::string &name)
{
	name_ = name;
}

const std::string&	Channel::getName(void) const
{
	return name_;
}

std::vector<Client*>& Channel::getUsers()
{
	return users_;
}

void Channel::eraseUser(int index)
{
	users_.erase(users_.begin() + index);
}
void Channel::eraseOper(const std::string nick)
{
	opers_.erase(nick);
	// std::map<std::string, Client*>::iterator iter;

}
