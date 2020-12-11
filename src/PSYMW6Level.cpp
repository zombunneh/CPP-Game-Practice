#include "PSYMW6Level.h"
#include "header.h"
#include "templates.h"
#include "PSYMW6Engine.h"
#include "JPGImage.h"
#include "TileManager.h"
#include "PSYMW6Laser.h"
#include "PSYMW6Platform.h"
#include "PSYMW6Coin.h"
#include <vector>

PSYMW6Level::PSYMW6Level(PSYMW6Engine *pEngine)
	: game_pEngine(pEngine)
{
}


PSYMW6Level::~PSYMW6Level()
{
}


void PSYMW6Level::LoadLevel() // loads all background features of the level
{
	//load background image
	LoadBackground();
	//load level tiles
	LoadTiles();
	//load offset for game timer
	game_pEngine->SetTimer(timeOffset);
	game_pEngine->UpdateScore(scoreDiff);
}


void PSYMW6Level::ReadLevel(char* filename) 
{
	objectArrayIndex = 0;
	if ((levelData = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Error opening level data.\n");
	}
	if (fgets(buf, 256, levelData) != NULL) // read in first line containing background image to be loaded
	{
		strcpy(bgImg, buf);
		strtok(bgImg, "\n");
	}

	if (fgets(buf, 256, levelData) != NULL) // read in time offset
	{
		timeOffset = atoi(buf);
	}

	if (fgets(buf, 256, levelData) != NULL) // read in score offset
	{
		scoreDiff = atoi(buf);
	}

	for (int i = 0; i < 30; i++) // reads the tile designations into 2d array
	{
		if (fgets(buf, 256, levelData) != NULL)
		{
			for (int j = 0; j < 40; j++)
			{
				int tempBuf = buf[j] - '0';
				tileMap[i][j] = tempBuf;
			}
		}
	}

	if (fgets(buf, 256, levelData) != NULL) // reads number of objects contained in level
		numObjectsToLoad = atoi(buf);

	for (int i = 0; i < 2; i++) // reads player position coordinates
	{
		if (fgets(buf, 256, levelData) != NULL)
		{
			int tempPos = atoi(buf);
			playerPos[i] = tempPos;
		}
	}

	while (fgets(buf, 256, levelData) != NULL)
	{
		double tempObj = atof(buf);
		autoObjects.push_back(tempObj);
	}

	if (fclose(levelData) != 0)
	{
		fprintf(stderr, "Error closing level data.\n");
	}
}


void PSYMW6Level::LoadBackground()
{
	ImageData im;
	im.LoadImage(bgImg);
	im.RenderImage(game_pEngine->GetBackground(), 0, 0, 0, 0, im.GetWidth(), im.GetHeight());
}


void PSYMW6Level::LoadTiles() // loads tiles from tileMap array read in earlier
{
	PSYMW6TileManager& game_tileRef = game_pEngine->GetTileManager();

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			game_tileRef.SetValue(j, i, tileMap[i][j]);
		}
	}
}


void PSYMW6Level::LoadPlayer() // loads a user object at the coordinates indicated in level data file
{
	myPlayer = new PSYMW6UserObj(game_pEngine, playerPos[0], playerPos[1]);
	game_pEngine->StoreObjectInArray(objectArrayIndex, myPlayer);
	objectArrayIndex++;
}


void PSYMW6Level::LoadObjects() // calls load player and auto objs to create object array.
{
	game_pEngine->DrawableObjectsChanged();
	game_pEngine->DestroyOldObjects();
	game_pEngine->CreateObjectArray(numObjectsToLoad+1); 
	LoadPlayer();
	LoadAutoObjs();
	game_pEngine->StoreObjectInArray((objectArrayIndex), NULL);
}


void PSYMW6Level::LoadAutoObjs() // loads automated objects at the coordinates indicated in level data file
{
	auto it = autoObjects.begin();
	while (it != autoObjects.end()) // iterators through int vector of triplets (objtype, xcoord, ycoord) and creates appropriate objects in their respective vectors
	{
		if (*it == 1)
		{
			int laserXCoord = *(it + 1);
			int laserYCoord = *(it + 2);
			int laserVect = *(it + 3);
			int laserSpeed = *(it + 4);
			int laserRange = *(it + 5);
			int laserReverse = *(it + 6);
			myLasers.push_back(new PSYMW6Laser(game_pEngine, laserXCoord, laserYCoord, laserVect, laserSpeed, laserRange, laserReverse));
			it += 7;
		}
		if (*it == 2)
		{
			int platformXCoord = *(it + 1);
			int platformYCoord = *(it + 2);
			double platformAngle = *(it + 3);
			int platformXOrigin = *(it + 4);
			int platformYOrigin = *(it + 5);
			myPlatforms.push_back(new PSYMW6Platform(game_pEngine, platformXCoord, platformYCoord, platformAngle, platformXOrigin, platformYOrigin));
			it += 6;
		}
		if (*it == 3)
		{
			int coinXCoord = *(it + 1);
			int coinYCoord = *(it + 2);
			int coinVisible = *(it + 3);
			myCoins.push_back(new PSYMW6Coin(game_pEngine, coinXCoord, coinYCoord, coinVisible));
			it += 4;
		}
	}

	auto it2 = myLasers.begin();
	while (it2 != myLasers.end()) // iterates through vector storing objects appropriately
	{
		game_pEngine->StoreObjectInArray(objectArrayIndex, *it2);
		objectArrayIndex++;
		it2++;
	}

	auto it3 = myPlatforms.begin();
	while (it3 != myPlatforms.end())
	{
		game_pEngine->StoreObjectInArray(objectArrayIndex, *it3);
		objectArrayIndex++;
		it3++;
	}

	auto it4 = myCoins.begin();
	while (it4 != myCoins.end())
	{
		game_pEngine->StoreObjectInArray(objectArrayIndex, *it4);
		objectArrayIndex++;
		it4++;
	}
}


void PSYMW6Level::SaveLevel() // implement to save current state of tiles and objects STILL NEED TO SAVE AUTO OBJECTS
{
	PSYMW6TileManager& tm = game_pEngine->GetTileManager();
	if ((levelData = fopen("levelsavedata.txt", "w+")) == NULL)
	{
		fprintf(stderr, "Error opening level save data.\n");
	}

	// implement to save bgimg, tiles arrangement, player obj position, npc positions+states.
	if (fputs(bgImg, levelData) == EOF) // saves bgimg file name to first line and then starts a new line
	{
		fprintf(stderr, "Error saving background image level data.\n");
	}
	addLine(levelData);

	char timeOffsetSave[4];
	sprintf(timeOffsetSave, "%f", game_pEngine->GetCurrentOffset());
	if (fputs(timeOffsetSave, levelData) == EOF)
	{
		fprintf(stderr, "Error saving time offset level data.\n");
	}
	addLine(levelData);

	char scoreDiffSave[4];
	sprintf(scoreDiffSave, "%d", game_pEngine->GetScoreDiff());
	if (fputs(scoreDiffSave, levelData) == EOF)
	{
		fprintf(stderr, "Error saving score diff level data.\n");
	}
	addLine(levelData);

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			char temptile[2];
			sprintf(temptile, "%d", tm.GetValue(j, i));
			if (fputs(temptile, levelData) == EOF) // writes every individual character saved in tilemap array back into file
			{
				fprintf(stderr, "Error saving tile level data.\n");
			}
		}
		addLine(levelData);
	}

	char objectSave[2];
	sprintf(objectSave, "%d", numObjectsToLoad);
	if (fputs(objectSave, levelData) == EOF) // saves the number of objects 
	{
		fprintf(stderr, "Error saving background image level data.\n");
	}
	addLine(levelData);

	// saving current player object position...??
	int screenPosX = myPlayer->getCurrentScreenX();
	int screenPosY = myPlayer->getCurrentScreenY();
	char playerPosSaveX[4];
	char playerPosSaveY[4];
	sprintf(playerPosSaveX, "%d", screenPosX);
	sprintf(playerPosSaveY, "%d", screenPosY);

	if (fputs(playerPosSaveX, levelData) == EOF) // saves the number of objects 
	{
		fprintf(stderr, "Error saving background image level data.\n");
	}
	addLine(levelData);
	if (fputs(playerPosSaveY, levelData) == EOF) // saves the number of objects 
	{
		fprintf(stderr, "Error saving background image level data.\n");
	}
	addLine(levelData);

	char objType[2];
	char objPosSaveX[4]; // really didn't need these lol.
	char objPosSaveY[4];
	char objVect[4];
	char objSpeed[4];
	char objRange[4];
	char objReverse[4];
	char objAngle[4];
	char objOriginSaveX[4];
	char objOriginSaveY[4];
	char objVisible[4];

	auto it = myLasers.begin();
	while (it != myLasers.end()) // save in order x, y, vect, speed, range, reverse
	{
		sprintf(objType, "1"); // first add a line with the player object type
		if (fputs(objType, levelData) == EOF) // saves the id of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);
		sprintf(objPosSaveX, "%d", (*it)->getCurrentScreenX());
		sprintf(objPosSaveY, "%d", (*it)->getCurrentScreenY());
		if (fputs(objPosSaveX, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);
		if (fputs(objPosSaveY, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objVect, "%d", (*it)->getTravelVect());
		if (fputs(objVect, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objSpeed, "%d", (*it)->getMoveSpeed());
		if (fputs(objSpeed, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objRange, "%d", (*it)->getMoveRange());
		if (fputs(objRange, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objReverse, "%d", (*it)->getReversed());
		if (fputs(objReverse, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		it++;
	}

	auto it2 = myPlatforms.begin();
	while (it2 != myPlatforms.end())
	{
		sprintf(objType, "2");
		if (fputs(objType, levelData) == EOF) // saves the id of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);
		sprintf(objPosSaveX, "%d", (*it2)->getCurrentScreenX());
		sprintf(objPosSaveY, "%d", (*it2)->getCurrentScreenY());
		if (fputs(objPosSaveX, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);
		if (fputs(objPosSaveY, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objAngle, "%0.6f", (*it2)->getAngle());
		if (fputs(objAngle, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objOriginSaveX, "%d", (*it2)->getOriginX());
		if (fputs(objOriginSaveX, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objOriginSaveY, "%d", (*it2)->getOriginY());
		if (fputs(objOriginSaveY, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);
		it2++;
	}

	auto it3 = myCoins.begin();
	while (it3 != myCoins.end())
	{
		sprintf(objType, "3");
		if (fputs(objType, levelData) == EOF) // saves the id of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objPosSaveX, "%d", (*it3)->getCurrentScreenX());
		if (fputs(objPosSaveX, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objPosSaveY, "%d", (*it3)->getCurrentScreenY());
		if (fputs(objPosSaveY, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);

		sprintf(objVisible, "%d", (*it3)->getChangeVisible());
		if (fputs(objVisible, levelData) == EOF) // saves the coord of objects 
		{
			fprintf(stderr, "Error saving object level data.\n");
		}
		addLine(levelData);
		it3++;
	}

	if (fclose(levelData) != 0)
	{
		fprintf(stderr, "Error closing level save data.\n");
	}
}


void PSYMW6Level::addLine(FILE* filestream)
{
	if (fputs("\n", filestream) == EOF)
	{
		fprintf(stderr, "Error saving background image level data.\n");
	}
}


void PSYMW6Level::DestroyVectors()
{
	autoObjects.clear();
	myLasers.clear();
	myPlatforms.clear();
	myCoins.clear();
}
