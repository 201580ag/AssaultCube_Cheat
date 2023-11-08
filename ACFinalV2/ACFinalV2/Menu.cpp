#include "Menu.h"

struct MenuConfig {
	bool menuActive = false;
	WNDPROC GameWindowProc = NULL;
}menuConf;

LRESULT CALLBACK hWindProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//pass message to imgui if menu active
	if (menuConf.menuActive) {
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
			return true;
		}
		ImGuiIO& io = ImGui::GetIO();

		switch (uMsg) {
		case WM_LBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[0];
			return 0;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[1];
			return 0;
		case WM_MBUTTONDOWN:
			io.MouseDown[2] = !io.MouseDown[2];
			return 0;
		case WM_MOUSEWHEEL:
			return 0;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			return 0;
		}


	}
	return CallWindowProc(menuConf.GameWindowProc, hWnd, uMsg, wParam, lParam);
}

Menu::Menu(int x, int y) {
	menuSize.x = x;
	menuSize.y = y;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	myEsp = new ESP();
	myAimbot = new AimBot();
	myCheats = new Others();
	myDebugger = new Debugger();
}
Menu::~Menu() {
	menuConf.menuActive = false;
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	SetWindowLongPtr(FindWindowA(NULL, "AssaultCube"), GWLP_WNDPROC, (LONG_PTR)menuConf.GameWindowProc);
	delete myEsp;
	delete myAimbot;
	delete myDebugger;
	delete myCheats;
}


void Menu::Init() {
	HWND hWnd = NULL;
	while (hWnd == NULL) {
		hWnd = FindWindowA(NULL, "AssaultCube");
	}
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplOpenGL2_Init();
	ImGuiStyle* myStyle = &ImGui::GetStyle();
	ImVec4* colors = myStyle->Colors;
	myStyle->WindowMinSize = ImVec2(300, 300);
	myStyle->WindowPadding = ImVec2(15, 15);
	myStyle->WindowRounding = 10.0f;
	myStyle->WindowPadding = ImVec2(6, 6);
	colors[ImGuiCol_WindowBg] = ImVec4(0.552f, 0.0f, 0.517f, 0.3f);
	colors[ImGuiCol_Border] = ImVec4(0, 0.921f, 0.694f, 1.0);
	colors[ImGuiCol_Text] = ImVec4(0.050f, 0.972f, 0.690f, 1.0f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.6f, 0.050f, 0.972f, 1.0);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.6f, 0.050f, 0.972f, 1.0f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.6f, 0.050f, 0.972f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.6f, 0.050f, 0.972f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.6f, 0.050f, 0.972f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.6f, 0.050f, 0.972f, 1.0f);
	colors[ImGuiCol_Tab] = ImVec4(1, 0.121f, 0.6f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(1, 0.121f, 0.6f, 0.56f);
	colors[ImGuiCol_TabActive] = ImVec4(1, 0.121f, 0.6f, 0.54f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(1, 0.121f, 0.6f, 1.00f);
	ImGui::SetNextWindowSize(menuSize, 0);
	menuConf.GameWindowProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)hWindProc);
}
void Menu::Render() {

	if (menuConf.menuActive) {
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("AC F1n4L v2 R1perXNX", &menuConf.menuActive);
		ImGui::BeginTabBar("tabbar");
		if (ImGui::BeginTabItem("PLAYER")) {
			ImGui::Checkbox("NO RECOIL", &myCheats->PCheatManager->noRecoil);
			ImGui::Checkbox("INF AMMO", &myCheats->PCheatManager->noAmmo);
			ImGui::Checkbox("RAPID FIRE", &myCheats->PCheatManager->rapidFire);
			ImGui::Checkbox("SPEEDHACK", &myCheats->PCheatManager->Speed);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("ESP")) {
			ImGui::Checkbox("ENABLE", &myEsp->PEspManager->bActive);
			ImGui::Checkbox("Draw Names", &myEsp->PEspManager->bDrawNames);
			ImGui::Checkbox("Draw Health", &myEsp->PEspManager->bDrawHealth);
			ImGui::Checkbox("Draw SnapLines", &myEsp->PEspManager->bDrawSnapLines);
			ImGui::Checkbox("Mark Visible", &myEsp->PEspManager->bMarkVisible);
			ImGui::Checkbox("Draw Healthbar", &myEsp->PEspManager->bDrawHealthBar);
			ImGui::Checkbox("Draw Distance", &myEsp->PEspManager->bDrawDistance);
			ImGui::Checkbox("Draw Weapon", &myEsp->PEspManager->bDrawWeapon);
			ImGui::SliderFloat("FOV", &myEsp->PEspManager->Fov, 10, 60);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("AIMBOT")) {
			ImGui::Checkbox("ENABLE", &myAimbot->PAimBotManager->bActive);
			ImGui::Checkbox("Smooth", &myAimbot->PAimBotManager->bSmooth);
			ImGui::SliderFloat("Smooth Percentage", &myAimbot->PAimBotManager->SmoothPercentage, 0, 1);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	}
}
void Menu::Toggle() {
	menuConf.menuActive = !menuConf.menuActive;
}
bool Menu::getStatus() {
	return menuConf.menuActive;
}
void Menu::DoCheat(Font myFont) {
	myEsp->Draw(myFont);
	myAimbot->Exec(myEsp->PEspManager->Fov);
	myCheats->RapidFire();
	myCheats->NoAmmo();
	myCheats->noRecoil();
	myCheats->SpeedHack();
	if (GetAsyncKeyState(VK_F6) & 1)
		myDebugger->Toggle();
}