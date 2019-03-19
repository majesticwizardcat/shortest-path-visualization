#pragma once

class SearchNode;
class Algorithm;

#include "graph.h"

typedef double (* costFunction_t)(double searchNodeCost, Node current, Node start, Node end);

#include <iostream>
#include <chrono>
#include <vector>
#include <SFML/Graphics.hpp>

class SearchNode {
public:
	int graphNodeID;
	double totalCost;
	double PQCost;
	std::vector<int> currentPath;

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
	std::chrono::seconds time;
	int nodesVisited;
	int pathNodes;

	double pathCost;

	void print() {
		std::cout << "Time: " << time.count() << " sec, "
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

