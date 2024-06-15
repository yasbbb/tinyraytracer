#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

std::vector<sf::VertexArray> getPointer(int windowSize, float length, sf::Color pointerColor);

std::vector<sf::VertexArray> getLines(sf::CircleShape player, int lineNumbers, float viewAngle, int windowSize);
std::vector<sf::RectangleShape> getBackground(int windowSize, sf::Color colorGround, sf::Color colorSky, int numberLine);

float mouseMoveConvert(sf::Event event, int sizeWindow, sf::CircleShape referentObject, float sensibility);
float mouseMove3D(sf::Event event, float oldMousePos, int sizeWindow, float sensibility);

void checkIntersection(sf::CircleShape referentObject, std::vector<sf::VertexArray>* lines, std::vector < std::vector<sf::RectangleShape> > map, sf::Color wallColor, int windowSize, double viewAngle, int blocNumber);
void changePosLines(sf::Vector2f moveVector, std::vector<sf::VertexArray>* lineRaycast, int windowSize, double viewAngle, float playerAngle);
void changeAngleLines(std::vector<sf::VertexArray>* lineRaycast, float playerX, float playerY, double angle, int windowSize, double viewAngle);
std::vector < sf::VertexArray> raycast(std::vector<sf::VertexArray>* lineRaycast, float playerAngle, int windowSize, double viewAngle, float distEcran, float hWall, sf::Color wallColor);
