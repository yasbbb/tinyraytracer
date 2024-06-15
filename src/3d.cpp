#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#define PI 3.141592653589793238462643383279502884197169399

#include "3d.h"

std::vector<sf::VertexArray> getPointer(int windowSize, float length, sf::Color pointerColor) {
	std::vector<sf::VertexArray> pointer;

	sf::VertexArray vertLine(sf::Lines, 2);
	sf::VertexArray horLine(sf::Lines, 2);

	vertLine[0].position = sf::Vector2f(float(windowSize / 2), float(windowSize / 2 - length / 2));
	vertLine[1].position = sf::Vector2f(float(windowSize / 2), float(windowSize / 2 + length / 2));

	horLine[0].position = sf::Vector2f(float(windowSize / 2 - length / 2), float(windowSize / 2));
	horLine[1].position = sf::Vector2f(float(windowSize / 2 + length / 2), float(windowSize / 2));

	vertLine[0].color = pointerColor;
	vertLine[1].color = pointerColor;

	horLine[0].color = pointerColor;
	horLine[1].color = pointerColor;

	pointer.push_back(vertLine);
	pointer.push_back(horLine);

	return(pointer);
}

std::vector<sf::VertexArray> getLines(sf::CircleShape player, int lineNumbers, float viewAngle, int windowSize) {
	std::vector<sf::VertexArray> lineVector;

	sf::VertexArray line(sf::Lines, 2);

	float rangeAngle = 2 * viewAngle;
	float diagWindow = float(sqrt(2 * windowSize * windowSize));

	for (int i = 0; i < lineNumbers; i++) {
		float theta = -viewAngle + i * (rangeAngle / lineNumbers);

		line[0].position = sf::Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
		line[1].position = sf::Vector2f(player.getPosition().x + diagWindow * cos(theta), player.getPosition().y + diagWindow * sin(theta));

		line[0].color = sf::Color(252, 209, 42, 128);
		line[1].color = sf::Color(252, 209, 42, 128);

		lineVector.push_back(line);
	}

	return(lineVector);
}

std::vector<sf::RectangleShape> getBackground(int windowSize, sf::Color colorGround, sf::Color colorSky, int numberLine) {
	std::vector<sf::RectangleShape> background;

	float middle = float(windowSize * 0.5);
	float lineSize = middle / numberLine;

	for (unsigned int i = 0; i * lineSize < middle; i++) {
		sf::RectangleShape lineGround(sf::Vector2f(float(windowSize), lineSize));
		sf::RectangleShape lineSky(sf::Vector2f(float(windowSize), lineSize));

		lineGround.setPosition(0, windowSize - lineSize * (i + 1));
		lineSky.setPosition(0, lineSize * i);

		double darkCoef = pow(0.975, i);

		lineGround.setFillColor(sf::Color(uint8_t(colorGround.r * darkCoef), uint8_t(colorGround.g * darkCoef), uint8_t(colorGround.b * darkCoef)));
		lineSky.setFillColor(sf::Color(uint8_t(colorSky.r * darkCoef), uint8_t(colorSky.g * darkCoef), uint8_t(colorSky.b * darkCoef)));

		background.push_back(lineGround);
		background.push_back(lineSky);
	}

	return(background);
}

float mouseMoveConvert(sf::Event event, int sizeWindow, sf::CircleShape referentObject, float sensibility) {
	float angle = 0.0;
	float objectPosition = referentObject.getPosition().x + referentObject.getRadius();

	angle = float(((objectPosition - event.mouseMove.x) / sizeWindow) * 2 * PI * sensibility);

	return(angle);
}

float mouseMove3D(sf::Event event, float oldMousePos, int windowSize, float sensibility) {
	float angle = float((-event.mouseMove.x + oldMousePos) / windowSize * 2 * PI * sensibility);

	return(angle);
}



void changePosLines(sf::Vector2f moveVector, std::vector<sf::VertexArray>* lineRaycast, int windowSize, double viewAngle, float playerAngle) {
	double diagWindow = sqrt(2 * windowSize * windowSize);

	double angleStep = (2 * viewAngle) / (*lineRaycast).size();

	for (unsigned int i = 0; i < (*lineRaycast).size(); i++) {
		(*lineRaycast)[i][0].color = sf::Color(252, 209, 42, 128);
		(*lineRaycast)[i][1].color = sf::Color(252, 209, 42, 128);

		double theta = -viewAngle + i * angleStep - playerAngle;

		(*lineRaycast)[i][1].position = sf::Vector2f(float((*lineRaycast)[i][0].position.x + diagWindow * cos(theta)), float((*lineRaycast)[i][0].position.y + diagWindow * sin(theta)));

		(*lineRaycast)[i][0].position.x += moveVector.x;
		(*lineRaycast)[i][0].position.y += moveVector.y;

		(*lineRaycast)[i][1].position.x += moveVector.x;
		(*lineRaycast)[i][1].position.y += moveVector.y;
	}
}

void changeAngleLines(std::vector<sf::VertexArray>* lineRaycast, float playerX, float playerY, double angle, int windowSize, double viewAngle) {
	std::vector < sf::VertexArray> tDLines;

	double diagWindow = sqrt(2 * windowSize * windowSize);

	double rangeAngle = 2 * viewAngle;
	double angleStep = ((rangeAngle) / (*lineRaycast).size());

	for (unsigned int i = 0; i < (*lineRaycast).size(); i++) {
		(*lineRaycast)[i][0].color = sf::Color(252, 209, 42, 128);
		(*lineRaycast)[i][1].color = sf::Color(252, 209, 42, 128);

		double theta = -viewAngle + i * angleStep - angle;
		(*lineRaycast)[i][1].position = sf::Vector2f(float(playerX + diagWindow*cos(theta)), float(playerY + diagWindow*sin(theta)));
	}

}

void checkIntersection(sf::CircleShape referentObject, std::vector<sf::VertexArray>* lines, std::vector < std::vector<sf::RectangleShape> > map, sf::Color wallColor, int windowSize, double viewAngle, int blocNumber) {
	float blocSize = float(windowSize) / float(blocNumber);

	for (unsigned int i = 0; i < (*lines).size(); i++) {

		float a = ((*lines)[i][0].position.y - (*lines)[i][1].position.y) / ((*lines)[i][0].position.x - (*lines)[i][1].position.x);
		float b = (*lines)[i][0].position.y - a * (*lines)[i][0].position.x;

		//VERTICAL INTERSECTION
		if ((*lines)[i][0].position.x > (*lines)[i][1].position.x) {

			for (int j = int(referentObject.getPosition().x / blocSize); j > 0; j--) {
				float xPos = j * blocSize;
				float yPos = a * xPos + b;

				if (yPos < windowSize && yPos > 0) {

					if (map[int(yPos / blocSize)][j-1].getFillColor() == wallColor) {
						(*lines)[i][1].position.x = xPos;
						(*lines)[i][1].position.y = yPos;

						(*lines)[i][0].color = sf::Color::Green;
						(*lines)[i][1].color = sf::Color::Green;

						break;
					}
				}

				else {
					break;
				}
			}
		}

		else {

			for (int j = int(referentObject.getPosition().x / blocSize) + 1; j < blocNumber; j++) {
				float xPos = j * blocSize;
				float yPos = a * xPos + b;

				if (yPos < windowSize && yPos > 0) {
					if (map[int(yPos / blocSize)][j].getFillColor() == wallColor) {
						(*lines)[i][1].position.x = xPos;
						(*lines)[i][1].position.y = yPos;

						(*lines)[i][0].color = sf::Color::Green;
						(*lines)[i][1].color = sf::Color::Green;

						break;
					}
				}

				else {
					break;
				}
			}
		}


		//HORIZONTAL INTERSECTION
		if ((*lines)[i][0].position.y > (*lines)[i][1].position.y) {

			if ((*lines)[i][1].position.y < 0) {
				for (int j = int(referentObject.getPosition().y / blocSize); j > 0; j--) {
					float yPos = j * blocSize;
					float xPos = (yPos - b) / a;

					if (xPos < windowSize && xPos > 0) {
						if (map[j-1][int(xPos / blocSize)].getFillColor() == wallColor) {
							(*lines)[i][1].position.x = xPos;
							(*lines)[i][1].position.y = yPos;

							(*lines)[i][0].color = sf::Color::Blue;
							(*lines)[i][1].color = sf::Color::Blue;

							break;
						}
					}

					else {
						break;
					}
				}
			}

			else {
				for (int j = int(referentObject.getPosition().y / blocSize); j > (*lines)[i][1].position.y / blocSize; j--) {
					float yPos = j * blocSize;
					float xPos = (yPos - b) / a;

					if (xPos < windowSize && xPos > 0) {

						if (map[j-1][int(xPos/blocSize)].getFillColor() == wallColor) {
							(*lines)[i][1].position.x = xPos;
							(*lines)[i][1].position.y = yPos;

							(*lines)[i][0].color = sf::Color::Blue;
							(*lines)[i][1].color = sf::Color::Blue;

							break;
						}
					}

					else {
						break;
					}
				}
			}
		}

		else {

			if ((*lines)[i][1].position.y > windowSize) {
				for (int j = int(referentObject.getPosition().y / blocSize) + 1; j < blocNumber; j++) {
					float yPos = j * blocSize;
					float xPos = (yPos - b) / a;

					if (xPos < windowSize && xPos > 0) {

						if (map[j][int(xPos / blocSize)].getFillColor() == wallColor) {
							(*lines)[i][1].position.x = xPos;
							(*lines)[i][1].position.y = yPos;

							(*lines)[i][0].color = sf::Color::Blue;
							(*lines)[i][1].color = sf::Color::Blue;

							break;
						}

					}

					else {
						break;
					}
				}
			}

			else {
				for (int j = int(referentObject.getPosition().y / blocSize) + 1; j < (*lines)[i][1].position.y / blocSize; j++) {
					float yPos = j * blocSize;
					float xPos = (yPos - b) / a;

					if (xPos < windowSize && xPos > 0) {

						if (map[j][int(xPos / blocSize)].getFillColor() == wallColor) {
							(*lines)[i][1].position.x = xPos;
							(*lines)[i][1].position.y = yPos;

							(*lines)[i][0].color = sf::Color::Blue;
							(*lines)[i][1].color = sf::Color::Blue;

							break;
						}
					}
				}
			}
		}
	}
}


std::vector < sf::VertexArray> raycast(std::vector<sf::VertexArray>* lineRaycast, float playerAngle, int windowSize, double viewAngle, float distEcran, float hWall, sf::Color wallColor) {
	std::vector < sf::VertexArray> tDLines;

	double rangeAngle = 2 * viewAngle;
	double angleStep = ((rangeAngle) / (*lineRaycast).size());

	float coefMax = 0.;

	for (unsigned int i = 0; i < (*lineRaycast).size(); i++) {
		if ((*lineRaycast)[i][0].color == sf::Color::Green || (*lineRaycast)[i][0].color == sf::Color::Blue) {
			float dist = float(sqrt(pow((*lineRaycast)[i][1].position.x - (*lineRaycast)[i][0].position.x, 2) + pow((*lineRaycast)[i][1].position.y - (*lineRaycast)[i][0].position.y, 2)));
			double angle = playerAngle - i * angleStep + viewAngle;

			float coef = float(distEcran / (dist * cos(angle)));

			if (coef > coefMax) {
				coefMax = coef;
			}
		}
	}

	for (unsigned int i = 0; i < (*lineRaycast).size(); i++) {
		if ((*lineRaycast)[i][0].color == sf::Color::Green || (*lineRaycast)[i][0].color == sf::Color::Blue) {
			sf::VertexArray pixelCol(sf::Lines, 2);

			float dist = float(sqrt(pow((*lineRaycast)[i][1].position.x - (*lineRaycast)[i][0].position.x, 2) + pow((*lineRaycast)[i][1].position.y - (*lineRaycast)[i][0].position.y, 2)));
			double angle = playerAngle - i * angleStep + viewAngle;

			float coef = float(distEcran / (dist * cos(angle)));
			float taille = coef * hWall;

			float darkCoef = float(int(coef / coefMax * 20)) / 20;

			if (darkCoef < 0.5) {
				darkCoef = 0.5;
			}

			pixelCol[0].position = sf::Vector2f(float(i), windowSize / 2 - (taille / 2));
			pixelCol[1].position = sf::Vector2f(float(i), windowSize / 2 + (taille / 2));

			if ((*lineRaycast)[i][0].color == sf::Color::Blue) {
				pixelCol[0].color = sf::Color(wallColor.r * 0.5 * darkCoef, wallColor.g * 0.5 * darkCoef, wallColor.b * 0.5 * darkCoef);
				pixelCol[1].color = sf::Color(wallColor.r * 0.5 * darkCoef, wallColor.g * 0.5 * darkCoef, wallColor.b * 0.5 * darkCoef);
			}

			if ((*lineRaycast)[i][0].color == sf::Color::Green) {
				pixelCol[0].color = sf::Color(wallColor.r * darkCoef, wallColor.g * darkCoef, wallColor.b * darkCoef);
				pixelCol[1].color = sf::Color(wallColor.r * darkCoef, wallColor.g * darkCoef, wallColor.b * darkCoef);
			}



			tDLines.push_back(pixelCol);
		}
	}



	return(tDLines);
}
