/*
  script.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim added code to read type, direction
  DoYoon Kim
  SeungGeon Kim wrote this all

  All content © 2019 DigiPen(USA) Corporation, all vectors reserved.
*/

#include "script.h"
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include "variables.h"

#include "enemy.h"
using namespace std;



const int MAX_SOURCE_LINE_SIZE = 1024;

#define MAX_COMMAND_SIZE 1024
#define MAX_PARAM_SIZE 1024

#define COMMAND_ADDENEMY "@"
#define COMMAND_DEFINEWAVE "wave"

int g_iScriptSize;
char** g_ppstrScript;
int g_iCurrScriptLineChar;
int g_iCurrScriptLine;
unsigned int curWave;



void GetStringParam(char* pstrDestString) {

	int iParamSize = 0;

	char cCurrChar;

	int iIsStringRead = 0;

	// Move past the opening double quote
	++g_iCurrScriptLineChar;

	// Read all characters until the closing double quote to isolate the string
	while (g_iCurrScriptLineChar <
		strlen(g_ppstrScript[g_iCurrScriptLine])) {

		// Read the next character from the line
		cCurrChar = g_ppstrScript
			[g_iCurrScriptLine][g_iCurrScriptLineChar];

		// If a newline has been read, the command is complete
		if (cCurrChar == '\n') {
			break;
		}

		// If a double quote has been read, check iIsStringRead && move on
		else if (cCurrChar == '"') {
			iIsStringRead = 1;
			++g_iCurrScriptLineChar;
		}

		// If a whitespace is read
		else if (cCurrChar == ' ') {

			// Casual space
			if (iIsStringRead == 0) {

				// Append it to the current command
				pstrDestString[iParamSize] = cCurrChar;

				// Increment the length of the command
				++iParamSize;

				// Move to the next character in the current line
				++g_iCurrScriptLineChar;
			}

			// Checked, sweeping off whitespace
			else if (iIsStringRead == 1) {
				++g_iCurrScriptLineChar;
			}
		}

		// If an escape sequence was read
		else if (cCurrChar == '^') {

			// Append it to the current command
			pstrDestString[iParamSize] = '"';

			// Increment the length of the command
			++iParamSize;

			// Move to the next character in the current line
			++g_iCurrScriptLineChar;
		}

		// Found something, not checked yet
		else if (iIsStringRead == 0) {

			// Append it to the current command
			pstrDestString[iParamSize] = cCurrChar;

			// Increment the length of the command
			++iParamSize;

			// Move to the next character in the current line
			++g_iCurrScriptLineChar;
		}

		// Found something, checked
		else if (iIsStringRead == 1) {
			break;
		}
	}

	// Append a null terminator
	pstrDestString[iParamSize] = '\0';
}

int GetIntParam(void) {

	char pstrString[MAX_PARAM_SIZE];

	int iParamSize = 0;

	char cCurrChar = 0;

	// Read all characters until the next space to isolate the integer
	while (g_iCurrScriptLineChar <
		strlen(g_ppstrScript[g_iCurrScriptLine])) {

		cCurrChar = g_ppstrScript
			[g_iCurrScriptLine][g_iCurrScriptLineChar];

		// If a space (or newline) has been read, the command is comlpete
		if (cCurrChar == ' ' || cCurrChar == '\n')
			break;

		pstrString[iParamSize] = cCurrChar;

		++iParamSize;

		++g_iCurrScriptLineChar;
	}

	// Move past the trailing space
	++g_iCurrScriptLineChar;

	// Append a null terminator
	pstrString[iParamSize] = '\0';

	int iIntValue = atoi(pstrString);

	return iIntValue;
}

int CompareCommand(char* pstrDestString) {

	// return 1 if valid, 2 if unvalid
	if (_stricmp(pstrDestString, COMMAND_ADDENEMY) == 0)
		return 1;
	else
		return 2;
}

void GetCommand(char* pstrDestString) {

	int iCommandSize = 0;

	char cCurrChar = 0;

	int iIsCommandRead = 0;

	// Read all characters until the first space to isolate the command
	while (g_iCurrScriptLineChar < 
		strlen(g_ppstrScript[g_iCurrScriptLine])) {

		cCurrChar = g_ppstrScript
			[g_iCurrScriptLine][g_iCurrScriptLineChar];

		// If a newline has been read, the command is comlpete
		if (cCurrChar == '\n') {
			break;
		} 

		// If a whitespace was found (may be at the start of the line, or at the end of a command)
		else if (cCurrChar == ' ') { 

			// If iIsCommandRead is not checked, call CompareCommand()
			if (iIsCommandRead == 0) {
				iIsCommandRead = CompareCommand(pstrDestString);

				++g_iCurrScriptLineChar;
			}

			// Command is valid, sweeping off whitespace
			else if (iIsCommandRead == 1) {

				++g_iCurrScriptLineChar;
			}

			// Command is invalid
			else if (iIsCommandRead == 2) {

				break;
			}
		} 

		// It's something else, so keep reading if iIsCommandRead is not checked
		else if (iIsCommandRead == 0) { 

			pstrDestString[iCommandSize] = cCurrChar;

			++iCommandSize;

			++g_iCurrScriptLineChar;
		}

		// Found something, and the command is valid
		else if (iIsCommandRead == 1) {

			break;
		}

		// Found something, but the command is invalid
		else if (iIsCommandRead == 2) {

			break;
		}

	}

	// Append a null terminator
	pstrDestString[iCommandSize] = '\0';
}

void UnloadScript(void) {

	// Return immediately if the script is already free
	if (!g_ppstrScript)
		return;

	// Free each line of code individually
	for (int iCurrLineIndex = 0;
		iCurrLineIndex < g_iScriptSize;
		++iCurrLineIndex) {
		delete(g_ppstrScript[iCurrLineIndex]);
	}

	// Free the script from the structure itself
	delete(g_ppstrScript);

}

void RunScript(void) {

	// Allocate strings for holding source substrings
	char pstrCommand[MAX_COMMAND_SIZE] = { 0 };


	//resize max wave before add enemy;
	enemyList.resize((long long(maxWave) + 1));
	tempEnemyList.resize((long long(maxWave) + 1));

	//char pstrStringParam[MAX_PARAM_SIZE] = { 0 };

	// Loop through each line of code and execute it
	for (g_iCurrScriptLine = 0;
		g_iCurrScriptLine < g_iScriptSize;
		++g_iCurrScriptLine) {

		// ---- Process the current line

		// Reset the current character
		g_iCurrScriptLineChar = 0;

		// Syntax improvements
		if (strlen(g_ppstrScript[g_iCurrScriptLine]) == 1 ||
			(g_ppstrScript[g_iCurrScriptLine][0] == '/' &&
				g_ppstrScript[g_iCurrScriptLine][1] == '/')) {

			++g_iCurrScriptLine;

			// Exit the function
			continue;
		}

		// Read the command
		GetCommand(pstrCommand);

		// ---- Execute the command

		// DefineWave
		if (_stricmp(pstrCommand, COMMAND_DEFINEWAVE) == 0) {
			curWave = GetIntParam();
		}

		// AddEnemy
		else if (_stricmp(pstrCommand, COMMAND_ADDENEMY) == 0) {

			Vector pos = toVector(static_cast<directionType>(GetIntParam()));
			enemyType type = static_cast<enemyType>(GetIntParam());

			if (static_cast<int>(type) == 0)
				type = enemyType::MODERATE;

			int startTime = GetIntParam();

			tempEnemyList[curWave].push_back(new enemy( pos , playerList[0], circleFlag, type, static_cast<float>(startTime)));

		}

		// Anything else is invalid
		else {
			printf("\tError: Invalid command.\n");
			break;
		}

		tempEnemyList[curWave].shrink_to_fit();
	}

}

void LoadScript(const char* pstrFilename) {

	// Initialize the script size variable
	g_iScriptSize = 0;

	// Open the file
	ifstream inputFileStream{pstrFilename};

	if (!inputFileStream) {
		cout << "File read failed :(";
		return;
	}

	// Look for \n, and add em all up
	while (!inputFileStream.eof()) {

		char cCurrChar = 0;
		inputFileStream.get(cCurrChar);
		if (cCurrChar == '\n')
			++g_iScriptSize;
	}

	// Add 1 to the size, regarding the fact that there's no \n in the last line
	++g_iScriptSize;

	inputFileStream.clear();
	inputFileStream.seekg(inputFileStream.beg);

	// Allocate a 'script' of the proper size
	g_ppstrScript = new char*[g_iScriptSize];

	if (g_ppstrScript != NULL) {

		// Load each line of code
		for (int iCurrLineIndex = 0;
			iCurrLineIndex < g_iScriptSize;
			++iCurrLineIndex) {

			// Allocate a laaarge space for the line and a null terminator -> \0
			g_ppstrScript[iCurrLineIndex] = new char[MAX_SOURCE_LINE_SIZE + 1];

			string tempLine = "";
			getline(inputFileStream, tempLine);

			tempLine.copy(g_ppstrScript[iCurrLineIndex], tempLine.size() + 1);
			g_ppstrScript[iCurrLineIndex][tempLine.size()] = '\0';
		}

	}

}