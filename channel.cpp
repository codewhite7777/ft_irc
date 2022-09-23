/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 18:32:38 by alee              #+#    #+#             */
/*   Updated: 2022/09/12 17:22:21 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iterator>

Channel::Channel(std::string name) : users_(), opers_(), name_(name) {}

Channel::Channel(void) : users_(), opers_() {}

Channel::~Channel(void) {}
#define MAP std::map
bool	Channel::isUserAlreadyIn(STRING nick)
{
	MAP<STRING, Client*>::iterator client_iter = users_.find(nick);
	if (client_iter == users_.end())
		return (false);
	return (true);
}

#define VECTOR std::vector
void	Channel::assignUser(STRING nick, Client* new_user)
{
	// user 찾기
	if (isUserAlreadyIn(nick))
	{
		std::cout << "The user already in this channel\n";
		return ;
	}
	
	// 유저 정보 삽입
	users_.insert(std::make_pair(nick, new_user));

	//"":nickName!userName@hostName JOIN #channName\r\n"
	std::string	user_info = new_user->getNickName() \
							+ "!" + new_user->getUserName() \
							+ "@" + new_user->getHostName();
	std::string	proto_join = " JOIN " + name_ + "\r\n";
	std::string proto_to_send;
	proto_to_send = user_info + proto_join;
	
	// 전체 멤버에게 join 명령어 보내기 + 이름 리스트
	STRING name_list = "";
	for (MAP<STRING, Client*>::iterator user_iter = users_.begin()
		; user_iter != users_.end()
		; ++user_iter
	)
	{
		Client * client_ptr = user_iter->second;
		std::cout << "memeber: [" << client_ptr->getNickName() << "] <- ";
		std::cout << proto_to_send;
		client_ptr->getSendBuf().append(proto_to_send);
		
		STRING find_nick = client_ptr->getNickName();
		if (opers_.find(find_nick) != opers_.end())
			name_list += "@";
		name_list += client_ptr->getNickName() + " ";
		//name_list += client_ptr->getNickName() + "!" + client_ptr->getUserName() + "@" + client_ptr->getHostName() + " ";
	}
	
	// reply찍어보기
	std::string host = ":bar.example.com ";
	//TODO:: host
	//topic이 있을 시에 전달하기.
	if (topic_.size() > 0)
	{
		Message topic(host, "332", new_user->getNickName(), name_, topic_);
		std::string re1 = host + "332 " + new_user->getNickName() + " " + name_ + ": " + topic_ + "\r\n";
		new_user->getSendBuf().append(re1);
		std::cout << re1 << '\n'; // todo: remove
	}

	std::string re2 = host + "353 " + new_user->getNickName() + " = " + name_ + " :";
	//std::string re2 = host + "353 " + user_info + " = " + name_ + " :";
	re2 += name_list;
	re2 += "\r\n";
	new_user->getSendBuf().append(re2);

	std::string re3 = host + "366 " + new_user->getNickName() + " " + name_ + " :End of /NAMES list.\r\n";
	//std::string re3 = host + "366 " + user_info + " " + name_ + " :End of /NAMES list.\r\n";
	new_user->getSendBuf().append(re3);
	std::cout << re2 << '\n'; // todo: remove
	std::cout << re3 << '\n'; // todo: remove
}

void	Channel::assignOper(STRING nick, Client* user)
{
	// std::string nick = user->getNickName();
	
	opers_[nick] = user;
}

void	Channel::setName(std::string &name)
{
	name_ = name;
}

const std::string&	Channel::getName(void) const
{
	return name_;
}

MAP<STRING, Client*>& Channel::getUsers_()
{
	return users_;
}


MAP<STRING, Client*>& Channel::getOpers_()
{
	return opers_;
}

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
