/*
  useful_functions.h

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim
  DoYoon Kim wrote all
  SeungGeon Kim

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#pragma once

#include "basic_math.h"



class GameObject;

bool getFirstObjectHitByRay(const Vector& originPoint, const Vector& endPoint, GameObject*& p_obj);


