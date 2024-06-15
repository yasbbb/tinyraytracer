#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

std::vector< std::vector<sf::RectangleShape> > drawMap2D(std::vector<std::vector<int>> mapTab, sf::Color mapInfo[], int windowSize, int blocNumber);

void changeValueMaze(std::pair<int, int> beginCase, int n, std::vector<std::vector<int>>* maze);
std::vector<std::vector<int>> generateMaze(int blockNumber);