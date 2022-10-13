/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 12:32:11 by mgo               #+#    #+#             */
/*   Updated: 2022/09/08 12:32:12 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>

typedef enum e_port
{
	WELL_KNOWN_PORT,
	REGISTERED_PORT,
	DYNAMIC_PORT,
	INVALID_PORT
}			t_port;

bool						isNotAlnumAndUnderscore(const std::string& str);
bool						isOnlyNums(std::string str);
bool						isSpecialCharactor(const std::string& str);

bool						isValidPort(const std::string& port);
t_port						getPortType(int value);
bool						setPortNumber(const char *str, int *o_value);

bool						isValidPwd(const std::string& pwd);

std::vector<std::string>	split(std::string input, char delimiter);

#endif
