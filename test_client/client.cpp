/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 16:11:18 by alee              #+#    #+#             */
/*   Updated: 2022/08/29 13:11:34 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef int	SOCKET;

int	main(void)
{
	std::cout << "Test Client" << std::endl;

	//input port
	unsigned short	s_port;
	std::cout << "Input Server port : " << std::endl;
	std::cin >> s_port;

	//input ip
	std::string		s_ip;
	std::cout << "Input Server IP (-1 = 127.0.0.1) : " << std::endl;
	std::cin >> s_ip;
	if (s_ip == "-1")
		s_ip = "127.0.0.1";

	//socket
	SOCKET	client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_sock == -1)
	{
		std::cerr << "Error : socket(...)" << std::endl;
		return (1);
	}

	//display port, ip
	std::cout << "Server port : " << s_port << std::endl;
	std::cout << "Server IP : " << s_ip << std::endl;

	//sockaddr_in init
	struct sockaddr_in	c_addr_in;
	memset(&c_addr_in, 0x00, sizeof(c_addr_in));
	c_addr_in.sin_family = AF_INET;
	c_addr_in.sin_port = htons(s_port);
	inet_pton(AF_INET, s_ip.c_str(), (void *)&c_addr_in.sin_addr);
	socklen_t	c_addr_len = sizeof(c_addr_in);

	//connect
	int	connect_result = connect(client_sock, (const sockaddr *)&c_addr_in, c_addr_len);
	if (connect_result == -1)
	{
		std::cerr << "Error : connect(...)" << std::endl;
		return (1);
	}

	while (1)
	{
		std::string	msg = "CAP LS 302\r\nPASS 1234\r\n";
		std::cout << "Msg : " << std::endl;

		// std::cin >> msg;

		int	send_result = send(client_sock, msg.c_str(), msg.length(), 0);
		if (send_result != msg.length() || send_result == -1)
		{
			std::cerr << "Error : send(...)" << std::endl;
			break ;
		}
		std::cout << "send " << send_result << "byte" << std::endl;
		while (1)
		{

		}
		msg.clear();
	}
	return (0);
}
