/*
  player.h

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim
  DoYoon Kim
  SeungGeon Kim arranged the initial player class

  All content © 2019 DigiPen(USA) Corporation, all vectors reserved.
*/

#pragma once

#include "basic_math.h"
#include "variables.h"
#include <vector>
#include "game_object.h"
using namespace std;

	

class Module;

class player : public GameObject {
	friend class Cannon;
public:

	virtual void update() override;
	virtual void draw() override;
	virtual void show() override;
	virtual void onHit()override;

	void addModule(Module* module);	

	const int& getLife() const {
		return life;
	}

	bool dead() {
		return isDead;
	}

	bool& getFireRef() { return isFiring; };

	player(Vector newPos2D) : GameObject(newPos2D, 6, red3) {};

protected:

	vector<Module*> moduleList;

	bool isDead = false;
	sf::Sound sound;
	bool isFiring = false;
	int life = playerLife;
	int initShakingTime = 60;
	int shakingTime = 0;
};


