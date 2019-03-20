#include "bdalgorithm.h"

#include <string.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>

void BDAlgorithm::startRunning(Graph* g, int start, int end) {
	P2PAlgorithm::startRunning(g, start, end);

	m_endVisited = new bool[g->getNodes()];
	memset(m_endVisited, false, sizeof(bool) * g->getNodes());
	m_endPredecessors = new int[g->getNodes()];
	memset(m_endPredecessors, -1, sizeof(int) * g->getNodes());
	m_endCosts = new cost_unit_t[g->getNodes()];
	memset(m_endCosts, 0, sizeof(cost_unit_t) * g->getNodes());
	SearchNode n;
	n.graphNodeID = end;
	n.totalCost = 0;
	m_endCosts[n.graphNodeID] = 0;
	n.PQCost = 0;
	m_endQ.push(n);
}

RunStats BDAlgorithm::runFull(Graph* g, int start, int end) {
	RunStats stats;

	startRunning(g, start, end);
	auto startTime = std::chrono::high_resolution_clock::now();

	while (!m_ended) {
		step();
	}

	auto stopTime = std::chrono::high_resolution_clock::now();

	stats.time = std::chrono::duration_cast<time_unit_t>(stopTime - startTime);
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

		if (visitNode(cur, m_visited, m_predecessors, m_costs, m_end)) {
			if (m_endVisited[cur.graphNodeID]) {
				m_ended = true;
				m_foundPath = true;

				std::vector<int> curPath = constructPath(m_predecessors, m_start, cur.predecessor);
				std::vector<int> other = constructPath(m_endPredecessors, m_end, cur.graphNodeID);
				m_shortestPath.insert(m_shortestPath.end(), curPath.begin(), curPath.end());
				m_shortestPath.insert(m_shortestPath.end(), other.begin(), other.end());

				m_shortestPathCost = m_costs[cur.graphNodeID] + m_endCosts[cur.graphNodeID];
				finalize();
				return;
			}

			Node n = m_graph->getNode(cur.graphNodeID);
			
			expandSearchFront(cur, n.edges, m_visited, m_costs, m_Q, m_start, m_end);
		}
	}
	
	else {
		cur = m_endQ.top();
		m_endQ.pop();

		if (visitNode(cur, m_endVisited, m_endPredecessors, m_endCosts, m_start)) {
			Node n = m_graph->getNode(cur.graphNodeID);

			if (m_visited[cur.graphNodeID]) {
				m_ended = true;
				m_foundPath = true;

				std::vector<int> curPath = constructPath(m_endPredecessors, m_end, cur.predecessor);
				std::vector<int> other = constructPath(m_predecessors, m_start, cur.graphNodeID);
				m_shortestPath.insert(m_shortestPath.end(), curPath.begin(), curPath.end());
				m_shortestPath.insert(m_shortestPath.end(), other.begin(), other.end());

				m_shortestPathCost = m_costs[cur.graphNodeID] + m_endCosts[cur.graphNodeID];

				finalize();
				return;
			}
			
			expandSearchFront(cur, n.invertedEdges, m_endVisited, m_endCosts, m_endQ, m_end, m_start);
		}
	}

	m_curNodeVisiting = cur.graphNodeID;
}

void BDAlgorithm::finalize() {
	SearchNode cur;

	while (!m_Q.empty()) {
		cur = m_Q.top();
		m_Q.pop();

		if (visitNode(cur, m_visited, m_predecessors, m_costs, m_end)) {
			if (m_endVisited[cur.graphNodeID]
					&& m_costs[cur.graphNodeID] + m_endCosts[cur.graphNodeID] < m_shortestPathCost) {

				m_shortestPath.clear();
				std::vector<int> curPath = constructPath(m_predecessors, m_start, cur.predecessor);
				std::vector<int> other = constructPath(m_endPredecessors, m_end, cur.graphNodeID);
				m_shortestPath.insert(m_shortestPath.end(), curPath.begin(), curPath.end());
				m_shortestPath.insert(m_shortestPath.end(), other.begin(), other.end());

				m_shortestPathCost = m_costs[cur.graphNodeID] + m_endCosts[cur.graphNodeID];
			}
		}
	}

	while (!m_endQ.empty()) {
		cur = m_endQ.top();
		m_endQ.pop();

		if (visitNode(cur, m_endVisited, m_endPredecessors, m_endCosts, m_start)) {
			Node n = m_graph->getNode(cur.graphNodeID);

			if (m_visited[cur.graphNodeID]
					&& m_costs[cur.graphNodeID] + m_endCosts[cur.graphNodeID] < m_shortestPathCost) {

				m_shortestPath.clear();
				std::vector<int> curPath = constructPath(m_endPredecessors, m_end, cur.predecessor);
				std::vector<int> other = constructPath(m_predecessors, m_start, cur.graphNodeID);
				m_shortestPath.insert(m_shortestPath.end(), curPath.begin(), curPath.end());
				m_shortestPath.insert(m_shortestPath.end(), other.begin(), other.end());

				m_shortestPathCost = m_costs[cur.graphNodeID] + m_endCosts[cur.graphNodeID];
			}
		}
	}
}

void BDAlgorithm::end() {
	P2PAlgorithm::end();
	m_endQ = std::priority_queue<SearchNode>();
	delete m_endVisited;
	delete m_endPredecessors;
	delete m_endCosts;
}

