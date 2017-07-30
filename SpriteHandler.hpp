#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <string>

class SpriteHandler {
	public:
		SpriteHandler();
		
		class SpriteSheet {
			public:
				SpriteSheet(unsigned w, unsigned h, unsigned s, std::string path);

				sf::Sprite get(unsigned r, unsigned c, unsigned h = 1);

			private:
				sf::Texture sheet;
				sf::Sprite spr;
				sf::Vector2u size;

				unsigned width, height, spacing;
		};

		sf::Sprite &create(std::string tex);

	private:
		SpriteSheet main_sheet, gui_sheet;
		std::map<std::string, sf::Sprite> sprites;
};