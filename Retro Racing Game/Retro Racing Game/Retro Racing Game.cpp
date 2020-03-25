// Retro Racing Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//The game is made by William Andersson.
//https://github.com/WilliamAnderssonDev/
//
//The game engine(olcConsoleGameEngine) is made byOneLoneCoder.
//http://www.youtube.com/javidx9

#include <iostream>
#include <string>
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
	int lap = 1;
	float fCurrentLapTime = 0;

	float fCarPos = 0.0f;
	float fDistance = 0.0f;
	float fSpeed = 0.0f;

	float fCurvature = 0.0f;
	float fTrackCurvature = 0.0f;
	float fPlayerCurvature = 0.0f;
	float fTrackDistance = 0.0f;

	vector<pair<float, float>> vecTrack1; // road curvature, distance
	list<float> listLapTimes;

protected:
	//Called by olcConsoleGameEngine
	virtual bool OnUserCreate()
	{
		//Track 1
		vecTrack1.push_back(make_pair(0.0f, 15.0f)); // Start / Finish Line
		vecTrack1.push_back(make_pair(0.0f, 100.0f)); // Start / Finish Line Lap updater
		vecTrack1.push_back(make_pair(0.0f, 200.0f)); // Long straight section
		vecTrack1.push_back(make_pair(1.0f, 200.0f)); // Sharp Curve Right
		vecTrack1.push_back(make_pair(0.0f, 400.0f)); // Extra Long straight section
		vecTrack1.push_back(make_pair(-1.0f, 200.0f)); // Sharp Curve Left
		vecTrack1.push_back(make_pair(1.0f, 200.0f)); // Sharp Curve Right
		vecTrack1.push_back(make_pair(0.0f, 500.0f)); // Extra Long straight section
		vecTrack1.push_back(make_pair(0.5f, 400.0f)); // Extra Long bent section
		vecTrack1.push_back(make_pair(0.0f, 150.0f)); // Long straight section
		vecTrack1.push_back(make_pair(-0.8f, 250.0f)); // Sharp Curve Left
		vecTrack1.push_back(make_pair(1.0f, 200.0f)); // Sharp Curve Right

		for (auto t : vecTrack1) {
			fTrackDistance += t.second;

			listLapTimes = { 0,0,0,0,0 };
		}

		return true;
	}

	//Called by olcConsoleGameEngine
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		//Key inputs
		if (m_keys[VK_UP].bHeld)
		{
			fSpeed += 2.0f * fElapsedTime;
		}
		else
		{
			fSpeed -= 0.8f * fElapsedTime;
		}

		int nCarDirection = 0;

		if (m_keys[VK_LEFT].bHeld)
		{
			fPlayerCurvature -= 0.7f * fElapsedTime;
			nCarDirection = -1;
		}
		if (m_keys[VK_RIGHT].bHeld)
		{
			fPlayerCurvature += 0.7f * fElapsedTime;
			nCarDirection = 1;
		}
		if (fabs(fPlayerCurvature - fTrackCurvature) >= 0.8f) {
			fSpeed -= 5.0f * fElapsedTime;
		}
		if (fabs(fPlayerCurvature - fTrackCurvature) >= 0.6f) {
			fSpeed -= 2.5f * fElapsedTime;
		}

		//Clamp Speed
		if (fSpeed < 0.0f) fSpeed = 0.0f;
		if (fSpeed > 1.0f) fSpeed = 1.0f;

		//Move car along tack according to car speed
		fDistance += (70.0f * fSpeed) * fElapsedTime;

		//Get Point on track
		float fOffset = 0;
		int nTrackSection = 0;

		fCurrentLapTime += fElapsedTime;
		if (fDistance >= fTrackDistance) {
			fDistance -= fTrackDistance;
			listLapTimes.push_front(fCurrentLapTime);
			listLapTimes.pop_back();
			fCurrentLapTime = 0;
			lap++;
		}

		//Find position on track
		while (nTrackSection < vecTrack1.size() && fOffset <= fDistance)
		{
			fOffset += vecTrack1[nTrackSection].second;
			nTrackSection++;
		}

		float fTargetCurvature = vecTrack1[nTrackSection - 1].first;

		float fTrackCurveDiff = (fTargetCurvature - fCurvature) * fElapsedTime * fSpeed;
		fCurvature += fTrackCurveDiff;

		fTrackCurvature += (fCurvature)*fElapsedTime * fSpeed;

		//Draw sky - light blue and dark blue
		for (int y = 0; y < ScreenHeight() / 2; y++)
		{
			for (int x = 0; x < ScreenWidth(); x++)
			{
				Draw(x, y, y < ScreenHeight() / 4 ? PIXEL_HALF : PIXEL_SOLID, FG_DARK_BLUE);
			}
		}

		//Draw hills
		for (int x = 0; x < ScreenWidth(); x++)
		{
			int nHillHeight = (int)(fabs(sinf(x * 0.01f + fTrackCurvature) * 16.0f));
			for (int y = (ScreenHeight() / 2) - nHillHeight; y < ScreenHeight() / 2; y++)
			{
				Draw(x, y, PIXEL_SOLID, FG_DARK_YELLOW);
			}
		}

		//Drawing on screen
		for (int y = 0; y < ScreenHeight() / 2; y++)
		{
			for (int x = 0; x < ScreenWidth(); x++)
			{
				float fPerspective = (float)y / (ScreenHeight() / 2.0f);

				float fMiddlePoint = 0.5f + fCurvature * powf((1.0f - fPerspective), 3);

				float fRoadWidth = 0.1f + fPerspective * 0.8f;
				float fRoadLineWidth = fRoadWidth * 0.15f; // 15% av vägen

				fRoadWidth *= 0.5; //Dela vägen på mitten

				int nLeftGrass = (fMiddlePoint - fRoadWidth - fRoadLineWidth) * ScreenWidth();
				int nLeftRoadLine = (fMiddlePoint - fRoadWidth) * ScreenWidth();
				int nRightRoadLine = (fMiddlePoint + fRoadWidth) * ScreenWidth();
				int nRightGrass = (fMiddlePoint + fRoadWidth + fRoadLineWidth) * ScreenWidth();

				int nRow = ScreenHeight() / 2 + y;

				int nGrassColor = sinf(20.0f * powf(1.0f - fPerspective, 3) + fDistance * 0.1f) > 0.0f ? FG_GREEN : FG_DARK_GREEN;
				int nRoadLineColor = sinf(80.0f * powf(1.0f - fPerspective, 3) + fDistance * 0.1f) > 0.0f ? FG_RED : FG_WHITE;

				int nRoadColor = FG_DARK_GREY;

				//Lap count
				if ((nTrackSection - 1) == 0)
				{
					if (lap == 1) {
						DrawString((ScreenWidth() / 2) - 2, (ScreenHeight() / 3), L"GO!");
						nRoadColor = FG_WHITE;
					}
					else
					{
						DrawString((ScreenWidth() / 2) - 7, (ScreenHeight() / 3), L"LAP: " + to_wstring(lap));
						nRoadColor = FG_WHITE;
					}
				}
				else if ((nTrackSection - 1) == 1)
				{
					if (lap == 1) {
						DrawString((ScreenWidth() / 2) - 2, (ScreenHeight() / 3), L"GO!");
						nRoadColor = FG_DARK_GREY;
					}
					else
					{
						DrawString((ScreenWidth() / 2) - 7, (ScreenHeight() / 3), L"LAP: " + to_wstring(lap));
						nRoadColor = FG_DARK_GREY;
					}
				}
				else
				{
					nRoadColor = FG_DARK_GREY;
				}

				//Drawing road
				//Left Side
				if (x >= 0 && x < nLeftGrass)
				{
					Draw(x, nRow, PIXEL_SOLID, nGrassColor);
				}
				if (x >= nLeftGrass && x < nLeftRoadLine) {
					Draw(x, nRow, PIXEL_SOLID, nRoadLineColor);
				}
				//Road Middle
				if (x >= nLeftRoadLine && x < nRightRoadLine) {
					Draw(x, nRow, PIXEL_SOLID, nRoadColor);
				}
				//Right Side
				if (x >= nRightGrass && x < ScreenWidth()) {
					Draw(x, nRow, PIXEL_SOLID, nGrassColor);
				}
				if (x >= nRightRoadLine && x < nRightGrass) {
					Draw(x, nRow, PIXEL_SOLID, nRoadLineColor);
				}
			}
		}

		//Draw Stats

		//Debug stats
		//DrawString(0, 1, L"Distance: " + to_wstring(fDistance));
		//DrawString(0, 2, L"Target Curvature: " + to_wstring(fCurvature));
		//DrawString(0, 3, L"Player Curvature: " + to_wstring(fPlayerCurvature));
		//DrawString(0, 4, L"Player Speed: " + to_wstring(fSpeed));
		//DrawString(0, 5, L"Track Curvature: " + std::to_wstring(fTrackCurvature));

		//Display current lap time
		auto disp_time = [](float t)
		{
			int nMinutes = t / 60.0f;
			int nSeconds = t - (nMinutes * 60.0f);
			int nMilliSeconds = (t - (float)nSeconds) * 1000.0f;
			return to_wstring(nMinutes) + L"." + to_wstring(nSeconds) + L":" + to_wstring(nMilliSeconds);
		};

		DrawString(2, 27, L"Current lap:");
		DrawString(2, 28, disp_time(fCurrentLapTime));

		//Display 5 last lap times
		int j = 33;
		for (auto l : listLapTimes)
		{
			DrawString(2, 32, L"Latest 5 laps:");
			DrawString(2, j, disp_time(l));
			j++;
		}

		//Drawing car
		fCarPos = fPlayerCurvature - fTrackCurvature;
		int nCarPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * fCarPos) / 2.0f) - 7;

		switch (nCarDirection)
		{
		case 0:
			DrawStringAlpha(nCarPos, 80, L"   ||####||   ");
			DrawStringAlpha(nCarPos, 81, L"      ##      ");
			DrawStringAlpha(nCarPos, 82, L"     ####     ");
			DrawStringAlpha(nCarPos, 83, L"     ####     ");
			DrawStringAlpha(nCarPos, 84, L"|||  ####  |||");
			DrawStringAlpha(nCarPos, 85, L"|||########|||");
			DrawStringAlpha(nCarPos, 86, L"|||  ####  |||");
			break;

		case +1:
			DrawStringAlpha(nCarPos, 80, L"      //####//");
			DrawStringAlpha(nCarPos, 81, L"         ##   ");
			DrawStringAlpha(nCarPos, 82, L"       ####   ");
			DrawStringAlpha(nCarPos, 83, L"      ####    ");
			DrawStringAlpha(nCarPos, 84, L"///  ####//// ");
			DrawStringAlpha(nCarPos, 85, L"//#######///O ");
			DrawStringAlpha(nCarPos, 86, L"/// #### //// ");
			break;

		case -1:
			DrawStringAlpha(nCarPos, 80, L"\\\\####\\\\      ");
			DrawStringAlpha(nCarPos, 81, L"   ##         ");
			DrawStringAlpha(nCarPos, 82, L"   ####       ");
			DrawStringAlpha(nCarPos, 83, L"    ####      ");
			DrawStringAlpha(nCarPos, 84, L" \\\\\\\\####  \\\\\\");
			DrawStringAlpha(nCarPos, 85, L" O\\\\\\#######\\\\");
			DrawStringAlpha(nCarPos, 86, L" \\\\\\\\ #### \\\\\\");
			break;
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