#pragma once

#include <string>

namespace City {
	template <class T> struct Coord {
		T x, y;

		Coord() { x = T(0); y = T(0); }
		Coord(T _x, T _y) { x = _x, y = _y; }
		Coord(const Coord<T> &copy) { x = copy.x; y = copy.y; }

		inline void operator+=(Coord<T> n) { x += n.x; y += n.y; }
		inline Coord<T> operator+(Coord<T> n) { return Coord<T>(x + n.x, y + n.y); }
	};

	struct Zone {
		Coord<int> begin, end;
		enum ZoneType { Residential, Commercial, Industrial } type;
	};

	class Tile {
		public:
			Tile() {}
			Tile(std::string _tile) { tile = _tile; }

			std::string getType() { return tile; }

		private:
			std::string tile;
	};

	class Building {
		public:
			double getTax(double tax) { return profit * tax; }

		protected:
			double profit;
	};

	class CommerceBuilding : public Building {
		public:
			CommerceBuilding() {}

		private:

	};

	class IndBuilding : public Building {
		public:
			IndBuilding() {}

		private:

	};

	class ResBuilding : public Building {
		public:
			ResBuilding() {}

		private:

	};

	template <class T> class CityMap {
		public:
			CityMap(int _w, int _h) {
				w = _w, h = _h;

				data.resize(h);

				for (int y = 0; y < h; y++)
					data[y].resize(w);
			}

			std::shared_ptr<T> get(int x, int y) { return data[y][x]; }
			void set(int x, int y, std::shared_ptr<T> b) { data[y][x] = b; }

			int getW() { return w; }
			int getH() { return h; }

			bool isEmpty() {
				for (int y = 0; y < h; y++) {
					for (int x = 0; x < w; x++) {
						if (data[y][x] != nullptr)
							return false;
					}
				}

				return true;
			}

		private:
			int w, h;

			std::vector<std::vector<std::shared_ptr<T>>> data;
	};
}

class EngineInterface {
	public:
		enum MapLayers { Ground, Build, Zones };

		virtual void setTile(City::Coord<int> position, std::string tile, int layer = Ground) = 0;
};