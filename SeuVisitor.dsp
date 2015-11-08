# Microsoft Developer Studio Project File - Name="SeuVisitor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SeuVisitor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SeuVisitor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SeuVisitor.mak" CFG="SeuVisitor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SeuVisitor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SeuVisitor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\GameCore" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ Fd"Debug/" /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 strmiids.lib d3d9.lib d3dx9.lib d3dxof.lib dxguid.lib dinput8.lib dsound.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /pdb:none /machine:I386
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "SeuVisitor - Win32 Release"
# Name "SeuVisitor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Building.cpp
# End Source File
# Begin Source File

SOURCE=.\GAMECORE\CORE_GRAPHICS.CPP
# End Source File
# Begin Source File

SOURCE=.\GAMECORE\CORE_INPUT.CPP
# End Source File
# Begin Source File

SOURCE=.\GAMECORE\CORE_SOUND.CPP
# End Source File
# Begin Source File

SOURCE=.\GAMECORE\CORE_SYSTEM.CPP
# End Source File
# Begin Source File

SOURCE=.\Document.cpp
# End Source File
# Begin Source File

SOURCE=.\GameRole.cpp
# End Source File
# Begin Source File

SOURCE=.\HuaRongDao.cpp
# End Source File
# Begin Source File

SOURCE=.\Lock.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\NPC.CPP
# End Source File
# Begin Source File

SOURCE=.\Script.cpp
# End Source File
# Begin Source File

SOURCE=.\Tile.cpp
# End Source File
# Begin Source File

SOURCE=.\TransferBall.cpp
# End Source File
# Begin Source File

SOURCE=.\TransferBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\TransferChair.cpp
# End Source File
# Begin Source File

SOURCE=.\WinMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Building.h
# End Source File
# Begin Source File

SOURCE=.\GAMECORE\CORE_GLOBAL.H
# End Source File
# Begin Source File

SOURCE=.\GAMECORE\CORE_GRAPHICS.H
# End Source File
# Begin Source File

SOURCE=.\GAMECORE\CORE_INPUT.H
# End Source File
# Begin Source File

SOURCE=.\GAMECORE\CORE_SOUND.H
# End Source File
# Begin Source File

SOURCE=.\GAMECORE\CORE_SYSTEM.H
# End Source File
# Begin Source File

SOURCE=.\Document.h
# End Source File
# Begin Source File

SOURCE=.\GameRole.h
# End Source File
# Begin Source File

SOURCE=.\HuaRongDao.h
# End Source File
# Begin Source File

SOURCE=.\Lock.h
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\NPC.H
# End Source File
# Begin Source File

SOURCE=.\Script.h
# End Source File
# Begin Source File

SOURCE=.\SmallGame.h
# End Source File
# Begin Source File

SOURCE=.\Tile.h
# End Source File
# Begin Source File

SOURCE=.\TransferBall.h
# End Source File
# Begin Source File

SOURCE=.\TransferBlock.h
# End Source File
# Begin Source File

SOURCE=.\TransferChair.h
# End Source File
# Begin Source File

SOURCE=.\WinMain.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Education.ico
# End Source File
# Begin Source File

SOURCE=.\Education1.ico
# End Source File
# Begin Source File

SOURCE=.\Script1.rc

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"

!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
