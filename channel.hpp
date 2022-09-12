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
# include "client.hpp"
# include "ModeClass.hpp"
# include "map"

#define MAP std::map
class Channel
{
private:
	std::map<int , Client*>	users_;
	std::map<int , Client*>	opers_;

	std::string				topic_;
	std::string				name_; // tmp... considering to remove
	// Mode state;
public:
	Channel(std::string name); // todo: use reference?
	Channel(void);
	~Channel(void);
	// todo: OCCF

	void	assignUser(int fd, Client* new_user);
	void	assignOper(int fd, Client* user);

	bool	isUserAlreadyIn(int);



	void				setName(std::string &name);
	const std::string&	getName(void) const;

	MAP<int, Client*>&	getUsers_();
	MAP<int, Client*>&	getOpers_();

	void eraseUser(int fd);
	void eraseOper(int fd);
	// void eraseUser(std::string& nick);	//void	removeUser(Client* user);
	
};

#endif
