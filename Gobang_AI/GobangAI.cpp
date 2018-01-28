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

bool judgeResult(int x, int y, int kinds) {
	// row
	int count = 0, lx = x - 1, rx = x + 1;
	while (lx >= 0 && board[lx][y] == kinds) { --lx; ++count;}
	while (rx < boardSize && board[rx][y] == kinds) { ++rx; ++count; }
	if (count >= 4) return true;

	// column
	count = 0;
	int uy = y - 1, dy = y + 1;
	while (uy >= 0 && board[x][uy] == kinds) { --uy; ++count; }
	while (dy < boardSize && board[x][dy] == kinds) { ++dy; ++count; }
	if (count >= 4) return true;

	// right-up / left-down
	count = 0;
	int rux = x + 1, ruy = y - 1, ldx = x - 1, ldy = y + 1;
	while (rux < boardSize && ruy >= 0 && board[rux][ruy] == kinds) { ++rux; --ruy; ++count;}
	while (ldx >= 0 && ldy < boardSize && board[ldx][ldy] == kinds) { --ldx; ++ldy; ++count;}
	if (count >= 4) return true;

	// left-up / right-down
	count = 0;
	int lux = x - 1, luy = y - 1, rdx = x + 1, rdy = y + 1;
	while (lux >= 0 && luy >= 0 && board[lux][luy] == kinds) { --lux; --luy; ++count;}
	while (rdx < boardSize && rdy < boardSize && board[rdx][rdy] == kinds) { ++rdx; ++rdy; ++count;}
	if (count >= 4) return true;

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
		while (placeStone(std::get<0>(tp), std::get<1>(tp), 1)) {
			std::cout << "illegal location, please reinput the black stone location!\n";
			tp = playStoneHuman(1);
		}
		showBoard();
		if (judgeResult(std::get<0>(tp), std::get<1>(tp), 1)) {
			std::cout << "Black Wins !!!!!\n";
			break;
		}

		tp = playStoneHuman(-1);
		while (placeStone(std::get<0>(tp), std::get<1>(tp), -1)) {
			std::cout << "illegal location, please reinput the white stone location!\n";
			tp = playStoneHuman(-1);
		}
		showBoard();
		if (judgeResult(std::get<0>(tp), std::get<1>(tp), -1)) {
			std::cout << "White Wins !!!!!\n";
			break;
		}
	}
}


int main()
{
	holdGame();
	system("pause");
    return 0;
}

