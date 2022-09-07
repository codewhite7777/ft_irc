#ifndef _MODE_HPP_
#define _MODE_HPP_

#define CHANNEL_O 0
#define CHANNEL_N 1
#define CHANNEL_T 2
#define CHANNEL_S 3

void Server::ChannelMode(std::map<SOCKET, Client*>::iterator &iter, \
						int signal, std::string& mode)
#endif