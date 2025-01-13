#pragma once
#include <vector>   // std::vector를 사용하기 위해 필요합니다.
#include <cstdint>  // uintptr_t 타입을 사용하기 위해 필요합니다.
#include <Windows.h>

uintptr_t moduleBase = (uintptr_t)GetModuleHandle("ac_client.exe"); // 대상 프로세스



//namespace Offsets {
//    constexpr uintptr_t Entity_List = 0x18AC04
//
//
//
//}


struct Offsets
{
    DWORD My_Base = 0x0017E0A8;
    DWORD E_Player = 0x00191FCC;
    DWORD Entity_List = 0x18AC04;
    DWORD FOV = 0x18A7CC;
    DWORD PlayerCount = 0x18AC0C;
    // 기존 값들
    DWORD Health = 0xEC;
    DWORD Armor = 0xF0;
    DWORD Ammo = 0x140;
    DWORD Camera_X = 0x34;
    DWORD Camera_Y = 0x38;
    DWORD POS_Z = 0x28;
    DWORD POS_Y = 0x30;
    DWORD POS_X = 0x2C;

    DWORD Head_Position_X = 0x4;
    DWORD Head_Position_Y = 0x8;
    DWORD Head_Position_Z = 0xC;

    // 플레이어의 카메라
    DWORD Player_Camera_X = 0x34;
    DWORD Player_Camera_Y = 0x38;

    // 무기별 탄약
    DWORD Submachine_Gun_Ammo = 0x138;
    DWORD Sniper_Ammo = 0x13C;
    DWORD Shotgun_Ammo = 0x134;
    DWORD Pistol_Ammo = 0x12C;
    DWORD Grenade_Ammo = 0x144;

    // 빠른 발사 관련
    DWORD Fast_Fire_Assault_Rifle = 0x164;
    DWORD Fast_Fire_Sniper = 0x160;
    DWORD Fast_Fire_Shotgun = 0x158;

    // 자동 사격
    DWORD Auto_Shoot = 0x204;

    // 플레이어 이름
    DWORD Player_Name = 0x205;

    // 모니터 해상도
    DWORD WIDTH = 0x191ED8;
    DWORD HEIGHT = 0x191EDC;

    //DWORD MATRIX_OFFSET = 0x57DFD0;
    DWORD MATRIX_OFFSET = 0x17DFD0;
};

Offsets offsets;