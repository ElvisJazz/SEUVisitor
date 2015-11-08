#include "Core_Global.h"
#include "GameRole.h"
#include "stdio.h"
#include "WinMain.h"
#include "HuaRongDao.h"
#include "Lock.h"
#include "TransferBlock.h"
#include "TransferChair.h"
#include "TransferBall.h"

#include <string>
#include <cmath>

//using namespace std;

cGameRole::cGameRole()
{
	xPos = yPos = 0;
	XPos = YPos = 0;
	XPos0 = YPos0 = 0;
	oxPos = oyPos = 0;
	toXPos = toYPos = -1;
	m_SWidth = m_SHeight = 0;
	m_isMove = 0;
	m_talker = 0;
	m_distance = 0;
	m_extralChoice = 0;
	m_dialogChoice = 0;
	m_finishGame = false;
	m_inGame = false;
	m_isInitial = false;
	m_lock = false;
	m_rollX = m_rollY = false;
	m_dialog = false;
	m_off = false;
	m_cursor = false;
	m_displayBuildingPic = false;
	m_displayBuildingInfo = false;
	m_tiles = NULL;
	m_map = NULL;
	m_smallGame = NULL;
	m_time = NULL;
	m_graphics = NULL;
	m_font = NULL;
	m_mouse = NULL;
	m_keyBoard = NULL;
}

cGameRole::~cGameRole()
{
	Free();
}

BOOL cGameRole::Create(cMap *Map, cGraphics *Graphics, cTiles *Tiles, HWND hwnd, cFont *Font, 
					  cInputDevice *KeyBoard, cInputDevice *Mouse, int PC, int startTile,  
					  int TileX, int TileY, bool hasVolunteer) 
{
	m_map = Map;
	m_graphics = Graphics;
	m_tiles = Tiles;
	m_PC = PC;
	m_Hdc = GetDC(hwnd);
	m_font = Font;
	m_mouse = Mouse;
	m_keyBoard = KeyBoard;
	xTile = TileX;
	yTile = TileY;
	m_hasVolunteer = hasVolunteer;

	m_SWidth = GetSystemMetrics(SM_CXSCREEN);
	m_SHeight = GetSystemMetrics(SM_CYSCREEN);

	if(!hasVolunteer)
	{
		XPos = 2880 - 0.5*m_SWidth;
		YPos = 1632 - 0.5*m_SHeight;
		xPos = 0.5*m_SWidth;
	    yPos = 0.5*m_SHeight;
	}
	
	m_building.Create(Tiles);
	m_building.Load("..\\SeuVisitor\\Data\\map\\BuildingMap\\buildings.txt");

	m_NPC.Create(Tiles);
	m_NPC.LoadWalker("..\\SeuVisitor\\Data\\map\\NPCMap\\walker.txt", startTile);
	if(m_hasVolunteer)
	{
		m_volunteer = 9;
		m_NPC.LoadVolunteer("..\\SeuVisitor\\Data\\map\\NPCMap\\volunteer1.txt", startTile+10, 1);
	}
	else
	{
		if(m_time != NULL)
		{
			delete m_time;
			m_time = NULL;
		}
		m_time = new cMyTime(); 
		m_document = new cDocument(this);
		m_document->Create(Graphics, Mouse, Font, Tiles);
		m_Texture = new cTexture();
		m_Texture->Load(Graphics, "..\\SeuVisitor\\Data\\pic\\sgames\\box.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));

		m_volunteer = startTile+11;
		m_NPC.LoadVolunteer("..\\SeuVisitor\\Data\\map\\NPCMap\\volunteer2.txt", startTile+10, 0);
		m_tiles->Load(m_volunteer, "..\\SeuVisitor\\Data\\pic\\photos\\1.bmp", 140, 240, D3DCOLOR_ARGB(255, 255, 0, 255));
	}
	cApp::LoadMapData("..\\SeuVisitor\\Data\\map\\MainMap\\background.map", m_background);
	
	if(m_hasVolunteer)
		m_numOfVolunteersToDraw = m_NPC.GetNumberOfVolunteers();
	else
		m_numOfVolunteersToDraw = 1;

	 //Init frame of dialog
	m_script.Create(m_mouse, m_font, m_tiles);
	m_script.InitFrame();

	return true;
}



BOOL cGameRole::GetPos(char Choice)
{
	switch(Choice)
	{
	case 'N': XPos = 110*32 - 0.5*m_SWidth;
			  YPos = 0, xPos = 0.5*m_SWidth;
			  yPos = 5*32 + 55;
			  break;

	case 'E': XPos = xTile*32 - m_SWidth;
		      YPos = 51*32 - 0.5*m_SHeight;
			  xPos = m_SWidth - 5*32;
			  yPos = 0.5*m_SHeight;
			  break;

	case 'S': XPos = 67*32 - 0.5*m_SWidth;
			  YPos = yTile*32 - m_SHeight;
			  xPos = 0.5*m_SWidth;
			  yPos = m_SHeight - 5*32;
			  break;

	case 'W': XPos = 0; 
		      YPos = 51*32 - 0.5*m_SHeight;
			  xPos = 7*32;
			  yPos = 0.5*m_SHeight;
			  break;

	default:  return false; break;
	}
	return true;
}

BOOL cGameRole::Run()
{
	D3DCOLOR color = 0xffffffff;
	if(m_extralChoice == -1)
		m_extralChoice = 0;
    else if(m_extralChoice != 0)
		color = 0x0ffff000;
	
	if(!m_lock)
		m_isMove = 0;
	oxPos = oyPos = 0;
    XPos0 = XPos;
	YPos0 = YPos;

	m_map->Render(XPos, YPos, m_SHeight/32+1, m_SWidth/32+1, color);//Render big map
	
	//Set screen rolling
	m_rollX = m_rollY = true;

    if(m_keyBoard->GetKeyState(KEY_M) == TRUE)
	{
		Sleep(500);
		// Choose to display current position of the role in the whole school
		if(m_extralChoice == 0)
			m_extralChoice = 1;
		else if(m_extralChoice == 1)
			m_extralChoice = 0;
	 }
	else if(m_keyBoard->GetKeyState(KEY_N)==TRUE && m_inGame)
	{
		Sleep(500);
		if(m_extralChoice == 2)
			m_extralChoice = 0;
		
		if(!m_finishGame)
		{
			m_dialogChoice = 1;
			m_inGame = false;
			
			if(m_talker==0 || m_talker==4)
				m_isInitial = false;
		}
	}
	else if(!m_dialog && m_mouse->GetButtonState(MOUSE_RBUTTON))
	{
		if(m_hasVolunteer)
		{
			if(!m_displayBuildingPic)
			{
				int worldX = m_mouse->GetXPos() + XPos;
				int worldY = m_mouse->GetYPos() + YPos;
				if(m_building.CheckSelectedBuilding(worldX, worldY)) // Display information of buildings 
				{
					m_displayBuildingInfo = true;
					m_building.InitInformation(m_graphics, m_mouse);
				}
			}
		}
		else if(!m_dialog) // Game menu
		{
			Sleep(500);
			if(m_extralChoice == 0)
				m_extralChoice = 3;
			else if(m_extralChoice == 3)
			{
				m_document->ResetValue();
				m_extralChoice = 0;
			}
		}
	}
	else if(m_extralChoice==0 && !m_dialog && !m_displayBuildingInfo && m_mouse->GetButtonState(MOUSE_LBUTTON)) // Walk
	{
		m_cursor = true;
		// Get the destination of the role
		toXPos = m_mouse->GetXPos();
	    toYPos = m_mouse->GetYPos();
	
       if(abs(xPos - toXPos) > 3 || abs(yPos - toYPos) > 3)
	   {
		   // Get the direction the role want to go
		   double x = toXPos - xPos;
		   double y = toYPos - yPos;
		   if(x == 0)
			   x = 0.001;
		   double ratio = y / x;
		   m_distance = x*x + y*y;

		   m_angle = atan(ratio);
		   // To east or west
			if(ratio>-0.2 && ratio<0.2)
			{
				m_angle = 0; 
				if(toXPos-xPos > 0)
					m_isMove = 1;
				else
					m_isMove = 5;
			}
			// To southeast  or northwest
			else if(ratio>=0.2 && ratio<=5)
			{
				if(toXPos-xPos > 0)
					m_isMove = 2;
				else
					m_isMove = 6;
			}
			//To south or north
			else if(ratio>0.2 || ratio<-5)
			{
				m_angle = 3.1415926/2;
				if(toYPos-yPos > 0)
					m_isMove = 3;
				else
					m_isMove = 7;
			}
			//To southwest or northeast
			else if(ratio>=-5 && ratio<=-0.2)
			{
				if(toYPos-yPos > 0)
					m_isMove = 4;
				else
					m_isMove = 8;
			}	
			m_isMove2 = m_isMove; //Copy of the value m_isMove
		}
	   //Convert screen coordinates into map coordinates
			toXPos += XPos;
			toYPos += YPos;

		if(CheckDialog())
			m_dialog = true;
		else
			m_dialog = false;
	}

	switch(m_isMove)
	{
	case 0: break;

	case 1: oxPos += 6*cos(m_angle);
		    oyPos += 6*sin(m_angle);
			break;

	case 2: oxPos += 6*cos(m_angle);
		    oyPos += 6*sin(m_angle);
			break;

	case 3: oxPos += 6*cos(m_angle);
		    oyPos += 6*sin(m_angle);
			break;

	case 4: oxPos -= 6*cos(m_angle);
		    oyPos += 6*sin(-m_angle);
		    break;

	case 5: oxPos -= 6*cos(m_angle);
		    oyPos -= 6*sin(m_angle);
			break;

	case 6: oxPos -= 6*cos(m_angle);
		    oyPos -= 6*sin(m_angle);
			break;

	case 7: oxPos -= 6*cos(m_angle);
		    oyPos -= 6*sin(m_angle);
			break;

	case 8: oxPos += 6*cos(m_angle);
		    oyPos -= 6*sin(-m_angle);
			break;

    default: return false;
	}
	
 	Render(color); 
		
	//Extral service
	if(m_extralChoice == 1)
		RenderSmallMap();
	else if(m_extralChoice==2)
	{
		if(!m_isInitial)
		{
			if(m_smallGame != NULL)
			{
				delete m_smallGame;
				m_smallGame = NULL;
			}
			
			switch(m_talker)
			{
			case 0:	m_smallGame = new cTransferChair(); 
				    m_smallGame->UseTile(m_tiles, m_PC);
					break;
			case 1:	m_smallGame = new cTransferBlock(); break;
			case 2:	m_smallGame = new cLock(); break;
			case 3:	m_smallGame = new cTransferBall(); break;
			case 4:	m_smallGame = new cHuaRongDao(); break;
			default: break;
			}
			
			m_smallGame->Create(m_Hdc, m_graphics, m_mouse, m_keyBoard);
			m_isInitial = true;
		}

		if(m_smallGame->Play())
		{	
			m_finishGame = true;
			m_isInitial = false;
			m_inGame = false;
			m_dialog = true;
			m_dialogChoice = 2;
			m_extralChoice = 0;
			m_time->EndCount();
			
			if(m_talker==4 && RecordGrade(false))
			{
				m_talker++;
				m_numOfVolunteersToDraw--;
			}

			Sleep(200);
			Dialog();
		}
		else
		{
			m_font->Print(m_time->GetClock(), m_SWidth-100, m_SHeight-100);
			m_finishGame = false;
		}
	
		return false;
	}
	else if(m_extralChoice == 3)
	{
		if(!m_document->MenuOption())
			m_extralChoice = -1;
	}

	if(!m_hasVolunteer)
	{
		if(m_numOfVolunteersToDraw == 3)
			m_Texture->Blit(m_NPC.volunteers[2].XPos-XPos, m_NPC.volunteers[2].YPos-YPos, 0, 0, 32, 32, 0.5, 0.5, color);
		else if(m_numOfVolunteersToDraw > 3)
			m_Texture->Blit(m_NPC.volunteers[2].XPos-XPos, m_NPC.volunteers[2].YPos-YPos, 32, 0, 32, 32, 0.5, 0.5, color);
	}

	if(m_dialog)
		return Dialog(); // If return true then game over
	else if(m_off)
	{
		m_font->Print("太远了，听不清……", 0, 0);
	}
	
	return false;
}

int  cGameRole::GetSound()
{
	if(m_off)
	{
		m_off = false;
		return 0;
	}
	else if(!m_hasVolunteer)
	{
		return m_document->GetSelectSound();
	}

	return -1;
}

int cGameRole::GetDeviceChoice()
{
	return m_extralChoice;
}

BOOL cGameRole::CheckDialog()
{
	//Find the m_talker
	for(int i=0; i<m_numOfVolunteersToDraw; i++)
		if((abs(m_NPC.volunteers[i].XPos - toXPos) < 13)
			&& (m_NPC.volunteers[i].YPos- toYPos < 46) && (m_NPC.volunteers[i].YPos- toYPos > 0))
		{
			m_isMove = 0;
			if(abs(XPos+xPos-m_NPC.volunteers[i].XPos)>50 || abs(YPos+yPos-m_NPC.volunteers[i].YPos-23)>50)
			{
				m_off = true;
				return false;
			}
			else
			{
				m_talker = i;
				return true;
			}
		}

	return false;
}

BOOL cGameRole::Dialog()
{
	// Load the content of m_dialog and display it
    char address[50];
	static bool role;
	static int num = 0;

	if(num < 2)
	{
		if(m_hasVolunteer)
			role = false;
		else
			role = true;
	}

	if(!m_hasVolunteer)
	{
		if(m_talker<m_numOfVolunteersToDraw-1 && m_talker<4)
			sprintf(address, "..\\\\SeuVisitor\\\\Data\\\\script\\\\NPC_0%d.txt", 0);
		else
			sprintf(address, "..\\\\SeuVisitor\\\\Data\\\\script\\\\NPC_%d.txt", m_talker+20+m_dialogChoice*5);
	}
	else
		sprintf(address, "..\\\\SeuVisitor\\\\Data\\\\script\\\\NPC_%d.txt", m_talker+1);

	if(!role)
		m_tiles->Draw(9, m_PC, 50, GetSystemMetrics(SM_CYSCREEN)-220);
	else
		m_tiles->Draw(m_volunteer, m_NPC.volunteers[m_talker].Photo, 50, GetSystemMetrics(SM_CYSCREEN)-220);
 
	if(!(m_mouse->GetButtonState(MOUSE_LBUTTON) || m_keyBoard->GetKeyState(KEY_SPACE)))
		m_script.RenderDialog(false, address);
	else
	{
		num++;
		if(num != 1)
		{
			if(!m_hasVolunteer)
			{
				if(role)
					role = false;
				else
					role = true;
			}
			else 
				role = true;
		}

		Sleep(200);	
		if(!m_script.RenderDialog(true, address))
		{
			num = 0;
			role = false;
			m_dialog = false;
			if(!m_hasVolunteer)
			{
				if(m_dialogChoice<2  && m_talker==m_numOfVolunteersToDraw-1)
				{
					m_dialogChoice = 1;
					m_extralChoice = 2;
					m_time->StartCount();
					m_inGame = true;
				}
				else if(m_dialogChoice >= 2)
				{
					m_extralChoice = 0;
					m_dialogChoice = 0;
					m_numOfVolunteersToDraw++;
					m_inGame = false;

					if(m_talker>3)
					{
						return true;  // Game Over !
					}
				}
			}
		}
	}
	return false;
}

BOOL cGameRole::Render(D3DCOLOR color) //Set the order of rendering and render buildings, NPCs and PCs 
{
	m_building.GetBuildingInScreen();
	m_NPC.MarkWalkerPosition(XPos0, YPos0, xPos, yPos, &m_building);
	m_NPC.RenderWalker(0, XPos0, YPos0, color);

	if(m_building.CheckPCPosition(xPos, yPos, XPos0, YPos0))
	{
		m_building.Draw(XPos0, YPos0, color, true); // Render the building which should be drawn firstly
		RenderCharacter(color, true);//Render PCs
		m_building.Draw(XPos0, YPos0, color, false); // Render buildings and check whether the role is in the back of the building
		m_NPC.RenderVolunteer(XPos,YPos, xPos, yPos, color, m_numOfVolunteersToDraw);//Render the volunteer and get the X-cordinate of the volunteer		
	}
	else
	{
		m_building.Draw(XPos0, YPos0, color, false);
    	RenderCharacter(color, false);
	}
	
	if(m_displayBuildingInfo)
	{
		if(m_building.DisplayInformationOfBuilding())
			m_displayBuildingPic = true;
		else
			m_displayBuildingPic = false;
	}
	if(!m_displayBuildingPic)
		m_displayBuildingInfo = false;

	m_NPC.RefreshPosition();
	return true;
}

BOOL cGameRole::RenderCharacter(D3DCOLOR color, bool isBehind)
{
	if(m_NPC.CheckPCPosition(xPos, yPos, XPos, YPos, m_isMove, oxPos, oyPos)) //Check whether the PC is behind the volunteer	                                                 
	{
		m_NPC.RenderWalker(1, XPos, YPos, color);
		RenderPC(color);

		if(!isBehind)
			m_NPC.RenderVolunteer(XPos,YPos, xPos, yPos, color, m_numOfVolunteersToDraw);//Render the volunteer and get the X-cordinate of the volunteer
		
		m_NPC.RenderWalker(2, XPos, YPos, color);
	}
	else
	{
		if(!isBehind)
			m_NPC.RenderVolunteer(XPos, YPos, xPos, yPos, color, m_numOfVolunteersToDraw);//Render the volunteer

		m_NPC.RenderWalker(1, XPos, YPos, color);
		RenderPC(color);
		m_NPC.RenderWalker(2, XPos, YPos, color);
	}
	return true;
}

BOOL cGameRole::RenderPC(D3DCOLOR color)
{
	static int move = 0;
	static int i = 0;
	static int j = 0;

	//Render PCs
	if(m_isMove)
	{         
	 	CheckBound();

	 	m_tiles->Draw(m_PC+7,m_isMove*4-4+i, xPos - 32, yPos - 55, color);	
		
		if(j<3 && m_cursor==true)
		{
			m_tiles->Draw(6, j, toXPos-XPos-32, toYPos-YPos-32, color);
			j++;
			if(j == 3)
			{
				j=0;
				m_cursor = false;
			}
		}

		move = m_isMove*4-4;
		i++;
		if(i == 4)
			i = 0;
	}
	else 
       m_tiles->Draw(m_PC+7, move, xPos - 32, yPos - 55, color);

    return true;
}

BOOL cGameRole::RenderSmallMap()
{
	static int i0 = 0;
	int x = m_SWidth/2-256;
	int y = m_SHeight/2-153;
	m_tiles->Draw(10, 0, x, y);
	if(i0<3)
	{
		m_tiles->Draw(6, i0, x+(XPos+xPos)/9.8-32, y+(YPos+yPos)/9.8-32);
		i0++;
		if(i0 == 3)
			i0=0;
	}
	return true;
}

BOOL cGameRole::Free()
{
	if(m_time != NULL)
	{
		delete m_time;
		m_time = NULL;
	}
	m_building.Free();
	m_NPC.Free();
	m_script.Free();
	m_Width = m_Height = 0;
	m_SHeight = m_SWidth = 0;
	m_numOfVolunteersToDraw = 0;
	xPos = yPos = 0;
	XPos = YPos = 0;
	m_isInitial = false;
	m_isMove = 0;
	m_distance = 0;
	m_dialogChoice = 0;
	m_lock = false;
	m_tiles = NULL;
	m_map = NULL;
	m_smallGame = NULL;
	m_graphics = NULL;
	m_font = NULL;
	m_mouse = NULL;
	m_keyBoard = NULL;
	return true;
}

BOOL cGameRole::CheckBound()
{
	// Bounds check map coordinates
    if(XPos + oxPos < 0)
      XPos = 0;
    if(XPos + oxPos > 32*xTile - m_SWidth)
      XPos = (32*xTile - m_SWidth);
    if(YPos + oyPos < 0)
      YPos = 0;
    if(YPos + oyPos > 32*yTile - m_SHeight)
      YPos = (32*yTile - m_SHeight);
		
	if(xPos < 0)
		xPos = 0;
	if(xPos > m_SWidth - 40)
		xPos = m_SWidth - 40;
	if(yPos < 0)
		yPos = 0;
	if(yPos > m_SHeight - 4)
		yPos = m_SHeight - 4;

	if((xPos >= 0) && (xPos <= m_SWidth - 4) && (yPos >= 0) && (yPos <= m_SHeight - 4))
	{
		if((!(abs(xPos - m_SWidth / 2) < 4) || (XPos + oxPos >= 32*xTile - m_SWidth) || (XPos + oxPos <= 0)) \
			&& ((m_isMove == 1) || (m_isMove == 2) || (m_isMove == 8) || (m_isMove == 4) || (m_isMove == 6) || (m_isMove == 5)))
		{
				switch(m_isMove)
				{
				case 1: 
				case 2:
				case 8:
				case 4:
				case 6: 
				case 5: m_rollX = false; break;
				default: return false;
				}
		}
		if((!(abs(yPos - m_SHeight / 2) < 4) || (YPos + oyPos >= 32*yTile - m_SHeight) || (YPos + oyPos<= 0)) \
			&& ((m_isMove == 2) || (m_isMove == 4) || (m_isMove == 3) || (m_isMove == 6) || (m_isMove == 8) || (m_isMove == 7)))
		{
				switch(m_isMove)
				{
				case 2:
				case 4:
				case 3:
				case 6:
				case 8: 
				case 7: m_rollY = false; break;
				default: return false;
				}
		}
	}

	int x = XPos + xPos + oxPos - toXPos;
	int y = YPos + yPos + oyPos - toYPos;
	if(!CheckBarrier())//Find no barriers and keep moving
		m_lock = true;
	else                    //Find barriers 
		m_lock = false;

	if(x*x+y*y >= m_distance) //Whether get to the destination
	{
		oxPos = oyPos = 0;
		m_lock = false;
	}
	else
		m_distance = x*x + y*y;

    //Add increment to the screen position of role position
	if(m_rollX)
		XPos += oxPos;
	else
		xPos += oxPos;
	if(m_rollY)
		YPos += oyPos;
	else
		yPos += oyPos;

	return true;
}

BOOL cGameRole::CheckBarrier()
{
	int TemX, TemY, TemX0, TemY0, TemX1, TemY1, TemX2, TemY2;
	int  sign1 = -1;
	int  sign2 = -1;
	int x, y;

	// Set the checking position of PC
	switch(m_isMove2)
	{
	case 1:
			TemX  = TemX1 = xPos + oxPos + 11;
			TemY  = TemY1 = yPos + oyPos + 1;
			TemX0 = TemX2 = xPos + oxPos + 11;
			TemY0 = TemY2 = yPos + oyPos - 4;
			break;
	case 5:
			TemX  = TemX1 = xPos + oxPos - 10;
			TemY  = TemY1 = yPos + oyPos + 1;
			TemX0 = TemX2 = xPos + oxPos - 10;
			TemY0 = TemY2 = yPos + oyPos - 4;
			break;
	case 6:
			TemX  = TemX1 = xPos + oxPos + 13;
			TemY  = TemY1 = yPos + oyPos - 15;
			TemX0 = TemX2 = xPos + oxPos - 16;
			TemY0 = TemY2 = yPos + oyPos - 19;
			break;
	case 7:
			TemX  = TemX1 = xPos + oxPos - 9;
			TemY  = TemY1 = yPos + oyPos - 7;
			TemX0 = TemX2 = xPos + oxPos + 9;
			TemY0 = TemY2 = yPos + oyPos - 7;
			break;
	case 8:
			TemX  = TemX1 = xPos + oxPos - 12;
			TemY  = TemY1 = yPos + oyPos - 13;
			TemX0 = TemX2 = xPos + oxPos + 16;
			TemY0 = TemY2 = yPos + oyPos - 18;
			break;
	default: 
			TemX = TemX0 = TemX1 = TemX2 = xPos + oxPos;
			TemY = TemY0 = TemX1 = TemX2 = yPos + oyPos;
			break;
	}
	// Whether the PC is locate in  barriers
	if(::GetPixel(m_Hdc, TemX, TemY) == RGB(184, 184, 184) || ::GetPixel(m_Hdc, TemX0, TemY0) == RGB(184, 184, 184))
		return false;
	// Set the new destination of the PC if he or she comes arcoss the barrier
	else
	{
		if(oxPos >= 0)
			sign1 = 1;
		if(oyPos >= 0)
			sign2 = 1;
		for(int i=1; i<=6; i++)
		{
			TemX  = TemX1 - i*cos(m_angle)*sign1;
		    TemY  = TemY1 - i*sin(abs(m_angle))*sign2;
			TemX0 = TemX2 - i*cos(m_angle)*sign1;
		    TemY0 = TemY2 - i*sin(abs(m_angle))*sign2;
			
			x = (XPos+xPos+oxPos-i*cos(m_angle)*sign1)/32;
			y = (YPos+yPos+oyPos-i*sin(abs(m_angle))*sign2)/32;
			if(m_background[y][x] != 0)//Whether there is a road in the back of buildings
			{
				oxPos -= i*cos(m_angle)*sign1;
				oyPos -= i*sin(abs(m_angle))*sign2;
				
				return false;
			}
		    else if(::GetPixel(m_Hdc, TemX, TemY) == RGB(184, 184, 184) || ::GetPixel(m_Hdc, TemX0, TemY0) == RGB(184, 184, 184))
			{
				oxPos -= i*cos(m_angle)*sign1;
				oyPos -= i*sin(abs(m_angle))*sign2;
				
				return true;
			}	

			if(i == 6)
			{
				oxPos = oyPos = 0;
				return true;
			}
		}
	}
	return false;
}

BOOL cGameRole::SaveData(int fileNum)
{
	int duration = m_time->GetClock0();
	m_currentTime = m_time->GetCurrentTime();

	char address[50];
	FILE *writeData = NULL ;
	sprintf(address, "..\\SeuVisitor\\Data\\document\\%d.history", fileNum);

	if((writeData=fopen(address,"w"))!=NULL)
	{
		fprintf(writeData, "[ %s ]\n %d %d %d %d %d %d %d ", m_currentTime, duration, XPos, YPos, xPos, yPos, m_numOfVolunteersToDraw, m_dialogChoice);
		fclose(writeData);
	}
	
	return true;
}

BOOL cGameRole::LoadData(int fileNum)
{
	char address[50];
	FILE *readData = NULL ;
	int XPos0, YPos0, xPos0, yPos0;
	int currentTime0, duration0, numOfVolunteersToDraw0, dialogChoice0;

	sprintf(address, "..\\SeuVisitor\\Data\\document\\%d.history", fileNum);

	if((readData=fopen(address,"r")) != NULL)
	{
		fscanf(readData, "[ %s ]\n %d %d %d %d %d %d %d ", &currentTime0, &duration0,  &XPos0, &YPos0, &xPos0, &yPos0, &numOfVolunteersToDraw0, &dialogChoice0);
		fclose(readData);
		XPos = XPos0;
		YPos = YPos0;
		xPos = xPos0;
		yPos = yPos0;
		m_extralChoice = 0;
		m_numOfVolunteersToDraw = numOfVolunteersToDraw0;
		m_dialogChoice = dialogChoice0;
		m_time->SetClock0(duration0);
		m_isInitial = false;

	}
	
	Sleep(500);
	return true;
}

BOOL cGameRole::RecordGrade(bool isDisplay, int XPos, int YPos)
{
	FILE *readRecord;
	int duration[3] = {0};
	int i = 0;

	if((readRecord=fopen("..\\SeuVisitor\\Data\\document\\record.cg","r")) != NULL)
	{
		while(!feof(readRecord))
		{
			fscanf(readRecord, "[ %d ]\r", &duration[i]);
			i++;
		}

		fclose(readRecord);
	}

	int tmp = duration[0];
	for(i=1; i<3; i++)
	{
		if(duration[i] < tmp)
		{
			tmp = duration[i];
			duration[i] = duration[i-1];
			duration[i-1] = tmp;
		}
	}
	if(duration[1] > duration[2])
	{
		tmp = duration[1];
		duration[1] = duration[2];
		duration[2] = tmp;
	}

	if(!isDisplay)
	{
		FILE *writeRecord;
		writeRecord = fopen("..\\SeuVisitor\\Data\\document\\record.cg", "w");
		int tmp0 = m_time->GetClock0();
		int tmp1 = duration[2]; 

		if(duration[2] > tmp0)
			duration[2] = tmp0;
		
		for(i=0; i<3; i++)
		{
			if(duration[i] > 0)
				fprintf(writeRecord, "[ %d ]\r", duration[i]);
		}
		fclose(writeRecord);
	
		if(tmp1 > tmp0)	
			return true;
		else
			return false;
	}
	else
	{
		char time[10];
		int hour, minute, second;
		if(duration[0] != 0)
		{
			for(int i=0; i<3; i++)
			{
				if(duration[i] == 0)
					continue;

				hour = duration[i] / 3600;
				minute = (duration[i] % 3600) / 60;
				second = (duration[i] - hour*3600 - minute*60);
	
				sprintf(time, "%02d:%02d:%02d", hour, minute, second);
				m_font->Print(time, XPos+90, YPos+i*50+90);
			}
		}
		else
		{
			sprintf(time, "无记录");
			m_font->Print(time, XPos+90, YPos+130);
		}

		return true;
	}
		
	return true;
}