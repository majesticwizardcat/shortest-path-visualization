#pragma once

class P2PAlgorithm;

#include "algorithm.h"

#include <vector>
#include <queue>

class VisitNode {
public:
	int nodeID;
	int totalCost;
	std::vector<int> path;

	bool operator==(VisitNode& other) {
		return nodeID == other.nodeID;
	}
	
	bool operator==(int id) {
		return nodeID == id;
	}
};

class P2PAlgorithm : public Algorithm {
private:
	char m_name[2048];

protected:
	std::priority_queue<SearchNode> m_Q;
	std::vector<int> m_shortestPath;
	std::vector<VisitNode> m_visited;
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

	bool visitNode(SearchNode& cur, std::vector<VisitNode>& visited, int end);

	void expandSearchFront(SearchNode& cur,
		std::vector<Edge>& edges, std::vector<VisitNode>& visited,
		std::priority_queue<SearchNode>& Q, int start, int end);
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

bool wasVisited(std::vector<VisitNode>& nodes, int id);

