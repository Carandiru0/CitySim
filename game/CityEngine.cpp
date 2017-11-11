#include "CityEngine.hpp"

#include <iostream>

using namespace std;

CityEngine::CityEngine(EngineInterface *_renderer) : renderer(_renderer) {
	net = make_shared<Net>(15817);
	
	center.x = 6, center.y = 6;

	roadNetwork = make_shared<City::RoadNetwork>(center);

	speed = 1000.0f;
	counter = speed;

	initValues();
	initMaps();
}

void CityEngine::update(float dt) {
	
}

void CityEngine::initMaps() {
	map_ground	= make_shared<City::CityMap<City::Tile>>(13, 13);
	map_build	= make_shared<City::CityMap<City::Tile>>(13, 13);

	for (int y = 0; y < map_ground->getH(); y++) {
		for (int x = 0; x < map_ground->getW(); x++) {
			setTile(x, y, "grass", 0);
		}
	}

	City::RoadNetwork::RoadNode node0 = roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(6, 10));
	City::RoadNetwork::RoadNode node1 = roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(10, 6));
	City::RoadNetwork::RoadNode node2 = roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(2, 6));
	City::RoadNetwork::RoadNode node3 = roadNetwork->addRoad(roadNetwork->getRoot(), City::Coord<int>(6, 2));

	updateRoadNetwork(roadNetwork->getRoot(), nullptr);
}

void CityEngine::expandRoads() {

}

void CityEngine::updateRoadNetwork(City::RoadNetwork::RoadNode node, City::RoadNetwork::RoadNode prev) {
	unsigned children = node->children.size();
	cout << node->n << ": lvl " << node->level << " (" << children << " children)\n";

	if (prev == nullptr) {
		if(children > 0)
			setTile(node->pos.x, node->pos.y, "road_c", 1);
		else
			setTile(node->pos.x, node->pos.y, "pavement", 1);
	}
	
	if (prev != nullptr) {
		setTile(node->pos.x, node->pos.y, "road_c", 1);

		auto diff = prev->pos - node->pos;
		int dist  = sqrt(diff.x * diff.x + diff.y * diff.y);
		bool vert;
		int dir;

		if(diff.x == 0) {
			vert = true;
			dir = (diff.y < 0) ? 1 : -1;
		} else {
			vert = false;
			dir = (diff.x < 0) ? 1 : -1;
		}

		for (int i = 1; i < dist; i++) {
			if (vert)
				setTile(prev->pos.x, prev->pos.y + (i * dir), "road_v", 1);
			else
				setTile(prev->pos.x + (i * dir), prev->pos.y, "road_h", 1);
		}
	}

	for (unsigned i = 0; i < node->children.size(); i++)
		updateRoadNetwork(node->children[i], node);
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
