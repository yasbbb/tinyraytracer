#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "2d.h"

std::vector< std::vector<sf::RectangleShape> > drawMap2D(std::vector<std::vector<int>> mapTab, sf::Color mapInfo[], int windowSize, int blocNumber) {
	std::vector< std::vector <sf::RectangleShape> > mapShape;
	std::vector<sf::RectangleShape> ligne;

	float blocSize = float(windowSize) / float(blocNumber);
	sf::RectangleShape bloc(sf::Vector2f(blocSize, blocSize));

	for (int i = 0; i < blocNumber; i++) {
		for (int j = 0; j < blocNumber; j++) {
			if (mapTab[i][j] == -1) {
				bloc.setFillColor(mapInfo[1]);
			}
			
			else {
				bloc.setFillColor(mapInfo[0]);
			}

			bloc.setPosition(blocSize * j, blocSize * i);

			ligne.push_back(bloc);
		}

		mapShape.push_back(ligne);
		ligne.clear();
	}

	return(mapShape);
}

void changeValueMaze(std::pair<int, int> beginCase, int n, std::vector<std::vector<int>> *maze) {
	std::vector<std::pair<int, int>> toVisit;
	toVisit.push_back(beginCase);

	std::pair<int, int> visiting;
	bool isStop = true;

	while (!(toVisit.empty())) {
		visiting = toVisit[toVisit.size() - 1];
		isStop = true;

		if (unsigned(visiting.first) < (*maze).size() - 1) {
			if ((*maze)[visiting.first + 1][visiting.second] != -1 && (*maze)[visiting.first + 1][visiting.second] != n) {
				(*maze)[visiting.first + 1][visiting.second] = n;
				toVisit.push_back({ visiting.first + 1, visiting.second });

				isStop = false;
			}
		}

		if (visiting.first > 0) {
			if ((*maze)[visiting.first - 1][visiting.second] != -1 && (*maze)[visiting.first - 1][visiting.second] != n) {
				(*maze)[visiting.first - 1][visiting.second] = n;
				toVisit.push_back({ visiting.first - 1, visiting.second });

				isStop = false;
			}
		}

		if (unsigned(visiting.second) < (*maze).size() - 1) {
			if ((*maze)[visiting.first][visiting.second + 1] != -1 && (*maze)[visiting.first][visiting.second + 1] != n) {
				(*maze)[visiting.first][visiting.second+1] = n;
				toVisit.push_back({ visiting.first, visiting.second+1 });

				isStop = false;
			}
		}

		if (visiting.second > 0) {
			if ((*maze)[visiting.first ][visiting.second-1] != -1 && (*maze)[visiting.first ][visiting.second-1] != n) {
				(*maze)[visiting.first ][visiting.second-1] = n;
				toVisit.push_back({ visiting.first , visiting.second -1});

				isStop = false;
			}
		}

		if (isStop) {
			toVisit.pop_back();
		}
	}
}

std::vector<std::vector<int>> generateMaze(int blockNumber) {
	std::vector<std::vector<int>> maze;
	std::vector<int> mazeLine;
	std::vector<int> firstLine;

	std::vector<std::pair<int, int>> wallList;

	int realBlockNumber = 2 * blockNumber - 1;
	int destroyCounter = blockNumber * blockNumber - 1;

	for (int i = 0; i < realBlockNumber + 2; i++) {
		firstLine.push_back(-1);
	}

	maze.push_back(firstLine);

	for (int i = 0; i < realBlockNumber; i++) {
		mazeLine.clear();
		mazeLine.push_back(-1);

		if (i % 2 == 0) {
			for (int j = 0; j < realBlockNumber - 1; j++) {
				if (j % 2 == 1) {
					mazeLine.push_back(-1);
					wallList.push_back({ i+1, j+1 });
				}

				else {
					mazeLine.push_back(i * blockNumber + j / 2);
				}
			}

			mazeLine.push_back(i * blockNumber + (realBlockNumber - 1) / 2);
		}

		else {
			for (int j = 0; j < realBlockNumber; j++) {
				mazeLine.push_back(-1);

				if (j % 2 == 0) {
					wallList.push_back({ i+1, j+1 });
				}
			}
		}

		mazeLine.push_back(-1);
		maze.push_back(mazeLine);
	}

	maze.push_back(firstLine);

	srand(int(time(0)));
	int randomWall = rand()%wallList.size();
	int iWall;
	int jWall;

	while (!(wallList.empty()) && destroyCounter > 0) {
		srand(int(time(0)));

		iWall = wallList[randomWall].first;
		jWall = wallList[randomWall].second;

		if (iWall % 2 == 1) {
			if (maze[iWall][jWall - 1] != maze[iWall][jWall + 1]) {
				maze[iWall][jWall] = maze[iWall][jWall - 1];
				maze[iWall][jWall+1] = maze[iWall][jWall - 1];

				changeValueMaze({ iWall, jWall + 1 }, maze[iWall][jWall - 1], &maze);

				wallList.erase(wallList.begin() + randomWall);
				destroyCounter--;
			}

			else {
				wallList.erase(wallList.begin() + randomWall);
			}
		}

		else {
			if (maze[iWall - 1][jWall] != maze[iWall + 1][jWall]) {
				maze[iWall][jWall] = maze[iWall-1][jWall];
				maze[iWall + 1][jWall] = maze[iWall-1][jWall];

				changeValueMaze({ iWall + 1, jWall }, maze[iWall-1][jWall], &maze);

				wallList.erase(wallList.begin() + randomWall);
				destroyCounter--;
			}

			else {
				wallList.erase(wallList.begin() + randomWall);
			}
		}
		
		if (!(wallList.empty())) {
			randomWall = rand() % wallList.size();
		}
	}

	changeValueMaze({ 0,0 }, 0, &maze);

	return(maze);
}
