#pragma once

#include "graph.h"

int dijkstra(int searchNodeCost, Node current, Node start, Node end) {
	return searchNodeCost;
}

int astar(int searchNodeCost, Node current, Node start, Node end) {
	return searchNodeCost + current.realDistance(end);
}

int greedy(int searchNodeCost, Node current, Node start, Node end) {
	return current.realDistance(end);
}

