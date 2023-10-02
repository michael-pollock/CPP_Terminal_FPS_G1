#pragma once
#include <stack>

int main(int argc, char** argv);
void createArea(int** area, int size);
void deleteArea(int** area, int size);
void clearArea(int** area, int size);
void spawnPlayer(int** area, int x, int y);
void spawnEnemy(int** area, int size, int enemyType, int numOfEnemy);
void displayArea(int** area, int size);
void reload(std::stack<int>&);
int fire(std::stack<int>&);
void move(int** area, const int& size, char ch, int& currX, int& currY);
void validMove(const int& size, int& currX, int& currY);
void removePlayer(int** area, int& currX, int& currY);
int fight();
void duelAscii();
void playerAttack();
void enemyAttack();
void frame1();
void frame2();
void frame3();
void frame4();
void frame5();
void frame6();
