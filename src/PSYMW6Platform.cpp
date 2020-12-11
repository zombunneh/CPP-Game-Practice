#include "header.h"
#include "PSYMW6Platform.h"
#include "PSYMW6Engine.h"
#include "JPGImage.h"


PSYMW6Platform::PSYMW6Platform(PSYMW6Engine* pEngine, int iXCoord, int iYCoord, double circAngle, int circOriginX, int circOriginY)
	: DisplayableObject(pEngine),
	gameEngine(pEngine)
{
	angle = circAngle;
	radius = 100;

	if (iYCoord > gameEngine->GetWindowHeight() - m_iDrawHeight)
		m_iCurrentScreenY = m_iPreviousScreenY = GetEngine()->GetWindowHeight() - m_iDrawHeight;
	else
		m_iCurrentScreenY = m_iPreviousScreenY = iYCoord;

	if (iXCoord > gameEngine->GetWindowWidth() - m_iDrawWidth)
		m_iCurrentScreenX = m_iPreviousScreenX = GetEngine()->GetWindowWidth() - m_iDrawWidth;
	else
		m_iCurrentScreenX = m_iPreviousScreenX = iXCoord;

	OriginX = circOriginX;
	OriginY = circOriginY;

	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;

	m_iDrawWidth = 60;
	m_iDrawHeight = 16;


	SetVisible(true);
}


PSYMW6Platform::~PSYMW6Platform()
{
}


void PSYMW6Platform::Draw()
{
	if (!IsVisible())
		return;

	if (flicker <= 6) 
	{
		im.LoadImage("hoverplatform1.png");
		flicker++;
	}
	else if (flicker > 6 && flicker < 12)
	{
		im.LoadImage("hoverplatform2.png");
		flicker++;
	}
	else
		flicker = 0;

	im.RenderImageWithMask(gameEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, im.GetWidth(), im.GetHeight());

	StoreLastScreenPositionForUndraw();
}


void PSYMW6Platform::DoUpdate(int iCurrentTime)
{
	if (!IsVisible())
		return;

	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;
 
	m_iCurrentScreenX = OriginX + (cos(angle) * radius);
	m_iCurrentScreenY = OriginY + (sin(angle) * radius);

	angle += 0.01;
	if (angle > 2 * M_PI)
		angle = 0;

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


long PSYMW6Platform::Notify(int iSignalNumber)
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
