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
	SearchNode n;
	n.graphNodeID = start;
	n.currentPath.push_back(start);
	n.totalCost = 0;
	n.PQCost = 0;
	m_Q.push(n);
	m_curNodeVisiting = start;
	m_lastNodeVisited = start;
}

bool P2PAlgorithm::visitNode(SearchNode& cur, std::vector<VisitNode>& visited,
		int end) {

	if (std::find(visited.begin(), visited.end(), cur.graphNodeID) == visited.end()) {
		VisitNode vn;
		vn.nodeID = cur.graphNodeID;
		vn.path.insert(vn.path.end(), cur.currentPath.begin(), cur.currentPath.end());
		vn.totalCost = cur.totalCost;
		visited.push_back(vn);
		m_nodesVisited++;

		if (cur.graphNodeID == end) {
			m_ended = true;
			m_foundPath = true;

			m_shortestPath.insert(m_shortestPath.end(), cur.currentPath.begin(), cur.currentPath.end());
			m_shortestPathCost = cur.totalCost;
			return false;
		}

		return true;
	}

	return false;
}

void P2PAlgorithm::expandSearchFront(SearchNode& cur,
		std::vector<Edge>& edges, std::vector<VisitNode>& visited,
		std::priority_queue<SearchNode>& Q, int start, int end) {

	for (Edge& e : edges) {
		if (std::find(visited.begin(), visited.end(), e.to) == visited.end()) {
			SearchNode sn;
			sn.graphNodeID = e.to;
			sn.totalCost = cur.totalCost + e.weight;
			sn.PQCost = m_costFunc(sn.totalCost, m_graph->getNode(e.to),
					m_graph->getNode(start), m_graph->getNode(end));
			sn.currentPath.insert(sn.currentPath.end(), cur.currentPath.begin(), cur.currentPath.end());
			sn.currentPath.push_back(e.to);
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

	if (visitNode(cur, m_visited, m_end)) {
		Node n = m_graph->getNode(cur.graphNodeID);

		expandSearchFront(cur, n.edges, m_visited,
				m_Q, m_start, m_end);
	}
}

void P2PAlgorithm::end() {
	m_Q = std::priority_queue<SearchNode>();
	m_visited.clear();
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

