/***************************************************************************
 *   Copyright (C) 2004 by Sébastien SIVERA & Sevin Roland                 *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
 *                                                                         *
 * *********************************************************************** *
 *  Portions of this code are derivated from PIP                           *
 *   and Copyright (C) René BEDDOK                                         *
 *   http://membres.lycos.fr/pradeau/PMSI/outils/CCAM/CCAM_acceuil.htm     *
 * ********************** Many Thanks to René BEDDOK ********************* *
 *     auteur de PIP logiciel de codage CCAM sous licence GPL              *
 *     auquel CCAM_View fait de larges emprunts dans ce fichier source     *
 *     notamment au niveau de la structure de la base de données           *
 *     et des requêtes SQL permettant de l'exploiter                       *
 * *********************************************************************** *
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
//=============================================== INCLUDES ===========================================================
#include <q3sqlcursor.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qfile.h>
#include <qregexp.h>
#include <qmessagebox.h>
#include <qobject.h>
//Added by qt3to4:
#include <QSqlQuery>
#include <QSqlError>
#include <stdlib.h>

#include "Global.h"
#include "CMoteurCCAM_Base.h"
#include "LivreFerme.xpm"   // image pour le répertoire fermé dans les listviews
//#include "LivreOuvert.xpm"  // image pour le répertoire ouvert dans les listviews

#define TR QObject::tr

//=============================================== IMPLEMENTATION =====================================================
//-----------------------------------------------------  CMoteurCCAM_Base --------------------------------------------
CMoteurCCAM_Base::CMoteurCCAM_Base()
{ m_pUser_Base = 0;
  m_pCCAM_Base = 0;
}

//-----------------------------------------------------  CMoteurCCAM_Base --------------------------------------------
CMoteurCCAM_Base::CMoteurCCAM_Base(const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                   const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                   const char* user,          // = "root"
                                   const char* password,      // = ""
                                   const char* hostname,
                                   QString*    errMess,
                                   int*        errNum)
{
  m_DriverName  = driver;
  m_BaseName    = DataBaseName;
  m_UserName    = user;
  m_PassWord    = password;
  m_HostName    = hostname;
  m_pCCAM_Base  = 0;

  //................ configurer la classe .............................................
  m_pCCAM_Base  = BaseConnect (driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                               DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                               m_obj_CCAM_Base,
                               user,          // = "root"
                               password,      // = ""
                               hostname,      // = "localhost"
                               BASE_CCAM,
                               errMess,
                               errNum);

}

//-----------------------------------------------------  ConnectUserBase --------------------------------------------
QSqlDatabase *CMoteurCCAM_Base::ConnectUserBase(const char* driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                                const char* DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                                const char* user,          // = "root"
                                                const char* password,      // = ""
                                                const char* hostname,
                                                QString*    errMess,
                                                int*        errNum)
{

  m_pUser_Base   = BaseConnect(driver,        // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                               DataBaseName,  // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                               m_obj_User_Base,
                               user,          // = "root"
                               password,      // = ""
                               hostname,      // = "localhost"
                               BASE_USER,
                               errMess,
                               errNum);
  return m_pUser_Base;
}

//-----------------------------------------------------  ~CMoteurCCAM_Base -------------------------------------------
CMoteurCCAM_Base::~CMoteurCCAM_Base()
{if (m_pCCAM_Base)
    {if (m_pCCAM_Base->isOpen()) m_pCCAM_Base->close();
     QSqlDatabase::removeDatabase (BASE_CCAM);
     //delete m_pCCAM_Base;
     //m_pCCAM_Base = 0;
    }
 if (m_pUser_Base)
    {if (m_pUser_Base->isOpen()) m_pUser_Base->close();
     QSqlDatabase::removeDatabase (BASE_USER);
     //delete m_pCCAM_Base;
     //m_pCCAM_Base = 0;
    }
}

//-----------------------------------------------------  BaseConnect -------------------------------------------
QSqlDatabase*  CMoteurCCAM_Base::BaseConnect(const char* driver,            // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                             const char* DataBaseName,      // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                             QSqlDatabase &r_QSqlDatabase,  // reference sur la database �  initialiser
                                             const char* user,              // = "root"
                                             const char* pasword,           // = ""
                                             const char* hostname,          // = "localhost"
                                             const char* baseName,
                                             QString*    errMess,
                                             int*        errNum)
    {   QSqlDatabase::removeDatabase (baseName);
        r_QSqlDatabase = QSqlDatabase::addDatabase( driver , baseName);
        r_QSqlDatabase.setDatabaseName( DataBaseName );
        r_QSqlDatabase.setUserName( user );
        r_QSqlDatabase.setPassword( pasword );
        r_QSqlDatabase.setHostName( hostname);
        if ( ! r_QSqlDatabase.open() )
           {QString qstr = "";
            int valInt = r_QSqlDatabase.lastError().number();
            if (errNum) *errNum = valInt;
            qstr += TR("Failed to open database : ") + QString(driver) + "  " + QString(DataBaseName) + "\r\n" +
                    r_QSqlDatabase.lastError().driverText() + "\r\n" + r_QSqlDatabase.lastError().databaseText();
            qWarning(qstr);
            if (errMess) *errMess = qstr;
            return 0;
           }
        return &r_QSqlDatabase;
    }

//-----------------------------------------------------  SetConfBase -------------------------------------------
int CMoteurCCAM_Base::SetConfBase(const char* confFile, QString *errMess)
{ QString baseCfg;
  QString  err = "";
  CGestIni::Param_UpdateFromDisk(confFile, baseCfg);
  if ( baseCfg.length() == 0 )
     { err = QString(TR("Le Fichier de configuration des bases :\n")) + confFile + TR("\nn'a pas été trouvé ou est vide");
      if (errMess) *errMess = err;
      qDebug(err);
      return 0;
     }
  char    *deb = (char*) (const char*) baseCfg;
  char     *pt = (char*) (const char*) baseCfg;
  int     line = 0;

  //................. USER_IDENT_TBL_NAME ......................................................................................
  pt = SetConfBase_SetProperties(deb,  m_USER_IDENT_TBL_NAME,      "m_USER_IDENT_TBL_NAME",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_TYPE_TABLE,    "m_USER_IDENT_TYPE_TABLE",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NOM,           "m_USER_IDENT_NOM",              &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_PRENOM,        "m_USER_IDENT_PRENOM",           &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_LOGIN,         "m_USER_IDENT_LOGIN",            &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_PASSWORD,      "m_USER_IDENT_PASSWORD",         &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_NUM_ORDRE,     "m_USER_IDENT_NUM_ORDRE",        &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_PRIMKEY,       "m_USER_IDENT_PRIMKEY",          &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_SIGEMS_LOG,    "m_USER_IDENT_SIGEMS_LOG",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_SIGEMS_PAS,    "m_USER_IDENT_SIGEMS_PAS",       &line , err); if (err.length())     goto SetConfBase_Error;
  pt = SetConfBase_SetProperties(pt,   m_USER_IDENT_CODE_SPEC,     "m_USER_IDENT_CODE_SPEC",        &line , err); if (err.length())     goto SetConfBase_Error;

  return 1;

SetConfBase_Error:
  if (errMess) *errMess = err;
  qDebug(err);
  return 0;
}

//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *CMoteurCCAM_Base::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
{int nLine = *line;
 err  = "";
 long len_token = strlen (token);
 while(*pt)
    {SKIP_BLANK_CAR(pt);
     if (strncmp(pt, token, len_token)==0)
        {//return pt+len_token;
         pt         += len_token;
         err         = "";
         propertie   = "";
         //............. chercher premier delimiteur ...............................
          while(*pt && *pt!='\r' && *pt !='\n' && *pt !='\'' ) pt++;
          if (*pt=='\'')         // si delimiteur pas trouve erreur
             {pt++;               // passer le delimiteur
              char *end = pt;
              //............. chercher deuxième delimiteur ...............................
              while(*end && *end!='\r' && *end !='\n' && *end !='\'')end++;
              if (*end=='\'')
                 {propertie.setLatin1(pt, end-pt);
                  if (line) *line = nLine;
                  return end +1;
                 }
              else
                {err  = TR("Syntax Error : SetConfBaseGetValue()\r\n End delimiter \"'\"  not found at line # ");
                 err += QString::number(nLine);
                 if (line) *line = nLine;
                 return 0;
                }
             }
           else
          {err  = TR("Syntax Error : SetConfBaseGetValue()\r\n Start delimiter \"'\"  not found at line # ");
              err += QString::number(nLine);
              if (line) *line = nLine;
              return 0;
             }
        } //endif (strncmp(pt, token, len_token)==0)

     NEXT_LINE(pt);
     ++nLine;

    }
 err  = TR("Syntax Error : SetConfBase()\r\n");
 err += token;
 err +=TR(" non trouvé �  la ligne N° : ");
 err += QString::number(nLine);
 if (line) *line = nLine;
 return pt;
}

//-----------------------------------------------------  GetCCAM_Version -------------------------------------------
QString CMoteurCCAM_Base::GetCCAM_Version()
{   // ......... Recherche des phases et des activités de l'acte sélectionné .......................
    QSqlQuery query("SELECT CHRONOLOGIE FROM VERSION;", m_obj_CCAM_Base);
    QString version = "?";
    while ( query.next() ) version = query.value(0).toString();
    char *pt_deb = (char*)(const char*) version;
    char *pt     = pt_deb;
    while (pt && *pt && *pt =='0')  {++pt;};
    int v = version.mid(pt-pt_deb).toInt()-1;
    return (QString::number(v));
}

//-----------------------------------------------------  GetActesParMotsClef -------------------------------------------
long CMoteurCCAM_Base::GetActesParMotsClef(Q3ListView* pQListView,
                                           const QString &mot_cle_saisie1,
                                           const QString &mot_cle_saisie2 ,
                                           const QString &codeCCAM,
                                           const QString &activite,
                                           int listMustBeCleared /*=1*/)
{
    long               nb = 0;
    int       lenMotClef1 = mot_cle_saisie1.length();
    int       lenMotClef2 = mot_cle_saisie2.length();
    QString second_filter = "";
    //.............................. methode QSqlQuery ...........................
    ///*
    QString requete ( "SELECT CODE,LIBELLELONG,PLACEARBORESCENCE FROM ACTES WHERE " ) ;
    int ok = 1;
    if (listMustBeCleared==2)
       {if (lenMotClef1>=3 || lenMotClef2>=3) pQListView->clear();
       }
    else if (listMustBeCleared)                                     pQListView->clear();

    if (mot_cle_saisie1 != "" && mot_cle_saisie2 != "")
       {if (lenMotClef1>=2 ||lenMotClef2 >= 2)
           {requete       +=  "    (   LIBELLELONG LIKE '%" + mot_cle_saisie1          + "%'  ";
            requete       +=  "     OR LIBELLELONG LIKE '%" + mot_cle_saisie1.toUtf8() + "%') ";
            requete       +=  "AND (   LIBELLELONG LIKE '%" + mot_cle_saisie2          + "%'  ";
            requete       +=  "     OR LIBELLELONG LIKE '%" + mot_cle_saisie2.toUtf8() + "%') ";
            //second_filter  = mot_cle_saisie2;
           }
        else ok=0;
       }
    else if (mot_cle_saisie1 != "")
       {if (lenMotClef1>=3)
           {requete       +=  "(LIBELLELONG LIKE '%" + mot_cle_saisie1 + "%' OR LIBELLELONG LIKE '%" + mot_cle_saisie1.toUtf8() + "%')";
           }
        else ok=0;
       }
    else if (mot_cle_saisie2 != "")
       {if (lenMotClef2>=3)
           {requete       +=  "(LIBELLELONG LIKE '%" + mot_cle_saisie2 + "%' OR LIBELLELONG LIKE '%" + mot_cle_saisie2.toUtf8() + "%')";
           }
        else ok=0;
       }

    // on effectue la recherche que si un critère est saisi
    if (codeCCAM != "_______" && mot_cle_saisie1 == "" && mot_cle_saisie2 == "")
       {requete        = "SELECT CODE,LIBELLELONG,PLACEARBORESCENCE FROM ACTES WHERE CODE LIKE '";
        requete       +=   codeCCAM + "%';";
        ok=1;
       }
    else if (codeCCAM != "_______")
       {requete       +=  " AND CODE Like '" + codeCCAM + "%';";
        ok=1;
       }
    else if (mot_cle_saisie1 == "" && mot_cle_saisie2 == "")
       {ok=0;
       }
    if (ok==0) return 0;


    QSqlQuery query(requete, m_obj_CCAM_Base);

    while ( query.next() )
    {QString libelle = CGestIni::Utf8_Query(query, 1);
     QString code    = query.value(0).toString();
     QString arbo    = query.value(2).toString();
     //................ si activité donnée alors tester que les codes avec cette activité .........................................
     if (activite != "")
        {requete         = QString ("SELECT ACTIVITE FROM ACTIVITEACTE WHERE CODEACTE = '") + code + "' AND ACTIVITE = '" +activite+ "';";
         QSqlQuery queryActivite(requete, m_obj_CCAM_Base);
         if (queryActivite.next()) ok = 1; // ok l'activité existe pour ce code
         else                      ok = 0; // ok l'activité n'existe pas pour ce code
        }
     if (ok)
        {new Q3ListViewItem(pQListView,
                           code,
                           libelle,
                           arbo);
         nb++;
        }
    }
    //*/
 return nb;
}

//-----------------------------------------------------  initArbo01 -------------------------------------------
void CMoteurCCAM_Base::initArbo01(Q3ListView*     pQListView )
{
    QImage livre_ferme(LivreFerme_xpm);       // définition l'image
    QPixmap pix_lf;                        // définition l'image
    pix_lf.convertFromImage(livre_ferme);  // définition de l'image
    Q3ListViewItem * pQListViewItemObjet;
    QSqlQuery queryArbo(QString::null, m_obj_CCAM_Base);
    queryArbo.prepare("SELECT RANG,LIBELLE,CODEPERE,CODEMENU FROM `ARBORESCENCE` where CODEPERE=000001;");
    queryArbo.exec();
    pQListView->clear();
    while ( queryArbo.next() )
    {QString     libelle = CGestIni::Utf8_Query(queryArbo, 1);
     pQListViewItemObjet = new Q3ListViewItem(pQListView,
                                             TR("Chapitre ") + queryArbo.value(0).toString().right(2),
                                             libelle,
                                             queryArbo.value(2).toString(),
                                             queryArbo.value(3).toString(),
                                             queryArbo.value(0).toString());
        pQListViewItemObjet->setPixmap(0,pix_lf);
    }
}

//-----------------------------------------------------  initArbo02 -------------------------------------------
void CMoteurCCAM_Base::initArbo02(Q3ListViewItem*     pQListViewItem, QString codeMenu )
{    if (pQListViewItem->childCount ()>0) return;
    QImage livre_ferme(LivreFerme_xpm);       // définition l'image
    QPixmap pix_lf;                        // définition l'image
    pix_lf.convertFromImage(livre_ferme);  // définition de l'image
    Q3ListViewItem * pQListViewItemObjet;
    QSqlQuery queryArbo(QString::null, m_obj_CCAM_Base);
    queryArbo.prepare(QString("SELECT RANG,LIBELLE,CODEPERE,CODEMENU FROM ARBORESCENCE where CODEPERE=")+codeMenu+";");
    queryArbo.exec();
    while ( queryArbo.next() )
    {pQListViewItemObjet = new Q3ListViewItem(pQListViewItem,
                                             TR("Partie ") + queryArbo.value(0).toString().right(2),
                                             CGestIni::Utf8_Query(queryArbo,1),
                                             queryArbo.value(2).toString(),
                                             queryArbo.value(3).toString(),
                                             queryArbo.value(0).toString());
        pQListViewItemObjet->setPixmap(0,pix_lf);
    }
}


//-----------------------------------------------------  GotoDebug ----------------------------------------------------------------------
long CMoteurCCAM_Base::GotoDebug()
{return 1;
}


//-----------------------------------------------------  GetActesIncompatibles -----------------------------------------------------------
long CMoteurCCAM_Base::GetActesIncompatibles(Q3ListView*     pQListView,
                                             const QString &codeCCAM,
                                             const QString &date)
{

    long        nb = 0;
    pQListView->clear();
    //.............................. methode QSqlQuery ........................................
    QString requete ( "select INCOMPATIBILITE.DATEEFFET ");
    requete       +=  "from INCOMPATIBILITE inner join ACTES on INCOMPATIBILITE.INCOMPATIBLE=ACTES.CODE ";
    requete       +=  "where INCOMPATIBILITE.CODEACTE='" + codeCCAM + "' and INCOMPATIBILITE.DATEEFFET <='" + date + "';";

    QSqlQuery query(requete, m_obj_CCAM_Base);

    QString date_effet = nearDateEffet(date, query);
    QString date_aff   = date_effet.mid(6,2) + "/" + date_effet.mid(4,2) + "/" + date_effet.left(4);

    requete        =  "select INCOMPATIBILITE.INCOMPATIBLE,  INCOMPATIBILITE.DATEEFFET, ACTES.LIBELLELONG ";
    requete       +=  "from INCOMPATIBILITE inner join ACTES on INCOMPATIBILITE.INCOMPATIBLE=ACTES.CODE ";
    requete       +=  "where INCOMPATIBILITE.CODEACTE='" + codeCCAM + "' and INCOMPATIBILITE.DATEEFFET='" + date_effet + "';";

    query.exec(requete);

     while ( query.next() ) {
            new Q3ListViewItem(pQListView,
                              date_aff,                       //date d'effet
                              query.value(0).toString(),      //code
                              "",                                             //activité
                              CGestIni::Utf8_Query(query,2));     //libelle
                nb++;
        }
    return nb;
}

//-----------------------------------------------------  GetActesIncompatibles -----------------------------------------------------------
long CMoteurCCAM_Base::GetActesIncompatibles(QStringList &qstringList, const QString &codeCCAM, const QString &date)
{
     long               nb = 0;
     // ......... Recherche de toutes les dates effets inférieures �  la date de l'acte pour conserver la plus proche ....
     QString requete ( "select INCOMPATIBILITE.DATEEFFET ");
     requete       +=  "from INCOMPATIBILITE ";
     requete       +=  "where INCOMPATIBILITE.CODEACTE='" + codeCCAM + "' and INCOMPATIBILITE.DATEEFFET <='" + date + "' ORDER BY INCOMPATIBILITE.DATEEFFET;";

     QSqlQuery query(requete, m_obj_CCAM_Base);

     QString date_effet = nearDateEffet(date, query);
     QString date_aff   = date_effet.mid(6,2) + "/" + date_effet.mid(4,2) + "/" + date_effet.left(4);

     requete        =  "select INCOMPATIBILITE.INCOMPATIBLE ";
     requete       +=  "from INCOMPATIBILITE ";
     requete       +=  "where INCOMPATIBILITE.CODEACTE='" + codeCCAM + "' and INCOMPATIBILITE.DATEEFFET='" + date_effet + "';";

     query.exec(requete);

     while ( query.next() )
       { qstringList << query.value(0).toString();
         nb++;
       }
    return nb;

}


//----------------------------------------------------- GetPhasesActe ------------------------------------------------------
long CMoteurCCAM_Base::GetPhasesActe(QStringList &qstringList, const QString &codeCCAM)
{
    long    nb = 0;
    // ......... Recherche des phases et des activités de l'acte sélectionné .......................
    QString requete ( "SELECT PHASE FROM PHASEACTE WHERE CODEACTE='");
    requete      +=     codeCCAM + "' ;";
    QSqlQuery query(requete, m_obj_CCAM_Base);

    while ( query.next() )
          {qstringList<<query.value(0).toString();
           ++nb;
          }
    return nb;
}

//----------------------------------------------------- GetActiviteActe ------------------------------------------------------
long CMoteurCCAM_Base::GetActiviteActe(QStringList &qstringList, const QString &codeCCAM)
{
    long    nb = 0;
    // ......... Recherche des phases et des activités de l'acte sélectionné .......................
    QString requete ( "SELECT ACTIVITE FROM ACTIVITEACTE WHERE CODEACTE='");
    requete      +=     codeCCAM + "' ;";
    QSqlQuery query(requete, m_obj_CCAM_Base);

    while ( query.next() )
          {qstringList << query.value(0).toString();
           ++nb;
          }
    return nb;
}

//----------------------------------------------------- GetSexeActe ------------------------------------------------------
void CMoteurCCAM_Base::GetSexeActe(QString &qstring, const QString &codeCCAM)
{
    // ......... Recherche des phases et des activités de l'acte sélectionné .......................
    QString requete ( "SELECT SEXE FROM ACTES WHERE CODE='");
    requete      +=     codeCCAM + "' ;";
    QSqlQuery query(requete, m_obj_CCAM_Base);

    while ( query.next() )
          {qstring =  query.value(0).toString();
          }
}

//-----------------------------------------------------  GetPlaceArbo -----------------------------------------------------------
QString CMoteurCCAM_Base::GetPlaceArbo(const QString &codeCCAM )
{QString placeArbo("");
 QString requete ( "SELECT PLACEARBORESCENCE FROM ACTES WHERE CODE='");
 requete      +=     codeCCAM + "' ;";
 QSqlQuery query(requete, m_obj_CCAM_Base);
 while ( query.next() )
       {placeArbo =  query.value(0).toString();
       }
 return placeArbo;
}


/*
              if (p_mix&1)       result += "<a href=\"#ModifX"+QString::number(id_list)+"\"><img source=\"./PosoAdjustOn.png\"></a>&nbsp;&nbsp;";
              else               result += "<a href=\"#ModifY"+QString::number(id_list)+"\"><img source=\"./PosoAdjustOff.png\"></a>&nbsp;&nbsp;";
              if (p_mix&2)       result += "<a href=\"#ModifQ"+QString::number(id_list)+"\"><img source=\"./PosoManuelleOn.png\"></a>&nbsp;&nbsp;";
              else               result += "<a href=\"#ModifT"+QString::number(id_list)+"\"><img source=\"./PosoManuelleOff.png\"></a>&nbsp;&nbsp;";
              if (poso.Ald == 0) result += "<a href=\"#ModifN"+QString::number(id_list)+"\"><img source=\"./AldOffPicto14-13.png\"></a>&nbsp;&nbsp;";
              else               result += "<a href=\"#ModifW"+QString::number(id_list)+"\"><img source=\"./AldOnPicto14-13.png\"></a>&nbsp;&nbsp;";
              result += "<a href=\"#ModifS"+QString::number(id_list)+"\"><img source=\"./DeletePicto14-13.png\"></a>&nbsp;&nbsp;";
              result += "<a href=\"#ModifZ"+QString::number(id_list)+"\"><img source=\"./Interrogation.png\"></a>&nbsp;&nbsp;";
*/
//-----------------------------------------------------  GetHierarchieActes -------------------------------------------
long CMoteurCCAM_Base::GetHierarchieActe(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM, const QString &libelle)
{   QStringList  hierarchie;
    QString     chapitre = "";
    QString     space_cel ="<TD rowSpan=1 colspan=2 width=6 >&nbsp;</TD>";
    long               nb = 0;
    pQTextBrowser->setText("");  //GetPlaceArbo(codeCCAM)
    //.............................. methode QSqlQuery ........................................
    ///*
    QString requete ( "SELECT ARBORESCENCE1,ARBORESCENCE2,");
    requete +=        "ARBORESCENCE3,ARBORESCENCE4,";
    requete +=        "ARBORESCENCE5,ARBORESCENCE6,";
    requete +=        "ARBORESCENCE7,ARBORESCENCE8,";
    requete +=        "ARBORESCENCE9,ARBORESCENCE10 ";
    requete +=        "FROM ACTES WHERE CODE ='";
    requete +=         codeCCAM + "';";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    while ( query.next() )
    {

        QString codepere = "000001";
        for (int i=0;i<10;++i){
            if (query.value(i).toString()=="000000") break;
            requete = ( "SELECT CODEMENU,CODEPERE,RANG,LIBELLE FROM ARBORESCENCE WHERE CODEPERE='");
            requete += codepere + "' AND RANG='" + query.value(i).toString() + "';";
            QSqlQuery querycp(requete, m_obj_CCAM_Base);
            while(querycp.next()){
                codepere = querycp.value(0).toString();
                //pQTextBrowser->insert(querycp.value(3).toString() + "\n");
                if (i>0) chapitre += ".";
                chapitre += (querycp.value(2).toString()).mid(4);
                hierarchie.append(chapitre + " " + CGestIni::Utf8_Query(querycp,3));
            }
        }
        nb++;

    }
    QString      result ("<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
                         "<body style=\"font-size:10pt;font-family:Arial Narrow\">" );
     result     +="  <a href=\"#CCAM_Code" + codeCCAM + TR("\">Ajouter l'acte : " ) + codeCCAM + "</a><br>";
     result     +="  <TABLE cellSpacing=\"10\" cellpadding=\"0\" width=416  border=\"0\">"
                  "  <TBODY> ";
    int             i = 0;
    QString deb_style = "";
    QString end_style = "";
    space_cel = "";
    for ( QStringList::Iterator it = hierarchie.begin(); it != hierarchie.end(); ++it )
        { QString label = *it;
          QString chap  = "";
         int f = label.find(" ");
         if (f != -1 ) {chap     = label.left(f);
                        label    = label.mid(f+1);
                       }//table border="0" cellpadding="0" cellspacing="10" width="100%"
         if (i==0)
            {deb_style = "<u><b>";
             end_style = "</u></b>";
            }
         else if (i==1)
            {deb_style = "<b>";
             end_style = "</b>";
            }
         else
            {deb_style = "";
             end_style = "";
            }
                  result     +=
                  "    <TR>"
                  "     <TD rowSpan=1 colspan=2 width=120 >"+chap+"</TD>"           //bgcolor=\"#FF00FF\"
                  "     <TD rowSpan=1 colspan=2 width=400 >"+space_cel + deb_style + label + end_style +"</TD>" //bgcolor=\"#0000FF\"
                  "    </TR>";
         i++;
        }

          //"<a href=\"#CCAM_Code"+ codeCCAM  "\">"
          chapitre   += "." + GetPlaceArbo(codeCCAM).mid(4,2);
          result     +=
                  "    <TR>"
                  "     <TD rowSpan=1 colspan=2 width=120 >"+chapitre+"</TD>"    //bgcolor=\"#FF00FF\"
                  "     <TD rowSpan=1 colspan=2 width=400 >"  +space_cel + libelle +  "        </TD>" //bgcolor=\"#0000FF\"
                  "     <TD rowSpan=1 colspan=2 width=1  valign = \"bottom\">"+codeCCAM+"</TD>"
                  "    </TR>";
    result +=     "  </TBODY>"
                  "  </TABLE>";
    result += "</body></html>";
    pQTextBrowser->setText(result);
    return nb;
}

//-----------------------------------------------------  GetActesParArborescence -------------------------------------------
long CMoteurCCAM_Base::GetActesParArborescence(Q3ListView *pQListView, QStringList& arborescence)
{
    // ATTENTION NE PAS OUBLIER DE RAZ LES FENETRES DE RESULTAT

    long         nb = 0;
    QString requete = "SELECT CODE,LIBELLELONG,PLACEARBORESCENCE FROM ACTES WHERE ";
    // ........................... construit la requete ....................................
    int i = 1;
    for ( QStringList::Iterator it = arborescence.begin(); it != arborescence.end(); ++it )
        {if (i==1)   requete += " ARBORESCENCE";
         else        requete += " AND ARBORESCENCE";
         requete             +=  QString::number(i,10) + "='" + *it + "'";
         i++;
        }
    requete += ";";
    // ........................... execute la requete ....................................
    QSqlQuery query(requete, m_obj_CCAM_Base);
    pQListView->clear();
    while ( query.next() )
    {
        nb++;
        new Q3ListViewItem(pQListView,
                          query.value(0).toString(),   //code
                          CGestIni::Utf8_Query(query,1),
                          query.value(2).toString());
    }
    return nb;
}

//-----------------------------------------------------  GetNotes -------------------------------------------
long CMoteurCCAM_Base::GetNotes(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM)
{
    long nb=0;
    pQTextBrowser->setText("");

    //........................... DEBUT du document HTML ...........................
    QString      result ("<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
                         "<body style=\"font-size:10pt;font-family:Arial Narrow\">" );
    result     += "  <TABLE cellSpacing=\"10\" cellpadding=\"0\" width=416 align=\"left\" border=\"0\">"
                  "  <TBODY> ";
    //..............................................................................

    QString requete ( "SELECT ARBORESCENCE1,ARBORESCENCE2,");
    requete +=        "ARBORESCENCE3,ARBORESCENCE4,";
    requete +=        "ARBORESCENCE5,ARBORESCENCE6,";
    requete +=        "ARBORESCENCE7,ARBORESCENCE8,";
    requete +=        "ARBORESCENCE9,ARBORESCENCE10 ";
    requete +=        "FROM ACTES WHERE CODE ='";
    requete +=         codeCCAM + "';";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    while ( query.next() )
    {

        QString codepere = "000001";
        for (int i=0;i<10;++i){
            if (query.value(i).toString()=="000000") break;
            requete = ( "SELECT CODEMENU,CODEPERE,RANG,LIBELLE FROM ARBORESCENCE WHERE CODEPERE='");
            requete += codepere + "' AND RANG='" + query.value(i).toString() + "';";
            QSqlQuery querycp(requete, m_obj_CCAM_Base);
            while(querycp.next()){
                codepere = querycp.value(0).toString();
                //....................... insertion du libelle du chapitre ....................
                //pQTextBrowser->insert(querycp.value(2).toString() + " " + querycp.value(3).toString() + "\n");
                result     +=
                "<TR>"
                "<TD rowSpan=1 colspan=2 width=120 >" + querycp.value(2).toString() + "</TD>"
                "<TD rowSpan=1 colspan=2 width=400 >" + CGestIni::Utf8_Query(querycp,3) + "</TD>"
                "</TR>";
                //....................... retrouve les notes associées au chapitre en question .......................
                requete = ("SELECT TEXTE FROM NOTESARBORESCENCE WHERE CODEMENU='" + codepere + "' ORDER BY TYPE;");
                QSqlQuery querynotes(requete, m_obj_CCAM_Base);
                while(querynotes.next()){
                    //................... insère les notes du chapitres ............................
                    //pQTextBrowser->insert(querynotes.value(0).toString() + "\n");
                    result     +=
                    "<TR>"
                    "<TD rowSpan=1 colspan=2 width=120 ></TD>"
                    "<TD rowSpan=1 colspan=2 width=400 >" + CGestIni::Utf8_Query(querynotes,0) + "</TD>"
                    "</TR>";
                }
            }
        }
        nb++;

    }
    //........................... retrouve les notes associées �  l'acte ........................
    //pQTextBrowser->insert("\nNote de l'acte :\n");
    result     +=
    "<TR>"
    "<TD rowSpan=1 colspan=2 width=120 >Note de l'acte :</TD>"
    "</TR>";
    requete = ("SELECT TEXTE FROM NOTES WHERE CODEACTE='" + codeCCAM + "' ORDER BY TYPE;");
    QSqlQuery querynotesactes(requete, m_obj_CCAM_Base);
    while(querynotesactes.next()){
        //pQTextBrowser->insert(querynotesactes.value(0).toString() + "\n");
        result     +=
        "<TR>"
        "<TD rowSpan=1 colspan=2 width=120 > </TD>"
        "<TD rowSpan=1 colspan=2 width=400 >" + CGestIni::Utf8_Query(querynotesactes, 0) + "</TD>"
        "</TR>";
    }

    //........................... FIN du document HTML ...........................
    result +=     "  </TBODY>"
                  "  </TABLE>";
    result += "</body></html>";
    //..............................................................................
    pQTextBrowser->setText(result);
    return nb;
}

//-----------------------------------------------------  GetActesIsolesProcedures -------------------------------------------
long CMoteurCCAM_Base::GetActesIsolesProcedures(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM,const QString &date)
{

    long               nb = 0;
    QString date_effet = "";
        QString date_aff   = "";
    QString type_acte="";
    QString result("<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
                   "<body style=\"font-size:10pt;font-family:Arial Narrow\">" );
    QString titre1="";
    QString titre2="";
    QString titre3="";
    // ......... RAZ de l'affichage ...............
    pQTextBrowser->setText("");
    // ......... Recherche du TYPE de l'acte sélectionné .......................
    QString requete ( "SELECT TYPE FROM ACTES WHERE CODE='" + codeCCAM + "';");
    QSqlQuery query(requete, m_obj_CCAM_Base);
    QSqlQuery queryproc(m_obj_CCAM_Base);
    while ( query.next() ){
        switch (query.value(0).toInt()){
        case 0: // si c'est un acte isolé
            requete =  "SELECT PROCEDURES.DATEEFFET FROM PROCEDURES,ACTES ";
                        requete += "WHERE PROCEDURES.CODEACTE='" + codeCCAM + "' AND PROCEDURES.DATEEFFET <='" + date + "' AND ACTES.CODE=PROCEDURES.CODEPROCEDURE ORDER BY PROCEDURES.DATEEFFET;";
                queryproc.exec(requete);
                        date_effet = nearDateEffet(date, queryproc);
                        date_aff = date_effet.mid(6,2) + "/" + date_effet.mid(4,2) + "/" + date_effet.left(4);
            requete =  "SELECT PROCEDURES.CODEPROCEDURE, ACTES.LIBELLELONG FROM PROCEDURES,ACTES ";
                        requete += "WHERE PROCEDURES.CODEACTE='" + codeCCAM + "' AND PROCEDURES.DATEEFFET='" + date_effet + "' AND ACTES.CODE=PROCEDURES.CODEPROCEDURE;";
                        queryproc.exec(requete);
                        titre1  = TR("Acte isolé");
            titre2  = TR(" appartenant �  la procédure :");
            titre3  = TR(" appartenant aux procédures :");
            break;
        case 1: // si c'est une procédure
            requete =  "SELECT PROCEDURES.DATEEFFET FROM PROCEDURES,ACTES ";
                        requete += "WHERE PROCEDURES.CODEPROCEDURE='" + codeCCAM + "' AND PROCEDURES.DATEEFFET <='" + date + "' AND ACTES.CODE=PROCEDURES.CODEACTE ORDER BY PROCEDURES.DATEEFFET;";
                queryproc.exec(requete);
                        date_effet = nearDateEffet(date, queryproc);
                        date_aff = date_effet.mid(6,2) + "/" + date_effet.mid(4,2) + "/" + date_effet.left(4);
            requete =  "SELECT PROCEDURES.CODEACTE, ACTES.LIBELLELONG FROM PROCEDURES,ACTES ";
                        requete += "WHERE PROCEDURES.CODEPROCEDURE='" + codeCCAM + "' AND PROCEDURES.DATEEFFET='" + date_effet + "' AND ACTES.CODE=PROCEDURES.CODEACTE;";
                        queryproc.exec(requete);
            titre1  = TR("Procédure");
            titre2  = TR(" contenant l' acte :");
            titre3  = TR(" contenant les actes :");
            break;
        case 2: // si c'est un acte complémentaire
            requete =  "SELECT PROCEDURES.DATEEFFET FROM PROCEDURES,ACTES ";
                        requete += "WHERE PROCEDURES.CODEACTE='" + codeCCAM + "' AND PROCEDURES.DATEEFFET <='" + date + "' AND ACTES.CODE=PROCEDURES.CODEPROCEDURE ORDER BY PROCEDURES.DATEEFFET;";
                queryproc.exec(requete);
                        date_effet = nearDateEffet(date, queryproc);
                        date_aff = date_effet.mid(6,2) + "/" + date_effet.mid(4,2) + "/" + date_effet.left(4);
            requete =  "SELECT PROCEDURES.CODEPROCEDURE, ACTES.LIBELLELONG FROM PROCEDURES,ACTES ";
                        requete += "WHERE PROCEDURES.CODEACTE='" + codeCCAM + "' AND PROCEDURES.DATEEFFET='" + date_effet + "' AND ACTES.CODE=PROCEDURES.CODEPROCEDURE;";
                        queryproc.exec(requete);
            titre1  = TR("Acte complémentaire");
            titre2  = TR(" appartenant �  la procédure :");
            titre3  = TR(" appartenant aux procédures :");
            break;
        }

        while ( queryproc.next() ) {
            nb++;
            type_acte +="<br><a href=\"#CCAM_Code";  //result     +="<p><a href=\"#CCAM_Code"+codeCCAM+"\">Ajouter l'acte: " + codeCCAM + "</p>";
            type_acte += queryproc.value(0).toString() + "\">";
            type_acte += queryproc.value(0).toString() + "</a> : " + CGestIni::Utf8_Query(queryproc,1) +  "<br>";
        }
                result = TR("<b><u>En date du ") + date_aff + "</b></u><br>";
        if (nb>0)
           {
            if (nb>1) titre2=titre3;
            result += QString("<b><u>")  + titre1 + titre2 + "</b></u><br>"+ type_acte ;
           }
        else
           { result += QString("<b><u>") + titre1 + "</b></u><br>";
           }
    }
    result += "</body></html>";
    pQTextBrowser->setText(result);
    return nb;
}

//-----------------------------------------------------  GetActesAssocies -------------------------------------------
long CMoteurCCAM_Base::GetActesAssocies(const QString &codeCCAM, const QString &activiteActe, const QString &dateActe, CODE_ASSO_LIST &codeAssoList)
{

        // ......... RAZ de la structure ...............
        codeAssoList.clear();

        // ......... Recherche de toutes les dates effets inférieures �  la date de l'acte pour conserver la plus proche ....
        QString requete ( "SELECT ASSOCIABILITE.DATEEFFET ");
        requete       +=  "FROM ASSOCIABILITE INNER JOIN ACTES ON ACTES.CODE=ASSOCIABILITE.ACTEASSO ";
        requete       +=  "WHERE ASSOCIABILITE.CODEACTE ='"  + codeCCAM + "' ";
        requete       +=  "AND ASSOCIABILITE.ACTIVITEASSO='" + activiteActe + "' ";
        requete       +=  "AND ASSOCIABILITE.DATEEFFET <='"  + dateActe + "' ORDER BY ASSOCIABILITE.DATEEFFET;";

        QSqlQuery query(requete, m_obj_CCAM_Base);
        QString date_effet = nearDateEffet(dateActe, query);
        QString date_aff   = date_effet.mid(6,2) + "/" + date_effet.mid(4,2) + "/" + date_effet.left(4);

        // ......... Recherche des actes associés et des geste complémentaires de l'acte sélectionné .......................
        requete        =  "SELECT ASSOCIABILITE.ACTEASSO,ACTES.LIBELLELONG,ASSOCIABILITE.ACTIVITEASSO,ACTES.TYPE,ASSOCIABILITE.REGLE ";
        requete       +=  "FROM ASSOCIABILITE INNER JOIN ACTES ON ACTES.CODE=ASSOCIABILITE.ACTEASSO ";
        requete       +=  "WHERE ASSOCIABILITE.CODEACTE ='"  + codeCCAM + "' ";
        requete       +=  "AND ASSOCIABILITE.ACTIVITEASSO='" + activiteActe + "' ";
        requete       +=  "AND ASSOCIABILITE.DATEEFFET  ='"  + date_effet + "' ORDER BY ASSOCIABILITE.ACTEASSO;";

        query.exec(requete);
        while ( query.next() ){
                codeAssoList.append(CODE_ASSO_STRUCT(query.value(0).toString(),        // Code acte associé
                                                     CGestIni::Utf8_Query(query, 1),        // libellé acte associé
                                                     query.value(2).toString(),        // Code activité acte associé
                                                     query.value(3).toString(),        // type association
                                                     query.value(4).toString()));      // regle association

        }
        return codeAssoList.count();
}

//----------------------------------------------------- GetModificateurs -------------------------------------------
long CMoteurCCAM_Base::GetModificateurs(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM,const QString &date)
{
    long    nb = 0;
    QString result="";
    QString requete ( "SELECT MODIFICATEURACTE.DATEEFFET ");
    requete +=        "from MODIFICATEURACTE,MODIFICATEUR ";
    requete +=        "WHERE MODIFICATEURACTE.CODEACTE ='" + codeCCAM + "' ";
    requete +=        "AND MODIFICATEURACTE.MODIFICATEUR=MODIFICATEUR.CODE ";
    requete +=        "AND MODIFICATEURACTE.DATEEFFET <='" + date + "' ";
    requete +=        "ORDER BY MODIFICATEURACTE.DATEEFFET;";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    QString date_effet = nearDateEffet(date, query);
    QString date_aff   = date_effet.mid(6,2) + "/" + date_effet.mid(4,2) + "/" + date_effet.left(4);

    // ......... RAZ de l'affichage ...............
    pQTextBrowser->setText("");
    // ......... Recherche des modificateurs de l'acte sélectionné .......................
    requete  =        "SELECT MODIFICATEURACTE.CODEACTIVITE, MODIFICATEURACTE.MODIFICATEUR, MODIFICATEUR.LIBELLE ";
    requete +=        "from MODIFICATEURACTE,MODIFICATEUR ";
    requete +=        "WHERE MODIFICATEURACTE.CODEACTE ='" + codeCCAM + "' ";
    requete +=        "AND MODIFICATEURACTE.MODIFICATEUR=MODIFICATEUR.CODE ";
    requete +=        "AND MODIFICATEURACTE.DATEEFFET='" + date_effet + "' ";
    requete +=        "ORDER BY MODIFICATEURACTE.CODEACTIVITE;";
    query.exec(requete);

    while ( query.next() ){
        result += TR("En date du ")      + date_aff;
        result += TR(" Code acte = ")    + codeCCAM;
        result += TR(" activité : ")     + query.value(0).toString();
        result += TR(" modificateur : ") + query.value(1).toString() + " : " + CGestIni::Utf8_Query(query,2)+ "\n";
        nb++;
    }
    // ......... Affiche le resultat et retour .............
    pQTextBrowser->setText(result);
    return nb;
}

//----------------------------------------------------- GetPhases ------------------------------------------------------
long CMoteurCCAM_Base::GetPhases(Q3TextBrowser* pQTextBrowser,const QString &codeCCAM,const QString &date)
{
    long    nb = 0;
    QString activite_precedente = "";
    QString activite = "";
    QString result="";
    // ......... RAZ de l'affichage ...............
    pQTextBrowser->setText("");
    // ......... Recherche des phases et des activités de l'acte sélectionné .......................
    //QString requete ( "SELECT ACTIVITEACTE.ACTIVITE, ACTIVITE.LIBELLE,PHASEACTE.PHASE, PHASE.LIBELLE ,PHASEACTE.ICR, PHASEACTE.CLASSANT FROM ACTIVITEACTE,PHASEACTE,ACTIVITE,PHASE WHERE ACTIVITEACTE.CODEACTE='" + codeCCAM + "' AND ACTIVITEACTE.ACTIVITE=ACTIVITE.CODE AND PHASEACTE.CODEACTE ='" + codeCCAM + "' AND PHASEACTE.PHASE=PHASE.CODE AND PHASEACTE.ACTIVITE=ACTIVITEACTE.ACTIVITE ORDER BY ACTIVITEACTE.ACTIVITE, PHASEACTE.PHASE;");
    QString requete ( "SELECT ACTIVITEACTE.ACTIVITE, ACTIVITE.LIBELLE,PHASEACTE.PHASE, PHASE.LIBELLE ,PHASEACTE.ICR, PHASEACTE.CLASSANT, HISTO_PHASE.PU FROM ACTIVITEACTE,PHASEACTE,ACTIVITE,PHASE,HISTO_PHASE WHERE ACTIVITEACTE.CODEACTE='" + codeCCAM + "' AND ACTIVITEACTE.ACTIVITE=ACTIVITE.CODE AND PHASEACTE.CODEACTE ='" + codeCCAM + "' AND PHASEACTE.PHASE=PHASE.CODE AND PHASEACTE.ACTIVITE=ACTIVITEACTE.ACTIVITE AND HISTO_PHASE.CODEACTE ='" + codeCCAM + "' AND HISTO_PHASE.PHASE=PHASE.CODE AND HISTO_PHASE.ACTIVITE=ACTIVITEACTE.ACTIVITE ORDER BY ACTIVITEACTE.ACTIVITE, PHASEACTE.PHASE;");
    QSqlQuery query(requete, m_obj_CCAM_Base);

    while ( query.next() ){
        activite = query.value(0).toString();
        if (activite != activite_precedente){
            result += TR("Activité ")+ activite + " : " + CGestIni::Utf8_Query(query,1) + "\n";
        }
        result += TR("Phase ") + query.value(2).toString() + " : " + CGestIni::Utf8_Query(query,3);
        result += " - ICR : "  + query.value(4).toString() + TR(" - Classant : ") + query.value(5).toString() + TR(" - Prix unitaire : ") + query.value(6).toString() + "\n";
        activite_precedente = activite;
        nb++;
    }
    // ......... Affiche le resultat et retour .............
    pQTextBrowser->setText(result);
    return nb;
}

//----------------------------------------------------- GetModificateurActe ------------------------------------------------------
long CMoteurCCAM_Base::GetModificateurActe(QStringList &qstringList, const QString &codeCCAM, const QString &date)
{

    long    nb = 0;
    QString requete ( "SELECT DATEEFFET FROM MODIFICATEURACTE ");
    requete      +=   "WHERE CODEACTE='" + codeCCAM + "' AND DATEEFFET <='" + date + "' ORDER BY DATEEFFET;";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    QString date_effet = nearDateEffet(date, query);
    //QString date_aff   = date_effet.mid(6,2) + "/" + date_effet.mid(4,2) + "/" + date_effet.left(4);

        // ......... Recherche des modificateurs de l'acte sélectionné .......................
    requete       =   "SELECT MODIFICATEUR FROM MODIFICATEURACTE ";
    requete      +=   "WHERE CODEACTE='" + codeCCAM + "' AND DATEEFFET ='" + date_effet + "';";
    query.exec(requete);

    while ( query.next() )
          {qstringList << query.value(0).toString();
           ++nb;
          }
    return nb;
}

//------------------------------------------------------ GetLibelleModificateur ---------------------------------------------------------
QString CMoteurCCAM_Base::GetLibelleModificateur(const QString &codeMod)
{
    QSqlQuery query(QString  ( "SELECT LIBELLE FROM MODIFICATEUR WHERE CODE='" + codeMod + "';"), m_obj_CCAM_Base);
    if ( query.next() ) return CGestIni::Utf8_Query(query,0);
    return QString  ("");
}

//------------------------------------------------------ GetPrixListeActe ---------------------------------------------------------
QString CMoteurCCAM_Base::GetPrixListeActe( CCAM_ACTE *ccamActe, int nb_actes)
{int i = 0;
 while (i<nb_actes)
    {
    }
 return GetPrixActe(ccamActe[i].m_CodeCCAM,
                       ccamActe[i].m_Date,
                       ccamActe[i].m_Code_asso,
                       ccamActe[i].m_Activite,
                       ccamActe[i].m_Phase,
                       ccamActe[i].m_Modificateur1,
                       ccamActe[i].m_Modificateur2,
                       ccamActe[i].m_Modificateur3,
                       ccamActe[i].m_Modificateur4);
}
//------------------------------------------------------ GetPrixActe ---------------------------------------------------------
QString CMoteurCCAM_Base::GetPrixActe(const CCAM_ACTE &ccamActe, const QString &code_asso)
{   return GetPrixActe(ccamActe.m_CodeCCAM,
                       ccamActe.m_Date,
                       ccamActe.m_Code_asso,
                       ccamActe.m_Activite,
                       ccamActe.m_Phase,
                       ccamActe.m_Modificateur1,
                       ccamActe.m_Modificateur2,
                       ccamActe.m_Modificateur3,
                       ccamActe.m_Modificateur4 );
}

//------------------------------------------------------ GetPrixActe ---------------------------------------------------------
QString CMoteurCCAM_Base::GetPrixActe(const QString &codeCCAM,
                                      const QString &dateStr,
                                      const QString &code_asso,
                                      const QString &activite,
                                      const QString &phase,
                                      const QString &mod_1,
                                      const QString &mod_2,
                                      const QString &mod_3,
                                      const QString &mod_4)
{
    return GetPrixActe(codeCCAM,
                       QDate ( atoi(dateStr.mid(6,4)),     // 10/12/2003  --> YYYY  2003
                               atoi(dateStr.mid(3,2)),     // 10/12/2003  --> MM    12
                               atoi(dateStr.mid(0,2))      // 10/12/2003dtStr  --> DD    10
                              ),
                       code_asso,
                       activite,
                       phase,
                       mod_1,
                       mod_2,
                       mod_3,
                       mod_4);
}
//------------------------------------------------------ GetPrixActe ---------------------------------------------------------
QString CMoteurCCAM_Base::GetPrixActe(const QString &codeCCAM,
                                      const QDate   &date,
                                      const QString &code_asso,
                                      const QString &activite,
                                      const QString &phase,
                                      const QString &mod_1,
                                      const QString &mod_2,
                                      const QString &mod_3,
                                      const QString &mod_4,
                                      const QString &spec_domtom,
                                      const QString &regle)
{   //............ Table HISTO_PHASE (Prix unitaire de l'acte en fonction acte et activité �  une date effet ....
    //             Table MODIFICATEURACTE retrouve la validité codage modificateur en fonction acte activité et date effet .........
    //             Table TB03  coefficient daté pour tous les actes
    //             Table TB08  dates des jours fériés reconnus par la sécu en fonction de la caisse (caisse 00)
    //             Table TB09  compatibilité entre modificateurs, entre eux et date
    //             Table TB10  permet de savoir si un modificateur peut être codé sur tous les actes du même jour
    //             Table TB11  permet de calculer le prix en fonction des modificateurs : prix de base * coef + forfait
    //             Table TB12  idem que l'autre pour médecins non conventionnés
    //             Table TB19  Nb maxi de modificateurs par acte
    //   ggnc001


    //.............................. date en format long.............................................
    QString date_acte    = date.toString ("yyyyMMdd");

    double l_tauxDeBase  = GetPrixBaseActe(codeCCAM, activite, phase, date_acte ).toDouble() *
                           GetCoeffGene(regle, date_acte ).toDouble();                          // appliquer le coefficient general au taux de base
           l_tauxDeBase += CalcMajoModificateurs(l_tauxDeBase, mod_1, mod_2, mod_3, mod_4, date_acte );       // appliquer les modificateurs
    //.............................. chercher coefficient ..................................................
    return QString::number(l_tauxDeBase);
}

//-----------------------------------------------------  CalcMajoModificateurs -------------------------------------------
double CMoteurCCAM_Base::CalcMajoModificateurs(const double l_tauxDeBase, const QString &mod_1, const QString &mod_2, const QString &mod_3, const QString &mod_4, const QString &date_acte )
{   //.............................. coefficient et forfait des modificateurs ...............................................
    double modCoef_1, modForf_1, major_1;
    double modCoef_2, modForf_2, major_2;
    double modCoef_3, modForf_3, major_3;
    double modCoef_4, modForf_4, major_4;
    int  isModif     = 0;

    isModif += GetModifificateurCoef(mod_1, date_acte, modCoef_1, modForf_1 );  major_1 = (l_tauxDeBase*(modCoef_1-1.0)) + modForf_1;
    isModif += GetModifificateurCoef(mod_2, date_acte, modCoef_2, modForf_2 );  major_2 = (l_tauxDeBase*(modCoef_2-1.0)) + modForf_2;
    isModif += GetModifificateurCoef(mod_3, date_acte, modCoef_3, modForf_3 );  major_3 = (l_tauxDeBase*(modCoef_3-1.0)) + modForf_3;
    isModif += GetModifificateurCoef(mod_4, date_acte, modCoef_4, modForf_4 );  major_4 = (l_tauxDeBase*(modCoef_4-1.0)) + modForf_4;
    if (isModif) return  major_1  +   major_2  +  major_3  +  major_4;
    else         return 0;
}

//-----------------------------------------------------  GetCoeffGene -------------------------------------------
QString CMoteurCCAM_Base::GetCoeffGene(const QString &regle, const QString &date_acte )
{   if (regle.length()<=0) return "1";
    //.............................. coefficient général en fonction de la date.............................................
    QString coefGene    =  "";
    long l_cur_date_deb =  0;
    long l_cur_date_fin =  0;
    long l_date_acte    = date_acte.toLong();
    QString     requete  ( "SELECT DATEDEBUT, DATEFIN, COEFFICIENT  FROM TB03 where" );
                requete += " REGLE='"            + regle     + "' AND ";
                requete += " AND DATEFIN <= '"   + date_acte + "' ORDER BY DATEFIN;";

    QSqlQuery query(requete, m_obj_CCAM_Base);

    while ( query.next() )
          {l_cur_date_deb =  query.value(0).toLongLong();
           l_cur_date_fin =  query.value(1).toLongLong();
           if   (l_cur_date_fin == 0 && l_date_acte >= l_cur_date_deb)              coefGene = query.value(2).toString();
           else if (l_date_acte >= l_cur_date_deb && l_date_acte <= l_cur_date_fin) coefGene = query.value(2).toString();
          }
 return coefGene;
}

//-----------------------------------------------------  GetPrixBaseActe -------------------------------------------
QString CMoteurCCAM_Base::GetPrixBaseActe(const QString &codeCCAM, const QString &activite, const QString &phase,const QString &date_acte )
{QString requete  = "SELECT PU,DATE_EFFET FROM HISTO_PHASE WHERE"; ;
                        requete += " CODEACTE     = '"     + codeCCAM  + "' ";
 if (activite.length()) requete += " AND ACTIVITE = '"     + activite  + "' ";
 if (phase.length())    requete += " AND PHASE = '"        + phase     + "' ";
                        requete += " AND DATE_EFFET <= '"  + date_acte + "' ORDER BY DATE_EFFET;";
 QSqlQuery query(requete, m_obj_CCAM_Base);

 QString tauxDeBase  = "";
 long    l_date      = date_acte.toLong();
 long    l_date_req  = 0;
 long    l_date_mem  = 0;
 while ( query.next() )
       {l_date_req = query.value(1).toString().toLong();
        if (l_date >= l_date_req)
           {l_date_mem = l_date_req;
            tauxDeBase = query.value(0).toString();
           }
       }
 return tauxDeBase;
}

//-----------------------------------------------------  GetModifificateurCoef -------------------------------------------
int CMoteurCCAM_Base::GetModifificateurCoef(const QString &mod, QString date_acte, double &coef, double &forfait )
{   coef              = 1;
    forfait           = 0;
    long l_date_acte  = date_acte.toLong();
    if (mod.length() && mod!="0")
       {QString requete  = "SELECT DATEDEBUT, DATEFIN, COEF, FORFAIT  FROM TB11 WHERE";
                requete += " MODIFICATEUR='"     + mod       + "'";
                requete += " AND DATEFIN <= '"   + date_acte + "' ORDER BY DATEFIN;";
        QSqlQuery  query(requete, m_obj_CCAM_Base);
        if (query.next())
           {coef    = query.value(2).toDouble();
            forfait = query.value(3).toDouble();
           }
        while ( query.next() )
          {long l_cur_date_deb =  query.value(0).toLongLong();
           long l_cur_date_fin =  query.value(1).toLongLong();
           if   (l_cur_date_fin == 0 && l_date_acte >= l_cur_date_deb)
                {coef    = query.value(2).toDouble();
                 forfait = query.value(3).toDouble();
                }
           else if (l_date_acte >= l_cur_date_deb && l_date_acte <= l_cur_date_fin)
                {coef    = query.value(2).toDouble();
                 forfait = query.value(3).toDouble();
                }
          }
        return 1;
       }
 return 0;
}

//-----------------------------------------------------  GetGestesComplementaires -------------------------------------------
long CMoteurCCAM_Base::GetGestesComplementaires(Q3ListView *pQListView_AA,Q3ListView *pQListView_GC,const QString &codeCCAM,const QString &date)
{
    long               nb = 0;
    // ......... RAZ de l'affichage ...............
    pQListView_AA->clear();
    pQListView_GC->clear();

        // ......... Recherche de toutes les dates effets inférieures �  la date de l'acte pour conserver la plus proche ....
        QString requete ( "SELECT ASSOCIABILITE.DATEEFFET ");
        requete       +=  "FROM ASSOCIABILITE INNER JOIN ACTES ON ACTES.CODE=ASSOCIABILITE.ACTEASSO ";
        requete       +=  "WHERE ASSOCIABILITE.CODEACTE ='" + codeCCAM + "' AND ASSOCIABILITE.DATEEFFET <='" + date + "' ORDER BY ASSOCIABILITE.DATEEFFET;";

        QSqlQuery query(requete, m_obj_CCAM_Base);

        QString date_effet = nearDateEffet(date, query);
        QString date_aff   = date_effet.mid(6,2) + "/" + date_effet.mid(4,2) + "/" + date_effet.left(4);

        // ......... Recherche des actes associés et des geste complémentaires de l'acte sélectionné .......................

        requete        =  "SELECT ASSOCIABILITE.CODEACTE,ASSOCIABILITE.ACTIVITE,ASSOCIABILITE.ACTEASSO,ASSOCIABILITE.ACTIVITEASSO,ACTES.LIBELLELONG,ACTES.TYPE ";
        requete       +=  "FROM ASSOCIABILITE INNER JOIN ACTES ON ACTES.CODE=ASSOCIABILITE.ACTEASSO ";
        requete       +=  "WHERE ASSOCIABILITE.CODEACTE ='" + codeCCAM + "' AND ASSOCIABILITE.DATEEFFET ='" + date_effet + "';";

        query.exec(requete);

        while ( query.next() ){
                if (query.value(5).toString() != "2") { // Acte associé
                        new Q3ListViewItem(pQListView_AA,
                                                          date_aff,
                                                          query.value(1).toString(),   //activité concernée
                                                          query.value(2).toString(),   //code acte asscié
                                                          query.value(3).toString(),   //activite associée
                                                          CGestIni::Utf8_Query(query,4));  //libelle acte associé
                        } else { // query.value(5).toString() == 2 alors Gestes complémentaires
                                        new Q3ListViewItem(pQListView_GC,
                                                                          date_aff,
                                                                          query.value(1).toString(),   //activité corcernée
                                                                          query.value(2).toString(),   //code acte asscié
                                                                          query.value(3).toString(),   //activite associée
                                                                          CGestIni::Utf8_Query(query,4));  //libelle acte associé
                                        }
                nb++;
                }
                return nb;
}

//----------------------------------------------------- nearDateEffet ---------------------------------------------------
QString CMoteurCCAM_Base::nearDateEffet(const QString &date)
{
        QString ret         = QString::null;
        QString date_req    = QString::null;
        long    l_date      = date.toLong();
        long    l_date_req  = 0;
        long    l_date_mem  = 0;
    QSqlQuery query(QString::null, m_obj_CCAM_Base);
    query.prepare("SELECT DATE_EFFET FROM DATES_EFFET ORDER BY DATE_EFFET;");
    query.exec();
    while ( query.next() ) {
                date_req = query.value(0).toString();
                l_date_req = date_req.toLong();
                if (l_date >= l_date_req) {
                        l_date_mem = l_date_req;
                }
                ret = QString::number(l_date_mem);
    }
        return ret;
}

//----------------------------------------------------- nearDateEffet ---------------------------------------------------
QString CMoteurCCAM_Base::nearDateEffet(const QString &date, QSqlQuery &query)
{
        QString ret         = QString::null;
        QString date_req    = QString::null;
        long    l_date      = date.toLong();
        long    l_date_req  = 0;
        long    l_date_mem  = 0;
    while ( query.next() ) {
                date_req = query.value(0).toString();
                l_date_req = date_req.toLong();
                if (l_date >= l_date_req) {
                        l_date_mem = l_date_req;
                }
                ret = QString::number(l_date_mem);
    }
        return ret;
}

//---------------------------------------------------- CheckThesaurus -----------------------------------------------------------
long CMoteurCCAM_Base::CheckThesaurus(Q3ListView *pQListView)
{
    long               nb = 0;
    //..................................................................
    //..........si la table CATEGORIE existe la détruit.................
    //..................................................................
    /*QString requete1 ( "DROP TABLE IF EXISTS CATEGORIE;" );
    QSqlQuery query1(requete1, m_pCCAM_Base);*/
    //..................................................................
    //..........si la table CATEGORIE n'existe pas la crée..............
    //..................................................................
    /*QString requete ( "CREATE TABLE IF NOT EXISTS CATEGORIE (" );
    requete +=        "CODE char(6) NOT NULL default '',";
    requete +=        " LIBELLE char(50) default NULL,";
    requete +=        " CODEMEDECIN char(6) default NULL,";
    requete +=        " PRIMARY KEY (CODE),";
    requete +=        " KEY (CODEMEDECIN)";
    requete +=        ");";
    QSqlQuery query_categorie(requete, m_pCCAM_Base);*/
    //..................................................................
    //..........si la table THESAURUS existe la détruit.................
    //..................................................................
    /*QString requete2 ( "DROP TABLE IF EXISTS THESAURUS;" );
    QSqlQuery query2(requete2, m_pCCAM_Base);*/
    //..................................................................
    //..........si la table THESAURUS n'existe pas la crée..............
    //..................................................................
    QString requete = "CREATE TABLE IF NOT EXISTS THESAURUS (";
    requete +=        "SERIE INT(11)  NOT NULL auto_increment ,";
    requete +=        " CODEMEDECIN char(6) default NULL,";
    requete +=        " CATEGORIE char(40) default NULL,";
    requete +=        " LIBUSUEL char(50) default NULL,";
    requete +=        " CODEACTE char(13) default NULL,";
    requete +=        " PRIMARY KEY (SERIE),";
    requete +=        " KEY (CODEMEDECIN)";
    requete +=        ");";
    QSqlQuery query_thesaurus(requete, m_obj_CCAM_Base);
    OutSQL_error(query_thesaurus, "CheckThesaurus(CREATE TABLE THESAURUS)", requete );
    return nb;
}

//---------------------------------------------------- CheckBasePatient -----------------------------------------------------------
int CMoteurCCAM_Base::CheckBasePatient()
{
    int               ret  = 0;
    //..........si la table Patients n'existe pas la créer..............
    QString requete = "CREATE TABLE IF NOT EXISTS Patients (";
    requete +=        " PK INT(11) NOT NULL auto_increment ,";
    requete +=        " NOM        char(40)   default NULL,";
    requete +=        " PRENOM     char(40)   default NULL,";
    requete +=        " DATE_NSS   char(10)   default NULL,";
    requete +=        " NOM_NSS    char(40)   default NULL,";
    requete +=        " SEXE       char(1)    default NULL,";

    requete +=        " ADRESSE    char(80)   default NULL,";
    requete +=        " CODE_POST  char(5)    default NULL,";
    requete +=        " VILLE      char(40)   default NULL,";
    requete +=        " NUM_SS     char(13)   default NULL,";
    requete +=        " CLE_SS     char(2)    default NULL,";
    requete +=        " TEL        char(20)   default NULL,";
    requete +=        " IP_PAT     char(20)   default NULL,";
    requete +=        " IP_EXT     char(20)   default NULL,";

    requete +=        " PRIMARY KEY (PK),";
    requete +=        " KEY (IP_PAT)";
    requete +=        " KEY (NOM)";
    requete +=        " KEY (PRENOM)";
    requete +=        ");";
    QSqlQuery query_Patient(requete, m_obj_CCAM_Base);
    OutSQL_error(query_Patient, "CheckBasePatient(CREATE TABLE Patients)", requete );


    //..........si la table ActesPatients n'existe pas la créer..............
    requete  = "CREATE TABLE IF NOT EXISTS ActesPatients (";
    requete +=        " PK INT(11)        NOT NULL   auto_increment ,";
    requete +=        " DATE              char(10)   default NULL,";
    requete +=        " HEURE             char(8)    default NULL,";
    requete +=        " PHASE             char(1)    default NULL,";
    requete +=        " ACTIVITE          char(1)    default NULL,";
    requete +=        " MODIFICATEUR_1    char(1)    default NULL,";
    requete +=        " MODIFICATEUR_2    char(1)    default NULL,";
    requete +=        " MODIFICATEUR_3    char(1)    default NULL,";
    requete +=        " MODIFICATEUR_4    char(1)    default NULL,";
    requete +=        " CODE_ACTE         char(13)   default NULL,";
    requete +=        " PRAT_NID          char(9)    default NULL,";
    requete +=        " PRAT_SPEC         char(2)    default NULL,";
    requete +=        " REF_PATIENT       INT(11)    NOT NULL,";

    requete +=        " PRIMARY KEY (PK),";
    requete +=        " KEY (REF_PATIENT)";
    requete +=        " KEY (PRAT_NID)";
    requete +=        " KEY (DATE)";
    requete +=        ");";
    QSqlQuery query_ActesPatients(requete, m_obj_CCAM_Base);
    OutSQL_error(query_ActesPatients, "CheckBasePatient(CREATE TABLE ActesPatients)", requete );
    return ret;
}

//-------------------------------------------------- OutSQL_error -----------------------------------------------
void CMoteurCCAM_Base::OutSQL_error(QSqlQuery &cur, const char *messFunc /*=0*/, const char *requete /*=0*/)
{QSqlError error = cur.lastError();
 QString   qserr("");
 int ret = error.type();
     if (error.type() != QSqlError::None)
        {switch (error.type())
           { case    QSqlError::None:           qserr =" - no error occurred: "          ; break;
             case    QSqlError::Connection:     qserr =" - connection error: "           ; break;
             case    QSqlError::Statement:      qserr =" - SQL statement syntax error: " ; break;
             case    QSqlError::Transaction:    qserr =" - transaction failed error: "   ; break;
             case    QSqlError::Unknown:        qserr =" - unknown error: "              ; break;
             default:                           qserr =" - unknown error: "              ; break;
           }
         if (messFunc) qDebug(messFunc);
         qDebug(qserr + error.databaseText ());
         if (requete) qDebug(requete);
        }
//     qDebug(qserr + error.databaseText ());
}

//---------------------------------------------------- TableMedecins -----------------------------------------------------------
int CMoteurCCAM_Base::CreeTableMedecins()
{
    int ok=0;
    //..................................................................
    //..........si la table MEDECINS n'existe pas la crée...............
    //..................................................................
    QString requete ( "CREATE TABLE IF NOT EXISTS MEDECINS (" );
    requete +=        "CODE char(6) NOT NULL default '',";
    requete +=        " PWD char(32) default NULL,";
    requete +=        " NOM char(30) default NULL,";
    requete +=        " PRENOM char(30) default NULL,";
    requete +=        " NID char(9) default NULL,";
    requete +=        " SPEC char(2) default NULL,";
    requete +=        " PRIMARY KEY (CODE)";
    requete +=        ");";
    QSqlQuery query_medecins(requete, m_obj_CCAM_Base);
    return ok;
}

//---------------------------------------------------- TableMedecins -----------------------------------------------------------
int CMoteurCCAM_Base::DropTablePraticiens()
{
    int ok=0;
    QString requete ( "DROP TABLE IF EXISTS MEDECINS;" );
    QSqlQuery query(requete, m_obj_CCAM_Base);
    return ok;
}

//---------------------------------------------------- GetMedecins -----------------------------------------------------------
long CMoteurCCAM_Base::GetMedecins(Q3ListView *pQListView)
{   //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return 0;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)  return 0;

    long nb = 0;
    pQListView->clear();
    // ......... Recherche des medecins .......................
    QString requete    ("SELECT ");

            requete += m_USER_IDENT_SIGEMS_LOG + "," +
                       m_USER_IDENT_SIGEMS_PAS + "," +
                       m_USER_IDENT_NOM        + "," +
                       m_USER_IDENT_PRENOM     + "," +
                       m_USER_IDENT_NUM_ORDRE  + "," +
                       m_USER_IDENT_CODE_SPEC  + "," +
                       m_USER_IDENT_PRIMKEY    + " FROM " +
                       m_USER_IDENT_TBL_NAME;

        // "CODE,PWD,NOM,PRENOM,NID,SPEC FROM MEDECINS;");
    QSqlQuery query(requete, m_obj_User_Base);

    while ( query.next() ) {
                new Q3ListViewItem(pQListView,
                                  query.value(0).toString(),   //code
                                  query.value(1).toString(),   //mot de passe
                                  query.value(2).toString(),   //nom
                                  query.value(3).toString(),   //prenom
                                  query.value(4).toString(),   //nid
                                  query.value(5).toString(),   //spec
                                  query.value(6).toString());  //Pk
        nb++;
    }
    if (nb==0) {
        new Q3ListViewItem(pQListView,TR("La table est vide"));
    }
    return nb;
}

//---------------------------------------------------- MedecinExisteByPrimKey -----------------------------------------------------------
bool CMoteurCCAM_Base::MedecinExisteByPrimKey(const QString &pk)
{   //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return FALSE;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)          return FALSE;

    bool exist = FALSE;
    long nb = 0;
    QString requete    ("SELECT ");
    requete += m_USER_IDENT_NUM_ORDRE  + " FROM " +
               m_USER_IDENT_TBL_NAME   + " WHERE "+ m_USER_IDENT_PRIMKEY + "='" + pk + "';";

    QSqlQuery query(requete, m_obj_User_Base);
    while ( query.next() ) {
        nb++;
    }
    if (nb != 0){
        exist = TRUE;
    }
    return exist;
}

//---------------------------------------------------- MedecinExisteByLoginMedinTux -----------------------------------------------------------
bool CMoteurCCAM_Base::MedecinExisteByLoginMedinTux(const QString &loginMedinTux)
{   //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return FALSE;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)          return FALSE;

    bool exist = FALSE;
    long nb = 0;
    QString requete    ("SELECT ");
    requete += m_USER_IDENT_NUM_ORDRE  + " FROM " +
               m_USER_IDENT_TBL_NAME   + " WHERE "+ m_USER_IDENT_LOGIN + "='" + loginMedinTux + "';";

    QSqlQuery query(requete, m_obj_User_Base);
    while ( query.next() ) {
        nb++;
    }
    if (nb != 0){
        exist = TRUE;
    }
    return exist;
}

//---------------------------------------------------- MedecinExiste -----------------------------------------------------------
bool CMoteurCCAM_Base::MedecinExiste(const QString &codeNID)
{   //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return FALSE;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)  return FALSE;

    bool exist = FALSE;
    long nb = 0;
    QString requete    ("SELECT ");
    requete += m_USER_IDENT_NUM_ORDRE  + " FROM " +
               m_USER_IDENT_TBL_NAME   + " WHERE "+ m_USER_IDENT_NUM_ORDRE + "='" + codeNID + "';";

    QSqlQuery query(requete, m_obj_User_Base);
    while ( query.next() ) {
        nb++;
    }
    if (nb != 0){
        exist = TRUE;
    }
    return exist;
}

//---------------------------------------------------- GetInfoMedecinsFromNid -----------------------------------------------------------
void CMoteurCCAM_Base::GetInfoMedecinsFromNid(const QString &nid, INFO_MED &info_med)
{   //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)          return ;
    QString requete ("SELECT ");
            requete += m_USER_IDENT_SIGEMS_LOG + "," +        // 0
                       m_USER_IDENT_SIGEMS_PAS + "," +        // 1
                       m_USER_IDENT_LOGIN      + "," +        // 2
                       m_USER_IDENT_PASSWORD   + "," +        // 3
                       m_USER_IDENT_NOM        + "," +        // 4
                       m_USER_IDENT_PRENOM     + "," +        // 5
                       m_USER_IDENT_NUM_ORDRE  + "," +        // 6
                       m_USER_IDENT_CODE_SPEC  + "," +        // 7
                       m_USER_IDENT_PRIMKEY    + " FROM  " +  // 8
                       m_USER_IDENT_TBL_NAME   + " WHERE " ;
           requete +=  m_USER_IDENT_NUM_ORDRE  + "='" + nid + "';";
    // QString requete ("SELECT CODE,PWD,NOM,PRENOM,NID,SPEC FROM MEDECINS WHERE NID='" + nid + "';");

    QSqlQuery query(requete, m_obj_User_Base);
    while ( query.next() )
        {    info_med.m_Code               = query.value(0).toString();
              info_med.m_Password          = query.value(1).toString();
              info_med.m_CodeMedinTux      = query.value(2).toString();
              info_med.m_PasswordMedinTux  = query.value(3).toString();

         info_med.m_Nom               = query.value(4).toString();
         info_med.m_Prenom            = query.value(5).toString();
         info_med.m_Nid               = query.value(6).toString();
         info_med.m_Spec              = query.value(7).toString();
         info_med.m_Pk                = query.value(8).toString();
        }
   /*
   QString requete ("SELECT ");
            requete += m_USER_IDENT_SIGEMS_LOG + "," +        // 0
                       m_USER_IDENT_SIGEMS_PAS + "," ;        // 1
    if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
       {    requete += m_USER_IDENT_SIGEMS_LOG + "," +        // 2
                       m_USER_IDENT_SIGEMS_PAS + "," ;        // 3
       }
    else if (m_USER_IDENT_TYPE_TABLE=="MedinTux")
       {   requete +=  m_USER_IDENT_LOGIN      + "," +        // 2
                       m_USER_IDENT_PASSWORD   + "," ;        // 3
       }
           requete +=  m_USER_IDENT_NOM        + "," +        // 4
                       m_USER_IDENT_PRENOM     + "," +        // 5
                       m_USER_IDENT_NUM_ORDRE  + "," +        // 6
                       m_USER_IDENT_CODE_SPEC  + " FROM  " +  // 7
                       m_USER_IDENT_TBL_NAME   + " WHERE " ;
           requete +=  m_USER_IDENT_NUM_ORDRE  + "='" + nid + "';";
    // QString requete ("SELECT CODE,PWD,NOM,PRENOM,NID,SPEC FROM MEDECINS WHERE NID='" + nid + "';");

    QSqlQuery query(requete, m_pUser_Base);
    while ( query.next() )
        {if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
            { info_med.m_Code              = query.value(0).toString();
              info_med.m_Password          = query.value(1).toString();
            }
         else
            { info_med.m_CodeMedinTux      = query.value(2).toString();
              info_med.m_PasswordMedinTux  = query.value(3).toString();
              info_med.m_Code              = query.value(2).toString();
              info_med.m_Password          = query.value(3).toString();
            }
         info_med.m_Nom               = query.value(4).toString();
         info_med.m_Prenom            = query.value(5).toString();
         info_med.m_Nid               = query.value(6).toString();
         info_med.m_Spec              = query.value(7).toString();
        }
    */
}


//---------------------------------------------------- GetInfoMedecinsFromCode -----------------------------------------------------------
void CMoteurCCAM_Base::GetInfoMedecinsFromCode(const QString &codeMEDECIN, INFO_MED &info_med)
{   if (codeMEDECIN=="SuperMan-999999999")
       {info_med.m_Code              = "SuperMan-999999999";
        info_med.m_Password          = "Nitrosoft";
        info_med.m_CodeMedinTux      = "MedinTux";
        info_med.m_PasswordMedinTux  = "";
        info_med.m_Nom               = "SuperMan";
        info_med.m_Prenom            = "Junior";
        info_med.m_Nid               = "999999999";
        info_med.m_Spec              = "XX";
        return;
       }
    //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return ;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)  return ;
    QString requete ("SELECT ");
            requete += m_USER_IDENT_SIGEMS_LOG + "," +        // 0
                       m_USER_IDENT_SIGEMS_PAS + "," +        // 1
                       m_USER_IDENT_LOGIN      + "," +        // 2
                       m_USER_IDENT_PASSWORD   + "," +        // 3
                       m_USER_IDENT_NOM        + "," +        // 4
                       m_USER_IDENT_PRENOM     + "," +        // 5
                       m_USER_IDENT_NUM_ORDRE  + "," +        // 6
                       m_USER_IDENT_CODE_SPEC  + "," +        // 7
                       m_USER_IDENT_PRIMKEY    + " FROM  " +  // 8
                       m_USER_IDENT_TBL_NAME   + " WHERE " ;
    if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
       {requete +=  m_USER_IDENT_SIGEMS_LOG + "='" + codeMEDECIN + "';";
       }
    else if (m_USER_IDENT_TYPE_TABLE=="MedinTux")
       {requete +=  m_USER_IDENT_LOGIN      + "='" + codeMEDECIN + "';";
       }

    //"CODE,PWD,NOM,PRENOM,NID,SPEC FROM MEDECINS WHERE CODE='" + codeMEDECIN + "';");
    QSqlQuery query(requete, m_obj_User_Base);
    while ( query.next() )
        {     info_med.m_Code              = query.value(0).toString();
              info_med.m_Password          = query.value(1).toString();
              info_med.m_CodeMedinTux      = query.value(2).toString();
              info_med.m_PasswordMedinTux  = query.value(3).toString();

         info_med.m_Nom               = query.value(4).toString();
         info_med.m_Prenom            = query.value(5).toString();
         info_med.m_Nid               = query.value(6).toString();
         info_med.m_Spec              = query.value(7).toString();
         info_med.m_Pk                = query.value(8).toString();
        }
    /*
    QString requete ("SELECT ");
            requete += m_USER_IDENT_SIGEMS_LOG + "," +        // 0
                       m_USER_IDENT_SIGEMS_PAS + "," ;        // 1
    if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
       {    requete += m_USER_IDENT_SIGEMS_LOG + "," +        // 2
                       m_USER_IDENT_SIGEMS_PAS + "," ;        // 3
       }
    else if (m_USER_IDENT_TYPE_TABLE=="MedinTux")
       {   requete +=  m_USER_IDENT_LOGIN      + "," +        // 2
                       m_USER_IDENT_PASSWORD   + "," ;        // 3
       }
           requete +=  m_USER_IDENT_NOM        + "," +        // 4
                       m_USER_IDENT_PRENOM     + "," +        // 5
                       m_USER_IDENT_NUM_ORDRE  + "," +        // 6
                       m_USER_IDENT_CODE_SPEC  + " FROM  " +  // 7
                       m_USER_IDENT_TBL_NAME   + " WHERE " ;
    if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
       {requete +=  m_USER_IDENT_SIGEMS_LOG + "='" + codeMEDECIN + "';";
       }
    else if (m_USER_IDENT_TYPE_TABLE=="MedinTux")
       {requete +=  m_USER_IDENT_LOGIN      + "='" + codeMEDECIN + "';";
       }

    //"CODE,PWD,NOM,PRENOM,NID,SPEC FROM MEDECINS WHERE CODE='" + codeMEDECIN + "';");
    QSqlQuery query(requete, m_pUser_Base);
    while ( query.next() )
        {if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
            { info_med.m_Code              = query.value(0).toString();
              info_med.m_Password          = query.value(1).toString();
            }
         else
            { info_med.m_CodeMedinTux      = query.value(2).toString();
              info_med.m_PasswordMedinTux  = query.value(3).toString();
              info_med.m_Code              = query.value(2).toString();
              info_med.m_Password          = query.value(3).toString();
            }
         info_med.m_Nom               = query.value(4).toString();
         info_med.m_Prenom            = query.value(5).toString();
         info_med.m_Nid               = query.value(6).toString();
         info_med.m_Spec              = query.value(7).toString();
        }
    */
}

//-------------------------------------- PatientIntervenantsDelete ----------------------------------------------------------------------------
int CMoteurCCAM_Base::DeleteMedecin( const char *primKey)
{ //................................. methode QSqlCursor .....................................
  //                         ne pose pas de PB avec valeur avec une apostrophe
  int ok  = FALSE;
  //............... ouvrir la base .......................................................
  if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));                 return  FALSE;}
  if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)                     return  FALSE;
  QString requete("DELETE FROM ");
          requete += m_USER_IDENT_TBL_NAME  + " WHERE ";
          requete += m_USER_IDENT_PRIMKEY  + "= '" + primKey + "' ";
 QSqlQuery query(QString::null , m_obj_User_Base );
 ok = query.exec(requete);
 m_pUser_Base->close();
 return ok;
}

//---------------------------------------------------- CreateMedecins -----------------------------------------------------------
bool CMoteurCCAM_Base::CreateMedecins( INFO_MED &info_med )
{  //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));                 return FALSE;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)                     return FALSE;
    if  (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
        {if (MedecinExisteByPrimKey(info_med.m_Code))                 return FALSE;  // avec la SIGEMS le info_med.m_Code = info_med.m_Pk donc doit pas etre auto
        }
    else
        {if (MedecinExisteByLoginMedinTux(info_med.m_CodeMedinTux))  return FALSE;  // avec  MedinTux le login doit etre unique
        }
    bool ok = FALSE;
       //............... crer un curseur SQL ..................................................
       Q3SqlCursor cur(m_USER_IDENT_TBL_NAME, TRUE, m_obj_User_Base);
       if ( !cur.canInsert ()) return FALSE;
       //............... si OK on recupere le buffer et on le charge avec les données .........
       QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
       buffer->setValue( m_USER_IDENT_SIGEMS_LOG,     info_med.m_Code               );  // y placer les données
       buffer->setValue( m_USER_IDENT_SIGEMS_PAS,     info_med.m_Password           );  // y placer les données
       buffer->setValue( m_USER_IDENT_LOGIN,          info_med.m_CodeMedinTux       );  // y placer les données
       buffer->setValue( m_USER_IDENT_PASSWORD,       info_med.m_PasswordMedinTux   );  // y placer les données
       buffer->setValue( m_USER_IDENT_NOM,            info_med.m_Nom                );  // y placer les données
       buffer->setValue( m_USER_IDENT_PRENOM,         info_med.m_Prenom             );  // y placer les données
       buffer->setValue( m_USER_IDENT_NUM_ORDRE,      info_med.m_Nid                );  // y placer les données
       buffer->setValue( m_USER_IDENT_CODE_SPEC,      info_med.m_Spec               );  // y placer les données
       if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
          {if (info_med.m_Pk.length()==6) buffer->setValue( m_USER_IDENT_PRIMKEY, info_med.m_Pk ); // avec la SIGEMS le info_med.m_Code = info_med.m_Pk donc doit pas etre auto
          }
       cur.insert();                                                   // ecrire le buffer avec les données du header
       ok  = TRUE;
    return ok;
}


//---------------------------------------------------- VerificationLogin -----------------------------------------------------------
bool CMoteurCCAM_Base::VerificationLogin(const QString &utilisateur,  QString password, QString *realPass /*=0*/)
{   //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return FALSE;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)          return FALSE;
    if (realPass) *realPass = "";
    bool exist = FALSE;
    long nb = 0;
    QString requete    ("SELECT ");
    if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
       {requete += m_USER_IDENT_SIGEMS_LOG + " FROM " +
                   m_USER_IDENT_TBL_NAME   + " WHERE "+ m_USER_IDENT_SIGEMS_LOG + "='" + utilisateur + "' AND " +
                                                        m_USER_IDENT_SIGEMS_PAS + "='" + password    + "'";
        QSqlQuery query(requete, m_obj_User_Base); if ( query.next() ) { if (realPass) *realPass = password; return TRUE;    }
       }
    //................. test du mode automatique on regarde si l'utilisateur �  un password SIGEMS AUTO .......................................
    //                  si oui on teste on retournera le pass cripté
    else
       {if (realPass && ( password=="NOPASSW"||password=="DEFAULT") )
           {requete += m_USER_IDENT_PASSWORD   + ","      + m_USER_IDENT_SIGEMS_PAS + " FROM " +
                       m_USER_IDENT_TBL_NAME   + " WHERE "+ m_USER_IDENT_LOGIN      + "='" + utilisateur + "' AND " +
                                                            m_USER_IDENT_SIGEMS_PAS + "='" + password    + "'";

            QSqlQuery query(requete, m_obj_User_Base);
            if ( query.next() ) {*realPass = query.value(0).toString();   // retourner le cripted pass
                                 return TRUE;
                                }
           }
        //...................... demande de verification d'un pass cripté  (car mode MedinTux et vient du .ini) ..............................
        //                       on le compare �  celui sur disque (qui est toujours cripte)
       else if (realPass)
           {requete += m_USER_IDENT_SIGEMS_LOG + ","      + m_USER_IDENT_SIGEMS_PAS + " FROM " +
                       m_USER_IDENT_TBL_NAME   + " WHERE "+ m_USER_IDENT_LOGIN      + "='" + utilisateur + "' AND " +
                                                            m_USER_IDENT_PASSWORD   + "='" + password    + "'"; // cripté le pass
            QSqlQuery query(requete, m_obj_User_Base); if ( query.next()) {if (realPass) *realPass = password; return TRUE;}
           }
       //...................... demande de verification d'un pass non cripté ou non (car mode MedinTux et vient de la saisie) ................
        //                      (déj�  cripté si mis en auto) donc on teste direct au cas ou saisie déj�  cripté sinon
       //                       on le le cripte et on le compare �  celui sur disque (qui est toujours cripte)
       else
           {
            requete += m_USER_IDENT_PASSWORD + ","      + m_USER_IDENT_PASSWORD   + " FROM " +     // rcuperer le pass cripté en value(1)
                       m_USER_IDENT_TBL_NAME   + " WHERE "+ m_USER_IDENT_LOGIN      + "='" + utilisateur + "' ";
            QSqlQuery query(requete, m_obj_User_Base);   if ( query.next() )
                                                        { QString passInRequete = "";
                                                          passInRequete = query.value(1).toString();
                                                          if (passInRequete.length()==0 && password.length()==0) return TRUE;
                                                          if (passInRequete == password)                         return TRUE; // y avait un cripted pass en entrée
                                                          PassWordEncode(password);
                                                          if (passInRequete==password) return TRUE; // y avait un non cripted pass en entrée
                                                        }
           }

       }
    return FALSE;
}

//---------------------------------------------------- GetComboPraticiens -----------------------------------------------------------
long CMoteurCCAM_Base::GetComboPraticiens(QComboBox *pQComboBox)
{   //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return 0;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)          return 0;


    QString requete ("SELECT ");
    if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
       {requete +=  m_USER_IDENT_SIGEMS_LOG + ",";
       }
    else if (m_USER_IDENT_TYPE_TABLE=="MedinTux")
       {requete +=  m_USER_IDENT_LOGIN      + ",";
       }
    requete += m_USER_IDENT_NOM        + "," +
               m_USER_IDENT_PRENOM     + "," +
               m_USER_IDENT_NUM_ORDRE  + " FROM  " +
               m_USER_IDENT_TBL_NAME   + " WHERE " + m_USER_IDENT_NUM_ORDRE + " <> '999%' ORDER BY " + m_USER_IDENT_NOM;

    long nb = 0;
    pQComboBox->clear();
    //QString requete =  "SELECT CODE,NOM,PRENOM,NID FROM MEDECINS WHERE NID <> '999999999' ORDER BY NOM;";
    QSqlQuery query(requete, m_obj_User_Base);
    OutSQL_error(query, "GetComboPraticiens ", requete );
    while (query.next())
    {
        pQComboBox->insertItem( nb,      query.value(1).toString() + " " + query.value(2).toString(), // nom prénom
                                         query.value(0).toString() + "-" + query.value(3).toString()  // log - num ordre
                              );
        ++nb;
    }
    return nb;
}

//---------------------------------------------------- GetNidPraticiens -----------------------------------------------------------
QString CMoteurCCAM_Base::GetNidPraticiens(const QString &code_utilisateur)
{
    //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return 0;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)  return 0;

    QString requete    ("SELECT ");
            requete += m_USER_IDENT_NUM_ORDRE  + " FROM " +
                       m_USER_IDENT_TBL_NAME   + " WHERE ";


    if (m_USER_IDENT_TYPE_TABLE=="SIGEMS")
       {requete +=   m_USER_IDENT_SIGEMS_LOG + "='" + code_utilisateur + "';";
       }
    else if (m_USER_IDENT_TYPE_TABLE=="MedinTux")
       {requete +=   m_USER_IDENT_LOGIN      + "='" + code_utilisateur + "';";
       }

    //QString requete =  "SELECT NID FROM MEDECINS WHERE CODE='" + code_utilisateur + "';";
    QSqlQuery query(requete, m_obj_User_Base);
    while (query.next()) {
        return query.value(0).toString();
    }
    return QString::null;
}

//---------------------------------------------------- SetComboPraticiensOnNID -----------------------------------------------------------
int CMoteurCCAM_Base::SetComboPraticiensOnNID(QComboBox* pQComboBox, QString& nid)
{QString nid_f = "";
 for (int i = 0; i < pQComboBox->count(); i++)
     {QVariant   qv = pQComboBox->itemData (i);
      QString  text = qv.toString () ;
      int             f = text.findRev("-");
      if (f != -1)
         {nid_f = text.mid(f+1);
          if (nid_f == nid)
             {pQComboBox->setCurrentItem (i);
              return i;
             }
         } //endif (f != -1)
     } //endfor (int i = 0; i <= pQComboBox->count(); i++)
 return -1;
}

//---------------------------------------------------- AjoutActesThesaurus -----------------------------------------------------------
long CMoteurCCAM_Base::AjoutActesThesaurus(const QString &utilisateur, const QString &codeACTE, QString* pCodeCategorie, QString* pCodeUsuel)
{   QString codeCategorie (TR("(Aucune)"));
    QString codeUsuel("");
    if ( pCodeCategorie != 0) codeCategorie = *pCodeCategorie;
    if ( pCodeUsuel != 0)     codeUsuel     = *pCodeUsuel;
    QString requete = "";
    // .... rempli le thesaurus avec l'acte selectionné ....
    requete =  "INSERT INTO THESAURUS (CODEMEDECIN , CATEGORIE , LIBUSUEL , CODEACTE ) VALUES (";
    requete += "'" + utilisateur   + "',";          // code medecin
    requete += "'" + codeCategorie + "',";                     // code categorie
    requete += "'" + codeUsuel     + "',";                             // libelle usuel
    requete += "'" + codeACTE      + "');";            // code acte
    QSqlQuery query_insert(requete, m_obj_CCAM_Base);

    return 1;
}

//---------------------------------------------------- GetThesaurus -----------------------------------------------------------
long CMoteurCCAM_Base::GetThesaurus(Q3ListView *pQListView, const QString &utilisateur,const QString &categorie)
{
    long nb=0;
    pQListView->clear();
    QString requete = "";
    if (categorie == TR("(Toutes)")) {
        requete = "SELECT SERIE,CATEGORIE,LIBUSUEL,CODEACTE FROM THESAURUS WHERE CODEMEDECIN='";
        requete        += utilisateur + "' AND CATEGORIE LIKE '%_%';";
     }
    else {
        requete = "SELECT SERIE,CATEGORIE,LIBUSUEL,CODEACTE FROM THESAURUS WHERE CODEMEDECIN='";
        requete        += utilisateur + "' AND CATEGORIE= '" + categorie + "';";
    }
    QSqlQuery query(requete, m_obj_CCAM_Base);
    while (query.next())
    {   QString tarif = GetPrixActe(query.value(3).toString(), QDate::currentDate());
        new Q3ListViewItem(pQListView,
                          ((QString) query.value(1).toString()).replace("?","'"),   //categorie
                          ((QString) query.value(2).toString()).replace("?","'"),   //code usuel
                          query.value(3).toString(),                                //code ccam
                          CodeCCAMToLibelleLong(query.value(3).toString()),         //libellé CCAM
                          query.value(0).toString(),                                //numéro de serie
                          CodeCCAMToPlaceArbo(query.value(3).toString()),           //PlaceArbo
                          tarif);
        nb++;
    }
    return nb;
}

//---------------------------------------------------- CodeCCAMToPlaceArbo -----------------------------------------------------------
QString CMoteurCCAM_Base::CodeCCAMToPlaceArbo(const QString& code)
{
    QString place_arbo = "";
    //.............................. methode QSqlQuery ............................
    ///*
    QString requete ( "SELECT PLACEARBORESCENCE FROM ACTES WHERE CODE = '" ) ;
    requete += code + "';";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    while ( query.next() )
    {place_arbo = query.value(0).toString();
    }
 return place_arbo;
}

//---------------------------------------------------- GetInfoActeThesaurus -----------------------------------------------------------
long CMoteurCCAM_Base::GetInfoActeThesaurus(QComboBox *pQComboBox, QLineEdit *pQLineEdit, const QString &serie)
{
    long nb = 0;
    QString categorie = "";
    QString lib_usuel = "";
    QString requete = "SELECT CATEGORIE,LIBUSUEL FROM THESAURUS WHERE SERIE=" + serie + ";";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    while (query.next()){
        categorie = ((QString) query.value(0).toString()).replace("¤","'");
        lib_usuel = ((QString) query.value(1).toString()).replace("¤","'");
        nb++;
    }
    if ( nb == 1) {
        int position = 0;
        // ... positionner la combobox sur la catégorie de l'acte ...
        for (int i = 0; i < pQComboBox->count(); i++){
            if (pQComboBox->text(i) == categorie) position = i;
        }
        pQComboBox->setCurrentItem(position);
        // ... remplir lineEdit ...
        pQLineEdit->setText(lib_usuel);
    }
    return nb;  // comme ca si nb = 0 acte non trouvé
                //          si nb = 1 un seul acte trouvé ok
                //          si nb > 1 problème dans la base
}
//---------------------------------------------------- MAJActeThesaurus -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJActeThesaurus(const QString &serie,const QString &categorie,const QString &lib_usuel)
{
    bool ok = FALSE;
    QString requete =  "UPDATE THESAURUS SET ";
    requete += "CATEGORIE='"        + ((QString)categorie).replace("'","¤") + "', ";
    requete += "LIBUSUEL='"         + ((QString)lib_usuel).replace("'","¤") + "' ";
    requete += "WHERE SERIE=" + serie + ";";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    //OutSQL_error(query, "MAJActeThesaurus ", requete );
    ok = TRUE;
    return ok;
}

//---------------------------------------------------- GetInfoActeThesaurus -----------------------------------------------------------
long CMoteurCCAM_Base::GetComboCategorie(QComboBox *pQComboBox,const QString &code_utilisateur)
{
    long nb = 0;
    QString requete = "SELECT CATEGORIE FROM THESAURUS WHERE CODEMEDECIN='" + code_utilisateur + "' GROUP BY CATEGORIE;";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    while (query.next()){
        pQComboBox->insertItem(((QString)query.value(0).toString()).replace("¤","'"));
        nb++;
    }
    return nb;
}

//---------------------------------------------------- SupprimeActeThesaurus -----------------------------------------------------------
bool CMoteurCCAM_Base::SupprimeActeThesaurus(const QString &serie)
{
    bool ok = FALSE;

    QString requete =  "DELETE FROM THESAURUS WHERE SERIE=" + serie + ";";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    ok = true;
    return ok;
}
//---------------------------------------------------- SupprimeThesaurusPraticien -----------------------------------------------------------
bool CMoteurCCAM_Base::SupprimeThesaurusPraticien(const QString &code_utilisateur)
{
    bool ok = FALSE;

    QString requete =  "DELETE FROM THESAURUS WHERE CODEMEDECIN='" + code_utilisateur + "';";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    ok = true;
    return ok;
}


//---------------------------------------------------- CodeCCAMToLibelleLong -----------------------------------------------------------
QString CMoteurCCAM_Base::CodeCCAMToLibelleLong(const QString& code)

{
    QString libelle = "";
    //.............................. methode QSqlQuery ...........................
    ///*
    QString requete ( "SELECT LIBELLELONG FROM ACTES WHERE CODE = '" ) ;
    requete += code + "';";
    QSqlQuery query(requete, m_obj_CCAM_Base);
    while ( query.next() )
    {libelle = CGestIni::Utf8_Query(query,0);
    }
 return libelle;
}

//-----------------------------------------------------  CalculeDiffAge -------------------------------------------------
// ACTION: calcule l'age entre deux dates de début et de fin.
// ENTREE: QDate *pDebut: pointe sur une QDate de debut (exemple date de naissance)
//         QDate *pFin:   pointe sur une QDate de  fin (peut etre zero alors il sera pris la date en cours)
//         bool bTexte:   TRUE   alors l'age sera exprimé comme: 4 ans 6 mois 3 jours
//                        FALSE  alors l'age sera exprimé comme un chiffre
//         int bAbrege:   2  et alors seules les années seront sorties sous la forme: "0" ou "35" etc ...
//                        1  et alors seules les années seront sorties sous la forme: "0" ou "35" etc ...
//                        0  et alors les sorties seront detaillées avec si age<3 avec les mois et jours:
QString CMoteurCCAM_Base::CalculeDiffAge(QDate *pDebut, QDate *pFin/*=NULL*/, bool bTexte/*=TRUE*/, int bAbrege/*=1*/)
{
 char finMois[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
 int an=0;
 int mois=0;
 int jour=0;
 int dayYearDebut;
 int dayYearFin;
 QString buffer;
 QString sRes("");
 bool bFinAlloue = FALSE;
 long lEtatDayYear;

 if (pDebut && pDebut->isValid ())
    {
     if (pFin==NULL)
        {pFin       = new QDate;
         *pFin      = QDate::currentDate ();
         bFinAlloue = TRUE;
        }//if

     if (pFin->isValid ())
        {
         //.........................................Partie calcul ..........................................
         //........Le jour de l'année .................
         dayYearDebut = pDebut->dayOfYear ();
         dayYearFin   = pFin->dayOfYear ();
         //................................ bisextiles and cie ..................................................
         //ATTENTION A L'ANNEE BISEXTILLE (sauf en l'an 1900, car les débuts de siecles ne sont pas bisextille sauf les débuts de millénaire
         //60eme jour => 29 Février
         if (pDebut->year()%4==0 && pDebut->year()%100 && pDebut->year()%1000    &&  dayYearDebut>60 )
             dayYearDebut--;
         if (pFin->year()%4==0   && pFin->year()%100   && pFin->year()%1000      &&  dayYearFin>60)
             dayYearFin--;

          //....................Position des jours l'un par rapport �  l'autre ....................................
          if      (dayYearFin<dayYearDebut)     lEtatDayYear = -1;
          else if (dayYearFin>dayYearDebut)     lEtatDayYear =  1;
          else                                  lEtatDayYear =  0;

          //...................Calcul année (-1 si le jour de l'année en cour est <) .............................
          an = pFin->year() - pDebut->year() - (lEtatDayYear<0? 1 :0);

          if (lEtatDayYear!=0)
             {
              //.......................Calcul mois .....................................
              if (lEtatDayYear<0)
                  mois = 12 - pDebut->month() + pFin->month()   - (pFin->day()<pDebut->day()? 1 :0);
              else
                  mois = pFin->month()        - pDebut->month() - (pFin->day()<pDebut->day()? 1 :0);

              //...................... Calcul du jour ...................................
              if (pFin->day() < pDebut->day())
                  jour=(finMois[pFin->month()>1 ? pFin->month()-2 : 11] - pDebut->day()) + pFin->day();
              else
                  jour=pFin->day() - pDebut->day();
             } //if(lEtatDayYear!=0)

             //.............................................. Partie texte .........................................
          if (an)
             {
              if (bTexte)
                  buffer.sprintf(TR("%d an%s "), an, an>1 ? "s" : "");
              else
                  buffer.sprintf("%d ", an);
              sRes = buffer;
             } //if (an)
          //.................... si bAbrege==2 alors on ne sort que les années .......................................
          if (bAbrege==2)
             {if (an==0) sRes="0";
              if(bFinAlloue)  delete pFin;
              return sRes;
             }
          if (!bAbrege || an < 3 /* || an<3*/) // age Inferieur �  3 ans et version detaillée
             {
              if (mois || !bTexte)
                 {
                  if (bTexte)
                      buffer.sprintf(TR("%d mois "), mois);
                  else
                      buffer.sprintf("%d ", mois);
                  sRes+=buffer;
                } // if (mois || !bTexte)

              if (jour || !bTexte)
                 {
                  if (bTexte)
                      buffer.sprintf(TR("%d jour%s "), jour, jour>1 ? "s" : "");
                  else
                      buffer.sprintf("%d ", jour);
                  sRes += buffer;
                 } // if  (jour || !bTexte)
              }// if (!bAbrege || an<3) //Inferieure �  3 ans
          }// if (pFin->m_status==COleDateTime::valid)
     }// if (pDebut && pDebut->m_status==COleDateTime::valid)

     if(bFinAlloue)  delete pFin;
     return sRes;
}

// ******************************************************************************************
// ********************** REQUETES POUR LA MISE A JOUR DE LA BASE CCAM **********************
// ******************************************************************************************

//-----------------------------------------------------  MAJAddActe -------------------------------------------------
bool CMoteurCCAM_Base::MAJAddActe(ENT_ACTE &acte)
{

    int ok = FALSE;
    Q3SqlCursor cur("ACTES", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    //............... si OK on recupere le buffer et on le charge avec les données .........
    QSqlRecord *buffer = cur.primeInsert();                         // recuperer le buffer d'insertion
    buffer->setValue( "CODE",               acte.m_CodeActe         );     // y placer les données
    buffer->setValue( "LIBELLECOURT",       acte.m_LibelleCourt     );     // y placer les données
    buffer->setValue( "TYPE",               acte.m_TypeActe  );     // y placer les données
    buffer->setValue( "SEXE",               acte.m_Sexe  );     // y placer les données
    buffer->setValue( "DATECREATION",       acte.m_DateCreation  );     // y placer les données
    buffer->setValue( "DATEFIN",            acte.m_DateFin  );     // y placer les données
    buffer->setValue( "ASSURANCE1",         acte.m_Assurance1  );     // y placer les données
    buffer->setValue( "ASSURANCE2",         acte.m_Assurance2  );     // y placer les données
    buffer->setValue( "ASSURANCE3",         acte.m_Assurance3  );     // y placer les données
    buffer->setValue( "ASSURANCE4",         acte.m_Assurance4  );     // y placer les données
    buffer->setValue( "ASSURANCE5",         acte.m_Assurance5  );     // y placer les données
    buffer->setValue( "ASSURANCE6",         acte.m_Assurance6  );     // y placer les données
    buffer->setValue( "ASSURANCE7",         acte.m_Assurance7  );     // y placer les données
    buffer->setValue( "ASSURANCE8",         acte.m_Assurance8  );     // y placer les données
    buffer->setValue( "ASSURANCE9",         acte.m_Assurance9  );     // y placer les données
    buffer->setValue( "ASSURANCE10",        acte.m_Assurance10 );     // y placer les données
    buffer->setValue( "DEPLACEMENT",        acte.m_Deplacement  );     // y placer les données
    buffer->setValue( "ARBORESCENCE1",      acte.m_Arborescence1  );     // y placer les données
    buffer->setValue( "ARBORESCENCE2",      acte.m_Arborescence2  );     // y placer les données
    buffer->setValue( "ARBORESCENCE3",      acte.m_Arborescence3  );     // y placer les données
    buffer->setValue( "ARBORESCENCE4",      acte.m_Arborescence4  );     // y placer les données
    buffer->setValue( "ARBORESCENCE5",      acte.m_Arborescence5  );     // y placer les données
    buffer->setValue( "ARBORESCENCE6",      acte.m_Arborescence6  );     // y placer les données
    buffer->setValue( "ARBORESCENCE7",      acte.m_Arborescence7  );     // y placer les données
    buffer->setValue( "ARBORESCENCE8",      acte.m_Arborescence8  );     // y placer les données
    buffer->setValue( "ARBORESCENCE9",      acte.m_Arborescence9  );     // y placer les données
    buffer->setValue( "ARBORESCENCE10",     acte.m_Arborescence10  );     // y placer les données
    buffer->setValue( "PLACEARBORESCENCE",  acte.m_PlaceArbo  );          // y placer les données
    buffer->setValue( "CODESTRUCTURE",      acte.m_CodeStructure  );     // y placer les données
    buffer->setValue( "CODEPRECEDENT",      acte.m_CodePrecedent  );     // y placer les données
    buffer->setValue( "CODESUIVANT",        acte.m_CodeSuivant  );       // y placer les données
    //acte.m_LibelleLong.replace(0x92, 0x27);
    buffer->setValue( "LIBELLELONG",        (const char*) acte.m_LibelleLong );
    buffer->setValue( "CCAM26",        acte.m_Ccam26  );     // y placer les données
    buffer->setValue( "CCAM27",        acte.m_Ccam27  );     // y placer les données
    buffer->setValue( "CCAM28",        acte.m_Ccam28  );     // y placer les données
    buffer->setValue( "CCAM22",        acte.m_Ccam22  );     // y placer les données
    buffer->setValue( "CCAM24",        acte.m_Ccam24  );     // y placer les données
    buffer->setValue( "CCAM25",        acte.m_Ccam25  );     // y placer les données
    buffer->setValue( "CCAM31",        acte.m_Ccam31  );     // y placer les données
    buffer->setValue( "CCAM34",        acte.m_Ccam34  );     // y placer les données
    if (cur.insert())                                               // ecrire le buffer avec les données du header
       {ok = TRUE;                                                  // s'occuper des données binaires BLOB
       }
    return ok;
}
//-----------------------------------------------------  MAJAddCompatibilite -------------------------------------------------
bool CMoteurCCAM_Base::MAJAddCompatibilite(QString &codeacte,QString &activite,QString &dateeffet,ENT_COMPATIBILITE *compatibilite)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 8) && (compatibilite[i].m_CodeActe != "")) {
        requete = "INSERT INTO ASSOCIABILITE (CODEACTE , ACTIVITE , DATEEFFET , ACTEASSO , ACTIVITEASSO , REGLE) VALUES (";
        requete += "'" + codeacte + "',";
        requete += "'" + activite + "',";
        requete += "'" + dateeffet + "',";
        requete += "'" + compatibilite[i].m_CodeActe + "',";
        requete += "'" + compatibilite[i].m_CodeActivite + "',";
        requete += "'" + compatibilite[i].m_RegleTarifaire + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}
//----------------------------------------------------- MAJAddActiviteActe -------------------------------------------------
bool CMoteurCCAM_Base::MAJAddActiviteActe(QString &codeacte,QString &activite)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    requete = "INSERT INTO ACTIVITEACTE (CODEACTE , ACTIVITE) VALUES (";
    requete += "'" + codeacte + "',";
    requete += "'" + activite + "');";
    query_insert.exec(requete);
    return TRUE;
}
//---------------------------------------------------- CheckTableVersion -----------------------------------------------------------
bool CMoteurCCAM_Base::CheckTableVersion()
{
    bool ok = FALSE;
    QSqlQuery query_version(QString::null, m_obj_CCAM_Base);
    QString requete = "CREATE TABLE IF NOT EXISTS VERSION (";
    requete +=        "SERIE INT(11)  NOT NULL auto_increment ,";
    requete +=        " IDFICHIER char(3) default NULL,";
    requete +=        " DATECREATION char(8) default NULL,";
    requete +=        " DATEINTEGRATION char(8) default NULL,";
    requete +=        " CHRONOLOGIE char(5) default NULL,";
    requete +=        " TYPEFICHIER char(1) default NULL,";
    requete +=        " NUMVERSION char(4) default NULL,";
    requete +=        " PRIMARY KEY (SERIE)";
    requete +=        ");";
    ok = query_version.exec(requete);
    return ok;
}
//---------------------------------------------------- CheckIntegration -----------------------------------------------------------
QString CMoteurCCAM_Base::CheckIntegration(VER_BASE &version)
{
    QString date_integree = "";
    QString requete =  "SELECT SERIE,IDFICHIER,DATECREATION,DATEINTEGRATION,CHRONOLOGIE,TYPEFICHIER,NUMVERSION FROM VERSION WHERE";
    requete += " IDFICHIER = '" + version.m_IdFichier + "' AND DATECREATION = '" + version.m_DateCreation + "'";
    requete += " AND CHRONOLOGIE = '" + version.m_Chrono + "' AND TYPEFICHIER = '" + version.m_TypeFichier + "'";
    requete += " AND NUMVERSION = '" + version.m_NumVersion + "' ORDER BY SERIE;";
    QSqlQuery query(requete, m_obj_CCAM_Base);

    while (query.next()) {
        date_integree = query.value(3).toString();
    }
    return date_integree;
}
//---------------------------------------------------- MAJAddVersionBase -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddVersionBase(VER_BASE &version)
{
    bool ok = FALSE;
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    requete = "INSERT INTO VERSION (IDFICHIER,DATECREATION,DATEINTEGRATION,CHRONOLOGIE,TYPEFICHIER,NUMVERSION) VALUES (";
    requete += "'" + version.m_IdFichier + "',";
    requete += "'" + version.m_DateCreation + "',";
    requete += "'" + version.m_DateIntegration + "',";
    requete += "'" + version.m_Chrono + "',";
    requete += "'" + version.m_TypeFichier + "',";
    requete += "'" + version.m_NumVersion + "');";
    ok = query_insert.exec(requete);
    return ok;
}
//---------------------------------------------------- MAJAddProcedure -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddProcedure(QString &codeacte,QString &dateeffet,QString *procedure)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 8) && (procedure[i] != "")) {
        requete = "INSERT INTO PROCEDURES (CODEACTE , DATEEFFET , CODEPROCEDURE) VALUES (";
        requete += "'" + codeacte + "',";
        requete += "'" + dateeffet + "',";
        requete += "'" + procedure[i] + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}
//---------------------------------------------------- MAJAddIncompatibilite -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddIncompatibilite(QString &codeacte,QString &dateeffet,QString *incompatibilite)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 8) && (incompatibilite[i] != "")) {
        requete = "INSERT INTO INCOMPATIBILITE (CODEACTE , DATEEFFET , INCOMPATIBLE) VALUES (";
        requete += "'" + codeacte + "',";
        requete += "'" + dateeffet + "',";
        requete += "'" + incompatibilite[i] + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}
//---------------------------------------------------- MAJAddModificateurActe -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddModificateurActe(QString &codeacte,QString &activite,QString &dateeffet,QString *modificateur)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 10) && (modificateur[i] != "")) {
        requete = "INSERT INTO MODIFICATEURACTE (CODEACTE , CODEACTIVITE , DATEEFFET , MODIFICATEUR) VALUES (";
        requete += "'" + codeacte + "',";
        requete += "'" + activite + "',";
        requete += "'" + dateeffet + "',";
        requete += "'" + modificateur[i] + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddNoteActe -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddNoteActe(QString &codeacte,NOTE_ACTE &noteacte)
{
    int ok = FALSE;
    Q3SqlCursor cur("NOTES", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODEACTE", codeacte);
    buffer->setValue( "TYPE",     noteacte.m_TypeNote);
    buffer->setValue( "TEXTE",    noteacte.m_TexteNote.stripWhiteSpace());
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddPhaseActe -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddPhaseActe(ENT_PHASE &phase)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
        requete = "INSERT INTO PHASEACTE (CODEACTE , ACTIVITE , PHASE , NBDENTS , AGEMINI , AGEMAXI , ICR , CLASSANT) VALUES (";
        requete += "'" + phase.m_CodeActe + "',";
        requete += "'" + phase.m_CodeActivite + "',";
        requete += "'" + phase.m_CodePhase + "',";
        requete += "'" + phase.m_NbDents + "',";
        requete += "'" + phase.m_AgeMini + "',";
        requete += "'" + phase.m_AgeMaxi + "',";
        requete += "'" + phase.m_ICR + "',";
        requete += "'" + phase.m_Classant + "');";
        query_insert.exec(requete);
    return TRUE;
}

//---------------------------------------------------- MAJAddArborescence -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddArborescence(ENT_ARBO &arborescence)
{
    int ok = FALSE;
    Q3SqlCursor cur("ARBORESCENCE", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODEMENU", arborescence.m_CodeMenu);
    buffer->setValue( "CODEPERE", arborescence.m_CodePere);
    buffer->setValue( "RANG",     arborescence.m_Rang);
    buffer->setValue( "LIBELLE",  (const char*) arborescence.m_Libelle.stripWhiteSpace());
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}
//---------------------------------------------------- MAJAddNoteArborescence -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddNoteArborescence(ENT_NOTEARBO &notearbo)
{
    int ok = FALSE;
    Q3SqlCursor cur("NOTESARBORESCENCE", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODEMENU", notearbo.m_CodeMenu);
    buffer->setValue( "TYPE",     notearbo.m_TypeNote);
    buffer->setValue( "TEXTE",    (const char*) notearbo.m_TexteNote.stripWhiteSpace());
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}
//---------------------------------------------------- MAJAddTabActivite -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabActivite(TAB_ACTIVITE &tabactivite)
{
    int ok = FALSE;
    Q3SqlCursor cur("ACTIVITE", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabactivite.m_CodeActivite);
    buffer->setValue( "LIBELLE", (const char*) tabactivite.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddTabAssociation -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabAssociation(TAB_ASSO *tabassociation)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 5) && (tabassociation[i].m_CodeAsso != "")) {
        requete = "INSERT INTO ASSOCIATION (CODE , DATEDEBUT , DATEFIN , COEFFICIENT) VALUES (";
        requete += "'" + tabassociation[i].m_CodeAsso + "',";
        requete += "'" + tabassociation[i].m_DateDebut + "',";
        requete += "'" + tabassociation[i].m_DateFin + "',";
        requete += "'" + tabassociation[i].m_Coefficient + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}
//---------------------------------------------------- MAJAddTabModificateur -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabModificateur(TAB_MODIFICATEUR &tabmodificateur)
{
    int ok = FALSE;
    Q3SqlCursor cur("MODIFICATEUR", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabmodificateur.m_CodeModificateur);
    buffer->setValue( "LIBELLE", (const char*) tabmodificateur.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddTypesNotes -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTypesNotes(TYPE_NOTE &typesnotes)
{
    int ok = FALSE;
    Q3SqlCursor cur("TYPENOTE", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    typesnotes.m_TypeNote);
    buffer->setValue( "LIBELLE", (const char*) typesnotes.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddTypesNotes -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabPhase(TAB_PHASE &tabphase)
{
    int ok = FALSE;
    Q3SqlCursor cur("PHASE", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabphase.m_CodePhase);
    buffer->setValue( "LIBELLE", (const char*) tabphase.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}
//---------------------------------------------------- CreationBase -----------------------------------------------------------
void CMoteurCCAM_Base::CreationBase(QString &nomBase)
{
    QString requete = "";
    QSqlQuery query(QString::null, m_obj_CCAM_Base);
    requete = "CREATE DATABASE IF NOT EXISTS " + nomBase + ";";
    query.exec(requete);
    return;
}
//---------------------------------------------------- CreationTables -----------------------------------------------------------
void CMoteurCCAM_Base::CreationTables()
{
    QSqlQuery query(QString::null, m_obj_CCAM_Base);

    query.exec("CREATE TABLE ACCES1 ("
               "CODE char(1) NOT NULL default '',"
               "ACCES varchar(80) default NULL,"
               "LIBELLE varchar(255) default NULL,"
               "PRIMARY KEY  (CODE),"
               "KEY ACCES (ACCES)"
               ");" );

    query.exec("CREATE TABLE ACCES2 ("
               "PERE char(1) default NULL,"
               "LIBELLE varchar(150) default NULL,"
               "KEY PERE (PERE)"
               ");" );

    query.exec("CREATE TABLE ACTION1 ("
               "VERBE varchar(20) NOT NULL default '',"
               "CODE char(1) default NULL,"
               "LIBELLE varchar(255) default NULL,"
               "PRIMARY KEY  (VERBE),"
               "KEY CODE (CODE)"
               ");");

    query.exec("CREATE TABLE ACTION2 ("
               "VERBE varchar(30) default NULL,"
               "CODE char(1) default NULL,"
               "SUBSTANTIF varchar(20) default NULL,"
               "KEY ACTION (VERBE),"
               "KEY CODE (CODE),"
               "KEY VERBE (SUBSTANTIF)"
               ");");

    query.exec("CREATE TABLE TOPOGRAPHIE1 ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(80) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE TOPOGRAPHIE2 ("
               "CODE char(2) NOT NULL default '',"
               "PERE char(1) default NULL,"
               "LIBELLE varchar(150) default NULL,"
               "PRIMARY KEY  (CODE),"
               "KEY PERE (PERE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS ACTES ("
               "CODE varchar(13) NOT NULL default '',"
               "LIBELLECOURT varchar(70) default NULL,"
               "TYPE char(1) default NULL,"
               "SEXE char(1) default NULL,"
               "DATECREATION varchar(8) default NULL,"
               "DATEFIN varchar(8) default NULL,"
               "ASSURANCE1 char(2) default NULL,"
               "ASSURANCE2 char(2) default NULL,"
               "ASSURANCE3 char(2) default NULL,"
               "ASSURANCE4 char(2) default NULL,"
               "ASSURANCE5 char(2) default NULL,"
               "ASSURANCE6 char(2) default NULL,"
               "ASSURANCE7 char(2) default NULL,"
               "ASSURANCE8 char(2) default NULL,"
               "ASSURANCE9 char(2) default NULL,"
               "ASSURANCE10 char(2) default NULL,"
               "DEPLACEMENT char(1) default NULL,"
               "ARBORESCENCE1 varchar(6) default NULL,"
               "ARBORESCENCE2 varchar(6) default NULL,"
               "ARBORESCENCE3 varchar(6) default NULL,"
               "ARBORESCENCE4 varchar(6) default NULL,"
               "ARBORESCENCE5 varchar(6) default NULL,"
               "ARBORESCENCE6 varchar(6) default NULL,"
               "ARBORESCENCE7 varchar(6) default NULL,"
               "ARBORESCENCE8 varchar(6) default NULL,"
               "ARBORESCENCE9 varchar(6) default NULL,"
               "ARBORESCENCE10 varchar(6) default NULL,"
               "PLACEARBORESCENCE varchar(12) default NULL,"
               "CODESTRUCTURE varchar(13) default NULL,"
               "CODEPRECEDENT varchar(13) default NULL,"
               "CODESUIVANT varchar(13) default NULL,"
               "LIBELLELONG text,"
               "CCAM26 varchar(8) default NULL,"
               "CCAM27 varchar(8) default NULL,"
               "CCAM28 varchar(8) default NULL,"
               "CCAM22 char(1) default NULL,"
               "CCAM24 char(1) default NULL,"
               "CCAM25 varchar(5) default NULL,"
               "CCAM31 varchar(20) default NULL,"
               "CCAM34 varchar(10) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS ACTIVITEACTE ("
               "CODEACTE char(13) default NULL,"
               "ACTIVITE char(1) default NULL,"
               "KEY CODEACTE (CODEACTE),"
               "KEY ACTIVITE (ACTIVITE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS ASSOCIABILITE ("
               "CODEACTE char(13) default NULL,"
               "ACTIVITE char(1) default NULL,"
               "DATEEFFET char(8) default NULL,"
               "ACTEASSO char(13) default NULL,"
               "ACTIVITEASSO char(1) default NULL,"
               "REGLE char(1) default NULL,"
               "KEY CODEACTE (CODEACTE),"
               "KEY ACTIVITE (ACTIVITE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS INCOMPATIBILITE ("
               "CODEACTE char(13) default NULL,"
               "DATEEFFET char(8) default NULL,"
               "INCOMPATIBLE char(13) default NULL,"
               "KEY CODEACTE (CODEACTE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS PROCEDURES ("
               "CODEACTE char(13) default NULL,"
               "DATEEFFET char(8) default NULL,"
               "CODEPROCEDURE char(13) default NULL,"
               "KEY CODEACTE (CODEACTE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS MODIFICATEURACTE ("
               "CODEACTE varchar(13) default NULL,"
               "CODEACTIVITE char(1) default NULL,"
               "DATEEFFET varchar(8) default NULL,"
               "MODIFICATEUR char(1) default NULL,"
               "KEY CODEACTE (CODEACTE),"
               "KEY CODEACTIVITE (CODEACTIVITE),"
               "KEY MODIFICATEUR (MODIFICATEUR)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS NOTES ("
               "CODEACTE varchar(13) default NULL,"
               "TYPE char(2) default NULL,"
               "TEXTE text,"
               "KEY CODEACTE (CODEACTE),"
               "KEY TYPE (TYPE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS PHASEACTE ("
               "CODEACTE char(13) default NULL,"
               "ACTIVITE char(1) default NULL,"
               "PHASE char(1) default NULL,"
               "NBDENTS char(2) default NULL,"
               "AGEMINI char(3) default NULL,"
               "AGEMAXI char(3) default NULL,"
               "ICR char(4) default NULL,"
               "CLASSANT char(1) default NULL"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS ARBORESCENCE ("
               "CODEMENU varchar(6) NOT NULL default '',"
               "CODEPERE varchar(6) default NULL,"
               "RANG varchar(6) default NULL,"
               "LIBELLE text,"
               "PRIMARY KEY  (CODEMENU),"
               "KEY CODEPERE (CODEPERE),"
               "KEY RANG (RANG)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS NOTESARBORESCENCE ("
               "CODEMENU varchar(6) default NULL,"
               "TYPE char(2) default NULL,"
               "TEXTE text,"
               "KEY CODEMENU (CODEMENU)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS ACTIVITE ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS ASSOCIATION ("
               "CODE char(1) NOT NULL default '',"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "COEFFICIENT decimal(4,3),"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS MODIFICATEUR ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS PHASE ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(80) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TYPENOTE ("
               "CODE char(2) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec(" CREATE TABLE IF NOT EXISTS MEDECINS ("
               "CODE char(6) NOT NULL default '',"
               "PWD char(32) default NULL,"
               "NOM char(30) default NULL,"
               "PRENOM char(30) default NULL,"
               "NID char(9) default NULL,"
               "SPEC char(2) default NULL,"
               "PRIMARY KEY (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS THESAURUS ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "CODEMEDECIN char(6) default NULL,"
               "CATEGORIE char(40) default NULL,"
               "LIBUSUEL char(50) default NULL,"
               "CODEACTE char(13) default NULL,"
               "PRIMARY KEY (SERIE),"
               "KEY (CODEMEDECIN)"
               ");");


    // ***** nouvelles tables pour la tairification *****
    query.exec("CREATE TABLE IF NOT EXISTS TB03 ("
               "REGLE char(1) NOT NULL default '',"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "COEFFICIENT decimal(4,3),"
               "PRIMARY KEY  (REGLE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB04 ("
               "SPECIALITE char(2) NOT NULL default '',"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "CLASSE_SPECIALITE char(2) default NULL,"
               "KEY SPECIALITE (SPECIALITE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB05 ("
               "NAT_PRESTATION char(3) NOT NULL default '',"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "TYPE_FORFAIT char(1) default NULL,"
               "KEY NAT_PRESTATION (NAT_PRESTATION)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB06 ("
               "CODE_MODIF char(1) NOT NULL default '',"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "UNITE_AGEMINI char(1) default NULL,"
               "AGEMINI INT(3) default NULL,"
               "UNITE_AGEMAXI char(1) default NULL,"
               "AGEMAXI INT(3) default NULL,"
               "KEY CODE_MODIF (CODE_MODIF)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB07 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "SEUIL_MET decimal(7,2) default NULL,"
               "SEUIL_ANT decimal(7,2) default NULL,"
               "SEUIL_REU decimal(7,2) default NULL,"
               "SEUIL_GUY decimal(7,2) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB08 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "CAISSE char(3) default NULL,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "TYPE_FERIE char(1) default NULL,"
               "JOUR char(8) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB09 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "MODIFICATEUR char(1) default NULL,"
               "COMPATIBLE char(1) default NULL,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB10 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "MODIFICATEUR char(1) default NULL,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "COHERENCE char(1) default NULL,"
               "PRES_MULTIPLE char(1) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB11 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "MODIFICATEUR char(1) default NULL,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "FORFAIT decimal(7,2) default NULL,"
               "COEF decimal(4,3) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB12 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "FORFAIT decimal(7,2) default NULL,"
               "COEF decimal(4,3) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB13 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "PRESTATION char(3) default NULL,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "EQUIVAL char(1) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB14 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "DMT char(3) default NULL,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "CLASSE_DMT char(2) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB15 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "MODIFICATEUR char(1) default NULL,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "MODIF_OC char(1) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB17 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "PRESTATION char(3) default NULL,"
               "SPECIALITE char(2) default NULL,"
               "DATE_OBLIG char(8) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB18 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "LOCALISATION char(2) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB19 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "NB_MODIF INT(3) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TB20 ("
               "SERIE INT(11)  NOT NULL auto_increment ,"
               "CONCEPT char(2) default NULL,"
               "DATEDEBUT char(8) default NULL,"
               "DATEFIN char(8) default NULL,"
               "VALEUR char(10) default NULL,"
               "PRIMARY KEY (SERIE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS COND_GEN ("
               "CODE char(4) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS CLASSE_DMT ("
               "CODE char(2) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS EXO_TM ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(80) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS NAT_ASS ("
               "CODE char(2) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS ADM_REMB ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(80) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS FRAIS_DEP ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(80) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TYPE_ACTE ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(80) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS TYPE_FORFAIT ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS EXT_DOC ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS CAT_MED ("
               "CODE char(2) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS COD_REGROUP ("
               "CODE char(3) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS CAT_SPEC ("
               "CODE char(2) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS AGR_RADIO ("
               "CODE char(2) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS CODE_PAIEMENT ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(80) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS NUM_DENTS ("
               "CODE char(2) NOT NULL default '',"
               "LIBELLE varchar(80) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS DOM ("
               "CODE char(3) NOT NULL default '',"
               "LIBELLE varchar(50) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS ASS_NPREV ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS RGL_TARIF ("
               "CODE char(1) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS SPECIALITES ("
               "CODE char(2) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS DMT ("
               "CODE char(3) NOT NULL default '',"
               "LIBELLE varchar(100) default NULL,"
               "PRIMARY KEY  (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS COMPAT_EXO_TM ("
               "EXOTM char(1) NOT NULL default '',"
               "EXOTMCOMPAT char(1) NOT NULL default '',"
               "PRIMARY KEY  (EXOTM)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS VAL_UNITE_OEUVRE ("
               "CODE char(3) NOT NULL default '',"
               "DATEEFFET char(8) default NULL,"
               "VALEUR decimal(6,2) default NULL,"
               "PRIMARY KEY (CODE)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS COND_GEN_ACTE ("
               "ACTE varchar(13) default NULL,"
               "CONDITION char(4) default NULL,"
               "KEY ACTE (ACTE),"
               "KEY CONDITION (CONDITION)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS REC_MED_ACTE ("
               "ACTE varchar(13) default NULL,"
               "ACTIVITE char(1) default NULL,"
               "RECMED char(5) default NULL,"
                   "TEXTE text,"
               "KEY ACTE (ACTE),"
               "KEY ACTIVITE (ACTIVITE),"
               "KEY RECMED (RECMED)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS LISTE_MOTS ("
               "MOT varchar(50) default NULL,"
               "DEFINITION text,"
               "KEY MOT (MOT)"
               ");");

    query.exec("CREATE TABLE IF NOT EXISTS HISTO_PHASE ("
               "CODEACTE varchar(13) default NULL,"
               "ACTIVITE char(1) default NULL,"
               "PHASE char(1) default NULL,"
               "DATE_EFFET char(8) default NULL,"
               "DATE_AM char(8) default NULL,"
               "DATE_JO char(8) default NULL,"
               "NBMAX INT(2) default NULL,"
               "UO char(3) default NULL,"
               "COEF_UO decimal(6,2) default NULL,"
               "CODEPAIEMENT char(1) default NULL,"
               "PU decimal(6,2) default NULL,"
               "MAJDOM1 decimal(4,3) default NULL,"
               "MAJDOM2 decimal(4,3) default NULL,"
               "MAJDOM3 decimal(4,3) default NULL,"
               "MAJDOM4 decimal(4,3) default NULL,"
               "SUPP_CHG decimal(6,2) default NULL"
               ");");

        query.exec("CREATE TABLE IF NOT EXISTS DATES_EFFET ("
               "DATE_EFFET char(8) default NULL"
               ");");


    return;
}

// ------------------------------------ MAJAddDatesEffet ------------------------------------
bool CMoteurCCAM_Base::MAJAddDatesEffet(QStringList liste)
{
    int ok = FALSE;
    Q3SqlCursor cur("DATES_EFFET", TRUE, m_obj_CCAM_Base);
        QSqlRecord *buffer;

        for ( QStringList::Iterator it = liste.begin(); it != liste.end(); ++it ) {
                if ( !cur.canInsert ()) return ok;
                buffer = cur.primeInsert();
                buffer->setValue( "DATE_EFFET",    *it);
                if (cur.insert())
                {ok = TRUE;
                }
        }
    return ok;
}


// ------------------------------------ GetTablesEssentielles ------------------------------------
QString CMoteurCCAM_Base::GetTablesEssentielles()
{
    QString retour = "";
    QString requete = "";
    QSqlQuery query(QString::null, m_obj_CCAM_Base);

    requete = "SELECT CODE,ACCES,LIBELLE FROM ACCES1;";
    query.exec(requete);
    while (query.next()) {
        retour += "001##.##" + query.value(0).toString() + "##.##" + query.value(1).toString() + "##.##" + query.value(2).toString() + "##.##\r\n";
    }
    requete = "SELECT PERE,LIBELLE FROM ACCES2;";
    query.exec(requete);
    while (query.next()) {
        retour += "002##.##" + query.value(0).toString() + "##.##" + query.value(1).toString() + "##.##\r\n";
    }
    requete = "SELECT VERBE,CODE,LIBELLE FROM ACTION1;";
    query.exec(requete);
    while (query.next()) {
        retour += "003##.##" + query.value(0).toString() + "##.##" + query.value(1).toString() + "##.##" + query.value(2).toString() + "##.##\r\n";
    }
    requete = "SELECT VERBE,CODE,SUBSTANTIF FROM ACTION2;";
    query.exec(requete);
    while (query.next()) {
        retour += "004##.##" + query.value(0).toString() + "##.##" + query.value(1).toString() + "##.##" + query.value(2).toString() + "##.##\r\n";
    }
    requete = "SELECT CODE,LIBELLE FROM TOPOGRAPHIE1;";
    query.exec(requete);
    while (query.next()) {
        retour += "005##.##" + query.value(0).toString() + "##.##" + query.value(1).toString() + "##.##\r\n";
    }
    requete = "SELECT CODE,PERE,LIBELLE FROM TOPOGRAPHIE2;";
    query.exec(requete);
    while (query.next()) {
        retour += "006##.##" + query.value(0).toString() + "##.##" + query.value(1).toString() + "##.##" + query.value(2).toString() + "##.##\r\n";
    }
    retour.replace(0x92,0x27);
    return retour;
}

// ------------------------------------ ImportTablesEssentielles ------------------------------------
long CMoteurCCAM_Base::ImportTablesEssentielles(QString &fichier)
{
    long nb=0;
    QFile *buffer = new QFile(fichier);
    QString valeur1;
    QString valeur2;
    QString valeur3;
    QRegExp sep("##.##");
    if ( buffer->size() < 2) return -1;
    if (buffer->open( QIODevice::ReadOnly )==FALSE) return -1;
    DeleteAllData("ACCES1");
    DeleteAllData("ACCES2");
    DeleteAllData("ACTION1");
    DeleteAllData("ACTION2");
    DeleteAllData("TOPOGRAPHIE1");
    DeleteAllData("TOPOGRAPHIE2");
    QString ligne_lu = "";
    char charBuff[450];
    while (buffer->readLine(charBuff,400) != -1)
       {ligne_lu = charBuff;
        if (nb == 0 && ligne_lu.mid(0,2) != "00")
           {buffer->close();
            delete buffer;
            return -1;
            }

        if (ligne_lu.mid(0,2) != "00") break; // si ligne incohérente la sauter
        if (ligne_lu.mid(0,3) == "001") {
            valeur1 = ligne_lu.section(sep,1,1);
            valeur2 = ligne_lu.section(sep,2,2);
            valeur3 = ligne_lu.section(sep,3,3);
            MAJAddTableAcces1(valeur1,valeur2,valeur3);
        }
        if (ligne_lu.mid(0,3) == "002") {
            valeur1 = ligne_lu.section(sep,1,1);
            valeur2 = ligne_lu.section(sep,2,2);
            MAJAddTableAcces2(valeur1,valeur2);
        }
        if (ligne_lu.mid(0,3) == "003") {
            valeur1 = ligne_lu.section(sep,1,1);
            valeur2 = ligne_lu.section(sep,2,2);
            valeur3 = ligne_lu.section(sep,3,3);
            MAJAddTableAction1(valeur1,valeur2,valeur3);
        }
        if (ligne_lu.mid(0,3) == "004") {
            valeur1 = ligne_lu.section(sep,1,1);
            valeur2 = ligne_lu.section(sep,2,2);
            valeur3 = ligne_lu.section(sep,3,3);
            MAJAddTableAction2(valeur1,valeur2,valeur3);
        }
        if (ligne_lu.mid(0,3) == "005") {
            valeur1 = ligne_lu.section(sep,1,1);
            valeur2 = ligne_lu.section(sep,2,2);
            MAJAddTableTopographie1(valeur1,valeur2);
        }
        if (ligne_lu.mid(0,3) == "006") {
            valeur1 = ligne_lu.section(sep,1,1);
            valeur2 = ligne_lu.section(sep,2,2);
            valeur3 = ligne_lu.section(sep,3,3);
            MAJAddTableTopographie2(valeur1,valeur2,valeur3);
        }
        nb++;
    }
    buffer->close();
    delete buffer;
    return nb;
}
// ------------------------------------ MAJAddTableAcces1 ------------------------------------
bool CMoteurCCAM_Base::MAJAddTableAcces1(QString &valeur1,QString &valeur2,QString &valeur3)
{
    int ok = FALSE;
    Q3SqlCursor cur("ACCES1", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    valeur1);
    buffer->setValue( "ACCES",   valeur2);
    buffer->setValue( "LIBELLE", valeur3);
    if (cur.insert())
    {ok = TRUE;
    }
    else qDebug(cur.lastError().text());
    return ok;
}
// ------------------------------------ MAJAddTableAcces2 ------------------------------------
bool CMoteurCCAM_Base::MAJAddTableAcces2(QString &valeur1,QString &valeur2)
{
    int ok = FALSE;
    Q3SqlCursor cur("ACCES2", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "PERE",    valeur1);
    buffer->setValue( "LIBELLE", valeur2);
    if (cur.insert())
    {ok = TRUE;
    }
    else qDebug(cur.lastError().text());
    return ok;
}
// ------------------------------------ MAJAddTableAction1 ------------------------------------
bool CMoteurCCAM_Base::MAJAddTableAction1(QString &valeur1,QString &valeur2,QString &valeur3)
{
    int ok = FALSE;
    Q3SqlCursor cur("ACTION1", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "VERBE",    valeur1);
    buffer->setValue( "CODE",   valeur2);
    buffer->setValue( "LIBELLE", valeur3);
    if (cur.insert())
    {ok = TRUE;
    }
    else qDebug(cur.lastError().text());
    return ok;
}
// ------------------------------------ MAJAddTableAction2 ------------------------------------
bool CMoteurCCAM_Base::MAJAddTableAction2(QString &valeur1,QString &valeur2,QString &valeur3)
{
    int ok = FALSE;
    Q3SqlCursor cur("ACTION2", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "VERBE",    valeur1);
    buffer->setValue( "CODE",   valeur2);
    buffer->setValue( "SUBSTANTIF", valeur3);
    if (cur.insert())
    {ok = TRUE;
    }
    else qDebug(cur.lastError().text());
    return ok;
}
// ------------------------------------ MAJAddTableTopographie1 ------------------------------------
bool CMoteurCCAM_Base::MAJAddTableTopographie1(QString &valeur1, QString &valeur2)
{
    int ok = FALSE;
    Q3SqlCursor cur("TOPOGRAPHIE1", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    valeur1);
    buffer->setValue( "LIBELLE", valeur2);
    if (cur.insert())
    {ok = TRUE;
    }
    else qDebug(cur.lastError().text());
    return ok;
}
// ------------------------------------ MAJAddTableTopographie2 ------------------------------------
bool CMoteurCCAM_Base::MAJAddTableTopographie2(QString &valeur1, QString &valeur2, QString &valeur3)
{
    int ok = FALSE;
    Q3SqlCursor cur("TOPOGRAPHIE2", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    valeur1);
    buffer->setValue( "PERE",    valeur2);
    buffer->setValue( "LIBELLE", valeur3);
    if (cur.insert())
    {ok = TRUE;
    }
    else qDebug(cur.lastError().text());
    return ok;
}
// ********************************************************************************************************************************
// Debut des mise �  jour pour les tarifs
// ********************************************************************************************************************************

//---------------------------------------------------- MAJAddTabTB03 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB03(TAB_TB03 *tabtb03)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 5) && (tabtb03[i].m_RegleTarifaire != "")) {
        requete = "INSERT INTO TB03 (REGLE , DATEDEBUT , DATEFIN , COEFFICIENT) VALUES (";
        requete += "'" + tabtb03[i].m_RegleTarifaire + "',";
        requete += "'" + tabtb03[i].m_DateDebut + "',";
        requete += "'" + tabtb03[i].m_DateFin + "',";
        requete += "'" + tabtb03[i].m_Coefficient + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB04 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB04(TAB_TB04 *tabtb04)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 6) && (tabtb04[i].m_Specialite != "00")) {
        requete = "INSERT INTO TB04 (SPECIALITE , DATEDEBUT , DATEFIN , CLASSE_SPECIALITE) VALUES (";
        requete += "'" + tabtb04[i].m_Specialite + "',";
        requete += "'" + tabtb04[i].m_DateDebut + "',";
        requete += "'" + tabtb04[i].m_DateFin + "',";
        requete += "'" + tabtb04[i].m_ClasseSpecialite + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB05 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB05(TAB_TB05 *tabtb05)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 6) && (tabtb05[i].m_NatPrestation != "")) {
        requete = "INSERT INTO TB05 (NAT_PRESTATION , DATEDEBUT , DATEFIN , TYPE_FORFAIT) VALUES (";
        requete += "'" + tabtb05[i].m_NatPrestation + "',";
        requete += "'" + tabtb05[i].m_DateDebut + "',";
        requete += "'" + tabtb05[i].m_DateFin + "',";
        requete += "'" + tabtb05[i].m_TypeForfait + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB06 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB06(TAB_TB06 *tabtb06)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 4) && (tabtb06[i].m_CodeModificateur != "")) {
        requete = "INSERT INTO TB06 (CODE_MODIF , DATEDEBUT , DATEFIN , UNITE_AGEMINI , AGEMINI , UNITE_AGEMAXI , AGEMAXI) VALUES (";
        requete += "'" + tabtb06[i].m_CodeModificateur + "',";
        requete += "'" + tabtb06[i].m_DateDebut + "',";
        requete += "'" + tabtb06[i].m_DateFin + "',";
        requete += "'" + tabtb06[i].m_UniteAgeMini + "',";
        requete += "'" + tabtb06[i].m_AgeMini + "',";
        requete += "'" + tabtb06[i].m_UniteAgeMaxi + "',";
        requete += "'" + tabtb06[i].m_AgeMaxi + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB07 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB07(TAB_TB07 *tabtb07)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 2) && (tabtb07[i].m_DateDebut != "00000000")) {
        requete = "INSERT INTO TB07 (DATEDEBUT , DATEFIN , SEUIL_MET , SEUIL_ANT , SEUIL_REU , SEUIL_GUY) VALUES (";
        requete += "'" + tabtb07[i].m_DateDebut + "',";
        requete += "'" + tabtb07[i].m_DateFin + "',";
        requete += "'" + tabtb07[i].m_S1 + "',";
        requete += "'" + tabtb07[i].m_S2 + "',";
        requete += "'" + tabtb07[i].m_S3 + "',";
        requete += "'" + tabtb07[i].m_S4 + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB08 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB08(TAB_TB08 *tabtb08)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 4) && (tabtb08[i].m_TypeFerie != "")) { // !(tabtb08[i].m_TypeFerie = "" && tabtb08[i].m_Caisse = "000")
        requete = "INSERT INTO TB08 (CAISSE , DATEDEBUT , DATEFIN , TYPE_FERIE , JOUR) VALUES (";
        requete += "'" + tabtb08[i].m_Caisse + "',";
        requete += "'" + tabtb08[i].m_DateDebut + "',";
        requete += "'" + tabtb08[i].m_DateFin + "',";
        requete += "'" + tabtb08[i].m_TypeFerie + "',";
        requete += "'" + tabtb08[i].m_Jour + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB09 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB09(TAB_TB09 *tabtb09)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 6) && (tabtb09[i].m_CodeModif != "")) {
        requete = "INSERT INTO TB09 (MODIFICATEUR , COMPATIBLE , DATEDEBUT , DATEFIN) VALUES (";
        requete += "'" + tabtb09[i].m_CodeModif + "',";
        requete += "'" + tabtb09[i].m_ModifCompat + "',";
        requete += "'" + tabtb09[i].m_DateDebut + "',";
        requete += "'" + tabtb09[i].m_DateFin + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB10 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB10(TAB_TB10 *tabtb10)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 6) && (tabtb10[i].m_Modificateur != "")) {
        requete = "INSERT INTO TB10 (MODIFICATEUR , DATEDEBUT , DATEFIN , COHERENCE , PRES_MULTIPLE) VALUES (";
        requete += "'" + tabtb10[i].m_Modificateur + "',";
        requete += "'" + tabtb10[i].m_DateDebut + "',";
        requete += "'" + tabtb10[i].m_DateFin + "',";
        requete += "'" + tabtb10[i].m_Coherence + "',";
        requete += "'" + tabtb10[i].m_PresMultiple + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB11 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB11(TAB_TB11 *tabtb11)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 4) && (tabtb11[i].m_Modificateur != "")) {
        requete = "INSERT INTO TB11 (MODIFICATEUR , DATEDEBUT , DATEFIN , FORFAIT , COEF) VALUES (";
        requete += "'" + tabtb11[i].m_Modificateur + "',";
        requete += "'" + tabtb11[i].m_DateDebut + "',";
        requete += "'" + tabtb11[i].m_DateFin + "',";
        requete += "'" + tabtb11[i].m_Forfait + "',";
        requete += "'" + tabtb11[i].m_Coef + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB12 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB12(TAB_TB12 *tabtb12)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 4) && (tabtb12[i].m_DateDebut != "00000000")) {
        requete = "INSERT INTO TB12 (DATEDEBUT , DATEFIN , FORFAIT , COEF) VALUES (";
        requete += "'" + tabtb12[i].m_DateDebut + "',";
        requete += "'" + tabtb12[i].m_DateFin + "',";
        requete += "'" + tabtb12[i].m_Forfait + "',";
        requete += "'" + tabtb12[i].m_Coef + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB13 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB13(TAB_TB13 *tabtb13)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 6) && (tabtb13[i].m_Prestation != "")) {
        requete = "INSERT INTO TB13 (PRESTATION , DATEDEBUT , DATEFIN , EQUIVAL) VALUES (";
        requete += "'" + tabtb13[i].m_Prestation + "',";
        requete += "'" + tabtb13[i].m_DateDebut + "',";
        requete += "'" + tabtb13[i].m_DateFin + "',";
        requete += "'" + tabtb13[i].m_Equiv + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB14 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB14(TAB_TB14 *tabtb14)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 5) && (tabtb14[i].m_Dmt != "000")) {
        requete = "INSERT INTO TB14 (DMT , DATEDEBUT , DATEFIN , CLASSE_DMT) VALUES (";
        requete += "'" + tabtb14[i].m_Dmt + "',";
        requete += "'" + tabtb14[i].m_DateDebut + "',";
        requete += "'" + tabtb14[i].m_DateFin + "',";
        requete += "'" + tabtb14[i].m_Classe + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB15 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB15(TAB_TB15 *tabtb15)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 6) && (tabtb15[i].m_Modificateur != "")) {
        requete = "INSERT INTO TB15 (MODIFICATEUR , DATEDEBUT , DATEFIN , MODIF_OC) VALUES (";
        requete += "'" + tabtb15[i].m_Modificateur + "',";
        requete += "'" + tabtb15[i].m_DateDebut + "',";
        requete += "'" + tabtb15[i].m_DateFin + "',";
        requete += "'" + tabtb15[i].m_ModifOC + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB17 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB17(TAB_TB17 *tabtb17)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 9) && (tabtb17[i].m_Prestation != "")) {
        requete = "INSERT INTO TB17 (PRESTATION , SPECIALITE , DATE_OBLIG) VALUES (";
        requete += "'" + tabtb17[i].m_Prestation + "',";
        requete += "'" + tabtb17[i].m_Specialite + "',";
        requete += "'" + tabtb17[i].m_DateObligation + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB18 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB18(TAB_TB18 *tabtb18)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 6) && (tabtb18[i].m_DateDebut != "00000000")) {
        requete = "INSERT INTO TB18 (DATEDEBUT , DATEFIN , LOCALISATION) VALUES (";
        requete += "'" + tabtb18[i].m_DateDebut + "',";
        requete += "'" + tabtb18[i].m_DateFin + "',";
        requete += "'" + tabtb18[i].m_Localisation + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB19 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB19(TAB_TB19 *tabtb19)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 7) && (tabtb19[i].m_DateDebut != "00000000")) {
        requete = "INSERT INTO TB19 (DATEDEBUT , DATEFIN , NB_MODIF) VALUES (";
        requete += "'" + tabtb19[i].m_DateDebut + "',";
        requete += "'" + tabtb19[i].m_DateFin + "',";
        requete += "'" + tabtb19[i].m_NbModif + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddTabTB20 -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTabTB20(TAB_TB20 *tabtb20)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 4) && (tabtb20[i].m_Concept != "00")) {
        requete = "INSERT INTO TB20 (CONCEPT , DATEDEBUT , DATEFIN , VALEUR) VALUES (";
        requete += "'" + tabtb20[i].m_Concept + "',";
        requete += "'" + tabtb20[i].m_DateDebut + "',";
        requete += "'" + tabtb20[i].m_DateFin + "',";
        requete += "'" + tabtb20[i].m_Valeur + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddCondGenerales -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddCondGenerales(TAB_CONDGEN &tabcondgen)
{
    int ok = FALSE;
    Q3SqlCursor cur("COND_GEN", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabcondgen.m_Condition);
    buffer->setValue( "LIBELLE", (const char*) tabcondgen.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddClasseDmt -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddClasseDmt(TAB_CLASSE_DMT &tabclassedmt)
{
    int ok = FALSE;
    Q3SqlCursor cur("CLASSE_DMT", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabclassedmt.m_ClasseDMT);
    buffer->setValue( "LIBELLE", (const char*) tabclassedmt.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddExoTm -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddExoTm(TAB_EXO_TM &tabexotm)
{
    int ok = FALSE;
    Q3SqlCursor cur("EXO_TM", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabexotm.m_CodeExo);
    buffer->setValue( "LIBELLE", (const char*) tabexotm.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddNatAssurance -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddNatAssurance(TAB_NAT_ASS &tabnatass)
{
    int ok = FALSE;
    Q3SqlCursor cur("NAT_ASS", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabnatass.m_NatAss);
    buffer->setValue( "LIBELLE", (const char*) tabnatass.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddAdmRemboursement -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddAdmRemboursement(TAB_REMBOURS &tabrembours)
{
    int ok = FALSE;
    Q3SqlCursor cur("ADM_REMB", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabrembours.m_AdmRem);
    buffer->setValue( "LIBELLE", (const char*) tabrembours.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddFraisDeplacement -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddFraisDeplacement(TAB_FRAIS_DEP &tabfraisdep)
{
    int ok = FALSE;
    Q3SqlCursor cur("FRAIS_DEP", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabfraisdep.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabfraisdep.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddTypeActe -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTypeActe(TAB_TYPE_ACTE &tabtypeacte)
{
    int ok = FALSE;
    Q3SqlCursor cur("TYPE_ACTE", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabtypeacte.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabtypeacte.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddTypeForfait -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddTypeForfait(TAB_TYPE_FORFAIT &tabtypeforfait)
{
    int ok = FALSE;
    Q3SqlCursor cur("TYPE_FORFAIT", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabtypeforfait.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabtypeforfait.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddExtDoc -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddExtDoc(TAB_EXT_DOC &tabextdoc)
{
    int ok = FALSE;
    Q3SqlCursor cur("EXT_DOC", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabextdoc.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabextdoc.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddCatMed -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddCatMed(TAB_CAT_MED &tabcatmed)
{
    int ok = FALSE;
    Q3SqlCursor cur("CAT_MED", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabcatmed.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabcatmed.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddCodRegroup -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddCodRegroup(TAB_COD_REGROUP &tabcodregroup)
{
    int ok = FALSE;
    Q3SqlCursor cur("COD_REGROUP", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabcodregroup.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabcodregroup.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddCatSpec -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddCatSpec(TAB_CAT_SPEC &tabcatspec)
{
    int ok = FALSE;
    Q3SqlCursor cur("CAT_SPEC", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabcatspec.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabcatspec.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddAgrRadio -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddAgrRadio(TAB_AGR_RAD &tabagrrad)
{
    int ok = FALSE;
    Q3SqlCursor cur("AGR_RADIO", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabagrrad.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabagrrad.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddCodePaiement -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddCodePaiement(TAB_CODE_PAIEMENT &tabcodepaiement)
{
    int ok = FALSE;
    Q3SqlCursor cur("CODE_PAIEMENT", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabcodepaiement.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabcodepaiement.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddNumDents -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddNumDents(TAB_NUM_DENTS &tabnumdents)
{
    int ok = FALSE;
    Q3SqlCursor cur("NUM_DENTS", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabnumdents.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabnumdents.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddDom -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddDom(TAB_DOM &tabdom)
{
    int ok = FALSE;
    Q3SqlCursor cur("DOM", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabdom.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabdom.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddAssNonPrevue -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddAssNonPrevue(TAB_ASS_NPREV &tabassnprev)
{
    int ok = FALSE;
    Q3SqlCursor cur("ASS_NPREV", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabassnprev.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabassnprev.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddRglTarif -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddRglTarif(TAB_RGL_TARIF &tabrgltarif)
{
    int ok = FALSE;
    Q3SqlCursor cur("RGL_TARIF", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabrgltarif.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabrgltarif.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddSpecialite -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddSpecialite(TAB_SPEC &tabspec)
{
    int ok = FALSE;
    Q3SqlCursor cur("SPECIALITES", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabspec.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabspec.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddDMT -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddDMT(TAB_DMT &tabdmt)
{
    int ok = FALSE;
    Q3SqlCursor cur("DMT", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODE",    tabdmt.m_Code);
    buffer->setValue( "LIBELLE", (const char*) tabdmt.m_Libelle);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddCompatExoTM -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddCompatExoTM(TAB_COMPAT_EXOTM *tabcompatexotm)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 60) && (tabcompatexotm[i].m_ExoTM != "0")) {
        requete = "INSERT INTO COMPAT_EXO_TM (EXOTM , EXOTMCOMPAT) VALUES (";
        requete += "'" + tabcompatexotm[i].m_ExoTM + "',";
        requete += "'" + tabcompatexotm[i].m_ExoTMCompat + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddValUnitOeuvre -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddValUnitOeuvre(TAB_VAL_UNITE_OEUVRE *tabvalunitoeuvre)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 7) && (tabvalunitoeuvre[i].m_Code != "")) {
        requete = "INSERT INTO VAL_UNITE_OEUVRE (CODE , DATEEFFET , VALEUR) VALUES (";
        requete += "'" + tabvalunitoeuvre[i].m_Code + "',";
        requete += "'" + tabvalunitoeuvre[i].m_DateEffet + "',";
        requete += "'" + tabvalunitoeuvre[i].m_Valeur + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddCondGenActe -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddCondGenActe(QString &codeacte,COND_GEN_ACTE *condgenacte)
{
    QString requete = "";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    int i=0;
    while ((i < 30) && (condgenacte[i].m_Code != "0000")) {
        requete = "INSERT INTO COND_GEN_ACTE (ACTE , CONDITION) VALUES (";
        requete += "'" + codeacte + "',";
        requete += "'" + condgenacte[i].m_Code + "');";
        query_insert.exec(requete);
        i++;
    }
    return TRUE;
}

//---------------------------------------------------- MAJAddRecMedActe -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddRecMedActe(QString &codeacte,QString &activite,REC_MED &recmedacte)
{
    int ok = FALSE;
    Q3SqlCursor cur("REC_MED_ACTE", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "ACTE",       codeacte);
    buffer->setValue( "ACTIVITE",   activite);
    buffer->setValue( "RECMED",     recmedacte.m_NumRecMed);
    buffer->setValue( "TEXTE",      recmedacte.m_TexteRecMed.stripWhiteSpace());
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddListeMots -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddListeMots(LISTE_MOTS &liste_mots)
{
    int ok = FALSE;
    Q3SqlCursor cur("LISTE_MOTS", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "MOT",        liste_mots.m_Mot);
    buffer->setValue( "DEFINITION", (const char*) liste_mots.m_Definition.stripWhiteSpace());
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

//---------------------------------------------------- MAJAddHistorique -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJAddHistorique(ENT_HISTO_PHASE &historique)
{
    int ok = FALSE;
    Q3SqlCursor cur("HISTO_PHASE", TRUE, m_obj_CCAM_Base);
    if ( !cur.canInsert ()) return ok;
    QSqlRecord *buffer = cur.primeInsert();
    buffer->setValue( "CODEACTE",     historique.m_CodeActe);
    buffer->setValue( "ACTIVITE",     historique.m_CodeActivite);
    buffer->setValue( "PHASE",        historique.m_CodePhase);
    buffer->setValue( "DATE_EFFET",   historique.m_DateEffet);
    buffer->setValue( "DATE_AM",      historique.m_DateAM);
    buffer->setValue( "DATE_JO",      historique.m_DateJO);
    buffer->setValue( "NBMAX",        historique.m_NbMaxSeances);
    buffer->setValue( "UO",           historique.m_UniteOeuvre);
    buffer->setValue( "COEF_UO",      historique.m_CoefUO);
    buffer->setValue( "CODEPAIEMENT", historique.m_CodePaiement);
    buffer->setValue( "PU",           historique.m_PU);
    buffer->setValue( "MAJDOM1",      historique.m_CoefMAJDom1);
    buffer->setValue( "MAJDOM2",      historique.m_CoefMAJDom2);
    buffer->setValue( "MAJDOM3",      historique.m_CoefMAJDom3);
    buffer->setValue( "MAJDOM4",      historique.m_CoefMAJDom4);
    buffer->setValue( "SUPP_CHG",     historique.m_SuppChgCab);
    if (cur.insert())
    {ok = TRUE;
    }
    return ok;
}

// ------------------------------------ GetTableMedecins ------------------------------------
QString CMoteurCCAM_Base::GetTableMedecins()
{
    QString retour  = "";
    QString requete = "";
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)  return 0;
    QSqlQuery query(QString::null, m_obj_User_Base);
    requete = "SELECT "                     +
               m_USER_IDENT_SIGEMS_LOG +"," +   //  0
               m_USER_IDENT_SIGEMS_PAS +"," +   //  1
               m_USER_IDENT_NOM        +"," +   //  2
               m_USER_IDENT_PRENOM     +"," +   //  3
               m_USER_IDENT_NUM_ORDRE  +"," +   //  4
               m_USER_IDENT_CODE_SPEC  +" FROM " + m_USER_IDENT_TBL_NAME ;
    query.exec(requete);
    while (query.next()) {
        retour += "MED##.##" + query.value(0).toString();
        retour += "##.##"    + query.value(1).toString();
        retour += "##.##"    + query.value(2).toString();
        retour += "##.##"    + query.value(3).toString();
        retour += "##.##"    + query.value(4).toString();
        retour += "##.##"    + query.value(5).toString() + "##.##\r\n";
    }
    retour.replace(0x92,0x27);
    return retour;
}
 /*
         informations.m_Code              = query.value(i++).toString();
         informations.m_Password          = query.value(i++).toString();
         informations.m_CodeMedinTux      = query.value(i++).toString();
         informations.m_PasswordMedinTux  = query.value(i++).toString();
         informations.m_Nom               = query.value(i++).toString();
         informations.m_Prenom            = query.value(i++).toString();
         informations.m_Nid               = query.value(i++).toString();
         informations.m_Spec              = query.value(i++).toString();

    //................. USER_IDENT ............................
   QString      m_USER_IDENT_TBL_NAME;           // nom de la table des données des RUBRIQUES
   QString      m_USER_IDENT_TYPE_TABLE;         // SIGEMS pure / MedinTux
   QString      m_USER_IDENT_NOM;                // Nom
   QString      m_USER_IDENT_PRENOM;             // Prenom
   QString      m_USER_IDENT_TITRE;              // Titre( Dr Mr Md Pr ...)
   QString      m_USER_IDENT_LOGIN;              // Identifiant d'utilisateur DrTUX
   QString      m_USER_IDENT_PASSWORD;           // mot de passe              DrTux
   QString      m_USER_IDENT_DROITS;             // Droits de cet utilisateur
   QString      m_USER_IDENT_NUM_ORDRE;          // Numéro d'ordre (si il y en a un)
   QString      m_USER_IDENT_PRIMKEY;            // clef primaire de cet enregistrement
   QString      m_USER_IDENT_GUID;               // Identifiant unique de cet utilisateur
   QString      m_USER_IDENT_CONVENTION;         // convention exple "01 MEDECINE GENERALE"
   QString      m_USER_IDENT_CODE_POST;          // Code postal
   QString      m_USER_IDENT_VILLE;              // Ville d'exercice
   QString      m_USER_IDENT_RUE;                // Rue d'exercice
   QString      m_USER_IDENT_QUALITE;            // Type d'intervenant (exple: Medecin Urgentiste)
   QString      m_USER_IDENT_SIGEMS_LOG;         // login    SIGEMS
   QString      m_USER_IDENT_SIGEMS_PAS;         // password SIGEMS
   QString      m_USER_IDENT_CODE_SPEC;          // password SIGEMS

   typedef struct _INFO_MED
      {QString m_Code;                  // 0
       QString m_Password;              // 1
       QString m_CodeMedinTux;          // 2
       QString m_PasswordMedinTux;      // 3
       QString m_Nom;                   // 4
       QString m_Prenom;                // 5
       QString m_Nid;                   // 6
       QString m_Spec;                  // 7
      }INFO_MED;
         for (i=0; i<7; i++) informations << query.value(i).toString();
         */
// ------------------------------------ ImportTableMedecins ------------------------------------
long CMoteurCCAM_Base::ImportTableMedecins(QWidget* parent,QString &fichier)
{
    long nb=0;
    QFile *buffer = new QFile(fichier);
    QString codeMedecin;
    INFO_MED infoMedecin;
    QRegExp sep("##.##");
    if ( buffer->size() < 2) return -1;
    if (buffer->open( QIODevice::ReadOnly )==FALSE) return -1;
    QString ligne_lu = "";
    while (buffer->readLine((char*)(const char*)ligne_lu,400) != -1){
        if (nb == 0 && ligne_lu.mid(0,3) != "MED") {
            buffer->close();
            delete buffer;
            return -1;
        }
        if (ligne_lu.mid(0,3) == "MED") {
            codeMedecin = ligne_lu.section(sep,1,1);
            if (MedecinExiste(codeMedecin)) {
                QMessageBox mb( TR("Importer la table des médecins"),
                                TR("Le medecin ") + codeMedecin + TR(" existe déj� .\n\n"
                                "Voulez-vous le remplacer par celui-ci ?\n\n")
                                + codeMedecin,
                                QMessageBox::Information,
                                QMessageBox::Yes | QMessageBox::Default,
                                QMessageBox::No,
                                QMessageBox::Cancel,
                                parent);
                mb.setButtonText( QMessageBox::Yes, TR("&Oui") );
                mb.setButtonText( QMessageBox::No, TR("&Non") );
                switch( mb.exec() ) {
                case QMessageBox::Yes:
                    // save and exit
                    break;
                case QMessageBox::No:
                    // exit without saving
                    break;
                case QMessageBox::Cancel:
                    // exit without doing anything
                    break;
                }
            }
            else {
                //infoMedecin.clear();
        int cpt = 1;
        infoMedecin.m_Code              = ligne_lu.section(sep,cpt,cpt); ++cpt;
        infoMedecin.m_Password          = ligne_lu.section(sep,cpt,cpt); ++cpt;
        infoMedecin.m_CodeMedinTux      = "";
        infoMedecin.m_PasswordMedinTux  = "";
        infoMedecin.m_Nom               = ligne_lu.section(sep,cpt,cpt); ++cpt;
        infoMedecin.m_Prenom            = ligne_lu.section(sep,cpt,cpt); ++cpt;
        infoMedecin.m_Nid               = ligne_lu.section(sep,cpt,cpt); ++cpt;
        infoMedecin.m_Spec              = ligne_lu.section(sep,cpt,cpt); ++cpt;

                /*
      typedef struct _INFO_MED
      {QString m_Code;
       QString m_Password;
       QString m_CodeMedinTux;
       QString m_PasswordMedinTux;
       QString m_Nom;
       QString m_Prenom;
       QString m_Nid;
       QString m_Spec;
       QString m_Pk;
      }INFO_MED;
                QMessageBox debugBox("debug",
                         infoMedecin.join(";"),
                         QMessageBox::Information,
                         QMessageBox::Yes,
                         QMessageBox::No,
                         QMessageBox::NoButton,
                         parent);
                debugBox.exec();
                */
                MAJMedecins(infoMedecin);
            }
        }
        nb++;
    }
    buffer->close();
    delete buffer;
    return nb;
}
//---------------------------------------------------- MAJMedecins -----------------------------------------------------------
bool CMoteurCCAM_Base::MAJMedecins( INFO_MED &info_med )
{  //............... chaines d'erreurs  ..................................................
    if  (m_pUser_Base==0){qDebug(TR("Erreur : m_pUser_Base : non initialisée"));      return FALSE;}
    if  (m_pUser_Base->isOpen()==FALSE && m_pUser_Base->open()==FALSE)          return FALSE;

        QString requete = "";
        //UPDATE tbl_name SET row_flag=1 WHERE id=ID AND row_flag <> 1;
        requete  = QString("UPDATE ")      + m_USER_IDENT_TBL_NAME                   + " SET ";
        requete += m_USER_IDENT_SIGEMS_LOG + "='"     + info_med.m_Code              + "', "+
                   m_USER_IDENT_SIGEMS_PAS + "='"     + info_med.m_Password          + "', "+
                   m_USER_IDENT_LOGIN      + "='"     + info_med.m_CodeMedinTux      + "', "+
                   m_USER_IDENT_PASSWORD   + "='"     + info_med.m_PasswordMedinTux  + "', "+
                   m_USER_IDENT_NOM        + "='"     + info_med.m_Nom               + "', "+
                   m_USER_IDENT_PRENOM     + "='"     + info_med.m_Prenom            + "', "+
                   m_USER_IDENT_NUM_ORDRE  + "='"     + info_med.m_Nid               + "', "+
                   m_USER_IDENT_CODE_SPEC  + "='"     + info_med.m_Spec              + "' WHERE " +
                   m_USER_IDENT_PRIMKEY    + "='"     + info_med.m_Pk + "';";
        QSqlQuery query(requete, m_obj_User_Base);
    return TRUE;
}



// ********************************************************************************************************************************
// fin des mise �  jour pour les tarifs
// ********************************************************************************************************************************

//---------------------------------------------------- DeleteAllData -----------------------------------------------------------
bool CMoteurCCAM_Base::DeleteAllData(const QString &nom_table)
{
    QString requete = "DELETE FROM " + nom_table + ";";
    QSqlQuery query_insert(QString::null, m_obj_CCAM_Base);
    query_insert.exec(requete);
    return TRUE;
}

//-----------------------------------------------------  PassWordEncode --------------------------------------------------
// Algo minimal pour l'instant
// chaque octet tourne de sa position dans la chaine + 1, et Xor final avec le mot de passe en clair puis converti en hexa
// resultat final en hexa;
int CMoteurCCAM_Base::PassWordEncode(QString &pass)
{char str[34];
 UINT16    val;
 int len         = pass.length();

 if (len>32)       return ERR_PASS_MAX;
 if (len==0)       return ERR_PASS_ZERO;
 strcpy(str , pass);
 pass = "";
 for (int i=0; i<len; ++i)
     { val   =  str[i];
       val   =  (val << ((i+1)&0x000F) )^val;
      pass  += UINTtoHex(val);
     }
 return ERR_PASS_OK;
}

//-----------------------------------------------------  PassWordDecode --------------------------------------------------
// Algo minimal pour l'instant
int CMoteurCCAM_Base::PassWordDecode(const char* str_to_decode, const char* str_pass)
{char     buf[16];
 UINT16   val;
 if (str_to_decode == 0 || str_pass == 0) return ERR_PASS_MAX;
 int len_to_decode      = strlen(str_to_decode);
 if (len_to_decode==0)             return ERR_PASS_ZERO;
 int len_pass           = strlen(str_pass);
 if (len_pass != len_to_decode/4)  return ERR_PASS_MAX;
 QString result;
 int end = len_to_decode/4;
 buf[4 ] = 0;
 for (int i=0; i<end; ++i)
     {memcpy(buf, str_to_decode + i*4, 4);  // recuperer les 4 caractere hexa
      val = HexToUINT(buf);                 // les convertir en UINT16
      val = val ^ str_pass[i];              // xor avec le mot de passe
      val = val >> ((i+1)&0x000F);          // rotation inverse maxi de 8 et on devrait avoir le code ASCII
      if (val != str_pass[i])      return ERR_PASS_MAX;    // erreur si pas egal
     }
 return ERR_PASS_OK;
}

//-----------------------------------------------------  UINTtoHex --------------------------------------------------------
QString CMoteurCCAM_Base::UINTtoHex( UINT16 val)
{char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};   // l�  on peut mettre une table Bidon
 QString res="";
 res += hex[(val>>12)& 0x000F];
 res += hex[(val>>8) & 0x000F];
 res += hex[(val>>4) & 0x000F];
 res += hex[val&0x000F];
 return res;
}

//-----------------------------------------------------  HexToUINT --------------------------------------------------------
UINT16 CMoteurCCAM_Base::HexToUINT( const char *str)
{int i   = strlen(str);
 if (i==0) return 0;
 UINT16 val = 0;
 int    dec = 0;
 while (i>0)
   {--i;
    unsigned char car = str[i] | 32;     // mettre en minuscules
    if      (car>='a'&& car<= 'f')  val  += (car-'W')<<dec;  //a (97) - W (87) --> 10
    else                            val  += (car-'0')<<dec;  //0 (48) - 0 (48) --> 0
    dec += 4;
   }
 return val;
}
