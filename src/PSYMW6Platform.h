#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"
#include "JPGImage.h"

class PSYMW6Engine;

class PSYMW6Platform :
	public DisplayableObject
{
public:
	PSYMW6Platform(PSYMW6Engine* pEngine, int iXCoord, int iYCoord, double circAngle, int circOriginX, int circOriginY);
	~PSYMW6Platform();
	void Draw();
	void DoUpdate(int iCurrentTime);
	int getCurrentScreenX() { return m_iCurrentScreenX; };
	int getCurrentScreenY() { return m_iCurrentScreenY; };
	int getHeight() { return m_iDrawHeight; };
	int getWidth() { return m_iDrawWidth; };
private:
	PSYMW6Engine * gameEngine;
	ImageData im;
	int OriginX; // origin of circle 
	int OriginY;
	double angle; // current angle of rotation
	double radius; // radius of circle
	int flicker = 0;
protected:
	MovementPosition m_oMovement;
public:
	long Notify(int iSignalNumber);
	double getAngle() { return angle; };
	int getOriginX() { return OriginX; };
	int getOriginY() { return OriginY; };
};

