#include "Command/CommandEventHandler.h"
#include "Command/CommandEvent.h"

CommandEventHandler::CommandEventHandler()
	: EventHandler("Command Event Handler", true) {}

void CommandEventHandler::HandleEvent(Event* event) {
	if (event->GetType() == CommandEvent::GetTypeS()) {	// Check if the event is a command event.
		// Convert it to the CommandEvent type.
		CommandEvent* commandEvent = reinterpret_cast<CommandEvent*>(event);

		std::string commandStr = commandEvent->GetCommand();
		for (auto& arg : commandEvent->GetArguments()) {
			commandStr += " " + arg;
		}

		GetLogger().LogDebug("CommandEvent: '%s' handled", commandStr.c_str());
	}
}