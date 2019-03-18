#include "bdalgorithm.h"

#include <SFML/Graphics.hpp>
#include <iostream>

void BDAlgorithm::startRunning(Graph* g, int start, int end) {
	P2PAlgorithm::startRunning(g, start, end);

	SearchNode n;
	n.graphNodeID = end;
	n.currentPath.push_back(end);
	n.totalCost = 0;
	n.PQCost = 0;
	m_endQ.push(n);
	m_endCurNodeVisiting = end;
	m_endLastNodeVisited = end;
}

void BDAlgorithm::step() {
	m_lastNodeVisited = m_curNodeVisiting;
	m_endLastNodeVisited = m_endCurNodeVisiting;
	
	if (m_Q.size() == 0 || m_endQ.size() == 0) {
		m_ended = true;
		m_foundPath = false;
		return;
	}

	if (m_Q.top().PQCost < m_endQ.top().PQCost) {
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

			std::vector<VisitNode>::iterator it = std::find(m_endVisited.begin(), m_endVisited.end(), cur.graphNodeID);

			if (it != m_endVisited.end()) {
				m_ended = true;
				m_foundPath = true;

				m_shortestPath.insert(m_shortestPath.end(), cur.currentPath.begin(), cur.currentPath.end());
				m_shortestPath.insert(m_shortestPath.end(), (*it).path.begin(), (*it).path.end());
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

	else {
		SearchNode endCur = m_endQ.top();
		m_endQ.pop();
		m_endCurNodeVisiting = endCur.graphNodeID;

		if (std::find(m_endVisited.begin(), m_endVisited.end(), endCur.graphNodeID) == m_endVisited.end()) {
			VisitNode endVn;
			endVn.nodeID = endCur.graphNodeID;
			endVn.path.insert(endVn.path.end(), endCur.currentPath.begin(), endCur.currentPath.end());
			m_endVisited.push_back(endVn);
			m_nodesVisited++;

			if (endCur.graphNodeID == m_start) {
				m_ended = true;
				m_foundPath = true;

				m_shortestPath.insert(m_shortestPath.end(), endCur.currentPath.begin(), endCur.currentPath.end());
				return;
			}

			std::vector<VisitNode>::iterator it = std::find(m_visited.begin(), m_visited.end(), endCur.graphNodeID);

			if (it != m_visited.end()) {
				m_ended = true;
				m_foundPath = true;

				m_shortestPath.insert(m_shortestPath.end(), endCur.currentPath.begin(), endCur.currentPath.end());
				m_shortestPath.insert(m_shortestPath.end(), (*it).path.begin(), (*it).path.end());
				return;
			}

			for (Edge& e : (m_graph->getNode(endCur.graphNodeID)).invertedEdges) {
				if (std::find(m_endVisited.begin(), m_endVisited.end(), e.to) == m_endVisited.end()) {
					SearchNode sn;
					sn.graphNodeID = e.to;
					sn.totalCost = endCur.totalCost + e.weight;
					sn.PQCost = m_costFunc(sn.totalCost, m_graph->getNode(e.to),
							m_graph->getNode(m_end), m_graph->getNode(m_start));
					sn.currentPath.insert(sn.currentPath.end(), endCur.currentPath.begin(), endCur.currentPath.end());
					sn.currentPath.push_back(e.to);
					m_endQ.push(sn);
				}
			}
		}
	}
}

void BDAlgorithm::end() {
	P2PAlgorithm::end();
}

void BDAlgorithm::drawLastVisited(sf::RenderWindow* window, float nodeSize) {
	P2PAlgorithm::drawLastVisited(window, nodeSize);

	if (m_endLastNodeVisited == m_start || m_endLastNodeVisited == m_end) {
		return;
	}

	Node last = m_graph->getNode(m_endLastNodeVisited);
	sf::CircleShape c(nodeSize);
	c.setFillColor(m_colors.visited);
	c.setPosition(last.x, last.y);
	window->draw(c);
}

void BDAlgorithm::drawCurrentVisiting(sf::RenderWindow* window, float nodeSize) {
	P2PAlgorithm::drawCurrentVisiting(window, nodeSize);

	if (m_endCurNodeVisiting == m_start || m_endCurNodeVisiting == m_end) {
		return;
	}

	Node last = m_graph->getNode(m_endCurNodeVisiting);
	sf::CircleShape c(nodeSize);
	c.setFillColor(m_colors.current);
	c.setPosition(last.x, last.y);
	window->draw(c);
}

