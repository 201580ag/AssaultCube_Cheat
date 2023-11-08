#include "Others.h"
uintptr_t FindDMAAddy(uintptr_t ptr, std::vector <unsigned int> offsets) {
	uintptr_t addr = ptr;

	for (unsigned int i = 0; i < offsets.size(); i++) {
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}
void Patch(void* dst, void* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}
void Nop(void* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}
uintptr_t baseModule = (uintptr_t)GetModuleHandle(NULL);;
void* rapidFireAddr = nullptr;
uintptr_t* weaponBase = (uintptr_t*)FindDMAAddy(baseModule + 0x10f4f4, { 0x374, 0x0 });
uintptr_t jmpBackRapid = (uintptr_t)rapidFireAddr + 5;

void _declspec(naked) RapidFireHack() {
	__asm {
		cmp esi, [weaponBase]
		jne label2
		NOP
		NOP
		mov esi, [esi + 0x14]
		jmp[jmpBackRapid]
		label2:
		mov[edx], ecx
			mov esi, [esi + 0x14]
			jmp[jmpBackRapid]
	}
}
Others::Others() {
	rapidFireAddr = myScanner->getAddress("\x89\x0A\x8B\x76", "xxxx");
	recoilAddr = myScanner->getAddress("\x50\x8D\x4C\x00\x00\x51\x8B\xCE\xFF\xD2\x8B\x46\x00\x3B\x05", "xxx??xxxxxxx?xx");
	ammoAddr = myScanner->getAddress("\xFF\x0E\x57\x8B\x7C\x00\x00\x8D\x74\x00\x00\xE8\x00\x00\x00\x00\x5F\x5E\xB0\x00\x5B\x8B\xE5\x5D\xC2\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55",
		"xxxxx??xx??x????xxx?xxxxx??xxxxxxxxxxxxx");
}
Others::~Others(){
	delete myScanner;
	delete PCheatManager;
}
void Others::NoAmmo() {
	while (ammoAddr == nullptr) {
		ammoAddr = myScanner->getAddress("\xFF\x0E\x57\x8B\x7C\x00\x00\x8D\x74\x00\x00\xE8\x00\x00\x00\x00\x5F\x5E\xB0\x00\x5B\x8B\xE5\x5D\xC2\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55",
			"xxxxx??xx??x????xxx?xxxxx??xxxxxxxxxxxxx");
	}
	int len = 2;
	if (PCheatManager->noAmmo && !localManager.bNoAmmo) {
		fixByteAmmo = new BYTE[len];
		memcpy(fixByteAmmo, ammoAddr, len);
		Nop(ammoAddr, len);
		localManager.bNoAmmo = true;
	}
	if (!PCheatManager->noAmmo && localManager.bNoAmmo) {
		Patch(ammoAddr, fixByteAmmo, len);
		localManager.bNoAmmo = false;
	}
}
void Others::noRecoil() {
	while (recoilAddr == nullptr) {
		recoilAddr = myScanner->getAddress("\x50\x8D\x4C\x00\x00\x51\x8B\xCE\xFF\xD2\x8B\x46\x00\x3B\x05", "xxx??xxxxxxx?xx");
	}
	int len = 10;
	if (PCheatManager->noRecoil && !localManager.bRecoil) {
		fixByteRecoil = new BYTE[len];
		memcpy(fixByteRecoil, recoilAddr, len);
		Nop(recoilAddr, len);
		localManager.bRecoil = true;
	}
	if (!PCheatManager->noRecoil && localManager.bRecoil) {
		Patch(recoilAddr, fixByteRecoil, len);
		localManager.bRecoil = false;
	}
}
void Others::SpeedHack() {
	int len = 5;
	if (PCheatManager->Speed && !localManager.bRapidFire) {
		fixByteRapid = new BYTE[len];
		memcpy(fixByteRapid, rapidFireAddr, len);
		Detour32((BYTE*)rapidFireAddr, (BYTE*)RapidFireHack, len);
		localManager.bRapidFire = true;
	}
	if (!PCheatManager->Speed && localManager.bRapidFire) {
		Patch(rapidFireAddr, fixByteRapid, len);
		localManager.bRapidFire = false;
	}
}
void Others::RapidFire() {
	while (rapidFireAddr == nullptr) {
		rapidFireAddr = myScanner->getAddress("\x89\x0A\x8B\x76", "xxxx");
	}
	if (PCheatManager->rapidFire && !localManager.bSpeed) {
		Patch((BYTE*)baseModule + 0x5BEA0, (BYTE*)"\xB8\x03\x00\x00\x00", 5);
		Patch((BYTE*)baseModule + 0x5BE40, (BYTE*)"\xB8\xFD\xFF\xFF\xFF", 5);
		Patch((BYTE*)baseModule + 0x5BF00, (BYTE*)"\xB8\x03\x00\x00\x00", 5);
		Patch((BYTE*)baseModule + 0x5BF60, (BYTE*)"\xB8\xFD\xFF\xFF\xFF", 5);
		localManager.bSpeed = true;
	}
	if (!PCheatManager->rapidFire && localManager.bSpeed) {
		Patch((BYTE*)baseModule + 0x5BEA0, (BYTE*)"\xB8\x01\x00\x00\x00", 5);
		Patch((BYTE*)baseModule + 0x5BE40, (BYTE*)"\xB8\xFF\xFF\xFF\xFF", 5);
		Patch((BYTE*)baseModule + 0x5BF00, (BYTE*)"\xB8\x01\x00\x00\x00", 5);
		Patch((BYTE*)baseModule + 0x5BF60, (BYTE*)"\xB8\xFF\xFF\xFF\xFF", 5);
		localManager.bSpeed = false;
	}
}