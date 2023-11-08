#include <Windows.h>
#include "Menu.h"

typedef BOOL(__stdcall* _wglSwapBuffers)(HDC hDc);
_wglSwapBuffers wglSwapBuffers;
typedef int(__cdecl* _printMiddle)(const char* format, ...);
_printMiddle printMiddle = (_printMiddle)(0x4090f0);
const char* LOGO = "AssaultCube Final V2 by R1perXNX";
Font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 8;
Menu* myMenu = new Menu(500, 500);

BOOL __stdcall hkSwapBuffers(HDC hDc) {
	myMenu->Render();
	HDC currentHDC = wglGetCurrentDC();
	if (!glFont.m_bBuilt || currentHDC != glFont.m_Hdc)
	{
		glFont.Build(FONT_HEIGHT);
	}
	GL::SetupOrtho();
	glFont.Print(GL::getRes().x - strlen(LOGO) * ESP_FONT_WIDTH, GL::getRes().y - 30.0f, rgb::purple, "%s", LOGO);//drawMyLogo	
	myMenu->DoCheat(glFont);
	GL::RestoreGL();
	return wglSwapBuffers(hDc);
}

DWORD WINAPI onLoad(HMODULE hModule) {
    printMiddle("\f8Loaded Successfully ");
    myMenu->Init();
    TrampolineHook* myHook = new TrampolineHook("wglSwapBuffers", "opengl32.dll", (BYTE*)hkSwapBuffers, (BYTE*)&wglSwapBuffers, 5);
    myHook->Toggle();
    while (true) {
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            myMenu->Toggle();
        }
        if (GetAsyncKeyState(VK_END) & 1) {
            break;
        }
       
    }
    printMiddle("\f8Unloaded Successfully ");
    delete myMenu;
    delete myHook;
    FreeLibraryAndExitThread(hModule, 0);
    CloseHandle(hModule);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = 0;
        hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)onLoad, hModule, 0, 0);
        if (hThread)
        {
            CloseHandle(hThread);
        }
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

