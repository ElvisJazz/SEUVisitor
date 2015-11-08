#ifndef _TRANSFERBALL_H
#define _TRANSFERBALL_H
#include "SmallGame.h"

typedef struct
{
	int xPos, yPos;
}sBall;

class cTransferBall:public cSmallGame
{
public:
	cTransferBall();
	virtual ~cTransferBall();
	BOOL Init();
	BOOL Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard);
	BOOL Play();
	BOOL FindDirection(int m, int n, int part0); // Check directions of arrows
	BOOL MoveBalls(int x, int y); // Move the selected string of balls
	BOOL Draw();
	BOOL CheckOver();

private:
	// Record information of each ball
	sBall greenBalls1[3];
	sBall purpleBalls1[12];
	sBall yellowBalls1[5];

	sBall greenBalls2[3];
	sBall purpleBalls2[5];
	sBall yellowBalls2[2];
	sBall blueBalls2[4];

	int  gridding[2][6][6]; // Value -1 means no way, 0 means nothing in the way and 1 means some ball in the way
	int  arrowPos[4][2]; // Position of 4 arrows
	int  firstMovingBall; // Mark the first moving ball
	int  firstBallXPos;
	int  firstBallYPos;
	int  part;
	bool isNorth;
	bool isSouth;
	bool isWest;
	bool isEast;
	bool isSelected;
};

#endif
