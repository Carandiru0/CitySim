#include "CityEngine.hpp"

#include <iostream>

using namespace std;

CityEngine::CityEngine(std::shared_ptr<EngineInterface> _renderer) {
	renderer = _renderer;

	initValues();
	initMaps();

	speed = 1000.0f;
	counter = speed;
}

void CityEngine::update(float dt) {
	counter -= dt;

	if (counter <= 0)
		sValues["days"]++, counter = speed;

	if (!map_zones->isEmpty()) {
		
	}
}

void CityEngine::initMaps() {
	map_ground	= make_shared<City::CityMap<City::Tile>>(13, 13);
	map_build	= make_shared<City::CityMap<City::Building>>(13, 13);
	map_zones	= make_shared<City::CityMap<City::Tile>>(13, 13);

	for (int y = 0; y < map_ground->getH(); y++)
		for (int x = 0; x < map_ground->getW(); x++)
			map_ground->set(x, y, std::make_shared<City::Tile>("grass"));
}

void CityEngine::initValues() {
	sValues["pop"]		= 0;
	sValues["year"]		= 2017;
	sValues["days"]		= 0;
	sValues["balance"]	= 100'000;

	dValues["demand_res"] = 1;
}

void CityEngine::action_highlightZone(City::Zone zone) {
	renderer->highlightZone(zone);
	
	for (int y = zone.begin.y; y < zone.end.y; y++)
		for (int x = zone.begin.x; x < zone.end.x; x++)
			map_zones->set(x, y, std::make_shared<City::Tile>("zone_res"));
}
