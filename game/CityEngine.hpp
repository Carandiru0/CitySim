#pragma once

#include <SFML\Graphics.hpp>

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <random>

#include "Net.hpp"
#include "EngineInterface.hpp"
#include "Tree.hpp"
#include "CityCar.hpp"
#include "Spritehandler.hpp"

class CityEngine {
	public:
		CityEngine(EngineInterface *renderer, SpriteHandler &sprhandler);

		void update(float dt);
		
	private:
		EngineInterface *renderer;
		SpriteHandler &sprHandler;
		std::shared_ptr<Net> net;
		std::mt19937 rand_gen;

		class BuildingScanner {
			public:
				BuildingScanner(std::mt19937 &gen) : rand_gen(gen), dist_angle(0, 360) {}
				int getAngle() { return dist_angle(rand_gen); }

			private:
				std::mt19937 &rand_gen;
				std::uniform_int_distribution<> dist_angle;
		};

		std::shared_ptr<BuildingScanner> scanner;

		bool stopRoads;
		int roadlevel, roadIterations;
		float speed, bspeed, counter, bcounter, upgradecounter;
		std::shared_ptr<long> pop;
		City::Coord<int> center, dims;

		enum MapLayers { Ground, Build, Zones };

		std::shared_ptr<City::CityMap<City::Tile>> map_ground;
		std::shared_ptr<City::CityMap<City::Tile>> map_build;
		std::shared_ptr<City::RoadNetwork> roadNetwork;

		std::map<City::Building::BuildingType, std::vector<std::string>> buildingTypes;
		std::map<City::Coord<int>, City::Building, City::CoordComp<int>> buildings;

		std::vector<City::RoadNode> topRoads;
		std::vector<City::RoadNode> nodesToFind;
		std::vector<std::shared_ptr<City::Car>> car_ai;

		void initBuildings();
		void initMaps();
		void initValues();

		City::RoadNode findClosestNode(City::Coord<int> pos, int &distance);
		City::Coord<int> findRandomWorkplace(City::Coord<int> loc, float radius);
		std::string getBuildingStr(City::Building::BuildingType type);
		void drawRoute(City::Coord<int> start, City::Coord<int> end);

		void setTile(int x, int y, std::string tile, int layer = 1);
		bool doesTileExist(int x, int y, int layer);

		bool inBounds(int x, int y);
		inline City::Coord<float> polar_to_xy(float angle, float r);

	public:
		void newBuilding(City::Building::BuildingType type);
		void expandRoads();
		void updateRoadNetwork(City::RoadNode node);
		void clickTile(int x, int y);

		void render(std::shared_ptr<sf::RenderWindow> app);
		std::map<std::string, std::shared_ptr<long>> getValues();

	private:
		
};