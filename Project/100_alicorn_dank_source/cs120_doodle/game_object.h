/*
  game_object.h

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim
  DoYoon Kim created and structed the class
  SeungGeon Kim

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#pragma once

#include "basic_math.h"
#include <vector>
#include "doodle/doodle.hpp"
using namespace doodle;

using std::vector;



class GameObject
{
	friend bool getFirstObjectHitByRay(const Vector& originPoint, const Vector& endPoint, GameObject*& obj);
public:
	GameObject(const Vector& newPos2D, int nEdges, HexColor color);
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void show() = 0;

	void syncPos2D(Vector& vector) { vector = pos2D; };
	void translatePos2D(const Vector& vector) { pos2D += vector; };
	void projectPos2D(const Vector& vector) { pos2DProjected = vector; };
	
	virtual void onHit();
	virtual void makeDying();
	virtual bool getisDying() {
		return false;
	}

	Vector getPos2D() const { return pos2D; }
	Vector getPos2DProjected() const { return pos2DProjected; }

	vector<Line> getEdges_GlobalPosition()const;
	vector<Line> getEdges_RelativePosition()const;
private:
	bool isCircle = false;
protected:
	Vector pos2D;
	Vector pos2DProjected;

	//relative coordinate to parent.
	vector<Line> edges;
	HexColor color;
};

