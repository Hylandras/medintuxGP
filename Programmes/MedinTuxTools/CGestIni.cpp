/**************************** CGestIni.cpp *********************************
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

//=============================================== INCLUDES ===================================================================
#include <qfile.h>
#include <qtextstream.h>
#include  "CGestIni.h"
#include <qdir.h>
#include <qfileinfo.h>
#include <qobject.h>
#include <qcstring.h>

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

#ifdef Q_WS_X11

#endif


/*! \class CGestIni
 *  \brief Gestion des fichiers INI de la suite Medintux.
 * Gére le chargement des paramètres, leur recherche et l'écriture des fichiers paramètres.
 * Les paramètres sont sauvegardés de la sorte :
 * [SECTION]\\r\\n
 * Parametre = val1, val2, val3, val4, val5, val6, val7, val7, val8, val9, val10\\r\\n
 * Plusieurs sections peuvent cohabiter dans un même fichier et plusieurs paramètres peuvent être défini par section.
 * Cette classe gère la compatibilté Windows, Mac, Linux pour les fichiers INI
 * \todo Un seul fichier INI pour toute la suite dans le QDir::home() qui ne contient que les données de connection à la base de données. Les autres paramètres sont dans la base de données.
*/

//-----------------------------------------------------  CGestIni --------------------------------
/*! \brief constructeur de la classe. Initialise les variables m_ParamData et m_ParamPath.
*/
CGestIni::CGestIni()
{

}
//---------------------------- addXmlData ------------------------------------------------
/*! \brief ajoute dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
 *   dans cet exemple la valeur serait ' et de fin' (sans les apostrophes)
 *  \param _tagXml : String indiquant le nom du tag  sans <  />
 *  \param valeur :  valeur a encadrer par les tags xml
 *  \param modeleXML :  QString XML dans laquelle on va ajouter la valeur
 *  \param ofset :  QString ofset de decalage vers la droite par defaut vide
 *  \param mustBeB64Protected :  si true la valeur sera convertie et inscrite en base 64.
*/

void CGestIni::addXmlData(const QString& _tagXml, QString valeur, QString &modeleXML, bool /* mustBeB64Protected  =FALSE */, const QString &ofset /* ="" */)
{QString tagXml      = _tagXml;
 QString dataToPlace = "";
 valeur.replace('&',"&amp;");
 valeur.replace('>',"&gt;");
 valeur.replace('<',"&lt;");
 dataToPlace = valeur;
 modeleXML += QString("%4   <%1>%2</%3>\r\n").arg(tagXml, dataToPlace, tagXml,ofset);
}

//---------------------------- getXmlData ------------------------------------------------
/*! \brief retourne dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
 *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
 *  \param tagXml : String indiquant le nom du tag  sans <  />
 *  \param dataXml :  String XML dans laquelle on va lire la valeur
 *  \param nextPos :  pointeur sur un int qui si il n'est pas egal a zero indique
 *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherche.
 *  \return QString qui est la valeur a rechercher.
*/

QString CGestIni::getXmlData(const QString& tagXml, const QString& dataXml, int *nextPos, int noConvertCharToHtml /* =0 */)
{int posEnd  = -1;
 int posDeb  =  0;

 QString tag =  tagXml+'>';
 if (nextPos) {posDeb  = dataXml.find(tag, *nextPos);*nextPos=0;}  // on  remet a zero au cas ou tag pas trouve (recommencer a zero)
 else         {posDeb  = dataXml.find(tag);}
 if (posDeb==-1)         return QString::null;
 posDeb += tag.length(); tag = tag.prepend("</");
 posEnd  = dataXml.find(tag, posDeb);
 if (posEnd==-1)         return QString::null;
 if (nextPos) *nextPos = posEnd + tag.length();
 QString retour = dataXml.mid(posDeb,posEnd-posDeb);
 if ( noConvertCharToHtml==0 )
    { retour.replace("&gt;",">");
      retour.replace("&lt;","<");
      retour.replace("&amp;","&");
    }
 return retour;
}
//---------------------------- setXmlData static ------------------------------------------------
/*! \brief place dans un fichier XML une valeur situee entre un tag de debut <tag> et de fin </tag>
 *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
 *  \param tagXml : String indiquant le nom du tag  sans <  />
 *  \param valeur :    QString valeur a ecrire
 *  \param dataXml :  String XML dans laquelle on va ecrire la valeur
 *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherche.
 *  \param noConvertCharToHtml  if zero (default value) '>' '<' '&' are  converted in '&gt;' '&lt;' '&amp;'
 *  \return true si tout est ok false sinon (en general le tag n'a pas ete trouve)
*/
bool CGestIni::setXmlData(const QString& tagXml,  QString valeur, QString& dataXml, int noConvertCharToHtml /* =0 */)
{int posEnd  = -1;
 int posDeb  =  0;
 if (noConvertCharToHtml == 0)
    { valeur.replace('&',"&amp;");
      valeur.replace('>',"&gt;");
      valeur.replace('<',"&lt;");
    }
 QString tag =  tagXml+'>';
 /*if (nextPos) {posDeb  = dataXml.find(tag, *nextPos);*nextPos=0;}  // on  remet a zero au cas ou tag pas trouve (recommencer a zero)
 else        */ {posDeb  = dataXml.find(tag);}
 if (posDeb==-1)         return false;
 posDeb += tag.length(); tag = tag.prepend("</");
 posEnd  = dataXml.find(tag, posDeb);
 if (posEnd==-1)         return false;
 //if (nextPos) *nextPos = posEnd + tag.length();
 dataXml = dataXml.remove(posDeb,posEnd-posDeb);
 dataXml = dataXml.insert(posDeb,valeur);
 return true;
}
//---------------------------- getXmlDataList static --------------------------------------------------------------------
/*! \brief retourne une QStringList de valeurs à partir d'un fichier XML et d'un tag
 *   dans cet exemple la valeur serait 'et de fin' (sans les apostrophes)
 *  \param tagName :  String indiquant le nom du tag sans les  <  />
 *  \param dataXml :  String XML dans laquelle on va lire la valeur
 *  \param nextPos :  pointeur sur un int qui si il n'est pas egal a zero indique
 *                    la position a partir de laquelle chercher. Sera positionne apres le tag recherche.
 *  \param noConvertCharToHtml  if zero (default value) '>' '<' '&' are  converted in '&gt;' '&lt;' '&amp;'
 *  \return QStringList qui est la liste de valeurs a rechercher.
*/
QStringList CGestIni::getXmlDataList(const QString& tagName, const QString& dataXml, int *nextPos /* =0 */, int noConvertCharToHtml /* =0 */ )
{int posEnd    = -1;
 int posDeb    =  0;
 QString tag   =  tagName+'>';
 QString toAdd = "";
 QStringList retList;

 if (nextPos) {posDeb  = dataXml.find(tag, *nextPos);*nextPos=0;}  // on  remet a zero au cas ou tag pas trouve (recommencer a zero)
 else         {posDeb  = dataXml.find(tag);}

 while (posDeb != -1)
       {posDeb += tag.length(); tag = tag.prepend("</");
        posEnd  = dataXml.find(tag, posDeb);
        if (posEnd==-1)         return retList;

        toAdd = dataXml.mid(posDeb,posEnd-posDeb);

        posDeb = posEnd + tag.length();      // on se place apres le tag de fin donc a la prochaine position
        if (nextPos) *nextPos = posDeb;      // on retourne si besoin cette prochaine position
        if (noConvertCharToHtml==0)
           { toAdd.replace("&gt;",">");
             toAdd.replace("&lt;","<");
             toAdd.replace("&amp;","&");
           }
        retList.append(toAdd);
        tag    =  tagName+'>';              // reinitialiser le tag car a ete modifie avant par tag = tag.prepend("</");
        posDeb = dataXml.find(tag, posDeb);
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
int  CGestIni::findFermant(const QString *ptext, int pos, int pos_max, const QString &ouvrant, const QString &fermant)
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
int  CGestIni::findFermant(const QString *ptext, int pos, int pos_max, const QString &ouvrant, const QString &fermant, int &isWithToken)
{ int stack        = 1;
  int ouvrant_len  = ouvrant.length();
  int fermant_len  = fermant.length();
  isWithToken      = 0;
  while (stack && pos < pos_max)
    {if (ptext->at(pos)==ouvrant.at(0) && ptext->mid(pos, ouvrant_len) == ouvrant)
        {++   stack;
         pos += ouvrant_len;
         ++   isWithToken;
        }
     else if (ptext->at(pos)==fermant.at(0) && ptext->mid(pos, fermant_len) == fermant)
        {--   stack;
         pos += fermant_len;
        }
     else
        {++pos;
        }
   }
 return pos-fermant_len;    // - fermant_len pour pointer avant le fermant
}
//-----------------------------------------------------  Param_UpdateToDisk --------------------------
/*! \brief sauvegarde les paramètres iniParam dans un fichier dont le chemin est spécifié.
 *  \todo Attention le fichier n'est pas fermé...
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
               qDebug(mess);
               return ;
            }
       }
    QFile file( file_ini);
    if ( !file.open( IO_WriteOnly ) )    return;
    QTextStream ts( &file );
    ts << inParam;
    file.close();
}

//-----------------------------------------------------  Param_UpdateFromDisk ------------------------------
/*! \brief Lit le fichier file_ini et renvoie son contenu dans outParam. L'encodage est géré.
*/
QString  CGestIni::Param_UpdateFromDisk(const QString &file_ini)
{ QString outParam="";
  if (Param_UpdateFromDisk(file_ini, outParam)==0) return QString::null;
  return outParam;
}
//-----------------------------------------------------  Param_UpdateFromDisk ------------------------------
/*! \brief Lit le fichier file_ini et renvoie son contenu dans outParam. L'encodage est géré.
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
                  qDebug(mess);
                  if (isRescue)
                     { QString mess = QString("ERROR : Param_UpdateFromDisk() initialisation file length is zero, trying with : %1").arg(file_ini+"_rescue");
                       qDebug(mess);
                       len = _loadFromDisk(file_ini+"_rescue", outParam, isUtf8_ret );
                       if (len==0)
                          { QString mess = QString("ERROR : Param_UpdateFromDisk() rescue file length is zero  %1").arg(file_ini+"_rescue");
                            qDebug(mess);
                          }
                     } // if (isRescue)
                  while (len && (outParam.at(len-1)==' '||outParam.at(len-1)=='\t'||outParam.at(len-1)=='\r'||outParam.at(len-1)=='\n')) --len;
                  outParam.truncate(len);
                } // if (len<=0)

             //....................... tester le tag d'integrite ......................
             if (len && !outParam.endsWith("[END_INI]") )
                { QString mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() integrity tag '[END_INI]' not found in: %1").arg(file_ini);
                  qDebug(mess);
                  if (isRescue)
                     { QString tmp_out         = "";
                       int     tmp_isUtf8_ret  = 0;
                       long ret                = _loadFromDisk(file_ini+"_rescue", tmp_out, &tmp_isUtf8_ret );
                       if (ret)
                          { mess     = QString("ERROR INTEGRITY : Param_UpdateFromDisk() using initialisation file rescue : %1").arg(file_ini+"_rescue");
                            qDebug(mess);
                            outParam = tmp_out;
                            if (isUtf8_ret) *isUtf8_ret = tmp_isUtf8_ret;
                            while (ret && (outParam.at(ret-1)==' '||outParam.at(ret-1)=='\t'||outParam.at(ret-1)=='\r'||outParam.at(ret-1)=='\n')) --ret;
                            outParam.truncate(ret);
                            if ( !outParam.endsWith("[END_INI]") )
                               { mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() end rescue file not good '[END_INI]' added to variable end");
                                 qDebug(mess);
                                 outParam += "\r\n[END_INI]";
                               }
                            return ret;
                          }
                     } //if (isRescue)
                  else
                     { mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() rescue file not found : %1").arg(file_ini+"_rescue");
                       qDebug(mess);
                       mess = QString("ERROR INTEGRITY : Param_UpdateFromDisk() integrity tag '[END_INI]' just added to variable end, hope that is enough");
                       qDebug(mess);
                       outParam.append("\r\n[END_INI]");
                     }
                }
            } // if (file_ini.endsWith(".ini"))
         return outParam.length();
}

//-----------------------------------------------------  _loadFromDisk ------------------------------
/*! \brief Lit le fichier file_ini et renvoie son contenu dans outParam. L'encodage est géré.
*/
long  CGestIni::_loadFromDisk(const QString &file_ini, QString &outParam, int *isUtf8_ret /* =0 */)
{        //............ charger le fichier .ini ..........
         QFile *pQFile = new QFile(file_ini );
         if (pQFile==0)                                                           return  0;
         if (pQFile->open( IO_ReadOnly )==FALSE){delete pQFile;                   return  0;}
         long file_len = pQFile->size();
         char    *text = new char[file_len+600];    // +5 pour permettre analyse utf8 qui explore trois apres
         if (text==0)                           {pQFile->close (); delete pQFile; return  0;}
         pQFile->readBlock (text, file_len);
         pQFile->close ();
         text[file_len] = 0;
         //........ analyse du texte charge ......................
         int isUtf8 = IsUtf8(text, file_len);
         if ( isUtf8_ret ) *isUtf8_ret = isUtf8;
         if (isUtf8)
            {outParam    =  QString::fromUtf8 ( text ) ;
             int pos     = file_ini.findRev('.');
             if (pos!=-1 && file_ini.mid(pos,4).lower() == ".htm")
                {outParam.replace("meta name=\"qrichtext\" content=\"charset=utf-8\"",   // obligé d'etre en content=\"1\" pour que les tabulations fonctionnent !!
                                  "meta name=\"qrichtext\" content=\"1\"");
                }
             outParam.replace("&nbsp;", " ");
            }
         else
             outParam    = text;
         delete[]text;
         return outParam.length();
}

//-----------------------------------------------------  IsUtf8 --------------------------------------------
/*! \brief gestion de l'Utf8.
*/
QString CGestIni::CharStrToQStringUtf8( const  char *ptr  )
{if (IsUtf8( ptr, strlen(ptr) ))
    {return QString::fromUtf8 ( ptr );
    }
 else
    {return QString(ptr) ;
    }
}

//-----------------------------------------------------  IsUtf8 -------------------------------------------------
/*! \brief gestion de l'Utf8.
*/
int CGestIni::IsUtf8( const  QString & txt )
{// QCString cs    = txt.utf8 ();
 // const char *pt = cs;
 // return IsUtf8( pt , cs.length () );
 const char *pt = txt.data ();  // obsolete mais utile
 return IsUtf8( pt , strlen(pt) );
}

//-----------------------------------------------------  IsUtf8 -------------------------------------------------
/*! \brief gestion de l'Utf8.
*/
int CGestIni::IsUtf8( const  char *txt , long len)
{char unsigned *pt   = (char unsigned*) txt;
 if (pt==0 || *pt==0) return 0;
 int nb_utf8_paterns = 0;
 unsigned char *pt_end = pt + len;
 while ( pt<pt_end)//*pt &&
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
                else
                    {return 0;    // si car >= 128 sans un des paterns sus jacents --> on est pas en UTF8
                    }
               }
        ++pt;
      }
 return nb_utf8_paterns;
}

//--------------------------------- Utf8_Query -----------------------------------
/*! \brief gestion de l'Utf8.
*/
QString CGestIni::Utf8_Query(QSqlQuery &cur, int field)
{QCString     cString  =  cur.value(field).toCString ();
 char *ptr             =  cString.data();
 if (IsUtf8( ptr, cString.length()))
    {return QString::fromUtf8 ( ptr );
    }
 else
    {return QString(ptr) ;
    }

}


//-----------------------------------------------------  Param_WriteParam -----------------------------------
/*! \brief Ecrit dans la string des paramètres passée à la fonction.
 *  \param param : String paramètres dans laquelle on va écrire. Elle sera modifiée.
 *  \param section : section dans laquelle on écrit
 *  \param variable : variable concernée
 *  \param val1 -> val10 : valeurs à écrire.
 *  \return QString::null en cas d'erreur, sinon le String de paramètre au complet.
 * Si la variable n'est pas trouvée dans la section elle est ajoutée. Si la section n'existe pas elle est ajoutée en fin de fichier.
*/
QString CGestIni::Param_WriteParam(QString *pQstr, const char *section, const char  *variable,
                                      const char *val1, const char *val2, const char *val3, const char *val4, const char *val5,
                                      const char *val6, const char *val7, const char *val8, const char *val9, const char *val10)
{QString result    = "";
 QString tmp       = "";
 if (pQstr == 0        )    return QString::null;
 if (pQstr->length()==0 && section && *section)   *pQstr = QString("[")+section+"]\r\n" ;
 if (pQstr->length()==0)    return QString::null;
 const char* txt   = *pQstr;
 char *pt          = (char*) txt;
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
     case '[': //....................debut de section: tester si c'est celle recherchée ...................
          ++pt;
          while (*pt && (*pt==' '||*pt=='\t'))  ++pt;                // enlever espaces de debut
          deb = pt;
          while (*pt && *pt != ']') ++pt;                            // chercher delimiteur de fin de nom de section
          end = pt;
          while (end>deb && (end[-1]==' '||end[-1]=='\t')) end--;    // enlever espaces de fin
          len_section = end - deb;
          if ((long)strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchée
             {pt = GotoNextDebLigne(pt);                                               // aller ligne suivante

              while (*pt && *pt != '[')                                                // tant que pas fin de zone de section (debut d'une autre)
                 {
                  while (*pt && (*pt==' '||*pt=='\t'))  ++pt ;                               // sauter espaces de debut de ligne
                  if  (*pt==';' || *pt=='\n'|| *pt =='\r') pt = GotoNextDebLigne(pt) ;       // sauter les commentaires et retour ligne
                  deb = pt;                       // on devrait etre au debut du nom d'une variable
                  while (*pt && *pt!='=' && *pt !='\n' && *pt!='\r' && *pt!='[') ++pt;      // le signe '=' marque la fin de zone du nom de la variable

                  //................... fin de nom de variable trouvé ( signe egal) ...........................
                  if (*pt=='=')
                     {end = pt;
                      while (end>deb && (end[-1]==' '||end[-1]=='\t')) end-- ;   // enlever espaces de fin du nom de la variable
                      len_variable = end - deb;
                      //.......................... Est ce celle recherchée ...................................
                      if ((long)strlen(variable)==len_variable && strncmp(variable, deb, len_variable)==0)   // SI c'est la variable recherchée
                         {deb_lgn = deb;
                          end_lgn = end;
                          while (*end_lgn && *end_lgn !=';' && *end_lgn !='\n' && *end_lgn!='\r')      // aller jusqu'au prochain marqueur de fin des valeurs
                                {if (*end_lgn=='\\' && end_lgn[1] !=0 ) end_lgn++;                     // en sautant le caractère d'echapement et le suivant
                                 end_lgn++;
                                }
                          while (end_lgn> end && (end_lgn[-1] ==' ' || end_lgn[-1] =='\t')) end_lgn--;  // reculer j'usqu'à la fin de la derniere valeur
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
               //.......................... fin de section atteinte sans avoir trouvé la variable .........
               //                           la section est trouvée mais pas la variable on la rajoute
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
               tmp     = tmp + "\r\n\r\n";
               pos     = pt-txt;
               result  = pQstr->left(pos) + tmp ;
               while (*pt && (*pt=='\r' || *pt=='\n' || *pt=='\t' || *pt==' ')) ++pt;           // nettoyer apres eliminer espaces et  CR/LF apres
               pos     = pt-txt;
               result += pQstr->mid(pos);
               *pQstr  = result;
               return result;
              } // end if (strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchée
          break;
      default:
          ++pt;
     } // end switch (pt[0])
  } // end while (*pt)

 //................pas de section trouvée on la rajoute ...........................................
 tmp = "\r\n\r\n[";
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
 pQstr->truncate(pos);
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
 *  \param txt : String paramètres dans laquelle on va lire
 *  \param section : section dans laquelle on va lire
 *  \param variable : variable concernée
 *  \return QString::null la variable n'existe pas. sinon retourne la valeur
*/
QString CGestIni::Param_ReadUniqueParam(const char* txt, const char *section, const char  *variable)
{QString val;
 if (Param_ReadParam( txt, section, variable, &val)==0) return val;
 else return QString::null;
}

//-----------------------------------------------------  Param_ReadParam --------------------------------
/*! \brief récupère les valeurs au sein du String paramètre passé à  la fonction.
 *  \param txt : String paramètres dans laquelle on va lire
 *  \param section : section dans laquelle on va lire
 *  \param variable : variable concernée
 *  \param val1 -> val10 : valeurs à lire
 *  \return QString::null si tout OK. sinon retourne le message d'erreur
*/
QString CGestIni::Param_ReadParam(  const char* txt, const char *section, const char  *variable,
                                    QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                    QString *val6, QString *val7, QString *val8, QString *val9, QString *val10)

{if (txt==0)   return QObject::tr("Erreur de syntaxe: Fichier de paramètres vide");
 char *pt          = (char*) txt;
 char *deb         = 0;
 char *end         = 0;
 long len_section  = 0;
 long len_variable = 0;
 if (section==0)
    {QString str(QObject::tr("Erreur : pas de section dans l'appel de la fonction CGestIni::Param_ReadParam()"));
     qDebug(str);
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
     case '[': //....................debut de section: tester si c'est celle recherchée ...................
          ++pt;
          while (*pt && (*pt==' '||*pt=='\t'))  ++pt;                // enlever espaces de debut
          deb = pt;
          while (*pt && *pt != ']') ++pt;                            // chercher delimiteur de fin de nom de section
          end = pt;
          while (end>deb && (end[-1]==' '||end[-1]=='\t')) end--;    // enlever espaces de fin
          len_section = end - deb;
          if ((long)strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchée
             {pt = GotoNextDebLigne(pt);
              while (*pt && (*pt==' '||*pt=='\t'||*pt=='\r'||*pt=='\n'))  ++pt ;                                                 // aller ligne suivante (zone des variables de section)
              while (*pt && *pt != '[')                                                      // tant que pas fin de zone de section (debut d'une autre ou fin)
                 {
                  while (*pt && (*pt==' '||*pt=='\t'))  ++pt ;                                             // sauter espaces de debut de ligne
                  if  (*pt==';' || *pt=='\n'|| *pt =='\r') { pt = GotoNextDebLigne(pt) ;                   // sauter les commentaires et retour ligne en allant ligne suivante
                                                             while (*pt && (*pt==' '||*pt=='\t'))  ++pt ;  // ressauter espaces de debut de ligne
                                                           }
                  else if (*pt=='/'&& pt[1] =='/')         { pt = GotoNextDebLigne(pt);
                                                             while (*pt && (*pt==' '||*pt=='\t'))  ++pt ;  // ressauter espaces de debut de ligne
                                                           }
                  deb = pt;                       // on devrait etre au debut du nom d'une variable
                  //while (*pt && *pt!='=')  ++pt;  // le signe '=' marque la fin de zone du nom de la variable
                  while (*pt && *pt!='=' && *pt!='[')
                        {if (*pt=='\\' && pt[1] !=0 ) pt += 2;
                         else ++pt;
                        }
                  if (*pt==0||*pt=='[') return QObject::tr("Signe = non trouvé dans la section : ") + section + QObject::tr(", variable recherchée : ") + variable ;
                  end = pt;
                  while (end>deb && (end[-1]==' '||end[-1]=='\t')) end-- ;   // enlever espaces de fin du nom de la vaiable
                  len_variable = end - deb;
                  if ((long)strlen(variable)==len_variable && strncmp(variable, deb, len_variable)==0)   // SI c'est la variable recherchée
                     {++pt;   // passer le signe egal
                      Param_ExtraireValeurs(pt, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10);
                      return QString::null;
                     }
                  pt = GotoNextDebLigne(pt);                                               // aller ligne suivante
                 }
               //......... si on arrive ici c'est que ok pour la section mais pas de variable ..........................
               return QObject::tr("Pas de variable : ") + variable + QObject::tr("  pour la section : ") + section;
              } // end if (strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchée
          break;
      default:
          ++pt;
     } // end switch (pt[0])
  } // end while (*pt)
 return QObject::tr("Erreur de syntaxe : Pas de section dans ce fichier");
}

//-----------------------------------------------------  ExtraireValeurs -----------------------------------
/*! \brief txt pointe sur un texte contenant differentes valeurs terminées par \r\n ou zero de fin, et separées par des virgules. Une valeur peut etre une chaine de caractères et si elle doit comporter une virgule ou point virgule ceux ci doivent etre annoncés par le caractere d'echapement qui est l'anti slash
*/
char *CGestIni::Param_ExtraireValeurs(const char* txt,
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
       {if (*pt=='\\' && pt[1] !=0 ) ++pt;                                       // sauter le caractère d'echapement et le suivant
        ++pt;
       }
    end = pt;
    while (end>deb && (end[-1]==' '||end[-1]=='\t')) --end;                      // enlever espaces de fin
    len    = end-deb;
    switch(num_param)
           {case 0: if (val1)  val1->setLatin1(deb,len);  break;
            case 1: if (val2)  val2->setLatin1(deb,len);  break;
            case 2: if (val3)  val3->setLatin1(deb,len);  break;
            case 3: if (val4)  val4->setLatin1(deb,len);  break;
            case 4: if (val5)  val5->setLatin1(deb,len);  break;
            case 5: if (val6)  val6->setLatin1(deb,len);  break;
            case 6: if (val7)  val7->setLatin1(deb,len);  break;
            case 7: if (val8)  val8->setLatin1(deb,len);  break;
            case 8: if (val9)  val9->setLatin1(deb,len);  break;
            case 9: if (val10) val10->setLatin1(deb,len); break;
           }
    if (*pt==',')  ++pt;      // on saute la virgule et on passe au param suivant
    ++num_param;
   }
 return pt;
}

//-----------------------------------------------------  ExtraireValeurs -----------------------------------
/*! \brief txt pointe sur un texte contenant differentes valeurs terminées par \r\n ou zero de fin, et separées par des virgules. Une valeur peut etre une chaine de caractères et si elle doit comporter une virgule ou point virgule ceux ci doivent etre annoncés par le caractere d'echapement qui est l'anti slash
*/
char *CGestIni::Param_ExtraireValeurs(const char* txt, QStringList &lst)
{char *pt          = (char*) txt;
 char *deb         = 0;
 char *end         = 0;
 long   len        = 0;
 QString       val = "";
 lst.clear();
 while (1)
   {while (*pt && *pt !='\r' && *pt !='\n' && *pt !=';' && (*pt==' '||*pt=='\t'))  ++pt;      // sauter espaces de debut
    if (*pt==0 || *pt =='\r' || *pt =='\n'|| *pt ==';') return pt;
    deb = pt;                                                                    // on devrait etre au debut du nom d'une valeur
    while (*pt && *pt !=',' && *pt !='\n' && *pt!='\r')                          // aller jusqu'a la prochaine virgule (fin de la valeur)
       {if (*pt=='\\' && pt[1] !=0 ) ++pt;                                       // sauter le caractère d'echapement et le suivant
        ++pt;
       }
    end = pt;
    while (end>deb && (end[-1]==' '||end[-1]=='\t')) end--;                      // enlever espaces de fin
    len    = end-deb;
    lst.append(val.setLatin1(deb,len));
    if (*pt==',')  ++pt;      // on saute la virgule et on passe au param suivant
   }
 return pt;
}

//----------------------------------------- Param_GetList ---------------------------------------------
/*! \brief Récupère les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string paramètre (outParam) dans une QStringList lst.
*/
void CGestIni::Param_GetList(const QString &outParam, const QString &sectionToRetrieve, const QString &varToRetrieve,  QStringList &lst , int isToStrip /*=0*/)
{
 QString        data    = "";
 QString      section   = "";
 QString     var_name   = "";
 const char      *txt   = outParam;
 char             *pt   = (char*) txt;
 char             *deb  = 0;
 if (pt==0)  return;
 if (*pt==0) return;
 while((pt = Param_GotoNextSection(pt, 0, &section)) && *pt)
    {if (section == sectionToRetrieve)
        {lst.clear();
         while (*pt && *pt != '[')
                {
                 /*
                 data    = "";
                 if (varToRetrieve.length())
                    {pt      = Param_ExtraireNextValeurs(pt, var_name, &data);
                     if (data.length() && (varToRetrieve.length()==0 || var_name.startsWith(varToRetrieve)) )
                        {lst.append(data.stripWhiteSpace());
                        }
                    }
                 else */
                    {
                     while (*pt && (*pt==' '||*pt=='\t') )      ++pt ; // sauter espaces de debut de ligne
                     if  (*pt==';' || *pt=='\n'|| *pt =='\r')
                         {pt = GotoNextDebLigne(pt) ;                  // sauter les commentaires et retour ligne
                          while (*pt && (*pt==' '||*pt=='\t') ) ++pt ; // sauter espaces de debut de ligne
                         }
                     if  (*pt=='[') return;
                     deb  = pt;                                                                  // on devrait etre au debut du nom d'une variable
                     pt   = GotoEndOfLigne(pt);
                     data = QString::fromLatin1 (deb, pt-deb).stripWhiteSpace();
                     if (data.length())
                        {int pos = data.find('=');
                         if (pos != -1)
                            {var_name  = data.left(pos).stripWhiteSpace();
                             data      = data.mid(pos+1);
                            }
                         if (isToStrip) data = data.stripWhiteSpace();
                         if (varToRetrieve.length()==0)                lst.append(data);
                         else if( var_name.startsWith(varToRetrieve))  lst.append(data);
                        }
                    }
                }
         break;
        }
     }
}
//----------------------------------------- Param_GetMap ---------------------------------------------
/*! \brief Récupère les valeurs d'une variables (varToRetrieve) d'une section (sectionToRetrieve) du string paramètre (outParam) dans une QStringList lst.
*/
void CGestIni::Param_GetMap(QString &outParam, const QString &sectionToRetrieve, const QString &varToRetrieve,  QMap<QString,QString> &map , int isToStrip /*=0*/)
{
 QString        data    = "";
 QString      section   = "";
 QString     var_name   = "";
 const char      *txt   = outParam;
 char             *pt   = (char*) txt;
 char             *deb  = 0;
 if (pt==0)  return;
 if (*pt==0) return;
 while((pt = Param_GotoNextSection(pt, 0, &section)) && *pt)
    {if (section == sectionToRetrieve)
        {
         while (*pt && *pt != '[')
                {data    = "";
                 if (varToRetrieve.length())
                    {pt      = Param_ExtraireNextValeurs(pt, var_name, &data);
                     if (data.length() && (varToRetrieve.length()==0 || var_name.startsWith(varToRetrieve)) )
                        {map[var_name]=(data.stripWhiteSpace());
                        }
                    }
                 else
                    {
                     while (*pt && (*pt==' '||*pt=='\t') )      ++pt ; // sauter espaces de debut de ligne
                     if  (*pt==';' || *pt=='\n'|| *pt =='\r')
                         {pt = GotoNextDebLigne(pt) ;                  // sauter les commentaires et retour ligne
                          while (*pt && (*pt==' '||*pt=='\t') ) ++pt ; // sauter espaces de debut de ligne
                         }
                     if  (*pt=='[') return;
                     deb  = pt;                                                                  // on devrait etre au debut du nom d'une variable
                     pt   = GotoEndOfLigne(pt);
                     data = QString::fromLatin1 (deb, pt-deb).stripWhiteSpace();
                     if (data.length())
                        {int pos = data.find('=');
                         if (pos != -1)
                            {var_name = data.left(pos).stripWhiteSpace();
                             data     = data.mid(pos+1);
                             if (isToStrip)   map[var_name]=(data.stripWhiteSpace());
                             else             map[var_name]=(data);
                            }
                        }
                    }
                }
         break;
        }
     }
}

//------------------------------ Param_RemoveSection -----------------------------------------
/*! \brief enlève une section.
*/
QString CGestIni::Param_RemoveSection(QString &param, QString section)
{    section.prepend("[");
     section.append("]");
     int pos_deb  = param.find(section);
     int pos_end  = -1;
     if (pos_deb != -1)
        {pos_end = param.find("[", pos_deb+section.length());
         //.............. eliminer tous les espaces rn tab  arrieres .......................
         while (pos_deb>0){QChar c=param[pos_deb-1];
                           if (c=='\t' || c=='\r'|| c=='\n'|| c==' ') --pos_deb;
                           else                                           break;
                          }
         if (pos_end != -1)
            { param = param.remove(pos_deb, pos_end-pos_deb);
              param = param.insert(pos_deb,"\r\n\r\n");
            }
         else
            { param.truncate(pos_deb);
            }
        }
    return param;
}

//------------------------------ Param_GotoNextSection -----------------------------------------
/*! \brief Passe à la section suivante en débutant au pointeur pt.
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
     case '[': //....................debut de section: tester si c'est celle recherchée ...................
          ++pt;
          while (*pt && (*pt==' '||*pt=='\t'))  ++pt;                // enlever espaces de debut
          deb = pt;
          while (*pt && *pt != ']') ++pt;                            // chercher delimiteur de fin de nom de section
          if ( *pt != ']')    return  pt;                                     // y en a pas --> erreur (*pt == 0)
          end = pt;
          while (end>deb && (end[-1]==' '||end[-1]=='\t')) end--;    // enlever espaces de fin
          len_section = end - deb;
          if (len_section>0 && pQsection != 0)
             {pQsection->setLatin1(deb, len_section);
             }
          if ( section == 0)  return  GotoNextDebLigne(pt);   // pas de section donnée, on s'arrete à la première trouvée
          if ((long)strlen(section)==len_section && strncmp(section, deb, len_section)==0)             // SI c'est la section recherchée
             {                return  GotoNextDebLigne(pt);                            // aller ligne suivante
             } // end if (strlen(section)==len_section && strncmp(section, deb, len_section)==0)   // SI c'est la section recherchée
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
/*! \brief Extrait les valeurs d'une variable donnée.
*/
char *CGestIni::Param_ExtraireNextValeurs(char* pt, QString &var_name,
                                             QString *val1, QString *val2, QString *val3, QString *val4, QString *val5,
                                             QString *val6, QString *val7, QString *val8, QString *val9, QString *val10)
{return Param_ExtraireNextValeurs(pt, var_name, 0, 0, val1,  val2,  val3,  val4,  val5,  val6,  val7,  val8,  val9, val10);
}

//------------------------------ Param_ExtraireNextValeurs ----------------------------------------
/*! \brief Extrait les valeurs d'une variable donnée dans un string paramètre donné.
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
    {var_name = QString::fromLatin1 (deb, len_variable).stripWhiteSpace();
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
//------------------------------ Param_ExtraireNextValeurs ----------------------------------------
/*! \brief Extrait les valeurs d'une variable donnée dans un string paramètre donné.
*/
char *CGestIni::Param_ExtraireNextValeurs(char* pt, QString &var_name, char**pt_lgn, long *len, QStringList &lst)
{char *deb         = 0;
 char *end         = 0;
 long len_variable = 0;
 lst.clear();
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
    {var_name = QString::fromLatin1 (deb, len_variable).stripWhiteSpace();
    }
 ++pt;   // passer le signe egal
 Param_ExtraireValeurs(pt, lst);
 if (pt_lgn)
    {*pt_lgn = deb;
     pt      = GotoEndOfLigne(pt);
     if (*len) *len = (pt-deb);
    }
 return GotoNextDebLigne(pt);                                               // aller ligne suivante
}

//------------------------------ GotoNextDebLigne -----------------------------------------
/*! \brief déplace le pointeur jusqu'au début de la ligne suivante.
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

//------------------------------ GotoEndOfLigne -----------------------------------------
/*! \brief déplace le pointeur jusqu'à la fin de la ligne.
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

//------------------------------ Construct_Name_Exe -----------------------------------------
/*! \brief construit le chemin vers l'executable du module (module) de MendinTux. Gère la compatibilité Windows/Linux/Mac.
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
        //Création du nom de fichier
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
        path = QDir::cleanDirPath(path);
        return path;

}


//------------------------------ Construct_Name_File_Ini -----------------------------------------
/*! \brief construit le chemin vers le fichier ini du module (module) de MendinTux. Gère la compatibilité Windows/Linux/Mac.
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

        path = QDir::cleanDirPath(path);
        return path;
}

//------------------------------ Construct_PathBin_Module -----------------------------------------
/*! \brief Permet la construction d'un nom complet de fichier executable en  relation avec le module correspondant
 *  \param QString  module : nom du programme dont on cherche à construire le chemin complet si egal à "" alors le nom du chemin sera extrait de : start_Argv
 *  \param const QString & start_Argv : Chemin complet de demarrage du programme
 *  \param QString *base_name : adresse d'une QString qui dans laquelle si elle est différente de zero sera retourné le nom du programme SANS SON EXTENSION
 *  \return nom du module  avec le chemin complet de demarrage du proc --> nom  ini complet du module
*/
QString CGestIni::Construct_PathBin_Module(const QString  &module, const QString & ref_dir, QString sep /* = "/" */)
{ return  QDir::cleanDirPath(ref_dir + "/" + PATH_SPEC_MAC + "../../" + module + "/bin/")+ sep ;
}

//------------------------------ AbsoluteToRelativePath -----------------------------------------
/*! \brief convertit un chemin absolu en chemin relatif par rapport a un chemin de reference, il est imperatif que les chemins d'entree soient tous en absolu.
 *  \param QString pathRef : chemin de reference par rapport auquel il faut relativiser
 *  \param QString pathToConvert : chemin � convertir
 *  \return chemin convertit en relatif
*/
QString CGestIni::AbsoluteToRelativePath(QString pathRef, QString pathToConvert)
{int       posR = -1;
 int       posC = -1;
 if (pathToConvert.length()>2 && pathToConvert.left(2)=="\\\\")  return pathToConvert;  // si reseau on ne peut pas relativiser
 if (pathToConvert.length()>2 && pathToConvert.left(2)=="//")    return pathToConvert;  // si reseau on ne peut pas relativiser
 pathRef       = pathRef.replace("\\","/");
 pathToConvert = pathToConvert.replace("\\","/");
 pathRef       = pathRef.replace("//","/");
 pathToConvert = pathToConvert.replace("//","/");
 //............ enlever l'�ventuel C: ...........................
 //             de facon � ce que les racines soient identiques : /xxxx/nnnnn/etc/
 posR           = pathRef.find(':');
 posC           = pathToConvert.find(':');
 if (posR !=-1 && posC !=-1 && pathRef[0] != pathToConvert[0]) return pathToConvert; // si les partitions sont differentes l'on ne peut pas relativiser
 if (posR !=-1) pathRef       = pathRef.mid(posR+1);
 if (posC !=-1) pathToConvert = pathToConvert.mid(posC+1);
 pathRef       = pathRef.stripWhiteSpace();
 pathToConvert = pathToConvert.stripWhiteSpace();
 //........................... rechercher jusqu'ou le debut des chemins est le meme ..............................
 int posRef      = pathRef.find('/');
 int posDst      = pathToConvert.find('/');
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
     posRef      = pathRef.find('/',last_posRef+1);
     posDst      = pathToConvert.find('/',last_posDst+1);
    }
  //........................... couper les chemins ..............................
 pathToConvert = pathToConvert.mid(posToCut);
 pathRef       = pathRef.mid(posToCut);
 //............................ compter le nombre de ../ a mettre ...................
 int            n = pathRef.contains( '/');
 QString relative = "";
 for (int i=1; i<n; ++i) relative += "../";
 relative   += pathToConvert;
 relative    = relative.replace("//","/");
 return relative;
}

//--------------------------------------------- isInBlackList --------------------------------------------
/*! \brief verifie si le terme name est concerne par la blackList
*  \param black_str :      const QString & black_str 
*  \param pathDoc :   const QString & name terme dont il faut verifie si concerne
*  \return int :   vrai/1 si concerne    faux/0 si non concerne
*/
int CGestIni::isInBlackList( QString black_str, const QString &name)
{  black_str              = black_str.replace('\r',"").stripWhiteSpace();
   QStringList black_list = QStringList::split('\n',black_str);
   return isInBlackList(black_list, name);
}

//--------------------------------------------- isInBlackList --------------------------------------------
/*! \brief verifie si le terme name est concerne par la blackList
*  \param black_list :      const QStringList & black_list 
*  \param pathDoc :   const QString & name terme dont il faut verifie si concerne
*  \return int :   vrai/1 si concerne    faux/0 si non concerne
*/
int CGestIni::isInBlackList(const QStringList &black_list, const QString &name)
{ for (int i=0; i<(int)black_list.count(); ++i)
      { QString black_str = black_list[i].stripWhiteSpace();
        if (black_str.startsWith("*") && black_str.endsWith("*"))
           { black_str = black_str.remove('*');
             if (name.contains(black_str))         return 1;
           }
        else if (black_str.startsWith("*"))
           { black_str = black_str.remove('*');
             if (name.endsWith(black_str))         return 1;
           }
        else if (black_str.endsWith("*"))
           { black_str = black_str.remove('*');
             if (name.startsWith(black_str))       return 1;
           }
        else
           { if (name==black_str)                  return 1;
           }
      }
  return 0;
}
//-------------------------- listDirectory -------------------------------------------
/*! \brief non documenté */
QStringList CGestIni::listDirectory(QString start_dir, const QString &filterExt_in/*=""*/, const QString &filterName/*=""*/, const QString &sep/*=";"*/, bool listWithoutExt/*=FALSE*/, bool listWithDirName /*=FALSE*/)
{
    QStringList    ret;
    QString     filterExt = filterExt_in;
    QStringList filterList;
    QStringList extList;
    if (filterName.length())  filterList = QStringList::split ( sep, filterName);
    if (filterExt.length())   extList    = QStringList::split ( sep, filterExt.remove('.'));   //suffix() retourne l'extension sans le point


    QDir cur_dir( start_dir );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );

    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return ret;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {   bool addToList = FALSE;
        QString fname  = fi->fileName().latin1();
        QString ext    = QFileInfo(fname).extension().lower();
        QStringList::Iterator ut;
        if      (fname == "..")
                {
                }
        else if (fi->isDir() && fname != "." && listWithDirName)
                {  addToList = TRUE;
                }
        else if (fi->isFile())
                {  addToList = TRUE;
                }
        if      (addToList)
                {   bool ok = TRUE;

                    if (extList.count())
                       {ok = FALSE;
                        ut = extList.find(ext);
                        if (ut !=  extList.end()) ok = TRUE;
                       }
                    if (ok && filterList.count())
                       {ok = FALSE;
                        for ( ut = filterList.begin(); ut != filterList.end() && ok==FALSE; ++ut )
                            {if (fname.find(*ut) != -1) ok = TRUE;
                            }
                       }
                    if (ok)
                       {if (listWithoutExt)    ret.append(fi->baseName());
                        else                   ret.append(fname);
                       }
                 }
        ++it;
    }

   /*------ QT4 -------------


    QStringList    ret;
    QString        filterExt = filterExt_in;
    QStringList filterList;
    QStringList extList;
    if (filterName.length())  filterList = QStringList::split ( sep, filterName);
    if (filterExt.length())   extList    = QStringList::split ( sep, filterExt.remove('.'));   //suffix() retourne l'extension sans le point

    //................ allons y  .....................................................
    QDir l_dir;
    l_dir.setPath ( start_dir );
    l_dir.convertToAbs();
    l_dir.setFilter( QDir::All | QDir::NoSymLinks );

    //............ exploration de la liste ......................
    QFileInfoList qFileInfoList   = l_dir.entryInfoList();
    QList<QFileInfo>::iterator it = qFileInfoList.begin();
    QFileInfo            fi;
     while ( it != qFileInfoList.end() )
          {fi = *it;
           QString fname = fi.fileName();
           QString ext   = QFileInfo(fname).suffix().lower();
           QStringList::Iterator ut;
           if      (fname == "..")
                   {
                   }
           else if (fi.isDir() && fname[0] != '.' )
                   {
                   }
           else if (fi.isFile() && fname[0] != '.')
                   {bool ok = TRUE;

                    if (extList.count())
                       {ok = FALSE;
                        ut = extList.find(ext);
                        if (ut !=  extList.end()) ok = TRUE;
                       }
                    if (ok && filterList.count())
                       {ok = FALSE;
                        for ( ut = filterList.begin(); ut != filterList.end() && ok==FALSE; ++ut )
                            {if (fname.find(*ut) != -1) ok = TRUE;
                            }
                       }
                    if (ok)
                       {if (listWithoutExt)    ret.append(fi.baseName());
                        else                   ret.append(fname);
                       }
                   }
           ++it;
          }
 */
  return ret;
}
