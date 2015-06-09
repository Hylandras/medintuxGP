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
#include <QString>

//========================================= C_Universal_Ident ============================================
//------------------------------ C_Universal_Ident --------------------------------------
C_Universal_Ident::C_Universal_Ident(const QString &guid         , /* = "" */
                                     const QString &usual_name   , /* = "Anonymous" */
                                     const QString &forename       /* =  "" */
                                    )
{ init (guid, usual_name, forename);
}
//------------------------------ init --------------------------------------
void C_Universal_Ident::init(        const QString &guid         , /* = "" */
                                     const QString &usual_name   , /* = "Anonymous" */
                                     const QString &forename       /* =  "" */
                            )
{ set_guid       (guid);
  set_usual_name (usual_name);
  set_forename   (forename);
}
//------------------------------ set_guid -----------------------------------------------
void C_Universal_Ident::set_guid (const QString &value)
{m_guid=value;
}
//------------------------------ get_guid -----------------------------------------------
QString C_Universal_Ident::get_guid ()
{return m_guid;
}
//------------------------------ set_usual_name -----------------------------------------------
void C_Universal_Ident::set_usual_name (const QString &value)
{m_usual_name=value;
}
//------------------------------ get_usual_name -----------------------------------------------
QString C_Universal_Ident::get_usual_name ()
{return m_usual_name;
}
//------------------------------ set_forename -----------------------------------------------
void    C_Universal_Ident::set_forename (const QString &value)
{m_forename=value;
}
//------------------------------ get_forename -----------------------------------------------
QString C_Universal_Ident::get_forename()
{return m_forename;
}
