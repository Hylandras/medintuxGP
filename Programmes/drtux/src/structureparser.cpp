/********************************* structureparser.cpp ****************************
 *                                                                                *
 *   #include "structureparser.h"                                                 *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
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



#include "structureparser.h"



#include <stdio.h>

#include <qstring.h>



//========================================== StructureParser ==================================================



//--------------------------------------------------  startDocument -------------------------------------------

bool StructureParser::startDocument()

{

    m_indent = "";

    return TRUE;

}



//--------------------------------------------------  processingInstruction -------------------------------------------

bool StructureParser::processingInstruction ( const QString & target, const QString & data )

{if (target!="" && data != "") 

    {printf( "%s"  "%s\n", (const char*)target, (const char*)data );

    }

 return TRUE;   

}



//--------------------------------------------------  caracters -------------------------------------------

bool StructureParser::caracters ( const QString & /*ch*/ )

{

 return TRUE;   

}



//--------------------------------------------------  startElement -------------------------------------------

bool StructureParser::startElement( const QString &/*namespaceURI*/,

				    const QString &/*localName*/,

                                    const QString &qName,

                                    const QXmlAttributes &atts )

{   int       i = 0;

    QString uri = "";

    int nb_index = atts.count();

    for (i=0; i<nb_index; i++)

       {

	uri  = atts.uri(i);

       }

    printf( "%s%s\n", (const char*)m_indent, (const char*)qName );

    m_indent += "    ";

    return TRUE;

}



//--------------------------------------------------  endElement -------------------------------------------

bool StructureParser::endElement( const QString &/*namespaceURI*/, const QString &/*localName*/, const QString &/*qName*/ )

{   m_indent.remove( 0, 4 );

    return TRUE;

}



//========================================== StructureLexical ==================================================

//--------------------------------------------------  startDTD -------------------------------------------

bool StructureLexical::startDTD (const QString & /*name*/, const QString & /*publicId*/, const QString & /*systemId*/ )

{return TRUE;

}



//--------------------------------------------------  endDTD -------------------------------------------

bool StructureLexical::endDTD ()

{return TRUE;

}



//--------------------------------------------------  startEntity -------------------------------------------

bool StructureLexical::startEntity ( const QString & name )

{if (name != "") 

    {m_truc++;

    }

 return TRUE;

}



//--------------------------------------------------  endEntity -------------------------------------------

bool StructureLexical::endEntity ( const QString & name )

{if (name != "") 

    {m_truc++;

    }

 return TRUE;

}





//--------------------------------------------------  startCDATA -------------------------------------------

bool StructureLexical::startCDATA ()

{return TRUE;

}



//--------------------------------------------------  endCDATA -------------------------------------------

bool StructureLexical::endCDATA ()

{return TRUE;

}



//--------------------------------------------------  comment -------------------------------------------

bool StructureLexical::comment ( const QString & /*ch*/ )

{return TRUE;

}



//--------------------------------------------------  errorString -------------------------------------------

QString StructureLexical::errorString ()

{return QString::null;

}





