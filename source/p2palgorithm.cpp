#include "p2palgorithm.h"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <SFML/Graphics.hpp>

P2PAlgorithm::P2PAlgorithm(const char* name, costFunction_t costFunc, NodeColors colors) : 
	m_costFunc(costFunc), m_colors(colors) {

	memset(&m_name, 0, sizeof(m_name));
	strcpy(m_name, name);
	m_ended = false;
	m_foundPath = false;
	m_nodesVisited = 0;
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

	if (std::find(m_visited.begin(), m_visited.end(), cur.graphNodeID) == m_visited.end()) {
		VisitNode vn;
		vn.nodeID = cur.graphNodeID;
		vn.path.insert(vn.path.end(), cur.currentPath.begin(), cur.currentPath.end());
		m_visited.push_back(vn);
		m_nodesVisited++;

		if (cur.graphNodeID == m_end) {
			m_ended = true;
			m_foundPath = true;

			m_shortestPath.insert(m_shortestPath.end(), cur.currentPath.begin(), cur.currentPath.end());
			return;
		}

		for (Edge& e : (m_graph->getNode(cur.graphNodeID)).edges) {
			if (std::find(m_visited.begin(), m_visited.end(), e.to) == m_visited.end()) {
				SearchNode sn;
				sn.graphNodeID = e.to;
				sn.totalCost = cur.totalCost + e.weight;
				sn.PQCost = m_costFunc(sn.totalCost, m_graph->getNode(e.to),
						m_graph->getNode(m_start), m_graph->getNode(m_end));
				sn.currentPath.insert(sn.currentPath.end(), cur.currentPath.begin(), cur.currentPath.end());
				sn.currentPath.push_back(e.to);
				m_Q.push(sn);
			}
		}
	}
}

void P2PAlgorithm::end() {
	m_Q = std::priority_queue<SearchNode>();
	m_visited.clear();
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

