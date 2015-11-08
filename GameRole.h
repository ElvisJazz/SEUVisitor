#ifndef _GAMEROLE_H_
#define _GAMEROLE_H_

#include "Tile.h"
#include "Map.h"
#include "Script.h"
#include "Document.h"
#include "Building.h"
#include "NPC.h"
#include "SmallGame.h"

class cGameRole
{
  private:
    int    m_Width, m_Height;  // Width and height of role
	int    m_SWidth, m_SHeight; //  Width and height of screen

	int    XPos, YPos;  // Position of map
	int    XPos0, YPos0; //Copy of position of map
	int    xPos, yPos; // Position of role
	int    oxPos, oyPos; // Increment of position of role
	int    toXPos, toYPos;// Destination of role 
	int     xTile, yTile;// Numbers of tile

	int     m_PC; // THe player character
	int     m_isMove;  // Direction of moving
	int     m_isMove2; // (Copy)Direction of moving
	double  m_angle;  // Angle of moving
	bool    m_lock; // Whether get to the destination
	bool    m_isInitial; //Whether it has been inited
	bool    m_rollX, m_rollY; // Whether screen rolls
	bool    m_cursor; // Whether click the cursor
	bool    m_dialog; // Whether have a dialog
	bool    m_off; // Delay time of displaying the message
	bool    m_hasVolunteer; // Whether there are volunteer or not
	bool    m_finishGame; // Whether finish the small game
	bool    m_inGame; // Whether be in game
	bool    m_displayBuildingInfo; // Whether display information of building
	bool    m_displayBuildingPic; // Whether display picture of building
	int     m_talker; // The current talker(NPC)
	double  m_distance; // Distance between present position and destintion
	int     m_extralChoice; // Choose to display extral service
	int     m_volunteer;
	int     m_dialogChoice; // Choose different dialog
	int     m_numOfVolunteersToDraw;
	int     m_background[94][156];
	char   *m_currentTime;


	cSmallGame   *m_smallGame;
	cTexture     *m_Texture;
	cGraphics    *m_graphics;
	cMap         *m_map;
	cTiles       *m_tiles;
	cFont        *m_font;
	cInputDevice *m_mouse;
	cInputDevice *m_keyBoard;
	cDocument    *m_document;
	cMyTime      *m_time;
    cScript       m_script;
	cBuilding     m_building;
	cNPC          m_NPC;
	HDC           m_Hdc;

  public:
	cGameRole(); // Constructor
	~cGameRole(); // Destructor

	BOOL Create(cMap *Map, cGraphics *Graphics, cTiles *Tiles, HWND hwnd, cFont *Font, \
		        cInputDevice *KeyBoard, cInputDevice *Mouse, int PC, int startTile, \
				int TileX, int TileY, bool hasVolunteer);//Create a GameRole class
	BOOL Free(); // Free a GameRole class

	BOOL GetPos(char Choice); // Get the initial position of the role
	int  GetSound();
	int  GetDeviceChoice();

	BOOL Run(); // Function for game's running

	BOOL CheckDialog(); // Check whether have a dialog
	BOOL Dialog(); // Render dialogue between role and NPC

	BOOL CheckBound(); // Check whether area the role can go around
	BOOL CheckBarrier(); // Check whether the role come across barriers

	BOOL Render(D3DCOLOR color); // Render the role
	BOOL RenderCharacter(D3DCOLOR color, bool isBehind); //Render the character including PCs and NPCs

	BOOL RenderPC(D3DCOLOR color); // Render the PC
	BOOL RenderSmallMap(); // Render small map

	BOOL SaveData(int fileNum); // Save current data
	BOOL LoadData(int fileNum); // Load game record
	BOOL RecordGrade(bool isDisplay, int XPos=0, int YPos=0); // Record and display good grades
	
};

#endif