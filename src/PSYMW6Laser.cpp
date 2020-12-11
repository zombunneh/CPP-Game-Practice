#include "header.h"
#include "PSYMW6Laser.h"
#include "PSYMW6Engine.h"
#include "JPGImage.h"


PSYMW6Laser::PSYMW6Laser(PSYMW6Engine* pEngine, int iXCoord, int iYCoord, int travelVect, int objSpeed, int objRange, int objReverse)
	: DisplayableObject(pEngine),
	gameEngine(pEngine)
{
	if (iYCoord > gameEngine->GetWindowHeight() - m_iDrawHeight)
		m_iCurrentScreenY = m_iPreviousScreenY = GetEngine()->GetWindowHeight() - m_iDrawHeight;
	else
		m_iCurrentScreenY = m_iPreviousScreenY = iYCoord;

	if (iXCoord > gameEngine->GetWindowWidth() - m_iDrawWidth)
		m_iCurrentScreenX = m_iPreviousScreenX = GetEngine()->GetWindowWidth() - m_iDrawWidth;
	else
		m_iCurrentScreenX = m_iPreviousScreenX = iXCoord;

	travelVector = travelVect;
	moveSpeed = objSpeed;
	moveRange = objRange;
	moveReverse = objReverse;

	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;

	m_iDrawWidth = 20;
	m_iDrawHeight = 54;

	SetVisible(true);
}


PSYMW6Laser::~PSYMW6Laser()
{
}


void PSYMW6Laser::Draw()
{
	if (!IsVisible())
		return;

	if (flicker <= 5)
	{
		im.LoadImage("LaserObject1.png");
		flicker++;
	}
	else if (flicker > 5 && flicker < 10)
	{
		im.LoadImage("LaserObject2.png");
		flicker++;
	}
	else
		flicker = 0;
	im.RenderImageWithMask(gameEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, im.GetWidth(), im.GetHeight());

	StoreLastScreenPositionForUndraw();
}


void PSYMW6Laser::DoUpdate(int iCurrentTime)
{
	if (!IsVisible())
		return;

	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;


 // move up at constant rate until travelvector = 10 then reverse and go down until travelvector = -10 and then reverse again


	if (travelVector <= moveRange && !moveReverse)
	{
		m_iCurrentScreenY -= moveSpeed;
		travelVector++;
		if (travelVector == moveRange)
			moveReverse = true;
	}
	else if (travelVector >= -moveRange && moveReverse)
	{
		m_iCurrentScreenY += moveSpeed;
		travelVector--;
		if (travelVector == -moveRange)
			moveReverse = false;
	}

	if (m_iCurrentScreenX < 0)
		m_iCurrentScreenX = 0;
	if (m_iCurrentScreenX >= GetEngine()->GetWindowWidth() - m_iDrawWidth)
		m_iCurrentScreenX = GetEngine()->GetWindowWidth() - m_iDrawWidth;
	if (m_iCurrentScreenY < 0)
		m_iCurrentScreenY = 0;
	if (m_iCurrentScreenY >= GetEngine()->GetWindowHeight() - m_iDrawHeight)
		m_iCurrentScreenY = GetEngine()->GetWindowHeight() - m_iDrawHeight;

	RedrawObjects();
}


long PSYMW6Laser::Notify(int iSignalNumber)
{
	switch (iSignalNumber)
	{
	case 0:
		SetVisible(false);
		break;
	case 1:
		SetVisible(true);
		break;
	}
	return 0;
}
