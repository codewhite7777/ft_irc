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

Channel::Channel(std::string name) : users_(), opers_(), name_(name) {}

Channel::Channel(void) : users_(), opers_() {}

Channel::~Channel(void) {}

void	Channel::assignUser(Client* new_user)
{
	for (std::vector<Client*>::iterator user_it = users_.begin(); user_it != users_.end(); ++user_it)
	{
		if ((*user_it)->getNickName() == new_user->getNickName())
			return ;
	}
	#ifdef DEBUG
	std::cout << "assign new client user in channel\n"; // test
	#endif
	users_.push_back(new_user);
	//"":nickName!userName@hostName JOIN #channName\r\n"
	std::string	user_info = ":" + new_user->getNickName() \
							+ "!" + new_user->getUserName() \
							+ "@" + new_user->getHostName();
	std::string	proto_join = " JOIN " + name_ + "\r\n";


	std::string proto_to_send;
	proto_to_send = user_info + proto_join;
	std::cout << "send msg [" << proto_to_send << "]\n";
	std::cout << "memeber size: [" << users_.size() << "]\n";
	for (std::vector<Client*>::iterator user_it = users_.begin(); user_it != users_.end(); ++user_it)
	{
		std::cout << "memeber: [" << (*user_it)->getNickName() << "]\n";
		(*user_it)->getSendBuf().append(proto_to_send);
	}
	std::string host = ":bar.example.com ";
	// std::string re1 = host + "332 " + new_user->getNickName()+ ' ' + name_ + " :\r\n";
	// new_user->getSendBuf().append(re1);
	std::string re2 = host + "353 " + new_user->getNickName()+ " = " + name_ + " :";
	for (std::vector<Client*>::iterator user_it = users_.begin(); user_it != users_.end(); ++user_it)
	{
		re2 += (*user_it)->getNickName() + " ";
	}
	re2 += "\r\n";
	new_user->getSendBuf().append(re2);
	std::string re3 = host + "366 " + new_user->getNickName()+ " " + name_ + " :End of NAMES list\r\n";
	new_user->getSendBuf().append(re3);
	// std::cout << re1 << '\n';
	std::cout << re2 << '\n';
	std::cout << re3 << '\n';
	#ifdef DEBUG
	std::cout << "Channel: send JOIN protocol to users in the channel\n"; // test
	#endif
	
}

void	Channel::setName(std::string &name)
{
	name_ = name;
}

std::vector<Client*>& Channel::getUsers()
{
	return users_;
}

std::vector<Client*>& Channel::getOpers_()
{
	return opers_;
}
void Channel::eraseUser(int index)
{
	users_.erase(users_.begin() + index);
}

void Channel::eraseOper(int sd)
{
	for (unsigned int i = 0 ; i < opers_.size() ; ++i)
		if (sd == opers_[i]->getSocket())
		{
			opers_.erase(opers_.begin() + i);
			break;
		}
}