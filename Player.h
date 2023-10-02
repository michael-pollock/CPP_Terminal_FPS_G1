#pragma once
#include <iostream>

class Player {
private:
	std::string name;
	int health;
public:
	Player(std::string aName, int maxHealth);
};