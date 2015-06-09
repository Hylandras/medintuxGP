 /********************************* CDlg_ExportConfig.cpp **********************************
 \file .cpp
 \brief (put some short descripion here).

     #include "CDlg_ExportConfig.h"
     Project: MedinTux
     Copyright (C) 2003 2004 2005 2006 2007
     by Sevin Roland  and Data Medical Design
     E-Mail: medintux@medintux.org

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


#include <qlistbox.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpushbutton.h> 
#include <qfile.h> 
#include <qdir.h> 
#include <qfileinfo.h> 
#include <qfiledialog.h> 
#include <qmessagebox.h> 
#include "CApp.h"
#include "CDlg_ExportConfig.h"
#include "../../MedinTuxTools/Theme.h"

//---------------------------------------  CDlg_ExportConfig --------------------------------------------------------------
CDlg_ExportConfig::CDlg_ExportConfig( QWidget* parent /* = 0*/, const char* name /* = 0 */, bool modal /* = FALSE */, WFlags fl /* = 0 */ )
: Dlg_ExportConfig(parent,name, modal,fl)
{
  pushButton_add_line->setPixmap ( Theme::getIcon("16x16/add.png" ) );
  pushButton_del_line->setPixmap ( Theme::getIcon("16x16/remove.png" ) );

  lineEdit_fileName->setText(decodeVirgule(G_pCApp->m_ExportFileName));
  lineEdit_deb_line->setText(decodeVirgule(G_pCApp->m_ExportDebLine));
  lineEdit_end_line->setText(decodeVirgule(G_pCApp->m_ExportEndLine));
  lineEdit_deb_field->setText(decodeVirgule(G_pCApp->m_ExportDebField));
  lineEdit_end_field->setText(decodeVirgule(G_pCApp->m_ExportEndField));
  lineEdit_nb_results_max->setText(G_pCApp->m_nb_max_results);
  QStringList list;
  CGestIni::Param_GetList(G_pCApp->m_LocalParam, "Export params", "data_" ,  list , 1);
  for (int i=0; i<list.count(); ++i)
      { list[i]= G_pCApp->convertToText( list[i] );
      }
  comboBox_configs->insertStringList ( list );
  connect( pushButton_accept,            SIGNAL( clicked()),                        this,     SLOT( accept()) );
  connect( pushButton_fileNameChoice,    SIGNAL( clicked()),                        this,     SLOT( Slot_fileChoice()) );
  connect( comboBox_configs,             SIGNAL( activated ( const QString &) ),    this,     SLOT( Slot_configs_activated(const QString &)) );
  connect( pushButton_add_line,          SIGNAL( clicked()),                        this,     SLOT( Slot_configs_add_line()) );
  connect( pushButton_del_line,          SIGNAL( clicked()),                        this,     SLOT( Slot_configs_del_line()) );
}

//--------------------------------------- ~CDlg_ExportConfig --------------------------------------------------------------
CDlg_ExportConfig::~CDlg_ExportConfig()
{
}

/*$SPECIALIZATION$*/
//--------------------------------------- reject --------------------------------------------------------------
void CDlg_ExportConfig::reject()
{ QDialog::reject();
}
//--------------------------------------- accept --------------------------------------------------------------
void CDlg_ExportConfig::accept()
{
  
  G_pCApp->m_ExportFileName = lineEdit_fileName->text();
  G_pCApp->m_ExportDebLine  = G_pCApp->convertToIni(codeVirgule(lineEdit_deb_line->text()));
  G_pCApp->m_ExportEndLine  = G_pCApp->convertToIni(codeVirgule(lineEdit_end_line->text()));
  G_pCApp->m_ExportDebField = G_pCApp->convertToIni(codeVirgule(lineEdit_deb_field->text()));
  G_pCApp->m_ExportEndField = G_pCApp->convertToIni(codeVirgule(lineEdit_end_field->text()));
  G_pCApp->m_nb_max_results = lineEdit_nb_results_max->text();

  CGestIni::Param_WriteParam( &G_pCApp->m_LocalParam, "Export config", "Delimiteurs",     G_pCApp->m_ExportDebLine, G_pCApp->m_ExportEndLine,G_pCApp->m_ExportDebField, G_pCApp->m_ExportEndField);
  CGestIni::Param_WriteParam( &G_pCApp->m_LocalParam, "Export config", "File Name",       G_pCApp->m_ExportFileName);
  CGestIni::Param_WriteParam( &G_pCApp->m_LocalParam, "Export config", "nb_max_results",  G_pCApp->m_nb_max_results);

  CGestIni::Param_RemoveSection(G_pCApp->m_LocalParam, "Export params");
  int pos = G_pCApp->m_LocalParam.find("[END_INI]");
  if (pos!=-1)
     { G_pCApp->m_LocalParam.insert(pos,"[Export params]\r\n");
       for ( int index=0; index<comboBox_configs->count(); ++index)
           { QString var      = QString("data_%1").arg(QString::number(index));
             QStringList list = QStringList::split(",",comboBox_configs->text ( index ), true);
             for ( int i=0; i<list.count(); ++i )
                 { list[i]= G_pCApp->convertToIni( list[i] );
                 }
             CGestIni::Param_WriteParam( &G_pCApp->m_LocalParam, "Export params", var, list[0]+","+list[1]+","+list[2]+","+list[3] );
           }
     }

  CGestIni::Param_UpdateToDisk(G_pCApp->m_PathIni, G_pCApp->m_LocalParam);
  QDialog::accept();
}

//--------------------------------------- Slot_configs_add_line --------------------------------------------------------------
void CDlg_ExportConfig::Slot_configs_add_line()
{ QString to_add = codeVirgule(lineEdit_deb_line->text())  +"," +
                   codeVirgule(lineEdit_end_line->text())  +"," +
                   codeVirgule(lineEdit_deb_field->text()) +"," +
                   codeVirgule(lineEdit_end_field->text());
  comboBox_configs->insertItem ( to_add );
  comboBox_configs->setCurrentItem ( comboBox_configs->count()-1 );
}

//--------------------------------------- codeVirgule --------------------------------------------------------------
QString CDlg_ExportConfig::codeVirgule(const QString &text)
{  QString result = text;
   result = result.replace(",","&#44");
   return result;
}
//--------------------------------------- decodeVirgule --------------------------------------------------------------
QString CDlg_ExportConfig::decodeVirgule(const QString &text)
{  QString result = text;
   result = result.replace("&#44",",");
   return result;
}
//--------------------------------------- Slot_configs_del_line --------------------------------------------------------------
void CDlg_ExportConfig::Slot_configs_del_line()
{ comboBox_configs->removeItem ( comboBox_configs->currentItem () );
}
//--------------------------------------- Slot_configs_activated --------------------------------------------------------------
void CDlg_ExportConfig::Slot_configs_activated(const QString &config)
{ QStringList paramList = QStringList::split(",",config,true);
  lineEdit_deb_line->setText(G_pCApp->convertToText(decodeVirgule(paramList[0])));
  lineEdit_end_line->setText(G_pCApp->convertToText(decodeVirgule(paramList[1])));
  lineEdit_deb_field->setText(G_pCApp->convertToText(decodeVirgule(paramList[2])));
  lineEdit_end_field->setText(G_pCApp->convertToText(decodeVirgule(paramList[3])));
}
//--------------------------------------- Slot_fileChoice --------------------------------------------------------------
void CDlg_ExportConfig::Slot_fileChoice()
{   QString                  file = G_pCApp->m_ExportFileName;
    if ( file.length()==0 )  file = G_pCApp->m_PathAppli + "Resultats/export_data.txt";
    if ( QDir(file).isRelative() ) file.prepend(G_pCApp->m_PathAppli);

    QFileDialog *fd = new QFileDialog ( QFileInfo(file).dirPath ( true ),
                                        tr( " Fichiers de texte (*.TXT *.txt *.xml *.XML *.csv *.CSV);;"
                                            " Tous les fichiers (*)"),
                                        this,
                                        "SaveDlg"
                                      ) ;
    if (fd==0)                                             return;
    fd->setMode(QFileDialog::AnyFile );    // selection multiple
    if ( fd->exec() == QDialog::Accepted )
       { QString fn = fd->selectedFile();
         if ( !fn.isEmpty() )
            {  file = CGestIni::AbsoluteToRelativePath(G_pCApp->m_PathAppli, fn);
               if ( file.startsWith("/") )file = file.mid(1);
               lineEdit_fileName->setText(file);
            }
       }
    delete fd;
}

