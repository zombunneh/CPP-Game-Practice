#include "header.h"
#include "PSYMW6UserObj.h"
#include "PSYMW6Engine.h"
#include "JPGImage.h"
#include "PSYMW6Platform.h"
#include "PSYMW6Laser.h"
#include "PSYMW6Coin.h"


PSYMW6UserObj::PSYMW6UserObj(PSYMW6Engine* pEngine, int iXCoord, int iYCoord)
	: DisplayableObject(pEngine),
	gameEngine(pEngine)
{
	m_iDrawWidth = 37; 
	m_iDrawHeight = 37;

	if(iYCoord > gameEngine->GetWindowHeight() - m_iDrawHeight)
		m_iCurrentScreenY = m_iPreviousScreenY = GetEngine()->GetWindowHeight() - m_iDrawHeight;
	else
		m_iCurrentScreenY = m_iPreviousScreenY = iYCoord;

	if (iXCoord > gameEngine->GetWindowWidth() - m_iDrawWidth)
		m_iCurrentScreenX = m_iPreviousScreenX = GetEngine()->GetWindowWidth() - m_iDrawWidth;
	else
		m_iCurrentScreenX = m_iPreviousScreenX = iXCoord;
	
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;

	SetVisible(true);
}


PSYMW6UserObj::~PSYMW6UserObj(void)
{
}


void PSYMW6UserObj::Draw()
{
	if (!IsVisible())
		return;


	if (energyOn == 0)
		im.LoadImage("playerstationary.png");
	else
		im.LoadImage("playerenergyright.png");

	if (gameEngine->IsKeyPressed(SDLK_LEFT) || gameEngine->IsKeyPressed(SDLK_a))
	{
		if(energyOn == 0)
			im.LoadImage("playerleft.png");
		if (energyOn == 1)
			im.LoadImage("playerenergyleft.png");
	}

	if (gameEngine->IsKeyPressed(SDLK_SPACE) || gameEngine->IsKeyPressed(SDLK_UP))
	{
		jumpAnim = 1;	
	}
	if (jumpAnim > 0 && jumpAnim < 5)
	{
		im.LoadImage("playersquat.png");
		jumpAnim++;
	}
	else
		jumpAnim = 0;

	im.RenderImageWithMask(gameEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, im.GetWidth(), im.GetHeight());

	StoreLastScreenPositionForUndraw();
}


void PSYMW6UserObj::DoUpdate(int iCurrentTime) // implement collision with visible tiles, and auto objects
{
	if (!IsVisible())
		return;

	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	if (gameEngine->IsKeyPressed(SDLK_LEFT) || gameEngine->IsKeyPressed(SDLK_a))
	{
		m_iCurrentScreenX -= 3;
	}
	if (gameEngine->IsKeyPressed(SDLK_RIGHT) || gameEngine->IsKeyPressed(SDLK_d))
	{	
		m_iCurrentScreenX += 3;
	}
	if ((gameEngine->IsKeyPressed(SDLK_SPACE) || gameEngine->IsKeyPressed(SDLK_UP)) && (CheckTile(3) == 1 || CheckTile(3) == 2 || CheckTile(4) == 1 || CheckTile(4) == 2 || onPlatform) && (!isJumping))
	{
			currentVelocity = 20;	// set to 15 for actual game play, 20 is for testing.
			isJumping = true;
	}
	else if (CheckTile(3) == 1 || CheckTile(3) == 2 || CheckTile(4) == 1 || CheckTile(4) == 2)
	{
		if(isJumping || isFalling)
			jumpAnim = 1;
		int overshoot = (m_iCurrentScreenY + m_iDrawHeight) % 20; // set to 15 for actual game play, 20 is for testing.
		m_iCurrentScreenY -= overshoot;
		currentVelocity = 0;
		isJumping = false;
		isFalling = false;
	}

	TileCollision();
	SimulateGravity();
	ObjectCollision();

	if (m_iCurrentScreenX < 0)
		m_iCurrentScreenX = 0;
	if (m_iCurrentScreenX >= gameEngine->GetWindowWidth() - m_iDrawWidth)
		m_iCurrentScreenX = gameEngine->GetWindowWidth() - m_iDrawWidth;
	if (m_iCurrentScreenY < 0)
		m_iCurrentScreenY = 0;
	if (m_iCurrentScreenY >= gameEngine->GetWindowHeight() - m_iDrawHeight)
	{
		m_iCurrentScreenY = gameEngine->GetWindowHeight() - m_iDrawHeight;
		currentVelocity = -10;
	}

	RedrawObjects();
}


long PSYMW6UserObj::Notify(int iSignalNumber)
{
	switch(iSignalNumber)
	{
	case 0:
		SetVisible(false);
		break;
	case 1:
		SetVisible(true);
		break;
	case 2: 
		mouseClicked = 1;
		break;
	case 3:
		mouseClicked = 0;
		break;
	}
	return 0;
}


void PSYMW6UserObj::TileCollision() // implement collision handling with tiles
{
	for (int i = 0; i < 9; i++)
	{
		switch (CheckTile(i))
		{
		case 7:
		{
			//gameEngine->UpdateScore(coinValue);
			ChangeTile(i, 1);
		}
			break;
		case 9:
			gameEngine->ChangeState(PSYMW6Engine::gameClear);
			break;
		}
	}
	

	if (mouseClicked == 1)
	{
		if (CheckTile(5) == 4)
		{
			ChangeTile(5, 5);
			ChangeTilesWithValue(6, 1);
		}
		if (CheckTile(6) == 4)
		{
			ChangeTile(6, 5);
			ChangeTilesWithValue(6, 1);
		}
		if (CheckTile(7) == 4)
		{
			ChangeTile(7, 5);
			ChangeTilesWithValue(6, 1);
		}
	}

}


int PSYMW6UserObj::CheckTile(int spritePos) //pos 1: top left, 2: top right, 3: bottom left, 4: bottom right, 5: bottom left current tile, 6: bottom right current tile, 7: bottom middle tile, 8: top middle tile, 9: centre tile
{
	PSYMW6TileManager& tm = gameEngine->GetTileManager();
	switch (spritePos)
	{
	case 1:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX );
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY );
		return tm.GetValue(i, j);
	}
	case 2:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + m_iDrawWidth );
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY );
		return tm.GetValue(i, j);
	}
	case 3:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight);
		return tm.GetValue(i, j);
	}
	case 4:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + m_iDrawWidth);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight);
		return tm.GetValue(i, j);
	}
	case 5:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight - 1);
		return tm.GetValue(i, j);
	}
	case 6:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + m_iDrawWidth);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight - 1);
		return tm.GetValue(i, j);
	}
	case 7:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + (m_iDrawWidth / 2));
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight - 1);
		return tm.GetValue(i, j);
	}
	case 8:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + (m_iDrawWidth / 2));
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY);
		return tm.GetValue(i, j);
	}
	case 9:
	{
		int i = tm.GetTileXForPositionOnScreen(GetXCentre());
		int j = tm.GetTileYForPositionOnScreen(GetYCentre());
		return tm.GetValue(i, j);
	}
	default:
		return 0;
	}
}


void PSYMW6UserObj::ChangeTile(int spritePos, int tileValue)
{
	PSYMW6TileManager& tm = gameEngine->GetTileManager();
	switch (spritePos)
	{
	case 1:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY);
		tm.UpdateTile(gameEngine, i, j, tileValue);
	}
		break;
	case 2:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + m_iDrawWidth);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY);
		tm.UpdateTile(gameEngine, i, j, tileValue);
	}
		break;
	case 3:
	{
		//int i = tm.GetTileXForPositionOnScreen(GetXCentre());
		//int j = tm.GetTileYForPositionOnScreen(GetYCentre());

		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight);
		tm.UpdateTile(gameEngine, i, j, tileValue);
	}
		break;
	case 4:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + m_iDrawWidth);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight);
		tm.UpdateTile(gameEngine, i, j, tileValue);
	}
		break;
	case 5:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight - 1);
		tm.UpdateTile(gameEngine, i, j, tileValue);
		printf("%d\n", tm.GetValue(i, j));
	}
	break;
	case 6:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + m_iDrawWidth);
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight - 1);
		tm.UpdateTile(gameEngine, i, j, tileValue);
	}
	break;
	case 7:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + (m_iDrawWidth / 2));
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY + m_iDrawHeight - 1);
		tm.UpdateTile(gameEngine, i, j, tileValue);
	}
	break;
	case 8:
	{
		int i = tm.GetTileXForPositionOnScreen(m_iCurrentScreenX + (m_iDrawWidth / 2));
		int j = tm.GetTileYForPositionOnScreen(m_iCurrentScreenY);
		tm.UpdateTile(gameEngine, i, j, tileValue);
	}
	break;
	case 9:
	{
		int i = tm.GetTileXForPositionOnScreen(GetXCentre());
		int j = tm.GetTileYForPositionOnScreen(GetYCentre());
		tm.UpdateTile(gameEngine, i, j, tileValue);
	}
	break;
	default:
		return;
	}
}


void PSYMW6UserObj::SimulateGravity()
{
	m_iCurrentScreenY -= currentVelocity;

	if (CheckTile(3) != 1 && CheckTile(3) != 2 && CheckTile(4) != 1 && CheckTile(4) != 2 && !onPlatform)
	{
		isFalling = true;
		if (currentVelocity < -10)
			currentVelocity = -10;
		else
			currentVelocity--;
	}
	if ((currentVelocity > 0 || currentVelocity < 0) && !onPlatform)
		energyOn = 1;
	else if (currentVelocity == 0)
		energyOn = 0;
}


void PSYMW6UserObj::ChangeTilesWithValue(int tileValue, int newValue)
{
	PSYMW6TileManager& tm = gameEngine->GetTileManager();
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			if (tm.GetValue(j, i) == tileValue)
			{
				tm.UpdateTile(gameEngine ,j, i, newValue);
			}
		}
	}
}


void PSYMW6UserObj::ObjectCollision()
{
	DisplayableObject* pObject;

	for (int objId = 0; (pObject = gameEngine->GetDisplayableObject(objId)) != NULL; objId++) // check for object type
	{
		if (PSYMW6UserObj* _pObject = dynamic_cast<PSYMW6UserObj*>(pObject))
		{
			continue;// do nothing
		}
		else if (PSYMW6Platform* _pObject = dynamic_cast<PSYMW6Platform*>(pObject))
		{
			if ((m_iCurrentScreenY + m_iDrawHeight) >= _pObject->getCurrentScreenY() && (m_iCurrentScreenY + m_iDrawHeight) <= (_pObject->getCurrentScreenY() + _pObject->getHeight()))
			{
				if ((m_iCurrentScreenX >= _pObject->getCurrentScreenX() && m_iCurrentScreenX <= (_pObject->getCurrentScreenX() + _pObject->getWidth())) || (m_iCurrentScreenX + m_iDrawWidth) >= _pObject->getCurrentScreenX() && (m_iCurrentScreenX + m_iDrawWidth) <= (_pObject->getCurrentScreenX() + _pObject->getWidth()))
				{
					m_iCurrentScreenY = _pObject->getCurrentScreenY() - m_iDrawHeight;
					currentVelocity = 0;
					onPlatform = true;
					isJumping = false;
					isFalling = false;
				}
				else if (_pObject->getCurrentScreenX() >= m_iCurrentScreenX && _pObject->getCurrentScreenX() <= (m_iCurrentScreenX + m_iDrawWidth))
				{
					m_iCurrentScreenY = _pObject->getCurrentScreenY() - m_iDrawHeight;
					currentVelocity = 0;
					onPlatform = true;
					isJumping = false;
					isFalling = false;
				}
			}
			else
				onPlatform = false;
		}
		else if (PSYMW6Laser* _pObject = dynamic_cast<PSYMW6Laser*>(pObject))
		{
			if ((m_iCurrentScreenY + m_iDrawHeight) >= _pObject->getCurrentScreenY() && (m_iCurrentScreenY + m_iDrawHeight) <= (_pObject->getCurrentScreenY() + _pObject->getHeight()))
			{
				if ((m_iCurrentScreenX >= _pObject->getCurrentScreenX() && m_iCurrentScreenX <= (_pObject->getCurrentScreenX() + _pObject->getWidth())) || (m_iCurrentScreenX + m_iDrawWidth) >= _pObject->getCurrentScreenX() && (m_iCurrentScreenX + m_iDrawWidth) <= (_pObject->getCurrentScreenX() + _pObject->getWidth()))
				{
					gameEngine->ChangeState(PSYMW6Engine::gameFail); // lose game on contact
				}
				else if (_pObject->getCurrentScreenX() >= m_iCurrentScreenX && _pObject->getCurrentScreenX() <= (m_iCurrentScreenX + m_iDrawWidth))
				{
					gameEngine->ChangeState(PSYMW6Engine::gameFail); // lose game on contact
				}
			}
			else if ((m_iCurrentScreenY) >= _pObject->getCurrentScreenY() && (m_iCurrentScreenY) <= (_pObject->getCurrentScreenY() + _pObject->getHeight()))
			{
				if ((m_iCurrentScreenX >= _pObject->getCurrentScreenX() && m_iCurrentScreenX <= (_pObject->getCurrentScreenX() + _pObject->getWidth())) || (m_iCurrentScreenX + m_iDrawWidth) >= _pObject->getCurrentScreenX() && (m_iCurrentScreenX + m_iDrawWidth) <= (_pObject->getCurrentScreenX() + _pObject->getWidth())) 
				{
					gameEngine->ChangeState(PSYMW6Engine::gameFail); // lose game on contact
				}
				else if (_pObject->getCurrentScreenX() >= m_iCurrentScreenX && _pObject->getCurrentScreenX() <= (m_iCurrentScreenX + m_iDrawWidth))
				{
					gameEngine->ChangeState(PSYMW6Engine::gameFail); // lose game on contact
				}
			}
		}
		else if (PSYMW6Coin* _pObject = dynamic_cast<PSYMW6Coin*>(pObject))
		{
			if ((m_iCurrentScreenY + m_iDrawHeight) >= _pObject->getCurrentScreenY() && (m_iCurrentScreenY + m_iDrawHeight) <= (_pObject->getCurrentScreenY() + _pObject->getHeight()))
			{
				if ((m_iCurrentScreenX >= _pObject->getCurrentScreenX() && m_iCurrentScreenX <= (_pObject->getCurrentScreenX() + _pObject->getWidth())) || (m_iCurrentScreenX + m_iDrawWidth) >= _pObject->getCurrentScreenX() && (m_iCurrentScreenX + m_iDrawWidth) <= (_pObject->getCurrentScreenX() + _pObject->getWidth()))
				{
					if (_pObject->IsVisible())
					{
						gameEngine->UpdateScore(coinValue);
						_pObject->setChangeVisible(false);
						_pObject->SetVisible(false);
					}
				}
				else if (_pObject->getCurrentScreenX() >= m_iCurrentScreenX && _pObject->getCurrentScreenX() <= (m_iCurrentScreenX + m_iDrawWidth))
				{
					if (_pObject->IsVisible())
					{
						gameEngine->UpdateScore(coinValue);
						_pObject->setChangeVisible(false);
						_pObject->SetVisible(false);
					}
				}
			}
			else if ((m_iCurrentScreenY) >= _pObject->getCurrentScreenY() && (m_iCurrentScreenY) <= (_pObject->getCurrentScreenY() + _pObject->getHeight()))
			{
				if ((m_iCurrentScreenX >= _pObject->getCurrentScreenX() && m_iCurrentScreenX <= (_pObject->getCurrentScreenX() + _pObject->getWidth())) || (m_iCurrentScreenX + m_iDrawWidth) >= _pObject->getCurrentScreenX() && (m_iCurrentScreenX + m_iDrawWidth) <= (_pObject->getCurrentScreenX() + _pObject->getWidth()))
				{
					if (_pObject->IsVisible())
					{
						gameEngine->UpdateScore(coinValue);
						_pObject->setChangeVisible(false);
						_pObject->SetVisible(false);
					}
				}
				else if (_pObject->getCurrentScreenX() >= m_iCurrentScreenX && _pObject->getCurrentScreenX() <= (m_iCurrentScreenX + m_iDrawWidth))
				{
					if (_pObject->IsVisible())
					{
						gameEngine->UpdateScore(coinValue);
						_pObject->setChangeVisible(false);
						_pObject->SetVisible(false);
					}
				}
			}
		}
	}
}