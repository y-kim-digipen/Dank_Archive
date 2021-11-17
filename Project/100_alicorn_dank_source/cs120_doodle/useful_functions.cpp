/*
  useful_functions.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim
  DoYoon Kim wrote all
  SeungGeon Kim

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include "useful_functions.h"
#include "game_object.h"
#include "variables.h"



bool getFirstObjectHitByRay(const Vector& originPoint, const Vector& endPoint, GameObject*& p_obj)
{
	vector<GameObject*> objects;
	objects.insert(objects.end(), enemyList[gameWave].begin(), enemyList[gameWave].end());
	sort(objects.begin(), objects.end(), [&originPoint](GameObject* obj1,GameObject* obj2) -> bool	{
		float dist1 = (obj1->getPos2D() - originPoint).sqrLength();
		float dist2 = (obj2->getPos2D() - originPoint).sqrLength();
		return dist1 < dist2;
	});

	Line ray(originPoint, endPoint);
	for (GameObject* obj : objects)
	{
		if (obj->isCircle)
		{
			float ray_dir_x = ray.getDirectionVector().x;
			float ray_dir_y = ray.getDirectionVector().y;
			Vector perp_rayDirection(-ray_dir_y,ray_dir_x);
			perp_rayDirection.toUnitVec();

			Vector intersectingPoint;
			Vector radiusVec = perp_rayDirection * enemyDrawSize / 2.f;
			bool isIntersecting = get_Intersect_Point_From_2_Lines(obj->pos2D - radiusVec, obj->pos2D + radiusVec, ray.getPoints()[0], ray.getPoints()[1], intersectingPoint);
			if (isIntersecting)
			{
				p_obj = obj;
				return true;
			}
		}
		else
		{
			const vector<Line>& edges = obj->getEdges_GlobalPosition();
			for (const Line& edge : edges)
			{
				Vector intersectingPoint;
				bool isIntersecting = get_Intersect_Point_From_2_Lines(ray, edge, intersectingPoint);
				if (isIntersecting)
				{
					p_obj = obj;
					return true;
				}
			}
		}
	}

	p_obj = nullptr;
	return false;
}

