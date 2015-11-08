#include "Core_Global.h"
#include "HuaRongDao.h"
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

cHuaRongDao::cHuaRongDao()
{
	px = GetSystemMetrics(SM_CXSCREEN)*0.5-256;
	py = GetSystemMetrics(SM_CYSCREEN)*0.5-256;

	Init();
}

cHuaRongDao::~cHuaRongDao()
{
	//cSmallGame::~cSmallGame();
}

BOOL cHuaRongDao::Init()
{
	ifstream readPosition1;
	readPosition1.open("..\\SeuVisitor\\Data\\map\\SmallGameMap\\HRD.map", ios::in);

	if(!readPosition1)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}

	m_Select = -1;
	sodiers[0].height = sodiers[0].width = 120; //Caocao
	readPosition1>>sodiers[0].xPos;
	readPosition1>>sodiers[0].yPos;

	for(int i=1; i<6; i++) //Five famous generals
	{
		sodiers[i].width = 120;
		sodiers[i].height = 60;
		readPosition1>>sodiers[i].xPos;
		readPosition1>>sodiers[i].yPos;
	}
	for(i=6; i<10; i++)
	{
		sodiers[i].height = sodiers[i].width = 60; //Arms
		readPosition1>>sodiers[i].xPos>>sodiers[i].yPos;
	}

	readPosition1.close();

	char address[60];
	srand(time(0));
	int num = 1 ; //rand()%4+1num = ; demo
	ifstream readPosition2;
	sprintf(address, "..\\\\SeuVisitor\\\\Data\\\\map\\\\SmallGameMap\\\\HRD-%d.map", num);
	readPosition2.open(address, ios::in);
	
	if(!readPosition2)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}

	for(i=0; i<10; i++)
	{
		readPosition2>>sodiers[i].toXPos>>sodiers[i].toYPos;
		sodiers[i].toXPos += px;
		sodiers[i].toYPos += py;
	}

	readPosition2.close();

	return true;
}

BOOL cHuaRongDao::Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard)
{
	m_Mouse = mouse;
	m_KeyBoard = keyBoard;
	m_Hdc = hdc;

	m_Texture0.Load(graphics, "..\\SeuVisitor\\Data\\pic\\sgames\\back.bmp", D3DCOLOR_ARGB(255, 255, 255, 255));
	m_Texture1.Load(graphics,"..\\SeuVisitor\\Data\\pic\\sgames\\HRD.bmp");
	return true;
}

BOOL cHuaRongDao::Play()
{
	
	if(m_Mouse->GetButtonState(MOUSE_LBUTTON) == TRUE)
	{
		Sleep(100);
		int temX = m_Mouse->GetXPos();
		int temY = m_Mouse->GetYPos();

		if(::GetPixel(m_Hdc, temX, temY) != RGB(206, 239, 8))
		{
			for(int i=0; i<10; i++)
			{
				if(sodiers[i].toXPos<temX && sodiers[i].toXPos+sodiers[i].width>temX
					&& sodiers[i].toYPos<temY && sodiers[i].toYPos+sodiers[i].height>temY)
					m_Select = i;
			}
		}
		else if(m_Select != -1)		
		{
			int x = sodiers[m_Select].toXPos+sodiers[m_Select].width;
			int y = sodiers[m_Select].toYPos+sodiers[m_Select].height;
			if(temX-x>0 && ::GetPixel(m_Hdc, x+30, sodiers[m_Select].toYPos+30)==RGB(206, 239, 8)
				&& ::GetPixel(m_Hdc, x+30, y-30)==RGB(206, 239, 8)) //Move east
				sodiers[m_Select].toXPos += 60;
			else if(temY-y>0 && ::GetPixel(m_Hdc, sodiers[m_Select].toXPos+30, y+30)==RGB(206, 239, 8) 
				&& ::GetPixel(m_Hdc, x-30, y+30)==RGB(206, 239, 8)) //Move south
				sodiers[m_Select].toYPos += 60;
			else if(temX-sodiers[m_Select].toXPos<0 
				&& ::GetPixel(m_Hdc, sodiers[m_Select].toXPos-30, sodiers[m_Select].toYPos+30)==RGB(206, 239, 8) 
				&& ::GetPixel(m_Hdc, sodiers[m_Select].toXPos-30, y-30)==RGB(206, 239, 8)) //Move west
				sodiers[m_Select].toXPos -= 60;
			else if(temY-sodiers[m_Select].toYPos<0 
				&& ::GetPixel(m_Hdc, sodiers[m_Select].toXPos+30, sodiers[m_Select].toYPos-30)==RGB(206, 239, 8) 
				&& ::GetPixel(m_Hdc, x-30, sodiers[m_Select].toYPos-30)==RGB(206, 239, 8)) //Move north
				sodiers[m_Select].toYPos -= 60;
			
			m_Select = -1;
		}
	}

	Draw();
	return CheckOver();
}

BOOL cHuaRongDao::Draw()
{
	D3DCOLOR color = 0xffffffff;

	m_Texture0.Blit(px, py, 0, 0, 512, 512);
	for(int i=0; i<10; i++)
	{
		if(m_Select == i)
			color = 0xfff00000;
		else
			color = 0xffffffff;
		m_Texture1.Blit(sodiers[i].toXPos, sodiers[i].toYPos, sodiers[i].xPos, sodiers[i].yPos, \
			sodiers[i].width, sodiers[i].height, 1.0f, 1.0f, color);
	}
	return true;
}

BOOL cHuaRongDao::CheckOver()
{
	if(sodiers[0].toYPos>=py+340)
		return true;

	return false;
}
