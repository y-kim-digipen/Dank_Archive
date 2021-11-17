/*
  enemy.h

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim made the initial enemy class
  DoYoon Kim added the enemy types and wrapped up the code
  SeungGeon Kim 

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#pragma once

#include "game_object.h"
#include "SFML/Audio.hpp"
#include "variables.h"



class player;
struct Vector;

const float Dyingtime = 2.f;

namespace enemySpeed {
	inline int easyMaxSpeed = 2;
	inline int moderateMaxSpeed = 3;
	inline int hardMaxSpeed = 4;
	inline int zigzagMaxSpeed = 2;
	inline int rushMaxSpeed = 2;
}


inline int maxBlinkSpeedModifier = 6;

enum class enemyType {
	EASY = 1, MODERATE, HARD , ZIGZAG ,WARP,SUPER_FAST
};

enum class directionType {
	UP = 1, DOWN, LEFT, RIGHT
};

Vector toVector(directionType type);

class enemy : public GameObject
{
public:

	enemy(const Vector& newPos2D, player* playerPtr, int nEdges, enemyType type, float cameoutTime);

	void update() override;
	void draw() override;
	void show() override;
	void onHit()override;
	void makeDying() override;
	bool getisDying() override {
		return isDying;
	}
	void setEmergenceTime(const float curTime) {
		emergenceTime += curTime;
	}
	
	void move();
	void noiseSpeed();

	sf::Sound& audioSource() { return sound; };
	int audioIndex() { return soundIndex; };

	float& getEmergenceTime() {
		return emergenceTime;
	}

	void warp();
private: 
	enemyType type;

	bool isDead = false;
	bool detected = true;
	bool isDying = false;

	float whenIsDie = 0.0f;

	//for zigzagmove
	int initDirectionChangeDelay = 180;
	int directionChangeDelay = initDirectionChangeDelay;
	bool directionFlag = false;

	float wheelSpeed = 1.f;
	const float warpDistance = 100.f;
	Vector acceleration{ 0.f,0.f };
	Vector speed{ 0.f,0.f };

	float alpha = 0;

	const player* targetPlayer = nullptr;

	int uniqueBlinkSpeedModifier = 0;

	const int maxWarpTimer = 300;
	int warpTimer = 0;

	sf::Sound sound;
	int soundIndex = 0;

	float detectionCounter = 0.0f;
	float detectionCount = 5.0f;

	float emergenceTime = 0.0f;

};