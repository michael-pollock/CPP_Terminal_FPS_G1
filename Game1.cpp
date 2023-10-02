
#include "Game1.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <stack>
#include <thread>
#include <chrono>

int playerHealth = 100;
std::stack<int> gunClip;

int main(int argc, char** argv) {
	srand(time(NULL));
	const int areaSize = 10;
	int** area = new int* [areaSize];
	createArea(area, areaSize);
	int player = 1;
	int enemy_pawn = 2;
	bool play = true;
	int playerX = 0;
	int playerY = 0;
	spawnEnemy(area, areaSize, enemy_pawn, 10);
	spawnPlayer(area, playerX, playerY);
	char select;
	std::cout << "Your location is marked by a 1. The enemy's location is marked by a 2." << std::endl;
	std::cout << "Find all of the enemies and detroy them." << std::endl;
	std::cout << "Move using the 'wasd' keys and hit enter after each keypress." << std::endl;
	while (play) {
		displayArea(area, areaSize);
		std::cin >> select;
		switch (select) {
		case 'w': move(area, areaSize, 'w', playerX, playerY);
			break;
		case 's': move(area, areaSize, 's', playerX, playerY);
			break;
		case 'a': move(area, areaSize, 'a', playerX, playerY);
			break;
		case 'd': move(area, areaSize, 'd', playerX, playerY);
			break;
		case 'p': play = false;
			break;
		}
	}
	deleteArea(area, areaSize);
	return 0;
}

void createArea(int** area, int size) {
	std::cout << "Created area." << std::endl;
	for (int i = 0; i < size; i++) {
		area[i] = new int[size];
		std::cout << "Created array " << i << std::endl;
	}
	clearArea(area, size);
}

void clearArea(int** area, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			area[i][j] = 0;
		}
	}
}

void deleteArea(int** area, int size) {
	for (int i = 0; i < size; i++) {
		delete area[i];
		std::cout << "Deleted area " << i << std::endl;
	}
	delete area;
	std::cout << "Deleted area." << std::endl;
}

void spawnEnemy(int** area, int size, int enemyType, int numOfEnemy) {
	int enemyX;
	int enemyY;
	for (int i = 1; i < numOfEnemy; i++) {
		bool open = false;
		while (!open) {
			enemyX = rand() % size;
			enemyY = rand() % size;
			if (area[enemyX][enemyY] == 0) {
				open = true;
			}
		}
		area[enemyX][enemyY] = enemyType;
		std::cout << "Enemy placed at (" << enemyX << ", " << enemyY << ')' << std::endl;
	}
}

void displayArea(int** area, int size) {
	std::cout << std::endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << '[' << area[i][j] << ']';
		}
		std::cout << std::endl;
	}
}

void reload(std::stack<int>& clip) {
	int full = 6 - clip.size();
	for (int i = 0; i < full; i++) {
		clip.push(rand() % 100);
	}
}

int fire(std::stack<int>& clip) {
	int damage = 0;
	if (!clip.empty()) {
		damage = clip.top();
		clip.pop();
	}
	else {
		std::cout << "Press R to reload!" << std::endl;
	}
	return damage;
}

void spawnPlayer(int** area, int x, int y) {
	area[x][y] = 1;
}

void move(int** area, const int& size, char ch, int& currX, int& currY) {
	removePlayer(area, currX, currY);
	switch (ch) {
	case 'w': currX--;
		break;
	case 's': currX++;
		break;
	case 'd': currY++;
		break;
	case 'a': currY--;
		break;
	}
	validMove(size, currX, currY);
	int winner;
	if (area[currX][currY] == 2) {
		duelAscii();
		winner = fight();
		if (winner == 3) {
			currX--;
			validMove(size, currX, currY);
			area[currX][currY] = 2;
			return;
		}
		if (winner == 2 ) exit(0);
		return;
	}
	spawnPlayer(area, currX, currY);
	return;
}

void validMove(const int& size, int& currX, int& currY) {
	if (currX > size - 1) currX = 0;
	if (currX < 0) currX = size - 1;
	if (currY > size - 1) currY = 0;
	if (currY < 0) currY = size - 1;
}

void removePlayer(int** area, int& currX, int& currY) {
	area[currX][currY] = 0;
}

int fight() {
	int enemyHealth = 50;
	char select;
	int damage;
	bool escape = false;
	std::cout << "Controls: " << std::endl;
	std::cout << "Reload: r" << std::endl;
	std::cout << "Fire: f" << std::endl;
	std::cout << "Heal: h" << std::endl;
	std::cout << "Escape: e" << std::endl;

	while (enemyHealth > 0 && playerHealth > 0 && !escape) {
		std::cout << "YOUR HEALTH: " << playerHealth << std::endl;
		std::cout << "ENEMY HEALTH: " << enemyHealth << std::endl;
		std::cout << "LOADED AMMO: " << gunClip.size() << std::endl;

		damage = 0;
		std::cin >> select;
		switch (select) {
		case 'r': 
			reload(gunClip);
			break;
		case 'f': 
			damage = fire(gunClip);
			playerAttack();
			break;
		case 'h': 
			std::cout << "Healing..." << std::endl;
			playerHealth += rand() % 50;
			break;
		case 'e':
			escape = true;
			break;
		default: 
			std::cout << "You fumbled and missed." << std::endl;
		}
		enemyHealth = enemyHealth - damage;
		std::cout << "You dealt " << damage << " damage." << std::endl;
		if (enemyHealth > 0) {
			damage = rand() % 50;
			enemyAttack();
			playerHealth = playerHealth - damage;
			std::cout << "You took " << damage << " damage." << std::endl;
		}
	}
	if (escape) return 3;
	if (enemyHealth <= 0) {
		std::cout << "You claimed this space." << std::endl;
		std::cout << "Remaining health: " << playerHealth << std::endl;
		return 1;
	}
	else {
		std::cout << "You have perished and been forgotten to nameless enemies." << std::endl;
		return 2;
	}
}

void playerAttack() {
	frame1();
	frame2();
	frame3();
}

void enemyAttack() {
	frame4();
	frame5();
	frame6();
}

void duelAscii() {
	std::cout << "  __     FIGHT     __     " << std::endl;
	std::cout << " _||_             _||_    " << std::endl;
	std::cout << "  ~oo             oo~     " << std::endl;
	std::cout << "  / \\            / \\   " << std::endl;
	std::cout << "  |                |      " << std::endl;
	std::cout << " / \\              / \\   " << std::endl;
}

void frame1() {
	std::cout << "  __     FIGHT     __     " << std::endl;
	std::cout << " _||_             _||_    " << std::endl;
	std::cout << "  ~oo             oo~     " << std::endl;
	std::cout << "  / \\  ->        / \\   " << std::endl;
	std::cout << "  |                |      " << std::endl;
	std::cout << " / \\              / \\   " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void frame2() {
	std::cout << "  __     FIGHT     __     " << std::endl;
	std::cout << " _||_             _||_    " << std::endl;
	std::cout << "  ~oo        ->   oo~     " << std::endl;
	std::cout << "  / \\            / \\   " << std::endl;
	std::cout << "  |                |      " << std::endl;
	std::cout << " / \\              / \\   " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void frame3() {
	std::cout << "  __     FIGHT     __     " << std::endl;
	std::cout << " _||_             _||_    " << std::endl;
	std::cout << "  ~oo            `xx`     " << std::endl;
	std::cout << "  / \\            / \\   " << std::endl;
	std::cout << "  |                |      " << std::endl;
	std::cout << " / \\              / \\   " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void frame4() {
	std::cout << "  __     FIGHT     __     " << std::endl;
	std::cout << " _||_             _||_    " << std::endl;
	std::cout << "  ~oo             oo~     " << std::endl;
	std::cout << "  / \\      <-    / \\   " << std::endl;
	std::cout << "  |                |      " << std::endl;
	std::cout << " / \\              / \\   " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void frame5() {
	std::cout << "  __     FIGHT     __     " << std::endl;
	std::cout << " _||_             _||_    " << std::endl;
	std::cout << "  ~oo   <-        oo~     " << std::endl;
	std::cout << "  / \\            / \\   " << std::endl;
	std::cout << "  |                |      " << std::endl;
	std::cout << " / \\              / \\   " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

void frame6() {
	std::cout << "  __     FIGHT     __     " << std::endl;
	std::cout << " _xx_             _||_    " << std::endl;
	std::cout << "  ~OO'            oo~     " << std::endl;
	std::cout << "  / \\            / \\   " << std::endl;
	std::cout << "  |                |      " << std::endl;
	std::cout << " / \\              / \\   " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
}