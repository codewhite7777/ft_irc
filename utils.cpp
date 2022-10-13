/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 12:32:07 by mgo               #+#    #+#             */
/*   Updated: 2022/09/08 12:32:09 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <string>
#include <vector>
#include <sstream>

bool	isOnlyNums(std::string str)
{
	for (std::string::iterator it = str.begin()
		; it != str.end()
		; ++it)
	{
		if (isnumber(*it) == false)
			return false;
	}
	return true;
}

bool	isValidPort(const std::string& port)
{
	return (port.find_first_not_of("0123456789") == std::string::npos);
}

t_port	getPortType(int value)
{
	if (value >= 0 && value < 1024)
		return (WELL_KNOWN_PORT);
	else if (value < 49152)
		return (REGISTERED_PORT);
	else if (value < 65536)
		return (DYNAMIC_PORT);
	return (INVALID_PORT);
}

bool	setPortNumber(const char *str, int *o_value)
{
	__int64_t	total(0);
	t_port		retPort(INVALID_PORT);

	while (*str)
	{
		total = total * 10 + *str - '0';
		if (total > 2147483647 || total < -2147483648)
			return (false);
		str++;
	}
	retPort = getPortType(total);
	if (retPort == WELL_KNOWN_PORT || retPort == INVALID_PORT)
		return (false);
	*o_value = total;
	return (true);
}

bool	isNotAlnumAndUnderscore(const std::string& str)
{
	size_t i = 0;
	while (i < str.size())
	{
		if ((std::isalnum(str[i]) == false) && (str[i] != '_'))
		{
			return (true);
		}
		++i;
	}
	return (false);
}

bool	isSpecialCharactor(const std::string& str)
{
	size_t i = 0;
	while (i < str.size())
	{
		if (!std::isalnum(str[i]))
			return (true);
		++i;
	}
	return (false);
}

bool	isValidPwd(const std::string& pwd)
{
	if (pwd.length() == 0)
		return (false);
	if (isSpecialCharactor(pwd))
		return (false);
	return (true);
}

std::vector<std::string> split(std::string input, char delimiter)
{
    std::vector<std::string>	answer;
    std::stringstream			ss(input);
    std::string					temp;

    while (getline(ss, temp, delimiter))
	{
        answer.push_back(temp);
    }
    return answer;
}
