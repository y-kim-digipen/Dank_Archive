/*
  game.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim made this all
  DoYoon Kim
  SeungGeon Kim 

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include "game.h"
#include "classes.h"
#include "script.h"
#include "SFML/Audio.hpp"

#include <iostream>



void operator++(gameState& curState)
{
	if (curState == gameState::CREDIT)
		curState = gameState::GAMEPLAY;
	else {
		int tempState = static_cast<int>(curState);
		tempState++;
		curState = static_cast<gameState>(tempState);
	}
}

void operator--(gameState& curState)
{
	if (curState == gameState::GAMEPLAY)
		curState = gameState::CREDIT;
	else {
		int tempState = static_cast<int>(curState);
		tempState--;
		curState = static_cast<gameState>(tempState);
	}
}

void Game::setup()
{
	if (!isSetted) {

		//statePtr = nullptr;
		switch (currentState) {
		case gameState::GAMEPLAY: {
			playerList.clear();
			enemyList.clear();
			statePtr = new GamePlay;
			isSetted = true;
			break;
		}
		case gameState::CREDIT:
		{
			statePtr = new Credit;
			isSetted = true;
			break;
		}
		case gameState::MAINMENU: {
			//delete statePtr;
			statePtr = new MainMenu;
			isSetted = true;
			break;
		}
		case gameState::GAMEOVER: {
			for (enemy* enemyInst : enemyList[gameWave]) {
				sf::Sound& tempSound = enemyInst->audioSource();
				tempSound.stop();
			}
			statePtr = new GameOver;
			isSetted = true;
			break;
		}
		case gameState::HOWTOPLAY: {
			statePtr = new HowToPlay;
			isSetted = true;
			break;
		}
		default: {
			statePtr = new SplashLogo;
			isSetted = true;
			break;
		}
		}
		statePtr->setup();
	}

}

void Game::update()
{
	if (statePtr != nullptr && isSetted)
		statePtr->update();
	else return;
}

void Game::toState(const gameState& state)
{
	isSetted = false;
	currentState = state;
}


void SplashLogo::setup()
{

	DigipenSplash.LoadFromPNG("assets/DigiPen_WHITE_1024px.png");
	DigipenSplash.SetSmooth(true);

	set_texture_mode(RectMode::Center);

}

void SplashLogo::update()
{
	dt_ += DeltaTime;

	push_settings();
	set_fill_color(defaultFillColor);
	set_outline_width(defaultEdgeWidth);
	set_outline_color(defaultEdgeColor);
	draw_texture(DigipenSplash, 0.0f, 0.0f, 600, 150);
	pop_settings();

	if (dt_ > 2.0f) {
		newGame.toState(gameState::MAINMENU);
	}
}

void GamePlay::setup()
{
	sf::Listener::setPosition(0.0f, 0.0f, 0.0f);

	loadSound("assets/enemy_destroy.wav");
	loadSound("assets/enemy_1.wav");
	loadSound("assets/enemy_1.wav");
	loadSound("assets/enemy_1.wav");
	loadSound("assets/enemy_zigzag.wav");
	loadSound("assets/enemy_warp.wav");
	loadSound("assets/enemy_superfast.wav");
	loadSound("assets/player_hit.wav");

	playerList.push_back(new player({ 0.0f, 0.0f }));
	playerList.back()->addModule(new Wheel());
	playerList.back()->addModule(new Eye());
	playerList.back()->addModule(new Ear());
	playerList.back()->addModule(new Cannon());

	//before actual gameplay starts, initialize gameWave to 1
	gameWave = 1;


	//Load Enemy Info
	LoadScript("scripts/script.txt");
	RunScript();
	UnloadScript();


	set_frame_of_reference(RightHanded_OriginCenter);
	set_ellipse_mode(EllipseMode::Center);
	set_rectangle_mode(RectMode::Center);
}

void drawUI() {

	push_settings();

	set_fill_color(defaultFillColor);
	no_outline();
	set_fill_color(red5);
	float fontSize = 25.f;
	set_font_size(fontSize);

	string Wave = " Wave " + to_string(gameWave);
	string Life = " Life " + to_string(playerList[0]->getLife());
	string EnemySize = " Enemy Left " + to_string(enemyList[gameWave].size());


	push_settings();
	set_fill_color( blue1 );
	draw_rectangle(-Width * 3.2f / 8, Height * 3.f / 7.f + fontSize * 3.0f / 4, EnemySize.length() * fontSize ,EnemySize.length() * fontSize*1.2f);

	pop_settings();

	draw_text(Wave, -Width * 3.7f / 8, Height * 3.f / 7.f);

	draw_text(Life, -Width * 3.7f / 8, Height * 2.5f / 7.f);

	draw_text(EnemySize, -Width * 3.7f / 8, Height * 2.f / 7.f);

	pop_settings();

}

void GamePlay::update()
{
	if (isESCKeyDown) {
		for (enemy* enemyInst : enemyList[gameWave]) {
			sf::Sound& tempSound = enemyInst->audioSource();
			tempSound.stop();
		}
		newGame.toState(gameState::MAINMENU);
		isESCKeyDown = false;
	}

	push_settings();
	set_fill_color(defaultFillColor);
	set_outline_width(defaultEdgeWidth);
	set_outline_color(defaultEdgeColor);
	pop_settings();

	if (enemyList[gameWave].size() == 0 && tempEnemyList[gameWave].size() == 0) {
		if (gameWave == maxWave) {
			gameWave = maxWave;
			LoadScript("scripts/script.txt");
			RunScript();
			UnloadScript();
		}
		else
			gameWave++;

		for (enemy* enemyInst : tempEnemyList[gameWave])
			enemyInst->setEmergenceTime(ElapsedTime);
	}



	enemyEmergence();

	for (enemy* instEnemy : enemyList[gameWave])
		instEnemy->update();

	for (player* instPlayer : playerList) {
		instPlayer->update();
		if(instPlayer->getLife() <= 0)
			newGame.toState(gameState::GAMEOVER);
	};

	drawUI();
}



void Credit::setup()
{
	Credit.LoadFromPNG("assets/Credit.png");
	Credit.SetSmooth(true);

	set_texture_mode(RectMode::Center);
}

void Credit::update()
{
	if (isESCKeyDown){
		newGame.toState(gameState::MAINMENU);
		isESCKeyDown = false;
	}
		push_settings();
		draw_texture(Credit, 0.0f, 0.0f, (float)Credit.GetWidth(), (float)Credit.GetHeight());
		pop_settings();
}

void MainMenu::setup()
{
	buttonList.push_back(new Button(-Width / 4.0f, 0, Width / 2.0f, Height / 10.0f, HexColor{ 0x000000 }, gameState::GAMEPLAY));
	buttonList.push_back(new Button(-Width / 4.0f, -Height / 6.0f, Width / 2.0f, Height / 10.0f, HexColor{ 0x000000 }, gameState::HOWTOPLAY));
	buttonList.push_back(new Button(-Width / 4.0f, -Height * 2 / 6.0f, Width / 2.0f, Height / 10.0f, HexColor{ 0x000000 }, gameState::CREDIT));

	title.LoadFromPNG("assets/Title.png");
	title.SetSmooth(true);

	set_frame_of_reference(RightHanded_OriginCenter);
	set_ellipse_mode(EllipseMode::Corner);
	set_rectangle_mode(RectMode::Corner);
}

void MainMenu::update()
{
	if (isUpKeyDown) {
		--playerSelect;
		isUpKeyDown = false;
	}
	if (isDownKeyDown) {
		++playerSelect;
		isDownKeyDown = false;
	}
	if (isSpaceKeyDown) {
		newGame.toState(playerSelect);
		isSpaceKeyDown = false;
	}
	if (isESCKeyDown) {
		close_window();
		isESCKeyDown = false;
		return;
	}

	for (Button* button : buttonList) {
		string name;
		push_settings();
		set_fill_color(button->color);
		draw_rectangle(button->pos.x, button->pos.y, button->width, button->height);
		pop_settings();

		switch (button->name) {
		case gameState::GAMEPLAY: {
			name = "PLAY";
			break;
		}
		case gameState::CREDIT:
		{
			name = "CREDIT";
			break;
		}
		case gameState::HOWTOPLAY: {
			name = "HOW TO PLAY";
			break;
		}
		}

		if (playerSelect == button->name) {
			push_settings();
			no_fill();
			set_outline_color(playerEdgeColor);
			draw_rectangle(button->pos.x, button->pos.y, button->width, button->height);
			set_fill_color(red2);
			pop_settings();

		}
		string CopyRight = "All content (c) 2019 DigiPen (USA) Corporation, all rights reserved.";

		push_settings();
		draw_texture(title, 0.f, Height / 4.f, title.GetWidth() / 2.f, title.GetHeight() / 2.f);
		float menuFontSize = button->height / 2; 
		set_font_size(menuFontSize);
		set_fill_color(red5);
		int strLen = (int)name.length();
		draw_text(name, - ((strLen / 2.f) * menuFontSize*3/4), button->pos.y);
		strLen = (int)CopyRight.length();
		set_font_size(menuFontSize/2.3f);
		set_fill_color(HexColor{ 0xFFFFFFFF });
		draw_text(CopyRight, -((strLen / 2.f) * menuFontSize/2.f * 2.2f / 4), -Height * 3.f / 7.f);
		
		pop_settings();
	}
}

void GameOver::setup()
{
}

void GameOver::update()
{
	if (isESCKeyDown) {
		newGame.toState(gameState::MAINMENU);
		isESCKeyDown = false;
		return;
	}
	push_settings();
	set_fill_color(defaultFillColor);
	no_outline();
	set_fill_color(red5);
	set_font_size(50.0f);
	string GameOverS = "Game  Over";
	draw_text(GameOverS, - (float)GameOverS.length()/2.f * 50.0f * 3 / 4, 0);
	pop_settings();
}

void enemyEmergence()
{
	for (vector<enemy*>::iterator it = tempEnemyList[gameWave].begin(); it != tempEnemyList[gameWave].end(); ) {
		enemy* const tempPtr = *it;
		if (tempPtr->getEmergenceTime() < ElapsedTime)
		{ 
			enemy* tempEnemyPtr = new enemy(*tempPtr);
			enemyList[gameWave].push_back(tempEnemyPtr);
			it = tempEnemyList[gameWave].erase(it); 
		}
		else ++it; 
	}

}

void HowToPlay::setup()
{
	Instructions.LoadFromPNG("assets/Instruction.png");
	Instructions.SetSmooth();
	GameGoal.LoadFromPNG("assets/GG.png");
	GameGoal.SetSmooth();
}

void HowToPlay::update()
{
	if (isESCKeyDown) {
		newGame.toState(gameState::MAINMENU);
		isESCKeyDown = false;
		return;
	}
	if (isSpaceKeyDown) {
		isGameGoal = false;
		isSpaceKeyDown = false;
	}
	if (isGameGoal)
		draw_texture(GameGoal, 0.f, 0.f);
	else
		draw_texture(Instructions, 0.f, 0.f);
}
