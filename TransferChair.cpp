#include "Core_Global.h"
#include "Tile.h"
#include "TransferChair.h"
#include <iostream>
#include <fstream>
using namespace std;

cTransferChair::cTransferChair()
{
	px = GetSystemMetrics(SM_CXSCREEN)*0.5-256;
	py = GetSystemMetrics(SM_CYSCREEN)*0.5-256;
	
	Init();
}

cTransferChair::~cTransferChair()
{
}

BOOL cTransferChair::Init()
{
	ifstream readPosition1, readPosition2;
	readPosition1.open("..\\SeuVisitor\\Data\\map\\SmallGameMap\\transferChair.map", ios::in);

	if(!readPosition1)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}

	for(int i=0; i<3; i++)
	{
		readPosition1>>chairs[i].xPos>>chairs[i].yPos;
	}

	readPosition1.close();

	readPosition2.open("..\\SeuVisitor\\Data\\map\\SmallGameMap\\transferChair-1.map", ios::in);

	if(!readPosition2)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}

	for(i=0; i<6; i++)
		for(int j=0; j<10; j++)
		{
			readPosition2>>barrier[i][j];
		}

	readPosition2.close();

	m_XPos = 320;
	m_YPos = 160;
	direction = 8;
	return true;
}

BOOL cTransferChair::Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard)
{
	m_Mouse = mouse;
	m_KeyBoard = keyBoard;
	m_Hdc = hdc;
	m_Texture0.Load(graphics, "..\\SeuVisitor\\Data\\pic\\sgames\\transferChair.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
	return true;
}

BOOL cTransferChair::UseTile(cTiles *tile)
{
	m_Tile = tile;
	return true;
}

BOOL cTransferChair::Play()
{
	if(m_Mouse->GetButtonState(MOUSE_LBUTTON)==TRUE)
	{
		int temX = m_Mouse->GetXPos();
		int temY = m_Mouse->GetYPos();
		int m = (temY-py-97) / 40;
		int n = (temX-px-19) / 40;
		int m0 = m_YPos / 40;
		int n0 = m_XPos / 40;
		int dm = 0;
		int dn = 0;
		int mark = 0;

		if(m0!=m && n0==n) // Go northward or go southward
		{	
			// Refresh the direction of the PC
			if(m > m0)
			{
				dm = 1;
				direction = 8;
			}
			else
			{
				dm = -1;
				direction = 24;
			}
			// Check whether the PC can go ahead
			if(barrier[m0+dm][n0]!=1)
			{				
				for(int i=0; i<3; i++)
				{
					if(chairs[i].xPos==n0 && chairs[i].yPos==m0+dm)
					{
						if(barrier[m0+dm*2][n0]==0)
						{
							mark = 1;
							chairs[i].yPos += dm;
							barrier[m0+dm*2][n0]=2;
							barrier[m0+dm][n0]=0;
						}
						else
							mark = 2;
					}
				}

				if(mark != 2)
				{
					int tmp = m_YPos+40*dm;
					if(tmp/40>=0 && tmp/40<=5)
						m_YPos = tmp;
				}
			}
		}
		else if(m0==m && n0!=n) // Go westward or go eastward
		{
			// Refresh the direction of the PC
			if(n > n0)
			{
				dn = 1;
				direction = 0;
			}
			else
			{
				dn = -1;
				direction = 16;
			}
			// Check whether the PC can go ahead
			if(barrier[m0][n0+dn] != 1)
			{
				for(int i=0; i<3; i++)
				{
					if(chairs[i].xPos==n0+dn && chairs[i].yPos==m0)
					{
						if(barrier[m0][n0+dn*2]==0 )
						{
							mark = 1;
							chairs[i].xPos += dn;
							barrier[m0][n0+dn*2]=2;
							barrier[m0][n0+dn]=0;
						}
						else
							mark = 2;
					}
				}
				if(mark != 2)
					m_XPos += (40*dn);
			}
		}
	}
	Draw();
	return CheckOver();
}

BOOL cTransferChair::Draw()
{
	m_Texture0.Blit(px, py, 0, 0, 507, 421);

	for(int i=0; i<3; i++)
	{
		m_Texture0.Blit(px+chairs[i].xPos*40+19, py+chairs[i].yPos*40+97, 472, 472, 40, 40);
	}

	m_Tile->Draw(m_PC+7, direction, px+m_XPos+2, py+m_YPos+74);

	return true;
}

BOOL cTransferChair::CheckOver()
{
	if(chairs[0].xPos==2 && chairs[0].yPos==0
		&& chairs[1].xPos==6 && chairs[1].yPos==0
		&& chairs[2].xPos==8 && chairs[2].yPos==0)
		return true;

	return false;
}