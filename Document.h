#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tile.h"

class cGameRole;

class cDocument
{
  private:
	char          m_dialog[1512]; // Record present dialog
	char          m_diaFrameBound[3][2]; // Width and height of the dialog frame
    int           m_offSet; // Last position of input file stream
	int           m_pointButton;
	int           m_selectButton;
	int			  m_subPointButton;
	int			  m_subSelectButton;
    int           xPos,yPos; // Position of dialog
	int           XPos,YPos; // Position of frame
	int           m_temX, m_temY; //Temp value of XPos and YPos
	int           m_sound;

	cTexture      m_Texture0;
	cInputDevice *m_Mouse;
	cFont        *m_Font;
	cTiles       *m_Tiles;
	cGameRole    *m_GameRole;

  public:
	cDocument(cGameRole *gameRole); //Constructor
	~cDocument(); //Destructor

    BOOL Create(cGraphics *Graphics, cInputDevice *Mouse, cFont *Font, cTiles *Tiles); // Create cScript class
	BOOL InitFrame(); // Load data of dialog frame
	void ResetValue();

	BOOL MenuOption(); // Option in menu
	BOOL RenderMenu();
	BOOL RenderSubMenu();
	BOOL RenderFrame(int frame, bool unLock, char *FileName=NULL);	// Display the dialog
	BOOL RenderChoice(int choice); // Display the choice for saveing or loading data

	BOOL Save(int num); // Save game
	BOOL Load(int num); // Load game
	int  GetTileNum(int frame, int l, int m); // Get the number of frame tile
	int  GetSelectSound(); // Get the sound of selecting
	BOOL Free(); // Free cScript class

};

class cMyTime
{
  private:
	clock_t start;
	clock_t finish;
	double duration;
	char clock[10];
    char time0[64];

  public:
	cMyTime();
	~cMyTime();

	void StartCount();
	void EndCount();
	int  GetClock0();
	void SetClock0(double duration0);
	char* GetClock();
	char* GetCurrentTime();
};

#endif


