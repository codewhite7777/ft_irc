/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_protocol.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 11:20:04 by mgo               #+#    #+#             */
/*   Updated: 2022/10/12 11:20:05 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_PROTOCOL_HPP
# define IRC_PROTOCOL_HPP

# define RPL_WELCOME	        "001"
# define RPL_YOURHOST	        "002"
# define RPL_CREATED	        "003"
# define RPL_MYINFO             "004"
# define RPL_NONE		        "300"
# define RPL_AWAY		        "301"
# define RPL_TOPIC		        "332"
# define RPL_INVITING	        "341"
# define RPL_NAMREPLY           "353"
# define RPL_ENDOFNAMES         "366"
# define RPL_YOUREOPER	        "381"

# define ERR_NOSUCHNICK			"401"
# define ERR_NOSUCHCHANNEL		"403"
# define ERR_CANNOTSENDTOCHAN	"404"
# define ERR_TOOMANYCHANNELS	"405"
# define ERR_TOOMANYTARGETS		"407"
# define ERR_NORECIPIENT		"411"
# define ERR_NOTEXTTOSEND		"412"
# define ERR_NOTOPLEVEL			"413"
# define ERR_WILDTOPLEVEL		"414"
# define ERR_UNKNOWNCOMMAND		"421"
# define ERR_NONICKNAMEGIVEN	"431"
# define ERR_ERRONEUSNICKNAME	"432"
# define ERR_NICKNAMEINUSE		"433"
# define ERR_NICKCOLLISION		"436"
# define ERR_USERNOTINCHANNEL	"441"
# define ERR_NOTONCHANNEL		"442"
# define ERR_USERONCHANNEL		"443"
# define ERR_NEEDMOREPARAMS		"461"
# define ERR_ALREADYREGISTRED	"462"
# define ERR_PASSWDMISMATCH		"464"
# define ERR_CHANNELISFULL		"471"
# define ERR_INVITEONLYCHAN		"473"
# define ERR_BANNEDFROMCHAN		"474"
# define ERR_BADCHANNELKEY		"475"
# define ERR_BADCHANMASK        "476"
# define ERR_NOPRIVILEGES		"481"
# define ERR_CHANOPRIVSNEEDED	"482"
# define ERR_NOOPERHOST			"491"

#endif
