#include "SpriteHandler.hpp"

using namespace sf;
using namespace std;

SpriteHandler::SpriteHandler()
	: main_sheet(64, 32, 0, "data/tex/sheet1.png"),
	  gui_sheet(64, 64, 0, "data/tex/gui.png") {

	sprites["grass"]		= main_sheet.get(0, 0);
	sprites["pavement"]		= main_sheet.get(0, 1);
	sprites["zone_res"]		= main_sheet.get(1, 0);

	sprites["building1"]	= main_sheet.get(0, 2, 1);
	sprites["building2"]	= main_sheet.get(0, 3, 1);
	sprites["building3"]	= main_sheet.get(2, 2, 2);

	sprites["btn_zone"]		= gui_sheet.get(0, 0);
	sprites["btn_zone_hov"] = gui_sheet.get(0, 1);
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
	spr.setTextureRect(IntRect(c * width, height * (r - h + 1), width, height * h));
	spr.setOrigin(32.0f, (32.0f * (float)h) - 16.0f);
	return spr;
}