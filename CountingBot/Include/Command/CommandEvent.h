#pragma once

#include "Event/Event.h"
#include "Message/MessageEvent.h"

#include <string>
#include <vector>

using CommandArgs = std::vector<std::string>;

struct CommandEvent : public Event {
public:
	CommandEvent(const MessageEvent& message, const std::string& command, const CommandArgs& arguments);

	virtual EventType GetType() const override;
	inline static EventType GetTypeS() { return EventType::COMMAND; }

	const MessageEvent& GetMessage() const;
	const std::string& GetCommand() const;
	const CommandArgs& GetArguments() const;

private:
	MessageEvent msg;
	std::string cmd;
	CommandArgs args;
};