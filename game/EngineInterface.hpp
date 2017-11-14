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

	class RoadNetwork {
		public:
			struct Node {
				int n, level;
				Coord<int> pos;

				std::shared_ptr<Node> parent;
				std::vector<std::shared_ptr<Node>> children;

				Node(std::shared_ptr<Node> _parent, Coord<int> _pos, int _n, int _level) {
					pos = _pos; n = _n, level = _level, parent = _parent;
				}
			};

			typedef std::shared_ptr<Node> RoadNode;

			RoadNetwork(Coord<int> initial_pos) : counter(0) {
				root = std::make_shared<Node>(nullptr, initial_pos, 0, 0);
			}

			RoadNode addRoad(RoadNode node, Coord<int> pos) {
				RoadNode road = std::make_shared<Node>(node, pos, ++counter, node->level + 1);
				node->children.push_back(road);
				return road;
			}

			RoadNode searchPosition(RoadNode node, Coord<int> pos) {
				if (node->pos.x == pos.x && node->pos.y == pos.y)
					return node;
				else {
					for (auto child : node->children) {
						RoadNode found;

						if ((found = searchPosition(child, pos)) != nullptr)
							return found;
					}
				}

				return nullptr;
			}

			RoadNode getRoot() { return root; }

		private:
			int counter;
			RoadNode root;
	};

	class Building {
		public:
			Building() {}

			enum class BuildingType { Res, Ind, Com };

			BuildingType getType() { return type; }
			Coord<int> getPos() { return map_pos; }

		private:
			BuildingType type;
			Coord<int> map_pos;
	};
}

class EngineInterface {
	public:
		enum MapLayers { Ground, Build, Zones };

		virtual City::Coord<int> getDimensions() = 0;
		virtual void setTile(City::Coord<int> position, std::string tile, int layer = Ground) = 0;
};