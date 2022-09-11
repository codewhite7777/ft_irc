/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 18:32:23 by alee              #+#    #+#             */
/*   Updated: 2022/08/28 18:37:58 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <vector>
# include "client.hpp"
# include "ModeClass.hpp"
# include "map"

class Channel
{
private:
	std::vector<Client*>	users_;
	std::map<std::string, Client*>	opers_;

	std::string				topic_;
	std::string				name_; // tmp... considering to remove
	// Mode state;
public:
	Channel(std::string name); // todo: use reference?
	Channel(void);
	~Channel(void);
	// todo: OCCF

	void	assignUser(Client* new_user);
	void	assignOper(Client* user);

	bool	isUserAlreadyIn(Client* user);

	

	void				setName(std::string &name);
	const std::string&	getName(void) const;


	std::vector<Client*>& getUsers();
	std::vector<Client*>& getOpers_();

	void eraseUser(int index);
	void eraseUser(const std::string& nick);	//void	removeUser(Client* user);
	void eraseOper(const std::string& nick);	//void	removeOper(Client* oper); // todo
};

#endif
