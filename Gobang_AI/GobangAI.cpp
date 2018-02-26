// GobangAI.cpp: 定义控制台应用程序的入口点。
//

#include"stdafx.h"
#include<vector>
#include<tuple>
#include<iostream>
#include<string>
#include<unordered_map>
#include<algorithm>
#include<queue>

const int boardSize = 11;
std::vector<std::vector<int> > board;
std::unordered_map<std::string, int> ump;

std::tuple<int, int> playStoneHuman(int kinds);
int evaluate(int x, int y, int kinds);
int evaluateWholeBoard(int kinds);
int minMaxAlphaBeta(int depth, int kinds, int mvKinds, int a, int b);

std::vector<std::vector<int>> directions = { {0,-1}, {-1,-1}, {-1,0},{-1,1}, {0,1}, {1,1}, {1,0}, {1, -1} };

struct location {
	int value;
	int x;
	int y;

	location(int n1, int n2, int n3) : value(n1), x(n2), y(n3) {}

	bool operator<(const struct location& other) const
	{
		//Your priority logic goes here
		return value < other.value;
	}
};

void init() {
	for (int i = 0; i < boardSize; ++i)
		board.push_back(std::vector<int>(boardSize, 0));
	ump["ooooo"] = 50000;
	ump["+oooo+"] = 4320;
	ump["+ooo++"] = 720;
	ump["++ooo+"] = 720;
	ump["+oo+o+"] = 720;
	ump["+o+oo+"] = 720;
	ump["oooo+"] = 720;
	ump["+oooo"] = 720;
	ump["oo+oo"] = 720;
	ump["o+ooo"] = 720;
	ump["ooo+o"] = 720;
	ump["++oo++"] = 120;
	ump["++o+o+"] = 120;
	ump["+o+o++"] = 120;
	ump["+++o++"] = 20;
	ump["++o+++"] = 20;
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
		std::cout << static_cast<char>('a' + i) << " ";
	std::cout << std::endl;
}

int placeStone(int x, int y, int kinds) {
	if (x >= boardSize || x < 0 || y >= boardSize || y < 0 || board[x][y] != 0) return 1;
	board[x][y] = kinds;
	return 0;
}

bool judgeResult(int x, int y, int kinds) {
	// row
	int count = 0, lx = x - 1, rx = x + 1;
	while (lx >= 0 && board[lx][y] == kinds) { --lx; ++count; }
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
	while (rux < boardSize && ruy >= 0 && board[rux][ruy] == kinds) { ++rux; --ruy; ++count; }
	while (ldx >= 0 && ldy < boardSize && board[ldx][ldy] == kinds) { --ldx; ++ldy; ++count; }
	if (count >= 4) return true;

	// left-up / right-down
	count = 0;
	int lux = x - 1, luy = y - 1, rdx = x + 1, rdy = y + 1;
	while (lux >= 0 && luy >= 0 && board[lux][luy] == kinds) { --lux; --luy; ++count; }
	while (rdx < boardSize && rdy < boardSize && board[rdx][rdy] == kinds) { ++rdx; ++rdy; ++count; }
	if (count >= 4) return true;

	return false;
}

bool besideStone(int x, int y) {
	int nx, ny;
	bool flag = false;
	for (auto dir : directions) {
		nx = x + dir[0];
		ny = y + dir[1];
		if (nx >= 0 && nx < boardSize && ny >= 0 && ny < boardSize && board[nx][ny] != 0) {
			flag = true;
		}
	}
	return flag;
}

void holdGame() {
	std::cout << "The Gobang game start!\n";
	init();
	std::tuple<int, int> tp;
	showBoard();
	location out(-1, -1, -1);
	int temp, maxScore, locx, locy;
	char color;
	int level;
	std::cout << "please select a level(1 to 3): // my computer support max level 2, level 3 is too slow\n";
	std::cin >> level;
	if (level < 1 || level > 3) level = 2;
	std::cout << "please input the color you want (b or w): \n";
	std::cin >> color;

	if (color == 'b' || color == 'B'){ 
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

			maxScore = INT_MIN;
			for (int i = 0; i < boardSize; ++i) {
				for (int j = 0; j < boardSize; ++j) {
					if (board[i][j] != 0 || !besideStone(i, j)) continue;
					board[i][j] = -1;
					temp = minMaxAlphaBeta(level, -1, 1, INT_MIN, INT_MAX);
					board[i][j] = 0;
					if (temp >= maxScore) {
						maxScore = temp;
						locx = i;
						locy = j;
					}
				}
			}
			board[locx][locy] = -1;

			showBoard();

			if (judgeResult(locx, locy, -1)) {  /*judgeResult(std::get<0>(tp), std::get<1>(tp), -1)*/
				std::cout << "White Wins !!!!!\n";
				break;
			}

			std::cout << "Black score: " << evaluateWholeBoard(1) << " White score: " << evaluateWholeBoard(-1) << std::endl;
		}
	}
	else if (color == 'w' || color == 'W') {
		board[5][5] = 1;
		showBoard();
		while (true)
		{
			tp = playStoneHuman(-1);
			while (placeStone(std::get<0>(tp), std::get<1>(tp), -1)) {
				std::cout << "illegal location, please reinput the black stone location!\n";
				tp = playStoneHuman(-1);
			}

			showBoard();

			if (judgeResult(std::get<0>(tp), std::get<1>(tp), -1)) {
				std::cout << "White Wins !!!!!\n";
				break;
			}

			maxScore = INT_MIN;
			for (int i = 0; i < boardSize; ++i) {
				for (int j = 0; j < boardSize; ++j) {
					if (board[i][j] != 0 || !besideStone(i, j)) continue;
					board[i][j] = 1;
					temp = minMaxAlphaBeta(level, 1, -1, INT_MIN, INT_MAX);
					board[i][j] = 0;
					if (temp >= maxScore) {
						maxScore = temp;
						locx = i;
						locy = j;
					}
				}
			}
			board[locx][locy] = 1;

			showBoard();

			if (judgeResult(locx, locy, 1)) {  /*judgeResult(std::get<0>(tp), std::get<1>(tp), -1)*/
				std::cout << "Black Wins !!!!!\n";
				break;
			}

			std::cout << "Black score: " << evaluateWholeBoard(1) << " White score: " << evaluateWholeBoard(-1) << std::endl;
		}
	}
	else {
		std::cout << "You Win, I surrender......\n";
		system("pause");
	}
}


int main()
{
	holdGame();
	system("pause");
	return 0;
}

// KMP algorithm for pattern match, return first index;
int strStr(std::string haystack, std::string needle) {
	int nsize = needle.size();
	int hsize = haystack.size();
	if (nsize == 0) return 0;
	int *table = new int[nsize];
	memset(table, 0, sizeof(int)*nsize);
	//building match table
	for (int i = 1, j = 0; i < nsize - 1;) {
		if (needle[i] != needle[j]) {
			if (j>0) {
				j = table[j - 1];
			}
			else {
				i++;
			}
		}
		else {
			table[i] = j + 1;
			i++;
			j++;
		}
	}
	//matching
	for (int i = 0, match_pos = 0; i < hsize;) {
		if (haystack[i] == needle[match_pos]) {
			if (match_pos == nsize - 1) {
				return i - (nsize - 1);
			}
			else {
				i++;
				match_pos++;
			}
		}
		else {
			if (match_pos == 0) {
				i++;
			}
			else {
				match_pos = table[match_pos - 1];
			}
		}
	}
	delete[]table;
	return -1;
}

int evaluate(int x, int y, int kinds) {
	std::string rows, cols, lus, rus;

	if (board[x][y] != 0) return 0;

	board[x][y] = kinds;
	// rows
	int lx = x - 4 < 0 ? 0 : x - 4;
	int rx = x + 4 >= boardSize ? boardSize - 1 : x + 4;
	for (int i = lx; i <= rx; ++i) {
		if (board[i][y] == 0) rows = rows + "+";
		else if (board[i][y] == kinds) rows = rows + "o";
		else rows += "A";
	}


	// cols
	int uy = y - 4 < 0 ? 0 : y - 4;
	int dy = y + 4 >= boardSize ? boardSize - 1 : y + 4;
	for (int j = uy; j <= dy; ++j) {
		if (board[x][j] == 0) cols += "+";
		else if (board[x][j] == kinds) cols += "o";
		else cols += "A";
	}

	// lus
	int lux = x - 4 < 0 ? 0 : x - 4;
	int rdx = x + 4 >= boardSize ? boardSize - 1 : x + 4;
	int luy = y - 4 < 0 ? 0 : y - 4;
	int rdy = y + 4 >= boardSize ? boardSize - 1 : y + 4;
	int lowabs = std::min(x - lux, y - luy);
	int highabs = std::min(rdx - x, rdy - y);
	for (int i = x-lowabs, j = y-lowabs; i <= x+highabs && j <= y+highabs; ++i, ++j) {
		if (board[i][j] == 0) lus += "+";
		else if (board[i][j] == kinds) lus += "o";
		else lus += "A";
	}

	// rus
	int ldx = x - 4 < 0 ? 0 : x - 4;
	int ldy = y + 4 >= boardSize ? boardSize - 1 : y + 4;
	int rux = x + 4 >= boardSize ? boardSize - 1 : x + 4;
	int ruy = y - 4 < 0 ? 0 : y - 4;
	lowabs = std::min(x - ldx, ldy - y);
	highabs = std::min(rux - x, y - ruy);
	for (int i = x - lowabs, j = y + lowabs; i <= x + highabs && j >= y - highabs; ++i, --j) {
		if (board[i][j] == 0) rus += "+";
		else if (board[i][j] == kinds) rus += "o";
		else rus += "A";
	}


	int score, score1 = 0, score2 = 0, score3 = 0, score4 = 0;
	for (auto it = ump.cbegin(); it != ump.cend(); ++it) {
		if (strStr(rows, it->first) != -1) score1 = std::max(score1, it->second);
		if (strStr(cols, it->first) != -1) score2 = std::max(score2, it->second);
		if (strStr(lus, it->first) != -1) score3 = std::max(score3, it->second);
		if (strStr(rus, it->first) != -1) score4 = std::max(score4, it->second);
	}

	score = score1 + score2 + score3 + score4;
	
	board[x][y] = 0;
	return score;
}

int evaluateWholeBoard(int kinds) {
	int finalScore = 0, score=0;

	// rows 
	for (int i = 0; i < boardSize; ++i) {
		std::string rows = "";
		score = 0;
		for (int j = 0; j < boardSize; ++j) {
			if (board[i][j] == 0) rows = rows + "+";
			else if (board[i][j] == kinds) rows = rows + "o";
			else rows += "A";
		}

		for (auto it = ump.cbegin(); it != ump.cend(); ++it)
			if (strStr(rows, it->first) != -1) score = std::max(score, it->second);

		finalScore += score;
	}

	// cols
	for (int j = 0; j < boardSize; ++j) {
		std::string cols = "";
		score = 0;
		for (int i = 0; i < boardSize; ++i) {
			if (board[i][j] == 0) cols += "+";
			else if (board[i][j] == kinds) cols += "o";
			else cols += "A";
		}

		for (auto it = ump.cbegin(); it != ump.cend(); ++it)
			if (strStr(cols, it->first) != -1) score = std::max(score, it->second);

		finalScore += score;
	}

	// lux rows[0, boardSize-5]  cols[1, boardSize-5]
	for (int i = 0; i < boardSize - 4; ++i) {
		std::string lus = "";
		score = 0;
		for (int x = i, y = 0; x < boardSize; ++x, ++y) {
			if (board[x][y] == 0) lus += "+";
			else if (board[x][y] == kinds) lus += "o";
			else lus += "A";
		}

		for (auto it = ump.cbegin(); it != ump.cend(); ++it)
			if (strStr(lus, it->first) != -1) score = std::max(score, it->second);

		finalScore += score;
	}

	for (int j = 1; j < boardSize - 4; ++j) {
		std::string lus = "";
		score = 0;
		for (int x = 0, y = j; y < boardSize; ++x, ++y) {
			if (board[x][y] == 0) lus += "+";
			else if (board[x][y] == kinds) lus += "o";
			else lus += "A";
		}

		for (auto it = ump.cbegin(); it != ump.cend(); ++it)
			if (strStr(lus, it->first) != -1) score = std::max(score, it->second);

		finalScore += score;
	}

	// rus rows[1, boardSize-5] cols[4, boardSize-1]
	for (int j = 4; j < boardSize; ++j) {
		std::string rus = "";
		score = 0;
		for (int x = 0, y = j; y >= 0; ++x, --y) {
			if (board[x][y] == 0) rus += "+";
			else if (board[x][y] == kinds) rus += "o";
			else rus += "A";
		}

		for (auto it = ump.cbegin(); it != ump.cend(); ++it)
			if (strStr(rus, it->first) != -1) score = std::max(score, it->second);

		finalScore += score;
	}

	for (int i = 1; i < boardSize - 4; ++i) {
		std::string rus = "";
		score = 0;
		for (int y = boardSize - 1, x = i; x < boardSize; ++x, --y) {
			if (board[x][y] == 0) rus += "+";
			else if (board[x][y] == kinds) rus += "o";
			else rus += "A";
		}

		for (auto it = ump.cbegin(); it != ump.cend(); ++it)
			if (strStr(rus, it->first) != -1) score = std::max(score, it->second);

		finalScore += score;
	}

	return finalScore;
}

int minMaxAlphaBeta(int depth, int kinds, int mvKinds, int a, int b) {
	if (depth <= 0) return evaluateWholeBoard(kinds)- evaluateWholeBoard(-kinds);
	if (mvKinds == kinds) {
		for (int i = 0; i < boardSize; ++i) {
			for (int j = 0; j < boardSize; ++j) {
				if (board[i][j] != 0 || !besideStone(i, j)) continue;
				board[i][j] = mvKinds;
				a = std::max(a, minMaxAlphaBeta(depth - 1, kinds, -mvKinds, a, b));
				board[i][j] = 0;
				if (b <= a) return a;
			}
		}
		return a;
	}
	else {
		for (int i = 0; i < boardSize; ++i) {
			for (int j = 0; j < boardSize; ++j) {
				if (board[i][j] != 0 || !besideStone(i, j)) continue;
				board[i][j] = mvKinds;
				b = std::min(b, minMaxAlphaBeta(depth - 1, kinds, -mvKinds, a, b));
				board[i][j] = 0;
				if (b <= a) return b;
			}
		}
		return b;
	}
}

std::tuple<int, int> playStoneHuman(int kinds) {
	char tmp;
	int x, y;
	if (kinds == 1) std::cout << "Black: ";
	if (kinds == -1) std::cout << "White: ";
	std::cin >> x >> tmp;
	y = tmp - 'a';
	return std::make_tuple(x, y);
}
