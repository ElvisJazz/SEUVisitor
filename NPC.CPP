#include "Core_Global.h"
#include "NPC.h"
#include <iostream>
#include <fstream>

using namespace std;

cNPC::cNPC()
{
	m_SWidth = m_SHeight = 0;
	m_Tiles = NULL;
	m_NumOfVolunteersToDraw = 0;
	m_NumOfWalkersToDraw = 0;
	m_StopWalker[0] = m_StopWalker[1] = -1;
}

cNPC::~cNPC()
{
	Free();
}

BOOL cNPC::Create(cTiles *Tile)
{
	m_Tiles = Tile;
	m_SWidth = GetSystemMetrics(SM_CXSCREEN);
	m_SHeight = GetSystemMetrics(SM_CYSCREEN);
	return true;
}

BOOL cNPC::Free()
{
	m_NumOfVolunteersToDraw = 0;
	m_NumOfWalkersToDraw = 0;
	m_StopWalker[0] = m_StopWalker[1] = -1;
	m_Tiles = NULL;
	return true;
}

int cNPC::GetNumberOfVolunteers()
{
	return m_NumOfVolunteersToDraw;
}

BOOL cNPC::CheckPCPosition(int xPos, int yPos, int XPos, int YPos, int &isMove, int oxPos, int oyPos)
{
	int x = xPos + XPos;
	int y = yPos + YPos;
	int ox = x + oxPos;
	int oy = y + oyPos;

	for(int i=0; i<m_NumOfWalkersToDraw; i++)
	{
		switch(walkers[i].Direc)
		{
		case 0: if(walkers[i].CurrentY+59>y && walkers[i].CurrentY+51<y &&
					walkers[i].CurrentX+33<x && walkers[i].CurrentX+49>x)
				{
					if(m_StopWalker[0] == -1)
						m_StopWalker[0] = i;
					else
						m_StopWalker[1] = i;

					walkers[i].CurrentX -= 4;
					walkers[i].Action = 0;
				
				}
			    break;
		case 1: if(walkers[i].CurrentX+19<x && walkers[i].CurrentX+47>x &&
					walkers[i].CurrentY+55<y && walkers[i].CurrentY+67>y)
				{
					if(m_StopWalker[0] == -1)
						m_StopWalker[0] = i;
					else
						m_StopWalker[1] = i;

					walkers[i].CurrentY -= 4;
					walkers[i].Action = 0;
				}
			    break;
		case 2: if(walkers[i].CurrentY+59>y && walkers[i].CurrentY+51<y &&
					walkers[i].CurrentX+33>x && walkers[i].CurrentX+17<x)
				{
					if(m_StopWalker[0] == -1)
						m_StopWalker[0] = i;
					else
						m_StopWalker[1] = i;

					walkers[i].CurrentX -= 4;
					walkers[i].Action = 0;
				}
			    break;
		case 3: if(walkers[i].CurrentX+19<x && walkers[i].CurrentX+47>x &&
					walkers[i].CurrentY+55>y && walkers[i].CurrentY+43<y)
				{
					if(m_StopWalker[0] == -1)
						m_StopWalker[0] = i;
					else
						m_StopWalker[1] = i;

					walkers[i].CurrentY += 4;
					walkers[i].Action = 0;
				}
			    break;
		default: break;
		}

		//Whether the PC is about to cross the walker
		if(isMove!=0 && ox>walkers[i].CurrentX+11 && ox<walkers[i].CurrentX+45
			&& oy>walkers[i].CurrentY+47 && oy<walkers[i].CurrentY+63)
			isMove = 0;
	}
	
	//Whether the PC is behind volunteers
	for(i=0; i<m_NumOfVolunteersToDraw; i++)
		if(xPos+XPos>volunteers[i].XPos-64 && xPos+XPos<volunteers[i].XPos+64
			&& yPos+YPos>volunteers[i].YPos-55 && yPos+YPos<volunteers[i].YPos)
			return true;
	
	return false;
}

BOOL cNPC::LoadVolunteer(char *FileName, int startTile, int pic)
{
    FILE *readChar;
	int x, y, photo, num;
	m_StartTile = startTile;

	if(!(readChar = fopen(FileName, "r")))
		return false;

    while(!feof(readChar))
	{
		fscanf(readChar, "(%d,%d,%d,%d)\r", &x, &y, &photo, &num);
		if(!AddVolunteer(x, y, photo, num))
		{
			cerr<<"Data Wrong!";
			exit(1);
		}
	}
	fclose(readChar);
	
	if(pic == 1)
		m_Tiles->Load(m_StartTile, "..\\SeuVisitor\\Data\\pic\\(N)PC\\volunteer1.bmp", 64, 64, D3DCOLOR_ARGB(255, 255, 0, 255));
	else
		m_Tiles->Load(m_StartTile, "..\\SeuVisitor\\Data\\pic\\(N)PC\\volunteer2.bmp", 64, 64, D3DCOLOR_ARGB(255, 255, 0, 255));
	return true;
}

BOOL cNPC::AddVolunteer(int XPos, int YPos, int Photo, int num)
{
	// Add data of each NPC to certain struct
	if(m_NumOfVolunteersToDraw < MAX_VOLUNTEERS) 
	{
		volunteers[m_NumOfVolunteersToDraw].XPos = XPos + 33;
		volunteers[m_NumOfVolunteersToDraw].YPos = YPos + 55;
		volunteers[m_NumOfVolunteersToDraw].Tile = num;
		volunteers[m_NumOfVolunteersToDraw].Photo = Photo;
		volunteers[m_NumOfVolunteersToDraw].Direc = m_NumOfVolunteersToDraw % 4;
		m_NumOfVolunteersToDraw++; 

		return true;
	}

	return false;
}


BOOL cNPC::RenderVolunteer(int XPos, int YPos, int xPos, int yPos, D3DCOLOR color, int currentNumOfTalkers)
{
	static DWORD LastTime = timeGetTime();
    DWORD ThisTime = timeGetTime();
    DWORD Elapsed = ThisTime - LastTime;
	 
	if(currentNumOfTalkers != -1)
		m_NumOfVolunteersToDraw = currentNumOfTalkers;

 	for(int i=0; i<m_NumOfVolunteersToDraw; i++)
	{
		int tile = volunteers[i].Tile;
		double x = volunteers[i].XPos - XPos;
		double y = volunteers[i].YPos - YPos;
		// Whether the Volunteer in the area of display
	 	if(x>0 && x<m_SWidth && y>0 && y<m_SHeight)
		{
			 x -= xPos;
			 y -= yPos;
			 
			 if( x*x+y*y < 3600)
			 {
				if(x == 0)
					x = 0.001;
				double ratio = y / x;
				// To east or west
				if(ratio>-0.2 && ratio<0.2)
				{
					if(x < 0)
						volunteers[i].Direc = 0;
					else
						volunteers[i].Direc = 4;
				}
				// To southeast  or northwest
				else if(ratio>=0.2 && ratio<=5)
				{
					if(x < 0)
						volunteers[i].Direc = 1;
					else
						volunteers[i].Direc = 5;
				}
				//To south or north
				else if(ratio>0.2 || ratio<-5)
				{
					if(y < 0)
						volunteers[i].Direc = 2;
					else
						volunteers[i].Direc = 6;
				}
				//To southwest or northeast
				else if(ratio>=-5 && ratio<=-0.2)
				{
					if(y < 0)
						volunteers[i].Direc = 3;
					else
						volunteers[i].Direc = 7;
				}
			 }
			else if(Elapsed > 1000) 
			{	 
				LastTime = ThisTime; 
				volunteers[i].Direc = (volunteers[i].Direc + 1) % 8;
			}
		   
			if(i == 2)
				m_Tiles->Draw(m_StartTile, 8+volunteers[i].Direc, volunteers[i].XPos-XPos-33, volunteers[i].YPos-YPos-55, color);
			else
				m_Tiles->Draw(m_StartTile, tile*8+volunteers[i].Direc, volunteers[i].XPos-XPos-33, volunteers[i].YPos-YPos-55, color);
		}
	}
	return true;
}

BOOL cNPC::LoadWalker(char *FileName, int startTile)
{
	FILE *readChar1;
	int x0, y0, x1, y1, x2, y2, x3, y3;
	m_TileOfWalker = startTile;

	if(!(readChar1 = fopen(FileName, "r")))
	{
		cerr<<"Data Wrong!";
		exit(1);
	}

    while(!feof(readChar1))
	{
		fscanf(readChar1, "(%d,%d,%d,%d,%d,%d,%d,%d)\r", &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
		if(!AddWalker(x0, y0, x1, y1, x2, y2, x3, y3))
			return false;
	}
    fclose(readChar1);
	return true;
}

BOOL cNPC::AddWalker(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
	char address[60];
	// Add data of each NPC to certain struct
	if(m_NumOfWalkersToDraw < MAX_WALKERS) 
	{
		walkers[m_NumOfWalkersToDraw].Pos[0][0] = x0;
		walkers[m_NumOfWalkersToDraw].Pos[0][1] = y0;
		walkers[m_NumOfWalkersToDraw].Pos[1][0] = x1;
		walkers[m_NumOfWalkersToDraw].Pos[1][1] = y1;
		walkers[m_NumOfWalkersToDraw].Pos[2][0] = x2;
		walkers[m_NumOfWalkersToDraw].Pos[2][1] = y2;
		walkers[m_NumOfWalkersToDraw].Pos[3][0] = x3;
		walkers[m_NumOfWalkersToDraw].Pos[3][1] = y3;
		walkers[m_NumOfWalkersToDraw].Line = 1;
		walkers[m_NumOfWalkersToDraw].Tile = m_TileOfWalker;
		walkers[m_NumOfWalkersToDraw].Action = 0;
		walkers[m_NumOfWalkersToDraw].Order = 1;
		walkers[m_NumOfWalkersToDraw].Mark = -1;
		walkers[m_NumOfWalkersToDraw].CurrentX = x0;
		walkers[m_NumOfWalkersToDraw].CurrentY = y0;

		SetDirection(m_NumOfWalkersToDraw);
		
		sprintf(address, "..\\\\SeuVisitor\\\\Data\\\\pic\\\\(N)PC\\\\Walker-%d.bmp", m_NumOfWalkersToDraw+1);
		m_Tiles->Load(m_TileOfWalker, address, 64, 64, D3DCOLOR_ARGB(255, 255, 0, 255));
		m_NumOfWalkersToDraw++; 
		m_TileOfWalker++;

		return true;
	}

	return false;
}

BOOL cNPC::RenderWalker(int mark, int XPos, int YPos, D3DCOLOR color)
{
	int temLine;
	for(int i=0; i<m_NumOfWalkersToDraw; i++)
	{
		 temLine = walkers[i].Line;
		//Set the line of NPC
		if(walkers[i].CurrentX==walkers[i].Pos[temLine][0] && walkers[i].CurrentY==walkers[i].Pos[temLine][1])
		{
			if(walkers[i].Line == 3)
				walkers[i].Order = -1;
			else if(walkers[i].Line == 0)
				walkers[i].Order = 1;

			walkers[i].Line += walkers[i].Order;
			walkers[m_NumOfWalkersToDraw].Action = 0;

			SetDirection(i); //Set new direction of NPC
		}

		if(mark == walkers[i].Mark)
		    m_Tiles->Draw(walkers[i].Tile, walkers[i].Direc*4+walkers[i].Action, walkers[i].CurrentX-XPos, walkers[i].CurrentY-YPos, color);
	}
    	
	return true;
}

BOOL cNPC::RefreshPosition()
{
	for(int i=0; i<m_NumOfWalkersToDraw; i++)
	{
		if(i!=m_StopWalker[0] && i!=m_StopWalker[1])
			walkers[i].Action = (walkers[i].Action+1)%4;
		switch(walkers[i].Direc)
		{
		case 0: walkers[i].CurrentX += 4; break;
		case 1: walkers[i].CurrentY += 4; break;
		case 2: walkers[i].CurrentX -= 4; break;
		case 3: walkers[i].CurrentY -= 4; break;
		default: break;
		}
	}
	m_StopWalker[0] = m_StopWalker[1] = -1;

	return true;
}

BOOL cNPC::SetDirection(int num)
{
	int temOrder = walkers[num].Order;
	int temLine = walkers[num].Line;

	if(walkers[num].Pos[temLine-temOrder][0] == walkers[num].Pos[temLine][0])
	{
		if(walkers[num].Pos[temLine-temOrder][1] > walkers[num].Pos[temLine][1])
			walkers[num].Direc = 3;
		else
			walkers[num].Direc = 1;
	}
	else if(walkers[num].Pos[temLine-temOrder][1] == walkers[num].Pos[temLine][1])
	{
		if(walkers[num].Pos[temLine-temOrder][0] > walkers[num].Pos[temLine][0])
			walkers[num].Direc = 2;
		else
			walkers[num].Direc = 0;
	}

	return true;
}

BOOL cNPC::MarkWalkerPosition(int XPos, int YPos, int xPos, int yPos, cBuilding *building)
{
	for(int i=0; i<m_NumOfWalkersToDraw; i++)
	{
		if(walkers[i].CurrentX+45>=XPos && walkers[i].CurrentX+19<=XPos+m_SWidth 
			&& walkers[i].CurrentY+55>=YPos && walkers[i].CurrentY+10<=YPos+m_SHeight) // In screen
		{
			int k;
			for(int j=0; j<10; j++)
			{
				k = building->m_SBuilding[j];
				if(k != -1)
				{
					if(walkers[i].CurrentX+45>=building->m_buildings[k].XPos1 && walkers[i].CurrentX+19<=building->m_buildings[k].XPos2 
						&& walkers[i].CurrentY+55>=building->m_buildings[k].YPos1 && walkers[i].CurrentY+10<=building->m_buildings[k].YPos2)
					{
						walkers[i].Mark = 0; // Behind the building
						break;
					}
					else
						walkers[i].Mark = -1;
				}
				else
					break;
			}
			if(walkers[i].Mark != 0)
			{
				if(walkers[i].CurrentY+55 < YPos+yPos)
					walkers[i].Mark = 1; // Behind the PC
				else
					walkers[i].Mark = 2; // In front of the PC
			}
		}
		else
			walkers[i].Mark = -1; // Out of screen
	}
	return true;
}
