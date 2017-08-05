#pragma once

namespace City {
	template <class T>
	struct Coord {
		T x, y;

		Coord() { x = T(0); y = T(0); }
		Coord(T _x, T _y) { x = _x, y = _y; }
		Coord(const Coord<T> &copy) { x = copy.x; y = copy.y; }

		void operator+=(Coord<T> n) { x += n.x; y += n.y; }
		Coord<T> operator+(Coord<T> n) { return Coord<T>(x + n.x, y + n.y); }
	};

	struct Zone {
		Coord<int> begin, end;
		enum ZoneType { Residential, Commercial, Industrial } type;
	};
}

class EngineInterface {
	public:
		virtual void highlightZone(City::Zone zone) = 0;
};