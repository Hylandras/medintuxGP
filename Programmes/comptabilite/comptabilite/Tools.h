 /********************************* Tools.h ************************************
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                     *
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
/*! \file Tools.h
    \brief (put some short descripion here).

     #include "Tools.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007
     by Eric Maeker , eric.maeker@free.fr
*/


#ifndef TOOLS_H
#define TOOLS_H

#include <qstringlist.h>



/**
	@author Eric Maeker <eric.maeker@free.fr>
*/
class Tools
{

public:

    Tools();

    ~Tools();

		//////////////////////////////////////////
		//		METHODES		//
		//////////////////////////////////////////

public :

		//////////////////////////////////////////
		//		HTML TOOLS		//
		//////////////////////////////////////////

	bool	createHTMLTable		(QStringList& hd, QStringList& lines, QString& retour, 
					 int padding, int spacing);

	bool	getIntoRemarque		(const char* champs_Remarque, const char* toFind, QString& value);



};

#endif
