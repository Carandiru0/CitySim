#include "SpriteHandler.hpp"

using namespace sf;
using namespace std;

SpriteHandler::SpriteHandler() : main_sheet(64, 32, 0, "data/tex/sheet1.png") {
	sprites["grass"]		= main_sheet.get(0, 0);
	sprites["pavement"]		= main_sheet.get(1, 0);
	sprites["building1"]	= main_sheet.get(2, 0, 2);
}

Sprite &SpriteHandler::create(string tex) {
	return sprites[tex];
}

SpriteHandler::SpriteSheet::SpriteSheet(unsigned w, unsigned h, unsigned s, string path) {
	sheet.loadFromFile(path);
	spr.setTexture(sheet);
	size = sheet.getSize();

	width = w, height = h, spacing = s;
}

Sprite SpriteHandler::SpriteSheet::get(unsigned r, unsigned c, unsigned h) {
	spr.setTextureRect(IntRect(c * width, r * height, width, height * h));
	spr.setOrigin(32.0f, 32.0f * (float)h - 16.0f);
	return spr;
}