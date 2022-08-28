#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include "client.hpp"


class ChannelData{
private:
	std::string name;           // ChannelName;
	std::vector<Client> clients;  // client Information

public:
	ChannelData();
};

// bool	IsExistChannel(std::string name);
#endif