/********************************** structureparser.h ****************************
 *                                                                                *
 *   #include "structureparser.h"                                                 *
 *   Project: MedinTux    and Data Medical Design                                 *
 *   Copyright (C) 2003 2004 2005 by Sébastien SIVERA & Sevin Roland              *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
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



#ifndef STRUCTUREPARSER_H

#define STRUCTUREPARSER_H



#include <qxml.h>



class QString;

//========================================== StructureParser ==================================================

class StructureParser : public QXmlDefaultHandler

{

public:

    bool startDocument();

    bool startElement( const QString&, const QString&, const QString& ,  const QXmlAttributes& );

    bool endElement(   const QString&, const QString&, const QString& );

    bool processingInstruction ( const QString & target, const QString & data );

    bool caracters ( const QString & ch );

private:

    QString m_indent;

    int     m_truc;

};





//========================================== StructureLexical ==================================================

class StructureLexical : public QXmlLexicalHandler

{

public:

    bool startDTD ( const QString & name, const QString & publicId, const QString & systemId );

    bool endDTD ();

    bool startEntity ( const QString & name );

    bool endEntity ( const QString & name );

    bool startCDATA ();

    bool endCDATA ();

    bool comment ( const QString & ch );

    QString errorString ();

private:

    int     m_truc;

};

#endif



