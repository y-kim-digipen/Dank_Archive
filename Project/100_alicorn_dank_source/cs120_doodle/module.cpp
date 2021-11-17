/*
  module.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim made 
  DoYoon Kim implemented the main cannon functionalities
  SeungGeon Kim arranged the initial player class, then added the visuals to the cannon, and then did the eye and the ears

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include "module.h"
#include "basic_math.h"
#include "variables.h"
#include "player.h"
#include "enemy.h"
#include "classes.h"
#include <doodle/doodle.hpp>
using namespace doodle;

#include "SFML/Audio.hpp"
using namespace sf;



void Module::syncRotation() { 

	if (mother->getFireRef()) return;

	if (isLeftKeyDown)
		coreTurn += coreAccelerationLeft * globalDeltaTime;
	if (isRightKeyDown)
		coreTurn -= coreAccelerationRight * globalDeltaTime;
	
	if (coreTurn >= coreTurnMax)
		coreTurn = coreTurnMax;
	else if (coreTurn <= -coreTurnMax)
		coreTurn = -coreTurnMax;

	if (!isLeftKeyDown && !isRightKeyDown)
		coreTurn = lerp(coreTurn, 0.0f, coreDeceleration);
	
	rotateVector(rotationVector, coreTurn);

}



void Wheel::update()
{
	syncRotation();

	(isProjectionOverlayed) ? (draw()) : (show());
}

void Wheel::move() {
	
	if (isDownKeyDown)
		wheelSpeed -= wheelAcceleration * globalDeltaTime;
	if (isUpKeyDown)
		wheelSpeed += wheelAcceleration * globalDeltaTime;

	if (wheelSpeed >= wheelSpeedMax)
		wheelSpeed = wheelSpeedMax;
	else if (wheelSpeed <= -wheelSpeedMax)
		wheelSpeed = -wheelSpeedMax;

	if (!isDownKeyDown && !isUpKeyDown)
		wheelSpeed = lerp(wheelSpeed, 0.0f, wheelDeceleration);

	mother->translatePos2D(rotationVector * wheelSpeed);

}

void Wheel::show()
{
	push_settings();

	pop_settings();
}

void Wheel::draw()
{
	push_settings();

	pop_settings();
}



void Ear::update()
{
	
	syncRotation();
	
	percept();

	(isProjectionOverlayed) ? (draw()) : (show());
	
}

void Ear::percept()
{
	for (enemy* instEnemy : enemyList[gameWave]) {
		Sound& enemyAudio = instEnemy->audioSource();

		if (!isStereoReversed) {
			enemyAudio.setPosition(Vector3f(instEnemy->getPos2DProjected().x, 0.0f, instEnemy->getPos2DProjected().y));
		}
		else {
			enemyAudio.setPosition(Vector3f(-instEnemy->getPos2DProjected().x, 0.0f, instEnemy->getPos2DProjected().y));
		}

	}
}

void Ear::show()
{
	push_settings();

	pop_settings();
}

void Ear::draw()
{
	push_settings();

	pop_settings();
}



void Eye::update() 
{

	syncRotation();

	percept();

	(isProjectionOverlayed) ? (draw()) : (show());
	
}

void Eye::percept() // gives enemy their initial 'projected' position vector
{
	for (enemy* instEnemy : enemyList[gameWave]) {

		Vector lineVector;
		lineVector.x = -rotationVector.y;
		lineVector.y = rotationVector.x;

		Vector posVector;
		mother->syncPos2D(posVector);

		Vector vectorToProject;
		instEnemy->syncPos2D(vectorToProject);



		Vector projectedVector;

		float lineVectorLength = returnVectorLength(lineVector);

		projectedVector = posVector + lineVector * 
			((lineVector * (vectorToProject - posVector)) / 
			(lineVectorLength * lineVectorLength));

		Vector projectedEnemyX = (projectedVector - posVector);
		Vector projectedEnemyY = (vectorToProject - projectedVector);

		Vector perceptedEnemyVector{ returnVectorLength(projectedEnemyX), returnVectorLength(projectedEnemyY) };

		Vector vectorPlayerToEnemy{ (vectorToProject - posVector).x, (vectorToProject - posVector).y };



		if (vectorPlayerToEnemy.x * lineVector.y - vectorPlayerToEnemy.y * lineVector.x < 0) perceptedEnemyVector.y *= -1;
		if (vectorPlayerToEnemy.x * rotationVector.y - vectorPlayerToEnemy.y * rotationVector.x < 0) perceptedEnemyVector.x *= -1;



		instEnemy->projectPos2D(perceptedEnemyVector);



		push_settings(); // The minimal indication of player rotation

		set_outline_color(HexColor(red3));
		set_outline_width(defaultEdgeWidth * 2);

		draw_line(0, 0,
			0 + -rotationVector.x * playerDrawSize * 3 / 4, 0 + rotationVector.y * playerDrawSize * 3 / 4);

		set_outline_color(HexColor(blue3));
		set_outline_width(defaultEdgeWidth * 2);
		draw_line(0, 0,
			0 + rotationVector.x * playerDrawSize * 3 / 4, 0 + -rotationVector.y * playerDrawSize * 3 / 4);

		pop_settings();

	}
}

void Eye::show()
{
	push_settings();

	pop_settings();
}

void Eye::draw()
{
	push_settings();

	pop_settings();
}



void Cannon::update()
{

	bool& isPlayerFiring = mother->getFireRef();

	syncRotation();

	isAnythingInRange = false;

	if (fireCount > 0) {
		fireCount -= globalDeltaTime;
	}
	else {
		isPlayerFiring = false;
	}

	if (isSpaceKeyDown && isProjectionOverlayed)
	{
		if(!mother->isFiring)
		if (!isCharging)
		{		
			isCharging = true;
		}
		else
		{
			charge();
		}
	}
	else //when space is released
	{
		if (isCharging) {
			isPlayerFiring = true;
			fireCount = chargedRange / 1000;
			fire();
		}
	}

	(isProjectionOverlayed) ? (draw()) : (show());

}

void Cannon::show()
{

	push_settings();
	apply_translate(0, 0);
	apply_rotate(HALF_PI);

	set_fill_color(red1);

	draw_rectangle(15, 10, 15, 5);
	draw_rectangle(15, -10, 15, 5);
	pop_settings();
}

void Cannon::draw()
{

	if (isCharging) {

		push_settings();

		set_outline_color(blue1);
		set_outline_width(5);

		Vector tempVectorSideLeft{ maxCannonWidth, windowBaseDepth };
		Vector tempVectorSideRight{ -maxCannonWidth, windowBaseDepth };

		float tempChargedRange = chargedRange;

		if (tempChargedRange > maxCannonRange - baseCannonDrawDistance)
			tempChargedRange = maxCannonRange - baseCannonDrawDistance;

		tempVectorSideLeft *= ((tempChargedRange + baseCannonDrawDistance) / maxCannonRange);
		tempVectorSideRight *= ((tempChargedRange + baseCannonDrawDistance) / maxCannonRange);

		draw_line(maxCannonWidth, -windowBaseDepth, maxCannonWidth + tempVectorSideRight.x, -windowBaseDepth + tempVectorSideRight.y);
		draw_line(-maxCannonWidth, -windowBaseDepth, -maxCannonWidth + tempVectorSideLeft.x, -windowBaseDepth + tempVectorSideLeft.y);

		pop_settings();

	}

	if (mother->getFireRef()) {

		push_settings();

		unsigned int tempColor = 0xFF;

		tempColor |= (55 << 24);
		tempColor |= (int(200 * random(0.f, 1.f)) << 16);
		tempColor |= (int(255 * random(0.8f, 1.f)) << 8);

		HexColor tempHex;
		tempHex.rgba = tempColor;

		set_fill_color(tempHex);

		float width = float(random(0.f, maxCannonWidth));

		Vector vectorSideLeft{ width, windowBaseDepth };
		Vector vectorSideRight{ -width, windowBaseDepth };

		if (shotRange > maxCannonRange - baseCannonDrawDistance)
			shotRange = maxCannonRange - baseCannonDrawDistance;

		vectorSideLeft *= ((shotRange + baseCannonDrawDistance) / maxCannonRange);
		vectorSideRight *= ((shotRange + baseCannonDrawDistance) / maxCannonRange);

		draw_quad(width, -windowBaseDepth, -width, -windowBaseDepth, 
			-width + vectorSideLeft.x, -windowBaseDepth + vectorSideLeft.y, width + vectorSideRight.x, -windowBaseDepth + vectorSideRight.y);

		pop_settings();

	}

	// Draw Cannon itself in first person view
	push_settings();

	push_settings();
	float shakeX = 0;
	float shakeY = 0;
	shakeX = float(mother->shakingTime) * random(-1.f, 1.f);
	shakeY = float(mother->shakingTime) * random(-1.f, 1.f);
	
	set_fill_color(red1);
	apply_rotate(0.65f);
	draw_rectangle(30 + shakeX, -600 + shakeY, 75, 600);

	apply_rotate(-1.3f);
	draw_rectangle(-30 + shakeX, -600 + shakeY, 75, 600);
	pop_settings();
	if (isAnythingInRange)
		set_fill_color(155, 255, 55, 255);
	else
		set_fill_color(255, 255, 255, 255);

	draw_triangle(0, 15.f, -10.f, -10.f, 10.f, -10.f);

	pop_settings();
}

void Cannon::charge()
{
	if (chargedRange < maxCannonRange)
		chargedRange += deltaChargeRange;
	else
		chargedRange = maxCannonRange;

	GameObject* obj;
	Vector position = mother->getPos2D();
	isAnythingInRange = getFirstObjectHitByRay(position, rotationVector * chargedRange, obj);
	
	if (isAnythingInRange)
	{
		float distanceToClosestObj = (obj->getPos2D() - position).length();
		if (distanceToClosestObj <= chargedRange)
			isAnythingInRange = true;
	}
}

void Cannon::fire()
{
	Vector pos = mother->getPos2D();

	GameObject* obj = nullptr;
	bool isHit = getFirstObjectHitByRay(pos, pos + rotationVector * chargedRange, obj);
	if (isHit && !(obj->getisDying()))
	{
		obj->makeDying();
	}

	shotRange = chargedRange;
	isCharging = false;
	chargedRange = initChargedRange;
}
