#include "graph.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>

void Node::remapLocation(int minX, int minY, int maxX, int maxY,
		int nMinX, int nMinY, int nMaxX, int nMaxY) {

	x = nMinX + (nMaxX - nMinX) * (float) ((float) (x - minX) / (float) (maxX - minX));
	y = (nMaxY) - (nMinY + (nMaxY - nMinY) * (float) ((float) (y - minY) / (float) (maxY - minY)));
}

bool Node::equals(Node& other) {
	return id == other.id;
}

bool Node::operator==(Node& other) {
	return id == other.id;
}

bool Node::operator!=(Node& other) {
	return id != other.id;
}

bool Node::hasEdge(Edge& e) {
	for (Edge& d : edges) {
		if (d.to == e.to) {
			return true;
		}
	}
	
	return false;
}

int Node::distance(Node& other) {
	return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
}

int Node::realDistance(Node& other) {
	return sqrt(pow(rx - other.rx, 2) + pow(ry - other.ry, 2));
}

Graph::Graph(char* nodesFileLocation, char* edgesFileLocation) {
	loadGraph(nodesFileLocation, edgesFileLocation);
}

void Graph::loadGraph(char* nodesFileLocation, char* edgesFileLocation) {
	std::fstream nodesFile(nodesFileLocation, std::ios::in);
	std::fstream edgesFile(edgesFileLocation, std::ios::in);

	int edges = 0;

	char buffer[2048];
	memset(&buffer, 0, sizeof(buffer));

	if (!nodesFile.is_open() || !edgesFile.is_open()) {
		std::cout << "Could not open one of the files" << std::endl;
		exit(-1);
	}

	m_maxX = 0;
	m_minX = 0;
	m_maxY = 0;
	m_minY = 0;

	while (!nodesFile.eof()) {
		memset(&buffer, 0, sizeof(buffer));
		nodesFile >> buffer;

		if (!strcmp(buffer, "v")) {
			int id, x, y;
			nodesFile >> id;
			nodesFile >> x;
			nodesFile >> y;

			if (x < m_minX || m_minX == 0) {
				m_minX = x;
			}
			else if (x > m_maxX || m_maxX == 0) {
				m_maxX = x;
			}

			if (y < m_minY || m_minY == 0) {
				m_minY = y;
			}
			else if (y > m_maxY || m_maxY == 0) {
				m_maxY = y;
			}
			
			m_nodes.push_back(Node((id - 1), x, y));
		}
	}

	std::cout << "Finished loading nodes" << std::endl;

	while (!edgesFile.eof()) {
		memset(&buffer, 0, sizeof(buffer));
		edgesFile >> buffer;

		if (!strcmp(buffer, "a")) {
			int f, t, w;
			edgesFile >> f;
			edgesFile >> t;
			edgesFile >> w;
			Node& from = m_nodes[f - 1];
			Edge e(t - 1, w);

			if (!from.hasEdge(e)) {
				from.edges.push_back(e);
				edges++;
			}
		}
	}

	nodesFile.close();
	edgesFile.close();

	std::cout << "Loaded " << m_nodes.size() << " nodes and " << edges << " edges" << std::endl;
}

void Graph::save(const char* nodesLocation, const char* edgesLocation) {
	std::fstream nodesFile(nodesLocation, std::ios::out);
	std::fstream edgesFile(edgesLocation, std::ios::out);

	for (Node& n : m_nodes) {
		nodesFile << "v " << (n.id + 1) << " " << n.rx << " " << n.ry << std::endl;
		
		for (Edge& e : n.edges) {
			edgesFile << "a " << (n.id + 1) << " " << (e.to + 1) << " " << e.weight << std::endl;
		}
	}

	nodesFile.close();
	edgesFile.close();
	std::cout << "Saved graph" << std::endl;
};

void Graph::remapLocations(int nMinX, int nMinY, int nMaxX, int nMaxY) {
	for (Node& n : m_nodes) {
		n.remapLocation(m_minX, m_minY, m_maxX, m_maxY,
			nMinX, nMinY, nMaxX, nMaxY);
	}
}

void Graph::drawNodes(sf::RenderWindow* window, float size) {
	sf::CircleShape c(size);
	c.setFillColor(sf::Color::White);

	for (Node& n : m_nodes) {
		c.setPosition(n.x, n.y);
		window->draw(c);
	}
}

