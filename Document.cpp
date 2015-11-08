#include "Core_Global.h"
#include "Document.h"
#include "GameRole.h"
#include <iostream>
#include <fstream>

using namespace std;

cDocument::cDocument(cGameRole *gameRole)
{
	m_GameRole = gameRole;
	m_dialog[0] = '\0';
	//Set the position of frame
	XPos = 0.5 * GetSystemMetrics(SM_CXSCREEN) - 384; 
	YPos = 0.5 * GetSystemMetrics(SM_CYSCREEN) - 256;
	xPos = XPos + 35;
	yPos = YPos + 80;
	//Initial the begining position of reading
	m_offSet = 0;
	m_pointButton = 0;
	m_selectButton = 2;
	m_subSelectButton = 0;
	m_sound = -1;
	InitFrame();
}

cDocument::~cDocument()
{
	Free();
}

BOOL cDocument::Create(cGraphics *Graphics, cInputDevice *Mouse, cFont *Font, cTiles *Tiles) // Create cScript class
{
	m_Mouse = Mouse;
	m_Font = Font;
	m_Tiles = Tiles;

	m_Texture0.Load(Graphics, "..\\SeuVisitor\\Data\\pic\\others\\menu3.bmp");
	return true;
}

BOOL cDocument::InitFrame() // Load data of dialog frame
{
	m_diaFrameBound[0][0] = 8;
	m_diaFrameBound[0][1] = 7;
	m_diaFrameBound[1][0] = 20;
	m_diaFrameBound[1][1] = 15;
	m_diaFrameBound[2][0] = 24;
	m_diaFrameBound[2][1] = 16;
	 
	return true;
}
void cDocument::ResetValue()
{
	m_pointButton = 0;
	m_selectButton = 2;
	m_subSelectButton = 0;
	m_sound = -1;
}

BOOL cDocument::MenuOption() // Option in menu
{
	m_temX = m_Mouse->GetXPos();
	m_temY = m_Mouse->GetYPos();

	if(m_temY-YPos>=0 && m_temY-YPos<=48)
	{
		int dx = m_temX - XPos - 8;
		m_pointButton = dx / 48 + 1;
		if(!(m_pointButton>=0 && m_pointButton<=3))
			m_pointButton = 0;
		else if(m_Mouse->GetButtonState(MOUSE_LBUTTON))
		{
			Sleep(200);
			m_selectButton = m_pointButton;
			m_sound = 1;
		}
	}
	else
		m_pointButton = 0;

	return RenderMenu();
}

BOOL cDocument::RenderMenu() // Display the menu
{
	int i1, i2, i3;
	bool isUnLock = false;

	switch(m_pointButton)
	{
	case 1: i1 = 1, i2 = i3 = 0; break;
	case 2: i2 = 1, i1 = i3 = 0; break;
	case 3: i3 = 1, i1 = i2 = 0; break;
	default:i1 = i2 = i3 = 0; break;
	}
	switch(m_selectButton)
	{
	case 1: i1 = 2; break;
	case 2: i2 = 2; break;
	case 3: i3 = 2; break;
	default: break;
	}

	m_Texture0.Blit(XPos+8, YPos, 100, 90+i1*32, 32, 32, 1.5, 1.5);
	m_Texture0.Blit(XPos+56, YPos, 132, 90+i2*32, 32, 32, 1.5, 1.5);
	m_Texture0.Blit(XPos+104, YPos, 164, 90+i3*32, 32, 32, 1.5, 1.5);

	if(m_selectButton != 2)
		m_subSelectButton = 0;
	switch(m_selectButton)
	{
	case 1:	RenderFrame(0, true);
			m_GameRole->RecordGrade(true, XPos, YPos);
			break;
	case 2: if(RenderFrame(0, true))
				return RenderSubMenu();
			break;
	case 3: if(m_Mouse->GetButtonState(MOUSE_LBUTTON))
			{
				Sleep(200);
				isUnLock = true;
				m_sound = 1;
			}
			RenderFrame(2, isUnLock, "..\\SeuVisitor\\Data\\document\\aboutUs.txt");
			break;
	default:break;
	}

	return true;
}

BOOL cDocument::RenderSubMenu()
{
	if(m_temX-XPos>52 && m_temX-XPos<202)
	{
		int dy = m_temY - YPos - 80;
		m_subPointButton = dy / 50 + 1;
		if(!(m_subPointButton>=0 && m_subPointButton<=3))
			m_subPointButton = 0;
		else if(m_Mouse->GetButtonState(MOUSE_LBUTTON))
		{
			Sleep(200);
			m_subSelectButton = m_subPointButton;
			m_sound = 1;
		}
	}
	else
		m_subPointButton = 0;

	int i1, i2, i3;
	switch(m_subPointButton)
	{
	case 1: i1 = 1, i2 = i3 = 0; break;
	case 2: i2 = 1, i1 = i3 = 0; break;
	case 3: i3 = 1, i1 = i2 = 0; break;
	default:i1 = i2 = i3 = 0; break;
	}
	switch(m_subSelectButton)
	{
	case 1: i1 = 2; break;
	case 2: i2 = 2; break;
	case 3: i3 = 2; break;
	default: break;
	}

	m_Texture0.Blit(XPos+52, YPos+80, 0, 0+i1*30, 100, 30, 1.5, 1.5);
	m_Texture0.Blit(XPos+52, YPos+130, 100, 0+i2*30, 100, 30, 1.5, 1.5);
	m_Texture0.Blit(XPos+52, YPos+180, 0, 90+i3*30, 100, 30, 1.5, 1.5);

	switch(m_subSelectButton)
	{
	case 1:	RenderFrame(-1, true, NULL);
			break;
	case 2: RenderFrame(-2, true, NULL);
			break;
	case 3: m_subSelectButton = 0;
			return false;
	default:break;
	}
	return true;
}

BOOL cDocument::RenderFrame(int frame, bool unLock, char *FileName) // Display the frame
{
	char x;
	int time;
	int selectTime;
	int extralValue = 0;
	static int time1 = 0;
	static int time2 = 0;
	static int oldFrame = 0;
	static int oldSubFrame = 1;

	if(frame>=0)
	{		
		if(oldFrame!=frame)
		{
			oldFrame = frame;
			time1 = 0;
		}
		time = time1;
		selectTime = 1;
	}
	else if(frame<0)
	{
		extralValue = 232;
		if(oldSubFrame!=frame)
		{
			oldSubFrame = frame;
			time2 = 0;
		}
		time = time2;
		selectTime = 2;
		frame = 1;
	}
	
	//Read dialogs from script
	if(FileName!=NULL && unLock)
	{
		ifstream readDialog;
		time = 0;
		readDialog.open(FileName, ios::in);
		if(!readDialog)
		{
			cerr<<"Data Wrong!"<<endl;
			exit(1);
		}
		readDialog.seekg(m_offSet,ios::cur);
	 
        x = readDialog.get();
 
		for(int i=0; i<1512; i++)
			m_dialog[i] = '\0';

		for(i=0; x!='@'; i++)
		{
	    	m_dialog[i] = x;
			
			if(x == '$')
				i--;
	    	x = readDialog.get();
			if(x == '#')
			{
				readDialog.close();
				m_offSet = 0;
				return false;
			}
		}

	 	m_offSet += (i+23);
		readDialog.close();
	}
			
	//Draw the frame
	int m1, m2, tileNum;
	int halfWidth = m_diaFrameBound[frame][0] / 2;
	D3DCOLOR color;
	for(int l=0; l<m_diaFrameBound[frame][1]; l++)
	  for(int i=0; i<=time; i++)
	  {
		  color = 0xffffffff;
		  m1 = time - i;
		  m2 = halfWidth - i - 1;
		  tileNum = GetTileNum(frame, l, m1);

		  if(tileNum == 5)
			  color = D3DCOLOR_ARGB(160, 0, 255, 255);
		  m_Tiles->Draw(5, tileNum, XPos+m2*32+extralValue, YPos+40+l*32, color);
    
		  m1 = m_diaFrameBound[frame][0] - time + i - 1;
		  m2 = halfWidth + i;
		  tileNum = GetTileNum(frame, l, m1);

		  m_Tiles->Draw(5, tileNum, XPos+m2*32+extralValue, YPos+40+l*32, color);
	  }

	if(time != halfWidth-1)
		time++;
	if(selectTime == 1)
		time1 = time;
	else
	{
		time2 = time;
		if(time == halfWidth-1)
			RenderChoice(-oldSubFrame);
	}
	//Display the content
	if(FileName!=NULL && time==halfWidth-1)
		m_Font->Print(m_dialog, xPos, yPos);
	
	if(time == halfWidth-1)
		return true;
	return false;
}

int  cDocument::GetSelectSound()
{
	int tmp = m_sound;
	m_sound = -1;
	return tmp;
}

BOOL cDocument::RenderChoice(int choice)
{
	
	D3DCOLOR color;
	int currentChoice = 0;
	char tmp[50];
	FILE *readData = NULL;

	if(m_temX-XPos>300 && m_temX-XPos<700)
		currentChoice = (m_temY - YPos) / 50 - 1;
	for(int i=1; i<=8; i++)
	{	
		if(i == currentChoice)
			color = D3DCOLOR_ARGB(255, 0, 0, 200);
		else
			color = D3DCOLOR_ARGB(255, 255, 255, 255);

		sprintf(tmp, "½ø¶È%d", i);
		m_Font->Print(tmp, XPos+300, YPos+50+i*50, 0, 0, color);

		sprintf(tmp, "..\\SeuVisitor\\Data\\document\\%d.history", i);
		if((readData=fopen(tmp,"r")) != NULL)
			fscanf(readData, "[ %s ]", tmp);
		else
			sprintf(tmp, "-----------------------------------------");

		m_Font->Print(tmp, XPos+500, YPos+50+i*50, 0, 0, color);

		if(readData!=NULL)
			fclose(readData);
	}
	
	if(m_Mouse->GetButtonState(MOUSE_LBUTTON) && currentChoice>0 && currentChoice<9)
	{	
		if(choice == 1)
			Load(currentChoice);
		else
			Save(currentChoice);
	}

	return true;
}

int cDocument::GetTileNum(int frame, int l, int m)
{
	if(l == 0)
	{
		if(m == 0)
			return 0;
		else if(m < m_diaFrameBound[frame][0]-1)
			return 1;
		else
			return 2;
	}
	else if(l < m_diaFrameBound[frame][1]-1)
	{
		if(m == 0)
			return 4;
		else if(m < m_diaFrameBound[frame][0]-1)
			return 5;
		else
			return 6;
	}
	else
	{
		if(m == 0)
			return 8;
		else if(m < m_diaFrameBound[frame][0]-1)
			return 9;
		else 
			return 10;
	}
}

BOOL cDocument::Save(int num)
{
	return m_GameRole->SaveData(num);
}

BOOL cDocument::Load(int num)
{
	return m_GameRole->LoadData(num);
}

BOOL cDocument::Free() // Free cScript class
{
	m_Mouse = NULL;
	m_Font = NULL;
	m_Tiles = NULL;
	m_offSet = 0;
	m_dialog[0] = '\0';
	m_Texture0.Free();
	return true;
}

cMyTime::cMyTime()
{
	duration = 0;
}

cMyTime::~cMyTime()
{
	start = 0;
	finish = 0;
	duration = 0;
}

void cMyTime::StartCount()
{
	start = ::clock();
}

void cMyTime::EndCount()
{
	finish = ::clock();
	duration += (double)(finish - start) / CLOCKS_PER_SEC;
}

char* cMyTime::GetClock()
{
	EndCount();
	int hour = (int)duration / 3600;
	int minute = ((int)duration % 3600) / 60;
	int second = ((int)duration - hour*3600 - minute*60);
	
	sprintf(clock, "%02d:%02d:%02d", hour, minute, second);
	StartCount();
	return clock;
}

int cMyTime::GetClock0()
{
	return duration;
}

char* cMyTime::GetCurrentTime()
{	
	time_t t = time(0);
	strftime(time0, sizeof(time0), "%Y/%m/%d/%X", localtime(&t));

	return time0;
}

void cMyTime::SetClock0(double duration0)
{
	duration = duration0;
}
