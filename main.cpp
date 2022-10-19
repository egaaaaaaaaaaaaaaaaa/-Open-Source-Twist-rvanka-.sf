#include <Windows.h>
#include "main.hpp"
#include "math.h"

SAMPFUNCS* SF = new SAMPFUNCS();

void __stdcall mainloop()
{
	static bool initialized = false;
	if (!initialized)
	{
		if (GAME && GAME->GetSystemState() == eSystemState::GS_PLAYING_GAME && SF->getSAMP()->IsInitialized())
		{
			initialized = true;

			SF->getRakNet()->registerRakNetCallback(RakNetScriptHookType::RAKHOOK_TYPE_OUTCOMING_PACKET, onSendPacket);

			SF->getSAMP()->registerChatCommand("ega", [](std::string id) {
				if (rvanka)
				{
					rvanka = false;
					AddChat(-1, "Долетались :(");
				}
				else
				{
					CVehicle* vehicle = PEDSELF->GetVehicle(); 
					if (vehicle != 0)
					{
						if (check(id) && std::stoi(id) >= 0)
						{
							if (SF->getSAMP()->getPlayers()->isListed[std::stoi(id)] != 1)
							{ 
								AddChat(-1, "Плеер потерялся :/.");
							} 
							else
							{
								if (SF->getSAMP()->getPlayers()->remotePlayerInfo[std::stoi(id)]->data->vehicleID == 0)
								{
									pos[0] = SF->getSAMP()->getPlayers()->remotePlayerInfo[std::stoi(id)]->data->onFootPos[0];
									pos[1] = SF->getSAMP()->getPlayers()->remotePlayerInfo[std::stoi(id)]->data->onFootPos[1];
									pos[2] = SF->getSAMP()->getPlayers()->remotePlayerInfo[std::stoi(id)]->data->onFootPos[2];
								}
								else
								{
									pos[0] = SF->getSAMP()->getPlayers()->remotePlayerInfo[std::stoi(id)]->data->inCarData.position[0];
									pos[1] = SF->getSAMP()->getPlayers()->remotePlayerInfo[std::stoi(id)]->data->inCarData.position[1];
									pos[2] = SF->getSAMP()->getPlayers()->remotePlayerInfo[std::stoi(id)]->data->inCarData.position[2];
								}
								
								D3DXVECTOR3 player(SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->localPlayerInfo.id)->position[0], SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->localPlayerInfo.id)->position[1], SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->localPlayerInfo.id)->position[2]);

								float dist = GetDistance(pos, player);
								
								if (dist <= 29)
								{
									rvanka = true;
									pid = std::stoi(id);
									AddChat(-1, "Ну че, полетели.");
								}
								else
								{
									AddChat(-1, "Чувачок далеко чет.");
								}
							}
						}
						else
						{
							AddChat(-1, "Введи норм ID, балбес.");
						}
					}
					else
					{
						AddChat(-1, "Ты не в каре -__-");
					}
				}
			});

			AddChat(-1, "Рванка в$ка.");
		}
	}
	else
	{
		if (rvanka)
		{
			CVehicle* vehicle = PEDSELF->GetVehicle();
			if (vehicle == 0)
			{
				rvanka = false;
				AddChat(-1, "Ты не в каре.");
			}
			else
			{
				if (SF->getSAMP()->getPlayers()->remotePlayerInfo[pid]->data->vehicleID == 0)
				{
					pos[0] = SF->getSAMP()->getPlayers()->remotePlayerInfo[pid]->data->onFootPos[0];
					pos[1] = SF->getSAMP()->getPlayers()->remotePlayerInfo[pid]->data->onFootPos[1];
					pos[2] = SF->getSAMP()->getPlayers()->remotePlayerInfo[pid]->data->onFootPos[2];
				}
				else
				{
					pos[0] = SF->getSAMP()->getPlayers()->remotePlayerInfo[pid]->data->inCarData.position[0];
					pos[1] = SF->getSAMP()->getPlayers()->remotePlayerInfo[pid]->data->inCarData.position[1];
					pos[2] = SF->getSAMP()->getPlayers()->remotePlayerInfo[pid]->data->inCarData.position[2];
				}

				D3DXVECTOR3 player(SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->localPlayerInfo.id)->position[0], SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->localPlayerInfo.id)->position[1], SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->localPlayerInfo.id)->position[2]);

				float dist = GetDistance(pos, player);

				if (dist <= 29)
				{
					BitStream BsVehicleSync;
					BsVehicleSync.Write((BYTE)ID_VEHICLE_SYNC);
					SF->getRakNet()->SendPacket(&BsVehicleSync);
				}
				else
				{
					rvanka = false;
					AddChat(-1, "Сэбався он далеко оч.");
				}
			}
		}
	}
}

bool __stdcall onSendPacket(stRakNetHookParams* param)
{
	if (rvanka)
	{
		if (param->bitStream->GetData()[0] == ID_VEHICLE_SYNC)
		{
			param->bitStream->ResetReadPointer();
			param->bitStream->IgnoreBits(8);
			InCarData data = { 0 };
			param->bitStream->Read((PCHAR)&data, sizeof(InCarData));

			/* data.position[0] = pos[0];
			data.position[1] = pos[1];
			data.position[2] = pos[2] - 0.5f; 
			data.moveSpeed[0] = 0;
			data.moveSpeed[1] = 0;
			data.moveSpeed[2] = 1.0f; */

			data.position[0] = pos[0];
			data.position[1] = pos[1];
			data.position[2] = pos[2];
			data.quaternion[3] = 1.0f;

			printStringNow("Send Packet", 100, 1, true);

			param->bitStream->ResetWritePointer();
			param->bitStream->Write<unsigned __int8>(ID_VEHICLE_SYNC);
			param->bitStream->Write((PCHAR)&data, sizeof(InCarData));
		}
	}

	return true;
}

void printStringNow(const char* text, unsigned int time, unsigned short flag, bool bPreviousBrief)
{
	((void(__cdecl*)(const char*, unsigned int, unsigned short, bool))0x69F1E0)(text, time, flag, bPreviousBrief);
}

float GetDistance(float lPos[3], float pPos[3]) {
	return sqrt((lPos[0] - pPos[0]) * (lPos[0] - pPos[0]) + (lPos[1] - pPos[1]) * (lPos[1] - pPos[1]) + (lPos[2] - pPos[2]) * (lPos[2] - pPos[2]));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
	if (dwReasonForCall == DLL_PROCESS_ATTACH)
		SF->initPlugin(mainloop, hModule);
	return TRUE;
}
