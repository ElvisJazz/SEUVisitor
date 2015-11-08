#ifndef _TRANSFERCHAIR_H_
#define _TRANSFERCHAIR_H_
#include "SmallGame.h"

typedef struct
{
	int xPos, yPos;
}sChair;

class cTransferChair:public cSmallGame
{
public:
	cTransferChair();
	virtual ~cTransferChair();
	BOOL Init();
	BOOL UseTile(cTiles *tile);
	BOOL Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard);
	BOOL Play();
	BOOL Draw();
	BOOL CheckOver();

private:
	sChair chairs[3];
	int barrier[6][10];
	int direction;
	int m_XPos;
	int m_YPos;
};

#endif