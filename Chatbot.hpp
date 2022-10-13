#ifndef _CHATBOT_HPP_
# define _CHATBOT_HPP_

# include <string>
# include <map>

#include <string>
class ChatBot
{
public:
	ChatBot();
	~ChatBot();

	//void	JoinChannel();

	bool	CheckChatBotCommand(std::string param, std::string &ret);
	void	BroadCast(std::string server_name);

	bool	ChatBotCommand(std::string&, std::string&);

	// function

	std::string		CommandList();
	void			AddCommand(std::string&);
	void			DeleteCommand(std::string &);
	std::string		GetNowTime();
	std::string		getPrefix();

private:
	std::map< std::string, std::string > chatbot_command_list;
};

#endif
