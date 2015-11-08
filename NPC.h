#ifndef _NPC_H_
#define _NPC_H_

#include "Tile.h"
#include "Building.h"

#define MAX_VOLUNTEERS  30
#define MAX_WALKERS 10
typedef struct 
{
  int XPos, YPos;
  int Tile, Photo;
  int  Direc;
} sVolunteer;

typedef struct
{
  int Pos[4][2]; //Position of walkers
  int Line; //Line of walkers
  int Tile;
  int Order;
  int Direc; //Direction
  int Action;//Action
  int Mark;  //Mark the position of the walker such as behind building, before PC, behind PC
  int CurrentX, CurrentY;
} sWalker;

class cNPC
{
  private:
	int     m_SWidth, m_SHeight; //Width and height of screen
	int     m_NumOfVolunteersToDraw; // Number of volunteers to draw
	int     m_NumOfWalkersToDraw; // Number of walkers to draw
	int     m_StartTile; //Start tile of volunteer
	int     m_TileOfWalker; //Start tile of walkers
	int     m_StopWalker[2]; //The walker who should stop moving
    cTiles *m_Tiles;
	sWalker walkers[MAX_WALKERS];

  public:
	sVolunteer    volunteers[MAX_VOLUNTEERS];

	cNPC();
	~cNPC();

	BOOL Create(cTiles *Tiles);
	BOOL Free();

	BOOL LoadVolunteer(char *FileName, int startTile, int pic); //Load the volunteer
    BOOL AddVolunteer(int XPos, int YPos, int Photo, int num); //Add the Volunteer to the game
	BOOL RenderVolunteer(int XPos, int YPos, int xPos, int yPos, D3DCOLOR color, int currentNumOfTalkers=-1); // Render the volunteer
	int  GetNumberOfVolunteers();

	BOOL LoadWalker(char *FileName, int startTile); //Load the walker
	BOOL AddWalker(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3); // Add the walker to the game
	BOOL RenderWalker(int mark, int XPos, int YPos, D3DCOLOR color); // Render the walker

	BOOL RefreshPosition(); //Refresh currentX and currentY of walkers
	BOOL MarkWalkerPosition(int XPos, int YPos, int xPos, int yPos, cBuilding *building); //Mark the position of walker in screen
	BOOL CheckPCPosition(int xPos, int yPos, int XPos, int YPos, int &isMove,  int oxPos, int oyPos); //Check whether the PC is behind the volunteer and on the way of walkers
	BOOL SetDirection(int num); //Set new direction for NPC
};
#endif