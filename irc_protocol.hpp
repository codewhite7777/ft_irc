/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_protocol.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 15:06:49 by alee              #+#    #+#             */
/*   Updated: 2022/08/22 19:38:26 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_PROTOCOL_HPP
# define IRC_PROTOCOL_HPP

// RPL
# define RPL_WELCOME "001"
# define RPL_NONE "300"
# define RPL_AWAY "301"
# define RPL_TOPIC "332"
# define RPL_INVITING "341"
# define RPL_YOUREOPER "381"

// ERR
# define ERR_NOSUCHNICK "401"
# define ERR_NOSUCHCHANNEL "403"
# define ERR_CANNOTSENDTOCHAN "404"
# define ERR_TOOMANYCHANNELS "405"
# define ERR_TOOMANYTARGETS "407"
# define ERR_NORECIPIENT "411"
# define ERR_NOTEXTTOSEND "412"
# define ERR_NOTOPLEVEL "413"
# define ERR_WILDTOPLEVEL "414"
# define ERR_NONICKNAMEGIVEN "431"
# define ERR_ERRONEUSNICKNAME "432"
# define ERR_NICKNAMEINUSE "433"
# define ERR_NICKCOLLISION "436"
# define ERR_NOTONCHANNEL "442"
# define ERR_USERONCHANNEL "443"
# define ERR_NEEDMOREPARAMS "461"
# define ERR_ALREADYREGISTRED "462"
# define ERR_PASSWDMISMATCH "464"
# define ERR_CHANNELISFULL "471"
# define ERR_INVITEONLYCHAN "473"
# define ERR_BANNEDFROMCHAN "474"
# define ERR_BADCHANNELKEY "475"
# define ERR_CHANOPRIVSNEEDED "482"
# define ERR_NOOPERHOST "491"

#endif
