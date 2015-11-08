#ifndef _HUARONGDAO_H_
#define _HUARONGDAO_H_
#include "SmallGame.h"

typedef struct
{
	int xPos, yPos;
	int toXPos, toYPos;
	int width, height;
}sSodier;

class cHuaRongDao:public cSmallGame
{
public:
	cHuaRongDao();
	virtual ~cHuaRongDao();

	BOOL Init();
	BOOL Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard);
	BOOL Play();
	BOOL Draw();
	BOOL CheckOver();

private:
	sSodier sodiers[10];

};

#endif