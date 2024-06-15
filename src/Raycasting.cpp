// Raycasting.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "2d.h"
#include "3d.h"

#define PI 3.141592653589793238462643383279502884197169399

int main()
{
    // create a window
    sf::RenderWindow window(sf::VideoMode(600, 600), "2d");
    sf::RenderWindow tDWindow(sf::VideoMode(600, 600), "3d");

    tDWindow.setMouseCursorVisible(false);

    static const int windowSize = window.getSize().x;
    static const int middle = windowSize / 2;
    
    static const int blockNumberWall = 10;
    static const int blocNumber = 2*blockNumberWall+1;
    static const float blockSize = float(windowSize) / float(blocNumber);

    static const float viewAngle = float(PI/(blockNumberWall/2));

    static const float distEcran = 10.;
    static const float hWall = float(windowSize);

    static const int playerSpeed = 50/blockNumberWall;

    int mouseX = 0;
    static const int mousePrec = 5;
    static const float sensibility = 0.5;
    static const float colPrec = blockSize/10;
    static const float pointerLength = float(windowSize / 30);

    static const sf::Color groundColor(195, 155, 119);
    static const sf::Color skyColor(135, 206, 235);
    static const sf::Color wallColor(144, 238, 144);
    static const sf::Color pointerColor(50, 50, 50);
    static const int numberLineBackground = 25;

    sf::Color mapInfo[2] = {groundColor, wallColor};

    std::vector<std::vector<int>> mapTab = generateMaze(blockNumberWall);
    std::vector<std::vector<sf::RectangleShape>> map = drawMap2D(mapTab, mapInfo, windowSize, blocNumber);

    sf::CircleShape player(colPrec);
    player.setFillColor(sf::Color::Red);
    player.setPosition(float(3*blockSize/2), float(3*blockSize/2));

    std::vector<sf::VertexArray> lineRaycast;
    std::vector<sf::VertexArray> tDLines;

    lineRaycast = getLines(player, windowSize, viewAngle, windowSize);

    std::vector<sf::RectangleShape> tDBackground = getBackground(windowSize, groundColor, skyColor, numberLineBackground);

    std::vector<sf::VertexArray> pointer = getPointer(windowSize, pointerLength, pointerColor);

    // main loop
    while (window.isOpen())
    {
        sf::Event event;
        sf::Event tDEvent;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseMoved)
            {
                float angle = mouseMoveConvert(event, windowSize, player, sensibility);
                player.setRotation(angle);

                changeAngleLines(&lineRaycast, player.getPosition().x, player.getPosition().y, angle, windowSize, viewAngle);
                checkIntersection(player, &lineRaycast, map, wallColor, windowSize, viewAngle, blocNumber);

                tDLines = raycast(&lineRaycast, angle/180*PI, windowSize, viewAngle, distEcran, hWall, wallColor);
            }
        }

        while (tDWindow.pollEvent(tDEvent)) {

            if (tDEvent.type == sf::Event::Closed) {
                tDWindow.close();
            }

            if (tDEvent.type == sf::Event::KeyPressed)
            {
                sf::Vector2f move;

                if (tDEvent.key.code == sf::Keyboard::D)
                {
                    double theta = player.getRotation();

                    float vectX = playerSpeed * cos(theta - PI / 2);
                    float vectY = -playerSpeed * sin(theta - PI / 2);

                    move = sf::Vector2f(vectX, vectY);
                    player.setPosition(player.getPosition().x + move.x, player.getPosition().y + move.y);

                    changePosLines(move, &lineRaycast, windowSize, viewAngle, theta);
                    checkIntersection(player, &lineRaycast, map, wallColor, windowSize, viewAngle, blocNumber);

                    tDLines = raycast(&lineRaycast, theta / 180 * PI, windowSize, viewAngle, distEcran, hWall, wallColor);
                }

                if (tDEvent.key.code == sf::Keyboard::Q)
                {
                    double theta = player.getRotation();

                    float vectX = playerSpeed * cos(theta + PI / 2);
                    float vectY = -playerSpeed * sin(theta + PI / 2);

                    move = sf::Vector2f(vectX, vectY);
                    player.setPosition(player.getPosition().x + move.x, player.getPosition().y + move.y);

                    changePosLines(move, &lineRaycast, windowSize, viewAngle, theta);
                    checkIntersection(player, &lineRaycast, map, wallColor, windowSize, viewAngle, blocNumber);

                    tDLines = raycast(&lineRaycast, theta / 180 * PI, windowSize, viewAngle, distEcran, hWall, wallColor);
                }

                if (tDEvent.key.code == sf::Keyboard::Z)
                {
                    double theta = player.getRotation();

                    float vectX = playerSpeed * cos(theta);
                    float vectY = -playerSpeed * sin(theta);

                    move = sf::Vector2f(vectX, vectY);
                    player.setPosition(player.getPosition().x + move.x, player.getPosition().y + move.y);

                    changePosLines(move, &lineRaycast, windowSize, viewAngle, theta);
                    checkIntersection(player, &lineRaycast, map, wallColor, windowSize, viewAngle, blocNumber);

                    tDLines = raycast(&lineRaycast, theta / 180 * PI, windowSize, viewAngle, distEcran, hWall, wallColor);
                }

                if (tDEvent.key.code == sf::Keyboard::S)
                {
                    double theta = player.getRotation();

                    float vectX = playerSpeed * cos(theta - PI);
                    float vectY = -playerSpeed * sin(theta - PI);

                    move = sf::Vector2f(vectX, vectY);
                    player.setPosition(player.getPosition().x + move.x, player.getPosition().y + move.y);

                    changePosLines(move, &lineRaycast, windowSize, viewAngle, theta);
                    checkIntersection(player, &lineRaycast, map, wallColor, windowSize, viewAngle, blocNumber);

                    tDLines = raycast(&lineRaycast, theta / 180 * PI, windowSize, viewAngle, distEcran, hWall, wallColor);
                }

                //If we move on a wall
                if (map[int((player.getPosition().y+colPrec) / blockSize)][int((player.getPosition().x+colPrec) / blockSize)].getFillColor() == wallColor ||
                    map[int((player.getPosition().y + colPrec) / blockSize)][int((player.getPosition().x -colPrec) / blockSize)].getFillColor() == wallColor || 
                    map[int((player.getPosition().y - colPrec) / blockSize)][int((player.getPosition().x + colPrec) / blockSize)].getFillColor() == wallColor || 
                    map[int((player.getPosition().y - colPrec) / blockSize)][int((player.getPosition().x - colPrec) / blockSize)].getFillColor() == wallColor) {

                    double theta = player.getRotation();

                    move = sf::Vector2f(-move.x, -move.y);
                    player.setPosition(player.getPosition().x + move.x, player.getPosition().y + move.y);

                    changePosLines(move, &lineRaycast, windowSize, viewAngle, theta);
                    checkIntersection(player, &lineRaycast, map, wallColor, windowSize, viewAngle, blocNumber);

                    tDLines = raycast(&lineRaycast, theta / 180 * PI, windowSize, viewAngle, distEcran, hWall, wallColor);
                }
            }

            if (tDEvent.type == sf::Event::MouseMoved)
            {
                if (tDEvent.mouseMove.x > windowSize - mousePrec) {
                    sf::Mouse::setPosition(sf::Vector2i(mousePrec*2, tDEvent.mouseMove.y), tDWindow);
                    mouseX = mousePrec*2;
                }

                if (tDEvent.mouseMove.x < mousePrec) {
                    sf::Mouse::setPosition(sf::Vector2i(windowSize - mousePrec*2 , tDEvent.mouseMove.y), tDWindow);
                    mouseX = windowSize - mousePrec*2;
                }

                float angle = mouseMove3D(tDEvent, mouseX, windowSize, sensibility)+player.getRotation();

                mouseX = tDEvent.mouseMove.x; 

                player.setRotation(angle);

                changeAngleLines(&lineRaycast, player.getPosition().x, player.getPosition().y, angle, windowSize, viewAngle);
                checkIntersection(player, &lineRaycast, map, wallColor, windowSize, viewAngle, blocNumber);

                tDLines = raycast(&lineRaycast, angle / 180 * PI, windowSize, viewAngle, distEcran, hWall, wallColor);
            }
        }

        window.clear();

        for (unsigned int i = 0; i < map.size(); i++)
        {
            for (unsigned int j = 0; j < map[i].size(); j++)
            {
                window.draw(map[i][j]);
            }
        }

        for (unsigned int i = 0; i < lineRaycast.size(); i++)
        {
            window.draw(lineRaycast[i]);
        }

        window.draw(player);

        window.display();

        tDWindow.clear();

        for (unsigned int i = 0; i < tDBackground.size(); i++)
        {
            tDWindow.draw(tDBackground[i]);
        }

        for (unsigned int i = 0; i < tDLines.size(); i++)
        {
            tDWindow.draw(tDLines[i]);
        }

        for (unsigned int i = 0; i < pointer.size(); i++) {
            tDWindow.draw(pointer[i]);
        }

        tDWindow.display();
    }

    return 0;
}
