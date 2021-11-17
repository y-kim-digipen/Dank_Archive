/*
  player.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim
  DoYoon Kim
  SeungGeon Kim wrote this all

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include "classes.h"
#include <doodle/doodle.hpp>
#include <cmath>    
#include "module.h"

using namespace doodle;
using namespace std;



void player::update() 
{

	for (Module* instModule : moduleList) { 
		instModule->update(); 
	}

	(isProjectionOverlayed) ? (draw()) : (show());
	if (shakingTime > 0)
		shakingTime--;
}



void player::addModule(Module* module) 
{
	moduleList.push_back(module);
	module->link(this);
}



void player::show() 
{

	push_settings();

	set_fill_color(playerFillColor);
	set_outline_color(playerEdgeColor);

	draw_ellipse(pos2DProjected.x, pos2DProjected.y, playerDrawSize, playerDrawSize);
	draw_ellipse(pos2DProjected.x, pos2DProjected.y, playerDrawSize / 1.3f, playerDrawSize / 1.3f);

	pop_settings();

}

void player::onHit()
{
	life--;
	sound.setBuffer(SoundBuffers[7]);
	sound.play();
	shakingTime = initShakingTime;
}

void player::draw() 
{

	push_settings();

	pop_settings();

}