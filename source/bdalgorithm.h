#pragma once

#include "p2palgorithm.h"

class BDAlgorithm : public P2PAlgorithm {
protected:
	std::priority_queue<SearchNode> m_endQ;
	bool* m_endVisited;
	int* m_endPredecessors;
	double* m_endCosts;

	void finalize();

public:
	BDAlgorithm(const char* name, costFunction_t costFunc, NodeColors colors) 
		: P2PAlgorithm(name, costFunc, colors) { }

	RunStats runFull(Graph* g, int start, int end);

	void startRunning(Graph* g, int start, int end);
	void step();
	void end();
};
