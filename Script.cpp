#include "Core_Global.h"
#include "Script.h"

#include <iostream>
#include <fstream>
using namespace std;

cScript::cScript()
{
	m_dialog[0] = '\0';
	//Set the position of dialog frame and dialog
	XPos = 0.5*GetSystemMetrics(SM_CXSCREEN)-500; 
	YPos = GetSystemMetrics(SM_CYSCREEN)-220;
	xPos = XPos + 42;
	yPos = YPos + 40;
	//Initial the begining position of reading
	m_offSet = 0;
}

cScript::~cScript()
{
	Free();
}

BOOL cScript::Create(cInputDevice *Mouse, cFont *Font, cTiles *Tiles)
{
	m_Mouse = Mouse;
	m_Font = Font;
	m_Tiles = Tiles;
	return true;
}

BOOL cScript::InitFrame()
{
	for(int j = 0; j < 32; j++)
		m_diaFrame[0][j] = 1;	
	
	for(int i = 1; i < 5; i++)
	for(j = 0; j < 32; j++)
		m_diaFrame[i][j] = 5;
	
	for(j = 0; j < 32; j++)
		m_diaFrame[5][j] = 9;

	m_diaFrame[0][0] = 0;
	m_diaFrame[0][31] = 2;
	for(i=1; i<5; i++)
	{
		m_diaFrame[i][0] = 4;
		m_diaFrame[i][31] = 6;
	}
	m_diaFrame[5][0] = 8;
	m_diaFrame[5][31] = 10;

	return true;
}
    
BOOL cScript::RenderDialog(bool unLock, char *FileName)
{
	char x;
	static int time = 0;

	//Read dialogs from script
	if(unLock)
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
 
		for(int i=0; i<512; i++)
			m_dialog[i] = '\0';

		for(i=0; x!='@'; i++)
		{
	    	m_dialog[i] = x;
			
			if(x=='$')
				i--;
	    	x = readDialog.get();
			if(x == '#')
			{
				readDialog.close();
				m_offSet = 0;
				return false;
			}
		}

	 	m_offSet += (i+9);
	}
			
	//Draw the dialog frame
	int m1, m2;
	D3DCOLOR color;
	for(int l=0; l<6; l++)
	  for(int i=0; i<=time; i++)
	  {
		  color = 0xffffffff;
		  m1 = time - i;
		  m2 = 15 - i;
		  if(m_diaFrame[l][m1] == 5)
			  color = D3DCOLOR_ARGB(160, 0, 255, 255);
		  m_Tiles->Draw(5, m_diaFrame[l][m1], XPos+m2*32, YPos+l*32, color);
    
		  m1 = 31 - time + i;
		  m2 = 16 + i;
		  m_Tiles->Draw(5, m_diaFrame[l][m1], XPos+m2*32, YPos+l*32, color);
	  }

	if(time != 15)
		time++;

	//Display the dialog
	if(time == 15)
		m_Font->Print(m_dialog, xPos, yPos);

	return true;
}

BOOL cScript::Free()
{
	m_Mouse = NULL;
	m_Font = NULL;
	m_Tiles = NULL;
	m_offSet = 0;
	m_dialog[0] = '\0';
	return true;
}