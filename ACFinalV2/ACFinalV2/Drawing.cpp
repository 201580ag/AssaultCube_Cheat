#include "Drawing.h"
void GL::SetupOrtho() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}
void GL::RestoreGL() {
	glPopMatrix();
	glPopAttrib();
}
Vector3 GL::getRes() {
	Vector3 screenRes;
	int viewport[4] = { 0 };
	glGetIntegerv(GL_VIEWPORT, viewport);

	screenRes.x = viewport[2];
	screenRes.y = viewport[3];
	screenRes.z = 0;
	return screenRes;
}
void GL::DrawLine(float fromX, float fromY, float toX, float toY, float lineWidth, const GLubyte color[3]) {
	glLineWidth(lineWidth);
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_LINES);
	glVertex2f(fromX, fromY);
	glVertex2f(toX, toY);
	glEnd();
}
void GL::DrawFilledRect(float x, float y, float width, float height, const GLubyte color[3]) {
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

void GL::DrawOutline(float x, float y, float x2, float y2, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x2, y);
	glVertex2f(x2, y2);
	glVertex2f(x, y2);
	glEnd();
}
void GL::DrawCircle(float x, float y, float radius, int numOfTriangles, float lineWidth, const GLubyte color[3])
{
	float doublePi = 2 * (FLOAT)PI;

	glLineWidth(lineWidth);
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= numOfTriangles; i++)
	{
		float theta = doublePi * i / numOfTriangles;
		float vx = cosf(theta) * radius + x;
		float vy = sinf(theta) * radius + y;

		glVertex2f(vx, vy);
	}

	glEnd();
}


void Font::Build(int height) {
	m_Hdc = wglGetCurrentDC();
	m_Base = glGenLists(96);
	HFONT hFont = CreateFontA(-height, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE || DEFAULT_PITCH, "Consolas");
	HFONT hOldFont = (HFONT)SelectObject(m_Hdc, hFont);
	wglUseFontBitmaps(m_Hdc, 32, 96, m_Base);
	SelectObject(m_Hdc, hOldFont);
	DeleteObject(hFont);

	m_bBuilt = true;
}
void Font::Print(float x, float y, const unsigned char color[3], const char* format, ...) {
	glColor3ub(color[0], color[1], color[2]);
	glRasterPos2f(x, y);

	char text[100];
	va_list args;

	va_start(args, format);
	vsprintf_s(text, 100, format, args);
	va_end(args);

	glPushAttrib(GL_LIST_BIT);
	glListBase(m_Base - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}
Vector3 Font::centerText(float x, float y, float width, float height, float textWidth, float textHeight) {
	Vector3 text;
	text.x = x + (width - textWidth) / 2;
	text.y = y + textHeight;

	return text;
}
float Font::centerText(float x, float width, float textWidth) {
	float difference{ 0 };
	if (width > textWidth)
	{
		difference = width - textWidth;
		return (x + (difference / 2));
	}
	else
	{
		difference = textWidth - width;
		return (x - (difference / 2));
	}
}