/********************************* main.cpp ***************************************
 *                                                                                *
 *                                                                                *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/
#include <qapplication.h>
#include <qobject.h>
#include <qmessagebox.h>
#include <qtranslator.h>
#include <qtextcodec.h>

#include "Global.h"
#include "CApp.h"
#include "drtux.h"

/**********************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : jeu sept 24 23:24:00 CEST 2003
    copyright            : (C) 2003 by Sevin Roland
    email                : upsis@club-internet.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
// DataSrcODBC-MySQL-DrTux

//=============================================== DEFINE =============================================================

#define TR QObject::tr

/*! \mainpage
 * MedinTux est une suite logicielle complète pour la gestion du dossier médical. Toutes les informations concernant cette suite se trouvent sur son site internet : http://www.medintux.org/.

 * <center><b>Description du fonctionnement de base de l'applicatif Docteur Tux (gestionnaire des dossiers médicaux)</b></center>
 * <b>Initilisation de l'application : </b> L'application débute par l'instanciation de la classe CApp. Cette classe est la classe mère de l'application. Un pointeur global est défini afin de pouvoir accéder à toutes les fonctions de cette classe et de ses sous-classe de tout endroit du code.\n
 * CApp instancie DrTux classe mère de gestion de l'interface et de gestion du dossier médical.\n

 * <b>Vues des documents :</b> Les classes CMDI_XXXXX sont les classes des " rubriques ". Une CMDI par type de rubrique : CMDI_Observation, CMDI Prescription, CMDI_Document, CMDI_Terrain, CMDI_ChoixPatient, CMDI_Ident. La classe CMDI_Generic est la structure de base de toutes les CMDI qui en dérivent.

 * <b>L'interpréteur de script</b> de DrTux est CDevilCrucible. Appelé lors de la création de nouveaux documents, ou d'ajout de bloc de texte (html, rtf ou txt). Les résultats de l'interprétation sont directement insérés à la place de l'instruction de script.

 * <b>Les éléments de thèmatisation :</b> sont encapsulés dans la classe Theme dans le répertoire MedinTuxTools. Cette classe est entièrement statique ce qui permet de l'appeler depuis n'importe quel partie du code sans instancier une nouvelle fois la classe. Les icones et couleurs sont disponibles dans cette classe.

 * <b>Les paramètres d'initialisation (fichiers ini) :</b> son gérés par CGestIni dont le fonctionnement est documenté.

 * <b>MyEditText :</b> est la réimplémentation de la classe d'éditeur de texte. Elle donne accès à la gestion des listes multi-niveaux.
*/




//=============================================== IMPLEMENTATION =====================================================

//---------------------------------- Main -----------------------------------------------------------------------------
int main( int argc, char ** argv )
{   int ret          = 0;
    QString app_name = "DrTux";
    if (argc>=7) app_name += app_name + " " + argv[3];
    //......................................... Instance de l'application ................................
    CApp a(app_name, argc, argv);
    QTranslator translator (0);
    translator.load (QString("drtux_") + QTextCodec::locale(), a.m_PathAppli + "/lang");
    a.installTranslator(&translator);

    if (G_pCApp==0)
       {if (a.m_LastError.length()) // si une erreur detectée (autre sortie avec G_pCApp==0 si instance unique)
           {QMessageBox::critical( 0,     QString(PROG_NAME) + TR (" Erreur :"),
                                          a.m_LastError ,
                                          TR("&Annuler"),0, 0,
                                          1, 1 ) ;
           }
        return 0;
       }

    new DrTux();
    if (a.m_pDrTux==0) return -1;


    a.setMainWidget(a.m_pDrTux);
    a.connect(a.m_pDrTux, SIGNAL(Sign_QuitterDrTux()),             &a,         SLOT(quit()) );
    a.connect( &a,        SIGNAL(lastWindowClosed()),              a.m_pDrTux, SLOT(OnQuitterDrTux()) );
    a.connect( &a,        SIGNAL(messageReceived(const QString&)), a.m_pDrTux, SLOT(SlotMakeVisible(const QString&)));

    //................ Lancer le bouzin ...................................................
    try
      {ret =  a.exec();
      }
    catch(...)
      {qDebug( "Planté !!!" );
       a.m_pDrTux->OnDrTuxSaveRubList();
       delete  a.m_pDrTux;
       ret = -1;
      }
    if (a.m_pDrTux)       delete  a.m_pDrTux;         a.m_pDrTux = 0;
    return ret;
}



