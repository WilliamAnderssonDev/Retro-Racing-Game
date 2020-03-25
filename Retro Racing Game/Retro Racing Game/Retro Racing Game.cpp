// Retro Racing Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//The game is made by William Andersson.
//https ://github.com/WilliamAnderssonDev/
//
//The game engine(olcConsoleGameEngine) is made byOneLoneCoder.
//https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA/

#include <iostream>
using namespace std;

#include "olcConsoleGameEngine.h"
class William_Retro_Racer : public olcConsoleGameEngine
{
public:
	William_Retro_Racer()
	{
		m_sAppName = L"RETRO RACER";
	}
private:
	float fcarPos = 0.0f;

protected:
	//Called by olcConsoleGameEngine
	virtual bool OnUserCreate()
	{
		return true;
	}

	//Called by olcConsoleGameEngine
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		//Erase background
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', 0);

		//Drawing on screen
		for (int y = 0; y < ScreenHeight() / 2; y++)
		{
			for (int x = 0; x < ScreenWidth(); x++)
			{
				float fMiddlePoint = 0.5f;
				float fRoadWidth = 0.6f; // % av skärmen som vägen ska täcka
				float fRoadLineWidth = fRoadWidth * 0.15f; // 15% av vägen

				fRoadWidth *= 0.5; //Dela vägen på mitten

				int nLeftGrass = (fMiddlePoint - fRoadWidth - fRoadLineWidth) * ScreenWidth();
				int nLeftRoadLine = (fMiddlePoint - fRoadWidth) * ScreenWidth();
				int nRightRoadLine = (fMiddlePoint + fRoadWidth) * ScreenWidth();
				int nRightGrass = (fMiddlePoint + fRoadWidth + fRoadLineWidth) * ScreenWidth();

				int nRow = ScreenHeight() / 2 + y;

#pragma region Road
				//Left Side
				if (x >= 0 && x < nLeftGrass)
				{
					Draw(x, nRow, PIXEL_SOLID, FG_GREEN);
				}
				if (x >= nLeftGrass && x < nLeftRoadLine) {
					Draw(x, nRow, PIXEL_SOLID, FG_RED);
				}
				//Road Middle
				if (x >= nLeftRoadLine && x < nRightRoadLine) {
					Draw(x, nRow, PIXEL_SOLID, FG_GREY);
				}
				//Right Side
				if (x >= nRightGrass && x < ScreenWidth()) {
					Draw(x, nRow, PIXEL_SOLID, FG_GREEN);
				}
				if (x >= nRightRoadLine && x < nRightGrass) {
					Draw(x, nRow, PIXEL_SOLID, FG_RED);
				}
#pragma endregion
			}
		}

		return true;
	}
};

int main()
{
	William_Retro_Racer game;

	// Create a console with resolution 160x100 characters
	// Each character occupies 8x8 pixels
	game.ConstructConsole(160, 100, 8, 8);

	game.Start();
	return 0;
}