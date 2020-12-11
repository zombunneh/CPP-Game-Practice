#pragma once
#include "stdio.h"
#include <vector>

class PSYMW6Engine;

class PSYMW6UserObj;

class PSYMW6Platform;

class PSYMW6Laser;

class PSYMW6Coin;

class PSYMW6Level
{
public:
	PSYMW6Level(PSYMW6Engine *pEngine);
	~PSYMW6Level();
	void LoadLevel();

private:
	PSYMW6Engine* game_pEngine; 
	PSYMW6UserObj* myPlayer;
	FILE *levelData;
	char bgImg[256]; // holds the background image to be drawn
	char buf[256]; // buffer input to read file into
	int tileMap[30][40]; // holds tile arrangement on level map
	int playerPos[2]; // holds the players start/current position in level
	int timeOffset;
	std::vector<double> autoObjects; // vector holding object positional data
	std::vector<PSYMW6Platform*> myPlatforms;
	std::vector<PSYMW6Laser*> myLasers;
	std::vector<PSYMW6Coin*> myCoins;
	int numPlatforms;
	int numLasers;
	int objectArrayIndex;
	int numObjectsToLoad;
	int scoreDiff;
public:
	void ReadLevel(char* filename);
	void LoadBackground();
	void LoadTiles();
	void LoadPlayer();
	void LoadObjects();
	void LoadAutoObjs();
	void SaveLevel();
	void addLine(FILE* filestream);
	void DestroyVectors();
};

