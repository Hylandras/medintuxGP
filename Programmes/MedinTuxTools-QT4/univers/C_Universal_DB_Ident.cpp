/***************************** C_Universal_DB_Ident.cpp *******************
*  #include "C_Universal_Ident.h"        QT4-ready                        *
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
#include "C_Universal_DB_Ident.h"
//========================================= C_Universal_DB_Ident ============================================
//------------------------------ C_Universal_DB_Ident --------------------------------------
C_Universal_DB_Ident::C_Universal_DB_Ident( C_PatientBase *pC_PatientBase,
                                            const QString &Guid          ,  /* = "" */
                                            const QString &Usual_name    ,  /* = "Anonymous" */
                                            const QString &Forename         /* =  "" */

                                          )
{init (pC_PatientBase, Guid, Usual_name, Forename);
}
//------------------------------ init --------------------------------------
void C_Universal_DB_Ident::init(     C_PatientBase *pC_PatientBase,
                                     const QString &guid          , /* = "" */
                                     const QString &usual_name    , /* = "Anonymous" */
                                     const QString &forename         /* =  "" */
                               )
{m_pC_PatientBase   = pC_PatientBase;
 m_pk               = "-1";
 QString u_name     = usual_name;
 QString u_forename = forename;
 if (guid.length())
    { set_pk( pC_PatientBase->guid_To_Pk_Name_Forename(guid, &u_name, &u_forename) );  // -1 si n'exite pas
      set_guid(guid);
    }
 else
    { set_guid(pC_PatientBase->GUID_Create()  /* C_MyUuid::create()*/);
    }
 set_usual_name (u_name);
 set_forename   (u_forename);
}
//------------------------------ set_pC_PatientBase -----------------------------------------------
void           C_Universal_DB_Ident::set_pC_PatientBase(C_PatientBase *value)
{ m_pC_PatientBase = value;
}

//------------------------------ get_pC_PatientBase -----------------------------------------------
C_PatientBase* C_Universal_DB_Ident::get_pC_PatientBase(){return m_pC_PatientBase;}

//------------------------------ gBD -----------------------------------------------
C_PatientBase* C_Universal_DB_Ident::gBD(){return m_pC_PatientBase;}


//------------------------------ set_pk -----------------------------------------------
void C_Universal_DB_Ident::set_pk (const QString &value)
{m_pk=value;
}

//------------------------------ get_pk -----------------------------------------------
QString C_Universal_DB_Ident::get_pk ()
{return m_pk;
}


//========================================= C_Human_DB_Ident ============================================
//------------------------------ C_Human_DB_Ident -----------------------------------------------
C_Human_DB_Ident::C_Human_DB_Ident (   C_PatientBase *pC_PatientBase,
                                       const QString &Guid               /* = "" */,
                                       const QString &Usual_name         /* =  "Anonymous" */,
                                       const QString &Forename           /* =  "" */,
                                       const QString &Young_name         /* =  "" */,
                                       const QString &Sexe               /* =  "I" */,
                                       const QString &AgeOrDateOfBirth   /* =  "" */ ,
                                       const QString &DateFormat         /* =  yyyy-MM-dd */)
       : C_Universal_DB_Ident(pC_PatientBase,Guid,Usual_name,Forename)
{
 init(pC_PatientBase, Guid ,Usual_name ,Forename ,Young_name,Sexe,AgeOrDateOfBirth,DateFormat);
}
//------------------------------ init -----------------------------------------------
void C_Human_DB_Ident::init           (C_PatientBase *pC_PatientBase,
                                       const QString &Guid               /* = "" */,
                                       const QString &Usual_name         /* =  "Anonymous" */,
                                       const QString &Forename           /* =  "" */,
                                       const QString &Young_name         /* =  "" */,
                                       const QString &Sexe               /* =  "I" */,
                                       const QString &AgeOrDateOfBirth   /* =  "" */ ,
                                       const QString &DateFormat         /* =  yyyy-MM-dd */)
{C_Universal_DB_Ident::init(pC_PatientBase,Guid, Usual_name, Forename);
 QString dateNss   = AgeOrDateOfBirth;
 QString sexe      = Sexe;
 QString nomJf     = Young_name;
 // lors initialisation C_Universal_Ident(Usual_name, Guid) il se positionne sur un pk valide si Guid deja existant
 // et le nom prenom sont dans Usual_name separes par  ":;:"
 if (get_pk() != "-1")
    {pC_PatientBase->pk_to_BaseIdent(get_pk(),
                                     &dateNss,
                                     &sexe,
                                     &nomJf);
    }
 set_ageOrDateOfBirth (dateNss, DateFormat);
 set_sexe(sexe);
 set_young_name(nomJf);
}