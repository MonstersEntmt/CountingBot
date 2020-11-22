#include "CountingBot.h"

#include "Event/EventHandler.h"
#include "Command/CommandHandler.h"
#include "Command/CommandEvent.h"

Logger CountingBotLogger("CountingBot");

int main() {
	Logger::Init();	// Initialize the logger

	CommandHandler commandHandler;							// Create a CommandHandler instance.
	EventHandler::RegisterEventHandler(&commandHandler);	// Register the commandHandler.

	std::vector<std::string> commandArgs{ "set", "cankick", "true" };	// Create a CommandEvent and push it out into the EventHandler.
	CommandEvent event = CommandEvent("admin", commandArgs);
	EventHandler::PushEvent(&event);

	EventHandler::HandleEvents();	// Handle the events in the queue.

	Logger::DeInit();	// Deinitialize the logger
}