#include "CityEngine.hpp"

#include <iostream>

using namespace std;

CityEngine::CityEngine(EngineInterface *_renderer) : renderer(_renderer) {
	initValues();
	initMaps();

	net = make_shared<Net>(15817);

	speed = 1000.0f;
	counter = speed;
}

void CityEngine::update(float dt) {
	counter -= dt;

	if (counter <= 0)
		sValues["days"]++, counter = speed;
}

void CityEngine::initMaps() {
	map_ground	= make_shared<City::CityMap<City::Tile>>(13, 13);
	map_build	= make_shared<City::CityMap<City::Building>>(13, 13);

	for (int y = 0; y < map_ground->getH(); y++) {
		for (int x = 0; x < map_ground->getW(); x++) {
			setTile(x, y, "grass");
		}
	}
}

void CityEngine::setTile(int x, int y, string tile) {
	map_ground->set(x, y, std::make_shared<City::Tile>(tile));
	renderer->setTile(City::Coord<int>(x, y), tile, EngineInterface::Ground);
}

void CityEngine::initValues() {
	pop = 1;

	sValues["year"]		= 2017;
	sValues["days"]		= 0;
	sValues["balance"]	= 100'000;

	dValues["demand_res"] = 1;
}
