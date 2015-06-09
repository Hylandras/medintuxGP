 /********************************* C_CpsExt.cpp **********************************
 \file C_CpsExt.cpp
 \brief (put some short descripion here).

     #include "C_CpsExt.h"
     Project: MedinTux
     Copyright (C) 2003 2004 2005 2006 2007
     by Sevin Roland  and Data Medical Design
     E-Mail: medintux@medintux.org

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


#include "C_CpsExt.h"

//------------------------------------------------------- C_CpsExt -------------------------------------------
C_CpsExt::C_CpsExt(const QString &codePorteur, const QString &serializeData )
 :   m_pCSesam_API(0)
{ m_CodePorteur = codePorteur;
  if ( serializeData.length() ) UnSerialize(serializeData);
}

//------------------------------------------------------- C_CpsExt -------------------------------------------
C_CpsExt::C_CpsExt( CSesam_API* pCSesam_API , unsigned short session )
 :   m_pCSesam_API(pCSesam_API)
{if ( pCSesam_API )
    {m_CodePorteur = pCSesam_API->m_CodePorteur;
     if (session != 0XFFFF) readCard( session);
    }

}
//------------------------------------------------------- C_CpsExt -------------------------------------------
void C_CpsExt::readCard( unsigned short session, CSesam_API* pCSesam_API)
{            if (pCSesam_API && m_pCSesam_API==0) { m_pCSesam_API = pCSesam_API; m_CodePorteur = pCSesam_API->m_CodePorteur;}
             CMesureTemps     mesureTemps;

             unsigned short   retour;
             unsigned short   ok                     = 1;
             unsigned short  nbEssais                = 3+1;
             unsigned short  iDGroupeSuivant         = 0;
             unsigned short  occurenceGroupeSuivant  = 0;
             unsigned short  ret__LireGroupeSuivant  = 0;

             m_pCSesam_API->toLog("\r\n");
             m_pCSesam_API->toLog(TR("_______ SSV_SIS_LireCartePS _______"));
             mesureTemps.InitTime();
             while (ok)
              {
               retour = m_pCSesam_API->SSV_SIS_LireCartePS(m_pCSesam_API->m_RessPS, m_pCSesam_API->m_RessLecteur, m_pCSesam_API->m_CodePorteur, session);
               if (ok<nbEssais && retour == 512)  { m_pCSesam_API->toLog(TR("SSV_SIS_LireCartePS Essai N° : ") + QString::number(ok ) + TR(" Echec")  );     ok++;}
               else                               { m_pCSesam_API->toLog(TR("SSV_SIS_LireCartePS Essai N° : ") + QString::number(ok ) + TR(" Succès") );     ok=0;}
              }
             //........................... si erreur sortir le message d'erreur .............................
             if (retour)
                {m_Erreur = m_pCSesam_API->SSV_SSV_ErrCodeToString(retour);
                 m_pCSesam_API->toLog(TR("Retour SSV_SIS_LireCartePS : ") + m_Erreur );
                 m_Erreur = m_Erreur.replace("\r\n","<br>");
                 m_Erreur = m_Erreur.replace(","," ");
                 m_Erreur = m_Erreur.replace("["," ");
                 m_Erreur = m_Erreur.replace("]"," ");
                }
             else
                {m_Erreur = "";
                 m_pCSesam_API->toLog(TR("Retour SSV_SIS_LireCartePS : Ok") );
                }

             m_pCSesam_API->toLog(TR("=> Lecture carte CPS en : ")    +  mesureTemps.GetResult());

             mesureTemps.InitTime();
             while (retour==0)
             {ret__LireGroupeSuivant   =  m_pCSesam_API->SGD_SGD_LireGroupeSuivant(session, iDGroupeSuivant, occurenceGroupeSuivant,  &iDGroupeSuivant, &occurenceGroupeSuivant);
              if (ret__LireGroupeSuivant==0xF80B) break;
              if (iDGroupeSuivant==1)
                 { m_pCSesam_API->toLogGroupe ( iDGroupeSuivant , occurenceGroupeSuivant );
                   m_pCSesam_API->LireEtAfficherChamps(7, session, iDGroupeSuivant, occurenceGroupeSuivant,  this);
                 }
              if (iDGroupeSuivant==2)
                 { m_pCSesam_API->toLogGroupe ( iDGroupeSuivant , occurenceGroupeSuivant );
                   m_pCSesam_API->LireEtAfficherChamps(21, session, iDGroupeSuivant, occurenceGroupeSuivant, this);
                 }
             }
             m_pCSesam_API->toLog(TR("=> Lecture Groupes CPS en : ") + mesureTemps.GetResult());
}

