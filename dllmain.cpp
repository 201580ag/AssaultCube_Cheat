#include <Windows.h>
#include "include/imgui_hook.h"
#include "include/imgui/imgui.h"
#include "include/mem.h"
#include "include/offset.h"
#include <iostream>
#include <sstream>   // std::ostringstream
#include <iomanip>   // std::fixed, std::setprecision
#include <vector>
#include <math.h> // 삼각함수 헤더

// matrix4x4 구조체 정의
struct matrix4x4 {
    float value[16];
};

struct Vec3 {
    float x, y, z;
};

struct Vec2 {
    float x, y;
};

struct Vec4 {
    float x, y, z, w;
};
class Entity
{
public:
    char pad_0000[0x4]; //0x0000
    float x; //0x0004
    float y; //0x0008
    float z; //0x000C
    char pad_0010[24]; //0x0010
    float x_foot; //0x0028
    float y_foot; //0x002C
    float z_foot; //0x0030
    float Yaw; //0x0034
    float Pitch; //0x0038
    char pad_003C[58]; //0x003C
    int8_t SpecMode; //0x0076
    char pad_0077[117]; //0x0077
    int32_t Health; //0x00EC
    int32_t Armor; //0x00F0
    char pad_00F4[273]; //0x00F4
    char Name[16]; //0x0204
    char pad_0214[247]; //0x0214
    int32_t TeamNum; //0x030C
    char pad_0310[87]; //0x0310
    //class Weapon* CurrentWeapon; //0x0368
    char pad_036C[223]; //0x036C
}; //Size: 0x044C
int windowWidth = 1080;
int windowHeight = 1060;

// ViewMatrix 읽기 (중복 제거)
//uintptr_t matrixAddress = mem::FindAddress(moduleBase + 0x000FB12C, { 0x0 });
//const matrix4x4& matrix = *(matrix4x4*)matrixAddress;

bool WorldToScreen(Vec3 pos, Vec2& screen, int windowWidth, int windowHeight)
{
    Vec4 clipCoords = {};
    const auto& matrix_value = *(matrix4x4*)(moduleBase + offsets.MATRIX_OFFSET);
    const auto& matrix = matrix_value.value;

    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    if (clipCoords.w < 0.1f)
    {
        return false;
    }

    Vec3 TranslatedCoords;
    TranslatedCoords.x = clipCoords.x / clipCoords.w;
    TranslatedCoords.y = clipCoords.y / clipCoords.w;
    TranslatedCoords.z = clipCoords.z / clipCoords.w;

    screen.x = (windowWidth / 2 * TranslatedCoords.x) + (TranslatedCoords.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * TranslatedCoords.y) + (TranslatedCoords.y + windowHeight / 2);

    return true;
}
void RenderMain()
{
    ImGui::NewFrame();

    const auto& entity_array = *(uintptr_t*)(moduleBase + offsets.Entity_List);
    const auto& entity_count = *(int*)(moduleBase + offsets.PlayerCount);
    const auto& width = *(int*)(moduleBase + offsets.WIDTH);
    const auto& height = *(int*)(moduleBase + offsets.HEIGHT);

    const auto& draw = ImGui::GetBackgroundDrawList();
    Vec3 head_position;


    int health;
    for (int i = 1; i < entity_count; i++) {
        const uint64_t& pointer = *(uintptr_t*)(entity_array + 0x4 * i);
        Entity player = *(Entity*)(pointer);

        health = *(int*)(pointer + offsets.Health);

        head_position = *(Vec3*)(pointer + offsets.Head_Position_X);

        Vec2 vScreen;
        if (WorldToScreen(head_position, vScreen, width, height)) {

            draw->AddCircle(ImVec2(vScreen.x, vScreen.y), 5, ImColor(255, 0, 255), 0, 2);

        }


    }
    //char* Player_Name = (char*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Player_Name });
    //int Health = *(int*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Health });
    //int Armor = *(int*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Armor });
    //int Ammo = *(int*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Ammo });
    //float Camera_X = *(float*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Camera_X });
    //float Camera_Y = *(float*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Camera_Y });
    //float POS_X = *(float*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.POS_X });
    //float POS_Y = *(float*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.POS_Y });
    //float POS_Z = *(float*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.POS_Z });
    //float Head_Position_X = *(float*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Head_Position_X });
    //float Head_Position_Y = *(float*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Head_Position_Y });
    //float Head_Position_Z = *(float*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Head_Position_Z });

    //float E1_POS_X = *(float*)mem::FindAddress(moduleBase + offsets.E_Player, { 0x0, offsets.POS_X });
    //float E1_POS_Y = *(float*)mem::FindAddress(moduleBase + offsets.E_Player, { 0x0, offsets.POS_Y });
    //float E1_POS_Z = *(float*)mem::FindAddress(moduleBase + offsets.E_Player, { 0x0, offsets.POS_Z });

    //int Fast_Fire_Assault_Rifle = *(int*)mem::FindAddress(moduleBase + offsets.My_Base, { offsets.Fast_Fire_Assault_Rifle });

    // ImGui 윈도우 내부에 매트릭스를 출력하는 코드
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255)); // 텍스트 색상 설정
    ImGui::Begin("INFO"); // 윈도우 시작

    if (ImGui::CollapsingHeader("INFO")) {
        //ImGui::Text("Player_Name: %s", Player_Name);
        ImGui::Text("Health: %d", health);
        //ImGui::Text("Armor: %d", Armor);
        //ImGui::Text("Ammo: %d", Ammo);
        //ImGui::Text("Camera_X: %.2f", Camera_X);
        //ImGui::Text("Camera_Y: %.2f", Camera_Y);
        //ImGui::Text("POS_X: %.2f", POS_X);
        //ImGui::Text("POS_Y: %.2f", POS_Y);
        //ImGui::Text("POS_Z: %.2f", POS_Z);
        ImGui::Text("Head_POS_X: %.2f", head_position.x);
        ImGui::Text("Head_POS_Y: %.2f", head_position.y);
        ImGui::Text("Head_POS_Z: %.2f", head_position.z);
        ImGui::Text("PlayerCount: %d", entity_count);
        ImGui::Text("PlayerCount: %p", entity_array);
        ImGui::Text("WIDTH: %d, HEIGHT: %d", width, height);
    }

    ImGui::PopStyleColor(); // 텍스트 색상 원래대로 돌려놓기
    ImGui::End(); // 윈도우 종료

    // 현재 프레임의 모든 작업을 렌더링합니다.
    ImGui::Render();
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hMod);

        //AllocConsole();

        // 표준 입출력 스트림을 콘솔로 리다이렉트합니다.
        //FILE* fp;
        //freopen_s(&fp, "CONOUT$", "w", stdout);

        ImGuiHook::Load(RenderMain);
        break;
    case DLL_PROCESS_DETACH:
        ImGuiHook::Unload();
        break;
    }
    return TRUE;
}
