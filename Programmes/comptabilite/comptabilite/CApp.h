/********************************** CApp.h ****************************************
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                     *
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
#ifndef CAPP_H
#define CAPP_H

#include <qapplication.h>
#include <qstring.h>
#include "../../MedinTuxTools/CGestIni.h"
#include "Listes.h"

#include "ComptaMoteurBase.h"
#include "ui/FormSelectionUser.h"
#include "FirstUse.h"
#include <qfileinfo.h>
#include <qtextcodec.h>
#include <qdir.h>

class ComptaMoteurBase;
class Utilisateurs;

//............................................. quelques définitions ..........................................................

/*! \brief Logiciel de comptabilité pour MedinTux .
 */

//================================================= CApp ==============================================================
class CApp : public QApplication , public CGestIni
{
   Q_OBJECT

public:

    CApp(int & argc, char ** argv);
    ~CApp();


    bool getIni();
    int  GotoDebug();


public:

  QString m_PathAppli;                /*!< Chemin absolu vers l'executable de cette application */
  QString m_PathIni;                  /*!< Chemin absolu vers le fichier ini de comptabilite */
  QString m_Param;                    /*!< Chemin absolu vers l'executable de cette application */
  QString m_FirstTime;                /*!< Param�tre du fichier Ini : premi�re utilisation de l'application. */
  QString m_PathSauvegarde;            /*!< Chemin absolu vers le r�pertoire des sauvegardes de l'application */

  QString m_AppliName;                /*!< Nom de cette application */
  QString m_LastError;                /*!< Message d'erreur � l'initialisation */

  QString m_db_Driver;                /*!< Paramètres de la base de données de comptabilité : driver */
  QString m_db_Base;                  /*!< Paramètres de la base de données de comptabilité : base de donnee*/
  QString m_db_User;                  /*!< Paramètres de la base de données de comptabilité : Utilisateur */
  QString m_db_Pass;                  /*!< Paramètres de la base de données de comptabilité : Password */
  QString m_db_Host;                  /*!< Paramètres de la base de données de comptabilité : hostname */
  QString m_db_Port;                  /*!< Paramètres de la base de données de comptabilité : port */

  QString m_tuxdb_User;               /*!< Param�tres de la base de donn�es de DrTux : Utilisateur */
  QString m_tuxdb_Pass;               /*!< Param�tres de la base de donn�es de DrTux : Password */
  QString m_tuxdb_Host;               /*!< Param�tres de la base de donn�es de DrTux : Serveur */
  QString m_tuxdb_Port;               /*!< Param�tres de la base de donn�es de DrTux : Serveur */
  QString m_tuxdb_Driver;             /*!< Param�tres de la base de donn�es de DrTux : Driver */
  QString m_tuxdb_Name;               /*!< Param�tres de la base de donn�es de DrTux : Nom de la base */


  Utilisateurs*             m_currentUser;               /*!< Enregistrement complet de l'utilisateur identifi� */
  int                       m_UsrPk;                     /*!< contient l'id de l'utilisateur courant */

  int                       m_NbParam;                   /*!< Nombre de param�tres pass�s en ligne de commande � l'application */

  UTILISATEURS_LISTE*       m_UsersListe;                /*!< Liste des utilisateurs de l'application de comptabilit� */

  ACTES_LISTE*              m_pActesListe;               /*!< Liste des actes disponibles dans la base toujours � jour. */

  ComptaMoteurBase*         m_pComptaMoteurBase;         /*!< Pointeur vers les fonctions de la base de donn�es comptabilit� */

  QStringList               m_args;                      /*!< StringList des arguments pass�s en ligne de commande */
  bool                      m_Verbose;                   /*!< Passe en mode d�buggage en ligne de commande (verbose) */

};

extern CApp* G_pCApp;


#endif            // #define CPLUGAPP_H
