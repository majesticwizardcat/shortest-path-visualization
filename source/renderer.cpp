#include "renderer.h"
#include "algorithm.h"

#include <iostream>

Renderer::Renderer(int width, int height, float nodeSize) 
	: m_width(width), m_height(height), m_nodeSize(nodeSize) {

	m_window = new sf::RenderWindow(sf::VideoMode(width, height), "Rendergraph");
	m_window->setFramerateLimit(0);
}

Renderer::~Renderer() {
	delete m_window;
}

void Renderer::render(Graph& graph, Algorithm** algorithms, int numOfAlgs, int startID, int endID) {
	graph.remapLocations(10, 10, m_width - 10, m_height - 10);

	m_window->clear();
	graph.drawNodes(m_window, m_nodeSize);

	for (int i = 0; i < numOfAlgs; ++i) {
		algorithms[i]->startRunning(&graph, startID, endID);
	}

	sf::CircleShape c(m_nodeSize);

	Node start = graph.getNode(startID);
	Node end = graph.getNode(endID);

	c.setFillColor(sf::Color::Green);
	c.setPosition(start.x, start.y);
	m_window->draw(c);

	c.setFillColor(sf::Color::Red);
	c.setPosition(end.x, end.y);
	m_window->draw(c);

	while (m_window->isOpen()) {
		sf::Event event;	
		if (m_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_window->close();
			}
		}

		for (int i = 0; i < numOfAlgs; ++i) {
			if (!algorithms[i]->hasEnded()) {
				algorithms[i]->step();
				algorithms[i]->drawLastVisited(m_window, m_nodeSize);
				algorithms[i]->drawCurrentVisiting(m_window, m_nodeSize);

				if (algorithms[i]->hasEnded()) {
					if (algorithms[i]->foundPath()) {
						algorithms[i]->drawPath(m_window, m_nodeSize);
					}
					
					algorithms[i]->end();
				}
			}
		}

		c.setFillColor(sf::Color::Green);
		c.setPosition(start.x, start.y);
		m_window->draw(c);

		c.setFillColor(sf::Color::Red);
		c.setPosition(end.x, end.y);
		m_window->draw(c);

		m_window->display();

		std::cout << "\rNodes visited: ";
		for (int i = 0; i < numOfAlgs; ++i) {
			std::cout << "[" << algorithms[i]->getName() << " "
				<< algorithms[i]->getNodesVisited() << "] ";
		}
	}
}

