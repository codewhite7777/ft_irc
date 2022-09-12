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

#include "channel.hpp"
#include "Message.hpp"
#include "client.hpp"
#include "server.hpp"
#include <iterator>

Channel::Channel(std::string name) : users_(), opers_(), name_(name) {}

Channel::Channel(void) : users_(), opers_() {}

Channel::~Channel(void) {}
#define MAP std::map
bool	Channel::isUserAlreadyIn(int fd)
{
	MAP<int, Client*>::iterator client_iter = users_.find(fd);
	if (client_iter == users_.end())
		return (false);
	return (true);
}

#define VECTOR std::vector
void	Channel::assignUser(int fd, Client* new_user)
{
	// user 찾기
	if (isUserAlreadyIn(fd))
		return ;
	
	// 유저 정보 삽입
	users_.insert(std::make_pair(fd, new_user));

	//"":nickName!userName@hostName JOIN #channName\r\n"
	std::string	user_info = ":" + new_user->getNickName() \
							+ "!" + new_user->getUserName() \
							+ "@" + new_user->getHostName();
	std::string	proto_join = " JOIN " + name_ + "\r\n";
	std::string proto_to_send;
	proto_to_send = user_info + proto_join;
	
	
	STRING name_list = "";
	// 전체 멤버에게 join 명령어 보내기 + 이름 리스트
	for (MAP<int, Client*>::iterator user_iter = users_.begin()
		; user_iter != users_.end()
		; ++user_iter
	)
	{
		Client * client_ptr = user_iter->second;
		std::cout << "memeber: [" << client_ptr->getNickName() << "]\n";
		client_ptr->getSendBuf().append(proto_to_send);
		
		int find_fd = client_ptr->getSocket();
		if (opers_.find(find_fd) != opers_.end())
			name_list += "@";
		name_list += client_ptr->getNickName() + " ";
	}
	
	// reply찍어보기
	std::string host = ":bar.example.com ";
	//TODO:: host
	// topic이 있을 시에 전달하기.
	// if (topic_.size() > 0)
	// {
	// 	Message topic(host, "332", new_user->getNickName(), name_, topic_);
	// 	std::string re1 =  + " " + new_user->getNickName()+ ' ' +  + " :" + topic_ + "\r\n";
	// 	new_user->getSendBuf().append(re1);
	// 	std::cout << re1 << '\n'; // todo: remove
	// }


	std::string re2 = host + "353 " + new_user->getNickName()+ " = " + name_ + " :";
	re2 += name_list;
	re2 += "\r\n";
	new_user->getSendBuf().append(re2);

	std::string re3 = host + "366 " + new_user->getNickName()+ " " + name_ + " :End of NAMES list\r\n";
	new_user->getSendBuf().append(re3);
	std::cout << re2 << '\n'; // todo: remove
	std::cout << re3 << '\n'; // todo: remove
}

void	Channel::assignOper(int fd, Client* user)
{
	std::string nick = user->getNickName();
	
	opers_[fd] = user;
}

void	Channel::setName(std::string &name)
{
	name_ = name;
}

const std::string&	Channel::getName(void) const
{
	return name_;
}

MAP<int, Client*>& Channel::getUsers_()
{
	return users_;
}


MAP<int, Client*>& Channel::getOpers_()
{
	return opers_;
}

void Channel::eraseUser(int fd)
{
	MAP<int, Client*>::iterator iter = users_.find(fd);
	if (iter != users_.end())
		users_.erase(iter);
}

void Channel::eraseOper(int fd)
{
	MAP<int, Client*>::iterator iter = opers_.find(fd);
	if (iter != opers_.end())
		opers_.erase(iter);
}
