#pragma once

#include "graph.h"

cost_unit_t dijkstra(cost_unit_t searchNodeCost, Node current, Node start, Node end) {
	return searchNodeCost;
}

cost_unit_t astar(cost_unit_t searchNodeCost, Node current, Node start, Node end) {
	return searchNodeCost + current.realDistance(end);
}

cost_unit_t greedy(cost_unit_t searchNodeCost, Node current, Node start, Node end) {
	return current.realDistance(end);
}

