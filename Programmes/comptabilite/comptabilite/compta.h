/***************************************************************************
 *   Copyright (C) 2007, 2008 by Eric,,,   *
 *   eric@Nunux   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef COMPTA_H
#define COMPTA_H

#include <qmainwindow.h>
#include <qobject.h>
#include <qworkspace.h>
#include <qaction.h>
#include <qpushbutton.h>
#include <qsizegrip.h>
#include <qfontdatabase.h>
#include <qlineedit.h>
#include <qcolor.h>
#include <qcolordialog.h>
#include <qdir.h>
#include <qsimplerichtext.h>
#include <qsplitter.h>
#include <qfile.h>
#include <qvbox.h>
#include <qlayout.h>
#include <qtabwidget.h>
#include <qworkspace.h>
#include <qlabel.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtextedit.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <qaccel.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>
#include <qdatatable.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qspinbox.h>


#include "Listes.h"
#include "CApp.h"
#include "../../MedinTuxTools/CGestIni.h"


#include "CMDI_Generic.h"
#include "CMDI_Actes.h"
#include "CMDI_Releve.h"
#include "CMDI_Honoraires.h"
#include "CMDI_Mouvements.h"
#include "CMDI_Depots.h"
#include "CMDI_Daf.h"
#include "CMDI_Cmu.h"
#include "CMDI_Param.h"
#include "CMDI_Livres.h"


#include "Depots.h"
#include "icones.h"

#include "ui/FormListMenu.h"
#include "ui/filtre_hono.h"
#include "ui/a_propos.h"



class QTextEdit;
class Hono_View;

class CMDI_Generic;
class CMDI_Mouvements;
class CMDI_Honoraires;
class CMDI_Depots;
class CMDI_DAF;
class CMDI_CMU;
class CMDI_Actes;
class CMDI_Param;
class CMDI_Livres;
class CMDI_Releve;


/*! \todo Traduction de l'interface Printer Setup */
class Compta: public QMainWindow, public CGestIni
{
    Q_OBJECT

public:
    Compta(ComptaMoteurBase* pComptaMoteurBase);
    ~Compta();

		//////////////////////////////////////////
		//		DONNEES			//
		//////////////////////////////////////////

protected:

    void closeEvent( QCloseEvent* );
    void setupEditActions();
    void setupTextActions();       // cree la barre d'outils et menu lié aux attributs texte (gras italique etc..)
    void setupFileActions();
    
    CMDI_Generic*	IsExistRubrique(int type);
    CMDI_Honoraires*	CMDI_HonoCreate ();
    CMDI_Mouvements*	CMDI_MouvementsCreate ();
    CMDI_Actes* 	CMDI_ActesCreate ();
    CMDI_DAF*	 	CMDI_DAFCreate ();
	CMDI_CMU*	 	CMDI_CMUCreate ();
    CMDI_Param*		CMDI_ParamCreate ();
    CMDI_Depots*	CMDI_DepotsCreate ();
    CMDI_Releve*	CMDI_ReleveCreate ();
    CMDI_Livres*	CMDI_LivresCreate ();


protected slots:
    // SLOTS du menu
    void slot_Aide();
    // SLOTS utilisés par FormListMenu
    void honorairesView();
    void actesView();
    void banqueView();
    void mouvementsView();
    void DAFView();
    void CMUView();
    void paramView();
    void releveView();
    void livresView();
    void OnOngletRubriquesChanged(QWidget * pCMDI_Qwidget);
    void OnActiverRubrique(const char* rubName);

    void emitComptesChange();
    void emitDepotsChange();
    void emitHonoChange();
    void emitMouvementsChange();

signals:
    void  Sign_ActiverRubrique(const char*);
    void  depChange();
    void  mvtChange();
    void  honChange();
    void  cptChange();

private slots:
/*
    void newDoc();
    void choose();
    void load( const QString &fileName );
    void save();
    void saveAs();
    void print();
*/
    void about();

public:
    Filtre_Hono         *m_pFiltreHono;		/*!< Dock du filtre des honoraires */

private:
    QPrinter *printer;
    QString filename;

    ComptaMoteurBase    *m_pComptaMoteurBase;
    QWorkspace          *m_pQWorkSpaceRub;	/*!< workspace CMDI contenant les rubriques */
    FormListMenu        *m_pFormListMenu;
    QString              m_ImagePath;
    QString              m_OldUser;
    QString              m_PluginRun;

    Utilisateurs*	m_currentUser;

};
void GlobalActiverWidget(QWidget *pQWidget);


#endif
