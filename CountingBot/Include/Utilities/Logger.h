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

	void Log(Severity severity, const char* format, ...);
	void LogInfo(const char* format, ...);
	void LogDebug(const char* format, ...);
	void LogWarning(const char* format, ...);
	void LogError(const char* format, ...);

public:
	static void Init();
	static void DeInit();

private:
	static uint64_t GetSeverityMaxBufferCount(Severity severity);
	static const char* GetSeverityColor(Severity severity);
	static const char* GetSeverityName(Severity severity);

	static void Log(const char* name, Severity severity, const char* format, va_list args);
	static void Flush();

private:
	const char* name;

private:
	static std::unordered_set<Severity> EnabledSeverities;

	static std::vector<std::string> Buffer;

	static std::string LogFile;

	static bool LogToFile;
};