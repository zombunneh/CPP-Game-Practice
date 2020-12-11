#include "header.h"
#include "PSYMW6Coin.h"
#include "PSYMW6Engine.h"
#include "JPGImage.h"

PSYMW6Coin::PSYMW6Coin(PSYMW6Engine* pEngine, int iXCoord, int iYCoord, int visibleChange)
	:DisplayableObject(pEngine),
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

	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;

	m_iDrawWidth = 20;
	m_iDrawHeight = 20;

	changeVisible = visibleChange;

	if (changeVisible == true)
		SetVisible(true);
	else
		SetVisible(false);
}


PSYMW6Coin::~PSYMW6Coin()
{
}

void PSYMW6Coin::Draw()
{
	if (!IsVisible())
		return;

	im.LoadImage("coin.png");
	im.RenderImageWithMask(gameEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, im.GetWidth(), im.GetHeight());

	StoreLastScreenPositionForUndraw();
}

long PSYMW6Coin::Notify(int iSignalNumber)
{
	switch (iSignalNumber)
	{
	case 0:
		SetVisible(false);
		break;
	case 1:
		if(changeVisible == true)
			SetVisible(true);
		break;
	}
	return 0;
}

void PSYMW6Coin::DoUpdate(int iCurrentTime)
{
	if (!IsVisible())
		return;

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