/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 18:32:23 by alee              #+#    #+#             */
/*   Updated: 2022/09/26 18:18:55 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <string>
# include <map>

class Channel
{
public:
	Channel(std::string name);	// todo: use reference?
	~Channel(void);
	// todo: OCCF

	
	std::size_t			getCurrUserCount(void) const;
	void				assignAsUser(Client* clnt);
	void				assignAsOperator(Client* clnt);

	const std::string&	getName(void) const;

	std::string			getUserListStr(void);

	bool				isOperator(Client* clnt);

	void				sendToAll(std::string msg);

private:
	void				eraseUser(std::string);
	void				eraseOper(std::string);

	bool				isAlreadyIn(Client* clnt); // todo: remove

	Channel(void);

	std::string						name_;
	std::map<std::string, Client*>	users_;
	std::map<std::string, Client*>	opers_;
	std::size_t						curr_user_count_;
};

#endif
