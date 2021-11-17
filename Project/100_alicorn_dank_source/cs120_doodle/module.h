/*
  module.h

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim
  DoYoon Kim 
  SeungGeon Kim arranged the initial module class

  All content © 2019 DigiPen(USA) Corporation, all vectors reserved.
*/

#pragma once

#include "basic_math.h"
#include <vector>



class player;

class Module {
protected:
	Vector rotationVector{ 0.0f, 1.0f };
	player* mother = nullptr;
	bool isJammed = false;

	float coreTurn = 0.0f;
	void syncRotation();

public:
	Module() {};

	virtual void update() = 0;
	virtual void show() = 0;
	virtual void draw() = 0;

	void link(player* playerPointer) { mother = playerPointer; };

};



class Wheel : public Module {
private:
	float wheelSpeed = 0.0f;
public:
	Wheel() {};

	void update() override;
	void show() override;
	void draw() override;

	void move();

	void syncRotationVector(Vector& vector) { vector = rotationVector; };

};



class Ear : public Module {
private:

public:
	Ear() {};

	void update() override;
	void show() override;
	void draw() override;

	void percept();

};



class Eye : public Module {
private:

public:
	Eye() {};

	void update() override;
	void show() override;
	void draw() override;

	void percept();

};

class Cannon : public Module
{
public:
	Cannon() {};

	void update() override;
	void show() override;
	void draw() override;
	void charge();
	void fire();
private:
	bool isCharging = false;
	float initChargedRange = 0.f;
	float chargedRange = initChargedRange;
	float shotRange = 0.f;
	const float deltaChargeRange = 5.f;

	bool isAnythingInRange = false;

	float fireCount = 0;
};
