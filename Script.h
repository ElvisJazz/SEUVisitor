#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include "Tile.h"

class cScript
{
  private:
	cInputDevice *m_Mouse;
	cTiles       *m_Tiles;
	cFont        *m_Font;
	char         m_dialog[512]; // Record present dialog
	char         m_diaFrame[6][32]; // Numbers of tiles the dialog frame will use
    int          m_offSet; // Last position of input file stream
    int          xPos,yPos; // Position of dialog
	int          XPos,YPos; // Position of dialog frame
  public:
	cScript(); //Constructor
	~cScript(); //Destructor

    BOOL Create(cInputDevice *Mouse, cFont *Font, cTiles *Tiles); // Create cScript class
	BOOL InitFrame(); // Load data of dialog frame
	BOOL RenderDialog(bool unLock, char *FileName);	// Display the dialog
	BOOL Free(); // Free cScript class


};
#endif