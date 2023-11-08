#include "Traceline.h"

struct traceresult_s {
    Vector3 end;
    bool collided;
};

bool IsVisible(ent* localPlayer, ent* enemy) {
    DWORD traceLine = 0x048a310;
    traceresult_s traceResult;
    traceResult.collided = false;
    Vector3 from = localPlayer->head;
    Vector3 to = enemy->head;

    __asm {
        push 0
        push 0
        push localPlayer
        push to.z
        push to.y
        push to.x
        push from.z
        push from.y
        push from.x
        lea eax, [traceResult]
        call traceLine
        add esp, 36
    }
    return !traceResult.collided;
}