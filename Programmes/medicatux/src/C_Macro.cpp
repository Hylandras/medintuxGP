
/*********************************** C_Macro.cpp **********************************
 *                                                                                *
 *   #include "C_Macro.h"                                                         *
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

#include "C_Macro.h"
#include "CApp.h"

// #include "../../MedinTuxTools-QT4/medicabase/C_BaseMedica.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"

#include <QString>
#include <QDateTime>
#include <QObject>
#include <QDir>
#include <QProcess>
#include <QByteArray>
#include <QWebFrame>


#define TR QObject::tr

#ifdef Q_WS_WIN
   #define F_EXE ".exe"
   #include <windows.h>
#else
   #define F_EXE ""
#endif

#define PATH_APPLI     CApp::pCApp()->pathAppli()
#define PT_VAR_MAP     CApp::pCApp()->getVarMap()
#define PT_DOCUMENT    CApp::pCApp()->getDocument()
#define APP_INST       CApp::pCApp
#define DB             CApp::pCApp()->BDM_getDB

static C_Utils_Html  *G_pC_Utils_Html  = 0;        // indicateur globaux d'instance
static C_Macro       *G_pC_Macro       = 0;        // indicateur globaux d'instance
static C_BDM_PluginI *G_pC_BDM_PluginI = 0;
//-------------------------- C_Macro -------------------------------------------
C_Macro::C_Macro(QObject *parentForC_Utils_Html)
    :C_Macro_Core(G_MacroTab_List)
{G_pC_Utils_Html  = new C_Utils_Html(parentForC_Utils_Html); //C_Utils_Html(parentForC_Utils_Html);
 G_pC_Macro       = this;
 G_pC_BDM_PluginI = 0;
 m_pQWebView      = 0;
}

//-------------------------- ~C_Macro -------------------------------------------
C_Macro::~C_Macro()
{ //if (m_pC_Utils_Html) delete m_pC_Utils_Html;
  G_pC_Utils_Html = 0;
  G_pC_Macro      = 0;
}
//------------------------------------ set_BDM_Plugin -------------------------------
/*! \brief set the G_pC_BDM_PluginI global pointeur on DBM plugin
 *  \param C_BDM_PluginI *pC_BDM_PluginI pointer on C_BDM_PluginI to set
 */
void  C_Macro::set_BDM_Plugin(C_BDM_PluginI *pC_BDM_PluginI)
{G_pC_BDM_PluginI = pC_BDM_PluginI;
}

//-------------------------- setQNetworkAccessManager -------------------------------------------
void C_Macro::setQNetworkAccessManager(QNetworkAccessManager *pQNetworkAccessManager)
{if (G_pC_Utils_Html)  G_pC_Utils_Html->setQNetworkAccessManager(pQNetworkAccessManager);
}

//------------------------------------ setQProgressBar -------------------------------
/*! \brief set the QProgressBar
 *  \param pQProgressBar pointer on QProgressBar to set
 */
void C_Macro::setQProgressBar(QProgressBar *pQProgressBar)
{if (G_pC_Utils_Html)  G_pC_Utils_Html->setQProgressBar(pQProgressBar);
}

//------------------------------------ outMessage -------------------------------
/*! \brief output a message on std error out put and logwidget if defined
 *  \param mess  const QString  message to display
 *  \param file  const QString source file from where the message is display
 *  \param line  const QString source line in file from where the message is display
 *  \return message displayed
 */
void  C_Macro::outMessage( const QString &mess, const QString &file /*  = "" */ , const int line  /* = -1 */)
{if (APP_INST()!=0)  APP_INST()->outMessage(mess, file, line);   // oblige d'n passer par la car static oblige
}
//----------------------------------------------- setLogWidget ---------------------------------------------------------------------
/*! \brief  initialise class on a QTextEdit display logwidget
 *  \param  QTextEdit *logWidget                pointer on the logwidget
*/
void  C_Macro::setLogWidget(QTextEdit *logWidget)
{C_Macro_Core::setLogWidget(logWidget);
 if (G_pC_Utils_Html==0) return;
 G_pC_Utils_Html->setLogWidget(logWidget);
}

//-------------------------- getQNetworkAccessManager -------------------------------------------
QNetworkAccessManager * C_Macro::getQNetworkAccessManager()
{if (G_pC_Utils_Html==0) return 0;
 return G_pC_Utils_Html->getQNetworkAccessManager();
}

//-------------------------- u_Html -------------------------------------------
C_Utils_Html *C_Macro::u_Html()
{return G_pC_Utils_Html;
}

//-------------------------- ABS_PATH -------------------------------------------
/*! \brief non documente */
//static QString ABS_PATH             (QStringList &arg_list);
QString C_Macro::ABS_PATH(QStringList &arg_list)
{ if (arg_list.count()<1)  return TR("::ABS_PATH() argument du path a convertir manquant.");
  return resolvePath (arg_list[0]);
}

//-------------------------- NOP -------------------------------------------
/*! \brief non documente */
//static QString NOP                  (QStringList &arg_list);
QString C_Macro::NOP(QStringList &)
{ return "";
}


//-------------------------- CALC -------------------------------------------
/*! \brief non documente */
//static QString CALC                 (QStringList &arg_list);
QString C_Macro::CALC(QStringList &arg_list)
    {if (arg_list.count()<3)  return TR("::CALC() nombre d'arguments insuffisant");
     double valeur1  = arg_list[0].toDouble();
     QString op      = arg_list[1].toLower();
     double valeur2  = arg_list[2].toDouble();
     if (op == "*" || op == "x")   // teste OK
       {   return QString::number(valeur1 * valeur2);
       }
     else if (op == "/")                   // teste OK
       {   return  QString::number(valeur1 / valeur2);
       }
     else if (op == "+")                   // teste OK
       {   return QString::number(valeur1 + valeur2);
       }
     else if (op == "-")                   // teste OK
       {   return  QString::number(valeur1 - valeur2);
       }
     else if (op == "div")
       {   int div_entiere = 0;
           div_entiere = (int)(valeur1 / valeur2);
           //resultat = (valeur1 - (valeur1 % valeur2)) / valeur2;
           return QString::number(div_entiere);
       }
     else if (op == "mod")
       {   return QString::number(((int)valeur1 % (int)valeur2));
       }
     return QString::null;
    }

//-------------------------- DATECOURANTE -------------------------------------------
// DATE TIRET DATE DD-MM-YYYY CURRENT DATE DATECOURANTE DT
/*! \brief non documente */
//static QString DATECOURANTE         (QStringList &arg_list);
QString C_Macro::DATECOURANTE(QStringList &arg_list)
{QString   frm = "dd-MM-yyyy";
 if (arg_list.count()>0) frm = arg_list[0].trimmed();
 QDateTime qdt = QDateTime::currentDateTime();
 return qdt.toString ( frm ).replace('|','h');
}

//-------------------------- DATE_CALC -------------------------------------------
/*
static QString DATE_CALC            (QStringList &arg_list);
4.5.3.133 |==> {{:: DATE_CALC( date de depart , valeur, operateur,  valTyp, format date retour )}}
Variante : {{:: DTC( date de depart , valeur, operateur,  valTyp, format date retour ) }}
retourne le nombre de jours a partir de la date placee dans le parametre Numero1 et le Parametre Numero2. Le format des dates doit etre xxxxddxxxxxxMMxxxxxxyyyyxxxx (voir : Format des dates) où x peut representer n'importe quel caractere non numerique.
[0]Parametre Numero 1 (date de depart) chaine de caracteres a traiter.
[1]Parametre Numero 2 (valeur a ajouter ou soustraire) valeur a ajouter ou soustraire
[2]Parametre Numero 3 (facultatif : operateur) + - par defaut (si omis) sur +
[3]Parametre Numero 4 (facultatif : type de valeur a ajouter ou soustraire) si non precise il prend la valeur J (jours).
 SI   contient S la valeur numerique a ajouter  est en secondes.
 SI   contient J la valeur numerique a ajouter  est en jours.
 SI   contient M la valeur numerique a ajouter  est en mois.
 SI   contient A la valeur numerique a ajouter  est en annee.
[4]Parametre Numero 5 (facultatif : format du type de date a retourner) Le format des dates doit etre xxxxddxxxxxxMMxxxxxxyyyyxxxx si non precise dd-MM-yyyy (voir : Format des dates)  .
*/
/*! \brief non documente */
QString C_Macro::DATE_CALC(QStringList &arg_list)
    {int          nb = arg_list.count();
     QChar    valTyp = 'J';
     QString  format = "dd-MM-yyyy";
     if (nb==0) return TR("::DATE_CALC() 1er argument de la date manquant");
     if (nb==1) return TR("::DATE_CALC() 2eme argument valeur manquante");
     int val  = arg_list[1].toInt();                                        // [1]
     if (nb>2 && arg_list[2].trimmed()=="-") val = -val;            // [2]
     if (nb>3)
        {valTyp   = arg_list[3].toUpper()[0];
        }
     if (nb>4)  format  = arg_list[4].trimmed();                    // [4]
     QString      date  = strToIsoStrDateTime(arg_list[0]);      // Retourne une chaine de la forme 12x09xx2007 a l'heure de 12h30 sous forme 2007-09-12T12:30:00
     QDateTime      d1  = QDateTime::fromString (date, Qt::ISODate );
     switch(valTyp.toLatin1())
            {case  'S': {d1=d1.addSecs(val);   break;}
             case  'M': {d1=d1.addMonths(val); break;}
             case  'A': {d1=d1.addYears(val);  break;}
             case  'Y': {d1=d1.addYears(val);  break;}
             default  : {d1=d1.addDays(val);   break;}
            }
     return  d1.toString(format);
    }
//-------------------------- DATE_FROMNUM -------------------------------------------
/*
static QString DATE_FROMNUM         (QStringList &arg_list);
4.5.3.135 |==> {{:: DATE_FROMNUM( num,  valTyp, format date retour )}}
Variante : {{:: DTFN( num ,  valTyp, format date retour  ) }}
retourne la date et temps depuis le nombre de secondes ecoulees depuis le 01 Janvier 1970  (voir : Format des dates).
Parametre Numero 1 (nombre de secondes) chaine de caracteres a traiter soit le nombre de secondes depuis le 01 Janvier 1970.
Parametre Numero 2 (facultatif : type de valeur numerique a retourner) si non precise il prend la valeur S (secondes).
 SI   contient S           la valeur numerique est en secondes.
 SI   contient   J ou D  la valeur numerique est en jours.
 SI   contient M          la valeur numerique est en mois.
 SI   contient A ou Y   la valeur numerique est en annees.
Parametre Numero 3 (facultatif : format du type de date a retourner) Le format des dates. si non precise dd-MM-yyyy (voir : Format des dates)  .
*/
/*! \brief non documente */
QString C_Macro::DATE_FROMNUM(QStringList &arg_list)
    {int      nb    = arg_list.count();
     if (nb==0) return TR("::DATE_FROMNUM() 1er argument du nombre de secondes manquant");
     QChar valTyp     = 'S';
     QString format   = "dd-MM-yyyy";
     unsigned int val = arg_list[0].toUInt();
     if (nb>1)
        {//char *pt = (char*)(const char*)arg_list[1].toUpper();
         //valTyp   = *pt;
         valTyp   = arg_list[1].toUpper()[0];
        }
     if (nb>2) format = arg_list[2].trimmed();                  // [2]
     QDateTime     d1 = QDateTime::fromString ("1970-01-01T00:00:00", Qt::ISODate );
     switch(valTyp.toLatin1())
            {case  'S':  {d1.setTime_t(val);return d1.toString(format);}
             case  'M': return d1.addMonths((int)val).toString(format);
             case  'A': return d1.addYears((int)val).toString(format);
             case  'Y': return d1.addYears((int)val).toString(format);
             default  : return d1.addDays((int)val).toString(format);
            }
     return QString("");
    }
//-------------------------- DATE_TONUM -------------------------------------------
/*
static QString DATE_TONUM           (QStringList &arg_list);
4.5.3.134 |==> {{:: DATE_TONUM( date de depart , valTyp)}}
Variante : {{:: DTN( date de depart , valTyp ) }}
retourne le nombre de jours a partir de la date placee dans le parametre Numero1 et le Parametre Numero2. Le format des dates doit etre xxxxddxxxxxxMMxxxxxxyyyyxxxx (voir : Format des dates) où x peut representer n'importe quel caractere non numerique.
Parametre Numero 1 (date de depart) chaine de caracteres a traiter.
Parametre Numero 2 (facultatif : type de valeur numerique a retourner) si non precise il prend la valeur S (secondes).
 SI   contient S la valeur numerique a retourner est en secondes.
 SI   contient J la valeur numerique a retourner est en jours.
 SI   contient M la valeur numerique a retourner est en mois.
 SI   contient A la valeur numerique a retourner est en annee.

1970-01-01T00:00:00
*/
/*! \brief non documente */
QString C_Macro::DATE_TONUM(QStringList &arg_list)
    {int      nb    = arg_list.count();
     QChar valTyp   = 'S';
     if (nb==0) return TR("::DATE_TONUM() 1er argument de la date manquant");
     if (nb>1)
        {//char *pt = (char*)(const char*)arg_list[1].toUpper();
         //valTyp   = *pt;
         valTyp = arg_list[1].toUpper()[0];
        }
     QString date = strToIsoStrDateTime(arg_list[0]);    // Retourne une chaine de la forme 12x09xx2007 a l'heure de 12h30 sous forme 2007-09-12T12:30:00
     QDateTime d1 = QDateTime::fromString ("1970-01-01T00:00:00", Qt::ISODate );
     QDateTime d2 = QDateTime::fromString (date,                  Qt::ISODate );
     int     nbd  = d1.daysTo ( d2 );
     switch(valTyp.toLatin1())
            {case  'S': return QString::number( d2.toTime_t());
             case  'M': return QString::number( (nbd/365)*12);
             case  'A': return QString::number(  nbd/365);
             case  'Y': return QString::number(  nbd/365);
             default  : return QString::number(  nbd);
            }
     return QString("");
    }
//-------------------------- DAYSTO -------------------------------------------
/*! \brief non documente */
//static QString DAYSTO               (QStringList &arg_list);
QString C_Macro::DAYSTO(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb==0) return QString::null;
     if (nb==1) return daysTo(arg_list[0],"");
     else       return daysTo(arg_list[0], arg_list[1]);
    }

//-------------------------- ECHO -------------------------------------------
/*! \brief non documente */
QString C_Macro::ECHO(QStringList &arg_list)
{if (arg_list.count()==0) outMessage("");
 else                     outMessage(arg_list[0]);
 return QString::null;
}

//-------------------------- INDEX_OF -------------------------------------------
/*! \brief return the index of a text patern.
 *  \param arg_list  QStringList& arguments list where :
 *   arg_list[0] string where find text patern. if is VAR=varName the string is the varName content.
 *   arg_list[1] text patern
 *   arg_list[2] number from where begin find, default value is zero if not precised
 *  \return index if patern finded in not -1
 */

QString C_Macro::INDEX_OF(QStringList &arg_list)
{ int from       = 0;
  if (arg_list.count()<1) return TR("::INDEX_OF() argument de la chaine a evaluer manquant.");
  if (arg_list.count()<2) return TR("::INDEX_OF() argument de la chaine a trouver manquant.");
  if (arg_list.count()>2) from = arg_list[2].toInt();

  int pos          = -1;
  QString str = arg_list[0];
  if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
     {QString var    = str.mid(pos+1).trimmed();
      return QString::number( (*PT_VAR_MAP)[var].indexOf(arg_list[1],from) );
     }
  return QString::number(str.indexOf(arg_list[1],from));
}
//-------------------------- LENGTH -------------------------------------------
/*! \brief return the length of a text.
 *  \param arg_list  QStringList& arguments list where :
 *   arg_list[0] string to evaluate. if is VAR=varName the string is the varName content.
 *  \return length
 */
QString C_Macro::LENGTH(QStringList &arg_list)
{  if (arg_list.count()<1) return TR("::LENGTH() argument de la chaine a evaluer manquant (string).");
    int pos          = -1;
    QString str = arg_list[0];
    if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
       {QString var = str.mid(pos+1).trimmed();
        (*PT_VAR_MAP)[var] = QString::number((*PT_VAR_MAP)[var].length());
        return QString::null;
       }
  return QString::number(str.length());
}
//-------------------------- CUT -------------------------------------------
/*! \brief cut or remove a part of text.
 *  \param arg_list  QStringList& arguments list where :
 *   arg_list[0] string where operation must be do. If is VAR=varName the string is the varName content.
 *   arg_list[1] index from where begin the part to remove.
 *   arg_list[2] number of caracters to remove.
 *   arg_list[3] if is "IS_POSITION"  arg_list[2] is not number of caracters to remove, but end position to remove
 *  \return string modified if action not in VAR, null if action is in VAR.
 */
QString C_Macro::CUT(QStringList &arg_list)
{   if (arg_list.count()<1) return TR("::CUT() argument 'string' de la chaine a evaluer manquant (string, from, len).");
    if (arg_list.count()<2) return TR("::CUT() argument 'from'   de la position du premier caractere a retirer manquant (string, from, len).");
    if (arg_list.count()<3) return TR("::CUT() argument 'len'    quantite de caracteres a effacer manquant (string, from, len).");
    int deb   = arg_list[1].trimmed().toInt();
    int len   = arg_list[2].trimmed().toInt();
    int isPos = 0;
    if (arg_list.count()>=4)
       { if (arg_list[3].trimmed()=="IS_POSITION") isPos=1;
       }
    if (isPos) len -= deb;
    int pos         = -1;
    QString str     = arg_list[0];
    if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
       {QString var = str.mid(pos+1).trimmed();
        (*PT_VAR_MAP)[var] = (*PT_VAR_MAP)[var].remove(arg_list[1].toInt(), arg_list[2].toInt());
        return QString::null;
       }
  return str.remove(arg_list[1].toInt(), arg_list[2].toInt());
}
//-------------------------- MID -------------------------------------------
/*! \brief non documente */
QString C_Macro::MID(QStringList &arg_list)
{    int nb = arg_list.count();
     if (nb<2)   return TR("::MID() nombre d'arguments insuffisant (string, position, len = '~')");
     int pos          = -1;
     QString str = arg_list[0];
     if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
        {QString var = str.mid(pos+1).trimmed();
         if (nb==2 ) (*PT_VAR_MAP)[var] = (*PT_VAR_MAP)[var].mid(arg_list[1].toInt());
         else        (*PT_VAR_MAP)[var] = (*PT_VAR_MAP)[var].mid(arg_list[1].toInt(),arg_list[2].toInt());
         return QString::null;
        }
     if (nb==2 ) return str.mid(arg_list[1].toInt());
     return str.mid(arg_list[1].toInt(), arg_list[2].toInt());
}

//-------------------------- LEFT -------------------------------------------
/*! \brief non documente */
QString C_Macro::LEFT(QStringList &arg_list)
{   if (arg_list.count()<2) return TR("::LEFT() nombre d'arguments insuffisant (string, position)");
    QString str = arg_list[0];
    int     pos = -1;
    if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
       {QString var = str.mid(pos+1).trimmed();
        (*PT_VAR_MAP)[var] = (*PT_VAR_MAP)[var].left(arg_list[1].toInt());
        return QString::null;
       }
    return str.left(arg_list[1].toInt());
}

//-------------------------- RIGHT -------------------------------------------
/*! \brief non documente */
QString C_Macro::RIGHT(QStringList &arg_list)
{   if (arg_list.count()<2) return TR("::RIGHT() nombre d'arguments insuffisant (string, position)");
    QString str = arg_list[0];
    int     pos = -1;
    if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
       {QString var = str.mid(pos+1).trimmed();
        (*PT_VAR_MAP)[var] = (*PT_VAR_MAP)[var].right(arg_list[1].toInt());
        return QString::null;
       }
    return str.right(arg_list[1].toInt());
}

//-------------------------- SIMPLIFY -------------------------------------------
QString C_Macro::SIMPLIFY(QStringList &arg_list)
{ int          nb = arg_list.count();
  if (nb < 1) return TR("::SIMPLIFY() 1er argument manquant (string, toRemove=' ')");
  QString   str      = arg_list[0];
  QString   toRemove = " ";
  int            pos = -1;
  if (nb>1) toRemove = arg_list[1];
  if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
     {QString var        = str.mid(pos+1).trimmed();
      (*PT_VAR_MAP)[var] = (*PT_VAR_MAP)[var].remove(toRemove);
      return QString::null;
     }
  return str.remove(toRemove);
}

//-------------------------- TRIMMED -------------------------------------------
QString C_Macro::TRIMMED(QStringList &arg_list)
{      if ( arg_list.count()==0) return TR("::TRIMMED() argument de la chaine a evaluer manquant (string).");
       QString str = arg_list[0];
       int     pos = -1;
       if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
          {QString var        = str.mid(pos+1).trimmed();
           (*PT_VAR_MAP)[var] = (*PT_VAR_MAP)[var].trimmed();
           return QString::null;
          }
       return str.trimmed();
}

//-------------------------- UPPER -------------------------------------------
/*! \brief non documente */
QString C_Macro::UPPER(QStringList &arg_list)
    {if ( arg_list.count()==0) return TR("::UPPER() argument de la chaine a evaluer manquant (string).");
     QString str = arg_list[0];
     int     pos = -1;
     if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
        {QString var        = str.mid(pos+1).trimmed();
         (*PT_VAR_MAP)[var] = (*PT_VAR_MAP)[var].toUpper();
         return QString::null;
        }
     return str.toUpper();
    }

//-------------------------- LOWER -------------------------------------------
/*! \brief non documente */
QString C_Macro::LOWER(QStringList &arg_list)
    {if ( arg_list.count()==0) return TR("::LOWER() argument de la chaine a evaluer manquant (string).");
     QString str = arg_list[0];
     int     pos = -1;
     if (str.startsWith("VAR", Qt::CaseInsensitive) && (pos = str.indexOf('=')) != -1)
        {QString var = str.mid(pos+1).trimmed();
         (*PT_VAR_MAP)[var] = (*PT_VAR_MAP)[var].toLower();
         return QString::null;
        }
     return str.toLower();
    }

//-------------------------- TEST -------------------------------------------
// TEST (S,PATIENT,>=,ADRESSE,TESTISTRUE,TESTISFALSE, 1)
/*! \brief non documente */
QString C_Macro::TEST(QStringList &arg_list)
    {if ( arg_list.count() <= 5 )  return TR("Erreur de syntaxe ::TEST() cette fonction doit comporter  au moins 5 arguments ");
     if ( arg_list.count() <= 6 )  arg_list.append("");
     QString op =  arg_list[2].trimmed();
     int    iop = 0;
     Qt::CaseSensitivity cs = Qt::CaseSensitive;
     if      (op=="=" )  {iop = 1; }
     else if (op==">" )  {iop = 2; }
     else if (op=="<" )  {iop = 3; }
     else if (op==">=")  {iop = 4; }
     else if (op=="<=")  {iop = 5; }
     else if (op=="!=")  {iop = 6; }
     else if (op=="%=")  {iop = 7; }
     else if (op=="=%")  {iop = 8; }
     else if (op=="%%")  {iop = 9; }

     QString val1 = arg_list[1].trimmed();
     QString val2 = arg_list[3].trimmed();
     if (arg_list.count() == 7) cs = Qt::CaseInsensitive;
     if (arg_list[0][0]=='N') // on teste les valeurs numeriques et si pas ok on continue mais avec les textes
        {double v1 = C_Utils_Html::toNum(val1).toDouble();
         double v2 = C_Utils_Html::toNum(val2).toDouble();
         switch (iop)
               {
                case 1:     // =
                    {if (v1==v2 ) return arg_list[4].trimmed();
                     else         return arg_list[5].trimmed();
                    }
                case 4:     // >=  si egal  ok
                    {if (v1==v2 ) return arg_list[4].trimmed();
                    }
                case 2:     // >
                    {if (v1>v2)  return arg_list[4].trimmed();
                     else        return arg_list[5].trimmed();
                    }
                case 5:     // <=  si egal  ok
                    {if (v1==v2 ) return arg_list[4].trimmed();
                    }
                case 3:     // <
                    {if (v1<v2)  return arg_list[4].trimmed();
                     else        return arg_list[5].trimmed();
                    }
                case 6:     // !=
                    {if (v1!=v2 ) return arg_list[4].trimmed();
                     else         return arg_list[5].trimmed();
                    }
               }
       }
     switch (iop)
               {case 1:     // =
                    {if (val1==val2 ) return arg_list[4].trimmed();
                     else             return arg_list[5].trimmed();
                    }
                case 4:     // >=  si egal  ok
                    {if (val1==val2 ) return arg_list[4].trimmed();
                    }
                case 2:     // >
                    {if (QString::compare ( val1, val2 )>0)  return arg_list[4].trimmed();
                     else                                    return arg_list[5].trimmed();
                    }
                case 5:     // <=  si egal  ok
                    {if (val1==val2 ) return arg_list[4].trimmed();
                    }
                case 3:     // <
                    {if (QString::compare ( val1, val2 )<0)  return arg_list[4].trimmed();
                     else                                    return arg_list[5].trimmed();
                    }
                case 6:     // !=
                    {if ( !(val1==val2) ) return arg_list[4].trimmed();
                     else                 return arg_list[5].trimmed();
                    }
                case 7:     // %=
                    {if (val1.startsWith(val2,cs))   return arg_list[4].trimmed();
                     else                            return arg_list[5].trimmed();
                    }
                case 8:    // =%
                    {if (val1.endsWith(val2,cs))  return arg_list[4].trimmed();
                     else                         return arg_list[5].trimmed();
                    }
                case 9:    // %%
                    {if (val1.contains(val2,cs)) return arg_list[4].trimmed();
                     else                        return arg_list[5].trimmed();
                    }
               } // endswitch (op)
    return QString::null;
    }

//-------------------------- EXE_PROCESS -------------------------------------------
/*! \brief non documente */
// static QString EXE_PROCESS          (QStringList &arg_list);
QString C_Macro::EXE_PROCESS(QStringList &arg_list)
    {//............. recupere les arguments passes a la fonction ................
     bool      addExe = true;
     bool   isCommand = false;
     int           nb = arg_list.count();
     if (nb < 2) return TR("ERREUR C_Macro::EXE_PROCESS() nombre d'arguments insuffisant.  (waitMode, process name , arg_1 , arg_2 , ..arg_n)");
     QString pathExe = arg_list[1].trimmed();
     QString mode    = arg_list[0].trimmed().toLower();            // waitEnd ou pass
     if ( pathExe.indexOf("%IsAbsPath") != -1 )
        {  pathExe    = pathExe.remove("%IsAbsPath").trimmed();
           isCommand  = true;       // ne pas derelativiser (une commande n'a pas de path)
        }
     else if ( QDir::isRelativePath ( pathExe ) )
        { pathExe  = QDir::cleanPath(pathExe.prepend(PATH_APPLI) );
        }
     if ( pathExe.indexOf("%IsBinPath") != -1 )
        { pathExe  = pathExe.remove("%IsBinPath").trimmed();
        }
     if (pathExe.indexOf("%NoAddExe") != -1)
        { addExe   = false;
          pathExe  = pathExe.remove("%NoAddExe");
        }
     #ifdef Q_WS_MAC
     else
        {if (addExe)   pathExe  +=  ".app/Contents/MacOS/" + QFileInfo(pathExe).baseName() ;                   // 0  Chemin de l'executable plugin a actionner
        }
     #else
         if (addExe)   pathExe  +=  F_EXE;                 // 0  Chemin de l'executable plugin a actionner
     #endif
     //................... les arguments .......................
     QStringList argList;
     QString stdInArg = "";
     for (int i=2; i<nb; ++i)
         {QString argument = resolvePathKeyWord(arg_list[i]);
          if (argument.startsWith("%toStdIn"))
             { stdInArg=argument.mid(8);
               loadContentsIfBeginByTokenFile(stdInArg);
             }
          else
             { loadContentsIfBeginByTokenFile(argument);
               argList << argument.replace("[%File]","%File");  // un argument n'est pas toujours un chemin
             }
         }
     //................. si process n'existe pas cassos ................
     if (isCommand==false &&  QFile::exists(pathExe ))        // une commande n'a pas a trouver son chemin car c'est le systeme qui le fait
        {return TR("ERREUR C_Macro::EXE_PROCESS() Le processus : <b>'%1'</b> n'a pas \303\251t\303\251 trouv\303\251.").arg(pathExe);
        }
     //................. on lance le process  ................
     QProcess proc;
     proc.start(pathExe, argList);
     if (!proc.waitForStarted()) return TR("ERREUR C_Macro::EXE_PROCESS() Le processus : <b>'%1'</b> n'a pas \303\251t\303\251 lanc\303\251.").arg(pathExe);
     if (stdInArg.length())
        { proc.write(stdInArg.toLatin1());
          proc.closeWriteChannel();
        }
     if (mode == "waitend")           // on doit attendre la fin du process
        { if (proc.waitForFinished(-1)==false) return TR("ERREUR C_Macro::EXE_PROCESS() Le processus : <b>'%1'</b> n'a pas termin\303\251 normalement.").arg(pathExe);
          return QString(proc.readAll());
        }
     else                             // on doit pas attendre la fin du process
        { return QString("");
        }
   }

//-------------------------- EXTRACT -------------------------------------------
/*! \brief non documente */
QString C_Macro::EXTRACT(QStringList &arg_list)
{if (PT_DOCUMENT==0) return TR("::EXTRACT() le pointeur du document est null ");
 int nb  = arg_list.count();
 int pos = -1;
 if (nb<5) for( int i=nb; i<=6; ++i) arg_list.append(""); //  completer la liste si < 5
 QString document = arg_list[0].trimmed();
 if (document.startsWith("VAR", Qt::CaseInsensitive) && (pos = document.indexOf('=')) != -1)
    { QString var =  document.mid(pos+1).trimmed();
      return func_Extract((*PT_VAR_MAP)[var], arg_list[1]        ,   arg_list[2]      ,      arg_list[3],      arg_list[4]   ,        arg_list[5]);
    }
 else //if (document=="THIS")
    { return func_Extract(*PT_DOCUMENT, arg_list[1]        ,   arg_list[2]      ,      arg_list[3],      arg_list[4]   ,        arg_list[5]);
    }
 return QString::null;
}

//-------------------------- REPLACE -------------------------------------------
/*! \brief non documente */
QString C_Macro::REPLACE(QStringList &arg_list)
{if (PT_DOCUMENT==0) return TR("::REPLACE() le pointeur du document est null ");
 int nb  = arg_list.count();
 int pos = -1;
 if (nb<3) return TR("::REPLACE() nombre d'arguments insuffisant : (target, toFind, replaceBy) ");
 QString target      = arg_list[0].trimmed();
 QString toFind      = C_Utils_Html::unProtectSymbolByAntiSlash(arg_list[1].trimmed());
 QString replaceBy   = C_Utils_Html::unProtectSymbolByAntiSlash(arg_list[2].trimmed());

 if (target.startsWith("VAR", Qt::CaseInsensitive) && (pos = target.indexOf('=')) != -1)
    { QString var =  target.mid(pos+1).trimmed();
     (*PT_VAR_MAP)[var].replace( toFind, replaceBy );
     return QString::null;
    }
 else if (target.toUpper()=="$THIS")
    { PT_DOCUMENT->replace( toFind, replaceBy );
      return QString::null;
    }
 if (toFind=="__SP__") toFind = " ";
 return target.replace( toFind, replaceBy );
}

//-------------------------- LOAD_URL_IN_VAR -------------------------------------------
QString C_Macro::LOAD_URL_IN_VAR(QStringList &arg_list)
{if (u_Html() == 0)        return TR("::LOAD_URL_IN_VAR() m_pC_Utils_Html est null et non instanci\303\251 ");
 int nb  = arg_list.count();
 if (nb<2)                 return TR("::LOAD_URL_IN_VAR() nombre d'arguments insuffisant : (var_name, url)");
 QString var_name = arg_list[0].trimmed();
 QString url      = C_Utils_Html::unProtectSymbolByAntiSlash(arg_list[1].trimmed());
 QStringList rawHeadersList;
 for (int i=2; i<nb;++i) rawHeadersList << C_Utils_Html::unProtectSymbolByAntiSlash(arg_list[i].trimmed());
 if (url.startsWith("file:"))
    {(*PT_VAR_MAP)[var_name] = CGestIni::Param_UpdateFromDisk(resolvePath (url.remove("file:").trimmed()));
    }
 if (url.startsWith("Monographie:"))
    {url = url.remove("Monographie:").trimmed();
     QString data = "";
     if (G_pC_BDM_PluginI)
        { data                    =   G_pC_BDM_PluginI->getMonographieFromCIP(url);
          (*PT_VAR_MAP)[var_name] = data;
        }
    //  if (APP_INST()->datasemp()) (*PT_VAR_MAP)[var_name] = APP_INST()->GetMonographieFromCIP(url);
    }
 else
    {QByteArray ba = u_Html()->getDataFromUrl(url, rawHeadersList, u_Html()->getQNetworkAccessManager());
     if (CGestIni::IsUtf8(&ba))
         PT_VAR_MAP->insert (var_name, QString::fromUtf8(ba.data(), ba.size()));
     else
         PT_VAR_MAP->insert (var_name, u_Html()->getDataFromUrl(url, rawHeadersList, u_Html()->getQNetworkAccessManager()));
    }
 // CGestIni::Param_UpdateToDisk( QDir::homePath()+"/LOAD_URL_IN_VAR_URL.html", url);
 // CGestIni::Param_UpdateToDisk( QDir::homePath()+"/LOAD_URL_IN_VAR.html", (*PT_VAR_MAP)[var_name]);
 return QString::null;
}
//-------------------------- LOAD_URL -------------------------------------------
QString C_Macro::LOAD_URL(QStringList &arg_list)
{if (u_Html() == 0)        return TR("::LOAD_URL() m_pC_Utils_Html est null et non instanci\303\251 ");
 int nb  = arg_list.count();
 if (nb<1)                 return TR("::LOAD_URL() nombre d'arguments insuffisant : (url, rawHeader-1, rawHeader-2, ...., rawHeader-n)");
 QString url      = C_Utils_Html::unProtectSymbolByAntiSlash(arg_list[0].trimmed());
 QStringList rawHeadersList;
 for (int i=1; i<nb;++i) rawHeadersList << C_Utils_Html::unProtectSymbolByAntiSlash(arg_list[i].trimmed());
 QString ret = "";
 if (url.startsWith("file:"))
    {ret = CGestIni::Param_UpdateFromDisk(resolvePath (url.remove("file:").trimmed()));
    }
 else if (url.startsWith("Monographie:"))
    {url = url.remove("Monographie:").trimmed();
     if (G_pC_BDM_PluginI)
        { ret  =  G_pC_BDM_PluginI->getMonographieFromCIP(url);
        }
    //  if (APP_INST()->datasemp()) (*PT_VAR_MAP)[var_name] = APP_INST()->GetMonographieFromCIP(url);
    }
 else
    {QByteArray ba = u_Html()->getDataFromUrl(url, rawHeadersList, u_Html()->getQNetworkAccessManager());
     if (CGestIni::IsUtf8(&ba))
         ret  = QString::fromUtf8(ba.data(), ba.size());
     else
         ret  = ba;   // u_Html()->getDataFromUrl(url, rawHeadersList, u_Html()->getQNetworkAccessManager());
    }
 //CGestIni::Param_UpdateToDisk( QDir::homePath()+"/LOAD_URL_URL.html",  url);
 //CGestIni::Param_UpdateToDisk( QDir::homePath()+"/LOAD_URL.html",      ret);
 return ret;
}
//-------------------------- POST_DATA -------------------------------------------
QString C_Macro::POST_DATA(QStringList &arg_list)
{if (u_Html() == 0)        return TR("::POST_DATA() m_pC_Utils_Html est null et non instanci\303\251 ");
 int nb  = arg_list.count();
 int pos = -1;
 if (nb<2)                 return TR("::POST_DATA() nombre d'arguments insuffisant : (dataToPost, url)");
 QString dataToPost   = arg_list[0].trimmed();
 QString url          = C_Utils_Html::unProtectSymbolByAntiSlash(arg_list[1].trimmed());
 QStringList rawHeadersList;
 for (int i=2; i<nb;++i) rawHeadersList << arg_list[i];
 //request.setRawHeader("User-Agent", "Free-for-all");
 if (dataToPost.startsWith("VAR", Qt::CaseInsensitive) && (pos = dataToPost.indexOf('=')) != -1)
     u_Html()->postDataFromUrl(url, (*PT_VAR_MAP)[dataToPost.mid(pos+1).trimmed()], rawHeadersList, u_Html()->getQNetworkAccessManager());
 else
     u_Html()->postDataFromUrl(url, dataToPost, rawHeadersList, u_Html()->getQNetworkAccessManager());
 return QString::null;
}

//-------------------------- DISPLAY_PAGE -------------------------------------------
QString C_Macro::DISPLAY_PAGE(QStringList &arg_list)
{if (pQWebView() == 0)        return TR("::DISPLAY_PAGE() m_pQWebView est null et non instanci\303\251 ");
 int nb           = arg_list.count();
 int pos          = -1;
 if (nb<2)                    return TR("::DISPLAY_PAGE() nombre d'arguments insuffisant : (dataToDisplay, baseUrl)");
 QString dataToDisplay   = arg_list[0].trimmed();
 QString baseUrl         = arg_list[1].trimmed();
 if (!baseUrl.startsWith("http:", Qt::CaseInsensitive))                                   // on est en local
    {baseUrl = APP_INST()->resolvePath(baseUrl);
     #ifdef Q_WS_WIN
     #else
     if (!baseUrl.startsWith("file:", Qt::CaseInsensitive))    baseUrl.prepend("file:");  // rajouter la mention precisant que l'url est en local
     if (!baseUrl.endsWith('/')) baseUrl += "/";
     #endif
    }
 APP_INST()->setEndPageDisplay(false);
 if (!baseUrl.endsWith('/')) baseUrl += '/';
 if (dataToDisplay.startsWith("VAR", Qt::CaseInsensitive) && (pos = dataToDisplay.indexOf('=')) != -1)
    { //if (!baseUrl.endsWith('/')) baseUrl += '/';
      dataToDisplay = (*PT_VAR_MAP)[dataToDisplay.mid(pos+1).trimmed()];
      //pQWebView()->setHtml((*PT_VAR_MAP)[dataToDisplay.mid(pos+1).trimmed()], QUrl(baseUrl));
      //CGestIni::Param_UpdateToDisk("/home/ro/htmlGet.html",dataToDisplay);
    }

 if (dataToDisplay.startsWith("http://"))
    { pQWebView()->load(dataToDisplay);                           // c'est une QString qui est une adresse html
    }
 else
    { pQWebView()->setHtml(dataToDisplay, QUrl(baseUrl));        // c'est une QString qui est un contenu html
    }
 long timeout = 500000;
 if (nb>=3) timeout = arg_list[2].trimmed().toLong();
 APP_INST()->waitForEndPageDisplay(timeout,  pQWebView());
 //long timeIn = timeout;
 //outMessage(TR("DISPLAY_PAGE() time out : %1 out for: %2").arg(timeIn).arg(APP_INST()->waitForEndPageDisplay(timeout,  pQWebView())));

 return QString::null;
}

//-------------------------- GET_DISPLAY_PAGE -------------------------------------------
QString C_Macro::GET_DISPLAY_PAGE(QStringList &arg_list)
{if (pQWebView() == 0)        return TR("::GET_DISPLAY_PAGE() m_pQWebView est null et non instanci\303\251 ");
 int nb             = arg_list.count();
 int pos            = -1;
 QString   var_name = "";
 if (nb>0) var_name = arg_list[0].trimmed();

 if (var_name.startsWith("VAR", Qt::CaseInsensitive) && (pos = var_name.indexOf('=')) != -1)
    {var_name = var_name.mid(pos+1).trimmed();
    }

 QWebFrame *pMainQWebFrame = pQWebView()->page()->mainFrame();
 //outMessage(QString("mainFrame : '%1'").arg(pMainQWebFrame->url().toString()));
 QString html              = pMainQWebFrame->toHtml();
 QList<QWebFrame *>     fl = pMainQWebFrame->childFrames();
 for (int i = 0; i < fl.size(); ++i)
     {QWebFrame *pQWebFrame =  fl.at(i);
      //outMessage(QString("subFrame : '%1'").arg(pQWebFrame->url().toString()));
      html += pQWebFrame->toHtml();
     }
 if (var_name.length()==0) return html;
 (*PT_VAR_MAP)[var_name] = html;
 return QString::null;
}

//-------------------------- EXE_JAVA_FUNC -------------------------------------------
QString C_Macro::EXE_JAVA_FUNC(QStringList &arg_list)
{if (pQWebView() == 0)        return TR("::EXE_JAVA_FUNC() m_pQWebView est null et non instanci\303\251 ");
 int nb       = arg_list.count();
 if (nb<1)                    return TR("::EXE_JAVA_FUNC() nombre d'arguments insuffisant : (java to evaluate)");
 QString arg  = arg_list[0].remove("\\");
 long timeout = 30000;
 if (nb>=2) timeout = arg_list[1].trimmed().toLong();
 APP_INST()->setEndPageDisplay(false);
 outMessage(TR("EXE_JAVA_FUNC() start evaluateJavaScript() time out : %1").arg(QString::number(timeout)));
 QTime tmd = QTime::currentTime();
 QTimer::singleShot ( timeout, pQWebView(), SLOT(stop()) );
 pQWebView()->page()->mainFrame()->evaluateJavaScript(arg);
 QTime tme = QTime::currentTime();
 QString tms = QString::number(tmd.msecsTo ( tme )).rightJustified(8,'0');
 outMessage(TR("EXE_JAVA_FUNC() end evaluateJavaScript() since <font color=#00c0ff>%1</font> time out : <b>%2</b>").arg(tms.insert(tms.length()-3,'.'), QString::number(timeout)));

 if (nb>=3) timeout = arg_list[2].trimmed().toLong();
 outMessage(TR("EXE_JAVA_FUNC() start waitForEndPageDisplay() initial time out : <font color=#00c0ff>%1</font>").arg(QString::number(timeout)));
 tmd     = QTime::currentTime();
 timeout = APP_INST()->waitForEndPageDisplay(timeout,  pQWebView() );
 tme     = QTime::currentTime();
 tms     = QString::number(tmd.msecsTo ( tme )).rightJustified(8,'0');
 outMessage(TR("EXE_JAVA_FUNC() end waitForEndPageDisplay() since <font color=#00c0ff>%1</font> time out : <b>%2</b>").arg(tms.insert(tms.length()-3,'.'), QString::number(timeout)));
 return QString::null;
}

//-------------------------- PUT_PAGE_IN_BASE -------------------------------------------
//$htmlPage,{{DCI1}},Vidal Mono For DCI, htmlPage,http://www.vidal.fr/recherche/index/q:{{DCI1}}
QString C_Macro::PUT_PAGE_IN_BASE(QStringList &arg_list)
{if (pQWebView() == 0)        return TR("::PUT_PAGE_IN_BASE() m_pQWebView est null et non instanci\303\251 ");
 if (u_Html() == 0)           return TR("::PUT_PAGE_IN_BASE() u_Html() est null et non instanci\303\251 ");
 int nb       = arg_list.count();
 if (nb<4)                    return TR("::PUT_PAGE_IN_BASE() nombre d'arguments insuffisant : (page, id, biblioType, biblioUrl)");
 QString varName = "";
 //................ recuperer la page ............................
 //                 soit directement dans                              arg_list[0]
 //                 soit dans la variable VAR = nomDeLaVariable    si arg_list[0] commence par VAR =
 QString   page     = arg_list[0];
 int            pos = -1;
 if (page.startsWith("VAR", Qt::CaseInsensitive) && (pos = page.indexOf('=')) != -1)
    {varName = page.mid(pos+1).trimmed();
     page    = (*PT_VAR_MAP)[varName];
    }
 QString id           = arg_list[1].trimmed();
 QString biblioType   = arg_list[2].trimmed();
 QString biblioUrl    = arg_list[3].trimmed();
 //QString biblioLogin  = arg_list[4].trimmed();
 //QString biblioPassw  = arg_list[5].trimmed();

 //................ creer le repertoire local des images ...............................
 QString imageDir    = APP_INST()->pathAppli() + "Ressources/web/" + biblioType;
 if ( !QDir(imageDir).exists() ) QDir().mkdir ( imageDir );
 imageDir += "/";
 //................ telecharger les ccs,images, js and cie de la page en local ............................................
 u_Html()->saveImagesFromTag(page, biblioUrl, imageDir, "img",    "src");
 u_Html()->saveImagesFromTag(page, biblioUrl, imageDir, "td",     "background");
 u_Html()->saveImagesFromTag(page, biblioUrl, imageDir, "link",   "href");
 u_Html()->saveImagesFromTag(page, biblioUrl, imageDir, "script", "src");
 //................ re referencer les ccs,images, js and cie en local ...............................
 u_Html()->setTagAttributeValue(  page, "img",     "src",        "./" , C_Utils_Html::SetAllJustDir);
 u_Html()->setTagAttributeValue(  page, "td",      "background", "./" , C_Utils_Html::SetAllJustDir);
 u_Html()->setTagAttributeValue(  page, "link",    "href",       "./" , C_Utils_Html::SetAllJustDir);
 u_Html()->setTagAttributeValue(  page, "script",  "src",        "./" , C_Utils_Html::SetAllJustDir);

// CGestIni::Param_UpdateToDisk( QDir::homePath()+"/PUT_PAGE_IN_BASE.html",  imageDir);
// CGestIni::Param_UpdateToDisk( QDir::homePath()+"/PUT_PAGE_IN_BASE.html",      page);

 //........................ chercher enregistrement biblio ..............................
 if (varName.length()) (*PT_VAR_MAP)[varName] = page;
 //                         pour ce type de medoc
 /*
 C_BiblioData cb;
 cb   = DB()->biblio_getDataFrom_TypeLangId(id, "fr", biblioType);
 cb.m_data         = page;              // placer la page dans les donnees
 if (cb.m_pk.length())                  //........... Si l'enregistrement existe deja .......
    {//outMessage( TR("PUT_PAGE_IN_BASE() cb.m_mime = <b>'%1'</b> ").arg(cb.m_mime));
     DB()->biblio_UpdateRecord(cb.m_pk,
                               cb.m_id,
                               cb.m_owner,
                               cb.m_lang,
                               cb.m_url,
                               QDate::currentDate ().toString("dd-MM-yyyy"),
                               cb.m_note,
                               biblioType,
                               cb.m_mime,
                               cb.m_data);
    }
 //....................... pas de biblio pour ce topic ...................................
 //                        faut le creer
 else
   { DB()->biblio_setLink_Page(id, biblioType,  biblioUrl, "AF", cb.m_data);  // enregistrer dans le cache
   }
 */
 //outMessage( TR("Page mise en base avec ID = <b>'%1'</b>  Type = <b>'%2'</b>").arg(id, biblioType));
 return QString::null;
}

//-------------------------- OS -------------------------------------------
/*! \brief non documente */
QString C_Macro::OS(QStringList &)
     {
       #ifdef Q_OS_MSDOS
            return QString("W");
       #endif
       #ifdef Q_OS_LINUX
            return QString("L");
       #endif
       #ifdef Q_OS_MAC
            return QString("M");
       #endif
     }

//-------------------------- PUT_VAR -------------------------------------------
/*! \brief non documente */
QString C_Macro::PUT_VAR(QStringList &arg_list)
{int nb = arg_list.count();
 QString fileName = "";
 QString var      = (*PT_VAR_MAP)[arg_list[0].trimmed()];
 if (PT_VAR_MAP==0)      return TR("::PUT_VAR() pointeur du tableau de variables null ");
 if (nb<1)               return TR("::PUT_VAR() nom de la variable non indique");
 if (nb>1)               // argument de modificateurs
    {if (arg_list[1].contains("%commaProtect"))  var = C_Utils_Html::protectSymbolByAntiSlash(var,',');
     if (arg_list[1].contains("%braketProtect")) var = C_Utils_Html::protectSymbolByAntiSlash(C_Utils_Html::protectSymbolByAntiSlash(var,'('),')');
    }
 if (nb>2)               // argument d'un nom de fichier
    {fileName = resolvePath (arg_list[2]);
     CGestIni::Param_UpdateToDisk(fileName, var);
     return QString("");
    }
 return var;
}

//-------------------------- SET_VAR -------------------------------------------
/*! \brief non documente */
QString C_Macro::SET_VAR(QStringList &arg_list)
    {if (PT_VAR_MAP==0)      return TR("::SET_VAR() pointeur du tableau de variables null ");
     int nb = arg_list.count();
     if (nb==0)  return QString::null;
     if (nb==1)  arg_list.append("");
     QString name  = arg_list[0].trimmed();
     QString val   = arg_list[1].trimmed();
     if (val.startsWith("%File"))
        {QString outParam;
         CGestIni::Param_UpdateFromDisk(resolvePath(val.mid(5).trimmed()), outParam);
         PT_VAR_MAP->insert (name, outParam);
        }
 //    else if (val.startsWith("%Selection"))
 //       {QStringList tmpLst; tmpLst.append(name);
 //        GET_SELECTION(tmpLst);
 //       }
     else
        {PT_VAR_MAP->insert (name, val);
        }
     return QString("");
    }

//-------------------------- TO_NUM -------------------------------------------
QString C_Macro::TO_NUM(QStringList &arg_list)
{int nb                 = arg_list.count();
 if (nb<1) return TR("::TO_NUM() 1er argument manquant");
 QString    cutFirstStr = "";
 if (nb>1)  cutFirstStr = arg_list[1];
 return C_Utils_Html::toNum(arg_list[0], cutFirstStr);
}

//-------------------------- VAR_LIST_CLEAR -------------------------------------------
/*! \brief non documente */
QString C_Macro::VAR_LIST_CLEAR(QStringList &)
    {if (PT_VAR_MAP==0)      return TR("::VAR_LIST_CLEAR() pointeur du tableau de variables null ");
     PT_VAR_MAP->clear();
     return QString("");
    }
//-------------------------- VAR_SPLIT -------------------------------------------
/*! \brief non documente */
QString C_Macro::VAR_SPLIT(QStringList &arg_list)
    {if (PT_VAR_MAP==0)      return TR("::VAR_SPLIT() pointeur du tableau de variables null ");
     int nb = arg_list.count();
     if (nb<2)  return TR("Erreur VAR_SPLIT() nombre d'arguments insuffisants" );
     QString  prefix   = arg_list[0];
     QString  toSplit  = arg_list[1];
     QString    sep    = "|";                // valeur par defaut
     QString    flag   = "";
     if (nb>=3) sep    = arg_list[2];
     if (nb>=4) flag   = arg_list[3];

     sep     = sep.replace("\\\\","-*@*-");     sep.remove("\\");     sep     = sep.replace("-*@*-","\\\\");
     toSplit = toSplit.replace("\\\\","-*@*-"); toSplit.remove("\\"); toSplit = toSplit.replace("-*@*-","\\\\");

     QStringList listeSegments;
     if (nb>=4 && arg_list[3].trimmed()=="FALSE") listeSegments = toSplit.split(sep, QString::SkipEmptyParts);
     else                                         listeSegments = toSplit.split(sep, QString::KeepEmptyParts);

     for ( int i = 0; i<listeSegments.count(); ++i )
         { if (flag.contains("%commaProtect"))
               PT_VAR_MAP->insert (prefix+QString::number(i), C_Utils_Html::protectSymbolByAntiSlash(listeSegments[i],','));
           else
               PT_VAR_MAP->insert (prefix+QString::number(i), listeSegments[i]);
         }
     return QString("");
    }

/*
static QString DO_LISTE             (QStringList &arg_list);
static QString EXIT                 (QStringList &arg_list);
static QString EXTRACT              (QStringList &arg_list);
static QString FILE_EXISTS          (QStringList &arg_list);
static QString FILL                 (QStringList &arg_list);
static QString GET_SAVE_FILE_NAME   (QStringList &arg_list);
static QString GET_SELECTION        (QStringList &arg_list);
static QString HHHMM                (QStringList &arg_list);
static QString IMPORT               (QStringList &arg_list);
static QString INCLUDE              (QStringList &arg_list);
static QString INDEX_OF             (QStringList &arg_list);

static QString INSERT_IMAGE         (QStringList &arg_list);
static QString LEFT                 (QStringList &arg_list);
static QString LENGTH               (QStringList &arg_list);
static QString LIST_DIR             (QStringList &arg_list);
static QString LOWER                (QStringList &arg_list);
static QString MID                  (QStringList &arg_list);
static QString OS                   (QStringList &arg_list);
static QString PUT_VAR              (QStringList &arg_list);
static QString REPLACE_BY           (QStringList &arg_list);
static QString REPLACE_IN_FILE      (QStringList &arg_list);
static QString RIGHT                (QStringList &arg_list);
static QString SET_VAR              (QStringList &arg_list);
static QString TEST                 (QStringList &arg_list);
static QString SIMPLIFY             (QStringList &arg_list);
static QString TO_NUM               (QStringList &arg_list);
static QString UPPER                (QStringList &arg_list);
static QString VAR_LIST_CLEAR       (QStringList &arg_list);
static QString VAR_SPLIT            (QStringList &arg_list);
*/
//--------------------------- func_Extract ---------------------------------------------------
/*! \brief Extrait une partie du texte du document selon le modele demande */
QString C_Macro::func_Extract(const QString &text, QString &patern_deb, QString &patern_end, QString &find_to, QString &replace_by, const QString justNum_in)
{if (patern_deb.length()==0 || patern_end.length()==0) return QString::null;
 QString   resolvToken    = "";
 int      end_patern_deb  = 0;
 int      deb_Patern_end  = 0;
 QString      justNum     = justNum_in.trimmed();
 patern_deb = patern_deb.trimmed(); patern_deb=patern_deb.replace("\\\\","-*@*-");patern_deb.remove("\\"); patern_deb=patern_deb.replace("-*@*-","\\\\");
 patern_end = patern_end.trimmed(); patern_end=patern_end.replace("\\\\","-*@*-");patern_end.remove("\\"); patern_end=patern_end.replace("-*@*-","\\\\");
 find_to    = find_to.trimmed();
 replace_by = replace_by.trimmed();
 //CGestIni::Param_UpdateToDisk("/home/ro/data.txt",text);
 if (justNum.contains("%text"))
    {justNum.remove("%text"); justNum = justNum.trimmed();
     if ( (end_patern_deb = text.indexOf(patern_deb)) != -1)
        {end_patern_deb  += patern_deb.length();
         if ( (deb_Patern_end = text.indexOf(patern_end, end_patern_deb)) != -1)
            {resolvToken =  text.mid(end_patern_deb, deb_Patern_end - end_patern_deb);
            }
        }
     if (justNum.contains("%commaProtect"))
        {justNum.remove("%commaProtect"); justNum = justNum.trimmed();
         resolvToken = C_Utils_Html::protectSymbolByAntiSlash(resolvToken, ',');
        }
     if (justNum.length()) return  C_Utils_Html::toNum(resolvToken, justNum);
     else                  return  resolvToken;
    }
 if ( (end_patern_deb = C_Utils_Html::htmlFind(text, patern_deb)) != -1)            // trouver la fin de la balise de debut
    {C_Utils_Html::htmlFind(text, patern_end, end_patern_deb, &deb_Patern_end);     // trouver le debut de la balise de fin
     if (deb_Patern_end!=-1)
        {resolvToken =  text.mid(end_patern_deb, deb_Patern_end - end_patern_deb);
         if (justNum.indexOf("%keepHtml") == -1){C_Utils_Html::htmlToAscii(resolvToken);}
         else                                   {justNum = justNum.remove("%keepHtml");}
         if ( find_to.length() )
            {if ( resolvToken.indexOf(find_to, true) != -1) resolvToken = replace_by;
             else                                           resolvToken = "";
            }
         resolvToken.remove('\r');                 // pour les puristes...resolvToken=resolvToken.stripWhiteSpace();
         resolvToken.replace("\n\n","<p>");        // Remplace les sauts de lignes ASCII par les sauts de ligne HTML
         resolvToken.replace("\n",  "<br>");       // Remplace les sauts de lignes ASCII par les sauts de ligne HTML
        }
     }
 if (justNum.length()) return  C_Utils_Html::toNum(resolvToken, justNum);
 else                  return  resolvToken;
}

//------------------------------ pQWebView --------------------------------------------------
/*! \brief retourne la QWebView destinee a afficher et recevoir les pages internet.
 *  \return  QWebView *destinee a afficher et recevoir les pages internet.
*/

QWebView *C_Macro::pQWebView()
{if (APP_INST()==0) return 0;
 return APP_INST()->getQWebView();
}

//------------------------------ loadContentsIfBeginByTokenFile --------------------------------------------------
/*! \brief Teste si la chaine arg commence par %File et si c'est le cas la charge avec le contenu du fichier.
 *  \param  QString& arg : argument a evaluer et eventuellement charger avec le contenu d'un fichier
*/
QString C_Macro::loadContentsIfBeginByTokenFile(QString &arg)
{return APP_INST()->loadContentsIfBeginByTokenFile(arg);
}

/*------------------------------ resolvePath --------------------------------------------------------------------*/
/*! \brief resoud un chemin comportant les eventuelles macros %Glossaire %Vigie %ToAbsPath %EndToAbsPath par le chemin absolu
 *  \param pathFile :  QString chemin d'entree
 *  \return Renvoie le chemin modifie absolutise si necessaire
*/
QString C_Macro::resolvePath(QString pathFile)
{ return APP_INST()->resolvePath(pathFile);
}
/*------------------------------ resolvePathKeyWord --------------------------------------------------------------------*/
/*! \brief resoud un chemin comportant les eventuelles macros %Glossaire %Vigie %ToAbsPath %EndToAbsPath par le chemin absolu
 *  \param pathFile :  QString chemin d'entree
 *  \return Renvoie le chemin modifie absolutise que lors des mots clefs et UNIQUEMENT DANS CE CAS (utile lors arguments de programmes externes)
*/
QString C_Macro::resolvePathKeyWord(QString pathFile)
{ return APP_INST()->resolvePathKeyWord(pathFile);
}
