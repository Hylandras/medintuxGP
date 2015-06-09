/***************************** C_Universal_Ident.h ************************
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

#ifndef C_UNIVERSAL_IDENT_H
#define C_UNIVERSAL_IDENT_H
#include<QString>
//========================== C_Universal_Ident ========================================================
/*! \class  C_Universal_Ident
 *  \brief  one universal thing must be have a unique Id and designation and falcutative designation name
 */

class C_Universal_Ident
{
  public:
    C_Universal_Ident( const QString &guid       = "",
                       const QString &usual_name = "Anonymous",
                       const QString &forename   = ""
                     );
    void init( const QString &guid       = ""          ,
               const QString &usual_name = "Anonymous" ,
               const QString &forename   = ""
             );
    void           set_guid (const QString &value);
    QString        get_guid ();
    void           set_usual_name (const QString &value);
    QString        get_usual_name();
    void           set_forename (const QString &value);
    QString        get_forename();

  private:
    QString        m_guid;
    QString        m_usual_name;
    QString        m_forename;

};
#endif // C_UNIVERSAL_IDENT_H
