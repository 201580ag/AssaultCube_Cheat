#pragma once
#include <Windows.h>
#pragma comment(lib, "OpenGL32.lib")
#include <gl\GL.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "geom.h"
#define PI 3.14159265358979323846 
namespace rgb {
	const GLubyte red[3] = { 255,0,0 };
	const GLubyte green[3] = { 0,255,0 };
	const GLubyte yellow[3] = { 247, 239, 2 };
	const GLubyte gray[3] = { 55,55,55 };
	const GLubyte lightgray[3] = { 192,192,192 };
	const GLubyte black[3] = { 0,0,0 };
	const GLubyte purple[3] = { 121, 2, 247 };
	const GLubyte aquamarine[3] = { 3, 255, 230 };
}

namespace GL {
	void SetupOrtho();
	void RestoreGL();
	Vector3 getRes();
	void DrawLine(float fromX, float fromY, float toX, float toY, float lineWidth, const GLubyte color[3]);
	void DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3]);
	void DrawOutline(float x, float y, float x2, float y2, float lineWidth, const GLubyte color[3]);
	void DrawCircle(float x, float y, float radius, int numOfTriangles, float lineWidth, const GLubyte color[3]);
}

class Font
{
public:
	bool m_bBuilt = false;
	unsigned int m_Base;
	HDC m_Hdc = nullptr;
	int m_Height;
	int m_Width;


	void Build(int height);
	void Print(float x, float y, const unsigned char color[3], const char* format, ...);
	Vector3 centerText(float x, float y, float width, float height, float textWidth, float textHeight);
	float centerText(float x, float width, float textWidth);
};
