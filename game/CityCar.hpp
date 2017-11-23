#pragma once

#include <SFML\Graphics.hpp>

#include <list>
#include <vector>

#include "EngineInterface.hpp"
#include "Tree.hpp"
#include "SpriteHandler.hpp"
#include "Vec3D.h"

namespace City {
	class Car : public IsoObject {
		public:
			Car(RoadNode pos, std::vector<std::shared_ptr<Car>> &_ai, SpriteHandler &sprHandler, Coord<float> offset);

			void update(float dt);
			void setDest(std::shared_ptr<RoadNetwork> net, RoadNode dest);
			sf::Sprite &getSprite();

		private:
			sf::Sprite spr;
			RoadNode current;
			int index;

			std::vector<std::shared_ptr<Car>> &ai;
			std::vector<RoadNode> route;
	};
}