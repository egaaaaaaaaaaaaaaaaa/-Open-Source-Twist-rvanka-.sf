#pragma once

#include "SAMPFUNCS_API.h"
#include "game_api.h"

bool __stdcall onSendPacket(stRakNetHookParams* param);
bool check(const std::string& str);
void printStringNow(const char* text, unsigned int time, unsigned short flag, bool bPreviousBrief);
float GetDistance(float lPos[3], float pPos[3]);
bool CALLBACK Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
HRESULT CALLBACK Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
bool CALLBACK WndProcHandler(HWND hwd, UINT msg, WPARAM wParam, LPARAM lParam);
void CALLBACK PluginFree();

static bool rvanka = false;
static bool menu = false;
float pos[3];
int pid;

#define AddChat SF->getSAMP()->getChat()->AddChatMessage
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1

extern SAMPFUNCS* SF;