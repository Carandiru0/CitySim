#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>

#include "Net.hpp"
#include "EngineInterface.hpp"

class CityEngine {
	public:
		CityEngine(EngineInterface *renderer);

		void update(float dt);

	private:
		EngineInterface *renderer;
		std::shared_ptr<Net> net;

		long pop;

		std::map<std::string, double> sValues;
		std::map<std::string, double> dValues;

		std::shared_ptr<City::CityMap<City::Tile>> map_ground;
		std::shared_ptr<City::CityMap<City::Building>> map_build;

		enum MapLayers { Ground, Build, Zones };

		void initMaps();
		void initValues();

		float speed, counter;

		void setTile(int x, int y, std::string tile);

	public:
		

	private:
		
};