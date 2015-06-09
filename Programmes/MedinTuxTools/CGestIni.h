/******************************** CGestIni.h *******************************
 *   Copyright (C) 2004 by Sébastien SIVERA & Sevin Roland                 *
 *   upsis@club-internet.fr                                                *
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
#ifndef  CGESTINI_H
#define  CGESTINI_H

//=============================================== INCLUDES ===================================================================
#include <qstring.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qsqlquery.h>
#include <qthread.h>
//================================================= CForSleep ==============================================================

    class CForSleep : public QThread
          {public:
                CForSleep(){}
                ~CForSleep(){}
                static void sleep(unsigned long secs){QThread::sleep(secs);}
                virtual void   run(){}
          };
    #define SLEEP(a) CForSleep::sleep((a))

//===================================== CGestIni =============================================================================
class CGestIni
{
 //............................... METHODES .........................................................................
public:
     CGestIni();
    ~CGestIni(){}
     //---------------------------- addXmlData ------------------------------------------------
     /*! \brief ajoute dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
      *   dans cet exemple la valeur serait ' et de fin' (sans les apostrophes)
      *  \param _tagXml : String indiquant le nom du tag
      *  \param valeur :  valeur a encadrer par les tags xml
      *  \param modeleXML :  String XML dans laquelle on va ajouter la valeur
      *  \param ofset :  QString ofset de decalage vers la droite par defaut vide
      *  \param mustBeB64Protected :  si true la valeur sera convertie et inscrite en base 64.
     */
     static void addXmlData(const QString& _tagXml, QString data, QString &modeleXML, bool mustBeB64Protected=FALSE, const QString &ofset ="" );

     //---------------------------- getXmlData --------------------------------------------------------------------
     /*! \brief retourne dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
      *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
      *  \param tagXml : String indiquant le nom du tag  sans <  />
      *  \param dataXml :  String XML dans laquelle on va lire la valeur
      *  \param nextPos :  pointeur sur un int qui si il n'est pas egal a zero indique
      *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherché.
      *  \param noConvertCharToHtml  if zero (default value) '>' '<' '&' are  converted in '&gt;' '&lt;' '&amp;'
      *  \return QString qui est la valeur a rechercher.
     */
     static QString getXmlData(const QString& tagXml, const QString& dataXml, int *nextPos=0, int noConvertCharToHtml  =0 );
     //---------------------------- getXmlDataList --------------------------------------------------------------------
     /*! \brief retourne une QStringList de valeurs à partir d'un fichier XML et d'un tag
      *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
      *  \param tagName :  QString indiquant le nom du tag  sans les </ ou < ou >
      *  \param dataXml :  QString XML dans laquelle on va lire la valeur
      *  \param nextPos :  pointeur sur un int qui si il n'est pas egal a zero indique
      *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherché.
      *  \param noConvertCharToHtml  if zero (default value) '>' '<' '&' are  converted in '&gt;' '&lt;' '&amp;'
      *  \return QStringList qui est la liste de valeurs a rechercher.
     */
     static QStringList getXmlDataList(const QString& tagName, const QString& dataXml, int *nextPos=0, int noConvertCharToHtml = 0 );

     //---------------------------- setXmlData static ------------------------------------------------
     /*! \brief place dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
      *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
      *  \param tagXml : String indiquant le nom du tag  sans <  />
      *  \param valeur :    QString valeur a ecrire
      *  \param dataXml :  String XML dans laquelle on va ecrire la valeur
      *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherché.
      *  \param noConvertCharToHtml  if zero (default value) '>' '<' '&' are  converted in '&gt;' '&lt;' '&amp;'
      *  \return true si tout est ok false sinon (en general le tag n'a pas ete trouve)
     */
     static bool setXmlData(const QString& tagXml,   QString valeur, QString& dataXml, int noConvertCharToHtml  =0 );


      //--------------------------------------------- isInBlackList --------------------------------------------
      /*! \brief verifie si le terme name est concerne par la blackList
      *  \param black_str :      const QString & black_str 
      *  \param pathDoc :   const QString & name terme dont il faut verifie si concerne
      *  \return int :   vrai/1 si concerne    faux/0 si non concerne
      */
     static int  isInBlackList( QString black_str, const QString &name);

     //--------------------------------------------- isInBlackList --------------------------------------------
      /*! \brief verifie si le terme name est concerne par la blackList "
      *  \param black_list :      const QStringList & black_list 
      *  \param pathDoc :   const QString & name terme dont il faut verifie si concerne
      *  \return int :   vrai/1 si concerne    faux/0 si non concerne
     */
     static int isInBlackList(const QStringList &black_list, const QString &name);

  static int            findFermant(const QString *ptext, int pos_deb, int pos_max, const QString &ouvrant, const QString &fermant);
  static int            findFermant(const QString *ptext, int pos_deb, int pos_max, const QString &ouvrant, const QString &fermant, int &isWithToken);
  static void           Param_UpdateToDisk(const QString &file_ini, const QString &inParam);
  static long           Param_UpdateFromDisk(const QString &file_ini, QString &outParam, int *isUtf8 = 0 );
  static QString        Param_UpdateFromDisk(const QString &file_ini);
  static QString        Param_WriteParam(QString *pQstr, const char *section, const char  *variable,
                                         const char *val1=0, const char *val2=0, const char *val3=0, const char *val4=0, const char *val5=0,
                                         const char *val6=0, const char *val7=0, const char *val8=0, const char *val9=0, const char *val10=0);
  static char          *Param_ExtraireValeurs(const char* txt,
                                              QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                              QString *val6, QString *val7, QString *val8, QString *val9, QString *val10);
  static char          *Param_ExtraireNextValeurs(char* pt, QString &var_name, char**pt_lgn, long *len, QStringList &lst);
  static char          *Param_ExtraireValeurs(const char* txt, QStringList &lst);
  static QString        Param_ReadUniqueParam(const char* txt, const char *section, const char  *variable);
  static QString        Param_ReadParam(const char* txt, const char *section, const char  *variable,
                                        QString *val1=0, QString *val2=0, QString *val3=0, QString *val4=0, QString *val5=0,
                                        QString *val6=0, QString *val7=0, QString *val8=0, QString *val9=0, QString *val10=0);
  static char          *Param_GotoNextSection(char *pt, const char* section=0, QString *pQsection=0);
  static char          *Param_ExtraireNextValeurs(char* pt, QString &var_name,
                                           QString *val1=0, QString *val2=0, QString *val3=0, QString *val4=0, QString *val5=0,
                                           QString *val6=0, QString *val7=0, QString *val8=0, QString *val9=0, QString *val10=0);
  static QString        Param_RemoveSection(QString &param, QString section);
  static char          *Param_ExtraireNextValeurs(char* pt, QString &var_name, char**pt_lgn, long *len,
                                           QString *val1=0, QString *val2=0, QString *val3=0, QString *val4=0, QString *val5=0,
                                           QString *val6=0, QString *val7=0, QString *val8=0, QString *val9=0, QString *val10=0);
  static char *         GotoNextDebLigne(const char* txt);
  static char *         GotoEndOfLigne(const char* txt);
  static  QString       Construct_Name_Exe(QString module, QString start_Argv, const QString &alternateExecName="");
  static  QString       Construct_Name_File_Ini(QString module, QString start_Argv, QString nom_Fichier_Alternatif=QString::null );
  static  QString       Construct_PathBin_Module(const QString &module, const QString &ref_dir, QString sep = "/" );
  static  int           IsUtf8(const  QString & txt );
  static  int           IsUtf8(const  char *txt  , long len);
  static  QString       CharStrToQStringUtf8( const  char *ptr);
  static  QString       Utf8_Query(QSqlQuery &cur, int field);
  static  void          Param_GetList(const QString &outParam, const QString &sectionToRetrieve, const QString &varToRetrieve,  QStringList &lst , int isToStrip =0);
  static  void          Param_GetMap(QString &outParam, const QString &sectionToRetrieve, const QString &varToRetrieve,  QMap<QString,QString> &map , int isToStrip =0);
  //static  void          Param_GetList(const QString &file_ini, QStringList &lst, int *isUtf8 = 0 );
  static  QString       AbsoluteToRelativePath(QString pathRef, QString pathToConvert);
  static  QString       formatStringForParam(QString &str);
  static  QString       retrieveStringForParam(QString &str);
  static  QStringList   listDirectory(QString start_dir, const QString &filterExt_in ="", const QString &filterName="", const QString &sep=";", bool listWithoutExt=FALSE, bool listWithDirName =FALSE);
 //............................... DATA .............................................................................
private:
  static long _loadFromDisk(const QString &file_ini, QString &outParam, int *isUtf8_ret  =0 );
};

#endif

