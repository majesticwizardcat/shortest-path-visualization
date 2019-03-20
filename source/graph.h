#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

typedef int cost_unit_t;

class Node;
class Edge;
class Graph;

class Node {
public:
	int id;
	int x;
	int y;
	int rx;
	int ry;
	std::vector<Edge> edges;
	std::vector<Edge> invertedEdges;
	
	Node() : id(-1) { }
	Node(int id, int x, int y) : id(id), x(x), y(y),
       		rx(x), ry(y) { }

	void remapLocation(int minX, int minY, int maxX, int maxY,
			int nMinX, int nMinY, int nMaxX, int nMaxY);

	bool equals(Node& other);
	bool operator==(Node& other);
	bool operator!=(Node& other);
	bool hasEdge(Edge& e);
	bool hasInvertedEdge(Edge& e);

	cost_unit_t distance(Node& other);
	cost_unit_t realDistance(Node& other);
	cost_unit_t realDistance2(Node& other);
	cost_unit_t manhattan(Node& other);
	cost_unit_t sphericalDistance(Node& other, int radius);
};

class Edge {
public:
	int to;
	cost_unit_t weight;

	Edge(int to, cost_unit_t weight) : to(to), weight(weight) { }
};

class Graph {
private:
	std::vector<Node> m_nodes;
	int m_minX;
	int m_maxX;
	int m_minY;
	int m_maxY;
	
	void loadGraph(char* nodesFileLocation, char* edgesFileLocation);
public:
	Graph(char* nodesFileLocation, char* edgesFileLocation);

	void save(const char* nodesLocation, const char* edgesLocation);
	void remapLocations(int nMinX, int nMinY, int nMaxX, int nMaxY);
	void drawNodes(sf::RenderWindow* window, float size);

	int getNodes() { return m_nodes.size(); }
	Node getNode(int index) { return m_nodes[index]; }
	Node* getNodePointer(int index) { return &m_nodes[index]; }
};

