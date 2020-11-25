#include "Command/CommandEvent.h"

CommandEvent::CommandEvent(const MessageEvent& message, const std::string& command, const CommandArgs& arguments)
	: msg(message), cmd(command), args(arguments) {}

EventType CommandEvent::GetType() const {
	return CommandEvent::GetTypeS();
}

const MessageEvent& CommandEvent::GetMessage() const {
	return this->msg;
}

const std::string& CommandEvent::GetCommand() const {
	return this->cmd;
}

const CommandArgs& CommandEvent::GetArguments() const {
	return this->args;
}