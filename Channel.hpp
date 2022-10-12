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

# include "Client.hpp"
# include <string>
# include <map>
# include <list>

class Channel
{
public:
	Channel(std::string name);	// todo: use reference?
	~Channel(void);
	// todo: OCCF
	
	std::size_t			getCurrUserCount(void) const;
	void				assignAsUser(Client* clnt);
	void				assignAsOperator(Client* clnt);

	// todo: eraseClntInChannel(Client* clnt);
	/*
		-> erase client as user in channel, 
			if channel operator erase client as operator
	*/

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


private:
	Channel(void);

	std::string						name_;
	std::map<std::string, Client*>	users_;
	std::map<std::string, Client*>	opers_;
	std::size_t						curr_user_count_;
};

#endif
