#pragma once
#include "structs.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Map {
private:
    static constexpr int ROWS = 100;
    static constexpr int COLS = 100;
    const int CELL_SIZE = 8;
    std::vector<std::vector<Node>> grid;
    int activityCount = 0;
    int awarenessCount = 0;

public:
    Map();
    void initialize(int ticks);
    void tick();
    void spread(Node* node);
    void draw(sf::RenderWindow& window);
    sf::Vector2f gridToScreen(int gridX, int gridY, int windowHeight);
};