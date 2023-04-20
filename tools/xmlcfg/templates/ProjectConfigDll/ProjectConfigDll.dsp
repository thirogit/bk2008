# Microsoft Developer Studio Project File - Name="ProjectConfigDll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ProjectConfigDll - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ProjectConfigDll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ProjectConfigDll.mak" CFG="ProjectConfigDll - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ProjectConfigDll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProjectConfigDll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProjectConfigDll - Win32 Debug Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ProjectConfigDll - Win32 Release Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ProjectConfigDll - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\components\cmTree" /I "..\components\cmOptionList" /I "..\components\cmTable" /I "..\components\common" /I "..\include" /I "..\components\InputBox" /I "..\project" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\components\xmlparser\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=postBuild_R.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\components\cmTree" /I "..\components\cmOptionList" /I "..\components\cmTable" /I "..\components\common" /I "..\include" /I "..\components\InputBox" /I "..\project" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=postBuild_D.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug Unicode"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ProjectConfigDll___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "ProjectConfigDll___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\include" /I "..\components\xmlparser\include" /I "..\components\AsOptionList" /I "..\components\AsTable" /I "..\components\InputBox" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\components\cmTree" /I "..\components\cmOptionList" /I "..\components\cmTable" /I "..\components\common" /I "..\include" /I "..\components\InputBox" /I "..\project" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\ProjectConfigDll_debug.def" /pdbtype:sept /libpath:"..\components\xmlparser\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\components\xmlparser\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=postBuild_DU.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Release Unicode"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ProjectConfigDll___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "ProjectConfigDll___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\include" /I "..\components\xmlparser\include" /I "..\components\AsOptionList" /I "..\components\AsTable" /I "..\components\InputBox" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\components\cmTree" /I "..\components\cmOptionList" /I "..\components\cmTable" /I "..\components\common" /I "..\include" /I "..\components\InputBox" /I "..\project" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /def:".\ProjectConfigDll_release.def" /libpath:"..\components\xmlparser\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\components\xmlparser\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=postBuild_RU.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ProjectConfigDll - Win32 Release"
# Name "ProjectConfigDll - Win32 Debug"
# Name "ProjectConfigDll - Win32 Debug Unicode"
# Name "ProjectConfigDll - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cmSplitDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectConfigDll.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectConfigDll.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cmSplitDialog.h
# End Source File
# Begin Source File

SOURCE=.\InputBox.h
# End Source File
# Begin Source File

SOURCE=.\ProjectConfigDll_Debug.def

!IF  "$(CFG)" == "ProjectConfigDll - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug"

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug Unicode"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Release Unicode"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProjectConfigDll_Debug_Unicode.def

!IF  "$(CFG)" == "ProjectConfigDll - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug Unicode"

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Release Unicode"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProjectConfigDll_Release.def

!IF  "$(CFG)" == "ProjectConfigDll - Win32 Release"

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug Unicode"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Release Unicode"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ProjectConfigDll_Release_Unicode.def

!IF  "$(CFG)" == "ProjectConfigDll - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Debug Unicode"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ProjectConfigDll - Win32 Release Unicode"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\calendar.ico
# End Source File
# Begin Source File

SOURCE=.\res\chkChecked.ico
# End Source File
# Begin Source File

SOURCE=.\res\chkUnchecked.ico
# End Source File
# Begin Source File

SOURCE=.\res\combobox_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\combobox_over.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HO_SPLIT.CUR
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ProjectConfigDll.rc2
# End Source File
# Begin Source File

SOURCE=.\res\tree_minus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_plus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\validation_error.ico
# End Source File
# Begin Source File

SOURCE=.\res\validation_info.ico
# End Source File
# Begin Source File

SOURCE=.\res\validation_question.ico
# End Source File
# Begin Source File

SOURCE=.\res\validation_warning.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
