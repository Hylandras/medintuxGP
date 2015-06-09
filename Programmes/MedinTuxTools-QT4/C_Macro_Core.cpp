/*********************************** C_Macro_Core.cpp *****************************
 *                                                                                *
 *   #include "C_Macro_Core.h"                                                    *
 *   Project: MedinTux                                                            *
 *   Copyright (C) for the eternity by Sevin Roland  and Data Medical Design      *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
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

#include "C_Macro_Core.h"
#include "CGestIni.h"
#include "C_Utils_Log.h"

#include <QObject>
#include <QDate>
#include <QDateTime>
#include <QDir>
#include <QDebug>

QMap <QString, QString>  C_Macro_Core::m_VariablesMap    = QMap<QString, QString>();
QMap <QString, QString> *C_Macro_Core::m_pVariablesMap   = 0;
QTextEdit               *C_Macro_Core::m_logWidget       = 0;
QString                 *C_Macro_Core::m_pDocument       = 0;
char                     C_Macro_Core::Utf8_Insecable[3] = {194,160,0};   // espace insecable
//---------------------------- C_Macro_Core ----------------------------------------
C_Macro_Core::C_Macro_Core( KEY_MACRO_TAB_DEF *pMacroTab_List , VAR_MAP *pVAR_MAP /* = 0 */)
{ m_MacroTab_List               = pMacroTab_List;
  m_pDocument                   = 0;
  if (pVAR_MAP) m_pVariablesMap = pVAR_MAP;
  else          m_pVariablesMap = &m_VariablesMap;
}

//---------------------------- ~C_Macro_Core ----------------------------------------
C_Macro_Core::~C_Macro_Core()
{
}

//----------------------------------------  run -------------------------------------------------
/*! \brief Fusionne les champs de fusion d'un document donnee en entree par: QString *document
 *  \param  document : pointeur sur le document
 *  \return  le document modifie par les macros resolues
 */
QString C_Macro_Core::run(QString &document)
{
   return run( &document );
}
//----------------------------------------  run -------------------------------------------------
/*! \brief Fusionne les champs de fusion d'un document donnee en entree par: QString *document
 *  \param  document : pointeur sur le document
 *  \return  le document modifie par les macros resolues
 */
QString C_Macro_Core::run(QString *document)
{ if (document==0) return QString::null;
  m_pDocument = document;
  run(document, 0, document->length());
  return *document;
}
//----------------------------------------  setLogWidget -------------------------------------------------
/*! \brief positionne l'eventuel widget de log
 */
void C_Macro_Core::setLogWidget(QTextEdit *logWidget){ C_Macro_Core::m_logWidget = logWidget;}

//----------------------------------------------- outMessage [static]---------------------------------------------------------------------
/*! \brief  append a message in a log widget QTextEdit
 *  \param  const QString &mess                 message to append
 *  \param  const QString   &file               source file
 *  \param  const int       &line               source line number
 * \return  QString:  message.
*/
QString     C_Macro_Core::outMessage( const QString &mess, const QString &file /* = "" */, const int line /* = -1*/)
{        if ( C_Macro_Core::m_logWidget ==0 )
            { qDebug() << mess;
              return mess;
            }
         C_Utils_Log::outMessage( C_Macro_Core::m_logWidget, mess, file, line );
         return mess;
}
//----------------------------------  run -------------------------------------------------
/*! \brief lance la resolution de macros
 *  \param  pos_deb  debut de la macro
 *  \param  pos_max int       &line               source line number
 * \return int :  qui est la nouvelle longueur.
 */
int C_Macro_Core::run(QString *pDocument, int pos_deb, int pos_max)
{//G_pCApp->m_VAR_MAP.insert("$SCRIPT_RUN","$TRUE");
 int isWithToken   =  0;
 int pos_end       =  0;
 //QString ptDeb;
 //QString ptEnd;
 int initialLen    =  pDocument->length();
 //if (initialLen==0) return 0;
 pos_deb           =  pDocument->indexOf("{{", pos_deb);
 int pDeltaLen     =  0;
 //int len           = 0;
 while (pos_deb   != -1 && pos_deb < pos_max)
       { pos_end    =  CGestIni::findFermant(pDocument, pos_deb+2, pos_max, "{{", "}}", isWithToken);
         //ptDeb = pDocument->mid(pos_deb,150);
         //ptEnd = pDocument->mid(pos_end,150);
         if (isWithToken)
            { pos_max   += run(pDocument, pos_deb+2, pos_end);
            }
         else
            { //QString token = pDocument->mid(pos_deb+2, pos_end - pos_deb-2);
              //qDebug() << "Token before : "<<token<<" pos_deb : "<<QString::number(pos_deb)<<" len_doc : "<<QString::number(pDocument->length())<<" pDocument : "<< pDocument->left(50);
              replaceTokenByValue(pDocument, pos_deb+2, pos_end - pos_deb-2 ,&pDeltaLen);
              pos_max += pDeltaLen;
              //qDebug() << "Token after  : "<<token<<" pos_deb : "<<QString::number(pos_deb)<<" len_doc : "<<QString::number(pDocument->length())<<" pDocument : "<< pDocument->left(50);

             //ptDeb = m_pDocument->mid(pos_deb,50);
             //ptEnd = m_pDocument->mid(pos_end,50);
             pos_deb   =  pDocument->indexOf("{{", pos_deb);
             //ptDeb     = *m_pDocument; ptDeb += pos_deb;
           }
       }
  //G_pCApp->m_VAR_MAP.insert("$SCRIPT_RUN","$FALSE");
  int ret     = pDocument->length() - initialLen;
  return ret;
}

//------------------------------------------ strToIsoStrDateTime -----------------------------
/*! \brief Retourne une chaine de la forme 12x09xx2007 a l'heure de 12h30 sous forme 2007-09-12T12:30:00 */
QString C_Macro_Core::strToIsoStrDateTime(const QString &date, QString *pDate  /* =0 */, QString *pTime  /* =0 */)
{int end = date.length();
 int   i = -1;
 QString str;
 QString hh,mm,ss,uu;
 while (++i < end)
       {if (  (date[i]>='0' && date[i]<='9') )
           {str += date[i];
           }
       }
 int len = str.length();
 if (len ==6 )  str.insert ( 4, "20" ); // date forme 011207 --> 01122007
 if (len <8 )   {return  QObject::tr("ERREUR : C_Macro_Core::strToIsoStrDateTime() format de date '%1' invalide").arg(date);}
 if (pTime)  *pTime = "";
 if (pDate)  *pDate = "";

 hh                 = str.mid (8,2);  if (hh.length()==0) hh="00";
 mm                 = str.mid (10,2); if (mm.length()==0) mm="00";
 ss                 = str.mid (12,2); if (ss.length()==0) ss="00";

 hh                 = hh+":"+mm+":"+ss;
 if (pTime)  *pTime = hh;
 str                = str.mid(4,4)+"-"+str.mid(2,2)+"-"+str.mid(0,2);
 if (pDate)  *pDate = str;
 return str+"T"+hh;
}

//------------------------------------------ daysTo -------------------------------------------------
/*! \brief Retourne le nombre de jours entre la date1 et la date2 */
QString C_Macro_Core::daysTo(const QString &date1, const QString &date2)
{QDate d2 = QDate::currentDate();
 if (date2.length()!=0) d2 = QDate::fromString (normaliseDate(date2), Qt::ISODate );
 QDate d1 = QDate::fromString (normaliseDate(date1), Qt::ISODate );
 return QString::number( d1.daysTo ( d2 ) );
}

//------------------------------------------ normaliseDate -----------------------------
/*! \brief Retourne une date de la forme 12x09xx2007 sous forme 2007-09-12 */
QString C_Macro_Core::normaliseDate(const QString &date)
{int end = date.length();
 int   i = -1;
 QString str;
 while (++i < end) {
                     if (  (date[i]>='0' && date[i]<='9') ) str += date[i];
                   }
 if (str.length() ==6 )  str.insert ( 4, "20" ); // date forme 011207 --> 01122007
 if (str.length() !=8 )  {return QObject::tr("ERREUR : C_Macro_Core::normaliseDate() format de date '%1' invalide").arg(date);}

 return str.mid(4,4)+"-"+str.mid(2,2)+"-"+str.mid(0,2);
}

//----------------- getVarValue [static]--------------------------------------------------
/*! \brief return value of a variable.
 *  \param varName : const QString & var name
 *  \return : contents of this varName
 */
QString   C_Macro_Core::getVarValue(const QString &varName)
{if (m_VariablesMap.contains(varName)) return m_VariablesMap[varName];
 return QString::null;
}
//----------------- setVarValue [static]--------------------------------------------------
/*! \brief return value of a variable.
 *  \param varName : const QString & var name
 *  \param value :  const QString & value to set for var name
 *  \return : contents of this varName
 */
void   C_Macro_Core::setVarValue(const QString &varName, const QString &value)
{m_VariablesMap[varName] = value;
}
//----------------- replaceTokenByValue --------------------------------------------------
/*! \brief Lorsque le code a ete trouve et interprete, remplace dans le texte le code par son resultat.
 *  \param document : QString du texte entier
 *  \param pos_deb : caractere de depart du remplacement
 *  \param len : longueur
 *  \param pDeltaLen ??
*/
int C_Macro_Core:: replaceTokenByValue(QString *document, int pos_deb, int len , int *pDeltaLen /*=0*/)
{int      originalLen = document->length();
 QString  resolvToken = getResolvTokenValue(document, pos_deb,  len )  ;
 resolvToken.replace("SautDeLigne", "<br/>");
 document->replace ( pos_deb-2, len+4, resolvToken);
 if (pDeltaLen) *pDeltaLen = document->length() - originalLen;
 return pos_deb-2+resolvToken.length();
}

//------------------------------ getResolvTokenValue ---------------------------------------
/*! \brief Analyse le texte scripte passe en parametre et "compile" avec les donnees calculees.
 *  \return les donnees calculees
*/
QString C_Macro_Core::getResolvTokenValue(QString*document, int pos_deb, int len )
{// char insecSpace[2]={(char)0xa0,(char)0x00};
 // QString  token = document.mid(pos_deb, len);
          //token.replace((char)0xa0," ");
          //token.replace("&gt;",">");
          //token.replace("&lt;","<");
          //token.replace("&amp;","&");
          //token.replace("&nbsp;",QChar(0xa0));

 QString  resolvToken("");
 resolvFunctionToken(resolvToken, document->mid(pos_deb, len) );
 return resolvToken;
}

//---------------------------- resolvFunctionToken ----------------------------------------
/*! \brief Interpreteur des fonctions de scripts ().
 * Les fonctions de scripts permettent de faire des actions au sein du document passe en parametre.
 * L'appel de fonctions de scripts se presente de la sorte :
 * \code {{FONCTION_DE_SCRIPT( Param1, param2 ....) }} \endcode
 * Appel la fonction exeFunction().
 * \sa exeFunction()
 */
int C_Macro_Core::resolvFunctionToken(QString &resolvToken, QString token)
{ resolvToken   = "";
  if (token[0]==':') token = token.mid(2);
  token = token.trimmed();
 //if (token[0] !='#')   C_HtmlTools::htmltoLatin1(token);
 //else                  token.remove (0, 1);

 //............ si token type {{TOKEN=value}} .................
 //             on transforme en passage d'arguments
 //             ce qu'il y a apres le  signe egal
 int pos_prem_p  =  token.indexOf('(');
 int pos_egal    = token.indexOf('=');
 if ( pos_egal  != -1 && (pos_prem_p == -1 || pos_egal < pos_prem_p) )  // VAR=oru_month   VAR=oru_monthfdfdfezrcefezez = "(""
    { pos_prem_p = pos_egal;
      token = token.left(pos_egal) + '(' +token.mid(pos_egal+1) +')';
    }
 //............ si token avec absence d'argument on en rajoute des vides.................
 if ( pos_prem_p ==-1 )
    { pos_prem_p = token.length();
      token     += "()";
    }
 int  pos_last_p =  findLastDelimiter(token, ')');
 if ( pos_last_p ==-1 ) {outMessage (QObject::tr("C_Macro_Core::ResolvScriptTokenSyntax() Syntax Error last  ')' not found in : ") + token ,__FILE__,__LINE__); return 0;}
 int ret = exeFunction(token.left(pos_prem_p).trimmed().toUpper(), token.mid(pos_prem_p + 1, pos_last_p-pos_prem_p - 1), resolvToken);
 //......... verifier si une variable de destination du resultat est indiquee ..............
 //          et auquel cas affecter la valeur a cette variable le resultat sera alors vide
 if (ret==0)
    { outMessage(QObject::tr("ERROR : <font color=#01d5ce>'%1'</font> C_Macro_Core UNKNOW INSTRUCTION ").arg(token));
      return 0;        // ne pas affecter une eventuelle variable si mot clef inconnu
    }
 QChar c = token[pos_last_p+1];
 if ( c=='>' ||
      c=='-' ||
      (c=='&'&& token[pos_last_p+2]=='g' && token[pos_last_p+3]=='t' && token[pos_last_p+4]==';')
    )  //
    { QString var_dest = "";
      if (c=='&')  var_dest = token.mid(pos_last_p+5).trimmed();
      else         var_dest = token.mid(pos_last_p+2).trimmed();
      if (var_dest.length())
         { if (c=='-') resolvToken  = resolvToken.trimmed();
           m_VariablesMap[var_dest] = resolvToken;
           resolvToken = "";
         }
    }
 return ret;
}

//-------------------------- exeFunction -------------------------------------------
int C_Macro_Core::exeFunction(QString token, const QString& args, QString &resolvToken)
{ QStringList arg_list;
  int       ret = 0;
  extractArgList(arg_list, args);
  //token = token.replace(' ','_').toUpper().remove('#');
  //................. batir la structure de comparaison .......................
  KEY_MACRO toTest;
            toTest.m_Token     = (char*)(const char*)token.toLatin1().replace(" ","_").replace( C_Macro_Core::Utf8_Insecable," " );

  //............... recuperer le tableau de fonctions correspondant a la premiere lettre du token.............................
  int i = (int)toTest.m_Token[0]-'A';
  if (i<0 || i>25)                   return ret;
  KEY_MACRO_TAB_DEF *p_MACRO_LIST = &m_MacroTab_List[i];

  //................. rechercher la fonction du token .........................
  KEY_MACRO *pKEY_MACRO = (KEY_MACRO*) bsearch(&toTest, p_MACRO_LIST->m_MacroTab, p_MACRO_LIST->m_size , sizeof(KEY_MACRO), C_Macro_Core::comparatorMacro);
  if (pKEY_MACRO)
     {resolvToken = (*(pKEY_MACRO->m_Function))(arg_list); //.replace("(","\\(").replace(")","\\)");
      ret = 1;
     }
  return ret;
}

//-------------------------- comparatorMacro -------------------------------------------
int C_Macro_Core::comparatorMacro (const void *a, const void *b)
{return strcmp(((KEY_MACRO*)a)->m_Token+1,((KEY_MACRO*)b)->m_Token+1);  // +1 car tri alphabetique deje fait sur le premier caractere.
}

//-------------------------- extractArgList -------------------------------------------
/*! \brief non documente */
long C_Macro_Core::extractArgList(QStringList &arg_list, const QString &arg_str)
{long len     = arg_str.length();
 int  pos     = 0;
 int  deb     = 0;
 QString arg  = "";
 QString argS = "";

 QMap<QString,QString>::Iterator it;
  while( pos <= len )                         //  <= car fin d'argument est aussi la fin de la chaine mais danger en cas de arg_str[pos]
    {
     //.......... pos==len est aussi la fin d'un argument comme arg_str[pos] == ',' .....................
     if ( pos==len || arg_str[pos] == ',' )   // DANGER si pos==len on suppose que le 2eme test n'est pas fait car plantage lors arg_str[pos] si pos=len
        { arg   = arg_str.mid(deb, pos-deb);  // DANGER le test de pos==len doit etre fait en premier si c'est le cas la boucle sera finie car ++pos
          argS  = arg.trimmed();
          if (   argS.startsWith("$") &&
               ! argS.startsWith("$VAR")
             )
             { it      =  m_VariablesMap.find(argS.mid(1));
               if (it !=  m_VariablesMap.end())  arg = it.value();
             }
          arg_list.append( arg );
          ++pos;                              // DANGER si pos==len le ++pos est la seule facon de sortir de while( pos <= len )
          deb = pos;
        }
     //.......... ce else de if ( pos==len ..) garanti que arg_str[pos] avec pos=len (ce qui est un plantage absolu)  ne sera pas teste  ............
     else  if ( arg_str[pos] == '\\' )        // DANGER si pos=len arg_str[pos] plante
        { pos += 2;
        }
     else
        { ++pos;
        }
    }
 return 1;
}

//----------------------------- findLastDelimiter -----------------------------------------------
/*! \brief Pour la recursivite. */
long C_Macro_Core::findLastDelimiter(const QString &str, const QChar delimiter)
{ int end = str.length();
    while (end>1)
     { --end;
       if (str[end]==delimiter) return end;
     }
    return -1;
}

