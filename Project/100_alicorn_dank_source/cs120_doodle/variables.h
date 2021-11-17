/*
  variables.h

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim added tempenemylist to manage enemybuffer
  DoYoon Kim added some colors
  SeungGeon Kim added the key and the color related vairables and managed the variables

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#pragma once

#include <doodle/doodle.hpp>
using namespace doodle;

#include "SFML/Audio.hpp"
#include <vector>
#include "game.h"



constexpr int playerLife = 5;
constexpr int targetFrameRate = 1;

constexpr int circleFlag = 1;

constexpr int alphaMask = 0x000000FF;
constexpr int fadeSpeed = 6;

constexpr int showMultiplier = 2;
constexpr int maxAxisDistance = 1500;
constexpr int maxDistance = 2200;
constexpr int maxSight = 1000;

constexpr float wheelSpeedMax = 6.0f;
constexpr float wheelAcceleration = 3.0f;
constexpr float wheelDeceleration = 0.07f;

constexpr float coreTurnMax = (TWO_PI / 360) * 2;
constexpr float coreAcceleration = (TWO_PI / 360) * 4;
constexpr float coreDeceleration = 0.1f;

extern float coreAccelerationLeft;
extern float coreAccelerationRight;

constexpr float enemyDrawSize3D = 100.0f;
constexpr float enemyDrawSize3DBase = 1000.0f;

constexpr float playerDrawSize = 35.0f;
constexpr float enemyDrawSize = 70.0f;
constexpr float missileDrawSize = 15.f;
constexpr float maxCannonRange = 1000.f;
constexpr float baseCannonDrawDistance = 800.f;
constexpr float maxCannonWidth = 400.f;
constexpr float windowBaseDepth = 550.f;

extern float globalDeltaTime;

inline bool isStereoReversed = false;

extern bool isUpKeyDown;
extern bool isDownKeyDown;
extern bool isLeftKeyDown;
extern bool isRightKeyDown;
extern bool isSpaceKeyDown;
extern bool isESCKeyDown;

extern bool isProjectionOverlayed;

extern HexColor defaultFillColor;
constexpr float defaultEdgeWidth = 1.5f;
extern HexColor defaultEdgeColor;

extern HexColor playerFillColor;
extern HexColor playerEdgeColor;

extern HexColor red1;
extern HexColor red2;
extern HexColor red3;
extern HexColor red4;
extern HexColor red5;

inline HexColor blue1 = 0x011f4bAA;
inline HexColor blue2 = 0x03396cAA;
inline HexColor blue3 = 0x6497b1AA;

inline HexColor green1 = 0x30cb00AA;

#include "enemy.h"
#include "player.h"

class enemy;


extern std::vector<player*> playerList;

//tempEnemyList store enemy info before came out
extern std::vector<std::vector<enemy*>> tempEnemyList;
//EnemyList store enemy info after came out
extern std::vector<std::vector<enemy*>> enemyList;

extern vector<sf::SoundBuffer> SoundBuffers;
extern vector<sf::Sound>       Sounds;

extern sf::Music music;

extern Game newGame;
extern unsigned int gameWave;
extern unsigned int maxWave;

