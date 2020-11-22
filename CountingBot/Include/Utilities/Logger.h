#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <unordered_set>

enum class Severity {
	INFO,
	DEBUG,
	WARNING,
	ERROR
};

class Logger {
public:
	Logger(const char* name);

	// Log a formatted message with the specified severity.
	void Log(Severity severity, const char* format, ...);
	// Log a formatted info message.
	void LogInfo(const char* format, ...);
	// Log a formatted debug message.
	void LogDebug(const char* format, ...);
	// Log a formatted warning message.
	void LogWarning(const char* format, ...);
	// Log a formatted error message.
	void LogError(const char* format, ...);

public:
	// Initializes the logger.
	static void Init();
	// Deinitializes the logger.
	static void DeInit();

private:
	// Get's the max number of messages before flushing to the file.
	static uint64_t GetSeverityMaxBufferCount(Severity severity);
	// Get's the color of the message based on it's severity.
	static const char* GetSeverityColor(Severity severity);
	// Get's the name of the severity.
	static const char* GetSeverityName(Severity severity);

	// Logs a formatted message to both the console and a file if it it can.
	static void Log(const char* name, Severity severity, const char* format, va_list args);
	// Flushes previously printed messages to a file.
	static void Flush();

private:
	const char* name;	// The name of this logger.

private:
	static std::unordered_set<Severity> EnabledSeverities;	// The enabled severities.

	static std::vector<std::string> Buffer;	// The message buffer

	static std::string LogFile;		// The file to log to.
	static uint32_t MaxLogFiles;	// The max number of logs to keep.

	static bool LogToFile;	// Can log to file.
};