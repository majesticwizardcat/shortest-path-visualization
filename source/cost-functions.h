#pragma once

#include "graph.h"

double dijkstra(double searchNodeCost, Node current, Node start, Node end) {
	return searchNodeCost;
}

double astar(double searchNodeCost, Node current, Node start, Node end) {
	return searchNodeCost + current.realDistance(end);
}

double greedy(double searchNodeCost, Node current, Node start, Node end) {
	return current.realDistance(end);
}

