/***************************************************************************
 *   Copyright (C) 2005 by Sevin Roland                                    *
 *   ro@vaio_ro                                                            *
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
#ifndef CCheckListThread_H
#define CCheckListThread_H

#include <qsqldatabase.h> 
#include <qthread.h> 
#include <qstring.h>


#define TEST_SEP      1
#define TEST_NAME     5
#define TEST_RESULT   7
#define TEST_UNIT     8
#define TEST_MINMAX   9

//================================================= CListPatientItem ==================================================================
class CPatientItem
{public:
   CPatientItem(const QString &nom, const QString &prenom, const QString &id, const QString &date,  const QString &heure, QString nb)
   : m_nom(nom), m_prenom(prenom), m_ID(id), m_Date(date), m_Heure(heure), m_nbOBX(nb)
   {
   };
   
   QString m_nom;
   QString m_prenom;
   QString m_ID;
   QString m_Date;
   QString m_Heure;
   QString m_nbOBX;
};
//..................... Liste de la classe definie au dessus .........................................................
typedef QPtrList<CPatientItem> CPatientItemList;

//================================================= CCheckListThread ==============================================================
class CCheckListThread : public QThread 
{

public:
    //.................... METHODES (Constructeur destructeur)..........
     CCheckListThread(QSqlDatabase *pQSqlDatabase,
                      CPatientItemList *pItemList, const QString &dateDeb , const QString &dateFin , const QString &nom, const QString &service,
                      const QStringList listResultToCheck);
     CCheckListThread(const QString &driver, const QString &base, const QString &user, const QString &password, const QString &hostname,                           
	             CPatientItemList *pItemList, const QString &dateDeb , const QString &dateFin , 
		     const QString &nom, const QString &service, const QStringList listResultToCheck);     
    ~CCheckListThread();
    //.................... METHODES ....................................
    virtual void   run()
                      {m_Fini = FALSE;
	               checkList();	
		       m_Fini = TRUE;
                      };
    void     stop()    {m_Fini = TRUE;}
    bool     IsFini()  {return m_Fini;}
    bool     IsReady() {return m_Ready;}
private:
    //.................... METHODES ....................................
    void               checkList();
    int                GetNbResultForThisPatient (const QString &id_patient, QStringList listResultToCheck);
    QString            Get_Result_testType(QString &segment_line , int posToRetrieve);
    QSqlDatabase     * BaseConnect(  const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                     const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                     const char* user,          // = "root"
                                     const char* pasword,       // = ""
                                     const char* hostname,      // = "localhost"
                                     QString*    errMess=0);
    //.................... DATA ....................................
    QSqlDatabase     *m_DataBase;
    CPatientItemList *m_pItemList;
    QString           m_nom;
    QString           m_DateDeb;
    QString           m_DateFin;
    QString           m_Service;
    QStringList       m_listResultToCheck;
    bool              m_Fini;
    bool              m_Ready;
};

#endif
