#include "graph.h"
#include "renderer.h"
#include "algorithm.h"
#include "p2palgorithm.h"
#include "bdalgorithm.h"
#include "cost-functions.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>

int main(int args, char** argv) {
	if (args < 8) {
		std::cout << "Need 7 args" << std::endl;
		std::cout << "rendergraph [nodesfile] [edgesfile] "
			<< "[windowwidth] [windowheight] [nodespixelsize] " 
			<< " [startnodeid] [endnodeid]" << std::endl;
	}

	Graph g(argv[1], argv[2]);
	int windowWidth = atoi(argv[3]);
	int windowHeight = atoi(argv[4]);
	float nodePixels = atof(argv[5]);
	int start = atoi(argv[6]);
	int end = atoi(argv[7]);
	int numOfAlgs = 3;

	NodeColors dcolors(sf::Color(40, 50, 125), sf::Color(130, 145, 230), sf::Color(170, 180, 230));
	NodeColors ascolors(sf::Color(155, 100, 100), sf::Color(255, 150, 150), sf::Color(255, 200, 120));
	NodeColors bdcolors(sf::Color(120, 155, 20), sf::Color(220, 250, 120), sf::Color(250, 255, 150));

	Algorithm** algorithms = new Algorithm*[numOfAlgs];
	algorithms[0] = new P2PAlgorithm("A*", astar, ascolors);
	algorithms[1] = new P2PAlgorithm("Dijkstra", dijkstra, dcolors);
	algorithms[2] = new BDAlgorithm("BD-Dijkstra", dijkstra, bdcolors);

	Renderer r(windowWidth, windowHeight, nodePixels);
	r.render(g, algorithms, numOfAlgs, start, end);

	for (int i = 0; i < numOfAlgs; ++i) {
		delete algorithms[i];
	}

	delete[] algorithms;
}

