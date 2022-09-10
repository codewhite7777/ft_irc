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

typedef int	SOCKET;

typedef enum e_port
{
	WELL_KNOWN_PORT,
	REGISTERED_PORT,
	DYNAMIC_PORT,
	INVALID_PORT,
}			t_port;

typedef enum e_buf
{
	BUFFER_MAX = 1024 << 4,
	// BUFFER_MAX = 100,
}			t_buf;

// configure port
bool	isValidPort(const std::string& port);
t_port	getPortType(int value);
bool	getPortNumber(const char *str, int *o_value);

// configure pwd
bool	isValidPwd(const std::string& pwd);
bool	isSpecialCharactor(const std::string& str);


std::vector<std::string> split(std::string input, char delimiter);
#endif
