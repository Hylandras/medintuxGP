/**************************** SaveRequest.h ********************************
 *   Copyright (C) 2004 by Sevin Roland                                    *
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
#ifndef SAV_REQUEST_H
#define SAV_REQUEST_H


class CSaveRequest
{
public:
    enum SaveRequest 
         {   NoSaveCurrentRequest   = 0x0002,
	     NoSaveALLRequest       = 0x0003,
             AbortRequest           = 0x0004,
             SaveCurrentRequest     = 0x0000,
	     SaveALLRequest         = 0x0001 
         };
};

#endif

