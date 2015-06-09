// GestRPU.cpp: implementation of the CGestRPU class.
/***************************************************************************
 *   Copyright (C) 2003 by Sevin Roland                                    *
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
//////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include <qsqlquery.h>
#include "GestRPU.h"

//============================================================  SSignal ===================================================================================
int SSignal::setFromGUID(const QString& guid, CMoteurBase* pCMoteurBase)
{if (pCMoteurBase->m_DataBase->isOpen()==FALSE && pCMoteurBase->m_DataBase->open()==FALSE)  return 0;  // si point = erreur d'ouverture de la base

 QString requete("SELECT ");
 requete +=  pCMoteurBase->m_DOSS_INDEX_PRIM_KEY   + ","       +        // 0
	     pCMoteurBase->m_DOSS_INDEX_NOM        + ","       +        // 1
	     pCMoteurBase->m_DOSS_INDEX_PRENOM     + " FROM  " +        // 2
             pCMoteurBase->m_DOSS_INDEX_TBL_NAME   + " WHERE " +
             pCMoteurBase->m_DOSS_INDEX_GUID       + " = '"    + guid + "'";
 QSqlQuery query(requete , *pCMoteurBase->m_DataBase );
 //.................. Recuperer la liste des patients ..............................................
 if ( query.isActive() && query.next())
    {m_Nom      = query.value(1).toString();
     m_Prenom   = query.value(2).toString();
     m_GUID     = guid;
     QString pk = query.value(0).toString();
     //.............. nouvelle requete à la recherche du reste .....................................
     requete = "SELECT ";
     requete +=  pCMoteurBase->m_DOSS_IDENT_CODE_POST   + ","       +        // 0
	         pCMoteurBase->m_DOSS_IDENT_SEX         + ","       +        // 1
	         pCMoteurBase->m_DOSS_IDENT_NSS         + " FROM  " +        // 2
                 pCMoteurBase->m_DOSS_IDENT_TBL_NAME    + " WHERE " +
                 pCMoteurBase->m_DOSS_IDENT_REF_PK      + " = '" + pk + "'";
     query.exec(requete);
     if ( query.isActive() && query.next())
        {m_codePostal  = query.value(0).toString();
	 QString s     = query.value(1).toString();
	 m_sexe[0]     = s.at(0).latin1();
	 QString dtNss = query.value(2).toString();           // 1999-03-02 00:00:00
	 m_Naissance.setYMD ( dtNss.left(4 ).toInt(),
	                      dtNss.mid(5,2).toInt(),
			      dtNss.mid(8,2).toInt() );
	 return 1;
        }
    }
 return 0;
}



//============================================================  CGestRPU ===================================================================================
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*----------------------- CONSTRUCTEUR ------------------------------*/
CGestRPU::CGestRPU(QDate &date_Debut,int hdeb, int mdeb, QDate &date_Fin,int hfin, int mfin,
                   const char *errPath /*=0*/,const char *errName /*=0*/,
                   const char *logPath /*=0*/,const char *logName /*=0*/,
                   const char *rpuPath /*=0*/,const char *rpuName /*=0*/,
                   long flag, char qualif /* = '3'*/ , const char *finess /* = "123456"*/ )
{
 QString csErrPath     = "";
 QString csLogPath     = "";
 QString csRpuPath     = "";

 m_csErrPath     = "";
 m_csLogPath     = "";
 m_csRpuPath     = "";
 m_date_Debut    = date_Debut;
 m_date_Fin      = date_Fin;
 m_Hdeb          = hdeb;
 m_Mdeb          = mdeb;
 m_Hfin          = hfin;
 m_Mfin          = mfin;
 //............. si cas ou le test des dates n'est pas desire ..............
 if (m_date_Debut==m_date_Fin &&
     m_Hdeb==0 && m_Hfin==0   &&
     m_Mdeb==0 && m_Mfin==0
    )
    {flag |= NO_TEST_DATE;
    }

 PathConcat(errPath, errName, &csErrPath);
 PathConcat(logPath, logName, &csLogPath);
 PathConcat(rpuPath, rpuName, &csRpuPath);
 m_ErrRpuList.setAutoDelete (TRUE );
 InitRpuTest(flag, csErrPath, csLogPath, csRpuPath, qualif, finess);
}

/*----------------------- CONSTRUCTEUR ------------------------------*/
CGestRPU::CGestRPU(QDate &date_Debut,int hdeb, int mdeb, QDate &date_Fin,int hfin, int mfin,
                   const char *errPath /*=0*/, const char *logPath/*=0*/, const char *rpuPath/*=0*/,
                   long flag, char qualif /* = '3'*/ , const char *finess /* = "123456"*/ )
{m_csErrPath     = "";
 m_csLogPath     = "";
 m_csRpuPath     = "";
 m_date_Debut    = date_Debut;
 m_date_Fin      = date_Fin;
 m_Hdeb          = hdeb;
 m_Mdeb          = mdeb;
 m_Hfin          = hfin;
 m_Mfin          = mfin;

 //............. si cas ou le test des dates n'est pas desire ..............
 if (m_date_Debut==m_date_Fin &&
     m_Hdeb==0 && m_Hfin==0   &&
     m_Mdeb==0 && m_Mfin==0
    )
    {flag |= NO_TEST_DATE;
    }

 m_ErrRpuList.setAutoDelete (TRUE );
 InitRpuTest(flag, errPath, logPath, rpuPath, qualif, finess);
}

/*----------------------- CONSTRUCTEUR ------------------------------*/
CGestRPU::CGestRPU(QDate &date_Debut,int hdeb, int mdeb, QDate &date_Fin,int hfin, int mfin,  long flag)
{m_csErrPath     = "";
 m_csLogPath     = "";
 m_csRpuPath     = "";
 m_date_Debut    = date_Debut;
 m_date_Fin      = date_Fin;
 m_Hdeb          = hdeb;
 m_Mdeb          = mdeb;
 m_Hfin          = hfin;
 m_Mfin          = mfin;
 //............. si cas ou le test des dates n'est pas desire ..............
 if (m_date_Debut==m_date_Fin &&
     m_Hdeb==0 && m_Hfin==0   &&
     m_Mdeb==0 && m_Mfin==0
    )
    {flag |= NO_TEST_DATE;
    }

 m_ErrRpuList.setAutoDelete (TRUE );
 InitRpuTest(flag, 0, 0, 0, '3', "123456789");
}

/*----------------------- CONSTRUCTEUR ------------------------------*/
CGestRPU::CGestRPU(long flag)
{m_csErrPath     = "";
 m_csLogPath     = "";
 m_csRpuPath     = "";
 m_date_Debut    = QDate::currentDate();
 m_date_Fin      = m_date_Debut;
 m_Hdeb          = 0;
 m_Mdeb          = 0;
 m_Hfin          = 0;
 m_Mfin          = 0;
 flag           |= NO_TEST_DATE;
 //............. si cas ou le test des dates n'est pas desire ..............
 m_ErrRpuList.setAutoDelete (TRUE );
 InitRpuTest(flag, 0, 0, 0, '3', "123456789");
}

/*------------------------ DESTRUCTEUR ------------------------------*/
CGestRPU::~CGestRPU()
{ResetErrorList();
}

/*------------------------ ResetErrorList ----------------------------------*/
void CGestRPU::ResetErrorList(void)
{ m_ErrRpuList.clear();
}

/*----------------------- SetRpuQualif ------------------------------*/
void CGestRPU::SetRpuQualif(char qualif , const char *finess )
{if (finess) m_ED_Finess     = finess;
 if (qualif) m_Qualif        = qualif;
}

/*----------------------- InitRpuTest ------------------------------*/
void CGestRPU::InitRpuTest(long flag /* =0 */, const char *errPath /* =0 */, const char *logPath /* =0 */, const char *rpuPath /* =0 */, char qualif /* =0 */, const char *finess /* =0 */)
{
 /*.................. creer le fichier de sortie ........................*/
 QString   sDate;
 QDateTime dtDateEnCours = QDateTime::currentDateTime();
 QString   endMess       = " ///////////////\r\n";

 ResetErrorList();
 m_NbErr         = 0;
 m_RPU_Record    = 0;
 m_NbFicWrited   = 0;
 if (flag)   m_TestFlag      = flag;
 if (finess) m_ED_Finess     = finess;
 if (qualif) m_Qualif        = qualif;

 memset(&m_CCMU,  0,sizeof(m_CCMU));
 memset(&m_DEST,  0,sizeof(m_DEST));
 memset(&m_GEMSA, 0,sizeof(m_GEMSA));
 memset(&m_DIVERS,0,sizeof(m_DIVERS));

 if (errPath && *errPath)
    {sDate  = dtDateEnCours.isValid() ? (TR("//////////// Dossiers invalides pour RPU : ") + dtDateEnCours.toString("dd-MM-yyyy") + endMess) : QString(""); //POUR LA DATE DU JOUR
     m_csErrPath = errPath;
     LogCreate(sDate, errPath);
    }
 if (logPath && *logPath)
    {sDate  = dtDateEnCours.isValid() ? (TR("//////////// Dossiers retenus pour RPU :   ") + dtDateEnCours.toString("dd-MM-yyyy") + endMess) : QString(""); //POUR LA DATE DU JOUR
     m_csLogPath = logPath;
     LogCreate(sDate, logPath);
    }
 if (rpuPath && *rpuPath)
    {m_csRpuPath = rpuPath;
     LogCreate("", rpuPath);
    }
}

/*----------------------- RPU_TesteRecord ------------------------------*/
long CGestRPU::RPU_TesteRecord(SSignal *pSignalPatient, const char* observ, long len_observ, const char* medecin)
{if (observ ==0)                        return -2;
 if (len_observ==0)                     return -3;
 if (*observ==0)                        return -4;

 {long D_deb,M_deb,Y_deb,h_deb,m_deb,s_deb;
  long D_end,M_end,Y_end,h_end,m_end,s_end;
  QDateTime coDt_deb;
  QDateTime coDt_end;
  int err_deb         = GetObservDateDeb(observ, len_observ, coDt_deb, D_deb,M_deb,Y_deb,h_deb,m_deb,s_deb);
  int err_end         = GetObservDateEnd(observ, len_observ, coDt_end, D_end,M_end,Y_end,h_end,m_end,s_end);
  QString mess        = "";
  long            err = 0;
  //.................... VERIFIER SI DATES DOSSIER DANS LA BONNE FOURCHETTE DEBUT......................
  if (err_deb == 0)      // date de début valide
     {err                 = IsDateInfDebLimit(D_deb,M_deb,Y_deb,h_deb,m_deb,s_deb);
     }
  else     // date de debut fausse on ne rentre pas dans la boucle mais on signale l'erreur
     {int isDtValid = 1;
      mess  = "\r\n=========== ";
      mess  = mess + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
      if (err_deb & 1)    mess  = mess + TR("\r\n    Date début : erreur sur le jour");
      if (err_deb & 2)    mess  = mess + TR("\r\n    Date début : erreur sur le mois");
      if (err_deb & 4)    mess  = mess + TR("\r\n    Date début : erreur sur l'année");
      if (err_deb & 8)    mess  = mess + TR("\r\n    Date début : erreur sur l'heure");
      if (err_deb & 16)   mess  = mess + TR("\r\n    Date début : erreur sur les minutes");
      if (err_deb & 32)   mess  = mess + TR("\r\n    La mention : \"Date:\" n'a pas été trouvée");
      if (err_deb & 64)   mess  = mess + TR("\r\n    La mention : \"Heure d'Arrivée SU:\" n'a pas été trouvée");
      if (err_deb == 128) mess  = mess + TR("\r\n    Date début : invalide (probable dépassement de jours dans le mois)");
      // LogWrite(mess, gsCheminPromethe, "ErreurStat.log");
      WriteErreur(mess);
 	  //....... ajouter dossier dans liste d'erreurs .....................
      mess   = TR("Début dossier :");
      if (err_deb == 128)
         {if (h_deb>23||h_deb<0)           mess += TR(" Heure invalide :  (>23 ou <0)");
          if (m_deb>59||m_deb<0)           mess += TR(" Minute invalide : (>59 ou <0)");
          if (mess==TR("Début dossier :")) mess += TR(" Date invalide : (probablement trop de jours pour le mois)");
         }
      if (err_deb & 96)
         {mess += TR(" Mention(s) :");
          if (err_deb & 32) {mess += TR(" \"Date:\"");               isDtValid=0; }
          if (err_deb & 64)  mess += TR(" \"Heure d'Arrivée SU:\"");
          mess += TR(" non trouvée(s)");
         }
      err_deb &= 31;   // annuler les erreurs precedentes
      if (err_deb)
         {mess += TR(" Saisie :");
          if (err_deb&7) {mess += TR(" date");                       isDtValid = 0;}
          if (err_deb&16) mess += TR(" heure");
          mess += TR(" invalide ou absente");
         }
      if (isDtValid) InsertError(pSignalPatient, ValJJMMAAAA_To_Str(D_deb, M_deb, Y_deb, "-"), mess, medecin);
      else           InsertError(pSignalPatient, "../../....",                                 mess, medecin);
      err = TRUE;
     }
  //.................... VERIFIER SI DATES DOSSIER DANS LA BONNE FOURCHETTE FIN......................
  if (err==FALSE)  // si dates début dossier dans la bonne fourchette de DEBUT du test et bien saisies: (err==FALSE)
     {err   = IsDateSupEndLimit(D_deb,M_deb,Y_deb,h_deb,m_deb,s_deb); // verifier si date debut dans bonne fourchette de FIN du test
      if (err == FALSE && err_end != 0) // si date de debut dans bonne fourchette de FIN du test mais date de fin mal saisie
         {mess  = "\r\n=========== ";
          mess  = mess + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
          if (err_end & 1)    mess  = mess + TR("\r\n    Date fin : erreur sur le jour");
          if (err_end & 2)    mess  = mess + TR("\r\n    Date fin : erreur sur le mois");
          if (err_end & 4)    mess  = mess + TR("\r\n    Date fin : erreur sur l'année");
          if (err_end & 8)    mess  = mess + TR("\r\n    Date fin : erreur sur l'heure");
          if (err_end & 16)   mess  = mess + TR("\r\n    Date fin : erreur sur les minutes");
          if (err_end & 32)   mess  = mess + TR("\r\n    La mention : \"Date SU:\" n'a pas été trouvée");
          if (err_end & 64)   mess  = mess + TR("\r\n    La mention : \"H.départ SU:\" n'a pas été trouvée");
          if (err_end == 128) mess  = mess + TR("\r\n    Date fin : invalide (probable dépassement de jours dans le mois)");
          //LogWrite(mess, gsCheminPromethe, "ErreurStat.log");
          WriteErreur(mess);
          //....... ici QDateTime debut ne peut etre que valide !!! ..............
          //list->SetItemText(NbErr, 1, ValJJMMAAAA_To_Str(D_deb, M_deb, Y_deb, "-"));
          mess = TR("Fin dossier :");
          if (err_end == 128)
             {if (h_end>23||h_end<0)         mess += TR(" Heure invalide :  (>23 ou <0)");
              if (m_end>59||m_end<0)         mess += TR(" Minute invalide : (>59 ou <0)");
              if (mess==TR("Fin dossier :")) mess += TR(" Date invalide : (probablement trop de jours pour le mois)");
             }
         if (err_end & 96)
            {mess += TR(" Mention(s) :");
             if (err_end & 32) mess += TR(" \"Date SU:\"");
             if (err_end & 64) mess += TR(" \"H.départ SU:\"");
             mess += TR("non trouvée(s)");
            }
         err_end &= 31;   // annuler les erreurs precedentes
         if (err_end)
            {mess += TR(" Saisie :");
             if (err_end &24) mess +=TR(" heure");
             if (err_end &7)  mess +=TR(" date");
             mess += TR(" invalide ou absente");
            }
         InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), mess, medecin);
         err = TRUE;           // mais ne peut etre retenu pour RPU
        }
     } // endif (err==FALSE)

  //............................. SI DATES OK et dans la bonne fourchette de test.........................................
  if (err==FALSE)
     { int dst = -1;
       m_RPU_Record++;       // ce dossier est dans la bonne fourchette pour la date d'entrée

       // #define RPU_TEST      1
       // #define CCMU_TEST     2
       // #define GEMSA_TEST    4
       // #define DEST_TEST     8
       // #define DIVERS_TEST   16
       if (m_TestFlag&DEST_TEST)   dst = CompteDEST( pSignalPatient, observ, len_observ, m_DEST,  coDt_deb, medecin);
       if (m_TestFlag&RPU_TEST)    RPU_Make(    pSignalPatient, observ, len_observ, coDt_deb, coDt_end, dst, medecin);
       if (m_TestFlag&CCMU_TEST)   CompteCCMU(  pSignalPatient, observ, len_observ, m_CCMU,   coDt_deb, medecin);
       if (m_TestFlag&GEMSA_TEST)  CompteGEMSA( pSignalPatient, observ, len_observ, m_GEMSA,  coDt_deb, medecin);
       if (m_TestFlag&DIVERS_TEST) CompteDIVERS(pSignalPatient, observ, len_observ, m_DIVERS, coDt_deb, medecin);

     } // endif (err==FALSE)
  } //endif (pSignal&&observ)
  return 1;
}

/*----------------------- RPU_ParseObservation ------------------------------*/
void CGestRPU::RPU_ParseObservation(const char *observ, long len, DATA_RPU &dataRpu)
{char *pt   = (char*) observ;
 len = strlen(pt);
 char *end  = pt + len;
 int act_ID = 0;
 //............... valeurs par defaut sur 'non' ..................
 dataRpu.Agressif[0] = '2';
 dataRpu.Communication[0] = '2';
 dataRpu.SocialEtAdmin[0] = '2';

 while (pt<end)
  {//........................**** TYPE D 'ARRIVEE ****..............................
   if (strncmp(pt, "Type d'arrivee :",16)==0)
      {pt += 15;
       while(pt<end     &&
             *pt !='\r' &&
             strncmp(pt, "Date:",5) !=0
            )
         {//................... TRANSPORT ....................
          if (dataRpu.TranspMedic[0]==0 && strncmp(pt, "Transport",9)==0)
             {pt += 9;
              while(pt<end    && *pt==' ') pt++;
              //.............. MEDICALISE ....................
              if (strncmp(pt,TR("médicalisé"),10)==0)
                 {pt += 10;
                  dataRpu.TranspMedic[0]='1';
                  while(pt<end    && *pt==' ') pt++;
                  //........... SMUR Iaire ...................
                  if (strncmp(pt,TR("SMUR Iaire"),10)==0)
                     {pt += 10;
                      dataRpu.TranspTyp[0]='1';
                     }
                  //........... SMUR IIaire ...................
                  else if (strncmp(pt,TR("SMUR IIaire"),11)==0)
                     {pt += 11;
                      dataRpu.TranspTyp[0]='2';
                     }
                  //........... AUTRE ...................
                  else dataRpu.TranspTyp[0]='3';
                 }
              //.............. NON MEDICALISE .................
              else
                 {dataRpu.TranspMedic[0]='2';
                 }
             } // end if (strncmp(pt, "Transport",9)==0)
           pt++;
          }// end while(pt<end    &&  *pt !='\r' && strncmp(pt, "Date:",5) !=0
      } // end if (strncmp(pt, "Type d'arrivee:",15)==0)

   //...............................**** DECEDE ****...................................
   else if (dataRpu.Decede[0]==0 && strncmp(pt, TR("Décédé:"),7)==0)
      {pt += 7;
       while(pt<end    && *pt !='\r')
         {if (strncmp(pt, "oui",3) ==0)
             {dataRpu.Decede[0]='1';
              pt += 3;
             }
          else if (strncmp(pt, "non",3) ==0)
             {dataRpu.Decede[0]='2';
              pt += 3;
             }
          else pt++;
         } // end while(pt<end    && pt !='\r') C.C.M.U.
      }

   //...............................**** CIM10 ****...................................
   else if (dataRpu.CIM10[0]==0 && strncmp(pt, "CIM10Urg",8)==0)
      {pt+=8;
       char *fst_str = pt;
       while(pt<end    && dataRpu.CIM10[0]==0)
         {if (*pt=='~' && pt[1]=='[')
             {//........... extraire le libélé du CIM10 ..............
              long len_cim10 = Min(pt-fst_str,255);
              memcpy(dataRpu.CIM10_Str, fst_str, len_cim10);
              dataRpu.CIM10_Str[len_cim10] = 0;
              char *end_str = &dataRpu.CIM10_Str[len_cim10];
              while(end_str>=dataRpu.CIM10_Str)
                   {if (*end_str =='\r' ||
                        *end_str =='\n' ||
                        *end_str =='\t'
                       )
                    *end_str = ' ';
                    end_str--;
                   }
              //........... extraire le code du CIM10 ..............
              pt +=2;
              end_str = pt;
              while(end_str<end    && *end_str !='\r')
                   {if (*end_str==']'&& end_str[1]=='~')
                       {char sav = *end_str; *end_str = 0;
                        QString cs = pt;     *end_str = sav;
                        cs = cs + "          ";
                        cs = cs.left(6);
                        strcpy(dataRpu.CIM10, cs);
                        pt=end_str;
                        end_str = end;   // pour breaker
                       }
                    end_str++;
                   } //end while(end_str<end    && *end_str !='\r')
             } //end while(pt<end    && *pt !='\r')
          pt++;
         }
      }
   //...............................**** Actes classants ****...................................
   else if (act_ID<=9 && strncmp(pt, "=[",2)==0)
      {pt+=2;
       char *end_str = pt;
       while(end_str<end    && *end_str !='\r')
            {if (*end_str==']'&& end_str[1]=='=')
                {long val;
                 char sav = *end_str; *end_str = 0;
                 QString cs = pt;     *end_str = sav;
                 cs = cs + "          ";
                 cs = cs.left(4);
                 strcpy(dataRpu.ActeRpu[act_ID].CodeAct, cs);
                 pt      = end_str+2;
                 //........... nombre d'actes (par defaut 1).............
                 //                justifié à gauche
                 if (ExtraitChiffre(&pt, &val, end)==TRUE)
                    {//cs = "00";     // pas bon car justifié à droite
                     //cs = cs + ltoa(val,buf,10);
                     //cs = cs.Right(2);
                     cs = QString::number(val);
                     cs = cs + "  ";
                     cs = cs.left(2);
                    }
                 else
                    {//cs = "01";     // pas bon car justifié à droite
                     cs = "1 ";
                    }
                 strcpy(dataRpu.ActeRpu[act_ID].NbAct, cs);
                 end_str = end;
                 act_ID++;
                }
             end_str++;
            } //end while(end_str<end    && *end_str !='\r')
      }
   //...............................**** Prise en charge ****...................................
   else if (dataRpu.PEC_Typ[0]==0 && strncmp(pt, "Prise en charge",15)==0)
      {pt+=15;
       while(dataRpu.PEC_Typ[0]==0 && pt<end    && *pt !='\r')
           {if (strncmp(pt, TR("Réa et SUSI"),11)==0)
               {pt += 11;
                dataRpu.PEC_Typ[0] = '1';      // par defaut surveillance de type reanimation
                while(pt<end    && *pt !='\r') // aller jusqu'en fin de ligne pour voir si complément d'info
                   {if (strncmp(pt, "d'1 fonction",12)==0)
                       {pt += 12;
                        dataRpu.PEC_Typ[0] = '2';
                       }
                    else if (strncmp(pt, "de 2 fonctions",14)==0)
                       {pt += 14;
                        dataRpu.PEC_Typ[0] = '3';
                       }
                    else if (strncmp(pt, "de 3 fonctions",14)==0)
                       {pt += 14;
                        dataRpu.PEC_Typ[0] = '4';
                       }
                    else if (strncmp(pt, "avec recours",12)==0)
                       {pt += 12;
                        dataRpu.AvisSpe[0] = '1';
                       }
                    pt++;
                   } // endwhile(pt<end    && *pt !='\r')
                if (dataRpu.AvisSpe[0]==0) dataRpu.AvisSpe[0] = '2';
               } // end if (strncmp(pt, "Réa et SUSI",11)==0)
            //...............................**** Recours à Avis ****...................................
            else if (strncmp(pt, TR("Médico-chirurgicale"),19)==0)
               {pt += 19;
                dataRpu.PEC_Typ[0] = '5';
                while(dataRpu.AvisSpe[0]==0 && pt<end    && *pt !='\r') // aller jusqu'en fin de ligne pour voir si complément d'info
                   {if (strncmp(pt, "avec recours",12)==0)
                       {pt += 12;
                        dataRpu.AvisSpe[0] = '1';
                       }
                     pt++;
                    }
                if (dataRpu.AvisSpe[0]==0) dataRpu.AvisSpe[0] = '2';
               }
            else if (strncmp(pt, TR("Médico-légale"),13)==0)
               {pt += 13;
                dataRpu.PEC_Typ[0] = '6';
                while(dataRpu.AvisSpe[0]==0 && pt<end    && *pt !='\r') // aller jusqu'en fin de ligne pour voir si complément d'info
                   {if (strncmp(pt, "avec recours",12)==0)
                       {pt += 12;
                        dataRpu.AvisSpe[0] = '1';
                       }
                     pt++;
                    }
                if (dataRpu.AvisSpe[0]==0) dataRpu.AvisSpe[0] = '2';
               }
            else if (strncmp(pt, "Simple contact",14)==0)
               {pt += 14;
                dataRpu.PEC_Typ[0] = '7';
                while(dataRpu.AvisSpe[0]==0 && pt<end    && *pt !='\r') // aller jusqu'en fin de ligne pour voir si complément d'info
                   {if (strncmp(pt, "avec recours",12)==0)
                       {pt += 12;
                        dataRpu.AvisSpe[0] = '1';
                       }
                     pt++;
                    }
                if (dataRpu.AvisSpe[0]==0) dataRpu.AvisSpe[0] = '2';
               }
            pt++;
           }// endwhile(dataRpu.PEC_Typ[0]==0 && pt<end    && *pt !='\r')
      }
   //...............................**** Issue du passage et Transport ****...................................
   else if (dataRpu.Devenir[0]==0 && strncmp(pt, "Issue du passage",16)==0)
      {pt+=16;
       while(dataRpu.Devenir[0]==0 && pt<end    && *pt !='\r')
           {if (strncmp(pt, TR("Hospitalisé"),11)==0)
               {pt += 11;
                dataRpu.Devenir[0] = '3';
                while(dataRpu.TranspHosp[0]==0 && pt<end    && *pt !='\r') // aller jusqu'en fin de ligne pour voir si complément d'info
                   {if (strncmp(pt, TR("ambulance médi"),14)==0)
                       {pt += 14;
                        dataRpu.TranspHosp[0] = '1';
                       }
                    if (strncmp(pt, "ambulance non",13)==0)
                       {pt += 13;
                        dataRpu.TranspHosp[0] = '2';
                       }
                     pt++;
                    }
                if (dataRpu.TranspHosp[0] == 0) dataRpu.TranspHosp[0] = ' ';
               }
            else if (strncmp(pt, "Retour",6)==0)
               {pt += 6;
                dataRpu.Devenir[0] = '2';
               }
            else if (strncmp(pt, TR("Décès"),5)==0)
               {pt += 5;
                dataRpu.Devenir[0] = '1';
               }
            pt++;
           }
      }
   //...............................**** Facteurs agravant ****...................................
   else if (dataRpu.Agressif[0]==0 && strncmp(pt, "Manifestations d'agitation",26)==0)
      {pt+=26;
       dataRpu.Agressif[0] = '1';
      }
   else if (dataRpu.Communication[0]==0 && strncmp(pt, TR("Difficulté de communication"),27)==0)
      {pt+=27;
       dataRpu.Communication[0] = '1';
      }
   else if (dataRpu.Communication[0]==0 && strncmp(pt, TR("d'intervention sociale"),22)==0)
      {pt+=22;
       dataRpu.SocialEtAdmin[0] = '1';
      }
   else
      {++pt;
      }
  }
}
/*------------------------ CompteCCMU -----------------------------------------*/
int CGestRPU::CompteCCMU(SSignal *pSignalPatient, const char *ptIn, long len, STRUCT_CCMU &CCMU, QDateTime &coDt_deb, const char *medecin)
{char *pt = (char*) ptIn;
 char ccmu_find = 0;
 char ccmu_str[64];
 char* pt_ccmu = ccmu_str;
 if (len==-1) len = strlen(pt);
 char *end = pt + len;
 ccmu_str[0]=0;
 /*.......... scanner et extraire valeur du ccmu ..................*/
 while (pt<end)
   {if (ccmu_find == 0 && strncmp(pt, "C.C.M.U", 7)==0)
        {ccmu_find = 1;
         pt += 7;
        }
    if (ccmu_find == 1 && strncmp(pt, "Niveau", 6)==0)
       {pt+=6;
        while(pt<end && (*pt==' '||*pt=='\t'))pt++;  // sauter espaces et tabulations
        while(pt<end && (*pt=='I'||*pt=='V'))        // copier les caractères CCMU
          {*pt_ccmu = *pt;
           pt++;
           pt_ccmu++;
          }
        *pt_ccmu=0;  // fermer la chaine et
        break;       // cassos
       }
    ++pt;
   }
 /*............... mettre à jour la structure ..........*/
 if (strcmp(ccmu_str,"I")==0)
    {CCMU.ccmu1++;
    }
 else if (strcmp(ccmu_str,"II")==0)
    {CCMU.ccmu2++;
    }
 else if (strcmp(ccmu_str,"III")==0)
    {CCMU.ccmu3++;
    }
 else if (strcmp(ccmu_str,"IV")==0)
    {CCMU.ccmu4++;
    }
 else if (strcmp(ccmu_str,"V")==0)
    {CCMU.ccmu5++;
    }
 else
    {CCMU.ccmu0++;
     InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), TR("CCMU non indiqué"), medecin);
     return 1;
    }

 return 0;
}

/*------------------------ CompteGEMSA -----------------------------------------*/
/*typedef struct
 {long hospi;
  long trans;
  long exter;
  long indeter;
  long gemsa_1;
  long gemsa_2;
  long gemsa_3;
  long gemsa_4;
  long gemsa_5;
  long gemsa_6;
  long gemsa_7;
 }STRUCT_DEST;
*/
int CGestRPU::CompteGEMSA(SSignal *pSignalPatient, const char *ptIn, long len, STRUCT_GEMSA &GEMSA , QDateTime &coDt_deb, const char *medecin)
{char *pt = (char*) ptIn;
 if (len==-1) len = strlen(pt);
 char *end = pt + len;
 /*.......... scanner et extraire valeur du ccmu ..................*/
 while (pt<end)
   {if (pt[0] == '.'  && pt[1] == 'E' &&
        pt[2] == '.'  && pt[3] == 'M' &&
        pt[4] == '.'  && pt[5] == 'S' &&
        pt[6] == '.'  && pt[7] == 'A'
       )
       {while(pt<end)
            {if (*pt=='\r'||*pt=='\n') break;
             if (*pt>='1' && *pt<='6')
                {switch(pt[0])
                   {case '1': GEMSA.gemsa_1++; return 0;
                    case '2': GEMSA.gemsa_2++; return 0;
                    case '3': GEMSA.gemsa_3++; return 0;
                    case '4': GEMSA.gemsa_4++; return 0;
                    case '5': GEMSA.gemsa_5++; return 0;
                    case '6': GEMSA.gemsa_6++; return 0;
                   }
                }
             ++pt;
            }
        GEMSA.gemsa_I++;
       } // end if (pt[0] == '.'  &
    ++pt;
   } //while (*pt)
 InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), TR("GEMSA non indiqué"), medecin);
 return 0;
}

/*------------------------ CompteDIVERS -----------------------------------------*/
/*typedef struct
 {long hospi;
  long trans;
  long exter;
  long indeter;
  long gemsa_1;
  long gemsa_2;
  long gemsa_3;
  long gemsa_4;
  long gemsa_5;
  long gemsa_6;
  long gemsa_7;
 }STRUCT_DEST;
*/
//	Prise en charge     : ##  ##
//	Issue du passage : ## Décès  ##
//	Destination:

int CGestRPU::CompteDIVERS(SSignal *pSignalPatient, const char *ptIn, long len, STRUCT_DIVERS &DIVERS , QDateTime &coDt_deb, const char *medecin)
{char *pt = (char*) ptIn;
 if (len==-1) len = strlen(pt);
 char *end = pt + len;
 /*.......... scanner et extraire valeur du ccmu ..................*/
 while (pt<end)
   {if (pt[0] == 'I'  && pt[1] == 's' &&
        pt[2] == 's'  && pt[3] == 'u' &&
        pt[4] == 'e'  && pt[5] == ' ' &&
        pt[6] == 'd'  && pt[7] == 'u'
       )
       {while(pt<end)
            {if (*pt=='\r'||*pt=='\n') break;
             if      (strncmp (pt,TR("Décès"),5)==0)               {DIVERS.Deces++; return 0;} // Retour à domicile     Hospitalisé
             else if (strncmp (pt,TR("Retour à domicile"),17)==0)  {DIVERS.RetDom++;return 0;}
             else if (strncmp (pt,TR("Hospitalisé"),11)==0)        {DIVERS.Hosp++;  return 0;}
             ++pt;
            }
        DIVERS.Ind++;
       } // end if (pt[0] == '.'  &
    ++pt;
   } //while (*pt)
 InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), TR("Issue du Passage non indiqué"), medecin);
 return 0;
}

/*-------------------------------------- _strnicmp -----------------------------------------*/
int CGestRPU::_strnicmp(const QString &t1, const QString &t2, int n)
{return strncmp(t1.lower(), t2.lower(), n);
}

/*-------------------------------------- CompteDEST -----------------------------------------*/
/*typedef struct
 {long hospi;
  long trans;
  long exter;
  long indeter;
  long gemsa_1;
  long gemsa_2;
  long gemsa_3;
  long gemsa_4;
  long gemsa_5;
  long gemsa_6;
  long gemsa_7;
 }STRUCT_DEST;
*/
int CGestRPU::CompteDEST(SSignal *pSignalPatient, const char *ptIn, long len, STRUCT_DEST &DEST, QDateTime &coDt_deb, const char *medecin)
{char *pt = (char*) ptIn;
 char dest_find = 0;
 int hospi = 0;
 if (len==-1) len = strlen(pt);
 char *end = pt + len;
 /*.......... scanner et extraire valeur du ccmu ..................*/
 while (pt<end)
   {if (dest_find == 0 && strncmp(pt, "Issue du passage :", 18)==0)
       {dest_find = 1;
        pt += 17;
       }
    if (dest_find != 2 && strncmp(pt, "Destination:", 12)==0)
       {dest_find = 2;
        pt += 12;
       }
    if (dest_find == 1 && _strnicmp(pt, "ospita", 6)==0) // on passe hospi RPU et on le note
       {hospi++;                                         // au cas ou oubli ensuite
        pt += 6;
       }
    if (dest_find == 2 && _strnicmp(pt, "ospita", 6)==0)
       {DEST.hospi++;                                        return 1;
       }
    if (dest_find == 2 && _strnicmp(pt, "vasion", 6)==0)
       {DEST.exter++;                                        return 2;
       }
    if (dest_find == 1 && _strnicmp(pt, "Retour à do", 11)==0)
       {DEST.exter++;                                        return 2;
       }
    if (dest_find == 2 && _strnicmp(pt, "xtern", 5)==0)
       {DEST.exter++;                                        return 2;
       }
    if (dest_find == 2 && _strnicmp(pt, "ansfert", 7)==0)
       {DEST.trans++;                                        return 3;
       }
    if (dest_find == 2 && _strnicmp(pt, "nord", 4)==0)
       {DEST.trans++;                                        return 3;
       }
    if (dest_find == 2 && _strnicmp(pt, "vitrol", 6)==0)
       {DEST.trans++;                                        return 3;
       }
    if (dest_find == 2 && _strnicmp(pt, "arigna", 6)==0)
       {DEST.trans++;                                        return 3;
       }
    if (dest_find == 2 && _strnicmp(pt, "timone", 6)==0)
       {DEST.trans++;                                        return 3;
       }
    if (dest_find == 2 && _strnicmp(pt, "martig", 6)==0)
       {DEST.trans++;                                        return 3;
       }
    if (dest_find == 2 && _strnicmp(pt, "concep", 6)==0)
       {DEST.trans++;                                        return 3;
       }
    if (dest_find == 2 && (*pt>='1'&& *pt<='5') )
       {DEST.hospi++;                                        return 1;
       }
    if (strncmp(pt, "Date SU:", 8)==0      ||
        strncmp(pt, "Cotation:", 9)==0     ||
        strncmp(pt, "fin de soins:", 13)==0
       )
       {if (hospi==0)
           {DEST.indeter++; InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), TR("DESTINATION non indiquée"), medecin);
            return 0;
           }
        else
           {DEST.hospi++;
            return 1;
           }
       }
    pt++;
   }
 DEST.indeter++;
 InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), TR("DESTINATION non indiquée"), medecin);
 return 0;
}

/*--------------------------------- RPU_Make-----------------------------------------------*/
void CGestRPU::RPU_Make(SSignal *pSignalPatient, const char *observ, long len_observ, QDateTime &coDt_deb, QDateTime &coDt_end, int dst, const char* medecin)
 {int err = 0;
  QString  mess("");
  DATA_RPU dataRpu;
  memset(&dataRpu, 0, sizeof(dataRpu));

  //.......................................... N° Finess ..........................
  strcpy(dataRpu.Num_Finess, m_ED_Finess);
  //.......................................... Type d'autorisation ................
  //     SAU POSU UPATOU
  dataRpu.TypeAutorisation[0] = m_Qualif;
  //.......................................... numero de dossier ..................
  mess  = QString::number(m_NbFicWrited);    // m_NbFicWrited = dossier ecris dans RPU
  mess += "            ";
  mess  = mess.left(12);
  strcpy(dataRpu.NumEnr, mess);
  //.......................................... sexe ..............................
  switch(pSignalPatient->m_sexe[0])
        {case 'm':
         case 'h':
         case 'M':
         case 'H': dataRpu.sexe[0] = '1';     break;
         default:  dataRpu.sexe[0] = '2';
        }
  //.............................. date naissance ......................................
  //                           validité date de naissance
  err = 0;
  if ( ! pSignalPatient->m_Naissance.isValid() )
     {mess  = "\r\n=========== ";
      mess  = mess + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
      mess  = mess +  TR("\r\n    Date de naissance invalide ou absente");
      //LogWrite(mess, gsCheminPromethe, "ErreurStat.log");
      WriteErreur(mess);
      err = 'N';         // mais ne peut etre retenu pour RPU
      InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), TR("Date de naissance invalide ou absente"), medecin);
     }
   else
     {//strcpy(dataRpu.DtNaiss,
      //       ValJJMMAAAA_To_Str((long)pSignalPatient->m_Naissance.day(),
      //                          (long)pSignalPatient->m_Naissance.month(),
      //                          (long)pSignalPatient->m_Naissance.year())
      //                         );
      strcpy(dataRpu.DtNaiss, pSignalPatient->m_Naissance.toString("ddMMyyyy"));
     }
   if (strlen(dataRpu.DtNaiss) != 8) err = 'N';         // date naissance invalide

   //.......................................... code postal ......................................
   if (pSignalPatient->m_codePostal.length()<=0)
      {mess  = "\r\n=========== ";
       mess  = mess + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
       mess  = mess +  TR("\r\n    Pas de code Postal: il sera placé: 99999");
       strcpy(dataRpu.CodePostal,"99999");
       //LogWrite(mess, gsCheminPromethe, "ErreurStat.log");
       WriteErreur(mess);
      }
   else if (pSignalPatient->m_codePostal.length() !=5)
      {mess  = "\r\n=========== ";
       mess  = mess + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
       mess  = mess +  TR("\r\n    Code postal invalide: il sera placé: 99999 à la place de : ") + pSignalPatient->m_codePostal;
       strcpy(dataRpu.CodePostal,"99999");
       //LogWrite(mess, gsCheminPromethe, "ErreurStat.log");
       WriteErreur(mess);
      }
   else
      {strcpy(dataRpu.CodePostal, pSignalPatient->m_codePostal);
      }
    //............................................ Date d'arrivée .................................
    //strcpy(dataRpu.DtArrivee, ValJJMMAAAA_To_Str(D_deb, M_deb, Y_deb));
    strcpy(dataRpu.DtArrivee, coDt_deb.toString("ddMMyyyy"));
    //............................................ Heure d'arrivée .................................
    //strcpy(dataRpu.HeureArrivee, ValHHmm_To_Str(h_deb, m_deb));
    strcpy(dataRpu.HeureArrivee, coDt_deb.toString("hhmm"));
    //............................................ Date d'issue .................................
    // strcpy(dataRpu.DtIssue, ValJJMMAAAA_To_Str(D_end, M_end, Y_end));
    strcpy(dataRpu.DtIssue, coDt_end.toString("ddMMyyyy"));
    //............................................ Heure d'issue .................................
    //strcpy(dataRpu.HeureIssue, ValHHmm_To_Str(h_end, m_end));
    strcpy(dataRpu.HeureIssue, coDt_end.toString("hhmm"));
    //.............................................Parsing du texte.....................
    RPU_ParseObservation(observ, len_observ, dataRpu);
    //.............................................Verifier données.....................
    if (err==0) err = RPU_Verifie(dataRpu, pSignalPatient);
    //.............................................Ecrire données RPU.....................
    if (err==0)
       {err = RPU_Write(dataRpu, pSignalPatient, coDt_deb, dst, medecin);
        if (err)
           { qDebug(QString::number(err));
	    switch(err)
                  {case 1:
                   case 3: mess = TR("Erreur dans checksum des données");                                             break;
                   case 2: mess = TR("Erreur dans les données Actes classants");                                      break;
                   case 4: mess = TR("Date de début supérieure à Date d'issue");                                      break;
                   case 5: mess = TR("Heure de début supérieure à Heure d'issue");                                    break;
                   case 6: mess = TR("Date ou heure de début invalide (plus de jours que possible dans un mois)");    break;
                   case 7: mess = TR("Date ou heure de fin invalide (plus de jours que possible dans un mois)");      break;
                  }
            InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), mess, medecin);
           }
       }
    else
       {switch(err)
              {case 'C': mess = TR("CIM10 non saisi");                                      break;
               case 'P': mess = TR("Prise en charge non saisie");                           break;
               case 'I': mess = TR("Issue du passage non saisie");                          break;
               case 'D': mess = TR("Décédé non saisi");                                     break;
               case 'H': mess = TR("Hospitalisé et Mode de transport non précisé");         break;
               case 'A': mess = TR("Arrivée Médicalisée et Mode de transport non précisé"); break;
               case 'N': mess = TR("Date de naissance invalide");                           break;
              }
        InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), mess, medecin);
       }
 }

/*--------------------------------- InsertError -------------------------------------------*/
int CGestRPU::InsertError(SSignal *pSignalPatient, const char* date, const char* mess, const char* medecin /* = ""  */)
{m_ErrRpuList.append (new ERR_RPU(pSignalPatient->m_Nom,
				  pSignalPatient->m_Prenom,
				  date,
				  medecin,
				  mess,
				  pSignalPatient->m_GUID
				 )
		      );
 m_NbErr++;
 return  1;
}

/*--------------------------------- RPU_Verifie --------------------------------------------*/
/*
typedef struct
 {char CodeAct[5];
  char NbAct[3];
 }ACTE_RPU;

typedef struct
 {char Num_Finess[10];
  char TypeAutorisation[2];
  char NumEnr[13];
  char sexe[2];
  char DtNaiss[9];
  char CodePostal[6];
  //....................
  char DtArrivee[9];
  char HeureArrivee[5];
  char TranspMedic[2];
  char TranspTyp[2];
  char Decede[2];
  char PEC_Typ[2];
  char AvisSpe[2];
  char CIM10[7];
  char Agressif[2];
  char Communication[2];
  char SocialEtAdmin[2];
  ACTE_RPU ActeRpu[10];
  char DtIssue[9];
  char HeureIssue[5];
  char Devenir[2];
  char TranspHosp[2];
 }DATA_RPU;
*/
int CGestRPU::RPU_Verifie(DATA_RPU &dataRpu, SSignal *pSignalPatient)
{QString mess  = "";
 if (dataRpu.CIM10[0]==0)            mess  = mess + TR("\r\n    CIM10 non saisi");
 if (dataRpu.PEC_Typ[0]==0)          mess  = mess + TR("\r\n    Prise en charge non saisie");
 if (dataRpu.Devenir[0]==0)          mess  = mess + TR("\r\n    Issue du passage non saisie");
 if (dataRpu.Decede[0]==0)           mess  = mess + TR("\r\n    Décédé non saisi");
 if (dataRpu.Devenir[0]=='3' &&
     dataRpu.TranspHosp[0]==0)       mess  = mess + TR("\r\n    Hospitalisé et Mode de transport non précisé");
 if (dataRpu.TranspMedic[0]=='2' &&
     dataRpu.TranspTyp[0]==0)        mess  = mess + TR("\r\n    Arrivée Médicalisée et Mode de transport non précisé");
 //........... par defaut ................................
 if (dataRpu.TranspMedic[0]==0)  dataRpu.TranspMedic[0]=' ';
 if (dataRpu.TranspTyp[0]==0)    dataRpu.TranspTyp[0]=' ';
 if (dataRpu.TranspHosp[0]==0)   dataRpu.TranspHosp[0]=' ';

 if (mess !="")
    {QString head = "\r\n=========== ";
             head = head + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
     WriteErreur(head);
     WriteErreur(mess);
     char c = mess.at(6).latin1();
     return c;  // sortir le premier caractère significatif du message
    }

 return 0;
}


//--------------------------------- DebugDATA_RPU -----------------------------------------------------------
/*
typedef struct
 {char Num_Finess[10];          // 9
  char TypeAutorisation[2];     // 1
  char NumEnr[13];              // 12
  char sexe[2];                 // 1
  char DtNaiss[9];              // 8
  char CodePostal[6];           // 5
  //....................
  char DtArrivee[9];            // 8
  char HeureArrivee[5];         // 4
  char TranspMedic[2];          // 1
  char TranspTyp[2];            // 1
  char Decede[2];               // 1
  char PEC_Typ[2];              // 1
  char AvisSpe[2];              // 1
  char CIM10[7];                // 6
  char Agressif[2];             // 1
  char Communication[2];        // 1
  char SocialEtAdmin[2];        // 1
  ACTE_RPU ActeRpu[10];         // 9
  char DtIssue[9];              // 8
  char HeureIssue[5];           // 4
  char Devenir[2];              // 1
  char TranspHosp[2];           // 1
  char CIM10_Str[256];          // 255
 }DATA_RPU;
 */
int  CGestRPU::DebugDATA_RPU(DATA_RPU &dataRpu, SSignal *pSignalPatient, QDateTime &coDt_deb, const char* medecin)
{QString mess("");
 long len=0;
 len = strlen(dataRpu.Num_Finess);       if (len != 9)  mess += " N° Fi:" + QString::number(len);
 len = strlen(dataRpu.TypeAutorisation); if (len != 1)  mess += " Autor:" + QString::number(len);
 len = strlen(dataRpu.NumEnr);           if (len != 12) mess += " N°Enr:" + QString::number(len);
 len = strlen(dataRpu.sexe);             if (len != 1)  mess += " Sexe:"  + QString::number(len);
 len = strlen(dataRpu.DtNaiss);          if (len != 8)  mess += " DtNss:" + QString::number(len);
 len = strlen(dataRpu.CodePostal);       if (len != 5)  mess += " CPost:" + QString::number(len);
 len = strlen(dataRpu.DtArrivee);        if (len != 8)  mess += " DtArr:" + QString::number(len);
 len = strlen(dataRpu.HeureArrivee);     if (len != 4)  mess += " HrArr:" + QString::number(len);
 len = strlen(dataRpu.TranspMedic);      if (len != 1)  mess += " TrMed:" + QString::number(len);
 len = strlen(dataRpu.TranspTyp);        if (len != 1)  mess += " TrTyp:" + QString::number(len);
 len = strlen(dataRpu.Decede);           if (len != 1)  mess += " Décès:" + QString::number(len);
 len = strlen(dataRpu.PEC_Typ);          if (len != 1)  mess += " PECTp:" + QString::number(len);
 len = strlen(dataRpu.AvisSpe);          if (len != 1)  mess += " AvisS:" + QString::number(len);
 len = strlen(dataRpu.CIM10);            if (len != 6)  mess += " CIM10:" + QString::number(len);
 len = strlen(dataRpu.Agressif);         if (len != 1)  mess += " Agres:" + QString::number(len);
 len = strlen(dataRpu.Communication);    if (len != 1)  mess += " Comun:" + QString::number(len);
 if (mess.length())
    {mess.prepend("RPU ERR: ");
     InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), mess, medecin);
     return 1;
    }
 return 0;
}

/*--------------------------------- RPU_Write -----------------------------------------------------------*/
int CGestRPU::RPU_Write(DATA_RPU &dataRpu, SSignal *pSignalPatient, QDateTime &coDt_deb, int dst, const char* medecin)
{QString mess="";
 mess += QString(dataRpu.Num_Finess) + QString(dataRpu.TypeAutorisation) + QString(dataRpu.NumEnr) + QString(dataRpu.sexe) + QString(dataRpu.DtNaiss);
 mess += QString(dataRpu.CodePostal) + QString(dataRpu.DtArrivee) + QString(dataRpu.HeureArrivee) + QString(dataRpu.TranspMedic) + QString(dataRpu.TranspTyp) + QString(dataRpu.Decede);
 mess += QString(dataRpu.PEC_Typ)    + QString(dataRpu.AvisSpe) +  QString(dataRpu.CIM10) + QString(dataRpu.Agressif) + QString(dataRpu.Communication);
 mess += QString(dataRpu.SocialEtAdmin);
 long len = mess.length();

 if (len != 62)
    {QString head = "\r\n=========== ";
             head = head + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
             head = head + TR("\r\n    Données non complètes sur ce dossier");
     WriteErreur(head);
     DebugDATA_RPU(dataRpu, pSignalPatient, coDt_deb,  medecin);
     return 1;
    }
 //................ ecriture des actes .......................................
 long acteID = 0;
 for (acteID = 0; acteID<10; acteID++)
     {char *pt = dataRpu.ActeRpu[acteID].CodeAct;
      if (pt[0] != 0)
         {mess += QString(pt);
          mess += QString(dataRpu.ActeRpu[acteID].NbAct);
         }
      else
         {mess += "      ";
         }
     }
 len = mess.length();
 if (len != 122)
    {QString head = "\r\n=========== ";
             head = head + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
             head = head + TR("\r\n    Erreurs sur la liste des actes classants");
     WriteErreur(head);
     return 2;
    }
 mess +=  QString(dataRpu.DtIssue) + QString(dataRpu.HeureIssue) + QString(dataRpu.Devenir) + QString(dataRpu.TranspHosp) ;
 len = mess.length();
 if (len != 136)
    {QString head = "\r\n=========== ";
             head = head + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
             head = head + TR("\r\n    Données non complètes sur ce dossier");
     WriteErreur(head);
     return 3;
    }

{QString slog = "";
 slog += QString("\r\n DtA: ") + QString(dataRpu.DtArrivee) + QString(" HA ") + QString(dataRpu.HeureArrivee) + QString(" DtI: ")+ QString(dataRpu.DtIssue) + QString(" HI ")+ QString(dataRpu.HeureIssue) + QString("  ")+QString(pSignalPatient->m_Nom) + QString(" ") + QString(pSignalPatient->m_Prenom)+QString("                                                   ");
 slog = slog.left(80);

 //...............jouons un peu avec les dates ...........................................
 long dt_deb = JJMMAAAA_To_Val(&dataRpu.DtArrivee[0]);
 long dt_fin = JJMMAAAA_To_Val(&dataRpu.DtIssue[0]);

 if (dt_fin < dt_deb)
    {QString head = "\r\n=========== ";
             head = head + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
             head = head + TR("\r\n    Date de début supérieure à Date d'issue");
     WriteErreur(head);
     return 4;
    }
 else if (dt_deb == dt_fin) //................... MEME DATE ...............................
    {if (atol(dataRpu.HeureArrivee)>atol(dataRpu.HeureIssue))
        {QString head = "\r\n=========== ";
                 head = head + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
                 head = head + TR("\r\n    Heure de début supérieure à Heure d'issue");
         WriteErreur(head);
         return 5;
        }
     else
        {long nb_sec_sejour = HeureMinuteSeconde_To_Val(dataRpu.HeureIssue) - HeureMinuteSeconde_To_Val(dataRpu.HeureArrivee);
         long h = nb_sec_sejour/3600;
         long m = (nb_sec_sejour - h*3600) / 60;
         slog   = slog + TR(" Durée du séjour de: ");
         if (h) slog = slog + QString::number(h) +"h";
         slog   = slog + QString::number(m)+"m                              ";
         slog   = slog.left(110);
         if (h>=6) slog = slog + TR(" >= 6 heures");
	 if (h==0 && m<=5) InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), TR(WARNING_HEAD" durée séjour <= 5mn"), medecin);
        }
    }
 else                   //................... DATES DIFFERENTES ...............................
    {long j,  h,  m;
     long ret = CalculeDureeSejour(dataRpu,  &j,  &h,  &m);
     if (ret == 1)
        {QString head = "\r\n=========== ";
                 head = head + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
                 head = head + TR("\r\n    Date ou heure de début invalide (plus de jours que possible dans un mois)");
         WriteErreur(head);
         return 6;
        }
     if (ret == 2)
        {QString head = "\r\n=========== ";
                 head = head + pSignalPatient->m_Nom + " " + pSignalPatient->m_Prenom + " =============";
                 head = head + TR("\r\n    Date ou heure de début invalide (plus de jours que possible dans un mois)");
         WriteErreur(head);
         return 7;
        }
     if (j>=0)
        {slog = slog + TR(" Durée du séjour de: ");
         if (j) slog = slog + QString::number(j) + "J";
         if (h) slog = slog + QString::number(h) + "h";
         slog        = slog + QString::number(m) + "m                              ";
         slog = slog.left(110);
         if      (j>0) {slog = slog + TR(" >= 24 heures");
                        InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), TR(WARNING_HEAD" durée séjour >= 24h"), medecin);
                       }
         else if (h>=6)
	         {slog = slog + " >=  6 heures";
	          if (h==0 && m<=5) InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), WARNING_HEAD" durée séjour <= 5mn", medecin);
		 }
        }
     else
        {slog        = slog + TR(" Durée du séjour de: ");
         if (h) slog = slog + QString::number(h) + "h";
         slog        = slog + QString::number(m) + "m                              ";
         slog        = slog.left(110);
         if (h>=6) slog = slog + TR(" >=  6 heures");
	 if (h==0 && m<=5) InsertError(pSignalPatient, coDt_deb.toString("dd-MM-yyyy"), TR(WARNING_HEAD" durée séjour <= 5mn"), medecin);
        }
    }
 if (dataRpu.Devenir[0]=='3')
    {slog = slog + "                      ";
     slog = slog.left(130);
     if (dst!=3) slog = slog + TR("Hospitalisé");
     else        slog = slog + TR("Transfert");

    }
 else
    {slog = slog + "                      ";
     slog = slog.left(130);
    }

 slog  +="                     ";
 //.............. Ajouter Age au fichier: Test_FILE.log..........................................................
 slog   = slog.left(143);
 slog  += " Age: " + CMoteurBase::CalculeDiffAge(&pSignalPatient->m_Naissance);
 slog  += "                                          ";

 slog   = slog.left(158);
 slog  += "|";
 slog  += dataRpu.CIM10_Str;

 WriteLogFile(slog);
 WriteRpuFile(mess);
 m_NbFicWrited++;
}
 return 0;
}

/*----------------------------------- WriteErreur ----------------------------------------------------------------*/
int CGestRPU::WriteErreur(const char *mess)
{if (m_csErrPath == "") return 0;
 LogWrite(mess, m_csErrPath);
 return 1;
}

/*----------------------------------- WriteLogFile ----------------------------------------------------------------*/
int CGestRPU::WriteLogFile(const char *mess)
{if (m_csLogPath == "") return 0;
 LogWrite(mess, m_csLogPath);
 return 1;
}

/*----------------------------------- WriteRpuFile ----------------------------------------------------------------*/
int CGestRPU::WriteRpuFile(const char *mess)
{if (m_csLogPath == "") return 0;
 LogWrite(mess, m_csRpuPath);
 return 1;
}

/*----------------------------------- CalculeDureeSejour ---------------------------------------------------------*/
long CGestRPU::CalculeDureeSejour(DATA_RPU &dataRpu, long *jour, long *heure, long *minutes, long *secondes)
{long nYearD,  nMonthD,  nDayD,  nHourD,  nMinD,  nSecD;
 long nYearE,  nMonthE,  nDayE,  nHourE,  nMinE,  nSecE;

 long nbSDuree  = 0;
 JJMMAAAA_To_Val(dataRpu.DtArrivee, &nDayD, &nMonthD, &nYearD);
 JJMMAAAA_To_Val(dataRpu.DtIssue,   &nDayE, &nMonthE, &nYearE);
 HeureMinuteSeconde_To_Val(dataRpu.HeureArrivee, &nHourD, &nMinD, &nSecD);
 HeureMinuteSeconde_To_Val(dataRpu.HeureIssue,   &nHourE, &nMinE, &nSecE);

 QDateTime codt_deb(  QDate(nYearD,  nMonthD,  nDayD),  QTime(nHourD,  nMinD,  nSecD) );
 QDateTime codt_end(  QDate(nYearE,  nMonthE,  nDayE),  QTime(nHourE,  nMinE,  nSecE) );

 if ( !codt_deb.isValid() )   return 1;
 if ( !codt_end.isValid() )   return 2;

 nbSDuree = codt_deb.secsTo(codt_end) ; //DifferenceSeconde(codt_deb, codt_end);
 NbSec_To_NbJHMS(nbSDuree, jour, heure, minutes, secondes);
 return nbSDuree;
}

/*----------------------------------- IsDateInfDebLimit -------------------------------------------------------------*/
BOOL CGestRPU::IsDateInfDebLimit(long D,long M,long Y,long h,long m, long /* s*/)
{if (m_TestFlag&NO_TEST_DATE) return FALSE;        // date debut toujours OK

 long Y_ref = m_date_Debut.year();
 long M_ref = m_date_Debut.month();
 long D_ref = m_date_Debut.day();
 long h_ref = m_Hdeb; //m_DateDebut.GetHour();
 long m_ref = m_Mdeb; //m_DateDebut.GetMinute();

 if (Y>Y_ref) return FALSE;
 if (Y<Y_ref) return TRUE;
 //........... à ce stade meme année ..............

 if (M>M_ref) return FALSE;
 if (M<M_ref) return TRUE;
 //........... à ce stade meme mois ..............

 if (D>D_ref) return FALSE;
 if (D<D_ref) return TRUE;
 //........... à ce stade meme jour ..............

 if (h>h_ref) return FALSE;
 if (h<h_ref) return TRUE;
 //........... à ce stade meme heure ..............

 if (m>m_ref) return FALSE;
 if (m<m_ref) return TRUE;
 //........... à ce stade meme minute ..............
 return FALSE;
}

/*----------------------------------- IsDateSupEndLimit --------------------*/
BOOL CGestRPU::IsDateSupEndLimit(long D,long M,long Y,long h,long m,long /* s*/ )
{if (m_TestFlag&NO_TEST_DATE) return FALSE;        // date fin toujours OK
 long Y_ref = m_date_Fin.year();
 long M_ref = m_date_Fin.month();
 long D_ref = m_date_Fin.day();
 long h_ref = m_Hfin; //m_DateFin.GetHour();
 long m_ref = m_Mfin; //m_DateFin.GetMinute();

 if (Y<Y_ref) return FALSE;
 if (Y>Y_ref) return TRUE;
 //........... à ce stade meme année ..............

 if (M<M_ref) return FALSE;
 if (M>M_ref) return TRUE;
 //........... à ce stade meme mois ..............

 if (D<D_ref) return FALSE;
 if (D>D_ref) return TRUE;
 //........... à ce stade meme jour ..............

 if (h<h_ref) return FALSE;
 if (h>h_ref) return TRUE;
 //........... à ce stade meme heure ..............

 if (m<m_ref) return FALSE;
 if (m>m_ref) return TRUE;
 //........... à ce stade meme minute ..............
 return FALSE;
}

/*----------------------- GetObservDateDeb ------------------------------*/
int CGestRPU::GetObservDateDeb(const char *observ, long len, QDateTime &coDt_deb, long &D,long &M,long &Y,long &h,long &m,long &s)
{char *pt   = (char*)observ;
 char *end  = pt + len;
 int   err  = 0;
 int  find  = 0;
 QString token_Date = TR("Date:");
 QString token_Heure = TR("vée SU:");

 while (find != 3 && pt<end)
  {if (strncmp(pt, token_Date, token_Date.length())==0)
      {pt += 5;
       D=M=Y=0;
       find |= 1; // la mention date a ete trouvée
       if (ExtraitChiffre(&pt, &D, end)==FALSE)
          {err |= 1;
          }
       if (ExtraitChiffre(&pt, &M, end)==FALSE)
          {err |= 2;
          }
       if (ExtraitChiffre(&pt, &Y, end)==FALSE)
          {err |= 4;
          }
      }
   if (strncmp(pt, token_Heure,token_Heure.length())==0)
      {pt += 7;
       h=m=s=0;
       find |= 2; // la mention heure a ete trouvée
       if (ExtraitChiffre(&pt, &h, end)==FALSE)
          {err |= 8;
          }
       if (ExtraitChiffre(&pt, &m, end)==FALSE)
          {//err |= 16;   // si pas de minute on considère que l'heure seule a été saisie
          }
       else
          {if ((find&1) == 0) // la mention date n'a pas ete trouvée
              {err |= 32;
              }
           //coDt_deb.SetDateTime((int)Y,(int)M,(int)D,(int)h,(int)m,(int)s);
	   coDt_deb.setDate(QDate((int)Y,(int)M,(int)D));
           coDt_deb.setTime(QTime((int)h,(int)m,(int)s));
           if ( !coDt_deb.isValid() ) err |= 128;
           return err;
          }
      }
   pt++;
  }
 if ((find&1) == 0) // la mention date n'a pas ete trouvée
    {err |= 32;
    }
 if ((find&2) == 0) // la mention heure n'a pas ete trouvée
    {err |= 64;
    }
 //coDt_deb.SetDateTime((int)Y,(int)M,(int)D,(int)h,(int)m,(int)s);
 coDt_deb.setDate(QDate((int)Y,(int)M,(int)D));
 coDt_deb.setTime(QTime((int)h,(int)m,(int)s));
 if ( !coDt_deb.isValid() ) err |= 128;
 return err;
}

/*----------------------- GetObservDateEnd ------------------------------*/
// 1 = jour
// 2 = mois
// 4 = année
int CGestRPU::GetObservDateEnd(const char *observ, long len, QDateTime &coDt_end, long &D,long &M,long &Y,long &h,long &m,long &s)
{char *pt   = (char*) observ;
 char *end  = pt + len;
 int   err  = 0;
 int  find  = 0;
 while (find != 3 && pt<end)
  {if (strncmp(pt, "Date SU:",8)==0)
      {pt+=8;
       D=M=Y=0;
       find |= 1; // la mention date a ete trouvée
       if (ExtraitChiffre(&pt, &D, end)==FALSE)
          {err |= 1;
          }
       if (ExtraitChiffre(&pt, &M, end)==FALSE)
          {err |= 2;
          }
       if (ExtraitChiffre(&pt, &Y, end)==FALSE)
          {err |= 4;
          }
      }
   if (strncmp(pt, "art SU:",7)==0)
      {pt += 7;
       h=m=s=0;
       find |= 2; // la mention heure a ete trouvée
       if (ExtraitChiffre(&pt, &h, end)==FALSE)
          {err |= 8;
          }
       if (ExtraitChiffre(&pt, &m, end)==FALSE)
          {//err |= 16;   // si pas de minute on considère que l'heure seule a été saisie
          }
       else
          {if ((find&1) == 0) // la mention date n'a pas ete trouvée
              {err |= 32;
              }
           //coDt_end.SetDateTime((int)Y,(int)M,(int)D,(int)h,(int)m,(int)s);
	   coDt_end.setDate(QDate((int)Y,(int)M,(int)D));
           coDt_end.setTime(QTime((int)h,(int)m,(int)s));
           if ( !coDt_end.isValid() ) err |= 128;
           return err;
          }
      }
   pt++;
  }
 if ((find&1) == 0) // la mention date n'a pas ete trouvée
    {err |= 32;
    }
 if ((find&2) == 0) // la mention heure n'a pas ete trouvée
    {err |= 64;
    }
 //coDt_end.SetDateTime((int)Y,(int)M,(int)D,(int)h,(int)m,(int)s);
 coDt_end.setDate(QDate((int)Y,(int)M,(int)D));
 coDt_end.setTime(QTime((int)h,(int)m,(int)s));
 if ( !coDt_end.isValid() ) err |= 128;
 return err;
}

/*------------------------------ PathConcat ----------------------------*/
QString *CGestRPU::PathConcat(QString &path, const char*  name)
{if (path=="")
    {path = name;
     return &path;
    }

 if (name)
    {if (name[0]==QDir::separator()) name++;
     if (path.at(strlen(path)-1)!=QDir::separator()) path +=QDir::separator();
     path += name;
    }
 return &path;
}

/*------------------------------ PathConcat ----------------------------*/
QString *CGestRPU::PathConcat(const char* path, const char*  name, QString* dst)
{QString    path_f  = "";
 if (path==0)
    {return dst;
    }
 path_f  = path;
 if (name)
    {if (path[strlen(path)-1]!=QDir::separator()&&name[0]!=QDir::separator()) path_f +=QDir::separator();
     path_f += name;
    }
 *dst = path_f;
 return dst;
}

/*------------------------------ PathConcat ----------------------------*/
char *CGestRPU::PathConcat(const char* path, const char*  name, char* dst)
{QString    path_f  = path;
 if (name)
    {if (path[strlen(path)-1]!=QDir::separator()&&name[0]!=QDir::separator()) path_f +=QDir::separator();
     path_f += name;
    }
 strcpy(dst, path_f);
 return dst;
}


char *CGestRPU::PathConcat(char* path, const char*  name)
{QString    path_f  = path;
 if (name)
    {if (path[strlen(path)-1]!=QDir::separator() &&name[0]!= QDir::separator()) path_f +=QDir::separator();
     path_f += name;
    }
 strcpy(path, path_f);
 return path;
}

/*------------------------------ ValJJMMAAAA_To_Str ----------------------------*/
QString CGestRPU::ValJJMMAAAA_To_Str(long d, long m, long y, char *sep)
{QString  csBuf("");
 QString  csDate ("");
 csBuf  = "00";
 csBuf  = csBuf  + QString::number(d);
 csDate = csDate + csBuf.right(2); if (sep) csDate += QString(sep); // "05-"

 csBuf  = "00";
 csBuf  = csBuf  + QString::number(m);
 csDate = csDate + csBuf.right(2); if (sep) csDate += QString(sep); // "05-06-"

 csBuf  = "0000";
 csBuf  = csBuf  + QString::number(y);
 csDate = csDate + csBuf.right(4);       // "05-06-1956"
 return csDate;
}

/*------------------------------ JJMMAAAA_To_Val -------------------------------*/
long CGestRPU::JJMMAAAA_To_Val(const char* date_in, long *d, long *m, long* y)
{char date[9];
 if (strlen(date_in)!=8) return -1;

 if (d)
    {memcpy(date, date_in,   2);
     date[2]=0;
     *d=atol(date);
    }
 if (m)
    {memcpy(date, date_in+2, 2);
     date[2]=0;
     *m=atol(date);
    }
 if (y)
    {memcpy(date, date_in+4, 4);
     date[4]=0;
     *y=atol(date);
    }
 memcpy(date,   date_in+4, 4);
 memcpy(date+4, date_in+2, 2);
 memcpy(date+6, date_in,   2);
 date[8]=0;
 return atol(date);
}

/*----------------------------- ExtraitChiffre -------------------------------------*/
// ACTION: à partir d'un pointeur texte quelconque va jusqu'au premier chiffre
//         rencontré, et tant qu'il y a des chiffres les collecte et retourne
//         dans long *result la valeur de ces chiffres.
// ENTREE: char **pt_t: pointe sur le pointeur de texte du texte.
//                      en sortie il pointe sur le caractère situé apres le chiffre
//                      si un chiffre à ete trouvé sinon il ne change pas
//         long *result: le resultat trouvé sera placé là
//         char *end:  pointeur sur la fin du texte. si il est à zero alors il sera
//                     considéré que la fin du texte est le zero de fin.
// RETOUR: BOOL : qui est à FALSE si pas de chiffre trouvé et à TRUE si trouvé.

bool CGestRPU::ExtraitChiffre(char **pt_t, long *result, char* end)
{char *pt = *pt_t;
 if (end==0) end = pt + strlen(pt);
 while (pt<end && *pt !='\r' && !(*pt>='0' && *pt<='9')) pt++;  // se positionner sur premier chiffre
 //........ si pas de chiffre trouvé avant fin de ligne, retourner erreur ................
 if (pt==end||*pt=='\r'||*pt=='\n') return FALSE;
 char *t_deb = pt;
 char *t_end = pt;
 char  sav;
 while (t_end<end && *t_end>='0'&& *t_end<='9') t_end++;
  sav    = *t_end; *t_end=0;
 *result = atol( t_deb);
 *t_end  =  sav;
 *pt_t   = t_end;
 return TRUE;
}

/*------------------------------ HeureMinuteSeconde_To_Val -------------------------------*/
long CGestRPU::HeureMinuteSeconde_To_Val(const char* heure_in, long *h, long *m, long* s)
{char heure[7];
 char buf[3];
 if (strlen(heure_in)==2)          // si 2 chiffres alors completer les secondes et minutes
    {memcpy(heure, heure_in,2);
     strcpy(heure+2, "0000");
    }
 if (strlen(heure_in)==4)          // si 4 chiffres alors completer les secondes
    {memcpy(heure, heure_in,4);
     strcpy(heure+4, "00");
    }
 else if (strlen(heure_in)==6)     // si 6 chiffres tout prendre
    {memcpy(heure, heure_in,6);
     heure[6]=0;
    }
 else                              // si rien de tout cela cassos
    {return -1;
    }
 int hv,mv,sv;
 //.......... heure ...........
 memcpy(buf, heure_in,   2);
 buf[2]=0;
 hv=atol(buf);

 memcpy(buf, heure_in+2, 2);
 buf[2]=0;
 mv=atol(buf);

 memcpy(buf, heure_in+4, 2);
 buf[2]=0;
 sv=atol(buf);

 if (h) *h=hv;
 if (m) *m=mv;
 if (s) *s=sv;

 return  hv*3600+mv*60+sv;
}

/*------------------------------ NbSec_To_NbJHMS -------------------------------*/
void CGestRPU::NbSec_To_NbJHMS(long secondes_in, long*jr, long *hr, long *mr, long* sr)
{long j,h,m,s;
 j = secondes_in/86400;
 secondes_in -= j*86400;

 h = secondes_in/3600;
 secondes_in -= h*3600;

 m = secondes_in/60;
 s = secondes_in - m*60;

 if (jr) *jr = j;
 if (hr) *hr = h;
 if (mr) *mr = m;
 if (sr) *sr = s;
}

/*------------------------------ LogWrite -------------------------------*/
long CGestRPU::LogWrite(const char *txt, const char *path, const char *name /*=0*/)
{//                      vivent les grands classiques
 FILE *fdst;
 QString    path_f  = path;
 if (name)
    {if (path[strlen(path)-1]!='\\'&&name[0]!='\\') path_f +='\\';
     path_f += name;
    }
 long      file_len = strlen(txt);
 if (file_len==0) return 0;
 fdst = fopen( path_f, "ab");
 if (fdst==0)     return 0;
 fwrite(txt, file_len, 1, fdst);
 fclose(fdst);
 return file_len;
}

/*------------------------------ LogCreate -------------------------------*/
long CGestRPU::LogCreate(const char *txt, const char *path, const char *name )
{//                      vivent les grands classiques
 FILE *fdst;
 QString    path_f  = path;
 if (name)
    {if (path[strlen(path)-1]!='\\'&&name[0]!='\\') path_f +='\\';
     path_f += name;
    }
 long      file_len = strlen(txt);

 fdst = fopen( path_f, "wb");
 if (fdst==0)     return 0;
 if (file_len!=0)
    {fwrite(txt, file_len, 1, fdst);
    }
 fclose(fdst);
 return file_len;
}

