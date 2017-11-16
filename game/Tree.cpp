#include "Tree.hpp"

#include <set>
#include <map>
#include <iostream>

using namespace City;

RoadNode City::RoadNetwork::addRoad(RoadNode node, RoadNetworkNode::Type type, Coord<int> pos) {
	RoadNode road = std::make_shared<RoadNetworkNode>(node, pos, ++counter, node->level + 1);
	road->type = type;
	node->children.push_back(road);
	return road;
}

RoadNode City::RoadNetwork::searchPosition(RoadNode node, Coord<int> pos) {
	if (node->pos.x == pos.x && node->pos.y == pos.y)
		return node;
	else {
		for (auto child : node->children) {
			RoadNode found;

			if ((found = searchPosition(std::static_pointer_cast<RoadNetworkNode>(child), pos)) != nullptr)
				return found;
		}
	}

	return nullptr;
}

std::list<RoadNode> City::RoadNetwork::breadthFirstSearch(RoadNode start, RoadNode end) {
	std::list<RoadNode> path;
	std::vector<RoadNode> open;
	std::set<RoadNode> closed;
	std::map<RoadNode, RoadNode> data;

	open.push_back(start);

	//std::cout << "Starting at (" << start->pos.x << ", " << start->pos.y << ")\n";
	//std::cout << "Finding (" << end->pos.x << ", " << end->pos.y << ")\n\n";

	bool goal_found = false;

	while (!open.empty()) {
		RoadNode current = open.front();
		open.erase(open.begin(), open.begin() + 1);

		if (current == end) {
			goal_found = true;
			break;
		}

		std::vector<RoadNode> next;

		if (current->parent != nullptr)
			next.push_back(std::dynamic_pointer_cast<RoadNetworkNode>(current->parent));

		for (auto child : current->children)
			next.push_back(std::dynamic_pointer_cast<RoadNetworkNode>(child));

		for (auto node : next) {
			if (closed.find(node) != closed.end())
				continue;

			bool found = false;

			for (auto i : open)
				if (i == node)
					found = true;

			if (!found) {
				data[node] = current;
				open.push_back(node);
			}
		}

		closed.insert(current);
	}

	if (goal_found) {
		RoadNode n = end;
		path.push_back(n);

		while (n = data[n])
			path.push_back(n);

		path.reverse();
	}

	/*for (auto i : path) {
		std::cout << "(" << i->pos.x << ", " << i->pos.y << ")\n";
	}*/

	return path;
}
