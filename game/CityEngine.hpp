#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <random>

#include "Net.hpp"
#include "EngineInterface.hpp"
#include "Tree.hpp"

class CityEngine {
	public:
		CityEngine(EngineInterface *renderer);

		void update(float dt);

	private:
		EngineInterface *renderer;
		std::shared_ptr<Net> net;
		std::mt19937 rand_gen;

		class BuildingScanner {
			public:
				BuildingScanner(std::mt19937 &gen) : rand_gen(gen), dist_angle(0, 360) {}
				int getAngle() { return dist_angle(rand_gen); }

			private:
				std::mt19937 rand_gen;
				std::uniform_int_distribution<> dist_angle;
		};

		std::shared_ptr<BuildingScanner> scanner;

		bool stopRoads;
		std::shared_ptr<long> pop;
		int sect, roadlevel;
		float speed, bspeed, counter, bcounter;

		enum MapLayers { Ground, Build, Zones };

		std::shared_ptr<City::CityMap<City::Tile>> map_ground;
		std::shared_ptr<City::CityMap<City::Tile>> map_build;

		City::Coord<int> center, dims;

		std::map<City::Building::BuildingType, std::vector<std::string>> buildingTypes;
		std::map<City::Coord<int>, City::Building, City::CoordComp<int>> buildings;
		std::shared_ptr<City::RoadNetwork> roadNetwork;
		std::vector<City::RoadNode> topRoads;
		std::vector<City::RoadNode> nodesToFind;

		void initBuildings();
		void initMaps();
		void initValues();
		
		City::Coord<int> findRandomWorkplace(City::Coord<int> loc, float radius);
		std::string getBuildingStr(City::Building::BuildingType type);

		void createRoadBetween(City::RoadNode n1, City::RoadNode n2);
		void setTile(int x, int y, std::string tile, int layer = 1);
		bool doesTileExist(int x, int y, int layer);

		bool inBounds(int x, int y);
		inline City::Coord<float> polar_to_xy(float angle, float r);

	public:
		void newBuilding(City::Building::BuildingType type);
		void expandRoads();
		void updateRoadNetwork(City::RoadNode node);
		void clickTile(int x, int y);

		std::map<std::string, std::shared_ptr<long>> getValues();

	private:
		
};