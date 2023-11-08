#include "AimBot.h"


AimBot::~AimBot() {
	delete PAimBotManager;
}
Vector3 AimBot::calcAngles(Vector3 from, Vector3 to) {
    Vector3 newAngles;
    Vector3 ipotenuse = to - from;
    float Side = to.z - from.z;
    float Ipotenuse = ipotenuse.Length();
    float pitch = asin(Side / Ipotenuse) * 180 / (float)PI;
    newAngles.y = pitch;
    float yaw = -atan2f(to.x - from.x, to.y - from.y) / (float)PI * 180 + 180;
    newAngles.x = yaw;
    newAngles.z = 0;
    return newAngles;
}
bool AimBot::IsEnemy(ent* e) {
    if (localPlayer->team == e->team)
        return false;
    else
        return true;
}
bool AimBot::IsTeamGame() {
    if (*gameMode == 0 || *gameMode == 4 || *gameMode == 5 || *gameMode == 7 || *gameMode == 11 || *gameMode == 13 ||
        *gameMode == 14 || *gameMode == 16 || *gameMode == 17 || *gameMode == 20 || *gameMode == 21)
        return true;
    else
        return false;
}
ent* AimBot::getClosest(ent* from, entList* to, float fov) {
    if (from && to) {
        float closestDist = 1000000;
        float currDist = 0;
        Vector3 distVec{ 0,0,0 };
        int closestDistanceIndex = -1;
        for (int i = 0; i < *numOfPlayers; i++) {
            if (to->ents[i] != nullptr) {
                if (IsVisible(from, to->ents[i])) {
                    distVec = calcAngles(from->head, to->ents[i]->head) - localPlayer->angle;
                    currDist = distVec.Length();
                    if (IsTeamGame()) {
                        if (IsEnemy(to->ents[i]) && currDist < closestDist && currDist < ((fov*0.5)+5) && to->ents[i]->health>0) {
                            closestDist = currDist;
                            closestDistanceIndex = i;
                        }
                    }
                    else {
                        if (currDist < closestDist && currDist < ((fov*0.5) +5) && to->ents[i]->health>0) {
                            closestDist = currDist;
                            closestDistanceIndex = i;
                        }
                    }
                }
            }

        }
        if (closestDistanceIndex == -1)
            return nullptr;
        return to->ents[closestDistanceIndex];
    }
}
void AimBot::Exec(float fov) {  ///SETTINGS FOR AIMBOT
    if (this->PAimBotManager->bActive && (GetAsyncKeyState(VK_XBUTTON1) || GetAsyncKeyState(VK_LCONTROL))) {
        ent* closestEnemy = getClosest(localPlayer, entlist, fov);
        if (closestEnemy == nullptr)
            return;
        if (this->PAimBotManager->bSmooth) {
            Vector3 dst = (calcAngles(localPlayer->head, closestEnemy->head) - localPlayer->angle);
            localPlayer->angle.x += dst.x / (this->PAimBotManager->SmoothPercentage* 100);
            localPlayer->angle.y += dst.y / (this->PAimBotManager->SmoothPercentage* 100);
            Sleep(1);
            return;
        }
        else {
            Vector3 dst = (calcAngles(localPlayer->head, closestEnemy->head));
            localPlayer->angle.x = dst.x;
            localPlayer->angle.y = dst.y;
            return;
        }
    }
}