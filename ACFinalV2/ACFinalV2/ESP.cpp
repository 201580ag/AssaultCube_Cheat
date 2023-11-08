#include "ESP.h"
#include "Geom.h"
 //CHANGE THIS 
float game_fov = 120 * PI / 180;
bool W2S(Vector3 pos, Vector3& screen, float matrix[16], int windowWidth, int windowHeight) {
    Vector4 clipCoords;
    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    //if entity is behoind u
    if (clipCoords.w < 0.1f)
        return false;
    Vector3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
    return true;
}


ESP::~ESP() {
    delete PEspManager;
}
bool ESP::IsTeamGame() {
    if (*gameMode == 0 || *gameMode == 4 || *gameMode == 5 || *gameMode == 7 || *gameMode == 11 || *gameMode == 13 ||
        *gameMode == 14 || *gameMode == 16 || *gameMode == 17 || *gameMode == 20 || *gameMode == 21)
        return true;
    else
        return false;
}
bool ESP::IsEnemy(ent* e) {
    if (localPlayer->team == e->team)
        return false;
    else
        return true;
}
bool ESP::IsValidEnt(ent* e) {
    if (e) {
        if (e->vTable == 0x4E4AC0 || e->vTable == 0x4E4A98) {
            return true;
        }

    }
    return false;
}
void ESP::Draw(Font& font) {
	if (PEspManager->bActive) {
		GL::DrawCircle(GL::getRes().x / 2, GL::getRes().y / 2, abs((tanf((PEspManager->Fov * PI / 180) / 2) / tanf(90 / 2)) * (1280 / 2)), 360, 1.0f, rgb::purple);
		const GLubyte* color = nullptr;
		glGetIntegerv(GL_VIEWPORT, viewport);
		for (int i = 0; i < *numOfPlayers; i++) {

			if (entlist != nullptr) {
				if (IsValidEnt(entlist->ents[i]) && entlist->ents[i]->health > 0) {
					if (IsTeamGame()) {
						if (IsEnemy(entlist->ents[i])) {
							if (!IsVisible(localPlayer, entlist->ents[i])) {
								color = rgb::red;
							}
							else {
								color = rgb::green;
							}
						}
						else
							continue;
					}
					else {
						if (!IsVisible(localPlayer, entlist->ents[i])) {
							color = rgb::red;
						}
						else {
							color = rgb::green;
						}
					}
					ent* e = entlist->ents[i];
					Vector3 headPos = e->head;
					Vector3 feetPos = e->pos;
					headPos.z += 0.5f; //add some space on top of the box;
					if (e->bCrouching) {
						headPos.z -= 0.5625;
					}

					Vector3 headScreenPos, feetScreenPos;
					if (W2S(headPos, headScreenPos, matrix, GL::getRes().x, GL::getRes().y) &&
						W2S(feetPos, feetScreenPos, matrix, GL::getRes().x, GL::getRes().y)) {
						float height = feetScreenPos.y - headScreenPos.y; //entity height
						float width = height / 2.0f; //get entity width

						Vector3 topLeft;
						topLeft.x = headScreenPos.x - width / 2.0f; // from head to left
						topLeft.y = headScreenPos.y;
						topLeft.z = 0;
						Vector3 bottomRight;
						bottomRight.x = headScreenPos.x + width / 2.0f;
						bottomRight.y = headScreenPos.y + height;
						bottomRight.z = 0;
						GL::DrawOutline(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 2.0f, color);
						if (PEspManager->bDrawHealth) {
							std::string tmp = std::to_string(e->health);
							const char* healthTXT = tmp.c_str();
							float textX = font.centerText((bottomRight.x - width), width, strlen(healthTXT) * ESP_FONT_WIDTH);
							float textY = bottomRight.y + ESP_FONT_HEIGHT;
							if (e->health > 75)
								font.Print(textX, textY, rgb::green, "%s:%s", "Health", healthTXT);
							if (e->health < 75 && e->health > 25)
								font.Print(textX, textY, rgb::yellow, "%s:%s", "Health", healthTXT);
							if (e->health < 25 && e->health > 0)
								font.Print(textX, textY, rgb::red, "%s:%s", "Health", healthTXT);
						}
						if (PEspManager->bDrawNames) {
							float text2X = font.centerText(topLeft.x, width, strlen(e->name) * ESP_FONT_WIDTH);
							float text2Y = topLeft.y - 0.5f;
							font.Print(text2X, text2Y, rgb::purple, "%s", e->name);
						}
						if (PEspManager->bDrawSnapLines) {
							GL::DrawLine(GL::getRes().x / 2, GL::getRes().y, feetScreenPos.x, feetScreenPos.y, 2.0f, rgb::purple);
						}
						//fix this
						if (PEspManager->bDrawDistance) {
							Vector3 distance = e->pos - localPlayer->pos;
							std::string tmp1 = std::to_string((int)(distance.Length() / 3.3f));
							const char* distanceTXT = tmp1.c_str();
							if (!PEspManager->bDrawHealth) {
								float text2X = font.centerText(topLeft.x, width, strlen(distanceTXT) * ESP_FONT_WIDTH);
								float text2Y = topLeft.y + height + ESP_FONT_HEIGHT;
								font.Print(text2X, text2Y, rgb::aquamarine, "%s%s", distanceTXT, "m");
							}
							else {
								float text2X = font.centerText(topLeft.x, width, strlen(distanceTXT) * ESP_FONT_WIDTH);
								float text2Y = topLeft.y + height + 2 * ESP_FONT_HEIGHT + 1.0f;
								font.Print(text2X, text2Y, rgb::aquamarine, "%s%s", distanceTXT, "m");
							}
						}
						if (PEspManager->bDrawHealthBar) {
							float x = width / 8.0f;
							if (x < 3.5)
								x = 3.5;
							Vector3 healthBottom;
							healthBottom.x = bottomRight.x + x;
							healthBottom.y = bottomRight.y;
							Vector3 healthTop;
							healthTop.x = bottomRight.x + x;
							healthTop.y = topLeft.y;
							float healthBarHeight = healthTop.y - healthBottom.y;
							float health = e->health * healthBarHeight / 100.0f;
							Vector3 healthA;
							healthA.y = healthBottom.y + health;

							if (e->health > 25 && e->health < 75)
								GL::DrawLine(healthBottom.x, healthBottom.y, healthTop.x, healthA.y, 5, rgb::yellow);
							if (e->health < 25)
								GL::DrawLine(healthBottom.x, healthBottom.y, healthTop.x, healthA.y, 5, rgb::red);
							if (e->health >= 75)
								GL::DrawLine(healthBottom.x, healthBottom.y, healthTop.x, healthA.y, 5, rgb::green);
						}
						if (PEspManager->bDrawWeapon) {
							float text3X = font.centerText(topLeft.x, width, strlen(e->name) * ESP_FONT_WIDTH);
							float text3Y = topLeft.y - 1.0f - ESP_FONT_HEIGHT;
							if (e->weapon->reloading)
								font.Print(text3X, text3Y, rgb::green, "%s", e->weapon->guninfo->N000002AA);
							else
								font.Print(text3X, text3Y, rgb::red, "%s", e->weapon->guninfo->N000002AA);
						}

					}
				}
			}
			else {
				entlist = *(entList**)0x50f4f8;
			}
		}
	}
}