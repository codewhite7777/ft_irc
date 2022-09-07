#include "channel.hpp"
#include "server.hpp"

// void UserMode(std::map<SOCKET, Client*>::iterator &iter, \
// 						std::string& name, int signal, std::string& mode)
// {
// 	std::cout << "Name: [" << name << "]\n";
// 	std::cout << "signal: [" << signal << "]\n";
// 	std::cout << "mode: [" << mode << "]\n";

// 	//
// 	iter->second->
// }
#include "mode.hpp"
bool test(int signal, char mode, unsigned &channelMode)
{
	if (!signal)
	{
		switch (mode) {
		case 'o':
			if (channelMode & (1 << CHANNEL_O))
				return false;
			channelMode |= (1 << CHANNEL_O);
			break;
		case 't':
			if (channelMode & (1 << CHANNEL_T))
				return false;
			channelMode |= (1 << CHANNEL_T);
			break;
		case 'n':
			if (channelMode & (1 << CHANNEL_N))
				return false;
			channelMode |= (1 << CHANNEL_N);
			break;
		case 's':
			if (channelMode & (1 << CHANNEL_S))
				return false;
			channelMode |= (1 << CHANNEL_S);
			break;
		default:
			return false;
		}
	}
	else {
		switch (mode) {
		case 'o':
			if (!channelMode & !(1 << CHANNEL_O))
				return false;
			channelMode &= !(1 << CHANNEL_O);
			break;
		case 't':
			if (channelMode & (1 << CHANNEL_T))
				return false;
			channelMode |= (1 << CHANNEL_T);
			break;
		case 'n':
			if (channelMode & (1 << CHANNEL_N))
				return false;
			channelMode |= (1 << CHANNEL_N);
			break;
		case 's':
			if (channelMode & (1 << CHANNEL_S))
				return false;
			channelMode |= (1 << CHANNEL_S);
			break;
		default:
			return false;
		}
	}
	return true;
}
void Server::ChannelMode(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& name, int signal, std::string& mode)
{
	std::cout << "signal: [" << signal << "]\n";
	std::cout << "mode: [" << mode << "]\n";
	std::map<std::string, Channel*>::iterator chanIter = chann_map_.find(name);
	if (chanIter == chann_map_.end())
	{
		;//not found
		return ;
	}
	else
	{
		if (!signal)
		{
			for (int i = 1 ; i < mode.size() ; ++i)
			{
				
			}
		}
	}
	}
}



#include <sstream>
static std::vector<std::string> split(std::string str, char delimiter)
{
	std::vector<std::string>	internal;
	std::stringstream			ss(str);
	std::string					temp;

	while (getline(ss, temp, delimiter))
		internal.push_back(temp);
	return (internal);
}

void		Server::requestMode(std::map<SOCKET, Client*>::iterator &iter, \
						std::string& command, std::string& param)
{
	std::string	cp_param;
	std::string	tar_chan;
	std::string	option;
	std::string	tar_nick;

	std::vector<std::string> splitVector = split(param, ' ');
	tar_chan = splitVector[0];
	if (tar_chan[0] != '#')
	{
		std::cout << "erroor\n";
		return ;
	}
	option = splitVector[1];
	int signal;
	// 더하기
	if (option[0] == '+')
	{
		signal = 0;
	}
	// 빼기
	else if (option[0] == '-')
	{
		signal = 1;
	}
	// 예외
	else {
		signal = 2;
		std::cerr << "에러임 ㅅㅂ\n";
		return ;
	}

	// user mode
	if (splitVector.size() > 2)
	{
		tar_nick = splitVector[2];
		// UserMode(iter, tar_nick, signal, option);
	}
	// channel mode
	else
	{
		
	}
	return ;
}