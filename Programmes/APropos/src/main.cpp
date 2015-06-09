
/********************************* main.cpp **************************************
* ...............................................................................*
*   Project   :  MedinTux  (typographie made in france)                          *
*   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
*   by        :  Sevin Roland     from   MedinTux project                        *
*   E-Mail    : roland-sevin@medintux.org                                        *
*   Web site  : www.medintux.org                                                 *
* ...............................................................................*
*                                                                                *
*   This program is free software; you can redistribute it and/or modify         *
*   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
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
#include <QtGui>

#if QT_VERSION >= 0x040890    // Qt 4.1.2, the QT_VERSION macro will expand to 0x040102.
    #include <QtWidgets/QApplication>
    #include <QtWidgets/QMessageBox>
#else
    #include <QApplication>
    #include <QMessageBox>
#endif

#include <QtCore>
#include <QDir>
#include <QFileInfo>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_DlgApropos/C_DlgApropos.h"
#include "CApp.h"

// la macro NAME_APPLI est definie dans le projet

int main(int argc, char *argv[])
{   CApp a(NAME_APPLI, argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);
    QTranslator myappTranslator;
    myappTranslator.load(QString("APropos_")+ QLocale::system().name(),G_pCApp->getPathAppli()+"Ressources/lang");
    a.installTranslator(&myappTranslator);

    if (a.m_ModuleName.indexOf("MailOnly") !=-1 )
       {if (!a.m_Description.startsWith("mailto:")) a.m_Description = a.m_Description.prepend("mailto:");
        C_DlgApropos::URL_ExecSpecialSyntax ( a.m_Description , a.m_PathAppli);
        return 0;
       }
    else if (a.m_ModuleName.toUpper().indexOf("TOPDF") !=-1 )
    {

      QString dest = "";
      //if (argc>3) dest = a.m_Version.trimmed();

      #ifdef Q_WS_X11
          QFileInfo fi(a.m_Description);
          dest = ""; //G_pCApp->m_Version.trimmed();
          if (dest.length()==0) dest = fi.path()+"/"+fi.baseName()+".pdf";                // name = "archive.tar.gz"
          if (QFile::exists(dest)) QFile::remove(dest);

          C_DlgApropos::toPDF(a.m_Description, dest);
          return 0; //a.exec();
      #endif

    }
    // qDebug()<<a.m_PathHelp;
    C_DlgApropos w(0,
                   a.m_PathAppli,
                   a.m_ModuleName,      // verbe :  MailOnly     BrowserOnly       toPDF
                   a.m_Description,     //               url     url               url
                   a.m_Version,         //           subject                       fileDest
                   a.m_PathChangements, //              body                       printerName
                   a.m_PathIcone,
                   a.m_PathHelp,
                   a.m_PathApropos,
                   a.m_BaseVersion);
     w.show();
    return a.exec();

}
