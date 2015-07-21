/*********************************** CDevilCrucible.cpp ***************************
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


#include "CDevilCrucible.h"
#include "C_TokenInterpret.h"
#include "CApp.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "ui/DLG_InsertList.h"
#include "../../MedinTuxTools/C_Organiseur.h"
#include "../../MedinTuxTools/CGenTools.h"

#include "MyEditText.h"
#include "CMDI_Generic.h"
//#include "CMDI_Documents.h"

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

#ifdef Q_WS_WIN
   #define F_EXE ".exe"
   #include <windows.h>
#else
   #define F_EXE ""
#endif

#define TR QObject::tr

//====================================== CDevilCrucible ==============================================================

//----------------------------  CDevilCrucible ---------------------------------------------
/*! \brief  Constructeur de la classe. Appelé par drtux::FusionneDocument.
 * \todo Utiliser Atcd_Code, Var_Code
*/
CDevilCrucible::CDevilCrucible(QString                *pDocument     ,       // 0 texte du document à fusionner
                               CMoteurBase            *pCMoteurBase  ,       // 1 moteur de base de données (faut bien accéder aux fonctions)
                               CMedicaBase            *pCMedicaBase  ,       // 2 moteur de base de données medicamenteuses(faut bien accéder aux fonctions)
                               RUBREC_LIST            *pRubList      ,       // 3 liste des documents composant le dossier patient
                               QString                &identPrimKey  ,       // 4 il faut les renseignements sur le patient
                               const DOCUMENT_DISPLAY_MAP   *pCurDisplayDocMap,    // 5 liste des rubriques courantes (affichées)
                               QString                &userActuelPk  ,       // 6 utilisateur courant du programme
                               CRubRecord             *pCRubCurrentRecord,   // 7 pointeur sur la rubrique en cours de modif (si c'est une rubrique) zero sinon
                               VAR_MAP *               pVariables,           // 8 pointeur sur la liste des variables (zero par defaut)
                               ACTION_TYPE             isJustForWatch        // 9 defaut = CDevilCrucible::DoAll indique que c'est juste pour la visualisation ou to faire
                              )
{m_pCurDisplayDocMap  = new DOCUMENT_DISPLAY_MAP;
 m_pCurDisplayDocMap->copy(pCurDisplayDocMap);

 m_pRubList           = pRubList;
 m_pMB                = pCMoteurBase;
 m_pMD                = pCMedicaBase;
 m_IdentPrimKey       = identPrimKey;
 m_pDocument          = pDocument;
 m_UserActuelPk       = userActuelPk;
 m_UserDocPk          = userActuelPk;
 m_pCRubCurrentRecord = pCRubCurrentRecord;
 m_isJustForWatch     = isJustForWatch;

 C_TokenInterpret::init(this);
 if (pVariables==0)
    {m_pVariables = new VAR_MAP;
     m_VariablesToDelete = TRUE;
    }
 else
    {m_pVariables = pVariables;
     m_VariablesToDelete = FALSE;
    }

 CRubRecord  *pCRubRecord                = C_TokenInterpret::GetIDCurrentDoc(TR("Observation"));
 if (pCRubRecord) m_UserDocPk            = m_pMB->GetUserPrimKey(pCRubRecord->m_SignUser);
}

/*! \brief destructeur de la classe */
CDevilCrucible::~CDevilCrucible()
{if (m_VariablesToDelete && m_pVariables) delete m_pVariables;
 if (m_pCurDisplayDocMap) delete m_pCurDisplayDocMap;
}

/*! \brief destructeur de la classe */
void CDevilCrucible::regenerateDisplayMap()
{    m_pCurDisplayDocMap->clear();
     if (G_pCApp==0)           return;
     if (G_pCApp->m_pDrTux==0) return;
     DOCUMENT_DISPLAY_MAP mapDoc;
     G_pCApp->m_pDrTux->MapActiveID_Doc(mapDoc);
     m_pCurDisplayDocMap->copy(mapDoc);
     /* CMDI_Generic *pCMDI_Generic = (CMDI_Generic*) */ G_pCApp->m_pDrTux->GetCurrentRubrique(0, 0, &m_pCRubCurrentRecord);
    /*
     // G_pCApp->m_pDrTux->MapActiveID_Doc(mapDoc);
     // G_mCDC->m_pCurDisplayDocMap->copy(mapDoc);
     // * G_mCDC->m_pCurDisplayDocMap = G_pCApp->m_pDrTux->GetMapActiveID_Doc();
     //  G_pCApp->m_pDrTux->MapActiveID_Doc(mapDoc);
    */

}

//----------------------------------------  DoMixture -------------------------------------------------
/*! \brief Fusionne les champs de fusion un document donnée en entrée par: QString &document cette fonction demande un contexte sur l'observation en cours afin de tenir compte de l'utilisateur ayant créé cette observation ainsi que des elements propres à l'observation: elements du contenu etc....
 * \return QString:  qui est l'utilisateur lié à ce document. En effet l'utilisateur du document peut etre différent de l'utilisateur en cours. Exemple je travaille sur ce patient, et j'edite un certificat créé auparavant sous un autre utilisateur.
 * NOTE:  la date de visite, et l'utilisateur createur du document à fusionner doivent etre imperativement à jour et sont recupérés dans: (*pRubList->at(idDoc)).m_Date et (*pRubList->at(idDoc)).m_User
 */
void CDevilCrucible::DoMixture()
{
 m_afterDataTag       = "";
 int p                = m_pDocument->find("</HTML_Data>");
 if (p !=-1) 
    { m_afterDataTag  = m_pDocument->mid(p);
      m_pDocument->truncate(p);
    }

 m_pDocument->replace("{{\\<br>"   ,"{{");
 m_pDocument->replace("{{\\<br/>"  ,"{{");
 m_pDocument->replace("{{\\<br />" ,"{{");
/*
 m_pDocument->replace("{{\\<p>","{{");
 m_pDocument->replace("{{\\<p />","{{");
 m_pDocument->replace("{{\\\n","{{");
 m_pDocument->replace("{{\\\r","{{");
*/
 m_pDocument->replace("}}\\<br/>"  ,"}}");
 m_pDocument->replace("}}\\<br>"   ,"}}");
 m_pDocument->replace("}}\\<br />" ,"}}");
/*
 m_pDocument->replace("}}\\<p>","}}");
 m_pDocument->replace("}}\\<p />","}}");
 m_pDocument->replace("}}\\\n","}}");
 m_pDocument->replace("}}\\\r","}}");
*/
 DoMixture(0, m_pDocument->length());
 if (p !=-1) 
    { m_pDocument->append( m_afterDataTag );
    }
 m_afterDataTag    = "";
}

//----------------------------------  DoMixture -------------------------------------------------
/*! \brief Idem que supra, mais en débutant au caractère pos_deb jusqu'au caractère pos_fin du texte.
 * \return QString:  qui est l'utilisateur lié à ce document. En effet l'utilisateur du document peut etre différent de l'utilisateur en cours. Exemple je travaille sur ce patient, et j'edite un certificat créé auparavant sous un autre utilisateur.
 * NOTE:  la date de visite, et l'utilisateur createur du document à fusionner doivent etre imperativement à jour et sont recupérés dans: (*pRubList->at(idDoc)).m_Date et (*pRubList->at(idDoc)).m_User
 */
int CDevilCrucible::DoMixture(int pos_deb, int pos_max)
{//G_pCApp->m_VAR_MAP.insert("$SCRIPT_RUN","$TRUE");
 const char *ptDeb;
 const char *ptEnd;
 int isWithToken   =  0;
 int pos_end       =  0;

 pos_deb           =  m_pDocument->find("{{", pos_deb);
 int pDeltaLen     =  0;
 int intialLen     =  m_pDocument->length();
 while (pos_deb   != -1 && pos_deb < pos_max)
       {pos_end        =  CGestIni::findFermant(m_pDocument, pos_deb+2, pos_max, "{{", "}}", isWithToken);
        ptDeb = *m_pDocument; ptDeb += pos_deb;
        ptEnd = *m_pDocument; ptEnd += pos_end;
        if (isWithToken)
           {pos_max   += DoMixture(pos_deb+2, pos_end);
           }
        else
           {
            ReplaceTokenByValue(*m_pDocument, pos_deb+2, pos_end - pos_deb-2 ,&pDeltaLen);
            ptDeb = *m_pDocument; ptDeb += pos_deb;
            ptEnd = *m_pDocument; ptEnd += pos_end;
            pos_max  += pDeltaLen;
            pos_deb   =  m_pDocument->find("{{", pos_deb);
            ptDeb     = *m_pDocument; ptDeb += pos_deb;
           }
       }
  //G_pCApp->m_VAR_MAP.insert("$SCRIPT_RUN","$FALSE");
  return m_pDocument->length() - intialLen;
}

//----------------- ReplaceTokenByValue --------------------------------------------------
/*! \brief Lorsque le code a été trouvé et interprété, remplace dans le texte le code par son résultat.
 *  \param document : QString du texte entier
 *  \param pos_deb : caractère de départ du remplacement
 *  \param len : longueur
 *  \param pDeltaLen ??
*/
int CDevilCrucible:: ReplaceTokenByValue(QString &document, int pos_deb, int len , int *pDeltaLen /*=0*/)
{int originalLen = document.length();
 //QString token = document.mid(pos_deb,len);
 //qDebug(token);
 QString  resolvToken = GetResolvTokenValue(document, pos_deb,  len )  ;
 resolvToken.replace("SautDeLigne", "<br />");
 document.replace ( pos_deb-2, len+4, resolvToken);
 if (pDeltaLen) *pDeltaLen = document.length() - originalLen;
 return pos_deb-2+resolvToken.length();
}

//------------------------------ GetResolvTokenValue ---------------------------------------
/*! \brief Analyse le texte scripté passé en paramètre et "compile" avec les données calculées.
 *  \return les données calculées
*/
QString CDevilCrucible::GetResolvTokenValue(QString &document, int pos_deb, int len )
{char insecSpace[2]={(char)0xa0,(char)0x00};
 QString  token = document.mid(pos_deb, len).ascii ();
          token.replace((char)0xa0," ");
          token.replace("&gt;",">");
          token.replace("&lt;","<");
          token.replace("&amp;","&");
          token.replace("&nbsp;",insecSpace);

 QString  resolvToken("");
 if (token.left(2)=="::")
    {C_TokenInterpret::ResolvFunctionToken(this, resolvToken, token.mid(2));
    }
 else if (C_TokenInterpret::ResolvMacroToken( this, resolvToken, token)==0)
   { exeItemsFrom_I_To_L(resolvToken, token, document);
   }
 return resolvToken;
}


//------------------------------ exeItemsFrom_I_To_L ---------------------------------------
/*! \brief Interprète les items de scripts débutant par la lettre I à L */
void CDevilCrucible::exeItemsFrom_I_To_L(QString &resolvToken, QString token, QString &document)
{ int f;
  if ( ( f=token.find('*')) !=-1)   // syntaxe {{ *20}}  ou {{\t*20}}
   {int        nb = atoi(token.mid(f+1));
    QString motif = token.left(f);
    if (motif    != "" )
       {if (motif[0]=='\\')  // echapement exple "\t"
           {
            char escape_car = motif.at(1).latin1();
            switch(escape_car)
              {case 't':
                    resolvToken.fill('\t', nb);
                    break;
               case 'n':
                    resolvToken.fill('\n', nb);
                    break;
               case '\\':
                    resolvToken.fill('\\', nb);
                    break;
               case 'r':
                    resolvToken.fill('\r', nb);
                    break;
               default:
                    resolvToken.fill(motif[1], nb);
                    break;
              } // endswitch(token[1])
           } //endif (token[0]=='\\')  // echapement
        else
           {resolvToken.fill(motif[0], nb);
           }
       }// endif (motif != "" )
   }
   else if (token.left(2) == "IF")  //  {{IF sexe = Féminin|S||}}
    {int op  =  0;
     int pos = -1;
     if      (op==0 && (pos =token.find('='))  != -1) {op = 1; }
     else if (op==0 && (pos =token.find('>'))  != -1) {op = 2; }
     else if (op==0 && (pos =token.find('<'))  != -1) {op = 3; }
     else if (op==0 && (pos =token.find(">=")) != -1) {op = 4; }
     else if (op==0 && (pos =token.find("<=")) != -1) {op = 5; }
     else if (op==0 && (pos =token.find("!=")) != -1) {op = 6; }
     else if (op==0 && (pos =token.find("%=")) != -1) {op = 7; }
     else if (op==0 && (pos =token.find("=%")) != -1) {op = 8; }
     else if (op==0 && (pos =token.find("%%")) != -1) {op = 9; }

     if (op == 0) resolvToken =  "Syntax error: operator not find";
     else
        {QString val          = token.mid(2, pos - 2).stripWhiteSpace();
         float  fVal          = val.toFloat();
         switch (op)
           {case 1: {QStringList values = QStringList::split('|', token.mid(pos+1), TRUE);
                     if (val==values[0].stripWhiteSpace() ) resolvToken = values[1].stripWhiteSpace();
                     else                                   resolvToken = values[2].stripWhiteSpace();
                     break;
                    }
            case 2: {QStringList values = QStringList::split('|', token.mid(pos+1), TRUE);
                     if (fVal>values[0].stripWhiteSpace().toFloat() )  resolvToken = values[1].stripWhiteSpace();
                     else                                   resolvToken = values[2].stripWhiteSpace();
                     break;
                    }
            case 3: {QStringList values = QStringList::split('|', token.mid(pos+1), TRUE);
                     if (fVal<values[0].stripWhiteSpace().toFloat() )  resolvToken = values[1].stripWhiteSpace();
                     else                                   resolvToken = values[2].stripWhiteSpace();
                     break;
                    }
            case 4: {QStringList values = QStringList::split('|', token.mid(pos+2), TRUE);
                     if (fVal>=values[0].stripWhiteSpace().toFloat() )  resolvToken = values[1].stripWhiteSpace();
                     else                                    resolvToken = values[2].stripWhiteSpace();
                     break;
                    }
            case 5: {QStringList values = QStringList::split('|', token.mid(pos+2), TRUE);
                     if (fVal <= values[0].stripWhiteSpace().toFloat() )  resolvToken = values[1].stripWhiteSpace();
                     else                                    resolvToken = values[2].stripWhiteSpace();
                     break;
                    }
            case 6: {QStringList values = QStringList::split('|', token.mid(pos+2), TRUE);
                     if (val != values[0].stripWhiteSpace() )  resolvToken = values[1].stripWhiteSpace();
                     else                                    resolvToken = values[2].stripWhiteSpace();
                     break;
                    }
            case 7: {QStringList values = QStringList::split('|', token.mid(pos+2), TRUE);
                     if (val.startsWith(values[0].stripWhiteSpace() ))  resolvToken = values[1].stripWhiteSpace();
                     else                                    resolvToken = values[2].stripWhiteSpace();
                     break;
                    }
            case 8: {QStringList values = QStringList::split('|', token.mid(pos+2), TRUE);
                     if (val.endsWith(values[0].stripWhiteSpace() ))  resolvToken = values[1].stripWhiteSpace();
                     else                                    resolvToken = values[2].stripWhiteSpace();
                     break;
                    }
            case 9: {QStringList values = QStringList::split('|', token.mid(pos+2), TRUE);
                     if (val.contains(values[0].stripWhiteSpace() ))  resolvToken = values[1].stripWhiteSpace();
                     else                                    resolvToken = values[2].stripWhiteSpace();
                     break;
                    }
           } // endswitch (op)
        }
    }
   else if (token.left(13)== "INTERV_STRUCT") //{{INTERV_STRUCT_XXX = Médecine generale STYLE = liste CHAMPS DESIRES = TNPSACV DECALAGE=    }}
    {
     QStringList listData;
     //......................... specification champ intervenant .....................
     int pos_deb         = token.find('=');
     if (pos_deb==-1)    {resolvToken = "Syntax error in INTERV_STRUCT first '=' not found";     return ;}
     QString prefix_ID   = token;
     prefix_ID.truncate(pos_deb);
     prefix_ID = prefix_ID.mid(13);
     prefix_ID = prefix_ID.stripWhiteSpace();

     int pos_end         = token.find("STYLE");
     if (pos_end==-1)    {resolvToken = "Syntax error in INTERV_STRUCT 'STYLE' not found";     return ;}
     ++pos_deb;    // passer le signe egal
     QString specialite  = token.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();

     pos_deb             = token.find('=', pos_end + 5);
     if (pos_deb==-1)    {resolvToken = "Syntax error in INTERV_STRUCT '=' after 'STYLE' not found";   return;}
     pos_end             = token.find("CHAMPS DESIRES");
     if (pos_end==-1)    {resolvToken = "Syntax error in INTERV_STRUCT 'CHAMPS DESIRES' not found";   return ;}
     ++pos_deb;    // passer le signe egal
     QString style       = token.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();

     pos_deb          = token.find('=', pos_end + 14);
     if (pos_deb==-1)    {resolvToken = "Syntax error in INTERV_STRUCT '=' after 'CHAMPS DESIRES' not found"; return ;}
     ++pos_deb;    // passer le signe egal
     pos_end          = token.find("DECALAGE", pos_deb);
     if (pos_end==-1)    {resolvToken = "Syntax error in INTERV_STRUCT 'DECALAGE' not found";   return ;}
     QString validStr = token.mid(pos_deb, pos_end - pos_deb).stripWhiteSpace();

     pos_deb          = token.find('=', pos_end + 8);
     if (pos_deb==-1)    {resolvToken = "Syntax error in INTERV_STRUCT '=' after 'DECALAGE' not found";    return ;}
     QString decalage = token.mid(pos_deb+1);
     DoFusionIntervenantStruct(m_IdentPrimKey , specialite, validStr, prefix_ID, style, decalage, document);
    }
}

//-------------------------------------- DoFusionIntervenantStruct ----------------------------------------------------------------------------
void CDevilCrucible::DoFusionIntervenantStruct(const char *primKeyPat ,
                                               const QString &specialite,
                                               QString validStr,
                                               QString &prefix_ID,
                                               const QString &style,
                                               const QString &decalage,
                                               QString &text) //Médecin généraliste
{ QStringList pkIntervList;
  int i_style      = 0;  // champs nommes
  validStr.remove(' ');
  validStr.remove(',');
  prefix_ID.prepend("{{INTERV_CHAMP");
  prefix_ID += "_";                             // la prefix_ID doit etre de la forme :  {{INTERV_CHAMP_XXX_
  if (style.left(1).upper()=="L") i_style = 1;
  //................. Preparer la requete .....................................
  if (m_pMB->OpenBase()==0)  return ;

  //................. recuperer tous les PK des intervenants sur ce patient ..................................................
  QString requete("");
  requete  += "SELECT " +   m_pMB->m_DOSS_INTERVENANTS_INTERVPK  + " FROM " + m_pMB->m_DOSS_INTERVENANTS_TBL_NAME   + " WHERE ";
  requete  +=               m_pMB->m_DOSS_INTERVENANTS_PAT_PK    + " = '"   + primKeyPat                            + "' ";
  QSqlQuery query(requete , m_pMB->m_DataBase );

  //.................. si la requète a un resultat ..............................................
  if (query.isActive())
     { while (query.next())
         {pkIntervList.append( query.value( 0 ).toString());
         }
     }
  QString fieldToRetrieve = "SELECT ";
  char *pt = (char *) (const char*)validStr;
  while (*pt)
      {fieldToRetrieve += IdentValidCarToFieldName(pt[0]);
       ++pt;
       if (*pt) fieldToRetrieve += ",";
      }
  if (pkIntervList.count()==0)  {m_pMB->CloseBase(); return;}
  for ( QStringList::Iterator rt = pkIntervList.begin(); rt != pkIntervList.end(); ++rt )
      {requete   = fieldToRetrieve;
       requete  += " FROM " + m_pMB->m_INTERVENANTS_TBL_NAME + " WHERE ";
       requete  +=   m_pMB->m_INTERVENANTS_PK    + " = '"    + *rt + "' ";
       if (specialite.length()) requete  += "AND "    + m_pMB->m_INTERVENANTS_SPEC + " = '" + specialite + "'";
       query.exec( requete );
       if (query.isActive())
          {QString ofset = decalage ;
           while (query.next())
              {int       i = 0;
               int       f = 0;
                        pt = (char *) (const char*)validStr;
               while (*pt)        // explorer les champs ainsi que determines par la liste de validation
                     {QString  result = m_pMB->Utf8_Query(  query, i++  );
                      if (i_style)           // champs listes de la forme INTERV_CHAMP_XXX_1 INTERV_CHAMP_XXX_2
                         {QStringList str_list = QStringList::split('\n', result.replace("\r\n", "\n"));
                          for ( QStringList::Iterator it = str_list.begin(); it != str_list.end(); ++it )
                              { text.replace(prefix_ID + QString::number(++f) + "}}", ofset + *it);
                              }
                         }
                      else                  // champs nommes de la forme INTERV_CHAMP_XXX_NOM INTERV_CHAMP_XXX_PRENOM
                         { switch (*pt)
                              {case 'N': text.replace(prefix_ID + "NOM}}",        ofset + result);    break;
                               case 'P': text.replace(prefix_ID + "PRENOM}}",     ofset + result);    break;
                               case 'A': {result               = result.replace("\r\n", "\n");
                                          QStringList adr_list = QStringList::split('\n',result);
                                          int n = 0;
                                          for ( QStringList::Iterator it = adr_list.begin(); it != adr_list.end(); ++it )
                                              { text.replace(prefix_ID + "ADR_" + QString::number(++n) + "}}", ofset + *it);
                                              }
                                         } break;
                               case 'C': text.replace(prefix_ID + "CODE_POST}}",  ofset + result);    break;
                               case 'V': text.replace(prefix_ID + "VILLE}}",      ofset + result);    break;
                               case 'T': text.replace(prefix_ID + "TITRE}}",      ofset + result);    break;
                               case 'S': text.replace(prefix_ID + "SPECIALITE}}", ofset + result);    break;
                              }
                         } //endif (i_style) champs listes ou cnamps nommes
                      ++pt;
                     } //end while (*pt)
              } // end while (query.next()) (ne devrait y en avoir qu'un)
          } // endif (query.isActive())
      }
  m_pMB->CloseBase();
  return ;
}

//-------------------------------------- IdentValidCarToFieldName ----------------------------------------------------------------------------
QString CDevilCrucible::IdentValidCarToFieldName(int c)
{switch (c)
   {case 'N': return m_pMB->m_INTERVENANTS_NOM;
    case 'P': return m_pMB->m_INTERVENANTS_PRENOM;
    case 'A': return m_pMB->m_INTERVENANTS_RUE;
    case 'C': return m_pMB->m_INTERVENANTS_CODE_POST;
    case 'V': return m_pMB->m_INTERVENANTS_VILLE;
    case 'T': return m_pMB->m_INTERVENANTS_TITRE;
    case 'S': return m_pMB->m_INTERVENANTS_SPEC;
   }
 return QString("");
}


