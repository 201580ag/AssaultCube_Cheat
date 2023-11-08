#include "Hooking.h"


void Patch(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oldProtect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy_s(dst, size, src, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}

bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len) {
	if (len < 5)
		return false;
	DWORD oldProtect;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(src, 0x90, len);
	uintptr_t relativeAddr = dst - (src + 5);
	*src = 0xE9;
	*(uintptr_t*)(src + 1) = relativeAddr;
	VirtualProtect(src, len, oldProtect, &oldProtect);
	return true;
}

BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len) {
	if (len < 5)
		return nullptr;
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy_s(gateway, len, src, len);
	uintptr_t gatewayRelativeAddr = src - gateway - 5;
	*(gateway + len) = 0xE9;
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddr;
	Detour32(src, dst, len);
	return gateway;
}

TrampolineHook::TrampolineHook(BYTE* src, BYTE* dst, BYTE* gatewayPtr, SIZE_T len) {
	m_src = src;
	m_dst = dst;
	m_gatewayPtr = gatewayPtr;
	m_len = len;
	this->m_originalBytes = new BYTE[len];
}
TrampolineHook::TrampolineHook(const char* exportName, const char* moduleName, BYTE* dst, BYTE* gatewayPtr, SIZE_T len) {
	m_dst = dst;
	m_gatewayPtr = gatewayPtr;
	m_len = len;
	HMODULE hModule = GetModuleHandleA(moduleName);
	if (hModule)
		m_src = (BYTE*)GetProcAddress(hModule, exportName);
	this->m_originalBytes = new BYTE[len];
}
bool TrampolineHook::getStatus() {
	return this->m_bStatus;
}
void TrampolineHook::Enable() {
	memcpy_s(m_originalBytes, m_len, m_src, m_len);
	*(uintptr_t*)m_gatewayPtr = (uintptr_t)TrampHook32(m_src, m_dst, m_len);
	m_bStatus = true;
}

void TrampolineHook::Disable() {
	Patch(m_src, m_originalBytes, m_len);
	VirtualFree((void*)*(uintptr_t*)m_gatewayPtr, 0, MEM_RELEASE);
	m_bStatus = false;
}
TrampolineHook::~TrampolineHook() {
	if (m_bStatus)
		Disable();
	delete[] this->m_originalBytes;
}
void TrampolineHook::Toggle() {
	if (!m_bStatus)
	{
		Enable();
		Sleep(10);
	}
	else
	{
		Disable();
		Sleep(10);
	}
}