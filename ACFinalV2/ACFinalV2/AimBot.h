#pragma once
#define PI 3.14159265358979323846 
#include <Windows.h>
#include "geom.h"
#include "Traceline.h"
#include "reclass.h"
class AimBot
{
private:
    ent* localPlayer = *(ent**)0x50f4f4;
    int* numOfPlayers = (int*)(0x50f500);
    int* gameMode = (int*)(0x50F49C);
    entList* entlist = *(entList**)0x50f4f8;
public:
    struct AimBotConf {
        bool bActive = false;
        bool bSmooth = false;
        float SmoothPercentage = 0.2f;
        
    };
    AimBotConf* PAimBotManager = new AimBotConf;
    ~AimBot();
    Vector3 calcAngles(Vector3 from, Vector3 to);
    bool IsEnemy(ent* e);
    bool IsTeamGame();
    ent* getClosest(ent* from, entList* to, float fov);
    void Exec(float fov);

};

