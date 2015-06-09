
/********************************* C_MyUuid.h *****************************
*  #include "C_MyUuid.h"              QT4-ready                           *
* ........................................................................*
*   Project   :  MedinTux  (typographie made in france)                   *
*   Copyright : (C) 2005-2006-2007-2008-2009-2010 and for the eternity    *
*   by        :  Sevin Roland     from   MedinTux project                 *
*   E-Mail    : roland-sevin@medintux.org                                 *
*   Web site  : www.medintux.org                                          *
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
***************************************************************************
* NOTE : this is a verry simplified version (only ramdom) of original     *
*        uuid lib from  e2fsprogs project                                 *
*        http://e2fsprogs.sourceforge.net                                 *
***************************************************************************/

#ifndef C_MYUUID_H
#define C_MYUUID_H

#include <QString>


struct uuid {
	quint32    time_low;
	quint16    time_mid;
	quint16    time_hi_and_version;
	quint16    clock_seq;
	quint8     node[6];
};

typedef unsigned char uuid_t[16];

class C_MyUuid
{
public:
    C_MyUuid();
    static QString create();
private:
    static QString guidToString(uuid *puuid);
    static void    pack(const struct uuid *uu, uuid_t ptr);
    static void    generate_random(uuid_t out);
    static void    unpack(const uuid_t in, struct uuid *uu);
    static void    get_random_bytes(void *buf, int nbytes);
};

#endif // C_MYUUID_H
