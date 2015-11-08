/**************************************************
WinMain.cpp
SeuVisitor
Programming Role-Playing Games with DirectX, by Jazz from SEU

Required libraries(DX9b):
  strmiids.lib d3d9.lib d3dx9.lib d3dxof.lib dxguid.lib dinput8.lib dsound.lib 
  winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib
  shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 
**************************************************/
#include "Core_Global.h"
#include "WinMain.h"
#include "resource.h"

static       cStateManager g_StateManager;
long         g_MenuOptions = -1;
static long  g_TextureX = (GetSystemMetrics(SM_CXSCREEN)-GetSystemMetrics(SM_CYSCREEN)) / 2;
static       cTexture g_MenuSelect;
static cFont g_SetFont;
static int   g_DeviceChoice; // Choose map device by pressing 'M' and choose load system by pressing 'L'
static bool  g_HasVolunteer; // Whether there are volunteers or not

#define NUM_SOUNDS 2
char *g_SoundFilenames[NUM_SOUNDS] = {
    { "..\\SeuVisitor\\Data\\sound&music\\error.wav" },
    { "..\\SeuVisitor\\Data\\sound&music\\click.wav" },
  };

// Global music filenames
long g_CurrentMusic = -1;
char *g_MusicFilenames[] = {
    { "..\\SeuVisitor\\Data\\sound&music\\title.mid" },
    { "..\\SeuVisitor\\Data\\sound&music\\menu.mid"  },
    { "..\\SeuVisitor\\Data\\sound&music\\game1.mid" },
    { "..\\SeuVisitor\\Data\\sound&music\\game2.mid" },
  };

cApp::cApp()
{ 
  if(MessageBox(NULL,"是否启用全屏？", "SEUVisitor", MB_YESNO)== IDYES)
	  m_Style = WS_POPUP;
  else
	  m_Style = WS_SYSMENU|WS_MINIMIZEBOX;

  m_Width  = GetSystemMetrics(SM_CXSCREEN);
  m_Height = GetSystemMetrics(SM_CYSCREEN);
  strcpy(m_Class, "俯瞰风景工作室");
  strcpy(m_Caption, "东大游侠");

 
}

BOOL cApp::Init()
{ 
  // Initialize the graphics device and set display mode
  SendMessage(GethWnd(), WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), (LPCTSTR)IDI_ICON1));
  m_Graphics.Init();
  m_Graphics.SetMode(GethWnd(), TRUE, FALSE);
  
 
  ShowCursor(false);
 
  m_wait = true;
  HANDLE hThread=CreateThread(NULL,0,cApp::Wait,this,0,NULL);

  m_Font.Create(&m_Graphics, "Arial", 16, TRUE);
  m_MyCursor1 = LoadCursorFromFile("..\\SeuVisitor\\Data\\pic\\others\\cursor.ani");
  // Initialize input and onput devices
  m_Input.Init(GethWnd(), GethInst());
  m_KeyBoard.Create(&m_Input, KEYBOARD);
  m_Mouse.Create(&m_Input, MOUSE, TRUE);

  //Wait(1);
  // Initialize the sound system and channels
  m_Sound.Init(GethWnd(), 22050, 1, 16);
  m_SoundChannel.Create(&m_Sound, 22050, 1, 16);
  m_MusicChannel.Create(&m_Sound);
  m_IsSilent = false;

  // Create and load the tile set
  m_Tiles.Create(&m_Graphics, 52);
  char address[60];
  for(int i=0; i<5; i++)//load textures of the ground
  {
	  sprintf(address, "..\\SeuVisitor\\Data\\pic\\tiles\\tile%d.bmp", i+1);
	  m_Tiles.Load(i, address, 32, 32);
  }

  m_Tiles.Load(5, "..\\SeuVisitor\\Data\\pic\\others\\dialog.bmp", 32, 32, D3DCOLOR_ARGB(255, 255, 0, 255));
  m_Tiles.Load(6, "..\\SeuVisitor\\Data\\pic\\others\\cursor.bmp", 64, 64, D3DCOLOR_ARGB(255, 255, 0, 255));
  m_Tiles.Load(7, "..\\SeuVisitor\\Data\\pic\\(N)PC\\PC-1.bmp", 64, 64, D3DCOLOR_ARGB(255, 255, 0, 255));
  m_Tiles.Load(8, "..\\SeuVisitor\\Data\\pic\\(N)PC\\PC-2.bmp", 64, 64, D3DCOLOR_ARGB(255, 255, 0, 255));
  m_Tiles.Load(9, "..\\SeuVisitor\\Data\\pic\\photos\\0.bmp", 170, 240, D3DCOLOR_ARGB(255, 255, 0, 255));//Photos of PC and volunteers
  m_Tiles.Load(10, "..\\SeuVisitor\\Data\\pic\\others\\map.bmp", 512, 512, D3DCOLOR_ARGB(255, 255, 0, 255));


  for(i=1; i<=29; i++)//load textures of buildings
  {
	  sprintf(address, "..\\SeuVisitor\\Data\\pic\\buildings\\building-%d.bmp", i);
      m_Tiles.Load(i+10, address, 32, 32, D3DCOLOR_ARGB(255, 255, 0, 255));
  }
  m_StartTile = i+10;
  // Create and set the map
  m_Map.Create(5, 156, 94);
  m_Map.UseTiles(&m_Tiles);

  LoadMapData("..\\SeuVisitor\\Data\\map\\MainMap\\frontground.map", m_MapData);
  m_Map.SetMapData(0, (int*)&m_MapData);

  m_wait = false;
  g_StateManager.Push(LogFrame, this);

  return TRUE;
}


	
BOOL cApp::Shutdown()
{
  // Pop all states
  g_StateManager.PopAll(this);
  // Free map object
  m_Map.Free();
  // Shutdown tiles object
  m_Tiles.Free();
  // Shutdown sound
  m_MusicChannel.Free();
  m_SoundChannel.Free();
  m_Sound.Shutdown();
  // Shutdown input
  m_KeyBoard.Free();
  m_Mouse.Free();
  m_Input.Shutdown();
  m_Font.Free();
  // Shutdown graphics
  m_Graphics.Shutdown();
  // Relieve cursor
  DestroyCursor(m_MyCursor1);
  SetNewCursor(0);
  ShowCursor(true);
  return TRUE;
}

BOOL cApp::Frame()
{
  static DWORD LastTime = timeGetTime();
  DWORD ThisTime = timeGetTime();
  DWORD Elapsed = ThisTime - LastTime;

  // Frame lock to 50ms per frame
  if(Elapsed < 50)
    return TRUE;
  LastTime = ThisTime;  
 
  	
  m_KeyBoard.Acquire(TRUE);
  m_KeyBoard.Read();
  if(g_DeviceChoice == 1)
	 m_Mouse.Acquire(FALSE);
  else
  {
	 m_Mouse.Acquire(TRUE);
	 m_Mouse.Read();
  }

  if(m_KeyBoard.GetKeyState(KEY_V))
  {
	  Sleep(300);
	  if(!m_IsSilent)
	  {
		 StopMusic();
		 m_IsSilent = true;
	  }
	  else
	  {
		 m_IsSilent = false;
	  }
  }
  // Process state, returning result
  return g_StateManager.Process(this);
}



int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
{
  cApp App;
  return App.Run();
}

D3DCOLOR cApp::SetColor(DWORD time, float rate)
{
	time /= (500*rate);
	switch(time)
	{
	case 0:return 0x00ffffff;
	case 1:return 0x10ffffff;
	case 2:return 0x50ffffff;
	case 3:return 0x90ffffff;
	case 4:return 0xd0ffffff;
	default: return 0xffffffff;
	}
}

BOOL cApp::PlaySound(long Num)
{
  if(!m_IsSilent && Num >= 0 && Num < NUM_SOUNDS) 
  {
    m_SoundData.Free();

    if(m_SoundData.LoadWAV(g_SoundFilenames[Num]) )
      m_SoundChannel.Play(&m_SoundData);
	
    return TRUE;
  }

  return FALSE;
}

BOOL cApp::PlayMusic(long Num)
{
  // Don't bother changing song if same already playing
  if(g_CurrentMusic == Num)
    return TRUE;

  // Stop and free current song
  m_MusicChannel.Stop();
  m_MusicChannel.Free();

  DWORD Timer = timeGetTime();
  while(timeGetTime() < Timer)
  {
    DWORD Level = (Timer - timeGetTime()) / 10;
    m_MusicChannel.SetVolume(Level);
  }

  // Load and play new song
  m_MusicChannel.Load(g_MusicFilenames[Num]);
  m_MusicChannel.Play(70,0);

  // Remember new song 
  g_CurrentMusic = Num;
  
  return TRUE;
}

BOOL cApp::StopMusic()
{
  // Stop and free music, marking current song as none
  m_MusicChannel.Stop();
  m_MusicChannel.Free();
  g_CurrentMusic = -1;

  return TRUE;
}

BOOL cApp::GetOldCursor()
{
	m_MyOldCursor = CopyCursor(GetCursor());
	return true;
}

BOOL cApp::SetNewCursor(int i)
{
	if(i == 0)
		SetSystemCursor(m_MyOldCursor, 32512);
	else if(i == 1)
		SetSystemCursor(m_MyCursor1, 32512);
	
	return true;
}

void cApp::LogFrame(void *Ptr, long Purpose)
{
	cApp            *App = (cApp*)Ptr;
	static cTexture LogTexture0;
	static cTexture LogTexture1;
	static cTexture LogTexture2;
	float           Scale = App->m_Height / 512.0;
	static int      num = 1;
	D3DCOLOR        Color;
	long recX1 = 0; 
	long recY1 = 0;
	long recX2 = 0; 
	long recY2 = 0;

	static DWORD LastTime = timeGetTime();
	DWORD ThisTime = timeGetTime();
	DWORD Elapsed = ThisTime - LastTime;

	if(!App->m_IsSilent)
		App->PlayMusic(0);
	App->GetOldCursor();

	// Initialize related data
	if(Purpose == INITPURPOSE)
	{
		// Load textures
		LogTexture0.Load(&App->m_Graphics,"..\\SeuVisitor\\Data\\pic\\others\\logo0.bmp");
		LogTexture1.Load(&App->m_Graphics,"..\\SeuVisitor\\Data\\pic\\others\\logo1.bmp");
		LogTexture2.Load(&App->m_Graphics,"..\\SeuVisitor\\Data\\pic\\others\\logo2.bmp");
		return;
	}

	static cTexture LogTexture = LogTexture1;

	// Shutdown resources used
	if(Purpose == SHUTDOWNPURPOSE)
	{
		App->StopMusic();
		LogTexture.Free();
		LogTexture0.Free();
		LogTexture1.Free();
		LogTexture2.Free();
		return;
	}

	// overleap log on frame if press SPACE pressed
	if(App->m_KeyBoard.GetKeyState(KEY_SPACE))
	{
        g_StateManager.Pop(App);
		g_StateManager.Push(App->MenuFrame, App);
	    return;
	}

	// Render the frame's graphics
   if(App->m_Graphics.BeginScene() ) 
   {
 	   if(Elapsed >= 5000)
	   {
		  LastTime = timeGetTime();
		  num++;
		  switch(num%2)
		  {
		  case 0: LogTexture = LogTexture0; break;
		  case 1: LogTexture = LogTexture2; App->m_Graphics.Clear();break;
		  default: break;
		  }
	   }
	   else
	   {
		 switch(num)
		 {
		 case 3: recX2 = 332; recY2 = 180; break;
		 case 5: recX1 = 332; recX2 = 180; recY2 = 512; break;
		 case 7: recX1 = 0; recY1 = 180; recX2 = 320; recY2 = 512; break;
		 case 9: g_StateManager.Pop(App); g_StateManager.Push(App->MenuFrame, App); break;
		 default: break;
		 }
		 Color = App->SetColor(Elapsed, 2);
		 LogTexture.Blit(g_TextureX, 0, recX1, recY1, recX2, recY2, Scale, Scale, Color);
	   }
	  
  }

  App->m_Graphics.EndScene();

  App->m_Graphics.Display();
}

void cApp::MenuFrame(void *Ptr, long Purpose)
{
	cApp            *App = (cApp*)Ptr;
	static cTexture MenuTexture;
	static cFont    MenuFont;
	float           Scale = App->m_Height / 512.0;
	long            Num;
    
	App->m_Graphics.Clear();
	
	// Initialize related data
	if(Purpose == INITPURPOSE)
	{
		App->SetNewCursor(1);
		ShowCursor(true);

		MenuTexture.Load(&App->m_Graphics,"..\\SeuVisitor\\Data\\pic\\others\\menu.bmp");
		g_MenuSelect.Load(&App->m_Graphics, "..\\SeuVisitor\\Data\\pic\\others\\select.bmp",D3DCOLOR_ARGB(255, 65, 53, 229));

		// Create a large font
	    MenuFont.Create(&App->m_Graphics, "New Times Roman", 72,1);
		g_SetFont.Create(&App->m_Graphics, "New Times Roman", 30,1);
		return;
	}
       
	// Shutdown resources used
	if(Purpose == SHUTDOWNPURPOSE)
	{
		MenuTexture.Free();
		g_MenuSelect.Free();
		MenuFont.Free();
		return;
	}

	if(!App->m_IsSilent)
		App->PlayMusic(1);
	// Exit game or return to game if ESC pressed
	if(App->m_KeyBoard.GetKeyState(KEY_ESC))
	{
        g_StateManager.Pop(App);
	    return;
	}

	// See which option was selected if mouse button pressed
    if(g_MenuOptions!=-1 && App->m_Mouse.GetButtonState(MOUSE_LBUTTON)) 
	{
		// Lock the mouse button and clear button state
		App->m_Mouse.SetLock(MOUSE_LBUTTON, TRUE);
        App->m_Mouse.SetButtonState(MOUSE_LBUTTON, FALSE);

        // Determine which, if any selection
        Num = App->m_Mouse.GetYPos() - 200;
        if(Num >= 0 && Num/64 < 6) 
		{
            Num /= 64;

			// Determine what to do based on selection
			switch(Num)
			{
			case 0: // Return to game
				    if(g_MenuOptions == 1)
						g_StateManager.Pop(App);
					break;

			case 1: // Enter the document
					WinExec("..\\SeuVisitor\\Data\\档案室.exe",SW_SHOWMAXIMIZED);
					break;

                   //New game
			case 2:// Pop all states
					g_MenuOptions = -2;
					g_HasVolunteer = true;
                    g_StateManager.PopAll(App);

					// Push game frame state
					g_StateManager.Push(App->GameFrame, App);
					break;

            case 3: // Pop all states
				    g_MenuOptions = -2;
					g_HasVolunteer = false;
                    g_StateManager.PopAll(App);

					// Push game frame state
					g_StateManager.Push(App->GameFrame, App);
					break;

			case 4: //About game
				    g_StateManager.Push(App->AboutFrame, App);
					break;

			case 5: //Quit game
					g_MenuOptions = -2;
			        App->StopMusic();
				    g_StateManager.PopAll(App);
					break;

			default: break;
			}

		}
	 }

	// Render the frame's graphics
    if(App->m_Graphics.BeginScene() )
	{
        MenuTexture.Blit(g_TextureX, 0, 0, 0, 0, 0, Scale, Scale);
          
		// Draw enabled options
		if(g_MenuOptions != -1)
		{
			App->m_Font.Create(&(App->m_Graphics), "Arial", 16, TRUE);

		    if(g_MenuOptions == 1)
			   App->m_Font.Print("返回游戏", g_TextureX, 200, App->m_Height, 64, 0xFFFFFFFF, DT_CENTER);

			   App->m_Font.Print("档案室", g_TextureX, 264, App->m_Height, 64, 0xFFFFFFFF, DT_CENTER);

               App->m_Font.Print("自由模式", g_TextureX, 328, App->m_Height, 64, 0xFFFFFFFF, DT_CENTER);

			   App->m_Font.Print("比赛模式", g_TextureX, 392, App->m_Height, 64, 0xFFFFFFFF, DT_CENTER);

			   App->m_Font.Print("操作相关", g_TextureX, 456, App->m_Height, 64, 0xFFFFFFFF, DT_CENTER);
			
			   App->m_Font.Print("退出", g_TextureX, 520, App->m_Height, 64, 0xFFFFFFFF, DT_CENTER);
		}

		// Draw the game's title
		MenuFont.Print("东 大 游 侠", g_TextureX, 0, App->m_Height, 100, 0xFFFFFFFF, DT_CENTER);

		// Select option based on mouse position
		Num = App->m_Mouse.GetYPos() - 200;
        if(g_MenuOptions!=-1 && Num >= 0) 
		{
			Num /= 64;

			if((Num==0 && g_MenuOptions==1) || Num==1 
				|| Num==2 || Num==3 || Num==4 || Num==5)

		    g_MenuSelect.Blit(g_TextureX+0.5*App->m_Height-128, Num*64+175, 0, 0, 256, 64, 1.0, 1.0, 0x5FFFFFFF);
		}
		// Set the role
	    if(g_MenuOptions == -1)
		{
			App->m_Tiles.Draw(9, 0, g_TextureX+72, 200);
			App->m_Tiles.Draw(9, 1, App->m_Width-g_TextureX-256, 200);
		
			g_SetFont.Print("选择你的角色", g_TextureX-80, 100, 500, 64, 0xFFFFFFFF, DT_CENTER);
			g_SetFont.Print("江 小 鱼", g_TextureX+72, 500, 184, 64, 0xFFFFFFFF, DT_CENTER);
			g_SetFont.Print("胡 小 依", App->m_Width-g_TextureX-256, 500, 184, 64, 0xFFFFFFFF, DT_CENTER);

			Num = App->m_Mouse.GetXPos();
			if(App->m_Mouse.GetYPos()>500 && App->m_Mouse.GetYPos()<550)
			{
				if(Num>g_TextureX+125 && Num<g_TextureX+210)
					g_MenuSelect.Blit(g_TextureX+36, 484, 0, 0, 256, 64, 1.0, 1.0, 0x5FFFFFFF);
				if(Num>g_TextureX+565 && Num<g_TextureX+650)
					g_MenuSelect.Blit(App->m_Width-g_TextureX-292, 484, 0, 0, 256, 64, 1.0, 1.0, 0x5FFFFFFF);
			
			
				if(App->m_Mouse.GetButtonState(MOUSE_LBUTTON))
				{
					if(Num>g_TextureX+125 && Num<g_TextureX+210)
					{
						App->m_PC = 0;
					}
					else if(Num>g_TextureX+565 && Num<g_TextureX+650)
					{
						App->m_PC = 1;
					}
					
					g_MenuOptions = 0;
					App->m_Mouse.SetLock(MOUSE_LBUTTON, TRUE);
					App->m_Mouse.SetButtonState(MOUSE_LBUTTON, FALSE);
				}

			}
		}

	   App->m_Graphics.EndScene();
	   }
       App->m_Graphics.Display();
}

void cApp::GameFrame(void *Ptr, long Purpose)
{
	cApp            *App = (cApp*)Ptr;
	static cTexture MenuTexture, Texture0, Texture1;
	static int      time = 0;
	int             Num;
	float           scale = App->m_Height / 512.0;
	static float    scale0 = App->m_Width / 512.0;
	char            choice;
	static bool     isOver = false;

	App->m_Graphics.Clear();

    // Initialize related data
	if(Purpose == INITPURPOSE)
	{
		scale0 = App->m_Width / 512.0;

		Texture0.Load(&App->m_Graphics,"..\\SeuVisitor\\Data\\pic\\others\\logo0.bmp");
		MenuTexture.Load(&App->m_Graphics,"..\\SeuVisitor\\Data\\pic\\others\\menu2.bmp");
		g_MenuSelect.Load(&App->m_Graphics, "..\\SeuVisitor\\Data\\pic\\others\\select.bmp",D3DCOLOR_ARGB(255, 65, 53, 229));
		//Create and set the game role
		App->m_Role = new cGameRole();
		App->m_Role->Create(&App->m_Map, &App->m_Graphics, &App->m_Tiles, App->GethWnd(), &(App->m_Font), &(App->m_KeyBoard), 
			 &(App->m_Mouse), App->m_PC, App->m_StartTile, 156, 94, g_HasVolunteer);
		return;
	}

    // Shutdown resources used
	if(Purpose == SHUTDOWNPURPOSE)
	{
		MenuTexture.Free();
		g_MenuSelect.Free();
		Texture0.Free();
		App->m_Role->Free();
		delete App->m_Role;
		App->StopMusic();
		return;
	}

	// Quit to menu screen if ESCAPE pressed
	if(g_DeviceChoice==-1 || (App->m_KeyBoard.GetKeyState(KEY_ESC) && g_DeviceChoice==0))
	{
		Sleep(500);
		g_DeviceChoice = 0;
		// Setup menu options
		if(g_MenuOptions != -2)
			g_MenuOptions = 1;

		// Push main menu state
		g_StateManager.Push(App->MenuFrame, App);

		return;
	 }

 	if(App->m_Graphics.BeginScene())
	{
		// Set the gate
	    if(g_MenuOptions==-2 && g_HasVolunteer)
		{
			scale0 = App->m_Width / 512.0;
			MenuTexture.Blit(g_TextureX, 0, 0, 0, 0, 0, scale, scale);

			g_SetFont.Print("选择游历的起点", g_TextureX-80, 100, 500, 64, 0xFFFFFFFF, DT_CENTER);
			g_SetFont.Print("南大门(群贤门)",g_TextureX, 300, App->m_Height, 64, 0xFF00FFFF, DT_CENTER);
			g_SetFont.Print("东大门(求贤门)", g_TextureX, 364, App->m_Height, 64, 0xFF00FFFF, DT_CENTER);
			g_SetFont.Print("北大门(崇贤门)", g_TextureX, 428, App->m_Height, 64, 0xFF00FFFF, DT_CENTER);
			g_SetFont.Print("西大门(成贤门)", g_TextureX, 492, App->m_Height, 64, 0xFF00FFFF, DT_CENTER);

			// Select option based on mouse position
			Num = App->m_Mouse.GetYPos() - 300;
            if(g_MenuOptions ==-2 && Num >= 0) 
			{
		    	Num /= 64;

			    if(Num==0 || Num==1 || Num==2 || Num==3 )
			        g_MenuSelect.Blit(g_TextureX+0.5*App->m_Height-128, Num*64+285, 0, 0, 256, 64, 1.0, 1.0, 0x5FFFFFFF);
			}
			
			if(Num>=0 && Num<4 && App->m_Mouse.GetButtonState(MOUSE_LBUTTON))
			{
				// Lock the mouse button and clear button state
				App->m_Mouse.SetLock(MOUSE_LBUTTON, TRUE);
				App->m_Mouse.SetButtonState(MOUSE_LBUTTON, FALSE);	
				
				switch(Num)
				{
				case 0: choice = 'S';break;
				case 1: choice = 'E';break;
				case 2: choice = 'N';break;
				case 3: choice = 'W';break;
				default: break;
				}
				
				App->m_Role->GetPos(choice);
		 		App->StopMusic();
				g_MenuOptions = 1;
			}

		}
	  else
	  {
		if(g_HasVolunteer)
		{
			if(!App->m_IsSilent)
				App->PlayMusic(2);
		}
		else
		{
			g_MenuOptions = 1;
			if(!App->m_IsSilent)
				App->PlayMusic(3);
		}

		if(App->m_Role->Run())
			isOver = true;

		App->PlaySound(App->m_Role->GetSound());
		g_DeviceChoice = App->m_Role->GetDeviceChoice();

		if(!isOver && scale0>0)
		{
			scale0 = scale0 - 0.05;
			Texture0.Blit(0, 0, 0, 0, 0, 0, scale0, scale);
		}
		else if(isOver)
		{	
			if(scale0 <= 0)
			{
				Texture1.Load(&App->m_Graphics,"..\\SeuVisitor\\Data\\pic\\others\\end.bmp");
			}
			Texture1.Blit(0, 0, 0, 0, 0, 0, scale0, scale);
			if(scale0 < App->m_Width/512.0+0.1)
				scale0 = scale0 + 0.05;
			else
			{
				g_MenuOptions = 0;
				isOver = false;

				App->m_Font.Create(&(App->m_Graphics), "Arial", 26, TRUE, TRUE);
				App->m_Font.Print("Thank you for your playing !",  g_TextureX, 364, App->m_Height, 64, 0xFFFFFFFF, DT_CENTER);
				Texture1.Free();

			}
		}
	  }
	
 		// End the scene and display the graphics
		App->m_Graphics.EndScene();
		App->m_Graphics.Display();
	}

	if(!isOver && scale0>=App->m_Width/512.0+0.1)
	{
		Sleep(5000);
		g_StateManager.Pop(App);
		g_StateManager.Push(App->MenuFrame, App);
	}

	return;
}


void cApp::AboutFrame(void *Ptr, long Purpose)
{
	cApp            *App = (cApp*)Ptr;
	static cTexture AboutTexture;
	float           scale = App->m_Height / 512.0;

	// Initialize related data
	if(Purpose == INITPURPOSE)
	{
		AboutTexture.Load(&App->m_Graphics,"..\\SeuVisitor\\Data\\pic\\others\\about.bmp");
		return;
	}

    // Shutdown resources used
	if(Purpose == SHUTDOWNPURPOSE)
	{
		AboutTexture.Free();
		return;
	}

	// Quit to menu screen if ESCAPE pressed
	if(App->m_KeyBoard.GetKeyState(KEY_ESC) == TRUE)
	{
		Sleep(500);
		
		// Pop current state
		g_StateManager.Pop(App);
		return;
	 }

 	if(App->m_Graphics.BeginScene())
	{
		AboutTexture.Blit(g_TextureX, 0, 0, 0, 0, 0, scale, scale);
		
 		// End the scene and display the graphics
		App->m_Graphics.EndScene();
		App->m_Graphics.Display();
		return;
	}
}
