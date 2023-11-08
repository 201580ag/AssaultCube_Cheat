#include "Scanner.h"


void * Scanner::PatternScan(char* base, size_t size, const char* pattern, const char* mask)
{
	size_t patternLength = strlen(mask);
	/*per ogni byte del pattern itireremo su ogni elemento della maskera
	usiamo size-pattern per una questione di ottimizzazione
	se togliamo la lunghezza di pattern da size evitiamo di scansionare pattern inutilmente
	se il primo byte non è uguale agli altri*/
	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		// base + i indica la posizione corrente. +j indica la posizione corrente + quale pattern
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(base + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (void*)(base + i);
		}
	}
	return nullptr;
}

MODULEINFO Scanner::GetModuleInfo(const char* szModule) {
	MODULEINFO modInfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == NULL) {
		return modInfo;
	}
	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
	return modInfo;
}

Scanner::Scanner(const char* modName) {
	this->modInfo = GetModuleInfo(modName);
}

void* Scanner::getAddress(const char* pattern, const char* mask) {
	size_t size = this->modInfo.SizeOfImage;
	char* base = (char*)this->modInfo.lpBaseOfDll;
	return PatternScan(base, size, pattern, mask);
}