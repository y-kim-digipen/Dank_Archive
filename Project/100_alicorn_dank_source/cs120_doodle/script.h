/*
  script.h

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim 
  DoYoon Kim
  SeungGeon Kim wrote this all

  All content © 2019 DigiPen(USA) Corporation, all vectors reserved.
*/

#pragma once



void GetStringParam(char* pstrDestString);

int GetIntParam(void);

int CompareCommand(char* pstrDestString);

void GetCommand(char* pstrDestString);

void UnloadScript(void);

void RunScript(void);

void LoadScript(const char* pstrFilename);