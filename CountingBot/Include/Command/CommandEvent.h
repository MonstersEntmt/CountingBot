#pragma once

#include "Event/Event.h"

#include <string>
#include <vector>

struct CommandEvent : public Event {
public:
	CommandEvent(/* const Message& message, */ const std::string& command, const std::vector<std::string>& arguments);

	__forceinline static EventType GetTypeS() { return EventType::COMMAND; }
	virtual EventType GetType() const override;

	// const Message& GetMessage() const;
	const std::string& GetCommand() const;
	const std::vector<std::string>& GetArguments() const;

private:
	//Message msg;
	std::string cmd;
	std::vector<std::string> args;
};