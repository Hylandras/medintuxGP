 /*********************************** CGestIni.h ***************************
 *  #include "CGestIni.h"         QT4-QT5-ready                            *
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

//=============================================== INCLUDES ===================================================================
#include  "CGestIni.h"
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QByteArray>
#include <QVariant>
#ifdef Q_OS_MACX
#define PATH_SPEC_MAC "../../../"
#define ARCHI_DEP_MAC ".app/Contents/MacOS/" + module
#else
#define PATH_SPEC_MAC ""
#define ARCHI_DEP_MAC ""
#endif
#ifdef Q_WS_WIN
#define F_EXT  ".exe"
#else
#define F_EXT ""
#endif

/*! \class CGestIni
 *  \brief Gestion des fichiers INI de la suite Medintux.
 * Gere le chargement des parametres, leur recherche et l'ecriture des fichiers parametres.
 * Les parametres sont sauvegardes de la sorte :
 * [SECTION]\\r\\n
 * Parametre = val1, val2, val3, val4, val5, val6, val7, val7, val8, val9, val10\\r\\n
 * Plusieurs sections peuvent cohabiter dans un meme fichier et plusieurs parametres peuvent etre defini par section.
 * Cette classe gere la compatibilite Windows, Mac, Linux pour les fichiers INI
 * \todo Un seul fichier INI pour toute la suite dans le QDir::home() qui ne contient que les donnees de connexion a la base de donnees. Les autres parametres sont dans la base de donnees.
*/

//-----------------------------------------------------  CGestIni --------------------------------
/*! \brief constructeur de la classe. Initialise les variables m_ParamData et m_ParamPath.
*/
CGestIni::CGestIni(const QString & fileName, QObject * parent /* = 0 */ ,  QString *pParamData /* = 0 */)
    : QSettings(fileName, QSettings::IniFormat,  parent )
{m_pParamData = pParamData;
 // QSettings::Format myIniFormat = QSettings::registerFormat("ini", readMyIniFile, writeMyIniFile);
}
//-----------------------------------------------------  ~CGestIni --------------------------------
/*! \brief constructeur de la classe. Initialise les variables m_ParamData et m_ParamPath.
*/
CGestIni::~CGestIni()
{
}

//-----------------------------------------------------  readMyIniFile --------------------------------
/*! \brief lit un fichier ini
*/
bool  CGestIni::readMyIniFile(QIODevice & /*device*/, QSettings::SettingsMap &/*map*/)
{
 return true;
}

//-----------------------------------------------------  writeMyIniFile --------------------------------
/*! \brief sauvegarde un fichier ini
*/
bool CGestIni::writeMyIniFile(QIODevice &/*device*/, const QSettings::SettingsMap &/*map*/)
{
 return true;
}

//-----------------------------------------------------  ~CGestIni --------------------------------
/*! \brief constructeur de la classe. Initialise les variables m_ParamData et m_ParamPath.
*/
void CGestIni::sync()
{   if (m_pParamData) Param_UpdateToDisk(fileName(), *m_pParamData);
    QSettings::sync();
}

//-----------------------------------------------------  Param_UpdateToDisk --------------------------
/*! \brief sauvegarde un QByteArray dans un fichier
 *  \todo Attention le fichier n'est pas ferme ???...
*/
void  CGestIni::Param_UpdateToDisk(const QString &file_ini, const QByteArray &ba)
{   if (file_ini.endsWith(".ini"))
       {  int p        = ba.length();
          QString endf = "";
          while (p && (ba.at(p-1)==' '||ba.at(p-1)=='\t'||ba.at(p-1)=='\r'||ba.at(p-1)=='\n')) --p;
          if (p>0)    endf  = ba.mid(p-9);
          if (p<=0 || endf != "[END_INI]")
             {
                QString mess = QString("ERROR CORRUPTED DATA in Param_UpdateToDisk()  integrity tag '[END_INI]' not found, initialisation file not rewrited : %1").arg(file_ini);
                qDebug()<<mess;
                return ;
             }
       }
    QFile file( file_ini);
    if ( !file.open( QIODevice::WriteOnly ) )    return;
    file.write( ba );
    file.close();
}
//-----------------------------------------------------  Param_UpdateToDisk --------------------------
/*! \brief ecrit et sauvegarde les parametres iniParam dans un fichier dont le chemin est specifie.
 *  \param const QString &file_ini   chemin ou doit etre ecrit le fichier
 *  \param const QString &inParam     donnees a ecrire
 *  \todo Attention le fichier n'est pas ferme corrrectement ???...
*/
void  CGestIni::Param_UpdateToDisk(const QString &file_ini, const QString &inParam)
{   if (file_ini.endsWith(".ini"))
       {  int p        = inParam.length();
          QString endf = "";
          while (p && (inParam.at(p-1)==' '||inParam.at(p-1)=='\t'||inParam.at(p-1)=='\r'||inParam.at(p-1)=='\n')) --p;
          if (p>0)    endf  = inParam.mid(p-9);
          if (p<=0 || endf != "[END_INI]")
             {
               QString mess = QString("ERROR CORRUPTED DATA in Param_UpdateToDisk()  integrity tag '[END_INI]' not found, initialisation file not rewrited : %1").arg(file_ini);
               qDebug()<<mess;
               return ;
             }
       }
    QFile file( file_ini);
    if ( !file.open( QIODevice::WriteOnly ) )    return;
    QTextStream ts( &file );
    ts << inParam;
    file.close();
}

//-----------------------------------------------------  Param_UpdateFromDisk ------------------------------
/*! \brief Lit le fichier file_ini et renvoie son contenu dans outParam. L'encodage est gere.
*/
QString  CGestIni::Param_UpdateFromDisk(const QString &file_ini,  int *isUtf8_ret /* =0 */)
{QString ret="";
 Param_UpdateFromDisk(file_ini, ret, isUtf8_ret);
 return ret;
}

//-----------------------------------------------------  Param_UpdateFromDisk ------------------------------
/*! \brief Lit le fichier file_ini et renvoie son contenu dans outParam. L'encodage est gere.
*/
QByteArray& CGestIni::Param_UpdateFromDisk(const QString &file_ini, QByteArray &ba, int *isUtf8_ret /* =0 */)
{        int len = _loadFromDisk(file_ini, ba, isUtf8_ret );
         if (file_ini.endsWith(".ini"))
            {while (len && (ba.at(len-1)==' '||ba.at(len-1)=='\t'||ba.at(len-1)=='\r'||ba.at(len-1)=='\n')) --len;
             ba.truncate(len);
             bool isRescue = QFile::exists ( file_ini+"_rescue" );
             //....................... longueur zero ......................
             if (len<=0)
                { QString mess = QString("ERROR : Param_UpdateFromDisk() initialisation file length is zero : %1").arg(file_ini);
                  qDebug()<<mess;
                  if (isRescue)
                     { QString mess = QString("ERROR : Param_UpdateFromDisk() initialisation file length is zero, trying with : %1").arg(file_ini+"_rescue");
                       qDebug()<<mess;
                       len = _loadFromDisk(file_ini+"_rescue", ba, isUtf8_ret );
                       if (len==0)
                          { QString mess = QString("ERROR : Param_UpdateFromDisk() rescue file length is zero  %1").arg(file_ini+"_rescue");
                            qDebug()<<mess;
                          }
                     } // if (isRescue)
                  while (len && (ba.at(len-1)==' '||ba.at(len-1)=='\t'||ba.at(len-1)=='\r'||ba.at(len-1)=='\n')) --len;
                  ba.truncate(len);
                } // if (len<=0)

             //....................... tester le tag d'integrite ......................
             if (len && !ba.endsWith("[END_INI]") )
                { QString mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() integrity tag '[END_INI]' not found in: %1").arg(file_ini);
                  qDebug()<<mess;
                  if (isRescue)
                     { QByteArray  tmp_ba_out      = "";
                       int         tmp_isUtf8_ret  = 0;
                       long ret                    = _loadFromDisk(file_ini+"_rescue", tmp_ba_out, &tmp_isUtf8_ret );
                       if (ret)
                          { mess     = QString("ERROR INTEGRITY : Param_UpdateFromDisk() using initialisation file rescue : %1").arg(file_ini+"_rescue");
                            qDebug()<<mess;
                            ba = tmp_ba_out;
                            if (isUtf8_ret) *isUtf8_ret = tmp_isUtf8_ret;
                            while (ret && (ba.at(ret-1)==' '||ba.at(ret-1)=='\t'||ba.at(ret-1)=='\r'||ba.at(ret-1)=='\n')) --ret;
                            ba.truncate(ret);
                            if ( !ba.endsWith("[END_INI]") )
                               { mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() end rescue file not good '[END_INI]' added to variable end");
                                 qDebug()<<mess;
                                 ba += "\r\n[END_INI]";
                               }
                            return ba;
                          }
                     } //if (isRescue)
                  else
                     { mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() rescue file not found : %1").arg(file_ini+"_rescue");
                       qDebug()<<mess;
                       mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() integrity tag '[END_INI]' just added to variable end, hope that is enough");
                       qDebug()<<mess;
                       ba.append("\r\n[END_INI]");
                     }
                }
            } // if (file_ini.endsWith(".ini"))
         return ba;
}

//-----------------------------------------------------  Param_UpdateFromDisk ------------------------------
/*! \brief Lit le fichier file_ini et renvoie son contenu dans outParam. L'encodage est gere.
*/
long  CGestIni::Param_UpdateFromDisk(const QString &file_ini, QString &outParam, int *isUtf8_ret /* =0 */)
{        int len = _loadFromDisk(file_ini, outParam, isUtf8_ret );
         if (file_ini.endsWith(".ini"))
            {while (len && (outParam.at(len-1)==' '||outParam.at(len-1)=='\t'||outParam.at(len-1)=='\r'||outParam.at(len-1)=='\n')) --len;
             outParam.truncate(len);
             bool isRescue = QFile::exists ( file_ini+"_rescue" );
             //....................... longueur zero ......................
             if (len<=0)
                { QString mess = QString("ERROR : Param_UpdateFromDisk() initialisation file length is zero : %1").arg(file_ini);
                  qDebug()<<mess;
                  if (isRescue)
                     { QString mess = QString("ERROR : Param_UpdateFromDisk() initialisation file length is zero, trying with : %1").arg(file_ini+"_rescue");
                       qDebug()<<mess;
                       len = _loadFromDisk(file_ini+"_rescue", outParam, isUtf8_ret );
                       if (len==0)
                          { QString mess = QString("ERROR : Param_UpdateFromDisk() rescue file length is zero  %1").arg(file_ini+"_rescue");
                            qDebug()<<mess;
                          }
                     } // if (isRescue)
                  while (len && (outParam.at(len-1)==' '||outParam.at(len-1)=='\t'||outParam.at(len-1)=='\r'||outParam.at(len-1)=='\n')) --len;
                  outParam.truncate(len);
                } // if (len<=0)

             //....................... tester le tag d'integrite ......................
             if (len && !outParam.endsWith("[END_INI]") )
                { QString mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() integrity tag '[END_INI]' not found in: %1").arg(file_ini);
                  qDebug()<<mess;
                  if (isRescue)
                     { QString tmp_out         = "";
                       int     tmp_isUtf8_ret  = 0;
                       long ret                = _loadFromDisk(file_ini+"_rescue", tmp_out, &tmp_isUtf8_ret );
                       if (ret)
                          { mess     = QString("ERROR INTEGRITY : Param_UpdateFromDisk() using initialisation file rescue : %1").arg(file_ini+"_rescue");
                            qDebug()<<mess;
                            outParam = tmp_out;
                            if (isUtf8_ret) *isUtf8_ret = tmp_isUtf8_ret;
                            while (ret && (outParam.at(ret-1)==' '||outParam.at(ret-1)=='\t'||outParam.at(ret-1)=='\r'||outParam.at(ret-1)=='\n')) --ret;
                            outParam.truncate(ret);
                            if ( !outParam.endsWith("[END_INI]") )
                               { mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() end rescue file not good '[END_INI]' added to variable end");
                                 qDebug()<<mess;
                                 outParam += "\r\n[END_INI]";
                               }
                            return outParam.length();
                          }
                     } //if (isRescue)
                  else
                     { mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() rescue file not found : %1").arg(file_ini+"_rescue");
                       qDebug()<<mess;
                       mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() integrity tag '[END_INI]' just added to variable end, hope that is enough");
                       qDebug()<<mess;
                       outParam += "\r\n[END_INI]";
                     }
                }
            } // if (file_ini.endsWith(".ini"))
         return outParam.length();
}

//-----------------------------------------------------  _loadFromDisk ------------------------------
/*! \brief Lit le fichier file_ini et renvoie son contenu dans outParam. L'encodage est gere.
  par defaut si un fichier est html il est converti en qrichtext pour pouvoir etre edite  et
  affiche dans un editeur qrichtext
  si l'on veut inhiber ce dispositif placer dans *isUtf8_ret la valeur 2
*/
long  CGestIni::_loadFromDisk(const QString &file_ini, QString &outParam, int *isUtf8_ret /* =0 */)
{        //............ charger le fichier .ini ..........
         QFile qFile(file_ini );
         if (qFile.open( QIODevice::ReadOnly )==false)   return  0;
         QByteArray ba = qFile.readAll();
         qFile.close ();
         QString ext =  QFileInfo(qFile).suffix();
         int isUtf8  = IsUtf8(&ba);
         if ( isUtf8_ret ) *isUtf8_ret = isUtf8;
         if (isUtf8)
            { outParam = CGestIni::fromMyUTF8( ba.data() );
              // outParam    =  QString::fromUtf8(ba.data(), ba.size());
              if (ext.toLower().left(3) == "htm" &&
                  (isUtf8_ret && *isUtf8_ret != 2)          // si *isUtf8_ret==2 --> preserver le tag html d'origine
                 )
                 { outParam.replace("meta name=\"qrichtext\" content=\"charset=utf-8\"",   // oblige d'etre en content=\"1\" pour que les tabulations fonctionnent !!
                                    "meta name=\"qrichtext\" content=\"1\"");
                 }
              outParam.replace("&nbsp;", " ");
            }
         else
            { outParam = CGestIni::fromMyUTF8( ba.data() );
              // outParam = ba;    //QString::fromLocal8Bit (ba)
            }
         return outParam.length();
}
//-----------------------------------------------------  _loadFromDisk ------------------------------
/*! \brief Lit le fichier file_ini et renvoie son contenu dans outParam. L'encodage est gere.
  par defaut si un fichier est html il est converti en qrichtext pour pouvoir etre edite  et
  affiche dans un editeur qrichtext
  si l'on veut inhiber ce dispositif placer dans *isUtf8_ret la valeur 2
*/
long CGestIni::_loadFromDisk(const QString &file_ini, QByteArray &ba, int *isUtf8_ret /* =0 */)
{        if (ba.size()>0) ba.data()[0]=0;
         QFile qFile(file_ini );
         if (qFile.open( QIODevice::ReadOnly )==false)   return  0;
         ba = qFile.readAll();
         qFile.close ();
         QString ext =  QFileInfo(qFile).suffix();
         int isUtf8  = IsUtf8(&ba);
         if ( isUtf8_ret ) *isUtf8_ret = isUtf8;
         if (isUtf8)
            {QString tmp =  QString::fromUtf8 ( ba );
             ba          =  tmp.toUtf8 ();
             if (ext.toLower().left(3) == "htm" &&
                 (isUtf8_ret && *isUtf8_ret != 2)          // si *isUtf8_ret==2 --> preserver le tag html d'origine
                )
                {ba.replace("meta name=\"qrichtext\" content=\"charset=utf-8\"",   // oblige d'etre en content=\"1\" pour que les tabulations fonctionnent !!
                            "meta name=\"qrichtext\" content=\"1\"");
                }
             ba.replace("&nbsp;", " ");
            }
         return ba.length();
}
//---------------------------- addXmlData ------------------------------------------------
/*! \brief ajoute dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
 *   dans cet exemple la valeur serait ' et de fin' (sans les apostrophes)
 *  \param _tagXml : String indiquant le nom du tag
 *  \param valeur :  valeur a encadrer par les tags xml
 *  \param modeleXML :  QString XML dans laquelle on va ajouter la valeur
 *  \param ofset :  QString ofset de decalage vers la droite par defaut vide
 *  \param mustBeB64Protected :  si true la valeur sera convertie et inscrite en base 64.
*/

void CGestIni::addXmlData(const QString& _tagXml, QByteArray valeur, QString &modeleXML, bool mustBeB64Protected /* =false */, const QString &ofset /* ="" */, int htmlConvert /* = CGestIni::SupInfAnd */)
{QString tagXml      = _tagXml;
 QString dataToPlace = "";
 if (htmlConvert==CGestIni::SupInfAnd)
    { valeur.replace('&',"&amp;");
      valeur.replace('>',"&gt;");
      valeur.replace('<',"&lt;");
    }
 if (mustBeB64Protected)
    {tagXml        = tagXml.prepend('_');
     valeur        = valeur.toBase64();
     dataToPlace   = valeur;
    }
 else
    {dataToPlace = valeur;
    }
 modeleXML += QString("%4   <%1>%2</%3>\r\n").arg(tagXml, dataToPlace, tagXml,ofset);
}

//---------------------------- getXmlData ------------------------------------------------
/*! \brief retourne dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
 *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
 *  \param tagName : String indiquant le nom du tag
 *  \param dataXml :  String XML dans laquelle on va lire la valeur
 *  \param nextPos :  pointeur sur un int qui si il n'est pas egal a zero indique
 *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherche.
 *  \param convert_Gt_Lt_Amp : if true il y a conversion des symboles '&lt;' en '< '   '&gt;' en '>'  '&amp;'  en '&'
 *  \return QString qui est la valeur a rechercher.
*/

QString CGestIni::getXmlData(const QString& tagName, const QString& dataXml, int *nextPos, bool convert_Gt_Lt_Amp /* = true */)
{bool mustBeB64Protected = false;
 int posEnd  = -1;
 int posDeb  =  0;

 QString tag =  tagName+'>';
 if (nextPos) {posDeb  = dataXml.indexOf(tag, *nextPos);*nextPos=0;}  // on  remet a zero au cas ou tag pas trouve (recommencer a zero)
 else         {posDeb  = dataXml.indexOf(tag);}
 if (posDeb==-1)         return QString::null;
 if (dataXml.at(posDeb-1)=='_') {posDeb += tag.length(); mustBeB64Protected = true; tag = tag.prepend("</_");}
 else                           {posDeb += tag.length(); tag = tag.prepend("</");}
 posEnd  = dataXml.indexOf(tag, posDeb);
 if (posEnd==-1)         return QString::null;
 if (nextPos) *nextPos = posEnd + tag.length();
 QString retour = "";
 if (mustBeB64Protected) retour = QString( QByteArray::fromBase64( dataXml.mid(posDeb,posEnd-posDeb).toLatin1() ) );
 else                    retour = dataXml.mid(posDeb,posEnd-posDeb);
 if (convert_Gt_Lt_Amp)
    { retour.replace("&gt;",">");
      retour.replace("&lt;","<");
      retour.replace("&amp;","&");
    }
 return retour;
}
//---------------------------- setXmlData static ------------------------------------------------
/*! \brief place dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
 *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
 *  \param dataName : String indiquant le nom du tag
 *  \param valeur :    QString valeur a ecrire
 *  \param dataXml :  String XML dans laquelle on va ecrire la valeur
 *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherche.
 *  \param noConvertCharToHtml  if zero (default value) '>' '<' '&' are  converted in '&gt;' '&lt;' '&amp;'
 *  \return true si tout est ok false sinon (en general le tag n'a pas ete trouve)
*/
bool CGestIni::setXmlData(const QString& dataName, const QString &valeur, QString& dataXml, int noConvertCharToHtml /* =0 */)
{
    QVariant v = valeur;
    return setXmlData( dataName, v.toByteArray() , dataXml, noConvertCharToHtml);
}
//---------------------------- setXmlData static ------------------------------------------------
/*! \brief place dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
 *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
 *  \param dataName : String indiquant le nom du tag
 *  \param valeur :    QByteArray valeur a ecrire
 *  \param dataXml :  String XML dans laquelle on va ecrire la valeur
 *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherche.
 *  \param noConvertCharToHtml  if zero (default value) '>' '<' '&' are  converted in '&gt;' '&lt;' '&amp;'
 *  \return true si tout est ok false sinon (en general le tag n'a pas ete trouve)
*/

bool CGestIni::setXmlData(const QString& dataName,  QByteArray valeur, QString& dataXml, int noConvertCharToHtml /* =0 */)
{bool mustBeB64Protected = false;
 int posEnd  = -1;
 int posDeb  =  0;
 if (noConvertCharToHtml == 0)
    { valeur.replace('&',"&amp;");
      valeur.replace('>',"&gt;");
      valeur.replace('<',"&lt;");
    }
 QString tag =  dataName+'>';
 /*if (nextPos) {posDeb  = dataXml.indexOf(tag, *nextPos);*nextPos=0;}  // on  remet a zero au cas ou tag pas trouve (recommencer a zero)
 else        */ {posDeb  = dataXml.indexOf(tag);}
 if (posDeb==-1)         return false;
 if (dataXml.at(posDeb-1)=='_') {posDeb += tag.length(); mustBeB64Protected = true; tag = tag.prepend("</_");}
 else                           {posDeb += tag.length(); tag = tag.prepend("</");}
 posEnd  = dataXml.indexOf(tag, posDeb);
 if (posEnd==-1)         return false;
 //if (nextPos) *nextPos = posEnd + tag.length();
 dataXml = dataXml.remove(posDeb,posEnd-posDeb);

 if (mustBeB64Protected) dataXml = dataXml.insert(posDeb,valeur.toBase64());
 else                    dataXml = dataXml.insert(posDeb,valeur);

 return true;
}
//---------------------------- getXmlDataList static --------------------------------------------------------------------
/*! \brief retourne une QStringList de valeurs a  partir d'un fichier XML et d'un tag
 *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
 *  \param tagName :  String indiquant le nom du tag sans les </ ou < ou >
 *  \param dataXml :  String XML dans laquelle on va lire la valeur
 *  \param nextPos :  pointeur sur un int qui si il n'est pas egal a zero indique
 *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherche.
 *  \return QStringList qui est la liste de valeurs a rechercher.
 *  LIMITE : il ne faut pas qu'un tag dont on recherche une liste, soit aussi la fin d'un autre
 *           exemple je veux faire la liste des <dir></dir>           si dans le fichier xml il y a
 *                                            <truc_dir></truc_dir>   ca ne le fera pas
 *                   car                            ^^^        ^^^
*/
QStringList CGestIni::getXmlDataList(const QString& tagName, const QString& dataXml, int *nextPos /* =0 */)
{bool mustBeB64Protected = false;
 int posEnd    = -1;
 int posDeb    =  0;
 QString tag   =  tagName+'>';
 QString toAdd = "";
 QStringList retList;

 if (nextPos) {posDeb  = dataXml.indexOf(tag, *nextPos);*nextPos=0;}  // on  remet a zero au cas ou tag pas trouve (recommencer a zero)
 else         {posDeb  = dataXml.indexOf(tag);}

 while (posDeb != -1)
       {if (dataXml.at(posDeb-1)=='_') {posDeb += tag.length(); mustBeB64Protected = true; tag = tag.prepend("</_");}
        else                           {posDeb += tag.length(); tag = tag.prepend("</");}
        posEnd  = dataXml.indexOf(tag, posDeb);
        if (posEnd==-1)         return retList;


        if (mustBeB64Protected) toAdd = QString( QByteArray::fromBase64( dataXml.mid(posDeb,posEnd-posDeb).toLatin1() ) );
        else                    toAdd = dataXml.mid(posDeb,posEnd-posDeb);

        posDeb = posEnd + tag.length();      // on se place apres le tag de fin donc a la prochaine position
        if (nextPos) *nextPos = posDeb;      // on retourne si besoin cette prochaine position

        toAdd.replace("&gt;",">");
        toAdd.replace("&lt;","<");
        toAdd.replace("&amp;","&");
        retList.append(toAdd);
        tag    =  tagName+'>';              // reinitialiser le tag car a ete modifie avant par tag = tag.prepend("</");
        posDeb = dataXml.indexOf(tag, posDeb);
       }
 return retList;
}

//-----------------------------------------------------  findFermant -----------------------------
/*! \brief Trouve le motif fermant correspondant au motif ouvrant
 *  \param QString &text   texte ou doit se faire la recherche
 *  \param int pos_deb     position dans le texte ou commence la recherche doit etre apres l'ouvrant dont on cherche le fermant
 *  \param const QString &ouvrant       motif ouvrant
 *  \param const QString &fermant       motif fermant
 *  \return position sur le motif fermant.
*/
int  CGestIni::findFermant(const QString &ptext, int pos, int pos_max, const QString &ouvrant, const QString &fermant)
{int isWithToken;
 return findFermant(ptext, pos, pos_max, ouvrant, fermant, isWithToken);
}

//-----------------------------------------------------  findFermant -----------------------------
/*! \brief Trouve le motif fermant correspondant au motif ouvrant
 *  \param QString &text   texte ou doit se faire la recherche
 *  \param int pos_deb     position dans le texte ou commence la recherche doit etre apres l'ouvrant dont on cherche le fermant
 *  \param const QString &ouvrant       motif ouvrant
 *  \param const QString &fermant       motif fermant
 *  \return position sur le motif fermant.
*/
int  CGestIni::findFermant(const QString &ptext, int pos, int pos_max, const QString &ouvrant, const QString &fermant, int &isWithToken)
{ return findFermant( &ptext,  pos,  pos_max, ouvrant, fermant, isWithToken);
}
//-----------------------------------------------------  findFermant -----------------------------
/*! \brief Trouve le motif fermant correspondant au motif ouvrant
 *  \param QString *ptext   texte ou doit se faire la recherche
 *  \param int pos_deb     position dans le texte ou commence la recherche doit etre apres l'ouvrant dont on cherche le fermant
 *  \param const QString &ouvrant       motif ouvrant
 *  \param const QString &fermant       motif fermant
 *  \return position sur le motif fermant.
*/
int  CGestIni::findFermant( const QString *ptext, int pos, int pos_max, const QString &ouvrant, const QString &fermant, int &isWithToken)
{ int stack        = 1;
  int ouvrant_len  = ouvrant.length();
  int fermant_len  = fermant.length();
  isWithToken      = 0;
  //QString deb = ptext->mid(pos,250);
  //QString end = ptext->mid(pos_max-10,250);
  while (stack && pos < pos_max)
    {if (ptext->at(pos)==ouvrant.at(0) && ptext->mid(pos, ouvrant_len) == ouvrant)
        {++   stack;
         pos += ouvrant_len;
         //deb = ptext->mid(pos,250);
         ++   isWithToken;
        }
     else if (ptext->at(pos)==fermant.at(0) && ptext->mid(pos, fermant_len) == fermant)
        {--   stack;
         //end = ptext->mid(pos,250);
         pos += fermant_len;
        }
     else
        {++pos;
         //deb = ptext->mid(pos,250);
        }
   }
 return pos-fermant_len;    // - fermant_len pour pointer avant le fermant
}
//-----------------------------------------------------  toUtf8 --------------------------------------------
/*! \brief converti le terme word passe en entree en Utf8 quelque soit son encodage. si deja UTF8 il est retourne tel quel
 *  \param word   mot a tester et eventuellement convertir
 *  \return utf8 converted word
*/
QString CGestIni::toUtf8( const QString &word)
{  //............ conversion utf8 ............
   QVariant vr(word);
   if ( IsUtf8( vr.toByteArray()) )
      { return word;
      }
   else
      {
        return QString::fromLatin1(vr.toByteArray());
      }
}
//-----------------------------------------------------  IsUtf8 --------------------------------------------
/*! \brief gestion de l'Utf8.
*/
QString CGestIni::CharStrToQStringUtf8( const  char *ptr)
{
#if QT_VERSION >= 0x040790
         return  fromMyUTF8( ptr );
#else

         if (IsUtf8( ptr))
            { return  QString::fromUtf8 ( ptr ); // fromMyUTF8( ptr );
            }
         else
            { return QString(ptr) ;
            }
#endif
}

//-----------------------------------------------------  IsUtf8 -------------------------------------------------
/*! \brief gestion de l'Utf8.
*/
int CGestIni::IsUtf8(const QByteArray &ba)
{ return IsUtf8 ( ba.constData() );
}

//-----------------------------------------------------  IsUtf8 -------------------------------------------------
/*! \brief gestion de l'Utf8.
*/
int CGestIni::IsUtf8(const QByteArray *ba)
{
    return IsUtf8( ba->constData() );
 /*
 char unsigned *pt   = (char unsigned*) ba->constData();
 char unsigned *end  = pt + ba->size();
 if (pt==0 || *pt==0) return 0;
 int nb_utf8_paterns = 0;

 while (pt<end && *pt)
      { if      (*pt >= 128)
                {if ( (*pt&224)==192 && (pt[1]&192)==128)
                    { ++nb_utf8_paterns;
                      ++pt;
                    }
                 else if ( (*pt&240)==224 && (pt[1]&192)==128 && (pt[2]&192)==128)
                    { ++nb_utf8_paterns;
                      pt += 2;
                    }
                 else if ( (*pt&248)==240 && (pt[1]&192)==128 && (pt[2]&192)==128 && (pt[3]&192)==128)
                    { ++nb_utf8_paterns;
                      pt += 3;
                    }
                else return 0;    // si car >= 128 sans un des paterns sus jacents --> on est pas en UTF8
               }
        ++pt;
      }
 return nb_utf8_paterns;
 */
}
//-----------------------------------------------------  IsUtf8 -------------------------------------------------
/*! \brief gestion de l'Utf8.
*/
int CGestIni::IsUtf8( const  char *txt)
{char unsigned *pt   = (char unsigned*) txt;
 if (pt==0 || *pt==0) return 0;
 int nb_utf8_paterns = 0;

 while (*pt)
      { if      (*pt >= 128)
                {if ( (*pt&224)==192 && (pt[1]&192)==128)
                    { ++nb_utf8_paterns;
                      ++pt;
                    }
                 else if ( (*pt&240)==224 && (pt[1]&192)==128 && (pt[2]&192)==128)
                    { ++nb_utf8_paterns;
                      pt += 2;
                    }
                 else if ( (*pt&248)==240 && (pt[1]&192)==128 && (pt[2]&192)==128 && (pt[3]&192)==128)
                    { ++nb_utf8_paterns;
                      pt += 3;
                    }
                else return 0;    // si car >= 128 sans un des paterns sus jacents --> on est pas en UTF8
               }
        ++pt;
      }
 return nb_utf8_paterns;
}
//--------------------------------- Utf8_Query -----------------------------------
/*! \brief gestion de l'Utf8.
*/
QString CGestIni::Utf8_Query(QSqlQuery *pCur, int field)
{//QByteArray   ba  =  pCur->value(field).toByteArray();
 //char       *ptr  =  ba.data();
 return fromMyUTF8( pCur->value(field).toByteArray() );
 /*
 if (IsUtf8( ptr ))
    {//return QString::fromUtf8 ( ptr );
     return fromMyUTF8(ptr);
    }
 else
    {return QString(ptr) ;
    }
 */
}
//--------------------------------- Utf8_Query -----------------------------------
/*! \brief gestion de l'Utf8.
*/
QString CGestIni::Utf8_Query(QSqlQuery &cur, int field)
{ //QByteArray   ba  =  cur.value(field).toByteArray();
 //char       *ptr  =  ba.data();
 return fromMyUTF8(cur.value(field).toByteArray());
 /*
 if (IsUtf8( ptr ))
    {//return QString::fromUtf8 ( ptr );
     return fromMyUTF8(ptr);
    }
 else
    {return QString(ptr) ;
    }
 */
}
//---------------------------------------------- fromMyUTF8 ------------------------------------------------
/*! \brief converti l'UTF8 avec une methode qui fonctionne en attendant de resoudre le PB par un moyen plus propre
 *  \param const QByteArray &ba donnees a convertir
 *  \return QString convertie
 *  * RAPPEL : 0xxx xxxx                                    // poids fort a  zero --> US-ASCII
 *          110x xxxx   10xx xxxx                        // code UTF8 sur deux octets    de  8 a  11 bits
 *          1110 xxxx   10xx xxxx  10xx xxxx             // code UTF8 sur trois octets   de 12 a  16 bits
 *          1110 xxxx   10xx xxxx  10xx xxxx  10xx xxxx  // code UTF8 sur quatre octets  de 17 a  21 bits
 */
QString CGestIni::fromMyUTF8(const QByteArray &ba)
{
  return fromMyUTF8( ba.constData() );
}

//---------------------------------------------- fromMyUTF8 ------------------------------------------------
/*! \brief converti l'UTF8 explosee venant des SqlQuery, avec une methode qui fonctionne a peu pres en attendant de resoudre le PB par un moyen plus propre
 *  \param ptr : const char* chaine codee en UTF8
 *  \return QString convertie
 * RAPPEL : 0xxx xxxx                                    // poids fort a  zero --> US-ASCII
 *          110x xxxx   10xx xxxx                        // code UTF8 sur deux octets    de  8 a  11 bits
 *          1110 xxxx   10xx xxxx  10xx xxxx             // code UTF8 sur trois octets   de 12 a  16 bits
 *          1111 xxxx   10xx xxxx  10xx xxxx  10xx xxxx  // code UTF8 sur quatre octets  de 17 a  21 bits
*/

QString CGestIni::fromMyUTF8(const char* ptr)
{QString ret = "";
 char unsigned *pt   = (char unsigned*) ptr;
 if (pt==0 || *pt==0)           return ret;
 while (*pt)
      { if      (*pt >= 128)   //......... unicode possible .................
                {if ( (*pt&224)==192 && (pt[1]&192)==128)
                    { int code_deuze  = 0;
                      int code_first  = pt[0]&3;
                      code_first      = code_first<<6;
                      code_first     += pt[1]&63;
                      if (code_first==195 && (pt[2]&192)==192)
                         {code_deuze  = pt[2]&3;
                          code_deuze  = code_deuze<<6;
                          code_deuze += pt[3]&63;
                          pt         += 2;
                          if (code_deuze>=142 && code_deuze<=195)
                             {ret += QChar (0x00CE + code_deuze-142);         // case 142: ret += QChar (0x00CE);       break;
                             }
                          else
                             {switch (code_deuze)
                                 {case 198: ret += QChar (0x00C8);       break;   //
                                  case 130: ret += QChar (0x00C0); ++pt; break;   //
                                  case 128: ret += QChar (0x00C9); ++pt; break;   //
                                  case 96:  ret += QChar (0x00CA);       break;   //
                                 }
                            }
                         }
                      else
                         {switch(code_first)
                             {case 232:  ret += QChar (0x00E8);       break;
                              case 233:  ret += QChar (0x00E9);       break;
                              default:   ret += QChar (code_first);
                             }
                         } // end if (code_first==195 && (pt[2]&192)==192)
                      pt += 2;
                    }
                 else if ( (*pt&240)==224 && (pt[1]&192)==128 && (pt[2]&192)==128)
                    { pt += 2;
                    }
                 else if ( (*pt&248)==240 && (pt[1]&192)==128 && (pt[2]&192)==128 && (pt[3]&192)==128)
                    { pt += 3;
                    }
                else
                    {int c =  *pt;
                     switch (c)
                        {case 194 : if (pt[1]=='?') {ret += QChar (0x0153);    ++pt;}   break;   //
                         case 195 : if (pt[1]=='?') {ret += QChar (0x0045);    ++pt;}   break;   //
                         case 156 :                 {ret += QChar (0x0153);         }   break;   //
                         default  : ret += *pt;
                        }
                      ++pt;
                    }
               }
           else   //.......... US-ASCI .................
               { ret += *pt;
                 ++pt;
               }
      }
 return ret;
}

//----------------------------------------- getListBetweenTwoTags ---------------------------------------------
/*! \brief cree la liste des termes trouves entre deux motifs textuel dans un texte quelconque.
 *  \param textToExamine : texte dans lequel rechercher les termes encadres par tag1 et tag2.
 *  \param tag1 : premier motif encadrant ou tag
 *  \param tag1 : deuxieme motif encadrant ou tag
 *  \param cleanMode :  0/si pas de nettoyage 0/ on vire espaces de fin et debut \r\n
 *  \return   une QStringList qui contient la liste des termes encadres par les deux tags
*/
QStringList   CGestIni::getListBetweenTwoTags(const QString &textToExamine, const QString &tag1, const QString &tag2, int cleanMode /*=1*/ )
{QStringList retList;
 int     pos   = 0;
 int     end   = 0;
 QString terme = "";
 while ( end  !=  -1  )
     { pos   = textToExamine.indexOf(tag1,pos);
       if (pos==-1) break;
       pos  += tag1.length();
       end   = textToExamine.indexOf(tag2,pos);
       if (end==-1) break;

       terme  = textToExamine.mid(pos,end-pos).trimmed();
       if (cleanMode==1) terme = terme.remove("\r").remove("\t").remove("\n");
       if (terme.length()) retList << terme;
       pos = end + tag2.length();
     }
 return retList;
}
//----------------------------------------- getTextBetweenTwoTags ---------------------------------------------
/*! \brief recherche le terme situe entre deux motifs textuel dans un texte quelconque.
 *  \param textToExamine : texte dans lequel rechercher les termes encadres par tag1 et tag2.
 *  \param tag1 : premier motif encadrant ou tag
 *  \param tag1 : deuxieme motif encadrant ou tag
 *  \param from : position dans laquelle commencer la recherche
 *  \param cleanMode :  0/si pas de nettoyage 0/ on vire espaces de fin et debut \r\n
 *  \return   une QString qui contient le premier terme encadre par les deux tags
*/
QString   CGestIni::getTextBetweenTwoTags(const QString &textToExamine, const QString &tag1, const QString &tag2, int cleanMode /*=1*/ )
{ int          from = 0;
  QString returnTag = "";
  if ( getTextBetweenTwoTags(textToExamine, returnTag, tag1, tag2, from , cleanMode )==-1) return "";
  return returnTag;
}

//----------------------------------------- getTextBetweenTwoTags ---------------------------------------------
/*! \brief recherche le terme situe entre deux motifs textuel dans un texte quelconque.
 *  \param textToExamine : texte dans lequel rechercher les termes encadres par tag1 et tag2.
 *  \param returnTag     : reference sur une QString dans laquelle sera retournee le terme recherche.
 *  \param tag1          : premier motif encadrant ou tag
 *  \param tag1          : deuxieme motif encadrant ou tag
 *  \param from          : position a partir de laquelle commencer la recherche
 *  \param cleanMode     :  0/si pas de nettoyage 0/ on vire espaces de fin et debut \r\n
 *  \return   position apres le terme trouve (-1 si pas de terme trouve)
*/
int   CGestIni::getTextBetweenTwoTags(const QString &textToExamine, QString &returnTag, const QString &tag1, const QString &tag2, int &from /*=0*/, int cleanMode /*=1*/ )
{int     pos   = from;
 int     end   = 0;
 QString terme = "";

 returnTag     = "";
 from          = textToExamine.length();                  // par defaut on se calle a la fin du texte (eviter les boucles infinies)
 pos           = textToExamine.indexOf(tag1,pos);
 if (pos==-1) return -1;
 pos          += tag1.length();
 end           = textToExamine.indexOf(tag2,pos);
 if (end==-1) return -1;

 returnTag     = textToExamine.mid(pos,end-pos).trimmed();
 if (cleanMode==1) terme = terme.remove("\r").remove("\t").remove("\n");
 from          = end + tag2.length();                    // on rectifie la position APRES le terme recherche
 return from;
}
//-----------------------------------------------------  Param_WriteParam -----------------------------------
/*! \brief Ecrit dans la string des parametres passee a la fonction.
 *  \param param : String parametres dans laquelle on va ecrire. Elle sera modifiee.
 *  \param section : section dans laquelle on ecrit
 *  \param variable : variable concernee
 *  \param val1 -> val10 : valeurs a ecrire.
 *  \return QString::null en cas d'erreur, sinon le String de parametre au complet.
 * Si la variable n'est pas trouvee dans la section elle est ajoutee. Si la section n'existe pas elle est ajoutee en fin de fichier.
*/
QString CGestIni::Param_WriteParam( QString *pQstr, const char *section, const char  *variable,
                                      const char *val1, const char *val2, const char *val3, const char *val4, const char *val5,
                                      const char *val6, const char *val7, const char *val8, const char *val9, const char *val10)
{if (pQstr == 0        )    return QString::null;
 if (pQstr->length()==0 && section && *section)   *pQstr = QString("[")+section+"]\r\n" ;
 if (pQstr->length()==0)    return QString::null;

 QString result    = "";
 QString tmp       = "";

 QByteArray ba     = pQstr->toLatin1 ();
 char* txt         = ba.data();
 char *pt          = txt;
 char *deb         = 0;
 char *end         = 0;
 char *deb_lgn     = 0;
 char *end_lgn     = 0;
 long len_section  = 0;
 long len_variable = 0;
 long         pos  = 0;
 while (*pt)
 {switch (pt[0])
    {case '\r':
     case '\n':
     case ';':
          pt = GotoNextDebLigne(pt);
          break;
     case '\\':
          if      ( pt[1] =='\\') pt = GotoNextDebLigne(pt);
          else if ( pt[1] !=0 )   pt +=2;
          break;
     /*
     case '\"':
          while (*pt && *pt != '\"')
             { if (*pt=='\\' && pt[1] !=0 ) ++pt;
               ++pt;
             }
          break;
     */
     case '[': //....................debut de section: tester si c'est celle recherchee ...................
          ++pt;
          while (*pt && (*pt==' '||*pt=='\t'))  ++pt;                // enlever espaces de debut
          deb = pt;
          while (*pt && *pt != ']') ++pt;                            // chercher delimiteur de fin de nom de section
          end = pt;
          while (end>deb && (end[-1]==' '||end[-1]=='\t')) end--;    // enlever espaces de fin
          len_section = end - deb;
          if ((long)strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchee
             {pt = GotoNextDebLigne(pt);                                               // aller ligne suivante

              while (*pt && *pt != '[')                                                // tant que pas fin de zone de section (debut d'une autre)
                 {
                  while (*pt && (*pt==' '||*pt=='\t'))  ++pt ;                               // sauter espaces de debut de ligne
                  if  (*pt==';' || *pt=='\n'|| *pt =='\r') pt = GotoNextDebLigne(pt) ;       // sauter les commentaires et retour ligne
                  deb = pt;                       // on devrait etre au debut du nom d'une variable
                  while (*pt && *pt!='=' && *pt !='\n' && *pt!='\r' && *pt!='[') ++pt;      // le signe '=' marque la fin de zone du nom de la variable

                  //................... fin de nom de variable trouve ( signe egal) ...........................
                  if (*pt=='=')
                     {end = pt;
                      while (end>deb && (end[-1]==' '||end[-1]=='\t')) end-- ;   // enlever espaces de fin du nom de la variable
                      len_variable = end - deb;
                      //.......................... Est ce celle recherchee ...................................
                      if ((long)strlen(variable)==len_variable && strncmp(variable, deb, len_variable)==0)   // SI c'est la variable recherchee
                         {deb_lgn = deb;
                          end_lgn = end;
                          while (*end_lgn && *end_lgn !=';' && *end_lgn !='\n' && *end_lgn!='\r')      // aller jusqu'au prochain marqueur de fin des valeurs
                                {if (*end_lgn=='\\' && end_lgn[1] !=0 ) end_lgn++;                     // en sautant le caractere d'echapement et le suivant
                                 end_lgn++;
                                }
                          while (end_lgn> end && (end_lgn[-1] ==' ' || end_lgn[-1] =='\t')) end_lgn--;  // reculer j'usqu'a  la fin de la derniere valeur
                          //........................... maintenant on peut supprimer et remplacer ........
                          pos    = deb_lgn-txt;
                          result = pQstr->left(pos) + variable + " = ";
                          if (val1)     result = result +         val1;
                          if (val2)     result = result + " , " + val2;
                          if (val3)     result = result + " , " + val3;
                          if (val4)     result = result + " , " + val4;
                          if (val5)     result = result + " , " + val5;
                          if (val6)     result = result + " , " + val6;
                          if (val7)     result = result + " , " + val7;
                          if (val8)     result = result + " , " + val8;
                          if (val9)     result = result + " , " + val9;
                          if (val10)    result = result + " , " + val10;
                          result   += pQstr->mid(end_lgn-txt);
                          *pQstr    = result;
                          return result;
                         }  //end if (strlen(variable)==len_variable && strncmp(variable, deb, len_variable)==0)
                      pt = GotoNextDebLigne(pt);                                               // aller ligne suivante
                     } // end if (*pt=='=')
                 } // end while (*pt && *pt != '[')
               //.......................... fin de section atteinte sans avoir trouve la variable .........
               //                           la section est trouvee mais pas la variable on la rajoute
               tmp = "";
               while (pt>txt && (pt[-1]=='\r'||pt[-1]=='\n' ||pt[-1]=='\t'||pt[-1]==' ')) pt--;  // nettoyer avant eliminer espaces et  CR/LF avant
               tmp = tmp + "\r\n";
               tmp = tmp + "  " + variable + " = ";
               if (val1)     tmp  = tmp +         val1;
               if (val2)     tmp  = tmp + " , " + val2;
               if (val3)     tmp  = tmp + " , " + val3;
               if (val4)     tmp  = tmp + " , " + val4;
               if (val5)     tmp  = tmp + " , " + val5;
               if (val6)     tmp  = tmp + " , " + val6;
               if (val7)     tmp  = tmp + " , " + val7;
               if (val8)     tmp  = tmp + " , " + val8;
               if (val9)     tmp  = tmp + " , " + val9;
               if (val10)    tmp  = tmp + " , " + val10;
               tmp     = tmp + "\r\n";
               pos     = pt-txt;
               result  = pQstr->left(pos) + tmp ;
               while (*pt && (*pt=='\r' || *pt=='\n' || *pt=='\t' || *pt==' ')) ++pt;           // nettoyer apres eliminer espaces et  CR/LF apres
               pos     = pt-txt;
               result += pQstr->mid(pos);
               *pQstr  = result;
               return result;
              } // end if (strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchee
          break;
      default:
          ++pt;
     } // end switch (pt[0])
  } // end while (*pt)

 //................pas de section trouvee on la rajoute ...........................................
 tmp = "\r\n[";
 tmp = tmp + section + "]\r\n  " + variable + " = ";
 if (val1)     tmp  = tmp +         val1;
 if (val2)     tmp  = tmp + " , " + val2;
 if (val3)     tmp  = tmp + " , " + val3;
 if (val4)     tmp  = tmp + " , " + val4;
 if (val5)     tmp  = tmp + " , " + val5;
 if (val6)     tmp  = tmp + " , " + val6;
 if (val7)     tmp  = tmp + " , " + val7;
 if (val8)     tmp  = tmp + " , " + val8;
 if (val9)     tmp  = tmp + " , " + val9;
 if (val10)    tmp  = tmp + " , " + val10;
 tmp     =  tmp  + "\r\n";
 //.............. eliminer tous les espaces rn tab  arrieres .......................
 pos = pQstr->length();
 while (pos>0)
       {QChar c= pQstr->at(pos-1);
        if (c=='\t' || c=='\r'|| c=='\n'|| c==' ') --pos;
        else                                       break;
       }
 pQstr->truncate(pos);   // [END_INI]
 //.............. si fichier avec [END_INI] le replace en fin de fichier  .......................
 if (pQstr->mid(pos-9,9)=="[END_INI]")
    { pQstr->truncate(pos-9);
      pos -=9;
      //.............. reeliminer tous les espaces rn tab  arrieres .......................
      while (pos>0)
       {QChar c= pQstr->at(pos-1);
        if (c=='\t' || c=='\r'|| c=='\n'|| c==' ') --pos;
        else                                       break;
       }
      pQstr->truncate(pos);
      result  = *pQstr + tmp + "\r\n[END_INI]";
    }
 else  //.......  il n' y a pas [END_INI] (cas des fichiers non .ini) ...................
    { result  = *pQstr + tmp ;
    }
 *pQstr  = result;
 return result;
}
//-----------------------------------------------------  Param_ReadUniqueParam --------------------------------
/*! \brief lit une valeur dans un fichier de configuration.
 *  \param txt : String parametres dans laquelle on va lire
 *  \param section : section dans laquelle on va lire
 *  \param variable : variable concernee
 *  \param pos : position de la valeur a retourner si plusieurs valeurs separees par des virgules
 *  \return QString::null la variable n'existe pas. sinon retourne la valeur
*/
QString CGestIni::Param_ReadUniqueParam(const char* txt, const char *section, const char  *variable, int pos /* = -1 */)
{
 QString val;
 if (pos != -1)
    { QString values[10];
      pos = qMin(9,pos);
      QString ret =  CGestIni::Param_ReadParam(   txt, section, variable,  &values[0], &values[1], &values[2], &values[3], &values[4],
                                                                           &values[5], &values[6], &values[7], &values[8], &values[9]);
      if (ret.length()!=0) return "";
      else                 return values[pos];
    }
 if (Param_ReadParam( txt, section, variable, &val)==QString::null) return val;
 else return QString::null;
}
//-----------------------------------------------------  Param_ReadLine --------------------------------
/*! \brief recupere les donnees situees apres le signe egal de la variable d'une section donnee
 *  \param txt : String parametres dans laquelle on va lire
 *  \param section : section dans laquelle on va lire
 *  \param variable : variable concernee
 *  \return QString las donnees
*/
QString CGestIni::Param_ReadLine(  const char* txt, const char *section, const char  *variable)

{if (txt==0)   return QObject::tr("Syntax error: empty parameter file");
 char *pt          = (char*) txt;
 char *endl        = 0;
 char *deb         = 0;
 char *end         = 0;
 long len_section  = 0;
 long len_variable = 0;
 if (section==0)
    {QString str(QObject::tr("Error : no section in the call of function CGestIni::Param_ReadParam()"));
     //qDebug(str);
     return str;
    }
 while (*pt)
 {switch (pt[0])
    {case '\r':
     case '\n':
     case ';':
          pt = GotoNextDebLigne(pt);
          break;
     case '/':
          if      ( pt[1] =='/')  pt = GotoNextDebLigne(pt);
          else if ( pt[1] !=0 )   pt +=2;
          break;
     /*
     case '\"':
          while (*pt && *pt != '\"')
             { if (*pt=='\\' && pt[1] !=0 ) ++pt;
               ++pt;
             }
          break;
     */
     case '[': //....................debut de section: tester si c'est celle recherchee ...................
          ++pt;
          while (*pt && (*pt==' '||*pt=='\t'||*pt=='\a'))  ++pt;                // enlever espaces de debut
          deb = pt;
          while (*pt && *pt != ']') ++pt;                            // chercher delimiteur de fin de nom de section
          end = pt;
          while (end>deb && (end[-1]==' '||end[-1]=='\t'|| end[-1]=='\a')) end--;    // enlever espaces de fin
          len_section = end - deb;
          if ((long)strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchee
             {pt = GotoNextDebLigne(pt);
              while (*pt && (*pt==' '||*pt=='\t'||*pt=='\r'||*pt=='\n'||*pt=='\a'))  ++pt ;                                                 // aller ligne suivante (zone des variables de section)
              while (*pt && *pt != '[')                                                      // tant que pas fin de zone de section (debut d'une autre ou fin)
                 {
                  while (*pt && (*pt==' '||*pt=='\t'||*pt=='\a'))  ++pt ;                                             // sauter espaces de debut de ligne
                  if  (*pt==';' || *pt=='\n'|| *pt =='\r'||*pt=='\a') { pt = GotoNextDebLigne(pt) ;                   // sauter les commentaires et retour ligne en allant ligne suivante
                                                             while (*pt && (*pt==' '||*pt=='\t'||*pt=='\a'))  ++pt ;  // ressauter espaces de debut de ligne
                                                           }
                  else if (*pt=='/'&& pt[1] =='/')         { pt = GotoNextDebLigne(pt);
                                                             while (*pt && (*pt==' '||*pt=='\t'||*pt=='\a'))  ++pt ;  // ressauter espaces de debut de ligne
                                                           }
                  deb = pt;                       // on devrait etre au debut du nom d'une variable
                  //while (*pt && *pt!='=')  ++pt;  // le signe '=' marque la fin de zone du nom de la variable
                  while (*pt && *pt!='=' && *pt!='[')
                        {if (*pt=='\\' && pt[1] !=0 ) pt += 2;
                         else ++pt;
                        }
                  if (*pt==0||*pt=='[') return QObject::tr("Sign = not found in section: ") + section + QObject::tr(", requested variable: ") + variable ;
                  end = pt;
                  while (end>deb && (end[-1]==' '||end[-1]=='\t'|| end[-1]=='\a')) end-- ;   // enlever espaces de fin du nom de la vaiable
                  len_variable = end - deb;
                  if ((long)strlen(variable)==len_variable && strncmp(variable, deb, len_variable)==0)   // SI c'est la variable recherchee
                     {++pt;   // passer le signe egal
                      //Param_ExtraireValeurs(pt, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10);
                      endl = GotoEndOfLigne(pt);
                      return utf8Conv(pt,endl-pt).trimmed();
                     }
                  pt = GotoNextDebLigne(pt);                                               // aller ligne suivante
                 }
               //......... si on arrive ici c'est que ok pour la section mais pas de variable ..........................
               return QObject::tr("No variable: ") + variable + QObject::tr("  for section : ") + section;
              } // end if (strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchee
          break;
      default:
          ++pt;
     } // end switch (pt[0])
  } // end while (*pt)
 return QObject::tr("Syntax error: no section in this file");
}

//-----------------------------------------------------  Param_ReadParam --------------------------------
/*! \brief recupere les valeurs au sein du String parametre passe a la fonction.
 *  \param txt : String parametres dans laquelle on va lire
 *  \param section : section dans laquelle on va lire
 *  \param variable : variable concernee
 *  \param val1 -> val10 : valeurs a lire
 *  \return QString::null si tout OK. sinon retourne le message d'erreur
*/
QString CGestIni::Param_ReadParam(  const char* txt, const char *section, const char  *variable,
                                    QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                    QString *val6, QString *val7, QString *val8, QString *val9, QString *val10)

{if (txt==0)   return QObject::tr("Syntax error: empty parameter file");
 char *pt          = (char*) txt;
 char *deb         = 0;
 char *end         = 0;
 long len_section  = 0;
 long len_variable = 0;
 if (section==0)
    { return QObject::tr("Error : no section in the call of function CGestIni::Param_ReadParam()");
    }
 while (*pt)
 {switch (pt[0])
    {case '\r':
     case '\n':
     case ';':
          pt = GotoNextDebLigne(pt);
          break;
     case '/':
          if      ( pt[1] =='/')  pt = GotoNextDebLigne(pt);
          else if ( pt[1] !=0 )   pt +=2;
          break;
     /*
     case '\"':
          while (*pt && *pt != '\"')
             { if (*pt=='\\' && pt[1] !=0 ) ++pt;
               ++pt;
             }
          break;
     */
     case '[': //....................debut de section: tester si c'est celle recherchee ...................
          ++pt;
          while (*pt && (*pt==' '||*pt=='\t'||*pt=='\a'))  ++pt;                // enlever espaces de debut
          deb = pt;
          while (*pt && *pt != ']') ++pt;                            // chercher delimiteur de fin de nom de section
          end = pt;
          while (end>deb && (end[-1]==' '||end[-1]=='\t'|| end[-1]=='\a')) end--;    // enlever espaces de fin
          len_section = end - deb;
          if ((long)strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchee
             {pt = GotoNextDebLigne(pt);
              while (*pt && (*pt==' '||*pt=='\t'||*pt=='\r'||*pt=='\n'||*pt=='\a'))  ++pt ;                                                 // aller ligne suivante (zone des variables de section)
              while (*pt && *pt != '[')                                                      // tant que pas fin de zone de section (debut d'une autre ou fin)
                 {
                  while (*pt && (*pt==' '||*pt=='\t'||*pt=='\a'))  ++pt ;                                             // sauter espaces de debut de ligne
                  if  (*pt==';' || *pt=='\n'|| *pt =='\r'||*pt=='\a')
                                                           { pt = GotoNextDebLigne(pt) ;                   // sauter les commentaires et retour ligne en allant ligne suivante
                                                             while (*pt && (*pt==' '||*pt=='\t'||*pt=='\a'))  ++pt ;  // ressauter espaces de debut de ligne
                                                           }
                  else if (*pt=='/'&& pt[1] =='/')         { pt = GotoNextDebLigne(pt);
                                                             while (*pt && (*pt==' '||*pt=='\t'||*pt=='\a'))  ++pt ;  // ressauter espaces de debut de ligne
                                                           }
                  deb = pt;                       // on devrait etre au debut du nom d'une variable
                  //while (*pt && *pt!='=')  ++pt;  // le signe '=' marque la fin de zone du nom de la variable
                  while (*pt && *pt!='=' && *pt!='[')
                        {if (*pt=='\\' && pt[1] !=0 ) pt += 2;
                         else ++pt;
                        }
                  if (*pt==0||*pt=='[') return QObject::tr("Sign = not found in section: ") + section + QObject::tr(", requested variable: ") + variable ;
                  end = pt;
                  while (end>deb && (end[-1]==' '||end[-1]=='\t'|| end[-1]=='\a')) end-- ;   // enlever espaces de fin du nom de la vaiable
                  len_variable = end - deb;
                  if ((long)strlen(variable)==len_variable && strncmp(variable, deb, len_variable)==0)   // SI c'est la variable recherchee
                     {++pt;   // passer le signe egal
                      Param_ExtraireValeurs(pt, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10);
                      return QString::null;
                     }
                  pt = GotoNextDebLigne(pt);                                               // aller ligne suivante
                 }
               //......... si on arrive ici c'est que ok pour la section mais pas de variable ..........................
               return QObject::tr("No variable: ") + variable + QObject::tr("  for section : ") + section;
              } // end if (strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchee
          break;
      default:
          ++pt;
     } // end switch (pt[0])
  } // end while (*pt)
 return QObject::tr("Syntax error: no section in this file");
}


//-----------------------------------------------------  ExtraireValeurs -----------------------------------
/*! \brief txt pointe sur un texte contenant differentes valeurs terminees par \r\n ou zero de fin, et separees par des virgules. Une valeur peut etre une chaine de caracteres et si elle doit comporter une virgule ou point virgule ceux ci doivent etre annonces par le caractere d'echapement qui est l'anti slash
*/
char *CGestIni::Param_ExtraireValeurs(  const char* txt,
                                        QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                        QString *val6, QString *val7, QString *val8, QString *val9, QString *val10)
{char *pt          = (char*) txt;
 int num_param     = 0;
 char *deb         = 0;
 char *end         = 0;
 long   len        = 0;

 if (val1)  *val1  = QString::null;
 if (val2)  *val2  = QString::null;
 if (val3)  *val3  = QString::null;
 if (val4)  *val4  = QString::null;
 if (val5)  *val5  = QString::null;
 if (val6)  *val6  = QString::null;
 if (val7)  *val7  = QString::null;
 if (val8)  *val8  = QString::null;
 if (val9)  *val9  = QString::null;
 if (val10) *val10 = QString::null;

 while (1)
   {while (*pt && *pt !='\r' && *pt !='\n' && *pt !=';' && (*pt==' '||*pt=='\t'))  ++pt;      // sauter espaces de debut
    if (*pt==0 || *pt =='\r' || *pt =='\n'|| *pt ==';') return pt;
    deb = pt;                                                                    // on devrait etre au debut du nom d'une valeur
    while (*pt && *pt !=',' && *pt !='\n' && *pt!='\r')                          // aller jusqu'a la prochaine virgule (fin de la valeur)
       {if (*pt=='\\' && pt[1] !=0 ) ++pt;                                       // sauter le caractere d'echapement et le suivant
        ++pt;
       }
    end = pt;
    while (end>deb && (end[-1]==' '||end[-1]=='\t')) end--;                      // enlever espaces de fin
    len    = end-deb;
    switch(num_param)
           {case 0: if (val1)  *val1  = utf8Conv(deb,len);  break;
            case 1: if (val2)  *val2  = utf8Conv(deb,len);  break;
            case 2: if (val3)  *val3  = utf8Conv(deb,len);  break;
            case 3: if (val4)  *val4  = utf8Conv(deb,len);  break;
            case 4: if (val5)  *val5  = utf8Conv(deb,len);  break;
            case 5: if (val6)  *val6  = utf8Conv(deb,len);  break;
            case 6: if (val7)  *val7  = utf8Conv(deb,len);  break;
            case 7: if (val8)  *val8  = utf8Conv(deb,len);  break;
            case 8: if (val9)  *val9  = utf8Conv(deb,len);  break;
            case 9: if (val10) *val10 = utf8Conv(deb,len); break;
           }
    if (*pt==',')  ++pt;      // on saute la virgule et on passe au param suivant
    num_param ++;
   }
 return pt;
}
//-----------------------------------------------------  Param_SplitValeurs -----------------------------------
/*! \brief txt pointe sur un texte contenant differentes valeurs separees par un separateur, tout ce qui precede le signe = +1  sera coupe.
 *  \param txt :               String parametres dans laquelle on va lire les valeurs
 *  \param sep :               const QString &sep separateur de valeurs
 *  \param mustBeTrimmed :     false/values are not trimmed, true/values are trimmed
 *  \param QString *val1 -> QString *val10 : valeurs a remplir avec les donnees situees entre le separateur
 *  \return int nombre de valeur trouvees
*/
int  CGestIni::Param_SplitValeurs(  const QString &str,const QString &sep, bool mustBeTrimmed,
                                          QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                          QString *val6, QString *val7, QString *val8, QString *val9, QString *val10)
{   QString varName;
    return Param_SplitValeurs( str, sep, varName, mustBeTrimmed, val1, val2, val3, val4, val5,val6,val7,val8, val9, val10);
}
//-----------------------------------------------------  Param_SplitValeurs -----------------------------------
/*! \brief txt pointe sur un texte contenant differentes valeurs separees par un separateur, tout ce qui precede le signe = +1  sera coupe.
 *  \param txt :              String parametres dans laquelle on va lire les valeurs
 *  \param sep :              const QString &sep separateur de valeurs
 *  \param mustBeTrimmed :    false/values are not trimmed, true/values are trimmed
 *  \param varName : const QString &varName nom de ce qui precede le signe = (si il existe)
 *  \param QString *val1 -> QString *val10 : valeurs a remplir avec les donnees situees entre le separateur
 *  \return int nombre de valeur trouvees
*/
int  CGestIni::Param_SplitValeurs(  const QString &str,const QString &sep, QString &varName, bool mustBeTrimmed,
                                          QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                          QString *val6, QString *val7, QString *val8, QString *val9, QString *val10)
{   QString line = str;
    line               = line.remove('\r').remove('\n').remove('\t');
    varName            = CutStrLeft(line,"=").trimmed();
    QStringList   lst  = CutStrRight(line,"=").split(sep);
    int             i  = lst.count();
    if (val1)  {*val1  = ""; if (i>=1)  *val1=lst[0];   if (mustBeTrimmed) *val1  = val1->trimmed(); }
    if (val2)  {*val2  = ""; if (i>=2)  *val2=lst[1];   if (mustBeTrimmed) *val2  = val2->trimmed(); }
    if (val3)  {*val3  = ""; if (i>=3)  *val3=lst[2];   if (mustBeTrimmed) *val3  = val3->trimmed(); }
    if (val4)  {*val4  = ""; if (i>=4)  *val4=lst[3];   if (mustBeTrimmed) *val4  = val4->trimmed(); }
    if (val5)  {*val5  = ""; if (i>=5)  *val5=lst[4];   if (mustBeTrimmed) *val5  = val5->trimmed(); }
    if (val6)  {*val6  = ""; if (i>=6)  *val6=lst[5];   if (mustBeTrimmed) *val6  = val6->trimmed(); }
    if (val7)  {*val7  = ""; if (i>=7)  *val7=lst[6];   if (mustBeTrimmed) *val7  = val7->trimmed(); }
    if (val8)  {*val8  = ""; if (i>=8)  *val8=lst[7];   if (mustBeTrimmed) *val8  = val8->trimmed(); }
    if (val9)  {*val9  = ""; if (i>=9)  *val9=lst[8];   if (mustBeTrimmed) *val9  = val9->trimmed(); }
    if (val10) {*val10 = ""; if (i>=10) *val10=lst[9];  if (mustBeTrimmed) *val10 = val10->trimmed();}
    return i;
}

//----------------------------------------- utf8Conv ---------------------------------------------
/*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
*/
QString CGestIni::utf8Conv(const char* deb, long len)
{if (IsUtf8( deb))
    {return fromMyUTF8(deb);
    }
  else
    {return QString::fromLatin1(deb,len);
    }
}

//------------------------------ Param_RemoveSection -----------------------------------------
/*! \brief enleve une section.
 *  \param param   : String parametres dans laquelle supprimer une section
 *  \param section : section a supprimer
*/
QString CGestIni::Param_RemoveSection(QString &param, QString section)
{    section.prepend("[");
     section.append("]");
     int pos_deb  = param.indexOf(section);
     int pos_end  = -1;
     if (pos_deb != -1)
        {pos_end = param.indexOf("[", pos_deb+section.length());
         //.............. eliminer tous les espaces rn tab  arrieres .......................
         while (pos_deb>0){QChar c=param[pos_deb-1];
                           if (c=='\t' || c=='\r'|| c=='\n'|| c==' ') --pos_deb;
                           else                                           break;
                          }
         if (pos_end != -1)
            { param = param.remove(pos_deb, pos_end-pos_deb);
              param = param.insert(pos_deb,"r\n\r\n");
            }
         else
            { param.truncate(pos_deb);
            }
        }
    return param;
}
//----------------------------------------- Param_GetListFromPath ---------------------------------------------
/*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
 *  \param file_ini : nom d'un fichier de parametrage ou l'on va extraire une liste.
 *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
 *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
 *                             si terminee par * alors on prend toutes les variables commencant par ce qui est avant le * de varToRetrieve
 *                             si commence par * alors on prend toutes les variables terminees  par ce qui est apres le * de varToRetrieve
 *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas a  la premiere valeur entre = et ,)
 *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
 *  \param isUtf8 : pointeur sur un int (par defaut a  zero) qui si different de zero sera initialise a un si le fichier est en UTF8
 *  \param entireDataLine :  entireDataLine a zero par defaut seule la premiere valeur apres le signe egal et avant la premiere virgule sera retenue.
 *                           si a un,   toutes les donnees apres le signe = seront retenues
 *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
 *  \return QStringList lst la liste des variables
*/

QStringList CGestIni::Param_GetListFromPath(const QString &file_ini, const QString &sectionToRetrieve, const QString &varToRetrieve/*=""*/,  int isToStrip /*=0*/, int *isUtf8 /*=0*/, int entireDataLine /*=0*/)
{QString     outParam;
 QStringList lst;
 Param_UpdateFromDisk( file_ini,  outParam,  isUtf8 );
 Param_GetList( outParam, sectionToRetrieve, varToRetrieve, lst, isToStrip, entireDataLine);
 return lst;
}

//----------------------------------------- Param_GetListFromPath ---------------------------------------------
/*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
 *  \param file_ini : nom du fichier ou l'on va extraire une liste.
 *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
 *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
 *                             si terminee par * alors on prend toutes les variables commencant par ce qui est avant le * de varToRetrieve
 *                             si commence par * alors on prend toutes les variables terminees  par ce qui est apres le * de varToRetrieve
 *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas a  la premiere valeur entre = et ,)
 *  \param lst :        liste a initialiser et construire
 *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
 *  \param isUtf8 : pointeur sur un int (par defaut a  zero) qui si different de zero sera initialise a un si le fichier est en UTF8
 *  \param entireDataLine :  entireDataLine a zero par defaut seule la premiere valeur apres le signe egal et avant la premiere virgule sera retenue.
 *                           si a un,   toutes les donnees apres le signe = seront retenues
 *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
*/
void CGestIni::Param_GetListFromPath(const QString &file_ini, const QString &sectionToRetrieve, QStringList &lst, const QString &varToRetrieve/*=""*/,  int isToStrip /*=0*/, int *isUtf8 /*=0*/, int entireDataLine /*=0*/)
{Param_GetList(file_ini, sectionToRetrieve, lst, varToRetrieve, isToStrip , isUtf8 , entireDataLine );
}

//----------------------------------------- Param_GetList ---------------------------------------------
/*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
 *  \param file_ini : nom du fichier ou l'on va extraire une liste.
 *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
 *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
 *                             si terminee par * alors on prend toutes les variables commencant par ce qui est avant le * de varToRetrieve
 *                             si commence par * alors on prend toutes les variables terminees  par ce qui est apres le * de varToRetrieve
 *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas a  la premiere valeur entre = et ,)
 *  \param lst :        liste a initialiser et construire
 *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
 *  \param isUtf8 : pointeur sur un int (par defaut a  zero) qui si different de zero sera initialise a un si le fichier est en UTF8
 *  \param entireDataLine :  entireDataLine a zero par defaut seule la premiere valeur apres le signe egal et avant la premiere virgule sera retenue.
 *                           si a un,   toutes les donnees apres le signe = seront retenues
 *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
*/

void CGestIni::Param_GetList(const QString &file_ini, const QString &sectionToRetrieve, QStringList &lst, const QString &varToRetrieve/*=""*/,  int isToStrip /*=0*/, int *isUtf8 /*=0*/, int entireDataLine /*=0*/)
{QString outParam;
 Param_UpdateFromDisk( file_ini,  outParam,  isUtf8 );
 Param_GetList( outParam, sectionToRetrieve, varToRetrieve, lst, isToStrip, entireDataLine);
}

//----------------------------------------- Param_GetList ---------------------------------------------
/*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
 *  \param QString &outParam : contenu d'un fichier de parametrage a annalyse ou l'on va extraire une lister.
 *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
 *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
 *                             si terminee par * alors on prend toutes les variables commencant par ce qui est avant le * de varToRetrieve
 *                             si commence par * alors on prend toutes les variables terminees  par ce qui est apres le * de varToRetrieve
 *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas a la premiere valeur entre = et ,)
 *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
 *  \param isUtf8 : pointeur sur un int (par defaut a  zero) qui si different de zero sera initialise a un si le fichier est en UTF8
 *  \param entireDataLine :  entireDataLine a zero par defaut seule la premiere valeur apres le signe egal et avant la premiere virgule sera retenue.
 *                           si a un,   toutes les donnees apres le signe = seront retenues
 *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
 *  \return QStringList lst la liste des variables
*/

QStringList CGestIni::Param_GetList(const QString &outParam, const QString &sectionToRetrieve, const QString &varToRetrieve/*=""*/,  int isToStrip /*=0*/, int entireDataLine /*=0*/)
{QStringList lst;
 Param_GetList( outParam, sectionToRetrieve, varToRetrieve, lst, isToStrip, entireDataLine);
 return lst;
}

//----------------------------------------- Param_GetList ---------------------------------------------
/*! \brief Recupere les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string parametre (outParam) dans une QStringList lst.
 *  \param outParam : String parametres dans laquelle l'on va extraire une liste.
 *  \param sectionToRetrieve : section contenant les variables dont il faut faire une liste
 *  \param varToRetrieve     : variable dont il faut faire une liste (si vide alors toutes les donnees de la section apres le signe = seront retenues)
 *                             si terminee par * alors on prend toutes les variables commencant par ce qui est avant le * de varToRetrieve
 *                             si commence par * alors on prend toutes les variables terminees  par ce qui est apres le * de varToRetrieve
 *                             si vide alors toutes les lignes de la section seront retenues, alors entireDataLine est un ou deux  (ne s'arrete pas a la premiere valeur entre = et virgule)
 *  \param lst :        liste a initialiser et construire
 *  \param isToStrip :  isToStrip a zero par defaut si a un, chaque element de la liste sera nettoye des espaces de debut et fin
 *  \param entireDataLine :  entireDataLine a zero par defaut seule la premiere valeur apres le signe egal et avant la premiere virgule sera retenue.
 *                           si a un,   toutes les donnees apres le signe = seront retenues
 *                           si a deux, toutes les donnees de la ligne seront retenues y compris ce qui est avant le signe =
*/
void CGestIni::Param_GetList(const QString &outParam, const QString &sectionToRetrieve, const QString &_varToRetrieve,  QStringList &lst , int isToStrip /*=0*/, int entireDataLine /*=0*/)
{QString  varToRetrieve = _varToRetrieve;
 bool       beginBy     = varToRetrieve.endsWith('*');
 bool       endBy       = varToRetrieve.startsWith('*');
 QString        data    = "";
 QString      section   = "";
 QString     var_name   = "";
 QByteArray       ba    = outParam.toLatin1();
 char             *pt   = ba.data();
 char             *pt_s = 0;
 char             *deb  = 0;
 if (varToRetrieve.endsWith('*'))   varToRetrieve = varToRetrieve.left(varToRetrieve.length()-1); // on vire l'eventuel * de fin de la variable dont il faut faire la liste
 if (varToRetrieve.startsWith('*')) varToRetrieve = varToRetrieve.mid(1);                         // on vire l'eventuel * de debut de la variable dont il faut faire la liste
 while((pt = Param_GotoNextSection(pt, 0, &section)) && *pt)
    {if (section == sectionToRetrieve)
        {lst.clear();
         while (*pt && *pt != '[')
                {data    = "";
                 //............. si nom de variable donne en entree ......................
                 if (varToRetrieve.length())
                    {pt_s    = pt;
                     pt      = Param_ExtraireNextValeurs(pt, var_name, &data);  // dans data est retourne seulement la premiere valeur trouve avant une virgule
                     if (entireDataLine)   // la on veut toute la ligne y compris toutes les valeurs separees par des virgules
                        { bool            ok = false;
                          if (beginBy)    ok = var_name.startsWith(varToRetrieve);
                          else if (endBy) ok = var_name.endsWith(varToRetrieve);
                          else            ok = (varToRetrieve==var_name);
                          if ( ok )
                             { pt = pt_s;
                               while (*pt && (*pt==' '||*pt=='\t') )      ++pt ; // sauter espaces de debut de ligne
                               if  (*pt==';' || *pt=='\n'|| *pt =='\r')
                                   {pt = GotoNextDebLigne(pt) ;                  // sauter les commentaires et retour ligne
                                    while (*pt && (*pt==' '||*pt=='\t') ) ++pt ; // sauter espaces de debut de ligne
                                   }
                               if  (*pt=='[') return;                            // si fin de section cassos
                               deb  = pt;                                        // on devrait etre au debut du nom d'une variable
                               pt   = GotoEndOfLigne(pt);
                               data = QString::fromLatin1 (deb, pt-deb).trimmed();
                               if (data.length())
                                  {if (entireDataLine==1)        // si un on coupe apres le signe =  si deux toute la ligne est retenue
                                      {int pos = data.indexOf('=');
                                       if (pos != -1) data = data.mid(pos+1);
                                      }
                                   if (isToStrip)   lst.append(data.trimmed());
                                   else             lst.append(data);
                                  }
                             }
                        }
                     else                  // la on veut toute la ligne y compris toutes les valeurs separees par des virgules
                        {
                          if (data.length() && varToRetrieve==var_name )
                             { lst.append(data.trimmed());
                             }
                        }
                    } // endif (varToRetrieve.length())
                 //............. si nom de variable absent (toutes les lignes)......................
                 else
                    {
                     while (*pt && (*pt==' '||*pt=='\t') )      ++pt ; // sauter espaces de debut de ligne
                     if  (*pt==';' || *pt=='\n'|| *pt =='\r')
                         {pt = GotoNextDebLigne(pt) ;                  // sauter les commentaires et retour ligne
                          while (*pt && (*pt==' '||*pt=='\t') ) ++pt ; // sauter espaces de debut de ligne
                         }
                     if  (*pt=='[') return;                            // si fin de section cassos
                     deb  = pt;                                        // on devrait etre au debut du nom d'une variable
                     pt   = GotoEndOfLigne(pt);
                     data = QString::fromLatin1 (deb, pt-deb).trimmed();
                     if (data.length())
                        {if (entireDataLine!=2)        // on coupe apres le signe = si pas egal a deux
                            {int pos = data.indexOf('=');
                             if (pos != -1) data = data.mid(pos+1);
                            }
                         if (isToStrip)   lst.append(data.trimmed());
                         else             lst.append(data);
                        }
                    }
                } // end while (*pt && *pt != '[')
         break;
        }
     }
}

//----------------------------------------- replaceList ---------------------------------------------
/*! \brief replace a variables list in section parameters by datas from QStringList
 *  \param outParam : parameters data document destination.
 *  \param QStringList &list list data sources to place.
 *  \param const QString &sectionToSet section where to find list of variables to replace (created if needed)
 *  \param const QString &varToSet  variable name of each list elements to replace
 */
void CGestIni::replaceList(QString &outParam, const QString &sectionToSet, const QString &varToSet,  const QStringList &list )
{QString line        = "";
 QString sectionName = "";
 QString varname     = "";
 int     pos         = 0;
 int     end         = 0;
 int     posNext     = 0;
 int     posDebLine  = 0;
 int     posNextSave = 0;

 //.................. aller dans la section ...........................................
 while  ( (posNext = readNextLine(outParam, posNext, line))  != outParam.length() )       // on lit une ligne de texte (se positionne au debut de la ligne suivante)
    {if ( (pos     = gotoNextNotBlank(line, 0)) == line.length() )       continue;        // on y cherche le premier caractere utile
     if (line[pos]==';')                                                 continue;        // si commentaire on saute la ligne
     if (line[pos]=='/' && line[pos+1]=='/')                             continue;        // si commentaire on saute la ligne
     if (line[pos]=='[')                                                                  // si debut de section on voit si c'est la bonne section
        {++pos;
         if ( (end       = line.indexOf(']',pos)) ==-1)                  continue;        // si fin de section pas trouvee on continue
         sectionName     = line.mid(pos,end-pos).trimmed();
         if (sectionName == sectionToSet)                                break;           // si section trouvee on arrete
         sectionName     = "";
        }
    }
 if (sectionName.length()==0)                               //             return;          // si section non trouvee on se casse TODO la creer en debut de fichier c'est fait
    { sectionName = "["+sectionToSet+"]\r\n";               // DANGER on se sert provisoirement de sectionName (n'est plus utilise apres )
      outParam.prepend(sectionName);
      posNext     = sectionName.length();                   // se positionner au debut de la ligne suivante
      sectionName = sectionToSet;                           // repositionner sur bon nom (plus de DANGER) on ne sait jamais ce qui va etre fait apres dans ce code ....
    }
 //.................. parcourir les variables de cette section et effacer celles concernees par la liste ...........................................
 int endSectionFound = -1;    // pointera sur la fin de la section ou inserer la liste
 posDebLine          = posNext;
 while  ( posNext  < outParam.length() )        // on lit une ligne de texte  (se positionne au debut de la ligne suivante)
    { posNext       = readNextLine(outParam, posNext, line);
      if ( (pos     = gotoNextNotBlank(line, 0)) == line.length() )       continue;       // on y cherche le premier caractere utile
      if (line[pos]=='[')     {endSectionFound = pos+posDebLine;          break;}         // on arrete si debut d'une autre section
      if (line[pos]==';')                                                 continue;       // si commentaire on saute la ligne
      if (line[pos]=='/' && line[pos+1]=='/')                             continue;       // si commentaire on saute la ligne
      if ( (end = line.indexOf('=',pos)) ==-1)                            continue;       // si pas de signe egal trouve on continue
      varname = line.mid(pos,end-pos).trimmed();
      if (varname==varToSet)
         {outParam.remove(posDebLine,posNext-posDebLine); // on efface la ligne
          posNext       = posDebLine;                     // on se replace au debut de la ligne qui vient d'etre effacee
          posNextSave   = posDebLine;                     // on garde la trace du dernier point ou s'est produit l'effacement
         }
      posDebLine = posNext;
    }
 //................. si rien a effacer on place posNextSave sur valeur correcte ...........
 if (endSectionFound != -1 )
    { posNextSave = endSectionFound;
      posNextSave = GotoBackWhileBlanck(outParam, endSectionFound);  // on recule jusqu'a la fin de la derniere ligne
      outParam.remove(posNextSave,endSectionFound-posNextSave);      // effacer tous les caracteres non significatifs
      outParam.insert(posNextSave, "\r\n");                          // inserer un saut de ligne
      posNextSave += 2;
    }
 if (posNext == outParam.length())   posNextSave = posNext;
 if (posNextSave==0)                 posNextSave = posNext;
 //............... faire le menage en aval .............................
 posNext = gotoNextNotBlank(outParam, posNextSave);
 outParam.remove(posNextSave,posNext-posNextSave);
 outParam.insert(posNextSave, "\r\n");                          // inserer un saut de ligne
 //................. on replace la liste des variables ....................................
 varname  = varToSet;
 varname  = varname.prepend("  ");
 varname += " = ";
 for (int i=0; i<list.size(); ++i)
    {outParam.insert(posNextSave, varname+list[i]+"\r\n");
    }
}

//----------------------------------------- readNextLine ---------------------------------------------
int CGestIni::readNextLine(const QString &outParam, int deb, QString &line)
{if (deb >= outParam.length())
    {line.clear();
     return outParam.length();
    }
 int end = gotoEndLine(outParam, deb);
 line    = outParam.mid(deb, end-deb);
 return gotoNextStartLine(outParam, end);    // on se positionne au debut de la ligne suivante
}
//----------------------------------------- gotoEndLine ---------------------------------------------
int CGestIni::gotoEndLine(const QString &outParam, int pos)
{int endLine = outParam.indexOf('\n',pos);
 if (endLine != -1) return endLine;
 endLine = outParam.indexOf('\r',pos);
 if (endLine != -1) return endLine;
 return outParam.length();
}
//----------------------------------------- gotoNextStartLine ---------------------------------------------
int CGestIni::gotoNextStartLine(const QString &outParam, int pos)
{pos = gotoEndLine(outParam, pos);
 while (pos<outParam.length() && (outParam[pos]=='\r'||outParam[pos]=='\n')) ++pos;
 return pos;
}
//----------------------------------------- gotoNextNotBlank ---------------------------------------------
/*! \brief goto next not blanck caracter
 *  \param outParam : document where is action .
 *  \param pos : positiion in document where start action .
 *  \return position to next not blanck caracter
*/
int CGestIni::gotoNextNotBlank(const QString &outParam, int pos)
{ while (pos<outParam.length() && (outParam[pos]=='\r'||outParam[pos]=='\n'||outParam[pos]==' '||outParam[pos]=='\t')) ++pos;
  return pos;
}
//----------------------------------------- gotoNextBlank ---------------------------------------------
int CGestIni::gotoNextBlank(const QString &outParam, int pos)
{pos =  CGestIni::gotoNextNotBlank(outParam,  pos);
 while (pos<outParam.length() && outParam[pos]!='\r' && outParam[pos]!='\n' && outParam[pos]!='\t' && outParam[pos]!=' ') ++pos;
 return pos;
}
//----------------------------------------- getNextWord ---------------------------------------------
int CGestIni::getNextWord(const QString &outParam, int pos, QString &word)
{int deb   = -1;
 word      = "";
 if ( (deb =  CGestIni::gotoNextNotBlank(outParam,  pos)) >= outParam.length()) return outParam.length();
 pos       = deb;
 while (pos<outParam.length() && outParam[pos]!='\r' && outParam[pos]!='\n' && outParam[pos]!='\t' && outParam[pos]!=' ') ++pos;
 word      = outParam.mid(deb, pos-deb);
 return pos;
}

//------------------------------ Param_GotoNextSection -----------------------------------------
/*! \brief Passe a la section suivante en debutant au pointeur pt.
*/
char *CGestIni::Param_GotoNextSection(char *pt, const char* section, QString *pQsection)
{char *deb         = 0;
 char *end         = 0;
 long len_section  = 0;
 while (*pt)
 {switch (pt[0])
    {case '\r':
     case '\n':
     case ';':
          pt = GotoNextDebLigne(pt);
          break;
     /*
     case '\"':
          while (*pt && *pt != '\"')
                { if (*pt=='\\' && pt[1] !=0 ) ++pt;
                  ++pt;
                }  // aller apres de guillemet fermant
          break;
     */
     case '\\':
          if ( pt[1] !=0 ) pt +=2;
          break;
     case '[': //....................debut de section: tester si c'est celle recherchee ...................
          ++pt;
          while (*pt && (*pt==' '||*pt=='\t'))  ++pt;                // enlever espaces de debut
          deb = pt;
          while (*pt && *pt != ']') ++pt;                            // chercher delimiteur de fin de nom de section
          if ( *pt != ']')    return  pt;                                     // y en a pas --> erreur (*pt == 0)
          end = pt;
          while (end>deb && (end[-1]==' '||end[-1]=='\t')) end--;    // enlever espaces de fin
          len_section = end - deb;
          if (len_section>0 && pQsection != 0)
             {*pQsection=QString::fromLatin1(deb, len_section);
             }
          if ( section == 0)  return  GotoNextDebLigne(pt);   // pas de section donnee, on s'arrete a  la premiere trouvee
          if ((long)strlen(section)==len_section && strncmp(section, deb, len_section)==0)             // SI c'est la section recherchee
             {                return  GotoNextDebLigne(pt);                            // aller ligne suivante
             } // end if (strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchee
          break;
      default:
          ++pt;
     } // end switch (pt[0])
  } // end while (*pt)
 return pt;
}
//------------------------------ formatStringForParam ----------------------------------------
QString  CGestIni::formatStringForParam(QString &str)
{   str = str.prepend("$DEB$");
    str = str.replace("[","$|$");
    str = str.replace("]","$*$");
    str = str.replace(",","$.$");
    str = str.replace("\r","$r$");
    str = str.replace("\n","$n$");
    str = str.replace(";","$^$");
    str = str + "$END$";
    return str;
}

//------------------------------ retrieveStringForParam ----------------------------------------
QString  CGestIni::retrieveStringForParam(QString &str)
{           str = str.remove("$DEB$");
            str = str.remove("$END$");
            str = str.replace("$|$","[");
            str = str.replace("$*$","]");
            str = str.replace("$.$",",");
            str = str.replace("$r$","\r");
            str = str.replace("$n$","\n");
            str = str.replace("$^$",";");
            return str;
}

//------------------------------ Param_ExtraireNextValeurs ----------------------------------------
/*! \brief Extrait les valeurs d'une variable donnee.
*/
char *CGestIni::Param_ExtraireNextValeurs(char* pt, QString &var_name,
                                             QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                             QString *val6, QString *val7, QString *val8, QString *val9, QString *val10)
{return Param_ExtraireNextValeurs(pt, var_name, 0, 0, val1,  val2,  val3,  val4,  val5,  val6,  val7,  val8,  val9, val10);
}

//------------------------------ Param_ExtraireNextValeurs ----------------------------------------
/*! \brief Extrait les valeurs d'une variable donnee dans un string parametre donne.
*/
char *CGestIni::Param_ExtraireNextValeurs(char* pt, QString &var_name, char**pt_lgn, long *len,
                                             QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                             QString *val6, QString *val7, QString *val8, QString *val9, QString *val10)
{char *deb         = 0;
 char *end         = 0;
 long len_variable = 0;

 while (*pt && (*pt==' '||*pt=='\t') )      ++pt ; // sauter espaces de debut de ligne
 if  (*pt==';' || *pt=='\n'|| *pt =='\r')
     {pt = GotoNextDebLigne(pt) ;                  // sauter les commentaires et retour ligne
      while (*pt && (*pt==' '||*pt=='\t') ) ++pt ; // sauter espaces de debut de ligne
     }
 if  (*pt=='[') return pt ;
 deb = pt;                                                                  // on devrait etre au debut du nom d'une variable
 while (*pt && *pt!='=')
       {if (*pt=='\\' && pt[1] !=0 ) pt += 2;
        else if (*pt == '[')  break;       // le signe '[' marque la fin de zone des variables de cette section
        else ++pt;
       }
 if (*pt==0)    return pt ;
 if (*pt=='[')  return pt ;
 end = pt;
 while (end>deb && (end[-1]==' '||end[-1]=='\t')) end-- ;   // enlever espaces de fin
 len_variable = end - deb;
 if (len_variable>0 )
    {var_name = QString::fromLatin1 (deb, len_variable).trimmed();
    }
 ++pt;   // passer le signe egal
 Param_ExtraireValeurs(pt, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10);
 if (pt_lgn)
    {*pt_lgn = deb;
     pt      = GotoEndOfLigne(pt);
     if (*len) *len = (pt-deb);
    }
 return GotoNextDebLigne(pt);                                               // aller ligne suivante
}

//------------------------------ GotoNextDebLigne -----------------------------------------
/*! \brief deplace le pointeur jusqu'au debut de la ligne suivante.
*/
char *CGestIni::GotoNextDebLigne(const char* txt)
{char *pt  = (char*) txt;
 while (*pt)
    {if      ( pt [0]=='\\'&& pt [1] !=0)            (pt+=2);
     else if ( pt [0]=='\r'&& pt [1] =='\n')  return (pt+=2);
     else if ( pt [0]=='\n'&& pt [1] =='\r')  return (pt+=2);
     else if ( pt [0]=='\n'|| pt [0] =='\r')  return (pt+1);
     else ++pt;
    }
 return pt;
}
//------------------------------ GotoBackWhileBlanck -----------------------------------------
/*! \brief recule jusqu'a la fin de la ligne precedente
 *  \param outParam : document where is action .
 *  \param pos : positiion in document where start action .
 *  \return position at end of previous line
*/
int CGestIni::GotoBackWhileBlanck(const QString &outParam, int pos)
{ while (pos > 0)
    { QChar c = outParam [pos-1];
      if ( c=='\r' || c=='\n' || c=='\t' ||c==' ') {--pos; continue;}
      return pos;
    }
 return pos;
}

//------------------------------ GotoEndOfLigne -----------------------------------------
/*! \brief deplace le pointeur jusqu'a  la fin de la ligne.
*/
char *CGestIni::GotoEndOfLigne(const char* txt)
{char *pt  = (char*) txt;
 while (*pt)
    {if      ( pt [0]=='\\'&& pt [1] !=0)            (pt+=2);
     else if ( pt [0]=='\r'&& pt [1] =='\n')  return pt;
     else if ( pt [0]=='\n'&& pt [1] =='\r')  return pt;
     else if ( pt [0]=='\n'|| pt [0] =='\r')  return pt;
     else ++pt;
    }
 return pt;
}
//------------------------------ CutStrLeft -----------------------------------------
/*! \brief coupe une chaine avant le premier motif at trouve.
*/
QString CGestIni::CutStrLeft(const QString &str, const QString &at, int mode_IfNotFound /* = CGestIni::IfNotFound_inchanged */)
{int pos = str.indexOf(at);
 if ( pos != -1 )                                            return str.left(pos);
 if ( mode_IfNotFound==CGestIni::IfNotFound_inchanged )      return str;
 else                                                        return "";
}
//------------------------------ CutStrRight -----------------------------------------
/*! \brief coupe une chaine apres le premier motif at trouve.
*/
QString CGestIni::CutStrRight(const QString &str, const QString &at, int mode_IfNotFound /* = CGestIni::IfNotFound_inchanged */)
{int pos   = str.indexOf(at);
 if ( pos  != -1 )                                           return str.mid(pos+at.length());
 if ( mode_IfNotFound==CGestIni::IfNotFound_inchanged )      return str;
 else                                                        return "";
}
//------------------------------ Construct_Name_Exe -----------------------------------------
/*! \brief construit le chemin vers l'executable du module (module) de MendinTux. Gere la compatibilite Windows/Linux/Mac.
*/
QString CGestIni::Construct_Name_Exe(QString module, QString start_Argv, const QString &alternateExecName /*="" */)
{
/*      Signature :  String         X String                              --> String
                     nom du module  X Chemin complet de demarrage du proc --> nom  ini complet du module
    Permet la construction d'un nom complet de fichier executable en =
relation avec le module correspondant
*/
        QString path;
        QString exeName;
        if (alternateExecName.length()) exeName = alternateExecName;
        else                            exeName = module;
        //Creation du nom de fichier
       if (module=="compta-plugins" || module=="check_dus"  || module=="comptabilite" )
          { path   = start_Argv + "/" + PATH_SPEC_MAC + "../../comptabilite/bin/" + exeName + F_EXT;
            module = exeName;
            path  +=  ARCHI_DEP_MAC;
          }
       else
          { path   = start_Argv + "/" + PATH_SPEC_MAC + "../../"+module+"/bin/"  + exeName + F_EXT;
            module = exeName;
            path  +=  ARCHI_DEP_MAC;
          }
        path = QDir::cleanPath (path);
        return path;

}


//------------------------------ Construct_Name_File_Ini -----------------------------------------
/*! \brief construit le chemin vers le fichier ini du module (module) de MendinTux. Gere la compatibilite Windows/Linux/Mac.
*/
QString CGestIni::Construct_Name_File_Ini(QString module, QString start_Argv, QString nom_Fichier_Alternatif ){
/*      Signature :  String         X String X String                                                   --> String
        nom du module  X Chemin complet proc demarrage X Nom du fichier ini alternatif      --> nom  ini complet du module
        avec en plus son chemin relatif
        au module en question
        ex : ccamview/bin/ccamview
        ici il renverra un nom de fichier
        du genre [chemin du module]/ccamview/bin/ccamview.ini
        Permet la construction d'un nom complet de fichier ini en relation =
        avec le module correspondant ou d'un fichier   alternatif
*/
        QString path;
        if (nom_Fichier_Alternatif.length()){
                path = start_Argv + "/" + PATH_SPEC_MAC + "../../" + module + "/bin/" + nom_Fichier_Alternatif;
        }else{
                path = start_Argv + "/" + PATH_SPEC_MAC + "../../" + module + "/bin/" + module + ".ini";
        }

        path = QDir::cleanPath (path);
        return path;
}

//------------------------------ Construct_PathBin_Module -----------------------------------------
/*! \brief Permet la construction d'un nom complet de fichier executable en  relation avec le module correspondant
 *  \param QString  module : nom du programme dont on cherche a  construire le chemin complet si egal a  "" alors le nom du chemin sera extrait de : start_Argv
 *  \param const QString & start_Argv : Chemin complet de demarrage du programme
 *  \param QString *base_name : adresse d'une QString qui dans laquelle si elle est differente de zero sera retourne le nom du programme SANS SON EXTENSION
 *  \return nom du module  avec le chemin complet de demarrage du proc --> nom  ini complet du module
*/
QString CGestIni::Construct_PathBin_Module(const QString  &module, const QString & ref_dir)
{ return  QDir::cleanPath (ref_dir + "/" + PATH_SPEC_MAC + "../../" + module + "/bin/")+ "/" ;
}

//------------------------------ AbsoluteToRelativePath -----------------------------------------
/*! \brief convertit un chemin absolu en chemin relatif par rapport a un chemin de reference, il est imperatif que les chemins d'entree soient tous en absolu.
 *  \param QString pathRef : chemin de reference par rapport auquel il faut relativiser
 *  \param QString pathToConvert : chemin a convertir
 *  \return chemin convertit en relatif
*/

QString CGestIni::AbsoluteToRelativePath(QString pathRef, QString pathToConvert)
{int      posR = -1;
 int      posC = -1;
 if (pathToConvert.length()>2 && pathToConvert.left(2)=="\\\\")  return pathToConvert;  // si reseau on ne peut pas relativiser
 pathRef       = pathRef.replace("\\","/");
 pathToConvert = pathToConvert.replace("\\","/");
 pathRef       = pathRef.replace("//","/");
 pathToConvert = pathToConvert.replace("//","/");
 //............ enlever l'eventuel C: ...........................
 //             de facon a ce que les racines soient identiques : /xxxx/nnnnn/etc/
 posR           = pathRef.indexOf(':');
 posC           = pathToConvert.indexOf(':');
 if (posR !=-1 && posC !=-1 && pathRef[0] != pathToConvert[0]) return pathToConvert; // si les partitions sont differentes l'on ne peut pas relativiser
 if (posR !=-1) pathRef       = pathRef.mid(posR+1);
 if (posC !=-1) pathToConvert = pathToConvert.mid(posC+1);
 pathRef       = pathRef.trimmed();
 pathToConvert = pathToConvert.trimmed();
 //........................... rechercher jusqu'ou le debut des chemins est le meme ..............................
 int posRef      = pathRef.indexOf('/');
 int posDst      = pathToConvert.indexOf('/');
 int last_posRef = posRef;
 int last_posDst = posDst;
 int posToCut    = 0;
 QString cpRef   ="";
 QString cpDst   ="";
 while (posRef  != -1 && posDst != -1)    //   /xxxxx/ccccc/nnnn/     /xxxxx/ccccc/nnnn/uuuuuuu/mmmmmm/ppppppppp
    {cpRef = pathRef.mid(last_posRef, posRef-last_posRef);
     cpDst = pathToConvert.mid(last_posDst, posDst-last_posDst);
     if (cpRef == cpDst )
        {posToCut   = posRef;
        }
     else break;
     last_posRef = posRef;
     last_posDst = posDst;
     posRef      = pathRef.indexOf('/',last_posRef+1);
     posDst      = pathToConvert.indexOf('/',last_posDst+1);
    }
  //........................... couper les chemins ..............................
 pathToConvert = pathToConvert.mid(posToCut);
 pathRef       = pathRef.mid(posToCut);
 //............................ compter le nombre de ../ a mettre ...................
 int            n = pathRef.count( '/');
 QString relative = "";
 for (int i=1; i<n; ++i) relative += "../";
 relative   += pathToConvert;
 relative    = relative.replace("//","/");
 return relative;
}
//-------------------------- listDirectory -------------------------------------------
/*! \brief make files an directories list which are in a start directorie
 *  \param start_dir : start directorie from wich make files an directories list
 *  \param filterExt : list of files suffix extensions that we wants exple ".txt; *df;.htm*;xml;.png" (if present, the first dot is ignored as in .htm* .txt and .png)
 *  \param filterName :   list of files names that we wants exple "info_*; *_log"
 *  \param sep : caracter separator in  filterExt_in filterName filterBlack default is ';'
 *  \param listWithoutEx :  if true the returned list is without extension.
 *  \param filterBlack :    list of files names (included extension) that we not wants exple "@*;.DS_Store*;*~"
 *  \return files list
*/
QStringList CGestIni::listDirectory(QString start_dir, const QString &filterExt_in/*=""*/, const QString &filterName/*=""*/, const QString &sep/*=";"*/, bool listWithoutExt/*=false*/, const QString &filterBlack/*=""*/)
{   QStringList dirList;
    return CGestIni::listDirectory(dirList, start_dir, filterExt_in, filterName, sep, listWithoutExt,filterBlack);
}

//-------------------------- listDirectory -------------------------------------------
/*! \brief make files an directories list which are in a start directorie
 *  \param dirList : directories list wich will be filled by directories
 *  \param start_dir : start directorie from wich make files an directories list
 *  \param filterExt : list of files suffix extensions that we wants exple ".txt; *df;.htm*;xml;.png" (if present, the first dot is ignored as in .htm* .txt and .png)
 *  \param filterName :   list of files names that we wants exple "info_*; *_log"
 *  \param sep : caracter separator in  filterExt_in filterName filterBlack default is ';'
 *  \param listWithoutEx :  if true the returned list is without extension.
 *  \param filterBlack :    list of files names (included extension) that we not wants exple "@*;.DS_Store*;*~"
 *  \return files list
*/

QStringList CGestIni::listDirectory(QStringList &dirList, QString start_dir, const QString &filterExt/*=""*/, const QString &filterName/*=""*/, const QString &sep/*=";"*/, bool listWithoutExt/*=false*/, const QString &filterBlack/*=""*/)
{   QStringList    ret;

    QStringList blackList;
    QStringList filterList;
    QStringList extList;
    dirList.clear();
    if (filterBlack.length())  blackList  = filterBlack.split (sep);
    if (filterName.length())   filterList = filterName.split  (sep);
    if (filterExt.length())    extList    = filterExt.split   (sep);

    //................ allons y  .....................................................
    QDir l_dir;
    l_dir.setPath ( start_dir );
    // l_dir.makeAbsolute ();
    // l_dir.setFilter( QDir::AllDirs | QDir::NoSymLinks | QDir::Files | QDir::CaseSensitive| QDir::NoDotAndDotDot);
    l_dir.setFilter( QDir::AllDirs | QDir::Files);     // INDISPENSABLE SOUS LINUX !!!!!
    l_dir.setNameFilters ( filterList );

    //............ exploration de la liste ......................
    QFileInfoList qFileInfoList   = l_dir.entryInfoList();
    QList<QFileInfo>::iterator it = qFileInfoList.begin();
    QFileInfo            fi;
     while ( it != qFileInfoList.end() )
          {fi = *it++;
           QString fname = fi.fileName();
           QString ext   = QFileInfo(fname).suffix().toLower();  // suffix() retourne un l'extension sans le point
           if      ( fname ==".." || fname == "." )          continue;
           if      ( isInBlackList(blackList, fname) )       continue;
           if      ( fi.isDir() )
                   { // if ( extList.count()     && ! isInBlackList( extList,    ext,  1) ) continue;  // le filtre d'extensions ne s'applique pas aux repertoires
                     // if ( filterList.count()  && ! isInBlackList( filterList, fname,1) ) continue;  // l_dir.setNameFilters ( filterList );
                     dirList << fname;
                   }
           else if (fi.isFile())
                   {  if ( extList.size()     && ! isInBlackList( extList,    ext,  1) ) continue;
                      if (listWithoutExt)    ret << fi.baseName();
                      else                   ret << fname;
                   }
          }
  return ret;
}

//----------------------------------------- Copy_Dir ---------------------------------------------
void CGestIni::Copy_Dir(QString src_Dir,  QString dest_Dir, QString *currentFileName /* = 0*/)
{
    //........................ explorer la source .......................................
    QDir cur_dir( src_Dir );
    cur_dir.setFilter( QDir::AllDirs | QDir::NoSymLinks | QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);

    if ( ! QFile::exists( dest_Dir  ) )
       { cur_dir.mkdir(dest_Dir);
       }
    //............ exploration de la liste ......................
    QFileInfoList qFileInfoList = cur_dir.entryInfoList();
    QList<QFileInfo>::iterator it = qFileInfoList.begin();
    QFileInfo            fi;
    while ( it != qFileInfoList.end() )
    {   fi            = *it;
        QString fname = fi.fileName().toLatin1();
        if (currentFileName)
           { *currentFileName = fname;
              qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
           }
        if      ( fi.isDir() )
                {    Copy_Dir( src_Dir + "/" + fname, dest_Dir + "/" + fname, currentFileName);
                }
        else if (fi.isFile())
                {    CGestIni::Copy_File(src_Dir + "/" + fname, dest_Dir, currentFileName);
                }
        ++it;
    }
}
//---------------------------------- Copy_File --------------------------------------------------------------
// ENTREE: const QString &fileSrc: fichier source chemin+file
//         const QString &pathDst: chemin de destination sans separateur de fin.
//         QString *file_name : nom du fichier de destination (si zero il sera extrait de celui source)
QString CGestIni::Copy_File(const QString &fileSrc, const QString &pathDst, QString *file_name /*=0*/, QString *currentFileName /* = 0*/)
{
  //................. choisir le fichier image src a inserer .................................................

  QString fileName = "";
  QFile* pfileSrc  = new QFile( fileSrc );
  if (pfileSrc==0)                                                              return fileName;
  if (file_name) fileName  = *file_name;
  else           fileName  = QFileInfo( *pfileSrc ).fileName();
  QFile* pfileDst = new QFile( pathDst + "/" +  fileName);
  if (pfileDst==0)                       {delete pfileSrc;                      return fileName;}
  if (currentFileName)
     { *currentFileName = fileName;
        qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();
     }
  //.................... si le dossier src n'est pas egal au dossier dst .......................
  //                     alors copier l'image src dans le dossier destination
  //                     sinon l'image s'y trouve forcement
  if ( ! (pathDst == QFileInfo( *pfileSrc ).absolutePath() && QFileInfo( *pfileDst ).fileName() == QFileInfo( *pfileSrc ).fileName()) )
     {char *buffer = new char[32000];
      long len_read;
      if (buffer==0)                         {delete pfileSrc; delete pfileDst; return fileName;}
      QString srcF = fileSrc;                     srcF.replace('\\','/'); srcF.replace("//","/");
      QString dstF = pathDst + "/" + fileName;    dstF.replace('\\','/'); dstF.replace("//","/");

      if (srcF == dstF )                     {delete pfileSrc; delete pfileDst; return fileName;}
      QFile::remove ( dstF );                               // eventuellement effacer un  du meme nom

      if ( !pfileSrc->open( QIODevice::ReadOnly ) )  {delete pfileSrc; delete pfileDst; return fileName;}
      if ( !pfileDst->open( QIODevice::WriteOnly ) ) {delete pfileDst; pfileSrc->close(); delete pfileSrc; return fileName;}

      do{
          len_read = pfileSrc->read(buffer, 32000 );
          if (len_read!=-1) pfileDst->write(buffer , len_read);
        } while (len_read == 32000);

      pfileSrc->close ();
      pfileDst->close ();
      delete[] buffer;
     }
 return fileName;
}
//-----------------------------------------------------  PassWordEncode --------------------------------------------------
// Algo minimal pour l'instant
// chaque octet tourne de sa position dans la chaine + 1, et Xor final avec le mot de passe en clair puis converti en hexa
// resultat final en hexa;
// les linges qui sechent mouillent les cordes
// brouiller les cartes

QString CGestIni::PassWordEncode(QString &pass)
{char encoded_car;
 char pt_magic_key[]    = "les linges qui sechent mouillent les cordes";
 QString encoded_str    = "";
 QByteArray       ba    = pass.toLatin1();
 char             *pt   = ba.data();
 int         len_pass   = ba.length();

 for (int i=0; i<len_pass;++i)
     {encoded_car  =  pt[i]^pt_magic_key[i];
      encoded_str += UINTtoHex(encoded_car);
     }
 return encoded_str;
}

//-----------------------------------------------------  PassWordDecode --------------------------------------------------
// Algo minimal pour l'instant
//
QString CGestIni::PassWordDecode(QString str_to_decode)
{if (str_to_decode.startsWith("#")) str_to_decode = str_to_decode.mid(1);
 char decoded_car;
 char pt_magic_key[] = "les linges qui sechent mouillent les cordes";
 QString decoded_str = "";
 int len_pass        = str_to_decode.length();
 int      pos        = 0;
 while ( pos < len_pass)
     {decoded_car  =  HexToUINT( str_to_decode.mid(pos,4).toLatin1());
      decoded_car  =  decoded_car ^ pt_magic_key[pos/4];
      decoded_str +=  decoded_car;
      pos         += 4;
     }
 return decoded_str;
}

//-----------------------------------------------------  UINTtoHex --------------------------------------------------------
QString CGestIni::UINTtoHex( quint16 val)
{char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};   //  on peut mettre une table Bidon
 QString res="";
 res += hex[(val>>12)& 0x000F];
 res += hex[(val>>8) & 0x000F];
 res += hex[(val>>4) & 0x000F];
 res += hex[val&0x000F];
 return res;
}

//-----------------------------------------------------  HexToUINT --------------------------------------------------------
quint16 CGestIni::HexToUINT( const char *str)
{int i   = strlen(str);
 if (i==0) return 0;
 quint16 val = 0;
 int     dec = 0;
 while (i>0)
   {--i;
    unsigned char car = str[i] | 32;     // mettre en minuscules
    if      (car>='a'&& car<= 'f')  val  += (car-'W')<<dec;  //a (97) - W (87) --> 10
    else                            val  += (car-'0')<<dec;  //0 (48) - 0 (48) --> 0
    dec += 4;
   }
 return val;
}

//--------------------------------------------- isInBlackList --------------------------------------------
/*! \brief verifie si le terme name est concerne par la blackList
*   \param  black_str :      const QString & black_str
*   \param  name :   const QString & name terme dont il faut verifie si concerne
*   \param  firstDotNotEvaluated :   bool if true/if names to compare starts with dot it's will be not evaluated
*   \return int :   vrai/1 si concerne    faux/0 si non concerne
*/
int CGestIni::isInBlackList( QString black_str, const QString &name, bool firstDotNotEvaluated  )
{  black_str              = black_str.replace('\r',"").trimmed();
   QStringList black_list = black_str.split('\n');
   return isInBlackList(black_list, name, firstDotNotEvaluated);
}

//--------------------------------------------- isInBlackList --------------------------------------------
/*! \brief verifie si le terme name est concerne par la blackList
*   \param black_list :      const QStringList & black_list
*   \param name :   const QString & name terme dont il faut verifie si concerne
*   \param firstDotNotEvaluated :   bool if true/if names to compare starts with dot it's will be not evaluated
*   \return int :   vrai/1 si concerne    faux/0 si non concerne
*/
int CGestIni::isInBlackList(const QStringList &black_list, const QString &_name, bool firstDotNotEvaluated)
{   QString name = _name;
    if (firstDotNotEvaluated && name.startsWith('.')) name=name.mid(1);

    for (int i=0; i<black_list.size(); ++i)
      { QString black_str = black_list[i].trimmed();

        if (black_str.startsWith("*") && black_str.endsWith("*"))
           { if (firstDotNotEvaluated && black_str.startsWith('.')) black_str=black_str.mid(1);
             black_str = black_str.remove('*');
             if (name.contains(black_str))         return 1;
           }
        else if (black_str.startsWith("*"))
           { if (firstDotNotEvaluated && black_str.startsWith('.')) black_str=black_str.mid(1);
             black_str = black_str.remove('*');
             if (name.endsWith(black_str))         return 1;
           }
        else if (black_str.endsWith("*"))
           { if (firstDotNotEvaluated && black_str.startsWith('.')) black_str=black_str.mid(1);
             black_str = black_str.remove('*');
             if (name.startsWith(black_str))       return 1;
           }
        else
           { if (firstDotNotEvaluated && black_str.startsWith('.')) black_str=black_str.mid(1);
             if (name==black_str)                  return 1;
           }
      }
  return 0;
}

