#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "Tile.h"

#define MAX_BUILDINGS  36
typedef struct 
{
  int XPos1, YPos1;
  int XPos2, YPos2;
  int Width, Height;
  int  *map;
} sBuilding;

class cBuilding
{
  private:
	int       m_SWidth, m_SHeight; //Width and height of screen
	int       m_XPos, m_YPos; // Position of big map
	cTiles   *m_tiles;
	cTexture *m_texture[4];
	cInputDevice *m_mouse;
	cFont     m_font;
	char      m_introduction[100]; // Record present introduction
	char      m_name[16]; // Name of the building
	int       m_buildingPic[31];
	int       m_numToDraw;
	int       m_transparentBuilding;
	int       m_extralBuilding;
	int       m_selectedBuilding;

	int       m_xCoordinate;
	int       m_yCoordinate;
	int       m_selectedOption;
	int       m_copyOfSelectedBuilding;
	bool      m_findTexture;
	
  public:
	 sBuilding  m_buildings[MAX_BUILDINGS];
	 int        m_SBuilding[10];

	cBuilding(); // Constructor
	~cBuilding(); // Destructor

	BOOL Create(cTiles *Tiles); // Create cBuilding class
	BOOL Free(); // Free cBuilding class

	BOOL Load(char *FileName); // Load data of each building
	BOOL AddBuilding(int x1, int y1, int x2, int y2, int width, int height); // Add data to sBuilding struct

	BOOL GetBuildingInScreen(); // Get builidngs in screen
	BOOL GetRec(int i, int &X1, int &X2, int &Y1, int &Y2); // Get the position of building that will be displayed
	BOOL Draw(int X, int Y, D3DCOLOR Color, bool extral); // Draw buildings
	BOOL CheckPCPosition(int xPos, int yPos, int XPos, int YPos); // Check whether the PC is behind the building

	BOOL InitInformation(cGraphics *graphics, cInputDevice *mouse); // Init informationn for selected building
	BOOL DisplayInformationOfBuilding(); // Display brief information of the selected building
	BOOL CheckSelectedBuilding(int worldX, int worldY); // Check whether the building is selected
	int  Option(); // See pictures or exit
	void ResetSelectedBuilding(); // Reset value
	void DrawSelectedFrame(); // Display the frame of selected option
	BOOL GetInformation(); // Get name and introduction from outer file
};
#endif
