#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "SpriteHandler.hpp"
#include "EngineInterface.hpp"

class IsoEngine : public EngineInterface {
	public:
		IsoEngine(const SpriteHandler &sprHandler, std::shared_ptr<sf::RenderWindow> app);

		class IsoMap {
			public:
				IsoMap(int w, int h, bool a = true);

				struct TileDef {
					sf::Sprite spr;
				};
								
				struct Tile {
					std::string tile;

					City::Coord<float> iso;
					City::Coord<int> screen;
				};

				int getW() { return w; }
				int getH() { return h; }
				
				bool isActive() { return active; }
				void activate() { active = true; }
				void deactivate() { active = false; }

				std::vector<std::vector<std::shared_ptr<Tile>>> data;

			private:
				int w, h;
				bool active;
		};

		void render();
		City::Coord<int> getDimensions() { return dimensions; }

	private:
		std::vector<std::shared_ptr<IsoMap>> map_layers;
		SpriteHandler sprHandler;
		std::shared_ptr<sf::RenderWindow> app;
		std::map<std::string, IsoMap::TileDef> tiles;

		City::Coord<int> dimensions;
		City::Coord<float> offset;

		inline City::Coord<float> xy_iso(City::Coord<float> xy);
		inline City::Coord<float> iso_xy(City::Coord<float> iso);
		inline City::Coord<float> origin(std::shared_ptr<IsoMap> map);

	public:
		void setTile(City::Coord<int> position, std::string tile, int layer = Ground);
};