 /*********************************** CGestIni.cpp *************************
 *  #include "CGestIni.h"        QT4-ready                                 *
 * ........................................................................*
 *   Project   :  MedinTux  (typographie made in france)                   *
 *   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
 *   by        :  Sevin Roland     from   MedinTux project                 *
 *   E-Mail    :  roland-sevin@medintux.org                                *
 *   Web site  :  www.medintux.org                                         *
 * ........................................................................*
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
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QSettings>
#include <QThread>
class QSqlQuery;
//================================================= CForSleep ==============================================================

class CForSleep : public QThread
      { public:
          CForSleep(){}
          ~CForSleep(){}
          static  void   sleep(unsigned long secs){QThread::sleep(secs);}
          virtual void   run(){}
      };
#define SLEEP(a) CForSleep::sleep((a))


//===================================== CGestIni =============================================================================
    class CGestIni : QSettings
{
 //............................... METHODES .........................................................................
public:

     Q_OBJECT

     CGestIni(const QString & fileName, QObject * parent = 0,  QString *pParamData = 0);
    ~CGestIni();
  void                  sync();
public:
  enum cutMode  {firstValue         = 0,
                 afterVarName       = 1,
                 entireLine         = 2
                };
  enum stripMode  {notStrip         = 0,
                   strip            = 1
                  };

  static bool           readMyIniFile(QIODevice &device, QSettings::SettingsMap &map);
  static bool           writeMyIniFile(QIODevice &device, const QSettings::SettingsMap &map);
  //-----------------------------------------------------  findFermant -----------------------------
  /*! \brief Trouve le motif fermant correspondant au motif ouvrant
   *  \param QString &text   texte ou doit se faire la recherche
   *  \param int pos_deb     position dans le texte ou commence la recherche doit etre apres l'ouvrant dont on cherche le fermant
   *  \param const QString &ouvrant       motif ouvrant
   *  \param const QString &fermant       motif fermant
   *  \return position sur le motif fermant.
  */
  static int            findFermant(const QString &ptext, int pos_deb, int pos_max, const QString &ouvrant, const QString &fermant);
  //-----------------------------------------------------  findFermant -----------------------------
  /*! \brief Trouve le motif fermant correspondant au motif ouvrant
   *  \param QString &text   texte ou doit se faire la recherche
   *  \param int pos_deb     position dans le texte ou commence la recherche doit etre apres l'ouvrant dont on cherche le fermant
   *  \param const QString &ouvrant       motif ouvrant
   *  \param const QString &fermant       motif fermant
   *  \return position sur le motif fermant.
  */

  static int            findFermant(const QString &ptext, int pos_deb, int pos_max, const QString &ouvrant, const QString &fermant, int &isWithToken);
  //-----------------------------------------------------  Param_UpdateToDisk --------------------------
  /*! \brief ecrit et sauvegarde les parametres iniParam dans un fichier dont le chemin est specifie.
   *  \param const QString &file_ini   chemin ou doit etre ecrit le fichier
   *  \param const QString &inParam     donnees a ecrire
   *  \todo Attention le fichier n'est pas ferme corrrectement ???...
  */
  static void           Param_UpdateToDisk(const QString &file_ini, const QString &inParam);

  //---------------------------- addXmlData ------------------------------------------------
  /*! \brief ajoute dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
   *   dans cet exemple la valeur serait ' et de fin' (sans les apostrophes)
   *  \param _tagXml : String indiquant le nom du tag
   *  \param valeur :  valeur a encadrer par les tags xml
   *  \param modeleXML :  String XML dans laquelle on va ajouter la valeur
   *  \param ofset :  QString ofset de decalage vers la droite par defaut vide
   *  \param mustBeB64Protected :  si true la valeur sera convertie et inscrite en base 64.
  */
  static void addXmlData(const QString& dName, QByteArray data, QString &modeleXML, bool mustBeB64Protected=false, const QString &ofset ="" );

  //---------------------------- getXmlData --------------------------------------------------------------------
  /*! \brief retourne dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
   *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
   *  \param dataName : String indiquant le nom du tag
   *  \param dataXml :  String XML dans laquelle on va lire la valeur
   *  \param nextPos :  pointeur sur un int qui si il n'est pas egal a zero indique
   *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherché.
   *  \return QString qui est la valeur a rechercher.
  */
  static QString getXmlData(const QString& dataName, const QString& dataXml, int *nextPos=0);
  //---------------------------- getXmlDataList --------------------------------------------------------------------
  /*! \brief retourne une QStringList de valeurs à partir d'un fichier XML et d'un tag
   *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
   *  \param tagName :  QString indiquant le nom du tag  sans les </ ou < ou >
   *  \param dataXml :  QString XML dans laquelle on va lire la valeur
   *  \param nextPos :  pointeur sur un int qui si il n'est pas egal a zero indique
   *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherché.
   *  \return QStringList qui est la liste de valeurs a rechercher.
  */
  static QStringList getXmlDataList(const QString& tagName, const QString& dataXml, int *nextPos=0);

  //---------------------------- setXmlData static ------------------------------------------------
  /*! \brief place dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
   *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
   *  \param dataName : String indiquant le nom du tag
   *  \param valeur :    QString valeur a ecrire
   *  \param dataXml :  String XML dans laquelle on va ecrire la valeur
   *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherché.
   * \param noConvertCharToHtml  if zero (default value) '>' '<' '&' are  converted in '&gt;' '&lt;' '&amp;'
   *  \return true si tout est ok false sinon (en general le tag n'a pas ete trouve)
  */
  static bool setXmlData(const QString& dataName, const QString &valeur, QString& dataXml, int noConvertCharToHtml = 0 );

  //---------------------------- setXmlData ------------------------------------------------
  /*! \brief place dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
   *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
   *  \param dataName : String indiquant le nom du tag
   *  \param valeur :    QByteArray valeur a ecrire
   *  \param dataXml :  String XML dans laquelle on va ecrire la valeur
   *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherché.
   * \param noConvertCharToHtml  if zero (default value) '>' '<' '&' are  converted in '&gt;' '&lt;' '&amp;'
   *  \return true si tout est ok false sinon (en general le tag n'a pas ete trouve)
  */

  static bool setXmlData(const QString& dataName, QByteArray valeur, QString& dataXml, int noConvertCharToHtml = 0 );
  //-----------------------------------------------------  Param_SplitValeurs -----------------------------------
  /*! \brief txt pointe sur un texte contenant differentes valeurs separees par un separateur, tout ce qui precede le signe = +1  sera coupe.
   *  \param txt : String parametres dans laquelle on va lire les valeurs
   *  \param sep :     const QString &sep separateur de valeurs
   *  \param mustBeTrimmed :     false/values are not trimmed, true/values are trimmed
   *  \param QString *val1 -> QString *val10 : valeurs a remplir avec les donnees situees entre le separateur
   *  \return int nombre de valeur trouvees
  */
  static int  Param_SplitValeurs(  const QString &str,const QString &sep, bool mustBeTrimmed,
                                   QString *val1=0, QString *val2=0, QString *val3=0, QString *val4=0, QString *val5=0,
                                   QString *val6=0, QString *val7=0, QString *val8=0, QString *val9=0, QString *val10=0);
  //-----------------------------------------------------  Param_SplitValeurs -----------------------------------
  /*! \brief txt pointe sur un texte contenant differentes valeurs separees par un separateur, tout ce qui precede le signe = +1  sera coupe.
   *  \param txt : String parametres dans laquelle on va lire les valeurs
   *  \param sep :     const QString &sep separateur de valeurs
   *  \param varName : const QString &varName nom de ce qui precede le signe = (si il existe)
   *  \param mustBeTrimmed :   false/values are not trimmed, true/values are trimmed
   *  \param QString *val1 -> QString *val10 : valeurs a remplir avec les donnees situees entre le separateur
   *  \return int nombre de valeur trouvees
  */
  static int  Param_SplitValeurs(  const QString &str,const QString &sep, QString &varName, bool mustBeTrimmed,
                                   QString *val1=0, QString *val2=0, QString *val3=0, QString *val4=0, QString *val5=0,
                                   QString *val6=0, QString *val7=0, QString *val8=0, QString *val9=0, QString *val10=0);
  //------------------------------ CutStrLeft -----------------------------------------
  /*! \brief retourne la chaine avant le premier motif at trouve.
  */
  static QString CutStrLeft(const QString &str, const QString &at);
  //------------------------------ CutStrRight -----------------------------------------
  /*! \brief retourne la chaine apres le premier motif at trouve.
  */
  static QString CutStrRight(const QString &str, const QString &at);

  //-----------------------------------------------------  Param_UpdateToDisk --------------------------
  /*! \brief sauvegarde un QByteArray dans un fichier
   *  \todo Attention le fichier n'est pas ferme ???...
  */
  static void           Param_UpdateToDisk(const QString &file_ini, const QByteArray &ba);
  static QString        Param_UpdateFromDisk(const QString &file_ini,  int *isUtf8_ret  =0 );
  static QByteArray&    Param_UpdateFromDisk(const QString &file_ini, QByteArray &ba, int *isUtf8_ret  =0 );
  static long           Param_UpdateFromDisk(const QString &file_ini, QString &outParam, int *isUtf8 = 0 );
  /*
  static QString        Param_WriteParam(         QString *pQstr,    const QString &section, const QString &variable,
                                            const QString &val1="",  const QString &val2="", const QString &val3="", const QString &val4="", const QString &val5="",
                                            const QString &val6="",  const QString &val7="", const QString &val8="", const QString &val9="", const QString &val10="");
  */

  static QString        _Param_WriteParam(   QString *pQstr,  const QString &section, const QString &variable,
                                          const char *val1=0, const char *val2=0,     const char *val3=0, const char *val4=0, const char *val5=0,
                                          const char *val6=0, const char *val7=0,     const char *val8=0, const char *val9=0, const char *val10=0);

  static QString        Param_WriteParam(      QString *pQstr,   const QString &section, const QString &variable,
                                         const QString &val1="", const QString &val2="", const QString &val3="", const QString &val4="", const QString &val5="",
                                         const QString &val6="", const QString &val7="", const QString &val8="", const QString &val9="", const QString &val10="")
  {

      return CGestIni::_Param_WriteParam(pQstr, section, variable,
                                         val1.toLatin1(), val2.toLatin1(), val3.toLatin1(), val4.toLatin1(), val5.toLatin1(),
                                         val6.toLatin1(), val7.toLatin1(), val8.toLatin1(), val9.toLatin1(), val10.toLatin1());
  }

  static QString        Param_ReadLine(  const char* txt, const char *section, const char  *variable);
  static char          *Param_ExtraireValeurs(const char* txt,
                                              QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                              QString *val6, QString *val7, QString *val8, QString *val9, QString *val10);
  static QString        Param_ReadUniqueParam(const char* txt, const char *section, const char  *variable);
  /*
  static QString        Param_ReadParam(const char* txt, const char *section, const char  *variable,
                                        QString *val1=0, QString *val2=0, QString *val3=0, QString *val4=0, QString *val5=0,
                                        QString *val6=0, QString *val7=0, QString *val8=0, QString *val9=0, QString *val10=0);
  */
  static QString        Param_ReadParam(  const QString &txt, const QString &section, const QString &variable,
                                          QString *val1=0, QString *val2=0, QString *val3=0, QString *val4=0, QString *val5=0,
                                          QString *val6=0, QString *val7=0, QString *val8=0, QString *val9=0, QString *val10=0);

  static char          *Param_GotoNextSection(char *pt, const char* section=0, QString *pQsection=0);
  static char          *Param_ExtraireNextValeurs(char* pt, QString &var_name,
                                           QString *val1=0, QString *val2=0, QString *val3=0, QString *val4=0, QString *val5=0,
                                           QString *val6=0, QString *val7=0, QString *val8=0, QString *val9=0, QString *val10=0);

  static char          *Param_ExtraireNextValeurs(char* pt, QString &var_name, char**pt_lgn, long *len,
                                           QString *val1=0, QString *val2=0, QString *val3=0, QString *val4=0, QString *val5=0,
                                           QString *val6=0, QString *val7=0, QString *val8=0, QString *val9=0, QString *val10=0);
  static char *         GotoNextDebLigne(const char* txt);
  static char *         GotoEndOfLigne(const char* txt);
  static  QString       Construct_Name_Exe(QString module, QString start_Argv, const QString &alternateExecName="");
  static  QString       Construct_Name_File_Ini(QString module, QString start_Argv, QString nom_Fichier_Alternatif=QString::null );
  static  QString       Construct_PathBin_Module(const QString &module, const QString &ref_dir);
  static  int           IsUtf8(QByteArray *ba);
  static  int           IsUtf8(const  QString & txt );
  static  int           IsUtf8(const  char *txt);
  static  QString       utf8Conv(const char* deb, long len);
  static  QString       CharStrToQStringUtf8( const  char *ptr );
  static  QString       Utf8_Query(QSqlQuery &cur,  int field);
  static  QString       Utf8_Query(QSqlQuery *pcur, int field);

  //----------------------------------------- Param_GetListFromPath ---------------------------------------------
  /*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
   *  \param file_ini : nom d'un fichier de parametrage ou l'on va extraire une liste.
   *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
   *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
   *                             si terminee par * alors on prend toutes les variables commençant par ce qui est avant le * de varToRetrieve
   *                             si commence par * alors on prend toutes les variables terminees  par ce qui est après le * de varToRetrieve
   *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas à la premiere valeur entre = et ,)
   *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
   *  \param isUtf8 : pointeur sur un int (par defaut a  zero) qui si different de zero sera initialise a un si le fichier est en UTF8
   *  \param entireDataLine :  entireDataLine a zero par defaut seule la première valeur apres le signe egal et avant la premiere virgule sera retenue.
   *                           si a un,   toutes les donnees apres le signe = seront retenues
   *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
   *  \return QStringList lst la liste des variables
  */
  static  QStringList Param_GetListFromPath(const QString &file_ini, const QString &sectionToRetrieve, const QString &varToRetrieve="",  int isToStrip =0, int *isUtf8 =0, int entireDataLine =0);

  //----------------------------------------- Param_GetListFromPath ---------------------------------------------
  /*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
   *  \param file_ini : nom du fichier ou l'on va extraire une liste.
   *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
   *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
   *                             si terminee par * alors on prend toutes les variables commençant par ce qui est avant le * de varToRetrieve
   *                             si commence par * alors on prend toutes les variables terminees  par ce qui est après le * de varToRetrieve
   *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas à la premiere valeur entre = et ,)
   *  \param lst :        liste a initialiser et construire
   *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
   *  \param isUtf8 : pointeur sur un int (par defaut a  zero) qui si different de zero sera initialise a un si le fichier est en UTF8
   *  \param entireDataLine :  entireDataLine a zero par defaut seule la première valeur apres le signe egal et avant la premiere virgule sera retenue.
   *                           si a un,   toutes les donnees apres le signe = seront retenues
   *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
  */
  static  void       Param_GetListFromPath(const QString &file_ini, const QString &sectionToRetrieve, QStringList &lst, const QString &varToRetrieve="",  int isToStrip =1, int *isUtf8 =0, int entireDataLine =0);

  //----------------------------------------- Param_GetList ---------------------------------------------
  /*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
   *  \param QString &outParam : contenu d'un fichier de parametrage a annalyse ou l'on va extraire une lister.
   *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
   *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
   *                             si terminee par * alors on prend toutes les variables commençant par ce qui est avant le * de varToRetrieve
   *                             si commence par * alors on prend toutes les variables terminees  par ce qui est après le * de varToRetrieve
   *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas à la premiere valeur entre = et ,)
   *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
   *  \param entireDataLine :  entireDataLine a zero par defaut seule la première valeur apres le signe egal et avant la premiere virgule sera retenue.
   *                           si a un,   toutes les donnees apres le signe = seront retenues
   *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
   *  \return QStringList lst la liste des variables
  */
  static  QStringList Param_GetList(QString &outParam, const QString &sectionToRetrieve, const QString &varToRetrieve="",  int isToStrip =0, int entireDataLine =0);

  //----------------------------------------- Param_GetList ---------------------------------------------
  /*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
   *  \param file_ini : nom du fichier ou l'on va extraire une liste.
   *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
   *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
   *                             si terminee par * alors on prend toutes les variables commençant par ce qui est avant le * de varToRetrieve
   *                             si commence par * alors on prend toutes les variables terminees  par ce qui est après le * de varToRetrieve
   *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas à la premiere valeur entre = et ,)
   *  \param lst :        liste a initialiser et construire
   *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
   *  \param isUtf8 : pointeur sur un int (par defaut a  zero) qui si different de zero sera initialise a un si le fichier est en UTF8
   *  \param entireDataLine :  entireDataLine a zero par defaut seule la première valeur apres le signe egal et avant la premiere virgule sera retenue.
   *                           si a un,   toutes les donnees apres le signe = seront retenues
   *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
  */
  static  void          Param_GetList(const QString &file_ini, const QString &sectionToRetrieve, QStringList &lst, const QString &varToRetrieve="",  int isToStrip =1, int *isUtf8 =0, int entireDataLine =0);

  //----------------------------------------- getListBetweenTwoTags ---------------------------------------------
  /*! \brief cree la liste des termes trouves entre deux motifs textuel dans un texte quelconque.
   *  \param textToExamine : texte dans lequel rechercher les termes encadres par tag1 et tag2.
   *  \param tag1 : premier motif encadrant ou tag
   *  \param tag1 : deuxieme motif encadrant ou tag
   *  \param cleanMode :  0/si pas de nettoyage 0/ on vire espaces de fin et debut \r\n
   *  \return   une QStringList qui contient la liste des termes encadres par les deux tags
  */
  static  QStringList          getListBetweenTwoTags(const QString &textToExamine, const QString &tag1, const QString &tag2="", int cleanMode =1);

  //----------------------------------------- Param_GetList ---------------------------------------------
  /*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
   *  \param outParam : String parametres dans laquelle l'on va extraire une liste.
   *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
   *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
   *                             si terminee par * alors on prend toutes les variables commençant par ce qui est avant le * de varToRetrieve
   *                             si commence par * alors on prend toutes les variables terminees  par ce qui est après le * de varToRetrieve
   *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas à la premiere valeur entre = et virgule)
   *  \param lst :        liste a initialiser et construire
   *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
   *  \param entireDataLine :  entireDataLine a zero par defaut seule la première valeur apres le signe egal et avant la premiere virgule sera retenue.
   *                           si a un,   toutes les donnees apres le signe = seront retenues
   *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
  */
  static  void          Param_GetList(QString &outParam, const QString &sectionToRetrieve, const QString &varToRetrieve,  QStringList &lst , int isToStrip =0, int entireDataLine =0);
  static  QString       Param_RemoveSection(QString &param, QString section);
  static  QString       AbsoluteToRelativePath(QString pathRef, QString pathToConvert);
  static  QString       formatStringForParam(QString &str);
  static  QString       retrieveStringForParam(QString &str);
  static  QStringList   listDirectory(QStringList &dirList, QString start_dir, const QString &filterExt_in="", const QString &filterName="", const QString &sep=";", bool listWithoutExt=false);
  static  QStringList   listDirectory(                      QString start_dir, const QString &filterExt_in="", const QString &filterName="", const QString &sep=";", bool listWithoutExt=false);
  static  quint16       HexToUINT( const char *str);
  static  QString       UINTtoHex( quint16 val);
  static  QString       PassWordDecode(QString str_to_decode);
  static  QString       PassWordEncode(QString &pass);
  static  void          Copy_Dir(QString src_Dir,  QString dest_Dir);
  static  QString       Copy_File(const QString &fileSrc, const QString &pathDst, QString *file_name =0);

  //----------------------------------------- replaceList ---------------------------------------------
  /*! \brief replace a variables list in section parameters by datas from QStringList
   *  \param outParam : parameters data document destination.
   *  \param QStringList &list list data sources to place.
   *  \param const QString &sectionToSet section where to find list of variables to replace
   *  \param const QString &varToSet  variable name of each list elements to replace
   */
  static  void  replaceList(QString &outParam, const QString &sectionToSet, const QString &varToSet,  const QStringList &lst );
  //----------------------------------------- readNextLine ---------------------------------------------
  static  int   readNextLine(const QString &outParam, int deb, QString &line);
  //----------------------------------------- gotoEndLine ---------------------------------------------
  static  int gotoEndLine(const QString &outParam, int pos);
  //----------------------------------------- gotoNextStartLine ---------------------------------------------
  static  int gotoNextStartLine(const QString &outParam, int pos);
  //----------------------------------------- gotoNextNotBlank ---------------------------------------------
  static  int gotoNextNotBlank(const QString &outParam, int pos);
  //----------------------------------------- gotoNextBlank ---------------------------------------------
  static  int gotoNextBlank(const QString &outParam, int pos);
  //----------------------------------------- getNextWord ---------------------------------------------
  static  int getNextWord(const QString &outParam, int pos, QString &word);
  //---------------------------------------------- fromMyUTF8 ------------------------------------------------
  /*! \brief converti l'UTF8 avec une methode qui fonctionne en attendant de resoudre le PB par un moyen plus propre
   *  \param ptr : const char* chaine codÃ©e en UTF8
   *  \return QString convertie
   */

  static QString        fromMyUTF8(const char* ptr);
 //............................... DATA .............................................................................
public:
    QString *m_pParamData;
private:
    static long _loadFromDisk(const QString &file_ini, QByteArray &ba,    int *isUtf8_ret=0 );
    static long _loadFromDisk(const QString &file_ini, QString &outParam, int *isUtf8_ret=0 );

};

#endif

