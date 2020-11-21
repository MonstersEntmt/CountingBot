#include "CountingBot.h"

Logger CountingBotLogger("CountingBot");

int main() {
	const char* format = "This log splits into two lines %s";

	Logger::Init();

	CountingBotLogger.LogInfo(format, "\nWhere this is the second line");
	CountingBotLogger.LogDebug(format, "\nWhere this is the second line");
	CountingBotLogger.LogWarning(format, "\nWhere this is the second line");
	CountingBotLogger.LogError(format, "\nWhere this is the second line");

	Logger::DeInit();
}