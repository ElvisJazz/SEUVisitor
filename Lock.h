#ifndef _LOCK_H_
#define _LOCK_H_
#include "SmallGame.h"

class cLock:public cSmallGame
{
public:
	cLock();
	virtual ~cLock();

	BOOL Init();
	BOOL Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard);
	BOOL Play();
	BOOL Draw();
	BOOL CheckOver();
	void Loop(int order, int plate);

private:
	int a[3][6];
	int b[12][2];
	int arrow[6][2];
};

#endif