#include "CityEngine.hpp"
#include "Vec3D.h"

#include <iostream>

using namespace std;

CityEngine::CityEngine(EngineInterface *_renderer) : renderer(_renderer) {
	net = make_shared<Net>(15817);
	
	center.x = 8, center.y = 8;

	roadNetwork = make_shared<City::RoadNetwork>(center);

	speed = 1000.0f;
	counter = speed;

	initValues();
	initMaps();
}

void CityEngine::update(float dt) {
	
}

void CityEngine::initMaps() {
	map_ground	= make_shared<City::CityMap<City::Tile>>(17, 17);
	map_build	= make_shared<City::CityMap<City::Tile>>(17, 17);

	for (int y = 0; y < map_ground->getH(); y++) {
		for (int x = 0; x < map_ground->getW(); x++) {
			setTile(x, y, "grass", 0);
		}
	}

	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(8, 6)));
	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(10, 8)));
	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(8, 10)));
	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(6, 8)));

	updateRoadNetwork(roadNetwork->getRoot());
}

void CityEngine::expandRoads() {
	auto tpRoads = topRoads;
	topRoads.clear();

	for (auto road : tpRoads) {
		Vec3D<int> dir(road->pos.x - road->parent->pos.x, 0, road->pos.y - road->parent->pos.y);
		auto adj = dir.cross(Vec3D<int>(0, 1, 0)).normalised() * 2;
		auto lne = dir.normalised() * 2;

		auto pos0 = road->pos + City::Coord<int>(lne.getX(), lne.getZ());
		auto pos1 = road->pos + City::Coord<int>(adj.getX(), adj.getZ());

		City::RoadNetwork::RoadNode n0 = roadNetwork->addRoad(road, pos0);
		City::RoadNetwork::RoadNode n1 = roadNetwork->addRoad(road, pos1);
		
		topRoads.push_back(n0);
		topRoads.push_back(n1);
	}

	updateRoadNetwork(roadNetwork->getRoot());
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
		setTile(node->pos.x, node->pos.y, "road_c", 1);
		createRoadBetween(node, node->parent);
	}

	for (unsigned i = 0; i < node->children.size(); i++)
		updateRoadNetwork(node->children[i]);
}

void CityEngine::createRoadBetween(City::RoadNetwork::RoadNode n1, City::RoadNetwork::RoadNode n2) {
	auto diff = n2->pos - n1->pos;
	int dist = sqrt(diff.x * diff.x + diff.y * diff.y);
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

void CityEngine::initValues() {
	pop = 1;
}
