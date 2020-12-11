#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"
#include "JPGImage.h"

class PSYMW6Engine;

class PSYMW6Laser :
	public DisplayableObject
{
public:
	PSYMW6Laser(PSYMW6Engine* pEngine, int iXCoord, int iYCoord, int travelVect, int objSpeed, int objRange, int objReverse);
	~PSYMW6Laser();
	void Draw();
	void DoUpdate(int iCurrentTime);
	int getCurrentScreenX() { return m_iCurrentScreenX; };
	int getCurrentScreenY() { return m_iCurrentScreenY; };
	int getReversed() { return moveReverse; };
	int getTravelVect() { return travelVector; };
	int getMoveSpeed() { return moveSpeed; };
	int getMoveRange() { return moveRange; };
	int getHeight() { return m_iDrawHeight; };
	int getWidth() { return m_iDrawWidth; };
private:
	PSYMW6Engine * gameEngine;
	ImageData im;
	bool moveReverse; // whether movement is reversed
	int travelVector; // determines direction of travel
	int moveSpeed; // movespeed of object
	int moveRange; // movement range of object
	int flicker = 0;
protected:
	MovementPosition m_oMovement;
public:
	long Notify(int iSignalNumber);
};

