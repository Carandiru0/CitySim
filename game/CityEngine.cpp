#include "CityEngine.hpp"
#include "Vec3D.h"

#include <iostream>
#include <chrono>

using namespace std;

CityEngine::CityEngine(EngineInterface *_renderer) : renderer(_renderer) {
	unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	rand_gen.seed(seed);

	dims = renderer->getDimensions();
	center.x = dims.x / 2, center.y = dims.y / 2;

	scanner		= make_shared<BuildingScanner>(rand_gen);
	net			= make_shared<Net>(15817);
	roadNetwork = make_shared<City::RoadNetwork>(center);

	sect	 = 3;
	speed	 = 7000.0f;
	bspeed	 = speed * 16.0f;
	counter  = speed;
	bcounter = speed;

	initValues();
	initMaps();
}

void CityEngine::update(float dt) {
	counter -= dt, bcounter -= dt;

	if (counter < 0.0f) {
		newBuilding();
		counter = speed;
	}

	if (!stopRoads && bcounter < 0.0f) {
		expandRoads();
		bspeed += bspeed;
		bcounter = bspeed;
	}
}

void CityEngine::initMaps() {
	map_ground	= make_shared<City::CityMap<City::Tile>>(dims.x, dims.y);
	map_build	= make_shared<City::CityMap<City::Tile>>(dims.x, dims.y);

	for (int y = 0; y < map_ground->getH(); y++) {
		for (int x = 0; x < map_ground->getW(); x++) {
			setTile(x, y, "grass", 0);
		}
	}

	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(center.x - sect, center.y)));
	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(center.x + sect, center.y)));
	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(center.x, center.y - sect)));
	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(center.x, center.y + sect)));

	updateRoadNetwork(roadNetwork->getRoot());
	newBuilding();
}

void CityEngine::expandRoads() {
	auto tpRoads = topRoads;
	topRoads.clear();

	for (auto road : tpRoads) {
		Vec3D<int> dir(road->pos.x - road->parent->pos.x, 0, road->pos.y - road->parent->pos.y);
		auto adj = dir.cross(Vec3D<int>(0, 1, 0)).normalised() * (float)sect;
		auto lne = dir.normalised() * (float)sect;

		auto pos0 = road->pos + City::Coord<int>((int)lne.getX(), (int)lne.getZ());
		auto pos1 = road->pos + City::Coord<int>((int)adj.getX(), (int)adj.getZ());

		City::RoadNetwork::RoadNode n0 = roadNetwork->addRoad(road, pos0);
		City::RoadNetwork::RoadNode n1 = roadNetwork->addRoad(road, pos1);
		
		topRoads.push_back(n0);
		topRoads.push_back(n1);
	}

	for(auto road : topRoads)
		updateRoadNetwork(road);

	roadlevel++;
}

void CityEngine::updateRoadNetwork(City::RoadNetwork::RoadNode node) {
	unsigned children = node->children.size();
	cout << node->n << ": lvl " << node->level << " (" << children << " children)\n";

	if (node->parent == nullptr) {
		if(children > 0)
			setTile(node->pos.x, node->pos.y, "road_c", 1);
		else
			setTile(node->pos.x, node->pos.y, "pavement", 1);
	}
	
	if (node->parent != nullptr) {
		if (inBounds(node->pos.x, node->pos.y)) {
			setTile(node->pos.x, node->pos.y, "road_c", 1);
			createRoadBetween(node, node->parent);
		} else
			stopRoads = true;
	}

	for (unsigned i = 0; i < node->children.size(); i++)
		updateRoadNetwork(node->children[i]);
}

void CityEngine::newBuilding() {
	int attempts = 0, maxattempts = 100;
	City::Coord<int> xy;
	bool found = false;

	std::uniform_int_distribution<> dist_radius(0, (float)roadlevel * sect);

	while (attempts < maxattempts) {
		float search_radius = dist_radius(rand_gen);
		float angle = (float)scanner->getAngle();
		
		City::Coord<float> c = polar_to_xy(angle * ((float)M_PI / 180.0f), search_radius);
		xy.x = (int)c.x;
		xy.y = (int)c.y;

		xy += center;

		if (doesTileExist(xy.x, xy.y, 1))
			++attempts;
		else {
			found = true;
			break;
		}
	}
	
	if(found)
		setTile(xy.x, xy.y, "building1", 1);
}

void CityEngine::createRoadBetween(City::RoadNetwork::RoadNode n1, City::RoadNetwork::RoadNode n2) {
	auto diff = n2->pos - n1->pos;
	int dist = (int)sqrt(diff.x * diff.x + diff.y * diff.y);
	bool vert;
	int dir;

	if (diff.x == 0) {
		vert = true;
		dir = (diff.y < 0) ? 1 : -1;
	} else {
		vert = false;
		dir = (diff.x < 0) ? 1 : -1;
	}

	for (int i = 1; i < dist; i++) {
		if (vert)
			setTile(n2->pos.x, n2->pos.y + (i * dir), "road_v", 1);
		else
			setTile(n2->pos.x + (i * dir), n2->pos.y, "road_h", 1);
	}
}

void CityEngine::setTile(int x, int y, string tile, int layer) {
	if(layer == 0)
		map_ground->set(x, y, std::make_shared<City::Tile>(tile));
	else
		map_build->set(x, y, std::make_shared<City::Tile>(tile));

	renderer->setTile(City::Coord<int>(x, y), tile, layer);
}

bool CityEngine::doesTileExist(int x, int y, int layer) {
	if (layer == 0) {
		if (map_ground->get(x, y) != nullptr)
			return true;
	} else {
		if (map_build->get(x, y) != nullptr)
			return true;
	}

	return false;
}

void CityEngine::initValues() {
	stopRoads = false;
	pop = 1;
	roadlevel = 1;
}

bool CityEngine::inBounds(int x, int y) {
	if (x >= 0 && y >= 0 && x < dims.x && y < dims.y)
		return true;

	return false;
}

City::Coord<float> CityEngine::polar_to_xy(float angle, float r) {
	return City::Coord<float>(r * cosf(angle), r * sinf(angle));
}