/***************************************************************************
 *   Copyright (C) 2003 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   Rubrique.h                                                      *
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
#ifndef _RUBRIQUES_H
#define _RUBRIQUES_H



//........................ types de rubriques ..........................................................
#define TYP_IDENTITE       1
#define TYP_CHOIX_PATIENT  2
#define TYP_PARAM          5

#define TYP_OBSERVATION    20030000   // +0 à +999 recouvre les observations textuelles
#define TYP_PRESCRIPTION   20020000   // +0 à +999 recouvre toutes les prescriptions (certif + ordo)

//...................... nouveaux sous types ...........................................................
#define TYP_ORDONNANCE     20020200
#define TYP_CERTIFICAT     20020300

#define TYP_URG_OBSERV     20040000


#endif


