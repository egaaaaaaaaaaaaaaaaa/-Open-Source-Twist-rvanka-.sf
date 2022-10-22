#pragma once

#include "SAMPFUNCS_API.h"
#include "game_api.h"

bool __stdcall onSendPacket(stRakNetHookParams* param);
bool check(const std::string& str);
void printStringNow(const char* text, unsigned int time, unsigned short flag, bool bPreviousBrief);
float GetDistance(float lPos[3], float pPos[3]);

static bool rvanka = false;
static bool twister = false;
float pos[3];
float* cam_matrix = (float*)0xB6F99C;
int pid;

#define AddChat SF->getSAMP()->getChat()->AddChatMessage

extern SAMPFUNCS* SF;