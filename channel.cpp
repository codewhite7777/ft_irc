/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 18:32:38 by alee              #+#    #+#             */
/*   Updated: 2022/09/26 18:04:24 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <string>
#include <map>
//#include <iterator>

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
							+ "@" + new_user->getName();
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
		//name_list += client_ptr->getNickName() + "!" + client_ptr->getUserName() + "@" + client_ptr->getName() + " ";
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

std::map<std::string, Client*>& Channel::getUsers_()
{
	return users_;
}

std::map<std::string, Client*>& Channel::getOpers_()
{
	return opers_;
}
