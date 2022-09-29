#include "Message.hpp"
Message::Message(STRING userFullInfo, STRING replyNumber, STRING param)
{
	replyMessage = userFullInfo + " " + replyNumber + " " + param + END;
}

Message::Message(STRING userFullInfo, STRING replyNumber, STRING param, STRING msg)
{
	replyMessage = userFullInfo + " " + 
									replyNumber + " " + 
									param + " " + 
									msg + 
									END;
}

Message::Message(STRING userFullInfo, STRING replyNumber, STRING param1, STRING param2, STRING msg)
{
	replyMessage = userFullInfo + " " + 
									replyNumber + " " + 
									param1 + " " + 
									param2 + " " + 
									msg + 
									END;
}

STRING Message::getReplyMessage()
{
	return replyMessage;
}