/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 18:32:23 by alee              #+#    #+#             */
/*   Updated: 2022/09/26 15:27:09 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <map>

class Channel
{
public:
	Channel(std::string name); // todo: use reference?
	Channel(void);
	~Channel(void);
	// todo: OCCF

	//chann_ptr->getCurrUserCount()
	//chann_ptr->assignOperator()
	//chann_ptr->assignUser()

	void	assignAsUser(Client* clnt);
	void	assignAsOperator(Client* clnt);

	void	assignUser(std::string, Client* new_user);
	void	assignOper(std::string, Client* user);

	bool	isUserAlreadyIn(std::string);

	void				setName(std::string &name);
	const std::string&	getName(void) const;

	std::map<std::string, Client*>&	getUsers_();
	std::map<std::string, Client*>&	getOpers_();

	void eraseUser(std::string);
	void eraseOper(std::string);
	// void eraseUser(std::string& nick);	//void	removeUser(Client* user);

private:
	std::map<std::string, Client*>	users_;
	std::map<std::string, Client*>	opers_;
	std::string						name_;

};

#endif
