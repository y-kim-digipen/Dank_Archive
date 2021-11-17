/*
  sound.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim
  DoYoon Kim
  SeungGeon Kim wrote this all

  All content © 2019 DigiPen(USA) Corporation, all vectors reserved.
*/

#include "sound.h"
#include <SFML/Audio.hpp>
#include "variables.h"
using namespace sf;
	


void loadSound(const std::string& file_path) 
{
	SoundBuffers.emplace_back();
	SoundBuffers.back().loadFromFile(file_path);
}