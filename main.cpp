/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:47:12 by alee              #+#    #+#             */
/*   Updated: 2022/08/29 15:00:58 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int	main(int argc, char *argv[])
{
	// std::string	buf1 = "CAP LS 302\r\nPASS 1234";
	// size_t	command_tok1 = buf1.find("\r\n");
	// std::cout << '[' << command_tok1 << ']' << std::endl;
	// std::string	buf2 = "CAP LS 302";
	// size_t	command_tok2 = buf2.find("\r\n");
	// std::cout << '[' << command_tok2 << ']' << std::endl;
	// std::string	command = buf1.substr(0, command_tok1);
	// std::string	param = buf1.substr(command_tok1 + 2);

	// std::cout << "command : " << '[' << command << ']' << std::endl;
	// std::cout << "param   : " << '[' << param << ']' << std::endl;

	Server	irc_server(argc, argv);
	while (irc_server.getStatus())
		irc_server.Run();
	return (0);
}
