#include "SpriteHandler.hpp"

using namespace sf;
using namespace std;

SpriteHandler::SpriteHandler()
	: main_sheet(64, 32, 0, "data/tex/sheet1.png"),
	  gui_sheet(64, 64, 0, "data/tex/gui.png") {

	sprites["grass"]		= main_sheet.get(0, 0);
	sprites["pavement"]		= main_sheet.get(0, 1);
	sprites["zone_res"]		= main_sheet.get(1, 0);

	sprites["res1"]			= main_sheet.get(0, 2, 1);
	sprites["res2"]			= main_sheet.get(0, 3, 1);
	sprites["res3"]			= main_sheet.get(2, 2, 2);
	sprites["com1"]			= main_sheet.get(2, 3, 2);

	sprites["road_c"]		= main_sheet.get(1, 1, 1);
	sprites["road_v"]		= main_sheet.get(2, 0, 1);
	sprites["road_h"]		= main_sheet.get(2, 1, 1);
	sprites["road_t"]		= main_sheet.get(3, 0, 1);
	sprites["road_b"]		= main_sheet.get(3, 1, 1);

	sprites["car_e"]		= main_sheet.get(4, 0, 1);
	sprites["car_s"]		= main_sheet.get(4, 1, 1);
	sprites["car_n"]		= main_sheet.get(5, 0, 1);
	sprites["car_w"]		= main_sheet.get(5, 1, 1);

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