/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 11:19:31 by mgo               #+#    #+#             */
/*   Updated: 2022/10/12 11:19:32 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <list>

#include "Chatbot.hpp"

class Client;

class Channel
{
public:
	Channel(std::string name);
	~Channel(void);

	std::size_t			getCurrUserCount(void) const;
	void				assignAsUser(Client* clnt);
	void				assignAsOperator(Client* clnt);
	void				eraseAsUser(Client* clnt);
	void				eraseAsOperator(Client* clnt);
	const std::string&	getName(void) const;
	std::string			getUserListStr(void);
	bool				isOperator(Client* clnt);
	void				sendToAll(std::string msg);
	void				sendToOthers(Client* clnt, std::string msg);
	bool				isUserIn(Client* clnt);
	std::list<Client*>*	makeClntListInChannExceptOne(Client* clnt_to_excpt);
	void				eraseClntIfIs(Client* clnt);
	void				replaceClntKeyNick(Client* clnt, \
											std::string nick_to_key);

	// chatbot
	ChatBot&			GetChatBot();
  
private:
	Channel(void);

	std::string						name_;
	std::map<std::string, Client*>	users_;
	std::map<std::string, Client*>	opers_;
	std::size_t						curr_user_count_;
	
	// chatbot
	ChatBot							chatbot;
};

#endif
