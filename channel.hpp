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

class Channel
{
private:
	std::vector<Client*>	users_;
	std::vector<Client*>	opers_;

	std::string				name_; // tmp... considering to remove
	
	//hena
	unsigned short			channelMode;

public:
	Channel(std::string name); // todo: use reference?
	Channel(void);
	~Channel(void);
	// todo: OCCF

	void	assignUser(Client* new_user);
	void	assignOper(Client* user);
	void	setName(std::string &name);


	std::vector<Client*> getUsers();
	std::vector<Client*> getOpers_();
	void eraseUser(int index);
	void eraseOper(int sd);
};

#endif
