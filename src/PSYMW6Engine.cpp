#include "header.h"
#include "templates.h"
#include "BaseEngine.h"
#include "TileManager.h"
#include "PSYMW6Platform.h"
#include "PSYMW6Engine.h"
#include <ctime>

PSYMW6Engine::PSYMW6Engine()
	: gameState(gameInit)
{
}


PSYMW6Engine::~PSYMW6Engine()
{
}


void PSYMW6Engine::SetupBackgroundBuffer()
{
	switch (gameState)
	{
	case gameInit:
	{
		FillBackground(0xeeeee0);
		DrawBackgroundString(270, 50, "Enter player name:", 0xff0000, NULL);
		DrawBackgroundRectangle(300,200,500,240, 0x696969);
	}
		break;
	case gameMenu:
	{

		levelRead = 0;
		FillBackground(0xeeeee0);
		DrawBackgroundString(300, 50, "Main Menu", 0xff0000, NULL);
	}
	break;
	case gameSelect:
	{
		FillBackground(0xeeeee0);
		DrawBackgroundString(300, 50, "Select Level", 0xff0000, NULL);
		DrawBackgroundString(0, 0, "Back", 0xff0000, NULL);
		LoadLevelList();
	}
	break;
	case gameLevel:
	{
		if (setPause == 0)
		{
			m_oTiles.SetSize(40, 30);


			if (loadLevel == 0)
			{
				strcpy(levelName, levelsList[levelNum]);
				strtok(levelName, "\n");
				strcat(levelName, ".txt");
			}
			else
			{
				sprintf(levelName, "levelsavedata.txt");
			}

			if (levelRead == 0)
			{
				levelScore = scoreInit;
				initTime = time(NULL);
				levelObj->ReadLevel(levelName);
				InitialiseObjects();
				levelRead = 1;
			}

			levelObj->LoadLevel();
			
			m_oTiles.SetBaseTilesPositionOnScreen(0, 0);
			m_oTiles.DrawAllTiles(this, this->GetBackground(), 0, 0, 39, 29);
		}
		NotifyAllObjects(1);
	}
	break;
	case gameClear:
	{
		FillBackground(0xeeeee0);
		DrawBackgroundString(300, 50, "Game Cleared!", 0xff0000, NULL);
		NotifyAllObjects(0);
		levelRead = 0;
		LoadHiscore();
		SortHiscores();
		SaveHiscore();
	}
		break;
	case gamePause:
		NotifyAllObjects(0);
		break;
	case gameScores:
	{
	FillBackground(0xeeeee0);
	DrawBackgroundString(300, 50, "Hi-Scores", 0xff0000, NULL);
	DrawBackgroundString(0, 0, "Back", 0xff0000, NULL);
	LoadHiscore();
	}
		break;
	case gameFail:
	{
		FillBackground(0xeeeee0);
		DrawBackgroundString(300, 50, "Game Failed", 0xff0000, NULL);
		NotifyAllObjects(0);
		levelRead = 0;
	}
		break;
	}
}


void PSYMW6Engine::KeyDown(int iKeyCode)
{
	switch (gameState)
	{
	case gameInit:
	{
		
		switch (iKeyCode)
		{
		case SDLK_KP_ENTER:
			if (nameIndex > 0)
			{
				strtok(nameBuf, "\n");
				ChangeState(gameMenu);
			}
			break;
		case SDLK_RETURN:
			if (nameIndex > 0)
			{
				strtok(nameBuf, "\n");
				ChangeState(gameMenu);
			}
			break;
				
		case SDLK_BACKSPACE:
			if (nameIndex > 0)
			{
				nameIndex--;
				nameBuf[nameIndex] = ' ';
			}
			break;
		default:
			if (nameIndex < 12)
			{
				nameBuf[nameIndex] = iKeyCode;
				nameIndex++;
			}
			break;
			
		}
	}
		break;
	case gameMenu:
	{
		switch (iKeyCode)
		{
		case SDLK_ESCAPE:
			SetExitWithCode(0);
			break;
		}
	}
		break;
	case gameLevel:
	{
		switch (iKeyCode)
		{
		case SDLK_ESCAPE:
			setPause = 1;
			gamePauseStarted = GetTime();
			pauseInitTime = time(0);
			ChangeState(gamePause);
			break;
		}
	}
		break;
	case gamePause:
	{
		switch (iKeyCode)
		{
		case SDLK_ESCAPE:
			ChangeState(gameLevel);
			pauseEndTime = time(0);
			SetTimer(-(difftime(pauseEndTime, pauseInitTime)));
			IncreaseTimeOffset(gamePauseStarted - GetTime());
			break;
		}
	}
		break;
	}
}


int PSYMW6Engine::InitialiseObjects()
{
	switch (gameState)
	{
	case gameLevel:
	{
		levelObj->LoadObjects();
	}
		break;
	}
	return 0;
}


void PSYMW6Engine::DrawStringsUnderneath()
{
	switch (gameState)
	{
	case gameInit:
	{
		DrawForegroundString(300, 205, nameBuf, 0xff0000, NULL);
		Redraw(true);
	}
		break;
	case gameMenu:
	{
		DrawForegroundString(100, 205, "Level Select", 0xff0000, NULL);
		DrawForegroundString(500, 205, "Load Save Data", 0xff0000, NULL);
		DrawForegroundString(300, 345, "Hi-Scores", 0xff0000, NULL);
	}
		break;
	case gameSelect:
	{
		for (int i = 0; i < 3; i++)
		{
			PrintLevels(i);
			DrawForegroundString(200, (150 + (i * 100)), printLevelBuf[i], 0xff0000, NULL);
		}
	}
		break;
	case gameLevel:
	{

	}
		break;
	case gameClear:
	{
		sprintf(gameClearMSG, "Your score was: %d", levelScore);
		DrawForegroundString(220, 205, gameClearMSG, 0xff0000, NULL);
		DrawForegroundString(245, 345, "Return to main menu", 0xff0000, NULL);
	}
		break;
	case gamePause:
	{
		DrawForegroundString(300, 50, "Game Paused", 0xff0000, NULL);
		DrawForegroundString(230, 250, "Save and exit to menu", 0xff0000, NULL); // remember to add clickability cx
		DrawForegroundString(300, 350, "Exit to menu", 0xff0000, NULL);
	}
		break;
	case gameScores:
	{
		for (int i = 0; i < 3; i++)
		{
			PrintHiscore(i);
			DrawForegroundString(200, (150 + (i * 100)), printbuf[i], 0xff0000, NULL);
		}
	}
		break;
	case gameFail:
	{
		DrawForegroundString(245, 345, "Return to main menu", 0xff0000, NULL);
	}
		break;
	}

}

void PSYMW6Engine::UnDrawStrings()
{
	switch (gameState)
	{
	case gameLevel:
	{
		CopyBackgroundPixels(0, 0, GetWindowWidth(), GetWindowHeight());
	}
		break;
	case gameScores:
	{
		CopyBackgroundPixels(0, 0, GetWindowWidth(), 350);
	}
		break;
	}

}

void PSYMW6Engine::MouseDown(int iButton, int iX, int iY)
{
	switch (gameState) // menu navigation handling
	{
	case gameMenu:
	{
		loadLevel = 1;
		ChangeStateArea(80, 300, 200, 240, iX, iY, gameSelect);
		ChangeStateArea(480, 600, 200, 240, iX, iY, gameLevel);
		ChangeStateArea(280, 460, 340, 380, iX, iY, gameScores);
	}
		break;
	case gameSelect:
	{
		ChangeStateArea(0, 80, 0, 40, iX, iY, gameMenu);
		for (int i = 0; i < 3; i++)
		{
			loadLevel = 0;
			levelNum = i;
			ChangeStateArea(180, 400, (145 + (i * 100)), (185 + (i * 100)), iX, iY, gameLevel);
		}
	}
		break;
	case gameLevel:
	{
		if(iButton == 1)
			NotifyAllObjects(2);
	}
		break;
	case gameClear:
	{
		ChangeStateArea(235, 545, 340, 380, iX, iY, gameMenu); 
		levelObj->DestroyVectors();
	}
		break;
	case gamePause:
	{
		ChangeStateArea(210, 550, 245, 285, iX, iY, gameMenu); // implement saving too dont forget!! donezo k1dd0
		if (iY >= 245 && iY <= 285)
		{
			levelObj->SaveLevel();
			setPause = 0;
			levelObj->DestroyVectors();
		}
		ChangeStateArea(280, 480, 345, 385, iX, iY, gameMenu);
		if (iY >= 345 && iY <= 385)
		{
			setPause = 0;
			levelObj->DestroyVectors();
		}
	}
	case gameScores:
	{
		ChangeStateArea(0, 80, 0, 40, iX, iY, gameMenu);
	}
		break;
	case gameFail:
	{
		ChangeStateArea(235, 545, 340, 380, iX, iY, gameMenu);
		levelObj->DestroyVectors();
	}
		break;
	}
	
}


void PSYMW6Engine::ChangeStateArea(int iX1, int iX2, int iY1, int iY2, int mouseX, int mouseY, State newState)
{
	if (mouseX >= iX1 && mouseX <= iX2 && mouseY >= iY1 && mouseY <= iY2)
	{
		ChangeState(newState);
	}
}

void PSYMW6Engine::ChangeState(State newState)
{
	gameState = newState;
	SetupBackgroundBuffer();
	Redraw(true);
}

void PSYMW6Engine::LoadHiscore()
{
	if ((hiScore = fopen("hiscores.txt", "r")) == NULL)
	{
		fprintf(stderr, "Error opening hiscore data.\n");
	}
	for(int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			fgets(buf, 256, hiScore);
			strcpy(scores[i][j], buf);
			strtok(scores[i][j], "\n");
		}
	}
	if (fclose(hiScore) != 0)
	{
		fprintf(stderr, "Error closing hiscore data.\n");
	}
}


void PSYMW6Engine::PrintHiscore(int printRow)
{
	sprintf(printbuf[printRow], "%d. %s: %s", printRow + 1, scores[printRow][0], scores[printRow][1]);
}


void PSYMW6Engine::SaveHiscore() // implement to save hiscore once timer + score calc implemented
{
	if ((hiScore = fopen("hiscores.txt", "w+")) == NULL)
	{
		fprintf(stderr, "Error opening hiscore data.\n");
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (fputs(scores[i][j], hiScore) == EOF)
			{
				fprintf(stderr, "Error writing to hiscore data.\n");
			}
			if (fputs("\n", hiScore) == EOF)
			{
				fprintf(stderr, "Error writing to hiscore data.\n");
			}
		}
	}

	if (fclose(hiScore) != 0)
	{
		fprintf(stderr, "Error closing hiscore data.\n");
	}
}


void PSYMW6Engine::LoadLevelList() // implement to read levellist and display.
{
	if ((levelList = fopen("levellist.txt", "r")) == NULL)
	{
		fprintf(stderr, "Error opening level list data.\n");
	}
	for (int i = 0; i < 3; i++)
	{
			fgets(buf, 256, levelList);
			strcpy(levelsList[i], buf);
			strtok(levelsList[i], "\n");
	}
	if (fclose(levelList) != 0)
	{
		fprintf(stderr, "Error closing level list data.\n");
	}
}


void PSYMW6Engine::PrintLevels(int printRow)
{
	sprintf(printLevelBuf[printRow], "%d. %s", printRow + 1, levelsList[printRow]);
}


void PSYMW6Engine::SaveTempScore(int iRowNum)
{
	for(int i = 0; i < 2; i++)
	{
		strcpy(tempScores[iRowNum][i], scores[iRowNum][i]);
		strtok(tempScores[iRowNum][i], "\n");
	}
}


void PSYMW6Engine::ReplaceTempScore(int iRowNum)
{
	for (int i = 0; i < 2; i++)
	{
		strcpy(scores[iRowNum][i], tempScores[iRowNum - 1][i]);
		strtok(scores[iRowNum][i], "\n");
	}
}


void PSYMW6Engine::ReplaceNewScore(int iRowNum)
{
	strcpy(scores[iRowNum][0], nameBuf);
	strtok(scores[iRowNum][0], "\n");
	sprintf(scores[iRowNum][1], "%d", levelScore);
}


void PSYMW6Engine::SortHiscores()
{
	for (int i = 0; i < 3; i++)
	{
		if (levelScore >= atoi(scores[i][1]))
		{
			if (i == 0)
			{
				SaveTempScore(i);
				ReplaceNewScore(i);
				i++;
				SaveTempScore(i);
				ReplaceTempScore(i);
				i++;
				ReplaceTempScore(i);
			}
			else if (i == 1)
			{
				SaveTempScore(i);
				ReplaceNewScore(i);
				i++;
				ReplaceTempScore(i);
			}
			else
			{
				ReplaceNewScore(i);
			}
		}
	}
}


void PSYMW6Engine::GameAction()
{
	if (!IsTimeToAct()) 
		return;

	SetTimeToAct(20);
	switch (gameState)
	{
	case gameInit:
	case gamePause:
		break;
	case gameLevel:
		UpdateAllObjects(GetModifiedTime());
		break;
	}
}


void PSYMW6Engine::CalculateScore() // implement to calculate score based on stuff and things ok nice
{
	if (currentOffset <= 30)
	{
		// do nothing
	}
	else if (currentOffset > 30 && levelScore > 0)
	{
		levelScore-= 100;
	}

}


void PSYMW6Engine::MouseUp(int iButton, int iX, int iY)
{
	if (gameState == gameLevel)
	{
		if(iButton == 1)
			NotifyAllObjects(3);
	}
}


void PSYMW6Engine::DrawStringsOnTop()
{
	
	if (gameState == gameLevel)
	{
		current = time(0);
		currentOffset = difftime(current, initTime);
		sprintf(timebuf, "Time taken: %.f", currentOffset);
		strtok(timebuf, "\n");
		DrawForegroundString(300, 530, timebuf, 0xff1493, NULL);
		CalculateScore();
		sprintf(scorebuf, "Score: %d", levelScore);
		strtok(scorebuf, "\n");
		DrawForegroundString(75, 530, scorebuf, 0xff1493, NULL);
	}
}
