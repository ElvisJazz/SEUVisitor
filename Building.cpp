#include "Core_Global.h"
#include "Building.h"
#include <iostream>
#include <fstream>

using namespace std;

cBuilding::cBuilding()
{
	m_SWidth = m_SHeight = 0;
	m_XPos = m_YPos = 0;
	m_numToDraw = 0;
	m_tiles = NULL;
	m_transparentBuilding = -1;
	m_extralBuilding = -1;
	m_selectedBuilding = -1;
	m_selectedOption = -1;
	for(int i=0; i<10; i++)
		m_SBuilding[i] = -1;
}

cBuilding::~cBuilding()
{
	Free();
}

BOOL cBuilding::Create(cTiles *Tiles)
{
	m_SWidth = GetSystemMetrics(SM_CXSCREEN);
	m_SHeight = GetSystemMetrics(SM_CYSCREEN);
	m_tiles = Tiles;
	return true;
}

BOOL cBuilding::Free()
{
	m_numToDraw = 0;
	m_tiles = NULL;
	for(int i=0; i<m_numToDraw; i++)
	{
		if(m_buildings[i].map != NULL)
		{
			delete m_buildings[i].map;
			m_buildings[i].map = NULL;
		}
	}
	return true;
}

BOOL cBuilding::Load(char *FileName)
{
	FILE *readHouse;
	// Load data of each building
	if(!(readHouse = fopen(FileName, "r")))
	{
		cerr<<"Data Wrong!"<<endl;
		exit(1);
	}

    while(!feof(readHouse))
	{
		int x1, y1, x2, y2, width, height;
		fscanf(readHouse, "(%d,%d,%d,%d,%d,%d)\r", &x1, &y1, &x2, &y2, &width, &height);
		if(!AddBuilding(x1, y1, x2, y2, width, height))
			return false;
	}

	return true;
}

BOOL cBuilding::AddBuilding(int x1, int y1, int x2, int y2, int width, int height)
{
	char address[60];
	int x;
	int map = width * height;
	ifstream readMap;
    // Add the data of each building to certain struct
	if(m_numToDraw < MAX_BUILDINGS) 
	{
		m_buildings[m_numToDraw].XPos1 = x1;
		m_buildings[m_numToDraw].YPos1 = y1;
		m_buildings[m_numToDraw].XPos2 = x2;
		m_buildings[m_numToDraw].YPos2 = y2;
		m_buildings[m_numToDraw].Width  = width;
		m_buildings[m_numToDraw].Height = height;
		m_buildings[m_numToDraw].map   = new int[map];
		
		if(m_numToDraw >= 28)
			sprintf(address, "..\\\\SeuVisitor\\\\Data\\\\map\\\\BuildingMap\\\\building-%d.map", 29);
		else
			sprintf(address, "..\\\\SeuVisitor\\\\Data\\\\map\\\\BuildingMap\\\\building-%d.map", m_numToDraw+1);

		readMap.open(address, ios::in);
		if(!readMap)
		{
			cerr<<"Data Wrong!"<<endl;
			exit(1);
		}

		for(int i = 0; i < map; i++)
		{
			readMap>>x;
			if(x!=0)
				x--;
			m_buildings[m_numToDraw].map[i] = x ;
		}

		readMap.close();
		m_numToDraw++; 
		return true;
	}
	return false;
}

BOOL cBuilding::Draw(int X, int Y, D3DCOLOR Color, bool extral)
{
	int MapX, MapY; 
    int XOff, YOff;
	int Rows, Columns;
	int TileNum;
	int *MapPtr;
	int TileX, TileY;
	int X1, X2, Y1, Y2;
	int i = 0;
	D3DCOLOR tmpColor = Color;
	 
	m_XPos = X;
	m_YPos = Y;

	// Error checking
	if(m_tiles == NULL)
		return false;

	for(int j=0; j<10; j++)
	{
		if(m_SBuilding[j]!=-1)
		{
			if(GetRec(m_SBuilding[j], X1, X2, Y1, Y2))
			{
				if(!extral)
				{
					if(m_extralBuilding != m_SBuilding[j])
						i = m_SBuilding[j];
					else
						continue;
				}
				else if(m_extralBuilding != -1)
				{
					i = m_extralBuilding;
					GetRec(i, X1, X2, Y1, Y2);
				}
				else
					return false;

				// Calculate row and column of building's display area
				Rows = (Y2-Y1)/m_tiles->GetHeight(0);
				Columns = (X2-X1)/m_tiles->GetWidth(0);
				if((Y2-Y1)%m_tiles->GetHeight(0)>0)
					Rows++;
				if((X2-X1)%m_tiles->GetWidth(0)>0)
					Columns++;

				// Calculate smooth scrolling variables
				MapX = (X1-m_buildings[i].XPos1) / m_tiles->GetWidth(0);
				MapY = (Y1-m_buildings[i].YPos1) / m_tiles->GetHeight(0);
				XOff = (X1-m_buildings[i].XPos1) % m_tiles->GetWidth(0);
				YOff = (Y1-m_buildings[i].YPos1) % m_tiles->GetHeight(0);		

				// Get a pointer to the map data
				MapPtr = m_buildings[i].map;

				// Transparent checking
				if(m_selectedBuilding == i)
					Color = 0xffff0000;
				else if(m_transparentBuilding == i)
					Color = 0x88ffffff;
				else
					Color = tmpColor;

				// Loop for each row and column
				for(int row=0; row<Rows; row++)
					 for(int column=0; column<Columns; column++) 
					 {
						// Get the tile number to draw (and draw it)
					 TileNum = MapPtr[(row + MapY)*m_buildings[i].Width+column+MapX];
						//Num = TileNum / 64;
						TileX = X1 - m_XPos + column * m_tiles->GetWidth(0) - XOff;
						TileY = Y1 - m_YPos + row * m_tiles->GetHeight(0) - YOff;
						
						if(i >= 28)
							i = 28;
						m_tiles->Draw(i+11, TileNum, TileX, TileY, Color);
					 }

				if(extral)
					return true;
			}
		}
		else	
			return false;
	}

	return false;
}

BOOL cBuilding::GetBuildingInScreen()
{
	int j = 0;
	int tileWidth = m_tiles->GetWidth(0);
	int dX, dY;

	for(int i=0; i<10; i++)
		m_SBuilding[i] = -1;
	m_transparentBuilding = -1;
	m_extralBuilding = -1;

	for(i=0; i<m_numToDraw; i++)
	{
		dX = m_XPos-m_buildings[i].XPos1;
		dY = m_YPos-m_buildings[i].YPos1;
		
		//Whether the building is in the area of screen
		if(dX<m_buildings[i].Width*tileWidth && dX>0-m_SWidth &&
			dY<m_buildings[i].Height*tileWidth && dY>0-m_SHeight)
		{
			m_SBuilding[j] = i;
			j++;
		}
	}
	
	return true;
}

BOOL cBuilding::GetRec(int i, int &X1, int &X2, int &Y1, int &Y2) //Get the displaying area of buildings
{
	int dX = m_XPos-m_buildings[i].XPos1;
	int dY = m_YPos-m_buildings[i].YPos1;
	int tileWidth = m_tiles->GetWidth(0);
	//Whether the building is in the area of screen
	if(dX<m_buildings[i].Width*tileWidth && dX>0-m_SWidth &&
	   dY<m_buildings[i].Height*tileWidth && dY>0-m_SHeight)
	{
		if(dX >= 0)
		{
			X1 = m_XPos;
			X2 = m_buildings[i].XPos1 + m_buildings[i].Width*tileWidth;
		}
		else
		{
			X1 = m_buildings[i].XPos1;
			if(m_buildings[i].XPos1+m_buildings[i].Width*tileWidth < m_XPos+m_SWidth)
				X2 = m_buildings[i].XPos1 + m_buildings[i].Width*tileWidth;
			else
				X2 = m_XPos + m_SWidth;
		}

		if(dY >= 0)
		{
			Y1 = m_YPos;
			Y2 = m_buildings[i].YPos1 + m_buildings[i].Height*tileWidth;
		}
		else
		{
			Y1 = m_buildings[i].YPos1;
			if(m_buildings[i].YPos1+m_buildings[i].Height*tileWidth < m_YPos+m_SHeight)
				Y2 = m_buildings[i].YPos1 + m_buildings[i].Height*tileWidth;
			else
				Y2 = m_YPos + m_SHeight;
		}

		return true;
	}

	return false;
}

BOOL cBuilding::CheckPCPosition(int xPos, int yPos, int XPos, int YPos)
{
	//Whether the PC is in the back of buildings
	for(int i=0; i<m_numToDraw; i++)
	{
		if(xPos+XPos>m_buildings[i].XPos1-4 && xPos+XPos<m_buildings[i].XPos2+4)
		{
			if(yPos+YPos>m_buildings[i].YPos1-10 && yPos+YPos<m_buildings[i].YPos2-10)
			{
				m_transparentBuilding = i;
			}
			else if(yPos+YPos>m_buildings[i].YPos2 && yPos+YPos<m_buildings[i].YPos2+64)
			{
				m_extralBuilding = i;
			}
		}
	}

	if(m_transparentBuilding != -1)
		return true;

	m_extralBuilding = -1;
	m_transparentBuilding = -1;
	return false;
}

BOOL cBuilding::DisplayInformationOfBuilding()
{
	m_xCoordinate = (m_SWidth - 384) / 2;
	m_yCoordinate = (m_SHeight - 256) / 2;
	int selectedBuilding = m_buildingPic[m_copyOfSelectedBuilding];

	m_texture[1]->Blit(m_xCoordinate-200, m_yCoordinate-100, 128, 256, 384, 256);

	if(selectedBuilding != -1)
	{
		if(selectedBuilding < 16)
			m_texture[0]->Blit(m_xCoordinate-170, m_yCoordinate-50, selectedBuilding%4*128, selectedBuilding/4*128, 128, 128);
		else if(selectedBuilding < 27)
		{
			selectedBuilding -= 16;
			if(selectedBuilding > 8)
				selectedBuilding = 8;

			m_texture[1]->Blit(m_xCoordinate-170, m_yCoordinate-50, selectedBuilding%4*128, selectedBuilding/4*128, 128, 128);
		}
		else
		{
			selectedBuilding -= 27;
			m_texture[3]->Blit(m_xCoordinate-170, m_yCoordinate-50, selectedBuilding%2*128, selectedBuilding/2*128, 128, 128);
		}

		for(int i=0; i<4; i++)
			m_texture[2]->Blit(m_xCoordinate-15+i*35, m_yCoordinate+75, i%2*256, i/2*256, 256, 256, 0.125, 0.125);
	}

	m_font.Print(m_introduction, m_xCoordinate-33, m_yCoordinate-30);
	DrawSelectedFrame();

	int tmp = Option();
	if(tmp == -1)
		return false;
	else if(tmp>0)
		m_texture[2]->Blit(m_xCoordinate+200, m_yCoordinate, (m_selectedOption-1)%2*256, (m_selectedOption-1)/2*256, 256, 256, 1.5, 1.0);

	return true;
}

void cBuilding::DrawSelectedFrame()
{
	if(m_selectedOption == 0)
		m_texture[1]->Blit(m_xCoordinate+142, m_yCoordinate-75, 0, 480, 32, 32);
	else if(m_findTexture && m_selectedOption>0)
		m_texture[1]->Blit(m_xCoordinate-15+(m_selectedOption-1)*35, m_yCoordinate+75, 32, 480, 32, 32);
}

int cBuilding::Option()
{
	int x = m_mouse->GetXPos();
	int y = m_mouse->GetYPos();
	m_selectedOption = -1;

	if(x>=m_xCoordinate+144 && x<=m_xCoordinate+167 && y>=m_yCoordinate-70 && y<=m_yCoordinate-46)
		m_selectedOption = 0;
	else
	{
		x -= (m_xCoordinate-15);
		y -= (m_yCoordinate+75);

		if(y>=0 && y<=32 && x>=0 && x<=137)
			m_selectedOption = x / 35 + 1;
	}

	if(m_mouse->GetButtonState(MOUSE_LBUTTON))
	{
		if(m_selectedOption == 0)
		{
			Sleep(200);
			m_selectedBuilding = -1;
			ResetSelectedBuilding();
			return -1;
		}
	}
	
	if(m_selectedOption > 0)
			return m_selectedOption;

	return 0;
		
}
BOOL cBuilding::CheckSelectedBuilding(int worldX, int worldY)
{
	for(int i=0; i<10; i++)
	{
		if(m_SBuilding[i] != -1)
		{
			int j = m_SBuilding[i];
			if(worldX>m_buildings[j].XPos1 && worldX<m_buildings[j].XPos2 
				&& worldY>m_buildings[j].YPos1 && worldY<m_buildings[j].YPos2)
			{
				m_selectedBuilding = j;
				return true;
			}
		}
		else
		{
			m_selectedBuilding = -1;
			return false;
		}
	}

	return false;
}

void cBuilding::ResetSelectedBuilding()
{
	m_selectedBuilding = -1;
	for(int i=0; i<3; i++)
	{
		m_texture[i]->Free();
		delete m_texture[i];
	}
}

BOOL cBuilding::InitInformation(cGraphics *graphics, cInputDevice *mouse)
{
	m_mouse = mouse;
	m_findTexture = false;
	m_font.Create(graphics, "Arial", 14, TRUE);

	ifstream read;
	read.open("..\\SeuVisitor\\Data\\map\\BuildingMap\\SBuilding.map", ios::in);
	for(int i=0; i<31; i++)
		read>>m_buildingPic[i];
	read.close();

	switch(m_selectedBuilding)
	{
	case 29: m_copyOfSelectedBuilding = 28; break;
	case 30:
	case 31: m_copyOfSelectedBuilding = 29; break;
	case 32:
	case 33:
	case 34: m_copyOfSelectedBuilding = 30; break;
	default: m_copyOfSelectedBuilding = m_selectedBuilding; break;
	}


	for(i=0; i<4; i++)
		m_texture[i] = new cTexture();

	m_texture[0]->Load(graphics, "..\\SeuVisitor\\Data\\pic\\buildingPics\\SBuilding1.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
	m_texture[1]->Load(graphics, "..\\SeuVisitor\\Data\\pic\\buildingPics\\SBuilding2.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
	m_texture[3]->Load(graphics, "..\\SeuVisitor\\Data\\pic\\buildingPics\\SBuilding3.bmp", D3DCOLOR_ARGB(255, 255, 0, 255));
	if(m_buildingPic[m_copyOfSelectedBuilding] != -1)
	{
		char address[50];
		sprintf(address, "..\\SeuVisitor\\Data\\pic\\buildingPics\\%d.bmp", m_buildingPic[m_copyOfSelectedBuilding]);
		if(m_texture[2]->Load(graphics, address, D3DCOLOR_ARGB(255, 255, 0, 255)))
			m_findTexture = true;
	}

	GetInformation(); // Get name and introduction from outer file

	return true;
}

BOOL cBuilding::GetInformation()
{
	char address[50];
	char x;
	int i = 0;

	for(int j=0; j<16; j++)
		m_name[j] = '\0';
	for(j=0; j<100; j++)
		m_introduction[j] = '\0';

	sprintf(address, "..\\SeuVisitor\\Data\\buildingIntroduction\\%d.txt", m_copyOfSelectedBuilding);

	ifstream read;
	read.open(address, ios::in);
	while(true)
	{
		x = read.get();
		if(x == '#')
			break;

		m_introduction[i] = x;

		i++;
	}
	read.close();

	return true;
}