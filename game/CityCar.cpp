#include "CityCar.hpp"

#include <iostream>

using namespace std;

City::Car::Car(RoadNode pos, std::vector<std::shared_ptr<Car>>& _ai, SpriteHandler &sprHandler, Coord<float> offset)
	: IsoObject(pos->pos, offset), ai(_ai), current(pos)
{
	index = 0;
	spr = sprHandler.create("car_n");
	calc_iso();
}

void City::Car::update(float dt) {
	if (current != nullptr) {
		Vec3D<float> c_pos(pos.x, 0.0f, pos.y);
		Vec3D<float> to((float)current->pos.x, 0.0f, (float)current->pos.y);
		Vec3D<float> n_pos = c_pos.moveTowards(to, dt * 2.0f);

		move(City::Coord<float>(n_pos.getX(), n_pos.getZ()));

		if (c_pos.distance(to) < 0.2f) {
			++index;

			if (index < route.size())
				current = route[index];
			else if(index >= route.size())
				iso = City::Coord<float>(-500.f, -500.f);
		}
	}
}

void City::Car::setDest(std::shared_ptr<RoadNetwork> net, RoadNode dest) {
	list<RoadNode> lroute = net->pathfind_astar(current, dest);
	
	for (auto &n : lroute)
		route.push_back(n);

	index = 0;
}

sf::Sprite &City::Car::getSprite() {
	spr.setPosition(iso.x, iso.y);
	return spr;
}
