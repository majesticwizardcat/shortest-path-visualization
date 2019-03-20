#pragma once

class SearchNode;
class Algorithm;

#include "graph.h"

typedef cost_unit_t (* costFunction_t)(cost_unit_t searchNodeCost, Node current, Node start, Node end);

#include <iostream>
#include <chrono>
#include <vector>
#include <SFML/Graphics.hpp>

typedef std::chrono::milliseconds time_unit_t;

class SearchNode {
public:
	int graphNodeID;
	int predecessor;
	cost_unit_t totalCost;
	cost_unit_t PQCost;

	bool friend operator<(const SearchNode& n0, const SearchNode& n1) {
		return n0.PQCost > n1.PQCost;
	}
};

class NodeColors {
public:
	sf::Color visited;
	sf::Color current;
	sf::Color path;
	NodeColors() { }
	NodeColors(sf::Color visited, sf::Color current, sf::Color path) :
		visited(visited), current(current), path(path) { }
};

class RunStats {
public:
	time_unit_t time;
	int nodesVisited;
	int pathNodes;

	cost_unit_t pathCost;

	void print() {
		std::cout << "Time: " << time.count() << " ms, "
			<< "Nodes visited: " << nodesVisited << ", "
			<< "Path cost: " << pathCost << ", "
			<< "Path nodes: " << pathNodes << std::endl;
	}
};

class Algorithm {
public:
	virtual NodeColors getNodeColors() = 0;
	virtual std::vector<int>& getPathIDs() = 0;
	virtual const char* getName() = 0;
	virtual int getNodesVisited() = 0;
	virtual int getStart() = 0;
	virtual int getEnd() = 0;
	virtual bool hasEnded() = 0;
	virtual bool foundPath() = 0;
	virtual RunStats runFull(Graph* g, int start, int end) = 0;
	virtual void startRunning(Graph* g, int start, int end) = 0;
	virtual void step() = 0;
	virtual void end() = 0;
	virtual void reset() = 0;
	virtual void drawLastVisited(sf::RenderWindow* window, float nodeSize) = 0;
	virtual void drawCurrentVisiting(sf::RenderWindow* window, float nodeSize) = 0;
	virtual void drawPath(sf::RenderWindow* window, float nodeSize) = 0;
};

