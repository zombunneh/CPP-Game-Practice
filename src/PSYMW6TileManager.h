#pragma once
#include "TileManager.h"
#include "JPGImage.h"

class PSYMW6TileManager :
	public TileManager
{
public:
	PSYMW6TileManager(void);
	~PSYMW6TileManager(void);
	void DrawTileAt(BaseEngine* pEngine,
		SDL_Surface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const;
};

