#include "Core_Global.h"
#include "TransferBlock.h"
#include <iostream>
#include <fstream>
using namespace std;

cTransferBlock::cTransferBlock()
{
	px = GetSystemMetrics(SM_CXSCREEN)*0.5-256;
	py = GetSystemMetrics(SM_CYSCREEN)*0.5-256;
	
	Init();
}

cTransferBlock::~cTransferBlock()
{
}

BOOL cTransferBlock::Init()
{
	outerBlock = -1;
	middleBlock = -1;
	button = 0;

	for(int i=0; i<2; i++)
		for(int j=0; j<5; j++)
		{
			gridding[i][j][0] = 0;
			gridding[i][j][1] = 4;
		}

	ifstream readPosition;
	readPosition.open("..\\SeuVisitor\\Data\\map\\SmallGameMap\\transferBlock.map", ios::in);

	if(!readPosition)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}

	int m, n;
	for(i=0; i<5; i++) // Record the position of each block and the number of each block in each gridding
	{
		readPosition>>blocks[i].xPos>>blocks[i].yPos>>blocks[i].gapDirection>>blocks[i].size;
		n = blocks[i].xPos / 40;
		m = blocks[i].yPos / 40;
		gridding[m][n][0] = blocks[i].gapDirection; //Direction of the gridding's gap
		gridding[m][n][1] = blocks[i].size; //Size of inner blocks in the gridding
	}
	
	readPosition.close();
	return true;
}

BOOL cTransferBlock::Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard)
{
	m_Mouse = mouse;
	m_KeyBoard = keyBoard;
	m_Hdc = hdc;
	m_Texture0.Load(graphics, "..\\SeuVisitor\\Data\\pic\\sgames\\transferBlock.bmp", D3DCOLOR_ARGB(255, 255, 255, 255));
	return true;
}

BOOL cTransferBlock::Play()
{
	if(m_Mouse->GetButtonState(MOUSE_LBUTTON)==TRUE)
	{
		Sleep(200);

		int temX = m_Mouse->GetXPos();
		int temY = m_Mouse->GetYPos();
		int m = blocks[4].yPos / 40;
		int n = blocks[4].xPos / 40;
		int middleBlock0 = middleBlock;
		int outerBlock0 = outerBlock;

		if(temX>px+144 && temX<px+188 && temY>py+280 && temY<py+316) // Move to north
		{
			button = 1;
			if(m!=0 && (gridding[m-1][n][0]==0 || gridding[m-1][n][0]==3))
			{
				//Change information of outerBlock
				if(outerBlock!=-1 && blocks[outerBlock].size<gridding[m-1][n][1] && blocks[outerBlock].gapDirection!=1)
					blocks[outerBlock].yPos -= 40;
				else
					outerBlock = -1;
				//Change information of middleBlock
				if(middleBlock!=-1)
				{
					if(outerBlock==-1 && outerBlock0!=-1 && blocks[outerBlock0].gapDirection==1 
						&& blocks[middleBlock].gapDirection!=1 && blocks[middleBlock].size<gridding[m-1][n][1])
						blocks[middleBlock].yPos -= 40;
					else if(outerBlock!=-1 && (blocks[outerBlock].gapDirection!=3 || blocks[middleBlock].gapDirection!=1))
						blocks[middleBlock].yPos -= 40;
					else if(outerBlock0==-1 && blocks[middleBlock].gapDirection!=1 && blocks[middleBlock].size<gridding[m-1][n][1])
						blocks[middleBlock].yPos -= 40;
					else
						middleBlock = -1;
				}
				//Change information of littleBlock
				if(middleBlock!=-1 || outerBlock!=-1 || gridding[m][n][0]==1 || gridding[m][n][0]==0)
				{
					blocks[4].yPos -= 40;

					//Refresh information of block and gridding
					Refresh(m, n, m-1, n);
				}
			}
		}
		else if(temX>px+186 && temX<px+222 && temY>py+317 && temY<py+353) // Move to east
		{
			button = 2;
			if(n!=4 && (gridding[m][n+1][0]==0 || gridding[m][n+1][0]==4))
			{
				//Change information of outerBlock
				if(outerBlock!=-1 && blocks[outerBlock].size<gridding[m][n+1][1] && blocks[outerBlock].gapDirection!=2)
					blocks[outerBlock].xPos += 40;
				else
					outerBlock = -1;
				//Change information of middleBlock
				if(middleBlock!=-1)
				{
					if(outerBlock==-1 && outerBlock0!=-1 && blocks[outerBlock0].gapDirection==2 
						&& blocks[middleBlock].gapDirection!=2 && blocks[middleBlock].size<gridding[m][n+1][1])
						blocks[middleBlock].xPos += 40;
					else if(outerBlock!=-1 && (blocks[outerBlock].gapDirection!=4 || blocks[middleBlock].gapDirection!=2))
						blocks[middleBlock].xPos += 40;
					else if(outerBlock0==-1 && blocks[middleBlock].gapDirection!=2 && blocks[middleBlock].size<gridding[m][n+1][1])
						blocks[middleBlock].xPos += 40;
					else
						middleBlock = -1;
				}
				//Change information of littleBlock
				if(middleBlock!=-1 || outerBlock!=-1 || gridding[m][n][0]==2 || gridding[m][n][0]==0)
				{
					blocks[4].xPos += 40;

					//Refresh information of block and gridding
					Refresh(m, n, m, n+1);
				}
			}
			else if(n==4 && m==1 && CheckOpen())
			{
				if(middleBlock!=-1)
					blocks[middleBlock].xPos += 40;
				if(outerBlock!=-1)
					blocks[outerBlock].xPos += 40;
				blocks[4].xPos += 40;

				Refresh(m, n, m, n+1);
			}
		}
		else if(temX>px+144 && temX<px+188 && temY>py+348 && temY<py+394) // Move to south
		{
			button = 3;
			if(m!=1 && (gridding[m+1][n][0]==0 || gridding[m+1][n][0]==1))
			{
				//Change information of outerBlock
				if(outerBlock!=-1 && blocks[outerBlock].size<gridding[m+1][n][1] && blocks[outerBlock].gapDirection!=3)
					blocks[outerBlock].yPos += 40;
				else
					outerBlock = -1;
				//Change information of middleBlock
				if(middleBlock!=-1)
				{
					if(outerBlock==-1 && outerBlock0!=-1 && blocks[outerBlock0].gapDirection==3 
						&& blocks[middleBlock].gapDirection!=3 && blocks[middleBlock].size<gridding[m+1][n][1])
						blocks[middleBlock].yPos += 40;
					else if(outerBlock!=-1 && (blocks[outerBlock].gapDirection!=1 || blocks[middleBlock].gapDirection!=3))
						blocks[middleBlock].yPos += 40;
					else if(outerBlock0==-1 && blocks[middleBlock].gapDirection!=3 && blocks[middleBlock].size<gridding[m+1][n][1])
						blocks[middleBlock].yPos += 40;
					else
						middleBlock = -1;
				}
				//Change information of littleBlock
				if(middleBlock!=-1 || outerBlock!=-1 || gridding[m][n][0]==3 || gridding[m][n][0]==0)
				{
					blocks[4].yPos += 40;

					//Refresh information of block and gridding
					Refresh(m, n, m+1, n);
				}
			}
		}

		else if(temX>px+100 && temX<px+135 && temY>py+317 && temY<py+353) // Move to west
		{
			button = 4;
			
	
			if(n!=0 && (gridding[m][n-1][0]==0 || gridding[m][n-1][0]==2))
			{
				//Change information of outerBlock
				if(outerBlock!=-1 && blocks[outerBlock].size<gridding[m][n-1][1] && blocks[outerBlock].gapDirection!=4)
					blocks[outerBlock].xPos -= 40;
				else
					outerBlock = -1;
				//Change information of middleBlock
				if(middleBlock!=-1)
				{
					if(outerBlock==-1 && outerBlock0!=0 && blocks[outerBlock0].gapDirection==4 
						&& blocks[middleBlock].gapDirection!=4 && blocks[middleBlock].size<gridding[m][n-1][1])
						blocks[middleBlock].xPos -= 40;
					else if(outerBlock!=-1 && (blocks[outerBlock].gapDirection!=2 || blocks[middleBlock].gapDirection!=4))
						blocks[middleBlock].xPos -= 40;
					else if(outerBlock0==-1 && blocks[middleBlock].gapDirection!=4 && blocks[middleBlock].size<gridding[m][n-1][1])
						blocks[middleBlock].xPos -= 40;
					else
						middleBlock = -1;
				}
				//Change information of littleBlock
				if(middleBlock!=-1 || outerBlock!=-1 || gridding[m][n][0]==4 || gridding[m][n][0]==0)
				{
					blocks[4].xPos -= 40;

					//Refresh information of block and gridding
					Refresh(m, n, m, n-1);
				}

			}
		}
		
		else if(temX>px+268 && temX<px+291 && temY>py+308 && temY<py+335) // Recover former statement
		{
			button = 5;
		}
	}

	Draw();
	return CheckOver();
}

BOOL cTransferBlock::Refresh(int m0, int n0, int m1, int n1) // Refresh information of block and gridding
{
	middleBlock = -1;
	outerBlock = -1;
	int x = -1;
	int y = -1;
	int tem1, tem2;
	//Block
	for(int i=0; i<4; i++)
	{
		tem1 = blocks[i].xPos / 40;
		tem2 = blocks[i].yPos / 40;
		if(tem1==n1 && tem2==m1)
		{
			if(i<3)
				outerBlock = i;
			else
				middleBlock = i;
		}
		if(tem1==n0 && tem2==m0)
		{
			if(i<3)
				x = i;
			else
				y = i;
		}
	}
	tem1 = middleBlock;
	tem2 = outerBlock;
	//Gridding
	RefreshGridding(m0, n0, y, x);
	RefreshGridding(m1, n1, tem1, tem2);

	return true;
}

BOOL cTransferBlock::RefreshGridding(int m, int n, int middleBlock, int outerBlock) // Refresh information of gridding
{
	if(middleBlock==-1 && outerBlock==-1)
	{
		gridding[m][n][0] = 0;
		gridding[m][n][1] = 4;
	}
	else if(middleBlock!=-1 && outerBlock!=-1)
	{
		gridding[m][n][1] = blocks[middleBlock].size;
		if(blocks[middleBlock].gapDirection != blocks[outerBlock].gapDirection)
			gridding[m][n][0] = -1;
		else
			gridding[m][n][0] = blocks[middleBlock].gapDirection;
	}
	else if(middleBlock != -1)
	{
		gridding[m][n][0] = blocks[middleBlock].gapDirection;
		gridding[m][n][1] = blocks[middleBlock].size;
	}
	else
	{
		gridding[m][n][0] = blocks[outerBlock].gapDirection;
		gridding[m][n][1] = blocks[outerBlock].size;
	}

	return true;
}

BOOL cTransferBlock::Draw()
{
	m_Texture0.Blit(px, py, 0, 0, 438, 478);

	switch(button)
	{
	case 1: m_Texture0.Blit(px+137, py+280, 472, 200, 40, 40);
			break;
	case 2: m_Texture0.Blit(px+179, py+317, 472, 240, 40, 40);
			break;
	case 3: m_Texture0.Blit(px+139, py+358, 472, 280, 40, 40);
			break;
	case 4: m_Texture0.Blit(px+100, py+322, 472, 320, 40, 40);
			break;
	case 5: m_Texture0.Blit(px+268, py+320, 472, 360, 40, 40);
			Init();
			break;
	default: break;
	}
	button = 0;

	for(int i=0; i<5; i++)
		m_Texture0.Blit(px+blocks[i].xPos+120, py+blocks[i].yPos+80, 472, i*40, 40, 40);

	if(!CheckOpen())
		m_Texture0.Blit(px+300, py+120, 472, 400, 40, 40);

	return true;
}

BOOL cTransferBlock::CheckOpen()
{
	if(blocks[2].xPos==blocks[3].xPos && blocks[2].yPos==blocks[3].yPos)
		return true;

	return false;
}

BOOL cTransferBlock::CheckOver()
{
	if(blocks[4].xPos>=200)
		return true;

	return false;
}