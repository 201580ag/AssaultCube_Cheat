#pragma once
#include "Hooking.h"
#include "Scanner.h"
#include <vector>
class Others
{
private:
	void* ammoAddr = nullptr;
	void* recoilAddr = nullptr;
	BYTE* fixByteAmmo{ nullptr };
	BYTE* fixByteRecoil{ nullptr };
	BYTE* fixByteRapid{ nullptr };
	Scanner* myScanner = new Scanner("ac_client.exe");
public:
	struct local {
		bool bNoAmmo = false;
		bool bRecoil = false;
		bool bRapidFire = false;
		bool bSpeed = false;
	}localManager;
	struct cCheats {
		bool noAmmo = false;
		bool rapidFire = false;
		bool noRecoil = false;
		bool Speed = false;
	};
	//handle the cheat;
	cCheats* PCheatManager = new cCheats;
	Others();
	~Others();
	void NoAmmo();
	void noRecoil();
	void SpeedHack();
	void RapidFire();
};

