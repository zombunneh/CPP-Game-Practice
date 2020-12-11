#pragma once
#include "DisplayableObject.h"
#include "JPGImage.h"

class PSYMW6Engine;

class PSYMW6Coin :
	public DisplayableObject
{
public:
	PSYMW6Coin(PSYMW6Engine* pEngine, int iXCoord, int iYCoord, int visibleChange);
	~PSYMW6Coin();
	void Draw();
	long Notify(int iSignalNumber);
	void DoUpdate(int iCurrentTime);
	int getCurrentScreenX() { return m_iCurrentScreenX; };
	int getCurrentScreenY() { return m_iCurrentScreenY; };
	int getHeight() { return m_iDrawHeight; };
	int getWidth() { return m_iDrawWidth; };
private:
	PSYMW6Engine* gameEngine;
	ImageData im;
	bool changeVisible;
public:
	void setChangeVisible(bool change) { changeVisible = change; };
	int getChangeVisible() { return changeVisible; };
};

