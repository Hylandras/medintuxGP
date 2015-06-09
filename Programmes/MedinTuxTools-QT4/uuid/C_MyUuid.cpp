
/********************************* C_MyUuid.cpp ***************************
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

#include "C_MyUuid.h"
#include <QTime>


//----------------------------------------------------- C_MyUuid --------------------------------------------------
C_MyUuid::C_MyUuid()
{
}

//-----------------------------------------------------  create -------------------------------------------------
QString C_MyUuid::create()
{uuid_t out;
 generate_random(out);
 return guidToString((uuid *)out);
}

//-----------------------------------------------------  guidToString -------------------------------------------------
//                               quint32      quint16      quint16                quint16     quint8
//                               time_low      time_mid    time_hi_and_version    clock_seq   node[6];
//                               %08X       -  %04X     -  %04X                 - %04X      - %02X%02X%02X%02X%02X%02X
//
QString C_MyUuid::guidToString(uuid *puuid)
{   QString rString;
    rString.sprintf("%08X-%04X-%04X-%04X-%02X%02X%02X%02X%02X%02X",
                    //   quint32        quint16          quint16                     quint16            quint8
                    //   time_low       time_mid         time_hi_and_version         clock_seq          node[6];
                    //   %08lX     -    %04X     -       %04X                 -      %04X      -        %02X%02X%02X%02X%02X%02X
                    puuid->time_low,    puuid->time_mid, puuid->time_hi_and_version, puuid->clock_seq,  puuid->node[0],puuid->node[1],puuid->node[2],puuid->node[3],puuid->node[4], puuid->node[5]
                   );
    return      rString;
}

//----------------------------------------------------- generate_random --------------------------------------------------
void C_MyUuid::generate_random(uuid_t out )
{
	uuid_t	buf;
	struct uuid uu;
	int i, n=1;

	for (i = 0; i < n; ++i) {
		get_random_bytes(buf, sizeof(buf));
		unpack(buf, &uu);

		uu.clock_seq = (uu.clock_seq & 0x3FFF) | 0x8000;
		uu.time_hi_and_version = (uu.time_hi_and_version & 0x0FFF)
			| 0x4000;
		pack(&uu, out);
		out += sizeof(uuid_t);
	}
}

//-----------------------------------------------------  pack --------------------------------------------------
void C_MyUuid::pack(const struct uuid *uu, uuid_t ptr)
{
	quint32	tmp;
	unsigned char	*out = ptr;

	tmp = uu->time_low;
	out[3] = (unsigned char) tmp;
	tmp >>= 8;
	out[2] = (unsigned char) tmp;
	tmp >>= 8;
	out[1] = (unsigned char) tmp;
	tmp >>= 8;
	out[0] = (unsigned char) tmp;

	tmp = uu->time_mid;
	out[5] = (unsigned char) tmp;
	tmp >>= 8;
	out[4] = (unsigned char) tmp;

	tmp = uu->time_hi_and_version;
	out[7] = (unsigned char) tmp;
	tmp >>= 8;
	out[6] = (unsigned char) tmp;

	tmp = uu->clock_seq;
	out[9] = (unsigned char) tmp;
	tmp >>= 8;
	out[8] = (unsigned char) tmp;

	memcpy(out+10, uu->node, 6);
}

//----------------------------------------------------- unpack --------------------------------------------------
void C_MyUuid::unpack(const uuid_t in, struct uuid *uu)
{
	const quint8	*ptr = in;
	quint32         tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_low = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->node, ptr, 6);
}

//-----------------------------------------------------  get_random_bytes --------------------------------------------------
/*
 * Generate a series of random bytes.
 * version minimale de l'algo uuid (cas ou /dev/random/urandom is out to lunch)
 */
void C_MyUuid::get_random_bytes(void *buf, int nbytes)
{
	int i;
	unsigned char *cp = (unsigned char *) buf;
	//.......... on fait tourner la boite magique ..........
	QTime tm = QTime::currentTime();
	// int m = (tm.second() ^ tm.msec()) & 0x1F;
	// qDebug(QString::number(m));
	for (i = (tm.second() ^ tm.msec()) & 0x1F; i > 0; --i)
	     qrand();
	/*
	 * We do this all the time, but this is the only source of
	 * randomness if /dev/random/urandom is out to lunch.
	 */
	for (cp = (unsigned char *)buf, i = 0; i < nbytes; ++i)
		*cp++ ^= (qrand() >> 7) & 0xFF;

}
