#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <stdlib.h>
#include <string>

using namespace std;

DWORD baseAddress = 0x0017E0A8;

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

DWORD GetPointerAddress(HWND hwnd, DWORD gameBaseAddr, DWORD address, vector<DWORD> offsets)
{
    DWORD pID = NULL; // Game process ID
    GetWindowThreadProcessId(hwnd, &pID);
    HANDLE phandle = NULL;
    phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (phandle == INVALID_HANDLE_VALUE || phandle == NULL);

    DWORD offset_null = NULL;
    ReadProcessMemory(phandle, (LPVOID*)(gameBaseAddr + address), &offset_null, sizeof(offset_null), 0);
    DWORD pointeraddress = offset_null; // the address we need
    for (int i = 0; i < offsets.size() - 1; i++) // we dont want to change the last offset value so we do -1
    {
        ReadProcessMemory(phandle, (LPVOID*)(pointeraddress + offsets.at(i)), &pointeraddress, sizeof(pointeraddress), 0);
    }
    return pointeraddress += offsets.at(offsets.size() - 1); // adding the last offset
}

int main()
{
    HWND hwnd_AC = FindWindowA(NULL, "AssaultCube"); // 윈도우 핸들 가져오기

    if (hwnd_AC != FALSE);
    DWORD pID = NULL;
    GetWindowThreadProcessId(hwnd_AC, &pID);
    HANDLE phandle = NULL;
    phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (phandle == INVALID_HANDLE_VALUE || phandle == NULL);

    char gamemodule1[] = "ac_client.exe"; // 모듈 입력
    DWORD gamebaseaddress1 = GetModuleBaseAddress(_T(gamemodule1), pID); // 모듈, 베이스 주소 가져오기

    // name 베이스 주소, 오프셋
    vector<DWORD> usernameOffsets{ 0x33C, 0x14, 0xDD }; // 오프셋 주소 입력
    DWORD usernamePtrAddr = GetPointerAddress(hwnd_AC, gamebaseaddress1, baseAddress, usernameOffsets);

    // Health 베이스 주소, 오프셋
    vector<DWORD> HealthOffsets{ 0xEC }; // 오프셋 주소 입력
    DWORD HealthPtrAddr = GetPointerAddress(hwnd_AC, gamebaseaddress1, baseAddress, HealthOffsets);

    // ammo 베이스 주소, 오프셋
    vector<DWORD> ammoOffsets{ 0X35C, 0X18, 0X200, 0X14, 0X0 }; // 오프셋 주소 입력
    DWORD ammoPtrAddr = GetPointerAddress(hwnd_AC, gamebaseaddress1, baseAddress, ammoOffsets);

    // 방탄복 bt 베이스 주소, 오프셋
    vector<DWORD> btOffsets{ 0Xf0 }; // 오프셋 주소 입력
    DWORD btPtrAddr = GetPointerAddress(hwnd_AC, gamebaseaddress1, baseAddress, btOffsets);

    // Grenade 베이스 주소, 오프셋
    vector<DWORD> GrenadeOffsets{ 0X144 }; // 오프셋 주소 입력
    DWORD GrenadePtrAddr = GetPointerAddress(hwnd_AC, gamebaseaddress1, baseAddress, GrenadeOffsets);


    // x 베이스 주소, 오프셋
    //vector<DWORD> xOffsets{ 0X28 }; // 오프셋 주소 입력
    //DWORD xPtrAddr = GetPointerAddress(hwnd_AC, gamebaseaddress1, baseAddress, xOffsets);

    // z 베이스 주소, 오프셋
    //vector<DWORD> zOffsets{ 0X2C }; // 오프셋 주소 입력
    //DWORD zPtrAddr = GetPointerAddress(hwnd_AC, gamebaseaddress1, baseAddress, zOffsets);

    // y 베이스 주소, 오프셋
    //vector<DWORD> yOffsets{ 0X30 }; // 오프셋 주소 입력
    //DWORD yPtrAddr = GetPointerAddress(hwnd_AC, gamebaseaddress1, baseAddress, yOffsets);

    //float x; // 수류탄
    //ReadProcessMemory(phandle, (LPCVOID)xPtrAddr, &x, sizeof(int), NULL);
    //cout << x << endl;

    //float z; // 수류탄
    //ReadProcessMemory(phandle, (LPCVOID)zPtrAddr, &z, sizeof(int), NULL);
    //cout << z << endl;;

    //float y; // 수류탄
    //ReadProcessMemory(phandle, (LPCVOID)yPtrAddr, &y, sizeof(int), NULL);
    //cout << y << endl;


    // 메모리 쓰기
    while (true)
    {
        string username = "201580ag"; // 닉네임
        size_t StringLength = username.length() + 1; // 문자열 길이 + null 종료 문자(\0)
        WriteProcessMemory(phandle, (LPVOID*)usernamePtrAddr, username.c_str(), StringLength, 0);

        int Health = 9999; // 체력
        WriteProcessMemory(phandle, (LPVOID*)(HealthPtrAddr), &Health, 4, 0);

        int bt = 100; // 방탄복
        WriteProcessMemory(phandle, (LPVOID*)(btPtrAddr), &bt, 4, 0);

        int ammo = 999; // 탄약(소총)
        WriteProcessMemory(phandle, (LPVOID*)(ammoPtrAddr), &ammo, 4, 0);

        int Grenade = 10; // 수류탄
        WriteProcessMemory(phandle, (LPVOID*)(GrenadePtrAddr), &Grenade, 4, 0);
    }
}
