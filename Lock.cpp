#include "Core_Global.h"
#include "Lock.h"
#include <iostream>
#include <fstream>
using namespace std;

cLock::cLock()
{
	px = GetSystemMetrics(SM_CXSCREEN)*0.5-128;
	py = GetSystemMetrics(SM_CYSCREEN)*0.5-128;
    m_Select = -1;

	Init();
}

cLock::~cLock()
{
	//cSmallGame::~cSmallGame();
}

BOOL cLock::Init()
{
	ifstream readPosition, readPosition1, readPosition2;
	readPosition.open("..\\SeuVisitor\\Data\\map\\SmallGameMap\\lock.map", ios::in);
	if(!readPosition)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}
	for(int i=0; i<3; i++)
		for(int j=0; j<6; j++)
			readPosition>>a[i][j];
	readPosition.close();

	readPosition1.open("..\\SeuVisitor\\Data\\map\\SmallGameMap\\lock-1.map", ios::in);
	if(!readPosition1)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}
	for(i=0; i<12; i++)
		readPosition1>>b[i][0]>>b[i][1];
	readPosition1.close();

	readPosition2.open("..\\SeuVisitor\\Data\\map\\SmallGameMap\\lock-2.map", ios::in);
	if(!readPosition2)
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}
	for(i=0; i<6; i++)
		readPosition2>>arrow[i][0]>>arrow[i][1];	
	readPosition2.close();

	return true;
}

BOOL cLock::Create(HDC hdc, cGraphics *graphics, cInputDevice *mouse, cInputDevice *keyBoard)
{
	m_Mouse = mouse;
	m_KeyBoard = keyBoard;
	m_Hdc = hdc;

	m_Texture0.Load(graphics, "..\\SeuVisitor\\Data\\pic\\sgames\\lock.bmp", D3DCOLOR_ARGB(255, 255, 255, 255));
	m_Texture1.Load(graphics, "..\\SeuVisitor\\Data\\pic\\sgames\\arrow.bmp", D3DCOLOR_ARGB(255, 255, 255, 255));
	
	return true;
}

BOOL cLock::Play()
{
	int temX = m_Mouse->GetXPos();
	int temY = m_Mouse->GetYPos();

	for(int i=0; i<6; i++)
		if(temX>px+arrow[i][0] && temX<px+arrow[i][0]+30 && temY>py+arrow[i][1]+5 && temY<py+arrow[i][1]+35)
		{
			m_Select = i;
			break;
		}
	if(i == 6)
		m_Select = -1;
	
	if(m_Mouse->GetButtonState(MOUSE_LBUTTON)==TRUE && m_Select!=-1)
	{
		Sleep(200);

		switch(m_Select)
		{
		case 0: Loop(1, 0);
				a[1][1]=a[0][2];
				a[1][3]=a[0][0];
				a[2][0]=a[0][3];
				a[2][2]=a[0][1];
				break;
		case 1: Loop(0, 1);
			    a[0][0]=a[1][3];
				a[0][2]=a[1][1];
				a[2][1]=a[1][2];
				a[2][3]=a[1][0];
				break;
		case 2: Loop(0, 0);
				a[1][1]=a[0][2];
				a[1][3]=a[0][0];
				a[2][0]=a[0][3];
				a[2][2]=a[0][1];
				break;
		case 3: Loop(1, 1);
				a[0][0]=a[1][3];
				a[0][2]=a[1][1];
				a[2][1]=a[1][2];
				a[2][3]=a[1][0];
				break;
		case 4: Loop(0, 2);
				a[0][3]=a[2][0];
				a[0][1]=a[2][2];
				a[1][0]=a[2][3];
				a[1][2]=a[2][1];
				break;
		case 5: Loop(1, 2);
				a[0][3]=a[2][0];
				a[0][1]=a[2][2];
				a[1][0]=a[2][3];
				a[1][2]=a[2][1];
				break;
		default: break;
		}

		m_Select = -1;
	}

	Draw();
	return CheckOver();
}
void cLock::Loop(int order, int plate)
{
	if(order == 0)
	{
		int j = a[plate][5];
		for(int i=5; i>0; i--)
			a[plate][i] = a[plate][i-1];
		a[plate][0] = j;
	}
	else
	{
		int j = a[plate][0];
		for(int i=0; i<5; i++)
			a[plate][i] = a[plate][i+1];
		a[plate][5] = j;
	}
}

BOOL cLock::Draw()
{
	m_Texture0.Blit(px, py, 0, 0, 256, 256);
	if(m_Select != -1)
	{
		if(m_Select < 4)
			m_Texture1.Blit(px+arrow[m_Select][0], py+arrow[m_Select][1], 30*m_Select, 0, 30, 40);
		else
			m_Texture1.Blit(px+arrow[m_Select][0], py+arrow[m_Select][1], 30*(m_Select-4), 40, 30, 40);	
	}

	int k = 0;
	for(int i=0; i<6; i++,k++)
	{
		if(a[0][i] == 0)
			m_Texture1.Blit(px+b[k][0], py+b[k][1], 60, 40, 30, 40);
		else
			m_Texture1.Blit(px+b[k][0], py+b[k][1], 90, 40, 30, 40);
	}
    int x[4] = {4, 5, 0, 2};
	int j;
	for(i=0; i<4; i++,k++)
	{
		j = x[i];
		if(a[1][j] == 0)
			m_Texture1.Blit(px+b[k][0], py+b[k][1], 60, 40, 30, 40);
		else
			m_Texture1.Blit(px+b[k][0], py+b[k][1], 90, 40, 30, 40);
	}
	for(i=0; i<2; i++,k++)
	{
		j = x[i];
		if(a[2][j] == 0)
			m_Texture1.Blit(px+b[k][0], py+b[k][1], 60, 40, 30, 40);
		else
			m_Texture1.Blit(px+b[k][0], py+b[k][1], 90, 40, 30, 40);
	}
	return true;
}

BOOL cLock::CheckOver()
{
	if(a[0][0]==0 && a[0][1]==0 && a[0][2]==0 && a[0][3]==0 && a[1][0]==0 && a[1][2]==0)
		return true;
	else
		return false;
}
