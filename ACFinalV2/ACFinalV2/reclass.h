#pragma once
#include <Windows.h>
#include "geom.h"

using vec3 = Vector3;



enum gameModes
{
	TEAMDEATHMATCH = 0,
	COOPEDIT,
	DEATHMATCH,
	SURVIVOR,
	TEAMSURVIVOR,
	CTF,
	PISTOLFRENZY,
	BOTTEAMDEATHMATCH,
	BOTDEATHMATCH,
	LASTSWISSSTANDING,
	ONESHOTONEKILL,
	TEAMONESHOTONEKILL,
	BOTONESHOTONEKILL,
	HUNTTHEFLAG,
	TEAMKEEPTHEFLAG,
	KEEPTHEFLAG,
	TEAMPF,
	TEAMLSS,
	BOTPISTOLFRENZY,
	BOtlSS,
	BOTTEAMSURVIVOR,
	BOTTEAMONESHOTONEKILL,
	NUM
};

class ent;
class N000002E8
{
public:
	char N000002AA[16]; //0x0000
}; //Size: 0x0010
class ammo_t
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004
class gunwait_t
{
public:
	int32_t N000002D6; //0x0000
}; //Size: 0x0004
class MagAmmo_t
{
public:
	int32_t ammo; //0x0000
}; //Size: 0x0004
class weapon
{
public:
	char pad_0000[4]; //0x0000
	int32_t ID; //0x0004
	class ent* owner; //0x0008
	class N000002E8* guninfo; //0x000C
	class ammo_t* ammo; //0x0010
	class MagAmmo_t* magAmmo; //0x0014
	class gunwait_t* gunwait; //0x0018
	int32_t shots; //0x001C
	int32_t reloading; //0x0020
	char pad_0024[41]; //0x0024
}; //Size: 0x004D

class ent
{
public:
	DWORD vTable; //0x0
	vec3 head; //0x0004
	char _0x0010[36];
	vec3 pos; //0x0034
	vec3 angle; //0x0040
	char pad_004C[34]; //0x004C
	bool bCrouching; //0x006E
	char pad_006F[137];
	__int32 health; //0x00F8
	__int32 armor; //0x00FC
	char _0x0100[292];
	BYTE bAttacking; //0x0224
	char name[16]; //0x0225
	char _0x0235[247];
	BYTE team; //0x032C
	char _0x032D[11];
	BYTE state; //0x0338
	char _0x0339[59];
	weapon* weapon; //0x0374
	char _0x0378[520];
};

struct entList
{
	ent* ents[31];
};