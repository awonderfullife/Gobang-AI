// GobangAI.cpp: 定义控制台应用程序的入口点。
//

#include"stdafx.h"
#include<vector>
#include<tuple>
#include<iostream>

const int boardSize = 10;
std::vector<std::vector<int> > board;

void init() {
	for (int i = 0; i < boardSize; ++i)
		board.push_back(std::vector<int>(boardSize, 0));
}

void showBoard() {
	for (int i = 0; i < boardSize; ++i) {
		std::cout << i << ": \t";
		for (int j = 0; j < boardSize; ++j) {
			switch (board[i][j])
			{
			case 1: std::cout << "O "; break;  // black
			case -1: std::cout << "X "; break; // white
			default: std::cout << ". "; break; // empty
			}
		}
		std::cout << std::endl;
	}
	std::cout << " : \t";
	for (int i = 0; i < boardSize; ++i)
		std::cout << static_cast<char>('A'+i) << " ";
	std::cout << std::endl;
}

std::tuple<int, int> playStoneHuman(int kinds) {
	int x, y;
	if (kinds == 1) std::cout << "Black: ";
	if (kinds == -1) std::cout << "White: ";
	std::cin >> x >> y;
	return std::make_tuple(x, y);
}


int placeStone(int x, int y, int kinds) {
	if (x >= boardSize || x < 0 || y >= boardSize || y < 0 || board[x][y] != 0) return 1;
	board[x][y] = kinds;
	return 0;
}

bool judgeResult() {

	return false;
}


void holdGame() {
	std::cout << "The Gobang game start!\n";
	init();
	std::tuple<int, int> tp;
	showBoard();
	while (true)
	{
		tp = playStoneHuman(1);
		placeStone(std::get<0>(tp), std::get<1>(tp), 1);
		showBoard();
		judgeResult();
		tp = playStoneHuman(-1);
		placeStone(std::get<0>(tp), std::get<1>(tp), -1);
		showBoard();
		judgeResult();
	}
}


int main()
{
	holdGame();
    return 0;
}

