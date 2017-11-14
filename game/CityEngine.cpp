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
	speed	 = 1.0f;
	bspeed	 = speed * 16.0f;
	counter  = speed;
	bcounter = bspeed;
	stopRoads = false;

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
		//expandRoads();
		//bspeed += bspeed;
		//bcounter = bspeed;
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

	expandRoads();

	updateRoadNetwork(roadNetwork->getRoot());
	newBuilding();
}

void CityEngine::expandRoads() {
	auto tpRoads = topRoads;
	topRoads.clear();

	for (auto road : tpRoads) {
		City::RoadNode parent = static_pointer_cast <City::RoadNetworkNode>(road->parent);
		
		Vec3D<int> dir(road->pos.x - parent->pos.x, 0, road->pos.y - parent->pos.y);
		auto adj = dir.cross(Vec3D<int>(0, 1, 0)).normalised() * (float)sect;
		auto lne = dir.normalised() * (float)sect;

		auto pos0 = road->pos + City::Coord<int>((int)lne.getX(), (int)lne.getZ());
		auto pos1 = road->pos + City::Coord<int>((int)adj.getX(), (int)adj.getZ());

		City::RoadNode eNode = roadNetwork->searchPosition(roadNetwork->getRoot(), pos1);
		
		if (inBounds(pos0.x, pos0.y)) {
			City::RoadNode n0 = roadNetwork->addRoad(road, pos0);
			topRoads.push_back(n0);
		}

		if (eNode == nullptr || (eNode != nullptr && (eNode->level == road->level - 1) || (eNode->level == road->level + 1))) {
			if (inBounds(pos1.x, pos1.y)) {
				City::RoadNode n1 = roadNetwork->addRoad(road, pos1);
				topRoads.push_back(n1);

				if (eNode != nullptr)
					roadNetwork->addNode(eNode, n1);
			}
		}
	}

	for(auto road : topRoads)
		updateRoadNetwork(road);

	roadlevel++;
}

void CityEngine::updateRoadNetwork(City::RoadNode node) {
	unsigned children = node->children.size();
	City::RoadNode parent = static_pointer_cast<City::RoadNetworkNode>(node->parent);
	
	//cout << node->n << ": lvl " << node->level << " (" << children << " children)\n";

	if (node->parent == nullptr) {
		if (inBounds(node->pos.x, node->pos.y)) {
			if (children > 0)
				setTile(node->pos.x, node->pos.y, "road_c", 1);
			else
				setTile(node->pos.x, node->pos.y, "pavement", 1);
		}
	}
	
	if (node->parent != nullptr) {
		if (inBounds(node->pos.x, node->pos.y)) {
			setTile(node->pos.x, node->pos.y, "road_c", 1);
			createRoadBetween(node, parent);
		} else
			stopRoads = true;
	}

	for (unsigned i = 0; i < node->children.size(); i++)
		updateRoadNetwork(dynamic_pointer_cast<City::RoadNetworkNode>(node->children[i]));
}

void CityEngine::clickTile(int x, int y) {
	City::Coord<int> coord = renderer->getIsoFromMouseXY(x, y);
	
	cout << coord.x << " " << coord.y << "\n";

	auto node = roadNetwork->searchPosition(roadNetwork->getRoot(), coord);
	
	if(node != nullptr)
		nodesToFind.push_back(node);

	if (nodesToFind.size() >= 2) {
		auto path = roadNetwork->breadthFirstSearch(nodesToFind[0], nodesToFind[1]);
		nodesToFind.clear();

		vector<City::Coord<int>> vertices;

		for (auto n : path)
			vertices.push_back(n->pos);

		if (!path.empty())
			renderer->drawPath(vertices);
	}
}

map<std::string, shared_ptr<long>> CityEngine::getValues() {
	map<std::string, shared_ptr<long>> vals;

	vals["Pop"] = pop;

	return vals;
}

void CityEngine::newBuilding() {
	int attempts = 0, maxattempts = 100;
	City::Coord<int> xy;
	bool found = false;

	std::uniform_int_distribution<> dist_radius(0, roadlevel * sect);

	while (attempts < maxattempts) {
		float search_radius = (float)dist_radius(rand_gen);
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
	
	if (found && inBounds(xy.x, xy.y)) {
		setTile(xy.x, xy.y, "building1", 1);
		*pop += 4;
	}
}

void CityEngine::createRoadBetween(City::RoadNode n1, City::RoadNode n2) {
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
	if (inBounds(x, y)) {
		if (layer == 0) {
			if (map_ground->get(x, y) != nullptr)
				return true;
		}
		else {
			if (map_build->get(x, y) != nullptr)
				return true;
		}
	}

	return false;
}

void CityEngine::initValues() {
	pop = make_shared<long>(0);
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