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
	for (std::vector<Client*>::iterator user_it = users_.begin(); user_it != users_.end(); ++user_it)
	{
		(*user_it)->getSendBuf().append(proto_to_send);
	}
	#ifdef DEBUG
	std::cout << "Channel: send JOIN protocol to users in the channel\n"; // test
	#endif
}

void	Channel::setName(std::string &name)
{
	name_ = name;
}
