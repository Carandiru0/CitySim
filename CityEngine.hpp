#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>

#include "EngineInterface.hpp"

class CityEngine {
	public:
		CityEngine(std::shared_ptr<EngineInterface> renderer);

	private:
		std::shared_ptr<EngineInterface> renderer;

		std::map<std::string, double> sValues;
		std::map<std::string, double> dValues;

		std::vector<std::shared_ptr<City::CityMap>> maps;

		enum MapLayers { Ground, Build, Zones };

		void initMaps();
		void initValues();

	public:
		void action_highlightZone(City::Zone zone) { renderer->highlightZone(zone); };

	private:
		
};