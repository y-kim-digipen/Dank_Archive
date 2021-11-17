/*
  enemy.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim added enemy types and the toVector function
  DoYoon Kim implemented enemy movements and added enemy types(zigzag,warp,super_fast)
  SeungGeon Kim did the sound, draw, blinking, and the show part

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include "enemy.h"
#include "module.h"
#include <doodle/doodle.hpp>
using namespace doodle;

#include "variables.h"
#include "player.h"



enemy::enemy(const Vector& newPos2D, player* playerPtr, int nEdges, enemyType type, float cameoutTime)
	:GameObject(newPos2D, nEdges, red5),type(type),targetPlayer(playerPtr), soundIndex(int(enemyType::MODERATE)), emergenceTime(cameoutTime), detectionCount(5.0f)
{
	int maxRand = 0;
	switch (type) // set properties according to its 'enemyType'
	{
		case enemyType::EASY:
		{
			maxRand = maxBlinkSpeedModifier;
			color = red3;
			wheelSpeed = 5;
			soundIndex = int(enemyType::EASY);
			break;
		}
		case enemyType::MODERATE:
		{
			maxRand = maxBlinkSpeedModifier;
			color = red4;
			wheelSpeed = 7;
			soundIndex = int(enemyType::MODERATE);
			break;
		}
		case enemyType::HARD:
		{
			maxRand = maxBlinkSpeedModifier;
			color = red5;
			wheelSpeed = 9;
			soundIndex = int(enemyType::HARD);
			break;
		}
		case enemyType::ZIGZAG:
		{
			maxRand = maxBlinkSpeedModifier;
			color = green1;
			wheelSpeed = 10;
			soundIndex = int(enemyType::ZIGZAG);
			break;
		}
		case enemyType::WARP:
		{
			maxRand = maxBlinkSpeedModifier;
			color = blue2;
			wheelSpeed = 2;
			soundIndex = int(enemyType::WARP);
			break;
		}
		case enemyType::SUPER_FAST:
			maxRand = maxBlinkSpeedModifier;
			color = blue3;
			wheelSpeed = 20;
			soundIndex = int(enemyType::SUPER_FAST);
			sound.setLoop(true);
			break;
		default:
			break;
	}
	uniqueBlinkSpeedModifier = (std::rand() % maxRand);

	sound.setMinDistance(500.0f); sound.setAttenuation(0.3f);

	pos2DProjected = { 2000.f, 2000.f };
};

void enemy::update()
{

	if (!isDying) {
		for (player* player : playerList)
		{
			float dist_To_Player = (player->getPos2D() - pos2D).length();
			if (dist_To_Player <= playerDrawSize / 2.f + enemyDrawSize / 2.f)
			{
				isDying = true;
				this->onHit();
				player->onHit();
				return;
			}
		}

		//for zigzag enemy
		if (directionChangeDelay > 0)
			directionChangeDelay--;
		else
		{
			directionChangeDelay = initDirectionChangeDelay;
			directionFlag = !directionFlag;
		}

		//for warp enemy
		warpTimer++;
		if (warpTimer == maxWarpTimer && type == enemyType::WARP)
		{
			warp();
			warpTimer = 0;
		}

		move();
		acceleration = Vector{ 0.f,0.f };
		speed = lerp(speed, { 0.f,0.f }, coreDeceleration);



		Vector targetVector = targetPlayer->getPos2D();

		detectionCounter += globalDeltaTime * (2000 / sqrt((targetVector.x - pos2D.x) * (targetVector.x - pos2D.x) + (targetVector.y - pos2D.y) * (targetVector.y - pos2D.y)));

		if (detectionCounter > detectionCount) { // For the Blinking & sound emit thing
			detectionCounter = 0;
			detected = true;
		}

		if (alpha > 0) {
			color.rgba &= ~alphaMask;
			color.rgba |= unsigned int(alpha);
			alpha -= 50.0f * (fadeSpeed + uniqueBlinkSpeedModifier) * globalDeltaTime;
			if (alpha < 0)
				alpha = 0;
		}
		else {
			if (detected) {
				alpha = float(alphaMask);
				sound.setBuffer(SoundBuffers[audioIndex()]);
				sound.play();
				detected = false;
			}
		}
	}
		else {
			if (whenIsDie < ElapsedTime) {
				this->onHit();
				return;
			}
		}
		(isProjectionOverlayed) ? (draw()) : (show());

}




void enemy::show()
{
	sound.setMinDistance(500.0f); sound.setAttenuation(0.3f);

	push_settings();

	set_fill_color(color);
	no_outline();

	//for warp enemy
	float shakeX = 0.f;
	float shakeY = 0.f;
	if (type == enemyType::WARP)
	{
		shakeX = (float)warpTimer * random(-0.1f, 0.1f);
		shakeY = (float)warpTimer * random(-0.1f, 0.1f);
	}
	if(isDying)
	{
		set_outline_width(3.0f);
		set_outline_color(HexColor{ 0xFFFFFFFF });
		draw_line(pos2DProjected.x - enemyDrawSize /4, pos2DProjected.y - enemyDrawSize / 4, pos2DProjected.x + enemyDrawSize / 4, pos2DProjected.y + enemyDrawSize / 4);
		draw_line(pos2DProjected.x + enemyDrawSize / 4, pos2DProjected.y - enemyDrawSize / 4, pos2DProjected.x - enemyDrawSize / 4, pos2DProjected.y + enemyDrawSize / 4);
	}
	else
		draw_ellipse((pos2DProjected.x + (float)shakeX) / showMultiplier, (pos2DProjected.y + (float)shakeY) / showMultiplier, enemyDrawSize, enemyDrawSize);

	pop_settings();

}
void enemy::onHit()
{
		isDead = true;
		auto it = find(enemyList[gameWave].begin(), enemyList[gameWave].end(), this);
		enemyList[gameWave].erase(it);
		delete this;
}

void enemy::makeDying()
{
	sound.stop();
	sound.setLoop(false);
	sound.setBuffer(SoundBuffers[0]);
	sound.play();
	isDying = true;
	whenIsDie = ElapsedTime + Dyingtime;
}

void enemy::draw()
{
	sound.setMinDistance(100.0f); sound.setAttenuation(0.6f);

	push_settings();

	no_outline();

	HexColor tempColor = color;

	if (isDying)
		tempColor = { random(0, 0xFFFFFF) };

	int tempAlpha = 0;

	if (abs(pos2DProjected.y) < maxSight) // Prevents enemies those are far away from being shown
		tempAlpha = lerp(0, 255, (abs(pos2DProjected.y - maxSight) / (maxDistance)));

	tempColor.rgba &= ~alphaMask;
	tempColor.rgba |= unsigned int(tempAlpha);



	set_fill_color(tempColor);



	float projectedSize = 0;
	if (pos2DProjected.y > 0) { // 2.5D projected size	
		projectedSize = enemyDrawSize3D * enemyDrawSize3DBase / sqrt(pos2DProjected.y * pos2DProjected.y + pos2DProjected.x * pos2DProjected.x);
	}
	else {
		projectedSize = 0;
	}

	//for warp enemy
	float shakeX = 0.f;
	float shakeY = 0.f;
	if (type == enemyType::WARP && !isDying)
	{
		shakeX = (float)warpTimer * random(-0.1f, 0.1f);
		shakeY = (float)warpTimer * random(-0.1f, 0.1f);
	}


	if (isDying) { // Display the crazy blinking when dying
		float startDyingTime = (whenIsDie - Dyingtime);
		float timeElapsed = ElapsedTime - startDyingTime;
		float randomno = random(-5.f, 5.f);
		draw_ellipse(((pos2DProjected.x + shakeX) * enemyDrawSize3DBase / pos2DProjected.y - randomno),
			shakeY, projectedSize * (Dyingtime - timeElapsed) / Dyingtime, projectedSize * (Dyingtime - timeElapsed) / Dyingtime);
	}
	else
		draw_ellipse(((pos2DProjected.x + shakeX) * enemyDrawSize3DBase / pos2DProjected.y), shakeY, projectedSize, projectedSize);

	pop_settings();

}
void enemy::move()
{
	Vector moveDir = targetPlayer->getPos2D() - pos2D;
	switch (type)
	{
	case enemyType::ZIGZAG:
		if (directionFlag)
			rotateVector(moveDir, QUARTER_PI*0.9f);
		else
			rotateVector(moveDir, -QUARTER_PI*0.9f);
		break;
	default:
	break;
	}
	moveDir.toUnitVec();
	acceleration = moveDir * wheelSpeed;
	speed += acceleration; 
	pos2D += speed * globalDeltaTime;
}

void enemy::warp()
{
	Vector warpDirection = (targetPlayer->getPos2D() - pos2D).getUnitVec();
	pos2D += warpDirection * warpDistance;
}

Vector toVector(directionType type) // implicit direction indication to actual vector
{
	Vector newVector;
	switch (type) {
	case directionType::UP:
		newVector = { float(random(-maxAxisDistance, maxAxisDistance)), float(random(maxAxisDistance * 3 / 4, maxAxisDistance)) };
		break;
	case directionType::DOWN:
		newVector = { float(random(-maxAxisDistance, maxAxisDistance)), float(random(-maxAxisDistance, -maxAxisDistance * 3 / 4)) };
		break;
	case directionType::LEFT: 
		newVector = { float(random(-maxAxisDistance, -maxAxisDistance * 3 / 4)), float(random(-maxAxisDistance, maxAxisDistance)) };
		break;
	case directionType::RIGHT:
		newVector = { float(random(-maxAxisDistance, maxAxisDistance * 3 / 4)), float(random(-maxAxisDistance, maxAxisDistance)) };
		break;
	}

	return newVector;
}
