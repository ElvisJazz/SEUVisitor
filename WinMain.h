#ifndef _WINMAIN_H_
#define _WINMAIN_H_
#include <fstream>
#include <iostream>
#include "Tile.h"
#include "Map.h"
#include "GameRole.h"
#include "Script.h"

using namespace std;

class cApp : public cApplication
{
  private:

	// Graphics device and font
    cGraphics        m_Graphics;
	cFont            m_Font;
  
	// Input devices and cursor
	cInputDevice    m_KeyBoard, m_Mouse;
	cInput          m_Input;
	HCURSOR         m_MyCursor1, m_MyCursor2, m_MyOldCursor;

	//Sound device and channels
    cSound          m_Sound;
    cSoundChannel   m_SoundChannel;
    cMusicChannel   m_MusicChannel;
    cSoundData      m_SoundData;

    cTiles          m_Tiles;
	cMap            m_Map;
    cGameRole       *m_Role;
	cScript         m_Script;
	cTexture        m_texture;
	bool            m_wait;
	int             m_PC;
	int             m_StartTile;
	bool            m_IsSilent;
	
	int             m_MapData[94][156];

	static void LogFrame(void *Ptr, long Purpose);
    static void MenuFrame(void *Ptr, long Purpose);
    static void GameFrame(void *Ptr, long Purpose);
	static void AboutFrame(void *Ptr, long Purpose);

  public:
    cApp(); // Constructor
    BOOL Init(); //Initial cApp class
    BOOL Shutdown(); //Shutdown cApp class
    BOOL Frame();
	//static BOOL LoadMapData(char *Filename, int Array[][156]); //Load data of main map
	D3DCOLOR SetColor(DWORD time, float rate); //Set the transparent level

	// Play a sound or music
    BOOL PlaySound(long Num);
    BOOL PlayMusic(long Num);
    BOOL StopMusic();

	// Get cursor
	BOOL GetOldCursor();
	// Set cursor
	BOOL SetNewCursor(int i);

	static BOOL LoadMapData(char *Filename, int Array[][156])
	{
		long x;
		ifstream readMap;

		readMap.open(Filename, ios::in);
		if(!readMap)
		{
			cerr<<"Wrong!"<<endl;
			exit(1);
		}

		for(int i = 0; i < 94; i++)
			for(int j = 0; j < 156; j++)
			{
				readMap>>x;
				if(x!=0)
					x--;
				Array[i][j] = x ;
			}	

		readMap.close();

		return TRUE;
	}	

	static DWORD WINAPI cApp::Wait(void* app0)
	{
		cApp* app = (cApp*)app0;
		app->m_texture.Load(&app->m_Graphics,"..\\SeuVisitor\\Data\\pic\\others\\wait.bmp");
		int num = 0;
		int tmpY = (GetSystemMetrics(SM_CYSCREEN) - 193) / 2;
		int tmpX = (GetSystemMetrics(SM_CXSCREEN) - 384) / 2;

		while(app->m_wait)
		{
			 app->m_Graphics.Clear();
			 app->m_Graphics.BeginScene();
			 switch(num)
			{
			case 0: app->m_texture.Blit(tmpX, tmpY, 0, 0, 256, 129, 1.5, 1.5); break;
			case 1: app->m_texture.Blit(tmpX, tmpY, 256, 0, 256, 129, 1.5, 1.5); break;
			case 2: app->m_texture.Blit(tmpX, tmpY, 0, 129, 256, 129, 1.5, 1.5); break;
			case 3: app->m_texture.Blit(tmpX, tmpY, 256, 129, 256, 129, 1.5, 1.5); break;
			case 4: app->m_texture.Blit(tmpX, tmpY, 0, 258, 256, 129, 1.5, 1.5); break;
			case 5: app->m_texture.Blit(tmpX, tmpY, 256, 258, 256, 129, 1.5, 1.5); break;
			default: break;
			}

			num = (num + 1) % 6;
			Sleep(100);
			app->m_Graphics.EndScene();
			app->m_Graphics.Display();
		}

		app->m_Graphics.Clear();
		app->m_texture.Free();
		return NULL;
	}

};

#endif
