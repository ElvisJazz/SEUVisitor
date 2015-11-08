#include "Core_Global.h"
#include "TransferBall.h"
#include <iostream>
#include <fstream>
using namespace std;

cTransferBall::cTransferBall()
{
	px = GetSystemMetrics(SM_CXSCREEN)*0.5-256;
	py = GetSystemMetrics(SM_CYSCREEN)*0.5-256;
	isNorth = isSouth = isWest = isEast = false;
	isSelected = false;
	part = 0;
	
	Init();
}

cTransferBall::~cTransferBall()
{
}

BOOL cTransferBall::Init()
{
	ifstream readPosition1, readPosition2;
	readPosition1.open("..\\SeuVisitor\\Data\\map\\SmallGameMap\\transferBall.map", ios::in);

	if(!readPosition1)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}
   
	// Record the information of gridding
	for(int i=0; i<6; i++)
		for(int j=0; j<6; j++)
		{
			readPosition1>>gridding[0][i][j];
		}
	for(i=0; i<6; i++)
		for(int j=0; j<6; j++)
		{
			readPosition1>>gridding[1][i][j];
		}

	readPosition1.close();

	readPosition2.open("..\\SeuVisitor\\Data\\map\\SmallGameMap\\transferBall-1.map", ios::in);

	if(!readPosition2)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}

	// Record the position of each ball
	for(i=0; i<3; i++) 
	{
		readPosition2>>greenBalls1[i].xPos>>greenBalls1[i].yPos;
	}
	for(i=0; i<12; i++)
	{
		readPosition2>>purpleBalls1[i].xPos>>purpleBalls1[i].yPos;
	}
	for(i=0; i<5; i++)
	{
		readPosition2>>yellowBalls1[i].xPos>>yellowBalls1[i].yPos;
	}
    for(i=0; i<3; i++) 
	{
		readPosition2>>greenBalls2[i].xPos>>greenBalls2[i].yPos;
	}
	for(i=0; i<5; i++)
	{
		readPosition2>>purpleBalls2[i].xPos>>purpleBalls2[i].yPos;
	}
	for(i=0; i<2; i++)
	{
		readPosition2>>yellowBalls2[i].xPos>>yellowBalls2[i].yPos;
	}
	for(i=0; i<4; i++)
	{
		readPosition2>>blueBalls2[i].xPos>>blueBalls2[i].yPos;
	}

	readPosition2.close();

	return true;
}

BOOL cTransferBall::Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard)
{
	m_Mouse = mouse;
	m_KeyBoard = keyBoard;
	m_Hdc = hdc;
	m_Texture0.Load(graphics, "..\\SeuVisitor\\Data\\pic\\sgames\\transferBall.bmp", D3DCOLOR_ARGB(255, 0, 0, 0));
	return true;
}

BOOL cTransferBall::Play()
{
	int temX = m_Mouse->GetXPos();
	int temY = m_Mouse->GetYPos();
	int m, n;
	bool isSelected0 = false;

    // Find direction of arrows point to
	m = (temX-px-170) / 25;
	n = (temY-py-55) / 25;
	if(n>=0 && n<=5 && m>=0 && m<=5)
	{
		part = 0;
		if(FindDirection(m, n, 0))
			isSelected0 = true;
	}
	else
	{
		n = (temY-py-308) / 25;
		if(n>=0 && n<=5 && m>=0 && m<=5) 
		{
			part = 1;
			if(FindDirection(m, n, 1))
				isSelected0 = true;
		}
	}

	// Check the terminal ball whether it is selected 
	if(m_Mouse->GetButtonState(MOUSE_LBUTTON) == TRUE)
	{
		if(isSelected0)
		{
			isSelected = true;
			firstBallXPos = m;
			firstBallYPos = n;
		}
	}
	else
		isSelected = false;

	// Move balls which is selected
	if(!isSelected0 && isSelected && gridding[part][n][m]==0)
		MoveBalls(m, n);
	
	Draw();
	return CheckOver();
}

BOOL cTransferBall::FindDirection(int m, int n, int part0) // Find direcions of arrows
{
	int x = -1;
	int y = -1;
	int extralDistance = 0;
	if(part0 == 0)
	{
		extralDistance = 55;
		sBall terminalBalls1[6];
		terminalBalls1[0] = greenBalls1[0];
		terminalBalls1[1] = greenBalls1[2];
		terminalBalls1[2] = purpleBalls1[0];
		terminalBalls1[3] = purpleBalls1[11];
		terminalBalls1[4] = yellowBalls1[0];
		terminalBalls1[5] = yellowBalls1[4];
		
		for(int i=0; i<6; i++)
		{
			int j = terminalBalls1[i].xPos / 25;
			int k = terminalBalls1[i].yPos / 25;
			if(m==j && n==k)
			{
				firstMovingBall = i;
				x = j;
				y = k;
				break;
			}
		}
	}
	else if(part0 == 1)
	{
		extralDistance = 308;
		sBall terminalBalls2[8];
		terminalBalls2[0] = greenBalls2[0];
		terminalBalls2[1] = greenBalls2[2];
		terminalBalls2[2] = purpleBalls2[0];
		terminalBalls2[3] = purpleBalls2[4];
		terminalBalls2[4] = yellowBalls2[0];
		terminalBalls2[5] = yellowBalls2[1];
		terminalBalls2[6] = blueBalls2[0];
		terminalBalls2[7] = blueBalls2[3];

		for(int i=0; i<8; i++)
		{
			int j = terminalBalls2[i].xPos / 25;
			int k = terminalBalls2[i].yPos / 25;
			if(m==j && n==k)
			{
				firstMovingBall = part0 * 6 + i;
				x = j;
				y = k;
				break;
			}
		}
	}

	if(x==-1 && y==-1)
		return false;
	else
	{
		if(x>0 && gridding[part0][y][x-1]==0)
		{
			isWest = true;
			arrowPos[2][0] = (x-1) * 25 + 170;
			arrowPos[2][1] = y * 25 + extralDistance;
		}
		if(x<5 && gridding[part0][y][x+1]==0)
		{
			isEast = true;
			arrowPos[3][0] = (x+1) * 25 + 170;
			arrowPos[3][1] = y * 25 + extralDistance;
		}
		if(y>0 && gridding[part0][y-1][x]==0)
		{
			isNorth = true;
			arrowPos[0][0] = x * 25 + 170;
			arrowPos[0][1] = (y-1) * 25 + extralDistance;
		}
		if(y<5 && gridding[part0][y+1][x]==0)
		{
			isSouth = true;
			arrowPos[1][0] = x * 25 + 170;
			arrowPos[1][1] = (y+1) * 25 + extralDistance;
		}

		return true;
	}
}

BOOL cTransferBall::MoveBalls(int x, int y) // Move the selected string of balls
{
	if(((x-firstBallXPos==-1 || x-firstBallXPos==1) && y==firstBallYPos)
		|| ((y-firstBallYPos==-1 || y-firstBallYPos==1) && x==firstBallXPos))
	{
		int numOfBalls;
		int m, n;
		int order = firstMovingBall % 2;
		sBall *balls = NULL;

		switch(firstMovingBall)
		{
		case 0:
		case 1: balls = greenBalls1;
				numOfBalls = 3;
				break;
		case 2:
		case 3: balls = purpleBalls1;
				numOfBalls = 12;
			    break;
		case 4: 
		case 5: balls = yellowBalls1;
				numOfBalls = 5;
			    break;
		case 6:
		case 7: balls = greenBalls2;
				numOfBalls = 3;
			    break;
		case 8:
		case 9: balls = purpleBalls2;
				numOfBalls = 5;
				break;
		case 10:
		case 11:balls = yellowBalls2;
				numOfBalls = 2;
				break;
		case 12:
		case 13:balls = blueBalls2;
				numOfBalls = 4;
				break;
		default:break;
		}
	
		// Move balls one by one in order
		if(order == 0)
		{	
			m = balls[numOfBalls-1].xPos / 25;
			n = balls[numOfBalls-1].yPos / 25;
			gridding[part][n][m] = 0;
			gridding[part][y][x] = 1;

			for(int i=numOfBalls-1; i>0; i--)
			{
				balls[i].xPos = balls[i-1].xPos;
				balls[i].yPos = balls[i-1].yPos;
			}
			balls[0].xPos = x * 25;
			balls[0].yPos = y * 25;
		}
		else if(order == 1)
		{	
			m = balls[0].xPos / 25;
			n = balls[0].yPos / 25;
			gridding[part][y][x] = 1;
			gridding[part][n][m] = 0;

			for(int i=0; i<numOfBalls-1; i++)
			{
				balls[i].xPos = balls[i+1].xPos;
				balls[i].yPos = balls[i+1].yPos;
			}
			balls[numOfBalls-1].xPos = x * 25;
			balls[numOfBalls-1].yPos = y * 25;
		}
	}

	return true;
}

BOOL cTransferBall::Draw()
{
	m_Texture0.Blit(px, py, 0, 0, 487, 512);

	for(int i=0; i<3; i++) 
	{
		m_Texture0.Blit(px+greenBalls1[i].xPos+170, py+greenBalls1[i].yPos+55, 487, 0, 25, 25);
	}
	for(i=0; i<12; i++)
	{
		m_Texture0.Blit(px+purpleBalls1[i].xPos+170, py+purpleBalls1[i].yPos+55, 487, 25, 25, 25);
	}
	for(i=0; i<5; i++)
	{
		m_Texture0.Blit(px+yellowBalls1[i].xPos+170, py+yellowBalls1[i].yPos+55, 487, 50, 25, 25);
	}
    for(i=0; i<3; i++) 
	{
		m_Texture0.Blit(px+greenBalls2[i].xPos+170, py+greenBalls2[i].yPos+308, 487, 0, 25, 25);
	}
	for(i=0; i<5; i++)
	{
		m_Texture0.Blit(px+purpleBalls2[i].xPos+170, py+purpleBalls2[i].yPos+308, 487, 25, 25, 25);
	}
	for(i=0; i<2; i++)
	{
		m_Texture0.Blit(px+yellowBalls2[i].xPos+170, py+yellowBalls2[i].yPos+308, 487, 50, 25, 25);
	}
	for(i=0; i<4; i++)
	{
		m_Texture0.Blit(px+blueBalls2[i].xPos+170, py+blueBalls2[i].yPos+308, 487, 75, 25, 25);
	}

	if(isNorth)
	{
		m_Texture0.Blit(px+arrowPos[0][0], py+arrowPos[0][1], 487, 100, 25, 25);
		isNorth = false;
	}
	if(isSouth)
	{
		m_Texture0.Blit(px+arrowPos[1][0], py+arrowPos[1][1], 487, 125, 25, 25);
		isSouth = false;
	}
	if(isWest)
	{
		m_Texture0.Blit(px+arrowPos[2][0], py+arrowPos[2][1], 487, 150, 25, 25);
		isWest = false;
	}
	if(isEast)
	{
		m_Texture0.Blit(px+arrowPos[3][0], py+arrowPos[3][1], 487, 175, 25, 25);
		isEast = false;
	}
	return true;
}

BOOL cTransferBall::CheckOver()
{
	if((greenBalls1[0].xPos==125 && greenBalls1[0].yPos==125) || (greenBalls1[2].xPos==125 && greenBalls1[2].yPos==125))
		if((greenBalls2[0].xPos==0 && greenBalls2[0].yPos==0) || (greenBalls2[2].xPos==0 && greenBalls2[2].yPos==0))
			return true;

	return false;
}