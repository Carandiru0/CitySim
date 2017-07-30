#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "SpriteHandler.hpp"

class IsoEngine {
	public:
		IsoEngine(std::shared_ptr<SpriteHandler> sprHandler, std::shared_ptr<sf::RenderWindow> app);

		template <class T>
		struct Coord {
			T x, y;

			Coord() { x = T(0); y = T(0); }
			Coord(T _x, T _y) { x = _x, y = _y; }
			Coord(const Coord<T> &copy) { x = copy.x; y = copy.y; }

			void operator+=(Coord<T> n) { x += n.x; y += n.y; }
			Coord<T> operator+(Coord<T> n) { return Coord<T>(x + n.x, y + n.y); }
		};

		class IsoMap {
			public:
				IsoMap(int w, int h);

				struct TileDef {
					sf::Sprite spr;
				};
								
				struct Tile {
					std::string tile;

					Coord<float> iso;
					Coord<int> screen;
				};

				int getW() { return w; }
				int getH() { return h; }

				std::vector<std::vector<std::shared_ptr<Tile>>> data;

			private:
				int w, h;
		};

		void render();
		void setTile(Coord<int> position, std::string tile, unsigned layer = 0);

	private:
		std::shared_ptr<IsoMap> map_grnd, map_build;
		std::shared_ptr<SpriteHandler> sprHandler;
		std::shared_ptr<sf::RenderWindow> app;
		std::map<std::string, IsoMap::TileDef> tiles;

		Coord<float> offset;

		inline Coord<float> xy_iso(Coord<float> xy);
		inline Coord<float> iso_xy(Coord<float> iso);
		inline Coord<float> origin(std::shared_ptr<IsoMap> map);
};