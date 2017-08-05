#pragma once

#include <vector>
#include <memory>

#include "EngineInterface.hpp"

class CityEngine {
	public:
		CityEngine(std::shared_ptr<EngineInterface> renderer);

	private:
		std::shared_ptr<EngineInterface> renderer;

	public:
		void action_highlightZone(City::Zone zone) { renderer->highlightZone(zone); };
};