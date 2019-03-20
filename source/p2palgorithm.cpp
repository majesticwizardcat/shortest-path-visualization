#include "p2palgorithm.h"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <chrono>

P2PAlgorithm::P2PAlgorithm(const char* name, costFunction_t costFunc, NodeColors colors) : 
	m_costFunc(costFunc), m_colors(colors) {

	memset(&m_name, 0, sizeof(m_name));
	strcpy(m_name, name);
	m_ended = false;
	m_foundPath = false;
	m_nodesVisited = 0;
}

RunStats P2PAlgorithm::runFull(Graph* g, int start, int end) {
	RunStats stats;

	auto startTime = std::chrono::high_resolution_clock::now();
	startRunning(g, start, end);

	while (!m_ended) {
		step();
	}

	auto stopTime = std::chrono::high_resolution_clock::now();

	stats.time = std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime);
	stats.nodesVisited = m_nodesVisited;
	stats.pathCost = m_shortestPathCost;
	stats.pathNodes = m_shortestPath.size();

	P2PAlgorithm::end();
	reset();

	return stats;
}

void P2PAlgorithm::startRunning(Graph* g, int start, int end) {
	m_start = start;
	m_end = end;
	m_graph = g;

	m_predecessors = new int[g->getNodes()];
	memset(m_predecessors, -1, sizeof(int) * g->getNodes());
	m_costs = new double[g->getNodes()];
	memset(m_costs, 0, sizeof(double) * g->getNodes());
	m_visited = new bool[g->getNodes()];
	memset(m_visited, false, sizeof(bool) * g->getNodes());

	SearchNode n;
	n.graphNodeID = start;
	m_costs[n.graphNodeID] = 0;
	n.PQCost = 0;
	m_Q.push(n);
	m_curNodeVisiting = start;
	m_lastNodeVisited = start;
}

std::vector<int> P2PAlgorithm::constructPath(int* predecessors, int start, int end) {
	int cur = end;
	std::vector<int> path;
	while (cur != start) {
		path.push_back(cur);
		cur = predecessors[cur];
	}
	
	path.push_back(start);
	return path;
}

bool P2PAlgorithm::visitNode(SearchNode& cur, bool* visited,
		int* predecessors, double* costs, int end) {

	if (!visited[cur.graphNodeID]) {
		visited[cur.graphNodeID] = true;
		predecessors[cur.graphNodeID] = cur.predecessor;
		costs[cur.graphNodeID] = cur.totalCost;
		m_nodesVisited++;

		if (cur.graphNodeID == end) {
			m_ended = true;
			m_foundPath = true;

			m_shortestPath = constructPath(m_predecessors, m_start, m_end);
			m_shortestPathCost = costs[cur.graphNodeID];
			return false;
		}

		return true;
	}

	return false;
}

void P2PAlgorithm::expandSearchFront(SearchNode& cur,
		std::vector<Edge>& edges, bool* visited, double* costs,
		std::priority_queue<SearchNode>& Q, int start, int end) {

	for (Edge& e : edges) {
		if (!visited[e.to]) {
			SearchNode sn;
			sn.graphNodeID = e.to;
			sn.totalCost = costs[cur.graphNodeID] + e.weight;
			sn.PQCost = m_costFunc(sn.totalCost, m_graph->getNode(e.to),
					m_graph->getNode(start), m_graph->getNode(end));
			sn.predecessor = cur.graphNodeID;
			Q.push(sn);
		}
	}
}

void P2PAlgorithm::step() {
	m_lastNodeVisited = m_curNodeVisiting;
	
	if (m_Q.size() == 0) {
		m_ended = true;
		m_foundPath = false;
		return;
	}

	SearchNode cur = m_Q.top();
	m_Q.pop();
	m_curNodeVisiting = cur.graphNodeID;

	if (visitNode(cur, m_visited, m_predecessors, m_costs, m_end)) {
		Node n = m_graph->getNode(cur.graphNodeID);

		expandSearchFront(cur, n.edges, m_visited, m_costs,
				m_Q, m_start, m_end);
	}
}

void P2PAlgorithm::end() {
	m_Q = std::priority_queue<SearchNode>();
	delete m_predecessors;
	delete m_visited;
	delete m_costs;
}

void P2PAlgorithm::reset() {
	m_ended = false;
	m_foundPath = false;
	m_nodesVisited = 0;
	m_shortestPath.clear();
	m_shortestPathCost = 0;
}

void P2PAlgorithm::drawLastVisited(sf::RenderWindow* window, float nodeSize) {
	if (m_lastNodeVisited == m_start || m_lastNodeVisited == m_end) {
		return;
	}

	Node last = m_graph->getNode(m_lastNodeVisited);
	sf::CircleShape c(nodeSize);
	c.setFillColor(m_colors.visited);
	c.setPosition(last.x, last.y);
	window->draw(c);
}

void P2PAlgorithm::drawCurrentVisiting(sf::RenderWindow* window, float nodeSize) {
	if (m_curNodeVisiting == m_start || m_curNodeVisiting == m_end) {
		return;
	}

	Node last = m_graph->getNode(m_curNodeVisiting);
	sf::CircleShape c(nodeSize);
	c.setFillColor(m_colors.current);
	c.setPosition(last.x, last.y);
	window->draw(c);
}

void P2PAlgorithm::drawPath(sf::RenderWindow* window, float nodeSize) {
	sf::CircleShape c(nodeSize);

	for (int id : m_shortestPath) {
		Node n  = m_graph->getNode(id);
		c.setFillColor(m_colors.path);
		c.setPosition(n.x, n.y);
		window->draw(c);
	}
}

