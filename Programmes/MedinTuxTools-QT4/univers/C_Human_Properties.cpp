/***************************** C_Universal_Ident.cpp **********************
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

#include "C_Universal_Ident.h"
#include "C_Human_Properties.h"
#include <QString>
#include <QDate>
#include <QObject>
// #include "../uuid/C_MyUuid.h"
#include "../univers/C_DateTools.h"
// #include "../C_PatientBase/C_PatientBase.h"

//========================================= C_Human_Properties ============================================
//------------------------------ C_Human_Properties -----------------------------------------------
C_Human_Properties::C_Human_Properties ( const QString &Young_name         /* =  "" */,
                                         const QString &Sexe               /* =  "I" */,
                                         const QString &AgeOrDateOfBirth   /* =  "" */ ,
                                         const QString &DateFormat         /* =  yyyy-MM-dd */)
{
 init( Young_name,Sexe,AgeOrDateOfBirth,DateFormat);
}
//------------------------------ init -----------------------------------------------
void C_Human_Properties::init           (   const QString &Young_name         /* =  "" */,
                                            const QString &Sexe               /* =  "I" */,
                                            const QString &AgeOrDateOfBirth   /* =  "" */ ,
                                            const QString &DateFormat         /* =  yyyy-MM-dd */)
{set_young_name(Young_name);
 set_sexe(Sexe);
 set_ageOrDateOfBirth (AgeOrDateOfBirth, DateFormat);
}

//------------------------------ set_young_name -----------------------------------------------
void    C_Human_Properties::set_young_name (const QString &value)
{m_young_name=value;
}
//------------------------------ get_young_name -----------------------------------------------
QString C_Human_Properties::get_young_name()
{return m_young_name;
}
//------------------------------ set_sexe -----------------------------------------------
void    C_Human_Properties::set_sexe (const QString &value)
{m_sexe=value;
}
//------------------------------ get_sexe -----------------------------------------------
QString C_Human_Properties::get_sexe()
{return m_sexe;
}

//------------------------------ set_ageOrDateOfBirth -----------------------------------------------
void    C_Human_Properties::set_ageOrDateOfBirth (const QDate &dateOfBirth)
{ m_dateOfBirth   = dateOfBirth;
  m_age           = C_DateTools::CalculeDiffAge(m_dateOfBirth);
}
//------------------------------ set_ageOrDateOfBirth -----------------------------------------------
void    C_Human_Properties::set_ageOrDateOfBirth (const QString &ageOrDateOfBirth, const QString &dateFormat)
{if (ageOrDateOfBirth.length()<=3)        //age  de 0 a 999
    { m_age         = ageOrDateOfBirth;
      m_dateOfBirth = QDate ();        //==>  m_dateOfBirth is invalid CAD just m_age must be to used
    }
 else
    { set_ageOrDateOfBirth (QDate::fromString(ageOrDateOfBirth,dateFormat));
    }
}
//------------------------------ get_dateOfBirth -----------------------------------------------
QDate   C_Human_Properties::get_dateOfBirth()
{return m_dateOfBirth;
}
//------------------------------ get_dateOfBirth -----------------------------------------------
QString C_Human_Properties::get_dateOfBirth(const QString& dateFormat)
{if (m_dateOfBirth.isValid()) return m_dateOfBirth.toString(dateFormat);
 return QString("");
}
//------------------------------ set_age -----------------------------------------------
QString C_Human_Properties::set_age(const QString &age, int keepOldAgeIfDateValid)
{   if (C_DateTools::CalculeDiffAge(m_dateOfBirth) == age) return QObject::tr("");
    if (keepOldAgeIfDateValid==1)
       {m_age = C_DateTools::CalculeDiffAge(m_dateOfBirth);
        return QObject::tr("keep age already defined, because date of birth is valid and keepOldAgeIfDateValid==1");
       }
    m_age = age;
    if (m_dateOfBirth.isValid())
       {m_dateOfBirth = QDate ();
        return QObject::tr("invalidate date of birth already defined, because age not corresponds and keepOldAgeIfDateValid==0");
       }
    m_dateOfBirth = QDate ();        //==>  m_dateOfBirth is invalid CAD just m_age must be to used
    return QObject::tr("change age, date of birth was already invalid and keepOldAgeIfDateValid==0");
}

//------------------------------ get_age -----------------------------------------------
QString C_Human_Properties::get_age(int type /* = 0  */  )
{   if (type) return C_DateTools::CalculeDiffAge(m_dateOfBirth,false, 3);       // 545  (12*45 + 12)
    else      return m_age;                                                     // 45 ans et 5 mois
}

//========================================= C_Human_Ident ============================================
//------------------------------ C_Human_Ident -----------------------------------------------
C_Human_Ident::C_Human_Ident (           //............. C_Universal_Ident ....................
                                         const QString &Guid              , /* = "" */
                                         const QString &Usual_name        , /* = "Anonymous" */
                                         const QString &Forename          , /* =  "" */
                                         //............. C_Human_Properties ....................
                                         const QString &Young_name        , /* =  "" */
                                         const QString &Sexe              , /* =  "I" */
                                         const QString &AgeOrDateOfBirth  , /* =  "" */
                                         const QString &DateFormat          /* =  yyyy-MM-dd */)
{
 init(Guid, Usual_name, Forename ,Young_name,Sexe,AgeOrDateOfBirth,DateFormat);
}
//------------------------------ init -----------------------------------------------
void C_Human_Ident::init                  (   //............. C_Universal_Ident ....................
                                            const QString &Guid              , /* = "" */
                                            const QString &Usual_name        , /* = "Anonymous" */
                                            const QString &Forename          , /* =  "" */
                                            //............. C_Human_Properties ....................
                                            const QString &Young_name        , /* =  "" */
                                            const QString &Sexe              , /* =  "I" */
                                            const QString &AgeOrDateOfBirth  , /* =  "" */
                                            const QString &DateFormat          /* =  yyyy-MM-dd */
                                          )
{   C_Universal_Ident::init(  Guid, Usual_name, Forename);
    C_Human_Properties::init( Young_name, Sexe, AgeOrDateOfBirth, DateFormat);
}



