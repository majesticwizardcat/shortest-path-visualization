#pragma once

class P2PAlgorithm;

#include "algorithm.h"

#include <vector>
#include <queue>

class P2PAlgorithm : public Algorithm {
private:
	char m_name[2048];

protected:
	std::priority_queue<SearchNode> m_Q;
	std::vector<int> m_shortestPath;
	int* m_predecessors;
	bool* m_visited;
	cost_unit_t* m_costs;
	int m_start;
	int m_end;
	int m_nodesVisited;
	int m_lastNodeVisited;
	int m_curNodeVisiting;
	int m_shortestPathCost;
	bool m_ended;
	bool m_foundPath;
	costFunction_t m_costFunc;
	Graph* m_graph;
	NodeColors m_colors;

	bool visitNode(SearchNode& cur, bool* visited, int* predecessors, 
			cost_unit_t* costs, int end);

	void expandSearchFront(SearchNode& cur,
		std::vector<Edge>& edges, bool* visited, cost_unit_t* costs,
		std::priority_queue<SearchNode>& Q, int start, int end);

	std::vector<int> constructPath(int* predecessors, int start, int end);

public:
	P2PAlgorithm(const char* name, costFunction_t costFunc, NodeColors colors);

	NodeColors getNodeColors() { return m_colors; }
	std::vector<int>& getPathIDs() { return m_shortestPath; }

	const char* getName() { return m_name; }

	int getNodesVisited() { return m_nodesVisited; }
	int getStart() { return m_start; }
	int getEnd() { return m_end; }

	bool hasEnded() { return m_ended; }
	bool foundPath() { return m_foundPath; }

	RunStats runFull(Graph* g, int start, int end);

	void startRunning(Graph* g, int start, int end);
	void step();
	void end();
	void reset();
	void drawLastVisited(sf::RenderWindow* window, float nodeSize);
	void drawCurrentVisiting(sf::RenderWindow* window, float nodeSize);
	void drawPath(sf::RenderWindow* window, float nodeSize);
};


