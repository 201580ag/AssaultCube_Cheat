#include "menu.h"

float fWindowScale;

void Menu::renderLine(std::string textString, float& xpos, float& ypos)
{
	//font size?
	font.Print(xpos, ypos, rgb::red, "%s", textString.c_str());
	ypos += textHeight + buff;
	glRasterPos2f(xpos, ypos);
}

void Menu::DrawMenu()
{
	std::vector <std::string> functions;
	std::string sHeader = "[GH]Rake - Dragnipur v2";
	std::string sTab1 = "Main Hacks";
	std::string sTab2 = "Misc Hacks";
	std::string sFooter = "GuidedHacking.com";
	if (iTab == 1)
	{
		functions.push_back("[F1] Aimbot    : " + sAimbotStatus);
		functions.push_back("[F2] ESP       : " + sESP);
		functions.push_back("[F3] No Recoil : " + sNoRecoil);
		functions.push_back("[F4] Map Hack  : " + sMapHack);
		functions.push_back("[F5] Scope Hack: " + sScopeHack);
		functions.push_back("[F6] FullBright: " + sFullbright);
		functions.push_back("[HOME] Change Tab");
		functions.push_back("[INS] Hide Menu");
	}
	if (iTab == 2)
	{
		functions.push_back("[F7]  Fly Hack : " + sFlyHackStatus);
		functions.push_back("[F8]  SetTeleporter");
		functions.push_back("[F9]  Teleport");
		functions.push_back("[F11] TeleFlag");
		functions.push_back(" ");//spacing
		functions.push_back("[HOME] Change Tab");
		functions.push_back("[INS] Hide Menu");
	}

	//Get Menu width
	std::string longest = functions[0];
	for (auto s : functions)
	{
		if (s.length() > longest.length())
		{
			longest = s;
		}
	}

	if (sHeader.length() >= longest.length())
	{
		menuWidth = (sHeader.length() * textWidth) + buffW;
	}
	else menuWidth = (longest.length() * textWidth) + buffW;

	GLfloat tmp_viewport[4];
	glGetFloatv(GL_VIEWPORT, tmp_viewport);

	MyRect header1;
	header1.tl = { float((tmp_viewport[2] * 0.65f) - (menuWidth / 2)), float(tmp_viewport[3] * .01) }; //WTF
	header1.tr = { float((tmp_viewport[2] * 0.65f) + menuWidth / 2), float(tmp_viewport[3] * .01) };
	header1.br = { float((tmp_viewport[2] * 0.65f) + menuWidth / 2), float(tmp_viewport[3] * .01 + header) };
	header1.bl = { float((tmp_viewport[2] * 0.65f) - menuWidth / 2), float(tmp_viewport[3] * .01 + header) };
	float middlex = header1.tl.x + ((header1.tr.x - header1.tl.x) / 2);
	float left = header1.tl.x;
	float right = header1.tr.x;

	glColor3ub(55, 55, 55);//gray
	drawFilledRect(header1);
	glColor3ub(0, 0, 0); //black
	drawOutline(header1);
	//draw text
	vec2 pos = centerTextXY(header1, sHeader);
	glColor3ub(255, 0, 0);
	glRasterPos2f(pos.x, pos.y);
	glDisable(GL_DEPTH_TEST);

	renderText(sHeader, pos.x, pos.y);

	glEnable(GL_DEPTH_TEST);

	MyRect tab1;//tab1
	tab1.tl = { left, header1.bl.y + outlineWidth };
	tab1.tr = { middlex - 1, header1.br.y + outlineWidth };
	tab1.br = { middlex - 1, header1.br.y + outlineWidth + tabHeight };
	tab1.bl = { left, header1.bl.y + outlineWidth + tabHeight };

	glColor3ub(55, 55, 55);//gray
	drawFilledRect(tab1);
	glColor3ub(0, 0, 0); //black
	drawOutline(tab1);
	//draw text
	pos = centerTextXY(tab1, sTab1); //using old rasterpos
	glColor3ub(255, 0, 0);
	glRasterPos2f(pos.x, pos.y);
	glDisable(GL_DEPTH_TEST);
	renderText(sTab1, pos.x, pos.y);
	glEnable(GL_DEPTH_TEST);

	MyRect tab2;
	tab2.tl = { middlex + 1, header1.bl.y + outlineWidth };
	tab2.tr = { right, header1.br.y + outlineWidth };
	tab2.br = { right, header1.br.y + outlineWidth + tabHeight };
	tab2.bl = { middlex + 1, header1.bl.y + outlineWidth + tabHeight };

	glColor3ub(55, 55, 55);//gray
	drawFilledRect(tab2);
	glColor3ub(0, 0, 0); //black
	drawOutline(tab2);
	//draw text // could make these all in one after boxes are drawn
	pos = centerTextXY(tab2, sTab2); //using old rasterpos
	glColor3ub(255, 0, 0);
	glRasterPos2f(pos.x, pos.y);
	glDisable(GL_DEPTH_TEST);
	renderText(sTab2, pos.x, pos.y);

	//draw Selected tab
	vec2 coverupleft, coverupright;

	if (iTab == 2)
	{
		coverupleft = { tab2.bl.x,				tab2.bl.y + 1.0f };
		coverupright = { tab2.br.x + 1.0f,		tab2.br.y + 1.0f };
	}
	else //tab1
	{
		coverupleft = { tab1.bl.x + 1.0f,			tab1.bl.y + 1.0f };
		coverupright = { tab1.br.x,					tab1.br.y + 1.0f };
	}
	vec2 lines[2] = { coverupleft, coverupright };

	initGLDraw();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, lines);//(coordinates per vertex, type, start, pointer)
	glColor3ub(55, 55, 55);//gray
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	MyRect body1;
	body1.tl = { left, tab1.bl.y + outlineWidth };
	body1.tr = { right, tab1.br.y + outlineWidth };
	body1.br = { right, tab1.br.y + outlineWidth + body };
	body1.bl = { left, tab1.bl.y + outlineWidth + body };

	glColor3ub(55, 55, 55);//gray
	drawFilledRect(body1);
	glColor3ub(0, 0, 0); //black
	drawOutline(body1);

	//Body Text writing section:
	float textrasterHeight = body1.tl.y + buff + textHeight;
	float textrasterWidth = centerTextX(body1, longest);

	glColor3ub(255, 0, 0);
	glRasterPos2f(textrasterWidth, textrasterHeight);

	glDisable(GL_DEPTH_TEST);
	for (auto string : functions)
	{
		renderLine(string, textrasterWidth, textrasterHeight); //15 font
	}

	glEnable(GL_DEPTH_TEST);

	MyRect footer1;
	footer1.tl = { left, body1.bl.y + outlineWidth };
	footer1.tr = { right, body1.br.y + outlineWidth };
	footer1.br = { right, body1.br.y + outlineWidth + footer };
	footer1.bl = { left, body1.bl.y + outlineWidth + footer };

	glColor3ub(55, 55, 55);//gray
	drawFilledRect(footer1);
	glColor3ub(0, 0, 0); //black
	drawOutline(footer1);

	//draw text
	pos = centerTextXY(footer1, sFooter); //using old rasterpos
	glColor3ub(255, 0, 0);
	glRasterPos2f(pos.x, pos.y);
	glDisable(GL_DEPTH_TEST);
	renderText(sFooter, pos.x, pos.y);
	glEnable(GL_DEPTH_TEST);

	glPopMatrix();
}

float centerTextX(MyRect box, std::string text)
{
	float boxWidth = box.tr.x - box.tl.x;
	float textWidth = text.length() * 9;
	float difference = boxWidth - textWidth;
	return (box.tl.x + (difference / 2));
}

vec2 centerTextXY(MyRect box, std::string text)
{
	vec2 textPOS;
	float boxWidth = box.tr.x - box.tl.x;
	float textWidth = text.length() * 9;
	float differenceX = boxWidth - textWidth;
	textPOS.x = box.tl.x + (differenceX / 2);

	float boxHeight = box.bl.y - box.tl.y;
	float textHeight = 15;
	float differenceY = boxHeight - textHeight;
	textPOS.y = box.tl.y + textHeight - (differenceY / 4);//why??
	return textPOS;
}