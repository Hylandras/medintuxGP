
/*********************************** C_TokenInterpret.cpp ***************************
 *                                                                                *
 *   #include "CDevilCrucible.h"                                                  *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *   Eric MAEKER 2007 2008 eric.maeker@free.fr                                    *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
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

#include "C_TokenInterpret.h"
#include "CDevilCrucible.h"
#include "CApp.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "ui/DLG_InsertList.h"
#include "../../MedinTuxTools/C_Organiseur.h"
#include "../../MedinTuxTools/CGenTools.h"
#include "../../MedinTuxTools/CMedicaBase.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "ui/Dlg_Calendar.h"

#include "MyEditText.h"
#include "CMDI_Generic.h"
#include "CDevilGraph.h"
#include "CDevilCalc.h"
#include "CRubList.h"
#include "CMoteur_Base.h"
#include "Rubriques.h"
#include "C_Dlg_Pendule.h"

#include "drtux.h"
#include <qprocess.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qprocess.h>
#include <qmime.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qeventloop.h>
#include <qthread.h>
#include <qfiledialog.h>
#include <stdlib.h>
#include <qrect.h>
#include <qtextcodec.h>

#ifdef Q_WS_WIN
   #define F_EXE ".exe"
   #include <windows.h>
#else
   #define F_EXE ""
#endif

#define TR QObject::tr
static  CDevilCrucible        *G_mCDC = 0;
static QString            G_PluginRun = "";
typedef                   QMap<QString, QStringList*> FAMILLE_GENRE_MAP;
//====================================== C_TokenInterpret ==============================================================
//-------------------------- C_TokenInterpret -------------------------------------------
C_TokenInterpret::C_TokenInterpret()
{

}
//-------------------------- ~C_TokenInterpret -------------------------------------------
C_TokenInterpret::~C_TokenInterpret()
{
}

//-------------------------- init -------------------------------------------
void C_TokenInterpret::init(CDevilCrucible *pCDevil)
{G_mCDC = pCDevil;
}
//---------------------------- ResolvScriptTokenSyntax ----------------------------------------
/*! \brief Interpreteur des fonctions de scripts ().
 * Les fonctions de scripts permettent de faire des actions au sein du document passe en paramètre.
 * L'appel de fonctions de scripts se presente de la sorte :
 * \code {{:: FONCTION_DE_SCRIPT ( Param1, param2 ....) }} \endcode
 * Appel la fonction exeFunction().
 * \sa exeFunction()
 */
int C_TokenInterpret::ResolvMacroToken(CDevilCrucible *pCDC, QString &resolvToken, QString token)
{ CDevilCrucible *sav_pCDC = G_mCDC;    // sauvegarder pour assurer eventuelle recursivite
  G_mCDC = pCDC;
  //............. separer eventuels argumens du token .................
  QStringList arg_list;
  int       ret = 0;
  resolvToken   = "";
  int       pos = token.find('=');
  if (pos != -1)
     {arg_list.append(token.mid(pos+1));
      token = token.left(pos).stripWhiteSpace();
     }
  token = token.replace(' ','_').upper();

  //................. batir la structure de comparaison .......................
  KEY_MACRO toTest;
            toTest.m_Token     = (char*)(const char*)token;
  //        toTest.m_Function  = 0;

  //............... recuperer le tableau de fonctions correspondant à la première lettre du token.............................
  int i = (int)toTest.m_Token[0]-'A';
  if (i<0 || i>25)                   return ret;
  KEY_MACRO_TAB_DEF *p_MACRO_LIST = &G_MacroTab_List[i];

  //................. rechercher la fonction du token ...............................................................
  KEY_MACRO *pKEY_MACRO = (KEY_MACRO*) bsearch(&toTest, p_MACRO_LIST->m_MacroTab, p_MACRO_LIST->m_size , sizeof(KEY_MACRO), C_TokenInterpret::comparatorMacro);
  if (pKEY_MACRO)
     {resolvToken = (*(pKEY_MACRO->m_Function))(arg_list); //.replace("(","\\(").replace(")","\\)");
      ret = 1;
     }
 G_mCDC  = sav_pCDC;
 return ret;
}

//-------------------------- exeFunction -------------------------------------------
int C_TokenInterpret::exeFunction(QString token, const QString& args, QString &resolvToken)
{ QStringList arg_list;
  int       ret = 0;
  extractArgList(arg_list, args);
  token = token.replace(' ','_').upper();
  //................. batir la structure de comparaison .......................
  KEY_MACRO toTest;
            toTest.m_Token     = (char*)(const char*)token;

  //............... recuperer le tableau de fonctions correspondant à la première lettre du token.............................
  int i = (int)toTest.m_Token[0]-'A';
  if (i<0 || i>25)                   return ret;
  KEY_MACRO_TAB_DEF *p_MACRO_LIST = &G_MacroTab_List[i];

  //................. rechercher la fonction du token .........................
  KEY_MACRO *pKEY_MACRO = (KEY_MACRO*) bsearch(&toTest, p_MACRO_LIST->m_MacroTab, p_MACRO_LIST->m_size , sizeof(KEY_MACRO), C_TokenInterpret::comparatorMacro);
  if (pKEY_MACRO)
     {resolvToken = (*(pKEY_MACRO->m_Function))(arg_list); //.replace("(","\\(").replace(")","\\)");
      ret = 1;
     }
  return ret;
}

//---------------------------- ResolvScriptTokenSyntax ----------------------------------------
/*! \brief Interpreteur des fonctions de scripts ().
 * Les fonctions de scripts permettent de faire des actions au sein du document passe en paramètre.
 * L'appel de fonctions de scripts se presente de la sorte :
 * \code {{:: FONCTION_DE_SCRIPT ( Param1, param2 ....) }} \endcode
 * Appel la fonction exeFunction().
 * \sa exeFunction()
 */
int C_TokenInterpret::ResolvFunctionToken(CDevilCrucible *pCDC, QString &resolvToken, QString token)
{CDevilCrucible *sav_pCDC = G_mCDC;    // sauvegarder pour assurer eventuelle recursivite
                   G_mCDC = pCDC;
 resolvToken   = "";
 if (token[0] !='#')   CHtmlTools::HtmlToAscii(token);
 else                  token.remove (0, 1);
 int pos_prem_p =  token.find('(');
 if (pos_prem_p ==-1)  {resolvToken = TR("C_TokenInterpret::ResolvScriptTokenSyntax rififi in syntax first open bracket not found in : ")  + token; return 0;}
 int pos_last_p =  findLastDelimiter(token, ')');   // part de la fin donc meme si ( ou ) entre les deux cela devrait le faire
 if (pos_last_p ==-1)  {resolvToken = TR("C_TokenInterpret::ResolvScriptTokenSyntax rififi in syntax last close bracket not found in : ")  + token; return 0;}
 int ret = exeFunction(token.left(pos_prem_p).stripWhiteSpace().upper(), token.mid(pos_prem_p + 1, pos_last_p-pos_prem_p - 1), resolvToken);
 //......... verifier si une variable de destination du resultat est indiquee ..............
 //          et auquel cas affecter la valeur a cette variable le resultat sera alors vide
 if (ret==0) return 0;        // ne pas affecter une eventuelle variable si mot clef inconnu
 QChar c = token[pos_last_p+1];
 if (c=='>'||c=='-')
    { QString var_dest = token.mid(pos_last_p+2);
      if (var_dest.length()) 
         { if (c=='-') resolvToken = resolvToken.stripWhiteSpace();
           (*G_mCDC->m_pVariables)[var_dest] = resolvToken; 
           resolvToken = "";
         }
    }
 G_mCDC  = sav_pCDC;
 return ret;
}

//-------------------------- comparatorMacro -------------------------------------------
int C_TokenInterpret::comparatorMacro (const void *a, const void *b)
{//KEY_MACRO *pA =  (KEY_MACRO*)a;
 //KEY_MACRO *pB =  (KEY_MACRO*)b;
 //return strcmp(pA->m_Token, pB->m_Token);
 return strcmp(((KEY_MACRO*)a)->m_Token+1,((KEY_MACRO*)b)->m_Token+1);  // +1 car tri alphaberique dejà fait sur le premier caractère.
}


///////////////////////////// FONCTIONS CALL BACK /////////////////////////////////////////////
//-------------------------- ABS_PATH -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::ABS_PATH(QStringList &arg_list)
{ if (arg_list.count()<1) return TR("::ABS_PATH() argument du path à convertir manquant.");
  return G_pCApp->resolvePath(arg_list[0]);
}
//-------------------------- APPEND_DOC_IN_CLASSEUR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::APPEND_DOC_IN_CLASSEUR(QStringList &arg_list)
{ if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
  if (G_mCDC->m_pCRubCurrentRecord)
     {if (G_pCApp->m_pDrTux->m_pDock_Organiseur)
         {return G_pCApp->m_pDrTux->m_pC_Organiseur->Append_Doc_In_Hierarchie(G_mCDC->m_pCRubCurrentRecord, arg_list[0], arg_list[1].toInt());
         }
      }
  return QString("");
}

//-------------------------- BASE_THERAPEUTIQUE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::BASE_THERAPEUTIQUE(QStringList &)
{  G_pCApp->m_pDrTux->LancerVidal(10);
   return QString("");
}

//-------------------------- CALC -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::CALC(QStringList &arg_list)
    {int nb = arg_list.count();
     QString ret = "";
     if (nb<4) //  completer la liste si < 5
        {int i;
         for( i=nb; i<=3; ++i) arg_list.append("");
        }
     CDevilCalc *pCalc = new CDevilCalc(G_mCDC);
     if (pCalc)
        {ret =  pCalc->Calc(arg_list[0], arg_list[1], arg_list[2]);
         delete pCalc;
        }
     return ret;
    }
//-------------------------- VAR_CALC -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::VAR_CALC(QStringList &arg_list)
{int                   nb     = arg_list.count();
 int                 op       = -1;
 int                 op_i     = -1;
 double              ret_d    = 0.0;
 QString             arg      = "";
 int                 isArgOp  = 0;
 static QString     operators = "+-*/:%?xX";
 if (nb<=0)           return "";

 for (int i=0; i<nb; ++i)
     {arg     = arg_list[i].stripWhiteSpace();
      //if (arg.startsWith("$"))
      //   {arg = (*G_mCDC->m_pVariables)[arg.mid(1)];
      //   }
      isArgOp = 0;
      if (arg.length()==1)  
         {op = operators.find(arg);
          if (op != -1 ) 
             {isArgOp = 1;         // l'argument en cours est un operateur
              op_i    = op;        // on note l'operateur
             }
         }
      // si l'argument en cours n'est pas un operateur soit on est au premier tour 
      // auquel cas il n'y a pas d'operateur encore valide (op_i==-1) 
      // alors et on affecte le resultat avec le contenu de arg 
      if (isArgOp==0)
         {
          switch (op_i)
           {case 0:     // +   addition
                 ret_d += arg.toDouble();
                 break;
            case 1:     // -   soustraction
                 ret_d -= arg.toDouble();
                 break;
            case 2:     // *   multiplication
                 ret_d *= arg.toDouble();
                 break;
            case 3:     // /   division flottante
                 ret_d /= arg.toDouble();
                 break;
            case 4:     // :   division entière
                 ret_d = ((int)ret_d / arg.toInt());
                 break;
            case 5:     // %   pourcentage
                 ret_d = (ret_d*arg.toDouble())/100;
                 break;
            case 6:     // ?   modulo
                 ret_d = (int)ret_d - (((int)ret_d /arg.toInt())*(int)ret_d);
                 break;
            case 7:     // x   multiplication entière 
            case 8:     // X   multiplication entière 
                 ret_d = ((int)ret_d * arg.toInt());
                 break;
            default:
                 ret_d = arg.toDouble();
           }
         }
     }
 return QString::number(ret_d);
}

//-------------------------- CADRE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::CADRE(QStringList &arg_list)
{int deb_width       = -1;
 int end_width       = -1;
 int nb              = arg_list.count();
 if (nb<=6)           return QString::null;
 QString width       = arg_list[0].stripWhiteSpace();
 QString border      = arg_list[1].stripWhiteSpace();
 QString cellpadding = arg_list[2].stripWhiteSpace();
 QString cellspacing = arg_list[3].stripWhiteSpace();
 QString justif      = arg_list[4].stripWhiteSpace();
 QString rnTobr      = arg_list[5].stripWhiteSpace();
 //<p align="right">
 //...................................... definition du cadre global ..................................................
 QString ret =QString("</p><p align=\"") + justif + QString("\"><table border=%1 cellpadding=%2 cellspacing=%3 width=%4 >\r\n<tr>").arg(border,cellpadding,cellspacing,width);
 width = QString::number(width.toInt()/(nb-6));
 for (int i=6; i<nb; ++i)
     {QString cadreStr = arg_list[i].remove("\r").replace("\n",rnTobr);
      if ( (deb_width  = cadreStr.find("$WIDTH")) !=-1)
         {deb_width += 6;
          if ( (end_width  = cadreStr.find("$END_WIDTH",deb_width )) !=-1)
             {width        = cadreStr.mid(deb_width,end_width-deb_width).stripWhiteSpace();
              cadreStr     = cadreStr.remove(deb_width-6,end_width-deb_width+16);
             }
         }
      ret += QString("<td width=%1>\r\n").arg(width);
      ret += cadreStr;
      ret += "</td>";
     }
 ret += "</tr></table></p><p>";
 return ret;
}

//-------------------------- COPY_FILE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::COPY_FILE(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");

     QString ret = "";
     int      nb = arg_list.count();
     if (nb<1)          return TR("::copy_file() 1er  argument du nom du fichier source manquant");
     if (nb<2)          return TR("::copy_file() 2ème argument du nom de fichier ou repertoire de destination manquant");
     QString            action   = "";
     QString            imageSrc = G_pCApp->resolvePath(arg_list[0]);
     QString            imageDst = G_pCApp->resolvePath(arg_list[1]);
     QString            imgName  = QFileInfo(imageSrc).fileName();
     if (nb>=3)         action   = arg_list[2].stripWhiteSpace();

     if ( !QFileInfo(imageDst).isDir ()) imgName = QFileInfo(imageDst).fileName();   // si le fichier de destination n'est pas un repertoire en extraire le nom du fichier de destination

     CHtmlTools::Copy_File(imageSrc, QFileInfo(imageDst).dirPath(), &imgName);
     if (action.find("remove_src")!= -1) QDir().remove(imageSrc);
     return ret;
    }
//-------------------------- MID -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::MID(QStringList &arg_list)
{    int nb = arg_list.count();
     if (nb<2) return TR("::MID() nombre d'arguments insuffisants");
     if (nb==2 ) return arg_list[0].mid(arg_list[1].toInt());
     return arg_list[0].mid(arg_list[1].toInt(), arg_list[2].toInt());
}

//-------------------------- LEFT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::LEFT(QStringList &arg_list)
{    if (arg_list.count()<2) return TR("::LEFT() nombre d'arguments insuffisants");
     return arg_list[0].left(arg_list[1].toInt());
}
//-------------------------- RIGHT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::RIGHT(QStringList &arg_list)
{    if (arg_list.count()<2) return TR("::RIGHT() nombre d'arguments insuffisants");
     return arg_list[0].mid(arg_list[1].toInt());
}

//-------------------------- DO_LISTE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::DO_LISTE(QStringList &arg_list)
    {//if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     QString ret("");
     QString modeSel("");
     QString sep;
     QString titre         = TR("Choisissez un item de la liste");
     int nbFieldsToDisplay = -1;
     int nb                = arg_list.count();
     if (nb<1) return TR("::Do_Liste() 1er  argument ( liste des items) manquant");
     if (nb>=2)              titre = arg_list[1];
     if (nb>=3)                sep = arg_list[2];
     else                      sep = "|";
     if (nb>=5)  nbFieldsToDisplay = arg_list[4].stripWhiteSpace().toInt();
     if (nb>=6)            modeSel = arg_list[5].stripWhiteSpace();
     // if (nb>=4)      nomVar  = arg_list[3];

     if (sep=="EOL")        {arg_list[0] = arg_list[0].remove("\r").replace("\n","#|)^"); sep = "#|)^";}  // si un mec met ca c'est qu'il est fele
     QStringList fieldList   = QStringList::split(sep,arg_list[0]);
     if (sep=="#|)^")    sep = "\n";
     if (fieldList.count()<=0)      return ret;
     Dlg_InsertList *dlg = new Dlg_InsertList(0, "InsertList", TRUE);
     if (dlg == 0)                  return TR("::Do_Liste() le dialogue n'a pas pu etre cree");
     QListBox *pQListBox         = dlg->listBox_InsertList;
     dlg->setCaption(titre);
     dlg->initDialogField( fieldList, nbFieldsToDisplay, modeSel);
     if (modeSel=="M") pQListBox->setSelectionMode ( QListBox::Extended ) ;
     if (dlg->exec() != QDialog::Accepted)
        {if (nb>=4) G_mCDC->m_pVariables->insert (arg_list[3].stripWhiteSpace(),"Rejected");
         delete dlg;
         return ret;
        }
     if (nb>=4) G_mCDC->m_pVariables->insert (arg_list[3].stripWhiteSpace(),"Accepted");
     //................. iterer la listbox pour chercher les selectionnes .......................
     int                       i = 0;
     nb                          = pQListBox->count();
     while (i<nb)
         {if ( pQListBox->isSelected (i) )
             {ret +=  fieldList[i];
              ++i;
              ret += sep;
             }
          else ++i;
         }
     i =        ret.findRev(sep);
     if (i!=-1) ret.truncate(i);
     delete dlg;
     return ret;
     delete dlg;
     return ret;
    }

//-------------------------- DATE_CALC -------------------------------------------
/*
4.5.3.133 |==> {{:: DATE_CALC( date de depart , valeur, operateur,  valTyp, format date retour )}}
Variante : {{:: DTC( date de depart , valeur, operateur,  valTyp, format date retour ) }}
retourne le nombre de jours à partir de la date placee dans le parametre N°1 et le Paramètre N°2. Le format des dates doit être xxxxddxxxxxxMMxxxxxxyyyyxxxx (voir : Format des dates) où x peut representer n'importe quel caractère non numerique.
[0]Paramètre N° 1 (date de depart) chaîne de caractères à traiter.
[1]Paramètre N° 2 (valeur à ajouter ou soustraire) valeur à ajouter ou soustraire
[2]Paramètre N° 3 (facultatif : operateur) + - par defaut (si omis) sur +
[3]Paramètre N° 4 (facultatif : type de valeur à ajouter ou soustraire) si non precise il prend la valeur J (jours).
 SI   contient S la valeur numerique à ajouter  est en secondes.
 SI   contient J la valeur numerique à ajouter  est en jours.
 SI   contient M la valeur numerique à ajouter  est en mois.
 SI   contient A la valeur numerique à ajouter  est en annee.
[4]Paramètre N° 5 (facultatif : format du type de date à retourner) Le format des dates doit être xxxxddxxxxxxMMxxxxxxyyyyxxxx si non precise dd-MM-yyyy (voir : Format des dates)  .
*/
/*! \brief non documente */
QString C_TokenInterpret::DATE_CALC(QStringList &arg_list)
    {int          nb = arg_list.count();
     char    valTyp = 'J';
     QString format = "dd-MM-yyyy";
     if (nb==0) return TR("::DATE_CALC() 1er argument de la date manquant");
     if (nb==1) return TR("::DATE_CALC() 2eme argument valeur manquante");
     int val  = arg_list[1].toInt();                                        // [1]
     if (nb>2 && arg_list[2].stripWhiteSpace()=="-") val = -val;            // [2]
     if (nb>3)
        {char *pt = (char*)(const char*)arg_list[3].upper();                // [3]
         valTyp   = *pt;
        }
     if (nb>4)  format  = arg_list[4].stripWhiteSpace();                    // [4]
     QString      date  = CGenTools::NormaliseDateTimeToIso(arg_list[0]);      // Retourne une chaine de la forme 12x09xx2007 à l'heure de 12h30 sous forme 2007-09-12T12:30:00
     QDateTime      d1  = QDateTime::fromString (date, Qt::ISODate );
     switch(valTyp)
            {case  'S': {d1=d1.addSecs(val);   break;}
             case  'M': {d1=d1.addMonths(val); break;}
             case  'A': {d1=d1.addYears(val);  break;}
             case  'Y': {d1=d1.addYears(val);  break;}
             default  : {d1=d1.addDays(val);   break;}
            }
     return  d1.toString(format);
    }
//-------------------------- DAYSTO -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::DAYSTO(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb==3)
        {
        }
     if (nb==0) return QString::null;
     if (nb==1) return CGenTools::daysTo(arg_list[0],"");
     else       return CGenTools::daysTo(arg_list[0], arg_list[1]);
    }
//-------------------------- DATE_TONUM -------------------------------------------
/*
4.5.3.134 |==> {{:: DATE_TONUM( date de depart , valTyp)}}
Variante : {{:: DTN( date de depart , valTyp ) }}
retourne le nombre de jours à partir de la date placee dans le parametre N°1 et le Paramètre N°2. Le format des dates doit être xxxxddxxxxxxMMxxxxxxyyyyxxxx (voir : Format des dates) où x peut representer n'importe quel caractère non numerique.
Paramètre N° 1 (date de depart) chaîne de caractères à traiter.
Paramètre N° 2 (facultatif : type de valeur numerique à retourner) si non precise il prend la valeur S (secondes).
 SI   contient S la valeur numerique à retourner est en secondes.
 SI   contient J la valeur numerique à retourner est en jours.
 SI   contient M la valeur numerique à retourner est en mois.
 SI   contient A la valeur numerique à retourner est en annee.

1970-01-01T00:00:00
*/
/*! \brief non documente */
QString C_TokenInterpret::DATE_TONUM(QStringList &arg_list)
    {int      nb    = arg_list.count();
     char valTyp    = 'S';
     if (nb==0) return TR("::DATE_TONUM() 1er argument de la date manquant");
     if (nb>1)
        {char *pt = (char*)(const char*)arg_list[1].upper();
         valTyp   = *pt;
        }
     QString dateTime2 = "1970-01-01T00:00:00";                             // Retourne une chaine de la forme 12x09xx2007 à l'heure de 12h30 sous forme 2007-09-12T12:30:00
     if (nb>2)
        {dateTime2 = CGenTools::NormaliseDateTimeToIso(arg_list[2]);
        }
     QString dateTime1 = CGenTools::NormaliseDateTimeToIso(arg_list[0]);    // Retourne une chaine de la forme 12x09xx2007 à l'heure de 12h30 sous forme 2007-09-12T12:30:00
     QDateTime d1      = QDateTime::fromString (dateTime1,  Qt::ISODate );
     QDateTime d2      = QDateTime::fromString (dateTime2,  Qt::ISODate );
     int val           = 0;
     switch(valTyp)
            {case  'S': val = d2.secsTo (d1);          break;
             case  'M': val = (d2.daysTo (d1)/365)*12; break;
             case  'A': val = d2.daysTo (d1)/365;      break;
             case  'Y': val = d2.daysTo (d1)/365;      break;
             default  : val = d2.daysTo (d1);          break;
            }
     return QString::number(val);
    }
//-------------------------- DATE_FROMNUM -------------------------------------------
/*
4.5.3.135 |==> {{:: DATE_FROMNUM( num,  valTyp, format date retour )}}
Variante : {{:: DTFN( num ,  valTyp, format date retour  ) }}
retourne la date et temps depuis le nombre de secondes ecoulees depuis le 01 Janvier 1970  (voir : Format des dates).
Paramètre N° 1 (nombre de secondes) chaîne de caractères à traiter soit le nombre de secondes depuis le 01 Janvier 1970.
Paramètre N° 2 (facultatif : type de valeur numerique à retourner) si non precise il prend la valeur S (secondes).
 SI   contient S           la valeur numerique est en secondes.
 SI   contient   J ou D  la valeur numerique est en jours.
 SI   contient M          la valeur numerique est en mois.
 SI   contient A ou Y   la valeur numerique est en annees.
Paramètre N° 3 (facultatif : format du type de date à retourner) Le format des dates. si non precise dd-MM-yyyy (voir : Format des dates)  .
*/
/*! \brief non documente */
QString C_TokenInterpret::DATE_FROMNUM(QStringList &arg_list)
    {int      nb      = arg_list.count();
     if (nb==0) return TR("::DATE_FROMNUM() 1er argument du nombre de secondes manquant");
     char valTyp      = 'S';
     QString format   = "dd-MM-yyyy";
     unsigned int val = arg_list[0].toUInt();
     if (nb>1)
        {char *pt = (char*)(const char*)arg_list[1].upper();
         valTyp   = *pt;
        }
     if (nb>2) format    = arg_list[2].stripWhiteSpace();                  // [2]
     QString dateTime1   = "1970-01-01T00:00:00";
     if (nb>3) dateTime1 = CGenTools::NormaliseDateTimeToIso(arg_list[3]);
     QDateTime        d1 = QDateTime::fromString (dateTime1, Qt::ISODate );
     switch(valTyp)
            {case  'S':  {if (nb>3) d1.addSecs((int)val);
                          else      d1.setTime_t(val);
                          return d1.toString(format);
                         }
             case  'M': return d1.addMonths((int)val).toString(format);
             case  'A': return d1.addYears((int)val).toString(format);
             case  'Y': return d1.addYears((int)val).toString(format);
             default  : return d1.addDays((int)val).toString(format);
            }
     return QString("");
    }

//-------------------------- DELETE_FILE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::DELETE_FILE(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");

     QString    ret = "";
     if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return ret;
     QString action = "";
     int         nb = arg_list.count();
     if (nb<1)          return TR("::delete_file() 1er argument du nom du fichier source manquant");
     QString            path = G_pCApp->resolvePath(arg_list[0]);
     if (nb>1)        action = arg_list[1].stripWhiteSpace();
     if (action.length() && action.find("clear") != -1)
        {QFile qfile(path);
         if (qfile.open(IO_WriteOnly)==TRUE)
            {qfile.close();
            }
        }
     else
        {QDir().remove(path);
        }
     return ret;
    }

//-------------------------- EXIT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EXIT(QStringList &)
{if (G_pCApp->m_pDrTux) QTimer::singleShot ( 21, G_pCApp->m_pDrTux, SLOT(OnMenuQuitterDrTux()) );   // executer le script de fin
 return QString::null;
}

//-------------------------- EXE_PROCESS -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EXE_PROCESS(QStringList &arg_list)
    {//............. recupere les arguments passes a la fonction ................
     QString ret      = "";
     QString arg      = "";
     bool      addExe = TRUE;
     //QString debug = "";
     bool   isComm = FALSE;
     int        nb = arg_list.count();
     if (nb>=2) //  completer la liste si < 5
        {int i;
         QProcess proc(0,"CDevilCrucibleProcess");
         QString pathExe = arg_list[1].stripWhiteSpace();

         QString mode    = arg_list[0].stripWhiteSpace().lower();            // waitEnd ou pass
         if ( pathExe.find("$IsAbsPath") != -1 )
           {  pathExe  = pathExe.remove("$IsAbsPath").stripWhiteSpace();
              isComm   = TRUE;
           }
         else if ( QDir::isRelativePath ( pathExe ) )
           {  pathExe  = QDir::cleanDirPath (pathExe.prepend(G_pCApp->m_PathAppli) );
           }
         if ( pathExe.find("$IsBinPath") != -1 )
            {pathExe   = pathExe.remove("$IsBinPath").stripWhiteSpace();
            }
         if (pathExe.find("$NoAddExe") != -1)
            {addExe  = FALSE;
             pathExe = pathExe.remove("$NoAddExe");
            }
         #ifdef Q_WS_MAC
         else
            {if (addExe)   pathExe  +=  ".app/Contents/MacOS/" + QFileInfo(pathExe).baseName() ;                   // 0  Chemin de l'executable plugin à actionner
            }
         #else
             if (addExe)   pathExe  +=  F_EXT;                 // 0  Chemin de l'executable plugin à actionner
         #endif
         QString stdInArg = "";
         if ( isComm || QFile::exists(pathExe ) )
            {   proc.addArgument( pathExe );                   // 0  Chemin de l'executable plugin a actionner
                //debug  += pathExe + "\r\n";
                for( i=2; i<nb; ++i)
                   {//    QMessageBox::information ( 0, QString("Before N° : %1").arg(QString::number(i)), arg_list[i],"ok" );
                    //    QString argument        = CGenTools::resolvePathKeyWord(arg_list[i], G_pCApp->m_PathAppli, G_pCApp->m_PathGlossaire ); // any Unicode text
                    //    QTextCodec       *codec = QTextCodec::codecForName("ISO8859-1"); // get the codec for KOI8-R
                    //    QCString locallyEncoded = codec->fromUnicode( argument );
                    QString argument = CGenTools::resolvePathKeyWord(arg_list[i], G_pCApp->m_PathAppli, G_pCApp->m_PathGlossaire );
                    if (argument.startsWith("$toStdIn"))
                       {stdInArg=argument.mid(8);
                        G_pCApp->loadContentsIfBeginByTokenFile(stdInArg);
                       }
                    else
                       {G_pCApp->loadContentsIfBeginByTokenFile(stdInArg);
                        proc.addArgument( argument.replace("[$File]","$File")  ); // un argument n'est pas toujours un chemin
                       }
                    //proc.addArgument( CGenTools::resolvePathKeyWord(arg_list[i], G_pCApp->m_PathAppli, G_pCApp->m_PathGlossaire )  ); // un argument n'est pas toujours un chemin

                    //   QMessageBox::information ( 0, QString("After N° : %1").arg(QString::number(i)), locallyEncoded,"ok" );
                   }
                if ( G_PluginRun == "" && proc.start() )
                   { G_PluginRun = pathExe;
                     SLEEP(1);
                     G_pCApp->processEvents ();
                     if (stdInArg.length()) proc.writeToStdin ( stdInArg.replace("[$File]","$File") );
                     while (mode == "waitend" && proc.isRunning ())
                        {
                          SLEEP(1);
                          QTimer timer;               // pour etre sur qu'un evenement se produise
                          timer.start(100, FALSE);    // sinon boucle sans fin dans    processEvents ( QEventLoop::WaitForMore )
                          QApplication::eventLoop()->processEvents ( QEventLoop::WaitForMore );
                        }
                     G_PluginRun = "";
                    }  // endif ( proc.start() )
                else
                    {QMessageBox::warning ( 0, TR ("ERREUR : EXE_PROCESS") ,
                                                  TR("Le processus : \r\n%1\r\n n'a pas demarre. ").arg(pathExe),
                                                  TR ("&Annuler"), 0, 0,
                                                  1, 1 );
                    }
           } // endif (QFile::exists( pathExe ) )
        else
           {QMessageBox::warning ( 0, TR ("ERREUR : EXE_PROCESS") ,
                                                  TR("Le processus : \r\n%1\r\n n'a pas ete trouve. ").arg(pathExe),
                                                  TR ("&Annuler"), 0, 0,
                                                  1, 1 );
           }
       }   //endif (nb>=2)
    return ret;
   }
//-------------------------- EXTRACT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EXTRACT(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb<5) //  completer la liste si < 5
        {int i;
         for( i=nb; i<=6; ++i) arg_list.append("");
        }
     return Func_Extract(arg_list[0], arg_list[1], arg_list[2], arg_list[3], arg_list[4],arg_list[5]);
    }
//-------------------------- EXTRACT_ATCD -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EXTRACT_ATCD(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb<6) //  completer la liste si < 5
        {int i;
         for( i=nb; i<=7; ++i) arg_list.append("");
        }
     return Func_Extract_ATCD(arg_list[0], arg_list[1], arg_list[2], arg_list[3]);
    }

//-------------------------- EXTRACT_TABLE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EXTRACT_TABLE(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb<10) //  completer la liste si < 5
        {int i;
         for( i=nb; i<=10; ++i) arg_list.append("");
        }
     return Func_Extract_Table(arg_list[0], arg_list[1], arg_list[2], arg_list[3], arg_list[4],
                               arg_list[5], arg_list[6], arg_list[7], arg_list[8], arg_list[9]);
    }
//-------------------------- EXTRACT_TRAITEMENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EXTRACT_TRAITEMENT(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb<6) //  completer la liste si < 5
        {int i;
         for( i=nb; i<=6; ++i) arg_list.append("");
        }
     return Func_Extract_TTT(arg_list[0], arg_list[1], arg_list[2], arg_list[3], arg_list[4], arg_list[5]);
    }
//-------------------------- EXTRACT_VAR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EXTRACT_VAR(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb<5) //  completer la liste si < 5
        {int i;
         for( i=nb; i<=5; ++i) arg_list.append("");
        }
     return Func_Extract_VAR(arg_list[0], arg_list[1], arg_list[2], arg_list[3], arg_list[4]);
    }

//-------------------------- INDEX_OF -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::INDEX_OF(QStringList &arg_list)
{ if (arg_list.count()<1) return TR("::INDEX_OF() argument de la chaîne à evaluer manquant.");
  if (arg_list.count()<2) return TR("::INDEX_OF() argument de la chaîne à trouver manquant.");
  return QString::number(arg_list[0].find(arg_list[1]));
}

//-------------------------- LENGTH -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::LENGTH(QStringList &arg_list)
{ if (arg_list.count()<1) return TR("::LENGTH() argument de la chaîne à evaluer manquant.");
  return QString::number(arg_list[0].length());
}
//-------------------------- FILE_EXISTS -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::FILE_EXISTS(QStringList &arg_list)
{ if (arg_list.count()<1) return TR("::FILE_EXISTS() argument du path à convertir et tester manquant.");
  QString path = G_pCApp->resolvePath(arg_list[0]);
  if (QFile::exists(path))  return QString(path);
  return QString("");
}
//-------------------------- JOIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::JOIN(QStringList &arg_list)
{ QString message = "";
  for ( int i=0; i<arg_list.size(); ++i )  message +=  arg_list[i];
  return message;
}
//-------------------------- FILL -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::FILL(QStringList &arg_list)
    {//............. recupère les arguments passes à la fonction ................
     int        nb = arg_list.count();
     QString motif = " ";
     if (nb==0) return QString::null;
     if (nb==2) motif = arg_list[1].stripWhiteSpace();
     nb = arg_list[0].toInt();
     QString ret = "";
     motif       = motif.replace("spc"," ");
     for (int i=0; i<nb; ++i)
         {ret += motif;
         }
     return ret;
    }
//-------------------------- FORM_DATE_HEURE -------------------------------------------
QString C_TokenInterpret::FORM_DATE_HEURE (QStringList &arg_list)
{       int      nb = arg_list.count();
        QString ret = "";

        QString    txt_date  = QDate::currentDate().toString("dd-MM-yyyy");
        if (nb>=1) txt_date  = arg_list[0].stripWhiteSpace();
        QString    txt_heure = QTime::currentTime().toString("hh-mm-ss");
        if (nb>=2) txt_heure = arg_list[1].stripWhiteSpace();
        QString    titre     = TR("Choix dans la date et heure");
        if (nb>=3) titre     = arg_list[2].stripWhiteSpace();
        QString    formatD   = "dd-MM-yyyy";
        if (nb>=4) formatD   = arg_list[3].stripWhiteSpace();
        QString    formatH   = "hh:mm:ss";
        if (nb>=5) formatH   = arg_list[4].stripWhiteSpace();
        QString    sepDH     = " ";
        if (nb>=6) sepDH     = arg_list[5];

        Dlg_Calendar          *dlg  = new Dlg_Calendar(0, "Calendar_Dial", FALSE);
        if (dlg ==0)                                                   return ret;
        dlg->setCaption(titre);
        if (nb>=7)
           {QStringList lst = QStringList::split ('|', arg_list[6].stripWhiteSpace());
            if (lst.count())
               {dlg->setMenuPreSelectDate(lst);
                dlg->setMenuPreselectDateOn();
               }
            }
        if ( ! dlg->setDateTimeNatural_jjMMyyyy_Order(txt_date+txt_heure ))
           {dlg->setDate  (QDate::currentDate());
            dlg->setHeure (QTime::currentTime());
           }
        adjustScreenDialogPosition(QCursor::pos(),dlg);
        if (dlg->exec()== QDialog::Accepted )
           {ret = dlg->getDate().toString(formatD) + sepDH + dlg->timeEdit_Heure->time ().toString(formatH);
           }
        delete dlg;
        return ret;
}
//-------------------------- FORM_DATE -------------------------------------------
QString C_TokenInterpret::FORM_DATE (QStringList &arg_list)
{       int          nb = arg_list.count();
        QString     ret = "";
        QString  presel = "";
        int isInvalid = 0;
        QString    txt_date = QDate::currentDate().toString("dd-MM-yyyy");
        if (nb>=1) txt_date = arg_list[0].stripWhiteSpace();
        QString    titre    = TR("Choix dans la date");
        if (nb>=2) titre    = arg_list[1].stripWhiteSpace();
        QString    format   = "dd-MM-yyyy";
        if (nb>=3) format   = arg_list[2].stripWhiteSpace();

        Dlg_Calendar          *dlg  = new Dlg_Calendar(0, "Calendar_Dial", FALSE);
        if (dlg ==0)                                                   return ret;
        dlg->setCaption(titre);
        dlg->setDate  (CGenTools::setDate(txt_date , isInvalid));
        if (nb>=4)
           {QStringList lst = QStringList::split ('|', arg_list[3].stripWhiteSpace());
            if (lst.count())
               {dlg->setMenuPreSelectDate(lst);
                dlg->setMenuPreselectDateOn();
                dlg->setMode("days");
               }
            }
        adjustScreenDialogPosition(QCursor::pos(),dlg);
        if (dlg->exec()== QDialog::Accepted )
           {ret = dlg->getDate().toString(format);
           }
        delete dlg;
        return ret;
}

//-------------------------- SIMPLIFY -------------------------------------------
QString C_TokenInterpret::SIMPLIFY(QStringList &arg_list)
{ int          nb = arg_list.count();
  if (nb < 1) return TR("::SIMPLIFY() 1er argument manquant");
  QString        str = arg_list[0];
  QString   toRemove = " ";
  if (nb>1) toRemove = arg_list[1];
  return str.remove(toRemove);
}

//-------------------------- CLEAR_SPACES -------------------------------------------
QString C_TokenInterpret::CLEAR_SPACES(QStringList &arg_list)
{ if (arg_list.count()<1) return TR("::CLEAR_SPACES() 1er argument manquant");
  return arg_list[0].stripWhiteSpace();
}

//-------------------------- TO_NUM -------------------------------------------
QString C_TokenInterpret::TO_NUM(QStringList &arg_list)
{int nb                 = arg_list.count();
 if (nb<1) return TR("::TO_NUM() 1er argument manquant");
 QString    cutFirstStr = "";
 if (nb>1)  cutFirstStr = arg_list[1];
 return toNum(arg_list[0], cutFirstStr);
}

//-------------------------- adjustScreenDialogPosition -------------------------------------------
QPoint C_TokenInterpret::adjustScreenDialogPosition(QPoint pos, QWidget* pQWidget)
{   QRect rect = pQWidget->frameGeometry();
    int x =rect.x();
    int y =rect.y();
    QPoint widgetPos = pQWidget->mapToGlobal ( QPoint(x,y) );
    int w =rect.width();
    int h =rect.height();
    int dlgX  = pos.x();
    int dlgY  = pos.y();
    //int deltX = widgetPos.x()-x;
    //int deltY = widgetPos.y()-y;

    int dlgW  = w; //pQWidget->width();
    int dlgH  = h; //pQWidget->height();
    QDesktopWidget *d = QApplication::desktop();
    int screenW = d->width();     // returns desktop width
    int screenH = d->height();
    dlgX  = (dlgX+dlgW >= screenW) ?  screenW-dlgW : dlgX;
    dlgY  = (dlgY+dlgH >= screenH) ?  screenH-dlgH : dlgY;
    dlgX  = (dlgX <= 0) ? 1 : dlgX;
    dlgY  = (dlgY <= 0) ? 1 : dlgY;
    //pQWidget->move(pQWidget->mapFromGlobal (QPoint(dlgX,dlgY)));
    return QPoint(dlgX,dlgY);
}

//-------------------------- FORM_HEURE -------------------------------------------
QString C_TokenInterpret::FORM_HEURE (QStringList &arg_list)
{    QString        ret = "";
     int             nb = arg_list.count();
     C_Dlg_Pendule* dlg = new C_Dlg_Pendule(0);
     if (dlg==0)                  return ret;
     dlg->setCaption(TR("Heure"));
     QString    txt_heure = QTime::currentTime().toString("hh:mm:ss");
     if (nb>=1) txt_heure = arg_list[0].stripWhiteSpace();
     QString    formatH   = "hh:mm:ss";
     if (nb>=2) formatH   = arg_list[1].stripWhiteSpace();

     QTime time  = hhmmssToQTime(txt_heure );    if ( !time.isValid () || txt_heure.length()==0) { time = QTime::currentTime(); }
     dlg->setTime(time);
     adjustScreenDialogPosition(QCursor::pos(),dlg);
     if (dlg->exec() == QDialog::Accepted)
        { ret =  dlg->getTime().toString(formatH);
        }
     delete dlg;
     return ret;
}
//---------------------------------------- hhmmssToQTime ---------------------------------------
QTime C_TokenInterpret::hhmmssToQTime(const QString & heure )
{ int h=0; int m=0; int s=0; int ms=0; int len = heure.length();
  QString str = C_TokenInterpret::toNum(heure,"$allString");
  len         = str.length();
  if (len>=2) h  = str.mid(0,2).toInt();  // hh
  if (len>=4) m  = str.mid(2,2).toInt();  // hhmm
  if (len>=6) s  = str.mid(4,2).toInt();  // hhmmss
  if (len>=9) ms = str.mid(6).toInt();    // hhmmss
  return QTime( h,  m,  s,  ms );
}

// -------------------- IS_DATE_VALID -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::IS_DATE_VALID(QStringList &arg_list)
{if (arg_list.count()<=0) return TR("::IS_DATE_VALID() 1er argument manquant");
 QDateTime   dt = QDateTime::fromString ( arg_list[0], Qt::ISODate );
 if ( dt.isValid ()  )               return "1";
 else                                return "0";
}

// -------------------- GET_DOC_DATE_FIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::GET_DOC_DATE_FIN(QStringList &arg_list)
{ if (G_mCDC->m_pCRubCurrentRecord==0)  return "";
  QDateTime   dt = QDateTime::fromString ( G_mCDC->m_pCRubCurrentRecord->m_Fin, Qt::ISODate );
  if ( ! dt.isValid ()  )               return "";
  if ( arg_list.count() )               return dt.toString(arg_list[0].stripWhiteSpace());
  return dt.toString("yyyy-MM-dd hh:mm:ss");
}

//-------------------------- GET_DOC_PROP_0 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::GET_DOC_PROP_0(QStringList &)
    {if (G_mCDC->m_pCRubCurrentRecord)
        {return G_mCDC->m_pCRubCurrentRecord->m_Prop_0;
        }
     return QString("");
    }
//-------------------------- GET_DOC_PROP_1 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::GET_DOC_PROP_1(QStringList &)
    {if (G_mCDC->m_pCRubCurrentRecord)
        {return G_mCDC->m_pCRubCurrentRecord->m_Prop_1;
        }
     return QString("");
    }
//-------------------------- GET_DOC_PROP_2 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::GET_DOC_PROP_2(QStringList &)
    {if (G_mCDC->m_pCRubCurrentRecord)
        {return G_mCDC->m_pCRubCurrentRecord->m_Prop_2;
        }
     return QString("");
    }
//-------------------------- GET_DOC_PROP_3 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::GET_DOC_PROP_3(QStringList &)
    {if (G_mCDC->m_pCRubCurrentRecord)
        {return G_mCDC->m_pCRubCurrentRecord->m_Prop_3;
        }
     return QString("");
    }
//-------------------------- GET_DOC_PROP_4 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::GET_DOC_PROP_4(QStringList &)
    {if (G_mCDC->m_pCRubCurrentRecord)
        {return G_mCDC->m_pCRubCurrentRecord->m_Prop_4;
        }
     return QString("");
    }
//-------------------------- GET_SELECTION -------------------------------------------
 /*! \brief retourne le texte en cours de selection */
QString C_TokenInterpret::GET_SELECTION(QStringList &arg_list)
{    MyEditText      *pMyEditText   = 0; //G_pCApp->m_pDrTux->currentEditor();
     CMDI_Generic  *pCMDI_Generic   = G_pCApp->GetCurrentRubrique();
     if (pCMDI_Generic) pMyEditText = pCMDI_Generic->GetCurrentEditor();
     if (pMyEditText==0)         return QString::null;
     if (arg_list.count()==0)    return pMyEditText->selectedText();
     //......... methode copy/paste (pas de header <--fragment---> )
     QString text = "";
     pMyEditText->copy();
     QTextEdit *pQTextEdit = new QTextEdit(); //juste pour avoir fonctions copier/coller
     if (pQTextEdit)
        {pQTextEdit->setTextFormat(QTextEdit::RichText);
         pQTextEdit->paste();
         text  = pQTextEdit->text();
         delete pQTextEdit;
       }
     QString arg1 = arg_list[0].stripWhiteSpace();
     if (arg1.startsWith("$File"))
        {CGestIni::Param_UpdateToDisk(G_pCApp->resolvePath(arg1.mid(5).stripWhiteSpace()), text);
         return QString("");
        }
     else if (arg1.contains("$Paste"))
        {return arg1.replace("$Paste",text);
        }
     G_mCDC->m_pVariables->insert (arg1, text);
     return QString("");
     //const char *p = text;
     //char pt = (const char *) p;

     //if (CGestIni::IsUtf8(text ))
     //   {//QCString   cs = text.utf8 ();
         //const char *p = cs;
         //char      *pt = (char*)(const char*) p;
         //text          = CGestIni::CharStrToQStringUtf8(pt);
         //text = QString::fromUtf8 (pt); //latin1 ();fromUtf8 (
         //QMessageBox::information ( 0, "ee", text, "ok" );
         //text = CGestIni::fromMyUTF8(cs);
     //   }

}
//-------------------------- GRAPH -------------------------------------------
QString C_TokenInterpret::GRAPH(QStringList &arg_list)
    {//............. recupère les arguments passes à la fonction ................
     int nb = arg_list.count();
     if (nb<3) //  completer la liste si < 5
        {int i;
         for( i=nb; i<=2; ++i) arg_list.append("");
        }
     CDevilGraph *pGraph = new CDevilGraph ( G_mCDC );
     if (pGraph)
        {pGraph->Graph(arg_list[0]);
         delete pGraph;
        }
     return QString("");
    }
//-------------------------- INCLUDE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::INCLUDE(QStringList &arg_list)
    {QString path;
     return INCLUDE(arg_list, path);
    }
//-------------------------- INCLUDE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::INCLUDE(QStringList &arg_list, QString &path)
    {int                       nb = arg_list.count();  if (nb<1) return TR("::INCLUDE() 1er argument du fichier à importer manquant");
     QString                  ret = "";
     QString         typeAction   = "";
     QString            pathFile  = G_pCApp->resolvePath(arg_list[0]);
     if ( QFileInfo(pathFile).isDir ())                          return ret;
     path = pathFile;
     if (nb>1)         typeAction = arg_list[1].stripWhiteSpace();
     CGestIni::Param_UpdateFromDisk(pathFile, ret);
     if (typeAction.find("crlf2html")!=-1) {ret = ret.replace("\r\n", "<br>"); ret = ret.replace("\n", "<br>");ret = ret.replace("\r", "<br>");}
     return ret;
    }

//-------------------------- EXPORT_TABLE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EXPORT_TABLE(QStringList &arg_list)
{int nb = arg_list.count();
 //............ valeurs par defaut ................
 QString date_deb           = "1900-01-01T00:00:00";    // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
 QString date_end           = "3000-01-01T00:00:00";    // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
 QString exportDefinition   = "";                       // liste des tables a exportet style : VAR Constantes de Base[Pouls L=50  & SAO2 L=80  & Temp L=100]; VAR Poids[Poids L=50  & Taille L=80]
 QString tableSep           = "<br/>";                  // motif de separation  entre les tables  <br> par defaut
 QString cell_W             = "25%";                    // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
 QString cell_H             = "30";                     // hauteur de la cellule
 QString border_W           = "1";                      // largeur de la bordure
 QString width              = "400";                    // largeur du tableau
 QString dateTimeFormat     = "dd-MM-yyyy hh:mm";       // format d'affichage de la date
 QString dateTimeWidth      = "100";

 if (nb>0) date_deb         = arg_list[0];
 if (nb>1) date_end         = arg_list[1];
 if (nb>2) exportDefinition = arg_list[2].stripWhiteSpace();
 if (nb>3) tableSep         = arg_list[3].stripWhiteSpace();
 if (nb>4) cell_W           = arg_list[4];
 if (nb>5) cell_H           = arg_list[5];
 if (nb>6) border_W         = arg_list[6];
 if (nb>7) width            = arg_list[7];
 if (nb>8) dateTimeFormat   = arg_list[8];
 if (nb>9) dateTimeWidth    = arg_list[9];

if  (exportDefinition.startsWith("file:"))
    {exportDefinition = G_pCApp->resolvePath(exportDefinition.mid(5));
     exportDefinition = CGestIni::Param_UpdateFromDisk(exportDefinition);
    }

if (tableSep.startsWith("file:"))
    {tableSep = G_pCApp->resolvePath(tableSep.mid(5));
     tableSep = CGestIni::Param_UpdateFromDisk(tableSep);
    }

 return Func_Export_Tables(date_deb  ,         // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                           date_end,           // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                           exportDefinition,   // liste des tables a exportet style : VAR Constantes de Base[Pouls L=50  & SAO2 L=80  & Temp L=100]; VAR Poids[Poids L=50  & Taille L=80]
                           tableSep,           // motif de separation  entre les tables  <br> par defaut
                           cell_W,             // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                           cell_H,             // hauteur de la cellule
                           border_W,           // largeur de la bordure
                           width,              // largeur du tableau
                           dateTimeFormat,     // format d'affichage de la date
                           dateTimeWidth );    // largeur de la cellule de la date
}
//-------------------------- EXPORT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EXPORT(QStringList &arg_list)
{// 1er    nom fichier     : 0  fichier.de destination                fichier.de destination        fichier.de destination                     fichier.de destination
 // 2eme   quoiExporter    : 1  T pour string enrichie                A pour ASCII                  D pour dossier                             S pour selection
 // 3eme   argument        : 2  string a exporter                     string a exporter             liste des rubriques separees par |         String a exporter a laquelle sera ajoutee la selection
 // 4eme   argument        : 3  mode conversion ascii                 mode conversion ascii         date debut                                 mode conversion ascii
 // 5eme   argument        : 4  A/alwaysWrite F/fullOnly              A/alwaysWrite F/fullOnly      date fin                                   A/alwaysWrite F/fullOnly
 // 6eme   argument        : 5 separation des fiches $NomPatient $PrenomPatient $DateDeb $DateFin $DateDebFiche $DateFinFiche $HeureDeb $HeureFin $NomRub $TitreRub $User $SignUser $NumFicheRub $GUIDPat $PkPat $pkFiche $Duree $NumFicheTot
 // 7eme   argument        : 6 header
 // 8eme   argument        : 7 filtre sql where

 int                       nb = arg_list.count();
 if (nb<=0) return TR("::EXPORT() 1er  argument du fichier de destination manquant");
 if (nb<=1) return TR("::EXPORT() 2eme argument du type d'exportation manquant");
 if (nb<=2) return TR("::EXPORT() 3eme argument string a exporter ou liste des rubriques separees par | ou mode de conversion manquant");
 QString            pathFile  = G_pCApp->resolvePath(arg_list[0]);
 QString                toDo  = arg_list[1].stripWhiteSpace().lower();
 QString             dataStr  = arg_list[2];
 QString           imageName  = "";
                //............. cas le plus simple d'ecriture d'une chaine sur le disque ........................................
 if (toDo=="t"||toDo=="a" ||toDo=="s")
    {int      mode    = 0;
     QString  wrCond  = "f";
     if (nb>3)         mode = arg_list[3].toInt();
     if (nb>4)       wrCond = arg_list[4].lower();
     if (toDo=="s")
        {QStringList tmpList; tmpList.append("$Paste");
         dataStr.append(GET_SELECTION(tmpList));     // arg_list etant non vide la methode copy/paste sera choisie
        }
     if (toDo=="a")      dataStr = CHtmlTools::HtmlToAscii(dataStr, mode);
     if (wrCond=="f" &&  dataStr.length()==0)          return QString("");   // si vide et mode F/fullOnly
     CGestIni::Param_UpdateToDisk(pathFile, dataStr);
     return QString("");
    }
 //............. cas le plus complexe d'ecriture de fiches patient sur le disque ........................................
 QStringList rubNameLst = QStringList::split("|", dataStr.stripWhiteSpace()); if (rubNameLst.count()==0)  rubNameLst.append(TR("Observation"));
 QString     dateDeb    = "1900-01-01T00:00:00";
 QString     dateFin    = "3000-01-01T00:00:00";
 if (nb>3)   dateDeb    = arg_list[3].stripWhiteSpace();
 if (nb>4)   dateFin    = arg_list[4].stripWhiteSpace();
 QString     sepFiche   = TR("<br><hr><b>--- Fiche : $NomRub '$LibelleFicheRub'  Numero : $NumFicheRub vu par : $User pour $SignUser le $DateDeb à $HeureDeb -----</b><hr><br>");
 if (nb>5)   sepFiche   = arg_list[5].stripWhiteSpace();

 G_pCApp->loadContentsIfBeginByTokenFile(sepFiche);     // si commence par $File charge le contenu du fichier

 QString     headerDoc  = TR("<br>--- Edition du dossier : $NomPatient $PrenomPatient Numero : $GUIDPat-----<br>");
 if (nb>6)   headerDoc  = arg_list[6].stripWhiteSpace();

 QString     sqlFilter  = "";
 if (nb>7)   sqlFilter  = arg_list[7].stripWhiteSpace();

 //....................... on y va ................................................
 QString                         user = "";
 QString                       droits = G_pCApp->m_Droits;
 QString                doc_sign_user = "";
 QString                friend_droits = "";
 QString                  prefixDroit = "";
 QString                 htmlToExport = "";
 int                          typeRub = 0;
 int                       typeRefRub = 0;
 QString                      nameRub = "";
 QDateTime                   dtRefDeb = QDateTime::fromString(dateDeb, Qt::ISODate);
 QDateTime                   dtRefFin = QDateTime::fromString(dateFin, Qt::ISODate);
 QDateTime                   dtRubDeb;
 RUBREC_LIST                  rubList;
 RUBREC_LIST::iterator             it;

 //....................... on recupere les donnees du header ................................................
 G_mCDC->m_pMB->initRubriquesList(&rubList,  G_pCApp->m_NumGUID, sqlFilter);
 //......... pour chaque rubrique (liste de nom donne en entree) .....................
 //          on examine la liste des rubriques et si correspond au nom
 //          alors on retient et examine les autres conditions
 for ( QStringList::Iterator lt = rubNameLst.begin(); lt != rubNameLst.end(); ++lt )
     {nameRub       = *lt;
      typeRefRub    = G_pCApp->RubNameToStringType(nameRub).toInt();
      //prefixDroit   = G_pCApp->GetMap_Prefix_Droits(QString::number(typeRefRub));
      prefixDroit   = G_pCApp->m_mapNameRubInfos[nameRub];
      for (it = rubList.begin(); it !=  rubList.end(); ++it) //int id=0; id<rubList.count(); ++id) //it = rubList.begin(); it !=  rubList.end(); ++it )
          {bool droitOk  = TRUE;
           bool droitOkA = TRUE;
           bool droitOkT = TRUE;
           bool droitOkV = TRUE;
           dtRubDeb      = QDateTime::fromString((*it).m_Date, Qt::ISODate);
           typeRub       = (*it).m_Type.toInt();
           user          = (*it).m_User;
           doc_sign_user = (*it).m_SignUser;
           droits        = G_pCApp->m_Droits;
           //................... droits
           if (G_pCApp->m_User != doc_sign_user) droits = G_pCApp->m_pCMoteurBase->PermsUserIsThisUserFriendFromName( doc_sign_user , G_pCApp->m_User); // on prend le droit delegue
           if (typeRub>=20060000 && typeRub<=20069999)
              { droitOkA = G_pCApp->IsThisDroitExist(droits,"atv");        //atv atc ttc  tvv tvc etc
                droitOkT = G_pCApp->IsThisDroitExist(droits,"ttv");
                droitOkV = G_pCApp->IsThisDroitExist(droits,"tvv");
              }
           else
              {droitOk              = G_pCApp->IsThisDroitExist(droits, prefixDroit+"v");
               if (droitOk) droitOk = G_pCApp->IsThisDroitExist(droits, prefixDroit+"p");
              }
           if (droitOk && typeRub>=typeRefRub && typeRub<=typeRefRub+9999  && dtRubDeb >= dtRefDeb && dtRubDeb <= dtRefFin) // filtrer sur type rubrique, et date
              {
                QByteArray          data;
                long                 len;
                QString        stringDST = "";
                char                *ptr = 0;
                int                  ret = G_mCDC->m_pMB->GetDataFromRubList(data, it);
                if (ret == RUB_READ_ON_DISK)      // en fait cela veut dire que ça ete lu sur le disque
                   {ptr        = data.data();
                    len        = data.size();
                    if (ptr && len>6)
                       {if (typeRub>=20020000 && typeRub<=20029999)               //>>> .......... ordonnances .....................
                           {if (strncmp(ptr, "<?xml", 5)==0)
                               {if (CGestIni::IsUtf8( ptr, len ))
                                   { CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(QString::fromUtf8 ( ptr ), &stringDST,0);
                                   }
                                else CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(data.data(), &stringDST,0);
                               }
                            else if (strncmp(ptr, "<html>", 6))
                               {CHtmlTools::RtfToQtextEdit_HTML(ptr+4, len-4, stringDST,1);
                               }
                            else
                               {if (CGestIni::IsUtf8( ptr ,data.size())) stringDST  = QString::fromUtf8 ( ptr );
                                else                                     stringDST  = ptr;
                               }
                           } //endif (typ>=20020000 && typ<=20029999)             //>>> .......... ordonnances ......................
                        else if (typeRub>=20060000 && typeRub<=20069999)          //>>> .......... terrain ..........................
                           {QString ordo;
                            QString str;
                            if (CGestIni::IsUtf8( ptr ,data.size())) str  = QString::fromUtf8 ( ptr );
                            else                                     str  = ptr;
                            stringDST  =  "<br>";
                            if (droitOkA) stringDST += Func_Extract_ATCD_D(str,
                                                             TR("<b>° <u>Antecedents ::TYPE:: :</u></b>"),
                                                             "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;::LIBELLE::    ::MEDCHIR::  ::NOTE::   ::DATE::",
                                                             "en date du :",
                                                             "est en rapport avec une pathologie ALD");
                            ordo = "";
                            if (droitOkT && CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(str, &ordo,0))
                               {stringDST +=                  "<TABLE cellSpacing=\"1\""  // motif de separation  entre les tables  <br> par defaut
                                                              " cellpadding=\"1\""
                                                              " width=\"100%\""
                                                              " border=\"1\">"
                                                              "<TBODY>"
                                                              "<TR bgcolor=\"#ece6c9\"><TD width=100% align=\"center\"><i>Traitement de fond</i></TD></TR>"
                                                              "</TBODY></TABLE>"+ordo+"<br>";
                               }
                            //stringDST += Func_Extract_TTT_D(str,
                            //                                TR("<br><b>° <u>Traitement de fond :</u></b><br>"),
                            //                                "<b>::NUM::)&nbsp;&nbsp;<u>::LIBELLE::</u></b>&nbsp;(::ALD::)<br>"
                            //                                "<i>::NB_PRISE::&nbsp;::REPART_PRISES::&nbsp;::POSO_TEXTUELLE::&nbsp;::NOTE::&nbsp;::DUREE::</i><br>","","","",""
                            //                                );
                            if (droitOkV) stringDST += Func_Export_Tables_D(str,            // donnees du terrain
                                                              dateDeb,        // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                                                              dateFin,        // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                                                              "",             // liste des tables a exportet style : VAR Constantes de Base[Pouls =50  ; SAO2 =80  ; Temp =100] VAR Poids[Poids =50  ; Taille =80]
                                                              "<TABLE cellSpacing=\"1\""  // motif de separation  entre les tables  <br> par defaut
                                                              " cellpadding=\"1\""
                                                              " width=\"100%\""
                                                              " border=\"1\">"
                                                              "<TBODY>"
                                                              "<TR bgcolor=\"#ece6c9\"><TD width=100% align=\"center\"><i>Tableaux de variables : <b>'$TableName'</b></i></TD></TR>"
                                                              "</TBODY></TABLE>",
                                                              "25%",                      // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                                                              "30",                       // hauteur de la cellule
                                                              "1",                        // largeur de la bordure
                                                              "100%"                      // largeur du tableau
                                                             );
                            stringDST += "<p>";
                           } //endif (typ>=20020000 && typ<=20029999)             //>>> .......... terrain ..........................
                        else if (typeRub>=20110000 && typeRub<=20119999)          //>>> .......... donnees externes .................
                           {
                           } //endif (typ>=20020000 && typ<=20029999)             //>>> .......... donnees externes .................
                        else                                                      //>>> .......... documents .................
                           {if (strncmp(ptr, "<?xml", 5)==0)
                               {if (CGestIni::IsUtf8( ptr ,data.size()))
                                   {G_pCApp->dataToInterface( QString::fromUtf8 ( ptr ), stringDST);     //setText(QString::fromUtf8 ( ptr ));
                                   }
                                else
                                   {G_pCApp->dataToInterface( ptr, stringDST );
                                   }
                               }
                            else if (strncmp(ptr, "<html>", 6))
                               {CHtmlTools::RtfToQtextEdit_HTML(ptr+4, len, stringDST,1);
                               }
                            else
                               {if (CGestIni::IsUtf8( ptr ,data.size())) stringDST  = QString::fromUtf8 ( ptr );
                                else                                     stringDST  = ptr;
                               }
                           } //endif (typ>=20020000 && typ<=20029999)     //>>> .......... documents .................
                        if (stringDST.length())
                           {QString tmp = sepFiche;
                            tmp.replace("$NomRub",nameRub);
                            tmp.replace("$NumFicheRub",(*it).m_PrimKey);
                            tmp.replace("$LibelleFicheRub",(*it).m_Libelle);
                            tmp.replace("$User",(*it).m_User);
                            tmp.replace("$SignUser",(*it).m_SignUser);
                            tmp.replace("$DateDeb",dtRubDeb.toString("dd-MM-yyyy"));
                            tmp.replace("$HeureDeb",dtRubDeb.toString("hh:mm:ss"));
                            htmlToExport += tmp + stringDST;
                           }
                       } //endif (ptr && len>6)
                   } //endif (ret == RUB_READ_ON_DISK)
              } //if (type==(*it).m_Type && dtRubDeb>=dtRefDeb && dtRubDeb<=dtRefFin)  filtrage des dates
          } //endfor (int id=0; id<rubList.count(); ++id) //it = rubList.begin(); it !=  rubList.end(); ++it )
     } //end for ( QStringList::Iterator lt = rubNameLst.begin(); lt != rubNameLst.end(); ++lt )
  //.................... FUSIONNER EN DERNIER (car sinon PB de reentrance ...) ..........................
  G_pCApp->loadContentsIfBeginByTokenFile(headerDoc);
  headerDoc = headerDoc.stripWhiteSpace();
  if (headerDoc.length())
     {
      //............. fusionner le header ...........................
      CDevilCrucible *pfusion = new CDevilCrucible( &headerDoc                      ,          // 0 texte du document à fusionner
                                                    G_pCApp->m_pCMoteurBase  ,          // 1 moteur de base de donnees (faut bien acceder aux fonctions)
                                                    G_pCApp->m_pCMedicaBase  ,          // 2 moteur de base de donnees medicamenteuses(faut bien acceder aux fonctions)
                                                    G_mCDC->m_pRubList              ,          // 3 liste des documents composant le dossier patient
                                                    G_mCDC->m_IdentPrimKey          ,          // 4 il faut les renseignements sur le patient
                                                    G_mCDC->m_pCurDisplayDocMap     ,          // 5 liste des rubriques courantes (affichees)
                                                    G_mCDC->m_UserDocPk             ,          // 6 utilisateur responsable du document
                                                    0                               ,          // 7 pointeur sur la rubrique en cours de modif (si c'est une rubrique) zero sinon
                                                    &G_pCApp->m_VAR_MAP                        // 8 pointeur sur la liste des variables (zero par defaut)
                                                  );
      if (  pfusion )
         {pfusion-> DoMixture();
          delete pfusion;
         }
     }
  htmlToExport      = htmlToExport.prepend(headerDoc);
  if (htmlToExport.length())
     { if (toDo=="d")       // si exportation d'un dossier on cree un repertoire du nom du fichier et on place les donnees dedans
          {QFileInfo fi(pathFile);
           QString folderExp = fi.dirPath (TRUE)+"/"+fi.baseName();
           QDir(pathFile).mkdir(folderExp);
           pathFile          = folderExp+"/"+fi.baseName()+".html";
           QStringList imgList;
           CHtmlTools::getTextImageList( htmlToExport, imgList);
           for ( QStringList::Iterator lt = imgList.begin(); lt != imgList.end(); ++lt )
               {imageName = *lt;
                QPixmap retPixMap;
                if (G_pCApp->m_C_ImageList.getPixmap( imageName , retPixMap))
                   {   QImageIO iio;
                       QImage   im;
                       im = retPixMap; // convert to image
                       iio.setImage( im );
                       iio.setFileName( folderExp+"/"+imageName );
                       iio.setFormat( "PNG" );
                       iio.write();
                   }
                else if (QFile::exists(G_pCApp->m_PathGlossaire+"/ImagesFolder/"+imageName))
                   {CHtmlTools::Copy_File(G_pCApp->m_PathGlossaire+"/ImagesFolder/"+imageName, folderExp);
                   }
               }
          }
       }
   CGestIni::Param_UpdateToDisk(pathFile, htmlToExport.prepend("<html><head><meta name=\"qrichtext\" content=\"charset=utf-8\" />"
                                                               "</head><body style=\"font-size:10pt;font-family:Sans Serif\">"));
  return htmlToExport;
}
//-------------------------- IMPORT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::IMPORT(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     int                       nb = arg_list.count();  if (nb<1) return TR("::import() 1er argument du fichier à importer manquant");
     QString                 ret  = "";
     MyEditText     *pMyEditText  = 0; //G_pCApp->m_pDrTux->currentEditor();
     CMDI_Generic  *pCMDI_Generic = 0;
     QString        rubriqueCible = "";
     QString                title = "";
     QString              subType = "";
     QString                 type = "";
     if (nb>2)      rubriqueCible = arg_list[2].stripWhiteSpace();
     if (nb>4)              title = arg_list[4].stripWhiteSpace();
     if (nb>5)            subType = arg_list[5].stripWhiteSpace();
     if (nb>5)               type = arg_list[6].stripWhiteSpace();
     if (rubriqueCible.length()==0)
        {pCMDI_Generic = G_pCApp->GetCurrentRubrique();
         if (pCMDI_Generic)    pMyEditText   = pCMDI_Generic->GetCurrentEditor();
         if (pMyEditText==0)   rubriqueCible = RUBNAME_DOCUMENTS;         // si pas d'editeur dans la rubrique courante ou pas de rubrique courante
         else                  rubriqueCible = pCMDI_Generic->m_RubName;
        }
     if (pMyEditText==0)
        {pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubriqueCible);
         if (pCMDI_Generic==0) G_pCApp->m_pDrTux->OnActiverOrCreateRubrique(rubriqueCible);
         pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubriqueCible);
         if (pCMDI_Generic==0) return TR("::import() la rubrique de destination: \"") + rubriqueCible + TR("\" n'a pu être cree");
         pMyEditText = pCMDI_Generic->GetCurrentEditor();
        }

     if (pMyEditText)
        {QString            destPath  = "";
         QString            imageFile =  G_pCApp->resolvePath(arg_list[0]);
         QString         typeAction   = "new";
         if (nb>1)         typeAction = arg_list[1].stripWhiteSpace();
         if (nb>3)           destPath = G_pCApp->resolvePath(arg_list[3]);
         bool                 verbose = (typeAction.find("verbose")!=-1);
         if (   QFileInfo(imageFile).isDir ())                                               return ret;
         if ( ! QFile::exists( imageFile ) && typeAction.find("remove_src")== -1)            return verbose ? TR("::import() Fichier image : \"") + imageFile + TR("\" non trouve") : "";
         QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
         if (QFileInfo(imageFile).size() <= 0)      {QApplication::restoreOverrideCursor();  return verbose ? TR("::import() Fichier image : \"") + imageFile + TR("\" vide") : "";  }              // si fichier vide on sort
         if (typeAction.length()==0) typeAction = "new";

         //.................. insertion html ............................................................
         QString toImport;
         CGestIni::Param_UpdateFromDisk(imageFile, toImport);
         if (typeAction.find("crlf2html")!=-1) {toImport = toImport.replace("\r\n", "<br>"); toImport = toImport.replace("\n", "<br>");toImport = toImport.replace("\r", "<br>");}
         if (destPath.length())
            {if ( !QFileInfo(destPath).exists() )
                {if (!QDir().mkdir ( destPath ) )
                    {QApplication::restoreOverrideCursor();
                     return verbose ? TR("::import() le repertoire de destination: \"") + destPath + TR("\" n'a pu être cree") : "";
                    }
                }
             CHtmlTools::Copy_File(imageFile, destPath);
             if (typeAction.find("clear_src") != -1)
                {QFile qfile(imageFile);
                 if (qfile.open(IO_WriteOnly)==TRUE)
                    {qfile.close();
                    }
                }
            }
         if (typeAction.find("remove_src")!= -1)
            {QDir().remove(imageFile);
            }
         if ( typeAction.find("insert") != -1)                 // si reference est la première creer l'icone de l'image
            {if (typeAction.find("no_html") != -1)  CHtmlTools::insertHtml(pMyEditText, toImport,1);        // html remove
             else                                   CHtmlTools::insertHtml(pMyEditText, toImport,0);        // keep html
             if (title.length()) pCMDI_Generic->ChangeCurrentComboBoxItemName(title);
            }
         else  if ( typeAction.find("replace") != -1)
            {pMyEditText->setText(toImport);
             if (title.length()) pCMDI_Generic->ChangeCurrentComboBoxItemName(title);
            }
         else  if ( typeAction.find("new") != -1)
            {int typeGeneric = pCMDI_Generic->GetType();
             if (typeGeneric>=20020000 && typeGeneric<=20020900)
                {if (QFileInfo(imageFile).extension().lower()=="ord")        typeGeneric = TYP_ORDO_CALC;
                 else                                                        typeGeneric = TYP_ORDONNANCE;
                }
             if (title.length()==0)  title = QFileInfo(arg_list[0]).baseName();
             pCMDI_Generic->AddNewDocument(toImport,  type.length()?type.toInt():typeGeneric, imageFile,0, &title, &subType, (typeAction.find("noInquire") != -1));
            }

         QApplication::restoreOverrideCursor();
        }
     return ret;
    }

//-------------------------- INSERT_CISP -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::INSERT_CISP(QStringList &arg_list)
{QString chapiFilter  = "";
 QString classFilter  = "";
 QString templateStr  = TR("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ::CISP_LIBELLE:: code Cisp = ::CISP_CODE::;");
 QString showCombos   = "Chapitres Rubriques";
 int                     nb = arg_list.count();
 if (nb>0)      chapiFilter = arg_list[0].stripWhiteSpace();
 if (nb>1)      classFilter = arg_list[1].stripWhiteSpace();
 if (nb>2)      templateStr = arg_list[2];
 if (nb>3)      showCombos  = arg_list[3];
 if (G_pCApp && G_pCApp->m_pDrTux) return G_pCApp->m_pDrTux->CodageCisp( chapiFilter, classFilter, templateStr , showCombos);
 return QString::null;
}

//-------------------------- INSERT_IMAGE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::INSERT_IMAGE(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     QString ret = "";
     int      nb = arg_list.count();
     if (nb<1)                                        return TR("::insert_image() 1er argument du fichier image manquant");
     MyEditText *pMyEditText  = G_pCApp->m_pDrTux->currentEditor();
     if (pMyEditText)
        {QString            imageFile = G_pCApp->resolvePath(arg_list[0]);
         int                    max_w = 500;
         int                    max_h = -1;
         QString       renameAction   = 0;
         QString       destPath       = 0;
         if (nb>=2)             max_w = arg_list[1].toInt();
         if (nb>=3)             max_h = arg_list[2].toInt();
         if (nb>=4)      renameAction = arg_list[3].stripWhiteSpace();
         if (nb>=5)          destPath = G_pCApp->resolvePath(arg_list[4]);

         if ( QFileInfo(imageFile).isDir ())                                                 return ret;
         if ( ! QFile::exists( imageFile ) && renameAction.find("remove_src")== -1)          return TR("::insert_image() Fichier image : \"") + imageFile + TR("\" non trouve");
         QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
         if (QFileInfo(imageFile).size() <= 10)      {QApplication::restoreOverrideCursor(); return ret; }                // si fichier vide on sort
         QPixmap      img(imageFile);     // format image devine car 'const char * format = 0'
         if ( img.isNull())                          {QApplication::restoreOverrideCursor(); return TR("::insert_image() l'image : \"") + imageFile + TR("\" n'a pu être creee");}
         //............... mise à l'echelle de l'image .....................................
         int w             = img.width ();
         int h             = img.height();
         float coefReduc_w = 1.00;
         float coefReduc_h = 1.00;
         if (max_h != -1 && w > max_w) coefReduc_w = (float)max_w / (float) w;
         if (max_h != -1 && h > max_h) coefReduc_h = (float)max_h / (float) h;
         if (coefReduc_w < 1.00 || coefReduc_h < 1.00)
            {if (coefReduc_h < coefReduc_w) coefReduc_w = coefReduc_h;         // on va se servir de coefReduc_w = au coeff de reduction le plus petit
             w = (int) ((float) w * coefReduc_w);
             h = (int) ((float) h * coefReduc_w);
             QImage   qi    = img.convertToImage().smoothScale (w, h );
             img            = QPixmap (qi );
            }
         //.................. insertion html ............................................................
         QString    imgName    = CMoteurBase::GUID_Create()+"-h"+ QString::number(h)+"w"+QString::number(w) + ".png";                  // nom et clef pour retrouver l'image
         if (renameAction.length())
            {if ( !QFileInfo(destPath).exists() )
                {if (!QDir().mkdir ( destPath ) )
                    {QApplication::restoreOverrideCursor();
                     return TR("::insert_image() le repertoire de destination: \"") + destPath + TR("\" n'a pu être cree");
                    }
                }
             CHtmlTools::Copy_File(imageFile, destPath, &imgName);
             if (renameAction.find("clear_src") != -1)
                {QFile qfile(imageFile);
                 if (qfile.open(IO_WriteOnly)==TRUE)
                    {qfile.close();
                    }
                }
             if (renameAction.find("remove_src")!= -1)
                {QDir().remove(imageFile);
                }
             imageFile = destPath + "/" + imgName;
            }
         if ( G_pCApp->m_C_ImageList.appendToList( &img , imgName, imageFile) == 1 )                 // si reference est la première creer l'icone de l'image
            {ret  = "<img src=\"";
             ret += imgName;
             ret += "\" width=\""  + QString::number(w);
             ret += "\" height=\"" + QString::number(h);
             ret += "\">";
            }
         QApplication::restoreOverrideCursor();
        }
     return ret;
    }
//-------------------------- INTITULE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::INTITULE(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     if (G_mCDC->m_pCRubCurrentRecord)
        {// Cette fonction ajoute ou remplace l'intitule de l'observation en cours
         // UTILISATION =
         //  {{:: Intitule ( Intitule du document , add  ) }}
         //  {{:: Intitule ( Intitule du document , change  ) }}
         //  {{:: Intitule ( Intitule du document ) (equivalent de "change") }}
         //............. recupère les arguments passes à la fonction ................
         QString libelle = arg_list[0].replace("&nbsp;"," ").stripWhiteSpace();
         int nb = arg_list.count();
         if (nb<6) //  completer la liste si < 5
            {int i;
             for( i=nb; i<=2; ++i) arg_list.append("");
            }

 //        arg_list[0] = arg_list[0];
         arg_list[1] = arg_list[1].stripWhiteSpace();
         // ......... modifie l'intutile de l'observation
         if      (arg_list[1].lower() == "add")       {G_mCDC->m_pCRubCurrentRecord->m_Libelle.append(" " + libelle );}
         else /* if (arg_list[1].lower() == "change")  {G_mCDC->m_pCRubCurrentRecord->m_Libelle = libelle; }
         else if (arg_list[1].lower() == "") */       {G_mCDC->m_pCRubCurrentRecord->m_Libelle = libelle; }
         //.......... recharger le combo ..................................................................
         QString qstr = G_mCDC->m_pCRubCurrentRecord->m_Date.left(10) + " " + G_mCDC->m_pCRubCurrentRecord->m_Libelle;
         CMDI_Generic *pCMDI_Generic =  G_pCApp->GetCurrentRubrique();
         if (pCMDI_Generic) pCMDI_Generic->ChangeCurrentComboBoxItemName(qstr);
        }
     return QString("");
    }
//-------------------------- LIST_DIR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::LIST_DIR(QStringList &arg_list)
{   QString        ret       = "";
    QString  start_dir       = "";
    QString  sep             = "|";
    QStringList filterList;
    QStringList extList;
    bool foldersMustBeListed = FALSE;
    int nb = arg_list.count();
    if (nb>=4)  sep                    = arg_list[3];
    if (nb>=1)  start_dir              = G_pCApp->resolvePath(arg_list[0]);
    if (nb>=2)  filterList             = QStringList::split ( sep, arg_list[1]);
    if (nb>=3)  extList                = QStringList::split ( sep, arg_list[2]);
    if (nb>=5)  foldersMustBeListed    = arg_list[4].toInt();

    //................ allons y  .....................................................
    QDir l_dir;
    l_dir.setPath ( start_dir );
    l_dir.convertToAbs();
    l_dir.setFilter( QDir::All | QDir::NoSymLinks );
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = l_dir.entryInfoList();
    if (pQFileInfoList==0) return 0;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;

    while ( (fi = it.current()) != 0 )
          {QString fname = fi->fileName();
           QString ext   = QFileInfo(fname).extension().lower();
           QStringList::Iterator ut;
           if      (fname == "..")
                   {
                   }
           else if (fi->isDir() && fname[0] != '.' )
                   {if (foldersMustBeListed)
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
                        if (ok) ret += fname+sep;
                       }
                   }
           else if (fi->isFile() && fname[0] != '.')
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
                    if (ok) ret += fname+sep;
                   }
           ++it;
          }
  return ret;
}

//-------------------------- LOWER -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::LOWER(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb<1) return QString::null;
     return arg_list[0].lower();
    }

//-------------------------- NOP -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOP(QStringList &)
{ return "";
}

//-------------------------- MESSAGE_ALERT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::MESSAGE_ALERT(QStringList &arg_list)
{int          nb     = arg_list.count();
 QString   title     = TR("Information");
 QString message     = "";
 int           w     = -1;
 int           h     = -1;
 QString imagePath   = "";
 //QString s_ok        = "Ok";
 char*   button1     = (char*) (const char*) "Ok" ;
 char*   button2     = 0 ;
 char*   button3     = 0 ;
 char*   button4     = 0 ;
 if (nb>=1) message    = arg_list[0];
 if (nb>=2) title      = arg_list[1];
 if (nb>=3) button1    = (char*) (const char*)arg_list[2];
 if (nb>=4) button2    = (char*) (const char*)arg_list[3];
 if (nb>=5) button3    = (char*) (const char*)arg_list[4];
 if (nb>=6) button4    = (char*) (const char*)arg_list[5];
 if (nb>=7) w          = arg_list[6].toInt();
 if (nb>=8) h          = arg_list[7].toInt();
 if (nb>=9)  imagePath = G_pCApp->resolvePath(arg_list[8]);

if (message.left(5)=="$File")
   {message=message.mid(5);
    QStringList lst;
    lst.append(message);
    QString finalPath;
    message = INCLUDE(lst, finalPath);
    if (imagePath.length()==0)
       {imagePath = QFileInfo(finalPath).dirPath();
       }
   }
 switch(CGenTools::MessageAlert(0, title,  message,  button1,  button2, button3, button4, w, h, imagePath))
    {case 100: return arg_list[2];
     case 200: return arg_list[3];
     case 300: return arg_list[4];
     case 400: return arg_list[5];
    }
 return QString::null;
}


//-------------------------- MESSAGE_POPUP -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::MESSAGE_POPUP(QStringList &arg_list)
{int          nb     = arg_list.count();
 QString message     = "";
 int     tempo       = 1000 ;
 QString imagePath   = Theme::getPath(TRUE)+"MessagePop.png";/* ="" */
 if (nb>=1) message    = arg_list[0];
 if (nb>=2) imagePath  = arg_list[1];
 if (nb>=3) tempo      = arg_list[2].toInt();
 G_pCApp->CouCou(message, imagePath, tempo );
 return QString::null;
}
//-------------------------- CURRENT_EDITOR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::CURRENT_EDITOR(QStringList &arg_list)
{ if (G_pCApp==0)           return "";
  if (G_pCApp->m_pDrTux==0) return "";
  // CMDI_Generic  *pCMDI_Generic = G_pCApp->GetCurrentRubrique();
  int          rub_type_ret    = 0;
  QString          rubName     = "";
  CRubRecord  *pCRubRecord     = 0;
  CMDI_Generic  *pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->GetCurrentRubrique(&rubName,  &rub_type_ret , &pCRubRecord);

  if ( pCMDI_Generic ==0 )                           return "";
  if ( arg_list.count()>=1)
     {QString infoTypToRet = arg_list[0].stripWhiteSpace();
      if (infoTypToRet=="type")                      return QString::number(rub_type_ret);
      else if (infoTypToRet== "name")                return rubName;
      else if (infoTypToRet== "default mask dir")    return pCMDI_Generic->GetDefaultMaskDirectory();
      else if (infoTypToRet== "default mask file")   return pCMDI_Generic->GetDefaultMaskFile();
      else if (infoTypToRet== "title")               return pCRubRecord->m_Libelle;
      else if (infoTypToRet.startsWith("date"))  
              { QString format = "dd-MM-yyyy hh:mm";
                QString toRet  =  "";
                int pos        = infoTypToRet.find("=");
                if (pos != -1) format = infoTypToRet.mid(pos+1).stripWhiteSpace();

                if (pCRubRecord)
                   { QDateTime   qdt = pCRubRecord->getDateTime();
                     toRet = qdt.toString(format);
                   }
                return toRet; 
              } 
     }
  return rubName;
}
//-------------------------- NOUVEAU_DOCUMENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOUVEAU_DOCUMENT(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     int nb = arg_list.count();
     if (nb<4) //  completer la liste si < 5
        {int i;
         for( i=nb; i<=3; ++i) arg_list.append("");
        }
     QString         dataToInsert = " ";
     MyEditText     *pMyEditText  = 0; //G_pCApp->m_pDrTux->currentEditor();
     CMDI_Generic  *pCMDI_Generic = 0;
     QString        rubriqueCible = "";
     if (nb>=1)     rubriqueCible = arg_list[0].stripWhiteSpace();
     if (nb>=2)     dataToInsert  = arg_list[1];
     if (rubriqueCible.length()==0||rubriqueCible=="*")
        {pCMDI_Generic = G_pCApp->GetCurrentRubrique();
         if (pCMDI_Generic)    pMyEditText = pCMDI_Generic->GetCurrentEditor();
         if (pMyEditText==0) rubriqueCible = RUBNAME_DOCUMENTS;         // si pas d'editeur dans la rubrique courante ou pas de rubrique courante
         else                rubriqueCible = pCMDI_Generic->m_RubName;
        }
     if (pMyEditText==0)
        {//pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubriqueCible);
         if (pCMDI_Generic==0) G_pCApp->m_pDrTux->OnActiverOrCreateRubrique(rubriqueCible);
         pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubriqueCible);
         if (pCMDI_Generic==0) return TR("::nouveau document() la rubrique de destination: \"") + rubriqueCible + TR("\" n'a pu être cree");
         pMyEditText = pCMDI_Generic->GetCurrentEditor();
        }
     // if (dataToInsert.length()==0) dataToInsert = " ";  // l'insertion d'un document vide echoue c'est en amont qu'il faut gerer vacuite d'un document
     if (pCMDI_Generic!=0) pCMDI_Generic->AddNewDocument(dataToInsert,  pCMDI_Generic->GetType() );
     return "";
    }
//-------------------------- OS -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::OS(QStringList &)
     {
       #ifdef Q_WS_WIN
            return QString("W");
       #endif
       #ifdef Q_WS_X11
            return QString("L");
       #endif
       #ifdef Q_WS_MAC
            return QString("M");
       #endif
     }
//-------------------------- MOVE_CURSOR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::MOVE_CURSOR(QStringList &arg_list)
{    if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     int nb = arg_list.count();
     if (arg_list.count()<1) return TR("::MOVE_CURSOR() nom de la rubrique non indique");
     if (arg_list.count()<2) return TR("::MOVE_CURSOR() position non indiquee");

     QString         position     = " ";
     MyEditText     *pMyEditText  = 0; //G_pCApp->m_pDrTux->currentEditor();
     CMDI_Generic  *pCMDI_Generic = 0;
     QString        rubriqueCible = "";
     if (nb>=1)     rubriqueCible = arg_list[0].stripWhiteSpace();
     if (nb>=2)     position      = arg_list[1].stripWhiteSpace().upper();
 
     if (rubriqueCible.length()==0||rubriqueCible=="*")
        { pCMDI_Generic = G_pCApp->GetCurrentRubrique();
          if (pCMDI_Generic==0) return TR("::MOVE_CURSOR() rubrique courante non trouve");
          pMyEditText = pCMDI_Generic->GetCurrentEditor();
          if (pMyEditText==0)   return TR("::MOVE_CURSOR() editeur de texte courant non trouve");
        }
     if (pMyEditText==0)
        {
         if (pCMDI_Generic==0) G_pCApp->m_pDrTux->OnActiverOrCreateRubrique(rubriqueCible);
         pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubriqueCible);
         if (pCMDI_Generic==0) return TR("::MOVE_CURSOR() la rubrique de destination: \"") + rubriqueCible + TR("\" n'a pu être cree");
         pMyEditText = pCMDI_Generic->GetCurrentEditor();
        }
     // if (dataToInsert.length()==0) dataToInsert = " ";  // l'insertion d'un document vide echoue c'est en amont qu'il faut gerer vacuite d'un document
     if (pMyEditText !=0 )
        {  int pos  = position.find("::MOVE");
           if (pos != -1)
              {position   = position.mid(pos+6);
               int toMove = -1;
               if      (position[0]=='B')                     toMove = QTextEdit::MoveBackward;
               else if (position[0]=='W' && position[4]=='B') toMove = QTextEdit::MoveWordBackward;
               else if (position[0]=='W' && position[4]=='F') toMove = QTextEdit::MoveWordForward;
               else if (position[0]=='F')                     toMove = QTextEdit::MoveForward;
               else if (position[0]=='U')                     toMove = QTextEdit::MoveUp;
               else if (position[0]=='D')                     toMove = QTextEdit::MoveDown;
               else if (position[0]=='L' && position[4]=='S') toMove = QTextEdit::MoveLineStart;
               else if (position[0]=='L' && position[4]=='E') toMove = QTextEdit::MoveLineEnd;
               else if (position[0]=='H')                     toMove = QTextEdit::MoveHome;
               else if (position[0]=='E')                     toMove = QTextEdit::MoveEnd;
               else if (position[0]=='P' && position[2]=='U') toMove = QTextEdit::MovePgUp;
               else if (position[0]=='P' && position[2]=='D') toMove = QTextEdit::MovePgDown;
               if (toMove != -1) pMyEditText->MyMoveCursor( toMove );
              }
        }
     return "";
}
//-------------------------- PUT_VAR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::PUT_VAR(QStringList &arg_list)
     {if (arg_list.count()<1) return TR("::PUT_VAR() nom de la variable non indique");
      return (*G_mCDC->m_pVariables)[arg_list[0].stripWhiteSpace()];
     }

//-------------------------- REINIT_CLASSEUR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::REINIT_CLASSEUR(QStringList &)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     if (G_pCApp->m_pDrTux->m_pDock_Organiseur)
        {G_pCApp->m_pDrTux->m_pC_Organiseur->makeListeMonitor();
        }
     return QString("");
    }
//-------------------------- REPLACE_BY -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::REPLACE_BY(QStringList &arg_list)
{int  nb = arg_list.count();
 if (nb<1)                                        return TR("::REPLACE_BY() 1er argument du texte à traiter manquant");
 if (nb<2)                                        return TR("::REPLACE_BY() 2ème argument motif à  remplacer manquant");
 if (nb<3)                                        return TR("::REPLACE_BY() 3ème argument motif de remplacement manquant");
 return arg_list[0].stripWhiteSpace().replace(arg_list[1],arg_list[2]);
}

//-------------------------- REPLACE_IN_FILE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::REPLACE_IN_FILE(QStringList &arg_list)
{QString str;
 QString by;
 QString toReplace;
 int  nb = arg_list.count();
 if (nb<1)                                        return TR("::REPLACE_IN_FILE() 1er argument du fichier source à traiter manquant");
 if (nb<2)                                        return TR("::REPLACE_IN_FILE() 2eme argument du fichier de destination manquant");
 QString   srcFile  = G_pCApp->resolvePath(arg_list[0].stripWhiteSpace());         // nom de du fichier image de remplacement
 QString   dstFile  = G_pCApp->resolvePath(arg_list[1].stripWhiteSpace());         // nom de du fichier image de remplacement
 CGestIni::Param_UpdateFromDisk(srcFile, str);
 for (int i=2; i<nb; ++i)
     { by = arg_list[i].stripWhiteSpace();  // nom de la variable ou nom d'un fichier (n'ayons peur de rien)
       toReplace = QString("{{%1}}").arg(by);      // ce qu'il y a a remplacer dans le fichier est le nom de la variable encadre de {{}}
       by        = (*G_mCDC->m_pVariables)[by];    // la valeur est le contenu de la variable
       str.replace(toReplace,by);
     }
 CGestIni::Param_UpdateToDisk(dstFile, str);
 return QString("");
}

//-------------------------- REPLACE_IMAGE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::REPLACE_IMAGE(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     QString ret = "";
     int      nb = arg_list.count();
     if (nb<1)                                        return TR("::replace_image() argument du nom de l'image à remplacer manquant");        //{{GET LAST IMAGE NAME}}
     if (nb<2)                                        return TR("::replace_image() argument du fichier image de remplacement manquant");     //$ToAbsPath ../../tmp/{{GET LAST IMAGE NAME}}
     MyEditText *pMyEditText  = G_pCApp->m_pDrTux->currentEditor();
     if (pMyEditText)
        {QString            imageName = arg_list[0].stripWhiteSpace();                               // nom de l'image à replacer
         QString            imageFile = G_pCApp->resolvePath(arg_list[1].stripWhiteSpace());         // nom de du fichier image de remplacement
         int                    max_w = 550;
         int                    max_h = -1;
         QString             action   = 0;
         QString             srcName  = 0;
         if (nb>=2)             max_w = arg_list[2].toInt();
         if (nb>=3)             max_h = arg_list[3].toInt();
         if (nb>=4)            action = arg_list[4].stripWhiteSpace();
         QPixmap                  img;
         //................ recuperer dimenssions originales ...........................
         G_pCApp->m_C_ImageList.getPixmap(imageName, img);
         int ori_w                    = img.width ();
         int ori_h                    = img.height();

         if ( QFileInfo(imageFile).isDir ())                                                 return ret;
         if ( ! QFile::exists( imageFile ) )                                                 return TR("::replace_image() Fichier image : \"") + imageFile + TR("\" non trouve");
         QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
         if (QFileInfo(imageFile).size() <= 10)      {QApplication::restoreOverrideCursor(); return TR("::replace_image() Fichier image : \"") + imageFile + TR("\" vide"); }                // si fichier vide on sort
         img = QPixmap(imageFile);     // format image devine car 'const char * format = 0'
         if ( img.isNull())                         {QApplication::restoreOverrideCursor();  return TR("::replace_image() l'image : \"")       + imageFile + TR("\" n'a pu être creee");}
         //............... mise à l'echelle de l'image .....................................
         if (action.find("keep_Original_W")!= -1 && ori_w>0) max_w = ori_w;
         if (action.find("keep_Original_H")!= -1 && ori_h>0) max_h = ori_h;
         int w             = img.width ();
         int h             = img.height();
         float coefReduc_w = 1.00;
         float coefReduc_h = 1.00;
         if (max_w != -1 && w > max_w) coefReduc_w = (float)max_w / (float) w;
         if (max_h != -1 && h > max_h) coefReduc_h = (float)max_h / (float) h;
         if (coefReduc_w < 1.00 || coefReduc_h < 1.00)
            {if (coefReduc_h < coefReduc_w) coefReduc_w = coefReduc_h;         // on va se servir de coefReduc_w = au coeff de reduction le plus petit
             w = (int) ((float) w * coefReduc_w);
             h = (int) ((float) h * coefReduc_w);
             QImage   qi    = img.convertToImage().smoothScale (w, h );
             img            = QPixmap (qi );
            }
         //...................... remplacer l'image d'origine de la liste des images ..............................................
         if (action.find("keep_src")!= -1)          srcName   = G_pCApp->m_C_ImageList.getSrcImageName(imageName);
         else                                       srcName   = imageFile;
         G_pCApp->m_C_ImageList.destroyFromList(imageName);                             // detruire dans la liste  tout ce qui concerne ce nom
         if ( G_pCApp->m_C_ImageList.appendToList( &img , imageName, srcName) == 1 )    // rajouter une l'image
            {QString t = pMyEditText->text (  );     // methode bourin pour forcer le redraw
             CHtmlTools::ReplaceImageTag(t, imageName, imageName,  w ,  h );
             pMyEditText->setText ( t );
             if (action.find("remove_src")!= -1)
                {QDir().remove(imageFile);
                }
            }
         QApplication::restoreOverrideCursor();
        }
     return ret;
    }
//-------------------------- RUBRIQUE_SHOW -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::RUBRIQUE_SHOW(QStringList &arg_list)
    {int      nb = arg_list.count();
     if (nb<1)                   return TR("::RUBRIQUE_SHOW() rubrique name omited");           //{{GET LAST IMAGE NAME}}
     if (G_pCApp->m_pDrTux==0)   return TR("::RUBRIQUE_SHOW() G_pCApp->m_pDrTux==0 ");

     QString rubrique            = arg_list[0].stripWhiteSpace();
     QString val                 = "t";
     if (nb>=2)              val = arg_list[1].stripWhiteSpace().lower();
     CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubrique);

     if (val[0]=='t')
        { if (pCMDI_Generic==0) G_pCApp->m_pDrTux->OnActiverOrCreateRubrique(rubrique);
          pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubrique);
          if (pCMDI_Generic==0)   return TR("::RUBRIQUE_SHOW() la rubrique de destination: \"") + rubrique + TR("\" n'a pu être cree");
         
          if (pCMDI_Generic) 
            { pCMDI_Generic->show();
              pCMDI_Generic->ActiverRubrique(rubrique,0);  // le dernier parametre n'est pas utilise et est un parametre mort
            }
        }
     else
        {if (pCMDI_Generic==0) G_pCApp->m_pDrTux->OnActiverOrCreateRubrique(rubrique);
         pCMDI_Generic = (CMDI_Generic*) G_pCApp->m_pDrTux->IsExistRubrique(rubrique);
         if (pCMDI_Generic==0)   return TR("::RUBRIQUE_SHOW() la rubrique de destination: \"") + rubrique + TR("\" n'a pu être cree");
         pCMDI_Generic->hide();
        }
     return QString("");
    }
//-------------------------- SET_DOC_DATE_DEB -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SET_DOC_DATE_DEB(QStringList &arg_list)
{ if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
  int nb = arg_list.count();
  if (nb<1)                 return TR("::SET_DOC_DATE_DEB() date omited");

  QString date = arg_list[0].stripWhiteSpace();
  QDateTime dt = QDateTime::fromString ( date, Qt::ISODate );

  if ( !dt.isValid () )     return TR("::SET_DOC_DATE_DEB() invalid date '%1' ").arg(date);

  if (G_mCDC->m_pCRubCurrentRecord)
     { G_mCDC->m_pCRubCurrentRecord->m_Date = date;
       if (G_pCApp->m_pDrTux==0)          {G_pCApp->CouCou(TR("Erreur VALIDER_DOSSIER() : DrTux non initialise"));return QString::null;}
       G_pCApp->m_pDrTux-> initAllRubriquesComboWithRubList();
     }
   return QString("");
}

//-------------------------- SET_DOC_DATE_FIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SET_DOC_DATE_FIN(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     if (G_mCDC->m_pCRubCurrentRecord)
        {if (arg_list.count()>0) G_mCDC->m_pCRubCurrentRecord->m_Fin = arg_list[0];
        }
     return QString("");
    }
//-------------------------- SET_DOC_PROP_0 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SET_DOC_PROP_0(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     if (G_mCDC->m_pCRubCurrentRecord)
        {if (arg_list.count()>0) G_mCDC->m_pCRubCurrentRecord->m_Prop_0 = arg_list[0];
         else                    G_mCDC->m_pCRubCurrentRecord->m_Prop_0 = "";
        }
     return QString("");
    }
//-------------------------- SET_DOC_PROP_1 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SET_DOC_PROP_1(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     if (G_mCDC->m_pCRubCurrentRecord)
        {if (arg_list.count()>0) G_mCDC->m_pCRubCurrentRecord->m_Prop_1 = arg_list[0];
         else                    G_mCDC->m_pCRubCurrentRecord->m_Prop_1 = "";
        }
     return QString("");
    }
//-------------------------- SET_DOC_PROP_2 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SET_DOC_PROP_2(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     if (G_mCDC->m_pCRubCurrentRecord)
        {if (arg_list.count()>0) G_mCDC->m_pCRubCurrentRecord->m_Prop_2 = arg_list[0];
         else                    G_mCDC->m_pCRubCurrentRecord->m_Prop_2 = "";
        }
     return QString("");
    }
//-------------------------- SET_DOC_PROP_3 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SET_DOC_PROP_3(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     if (G_mCDC->m_pCRubCurrentRecord)
        {if (arg_list.count()>0) G_mCDC->m_pCRubCurrentRecord->m_Prop_3 = arg_list[0];
         else                    G_mCDC->m_pCRubCurrentRecord->m_Prop_3 = "";
        }
     return QString("");
    }
//-------------------------- SET_DOC_PROP_4 -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SET_DOC_PROP_4(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     if (G_mCDC->m_pCRubCurrentRecord)
        {if (arg_list.count()>0) G_mCDC->m_pCRubCurrentRecord->m_Prop_4 = arg_list[0];
         else                    G_mCDC->m_pCRubCurrentRecord->m_Prop_4 = "";
        }
     return QString("");
    }
//-------------------------- SET_VAR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SET_VAR(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb==0)  return QString::null;
     if (nb==1)  arg_list.append("");
     QString name  = arg_list[0].stripWhiteSpace();
     QString val   = arg_list[1].stripWhiteSpace();
     if (val.startsWith("$File"))
        {QString outParam;
         CGestIni::Param_UpdateFromDisk(G_pCApp->resolvePath(val.mid(5).stripWhiteSpace()), outParam);
         G_mCDC->m_pVariables->insert (name, outParam);
        }
     else if (val.startsWith("$Selection"))
        {QStringList tmpLst; tmpLst.append(name);
         GET_SELECTION(tmpLst);
        }
     else
        {G_mCDC->m_pVariables->insert (name, val);
        }
     return QString("");
    }
//-------------------------- VAR_LIST_CLEAR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::VAR_LIST_CLEAR(QStringList &)
    {G_mCDC->m_pVariables->clear();
     return QString("");
    }
//-------------------------- VAR_SPLIT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::VAR_SPLIT(QStringList &arg_list)
    {int nb = arg_list.count();
     if (nb<2)  return TR("Erreur VAR_SPLIT() nombre d'arguments insuffisants" );
     QString  prefix = arg_list[0];
     QString  toSep  = arg_list[1];
     QString    sep  = "|";                // valeur par defaut
     QString  cutTo  = "";
     QString item    = "";
     QString  modif  = "";
     if (nb>=3) sep  = arg_list[2];
     bool allowEmptyEntries = TRUE;        // valeur par defaut
     bool resultTrimmed     = FALSE;       // valeur par defaut
     if (nb>=4) modif = arg_list[3].stripWhiteSpace(); //=="FALSE") allowEmptyEntries = FALSE;
     if (modif.contains('F')) allowEmptyEntries = FALSE;
     if (modif.contains('T')) resultTrimmed     = TRUE;
     if (nb>=5) cutTo = arg_list[4].stripWhiteSpace();
     QStringList listeSegments = QStringList::split(sep,toSep,allowEmptyEntries);
     int i = 0;
     int n = 0;
     for ( QStringList::Iterator it = listeSegments.begin(); it != listeSegments.end(); ++it )
         {item = listeSegments[i];
          if (cutTo.length())
             { int p = item.find(cutTo);
               if (p!=-1) item.truncate(p);
               else       item="";         // si cutTo est defini on ne retient que les items encadres
             }
          if (resultTrimmed) item=item.stripWhiteSpace();
          if (allowEmptyEntries==FALSE)
             { if (item.length()) 
                  { G_mCDC->m_pVariables->insert (prefix+QString::number(n), item);
                    ++n;
                  }
             }
          else
             {G_mCDC->m_pVariables->insert (prefix+QString::number(n), item);
              ++n;
             }
          ++i;
         }
     return QString("");
    }

//-------------------------- SOUS_TYPE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SOUS_TYPE(QStringList &arg_list)
    {if (G_mCDC->m_isJustForWatch==CDevilCrucible::JustForWatch) return QString("");
     if (G_mCDC->m_pCRubCurrentRecord)
        {// Cette fonction ajoute ou remplace l'intitule de l'observation en cours
         // UTILISATION =
         //  {{:: sous_type ( sous type du document , add  ) }}
         //  {{:: sous_type ( sous type du document , change  ) }}
         //  {{:: sous_type ( sous type du document ) (equivalent de "change") }}
         //............. recupère les arguments passes à la fonction ................
         int nb = arg_list.count();
         if (nb<1) return QString::null;
         if (nb<2) arg_list.append("");

         arg_list[0] = arg_list[0].stripWhiteSpace();
         arg_list[1] = arg_list[1].stripWhiteSpace();
         // ......... modifie l'intutile de l'observation
         if      (arg_list[1].lower() == "add")       {G_mCDC->m_pCRubCurrentRecord->m_SubType.append(" " + arg_list[0]);}
         else /*if (arg_list[1].lower() == "change")  {G_mCDC->m_pCRubCurrentRecord->m_SubType = arg_list[0]; }
         else if (arg_list[1].lower() == "")    */    {G_mCDC->m_pCRubCurrentRecord->m_SubType = arg_list[0]; }
         //.......... recharge le combo
        }
     return QString("");
    }

//-------------------------- SQL_EXEC -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SQL_EXEC(QStringList &arg_list)
    {if (G_mCDC->m_pMB->OpenBase()==0)
        { return TR("Base non ouverte ::sql_exec() ");
        }
     //............. recupère les arguments passes à la fonction ................
     QString ret = "";
     int      nb = arg_list.count();
     if (nb<1) //  completer la liste si < 5
        {return TR("Erreur de syntaxe ::sql_exec() cette fonction doit comporter  au moins 1 argument ");
        }
     QString requete = arg_list[0];
     requete.replace("&#44;",",");
     requete.replace("&#40;","(");
     requete.replace("&#41;",")");
     QSqlQuery query(requete , G_mCDC->m_pMB->m_DataBase );
     if ( !query.isActive() )
        {
         G_mCDC->m_pMB->OutSQL_error(query, " sql_exec() " , requete);
        }
     G_mCDC->m_pMB->CloseBase();
     return ret;
    }

//-------------------------- SQL_GET_RUB_HEAD_PK -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SQL_GET_RUB_HEAD_PK(QStringList &arg_list)
    { int nb          = arg_list.count();
      if (nb<1) //  completer la liste si < 5
        {if (G_mCDC->m_pCRubCurrentRecord) return G_mCDC->m_pCRubCurrentRecord->m_PrimKey;
         else                              return TR("Erreur de syntaxe ::SQL_GET_RUB_HEAD_PK() car m_pCRubCurrentRecord = 0  ");
        }
      CRubRecord *pCRubRecord = GetIDCurrentDoc(arg_list[0]);
      if (pCRubRecord) return pCRubRecord->m_PrimKey;
      return QString(TR("SQL_GET_RUB_HEAD_PK() not found for this Name : ")+arg_list[0]);
    }
//-------------------------- SQL_GET_DOSS_PK -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SQL_GET_DOSS_PK(QStringList &)
    {return G_mCDC->m_IdentPrimKey;
    }
//-------------------------- SQL_GET_DOSS_GUID -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SQL_GET_DOSS_GUID(QStringList &)
    {return G_pCApp->m_NumGUID;
    }
//-------------------------- SQL_WRITE -------------------------------------------
  /*! \brief Fonction qui permet de mettre a jour ou inserer des donnees dans une table SQL Si l'enregistrement existe deja il est mis a jour sinon il est insere
   *  \param arg_list const &QStringList  : reference sur la liste des arguments de la fonction
   *                       premier  argument : nom de la table a mettre a jour
   *                       deuxieme argument : condition WHERE (la mention WHERE ne doit pas etre fournie) pour selectionner les enregistrements a mettre a jour
   *                       troisieme arguement : nom du champ a mettre a jour doit etre suivi du
   *                       quatrieme arguement : valeur a mettre a jour dans ce champ.
   *                              .
   *                              .
   *                       nieme argument    : nom du champ a mettre a jour doit etre suivi du
   *                       nieme argument +1 : valeur a placer dans ce champ
   *  \return        une chaine vide si tout est Ok, sinon un message d'erreur.
   */
QString C_TokenInterpret::SQL_WRITE(QStringList &arg_list)
{  int nbArg = arg_list.count();
   int     i = 0;
   if (nbArg<1)  return QObject::tr("Syntax error in  SQL_WRITE() first argument table name not found");
   if (nbArg<2)  return QObject::tr("Syntax error in  SQL_WRITE() second argument 'where condition' not found");
   if (nbArg<3)  return QObject::tr("Syntax error in  SQL_WRITE() third argument 'first field name' fnot found");
   if (nbArg<4)  return QObject::tr("Syntax error in  SQL_WRITE() forth argument 'first field value' fnot found");

   //.......... si la condition est vraie on update ....................................
   QString tbl_name  = arg_list[0];
   QString str_where = arg_list[1]; str_where.replace("&#44;",","); str_where.replace("&#40;","("); str_where.replace("&#41;",")");
   QString prepare   = "";                                                  //  first field , tbl name , where condition
   QString requete   = QString("SELECT `%1` FROM `%2` WHERE %3 LIMIT 0,1").arg( arg_list[2] , tbl_name , str_where );   // un seul resultat est suffisant pour evaluer si record existe deja
   QSqlQuery sqlQuery_i (requete , G_mCDC->m_pMB->m_DataBase );
   bool update = (sqlQuery_i.isActive() && sqlQuery_i.next());
   if (update) 
      { prepare          = QString("UPDATE `%1` SET     \n").arg(tbl_name);
              i          = 2;    // on part du nom du champ (0=nom table 1=where condition 2=nom premier champ 3=valeur du premier champ ..... )
        while (i<nbArg)
            { prepare   +=  arg_list[i].stripWhiteSpace()  + "=?,\n";   // recuperer le nom du premier champ
              i         += 2;                                           // champ suivant
            }
        prepare.truncate(prepare.length()-2);   // on vire le dernier ",\n"
        prepare         += "\n";                // on le remplace par  "\n"
        prepare         += QString(" WHERE %1 ;").arg(str_where);
      }
   else 
      { prepare          = QString("INSERT INTO `%1`  (\n").arg(tbl_name);
              i          = 2;     // on part du nom du champ (0=nom table 1=where condition 2=nom premier champ 3=valeur du premier champ ..... )
        while (i<nbArg)
            { prepare   +=  arg_list[i].stripWhiteSpace()  + ",\n";    // recuperer le nom du premier champ
              i         += 2;                                          // champ suivant
            }
        prepare.truncate(prepare.length()-2);   // on vire le dernier ",\n"
        prepare         += "\n";                // on le remplace par "\n"
        prepare         += " )\n  VALUES (";
        i                = 2;
        while (i<nbArg)
             { prepare  += "?,";
               i        += 2;
             }
        prepare.truncate(prepare.length()-1);   // on vire le dernier ","
        prepare          += ");";
      }

   //......... les deux options UPDATE ou INSERT sont pretes pour le bind ..............
   QString err = "";
   QSqlQuery query(G_mCDC->m_pMB->m_DataBase);
   if (!query.prepare(prepare))
      { err  = QObject::tr("ERREUR  : SQL_WRITE (%1)  query.prepare <br/> %2").arg(update?"UPDATE":"INSERT",prepare);
        err += "\n"+query.lastError().text();
        qDebug(err);
        return err;
      }
   i     = 3;   // on part de la valeur du champ (0=nom table 1=where condition 2=nom premier champ 3=valeur du premier champ ..... )
   int n = 0;
   while (i<nbArg)
         { query.bindValue(n, arg_list[i]); ++n;
           i += 2;
         }
   if (!query.exec())
      { err  = QObject::tr("ERREUR  : SQL_WRITE (%1)  query.exec <br/> %2").arg(update?"UPDATE":"INSERT",prepare);
        err += "\n"+query.lastError().text();
        qDebug(err);
        return err;
      }
   return "";
}

//-------------------------- SQL_SELECT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SQL_SELECT(QStringList &arg_list)
    {// Cette fonction execute une requete SQL et retourne le resultat dans une liste
     // UTILISATION =
     //  {{:: SQL_SELECT ( table, liste des champs a recuperer separes par | , condition  , nbrMaxListe, separateur de champ, separateur de ligne) }}

     //............. recupère les arguments passes à la fonction ................
     int i, row;
     QString retMask = "";
     QString ret     = "";
     int nb          = arg_list.count();
     QString   val   = "";
     QStringList fieldList;
     if (nb<4) //  completer la liste si < 5
        {return TR("Erreur de syntaxe ::sql_select() cette fonction doit comporter  au moins 4 arguments ");
        }
     if (nb==7) retMask = arg_list[6];
     if (nb<4)  {for( i=nb; i<=7; ++i) arg_list.append("");}

     QString requete("SELECT ");
     int nb_c  = arg_list[1].contains('|') + 1;
     fieldList = QStringList::split('|',arg_list[1]);
     requete  += arg_list[1].replace('|',',');
     requete  += " FROM ";
     requete  += arg_list[0] + " ";
     requete  += arg_list[2];
     if (G_mCDC->m_pMB->OpenBase()==0)
        { return TR("Base non ouverte ::sql_select() ");
        }
    //qDebug(requete);
    requete.replace("&#44;",",");
    requete.replace("&#40;","(");
    requete.replace("&#41;",")");
    QSqlQuery sqlQuery (requete , G_mCDC->m_pMB->m_DataBase );
    QString f_sep = arg_list[4];
    QString l_sep = arg_list[5];
    int     nb_l  = arg_list[3].toInt();    if (nb_l<=0) nb_l = 20;
    if (l_sep.length()==0) l_sep = " ";
    if (f_sep.length()==0) f_sep = "\n";
    if (sqlQuery.isActive())
       {row = 0;
        while (sqlQuery.next() && nb_l)
              {if (retMask.length()) retMask  = arg_list[6];  // recharger le masque
               for( i=0; i<nb_c; ++i)      // pour toutes les colones (champs de la base)
                  {val = sqlQuery.value(i).toString();
                   if (f_sep=="$SET_VAR")
                      {QString varName = arg_list[0]+"."+fieldList[i]+QString::number(row);
                       G_mCDC->m_pVariables->insert (varName, val);
                      }
                   else if (retMask.length())
                      {retMask.replace(f_sep+QString::number(i), val);
                       if (i>=nb_c-1) ret += retMask + l_sep;
                      }
                   else
                      {ret += val;
                       if (i<nb_c-1) ret += f_sep;
                       else          ret += l_sep;
                      }
                  }
               ++row;
               --nb_l;
              }
       }
     G_mCDC->m_pMB->CloseBase();
     return ret;
    }

//-------------------------- TEST -------------------------------------------
// TEST (S,PATIENT,>=,ADRESSE,TESTISTRUE,TESTISFALSE, 1)
/*! \brief non documente */
QString C_TokenInterpret::TEST(QStringList &arg_list)
    {if ( arg_list.count() <= 5 )  return TR("Erreur de syntaxe ::TEST cette fonction doit comporter  au moins 5 arguments ");
     if ( arg_list.count() <= 6 )  arg_list.append("");
     QString op =  arg_list[2].stripWhiteSpace();
     int    iop = 0;
     bool    cs = TRUE;
     if      (op=="=" )  {iop = 1; }
     else if (op==">" )  {iop = 2; }
     else if (op=="<" )  {iop = 3; }
     else if (op==">=")  {iop = 4; }
     else if (op=="<=")  {iop = 5; }
     else if (op=="!=")  {iop = 6; }
     else if (op=="%=")  {iop = 7; }
     else if (op=="=%")  {iop = 8; }
     else if (op=="%%")  {iop = 9; }

     QString val1    = arg_list[1].stripWhiteSpace();
     QString val2    = arg_list[3].stripWhiteSpace();
     QString ifTrue  = arg_list[4].stripWhiteSpace();
     QString ifFalse = arg_list[5].stripWhiteSpace();

     if (arg_list.count() == 7) cs = (bool)arg_list[6].toInt();
     if (arg_list[0][0]=='N') // on teste les valeurs numeriques et si pas ok on continue mais avec les textes
        {double v1 = toNum(val1).toDouble();
         double v2 = toNum(val2).toDouble();
         switch (iop)
               {
                case 1:     // =
                    {if (v1==v2 ) return arg_list[4].stripWhiteSpace();
                     else         return arg_list[5].stripWhiteSpace();
                    }
                case 4:     // >=  si egal  ok
                    {if (v1==v2 ) return arg_list[4].stripWhiteSpace();
                    }
                case 2:     // >
                    {if (v1>v2)  return arg_list[4].stripWhiteSpace();
                     else        return arg_list[5].stripWhiteSpace();
                    }
                case 5:     // <=  si egal  ok
                    {if (v1==v2 ) return arg_list[4].stripWhiteSpace();
                    }
                case 3:     // <
                    {if (v1<v2)  return arg_list[4].stripWhiteSpace();
                     else        return arg_list[5].stripWhiteSpace();
                    }
                case 6:     // !=
                    {if (v1!=v2 ) return arg_list[4].stripWhiteSpace();
                     else         return arg_list[5].stripWhiteSpace();
                    }
               }
       }
     switch (iop)
               {case 1:     // =
                    {if (val1.length()==0 && val2.length()==0) return ifTrue;   //si les deux chaines sont vides == fonctionne mal
                     else if (val1==val2 )                     return ifTrue;
                     else                                      return ifFalse;
                    }
                case 4:     // >=  si egal  ok
                    {if (val1==val2 ) return arg_list[4].stripWhiteSpace();
                    }
                case 2:     // >
                    {if (QString::compare ( val1, val2 )>0)  return arg_list[4].stripWhiteSpace();
                     else                                    return arg_list[5].stripWhiteSpace();
                    }
                case 5:     // <=  si egal  ok
                    {if (val1==val2 ) return arg_list[4].stripWhiteSpace();
                    }
                case 3:     // <
                    {if (QString::compare ( val1, val2 )<0)  return arg_list[4].stripWhiteSpace();
                     else                                    return arg_list[5].stripWhiteSpace();
                    }
                case 6:     // !=
                    {if (val1.length()==0 && val2.length()==0) return ifFalse;      // egales donc test faux si les deux chaines sont vides == fonctionne mal
                     else if (val1==val2 )                     return ifFalse;      // egales donc test faux
                     else                                      return ifTrue;       // pas egale donc test vrai
                    }
                case 7:     // %=
                    {if (val1.startsWith(val2,cs))   return arg_list[4].stripWhiteSpace();
                     else                            return arg_list[5].stripWhiteSpace();
                    }
                case 8:    // =%
                    {if (val1.endsWith(val2,cs))  return arg_list[4].stripWhiteSpace();
                     else                         return arg_list[5].stripWhiteSpace();
                    }
                case 9:    // %%
                    {if (val1.contains(val2,cs)) return arg_list[4].stripWhiteSpace();
                     else                        return arg_list[5].stripWhiteSpace();
                    }
               } // endswitch (op)
    return QString::null;
    }

//-------------------------- TO_SCRIPT -------------------------------------------
// TO_TEXT (string,mode)
/*! \brief non documente */
QString C_TokenInterpret::TO_SCRIPT(QStringList &arg_list)
{if ( arg_list.count() <= 0 )  return TR("Erreur de syntaxe ::TO_TEXT() cette fonction doit comporter  au moins 1 arguments (le texte à convertir) ");
 int mode = 10;      // strict ASCII ne conserve pas les <br>  mode&CHtmlTools::commaProtect|mode&CHtmlTools::StrictASCII
 if ( arg_list.count() == 2 )  mode = arg_list[1].toInt()|8;     // |8 pour protection des virgules
 QString str = arg_list[0];
 return CHtmlTools::HtmlToAscii(str,  mode);
}

//-------------------------- UPPER -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::UPPER(QStringList &arg_list)
    {if ( arg_list.count()==0) return QString::null;
     return arg_list[0].upper();
    }
//-------------------------- VALIDER_DOSSIER -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::VALIDER_DOSSIER(QStringList & /*arg_list*/)
    {if (G_pCApp==0)                    return QString::null;
     if (G_pCApp->m_pDrTux==0)          {G_pCApp->CouCou(TR("Erreur VALIDER_DOSSIER() : DrTux non initialise"));return QString::null;}
     G_pCApp->m_pDrTux->OnDrTuxSaveRubList();
     G_mCDC->regenerateDisplayMap();
     if (G_pCApp==0)           return "";
     if (G_pCApp->m_pDrTux==0) return "";
     
     //DOCUMENT_DISPLAY_MAP mapDoc;
     //G_pCApp->m_pDrTux->MapActiveID_Doc(mapDoc);
     //G_mCDC->m_pCurDisplayDocMap->copy(mapDoc);
     // *G_mCDC->m_pCurDisplayDocMap = G_pCApp->m_pDrTux->GetMapActiveID_Doc();
     //  G_pCApp->m_pDrTux->MapActiveID_Doc(mapDoc);
     /* 
     G_mCDC->     CRubRecord             *m_pCRubCurrentRecord;/ *!< current  rubrique record  (if exist) * /
     VAR_MAP                *m_pVariables;        / *!< contiendra la liste des variables locales des scripts * /
     CMoteurBase            *m_pMB;               / *!< pointer to CMoteurBase class to provide database functions manipulation's * /
     QString                 m_IdentPrimKey;      / *!< patient identity primary key on m_DOSS_IDENT_TBL_NAME * /
     QString                 m_UserDocPk;         / *!< current  user primary key on m_USER_IDENT_TBL_NAME from document user * /
     RUBREC_LIST            *m_pRubList;          / *!< pointer to RUBREC_LIST documents list of the patient * /
     const DOCUMENT_DISPLAY_MAP   *m_pCurDisplayDocMap; / *!< pointer to DOCUMENT_DISPLAY_MAP, current displaying documents list * /
     CMedicaBase            *m_pMD;               / *!< pointer to CMedicaBase class to provide drugs database functions manipulation's * /
     QString                *m_pDocument;         / *!< QString pointer to the text to be modified * /
     QString                 m_UserActuelPk;      / * !< current  user primary key on m_USER_IDENT_TBL_NAME from actual   user * /
     int                     m_isJustForWatch;    / *!< to specifie that fusion must be doo just for see * /
   */
 //........ il vaut mieux recuperer la rubrique avant Slot_ExeMixture () .......
 //         car si zero Slot_ExeMixture() la recherche a chaque fois
 //         et l'operation est longue

     return QString::null;
    }
///////////////////////////// MACROS CALL BACK /////////////////////////////////////////////

//-------------------------- ADRESSE COMPLETE MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::ADRESSE_COMPLETE_MEDECIN(QStringList &)
{QString resolvToken("");
 resolvToken  = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_RUE,       G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserDocPk)  + "\n";
 resolvToken += G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_CODE_POST, G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserDocPk)  + " ";
 resolvToken += G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_VILLE,     G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserDocPk) ;
 return resolvToken.replace("\n","<br />");
}
//-------------------------- EMAIL_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EMAIL_MEDECIN(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_EMAIL,       G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserDocPk);
}
//-------------------------- TEL_1_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::TEL_1_MEDECIN(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_TEL_1,       G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserDocPk);
}
//-------------------------- TEL_2_MEDECIN -------------------------------------------//-------------------------- TEL_1_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::TEL_2_MEDECIN(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_TEL_2,       G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserDocPk);
}
//-------------------------- TEL_3_MEDECIN -------------------------------------------//-------------------------- TEL_1_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::TEL_3_MEDECIN(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_TEL_3,       G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserDocPk);
}

//-------------------------- ADRESSE COMPLETE PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::ADRESSE_COMPLETE_PATIENT(QStringList &)
    {QString resolvToken("");
     resolvToken  = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_RUE,       G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey)+ "\n";
     resolvToken += G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_CODE_POST, G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey)+ " ";
     resolvToken += G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_VILLE,     G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
     return resolvToken.replace("\n","<br />");
    }

//-------------------------- ADRESSE MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::ADRESSE_MEDECIN(QStringList &)
    {QString resolvToken("");
     resolvToken  = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_RUE, G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
     return resolvToken.replace("\n","<br />");
    }
//-------------------------- ADRESSE PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::ADRESSE_PATIENT(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_RUE,        G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey);
    }
//-------------------------- ADRESSE PATIENT UNE LIGNE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::ADRESSE_PATIENT_UNE_LIGNE(QStringList &)
    {QString resolvToken("");
     resolvToken  = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_RUE,       G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey)+ " ";
     resolvToken += G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_CODE_POST, G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey)+ " ";
     resolvToken += G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_VILLE,     G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey);
     return resolvToken.replace("\n"," ");
    }

//-------------------------- AGE PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::AGE_PATIENT(QStringList &)
    {QVariant qvar;
     G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_NSS, G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey, &qvar);
     QDate qDteNss = qvar.toDate();
     return  G_mCDC->m_pMB->CalculeDiffAge(&qDteNss);
    }

//-------------------------- AGE PATIENT ABREGE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::AGE_PATIENT_ABREGE(QStringList &)
    {QVariant qvar;
     G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_NSS, G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey, &qvar);
     QDate qDteNss = qvar.toDate();
     return G_mCDC->m_pMB->CalculeDiffAge(&qDteNss);
    }
//-------------------------- AGE PATIENT ANNEE ABREGE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::AGE_PATIENT_ANNEE_ABREGE(QStringList &)
    {QString resolvToken("");
     QVariant qvar;
     G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_NSS, G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey, &qvar);
     QDate qDteNss = qvar.toDate();
     resolvToken   = G_mCDC->m_pMB->CalculeDiffAge(&qDteNss);
     return resolvToken.left(resolvToken.length()-5);
    }
//-------------------------- BMI PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::BMI_PATIENT(QStringList &)
    {   //....... Utilise la fonction calc pour renvoyer la bonne valeur
        QString resolvToken("");
        QString un      = "1";
        QString mult    = "*";
        QString demande = "BMI";
        CDevilCalc *pCalc = new CDevilCalc(G_mCDC);
        if (pCalc)
           {resolvToken = pCalc->Calc(un, mult, demande);
            delete pCalc;
           }
        return resolvToken;
    }
//-------------------------- CODE POSTAL PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::CODE_POSTAL_PATIENT(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_CODE_POST,  G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey);
    }
//-------------------------- BMI PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::CONVENTION_MEDECIN(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_CONVENTION, G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
    }

//-------------------------- BMI PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::CP_MEDECIN(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_CODE_POST, G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
    }
//-------------------------- DATECOURANTE -------------------------------------------
// DATE TIRET DATE DD-MM-YYYY CURRENT DATE DATECOURANTE DT
/*! \brief non documente */
QString C_TokenInterpret::DATECOURANTE(QStringList &arg_list)
{QString   frm = "dd-MM-yyyy";
 if (arg_list.count()>0) frm = arg_list[0].stripWhiteSpace();
 QDateTime qdt = QDateTime::currentDateTime ( Qt::LocalTime );
 return qdt.toString ( frm ).replace('|','h');
}

//-------------------------- DATE VISITE -------------------------------------------
//DATE OBSERVATION
/*! \brief non documente */
QString C_TokenInterpret::DATE_VISITE(QStringList &)
    {CRubRecord  *pCRubRecord = GetIDCurrentDoc(TR("Observation"));
     if (pCRubRecord)
        {QString  dt = pCRubRecord->m_Date.left(10);
         return   dt.mid(8) + "-" + dt.mid(5,2) + "-" + dt.left(4);
        }
     return QString::null;
    }

//-------------------------- DATE_DOCUMENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::DATE_DOCUMENT(QStringList &arg_list)
    {if (arg_list.count()==0) return TR("Syntax error in DATE DOCUMENT '=' not found");
     QString arg      = arg_list[0];
     int pos_deb      = 0;
     int pos_end      = arg.find("FORMAT");
     if (pos_end==-1)         return TR("Syntax error in DATE DOCUMENT 'FORMAT' not found");

     QString doc_typ  = arg.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();
     pos_deb          = arg.find('=', pos_end + 6);
     if (pos_deb==-1)         return TR ( "Syntax error in DATE DOCUMENT '=' after 'FORMAT' not found");

     ++pos_deb;    // passer le signe egal
     QString format   = arg.mid(pos_deb);

     if (doc_typ.length()==0) return TR("Syntax error in DATE DOCUMENT Document type missed after 'DATE DOCUMENT'");
     if (format.length()==0)  return TR("Syntax error in DATE DOCUMENT format date  missed after 'FORMAT'");
     //int  id = GetIDCurrentDoc(typ_doc.toLong());
     CRubRecord  *pCRubRecord;
     if (doc_typ=='*' ||doc_typ.upper() == "THIS") pCRubRecord = G_mCDC->m_pCRubCurrentRecord;
     else                                          pCRubRecord = GetIDCurrentDoc(doc_typ);
     if (pCRubRecord==0)      return TR("Syntax error in DATE DOCUMENT Document type : '") + doc_typ + "' not found";
     QDateTime    qdt =  pCRubRecord->getDateTime(); // QDate::fromString ( pCRubRecord->m_Date, Qt::ISODate ); // pCRubRecord->getDateTime()
     return  qdt.toString (format);
    }
//-------------------------- DATE_NAISSANCE_ASSURE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::DATE_NAISSANCE_ASSURE(QStringList &arg_list)
    {QString format = "dd-MM-yyyy"; if (arg_list.count()>0) format = arg_list[0].stripWhiteSpace();
     QVariant qvar;
     G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_NSS_ASS, G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey, &qvar);
     QDate qDteNss = qvar.toDate();
     return qDteNss.toString(format);
    }

//-------------------------- DATE_NAISSANCE_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::DATE_NAISSANCE_PATIENT(QStringList &arg_list)
    {QString format = "dd-MM-yyyy"; if (arg_list.count()>0) format = arg_list[0].stripWhiteSpace();
     QVariant qvar;
     G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_NSS, G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK , G_mCDC->m_IdentPrimKey, &qvar);
     QDate qDteNss = qvar.toDate();
     return qDteNss.toString(format);
    }
//-------------------------- DDR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::DDR(QStringList &arg_list)
    {   QString ret = GetDDR();
        QString frm = "dddd dd MMM yyyy";
        if (arg_list.count()>0) frm = arg_list[0].stripWhiteSpace();
        if (ret != " ")
          {  QDate DDR;
             DDR = DDR.fromString(ret , Qt::ISODate);
             ret = DDR.toString( frm ).replace('|','h');
          }
        return ret;
    }

//-------------------------- EMAIL_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::EMAIL_PATIENT(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME,
                                         G_mCDC->m_pMB->m_DOSS_IDENT_EMAIL, G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK,
                                         G_mCDC->m_IdentPrimKey);
    }

//-------------------------- LINE_EDIT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::LINE_EDIT(QStringList &arg_list)
    {QString title       = TR("Saisir du texte");
     QString text        = "";
     QString message     = "";
     QString imagePath   = "";
     int            nb   = arg_list.count();
     if (nb>0) text      = arg_list[0];
     if (nb>1) title     = arg_list[1];
     if (nb>2) message   = arg_list[2];
     if (nb>3) imagePath = G_pCApp->resolvePath(arg_list[3]);

     if (CGenTools::DialogGetString(message,0, title, text,imagePath)==QDialog::Accepted)
        {return text;
        }
     else return QString::null;
    }

//-------------------------- GET_LAST_IMAGE_NAME -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::GET_LAST_IMAGE_NAME(QStringList &)
    {QString resolvToken="";
     if (G_pCApp->m_pDrTux)
        {MyEditText *pMyEditText  = G_pCApp->m_pDrTux->currentEditor();
         if (pMyEditText)
            {resolvToken = pMyEditText->GetLastImageName();
             int     pos = resolvToken.findRev('.');
             if ( pos   != -1) {resolvToken = resolvToken.left(pos) + ".png";}
            }
        }
     return resolvToken;
    }
//-------------------------- GET_LAST_IMAGE_SRC_NAME -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::GET_LAST_IMAGE_SRC_NAME(QStringList &)
    {if (G_pCApp->m_pDrTux)
        {MyEditText *pMyEditText  = G_pCApp->m_pDrTux->currentEditor();
         if (pMyEditText) return G_pCApp->m_C_ImageList.getSrcImageName(pMyEditText->GetLastImageName());
        }
     return QString::null;
    }

//-------------------------- GET_SAVE_FILE_NAME -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::GET_SAVE_FILE_NAME(QStringList &arg_list)
    { if (arg_list.count()==0) return QString::null;
      QString   preselect = arg_list[0];
      QString      filter = TR("Tous fichiers (*)");
      int             pos = preselect.find(',');
      if (   pos != -1)
         {filter    = preselect.mid(pos+1).stripWhiteSpace();
          preselect.truncate(pos);
         }
      return QFileDialog::getSaveFileName(G_pCApp->resolvePath(preselect), filter, 0, TR("Enregistrer sous ..."), TR("Choisissez un nom de fichier pour enregistrer"));
    }
//-------------------------- HHHMM -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::HHHMM(QStringList &arg_list)
    {QDateTime qdt = QDateTime::currentDateTime ( Qt::LocalTime );
     QString frm = "hh:mm";
     if (arg_list.count()>0) frm = arg_list[0].stripWhiteSpace();
     return qdt.toString ( frm );
    }

//-------------------------- IDENTIFICATION_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::IDENTIFICATION_MEDECIN(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_NUM_ORDRE, G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
    }
//-------------------------- INTERVENANT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::INTERVENANT(QStringList &arg_list) //{{INTERVENANT = Medecine generale CHAMPS DESIRES = NPA DECALAGE= SEP=}}
    {if (arg_list.count()==0) return TR("Syntax error in INTERVENANT '=' not found");
     QStringList listData;
     QString sep = "";
     QString      token  = arg_list[0];
     int pos_end         = token.find("CHAMPS DESIRES");
     if (pos_end==-1)         return TR ("Syntax error in INTERVENANT 'CHAMPS DESIRES' not found");
     QString qualite     = token.mid(0, pos_end).stripWhiteSpace();
     int pos_deb         = token.find('=', pos_end + 14);
     if (pos_deb==-1)         return TR ("Syntax error in INTERVENANT '=' after 'CHAMPS DESIRES' not found");
     ++pos_deb;    // passer le signe egal
     pos_end          = token.find("DECALAGE", pos_deb);
     if (pos_end==-1)         return TR ("Syntax error in INTERVENANT 'DECALAGE' not found");
     QString validStr = token.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();
     pos_deb          = token.find('=', pos_end + 8);
     if (pos_deb==-1)         return TR ("Syntax error in INTERVENANT '=' after 'DECALAGE' not found");
     ++pos_deb;    // passer le signe egal
     pos_end          = token.find("SEP", pos_deb);
     QString decalage = "";
     if (pos_end == -1)  decalage = token.mid(pos_deb);
     else               {decalage = token.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();
                         pos_deb  = token.find('=', pos_end + 3);
                         if (pos_deb==-1)         return TR ("Syntax error in INTERVENANT '=' after 'SEP' not found");
                         ++pos_deb;
                         sep      = token.mid(pos_deb);
                        }
     G_mCDC->m_pMB->PatientIntervenantsGetData(G_mCDC->m_IdentPrimKey , listData, qualite, validStr, decalage);
     if (listData.count())
        {if (sep.length()==0) return listData[0];  // on prend le premier medecin traitant de la liste
         else                                      // on affiche toute la liste des infos en la separant par 'sep'
             {QStringList::Iterator it = listData.begin();
              QString resolvToken(*it);
              ++it;
              while  (it != listData.end() )
                  {resolvToken += sep + *it;
                   ++it;
                  }
              return resolvToken;
             }
        }
     return QString::null;
    }
//-------------------------- NOM_NAISSANCE_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOM_NAISSANCE_PATIENT( QStringList&)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME,  G_mCDC->m_pMB->m_DOSS_IDENT_JFNOM,
                                         G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,   G_mCDC->m_IdentPrimKey);
    }
//-------------------------- NOM_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOM_PATIENT( QStringList&)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_INDEX_TBL_NAME,  G_mCDC->m_pMB->m_DOSS_INDEX_NOM,
                                         G_mCDC->m_pMB->m_DOSS_INDEX_PRIM_KEY , G_mCDC->m_IdentPrimKey);
    }
//-------------------------- NOM_PRENOM_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOM_PRENOM_PATIENT(QStringList &arg_list)
    { QString sep = arg_list.count()?arg_list[0]:" ";
      return         G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_INDEX_TBL_NAME,  G_mCDC->m_pMB->m_DOSS_INDEX_NOM,
                                                  G_mCDC->m_pMB->m_DOSS_INDEX_PRIM_KEY , G_mCDC->m_IdentPrimKey) + sep +
                     G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_INDEX_TBL_NAME,  G_mCDC->m_pMB->m_DOSS_INDEX_PRENOM,
                                                  G_mCDC->m_pMB->m_DOSS_INDEX_PRIM_KEY , G_mCDC->m_IdentPrimKey);
    }
//-------------------------- POLITESSE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::POLITESSE_MEDECIN(QStringList &)
     {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_CHER,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
     }

//-------------------------- NOM_PRENOM_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOM_PRENOM_MEDECIN(QStringList &)
     {return        G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_NOM,
                                                 G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk) + " " +
                    G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_PRENOM,
                                                 G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
    }
//-------------------------- NOM_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOM_MEDECIN(QStringList &)
    {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME,    G_mCDC->m_pMB->m_USER_IDENT_NOM,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,    G_mCDC->m_UserDocPk);
    }
//-------------------------- NOM_PRENOM_ASSURE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOM_PRENOM_ASSURE(QStringList &)
    {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_NOM_ASS,
                                          G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey)+" "+
             G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_PRENOM_ASS,
                                          G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
    }
//-------------------------- NOM_PRENOM_INTERVENANT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOM_PRENOM_INTERVENANT(QStringList &arg_list)
    {if (arg_list.count()==0) return TR("Syntax error in NOM_PRENOM_INTERVENANT '=' not found");
     QStringList listPraticiens;
     QString qualite = arg_list[0].stripWhiteSpace();
     G_mCDC->m_pMB->PatientIntervenantsGetData(G_mCDC->m_IdentPrimKey , listPraticiens, qualite,"NP", " ");   //  "NP"  pour nom prenom intervenant
     if (listPraticiens.count())
        {return listPraticiens[0];  // on prend le premier medecin traitant de la liste
        }
     return QString::null;
    }

//-------------------------- NOMBRE_D_ENFANT_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NOMBRE_D_ENFANT_PATIENT(QStringList &)
    {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_NB_ENF,
                                          G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
    }

//-------------------------- NUM_TEL3_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NUM_TEL3_PATIENT(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_TEL3,
                                         G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
    }
//-------------------------- NUM_TEL2_PATIENT -------------------------------------------
/*! \brief non documente */
// HEURECOURANTE
QString C_TokenInterpret::NUM_TEL2_PATIENT(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_TEL2,
                                         G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
    }
//-------------------------- NUM_TEL1_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NUM_TEL1_PATIENT( QStringList&)
    {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_TEL1,
                                          G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
    }
//-------------------------- NUM_CLEF_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NUM_CLEF_PATIENT(QStringList &)
    {QString ns = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME,  G_mCDC->m_pMB->m_DOSS_IDENT_NUMSS,
                                         G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,   G_mCDC->m_IdentPrimKey).left(13);
     return G_mCDC->m_pMB->ComputeClef_Secu(ns);
    }
//-------------------------- NUM_SECU_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::NUM_SECU_PATIENT(QStringList &arg_list)
    {QString ns = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME,  G_mCDC->m_pMB->m_DOSS_IDENT_NUMSS,
                                               G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,   G_mCDC->m_IdentPrimKey);
     int nb = arg_list.count();
     if (nb==0)                                            return ns;
     ns = ns.left(13);
     if (arg_list[0].stripWhiteSpace().lower()=="$no key") return ns;
     return ns + arg_list[0] + G_mCDC->m_pMB->ComputeClef_Secu(ns);
    }
//-------------------------- OPEN_FILE_NAME -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::OPEN_FILE_NAME(QStringList &arg_list)
    {QString preselect      = "";
     QString dirPresel      = "";
     if (arg_list.count()>=1)  preselect = arg_list[0].stripWhiteSpace().replace("[","(").replace("]",")");
     if (arg_list.count()>=2)  dirPresel = G_pCApp->resolvePath(arg_list[1].stripWhiteSpace());
     if (G_pCApp->m_pDrTux) return  G_pCApp->GetImageFileName(&dirPresel, preselect, 1);
     return QString::null;
    }

//-------------------------- POIDS_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::POIDS_PATIENT(QStringList &)
    {   //....... Utilise la fonction calc pour renvoyer la bonne valeur
        QString un        = "1";
        QString mult      = "*";
        QString demande   = "POIDS";
        CDevilCalc calc(G_mCDC);
        return calc.Calc(un, mult, demande);
    }
//-------------------------- PRENOM_PATIENT -------------------------------------------
/*! \brief non documente */
// HEURECOURANTE
QString C_TokenInterpret::PRENOM_PATIENT(QStringList &)
    {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_INDEX_TBL_NAME,  G_mCDC->m_pMB->m_DOSS_INDEX_PRENOM,
                                          G_mCDC->m_pMB->m_DOSS_INDEX_PRIM_KEY , G_mCDC->m_IdentPrimKey);
    }
//-------------------------- PRENOM_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::PRENOM_MEDECIN(QStringList &)
    {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_PRENOM,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
    }
//-------------------------- PROFESSION_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::PROFESSION_PATIENT(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_PROF,
                                         G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
    }
//-------------------------- SEXE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SEXE_MEDECIN(QStringList &)
    {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_SEXE,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
    }
//-------------------------- SEXE_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SEXE_PATIENT(QStringList &arg_list)
    {QString sexe = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_SEX,
                                                 G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
     if (arg_list.count()>0) return sexe;
     if (sexe=="F")          return TR("Feminin");
     else                    return TR("Masculin");
    }
//-------------------------- SPECIALITE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SPECIALITE_MEDECIN(QStringList &)
    {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_QUALITE,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
    }
//-------------------------- SAVE_LAST_IMAGE_NAME -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SAVE_LAST_IMAGE_NAME(QStringList &arg_list)
    {     if (arg_list.count()==0) return TR("Syntax error in SAVE LAST IMAGE NAME '=' not found");
          QPixmap retPixMap;
          QString path = G_pCApp->resolvePath(arg_list[0]);
          if ( !path.endsWith("/") ) path += "/";
          MyEditText *pMyEditText  = G_pCApp->m_pDrTux->currentEditor();
          if ( pMyEditText && QFile::exists( path ) ) // si repertoire existe
             {path   += pMyEditText->GetLastImageName();
              int pos = path.findRev('.');
              if ( pos != -1) {path = path.left(pos) + ".png"; }
              if (G_pCApp->m_C_ImageList.getPixmap( pMyEditText->GetLastImageName() , retPixMap))
                 { QImageIO iio;
                   QImage   im;
                   im = retPixMap; // convert to image
                   iio.setImage( im );
                   iio.setFileName( path );
                   iio.setFormat( "PNG" );
                   iio.write();
                 }
             }
          return QString::null;
    }
//-------------------------- SAVE_CURRENT_DOC -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SAVE_CURRENT_DOC(QStringList &arg_list)
    {     if (arg_list.count()==0) return TR("Syntax error in SAVE_CURRENT_DOC () first argument path not found");
          QString path = G_pCApp->resolvePath(arg_list[0]);
          MyEditText *pMyEditText  = G_pCApp->m_pDrTux->currentEditor();
          if ( pMyEditText  ) // si repertoire existe
             {QString text = pMyEditText->text();
              CGestIni::Param_UpdateToDisk(path,text);
             }
          return QString::null;
    }
//-------------------------- KEY_VALUE -------------------------------------------
/*! \brief extrait et retourne la valeur d'une clef d'une section dans un texte de type \
 *     [SECTION]
 *         key = value
 *  \param arg_list : liste d'arguments dans laquelle :
 *                    1er  argument est le texte dans lequel retrouver la valeur
 *                    2eme argument est la section : [SECTION] 
 *                    3eme argument est la clef : key
 *                    4eme argument est le nombre de valeurs que la clef comporte. par defaut a 1
 *       si un seul argument alors il est retourne comme valeur de retour
 *       si plusieurs arguments alors ils seront retournes dans des variables ayant pour nom
 *          le_nom_de_la_clef_0 le_nom_de_la_clef_1 le_nom_de_la_clef_2 ..... le_nom_de_la_clef_n
*/
QString C_TokenInterpret::KEY_VALUE(QStringList &arg_list)
{  int nbValues = 0;
   if (arg_list.count()==0) return TR("Syntax error in  KEY_VALUE() first argument 'datas' where to find not found");
   if (arg_list.count()==1) return TR("Syntax error in  KEY_VALUE() second argument 'section' where to find not found");
   if (arg_list.count()==2) return TR("Syntax error in  KEY_VALUE() third argument 'key' from where find value not found");
   if (arg_list.count()==3) nbValues = qMin(arg_list[3].stripWhiteSpace().toInt(),10);
   QString datas;
   if (arg_list[0].stripWhiteSpace()=="::param") datas = G_pCApp->m_DrTuxParam;
   else                                          datas = arg_list[0].stripWhiteSpace();
   if (nbValues<=1) 
      { return CGestIni::Param_ReadUniqueParam( datas,
                                                arg_list[1].stripWhiteSpace(),
                                                arg_list[2].stripWhiteSpace());
      }
   else
      { QStringList valuesList = QStringList()<<""<<""<<""<<""<<""<<""<<""<<""<<""<<"";    //  QString X 10
        QString key = arg_list[2].stripWhiteSpace();
        QString ret = CGestIni::Param_ReadParam(  datas                         , arg_list[1].stripWhiteSpace() , key,
                                                  &valuesList[0], &valuesList[1], &valuesList[2], &valuesList[3], &valuesList[4],
                                                  &valuesList[5], &valuesList[6], &valuesList[7], &valuesList[8], &valuesList[9]);
        for (int i=0; i<nbValues; ++i)
            { (*G_mCDC->m_pVariables)[key+"_"+QString::number(i)]=valuesList[i];       // on cree le nombre de variable de type key_1  key_2 .... key_n
            }
        return ret==QString::null?"1":ret;
      }
}

//-------------------------- SURFACE_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::SURFACE_PATIENT(QStringList &)
    {   //....... Utilise la fonction calc pour renvoyer la bonne valeur
        QString un      = "1";
        QString mult    = "*";
        QString demande = "SURFACE";
        CDevilCalc calc(G_mCDC);
        return calc.Calc(un, mult, demande);
    }   // Fin == S
//-------------------------- TITRE_PS -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::TITRE_PS(QStringList &)
    {return        G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_TITRE,
                                                G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
    }
//-------------------------- TITRE_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::TITRE_PATIENT(QStringList &)
    {QString resolvToken = "";
     // QString resolvToken = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_TITRE,
     //                                                    G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
     if (resolvToken=="")
        {QVariant qvar;
         G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_NSS,
                                      G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey, &qvar);
         QDate qDteNss = qvar.toDate();
         int age = atoi(G_mCDC->m_pMB->CalculeDiffAge(&qDteNss, 0, FALSE, 2));
         if (age < 15)          resolvToken="L'enfant";
         if (age >= 15 && age <= 21)
            {QString sexe = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_SEX,
                                                         G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
             if (sexe=="F")     resolvToken="Mademoiselle";
             else               resolvToken="M.";
            }
         if (age>21)
            {QString sexe = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_SEX,
                                                         G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
             if (sexe=="F")     resolvToken="Mme";
             else               resolvToken="M.";
            }
        }
     return resolvToken;
    }
//-------------------------- TAILLE_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::TAILLE_PATIENT(QStringList &)
    {   //....... Utilise la fonction calc pour renvoyer la bonne valeur
        QString un      = "1";
        QString mult    = "*";
        QString demande = "TAILLE";
        CDevilCalc calc(G_mCDC);
        return calc.Calc(un, mult, demande);
    }
//-------------------------- TERME_GROSSESSE -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::TERME_GROSSESSE(QStringList &)
    {   QString ret = "";
        ret = GetDDR();
        if (ret[0] == 'L')  return QString(" ");
        else                return Terme_Obst ( ret , QDate::currentDate());
    }
//-------------------------- USER_ADRESSE_COMPLETE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_ADRESSE_COMPLETE_MEDECIN(QStringList &)
     {QString resolvToken;
      resolvToken  = G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME,       G_mCDC->m_pMB->m_USER_IDENT_RUE,
                                                  G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk)  + "\n"+
                     G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME,       G_mCDC->m_pMB->m_USER_IDENT_CODE_POST,
                                                  G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk)  + " " +
                     G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME,       G_mCDC->m_pMB->m_USER_IDENT_VILLE,
                                                  G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk) ;
      return (resolvToken.replace("\n","<br />"));
     }

//-------------------------- USER_RPPS -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_RPPS(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_NUM_RPPS,    G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk);
}
//-------------------------- USER_CLEF_RPPS -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_CLEF_RPPS(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_CLEF_RPPS,   G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk);
}
//-------------------------- USER_GUID -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_GUID(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_GUID,        G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk);
}
//-------------------------- USER_EMAIL_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_EMAIL_MEDECIN(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_EMAIL,       G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk);
}
//-------------------------- USER_TEL_1_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_TEL_1_MEDECIN(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_TEL_1,       G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk);
}
//-------------------------- USER_TEL_2_MEDECIN -------------------------------------------//-------------------------- TEL_1_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_TEL_2_MEDECIN(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_TEL_2,       G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk);
}
//-------------------------- USER_TEL_3_MEDECIN -------------------------------------------//-------------------------- TEL_1_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_TEL_3_MEDECIN(QStringList &)
{return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_TEL_3,       G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,       G_mCDC->m_UserActuelPk);
}

//-------------------------- USER_ADRESSE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_ADRESSE_MEDECIN( QStringList&)
     {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_RUE,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk).replace("\n","<br />");
     }
//-------------------------- USER_CP_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_CP_MEDECIN(QStringList &)
     {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_CODE_POST,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
     }
//-------------------------- USER_CONVENTION_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_CONVENTION_MEDECIN(QStringList &)
     {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_CONVENTION,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
     }
//-------------------------- USER_NOM_PRENOM_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_NOM_PRENOM_MEDECIN(QStringList &)
     {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_NOM,
                                           G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk) + " " +
              G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_PRENOM,
                                           G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
     }
//-------------------------- USER_POLITESSE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_POLITESSE_MEDECIN(QStringList &)
     {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_CHER,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
     }
//-------------------------- USER_NOM_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_NOM_MEDECIN(QStringList &)
     {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_NOM,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
     }
//-------------------------- USER_SEXE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_SEXE_MEDECIN(QStringList &)
     {return  G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME,  G_mCDC->m_pMB->m_USER_IDENT_SEXE,
                                           G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY ,  G_mCDC->m_UserActuelPk);
     }
//-------------------------- USER_SPECIALITE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_SPECIALITE_MEDECIN(QStringList &)
     {return   G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_QUALITE,
                                            G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
     }
//-------------------------- USER_TITRE_PS -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_TITRE_PS(QStringList &)
     {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_TITRE,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
     }
//-------------------------- USER_VILLE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_VILLE_MEDECIN(QStringList &)
     {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_VILLE,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
     }
//-------------------------- USER_LOGIN_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_LOGIN_MEDECIN(QStringList &)
     {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_LOGIN,
                                          G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
     }

//-------------------------- USER_IDENTIFICATION_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_IDENTIFICATION_MEDECIN(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_NUM_ORDRE,
                                         G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
    }
//-------------------------- USER_PRENOM_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::USER_PRENOM_MEDECIN(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_PRENOM,
                                         G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserActuelPk);
    }
//-------------------------- VILLE_PATIENT -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::VILLE_PATIENT(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_DOSS_IDENT_TBL_NAME, G_mCDC->m_pMB->m_DOSS_IDENT_VILLE,
                                         G_mCDC->m_pMB->m_DOSS_IDENT_REF_PK ,  G_mCDC->m_IdentPrimKey);
    }
//-------------------------- VILLE_MEDECIN -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::VILLE_MEDECIN(QStringList &)
    {return G_mCDC->m_pMB->GetFieldValue(G_mCDC->m_pMB->m_USER_IDENT_TBL_NAME, G_mCDC->m_pMB->m_USER_IDENT_VILLE,
                                         G_mCDC->m_pMB->m_USER_IDENT_PRIMKEY , G_mCDC->m_UserDocPk);
    }
//-------------------------- WAIT_CURSOR -------------------------------------------
/*! \brief non documente */
QString C_TokenInterpret::WAIT_CURSOR (QStringList &arg_list)
    {
          if (arg_list[0]=="RESTORE"||arg_list[0]=="NORMAL")
             { QApplication::restoreOverrideCursor();
             }
          else
             {QApplication::setOverrideCursor ( Qt::WaitCursor );
             }
          return "";
    }

/////////////////////////////////// FONCTIONS DE SERVICE ////////////////////////////
//----------------------- Func_Extract_TTT ------------------------------------------
/*! \brief Retourne le traitement en cours du patient.
*/
QString C_TokenInterpret::Func_Extract_TTT( QString title, QString linePatern, QString renouvInterc, QString tableau, QString poids_name, QString taille_name)
{QString  str_data = G_mCDC->m_pMB->GetDataFromRubList( GetIDCurrentDoc(TR("Terrain")) );       // recuperer donnees soit dans liste cache soit sur disque
 if (str_data.length()==0)              return QString("");
 return Func_Extract_TTT_D(str_data, title, linePatern, renouvInterc, tableau, poids_name, taille_name);
}
//----------------------- Func_Extract_TTT ------------------------------------------
/*! \brief Retourne le traitement en cours du patient.
*/
QString C_TokenInterpret::Func_Extract_TTT_D(const QString  &str_data, QString title, QString linePatern, QString , QString tableau, QString poids_name, QString taille_name)
{QString           resolvToken = "";
 QString           poids_var   = "";
 QString           taille_var  = "";

 QPL_CPosologie    ordoList;
 QStringList       lineList;

 char* pt          = (char*) (const char*) str_data;
 //renouvInterc      = renouvInterc.stripWhiteSpace();

 //................ recuperer le poids et taille .................................................
 GetPoidsTaille(pt, tableau, poids_name, taille_name, poids_var, taille_var);
 //........... effacer les anciennes tables de variables ................
 //
 QString val1, val2, val3, val4, val5, val6, val7, val8, val9, val10;
 QString      section   = "";
 QString     var_name   = "";

 char     *prop_section = 0;       // permet de noter le pointeur sur la section propriete renouvelable/intercurent des produits

 while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
  {if (section == TR("Propriete Ordonnance"))                  // à ce stade pt pointe au debut de la nouvelle ligne
        {prop_section = pt;                                // noter section des proprietes (à traiter apres liste med)
         section      = "";
        }
     //.................................. TTT ........................................................
     // IMPORTANT: cette section doit tjrs etre en dernier dans le fichier pour des raisons de fin !!!
     if (section==TR("Traitement"))                  // à ce stade pt pointe au debut de la nouvelle ligne
      {  QString stringStruct;
         CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(pt, 0, &stringStruct); // y isoler et recuperer les donnees calculables
         CMedicaBase::Medica_PosologieListDeserialize(stringStruct, ordoList);

         if (linePatern.contains("tableau", FALSE) != 0)
          {     QString tabTTT = "<table width=\"100%\" border=\"1\" cellpadding=\"2\" cellspacing=\"2\">";
                tabTTT += "<tr><td width=\"50%\">"+TR("Medicament")+"</td><td width = \"10%\">"+TR("Matin")+"</td>";
                tabTTT += "<td width = \"10%\">"+TR("Midi")+"</td><td width = \"10%\">"+TR("Soir")+"</td>";
                tabTTT += "<td width = \"10%\">"+TR("Coucher")+"</td></tr>";
                int nb = 0;
                QString libMed = "";
                for (QPL_CPosologie::iterator it = ordoList.begin(); it != ordoList.end(); ++it )
                  {  ++ nb; (*it).m_NumOrdre = nb;
                     libMed  = G_mCDC->m_pMD->Medica_GetMedicamentNameByCIP((*it).m_MEDICA_POSO_CIP);
                     libMed  = libMed.section (" ", 0, 1); // fait de la place dans le libelle du medicament...
                     if ((*it).m_ALD == 1) // en ALD
                        { libMed.append("<span style=\"font-weight:600\">*</span>"); }
                     tabTTT += "<tr><td width=\"50%\">"+libMed+"</td>";
                     QString tmpPoso = (*it).m_MEDICA_POSO_REPART_PRISES;
                     if (tmpPoso.contains("MA", FALSE) != 0)
                        { tabTTT += "<td width = \"10%\">&nbsp;X&nbsp;</td>";}
                     else
                        { tabTTT += "<td width = \"10%\">&nbsp;</td>";}
                     if (tmpPoso.contains("MI", FALSE) != 0)
                        { tabTTT += "<td width = \"10%\">&nbsp;X&nbsp;</td>";}
                     else
                        { tabTTT += "<td width = \"10%\">&nbsp;</td>";}
                     if (tmpPoso.contains("SO", FALSE) != 0)
                        { tabTTT += "<td width = \"10%\">&nbsp;X&nbsp;</td>";}
                     else
                        { tabTTT += "<td width = \"10%\">&nbsp;</td>";}
                     if (tmpPoso.contains("CO", FALSE) != 0)
                        { tabTTT += "<td width = \"10%\">&nbsp;X&nbsp;</td>";}
                     else
                        { tabTTT += "<td width = \"10%\">&nbsp;</td>";}
                  } // Fin for
             tabTTT += "</table><br />";
             return tabTTT;
          } // fin if tableau

         int nb = 0;
         //int medALD = 0;
         QString tmp;
         for (QPL_CPosologie::iterator it = ordoList.begin(); it != ordoList.end(); ++it )
             {++ nb; (*it).m_NumOrdre = nb;

              QString lineToInsert = linePatern;

              lineToInsert.replace("::NUM::", QString::number(nb));
              lineToInsert.replace("::LIBELLE::", G_mCDC->m_pMD->Medica_GetMedicamentNameByCIP((*it).m_MEDICA_POSO_CIP));
              lineToInsert.replace("::NB_PRISE::", (*it).m_MEDICA_POSO_NB_PRISES);
              lineToInsert.replace("::REPART_PRISES::", (*it).m_MEDICA_POSO_REPART_PRISES);
              if ((*it).m_ALD == 1)      { tmp = TR("En ALD");   }
              else if ((*it).m_ALD == 0) { tmp = TR("Hors ALD"); }
              lineToInsert.replace("::ALD::", tmp);
              lineToInsert.replace("::POSO_TEXTUELLE::", (*it).m_PosoTextuelle);
              lineToInsert.replace("::NOTE::", (*it).m_Note);
              lineToInsert.replace("::DUREE::", (*it).m_MEDICA_POSO_PENDANT);

              // m_pCMedicaBase->Medica_PosologieToSimpleText(*it, m_pCMDI_Terrain->GetPoids(), m_pCMDI_Terrain->GetTaille() );
              lineList.append(lineToInsert);
             } //end for

      break;
     } // end if section
  } // end while((pt=m_pMB->Param_GotoNextSection(pt, 0, &section)) && *pt)
 for ( QStringList::Iterator lt = lineList.begin(); lt != lineList.end(); ++lt )
     {resolvToken += *lt;
     }
 if (resolvToken.length()) resolvToken.prepend(title);

 return    resolvToken;
}
/*
VAR Constantes de Base]
   Modèle = Date, Pouls, 31536000, 130, 40, pps, Tachycardie, Bradycardie
   Modèle = Date, TA Syst, 31536000, 160, 80, pps, Hypertension, Hypotension
   Modèle = Date, TA Dias, 31536000, 90, 40, pps, Hypertension, Hypotension
   Modèle = Date, SAO2, 31536000, 0, 92, %, , Hypoxie
   Modèle = Date, EN, 31536000, 10, 1, 1à10, Hyporthermie, Hyperthermie
   Modèle = Date, Temp, 31536000, 40.5, 35, degres, Hyporthermie, Hyperthermie
   Modèle = Date, HGT, 31536000, 0.6, 5, g/L, Hypoglycemie critique, Hypeglycemie critique
    2010-06-15T15:51:20 = 135, 210, 110, 95, 8, 40.1, 1.10
[VAR Poids]
   Modèle = Date, Poids, 31536000, 100, 50, Kg(s), Obesite, Amaigrissement
   Modèle = Date, Taille, 31536000, 0, 0, cm(s), ,
    2010-06-15T15:51:53 = 80,
*/

//---------------------- constructExportExportTableDefinition ----------------------------------
/*! \brief Cree une chaine de parametres a placer en entree des focntions d'affichage permettant d'afficher toutes les tables et ses valeurs.
 *  \param QString     &data : donnees du terrain
 *  \param QStringList &list_SECTION sera alimente par le nom de la section relative a un tableau suivie des parametres
 *  \return QString du style : VAR Constantes de Base[Pouls=50  ; SAO2 =80  ; Temp =100] VAR Poids[Poids =50  ; Taille =80]
*/
QString C_TokenInterpret::constructExportExportTableDefinition(const QString &str_data, QStringList &list_SECTION)
{    QStringList   modelesList;
     QStringList   valuesList;
     list_SECTION.clear();
     QString         modele  = "";
     QString strToConstruct  = "";
     QString        section  = "";
     char               *pt  = (char*) (const char*) str_data;
     while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
          {//............ si une section est une section correspondant a un tableau ....................
           if ( section.left(3) == "VAR" )                  // à ce stade pt pointe au debut de la nouvelle ligne
              {QString sectionFind = section;
               //.............. relever liste des variables de ce tableau .........................
               CGestIni::Param_GetList(str_data, section, "Mod",  modelesList , 1);
               int nb_Col = modelesList.count();
               if (nb_Col)
                  {int l = 100 / nb_Col;
                   //int r = 100 - ( l*100 );
                   int i = 0;
                   sectionFind += "[";
                   //.......... batir la chaine de definition des colonnes du tableau ............
                   for ( QStringList::Iterator it = modelesList.begin(); it != modelesList.end(); ++it )
                       { modele       = *it;
                         valuesList   = QStringList::split(',' ,  modele);
                         sectionFind +=  valuesList[1] += "=" + QString::number(l) + "%";
                         ++i;
                         if (i != (int) modelesList.count()) sectionFind +=  ";";
                       }
                   list_SECTION.append(sectionFind + "]");
                   strToConstruct +=   sectionFind + "]";
                  } //eendif (nb_Col)
              } // end while (*pt && *pt != '[')
           section="";
          } // endif (section.left(3)=="VAR" )
 return strToConstruct;
}
//---------------------- Func_Export_Table ----------------------------------
/*! \brief Cree un tableau avec les variables du terrain en cours d'affichage.
*/
QString C_TokenInterpret::Func_Export_Tables(QString date_deb,             // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                                             QString date_end,             // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                                             QString exportDefinition,     // liste des tables a exportet style : VAR Constantes de Base[Pouls=50  ; SAO2 =80  ; Temp =100] VAR Poids[Poids =50  ; Taille =80]
                                             QString tableSep,             // motif de separation  entre les tables  <be> par defaut
                                             QString cell_W ,              // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                                             QString cell_H,               // hauteur de la cellule
                                             QString border_W,             // largeur de la bordure
                                             QString width,                // largeur du tableau
                                             QString dateTimeFormat,       // format d'affichage de la date
                                             QString dateTimeWidth)        // largeur de la cellule de la date

{//.................. recuperer les donnees du terrain courant ..........................
 QString  str_data = G_mCDC->m_pMB->GetDataFromRubList( GetIDCurrentDoc(TR("Terrain")) );       // recuperer donnees soit dans liste cache soit sur disque
 if (str_data.length()==0)              return QString("");
 return                 Func_Export_Tables_D(str_data,
                                             date_deb,             // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                                             date_end,             // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                                             exportDefinition,     // liste des tables a exportet style : VAR Constantes de Base[Pouls=50  ; SAO2 =80  ; Temp =100] VAR Poids[Poids =50  ; Taille =80]
                                             tableSep,             // motif de separation  entre les tables  <be> par defaut
                                             cell_W ,              // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                                             cell_H,               // hauteur de la cellule
                                             border_W,             // largeur de la bordure
                                             width,                // largeur du tableau
                                             dateTimeFormat,       // format d'affichage de la date
                                             dateTimeWidth);
}
//---------------------- Func_Export_Table_D ----------------------------------
/*! \brief Cree un tableau avec les variables du terrain en cours d'affichage.
*/
QString C_TokenInterpret::Func_Export_Tables_D(const QString &str_data,
                                               QString date_deb,             // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                                               QString date_end,             // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                                               QString exportDefinition,     // liste des tables a exporter style : VAR Constantes de Base[Pouls=50  ; SAO2 =80  ; Temp =100] VAR Poids[Poids =50  ; Taille =80]
                                               QString tableSep,             // motif de separation  entre les tables  <br> par defaut
                                               QString cell_W ,              // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                                               QString cell_H,               // hauteur de la cellule
                                               QString border_W,             // largeur de la bordure
                                               QString width,                // largeur du tableau
                                               QString dateTimeFormat,       // format d'affichage de la date
                                               QString dateTimeWidth)        // largeur de la cellule de la date

{QString       resolvToken   = "";
 QString   tableDefinition   = "";
 QString columnsDefinition   = "";
 QString         tableName   = "";
 QString              name   = "";
 QString               tmp   = "";
 date_deb                    = date_deb.stripWhiteSpace();
 date_end                    = date_end.stripWhiteSpace();
 exportDefinition            = exportDefinition.stripWhiteSpace();
 if (date_deb.length()==0) date_deb = "1900-01-01T00:00:00";
 if (date_end.length()==0) date_end = "3000-01-01T00:00:00";

 //.................. liste des tables dont il faut exporter les donnees .........................
 QStringList list_SECTION;
 if (exportDefinition.length()==0) exportDefinition = constructExportExportTableDefinition(str_data, list_SECTION);
 else                              list_SECTION     = QStringList::split(']', exportDefinition);
 //.................... exploiter la liste .........................................................

 for ( QStringList::Iterator it = list_SECTION.begin(); it != list_SECTION.end(); ++it )
     { tableDefinition   = *it;
       int posDebColDef  = tableDefinition.find("[");
       if (posDebColDef != -1 )
          {tableName     = tableDefinition.left(posDebColDef).stripWhiteSpace();
           ++posDebColDef;
           columnsDefinition = tableDefinition.mid(posDebColDef).stripWhiteSpace();
           name          = tableName; name = name.remove("VAR ").stripWhiteSpace();
           tmp           = tableSep;
           resolvToken  += tmp.replace("$TableName", tableName);
           resolvToken  += Func_Extract_Table_D(str_data,
                                                tableName,          // VAR Constantes de Base
                                                columnsDefinition,  // Pouls =50  ; SAO2 =80  ; Temp =100
                                                date_deb,           // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                                                date_end,           // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                                                cell_W ,            // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                                                cell_H,             // hauteur de la cellule
                                                border_W,           // largeur de la bordure
                                                width,              // largeur du tableau
                                                dateTimeFormat,     // format d'affichage de la date
                                                dateTimeWidth);     // largeur de la cellule de la date
          }
     }
 return resolvToken;
}

//---------------------- Func_Extract_Table ----------------------------------
/*! \brief Cree un tableau avec les variables du terrain en cours d'affichage.
 *  \todo Pour ameliorer le debuggage utiliser Var_Code
 * {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE ,   0  , TOUTES )}}
 * {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , -24  , PREMIERE )}}
 * {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , +24  , DERNIERE )}}
 * {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , +-24 , PLUS PROCHE )}}
 * {{:: Extract_Table (VAR Constantes de Base , Pouls=50 ; SAO2=80 ; Temp =100 ,DATE VISITE ,0  , TOUTES , 50 ,3)}}
*/
QString C_TokenInterpret::Func_Extract_Table(QString section_VAR,     // VAR Constantes de Base
                                             QString list_VAR,        // Pouls =50  ; SAO2 =80  ; Temp =100
                                             QString date_deb,        // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                                             QString date_end,        // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                                             QString cell_W ,         // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                                             QString cell_H,          // hauteur de la cellule
                                             QString border_W,        // largeur de la bordure
                                             QString width,           // largeur du tableau
                                             QString dateTimeFormat,  // format d'affichage de la date
                                             QString dateTimeWidth)   // largeur de la cellule de la date
{//.................. recuperer les donnees du terrain courant ..........................
 QString  str_data = G_mCDC->m_pMB->GetDataFromRubList( GetIDCurrentDoc(TR("Terrain")) );       // recuperer donnees soit dans liste cache soit sur disque
 if (str_data.length()==0)              return QString("");
 return Func_Extract_Table_D(str_data,
                             section_VAR,     // VAR Constantes de Base
                             list_VAR,        // Pouls =50  ; SAO2 =80  ; Temp =100
                             date_deb,        // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                             date_end,        // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                             cell_W ,         // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                             cell_H,          // hauteur de la cellule
                             border_W,        // largeur de la bordure
                             width,           // largeur du tableau
                             dateTimeFormat,  // format d'affichage de la date
                             dateTimeWidth
                            );
}

//---------------------- Func_Extract_Table_D ----------------------------------
/*! \brief Cree un tableau avec les variables du terrain en cours d'affichage.
 *  \todo Pour ameliorer le debuggage utiliser Var_Code
 * {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE ,   0  , TOUTES )}}
 * {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , -24  , PREMIERE )}}
 * {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , +24  , DERNIERE )}}
 * {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , +-24 , PLUS PROCHE )}}
 * {{:: Extract_Table (VAR Constantes de Base , Pouls=50 ; SAO2=80 ; Temp =100 ,DATE VISITE ,0  , TOUTES , 50 ,3)}}
*/
QString C_TokenInterpret::Func_Extract_Table_D(const QString &str_data,
                                               QString section_VAR,     // VAR Constantes de Base
                                               QString list_VAR,        // Pouls =50  ; SAO2 =80  ; Temp =100
                                               QString date_deb,        // DATE VISITE / TOUTE DATE / DATE ACTUELLE /        ou date de debut forme : 2004-03-15T17:14:48:00
                                               QString date_end,        // DATE VISITE / TOUTE DATE / DATE ACTUELLE / +-24 / ou date de fin   forme : 2004-03-15T17:14:48:00
                                               QString cell_W ,         // largeur de chaque cellule  si non indiquee  dans la chaine list_VAR
                                               QString cell_H,          // hauteur de la cellule
                                               QString border_W,        // largeur de la bordure
                                               QString width,           // largeur du tableau
                                               QString dateTimeFormat,  // format d'affichage de la date
                                               QString dateTimeWidth
                                             )   // largeur de la cellule de la date
{QString   resolvToken    = "";
 char*              pt    = (char*) (const char*) str_data;
 //..................... menage dans les espaces............................................
 section_VAR    = section_VAR.stripWhiteSpace();
 list_VAR       = list_VAR.stripWhiteSpace();
 date_deb       = date_deb.stripWhiteSpace();
 date_end       = date_end.stripWhiteSpace();
 cell_W         = cell_W.stripWhiteSpace();
 cell_H         = cell_H.stripWhiteSpace();
 border_W       = border_W.stripWhiteSpace();
 width          = width.stripWhiteSpace();
 dateTimeFormat = dateTimeFormat.stripWhiteSpace();
 dateTimeWidth  = dateTimeWidth.stripWhiteSpace();
 //..................... recuperer les dates et determiner le filtrage ............................................
 //                      resolution des macros  TOUTE DATE / DATE VISITE / DATE ACTUELLE
 int dateMustBeFiltred = 0;
 if ( ! (date_deb == "TOUTE DATE" && date_end == "TOUTE DATE"))
    {CRubRecord *pCRubRecord  = 0;
     //............. date de fin ...........................
     if (date_end == "DATE VISITE")
        {pCRubRecord = GetIDCurrentDoc(TR("Observation"));
         if (pCRubRecord)
            {date_end          = pCRubRecord->m_Date;
             date_end          = date_end.left(10) + "T23:59:59";           // DATE VISITE finit à 23h59 de la date
             dateMustBeFiltred = 1;
            }
        }
     else if (date_end == "TOUTE DATE")
        { date_end          = "3000-01-01T00:00:00";
          //date_end = QDateTime::currentDateTime().addYears(100).toString(Qt::ISODatve);  //"dd-MM-yyyyThh:mm:ss"
          dateMustBeFiltred = 1;
        }
     else if ( date_end == "DATE ACTUELLE")
        {date_end = QDateTime::currentDateTime().toString(Qt::ISODate);  //"dd-MM-yyyyThh:mm:ss"
         dateMustBeFiltred = 1;
        }
     //............. date de debut ...........................
     if (date_deb== "DATE VISITE")
        {if (pCRubRecord==0) pCRubRecord = GetIDCurrentDoc(TR("Observation"));
         if (pCRubRecord)
            {date_deb          = pCRubRecord->m_Date;
             date_deb          = date_deb.left(10) + "T00:00:00";      // DATE VISITE commence à 00h00 de la date
             dateMustBeFiltred = 1;
            }
         }
     else if ( date_deb == "TOUTE DATE" )
        { date_deb          = "1900-01-01T00:00:00";
          //date_deb = QDateTime::currentDateTime().addYears(-100).toString(Qt::ISODate);  //"dd-MM-yyyyThh:mm:ss"
          dateMustBeFiltred = 1;
        }
     else if ( date_deb == "DATE ACTUELLE")
        {date_deb = QDateTime::currentDateTime().toString(Qt::ISODate);  //"dd-MM-yyyyThh:mm:ss"
         dateMustBeFiltred = 1;
        }
     else if (QDateTime::fromString ( date_end, Qt::ISODate ).isValid ()&&
              QDateTime::fromString ( date_deb, Qt::ISODate ).isValid ()
             )
        {dateMustBeFiltred = 1;
        }
    }

 //........... noter les dimensions du tableau ................
 //
 QString val1, val2, val3, val4, val5, val6, val7, val8, val9, val10;
 QString       strTab[10];
 QString      section       = "";
 QString     var_name       = "";
 QStringList varNameList    = QStringList::split(';', list_VAR);
 QString            w_max   = cell_W;
 QStringList        var_w;
 //.................. epurer la liste du nom de variable des '= 80' et relever largeurs .......................
 for ( QStringList::Iterator it = varNameList.begin(); it != varNameList.end(); ++it )
     { QString v = *it;
       int a_pos = v.find('=');

       if (a_pos != -1)                         // si largeur indiquee s'en servir
          { var_w.append(v.mid(a_pos+1));
            *it = v.left(a_pos).stripWhiteSpace();
          }
       else                                     // si largeur non indiquee prendre celle par feaut
          { var_w.append(w_max);
            *it = v.stripWhiteSpace();
          }
     }

 QValueList<int> pos_tab;
 QString    head_table = "";
 QString    body_table = "";
 int     isOkToDisplay = 0;


 //......................... dates de debut et de fin ...................................
 QDateTime qdt_Deb;
 QDateTime qdt_End;

 if ( dateMustBeFiltred )
    {qdt_Deb = QDateTime::fromString ( date_deb, Qt::ISODate );
     if    (date_end == "0" || date_end.length() <= 0)  qdt_End = qdt_Deb;
     else                                               qdt_End = QDateTime::fromString ( date_end, Qt::ISODate );
     int nb_h = 0;
     if      (date_end == "0" || date_end.length() <= 0)   //YYYY-MM-DDTHH:MM:SS
             {qdt_Deb = QDateTime::fromString ( date_deb.left(10)+"T00:00:00", Qt::ISODate );
              qdt_End = qdt_Deb.addSecs (   24 * 3600 );
              dateMustBeFiltred = 1;
             }
     else if (date_end.left(2) == "+-" || date_end.left(2) == "-+")
             {nb_h = date_end.mid(2).toInt();
              qdt_Deb.addSecs ( - nb_h * 3600 );
              qdt_End.addSecs (   nb_h * 3600 );
              dateMustBeFiltred = 1;
             }
     else if (date_end.left(1) == "+" )
             {nb_h = date_end.mid(1).toInt();
              qdt_End.addSecs (   nb_h * 3600 );
              dateMustBeFiltred = 1;
             }
     else if (date_end.left(1) == "-" )
             {nb_h = date_end.mid(1).toInt();
              qdt_Deb.addSecs ( - nb_h * 3600 );
              dateMustBeFiltred = 1;
             }
    }

 head_table           += "<TABLE cellSpacing=\"" + QString("1")       + "\"" +
                         " cellpadding=\""       + border_W           + "\"" +
                         " width=\""             + width              + "\"" +
                         " border=\""            + border_W           + "\"><TBODY> <TR>";

 while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
    {if ( section == section_VAR )                  // à ce stade pt pointe au debut de la nouvelle ligne
        {// QString name_section = section.mid(4);
         // [VAR Poids]     1      2       3       4    5   6        7                                      8
         //    Modèle  = Date, Valeur, 31536000, 100, 50, Kg(s), Attention à la surcharge ponderale, Attention à l'amaigrissement
         //    2004-03-15T17:14:48 =  68
         //                                   val1                             ,    val2               val3      val4 val5 val6  val7       val8
         int            i      =  0;
         int        n_var      =  0;

         if (dateTimeFormat.length())
            {//.......................... titre de la colonne Date ..................................
             QString dateTimeTitle = "Date";
             int p  = dateTimeFormat.find('=');
             if (p !=-1) {dateTimeTitle  = dateTimeFormat.mid(p+1);
                          dateTimeFormat = dateTimeFormat.left(p);
                         }
             head_table += "<TD width=" + dateTimeWidth + ">" + dateTimeTitle + "</TD>";   // titre colonne  date
            }
         while (*pt && *pt != '[')
               {var_name = "";
                pt   = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &strTab[0], &strTab[1], &strTab[2], &strTab[3], &strTab[4], &strTab[5], &strTab[6], &strTab[7], &strTab[8]);
                //...................... definition des colonnes .....................................
                //
                if (var_name == TR("Modèle"))
                   {int ind  = varNameList.findIndex (  strTab[1] );                          // voir si ce nom de colonne est à afficher
                    if (ind != -1 )
                       {pos_tab.append( n_var );                                              // noter la position de la colonne
                        head_table += "<TD width=" + var_w[ind] + QString(">") + strTab[1] + "</TD>";
                       }
                    else
                       { pos_tab.append( -1 );                                                // cette colonne n'est pas desiree à l'affichage
                       }
                    ++n_var;
                   } // endif (var_name=="Modèle")
                 //................ VALEURS DATEES DE VARIABLE ........................................
                 //                                             2004-03-15T17:14:48 =  68 , truc, 1000
                 else if ( n_var )                // NB :  var_name  reçoit la date
                   {int p_w      =  0;
                    int isDateOK =  1;

                    //................. filtrer selon les dates .......................................
                    QDateTime qdt = QDateTime::fromString ( var_name, Qt::ISODate );
                    if (dateMustBeFiltred)
                       {if (qdt>=qdt_Deb && qdt<=qdt_End) isDateOK = 1;
                        else                              isDateOK = 0;
                       }
                    if (isDateOK)
                       {body_table  += "<TR>\r\n";
                        if (dateTimeFormat.length() && qdt.isValid())  // afficher eventuellement la date
                            {var_name    = qdt.toString(dateTimeFormat);
                             body_table += "        <TD width=" + dateTimeWidth + ">" + var_name  + "</TD>\r\n";
                            }

                         for ( i = 0; i < n_var; ++i)     // parcourir toute les colonnes de variables
                             {if (pos_tab[i] != -1)       // cette colonne est elle à afficher si oui le faire
                                 {body_table += "        <TD width=" + var_w[p_w] + QString(">") + strTab[i] + "</TD>\r\n";
                                  ++p_w;
                                  ++isOkToDisplay;
                                 }
                             }
                         body_table += "</TR>\r\n";
                       } // end if (isDateOK)
                   } // end else if ( n_var )
               } // end while (*pt && *pt != '[')
         section="";
        } // endif (section.left(3)=="VAR" )
    }
 if (isOkToDisplay)
    {resolvToken = head_table + "</TR>\r\n" + body_table + "</TBODY></TABLE>\r\n";
    }
 return    resolvToken;
}



//---------------------- Func_Extract_ATCD ------------------------------------------
/*! \brief Fonction script de MedinTux qui permet d'acceder aux antecedents enregistres des patients.
 * Plusieurs utilisations possibles de cette fonction : l'appel par "pattern" qui peut contenir
 * - ::LIBELLE::        Pour obtenir le libelle de l'antecedent
 * - ::MEDCHIR::        Pour le type d'antecedent (Medical, chirurgical, Facteur de risque, cardio-vasculaire...)
 * - ::ETAT::                Pour obtenir son etat (gueri ou actif)
 * - ::CODE::                Pour obtenir le code CIM10 de l'antecedent
 * - ::NOTE::                Pour obtenir le commentaire de l'antecedent
 * - ::DATE::                Pour obtenir la date de l'antecedent.
*  - ::ALD::                 Pour obtenir la propriete ALD de l'antecedent.

 * L'appel du mode "lineaire" ou lisible permet d'obtenir une phrase digeree des antecedents. Pratique pour les courriers de correspondance.

 * Exemples d'utilisation dans les masques :
 *  \code   {{:: Extract_ATCD (lineaire) }}  \endcode
 *  \code   {{::#Extract_ATCD (° <b><u>::TYPE:: :</u></b>, ::LIBELLE:: (::NOTE::) en ::DATE::), en, en rapport avec une pathologie ALD}}  \endcode
 *  \todo A CONNECTER AVEC G_pCApp->m_pAtcd_Code
*/
QString C_TokenInterpret::Func_Extract_ATCD(const QString &typePrefix,
                                            const QString &linePatern,
                                            const QString &datePrefix,
                                            const QString &aldPrefix)
{QString   str_data    = G_mCDC->m_pMB->GetDataFromRubList( GetIDCurrentDoc(TR("Terrain")) );       // recuperer donnees soit dans liste cache soit sur disque
 if (str_data.length()==0)              return QString("");
 else                                   return Func_Extract_ATCD_D(str_data, typePrefix, linePatern, datePrefix, aldPrefix);
}

//---------------------- Func_Extract_ATCD_D -----------------------------------------------------------------------------------
QString C_TokenInterpret::Func_Extract_ATCD_D(const QString &str_data,
                                              const QString &typePrefix,
                                              const QString &linePatern,
                                              const QString &datePrefix,
                                              const QString &aldPrefix
                                            )
{
 QString   resolvToken = "";
 char *pt = (char*) (const char*)str_data;

 //........... effacer les anciennes tables de variables ................
 //
 QString libelle      , familleGenre     , etat     , code     , note     , dt_str     , ald;
         libelle  = ""; familleGenre = ""; etat = ""; code = ""; note = ""; dt_str = ""; ald="";

 QString      section          = "";
 QString lineToInsert          = "";
 //............... gestion des categories d'ATCD ....................
//typedef       QMap<QString, QStringList*> FAMILLE_GENRE_MAP;
 FAMILLE_GENRE_MAP           familleGenre_MAP;
 FAMILLE_GENRE_MAP::Iterator it;
 QString                     keyInMap;
 QString                     genre;
 QStringList*                pfamilleGenreList;

 bool MODE_LINEAIRE   = (typePrefix.find(TR("lineaire")) != -1) ;
 bool MODE_TYPE_PREF  = (typePrefix.find(TR("::TYPE::")) != -1) ;

 while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
      { //.............. exploration de la liste des ATCD et construction des structures ....................
        if (section.startsWith("Ant") && section.endsWith("dents"))
           { while (*pt && *pt != '[')
                   {libelle  = "";
                    pt       = CGestIni::Param_ExtraireNextValeurs(pt, libelle,  &familleGenre, &etat, &code, &note, &dt_str, &ald);
                    keyInMap = familleGenre;
                    genre    = "";
                    int  deb = keyInMap.find("(");
                    if (deb != -1)
                       {keyInMap.truncate(deb); keyInMap = keyInMap.stripWhiteSpace();
                        genre = familleGenre.mid(deb+1).stripWhiteSpace();
                        genre = genre.left(genre.length()-1);                 // virer le ')' de fin
                       }
                    if (keyInMap.length()==0)      // si familleGenre non definie on tente avec la fonction de classement par defaut (tient compte au moins des allergies)
                       {keyInMap = CGenTools::get_FamilleGenreClasse( keyInMap, code);
                       }
                    // Nettoie et prepare la lisibilite des items de l'antecedent
                    libelle.replace("\\,",",");        libelle.replace("\\;",";");        libelle.replace("\\[","[");
                    libelle.replace("\\]","]");        note.replace("\\,",",");           note.replace("\\;",";");
                    note.replace("\\[","[");           note.replace("\\]","]");

                    // ATCD code CIM 10 ==> alleger le libelle
                    if (code.left(1) == "~" )
                       {  // Enlève SAI, le texte après la virgule, le texte entre parenthèse
                          libelle  = libelle.replace(" SAI", "");
                          libelle  = libelle.replace(" sai", "");
                          int deb  = libelle.find(",");
                          int fin  = -1;
                          if (deb != -1) libelle = libelle.left(deb);
                          int i    = 0;
                          while ((libelle.find("(") != -1) || (i != 10))
                                { ++i;
                                  deb = libelle.find("(");
                                  fin = libelle.find(")", deb);
                                  if (fin != -1)
                                     { libelle = libelle.left(deb-1) + libelle.right(libelle.length() - fin - 1);
                                     }
                                  else break;
                                }
                          libelle = libelle.stripWhiteSpace();
                       } // Fin ATCD CIM 10

                    //........... Construction des lignes .........................................................
                    if (MODE_LINEAIRE)
                       {                     lineToInsert  = libelle;                           // Libelle
                        if (note.length())   lineToInsert += " (" + note + ")";                 // Note
                        if (dt_str.length()) lineToInsert += " en " + dt_str.right(4);          // Date
                        if (ald.length())    lineToInsert += " pathologie ALD" ;                // ald
                       }
                    else
                       {lineToInsert = linePatern;
                        lineToInsert.replace("::LIBELLE::",   libelle);
                        lineToInsert.replace("::MEDCHIR::",   genre);        // la famille sert de titre donc pas besoin
                        if (genre.length()) lineToInsert.replace("::(MEDCHIR)::", genre.prepend('(').append(')'));        // la famille sert de titre donc pas besoin
                        else                lineToInsert.replace("::(MEDCHIR)::", "");
                        lineToInsert.replace("::ETAT::",      etat);
                        lineToInsert.replace("::CODE::",      code);
                        lineToInsert.replace("::NOTE::",      note);
                        if (dt_str.length())
                           {dt_str.prepend(datePrefix);
                            lineToInsert.replace("::DATE::",    dt_str);
                           }
                        else
                           {lineToInsert.replace("::DATE::",    "");
                           }
                        if (ald.length())
                           {if (aldPrefix.length()) lineToInsert.replace("::ALD::",    aldPrefix);
                            else                    lineToInsert.replace("::ALD::",    ald);
                           }
                        else
                           {lineToInsert.replace("::ALD::",    "");
                           }
                       }
                    //........... Affecter la ligne selon categories (medical, chirurgical, allergie) ...........
                    //            nourrir la MAP et les QStringList
                    pfamilleGenreList = 0;
                                   it = familleGenre_MAP.find(keyInMap);
                    if (it == familleGenre_MAP.end())
                       {pfamilleGenreList          = new QStringList();
                        familleGenre_MAP[keyInMap] = pfamilleGenreList;
                       }
                    else
                       {pfamilleGenreList = it.data();
                       }
                    if (pfamilleGenreList)
                       {pfamilleGenreList->append(lineToInsert);
                       }
                   }  //  Fin while(*pt && *pt != '[')
            section="";
            // ...........  Expoitation des structures .................... ...................
            //              Construction des phrases à inserer en exploitatant la MAP et ses QStringList
            lineToInsert = "";
            for ( it = familleGenre_MAP.begin(); it != familleGenre_MAP.end(); ++it )
                {pfamilleGenreList = it.data();
                 keyInMap          = it.key();
                 if (MODE_LINEAIRE)
                    {//.......... le titre de l'ATCD (famille) ............
                     lineToInsert     += " - " + keyInMap + " :  ";
                     //.......... les lignes des ATCD .............
                     for ( QStringList::Iterator lt = pfamilleGenreList->begin(); lt != pfamilleGenreList->end(); ++lt )
                         { lineToInsert += *lt + ", ";
                         }
                     lineToInsert = lineToInsert.left(lineToInsert.length()-2) + ".<br />\n"; // corriger dernière ligne pour remplacer ", " de fin par "<br>.\n"
                    }
                 else
                    {//.......... le titre de l'ATCD (famille) ............
                     if (MODE_TYPE_PREF)
                        {lineToInsert     += typePrefix;
                         lineToInsert      = lineToInsert.replace("::TYPE::", keyInMap ) + "<br />\n";
                        }
                     else
                        {lineToInsert     += "° " + keyInMap + "<br />\n";
                        }
                     //.......... les lignes des ATCD .............
                     for ( QStringList::Iterator lt = pfamilleGenreList->begin(); lt != pfamilleGenreList->end(); ++lt )
                         { lineToInsert += *lt + "<br />\n";
                         }
                    }
                } //endfor ( it = familleGenre_MAP.begin(); it != familleGenre_MAP.end(); ++it )
            //...........  ajustements cometiques ...........
            if (lineToInsert.length()==0)
               {if (MODE_LINEAIRE) lineToInsert = TR("Sans antecedents connus.");
               }
            else
               {if (MODE_LINEAIRE) lineToInsert.prepend( TR("Ses antecedents sont marques par : ") + "<br />\n" );
               }
            resolvToken = lineToInsert ;
            //...........  effacement des QStringList de la map ...........
            for ( it = familleGenre_MAP.begin(); it != familleGenre_MAP.end(); ++it )
                {pfamilleGenreList = it.data();
                 keyInMap          = it.key();
                 if (pfamilleGenreList) delete pfamilleGenreList;
                }
           }  // Fin if section == "Antecedents"
      } //end while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
 return    resolvToken;
}

//------------------------ GetDateOfActiveRubrique -------------------------------------
/*! \brief Retourne la date du document en cours */
QString C_TokenInterpret::GetDateOfActiveRubrique()
{
  if (G_mCDC &&G_mCDC->m_pCRubCurrentRecord ) return G_mCDC->m_pCRubCurrentRecord->m_Date;
  else                               return QString(" ");
}


//-------------------- GetPoidsTaille --------------------------------------------
/*! \brief Retourne le poids et la taille.
 *  \todo pour faciliter debuggage utiliser Var_Code
*/
void C_TokenInterpret::GetPoidsTaille(const char* pt_doc, QString &tableau, QString &poids_name, QString &taille_name, QString &poids_var, QString &taille_var)
{poids_var      = "";
 taille_var     = "";
 if (pt_doc==0 || strlen(pt_doc)==0)    return;
 tableau        = tableau.stripWhiteSpace();
 poids_name     = poids_name.stripWhiteSpace();
 taille_name    = taille_name.stripWhiteSpace();
 //..................... recuperer la date ........................................
 char *pt = (char*) pt_doc;
 //........... effacer les anciennes tables de variables ................
 QString       strTab[10];
 QString      section   = "";
 QString     var_name   = "";

 while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
    {if (section == tableau )                  // à ce stade pt pointe au debut de la nouvelle ligne
        {int               n_var   = 0;
         int    pos_poids_to_get   = -1;
         int   pos_taille_to_get   = -1;
         while (*pt && *pt != '[')
               {var_name = "";
                pt   = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &strTab[0], &strTab[1], &strTab[2], &strTab[3], &strTab[4], &strTab[5], &strTab[6], &strTab[7], &strTab[8]);
                //...................... DEFINITION DE VARIABLE .....................................
                //                       contruire le dialogue des variables
                if (var_name== TR("Modèle") )
                   {if (pos_poids_to_get  == -1 && strTab[1] == poids_name)  pos_poids_to_get  = n_var;   // noter la position de la variable
                    if (pos_taille_to_get == -1 && strTab[1] == taille_name) pos_taille_to_get = n_var;   // noter la position de la variable
                    n_var++;
                   } // endif (var_name=="Modèle")
                 //................ VALEURS DATEES DE VARIABLE ........................................
                 else if (pos_poids_to_get!= -1 && var_name !="" ) // var_name  reçoit la date
                   {poids_var = strTab[pos_poids_to_get];
                   }
                 //................ VALEURS DATEES DE VARIABLE ........................................
                 else if (pos_taille_to_get!= -1 && var_name !="" ) // var_name  reçoit la date
                   {taille_var = strTab[pos_taille_to_get];
                   }
               } // end while (*pt && *pt != '[')
         section="";
        } // endif (section.left(3)=="VAR" )
    } // end while((pt=m_pMB->Param_GotoNextSection(pt, 0, &section)) && *pt)
}


//------------------------------------------ GetVarNeerer --------------------------------------------------------
/*! \brief Recupère toutes la valeurs de la variable la plus poche de la date passee en entree
 * \param QString section         la section de la variable  (VAR Poids)
 * \param QString variable        la variable en question    (Poids)
 * \param QString dateSel         la date voulue en format String ISO
 * \return QString de la valeurs sans sa date
 * EXEMPLE :
 *      QString sect = "VAR Poids";
 *      QString varn = "Poids";
 *      QString datePlusProche = "2007-09-23T13:00:00"
 *      QString valeurPlusProche = GetVarNeerer ( sect, varn, datePlusProche);
 *  \todo Pour ameliorer debuggage utiliser Var_Code
*/
//-----------------------------------------------------------------------------------------------------------------
QString C_TokenInterpret::GetVarNeerer (QString &section, QString &variable, QString &dateSel)
{  QStringList valeurs          = GetVar (section, variable);
   QString     valeurPlusProche = GetVarNeerer ( valeurs, dateSel);
   QStringList ret = QStringList::split( "/;/" , valeurPlusProche);
   if (ret.count()>=2) return ret[1];
   else                return QString::null;
}

//------------------------------------------ GetVarNeerer --------------------------------------------------------
/*! \brief Recupère toutes la valeurs de la variable la plus poche de la date passee en entree
 * \param QStringList     les valeurs datees (renvoyees par GetVar)
 * \param QString         la date voulue en format String ISO
 * \return QString de la valeurs datee au format :   DATE/;/VALEUR
 * EXEMPLE :
 *      QString sect = "VAR Poids";
 *      QString varn = "Poids";
 *      QStringList valeurs = GetVar (sect, svarn);
 *      QString datePlusProche = "2007-09-23T13:00:00"
 *      QString valeurPlusProche = GetVarNeerer ( valeurs, datePlusProche);
 *  \todo Pour ameliorer debuggage utiliser Var_Code
*/
//-----------------------------------------------------------------------------------
QString C_TokenInterpret::GetVarNeerer (QStringList &valuesList, QString &dateSel)
{if (valuesList.count() != 0 )
 {long          secondsToVar;
  long          secondsToRecent;
  QString       mostRecentVal = "";
  QString       sDate = dateSel.stripWhiteSpace();
  QDateTime     varQDate;
  QDateTime     dt = QDateTime::fromString( sDate , Qt::ISODate );   // DATE VISITE

  if (valuesList.count() == 1)
   {    //varDateVal = QStringList::split( "/;/", values[0]);
        //return varDateVal[1];
        return valuesList[0];
   }
  // trie de façon ascendante par la date (valeurVar[0] = la plus ancienne)
  valuesList.sort();

  // recupère dans varDate la première date comme reference la plus ancienne
  QStringList varDateValList    = QStringList::split( "/;/", valuesList[0]);
  QDateTime   varQDateRecent    = varQDateRecent.fromString(varDateValList[0] , Qt::ISODate);
  mostRecentVal                 = valuesList[0];        // au cas où il n'y aurait qu'une seule valeur...

  for (int i = 1; i < (int)valuesList.count(); ++i)
    {   varDateValList = QStringList::split( "/;/", valuesList[i] );
        varQDate       = varQDate.fromString(varDateValList[0], Qt::ISODate);

        // Compare le nombre de secondes entre DATE VISITE et les deux dates
        secondsToVar    = varQDate.secsTo(dt);
        secondsToRecent = varQDateRecent.secsTo(dt);
        if (secondsToVar < 0)         secondsToVar          = secondsToVar * (-1);
        if (secondsToRecent < 0)      secondsToRecent       = secondsToRecent * (-1);

    if (secondsToVar < secondsToRecent)
        { mostRecentVal  = valuesList[i];
          varQDateRecent = varQDate;
        }
     } // Fin  for

  return mostRecentVal;
 } // Fin if count > 0
 return "";
}


//------------------------------------------ GetVar ---------------------------------------------------------------
/*! \brief Recupère toutes les valeurs de la variable incluse dans section section_var de la rubrique Terrain
    \todo Utiliser Var_Code pour ameliorer le debuggage
*/
// ENTREE :
//      section_var     : section
//      nalme_var       :le nom de la variable
// SORTIE :
//      QStringList des valeurs tries en ascendant des valeurs
// FORMAT DES DONNEES :
//      DATE/;/VALEUR
//
//  EXEMPLE :
//      QString sect = "VAR Poids";
//      QString varn = "Poids";
//      QStringList valeurs = GetVar (&sect, &varn);
//-----------------------------------------------------------------------------------------------------------------
QStringList C_TokenInterpret::GetVar (QString &section_var, QString &name_var)
{// recuperer donnees soit dans liste cache soit sur disque
 QString   str_data    = G_mCDC->m_pMB->GetDataFromRubList( GetIDCurrentDoc(TR("Terrain")) );
 if (str_data.length()==0)              return QString(" ");
 char *pt = (char*) (const char*) str_data;

 //........... effacer les anciennes tables de variables ................
 //
 QString       strTab[10];
 QString      section   = "";
 QString     var_name   = "";

 // char     *prop_section = 0;       // permet de noter le pointeur sur la section propriete renouvelable/intercurent des produits
 QStringList  valeurVar;        // recupère toutes les valeurs de VAR_name ainsi que leur date separees par /;/

 while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
    {//.................................. VAR ........................................................
     if (section == section_var )                  // à ce stade pt pointe au debut de la nouvelle ligne
        {int        n_var      = 0;
         int   pos_var_to_get  = -1;
         while (*pt && *pt != '[')
               {var_name = "";
                pt   = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &strTab[0], &strTab[1], &strTab[2], &strTab[3], &strTab[4], &strTab[5], &strTab[6], &strTab[7], &strTab[8]);
                //...................... DEFINITION DE VARIABLE .....................................
                //                       contruire le dialogue des variables
                if (var_name==TR("Modèle"))
                   {if (pos_var_to_get == -1 && strTab[1] == name_var) pos_var_to_get = n_var;   // noter la position de la variable
                    n_var++;
                   } // fin Modèle
                 //................ VALEURS DATEES DE VARIABLE ........................................
                 else if (pos_var_to_get != -1 && var_name !="" &&  strTab[pos_var_to_get] != "") // var_name = la date
                   {// Recupère les valeurs et les dates de la variable demandee dans les listes
                    valeurVar.append(var_name+"/;/"+strTab[pos_var_to_get]);
                   }
               } // end while (*pt && *pt != '[')
         section="";
        } // end if section_VAR

    } // end while
 return valeurVar;
}

//----------------------------- Func_Extract_VAR --------------------------------------
/*! \brief Extrait les variables selon le modèle demande.
 *  \todo Utiliser Var_Code pour ameliorer debuggage.
*/
//   [VAR Constantes de Base]
//      Modèle = Date, Pouls, 31536000, 130, 40, pps, Tachycardie, Bradycardie
//      Modèle = Date, SAO2, 31536000, 0, 92, %, , Hypoxie
//      Modèle = Date, Temp, 31536000, 40.5, 35, ?, Hyporthermie, Hyperthermie
//   [VAR Poids]
//      Modèle = Date, Poids,  31536000, 100, 50, Kg(s), Obesite, Amaigrissement
//      Modèle = Date, Taille, 31536000, 0, 0, cm(s),,
//
//      {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE ,   0  , TOUTES )}}
//      {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , -24  , PREMIERE )}}
//      {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , +24  , DERNIERE )}}
//      {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , +-24 , PLUS PROCHE )}}

QString C_TokenInterpret::Func_Extract_VAR(QString &section_VAR, QString &name_VAR, QString &date_deb, QString &date_end, QString &date_intervalle)
{QString   resolvToken = "";

 section_VAR = section_VAR.stripWhiteSpace();
 name_VAR    = name_VAR.stripWhiteSpace();
 date_deb    = date_deb.stripWhiteSpace();
 date_end    = date_end.stripWhiteSpace();
 date_intervalle = date_intervalle.stripWhiteSpace().lower();

 //..................... recupère les valeurs et leurs dates ......................................
 QStringList valeurVar = GetVar(section_VAR, name_VAR);

 //..................... recuperer la date du document ............................................
 QString str_date = GetDateOfActiveRubrique();

//      {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE ,   0  , TOUTES )}}
//              Insère un tableau avec toutes les valeurs

//      {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , -24  , PREMIERE )}}
//              Insère la valeur chronologiquement la plus ancienne

//      {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , +24  , DERNIERE )}}
//              Insère la valeur chronologiquement la plus recente (voir futuriste...)

//      {{:: Extract_VAR (VAR Constantes de Base , SAO2 , DATE VISITE , +-24 , PLUS PROCHE )}}
//              Insère la valeur la plus proche (à la seconde près) de la date qu'elle soit anterieure ou posterieure à l'observation

  if (!valeurVar.isEmpty ())
   {
     QString            varDateStr;
     QDateTime          varDate;
     QDateTime          varDateMostRecent;
     QStringList        varDateVal;  // en  0 -> date ; en 1 -> valeur de la variable

     if (date_intervalle == "toutes")                                                           // Teste = OK
      { QString tableau;
        // tri les valeurs selon leur date  -- tri ascendant
        valeurVar.sort();

        // cree l'entete du tableau
        tableau =   "<table border=1 cellpadding=2 cellspacing=2>";
        tableau +=  "<tr><td><center><b>Date</b></center></td><td><center><b>";
        tableau +=  name_VAR + "</b></center></td></tr>";

        // insère les valeurs retrouvees
        for( int i = 0; i < (int)valeurVar.count() ; ++i)
         {
          varDateVal = QStringList::split( "/;/", valeurVar[i] );
          varDateVal[0].replace("T","&nbsp;&nbsp;");
          tableau += "<tr><td>"+varDateVal[0]+"</td><td>" +varDateVal[1] + "</td></tr>";
         }

        // Fin du tableau
        tableau += "</table><br /><br />";
        resolvToken = tableau;
      }
     else if (date_intervalle == TR("derniere"))                                                    // AMELIORABLE
      { valeurVar.sort();
        varDateVal = QStringList::split( "/;/", valeurVar.last () );
        resolvToken = varDateVal[1];
      }
     else if (date_intervalle == TR("premiere"))                                                   // AMELIORABLE
      { valeurVar.sort();
        varDateVal = QStringList::split( "/;/", valeurVar[0] );
        resolvToken = varDateVal[1];
      }
     else if (date_intervalle == TR("plus proche"))                                                 // Teste = OK
          { /*QString mostRecent = GetVarNeerer( valeurVar , str_date );
            varDateVal = QStringList::split( "/;/", mostRecent );
            resolvToken = varDateVal[1];*/
            resolvToken = GetVarNeerer ( section_VAR, name_VAR, str_date);
          }
  } // Fin si des valeurs ont ete trouvees
  else
  { resolvToken = TR("Pas de valeur trouvee pour ") + name_VAR; }

  return    resolvToken;
}

//--------------------------- Func_Extract ---------------------------------------------------
/*! \brief Extrait une partie du texte du document selon le modèle demande */
QString C_TokenInterpret::Func_Extract(QString &doc_type, QString &tag_deb, QString &tag_end, QString &find_to, QString &replace_by, const QString justNum_in)
{QString   resolvToken    = "";
 const char* pt_doc       = 0;
 char*       deb          = 0;
 char    *debEndMotif     = 0;
 QString      justNum     = justNum_in.stripWhiteSpace();
 QString     var_nameRech = "";
 QString     var_nameRepl = "";
 QString     str_data     = "";
 doc_type   = doc_type.stripWhiteSpace();
 tag_deb    = tag_deb.stripWhiteSpace();
 tag_end    = tag_end.stripWhiteSpace();
 find_to    = find_to.stripWhiteSpace();
 replace_by = replace_by.stripWhiteSpace();
 if (doc_type.startsWith("$USER_PARAM"))
    {int              i = doc_type.find("=");
     if (i==-1)       i = doc_type.find("|");
     else if (i==-1)  i = doc_type.find(" ");
     else if (i==-1)   return resolvToken;
     QString user   = doc_type.mid(i+1).stripWhiteSpace();
     QString param  = G_mCDC->m_pMB->ReadDrTuxUserParametres(user);
     resolvToken    = CGestIni::Param_ReadUniqueParam(param,tag_deb,tag_end);
     return resolvToken;
    }
 if (doc_type.startsWith("$VAR"))
    {int p                =  doc_type.find("=");
     if (p !=-1) doc_type =  doc_type.mid(p+1).stripWhiteSpace();
     else        doc_type =  doc_type.mid(4).stripWhiteSpace();
     str_data             =  (*G_mCDC->m_pVariables)[doc_type];
     pt_doc               =  (char*) (const char*) str_data;
    }
 else if (doc_type.length()>0)
    {str_data    = G_mCDC->m_pMB->GetDataFromRubList( GetIDCurrentDoc(doc_type) );       // recuperer donnees soit dans liste cache soit sur disque
     if (str_data.length()==0)              return resolvToken;
     int p = str_data.find("</HTML_Data>");
     if (p !=-1) str_data.truncate(p);
     pt_doc      = (char*) (const char*) str_data;
    }
 if (tag_deb=="$FROMSTART") deb = (char*) (const char*) pt_doc;
 else if (tag_deb.length()) deb = CHtmlTools::HtmlFind(pt_doc, tag_deb);

 if (deb && tag_end.length())
    {if (tag_end.upper()=="$TOEND")
        {resolvToken = str_data.mid(deb-pt_doc);
         //int p = resolvToken.find("</HTML_Data>");
         //if (p !=-1) resolvToken = resolvToken.left(p);
        }
     else 
        {
          CHtmlTools::HtmlFind(deb, tag_end, &debEndMotif);
          if (debEndMotif)
             {resolvToken.setLatin1 (deb, debEndMotif - deb);
              if (justNum.find("$keepHtml") == -1){CHtmlTools::HtmlToAscii(resolvToken);}
              else                                {justNum = justNum.remove("$keepHtml");}
              resolvToken.replace("\n","<br />");       // Remplace les sauts de lignes ASCII par les sauts de ligne HTML
              resolvToken.replace("\r","");             // pour les puristes...resolvToken=resolvToken.stripWhiteSpace();
             }
         }
     if ( find_to.length() )
        {
          if ( resolvToken.find(find_to,0,TRUE) != -1)
             { resolvToken = replace_by;
             }
          else resolvToken = "";
        }  // end if ( find_to.length() )
     } // end if (deb && tag_end.length())
 if (justNum.length()) return  toNum(resolvToken, justNum);
 else                  return  resolvToken;
}

//--------------------------------- GetIDCurrentDoc -----------------------------------------------------
/*! \brief Retourne le CRubRecord correspondant au document actuellement affiche dans la CMDI gerant le doc_type.
*/
CRubRecord  *C_TokenInterpret::GetIDCurrentDoc(const QString &doc_type_in)
{ return G_pCApp->GetIDCurrentDoc(doc_type_in, G_mCDC->m_pCurDisplayDocMap);


/*
 CRubRecord  *pCRubRecordRet = 0;
 int                    pos  = -1;
 QString            libelle  = "";
 QString            doc_type = doc_type_in;
 //................ si le type contient le sous type .................................
 if (doc_type.length() && (pos=doc_type.find('|')) != -1)
    {libelle  = doc_type.mid(pos+1).stripWhiteSpace();
     doc_type = doc_type.left(pos).stripWhiteSpace();
    }

 //................... si non precise retourner la rubrique courante..............................
 if (doc_type.length()==0||doc_type==TR("*"))
    { CMDI_Generic *pCMDI_Generic   =  G_pCApp->GetCurrentRubrique();
      if (pCMDI_Generic)
         {RUBREC_LIST::iterator rit = pCMDI_Generic->Current_RubList_Iterator();
          if (rit != G_mCDC->m_pRubList->end())
             { pCRubRecordRet = &(*rit);
               if (libelle.length())   // si libelle fourni on verifie si il correspond
                  {if (pCRubRecordRet->m_Libelle==libelle) return pCRubRecordRet;
                  }
               else
                  {                                        return pCRubRecordRet;
                  }
             }
         }
    }

 //................... on cherche celui de la map (ceux en cours d'affichage)..............................
 //                    si libelle fourni on verifie si il correspond
 DOCUMENT_DISPLAY_MAP::Iterator it = G_mCDC->m_pCurDisplayDocMap->find ( doc_type );
 if (! (it == G_mCDC->m_pCurDisplayDocMap->end()) )
    {pCRubRecordRet = it.data();
     if (libelle.length())
        {if (pCRubRecordRet->m_Libelle==libelle)           return pCRubRecordRet;
        }
     else
        {                                                  return pCRubRecordRet;
        }
    }
 //...................................... si pas trouve on va rechercher le dernier .........................
 //                                       correspondant au type numerique dans la liste generale
 QString doc_type_num = doc_type;
 if ( ! (doc_type[0]>='0' && doc_type[0]<='9') ) // si pas exprime sous forme numerique alors on convertit
    {doc_type_num = G_pCApp->RubNameToStringType(doc_type);
    }
 //................... si pas trouve on cherche le dernier correspondant...................................
 RUBREC_LIST::Iterator ut ;
 CRubRecord  *pCRubRecord = 0;   // iterera sur tous les record
 pCRubRecordRet           = 0 ;  // retiendra le dernier Ok de la liste
 for ( ut = G_mCDC->m_pRubList->begin(); ut != G_mCDC->m_pRubList->end(); ++ut )
     {pCRubRecord = &(*ut);
      if (libelle.length())   // si libelle fourni on verifie aussi si il correspond
         {if ( pCRubRecord->m_Type==doc_type_num && pCRubRecord->m_Libelle==libelle) pCRubRecordRet = pCRubRecord;
         }
      else                    // sinon on ne verifie que le type
         {if ( pCRubRecord->m_Type==doc_type_num)                                    pCRubRecordRet = pCRubRecordRet;
         }
     }
 return pCRubRecordRet;
*/
}


//--------------------------- DocNameToDocNumType --------------------------------------------------
/*! \brief Transforme le nom de la rubrique en son equivalent numerique. Exemple : "Observation" -> "20030000"
*/
/*
QString C_TokenInterpret::DocNameToDocNumType(const QString &doc_name, int part / * = 8 * /)
{QString ret = "";

 const char *c = doc_name;
 switch (*c)
   {case 'U':                             ret = "20040000"; break;      //TYP_URG_OBSERV
    case 'O':                             ret = QString::number(CMDI_Observation::S_GetType()); break;
    case 'P':                             ret = "20020000"; break;
    case 'T':                             ret = "20060000"; break;
    case 'D':                             ret = "20080000"; break;
    case 'I':                             ret = "20080000"; break;
    case 'C':
         {if (doc_name== "Courrier")      ret = "20020500"; break;
          if (doc_name== "Certificat")    ret = "20020300"; break;
         }
   }
  return ret.left(part);
    m_ListRubName.append(CMDI_Observation::S_GetRubName());
    m_ListRubName.append(CMDI_Prescription::S_GetRubName());
    m_ListRubName.append(CMDI_Documents::S_GetRubName());
    m_ListRubName.append(CMDI_Terrain::S_GetRubName());
    m_ListRubName.append(CMDI_Ident::S_GetRubName());
    m_ListRubName.append(CMDI_ChoixPatient::S_GetRubName());

}
*/
//-------------------------- extractArgList -------------------------------------------
/*! \brief non document� */
long C_TokenInterpret::extractArgList(QStringList &arg_list, const QString &arg_str)
{long len     = arg_str.length();
 long pos     = 0;
 long deb     = 0;
 QString arg  = "";
 QString argS = "";

 QMap<QString,QString>::Iterator it;
 while(pos <= len)
    {if (arg_str.at(pos) == '\\') 
        { pos += 2;
        }
     else if (pos==len || arg_str.at(pos) == ',')
        {arg   = arg_str.mid(deb, pos-deb);
         argS  = arg.stripWhiteSpace();
         //........... prendre le contenu de la variable si l'agument commence par $ .............
         if (  argS.startsWith("$")               &&
             ! argS.startsWith("$VAR")
            )
            { it      = (*G_mCDC->m_pVariables).find(argS.mid(1));
              if (it != (*G_mCDC->m_pVariables).end())  arg = it.data();
            }
         arg_list.append( argUnProtect(arg) );
         ++pos;
         deb = pos;
        }
     else
        {++pos;
        }
    }
 return 1;
}

//-------------------------- argUnProtect -------------------------------------------
QString C_TokenInterpret::argUnProtect(QString arg)
{arg = arg.replace("\\\\","!°!;:");
 arg = arg.remove("\\");
 arg = arg.replace("!°!;:","\\");
 return arg;
}

//-------------------------- isReservedKeyWord -------------------------------------------
bool C_TokenInterpret::isReservedKeyWord(const QString &word)
{ KEY_MACRO toTest;
            toTest.m_Token     = word.latin1();
  //............... recuperer le tableau des mots clefs.............................
  KEY_MACRO_TAB_DEF *p_MACRO_LIST = &G_MacroTab_List[26];
  return bsearch(&toTest, p_MACRO_LIST->m_MacroTab, p_MACRO_LIST->m_size , sizeof(KEY_MACRO), C_TokenInterpret::comparatorMacro);
}

//----------------------------- findLastDelimiter -----------------------------------------------
/*! \brief Pour la recursivite. */
long C_TokenInterpret::findLastDelimiter(QString &str, char delimiter)
{
 int end = str.length();
 while (end>1)
  { --end;
    if (str[end]==delimiter) return end;
  }
 return -1;
}

//------------------------------------------ GetDDR -----------------------------------------------------------------
/*! \brief Recupère dans le terrain la date des dernières règles en cas de grossesse. Retourne la date au format QT::ISODate ou QString(" ") si n'est pas trouve.
 * \todo utiliser Atcd_Code
*/
QString C_TokenInterpret::GetDDR()
{ QString tmp1;
  // recuperer donnees soit dans liste cache soit sur disque
   QString   str_data    = G_mCDC->m_pMB->GetDataFromRubList( GetIDCurrentDoc(TR("Terrain")) );
   if (str_data.length()==0)              return QString(" ");
   char *pt = (char*) (const char*) str_data;

   //........... effacer les anciennes tables de variables ................
   QString   section   = "";
   QString   var_name   = "";

   //char     *prop_section = 0;       // permet de noter le pointeur sur la section propriete renouvelable/intercurent des produits

   while((pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
    {//.................................. Obstetrique ........................................................;
     if (section == TR("Obstetrique")||section == TR("Obstetrique"))// && m_IsFeminin == 1)                  // à ce stade pt pointe au debut de la nouvelle ligne
        {
         while (*pt && *pt != '[')
               {pt = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &tmp1); // 18-03-2004
                if (var_name == TR("Grossesse") && tmp1 != TR("non"))   // val1 contient non ou ddr sauvegardee
                   {    tmp1 = tmp1.mid(6) + "-" + tmp1.mid(3,2) + "-" + tmp1.left(2);
                        return tmp1;
                   }
               }// Fin while [
         section="";
        } // Fin if section
    } // Fin while Section
  return QString(" ");
}

//------------------------------------------ Terme_Obst -------------------------------------------------------------
/*! \brief Calcul le terme de la grossesse à la date souhaitee.
 *  \param QDate DDR       Date des dernières règles
 *  \param QDate dt        Date pour le calcul
 *  \return Le terme au format string du style : " XX SA XX Jours "
 *  \todo à deplacer dans Atcd_Code.
*/
QString C_TokenInterpret::Terme_Obst(QDate DDR , QDate dt)
{ int nbDays = DDR.daysTo(dt);
  int nbDaysLeft = nbDays % 7;
  int nbWeeks = (nbDays - nbDaysLeft ) / 7;
  QString ret;

  if (dt < DDR) { ret = TR("Le terme n'est pas calculable (terme negatif). Verifiez vos dates."); }
  else if (nbWeeks > 45) { ret = TR("Le terme n'est pas calculable (> 45 SA). Verifiez vos dates."); }
  else { ret = QString::number(nbWeeks) + " SA "+ QString::number(nbDaysLeft) + TR(" Jours "); }

  return ret;
}


//------------------------------------------ Terme_Obst -----------------------------------------
/*! \brief Calcul le terme de la grossesse à la date souhaitee.
 *  \param QString DDR     Date des dernières règles format ISODate
 *  \param QDate dt        Date pour le calcul
 *  \return Le terme au format string du style : " XX SA XX Jours "
 */
QString C_TokenInterpret::Terme_Obst(QString DDR , QDate dt)
{ if (DDR == "" || DDR == " ") return " ";
  QDate DDR_dt = DDR_dt.fromString ( DDR , Qt::ISODate);

  return Terme_Obst(DDR_dt , dt)        ;
}

//------------------------------------------ toNum -----------------------------------------
/*! \brief ne retient d'une chaine de caracteres que les valeurs numeriques et le point (au passage les virgules seront transformees en point).
 *  \param text           const QString & texte a convertir en numerique
 *  \param cutFirstStr_in const QString & mode de conversion numerique :\
 SI   contient $stopIfNotNum la conversion s'arr�te au premier caract�re non num�rique.
 SI   contient $allString tous les caract�res num�riques de la cha�ne seront retenus.
 SI   contient $toInt la conversion ne conservera que la partie enti�re.
 SI   toute autre valeur, la conversion s'arr�tera � la premi�re occurence de cette valeur.
 *  \return une QString resultat du traitement
 */
QString C_TokenInterpret::toNum(const QString &text, const QString &cutFirstStr_in)
{
 QString str               = text.stripWhiteSpace();
 QString cutFirstStr       = cutFirstStr_in;
 bool    cutFirstNonNum    = TRUE;
 bool    toInt             = (cutFirstStr.find("$toInt") != -1);
 if (toInt) cutFirstStr    = cutFirstStr.remove("$toInt");
 if (cutFirstStr.length())
    {
     if      (cutFirstStr.find("$stopIfNotNum") != -1)    cutFirstNonNum = TRUE;
     else if (cutFirstStr.find("$allString")    != -1)    cutFirstNonNum = FALSE;
     else {int pos = str.find(cutFirstStr);
           if (pos != -1) str.truncate(pos);
          }
    }
 int     end = str.length();
 QString ret = "";
 int       i = 0;
 while (i<end)
     {if (str[i]>='0' && str[i]<='9')  {ret += str[i];}
      else if (str[i]=='.')            {ret += ".";   }
      else if (str[i]==',')            {ret += ".";   }
      else if (cutFirstNonNum)         {i    = end;   }
      ++i;
     }
 if (toInt)
    {int pos  =     ret.find(".");
     if (pos != -1) ret.truncate(pos);
    }
 return ret;
}

//-------------------------------------- FindToken ----------------------------------------------------------------
/*! \brief ne retient d'une chaine de caracteres que les valeurs numeriques et le point (au passage les virgules seront transformees en point).
 *  \param tokenToFind const QString &tokenToFind : token  a rechercher
 *  \param document const QString &document : document dans lequel rechercher le token
 *  \param fin int * : si different de zero il y sra retourne la fin du token dans le document
 *  \return un int si -1 pas trouve sinon position du token dans le documnt.
 */
int C_TokenInterpret::FindToken(const QString &tokenToFind, const QString &document, int *fin /* = 0*/)
{int deb = -1;
 FindToken(0, document.length(), tokenToFind, document, &deb, fin);
 return deb;
}

int C_TokenInterpret::FindToken(int pos_deb, int pos_max, const QString &tokenToFind, const QString &document, int *deb, int *fin /* = 0*/)
{int isWithToken   =  0;
 int pos_end       =  0;
     pos_deb       =  document.find("{{", pos_deb);
 while (pos_deb != -1 && pos_deb < pos_max)
       {pos_end        =  CGestIni::findFermant(&document, pos_deb+2, pos_max, "{{", "}}", isWithToken);
        if (isWithToken)
           { pos_max  = FindToken(pos_deb+2, pos_end, tokenToFind, document, deb,fin);
           }
        else
           {QString token     = document.mid(pos_deb+2, pos_end - pos_deb-2).upper();
            if (token.find(tokenToFind) != -1)
               {int pos;
                if (token.startsWith("::"))
                  { token = token.mid(2);
                    pos   =  token.find('(');
                    if (pos != -1) {token.truncate(pos);}
                    token = token.remove('#');
                  }
                else
                  {pos   =  token.find('=');
                   if (pos != -1) {token.truncate(pos);}
                  }
                token = token.stripWhiteSpace();
                if (token==tokenToFind)
                   {if (fin) *fin = pos_end;
                    *deb = pos_deb;
                    return 0;        // pos max devient Zero si recursivite engagee et donc fin de la boucl
                   }
               }
            pos_deb   =  document.find("{{", pos_deb+2);
           }
       }
  return pos_max;
}

