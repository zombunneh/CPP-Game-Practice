#include "header.h"
#include "stdlib.h"
#include "PSYMW6TileManager.h"
#include "JPGImage.h"



PSYMW6TileManager::PSYMW6TileManager(void)
	: TileManager(20, 20)
{
	
}


PSYMW6TileManager::~PSYMW6TileManager(void)
{
}


void PSYMW6TileManager::DrawTileAt(BaseEngine* pEngine,
	SDL_Surface* pSurface,
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY) const
{
	ImageData button;
	button.LoadImage("button.png");
	ImageData coin1;
	ImageData button2;
	ImageData coin2;
	ImageData goalTile2;
	ImageData stone;
	stone.LoadImage("stoneblock.png");
	ImageData dirt;
	dirt.LoadImage("dirtblock.png");

	switch (GetValue(iMapX, iMapY))
	{
	case 0:
		switch (rand() % 4)
		{
		case 1:
			dirt.RenderImage(pSurface, 0, 20, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
			break;
		case 2:
			dirt.RenderImage(pSurface, 20, 0, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
			break;
		case 3:
			dirt.RenderImage(pSurface, 20, 20, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
			break;
		default:
			dirt.RenderImage(pSurface, 20, 20, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
			break;
		}
		break;
	case 1: 
		switch (rand() % 5)
		{
		case 1:
			stone.RenderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
			break;
		case 2:
			stone.RenderImage(pSurface, 0, 20, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
			break;
		case 3:
			stone.RenderImage(pSurface, 20, 0, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
			break;
		case 4:
			stone.RenderImage(pSurface, 20, 20, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
			break;
		default:
			stone.RenderImage(pSurface, 20, 20, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
			break;
		}
		break;
	case 2: 
		dirt.RenderImage(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
		break;
	case 3: // 100% empty tile
		break;
	case 4: //tile that will change to make a new bridge(case1)
		button.FlexibleRenderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight(), 0, 0, 0);
		break;
	case 5: // tile indicating button pressed
		button.FlexibleRenderImageWithMask(pSurface, 0, 20, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight(), 0, 0, 0);
		break; 
	case 6: // tile that will be changed by  case 5 to case 1
		break;
	case 7: // coin tile that changes to case 8 on collection
		break;
	case 8: // coin collected image then changes to case 3 CURRENTLY REDUNDANT
		break;
	case 9: // this tile will be goal tile (transitions to game clear and does appropriate score calculation)
		goalTile2.LoadImage("goaltile.png");
		goalTile2.RenderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
		break;
	}
}
