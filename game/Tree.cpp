#include "Tree.hpp"

using namespace City;

RoadNode City::RoadNetwork::addRoad(RoadNode node, Coord<int> pos) {
	RoadNode road = std::make_shared<RoadNetworkNode>(node, pos, ++counter, node->level + 1);
	node->children.push_back(road);
	return road;
}

RoadNode City::RoadNetwork::searchPosition(RoadNode node, Coord<int> pos) {
	if (node->pos.x == pos.x && node->pos.y == pos.y)
		return node;
	else {
		for (auto child : node->children) {
			RoadNode found;

			auto c = std::static_pointer_cast<RoadNetworkNode>(child);

			if ((found = searchPosition(c, pos)) != nullptr)
				return found;
		}
	}

	return nullptr;
}
