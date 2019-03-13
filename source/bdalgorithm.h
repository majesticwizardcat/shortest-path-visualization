#pragma once

#include "p2palgorithm.h"

class BDAlgorithm : public P2PAlgorithm {
protected:
	std::priority_queue<SearchNode> m_endQ;
	std::vector<VisitNode> m_endVisited;
	int m_endLastNodeVisited;
	int m_endCurNodeVisiting;

public:
	BDAlgorithm(const char* name, costFunction_t costFunc, NodeColors colors) 
		: P2PAlgorithm(name, costFunc, colors) { }

	void startRunning(Graph* g, int start, int end);
	void step();
	void end();
	void drawLastVisited(sf::RenderWindow* window, float nodeSize);
	void drawCurrentVisiting(sf::RenderWindow* window, float nodeSize);
};
