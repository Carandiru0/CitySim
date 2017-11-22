#include "CityEngine.hpp"
#include "Vec3D.h"

#include <array>
#include <iostream>
#include <chrono>

using namespace std;

CityEngine::CityEngine(EngineInterface *_renderer, SpriteHandler &_handler) : renderer(_renderer), sprHandler(_handler) {
	unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	rand_gen.seed(seed);

	dims = renderer->getDimensions();
	center.x = dims.x / 2, center.y = dims.y / 2;

	scanner		= make_shared<BuildingScanner>(rand_gen);
	net			= make_shared<Net>(15817);
	roadNetwork = make_shared<City::RoadNetwork>(center);

	stopRoads = false;
	speed	  = 1.0f;
	bspeed	  = 0.7f;
	counter   = speed;
	bcounter  = bspeed;
	upgradecounter = 8.f;
	roadIterations = 4;

	initBuildings();
	initValues();
	initMaps();
}

void CityEngine::update(float dt) {
	counter -= dt, upgradecounter -= dt;

	if (counter < 0.0f) {
		newBuilding(City::Building::Res);
		counter = speed;
	}

	if (!stopRoads && upgradecounter < 0.0f) {
		bcounter -= dt;

		if (roadIterations <= 0) {
			roadIterations = 3;
			upgradecounter = 8.f;
		} else if (bcounter < 0.0f) {
			expandRoads();
			bcounter = 0.7f;
			roadIterations--;
		}
	}

	for (auto car : car_ai)
		car->update(dt);
}

void CityEngine::initBuildings() {
	buildingTypes[City::Building::Res] = { "res1", "res2", "res3" };
	buildingTypes[City::Building::Com] = { "com1" };
}

void CityEngine::initMaps() {
	map_ground	= make_shared<City::CityMap<City::Tile>>(dims.x, dims.y);
	map_build	= make_shared<City::CityMap<City::Tile>>(dims.x, dims.y);

	for (int y = 0; y < map_ground->getH(); y++) {
		for (int x = 0; x < map_ground->getW(); x++) {
			setTile(x, y, "grass", 0);
		}
	}

	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::RoadNetworkNode::Horizontal, City::Coord<int>(center.x - 1, center.y)));
	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::RoadNetworkNode::Horizontal, City::Coord<int>(center.x + 1, center.y)));
	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::RoadNetworkNode::Vertical, City::Coord<int>(center.x, center.y - 1)));
	topRoads.push_back(roadNetwork->addRoad(roadNetwork->getRoot(), City::RoadNetworkNode::Vertical, City::Coord<int>(center.x, center.y + 1)));

	expandRoads();
	updateRoadNetwork(roadNetwork->getRoot());

	newBuilding(City::Building::Com);
}

void CityEngine::expandRoads() {
	auto tpRoads = topRoads;
	topRoads.clear();

	for (auto road : tpRoads) {
		City::RoadNode parent = static_pointer_cast<City::RoadNetworkNode>(road->parent);
		Vec3D<int> dir(road->pos.x - parent->pos.x, 0, road->pos.y - parent->pos.y);
		
		auto lne = dir.normalised();
		auto pos = road->pos + City::Coord<int>((int)lne.getX(), (int)lne.getZ());

		if (road->type != City::RoadNetworkNode::Cross) {
			int dist_crossroads = 0;
			auto node = road;

			do {
				++dist_crossroads;

				if (node->type == City::RoadNetworkNode::Cross)
					break;
			} while ((node = static_pointer_cast<City::RoadNetworkNode>(node->parent)) != nullptr);

			City::RoadNode eroad = roadNetwork->searchPosition(roadNetwork->getRoot(), pos);

			if (dist_crossroads < 3) {
				if (eroad != nullptr) {
					roadNetwork->addNode(eroad, road);
				} else {
					auto type = (dir.getX() == 0) ? City::RoadNetworkNode::Vertical : City::RoadNetworkNode::Horizontal;
					auto n = roadNetwork->addRoad(road, type, pos);
					topRoads.push_back(n);
				}
			} else {
				if (eroad != nullptr)
					roadNetwork->addNode(eroad, road);
				else {
					auto n = roadNetwork->addRoad(road, City::RoadNetworkNode::Cross, pos);
					topRoads.push_back(n);
				}
			}
		} else {
			auto adj_r = dir.cross(Vec3D<int>(0, 1, 0)).normalised();
			auto adj_l = dir.cross(Vec3D<int>(0, -1, 0)).normalised();

			auto pos_r = road->pos + City::Coord<int>((int)adj_r.getX(), (int)adj_r.getZ());
			auto pos_l = road->pos + City::Coord<int>((int)adj_l.getX(), (int)adj_l.getZ());

			auto type  = (pos_r.x - road->pos.x == 0) ? City::RoadNetworkNode::Vertical : City::RoadNetworkNode::Horizontal;
			auto ftyp  = (pos_r.x - road->pos.x == 0) ? City::RoadNetworkNode::Horizontal : City::RoadNetworkNode::Vertical;
			auto right = roadNetwork->addRoad(road, type, pos_r);
			auto left  = roadNetwork->addRoad(road, type, pos_l);
			auto forw  = roadNetwork->addRoad(road, ftyp, pos);

			topRoads.push_back(forw), topRoads.push_back(right), topRoads.push_back(left);
		}
		
		/*
		auto adj = dir.cross(Vec3D<int>(0, 1, 0)).normalised() * (float)sect;
		auto pos0 = road->pos + City::Coord<int>((int)lne.getX(), (int)lne.getZ());
		auto pos1 = road->pos + City::Coord<int>((int)adj.getX(), (int)adj.getZ());

		City::RoadNode eNode = roadNetwork->searchPosition(roadNetwork->getRoot(), pos1);*/
	}

	//for(auto road : topRoads)
	updateRoadNetwork(roadNetwork->getRoot());

	roadlevel++;
}

void CityEngine::updateRoadNetwork(City::RoadNode node) {
	unsigned children = node->children.size();
	City::RoadNode parent = static_pointer_cast<City::RoadNetworkNode>(node->parent);
	
	//cout << node->n << ": lvl " << node->level << " (" << children << " children)\n";

	if (node->parent == nullptr) {
		if (inBounds(node->pos.x, node->pos.y)) {
			 if (children > 1)
				setTile(node->pos.x, node->pos.y, "road_c", 1);
			else
				setTile(node->pos.x, node->pos.y, "pavement", 1);
		}
	} else {
		if (inBounds(node->pos.x, node->pos.y)) {
			switch (node->type) {
				case City::RoadNetworkNode::Horizontal: setTile(node->pos.x, node->pos.y, "road_h", 1); break;
				case City::RoadNetworkNode::Vertical: setTile(node->pos.x, node->pos.y, "road_v", 1); break;
				case City::RoadNetworkNode::Cross: setTile(node->pos.x, node->pos.y, "road_c", 1); break;
			}
		} else
			stopRoads = true;
	}

	for (unsigned i = 0; i < node->children.size(); i++)
		updateRoadNetwork(dynamic_pointer_cast<City::RoadNetworkNode>(node->children[i]));
}

void CityEngine::clickTile(int x, int y) {
	City::Coord<int> coord = renderer->getIsoFromMouseXY(x, y);

	if(buildings.find(coord) != buildings.end()) {
		City::Coord<int> place = buildings[coord].getWorkplace();

		if (place.x >= 0 && place.y >= 0) {
			int d;

			auto n0 = findClosestNode(coord, d);
			auto n1 = findClosestNode(place, d);

			auto path_a = roadNetwork->pathfind_bfs(n0, n1);
			auto path_b = roadNetwork->pathfind_astar(n0, n1);

			vector<City::Coord<int>> vert_a, vert_b;

			for (auto n : path_a)
				vert_a.push_back(n->pos);

			for (auto n : path_b)
				vert_b.push_back(n->pos);

			if (!path_a.empty())
				renderer->drawPath(vert_a, 0);

			if (!path_b.empty())
				renderer->drawPath(vert_b, 1);
		}
	}
	else {
		auto node = roadNetwork->searchPosition(roadNetwork->getRoot(), coord);

		if (node != nullptr) {
			cout << "Node " << node->pos.toString() << "\n";
			cout << "\t- " << node->children.size() << " children\n";
			cout << "\t- Type: " << ((node->type == 0) ? "Cross" : ((node->type == 1) ? "Vertical" : "Horizontal")) << "\n";
			cout << "\t- Level " << node->level << "\n\n";
		}
	}
}

void CityEngine::render(std::shared_ptr<sf::RenderWindow> app) {
	for (auto car : car_ai)
		app->draw(car->getSprite());
}

map<std::string, shared_ptr<long>> CityEngine::getValues() {
	map<std::string, shared_ptr<long>> vals;

	vals["Pop"] = pop;

	return vals;
}

void CityEngine::newBuilding(City::Building::BuildingType type) {
	int attempts = 0, maxattempts = 100;
	City::Coord<int> xy;
	bool found = false;

	int r = (roadlevel <= 2 ) ? roadlevel : roadlevel - 2;
	std::uniform_int_distribution<> dist_radius(0, r);

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
		setTile(xy.x, xy.y, getBuildingStr(type), 1);
		buildings[xy] = City::Building(type);
		*pop += 4;

		City::Coord<int> place = findRandomWorkplace(xy, 50.f);

		if (type == City::Building::Res && place.x >= 0 && place.y >= 0) {
			buildings[xy].assignWorkplace(place);

			int d;
			auto car_pos = findClosestNode(xy, d);
			auto ai = make_shared<City::Car>(car_pos, car_ai, sprHandler, renderer->getOffset());
			
			ai->setDest(roadNetwork, findClosestNode(place, d));
			car_ai.push_back(ai);
		}
	}
}

City::RoadNode CityEngine::findClosestNode(City::Coord<int> pos, int &distance) {
	float ldist = 1000.f;
	City::RoadNode node = roadNetwork->getRoot();

	for (int y = pos.y - 2; y <= pos.y + 2; ++y) {
		for (int x = pos.x - 2; x <= pos.x + 2; ++x) {
			City::Coord<int> p(x, y);

			City::RoadNode n = roadNetwork->searchPosition(roadNetwork->getRoot(), p);

			if (n != nullptr) {
				float dx = (float)(n->pos.x - pos.x), dy = (float)(n->pos.y - pos.y);
				float dist = sqrtf(dx * dx + dy * dy);

				if (dist < ldist)
					ldist = dist, node = n;
			}
		}
	}
	
	distance = (int)ldist;

	return node;
}

City::Coord<int> CityEngine::findRandomWorkplace(City::Coord<int> loc, float radius) {
	vector<City::Coord<int>> available;

	for (auto &place : buildings) {
		if (place.second.getType() == City::Building::Com && !place.second.isFull()) {
			float dx = (float)(loc.x - place.first.x), dy = (float)(loc.y - place.first.y);
			float dist = sqrtf(dx * dx + dy * dy);

			if (dist <= radius)
				available.push_back(place.first);
		}
	}

	if (available.size() > 0) {
		std::uniform_int_distribution<> dist(0, available.size() - 1);
		return available[dist(rand_gen)];
	}

	return City::Coord<int>(-1, -1);
}

std::string CityEngine::getBuildingStr(City::Building::BuildingType type) {
	int max = (int)buildingTypes[type].size();

	std::vector<int> d;

	for (int i = 0; i < max; i++)
		d.push_back(max - i);

	std::discrete_distribution<int> dist(d.begin(), d.end());

	return buildingTypes[type][dist(rand_gen)];
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