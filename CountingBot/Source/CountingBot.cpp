#include "CountingBot.h"

#include "Event/EventHandler.h"
#include "Command/CommandHandler.h"
#include "Command/CommandEvent.h"

void CountingBot::Init(const char* token) {
	Logger::Init();	// Initialize the logger

	if (!token) throw std::exception("Token was not given!");

	this->hyperClient = new HyperDiscord::HyperClient(token, HyperDiscord::TokenType::BOT);

	this->running = true;
}

void CountingBot::Update() {
	this->hyperClient->Update();

	CommandHandler commandHandler;							// Create a CommandHandler instance.
	EventHandler::RegisterEventHandler(&commandHandler);	// Register the commandHandler.

	std::vector<std::string> commandArgs{ "set", "cankick", "true" };	// Create a CommandEvent and push it out into the EventHandler.
	CommandEvent event = CommandEvent("admin", commandArgs);
	EventHandler::PushEvent(&event);

	EventHandler::HandleEvents();	// Handle the events in the queue.

	this->running = false;
}

void CountingBot::DeInit() {
	Logger::DeInit();	// Deinitialize the logger
}

bool CountingBot::IsRunning() {
	return this->running;
}

int main(int argc, char** argv) {
	Logger mainLogger{ "Main" };
	const char* token = nullptr;
	if (argc > 0) token = argv[1];
	CountingBot countingBot;
	try {
		countingBot.Init(token);

		try {
			while (countingBot.IsRunning()) countingBot.Update();
		} catch (std::exception e) {
			mainLogger.LogError("An exception was thrown from the countingBot.Update()!\n%s", e.what());
		}

		try {
			countingBot.DeInit();
		} catch (std::exception e) {
			mainLogger.LogError("An exception was thrown from countingBot.DeInit()!\n%s", e.what());
		}
	} catch (std::exception e) {
		mainLogger.LogError("An exception was thrown from countingBot.Init()!\n%s", e.what());
		Logger::DeInit();
	}
}