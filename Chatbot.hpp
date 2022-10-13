/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chatbot.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 13:26:15 by hena              #+#    #+#             */
/*   Updated: 2022/10/13 13:27:21 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef _CHATBOT_HPP_
# define _CHATBOT_HPP_

# include <string>
# include <map>

class ChatBot
{
public:
	ChatBot(void);
	~ChatBot(void);

	bool	CheckList(std::string param, std::string &ret);
	bool	ChatBotCommand(std::string&, std::string&);
	std::string		getPrefix(void);

private:
	std::string		CommandList(void);
	void			AddCommand(std::string&);
	void			DeleteCommand(std::string &);
	std::string		GetNowTime(void);
	
private:
	std::map< std::string, std::string > chatbot_command_list;
};

#endif
