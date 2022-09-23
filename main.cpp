/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 12:47:12 by alee              #+#    #+#             */
/*   Updated: 2022/08/22 19:48:26 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main(int argc, char *argv[])
{
	Server	irc_server(argc, argv);
	while (irc_server.getStatus())
		irc_server.Run();
	return (0);
}
