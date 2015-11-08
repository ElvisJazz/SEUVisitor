# Microsoft Developer Studio Generated NMAKE File, Based on SeuVisitor.dsp
!IF $(CFG)" == "
CFG=SeuVisitor - Win32 Debug
!MESSAGE No configuration specified. Defaulting to SeuVisitor - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SeuVisitor - Win32 Release" && "$(CFG)" != "SeuVisitor - Win32 Debug"
!MESSAGE 指定的配置 "$(CFG)" 无效.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# 开始自定义宏
OutDir=.\Release
# 结束自定义宏

ALL : "$(OUTDIR)\SeuVisitor.exe"


CLEAN :
	-@erase "$(INTDIR)\Building.obj"
	-@erase "$(INTDIR)\CORE_Graphics.obj"
	-@erase "$(INTDIR)\CORE_INPUT.OBJ"
	-@erase "$(INTDIR)\CORE_SOUND.OBJ"
	-@erase "$(INTDIR)\CORE_SYSTEM.OBJ"
	-@erase "$(INTDIR)\Document.obj"
	-@erase "$(INTDIR)\GameRole.obj"
	-@erase "$(INTDIR)\HuaRongDao.obj"
	-@erase "$(INTDIR)\Lock.obj"
	-@erase "$(INTDIR)\Map.obj"
	-@erase "$(INTDIR)\NPC.OBJ"
	-@erase "$(INTDIR)\Script.obj"
	-@erase "$(INTDIR)\Tile.obj"
	-@erase "$(INTDIR)\TransferBall.obj"
	-@erase "$(INTDIR)\TransferBlock.obj"
	-@erase "$(INTDIR)\TransferChair.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(OUTDIR)\SeuVisitor.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SeuVisitor.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SeuVisitor.pdb" /machine:I386 /out:"$(OUTDIR)\SeuVisitor.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Building.obj" \
	"$(INTDIR)\CORE_Graphics.obj" \
	"$(INTDIR)\CORE_INPUT.OBJ" \
	"$(INTDIR)\CORE_SOUND.OBJ" \
	"$(INTDIR)\CORE_SYSTEM.OBJ" \
	"$(INTDIR)\Document.obj" \
	"$(INTDIR)\GameRole.obj" \
	"$(INTDIR)\HuaRongDao.obj" \
	"$(INTDIR)\Lock.obj" \
	"$(INTDIR)\Map.obj" \
	"$(INTDIR)\NPC.OBJ" \
	"$(INTDIR)\Script.obj" \
	"$(INTDIR)\Tile.obj" \
	"$(INTDIR)\TransferBall.obj" \
	"$(INTDIR)\TransferBlock.obj" \
	"$(INTDIR)\TransferChair.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\SeuVisitor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# 开始自定义宏
OutDir=.\Debug
# 结束自定义宏

ALL : "$(OUTDIR)\SeuVisitor.exe" "$(OUTDIR)\SeuVisitor.bsc"


CLEAN :
	-@erase "$(INTDIR)\Building.obj"
	-@erase "$(INTDIR)\Building.sbr"
	-@erase "$(INTDIR)\CORE_Graphics.obj"
	-@erase "$(INTDIR)\CORE_Graphics.sbr"
	-@erase "$(INTDIR)\CORE_INPUT.OBJ"
	-@erase "$(INTDIR)\CORE_INPUT.SBR"
	-@erase "$(INTDIR)\CORE_SOUND.OBJ"
	-@erase "$(INTDIR)\CORE_SOUND.SBR"
	-@erase "$(INTDIR)\CORE_SYSTEM.OBJ"
	-@erase "$(INTDIR)\CORE_SYSTEM.SBR"
	-@erase "$(INTDIR)\Document.obj"
	-@erase "$(INTDIR)\Document.sbr"
	-@erase "$(INTDIR)\GameRole.obj"
	-@erase "$(INTDIR)\GameRole.sbr"
	-@erase "$(INTDIR)\HuaRongDao.obj"
	-@erase "$(INTDIR)\HuaRongDao.sbr"
	-@erase "$(INTDIR)\Lock.obj"
	-@erase "$(INTDIR)\Lock.sbr"
	-@erase "$(INTDIR)\Map.obj"
	-@erase "$(INTDIR)\Map.sbr"
	-@erase "$(INTDIR)\NPC.OBJ"
	-@erase "$(INTDIR)\NPC.SBR"
	-@erase "$(INTDIR)\Script.obj"
	-@erase "$(INTDIR)\Script.sbr"
	-@erase "$(INTDIR)\Tile.obj"
	-@erase "$(INTDIR)\Tile.sbr"
	-@erase "$(INTDIR)\TransferBall.obj"
	-@erase "$(INTDIR)\TransferBall.sbr"
	-@erase "$(INTDIR)\TransferBlock.obj"
	-@erase "$(INTDIR)\TransferBlock.sbr"
	-@erase "$(INTDIR)\TransferChair.obj"
	-@erase "$(INTDIR)\TransferChair.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WinMain.obj"
	-@erase "$(INTDIR)\WinMain.sbr"
	-@erase "$(OUTDIR)\SeuVisitor.bsc"
	-@erase "$(OUTDIR)\SeuVisitor.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\..\GameCore" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\SeuVisitor.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ Fd"Debug/" /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SeuVisitor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Building.sbr" \
	"$(INTDIR)\CORE_Graphics.sbr" \
	"$(INTDIR)\CORE_INPUT.SBR" \
	"$(INTDIR)\CORE_SOUND.SBR" \
	"$(INTDIR)\CORE_SYSTEM.SBR" \
	"$(INTDIR)\Document.sbr" \
	"$(INTDIR)\GameRole.sbr" \
	"$(INTDIR)\HuaRongDao.sbr" \
	"$(INTDIR)\Lock.sbr" \
	"$(INTDIR)\Map.sbr" \
	"$(INTDIR)\NPC.SBR" \
	"$(INTDIR)\Script.sbr" \
	"$(INTDIR)\Tile.sbr" \
	"$(INTDIR)\TransferBall.sbr" \
	"$(INTDIR)\TransferBlock.sbr" \
	"$(INTDIR)\TransferChair.sbr" \
	"$(INTDIR)\WinMain.sbr"

"$(OUTDIR)\SeuVisitor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=strmiids.lib d3d9.lib d3dx9.lib d3dxof.lib dxguid.lib dinput8.lib dsound.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /pdb:none /machine:I386 /out:"$(OUTDIR)\SeuVisitor.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Building.obj" \
	"$(INTDIR)\CORE_Graphics.obj" \
	"$(INTDIR)\CORE_INPUT.OBJ" \
	"$(INTDIR)\CORE_SOUND.OBJ" \
	"$(INTDIR)\CORE_SYSTEM.OBJ" \
	"$(INTDIR)\Document.obj" \
	"$(INTDIR)\GameRole.obj" \
	"$(INTDIR)\HuaRongDao.obj" \
	"$(INTDIR)\Lock.obj" \
	"$(INTDIR)\Map.obj" \
	"$(INTDIR)\NPC.OBJ" \
	"$(INTDIR)\Script.obj" \
	"$(INTDIR)\Tile.obj" \
	"$(INTDIR)\TransferBall.obj" \
	"$(INTDIR)\TransferBlock.obj" \
	"$(INTDIR)\TransferChair.obj" \
	"$(INTDIR)\WinMain.obj"

"$(OUTDIR)\SeuVisitor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SeuVisitor.dep")
!INCLUDE "SeuVisitor.dep"
!ELSE 
!MESSAGE Warning: cannot find "SeuVisitor.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SeuVisitor - Win32 Release" || "$(CFG)" == "SeuVisitor - Win32 Debug"
SOURCE=.\Building.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\Building.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\Building.obj"	"$(INTDIR)\Building.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\GAMECORE\CORE_Graphics.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\CORE_Graphics.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\CORE_Graphics.obj"	"$(INTDIR)\CORE_Graphics.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\GAMECORE\CORE_INPUT.CPP

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\CORE_INPUT.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\CORE_INPUT.OBJ"	"$(INTDIR)\CORE_INPUT.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\GAMECORE\CORE_SOUND.CPP

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\CORE_SOUND.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\CORE_SOUND.OBJ"	"$(INTDIR)\CORE_SOUND.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\GAMECORE\CORE_SYSTEM.CPP

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\CORE_SYSTEM.OBJ" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\CORE_SYSTEM.OBJ"	"$(INTDIR)\CORE_SYSTEM.SBR" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\Document.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\Document.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\Document.obj"	"$(INTDIR)\Document.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\GameRole.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\GameRole.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\GameRole.obj"	"$(INTDIR)\GameRole.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\HuaRongDao.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\HuaRongDao.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\HuaRongDao.obj"	"$(INTDIR)\HuaRongDao.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Lock.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\Lock.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\Lock.obj"	"$(INTDIR)\Lock.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Map.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\Map.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\Map.obj"	"$(INTDIR)\Map.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\NPC.CPP

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\NPC.OBJ" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\NPC.OBJ"	"$(INTDIR)\NPC.SBR" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Script.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\Script.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\Script.obj"	"$(INTDIR)\Script.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Tile.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\Tile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\Tile.obj"	"$(INTDIR)\Tile.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TransferBall.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\TransferBall.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\TransferBall.obj"	"$(INTDIR)\TransferBall.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TransferBlock.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\TransferBlock.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\TransferBlock.obj"	"$(INTDIR)\TransferBlock.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TransferChair.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\TransferChair.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\TransferChair.obj"	"$(INTDIR)\TransferChair.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\WinMain.cpp

!IF  "$(CFG)" == "SeuVisitor - Win32 Release"


"$(INTDIR)\WinMain.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "SeuVisitor - Win32 Debug"


"$(INTDIR)\WinMain.obj"	"$(INTDIR)\WinMain.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

