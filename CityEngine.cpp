#include "CityEngine.hpp"

using namespace std;

CityEngine::CityEngine(std::shared_ptr<EngineInterface> _renderer) {
	renderer = _renderer;

	initValues();
	initMaps();
}

void CityEngine::initMaps() {
	maps.push_back(make_shared<City::CityMap>(13, 13));
	maps.push_back(make_shared<City::CityMap>(13, 13));
	maps.push_back(make_shared<City::CityMap>(13, 13));

	for (int y = 0; y < maps[Ground]->getH(); y++)
		for (int x = 0; x < maps[Ground]->getW(); x++)
			maps[Ground]->set(x, y, "grass");
}

void CityEngine::initValues() {
	sValues["pop"]		= 0;
	sValues["year"]		= 2017;
	sValues["balance"]	= 100'000;
}