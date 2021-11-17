/*
  main.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim arranged the core loop
  DoYoon Kim
  SeungGeon Kim 

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include <doodle/doodle.hpp>
using namespace doodle;

#include "variables.h"

#include "classes.h"
#include "sound.h"
#include "game.h"



void onWindowIsNotFocused() 
{
	isUpKeyDown = false;
	isDownKeyDown = false;
	isLeftKeyDown = false;
	isRightKeyDown = false;
}

int main(void) 
{
	create_window(820, 820);
	toggle_full_screen();
	show_cursor(false);

	while (!is_window_closed()) 
	{
		globalDeltaTime = DeltaTime / targetFrameRate;

		newGame.setup();

		if (!WindowIsFocused) onWindowIsNotFocused();

		clear_background(defaultFillColor);

		newGame.update();

		update_window(); // Screen gets updated here, don't forget

	}

	return 0;
}