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

void	Channel::assignUser(Client* new_user) {
	// find the user already in
	for (std::vector<Client*>::iterator user_itr = users_.begin(); user_itr != users_.end(); ++user_itr)
	{
		// consider: 지금은 nickName을 비교해서 확인하는데, 최적화를 고려한다면 클라이언트 구조체 주소를 비교하거나 SOCKET을 비교해도 될 듯?
		if ((*user_itr)->getNickName() == new_user->getNickName())
			return ;
	}
	std::cout << "assign new client user in channel\n"; // test
	users_.push_back(new_user);

	// send JOIN protocol to client
	//new_user->getSendBuf().append(":" + new_user->getNickName() + " JOIN " + name_ + "\r\n");

	//"":nickName!userName@hostName JOIN #channName\r\n"
	std::string	user_info = ":" + new_user->getNickName() \
							+ "!" + new_user->getUserName() \
							+ "@" + new_user->getHostName();
	std::string	proto_join = " JOIN " + name_ + "\r\n";
	std::string proto_to_send;

	proto_to_send = user_info + proto_join;
	new_user->getSendBuf().append(proto_to_send);
	std::cout << "Channel: send JOIN protocol to client\n"; // 보내니까 클라에 has joined 뜸

	// todo: set and use server hostname
	// what serv_hostname_?
	// 임시로 server hostname을 127.0.0.1 로 하드코딩함
	std::string	tmp_servhostname = "127.0.0.1";

	// ":127.0.0.1 332 :nick!user@host #channName :TEST1\r\n"
	proto_to_send = ":" + tmp_servhostname + " 332 " + user_info \
					+ " " + name_ + " :TEST1 \r\n";
	new_user->getSendBuf().append(proto_to_send);
	std::cout << "Channel: send 332 protocol to client\n"; // 클라에 TOPIC: 라고 뜸

	// ":127.0.0.1 353 :nick!user@host #channName :@host\r\n"
	proto_to_send = ":" + tmp_servhostname + " 353 " + user_info \
					+ " = " + name_ + " :@" + new_user->getHostName() + " \r\n";
	new_user->getSendBuf().append(proto_to_send);
	std::cout << "Channel: send 353 protocol to client\n"; // 클라에 Names: 라고 뜸

	// ":127.0.0.1 366 :nick!user@host #channName :End of Names list\r\n"
	proto_to_send = ":" + tmp_servhostname + " 366 " + user_info \
					+ " " + name_ + " :End of Names list \r\n";
	new_user->getSendBuf().append(proto_to_send);
	std::cout << "Channel: send 366 protocol to client\n"; // 암것도 안뜨네..?

	/*
	std::string test = ":hena!hena@" + iter->second->getHostName();
	std::string msg = "";
	msg = test + " JOIN #asdf \r\n";
	insertSendBuffer(iter->second, msg);
	sendPacket(iter);

	msg = ":" + serv_hostname_ +" 332 "+test+" #asdf :TEST1 \r\n"; 
	// ":127.0.0.1 332 :nick!user@host #channName :TEST1\r\n"
	insertSendBuffer(iter->second, msg);
	sendPacket(iter);

	msg = ":" + serv_hostname_ +" 353 "+test+" = #asdf :@hena \r\n";
	// ":127.0.0.1 353 :nick!user@host #channName :@host\r\n"
	insertSendBuffer(iter->second, msg);
	sendPacket(iter);

	msg = ":" + serv_hostname_ +" 366 "+test+" #asdf :End of Names list \r\n";
	// ":127.0.0.1 366 :nick!user@host #channName :End of Names list\r\n"
	insertSendBuffer(iter->second, msg);
	sendPacket(iter);

	return ;
	*/
}

void	Channel::setName(std::string &name)
{
	name_ = name;
}
