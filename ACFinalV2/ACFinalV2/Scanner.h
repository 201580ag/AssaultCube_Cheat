#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
class Scanner
{
private:
	MODULEINFO modInfo{ 0 };
public:
	void* PatternScan(char* base, size_t size, const char* pattern, const char* mask);
	MODULEINFO GetModuleInfo(const char* szModule);
	Scanner(const char* modName);
	void* getAddress(const char* pattern, const char* mask);
};

