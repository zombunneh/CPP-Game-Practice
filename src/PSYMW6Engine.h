#pragma once
#include "BaseEngine.h"
#include "PSYMW6TileManager.h"
#include "PSYMW6UserObj.h"
#include "PSYMW6Platform.h"
#include "TileManager.h"
#include "PSYMW6Level.h"
#include <ctime>

class PSYMW6Engine :
	public BaseEngine
{
public:
	PSYMW6Engine();
	~PSYMW6Engine();
	void SetupBackgroundBuffer();
	void KeyDown(int iKeyCode);
	PSYMW6TileManager& GetTileManager() { return m_oTiles; }
	int gamePauseStarted;
	enum State { gameInit, gameMenu, gameSelect, gameLevel, gameClear, gamePause, gameScores, gameFail };
private:
	PSYMW6TileManager m_oTiles;
	PSYMW6Level* levelObj = new PSYMW6Level(this);
	FILE *hiScore;
	FILE *levelList;
	char buf[256]; // holds what is read in from txt files
	char printbuf[3][256]; // holds hiscores to be printed
	char printLevelBuf[3][256]; // holds levels to be printed
	char timebuf[128]; // holds timer
	char scorebuf[128]; // holds score string
	char gameClearMSG[128]; // holds message shown after clearing game.
	char scores[3][2][12]; // holds scores from hiscore.txt
	char tempScores[2][2][12];
	char levelsList[3][256]; // holds levels from levellist.txt
	char levelName[32];
	int levelScore; // remember to change this back to initialised as 0
	int scoreInit = 100000;
	char nameBuf[24] = ""; // stores the player input on gameInit screen
	int nameIndex = 0; // index number for nameBuf array to decide which character to change
	int levelRead = 0; // determines whether to load new level data, or if data has been loaded (i.e when transitioning from pause to level don't need to re-load the level, but do when going from main menu to level)
	int loadLevel; // determines whether to load save data or just load from level file.
	int levelNum; // level index for which level.txt file to load
	int setPause = 0;
	float currentOffset;
	time_t initTime, pauseInitTime, pauseEndTime, current;
	State gameState;
public:
	State& GetState() { return gameState; }
	void UpdateScore(int addScore) { levelScore += addScore; };
	int GetScoreDiff() { return (levelScore - scoreInit); };
	void SetTimer(int timeOffset) { initTime -= timeOffset; };
	float GetCurrentOffset() { return currentOffset; };
	int InitialiseObjects();
	void DrawStringsUnderneath();
	void UnDrawStrings();
	void MouseDown(int iButton, int iX, int iY);
	void ChangeStateArea(int iX1, int iX2, int iY1, int iY2, int mouseX, int mouseY, State newState);
	void PSYMW6Engine::ChangeState(State newState);
	void LoadHiscore();
	void PrintHiscore(int printRow);
	void SaveHiscore();
	void LoadLevelList();
	void PrintLevels(int printRow);
	void SaveTempScore(int iRowNum);
	void ReplaceTempScore(int iRowNum);
	void ReplaceNewScore(int iRowNum);
	void SortHiscores();
	void GameAction();
	void CalculateScore();
	void MouseUp(int iButton, int iX, int iY);
	void DrawStringsOnTop();
};

