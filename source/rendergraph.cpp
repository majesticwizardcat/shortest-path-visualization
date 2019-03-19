#include "graph.h"
#include "renderer.h"
#include "algorithm.h"
#include "p2palgorithm.h"
#include "bdalgorithm.h"
#include "cost-functions.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

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
	int start = atoi(argv[6]) - 1;
	int end = atoi(argv[7]) - 1;
	int numOfAlgs = 4;
	bool runFull = false;
	bool render = true;

	if (args > 8) {
		if(strcmp(argv[8], "-runfull") == 0) {
			runFull = true;
		}
	}

	if (args > 9) {
		if (strcmp(argv[9], "-norender") == 0) {
			render = false;
		}
	}

	NodeColors dcolors(sf::Color(40, 50, 125), sf::Color(130, 145, 230), sf::Color(170, 180, 230));
	NodeColors ascolors(sf::Color(155, 100, 100), sf::Color(255, 150, 150), sf::Color(255, 200, 120));
	NodeColors bdcolors(sf::Color(120, 155, 20), sf::Color(220, 250, 120), sf::Color(250, 255, 150));
	NodeColors bdastarcolors(sf::Color(120, 0, 100), sf::Color(220, 0, 220), sf::Color(220, 50, 220));

	Algorithm** algorithms = new Algorithm*[numOfAlgs];
	algorithms[0] = new P2PAlgorithm("Dijkstra", dijkstra, dcolors);
	algorithms[1] = new P2PAlgorithm("A*", astar, ascolors);
	algorithms[2] = new BDAlgorithm("BD-Dijkstra", dijkstra, bdcolors);
	algorithms[3] = new BDAlgorithm("BD-A*", astar, bdastarcolors);

	if (runFull) {
		for (int i = 0; i < numOfAlgs; ++i) {
			std::cout << "Running " << algorithms[i]->getName() << ": " << std::endl;
			RunStats stats = algorithms[i]->runFull(&g, start, end);
			stats.print();
		}
	}

	if (render) {
		Renderer r(windowWidth, windowHeight, nodePixels);
		r.render(g, algorithms, numOfAlgs, start, end);
	}

	for (int i = 0; i < numOfAlgs; ++i) {
		delete algorithms[i];
	}

	delete[] algorithms;
}

