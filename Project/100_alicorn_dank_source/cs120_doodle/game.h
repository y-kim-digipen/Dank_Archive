/*
  game.h

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim Made this all
  DoYoon Kim	
  SeungGeon Kim

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#pragma once

#include <vector>
#include "sound.h"
#include <string>
#include "doodle/doodle.hpp"
#include "basic_math.h"



using namespace std;
using namespace doodle;

struct Vector;

enum class gameState : int {
	GAMEPLAY, HOWTOPLAY, CREDIT, SPLASHLOGO, MAINMENU, GAMEOVER };

void operator++(gameState&);
void operator--(gameState&);

class State {
protected:

public:
	virtual void setup() = 0;
	virtual void update() = 0;

	State* thisPtr() {
		return this;
	}
};

class Game {
private:
	gameState currentState = gameState::SPLASHLOGO;
	State* statePtr = nullptr;
	bool isSetted = false;

public:
	void setup();
	void update();
	void toState(const gameState& state);

	State* getStatePtr() {
		return statePtr;
	}
};

class SplashLogo : public State {
private:
	Texture        DigipenSplash;
	float dt_ = 0.0f;

public:
	void setup() override;
	void update() override;
};

enum class ParagraphType {
	Title, normalText
};


void enemyEmergence();

class Credit : public State {
private:
	Texture        Credit;
public:
	void setup() override;
	void update() override;
};


class GamePlay : public State {
public:
	void setup() override;
	void update() override;
};

struct Button {
	Vector pos;
	float width = 0.0f;
	float height = 0.0f;
	doodle::HexColor color;
	gameState name; 

	Button(float newPosX, float newPosY, float newWidth, float newHeight, doodle::HexColor newColor, gameState state):
		pos{ Vector{newPosX , newPosY} }, width{ newWidth }, height{ newHeight }, color{ newColor }, name{ state }{}
};

class MainMenu : public State {
private:
	Texture title;
	vector<Button*> buttonList;
	gameState playerSelect = gameState::GAMEPLAY;
public:
	void setup() override;
	void update() override;
};

class GameOver : public State {
public:
	void setup() override;
	void update() override;
};

class HowToPlay : public State {
private:
	Texture Instructions;
	Texture GameGoal;
	bool isGameGoal = true;
public:
	void setup() override;
	void update() override;
};





