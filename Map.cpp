#include "Map.h"

Map::Map() {
	grid.resize(ROWS);
	for (int y = 0; y < ROWS; ++y) {
		grid[y].reserve(COLS);
		for (int x = 0; x < COLS; ++x) {
			grid[y].emplace_back(y, x, amountOfExposure, 15, 5, 5, ignorant);
		}
	}
	grid[50][50] = Node(50, 50, proximity, 3, 30, 5, active);
}

void Map::initialize(int ticks) {
	sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Simulation");

	// Run simulation
	for (int i = 0; i < ticks && window.isOpen(); ++i) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}
		tick();
		window.clear();
		draw(window);
		window.display();
		sf::sleep(sf::milliseconds(100)); // 0.1 seconds per tick
	}

	// Show final result for a few seconds
	sf::sleep(sf::milliseconds(5000));
}

void Map::tick() {
	// Every tick, go through the whole graph. The active nodes spread, others don't
	for (int y = 0; y < ROWS; ++y) {
		for (int x = 0; x < COLS; ++x) {
			if (grid[y][x].state == active && grid[y][x].enoughEnergy())
				spread(&grid[y][x]);
		}
	}

	// Then, go through the whole graph, each nodes evaluates pending interactions
	for (int y = 0; y < ROWS; ++y) {
		for (int x = 0; x < COLS; ++x) {
			grid[y][x].goThroughInteractions(&activityCount, &awarenessCount);
		}
	}
}

void Map::spread(Node* node) {
	int minX = std::max(node->X - node->range, 0);
	int maxX = std::min(node->X + node->range, COLS - 1);
	int minY = std::max(node->Y - node->range, 0);
	int maxY = std::min(node->Y + node->range, ROWS - 1);

	int picked = 0;

	for (int y = minY; y <= maxY && picked < node->spreadAmount; ++y) {
		for (int x = minX; x <= maxX && picked < node->spreadAmount; ++x) {
			if (rand() % node->area < node->spreadAmount) {
				grid[y][x].addPendingInteraction(node);
				++picked;
			}
		}
	}
}

void Map::draw(sf::RenderWindow &window) {
	sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	for (int y = 0; y < ROWS; ++y) {
		for (int x = 0; x < COLS; ++x) {
			switch (grid[y][x].state) {
				case ignorant: cell.setFillColor(sf::Color::Cyan); break;
				case aware: cell.setFillColor(sf::Color::Blue); break;
				case active: cell.setFillColor(sf::Color::Green); break;
			}
			auto pos = gridToScreen(x, y, window.getSize().y);
			cell.setPosition(pos);
			cell.setOutlineThickness(1.0);
			window.draw(cell);
		}
	}
}

sf::Vector2f Map::gridToScreen(int gridX, int gridY, int windowHeight) {
	float screenX = gridX * CELL_SIZE;
	float screenY = windowHeight - (gridY + 1) * CELL_SIZE;
	return { screenX, screenY };
}
