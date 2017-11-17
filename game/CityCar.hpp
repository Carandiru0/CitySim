#pragma once

#include <queue>
#include <vector>

#include "EngineInterface.hpp"
#include "Tree.hpp"

namespace City {
	class Car : IsoObject {
		public:
			Car(Coord<float> pos, std::vector<std::shared_ptr<Car>> &_ai) : IsoObject(pos), ai(_ai) {}

			void update(float dt);

		private:
			std::vector<std::shared_ptr<Car>> &ai;
			std::queue<RoadNode> route;
	};
}