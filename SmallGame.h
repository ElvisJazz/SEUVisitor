#ifndef _SMALLGAME_H_
#define _SMALLGAME_H_

#include "Tile.h"
class cSmallGame
{
	public:
	cSmallGame(){}
	virtual ~cSmallGame()
	{
		m_Select = -1;
		m_Texture0.Free();
		m_Texture1.Free();
		m_Mouse = NULL;
		m_KeyBoard = NULL;
	}

	virtual BOOL Init(){return true;}

	virtual BOOL Create(HDC hdc,cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard)
	{
		m_Mouse = mouse;
		m_KeyBoard = keyBoard;
		return true;
	}

	BOOL UseTile(cTiles *tile, int PC)
	{
		m_Tile = tile;
		m_PC = PC;
		return true;
	}

	virtual BOOL Play(){return true;}
	virtual BOOL Draw(){return true;}
	virtual BOOL CheckOver(){return true;}

protected:
	HDC m_Hdc;
	int px, py;
	int m_Select;
	cTexture m_Texture0, m_Texture1;
	cInputDevice *m_Mouse;
	cInputDevice *m_KeyBoard;
	cTiles *m_Tile;
	int m_PC;
};
#endif