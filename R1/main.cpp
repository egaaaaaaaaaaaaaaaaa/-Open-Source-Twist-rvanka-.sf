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

			SF->getSAMP()->registerChatCommand("ega", [](std::string id) {
				if (rvanka)
				{
					rvanka = false;
					AddChat(-1, "���������� :(");
				}
				else
				{
					CVehicle* vehicle = PEDSELF->GetVehicle(); 
					if (vehicle != 0)
					{
						if (check(id) && std::stoi(id) >= 0)
						{
							if (SF->getSAMP()->getPlayers()->iIsListed[std::stoi(id)] != 1)
							{ 
								AddChat(-1, "����� ��������� :/.");
							} 
							else
							{
								if (SF->getSAMP()->getPlayers()->pRemotePlayer[std::stoi(id)]->pPlayerData->sVehicleID == 0)
								{
									pos[0] = SF->getSAMP()->getPlayers()->pRemotePlayer[std::stoi(id)]->pPlayerData->fOnFootPos[0];
									pos[1] = SF->getSAMP()->getPlayers()->pRemotePlayer[std::stoi(id)]->pPlayerData->fOnFootPos[1];
									pos[2] = SF->getSAMP()->getPlayers()->pRemotePlayer[std::stoi(id)]->pPlayerData->fOnFootPos[2];
								}
								else
								{
									pos[0] = SF->getSAMP()->getPlayers()->pRemotePlayer[std::stoi(id)]->pPlayerData->inCarData.fPosition[0];
									pos[1] = SF->getSAMP()->getPlayers()->pRemotePlayer[std::stoi(id)]->pPlayerData->inCarData.fPosition[1];
									pos[2] = SF->getSAMP()->getPlayers()->pRemotePlayer[std::stoi(id)]->pPlayerData->inCarData.fPosition[2]; 
								} 
								
								D3DXVECTOR3 player(SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->sLocalPlayerID)->fPosition[0], SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->sLocalPlayerID)->fPosition[1], SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->sLocalPlayerID)->fPosition[2]);

								float dist = GetDistance(pos, player);
								
								if (dist <= 29)
								{
									rvanka = true;
									pid = std::stoi(id);
									AddChat(-1, "�� ��, ��������.");
								}
								else
								{
									AddChat(-1, "������� ������ ���.");
								}
							}
						}
						else
						{
							AddChat(-1, "����� ���� ID, ������.");
						}
					}
					else
					{
						AddChat(-1, "�� �� � ���� -__-");
					}
				}
			});

			AddChat(-1, "������ �$��.");
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
				AddChat(-1, "�� �� � ����.");
			}
			else
			{ 
				if (SF->getSAMP()->getPlayers()->pRemotePlayer[pid]->pPlayerData->sVehicleID == 0)
				{
					pos[0] = SF->getSAMP()->getPlayers()->pRemotePlayer[pid]->pPlayerData->fOnFootPos[0];
					pos[1] = SF->getSAMP()->getPlayers()->pRemotePlayer[pid]->pPlayerData->fOnFootPos[1];
					pos[2] = SF->getSAMP()->getPlayers()->pRemotePlayer[pid]->pPlayerData->fOnFootPos[2];
				}
				else
				{ 
					pos[0] = SF->getSAMP()->getPlayers()->pRemotePlayer[pid]->pPlayerData->inCarData.fPosition[0];
					pos[1] = SF->getSAMP()->getPlayers()->pRemotePlayer[pid]->pPlayerData->inCarData.fPosition[1];
					pos[2] = SF->getSAMP()->getPlayers()->pRemotePlayer[pid]->pPlayerData->inCarData.fPosition[2];
				}

				D3DXVECTOR3 player(SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->sLocalPlayerID)->fPosition[0], SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->sLocalPlayerID)->fPosition[1], SF->getSAMP()->getPlayers()->GetInCarData(SF->getSAMP()->getPlayers()->sLocalPlayerID)->fPosition[2]);

				float dist = GetDistance(pos, player);

				if (dist <= 29)
				{
					stInCarData data = SF->getSAMP()->getPlayers()->pLocalPlayer->inCarData;

					data.fPosition[0] = pos[0];
					data.fPosition[1] = pos[1];
					data.fPosition[2] = pos[2];
					data.fQuaternion[3] = 1.0f;

					BitStream bs;
					bs.Write<unsigned __int8>(ID_VEHICLE_SYNC);
					bs.Write((PCHAR)&data, sizeof(stInCarData));
					SF->getRakNet()->SendPacket(&bs);

					printStringNow("Send Packet", 160, 1, true);
				}
				else
				{
					rvanka = false;
					AddChat(-1, "������� �� ������ ��.");
				}
			}
		}
		else
		{
			if (SF->getGame()->isKeyPressed(67))
			{
				if (twister)
				{
					twister = false;
					printStringNow("InCar Rvanka - ~r~OFF", 1500, 1, true);
				}
				else
				{
					CVehicle* vehicle = PEDSELF->GetVehicle();
					if (vehicle == 0)
					{
						AddChat(-1, "�� �� � ����.");
					}
					else
					{
						twister = true;
						printStringNow("InCar Rvanka - ~g~ON", 1500, 1, true);
					}
				}
			}
			if (twister)
			{
				CVehicle* vehicle = PEDSELF->GetVehicle();
				if (vehicle == 0)
				{
					twister = false;
					AddChat(-1, "�� �� � ����.");
				}
				else
				{
					stInCarData data = SF->getSAMP()->getPlayers()->pLocalPlayer->inCarData;

					data.fMoveSpeed[0] = cam_matrix[4] * 1;
					data.fMoveSpeed[1] = cam_matrix[5] * 1;
					data.fMoveSpeed[2] = cam_matrix[6] * 1;
					data.fQuaternion[3] = 1.0f;

					BitStream bs;
					bs.Write<unsigned __int8>(ID_VEHICLE_SYNC);
					bs.Write((PCHAR)&data, sizeof(stInCarData));
					SF->getRakNet()->SendPacket(&bs);

					printStringNow("Send Packet", 100, 1, true);
				}
			}
		}
	}
}

void printStringNow(const char* text, unsigned int time, unsigned short flag, bool bPreviousBrief)
{
	((void(__cdecl*)(const char*, unsigned int, unsigned short, bool))0x69F1E0)(text, time, flag, bPreviousBrief);
}

bool check(const std::string& str)
{
	for (auto it = str.cbegin(); it != str.cend(); it++)
		if (!isdigit(*it) && (it != str.cbegin() || *it != '-' || str.length() == 1))
			return false;

	return !str.empty();
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