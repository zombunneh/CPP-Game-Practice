#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"
#include "JPGImage.h"

class PSYMW6Engine;

class PSYMW6UserObj :
	public DisplayableObject
{
public:
	PSYMW6UserObj(PSYMW6Engine* pEngine, int iXCoord, int iYCoord);
	~PSYMW6UserObj(void);
	void Draw();
	void DoUpdate(int iCurrentTime);
	int getCurrentScreenX() { return m_iCurrentScreenX; };
	int getCurrentScreenY() { return m_iCurrentScreenY; };

private:
	PSYMW6Engine* gameEngine;
	ImageData im;
	int movementOn = 0; // decide whether object needs to move
	int moveReversed; // check whether object has been reversed yet
	int currentVelocity; // the initial velocity of player character upon jumping
	int mouseClicked; //maybe just for testing
	int energyOn;
	bool isJumping;
	bool isFalling;
	bool onPlatform;
	int coinValue = 1000; // value of collected coin
	int jumpAnim = 0;
protected:
	MovementPosition m_oMovement;
public:
	long Notify(int iSignalNumber);
	void TileCollision();
	int CheckTile(int spritePos);
	void ChangeTile(int spritePos, int tileValue);
	void SimulateGravity();
	void ChangeTilesWithValue(int tileValue, int newValue);
	void ObjectCollision();
};

