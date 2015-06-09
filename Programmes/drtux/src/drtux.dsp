# Microsoft Developer Studio Project File - Name="" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86)  Application" 0x0101

CFG= - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f ".mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f ".mak" CFG=" - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE " - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE " - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == " - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD CPP -MD /W3 /I "..\src" /I "..\..\SesamVitale-QT3" /I "$(QTDIR)\include" /I "ui" /I "." /I "..\..\gest_user\src" /I "..\..\MedinTuxTools" /I "..\..\MedinTuxTools\Calendar" /I "." /I "moc" /I "C:\Qt3\3.3.4\mkspecs\win32-msvc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _DR_TUX /D SESAMVITALE_VERSION /D QT_DLL /D QT_THREAD_SUPPORT /D "QT_NO_DEBUG" /FD /c -nologo -Zm200 -GX -O1 
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD LINK32  "qt-mt334.lib"  "qtmain.lib"  "kernel32.lib"  "user32.lib"  "gdi32.lib"  "comdlg32.lib"  "advapi32.lib"  "shell32.lib"  "ole32.lib"  "oleaut32.lib"  "uuid.lib"  "imm32.lib"  "winmm.lib"  "wsock32.lib"  "winspool.lib"  "kernel32.lib"  "user32.lib"  "gdi32.lib"  "comdlg32.lib"  "advapi32.lib"  "shell32.lib"  "ole32.lib"  "oleaut32.lib"  "uuid.lib"  "imm32.lib"  "winmm.lib"  "wsock32.lib"  "winspool.lib"  "opengl32.lib"  "glu32.lib"  "delayimp.lib"   /NOLOGO /SUBSYSTEM:windows /LIBPATH:"$(QTDIR)\lib" delayimp.lib /DELAYLOAD:comdlg32.dll /DELAYLOAD:oleaut32.dll /DELAYLOAD:winmm.dll /DELAYLOAD:wsock32.dll /DELAYLOAD:winspool.dll  /out:"../bin/drtux.exe


!ELSEIF  "$(CFG)" == " - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "obj"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "obj"
# PROP Target_Dir ""
# ADD CPP -MDd /W3 /GZ /ZI /Od /I "..\src" /I "..\..\SesamVitale-QT3" /I "$(QTDIR)\include" /I "ui" /I "." /I "..\..\gest_user\src" /I "..\..\MedinTuxTools" /I "..\..\MedinTuxTools\Calendar" /I "." /I "moc" /I "C:\Qt3\3.3.4\mkspecs\win32-msvc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D _DR_TUX /D SESAMVITALE_VERSION /D QT_DLL /D QT_THREAD_SUPPORT /FD /c -nologo -Zm200 -GX -Zi  
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD LINK32  "qt-mt334.lib"  "qtmain.lib"  "kernel32.lib"  "user32.lib"  "gdi32.lib"  "comdlg32.lib"  "advapi32.lib"  "shell32.lib"  "ole32.lib"  "oleaut32.lib"  "uuid.lib"  "imm32.lib"  "winmm.lib"  "wsock32.lib"  "winspool.lib"  "kernel32.lib"  "user32.lib"  "gdi32.lib"  "comdlg32.lib"  "advapi32.lib"  "shell32.lib"  "ole32.lib"  "oleaut32.lib"  "uuid.lib"  "imm32.lib"  "winmm.lib"  "wsock32.lib"  "winspool.lib"  "opengl32.lib"  "glu32.lib"  "delayimp.lib"   /NOLOGO /SUBSYSTEM:windows /LIBPATH:"$(QTDIR)\lib" /pdbtype:sept /DEBUG  /out:"../bin/drtux.exe


!ENDIF 

# Begin Target

# Name " - Win32 Release"
# Name " - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\qtsingleapplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\qtsingleapplication_win.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\C_Cps.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\C_Vitale.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\CSesam_API_InfoGroupe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\C_MUI_App.cpp
# End Source File
# Begin Source File

SOURCE=drtux.cpp
# End Source File
# Begin Source File

SOURCE=CApp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\drtux\src\C_DlgListFieldMngr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\C_ImageList.cpp
# End Source File
# Begin Source File

SOURCE=C_FormGlossaire.cpp
# End Source File
# Begin Source File

SOURCE=C_TokenInterpret.cpp
# End Source File
# Begin Source File

SOURCE=CDevilCrucible.cpp
# End Source File
# Begin Source File

SOURCE=main.cpp
# End Source File
# Begin Source File

SOURCE=MyEditText.cpp
# End Source File
# Begin Source File

SOURCE=CMoteur_Base.cpp
# End Source File
# Begin Source File

SOURCE=FormOngletRubrique.cpp
# End Source File
# Begin Source File

SOURCE=CPrinter.cpp
# End Source File
# Begin Source File

SOURCE=structureparser.cpp
# End Source File
# Begin Source File

SOURCE=MyButton.cpp
# End Source File
# Begin Source File

SOURCE=CMyQtable.cpp
# End Source File
# Begin Source File

SOURCE=C_KeyPressEater.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\C_Organiseur.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Codecs\qmdcodec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CW_EditText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Calendar\MyDateTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Calendar\C_Horloge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CGenTools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CHtmlTools.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CMaskedLineEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CCoolPopup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CGestIni.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CMedicaBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\cdlg_passwordget.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CPrtQListBoxItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\ThemePopup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Theme.cpp
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CPrtQListViewItem.cpp
# End Source File
# Begin Source File

SOURCE=CMDI_Ident.cpp
# End Source File
# Begin Source File

SOURCE=CMDI_ChoixPatient.cpp
# End Source File
# Begin Source File

SOURCE=CMDI_Observation.cpp
# End Source File
# Begin Source File

SOURCE=CMDI_Prescription.cpp
# End Source File
# Begin Source File

SOURCE=CMDI_Terrain.cpp
# End Source File
# Begin Source File

SOURCE=CMDI_Generic.cpp
# End Source File
# Begin Source File

SOURCE=CDevilCalc.cpp
# End Source File
# Begin Source File

SOURCE=CDevilGraph.cpp
# End Source File
# Begin Source File

SOURCE=C_RubIdentite.cpp
# End Source File
# Begin Source File

SOURCE=C_Dlg_ImageProperty.cpp
# End Source File
# Begin Source File

SOURCE=C_Atcd.cpp
# End Source File
# Begin Source File

SOURCE=C_Var.cpp
# End Source File
# Begin Source File

SOURCE=C_Dlg_GestionATCD.cpp
# End Source File
# Begin Source File

SOURCE=C_Dlg_VigieFiltreConf.cpp
# End Source File
# Begin Source File

SOURCE=C_Dlg_Pendule.cpp
# End Source File
# Begin Source File

SOURCE=C_Dlg_ChoixUser.cpp
# End Source File
# Begin Source File

SOURCE=C_DrTuxDb_CnxManager.cpp
# End Source File
# Begin Source File

SOURCE=C_ListViewATCDManager.cpp
# End Source File
# Begin Source File

SOURCE=C_Dlg_FamilleGenre.cpp
# End Source File
# Begin Source File

SOURCE=C_RubObservation.cpp
# End Source File
# Begin Source File

SOURCE=RubTerrain.ui.h
# End Source File
# Begin Source File

SOURCE=RubIdentite.ui.h
# End Source File
# Begin Source File

SOURCE=RubChoixPatient.ui.h
# End Source File
# Begin Source File

SOURCE=RubPrescription.ui.h
# End Source File
# Begin Source File

SOURCE=Dock_TabRubrique.ui.h
# End Source File
# Begin Source File

SOURCE=DlgListFieldMngr.ui.h
# End Source File
# Begin Source File

SOURCE=..\..\gest_user\src\Dlg_PermsUserChoice.ui.h
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\DLG_InsertList.ui.h
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\DlgGetString.ui.h
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui.h
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\DlgListOff_Listes.ui.h
# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\DlgMyMessageBox.ui.h
# End Source File
# Begin Source File

SOURCE=Dlg_GestionATCD.ui.h
# End Source File
# Begin Source File

SOURCE=DlgPrint.ui.h
# End Source File
# Begin Source File

SOURCE=DlgGetText.ui.h
# End Source File
# Begin Source File

SOURCE=DlgChoixUser.ui.h
# End Source File
# Begin Source File

SOURCE=DlgDateNom.ui.h
# End Source File
# Begin Source File

SOURCE=Dlg_PosoManager.ui.h
# End Source File
# Begin Source File

SOURCE=Dlg_MedicaTux.ui.h
# End Source File
# Begin Source File

SOURCE=Dlg_MedicaAdjustPoso.ui.h
# End Source File
# Begin Source File

SOURCE=Dlg_HtmlView.ui.h
# End Source File
# Begin Source File

SOURCE=Dlg_AdvancedTerrain.ui.h
# End Source File
# Begin Source File

SOURCE=FormConfig.ui.h
# End Source File
# Begin Source File

SOURCE=DlgAtcd_txt.ui.h
# End Source File
# Begin Source File

SOURCE=Dock_Menu.ui.h
# End Source File

# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\qtsingleapplication.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\SesamVitale-QT3\qtsingleapplication.h...
InputPath=.\..\..\SesamVitale-QT3\qtsingleapplication.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\SesamVitale-QT3\qtsingleapplication.h -o moc\moc_qtsingleapplication.cpp \

"moc\moc_qtsingleapplication.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\SesamVitale-QT3\qtsingleapplication.h...
InputPath=.\..\..\SesamVitale-QT3\qtsingleapplication.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\SesamVitale-QT3\qtsingleapplication.h -o moc\moc_qtsingleapplication.cpp \

"moc\moc_qtsingleapplication.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\C_Cps.h

# End Source File
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\C_Vitale.h

# End Source File
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\CSesam_API_InfoGroupe.h

# End Source File
# Begin Source File

SOURCE=..\..\SesamVitale-QT3\C_MUI_App.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\SesamVitale-QT3\C_MUI_App.h...
InputPath=.\..\..\SesamVitale-QT3\C_MUI_App.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\SesamVitale-QT3\C_MUI_App.h -o moc\moc_C_MUI_App.cpp \

"moc\moc_C_MUI_App.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\SesamVitale-QT3\C_MUI_App.h...
InputPath=.\..\..\SesamVitale-QT3\C_MUI_App.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\SesamVitale-QT3\C_MUI_App.h -o moc\moc_C_MUI_App.cpp \

"moc\moc_C_MUI_App.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=drtux.h

USERDEP_drtux=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing drtux.h...
InputPath=.\drtux.h


BuildCmds= \
	$(QTDIR)\bin\moc drtux.h -o moc\moc_drtux.cpp \

"moc\moc_drtux.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing drtux.h...
InputPath=.\drtux.h


BuildCmds= \
	$(QTDIR)\bin\moc drtux.h -o moc\moc_drtux.cpp \

"moc\moc_drtux.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CApp.h

USERDEP_CApp=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CApp.h...
InputPath=.\CApp.h


BuildCmds= \
	$(QTDIR)\bin\moc CApp.h -o moc\moc_CApp.cpp \

"moc\moc_CApp.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CApp.h...
InputPath=.\CApp.h


BuildCmds= \
	$(QTDIR)\bin\moc CApp.h -o moc\moc_CApp.cpp \

"moc\moc_CApp.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\drtux\src\C_DlgListFieldMngr.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\drtux\src\C_DlgListFieldMngr.h...
InputPath=.\..\..\drtux\src\C_DlgListFieldMngr.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\drtux\src\C_DlgListFieldMngr.h -o moc\moc_C_DlgListFieldMngr.cpp \

"moc\moc_C_DlgListFieldMngr.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\drtux\src\C_DlgListFieldMngr.h...
InputPath=.\..\..\drtux\src\C_DlgListFieldMngr.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\drtux\src\C_DlgListFieldMngr.h -o moc\moc_C_DlgListFieldMngr.cpp \

"moc\moc_C_DlgListFieldMngr.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\C_ImageList.h

# End Source File
# Begin Source File

SOURCE=C_FormGlossaire.h

USERDEP_C_FormGlossaire=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_FormGlossaire.h...
InputPath=.\C_FormGlossaire.h


BuildCmds= \
	$(QTDIR)\bin\moc C_FormGlossaire.h -o moc\moc_C_FormGlossaire.cpp \

"moc\moc_C_FormGlossaire.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_FormGlossaire.h...
InputPath=.\C_FormGlossaire.h


BuildCmds= \
	$(QTDIR)\bin\moc C_FormGlossaire.h -o moc\moc_C_FormGlossaire.cpp \

"moc\moc_C_FormGlossaire.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=C_TokenInterpret.h

# End Source File
# Begin Source File

SOURCE=CDevilCrucible.h

# End Source File
# Begin Source File

SOURCE=MyEditText.h

USERDEP_MyEditText=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing MyEditText.h...
InputPath=.\MyEditText.h


BuildCmds= \
	$(QTDIR)\bin\moc MyEditText.h -o moc\moc_MyEditText.cpp \

"moc\moc_MyEditText.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing MyEditText.h...
InputPath=.\MyEditText.h


BuildCmds= \
	$(QTDIR)\bin\moc MyEditText.h -o moc\moc_MyEditText.cpp \

"moc\moc_MyEditText.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CMoteur_Base.h

USERDEP_CMoteur_Base=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CMoteur_Base.h...
InputPath=.\CMoteur_Base.h


BuildCmds= \
	$(QTDIR)\bin\moc CMoteur_Base.h -o moc\moc_CMoteur_Base.cpp \

"moc\moc_CMoteur_Base.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CMoteur_Base.h...
InputPath=.\CMoteur_Base.h


BuildCmds= \
	$(QTDIR)\bin\moc CMoteur_Base.h -o moc\moc_CMoteur_Base.cpp \

"moc\moc_CMoteur_Base.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=FormOngletRubrique.h

USERDEP_FormOngletRubrique=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing FormOngletRubrique.h...
InputPath=.\FormOngletRubrique.h


BuildCmds= \
	$(QTDIR)\bin\moc FormOngletRubrique.h -o moc\moc_FormOngletRubrique.cpp \

"moc\moc_FormOngletRubrique.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing FormOngletRubrique.h...
InputPath=.\FormOngletRubrique.h


BuildCmds= \
	$(QTDIR)\bin\moc FormOngletRubrique.h -o moc\moc_FormOngletRubrique.cpp \

"moc\moc_FormOngletRubrique.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Global.h

# End Source File
# Begin Source File

SOURCE=CPrinter.h

USERDEP_CPrinter=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CPrinter.h...
InputPath=.\CPrinter.h


BuildCmds= \
	$(QTDIR)\bin\moc CPrinter.h -o moc\moc_CPrinter.cpp \

"moc\moc_CPrinter.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CPrinter.h...
InputPath=.\CPrinter.h


BuildCmds= \
	$(QTDIR)\bin\moc CPrinter.h -o moc\moc_CPrinter.cpp \

"moc\moc_CPrinter.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Rubriques.h

# End Source File
# Begin Source File

SOURCE=structureparser.h

# End Source File
# Begin Source File

SOURCE=MyButton.h

USERDEP_MyButton=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing MyButton.h...
InputPath=.\MyButton.h


BuildCmds= \
	$(QTDIR)\bin\moc MyButton.h -o moc\moc_MyButton.cpp \

"moc\moc_MyButton.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing MyButton.h...
InputPath=.\MyButton.h


BuildCmds= \
	$(QTDIR)\bin\moc MyButton.h -o moc\moc_MyButton.cpp \

"moc\moc_MyButton.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CBigShrinker.h

USERDEP_CBigShrinker=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CBigShrinker.h...
InputPath=.\CBigShrinker.h


BuildCmds= \
	$(QTDIR)\bin\moc CBigShrinker.h -o moc\moc_CBigShrinker.cpp \

"moc\moc_CBigShrinker.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CBigShrinker.h...
InputPath=.\CBigShrinker.h


BuildCmds= \
	$(QTDIR)\bin\moc CBigShrinker.h -o moc\moc_CBigShrinker.cpp \

"moc\moc_CBigShrinker.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CMyQtable.h

USERDEP_CMyQtable=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CMyQtable.h...
InputPath=.\CMyQtable.h


BuildCmds= \
	$(QTDIR)\bin\moc CMyQtable.h -o moc\moc_CMyQtable.cpp \

"moc\moc_CMyQtable.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CMyQtable.h...
InputPath=.\CMyQtable.h


BuildCmds= \
	$(QTDIR)\bin\moc CMyQtable.h -o moc\moc_CMyQtable.cpp \

"moc\moc_CMyQtable.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CRubList.h

# End Source File
# Begin Source File

SOURCE=CPreview.h

# End Source File
# Begin Source File

SOURCE=C_KeyPressEater.h

USERDEP_C_KeyPressEater=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_KeyPressEater.h...
InputPath=.\C_KeyPressEater.h


BuildCmds= \
	$(QTDIR)\bin\moc C_KeyPressEater.h -o moc\moc_C_KeyPressEater.cpp \

"moc\moc_C_KeyPressEater.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_KeyPressEater.h...
InputPath=.\C_KeyPressEater.h


BuildCmds= \
	$(QTDIR)\bin\moc C_KeyPressEater.h -o moc\moc_C_KeyPressEater.cpp \

"moc\moc_C_KeyPressEater.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\C_Organiseur.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\C_Organiseur.h...
InputPath=.\..\..\MedinTuxTools\C_Organiseur.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\C_Organiseur.h -o moc\moc_C_Organiseur.cpp \

"moc\moc_C_Organiseur.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\C_Organiseur.h...
InputPath=.\..\..\MedinTuxTools\C_Organiseur.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\C_Organiseur.h -o moc\moc_C_Organiseur.cpp \

"moc\moc_C_Organiseur.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Codecs\qmdcodec.h

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CW_EditText.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\CW_EditText.h...
InputPath=.\..\..\MedinTuxTools\CW_EditText.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\CW_EditText.h -o moc\moc_CW_EditText.cpp \

"moc\moc_CW_EditText.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\CW_EditText.h...
InputPath=.\..\..\MedinTuxTools\CW_EditText.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\CW_EditText.h -o moc\moc_CW_EditText.cpp \

"moc\moc_CW_EditText.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Calendar\MyDateTable.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\Calendar\MyDateTable.h...
InputPath=.\..\..\MedinTuxTools\Calendar\MyDateTable.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\Calendar\MyDateTable.h -o moc\moc_MyDateTable.cpp \

"moc\moc_MyDateTable.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\Calendar\MyDateTable.h...
InputPath=.\..\..\MedinTuxTools\Calendar\MyDateTable.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\Calendar\MyDateTable.h -o moc\moc_MyDateTable.cpp \

"moc\moc_MyDateTable.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Calendar\C_Horloge.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\Calendar\C_Horloge.h...
InputPath=.\..\..\MedinTuxTools\Calendar\C_Horloge.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\Calendar\C_Horloge.h -o moc\moc_C_Horloge.cpp \

"moc\moc_C_Horloge.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\Calendar\C_Horloge.h...
InputPath=.\..\..\MedinTuxTools\Calendar\C_Horloge.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\Calendar\C_Horloge.h -o moc\moc_C_Horloge.cpp \

"moc\moc_C_Horloge.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CGenTools.h

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CHtmlTools.h

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CMaskedLineEdit.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\CMaskedLineEdit.h...
InputPath=.\..\..\MedinTuxTools\CMaskedLineEdit.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\CMaskedLineEdit.h -o moc\moc_CMaskedLineEdit.cpp \

"moc\moc_CMaskedLineEdit.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\CMaskedLineEdit.h...
InputPath=.\..\..\MedinTuxTools\CMaskedLineEdit.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\CMaskedLineEdit.h -o moc\moc_CMaskedLineEdit.cpp \

"moc\moc_CMaskedLineEdit.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CCoolPopup.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\CCoolPopup.h...
InputPath=.\..\..\MedinTuxTools\CCoolPopup.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\CCoolPopup.h -o moc\moc_CCoolPopup.cpp \

"moc\moc_CCoolPopup.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\CCoolPopup.h...
InputPath=.\..\..\MedinTuxTools\CCoolPopup.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\CCoolPopup.h -o moc\moc_CCoolPopup.cpp \

"moc\moc_CCoolPopup.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CGestIni.h

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CMedicaBase.h

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\cdlg_passwordget.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\cdlg_passwordget.h...
InputPath=.\..\..\MedinTuxTools\cdlg_passwordget.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\cdlg_passwordget.h -o moc\moc_cdlg_passwordget.cpp \

"moc\moc_cdlg_passwordget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\cdlg_passwordget.h...
InputPath=.\..\..\MedinTuxTools\cdlg_passwordget.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\cdlg_passwordget.h -o moc\moc_cdlg_passwordget.cpp \

"moc\moc_cdlg_passwordget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CPrtQListBoxItem.h

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\ThemePopup.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\ThemePopup.h...
InputPath=.\..\..\MedinTuxTools\ThemePopup.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\ThemePopup.h -o moc\moc_ThemePopup.cpp \

"moc\moc_ThemePopup.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\ThemePopup.h...
InputPath=.\..\..\MedinTuxTools\ThemePopup.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\ThemePopup.h -o moc\moc_ThemePopup.cpp \

"moc\moc_ThemePopup.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Theme.h

# End Source File
# Begin Source File

SOURCE=CMDI_Terrain.h

USERDEP_CMDI_Terrain=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CMDI_Terrain.h...
InputPath=.\CMDI_Terrain.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Terrain.h -o moc\moc_CMDI_Terrain.cpp \

"moc\moc_CMDI_Terrain.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CMDI_Terrain.h...
InputPath=.\CMDI_Terrain.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Terrain.h -o moc\moc_CMDI_Terrain.cpp \

"moc\moc_CMDI_Terrain.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CMDI_Generic.h

USERDEP_CMDI_Generic=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CMDI_Generic.h...
InputPath=.\CMDI_Generic.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Generic.h -o moc\moc_CMDI_Generic.cpp \

"moc\moc_CMDI_Generic.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CMDI_Generic.h...
InputPath=.\CMDI_Generic.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Generic.h -o moc\moc_CMDI_Generic.cpp \

"moc\moc_CMDI_Generic.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CMDI_Ident.h

USERDEP_CMDI_Ident=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CMDI_Ident.h...
InputPath=.\CMDI_Ident.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Ident.h -o moc\moc_CMDI_Ident.cpp \

"moc\moc_CMDI_Ident.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CMDI_Ident.h...
InputPath=.\CMDI_Ident.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Ident.h -o moc\moc_CMDI_Ident.cpp \

"moc\moc_CMDI_Ident.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CMDI_ChoixPatient.h

USERDEP_CMDI_ChoixPatient=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CMDI_ChoixPatient.h...
InputPath=.\CMDI_ChoixPatient.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_ChoixPatient.h -o moc\moc_CMDI_ChoixPatient.cpp \

"moc\moc_CMDI_ChoixPatient.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CMDI_ChoixPatient.h...
InputPath=.\CMDI_ChoixPatient.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_ChoixPatient.h -o moc\moc_CMDI_ChoixPatient.cpp \

"moc\moc_CMDI_ChoixPatient.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CMDI_Observation.h

USERDEP_CMDI_Observation=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CMDI_Observation.h...
InputPath=.\CMDI_Observation.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Observation.h -o moc\moc_CMDI_Observation.cpp \

"moc\moc_CMDI_Observation.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CMDI_Observation.h...
InputPath=.\CMDI_Observation.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Observation.h -o moc\moc_CMDI_Observation.cpp \

"moc\moc_CMDI_Observation.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CMDI_Prescription.h

USERDEP_CMDI_Prescription=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing CMDI_Prescription.h...
InputPath=.\CMDI_Prescription.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Prescription.h -o moc\moc_CMDI_Prescription.cpp \

"moc\moc_CMDI_Prescription.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing CMDI_Prescription.h...
InputPath=.\CMDI_Prescription.h


BuildCmds= \
	$(QTDIR)\bin\moc CMDI_Prescription.h -o moc\moc_CMDI_Prescription.cpp \

"moc\moc_CMDI_Prescription.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=CDevilCalc.h

# End Source File
# Begin Source File

SOURCE=CDevilGraph.h

# End Source File
# Begin Source File

SOURCE=FormConfig.ui.h

# End Source File
# Begin Source File

SOURCE=C_RubIdentite.h

USERDEP_C_RubIdentite=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_RubIdentite.h...
InputPath=.\C_RubIdentite.h


BuildCmds= \
	$(QTDIR)\bin\moc C_RubIdentite.h -o moc\moc_C_RubIdentite.cpp \

"moc\moc_C_RubIdentite.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_RubIdentite.h...
InputPath=.\C_RubIdentite.h


BuildCmds= \
	$(QTDIR)\bin\moc C_RubIdentite.h -o moc\moc_C_RubIdentite.cpp \

"moc\moc_C_RubIdentite.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=C_Dlg_ImageProperty.h

USERDEP_C_Dlg_ImageProperty=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_Dlg_ImageProperty.h...
InputPath=.\C_Dlg_ImageProperty.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_ImageProperty.h -o moc\moc_C_Dlg_ImageProperty.cpp \

"moc\moc_C_Dlg_ImageProperty.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_Dlg_ImageProperty.h...
InputPath=.\C_Dlg_ImageProperty.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_ImageProperty.h -o moc\moc_C_Dlg_ImageProperty.cpp \

"moc\moc_C_Dlg_ImageProperty.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dock_Menu.ui.h

# End Source File
# Begin Source File

SOURCE=C_Atcd.h

USERDEP_C_Atcd=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_Atcd.h...
InputPath=.\C_Atcd.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Atcd.h -o moc\moc_C_Atcd.cpp \

"moc\moc_C_Atcd.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_Atcd.h...
InputPath=.\C_Atcd.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Atcd.h -o moc\moc_C_Atcd.cpp \

"moc\moc_C_Atcd.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\CPrtQListViewItem.h

USERDEP_=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\CPrtQListViewItem.h...
InputPath=.\..\..\MedinTuxTools\CPrtQListViewItem.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\CPrtQListViewItem.h -o moc\moc_CPrtQListViewItem.cpp \

"moc\moc_CPrtQListViewItem.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing ..\..\MedinTuxTools\CPrtQListViewItem.h...
InputPath=.\..\..\MedinTuxTools\CPrtQListViewItem.h


BuildCmds= \
	$(QTDIR)\bin\moc ..\..\MedinTuxTools\CPrtQListViewItem.h -o moc\moc_CPrtQListViewItem.cpp \

"moc\moc_CPrtQListViewItem.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=C_Var.h

# End Source File
# Begin Source File

SOURCE=RubTerrain.ui.h

# End Source File
# Begin Source File

SOURCE=RubPrescription.ui.h

# End Source File
# Begin Source File

SOURCE=RubChoixPatient.ui.h

# End Source File
# Begin Source File

SOURCE=DlgPrint.ui.h

# End Source File
# Begin Source File

SOURCE=RubIdentite.ui.h

# End Source File
# Begin Source File

SOURCE=DlgListFieldMngr.ui.h

# End Source File
# Begin Source File

SOURCE=C_Dlg_GestionATCD.h

USERDEP_C_Dlg_GestionATCD=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_Dlg_GestionATCD.h...
InputPath=.\C_Dlg_GestionATCD.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_GestionATCD.h -o moc\moc_C_Dlg_GestionATCD.cpp \

"moc\moc_C_Dlg_GestionATCD.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_Dlg_GestionATCD.h...
InputPath=.\C_Dlg_GestionATCD.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_GestionATCD.h -o moc\moc_C_Dlg_GestionATCD.cpp \

"moc\moc_C_Dlg_GestionATCD.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=C_Dlg_VigieFiltreConf.h

USERDEP_C_Dlg_VigieFiltreConf=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_Dlg_VigieFiltreConf.h...
InputPath=.\C_Dlg_VigieFiltreConf.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_VigieFiltreConf.h -o moc\moc_C_Dlg_VigieFiltreConf.cpp \

"moc\moc_C_Dlg_VigieFiltreConf.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_Dlg_VigieFiltreConf.h...
InputPath=.\C_Dlg_VigieFiltreConf.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_VigieFiltreConf.h -o moc\moc_C_Dlg_VigieFiltreConf.cpp \

"moc\moc_C_Dlg_VigieFiltreConf.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=C_Dlg_Pendule.h

USERDEP_C_Dlg_Pendule=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_Dlg_Pendule.h...
InputPath=.\C_Dlg_Pendule.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_Pendule.h -o moc\moc_C_Dlg_Pendule.cpp \

"moc\moc_C_Dlg_Pendule.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_Dlg_Pendule.h...
InputPath=.\C_Dlg_Pendule.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_Pendule.h -o moc\moc_C_Dlg_Pendule.cpp \

"moc\moc_C_Dlg_Pendule.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=C_Dlg_ChoixUser.h

USERDEP_C_Dlg_ChoixUser=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_Dlg_ChoixUser.h...
InputPath=.\C_Dlg_ChoixUser.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_ChoixUser.h -o moc\moc_C_Dlg_ChoixUser.cpp \

"moc\moc_C_Dlg_ChoixUser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_Dlg_ChoixUser.h...
InputPath=.\C_Dlg_ChoixUser.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_ChoixUser.h -o moc\moc_C_Dlg_ChoixUser.cpp \

"moc\moc_C_Dlg_ChoixUser.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=C_DrTuxDb_CnxManager.h

# End Source File
# Begin Source File

SOURCE=C_ListViewATCDManager.h

USERDEP_C_ListViewATCDManager=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_ListViewATCDManager.h...
InputPath=.\C_ListViewATCDManager.h


BuildCmds= \
	$(QTDIR)\bin\moc C_ListViewATCDManager.h -o moc\moc_C_ListViewATCDManager.cpp \

"moc\moc_C_ListViewATCDManager.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_ListViewATCDManager.h...
InputPath=.\C_ListViewATCDManager.h


BuildCmds= \
	$(QTDIR)\bin\moc C_ListViewATCDManager.h -o moc\moc_C_ListViewATCDManager.cpp \

"moc\moc_C_ListViewATCDManager.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=C_Dlg_FamilleGenre.h

USERDEP_C_Dlg_FamilleGenre=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_Dlg_FamilleGenre.h...
InputPath=.\C_Dlg_FamilleGenre.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_FamilleGenre.h -o moc\moc_C_Dlg_FamilleGenre.cpp \

"moc\moc_C_Dlg_FamilleGenre.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_Dlg_FamilleGenre.h...
InputPath=.\C_Dlg_FamilleGenre.h


BuildCmds= \
	$(QTDIR)\bin\moc C_Dlg_FamilleGenre.h -o moc\moc_C_Dlg_FamilleGenre.cpp \

"moc\moc_C_Dlg_FamilleGenre.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=C_RubObservation.h

USERDEP_C_RubObservation=""$(QTDIR)\bin\moc.exe""

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Moc'ing C_RubObservation.h...
InputPath=.\C_RubObservation.h


BuildCmds= \
	$(QTDIR)\bin\moc C_RubObservation.h -o moc\moc_C_RubObservation.cpp \

"moc\moc_C_RubObservation.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Moc'ing C_RubObservation.h...
InputPath=.\C_RubObservation.h


BuildCmds= \
	$(QTDIR)\bin\moc C_RubObservation.h -o moc\moc_C_RubObservation.cpp \

"moc\moc_C_RubObservation.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File

# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group

# Begin Group "Forms"
# Prop Default_Filter "ui"
# Begin Source File

SOURCE=RubTerrain.ui
USERDEP_RubTerrain.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing RubTerrain.ui...
InputPath=.\RubTerrain.ui

BuildCmds= \
	$(QTDIR)\bin\uic RubTerrain.ui -o ui\RubTerrain.h \
	$(QTDIR)\bin\uic RubTerrain.ui -i RubTerrain.h -o ui\RubTerrain.cpp \
	$(QTDIR)\bin\moc  ui\RubTerrain.h -o moc\moc_RubTerrain.cpp \

"ui\RubTerrain.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\RubTerrain.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_RubTerrain.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing RubTerrain.ui...
InputPath=.\RubTerrain.ui

BuildCmds= \
	$(QTDIR)\bin\uic RubTerrain.ui -o ui\RubTerrain.h \
	$(QTDIR)\bin\uic RubTerrain.ui -i RubTerrain.h -o ui\RubTerrain.cpp \
	$(QTDIR)\bin\moc  ui\RubTerrain.h -o moc\moc_RubTerrain.cpp \

"ui\RubTerrain.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\RubTerrain.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_RubTerrain.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=RubIdentite.ui
USERDEP_RubIdentite.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing RubIdentite.ui...
InputPath=.\RubIdentite.ui

BuildCmds= \
	$(QTDIR)\bin\uic RubIdentite.ui -o ui\RubIdentite.h \
	$(QTDIR)\bin\uic RubIdentite.ui -i RubIdentite.h -o ui\RubIdentite.cpp \
	$(QTDIR)\bin\moc  ui\RubIdentite.h -o moc\moc_RubIdentite.cpp \

"ui\RubIdentite.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\RubIdentite.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_RubIdentite.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing RubIdentite.ui...
InputPath=.\RubIdentite.ui

BuildCmds= \
	$(QTDIR)\bin\uic RubIdentite.ui -o ui\RubIdentite.h \
	$(QTDIR)\bin\uic RubIdentite.ui -i RubIdentite.h -o ui\RubIdentite.cpp \
	$(QTDIR)\bin\moc  ui\RubIdentite.h -o moc\moc_RubIdentite.cpp \

"ui\RubIdentite.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\RubIdentite.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_RubIdentite.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=RubChoixPatient.ui
USERDEP_RubChoixPatient.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing RubChoixPatient.ui...
InputPath=.\RubChoixPatient.ui

BuildCmds= \
	$(QTDIR)\bin\uic RubChoixPatient.ui -o ui\RubChoixPatient.h \
	$(QTDIR)\bin\uic RubChoixPatient.ui -i RubChoixPatient.h -o ui\RubChoixPatient.cpp \
	$(QTDIR)\bin\moc  ui\RubChoixPatient.h -o moc\moc_RubChoixPatient.cpp \

"ui\RubChoixPatient.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\RubChoixPatient.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_RubChoixPatient.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing RubChoixPatient.ui...
InputPath=.\RubChoixPatient.ui

BuildCmds= \
	$(QTDIR)\bin\uic RubChoixPatient.ui -o ui\RubChoixPatient.h \
	$(QTDIR)\bin\uic RubChoixPatient.ui -i RubChoixPatient.h -o ui\RubChoixPatient.cpp \
	$(QTDIR)\bin\moc  ui\RubChoixPatient.h -o moc\moc_RubChoixPatient.cpp \

"ui\RubChoixPatient.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\RubChoixPatient.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_RubChoixPatient.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=RubPrescription.ui
USERDEP_RubPrescription.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing RubPrescription.ui...
InputPath=.\RubPrescription.ui

BuildCmds= \
	$(QTDIR)\bin\uic RubPrescription.ui -o ui\RubPrescription.h \
	$(QTDIR)\bin\uic RubPrescription.ui -i RubPrescription.h -o ui\RubPrescription.cpp \
	$(QTDIR)\bin\moc  ui\RubPrescription.h -o moc\moc_RubPrescription.cpp \

"ui\RubPrescription.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\RubPrescription.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_RubPrescription.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing RubPrescription.ui...
InputPath=.\RubPrescription.ui

BuildCmds= \
	$(QTDIR)\bin\uic RubPrescription.ui -o ui\RubPrescription.h \
	$(QTDIR)\bin\uic RubPrescription.ui -i RubPrescription.h -o ui\RubPrescription.cpp \
	$(QTDIR)\bin\moc  ui\RubPrescription.h -o moc\moc_RubPrescription.cpp \

"ui\RubPrescription.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\RubPrescription.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_RubPrescription.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dock_TabRubrique.ui
USERDEP_Dock_TabRubrique.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dock_TabRubrique.ui...
InputPath=.\Dock_TabRubrique.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dock_TabRubrique.ui -o ui\Dock_TabRubrique.h \
	$(QTDIR)\bin\uic Dock_TabRubrique.ui -i Dock_TabRubrique.h -o ui\Dock_TabRubrique.cpp \
	$(QTDIR)\bin\moc  ui\Dock_TabRubrique.h -o moc\moc_Dock_TabRubrique.cpp \

"ui\Dock_TabRubrique.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dock_TabRubrique.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dock_TabRubrique.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dock_TabRubrique.ui...
InputPath=.\Dock_TabRubrique.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dock_TabRubrique.ui -o ui\Dock_TabRubrique.h \
	$(QTDIR)\bin\uic Dock_TabRubrique.ui -i Dock_TabRubrique.h -o ui\Dock_TabRubrique.cpp \
	$(QTDIR)\bin\moc  ui\Dock_TabRubrique.h -o moc\moc_Dock_TabRubrique.cpp \

"ui\Dock_TabRubrique.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dock_TabRubrique.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dock_TabRubrique.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=FormGlossaire.ui
USERDEP_FormGlossaire.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing FormGlossaire.ui...
InputPath=.\FormGlossaire.ui

BuildCmds= \
	$(QTDIR)\bin\uic FormGlossaire.ui -o ui\FormGlossaire.h \
	$(QTDIR)\bin\uic FormGlossaire.ui -i FormGlossaire.h -o ui\FormGlossaire.cpp \
	$(QTDIR)\bin\moc  ui\FormGlossaire.h -o moc\moc_FormGlossaire.cpp \

"ui\FormGlossaire.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\FormGlossaire.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_FormGlossaire.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing FormGlossaire.ui...
InputPath=.\FormGlossaire.ui

BuildCmds= \
	$(QTDIR)\bin\uic FormGlossaire.ui -o ui\FormGlossaire.h \
	$(QTDIR)\bin\uic FormGlossaire.ui -i FormGlossaire.h -o ui\FormGlossaire.cpp \
	$(QTDIR)\bin\moc  ui\FormGlossaire.h -o moc\moc_FormGlossaire.cpp \

"ui\FormGlossaire.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\FormGlossaire.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_FormGlossaire.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=DlgListFieldMngr.ui
USERDEP_DlgListFieldMngr.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing DlgListFieldMngr.ui...
InputPath=.\DlgListFieldMngr.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgListFieldMngr.ui -o ui\DlgListFieldMngr.h \
	$(QTDIR)\bin\uic DlgListFieldMngr.ui -i DlgListFieldMngr.h -o ui\DlgListFieldMngr.cpp \
	$(QTDIR)\bin\moc  ui\DlgListFieldMngr.h -o moc\moc_DlgListFieldMngr.cpp \

"ui\DlgListFieldMngr.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgListFieldMngr.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgListFieldMngr.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing DlgListFieldMngr.ui...
InputPath=.\DlgListFieldMngr.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgListFieldMngr.ui -o ui\DlgListFieldMngr.h \
	$(QTDIR)\bin\uic DlgListFieldMngr.ui -i DlgListFieldMngr.h -o ui\DlgListFieldMngr.cpp \
	$(QTDIR)\bin\moc  ui\DlgListFieldMngr.h -o moc\moc_DlgListFieldMngr.cpp \

"ui\DlgListFieldMngr.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgListFieldMngr.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgListFieldMngr.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\gest_user\src\Dlg_PermsUserChoice.ui
USERDEP_..\..\gest_user\src\Dlg_PermsUserChoice.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing ..\..\gest_user\src\Dlg_PermsUserChoice.ui...
InputPath=.\..\..\gest_user\src\Dlg_PermsUserChoice.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\gest_user\src\Dlg_PermsUserChoice.ui -o ui\Dlg_PermsUserChoice.h \
	$(QTDIR)\bin\uic ..\..\gest_user\src\Dlg_PermsUserChoice.ui -i Dlg_PermsUserChoice.h -o ui\Dlg_PermsUserChoice.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_PermsUserChoice.h -o moc\moc_Dlg_PermsUserChoice.cpp \

"ui\Dlg_PermsUserChoice.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_PermsUserChoice.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_PermsUserChoice.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing ..\..\gest_user\src\Dlg_PermsUserChoice.ui...
InputPath=.\..\..\gest_user\src\Dlg_PermsUserChoice.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\gest_user\src\Dlg_PermsUserChoice.ui -o ui\Dlg_PermsUserChoice.h \
	$(QTDIR)\bin\uic ..\..\gest_user\src\Dlg_PermsUserChoice.ui -i Dlg_PermsUserChoice.h -o ui\Dlg_PermsUserChoice.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_PermsUserChoice.h -o moc\moc_Dlg_PermsUserChoice.cpp \

"ui\Dlg_PermsUserChoice.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_PermsUserChoice.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_PermsUserChoice.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\DLG_InsertList.ui
USERDEP_..\..\MedinTuxTools\DLG_InsertList.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\DLG_InsertList.ui...
InputPath=.\..\..\MedinTuxTools\DLG_InsertList.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DLG_InsertList.ui -o ui\DLG_InsertList.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DLG_InsertList.ui -i DLG_InsertList.h -o ui\DLG_InsertList.cpp \
	$(QTDIR)\bin\moc  ui\DLG_InsertList.h -o moc\moc_DLG_InsertList.cpp \

"ui\DLG_InsertList.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DLG_InsertList.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DLG_InsertList.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\DLG_InsertList.ui...
InputPath=.\..\..\MedinTuxTools\DLG_InsertList.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DLG_InsertList.ui -o ui\DLG_InsertList.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DLG_InsertList.ui -i DLG_InsertList.h -o ui\DLG_InsertList.cpp \
	$(QTDIR)\bin\moc  ui\DLG_InsertList.h -o moc\moc_DLG_InsertList.cpp \

"ui\DLG_InsertList.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DLG_InsertList.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DLG_InsertList.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\DlgGetString.ui
USERDEP_..\..\MedinTuxTools\DlgGetString.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\DlgGetString.ui...
InputPath=.\..\..\MedinTuxTools\DlgGetString.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgGetString.ui -o ui\DlgGetString.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgGetString.ui -i DlgGetString.h -o ui\DlgGetString.cpp \
	$(QTDIR)\bin\moc  ui\DlgGetString.h -o moc\moc_DlgGetString.cpp \

"ui\DlgGetString.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgGetString.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgGetString.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\DlgGetString.ui...
InputPath=.\..\..\MedinTuxTools\DlgGetString.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgGetString.ui -o ui\DlgGetString.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgGetString.ui -i DlgGetString.h -o ui\DlgGetString.cpp \
	$(QTDIR)\bin\moc  ui\DlgGetString.h -o moc\moc_DlgGetString.cpp \

"ui\DlgGetString.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgGetString.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgGetString.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui
USERDEP_..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui...
InputPath=.\..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui -o ui\Dlg_Calendar.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui -i Dlg_Calendar.h -o ui\Dlg_Calendar.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_Calendar.h -o moc\moc_Dlg_Calendar.cpp \

"ui\Dlg_Calendar.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_Calendar.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_Calendar.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui...
InputPath=.\..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui -o ui\Dlg_Calendar.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Calendar\Dlg_Calendar.ui -i Dlg_Calendar.h -o ui\Dlg_Calendar.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_Calendar.h -o moc\moc_Dlg_Calendar.cpp \

"ui\Dlg_Calendar.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_Calendar.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_Calendar.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\DlgListOff_Listes.ui
USERDEP_..\..\MedinTuxTools\DlgListOff_Listes.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\DlgListOff_Listes.ui...
InputPath=.\..\..\MedinTuxTools\DlgListOff_Listes.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgListOff_Listes.ui -o ui\DlgListOff_Listes.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgListOff_Listes.ui -i DlgListOff_Listes.h -o ui\DlgListOff_Listes.cpp \
	$(QTDIR)\bin\moc  ui\DlgListOff_Listes.h -o moc\moc_DlgListOff_Listes.cpp \

"ui\DlgListOff_Listes.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgListOff_Listes.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgListOff_Listes.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\DlgListOff_Listes.ui...
InputPath=.\..\..\MedinTuxTools\DlgListOff_Listes.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgListOff_Listes.ui -o ui\DlgListOff_Listes.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgListOff_Listes.ui -i DlgListOff_Listes.h -o ui\DlgListOff_Listes.cpp \
	$(QTDIR)\bin\moc  ui\DlgListOff_Listes.h -o moc\moc_DlgListOff_Listes.cpp \

"ui\DlgListOff_Listes.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgListOff_Listes.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgListOff_Listes.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\DlgMyMessageBox.ui
USERDEP_..\..\MedinTuxTools\DlgMyMessageBox.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\DlgMyMessageBox.ui...
InputPath=.\..\..\MedinTuxTools\DlgMyMessageBox.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgMyMessageBox.ui -o ui\DlgMyMessageBox.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgMyMessageBox.ui -i DlgMyMessageBox.h -o ui\DlgMyMessageBox.cpp \
	$(QTDIR)\bin\moc  ui\DlgMyMessageBox.h -o moc\moc_DlgMyMessageBox.cpp \

"ui\DlgMyMessageBox.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgMyMessageBox.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgMyMessageBox.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\DlgMyMessageBox.ui...
InputPath=.\..\..\MedinTuxTools\DlgMyMessageBox.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgMyMessageBox.ui -o ui\DlgMyMessageBox.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\DlgMyMessageBox.ui -i DlgMyMessageBox.h -o ui\DlgMyMessageBox.cpp \
	$(QTDIR)\bin\moc  ui\DlgMyMessageBox.h -o moc\moc_DlgMyMessageBox.cpp \

"ui\DlgMyMessageBox.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgMyMessageBox.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgMyMessageBox.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Form_Organiseur.ui
USERDEP_..\..\MedinTuxTools\Form_Organiseur.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\Form_Organiseur.ui...
InputPath=.\..\..\MedinTuxTools\Form_Organiseur.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Form_Organiseur.ui -o ui\Form_Organiseur.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Form_Organiseur.ui -i Form_Organiseur.h -o ui\Form_Organiseur.cpp \
	$(QTDIR)\bin\moc  ui\Form_Organiseur.h -o moc\moc_Form_Organiseur.cpp \

"ui\Form_Organiseur.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Form_Organiseur.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Form_Organiseur.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\Form_Organiseur.ui...
InputPath=.\..\..\MedinTuxTools\Form_Organiseur.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Form_Organiseur.ui -o ui\Form_Organiseur.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Form_Organiseur.ui -i Form_Organiseur.h -o ui\Form_Organiseur.cpp \
	$(QTDIR)\bin\moc  ui\Form_Organiseur.h -o moc\moc_Form_Organiseur.cpp \

"ui\Form_Organiseur.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Form_Organiseur.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Form_Organiseur.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_GestionATCD.ui
USERDEP_Dlg_GestionATCD.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_GestionATCD.ui...
InputPath=.\Dlg_GestionATCD.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_GestionATCD.ui -o ui\Dlg_GestionATCD.h \
	$(QTDIR)\bin\uic Dlg_GestionATCD.ui -i Dlg_GestionATCD.h -o ui\Dlg_GestionATCD.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_GestionATCD.h -o moc\moc_Dlg_GestionATCD.cpp \

"ui\Dlg_GestionATCD.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_GestionATCD.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_GestionATCD.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_GestionATCD.ui...
InputPath=.\Dlg_GestionATCD.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_GestionATCD.ui -o ui\Dlg_GestionATCD.h \
	$(QTDIR)\bin\uic Dlg_GestionATCD.ui -i Dlg_GestionATCD.h -o ui\Dlg_GestionATCD.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_GestionATCD.h -o moc\moc_Dlg_GestionATCD.cpp \

"ui\Dlg_GestionATCD.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_GestionATCD.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_GestionATCD.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=DlgPrint.ui
USERDEP_DlgPrint.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing DlgPrint.ui...
InputPath=.\DlgPrint.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgPrint.ui -o ui\DlgPrint.h \
	$(QTDIR)\bin\uic DlgPrint.ui -i DlgPrint.h -o ui\DlgPrint.cpp \
	$(QTDIR)\bin\moc  ui\DlgPrint.h -o moc\moc_DlgPrint.cpp \

"ui\DlgPrint.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgPrint.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgPrint.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing DlgPrint.ui...
InputPath=.\DlgPrint.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgPrint.ui -o ui\DlgPrint.h \
	$(QTDIR)\bin\uic DlgPrint.ui -i DlgPrint.h -o ui\DlgPrint.cpp \
	$(QTDIR)\bin\moc  ui\DlgPrint.h -o moc\moc_DlgPrint.cpp \

"ui\DlgPrint.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgPrint.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgPrint.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=DlgGetText.ui
USERDEP_DlgGetText.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing DlgGetText.ui...
InputPath=.\DlgGetText.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgGetText.ui -o ui\DlgGetText.h \
	$(QTDIR)\bin\uic DlgGetText.ui -i DlgGetText.h -o ui\DlgGetText.cpp \
	$(QTDIR)\bin\moc  ui\DlgGetText.h -o moc\moc_DlgGetText.cpp \

"ui\DlgGetText.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgGetText.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgGetText.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing DlgGetText.ui...
InputPath=.\DlgGetText.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgGetText.ui -o ui\DlgGetText.h \
	$(QTDIR)\bin\uic DlgGetText.ui -i DlgGetText.h -o ui\DlgGetText.cpp \
	$(QTDIR)\bin\moc  ui\DlgGetText.h -o moc\moc_DlgGetText.cpp \

"ui\DlgGetText.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgGetText.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgGetText.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=DlgChoixUser.ui
USERDEP_DlgChoixUser.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing DlgChoixUser.ui...
InputPath=.\DlgChoixUser.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgChoixUser.ui -o ui\DlgChoixUser.h \
	$(QTDIR)\bin\uic DlgChoixUser.ui -i DlgChoixUser.h -o ui\DlgChoixUser.cpp \
	$(QTDIR)\bin\moc  ui\DlgChoixUser.h -o moc\moc_DlgChoixUser.cpp \

"ui\DlgChoixUser.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgChoixUser.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgChoixUser.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing DlgChoixUser.ui...
InputPath=.\DlgChoixUser.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgChoixUser.ui -o ui\DlgChoixUser.h \
	$(QTDIR)\bin\uic DlgChoixUser.ui -i DlgChoixUser.h -o ui\DlgChoixUser.cpp \
	$(QTDIR)\bin\moc  ui\DlgChoixUser.h -o moc\moc_DlgChoixUser.cpp \

"ui\DlgChoixUser.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgChoixUser.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgChoixUser.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=DlgDateNom.ui
USERDEP_DlgDateNom.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing DlgDateNom.ui...
InputPath=.\DlgDateNom.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgDateNom.ui -o ui\DlgDateNom.h \
	$(QTDIR)\bin\uic DlgDateNom.ui -i DlgDateNom.h -o ui\DlgDateNom.cpp \
	$(QTDIR)\bin\moc  ui\DlgDateNom.h -o moc\moc_DlgDateNom.cpp \

"ui\DlgDateNom.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgDateNom.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgDateNom.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing DlgDateNom.ui...
InputPath=.\DlgDateNom.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgDateNom.ui -o ui\DlgDateNom.h \
	$(QTDIR)\bin\uic DlgDateNom.ui -i DlgDateNom.h -o ui\DlgDateNom.cpp \
	$(QTDIR)\bin\moc  ui\DlgDateNom.h -o moc\moc_DlgDateNom.cpp \

"ui\DlgDateNom.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgDateNom.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgDateNom.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_PosoManager.ui
USERDEP_Dlg_PosoManager.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_PosoManager.ui...
InputPath=.\Dlg_PosoManager.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_PosoManager.ui -o ui\Dlg_PosoManager.h \
	$(QTDIR)\bin\uic Dlg_PosoManager.ui -i Dlg_PosoManager.h -o ui\Dlg_PosoManager.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_PosoManager.h -o moc\moc_Dlg_PosoManager.cpp \

"ui\Dlg_PosoManager.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_PosoManager.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_PosoManager.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_PosoManager.ui...
InputPath=.\Dlg_PosoManager.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_PosoManager.ui -o ui\Dlg_PosoManager.h \
	$(QTDIR)\bin\uic Dlg_PosoManager.ui -i Dlg_PosoManager.h -o ui\Dlg_PosoManager.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_PosoManager.h -o moc\moc_Dlg_PosoManager.cpp \

"ui\Dlg_PosoManager.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_PosoManager.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_PosoManager.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_MedicaTux.ui
USERDEP_Dlg_MedicaTux.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_MedicaTux.ui...
InputPath=.\Dlg_MedicaTux.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_MedicaTux.ui -o ui\Dlg_MedicaTux.h \
	$(QTDIR)\bin\uic Dlg_MedicaTux.ui -i Dlg_MedicaTux.h -o ui\Dlg_MedicaTux.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_MedicaTux.h -o moc\moc_Dlg_MedicaTux.cpp \

"ui\Dlg_MedicaTux.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_MedicaTux.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_MedicaTux.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_MedicaTux.ui...
InputPath=.\Dlg_MedicaTux.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_MedicaTux.ui -o ui\Dlg_MedicaTux.h \
	$(QTDIR)\bin\uic Dlg_MedicaTux.ui -i Dlg_MedicaTux.h -o ui\Dlg_MedicaTux.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_MedicaTux.h -o moc\moc_Dlg_MedicaTux.cpp \

"ui\Dlg_MedicaTux.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_MedicaTux.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_MedicaTux.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_MedicaAdjustPoso.ui
USERDEP_Dlg_MedicaAdjustPoso.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_MedicaAdjustPoso.ui...
InputPath=.\Dlg_MedicaAdjustPoso.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_MedicaAdjustPoso.ui -o ui\Dlg_MedicaAdjustPoso.h \
	$(QTDIR)\bin\uic Dlg_MedicaAdjustPoso.ui -i Dlg_MedicaAdjustPoso.h -o ui\Dlg_MedicaAdjustPoso.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_MedicaAdjustPoso.h -o moc\moc_Dlg_MedicaAdjustPoso.cpp \

"ui\Dlg_MedicaAdjustPoso.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_MedicaAdjustPoso.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_MedicaAdjustPoso.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_MedicaAdjustPoso.ui...
InputPath=.\Dlg_MedicaAdjustPoso.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_MedicaAdjustPoso.ui -o ui\Dlg_MedicaAdjustPoso.h \
	$(QTDIR)\bin\uic Dlg_MedicaAdjustPoso.ui -i Dlg_MedicaAdjustPoso.h -o ui\Dlg_MedicaAdjustPoso.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_MedicaAdjustPoso.h -o moc\moc_Dlg_MedicaAdjustPoso.cpp \

"ui\Dlg_MedicaAdjustPoso.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_MedicaAdjustPoso.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_MedicaAdjustPoso.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\MedinTuxTools\Dlg_PasswordGet.ui
USERDEP_..\..\MedinTuxTools\Dlg_PasswordGet.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\Dlg_PasswordGet.ui...
InputPath=.\..\..\MedinTuxTools\Dlg_PasswordGet.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Dlg_PasswordGet.ui -o ui\Dlg_PasswordGet.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Dlg_PasswordGet.ui -i Dlg_PasswordGet.h -o ui\Dlg_PasswordGet.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_PasswordGet.h -o moc\moc_Dlg_PasswordGet.cpp \

"ui\Dlg_PasswordGet.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_PasswordGet.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_PasswordGet.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing ..\..\MedinTuxTools\Dlg_PasswordGet.ui...
InputPath=.\..\..\MedinTuxTools\Dlg_PasswordGet.ui

BuildCmds= \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Dlg_PasswordGet.ui -o ui\Dlg_PasswordGet.h \
	$(QTDIR)\bin\uic ..\..\MedinTuxTools\Dlg_PasswordGet.ui -i Dlg_PasswordGet.h -o ui\Dlg_PasswordGet.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_PasswordGet.h -o moc\moc_Dlg_PasswordGet.cpp \

"ui\Dlg_PasswordGet.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_PasswordGet.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_PasswordGet.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_HtmlView.ui
USERDEP_Dlg_HtmlView.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_HtmlView.ui...
InputPath=.\Dlg_HtmlView.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_HtmlView.ui -o ui\Dlg_HtmlView.h \
	$(QTDIR)\bin\uic Dlg_HtmlView.ui -i Dlg_HtmlView.h -o ui\Dlg_HtmlView.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_HtmlView.h -o moc\moc_Dlg_HtmlView.cpp \

"ui\Dlg_HtmlView.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_HtmlView.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_HtmlView.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_HtmlView.ui...
InputPath=.\Dlg_HtmlView.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_HtmlView.ui -o ui\Dlg_HtmlView.h \
	$(QTDIR)\bin\uic Dlg_HtmlView.ui -i Dlg_HtmlView.h -o ui\Dlg_HtmlView.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_HtmlView.h -o moc\moc_Dlg_HtmlView.cpp \

"ui\Dlg_HtmlView.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_HtmlView.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_HtmlView.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_AdvancedTerrain.ui
USERDEP_Dlg_AdvancedTerrain.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_AdvancedTerrain.ui...
InputPath=.\Dlg_AdvancedTerrain.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_AdvancedTerrain.ui -o ui\Dlg_AdvancedTerrain.h \
	$(QTDIR)\bin\uic Dlg_AdvancedTerrain.ui -i Dlg_AdvancedTerrain.h -o ui\Dlg_AdvancedTerrain.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_AdvancedTerrain.h -o moc\moc_Dlg_AdvancedTerrain.cpp \

"ui\Dlg_AdvancedTerrain.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_AdvancedTerrain.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_AdvancedTerrain.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_AdvancedTerrain.ui...
InputPath=.\Dlg_AdvancedTerrain.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_AdvancedTerrain.ui -o ui\Dlg_AdvancedTerrain.h \
	$(QTDIR)\bin\uic Dlg_AdvancedTerrain.ui -i Dlg_AdvancedTerrain.h -o ui\Dlg_AdvancedTerrain.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_AdvancedTerrain.h -o moc\moc_Dlg_AdvancedTerrain.cpp \

"ui\Dlg_AdvancedTerrain.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_AdvancedTerrain.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_AdvancedTerrain.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=FormConfig.ui
USERDEP_FormConfig.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing FormConfig.ui...
InputPath=.\FormConfig.ui

BuildCmds= \
	$(QTDIR)\bin\uic FormConfig.ui -o ui\FormConfig.h \
	$(QTDIR)\bin\uic FormConfig.ui -i FormConfig.h -o ui\FormConfig.cpp \
	$(QTDIR)\bin\moc  ui\FormConfig.h -o moc\moc_FormConfig.cpp \

"ui\FormConfig.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\FormConfig.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_FormConfig.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing FormConfig.ui...
InputPath=.\FormConfig.ui

BuildCmds= \
	$(QTDIR)\bin\uic FormConfig.ui -o ui\FormConfig.h \
	$(QTDIR)\bin\uic FormConfig.ui -i FormConfig.h -o ui\FormConfig.cpp \
	$(QTDIR)\bin\moc  ui\FormConfig.h -o moc\moc_FormConfig.cpp \

"ui\FormConfig.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\FormConfig.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_FormConfig.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_ImageProperty.ui
USERDEP_Dlg_ImageProperty.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_ImageProperty.ui...
InputPath=.\Dlg_ImageProperty.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_ImageProperty.ui -o ui\Dlg_ImageProperty.h \
	$(QTDIR)\bin\uic Dlg_ImageProperty.ui -i Dlg_ImageProperty.h -o ui\Dlg_ImageProperty.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_ImageProperty.h -o moc\moc_Dlg_ImageProperty.cpp \

"ui\Dlg_ImageProperty.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_ImageProperty.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_ImageProperty.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_ImageProperty.ui...
InputPath=.\Dlg_ImageProperty.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_ImageProperty.ui -o ui\Dlg_ImageProperty.h \
	$(QTDIR)\bin\uic Dlg_ImageProperty.ui -i Dlg_ImageProperty.h -o ui\Dlg_ImageProperty.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_ImageProperty.h -o moc\moc_Dlg_ImageProperty.cpp \

"ui\Dlg_ImageProperty.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_ImageProperty.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_ImageProperty.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=DlgAtcd_txt.ui
USERDEP_DlgAtcd_txt.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing DlgAtcd_txt.ui...
InputPath=.\DlgAtcd_txt.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgAtcd_txt.ui -o ui\DlgAtcd_txt.h \
	$(QTDIR)\bin\uic DlgAtcd_txt.ui -i DlgAtcd_txt.h -o ui\DlgAtcd_txt.cpp \
	$(QTDIR)\bin\moc  ui\DlgAtcd_txt.h -o moc\moc_DlgAtcd_txt.cpp \

"ui\DlgAtcd_txt.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgAtcd_txt.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgAtcd_txt.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing DlgAtcd_txt.ui...
InputPath=.\DlgAtcd_txt.ui

BuildCmds= \
	$(QTDIR)\bin\uic DlgAtcd_txt.ui -o ui\DlgAtcd_txt.h \
	$(QTDIR)\bin\uic DlgAtcd_txt.ui -i DlgAtcd_txt.h -o ui\DlgAtcd_txt.cpp \
	$(QTDIR)\bin\moc  ui\DlgAtcd_txt.h -o moc\moc_DlgAtcd_txt.cpp \

"ui\DlgAtcd_txt.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\DlgAtcd_txt.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_DlgAtcd_txt.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dock_Menu.ui
USERDEP_Dock_Menu.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dock_Menu.ui...
InputPath=.\Dock_Menu.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dock_Menu.ui -o ui\Dock_Menu.h \
	$(QTDIR)\bin\uic Dock_Menu.ui -i Dock_Menu.h -o ui\Dock_Menu.cpp \
	$(QTDIR)\bin\moc  ui\Dock_Menu.h -o moc\moc_Dock_Menu.cpp \

"ui\Dock_Menu.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dock_Menu.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dock_Menu.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dock_Menu.ui...
InputPath=.\Dock_Menu.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dock_Menu.ui -o ui\Dock_Menu.h \
	$(QTDIR)\bin\uic Dock_Menu.ui -i Dock_Menu.h -o ui\Dock_Menu.cpp \
	$(QTDIR)\bin\moc  ui\Dock_Menu.h -o moc\moc_Dock_Menu.cpp \

"ui\Dock_Menu.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dock_Menu.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dock_Menu.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_VigieFiltreConf.ui
USERDEP_Dlg_VigieFiltreConf.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_VigieFiltreConf.ui...
InputPath=.\Dlg_VigieFiltreConf.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_VigieFiltreConf.ui -o ui\Dlg_VigieFiltreConf.h \
	$(QTDIR)\bin\uic Dlg_VigieFiltreConf.ui -i Dlg_VigieFiltreConf.h -o ui\Dlg_VigieFiltreConf.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_VigieFiltreConf.h -o moc\moc_Dlg_VigieFiltreConf.cpp \

"ui\Dlg_VigieFiltreConf.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_VigieFiltreConf.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_VigieFiltreConf.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_VigieFiltreConf.ui...
InputPath=.\Dlg_VigieFiltreConf.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_VigieFiltreConf.ui -o ui\Dlg_VigieFiltreConf.h \
	$(QTDIR)\bin\uic Dlg_VigieFiltreConf.ui -i Dlg_VigieFiltreConf.h -o ui\Dlg_VigieFiltreConf.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_VigieFiltreConf.h -o moc\moc_Dlg_VigieFiltreConf.cpp \

"ui\Dlg_VigieFiltreConf.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_VigieFiltreConf.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_VigieFiltreConf.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_Pendule.ui
USERDEP_Dlg_Pendule.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_Pendule.ui...
InputPath=.\Dlg_Pendule.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_Pendule.ui -o ui\Dlg_Pendule.h \
	$(QTDIR)\bin\uic Dlg_Pendule.ui -i Dlg_Pendule.h -o ui\Dlg_Pendule.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_Pendule.h -o moc\moc_Dlg_Pendule.cpp \

"ui\Dlg_Pendule.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_Pendule.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_Pendule.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_Pendule.ui...
InputPath=.\Dlg_Pendule.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_Pendule.ui -o ui\Dlg_Pendule.h \
	$(QTDIR)\bin\uic Dlg_Pendule.ui -i Dlg_Pendule.h -o ui\Dlg_Pendule.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_Pendule.h -o moc\moc_Dlg_Pendule.cpp \

"ui\Dlg_Pendule.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_Pendule.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_Pendule.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Dlg_FamilleGenre.ui
USERDEP_Dlg_FamilleGenre.ui="$(QTDIR)\bin\moc.exe" "$(QTDIR)\bin\uic.exe"

!IF  "$(CFG)" == " - Win32 Release"

# Begin Custom Build - Uic'ing Dlg_FamilleGenre.ui...
InputPath=.\Dlg_FamilleGenre.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_FamilleGenre.ui -o ui\Dlg_FamilleGenre.h \
	$(QTDIR)\bin\uic Dlg_FamilleGenre.ui -i Dlg_FamilleGenre.h -o ui\Dlg_FamilleGenre.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_FamilleGenre.h -o moc\moc_Dlg_FamilleGenre.cpp \

"ui\Dlg_FamilleGenre.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_FamilleGenre.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_FamilleGenre.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ELSEIF  "$(CFG)" == " - Win32 Debug"

# Begin Custom Build - Uic'ing Dlg_FamilleGenre.ui...
InputPath=.\Dlg_FamilleGenre.ui

BuildCmds= \
	$(QTDIR)\bin\uic Dlg_FamilleGenre.ui -o ui\Dlg_FamilleGenre.h \
	$(QTDIR)\bin\uic Dlg_FamilleGenre.ui -i Dlg_FamilleGenre.h -o ui\Dlg_FamilleGenre.cpp \
	$(QTDIR)\bin\moc  ui\Dlg_FamilleGenre.h -o moc\moc_Dlg_FamilleGenre.cpp \

"ui\Dlg_FamilleGenre.h" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"ui\Dlg_FamilleGenre.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

"moc\moc_Dlg_FamilleGenre.cpp" : "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	$(BuildCmds)

# End Custom Build

!ENDIF 

# End Source File

# End Group

# Begin Group "Translations"
# Prop Default_Filter "ts"
# Begin Source File

SOURCE=drtux_fr.ts

# End Source File
# Begin Source File

SOURCE=drtux_fi.ts

# End Source File
# Begin Source File

SOURCE=drtux_us.ts

# End Source File
# Begin Source File

SOURCE=drtux_no.ts

# End Source File
# Begin Source File

SOURCE=drtux_se.ts

# End Source File

# End Group





# Begin Group "Generated"
# Begin Source File

SOURCE=moc\moc_qtsingleapplication.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_MUI_App.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_drtux.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CApp.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_DlgListFieldMngr.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_FormGlossaire.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_MyEditText.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CMoteur_Base.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_FormOngletRubrique.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CPrinter.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_MyButton.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CBigShrinker.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CMyQtable.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_KeyPressEater.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_Organiseur.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CW_EditText.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_MyDateTable.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_Horloge.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CMaskedLineEdit.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CCoolPopup.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_cdlg_passwordget.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_ThemePopup.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CMDI_Terrain.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CMDI_Generic.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CMDI_Ident.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CMDI_ChoixPatient.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CMDI_Observation.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CMDI_Prescription.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_RubIdentite.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_Dlg_ImageProperty.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_Atcd.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_CPrtQListViewItem.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_Dlg_GestionATCD.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_Dlg_VigieFiltreConf.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_Dlg_Pendule.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_Dlg_ChoixUser.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_ListViewATCDManager.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_Dlg_FamilleGenre.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_C_RubObservation.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_RubTerrain.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_RubIdentite.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_RubChoixPatient.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_RubPrescription.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dock_TabRubrique.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_FormGlossaire.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DlgListFieldMngr.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_PermsUserChoice.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DLG_InsertList.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DlgGetString.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_Calendar.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DlgListOff_Listes.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DlgMyMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Form_Organiseur.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_GestionATCD.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DlgPrint.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DlgGetText.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DlgChoixUser.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DlgDateNom.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_PosoManager.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_MedicaTux.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_MedicaAdjustPoso.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_PasswordGet.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_HtmlView.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_AdvancedTerrain.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_FormConfig.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_ImageProperty.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_DlgAtcd_txt.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dock_Menu.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_VigieFiltreConf.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_Pendule.cpp
# End Source File
# Begin Source File

SOURCE=moc\moc_Dlg_FamilleGenre.cpp
# End Source File

# Begin Source File

SOURCE=ui\RubTerrain.cpp
# End Source File
# Begin Source File

SOURCE=ui\RubIdentite.cpp
# End Source File
# Begin Source File

SOURCE=ui\RubChoixPatient.cpp
# End Source File
# Begin Source File

SOURCE=ui\RubPrescription.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dock_TabRubrique.cpp
# End Source File
# Begin Source File

SOURCE=ui\FormGlossaire.cpp
# End Source File
# Begin Source File

SOURCE=ui\DlgListFieldMngr.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_PermsUserChoice.cpp
# End Source File
# Begin Source File

SOURCE=ui\DLG_InsertList.cpp
# End Source File
# Begin Source File

SOURCE=ui\DlgGetString.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_Calendar.cpp
# End Source File
# Begin Source File

SOURCE=ui\DlgListOff_Listes.cpp
# End Source File
# Begin Source File

SOURCE=ui\DlgMyMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=ui\Form_Organiseur.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_GestionATCD.cpp
# End Source File
# Begin Source File

SOURCE=ui\DlgPrint.cpp
# End Source File
# Begin Source File

SOURCE=ui\DlgGetText.cpp
# End Source File
# Begin Source File

SOURCE=ui\DlgChoixUser.cpp
# End Source File
# Begin Source File

SOURCE=ui\DlgDateNom.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_PosoManager.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_MedicaTux.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_MedicaAdjustPoso.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_PasswordGet.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_HtmlView.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_AdvancedTerrain.cpp
# End Source File
# Begin Source File

SOURCE=ui\FormConfig.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_ImageProperty.cpp
# End Source File
# Begin Source File

SOURCE=ui\DlgAtcd_txt.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dock_Menu.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_VigieFiltreConf.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_Pendule.cpp
# End Source File
# Begin Source File

SOURCE=ui\Dlg_FamilleGenre.cpp
# End Source File

# Begin Source File

SOURCE=ui\RubTerrain.h
# End Source File
# Begin Source File

SOURCE=ui\RubIdentite.h
# End Source File
# Begin Source File

SOURCE=ui\RubChoixPatient.h
# End Source File
# Begin Source File

SOURCE=ui\RubPrescription.h
# End Source File
# Begin Source File

SOURCE=ui\Dock_TabRubrique.h
# End Source File
# Begin Source File

SOURCE=ui\FormGlossaire.h
# End Source File
# Begin Source File

SOURCE=ui\DlgListFieldMngr.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_PermsUserChoice.h
# End Source File
# Begin Source File

SOURCE=ui\DLG_InsertList.h
# End Source File
# Begin Source File

SOURCE=ui\DlgGetString.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_Calendar.h
# End Source File
# Begin Source File

SOURCE=ui\DlgListOff_Listes.h
# End Source File
# Begin Source File

SOURCE=ui\DlgMyMessageBox.h
# End Source File
# Begin Source File

SOURCE=ui\Form_Organiseur.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_GestionATCD.h
# End Source File
# Begin Source File

SOURCE=ui\DlgPrint.h
# End Source File
# Begin Source File

SOURCE=ui\DlgGetText.h
# End Source File
# Begin Source File

SOURCE=ui\DlgChoixUser.h
# End Source File
# Begin Source File

SOURCE=ui\DlgDateNom.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_PosoManager.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_MedicaTux.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_MedicaAdjustPoso.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_PasswordGet.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_HtmlView.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_AdvancedTerrain.h
# End Source File
# Begin Source File

SOURCE=ui\FormConfig.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_ImageProperty.h
# End Source File
# Begin Source File

SOURCE=ui\DlgAtcd_txt.h
# End Source File
# Begin Source File

SOURCE=ui\Dock_Menu.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_VigieFiltreConf.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_Pendule.h
# End Source File
# Begin Source File

SOURCE=ui\Dlg_FamilleGenre.h
# End Source File




# Prop Default_Filter "moc"
# End Group
# End Target
# End Project

