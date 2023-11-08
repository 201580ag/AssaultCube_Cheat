#pragma once
#include "reclass.h"
#include "Drawing.h"
#include "Traceline.h"
#include <string>
const int ESP_FONT_HEIGHT = 15;
const int ESP_FONT_WIDTH = 9;

class ESP
{
private:
	int* gameMode = (int*)(0x50F49C);
	int* numOfPlayers = (int*)(0x50f500);
	float* matrix = (float*)(0x501ae8);
	ent* localPlayer = *(ent**)0x50f4f4;
	entList* entlist = *(entList**)0x50f4f8;
	int viewport[4];
public:
	struct EspConf {
		bool bActive = false;
		bool bDrawNames = false;
		bool bDrawHealth = false;
		bool bDrawSnapLines = false;
		bool bMarkVisible = false; // not implemented yet
		bool bDrawDistance = false;
		bool bDrawHealthBar = false;
		bool bDrawWeapon = false;
		float Fov = 30.0f;
	};
	EspConf* PEspManager = new EspConf;

	~ESP();
	bool IsTeamGame();
	bool IsEnemy(ent* e);
	bool IsValidEnt(ent* e);
	void Draw(Font& font);
};

