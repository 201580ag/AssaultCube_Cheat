#pragma once
#include <Windows.h>
#include <iostream>
#define PI 3.14159265358979323846 


void Patch(BYTE* dst, BYTE* src, unsigned int size);
bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len);
BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len);


class TrampolineHook
{
private:
	bool m_bStatus{ false };
	BYTE* m_src{ nullptr };	
	BYTE* m_dst{ nullptr };		
	BYTE* m_gatewayPtr{ nullptr };
	SIZE_T m_len{ 0 };
	BYTE* m_originalBytes{ nullptr };
public:
	bool getStatus();
	void Enable();
	void Disable();
	TrampolineHook(BYTE* src, BYTE* dst, BYTE* gatewayPtr, SIZE_T len);
	TrampolineHook(const char* exportName, const char* moduleName, BYTE* dst, BYTE* gatewayPtr, SIZE_T len);
	~TrampolineHook();
	void Toggle();

};

