/*
  variables.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim added tempenemylist to manage enemybuffer
  DoYoon Kim
  SeungGeon Kim added the color related variables and and managed the variables

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include "variables.h"
#include "SFML/Audio.hpp"
using namespace std;

#include "classes.h"



HexColor defaultFillColor = 0x000000AA;
HexColor defaultEdgeColor = 0xFFC837AA;

HexColor playerFillColor = 0x232D37FF;
HexColor playerEdgeColor = 0xFFC837FF;

HexColor red1 = 0xF9C73FFF;
HexColor red2 = 0xFFE046FF;
HexColor red3 = 0xFF4E41FF;
HexColor red4 = 0xDA3330FF;
HexColor red5 = 0xA21212FF;

vector<player*> playerList;

vector<vector<enemy*>> tempEnemyList;
vector<vector<enemy*>> enemyList;

vector<sf::SoundBuffer> SoundBuffers{};
vector<sf::Sound>       Sounds{};

sf::Music music;

Game newGame;

unsigned int gameWave = 1;
unsigned int maxWave = 5;

float globalDeltaTime = 0;

float coreAccelerationLeft = (TWO_PI / 360) * 4;
float coreAccelerationRight = (TWO_PI / 360) * 4;

bool isUpKeyDown = false;
bool isDownKeyDown = false;
bool isLeftKeyDown = false;
bool isRightKeyDown = false;
bool isSpaceKeyDown = false;
bool isESCKeyDown = false;

bool isProjectionOverlayed = false;