#pragma once

#include <string>

namespace City {
	template <class T> struct Coord {
		T x, y;

		Coord() { x = T(0); y = T(0); }
		Coord(T _x, T _y) { x = _x, y = _y; }
		Coord(const Coord<T> &copy) { x = copy.x; y = copy.y; }

		inline void operator+=(Coord<T> n) { x += n.x; y += n.y; }
		inline void operator-=(Coord<T> n) { x -= n.x; y -= n.y; }
		inline Coord<T> operator+(Coord<T> n) { return Coord<T>(x + n.x, y + n.y); }
		inline Coord<T> operator-(Coord<T> n) { return Coord<T>(x - n.x, y - n.y); }

		inline std::string toString() { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }
	};

	template <class T> struct CoordComp {
		bool operator() (const Coord<T> &l, const Coord<T> &r) const {
			return std::make_pair(l.x, l.y) > std::make_pair(r.x, r.y);
		}
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

	class Building {
		public:
			enum BuildingType { Res, Ind, Com };

			Building() {
				type = Res, numWorkers = 0, maxWorkers = 20;
				workplace = Coord<int>(-1, -1);
			}

			Building(BuildingType _type) {
				type = _type, numWorkers = 0, maxWorkers = 20;
				workplace = Coord<int>(-1, -1);
			}

			int getWorkers() { return numWorkers; }
			int getMaxJobs() { return maxWorkers; }
			bool isFull() { return numWorkers >= maxWorkers; }
 			
			BuildingType getType() { return type; }

			Coord<int> getWorkplace() { return workplace; }
			void assignWorkplace(Coord<int> place) { workplace = place; }

		private:
			Coord<int> workplace;
			BuildingType type;
			int numWorkers, maxWorkers;
	};
}

class EngineInterface {
	public:
		enum MapLayers { Ground, Build, Zones };

		virtual City::Coord<int> getDimensions() = 0;
		virtual City::Coord<int> getIsoFromMouseXY(int x, int y) = 0;

		virtual void drawPath(std::vector<City::Coord<int>> vertices, int layer) = 0;
		virtual void setTile(City::Coord<int> position, std::string tile, int layer = Ground) = 0;
};