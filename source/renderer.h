#pragma once

#include "graph.h"
#include "algorithm.h"

#include <SFML/Graphics.hpp>

class Renderer {
private:
	sf::RenderWindow* m_window;
	int m_width;
	int m_height;
	float m_nodeSize;

public:
	Renderer(int width, int height, float m_nodeSize);
	~Renderer();

	void render(Graph& graph, Algorithm** algorithms, int numOfAlgs, int startID, int endID);
};
