#ifndef _TRANSFERBLOCK_H_
#define _TRANSFERBLOCK_H_
#include "SmallGame.h"

typedef struct
{
	int xPos, yPos;
	int gapDirection; //0:no direction 1:north 2:east 3:south 4:west
	int size; //4:biggest 3:bigger 2:big 1:small
}sBlock;

class cTransferBlock:public cSmallGame
{
public:
	cTransferBlock();
	virtual ~cTransferBlock();
	BOOL Init();
	BOOL Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard);
	BOOL Play();
	BOOL Refresh(int m0, int n0, int m1, int n1); // Refresh information of block and gridding
	BOOL RefreshGridding(int m, int n, int middleBlock, int outBlock); // Refresh information of gridding
	BOOL Draw();
	BOOL CheckOpen();
	BOOL CheckOver();

private:
	sBlock blocks[5]; // Record information of each block
	int gridding[2][5][2]; // Record the gridding's gap direction and the inner block's size
	int outerBlock; // Outer block which contain the little block
	int middleBlock; // Middle block in outer block
	int button; // The button pressed
};

#endif