 /********************************* FirstUse.h ************************************
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
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
/*! \file FirstUse.h
    \brief (put some short descripion here).

     #include "FirstUse.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007, 2008
     by Eric Maeker , eric.maeker@free.fr
*/

#ifndef FIRSTUSE_H
#define FIRSTUSE_H

class QObject;
class QMessageBox;
class QTextStream;
class QFile;
class QFileDialog;
class QDir;
class QPixmap;
class QPushButton;
class QLabel;
class QLineEdit;
class QWidgetStack;
class QListBox;
class QListBoxItem;
class QImage;


class QBuffer;

class QSqlCursor;
class QSqlrecord;
class QSqlQatabase;

#include "ui/FormFirstUse.h"
#include "ui/Dlg_LoginPass_Medintux.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "Listes.h"
#include "CApp.h"
#include "Defines.h"
#include "icones.h"


/**
	@author Eric Maeker <eric.maeker@free.fr>
*/
class FirstUse :  public FormFirstUse, public CGestIni
{
Q_OBJECT

public:
    FirstUse(QString params, QString pathIni, bool verboseMode = FALSE, QObject *parent = 0, const char *name = 0);

    ~FirstUse();

private:
        void    createBase(QSqlDatabase* dataBase);
        void    getMedinTux(ComptaMoteurBase* pMedinTuxBase);


private slots:
        void    getConnection();
        void    on_testIniTux_clicked();
        void    on_test_DDB_clicked();
        void    on_but_Quitter_clicked();
        void    on_chooseManagerDir_clicked();
        void    on_usersList_doubleClicked(QListBoxItem* item);
        void    on_but_Valider_clicked();

private:
    ComptaMoteurBase* 	m_pComptaMoteurBase;
    UTILISATEURS_LISTE 	m_Utilisateurs_Liste_Tux;

    QString 	m_db_Usr;
    QString 	m_db_Pass;
    QString 	m_db_Host;
    QString     m_db_Port;
    QString 	m_db_Driver;
    QString 	m_db_Label;
    QString 	m_managerIniPath;

    QString 	m_ddbTux_Driver;
    QString 	m_ddbTux_Base;
    QString 	m_ddbTux_User;
    QString 	m_ddbTux_Pass;
    QString 	m_ddbTux_Host;
    QString     m_ddbTux_Port;

    QString     m_Params;
    QString     m_PathIni;
    bool        m_verbose;
    QString     m_etape1, m_etape2, m_etape3;

    int     m_drtuxLinkedId;
    bool    m_connectionBase;		/*!< bool qui contient l'état de vérification des informations concernant
					     la connection à la base de données de la comptabilité */
    bool    m_paramUtilisateur;		/*!< bool qui contient l'état de vérification des informations concernant
						l'utilisateur principal */
    bool    m_connectionDrTux;		/*!< bool qui contient l'état de vérification des informations concernant
					     la connection à la base de données de DrTux */
};

#endif
