/***************************************************************************
 *   Copyright (C) 2004 by Sébastien SIVERA & Sevin Roland                *
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

/* base64 encoder/decoder. Originally part apache sources */

//=============================================== INCLUDES ======================================================================
#include "CSoapInterface.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "Global.h"
#include <q3url.h> 
#include <q3cstring.h> 
#include <qmessagebox.h> 
#include <qtimer.h>
#include <stdlib.h>


//=============================================== DEFINE & GLOBAL ===============================================================
//===============================================================================================================================
//:::::::::::::::::::::::::::::::::::::::::::::   WsActeDel :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//===============================================================================================================================
//------------------------------------------ Add_Data ----------------------------------------------------------------------------
int WsActeDel::AddData(const QString &dataName, const char* Data,  QString &attName,  QString &attValue)
{ //.................. relever code de retour ..................................................
  //                   de l'appel sigems
  if ( dataName == m_Type) 
     {if (attName=="#~#ToPlace#~#") m_RetCode = Data;   // on place les données caractères obtenues à la suite du return 1
      else                          return 1;           // les prochaines données caractères devraient retourner le nom: 
     } 
  //............... Si code de retour trouvé alors ou peut y aller ..........................
  else if (m_RetCode != "")
     {return _AddData(dataName, Data, attName,  attValue);
     }
  return 0;
}

//------------------------------------------ _Add_Data ----------------------------------------------------------------------------
int WsActeDel::_AddData(const QString &dataName, const char* Data, QString &,  QString &)
{ return 0;
}

//===============================================================================================================================
//:::::::::::::::::::::::::::::::::::::::::::::   WsActeCcam ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//===============================================================================================================================
//------------------------------------------ Add_Data ----------------------------------------------------------------------------
int WsActeCcam::AddData(const QString &dataName, const char* Data,  QString &attName,  QString &attValue)
{ //.................. relever code de retour ..................................................
  //                   de l'appel sigems
  if ( dataName == m_Type) 
     {if (attName=="#~#ToPlace#~#") m_RetCode = Data;   // on place les données caractères obtenues à la suite du return 1
      else                          return 1;           // les prochaines données caractères devraient retourner le nom: "listeDatabases"
     } 
  //............... Si code de retour trouvé alors ou peut y aller ..........................
  else if (m_RetCode != "")
     {return _AddData(dataName, Data, attName,  attValue);
     }
  return 0;
}

//------------------------------------------ _Add_Data ----------------------------------------------------------------------------
int WsActeCcam::_AddData(const QString &dataName, const char* Data, QString &,  QString &)
{
   if (dataName=="activite")
      {if (Data==0) return 1;
       else       m_activite                     = Data;
      }					 
   if (dataName=="associationNonPrevue")
      {if (Data==0) return 1;
       else       m_associationNonPrevue         = Data;
      }
   if (dataName=="chargeSupplementaire")
      {if (Data==0) return 1;
       else       m_chargeSupplementaire         = Data;
      }
   if (dataName=="codeActe")
      {if (Data==0) return 1;
       else       m_codeActe                     = Data;
      }
   if (dataName=="date")
      {if (Data==0) return 1;
       else       m_date                         = Data;
      }
   if (dataName=="enfant")
      {if (Data==0) return 1;
       else       m_enfant                       = Data;
      }
   if (dataName=="extensionDocumentaire")
      {if (Data==0) return 1;
       else       m_extensionDocumentaire        = Data;
      }
   if (dataName=="heureDebut")
      {if (Data==0) return 1;
       else       m_heureDebut                   = Data;
      }
   if (dataName=="idPraticien")
      {if (Data==0) return 1;
       else       m_idPraticien                  = Data;
      }
   if (dataName=="ippSigems")
      {if (Data==0) return 1;
       else       m_ippSigems                    = Data;
      }
   if (dataName=="modificateur1")
      {if (Data==0) return 1;
       else       m_modificateur1                = Data;
      }
   if (dataName=="modificateur2")
      {if (Data==0) return 1;
       else       m_modificateur2                = Data;
      }
   if (dataName=="modificateur3")
      {if (Data==0) return 1;
       else       m_modificateur3                = Data;
      }
   if (dataName=="modificateur4")
      {if (Data==0) return 1;
       else       m_modificateur4                = Data;
      }
   if (dataName=="nbOccurence")
      {if (Data==0) return 1;
       else       m_nbOccurence                  = Data;
      }
   if (dataName=="numDossier")
      {if (Data==0) return 1;
       else       m_numDossier                   = Data;
      }
   if (dataName=="phase")
      {if (Data==0) return 1;
       else       m_phase                        = Data;
      }
   if (dataName=="remboursementExceptionnel")
      {if (Data==0) return 1;
       else       m_remboursementExceptionnel    = Data;
      }
   if (dataName=="uniteMedicale")
      {if (Data==0) return 1;
       else       m_uniteMedicale                = Data;
      }
   if (dataName=="lienAvecActeExonereAnterieur")
      {if (Data==0) return 1;
       else       m_lienAvecActeExonereAnterieur = Data;
      }
   if (dataName=="prescripteur")
      {if (Data==0) return 1;
       else       m_prescripteur                 = Data;
      }
   if (dataName=="depassementHonoraire")
      {if (Data==0) return 1;
       else       m_depassementHonoraire         = Data;
      }
   if (dataName=="codeRejet")
      {if (Data==0) return 1;
       else       m_codeRejet                    = Data;
      }
   
   return 0;
}
//===============================================================================================================================
//:::::::::::::::::::::::::::::::::::::::::::::   WsActeCcamListe :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//===============================================================================================================================

//------------------------------------------ AddData ----------------------------------------------------------------------------
int WsActeCcamListe::AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue)
{//................. recuperer valeur de retour (nom et code) .......................................

   //............... SI Analyse des propriétés tableau de patient pas encore terminée ...............
   //                (le tableau: m_pWsDossier n'a pu encore etre créé donc à zero)
    //                on continue à les chercher 
   if (m_pWsActeCcam == 0) 
      {//.................. relever code de retour ..................................................
       //                   de l'appel sigems
       if (dataName == m_Type) 
	  {if (attName=="#~#ToPlace#~#") m_RetCode = Data;   // on place les données caractères
	   else                          return 1;           
	  }       
       //........ relever le nombre de structures WsDossier: WsActeCcam  ................
       //                  que devra stocker la liste:       WsActeCcamListe
       else if  ( attName=="soapenc:arrayType")
           {int  d = attValue.find(":");             if  (d==-1)   return 0;
            int  e = attValue.find("[");             if  (e==-1)   return 0;
            d      = e+1;
            e      = attValue.find( "]", d);         if  (e==-1)   return 0;
            m_Nb   = atoi(attValue.mid(d, e-d));     if  (m_Nb==0) return 0;
	    //............ si Ok on a trouvé le nombre alors créer le tableau........................
	    //             de structures:                  WsDossierListe
            if (m_pWsActeCcam == 0)
               {m_pWsActeCcam = new WsActeCcam[m_Nb]; 
	        for (int i=0; i<m_Nb; i++)m_pWsActeCcam[i].Init(WS_ACTECCAM);   //#define  WS_ACTECCAM                     "WsActeCcam"  
               }
	      }
      } // endif (m_pWsActeCcam==0)   
   
   //............... LE TABLEAU EST CREE IL FAUT REMPLIR LES STRUCTURES .............................
   else 
      {//................... marque de debut de structure ...........................................
       if ( attName=="id")
          {if (m_Current != -1) m_pWsActeCcam[m_Current].m_ParseState = PARSE_STATE_FINISHED;
	   ++m_Current;	  
          }
       //................... si indice du tableau coherent ..........................................
       else if (m_Current != -1 && m_Current < m_Nb)
	  {WsActeCcam* pWsActeCcam = &m_pWsActeCcam[m_Current]; 
	   //............... le parseur doit placer des données ....................................
	   if ( attName == "#~#ToPlace#~#" ) 
	      {return pWsActeCcam->_AddData(dataName, Data, attName, attValue);
	      }
	   if ( attName == "xsi:type")
	      {if ( pWsActeCcam->m_ParseState == PARSE_STATE_NOTPARSED )
                  {int  d = attValue.find(":");
                   if  (d==-1) return 0;
	           if (attValue.mid(d+1) == pWsActeCcam->m_Type) pWsActeCcam->m_ParseState = PARSE_STATE_BEGIN;
	          }
               else 
                  {return pWsActeCcam->_AddData(dataName, Data, attName, attValue);
                  }
	      } //end if ( attName == "xsi:type")
          } // end else if (m_Current != -1 && m_Current < m_Nb)
      }// end else if (m_pWsActeCcam==0) 
   return 0;

} //end   int AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue)





//===============================================================================================================================
//:::::::::::::::::::::::::::::::::::::::::::::   WsDossier ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//===============================================================================================================================

//------------------------------------------ Add_Data ----------------------------------------------------------------------------
int WsDossier::AddData(const QString &dataName, const char* Data,  QString &attName,  QString &attValue)
{//.................. relever code de retour ..................................................
  //                   de l'appel sigems
  if ( dataName == m_Type) 
     {if (attName=="#~#ToPlace#~#") m_RetCode = Data;   // on place les données caractères obtenues à la suite du return 1
      else                          return 1;           // les prochaines données caractères devraient retourner le nom: "listeDatabases"
     } 
  //............... Si code de retour trouvé alors ou peut y aller ..........................
  else if (m_RetCode != "")
     {return _AddData(dataName, Data, attName,  attValue);
     }
  return 0;
}


//------------------------------------------ _Add_Data ----------------------------------------------------------------------------
int WsDossier::_AddData(const QString &dataName, const char* Data, QString &,  QString &)
{
   if (dataName=="numero")
      {if (Data==0) return 1;
       else       m_numero            = Data;
      }
   if (dataName=="ippSigemsPatient")
      {if (Data==0) return 1;
       else       m_ippSigemsPatient  = Data;
      }
   if (dataName=="ippSigemsAssure")
      {if (Data==0) return 1;
       else       m_ippSigemsAssure   = Data;
      }
   if (dataName=="dateEntree")
      {if (Data==0) return 1;
       else       m_dateEntree        = Data;
      }
   if (dataName=="heureEntree")
      {if (Data==0) return 1;
       else       m_heureEntree       = Data;
      }
   if (dataName=="dateSortiePrevue")
      {if (Data==0) return 1;
       else       m_dateSortiePrevue  = Data;
      }
   if (dataName=="dateSortieReelle")
      {if (Data==0) return 1;
       else       m_dateSortieReelle  = Data;
      }
   if (dataName=="heureSortie")
      {if (Data==0) return 1;
       else       m_heureSortie       = Data;
      }
   if (dataName=="modeTraitement")
      {if (Data==0) return 1;
       else       m_modeTraitement    = Data;
      }
   if (dataName=="service")
      {if (Data==0) return 1;
       else       m_service           = Data;
      }
   if (dataName=="motif")
      {if (Data==0) return 1;
       else       m_motif             = Data;
      }
   if (dataName=="typeChambre")
      {if (Data==0) return 1;
       else       m_typeChambre       = Data;
      }
   if (dataName=="dateIntervention")
      {if (Data==0) return 1;
       else       m_dateIntervention  = Data;
      }
   if (dataName=="heureIntervention")
      {if (Data==0) return 1;
       else       m_heureIntervention = Data;
      }
   if (dataName=="idMedecinTraitant")
      {if (Data==0) return 1;
       else       m_idMedecinTraitant = Data;
      }
   if (dataName=="idChirurgien")
      {if (Data==0) return 1;
       else       m_idChirurgien      = Data;
      }
   if (dataName=="idAnesthesiste")
      {if (Data==0) return 1;
       else       m_idAnesthesiste    = Data;
      }
   return 0;
}

//===============================================================================================================================
//:::::::::::::::::::::::::::::::::::::::::::::   WsDossierListe ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//===============================================================================================================================


//------------------------------------------ AddData ----------------------------------------------------------------------------
int WsDossierListe::AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue)
{//................. recuperer valeur de retour (nom et code) .......................................


   //............... SI Analyse des propriétés tableau de patient pas encore terminée ...............
   //                (le tableau: m_pWsDossier n'a pu encore etre créé donc à zero)
    //                on continue à les chercher 
   if (m_pWsDossier == 0) 
      {//.................. relever code de retour ..................................................
       //                   de l'appel sigems
       if (dataName == m_Type) 
	  {if (attName=="#~#ToPlace#~#") m_RetCode = Data;   // on place les données caractères
	   else                          return 1;           
	  }       
       //........ relever le nombre de structures WsDossier: WsDossier  ................
       //                  que devra stocker la liste:       WsDossierListe
       else if  ( attName=="soapenc:arrayType")
           {int  d = attValue.find(":");             if  (d==-1)   return 0;
            int  e = attValue.find("[");             if  (e==-1)   return 0;
            d      = e+1;
            e      = attValue.find( "]", d);         if  (e==-1)   return 0;

            m_Nb   = atoi(attValue.mid(d, e-d));     if  (m_Nb==0) return 0;

	    //............ si Ok on a trouvé le nombre alors créer le tableau........................

	    //             de structures:                  WsDossierListe

            if (m_pWsDossier == 0)

               {// m_pWsDossier = new WsDossier[m_Nb]("WsDossier");    // pas bon pour borland

	        m_pWsDossier = new WsDossier[m_Nb]; 

	        for (int i=0; i<m_Nb; i++)m_pWsDossier[i].Init("WsDossier");

               }
	   }
      } // endif (m_pWsDossier==0)   
   
   //............... LE TABLEAU EST CREE IL FAUT REMPLIR LES STRUCTURES .............................
   else 
      {//................... marque de debut de structure ...........................................
       if ( attName=="id")
          {if (m_Current != -1) m_pWsDossier[m_Current].m_ParseState = PARSE_STATE_FINISHED;
	   ++m_Current;	  
          }
       //................... si indice du tableau coherent ..........................................
       else if (m_Current != -1 && m_Current < m_Nb)
	  {WsDossier* pWsDossier = &m_pWsDossier[m_Current];
	   //............... le parseur doit placer des données ....................................
	   if ( attName == "#~#ToPlace#~#" ) 
	      {return pWsDossier->_AddData(dataName, Data, attName, attValue);
	      }
	   if ( attName == "xsi:type")
	      {if ( pWsDossier->m_ParseState == PARSE_STATE_NOTPARSED )
                  {int  d = attValue.find(":");
                   if  (d==-1) return 0;
	           if (attValue.mid(d+1) == pWsDossier->m_Type) pWsDossier->m_ParseState = PARSE_STATE_BEGIN;
	          }
               else 
                  {return pWsDossier->_AddData(dataName, Data, attName, attValue);
                  }
	      } //end if ( attName == "xsi:type")
          } // end else if (m_Current != -1 && m_Current < m_Nb)
      }// end else if (m_pWsDossier==0) 
   return 0;
} //end   int AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue)


//===============================================================================================================================
//:::::::::::::::::::::::::::::::::::::::::::::   WsPatient :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//===============================================================================================================================

//------------------------------------------ AddData ----------------------------------------------------------------------------
int WsPatient::AddData(const QString &dataName, const char* Data, QString &attName,  QString &)
{ //.................. relever code de retour ..................................................
  //                   de l'appel sigems
  if ( dataName == m_Type) 
     {if (attName=="#~#ToPlace#~#") m_RetCode = Data;   // on place les données caractères obtenues à la suite du return 1
      else                          return 1;           // les prochaines données caractères devraient retourner le nom: "listeDatabases"
     } 
  //............... Si code de retour trouvé alors ou peut y aller ..........................
  else if (m_RetCode != "")
     { if (dataName=="adresse1")
          {if (Data==0) return 1;
           else       m_adresse1       = Data;
          }
       if (dataName=="adresse2")
          {if (Data==0) return 1;
           else       m_adresse2       = Data;
          }
       if (dataName=="cleSecu")
          {if (Data==0) return 1;
           else       m_cleSecu        = Data;
          }
       if (dataName=="codePostal")
          {if (Data==0) return 1;
           else       m_codePostal     = Data;
          }
       if (dataName=="dateNaissance")
          {if (Data==0) return 1;
           else       m_dateNaissance  = Data;
          }
       if (dataName=="ippExtern")
          {if (Data==0) return 1;
           else       m_ippExtern      = Data;
          }
       if (dataName=="ippSigems")
          {if (Data==0) return 1;
           else       m_ippSigems      = Data;
          }
       if (dataName=="nomNaissance")
          {if (Data==0) return 1;
           else       m_nomNaissance   = Data;
          }
       if (dataName=="nomUsuel")
          {if (Data==0) return 1;
           else       m_nomUsuel       = Data;
          }
       if (dataName=="numSecu")
          {if (Data==0) return 1;
           else       m_numSecu        = Data;
          }
       if (dataName=="prenom")
          {if (Data==0) return 1;
           else       m_prenom         = Data;
          }
       if (dataName=="sexe")
          {if (Data==0) return 1;
           else       m_sexe           = Data;
          }
       if (dataName=="telephone")
          {if (Data==0) return 1;
           else       m_telephone      = Data;
          }
       if (dataName=="ville")
          {if (Data==0) return 1;
           else       m_ville          = Data;
          }
       if (dataName=="villeNaissance")
          {if (Data==0) return 1;
           else       m_villeNaissance = Data;
          }
      }
   return 0;
}
//===============================================================================================================================
//:::::::::::::::::::::::::::::::::::::::::::::   DatabaseIdentification ::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//===============================================================================================================================

//------------------------------------------ AddData ----------------------------------------------------------------------------
int DatabaseIdentification::AddData(const QString &dataName, const char* Data, QString &,  QString &)
{
   if (dataName=="description")
      {if (Data==0) return 1;
       else       m_description     = Data;
      }
   if (dataName=="identifiant")
      {if (Data==0) return 1;
       else       m_identifiant     = Data;
      }
   return 0;
}

//===============================================================================================================================
//:::::::::::::::::::::::::::::::::::::::::::::   DatabaseIdentificationListe :::::::::::::::::::::::::::::::::::::::::::::::::::
//===============================================================================================================================

//------------------------------------------ AddData ----------------------------------------------------------------------------
int DatabaseIdentificationListe::AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue)
{//................. recuperer valeur de retour (nom et code) .......................................

   //............... SI Analyse des propriétés tableau de patient pas encore terminée ...............
   //                (le tableau: m_pDatabaseIdentification n'a pu encore etre créé donc à zero)
    //                on continue à les chercher 
   if (m_pDatabaseIdentification==0) 
      {//.................. relever code de retour ..................................................
       //                   de l'appel sigems
       if (dataName == m_Type) 
	  {if (attName=="#~#ToPlace#~#") m_RetCode = Data;   // on place les données caractères obtenues à la suite du return 1
	   else                          return 1;           // les prochaines données caractères devraient retourner le nom: "listeDatabases"
	  }       
       //........ relever le nombre de structures patients: DatabaseIdentification  ................
       //                  que devra stocker la liste:      m_pDatabaseIdentificationListe
       else if  ( attName=="soapenc:arrayType")
           {int  d = attValue.find(":");             if  (d==-1)   return 0;
            int  e = attValue.find("[");             if  (e==-1)   return 0;
            d = e+1;
            e = attValue.find( "]", d);              if  (e==-1)   return 0;
            m_Nb = atoi(attValue.mid(d, e-d));       if  (m_Nb==0) return 0;
	    //............ si Ok on a trouvé le nombre alors créer le tableau........................
	    //             de structures:                  DatabaseIdentification
            if (m_pDatabaseIdentification==0)
               {m_pDatabaseIdentification = new DatabaseIdentification[m_Nb];                
               }
	   }
      } // endif (m_pWsPatientIdentification==0)   
   
   //............... LE TABLEAU EST CREE IL FAUT REMPLIR LES STRUCTURES .............................
   else 
      {//................... marque de debut de structure ...........................................
       if ( attName=="id")
          {if (m_Current != -1) m_pDatabaseIdentification[m_Current].m_ParseState = PARSE_STATE_FINISHED;
	   ++m_Current;	  
          }
       //................... si indice du tableau coherent ..........................................
       else if (m_Current != -1 && m_Current < m_Nb)
	  {DatabaseIdentification* pElement = &m_pDatabaseIdentification[m_Current];
	   //............... le parseur doit placer des données ....................................
	   if ( attName == "#~#ToPlace#~#" ) 
	      {return pElement->AddData(dataName, Data, attName, attValue);
	      }
	   if ( attName == "xsi:type")
	      {if ( pElement->m_ParseState == PARSE_STATE_NOTPARSED )
                  {int  d = attValue.find(":");
                   if  (d==-1) return 0;
	           if (attValue.mid(d+1) == pElement->m_Type) pElement->m_ParseState = PARSE_STATE_BEGIN;
	          }
               else 
                  {return pElement->AddData(dataName, Data, attName, attValue);
                  }
	      } //end if ( attName == "xsi:type")
          } // end else if (m_CurrentPat != -1 && m_Current < m_Nb)
      }// end else if (m_pWsPatientIdentification==0) 
   return 0;
} //end   int AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue)


//===============================================================================================================================
//:::::::::::::::::::::::::::::::::::::::::::::   WsPatientIdentification :::::::::::::::::::::::::::::::::::::::::::::::::::::::
//===============================================================================================================================


//------------------------------------------ AddData ----------------------------------------------------------------------------
int WsPatientIdentification::AddData(const QString &dataName, const char* Data, QString &,  QString &)
{

   if (dataName=="ippSigems")

      {if (Data==0) return 1;

       else       m_ippSigems     = Data;

      }

   if (dataName=="nomNaissance")

      {if (Data==0) return 1;

       else       m_nomNaissance  = Data;

      }

   if (dataName=="nomUsuel")

      {if (Data==0) return 1;

       else       m_nomUsuel      = Data;

      }

   if (dataName=="prenom")

      {if (Data==0) return 1;

       else       m_prenom        = Data;

      }

   if (dataName=="dateNaissance")

      {if (Data==0) return 1;

       else       m_dateNaissance = Data;

      }

   return 0;

}



//===============================================================================================================================

//:::::::::::::::::::::::::::::::::::::::::::::   WsPatientIdentificationListe ::::::::::::::::::::::::::::::::::::::::::::::::::

//===============================================================================================================================



//------------------------------------------ AddData ----------------------------------------------------------------------------

int WsPatientIdentificationListe::AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue)

{//................. recuperer valeur de retour (nom et code) .......................................



   //............... SI Analyse des propriétés tableau de patient pas encore terminée ...............

   //                (le tableau: m_pWsPatientIdentification n'a pu encore etre créé donc à zero)

    //                on continue à les chercher 

   if (m_pWsPatientIdentification==0) 

      {//.................. relever code de retour ..................................................

       //                   de l'appel sigems

       if (dataName == m_Type) 

	  {if (attName=="#~#ToPlace#~#") m_RetCode = Data;   // on place les données caractères

	   else                          return 1;           // les prochaines données caractères devraient retourner le nom: "listePatients"

	  }       

       //........ relever le nombre de structures patients: WsPatientIdentification  ................

       //                  que devra stocker la liste:      WsPatientIdentificationListe

       else if  ( attName=="soapenc:arrayType")

           {int  d = attValue.find(":");             if  (d==-1)   return 0;

            int  e = attValue.find("[");             if  (e==-1)   return 0;

            d = e+1;

            e = attValue.find( "]", d);              if  (e==-1)   return 0;

            m_Nb = atoi(attValue.mid(d, e-d));       if  (m_Nb==0) return 0;

	    //............ si Ok on a trouvé le nombre alors créer le tableau........................

	    //             de structures:                  WsPatientIdentification

            if (m_pWsPatientIdentification==0)

               {m_pWsPatientIdentification = new WsPatientIdentification[m_Nb];                

               }

	   }

      } // endif (m_pWsPatientIdentification==0)   

   

   //............... LE TABLEAU EST CREE IL FAUT REMPLIR LES STRUCTURES .............................

   else 

      {//................... marque de debut de structure ...........................................

       if ( attName=="id")

          {if (m_Current != -1) m_pWsPatientIdentification[m_Current].m_ParseState = PARSE_STATE_FINISHED;

	   ++m_Current;	  

          }

       //................... si indice du tableau coherent ..........................................

       else if (m_Current != -1 && m_Current < m_Nb)

	  {WsPatientIdentification* pPatIdent = &m_pWsPatientIdentification[m_Current];

	   //............... le parseur doit placer des données ....................................

	   if ( attName == "#~#ToPlace#~#" ) 

	      {return pPatIdent->AddData(dataName, Data, attName, attValue);

	      }

	   if ( attName == "xsi:type")

	      {if ( pPatIdent->m_ParseState == PARSE_STATE_NOTPARSED )

                  {int  d = attValue.find(":");

                   if  (d==-1) return 0;

	           if (attValue.mid(d+1) == pPatIdent->m_Type) pPatIdent->m_ParseState = PARSE_STATE_BEGIN;

	          }

               else 

                  {return pPatIdent->AddData(dataName, Data, attName, attValue);

                  }

	      } //end if ( attName == "xsi:type")

          } // end else if (m_Current != -1 && m_Current < m_Nb)

      }// end else if (m_pWsPatientIdentification==0) 

   return 0;

} //end   int AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue)



//===============================================================================================================================

//:::::::::::::::::::::::::::::::::::::::::::::   CSoapInterface ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//===============================================================================================================================



//------------------------------------------ CSoapInterface ---------------------------------------------------------------------

CSoapInterface::CSoapInterface ()

 { m_ResponseHeader       = "";

   m_ResponseBody         = "";

   m_ResponseTestFile     = "";

   m_TestFlag             = FALSE;

 };



/*

CSoapInterface::CSoapInterface (QObject * parent , const char * name )

    :QHttp(parent, name)

 { m_ResponseHeader       = "";

   m_ResponseBody         = "";

   m_ResponseTestFile     = "";

   m_TestFlag             = FALSE;

 };

*/

//------------------------------------------ SendMessage ------------------------------------------------------------------------

void CSoapInterface::SetDebugMode(const QString &file, int debug_mode /* =FALSE */)

{m_ResponseTestFile = file;

 m_TestFlag         = debug_mode;

}



//------------------------------------------ SendMessage ------------------------------------------------------------------------

QString CSoapInterface::SendMessage(QString             url_string,  

                                    QString             soapaction_string, 

				    QString             message,

				    QString             multiRef           /*=""*/,				 

				    QString             contenttype_string /*="text/xml; charset=\"utf-8\""*/, 

				    QString             proxy_string       /*= "" */,

				    QString             proxy_port         /*= 80 */

				   )

{Q3HttpRequestHeader header = PrepareHttpRequestHeader( url_string,  

						       soapaction_string, 

                                                       contenttype_string /*="text/xml; charset=\"utf-8\""*/, 

						       proxy_string       /*= "" */,

						       proxy_port         /*= 80 */

						     );

 QString ret("");

 QString soapenv_mess = CreateSOAPMessage(soapaction_string, message, multiRef).utf8();

 //.................. annuler les reponses .........................

 m_ResponseHeader = "";

 m_ResponseBody   = "";

 //.................. envoyer la requete ...........................

 if (m_TestFlag) // SI MODE SIMULE

    {header.setValue( "Content-Length", QString::number(soapenv_mess.length()) );

     //........... retourner la requete ............................................

     ret = header.toString() +  soapenv_mess;     

     //........... créer la reponse simulée ........................................
     QString txt;

     CGestIni::Param_UpdateFromDisk(m_ResponseTestFile, txt);

     GetXML_Part(txt,  m_ResponseBody, &m_ResponseHeader );   // separer l'ivraie du bon grain     

    }

 else            // SI MODE REEL

    {ret = header.toString() +  soapenv_mess;

     QByteArray ba;

     ba.duplicate( (char*)(const char*) soapenv_mess.data(), soapenv_mess.length() );

     request( header, ba  );

    }

 return ret;    

}





//------------------------------------------ ResponseHeaderToStr -------------------------------------------------------------------------

QString CSoapInterface::ResponseHeaderToStr(const Q3HttpResponseHeader& response )

{if (m_ResponseHeader=="" )  // en mode simulé il n'est  pas appelé

    {m_ResponseHeader.append( QString("<b>HTTP/%1.%2 %3 %4</b><br>").arg( response.majorVersion() ).arg( response.minorVersion() ).arg( response.statusCode() ).arg( response.reasonPhrase() ) );

     QStringList headers = response.keys();

     for ( QStringList::Iterator it = headers.begin(); it != headers.end(); ++it ) 

         {m_ResponseHeader.append( "<b>" + (*it) + "</b>: " + response.value( *it ) + "<br>" );

	 }

     if ( !m_ResponseHeader.isEmpty() ) m_ResponseHeader.append( "<br>" );

    }

 return m_ResponseHeader;

}



//------------------------------------------ GetResponseHeader -------------------------------------------------------------------------

QString CSoapInterface::GetResponseHeader()

{return m_ResponseHeader;

}



//------------------------------------------ GetResponseBody -------------------------------------------------------------------------

QString CSoapInterface::GetResponseBody(bool error)   // en mode simulé toujours FALSE

{if (m_ResponseBody=="") 

    {if ( error )    {QString resultStr = "Error sending the SOAP message:\n";

	              resultStr.append(errorString());

	              QMessageBox::warning( 0,"HTTP Error", resultStr );			

		      return QString("Error");

	             }

     m_ResponseBody.append(readAll());

    }

 return m_ResponseBody;

}

  

//------------------------------------------ GetXML_Part -------------------------------------------------------------------------

int CSoapInterface::GetXML_Part( QString &txt,  QString& xml, QString* http_header /* = 0 */)

{int f = txt.find("<?xml");

 if (f == -1) return 0;

 xml = txt.mid(f);

 if (http_header)

    {*http_header = txt.left(f);

    }

 return f;

}



//------------------------------------------ XML_To_Colored ---------------------------------------------------------------------

bool CSoapInterface::XML_To_Colored( const QString& xml, QString& result )

{

	SOAPSyntaxHighlighter handler;



        QXmlInputSource source;

	source.setData( xml );

        QXmlSimpleReader reader;

	reader.setFeature( "http://xml.org/sax/features/namespaces", TRUE );

	reader.setFeature( "http://xml.org/sax/features/namespace-prefixes", TRUE );

	reader.setFeature( "http://trolltech.com/xml/features/report-start-end-entity", FALSE );



        reader.setContentHandler( &handler );

	reader.setLexicalHandler( &handler );



	bool ret = reader.parse( source );

	result   = handler.getResult();

	return ret;

}



//------------------------------------------ XML_To_SigemsResult ---------------------------------------------------------------------

bool CSoapInterface::XML_To_SigemsResult( const QString& xml, CSigemsReturn* pPesult )

{

	CSoapGenericXMLHandler handler;

        handler.SetResult(pPesult);

        QXmlInputSource source;

	source.setData( xml );

        QXmlSimpleReader reader;

	reader.setFeature( "http://xml.org/sax/features/namespaces", TRUE );

	reader.setFeature( "http://xml.org/sax/features/namespace-prefixes", TRUE );

	reader.setFeature( "http://trolltech.com/xml/features/report-start-end-entity", FALSE );



        reader.setContentHandler( &handler );

	reader.setLexicalHandler( &handler );

	bool ret = reader.parse( source );

	

	return ret;

}



//------------------------------------------ DisplaySigemsError -----------------------------------------------------------------

void CSoapInterface::DisplaySigemsError(const QString &error_str)

{int error    = error_str.toInt();
 QString str  = "";
 switch (error)
    {case -1: str = TR("Votre requête s'est mal passée ou ne retourne aucun résultat.");         break;
     case -2: str = TR("Les paramètres d'entrée sont tous vides.");                              break;
     case -3: str = TR("Le patient passé en paramètre est déjà présent dans la base Sigems.");   break;
     case -4: str = TR("Les paramètres d'entrée sont incomplets ou mal renseignés.");            break;
     case -5: str = TR("Le patient passé en paramètre d'entrée est absent de la base Sigems.");  break;
     case -6: str = TR("Erreur interne du service, veuillez en informer la société Sigems.");    break;
     case -7: str = TR("Accés refusé.");                                                         break;
     default: str = TR("Erreur inconnue.");
    }

 QString resultStr("");
 resultStr += TR("Erreur SIGEMS N° ") + error_str + "\n";
 resultStr += str;
 QMessageBox::warning( 0,TR("Erreur"), resultStr );

}



//------------------------------------------ PrepareHttpRequestHeader -----------------------------------------------------------

Q3HttpRequestHeader CSoapInterface::PrepareHttpRequestHeader(QString             url_string,  

							    QString             soapaction_string, 

							    QString             contenttype_string /*="text/xml; charset=\"utf-8\""*/, 

							    QString             proxy_string       /*= "" */,

							    QString             proxy_port         /*= 80 */

							   )

{       

        Q3Url               url( url_string);

	Q3HttpRequestHeader header;

                                                            

	//.......... if we post through a proxy server, the post request must contain ................

	if ( proxy_string != "" )

		header.setRequest( "POST", url.toString() );

	else

        	header.setRequest( "POST", url.path() );



	//.......... url host and port ...............................................................

	if ( url.hasPort() )

		header.setValue( "Host", url.host() + ":"  + QString::number(url.port())  );

	else

        	header.setValue( "Host", url.host() );



	//.......... soap action header ...............................................................

	if ( !soapaction_string.isEmpty())

	{       header.setValue( "SOAPAction", QString("\"") + "urn:" + soapaction_string + "\"" );

	}

        //.......... base 64 autentification ..........................................................

	/*

	if (url.hasPassword()&& url.hasUser())

	   {QCString auth = QCodecs::base64Encode(QCString(url.user()+":"+url.password()));

            header.setValue("Authorization", auth.data());

	   }

	*/

	///*

	if (url.hasPassword()&& url.hasUser())

	{size_t        len_user         =   url.user().length();

	 size_t    len_password         =   url.password().length();

         size_t     len_encoded         =   Base64_encode_len(len_user + len_password + 1);	    

         char*   encoded_string         =   new  char[len_encoded+9]; // +9 for " Basic "

         if (encoded_string)	             

	   {  

            //..................................... encode the plain string ..........................

            strcpy(encoded_string," Basic ");

            Base64_encode(encoded_string+7, (url.user()+":"+url.password()) , len_user + len_password + 1);

            header.setValue( "Authorization", encoded_string);

            delete[]encoded_string;

           }

	}

	//*/

	//.......... contenttype ..........................................................

	header.setContentType(contenttype_string );

        header.setValue("User-Agent", "CCAM_View");

	//.......... Proxy ..........................................................

	if ( proxy_string != "" )

		setHost( proxy_string, atoi(proxy_port) );

	else

		if ( url.hasPort() )

			setHost( url.host(), url.port() );

		else

        		setHost( url.host() );

 return header;

}



//------------------------------------------ CreateSOAPMessage -----------------------------------------------------------

QString CSoapInterface::CreateSOAPMessage(QString &action,

					  QString &message,

					  QString &multiRef,				 

                                          QString ns_string  /* = "\n             xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"

                                                                  "               xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" */,

                                          bool sendRawMess   /* = FALSE */

                                         )

{

	if ( sendRawMess ) return message;

	QString ret;



	// FIXME: hard-coded XML

	ret  = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<SOAP-ENV:Envelope  xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" ";

	ret += ns_string;	

	ret += ">\n";	

	ret +=  "  <SOAP-ENV:Body>\n"

	        "      <"   + action + " SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n"     

		+ message   + 

		"      </"  + action + ">\n"

		+ multiRef  +

		"  </SOAP-ENV:Body>\n"

		"</SOAP-ENV:Envelope>";

	return ret;

}



//------------------------------------------ GotoDebug ------------------------------------------------------------------

void CSoapInterface::GotoDebug()

{printf("Debug");

}



//------------------------------------------ Base64_encode_len ------------------------------------------------------------------

int CSoapInterface::Base64_encode_len(int len)

{ return ((len + 2) / 3 * 4) + 1;

}



//------------------------------------------ Base64_encode -----------------------------------------------------------------------

int CSoapInterface::Base64_encode(char *encoded, const char *str, int len)

{   char             basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    const unsigned char* string = (const unsigned char*)str;

    char                     *p = encoded;

    int i;

    

    for (i = 0; i < len - 2; i += 3)

        {

        *p++ = basis_64[(string[i] >> 2) & 0x3F];

        *p++ = basis_64[((string[i] & 0x3) << 4) | ((int) (string[i + 1] & 0xF0) >> 4)];

        *p++ = basis_64[((string[i + 1] & 0xF) << 2) | ((int) (string[i + 2] & 0xC0) >> 6)];

        *p++ = basis_64[string[i + 2] & 0x3F];

        }

    if (i < len)

       {

        *p++ = basis_64[(string[i] >> 2) & 0x3F];

        if (i == (len - 1))

           {   *p++ = basis_64[((string[i] & 0x3) << 4)];

               *p++ = '=';

           }

        else

           {   *p++ = basis_64[((string[i] & 0x3) << 4) | ((int) (string[i + 1] & 0xF0) >> 4)];

               *p++ = basis_64[((string[i + 1] & 0xF) << 2)];

           }

        *p++ = '=';

       }

    *p++ = '\0';

    return p - encoded;

}



//===============================================================================================================================

//:::::::::::::::::::::::::::::::::::::::::::::   CSoapGenericXMLHandler ::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//===============================================================================================================================



//------------------------------------------ startElement ----------------------------------------------------------------------

bool CSoapGenericXMLHandler::startElement( const QString& , const QString& str_name, const QString& qName, const QXmlAttributes& attributes )

{       QString name     = qName;

        QString attName  = "";

	QString attValue = "";

	if ( attributes.length() > 0 )

	{

		for ( int i = 0 ; i < attributes.length(); i++ )

		{       attName  = attributes.qName( i );

		        attValue = attributes.value( i );

			if (m_WantCar==0) m_WantCar = m_pResult->AddData(str_name, 0, attName,  attValue);			    							

		}

	}

	else

	{ if (m_WantCar==0) m_WantCar = m_pResult->AddData(str_name, 0, attName,  attValue);

	}

	if (m_WantCar==1) 

	   {m_DataName = str_name;

	   }

	return TRUE;

}



//------------------------------------------ characters ----------------------------------------------------------------------

bool CSoapGenericXMLHandler::characters ( const QString & ch )

{       QString blabla  = "#~#ToPlace#~#";        

	QString str     = ch.stripWhiteSpace();



	//..................... si vide on annule tout ..................................................

	if ( str.isEmpty() ) 

	   { m_WantCar  = 0;

	     m_DataName = "";  

	     return TRUE;

	   }

        //..................... si le flag de demande de caractere est positionné .......................

	//                      alors appel de la fonction correspondante

	if (m_WantCar==1) 

	   {m_WantCar  = m_pResult->AddData(m_DataName, str, blabla,  m_DataName); 

	    m_DataName = "";		 

	   }	           

	return TRUE;

}



//------------------------------------------ CSoapGenericXMLHandler ------------------------------------------------------------------

CSoapGenericXMLHandler::CSoapGenericXMLHandler()

{       m_pResult                    = 0;

	m_WantCar                    = 0;

	m_DataName                   = "";

}



//------------------------------------------ startDocument --------------------------------------------------------------------------

bool CSoapGenericXMLHandler::startDocument()

{       return TRUE;

}



//------------------------------------------ startCDATA -----------------------------------------------------------------------------

bool CSoapGenericXMLHandler::startCDATA()

{       return TRUE;

}



//------------------------------------------ endCDATA --------------------------------------------------------------------------------

bool CSoapGenericXMLHandler::endCDATA()

{	return TRUE;

}



//------------------------------------------ comment ----------------------------------------------------------------------------------

bool CSoapGenericXMLHandler::comment( const QString &  )

{	return TRUE;

}

//------------------------------------------ endElement -------------------------------------------------------------------------------

bool CSoapGenericXMLHandler::endElement( const QString&, const QString&, const QString&  )

{       return TRUE;

}



//------------------------------------------ processingInstruction ----------------------------------------------------------------------

bool CSoapGenericXMLHandler::processingInstruction ( const QString & , const QString &  )

{       return TRUE;

}





//===============================================================================================================================

//:::::::::::::::::::::::::::::::::::::::::::::   SOAPSyntaxHighlighter :::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//===============================================================================================================================

#define INDENT "&nbsp;&nbsp;&nbsp;&nbsp;"



//------------------------------------------ SOAPSyntaxHighlighter ------------------------------------------------------------------

SOAPSyntaxHighlighter::SOAPSyntaxHighlighter()

{

	m_Result                    = "";

	m_XMLProcessingMarkupColor  = "darkblue";

	m_XMLElementColor           = "darkcyan";

	m_XMLDataColor              = "black";

	m_XMLDTDColor               = "darkgreen";

	m_XMLCommentColor           = "orange";

	m_XMLAttributeNameColor     = "darkgreen";

	m_XMLAttributeValueColor    = "darkred";

	m_XMLErrorColor             = "red";

	m_XMLCDATATagColor          = "darkgreen";

	m_XMLCDATAColor             = "magenta";

}



//------------------------------------------ encodeXMLEntities ----------------------------------------------------------------------

QString SOAPSyntaxHighlighter::encodeXMLEntities( const QString &s ) const

{

	QString data = s;

	data.replace( "&", "&amp;" ); // This _must_ come first

	data.replace( "<", "&lt;" );

	data.replace( ">", "&gt;" );

	data.replace( "\"", "&quot;" );

	data.replace( "'", "&apos;" );

	return data;

}



//------------------------------------------ startDocument --------------------------------------------------------------------------

bool SOAPSyntaxHighlighter::startDocument()

{

	m_indent          = "";

	m_StartedElement  = false;

	m_inCDATA         = false;

	return TRUE;

}



//------------------------------------------ startCDATA -----------------------------------------------------------------------------

bool SOAPSyntaxHighlighter::startCDATA()

{

	if ( m_StartedElement )

		m_Result += "&gt;";

	m_Result += "<br>" + m_indent + "<font color=\"" + m_XMLCDATATagColor +  "\">&lt;![CDATA[</font>";

	m_indent += INDENT;

	m_StartedElement = false;

	m_inCDATA = true;

	return TRUE;

}



//------------------------------------------ endCDATA --------------------------------------------------------------------------------

bool SOAPSyntaxHighlighter::endCDATA()

{

	m_indent.remove( 0, QString( INDENT ).length() );

	m_Result += "<br>" + m_indent + "<font color=\"" + m_XMLCDATATagColor + "\">]]&gt;</font>";

	m_StartedElement = false;

	m_inCDATA = false;

	return TRUE;

}



//------------------------------------------ comment ----------------------------------------------------------------------------------

bool SOAPSyntaxHighlighter::comment( const QString & ch )

{

	if ( m_StartedElement )

	{

		m_Result += "&gt;";

	}

	// copy because ch ist const

	QString comment = ch;

	comment.replace( "\n", "<br>" + m_indent );

	m_Result += "<br>" + m_indent + "<font color=\"" + m_XMLCommentColor + "\">&lt;!--" + comment + "--&gt;</font>";

	m_StartedElement = false;

	return TRUE;

}



//------------------------------------------ startElement ----------------------------------------------------------------------

bool SOAPSyntaxHighlighter::startElement( const QString& , const QString& , const QString& qName, const QXmlAttributes& attributes )

{       QString name = qName;

	//m_indent += INDENT;

	if ( m_StartedElement )

	{

		m_Result += "&gt;</font>";

	}

	m_Result +="<br>";

	m_Result += m_indent;

	

	m_Result += "<font color=\"" + m_XMLElementColor + "\">&lt;";

	m_Result += qName;

	if ( attributes.length() > 0 )

	{

		for ( int i = 0 ; i < attributes.length(); i++ )

		{       QString attName  = attributes.qName( i );

		        QString attValue = attributes.value( i );

			m_Result += " <font color=\"" + m_XMLAttributeNameColor    + "\">" + attName    + "</font>";

			m_Result += "<font color=\""  + m_XMLProcessingMarkupColor + "\">=</font>";

			m_Result += "<font color=\""  + m_XMLAttributeValueColor   + "\">\"" + attValue + "\"</font>";

		}

	}

	m_indent += INDENT;

	m_StartedElement = true;

	return TRUE;

}



//------------------------------------------ endElement ----------------------------------------------------------------------

bool SOAPSyntaxHighlighter::endElement( const QString&, const QString&, const QString& qName )

{

	m_indent.remove( 0, QString( INDENT ).length() );

	if ( m_StartedElement )

	{

		m_StartedElement = false;

		m_Result += " /&gt;</font>";

		return TRUE;

	}

	//m_Result += (m_IsDataValue?"":"<br>") + m_indent + "<font color=\"" + m_XMLElementColor + "\">&lt;/";

	//m_IsDataValue    = FALSE;

	m_Result += "<br>" + m_indent + "<font color=\"" + m_XMLElementColor + "\">&lt;/";

	m_Result += qName + "&gt;</font>";

	m_StartedElement = false;

	return TRUE;

}



//------------------------------------------ processingInstruction ----------------------------------------------------------------------

bool SOAPSyntaxHighlighter::processingInstruction ( const QString & target, const QString & data )

{       

        m_Result += "<font color=\"" + m_XMLProcessingMarkupColor + "\">&lt;?</font><font color=\"" +  m_XMLDTDColor + "\">";

	m_Result += target + " ";

	m_Result += data;

	m_Result += "</font><font color=\"" + m_XMLProcessingMarkupColor + "\">?&gt;</font>";

	m_StartedElement = false;

        return TRUE;

}



//------------------------------------------ characters ----------------------------------------------------------------------

bool SOAPSyntaxHighlighter::characters ( const QString & ch )

{       /*

	QString str = ch.stripWhiteSpace();

	if ( str.isEmpty() ) return TRUE;



	if ( m_StartedElement )

	{

		m_Result += "&gt;</font>";

	}

	// note that we have to call encodeSGMLEntities twice:

	// once to encode the character string and once because the edit field interpretes

	// this as html ( which would undo the first encoding )

	m_Result += "<br>" + m_indent + "<font color=\""

		 + (m_inCDATA ? m_XMLCDATAColor : m_XMLDataColor) + " \">" 

		 + encodeXMLEntities( encodeXMLEntities(str) ).replace( "\n", "<br>" + m_indent ) + "</font>";

	m_StartedElement = false;

        */

       #define SPACE_MARK 190

        QString str; 

	if  (m_inCDATA==FALSE)

	    {str = ch;

	     str.remove("\n"); 

	     str.remove("\r"); 

	     int   i = 0;

	     int len = str.length();    

	     while (i<len  && (str[i]==' '|| str[i]=='\t')) {str[i]= SPACE_MARK;  ++i; }

	     if (i==len)     return TRUE;

	     --len;                  

	     while(len>=0 && (str[len]==' '|| str[len]=='\t')) {str[len]=SPACE_MARK; --len;}	     	     

	    }

	else

	    {str = ch.stripWhiteSpace();

	     if ( str.isEmpty() ) return TRUE;

	    }

	if ( str.isEmpty() ) return TRUE;



	if ( m_StartedElement )

	{

		m_Result += "&gt;</font>";

	}

	// note that we have to call encodeSGMLEntities twice:

	// once to encode the character string and once because the edit field interpretes

	// this as html ( which would undo the first encoding )

	m_Result +=  "<br>" + m_indent + "<font color=\""

		 + (m_inCDATA ? m_XMLCDATAColor : m_XMLDataColor) + " \">" 

		 + encodeXMLEntities( encodeXMLEntities(str) ).replace( "\n", "<br>" + m_indent ) + "</font>";

	//if (m_inCDATA) m_IsDataValue = FALSE;

	//else           m_IsDataValue = TRUE;

	m_StartedElement = false;

	return TRUE;

}



//------------------------------------------ getResult ----------------------------------------------------------------------

QString SOAPSyntaxHighlighter::getResult()

{

	return m_Result;

}

