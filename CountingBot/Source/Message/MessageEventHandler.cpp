#include "Message/MessageEventHandler.h"
#include "Message/MessageEvent.h"
#include "Command/CommandEvent.h"

#include <stdint.h>

void Split(std::string str, char delimiter, uint64_t offset, std::vector<std::string>& vector) {
	uint64_t index;
	while ((index = str.find_first_of(delimiter, offset)) < str.length()) {
		vector.push_back(str.substr(offset, index - offset));
		offset = index + 1;
	}

	if (offset < str.length()) {
		vector.push_back(str.substr(offset));
	}
}

MessageEventHandler::MessageEventHandler()
	: EventHandler("Message Event Handler", true) {}

void MessageEventHandler::HandleEvent(Event* event) {
	if (event->GetType() == MessageEvent::GetTypeS()) {
		MessageEvent* messageEvent = reinterpret_cast<MessageEvent*>(event);

		const HyperDiscord::Message& msg = messageEvent->GetMessage();

		if (msg.Content._Starts_with("!")) {
			uint64_t firstSpace = msg.Content.find_first_of(' ');
			CommandArgs args;
			if (firstSpace < msg.Content.length()) {
				Split(msg.Content, ' ', firstSpace + 1, args);
			}

			EventHandler::PushEvent(new CommandEvent(*messageEvent, msg.Content.substr(1, firstSpace - 1), args));
		}
	}
}