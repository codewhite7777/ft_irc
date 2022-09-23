/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 18:32:23 by alee              #+#    #+#             */
/*   Updated: 2022/09/12 16:45:00 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <vector>
# include "Client.hpp"
# include "ModeClass.hpp"
# include "map"

#define MAP std::map
class Channel
{
private:
	std::map<STRING , Client*>	users_;
	std::map<STRING , Client*>	opers_;

	std::string				topic_;
	std::string				name_; // tmp... considering to remove
	// Mode state;
public:
	Channel(std::string name); // todo: use reference?
	Channel(void);
	~Channel(void);
	// todo: OCCF

	void	assignUser(STRING , Client* new_user);
	void	assignOper(STRING , Client* user);

	bool	isUserAlreadyIn(STRING);



	void				setName(std::string &name);
	const std::string&	getName(void) const;

	MAP<STRING, Client*>&	getUsers_();
	MAP<STRING, Client*>&	getOpers_();

	void eraseUser(STRING);
	void eraseOper(STRING);
	// void eraseUser(std::string& nick);	//void	removeUser(Client* user);
	
};

#endif
