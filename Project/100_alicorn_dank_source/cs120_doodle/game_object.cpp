/*
  game_object.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim
  DoYoon Kim wrote this all
  SeungGeon Kim

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include "game_object.h"
#include "variables.h"
#include "doodle/doodle.hpp"
using namespace doodle;



GameObject::GameObject(const Vector& newPos2D, int nEdges, HexColor color)
	:pos2D(newPos2D), color(color)
{
	if(nEdges == circleFlag)
	{
		isCircle = true;
	}
	else
	{
		vector<Vector> points;
		points.reserve(nEdges);

		float d_angle = TWO_PI / (float)nEdges;
		float startAngle = 0.f;
		
		if (nEdges % 2 == 1)
			startAngle += PI / (float)nEdges;

		for(int i = 0; i < nEdges; i++)
		{
			float angle = startAngle + (float)i * d_angle;
			float x = cosf(angle);
			float y = sinf(angle);
			points.push_back(Vector(x, y));
		}

		for (size_t i = 0; i < points.size() - 1; i++)
		{
			const Vector& p1 = points[i];
			const Vector& p2 = points[i + (size_t)1];
			Line edge(p1, p2);
			edges.push_back(edge);
		}
		const Vector& pn = points[points.size() - 1];
		const Vector& p1 = points[0];
		Line edge(p1, pn);
		edges.push_back(edge);
	}
}

void GameObject::onHit()
{
}

void GameObject::makeDying()
{
}

vector<Line> GameObject::getEdges_GlobalPosition() const
{
	vector<Line> Edges;
	for (const Line& line : edges)
	{
		const Vector& p1 = line.getPoints()[0];
		const Vector& p2 = line.getPoints()[1];
		Edges.push_back(Line(p1+pos2D,p2+pos2D));
	}

	return Edges;
}

vector<Line> GameObject::getEdges_RelativePosition() const
{
	return edges;
}
