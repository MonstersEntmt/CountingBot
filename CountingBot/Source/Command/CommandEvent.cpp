#include "Command/CommandEvent.h"

CommandEvent::CommandEvent(/* const Message& message, */ const std::string& command, const std::vector<std::string>& arguments)
	: /* msg(message), */ cmd(command), args(arguments) {}

EventType CommandEvent::GetType() const {
	return CommandEvent::GetTypeS();
}

//const Message& Command::GetMessage() const {
//	return this->msg;
//}

const std::string& CommandEvent::GetCommand() const {
	return this->cmd;
}

const std::vector<std::string>& CommandEvent::GetArguments() const {
	return this->args;
}