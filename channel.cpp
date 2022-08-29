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

Channel::Channel(void) : users_(), operators_() {}

Channel::~Channel(void) {}

void	Channel::assignUser(Client* new_user) {
	// todo: if the new_user already is in this channel, don't assign
	// is already in?
	for (std::vector<Client*>::iterator user_itr = users_.begin(); user_itr != users_.end(); ++user_itr)
	{
		std::cout << "user nick in vec: [" << (*user_itr)->getNickName() << "]\n";
		std::cout << "user nick of new: [" << (*user_itr)->getNickName() << "]\n";
		// 지금은 nickName을 비교해서 확인하는데, 최적화를 고려한다면 클라이언트 구조체 주소를 비교하거나 SOCKET을 비교해도 될 듯?
		if ((*user_itr)->getNickName() == new_user->getNickName())
		{
			return ;
		}
	}
	std::cout << "assign new client user in channel\n"; // test
	users_.push_back(new_user);
	new_user->getSendBuf().append(":" + new_user->getNickName() + " JOIN " + name_ + "\r\n");
}

void	Channel::setName(std::string &name)
{
	name_ = name;
}
