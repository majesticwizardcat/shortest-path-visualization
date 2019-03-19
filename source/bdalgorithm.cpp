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
}

RunStats BDAlgorithm::runFull(Graph* g, int start, int end) {
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

	BDAlgorithm::end();
	reset();

	return stats;
}

void BDAlgorithm::step() {
	m_lastNodeVisited = m_curNodeVisiting;
	
	if (m_Q.size() == 0 || m_endQ.size() == 0) {
		m_ended = true;
		m_foundPath = false;
		return;
	}

	SearchNode cur;

	if (m_Q.top().PQCost < m_endQ.top().PQCost) {
		cur = m_Q.top();
		m_Q.pop();

		if (visitNode(cur, m_visited, m_end)) {
			Node n = m_graph->getNode(cur.graphNodeID);

			std::vector<VisitNode>::iterator it = std::find(m_endVisited.begin(), m_endVisited.end(), cur.graphNodeID);

			if (it != m_endVisited.end()) {
				m_ended = true;
				m_foundPath = true;

				m_shortestPath.insert(m_shortestPath.end(), cur.currentPath.begin(), cur.currentPath.end() - 1);
				m_shortestPath.insert(m_shortestPath.end(), (*it).path.begin(), (*it).path.end());
				m_shortestPathCost = cur.totalCost + (*it).totalCost;
				finalize();
				return;
			}
			
			expandSearchFront(cur, n.edges, m_visited, m_Q, m_start, m_end);
		}
	}
	
	else {
		cur = m_endQ.top();
		m_endQ.pop();

		if (visitNode(cur, m_endVisited, m_start)) {
			Node n = m_graph->getNode(cur.graphNodeID);

			std::vector<VisitNode>::iterator it = std::find(m_visited.begin(), m_visited.end(), cur.graphNodeID);

			if (it != m_visited.end()) {
				m_ended = true;
				m_foundPath = true;

				m_shortestPath.insert(m_shortestPath.end(), cur.currentPath.begin(), cur.currentPath.end() - 1);
				m_shortestPathCost = cur.totalCost + (*it).totalCost;
				m_shortestPath.insert(m_shortestPath.end(), (*it).path.begin(), (*it).path.end());
				finalize();
				return;
			}
			
			expandSearchFront(cur, n.invertedEdges, m_endVisited, m_endQ, m_end, m_start);
		}
	}

	m_curNodeVisiting = cur.graphNodeID;
}

void BDAlgorithm::finalize() {
	SearchNode cur;

	while (!m_Q.empty()) {
		cur = m_Q.top();
		m_Q.pop();

		if (visitNode(cur, m_visited, m_end)) {
			Node n = m_graph->getNode(cur.graphNodeID);

			std::vector<VisitNode>::iterator it = std::find(m_endVisited.begin(), m_endVisited.end(), cur.graphNodeID);

			if (it != m_endVisited.end() && cur.totalCost + (*it).totalCost < m_shortestPathCost) {
				m_shortestPath.clear();
				m_shortestPath.insert(m_shortestPath.end(), cur.currentPath.begin(), cur.currentPath.end() - 1);
				m_shortestPath.insert(m_shortestPath.end(), (*it).path.begin(), (*it).path.end());
				m_shortestPathCost = cur.totalCost + (*it).totalCost;
			}
		}
	}

	while (!m_endQ.empty()) {
		cur = m_endQ.top();
		m_endQ.pop();

		if (visitNode(cur, m_endVisited, m_start)) {
			Node n = m_graph->getNode(cur.graphNodeID);

			std::vector<VisitNode>::iterator it = std::find(m_visited.begin(), m_visited.end(), cur.graphNodeID);

			if (it != m_visited.end() && cur.totalCost + (*it).totalCost < m_shortestPathCost) {
				m_shortestPath.clear();
				m_shortestPath.insert(m_shortestPath.end(), cur.currentPath.begin(), cur.currentPath.end() - 1);
				m_shortestPathCost = cur.totalCost + (*it).totalCost;
				m_shortestPath.insert(m_shortestPath.end(), (*it).path.begin(), (*it).path.end());
			}
		}

	}
}

void BDAlgorithm::end() {
	P2PAlgorithm::end();
	m_endQ = std::priority_queue<SearchNode>();
	m_endVisited.clear();
}

