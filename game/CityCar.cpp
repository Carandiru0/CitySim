#include "CityCar.hpp"

using namespace std;

City::Car::Car(RoadNode pos, std::vector<std::shared_ptr<Car>>& _ai, SpriteHandler &sprHandler, Coord<float> offset)
	: IsoObject(pos->pos, offset), ai(_ai), current(pos)
{
	spr = sprHandler.create("car_n");
	calc_iso();
}

void City::Car::update(float dt) {
	if (current != nullptr && !route.empty()) {
		Vec3D<float> c_pos(pos.x, 0.0f, pos.y);
		Vec3D<float> to((float)current->pos.x, 0.0f, (float)current->pos.y);
		Vec3D<float> n_pos = c_pos.moveTowards(to, dt * 1.0f);

		move(City::Coord<float>(n_pos.getX(), n_pos.getZ()));

		if (n_pos.collision(c_pos, to)) {
			if(!route.empty())
				current = route.front(), route.pop_front();
		}
	}
}

void City::Car::setDest(std::shared_ptr<RoadNetwork> net, RoadNode dest) {
	route = net->pathfind_astar(current, dest);
	
	if (!route.empty())
		route.pop_front();
}

sf::Sprite &City::Car::getSprite() {
	spr.setPosition(iso.x, iso.y);
	return spr;
}
