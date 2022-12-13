#pragma once

#include <mutex>
#include <vector>
#include <memory>

#ifdef _DEBUG
#include <ios>
#include <iosfwd>
#include <iostream>
#endif

#include "xorstr.h"
#include <string>

#pragma comment(lib, "runtimeobject")

#ifndef logF
//#define logF(x) Logger::WriteLogFileF(XorString(x))
#define logF(x, ...) Logger::WriteLogFileF(x, __VA_ARGS__)
//#define logF(x, ...) Logger::WriteLogFileF(XorString(x), __VA_ARGS__)
#endif

#ifndef logF_Debug
#ifdef _DEBUG
#define logF_Debug(x, ...) Logger::WriteLogFileF(x, __VA_ARGS__)
#else
#define logF_Debug(x, ...)
#endif
#endif

#ifndef logBF
//#define logF(x) Logger::WriteLogFileF(XorString(x))
#define logBF(x, ...) Logger::WriteBigLogFileF(2000, x, __VA_ARGS__)
//#define logF(x, ...) Logger::WriteLogFileF(XorString(x), __VA_ARGS__)
#endif

#ifndef logBF_Debug
#ifdef _DEBUG
#define logBF_Debug(x, ...) Logger::WriteBigLogFileF(2000, x, __VA_ARGS__)
#else
#define logBF_Debug(x, ...)
#endif
#endif

struct TextForPrint {
	char time[20];
	char text[100];
};

struct TextForPrintBig {
	char time[20];
	char text[2900];
};

class Logger {

public:
	static bool isActive();
	static std::wstring GetRoamingFolderPath();
	static void WriteLogFileF(const char* fmt, ...);
	static void WriteBigLogFileF(size_t maxSize, const char* fmt, ...);
	static void SendToConsoleF(const char* msg);
	static std::vector<TextForPrint>* GetTextToPrint();
	static std::vector<std::shared_ptr<TextForPrintBig>>* GetTextToSend();
	static std::lock_guard<std::mutex> GetTextToPrintLock();
	static std::lock_guard<std::mutex> GetTextToInjectorLock();
	//static std::vector<TextForPrint*> stringPrintVector;
	static void Disable();
};
