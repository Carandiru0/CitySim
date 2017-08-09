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

	class CityMap {
		public:
			CityMap(int _w, int _h) {
				w = _w, h = _h;

				data.resize(h);

				for (int y = 0; y < h; y++)
					data[y].resize(w);
			}

			std::string get(int x, int y) { return data[y][x]; }
			void set(int x, int y, std::string t) { data[y][x] = t; }

			int getW() { return w; }
			int getH() { return h; }

		private:
			int w, h;

			std::vector<std::vector<std::string>> data;
	};
}

class EngineInterface {
	public:
		virtual void highlightZone(City::Zone zone) = 0;
};