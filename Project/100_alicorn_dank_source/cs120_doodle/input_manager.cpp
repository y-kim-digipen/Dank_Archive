/*
  input_manager.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim
  DoYoon Kim
  SeungGeon Kim wrote this all

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include <doodle/doodle.hpp>
using namespace doodle;

#include "variables.h"
#include "sound.h"



void on_key_pressed(KeyboardButtons button) {
	switch (button) {
	case KeyboardButtons::W:
		isUpKeyDown = true;
		break;
	case KeyboardButtons::A:
		isLeftKeyDown = true;
		break;
	case KeyboardButtons::S:
		isDownKeyDown = true;
		break;
	case KeyboardButtons::D:
		isRightKeyDown = true;
		break;
	case KeyboardButtons::R:
		isProjectionOverlayed = !isProjectionOverlayed;
		break;
	case KeyboardButtons::Space:
		isSpaceKeyDown = true;
		break;
	case KeyboardButtons::Escape:
		isESCKeyDown = true;
		break;
	}
}

void on_key_released(KeyboardButtons button) {
	switch (button) {
	case KeyboardButtons::W:
		isUpKeyDown = false;
		break;
	case KeyboardButtons::A:
		isLeftKeyDown = false;
		break;
	case KeyboardButtons::S:
		isDownKeyDown = false;
		break;
	case KeyboardButtons::D:
		isRightKeyDown = false;
		break;
	case KeyboardButtons::NumPad_5:
		isStereoReversed = !isStereoReversed;
		break;
	case KeyboardButtons::Space:
		isSpaceKeyDown = false;
		break;
	case KeyboardButtons::Escape:
		isESCKeyDown = false;
		break;
	}
}