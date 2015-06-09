
/********************************* CGenTools.h ***********************************
 *                                                                                *
 *   #include "CGenTools.h"                                                       *
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


#ifndef  CGEN_TOOLS_H
#define  CGEN_TOOLS_H

#include <qobject.h>
#include <qwidget.h>
#include <qstring.h>


#define DEB_PAR              "<br />"
#define END_PAR              ""

#define ASCII_DEB_PAR        "\r\n"
#define ASCII_END_PAR        ""
#define NB_SP_INDENT         2
#define CR_TO_HTML           1
class QDateTime;
class QDate;
class QTime;
//===================================== CGenTools ========================================================================
/*! \brief miscellanous tools for MedinTux.
 *
 *  - this class is here, for backward compatibility whith old MedinTux sources:
 *   *         -# alls methodes are static.
 */

class CGenTools
{

  //............................... METHODES .........................................................................

 public:

  //................................... configuration et connection de la base de données ..............................
 CGenTools(){};
~CGenTools(){};
static       int          DialogGetString(QWidget * parent, const char* title, QString &string, int x=0, int y=0);
static       int          MessageAlert(QWidget * parent, const char* title, const char* message=0, const char* button1=0, const char* button2=0,const char* button3=0,const char* button4=0);
static       int          MyMessageAlert(QWidget * parent, const char* title, const QString& mimeSourceFactoryPath, const char* message=0, const char* button1=0, const char* button2=0,const char* button3=0,const char* button4=0);
static       QString      qstringLoad(const char *path, int flag=0, long *nb_ligne=0);
static       int          Convert_CRLF_To_BR(const char *text, QString &dstQStr, int flag =0);
static       void         FileToQString(const char *path, QString  &dst, int mode  = 1 , int htmlHeader  = 0 );
static       QString      daysTo(const QString &date1, const QString &date2);
static       QString      NormaliseDate(const QString &date);
static       QString      GetExtension(QString &file, int cut_to  = -1 );
static       int          NbLignes(const char *txt);
static       QDateTime    setQDateTime(const QString & dateTime , int &isValid);
static       QDate        setDate(const QString & date ,  int &isValid);
static       QTime        setTime(const QString & heure , int &isValid);
static       int          GetXMLValue(const QString &dataToExamine, const QString &xmlToken_in, QString &valueToGet, int pos , QString *p_LastErr = 0  );
//......................................... DATA ...............................................

};

#endif

