 /********************************* .cpp **********************************
 \file .cpp
 \brief (put some short descripion here).

     #include ".h"
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
#include <qfiledialog.h>
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qapplication.h>
#include <qcursor.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qinputdialog.h>
#include <qfile.h>

#include "C_Dlg_MainDialog.h"
#include "../../MedinTuxTools/CGestIni.h"

//----------------------------------------- C_Dlg_MainDialog ---------------------------------------------
C_Dlg_MainDialog::C_Dlg_MainDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
: Dlg_MainDialog(parent,name, modal,fl)
{ QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );

 //.......... un peu de geolocalisation ....................
 QFileInfo qfi(qApp->argv()[0]);
 m_PathAppli       = CGestIni::Construct_PathBin_Module("MrPropre",  qfi.dirPath (true));
 m_PathProfils     = m_PathAppli+"Profils/";
 //.......... recuperer parametres .........................
 CGestIni::Param_UpdateFromDisk(m_PathAppli+"MrPropre.ini",  m_Param );
 QString profil    = CGestIni::Param_ReadUniqueParam(m_Param, "Configuration","Profil a utiliser");
 //.................... initialiser la combo box des profils .............................................................
 QStringList listProfils = CGestIni::listDirectory(m_PathProfils, "cfg", "", ";",TRUE);
 comboBoxProfils->insertStringList (listProfils );
 setComboOnProfil(profil);                // le faire avant les connexions
 setInterfaceOnProfil(profil);            // oblige car si combo sur item zero (etat initial) et setComboOnProfil idem alors pas de SIGNAL
 //.......... connecter le bouzin aux slots ................
 connect( pushButton_Quitter,    SIGNAL( clicked()),                   this,    SLOT  ( Slot_On_pushButton_Quitter())  );
 connect( pushButton_Effacer,    SIGNAL( clicked()),                   this,    SLOT  ( Slot_On_pushButton_Effacer()) );
 connect( pushButton_find,       SIGNAL( clicked()),                   this,    SLOT  ( Slot_On_pushButton_find())  );
 connect( pushButtonAddProfil,   SIGNAL( clicked()),                   this,    SLOT  ( Slot_On_pushButtonAddProfil())  );
 connect( pushButtonDelProfil,   SIGNAL( clicked()),                   this,    SLOT  ( Slot_On_pushButtonDelProfil()) );
 connect( pushButtonSaveProfil,  SIGNAL( clicked()),                   this,    SLOT  ( Slot_On_pushButtonSaveProfil()) );
 connect( comboBoxProfils,       SIGNAL( activated(const QString &)),  this,    SLOT  ( Slot_On_comboBoxProfils_activated(const QString &))  );
 connect( comboBoxProfils,       SIGNAL( highlighted(const QString &)),this,    SLOT  ( Slot_On_comboBoxProfils_highlighted(const QString &))  );
}

//----------------------------------------- ~C_Dlg_MainDialog ---------------------------------------------
C_Dlg_MainDialog::~C_Dlg_MainDialog()
{QString text =  comboBoxProfils->currentText ();
 CGestIni::Param_WriteParam(&m_Param, "Configuration" , "Profil a utiliser" , text);
 CGestIni::Param_UpdateToDisk(m_PathAppli+"MrPropre.ini",  m_Param );
}

/*$SPECIALIZATION$*/
//----------------------------------------- saveProfil ---------------------------------------------
void C_Dlg_MainDialog::saveProfil(const QString & profil)
{//.......... enregistrer parametres ................
 QString dirList    = lineEdit_repToErase->text().replace(";","|");
 QString fileMotif  = lineEdit_fileToErase->text().replace(";","|");
 QString targetDir  = lineEdit_repWhereErase->text();
 QString profilData = "";
 CGestIni::Param_WriteParam(&profilData,"Configuration","Liste des repertoires a effacer",  dirList);
 CGestIni::Param_WriteParam(&profilData,"Configuration","Motif des fichiers a effacer",     fileMotif);
 CGestIni::Param_WriteParam(&profilData,"Configuration","Repertoire cible",                 targetDir);
 CGestIni::Param_UpdateToDisk(m_PathProfils+profil+".cfg",  profilData );
}

//----------------------------------------- setComboOnProfil ---------------------------------------------
void C_Dlg_MainDialog::setComboOnProfil(const QString & profil)
{QListBox         *pQListBox =  comboBoxProfils->listBox();
 QListBoxItem *pQListBoxItem = pQListBox->findItem ( profil, Qt::ExactMatch);
 if (pQListBoxItem)
    {int index = pQListBox->index ( pQListBoxItem );
     comboBoxProfils->setCurrentItem ( index );
    }
}

//----------------------------------------- Slot_On_pushButtonSaveProfil ---------------------------------------------
void C_Dlg_MainDialog::Slot_On_pushButtonSaveProfil()
{QString profil =  comboBoxProfils->currentText ();
 saveProfil(profil);
}

//----------------------------------------- Slot_On_pushButtonAddProfil ---------------------------------------------
void C_Dlg_MainDialog::Slot_On_pushButtonAddProfil()
{bool ok;
 QString profil = QInputDialog::getText(  "Danger", tr("Nom de cette configuration :"), QLineEdit::Normal,  QString::null, &ok, this );
 if ( ok && !profil.isEmpty() )
    {QListBox         *pQListBox =  comboBoxProfils->listBox();
     QListBoxItem *pQListBoxItem = pQListBox->findItem ( profil, Qt::ExactMatch);
     if (pQListBoxItem)
        { QMessageBox::information( this, tr (" ATTENTION profil déjà existant"),
                                              tr ( "<b><u>ATTENTION</b></u> ! Le profil : <b>'%1'</b> existe déjà,"
                                                   " veuillez recommencer en choisissant un autre nom.").arg (profil) ,
                                              tr ("&Annuler cette bétise"), 0, 0 /*tr("Annuler")*/,
                                             0, 1 );
          return;
        }
     saveProfil(profil);
     comboBoxProfils->insertItem (profil );
     setComboOnProfil(profil);
    }
}

//----------------------------------------- Slot_On_pushButtonDelProfil ---------------------------------------------
void C_Dlg_MainDialog::Slot_On_pushButtonDelProfil()
{QString profil =  comboBoxProfils->currentText ();
 switch( QMessageBox::information( this, tr (" ATTENTION effacement d'un profil de nettoyage"),
                                              tr ( "<b><u>ATTENTION</b></u> ! Faut-il vraiment effacer : <b>'%1'</b>"
                                                   " ce superbe profil de nettoyage ? ").arg (profil) ,
                                              tr ("&Annuler cette bétise"), tr("Au diable cette ordure"), 0 /*tr("Annuler")*/,
                                             0, 1 ) )
            {case 0: return;
             case 1: {QFile::remove ( m_PathProfils+profil+".cfg" );
                      QStringList listProfils = CGestIni::listDirectory(m_PathProfils, "cfg", "", ";",TRUE);
                      comboBoxProfils->clear();
                      comboBoxProfils->insertStringList (listProfils );
                      comboBoxProfils->setCurrentItem ( 0) ;
                      profil =  comboBoxProfils->currentText ();
                      setComboOnProfil(profil);                // le faire avant les connexions
                      setInterfaceOnProfil(profil);            // oblige car si combo sur item zero (etat initial) et setComboOnProfil idem alors pas de SIGNAL
                     }
            }

}

//----------------------------------------- Slot_On_comboBoxProfils_highlighted ---------------------------------------------
void C_Dlg_MainDialog::Slot_On_comboBoxProfils_highlighted(const QString &profil)
{setInterfaceOnProfil(profil);
}

//----------------------------------------- Slot_On_comboBoxProfils_activated ---------------------------------------------
void C_Dlg_MainDialog::Slot_On_comboBoxProfils_activated(const QString &profil)
{setInterfaceOnProfil(profil);
}

//----------------------------------------- setInterfaceOnProfil ---------------------------------------------
void C_Dlg_MainDialog::setInterfaceOnProfil(QString profil)
{ //.......... recuperer parametres ................
 QString profilData = "";
 CGestIni::Param_UpdateFromDisk(m_PathProfils+profil+".cfg",  profilData );
 QString dirList    = CGestIni::Param_ReadUniqueParam(profilData, "Configuration","Liste des repertoires a effacer");
 dirList            = dirList.replace("|",";");
 QString fileMotif  = CGestIni::Param_ReadUniqueParam(profilData, "Configuration","Motif des fichiers a effacer");
 fileMotif          = fileMotif.replace("|",";");
 QString targetDir  = CGestIni::Param_ReadUniqueParam(profilData, "Configuration","Repertoire cible");
 //.......... placer les parametres ................
 if (dirList.length()==0)   dirList   = "cvs;CVS;.svn;.SVN;";
 if (fileMotif.length()==0) fileMotif = "~";
 lineEdit_repToErase->setText(dirList);
 lineEdit_fileToErase->setText(fileMotif);
 lineEdit_repWhereErase->setText(targetDir);
}

//----------------------------------------- Slot_On_pushButton_Quitter ---------------------------------------------
void C_Dlg_MainDialog::Slot_On_pushButton_Quitter()
{accept();
}

//----------------------------------------- Slot_On_pushButton_find ---------------------------------------------
void C_Dlg_MainDialog::Slot_On_pushButton_find()
{ QString s = QFileDialog::getExistingDirectory(
                    "/home",
                    this,
                    "get existing directory",
                    tr("choisissez le répertoire cible"),
                    TRUE );
  if (s.length()) lineEdit_repWhereErase->setText(s);
  Slot_On_pushButtonSaveProfil();
}

//----------------------------------------- Slot_On_pushButton_Effacer ---------------------------------------------
void C_Dlg_MainDialog::Slot_On_pushButton_Effacer()
{ int               isAlerted = 0;
  QStringList    fileToRmList = QStringList::split(";",lineEdit_repToErase->text());
  if (fileToRmList.findIndex("src")!=-1)
     {switch( QMessageBox::information( this, tr (" ATTENTION effacement possible de fichiers sources"),
                                              tr ( "<b><u>ATTENTION</b></u> ! La liste des répertoires à effacer "
                                                  "semble indiquer qu'il faille effacer des répertoires sources : <b>'src'</b>") ,
                                              tr ("&Annuler cette bétise"), tr("Au diable les répertoires sources"), 0 /*tr("Annuler")*/,
                                             0, 1 ) )
            {case 0: return;
            }
      isAlerted = 1;
     }
  
  if (fileToRmList.findIndex("*.pro")!=-1)
     {switch( QMessageBox::information( this, tr (" ATTENTION effacement possible de fichiers sources"),
                                              tr ( "<b><u>ATTENTION</b></u> ! La liste des motifs de fichiers à effacer "
                                                  "semble indiquer qu'il faille effacer des fichiers : <b>'*.pro'</b>") ,
                                              tr ("&Annuler cette bétise"), tr("Au diable les répertoires sources"), 0 /*tr("Annuler")*/,
                                             0, 1 ) )
            {case 0: return;
            }
      isAlerted = 1;
     }
  if (isAlerted==0)
     {switch( QMessageBox::information( this, tr (" ATTENTION effacement de fichiers et répertoires imminent"),
                                              tr ( "<b><u>NETTOYAGE A HAUT RISQUE : PAS DE REGRETS</u> ? </b>") ,
                                              tr ("&Annuler cette bétise"), tr("Au diable tous ces fichiers"), 0 /*tr("Annuler")*/,
                                             0, 1 ) )
            {case 0: return;
            }
     }

  QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
  Test_And_RemoveDir(lineEdit_repWhereErase->text(),  lineEdit_repToErase->text(), lineEdit_fileToErase->text() );
  QApplication::restoreOverrideCursor();
}

//----------------------------------------- Test_And_RemoveDir ---------------------------------------------
void C_Dlg_MainDialog::Test_And_RemoveDir(QString src_Dir,  QString dirList_To_remove,  QString motifList_To_Erase)
{
    //........................ explorer la source .......................................
    if (QDir(src_Dir).isRelative()) src_Dir.prepend(m_PathAppli);
    src_Dir = QDir::cleanDirPath(src_Dir);
    QDir cur_dir( src_Dir );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks | QDir::Hidden );
    QStringList         dirToRmList  = QStringList::split(";",dirList_To_remove);
    QStringList         fileToRmList = QStringList::split(";",motifList_To_Erase);
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {
        QString fname = fi->fileName().latin1();
        if      (fname == "..")
                {
                }
        else if (fi->isDir() && fname != "." )
                {if (isThisDirMustBeErase(fname, dirToRmList))
                    EraseDirectory(src_Dir + "/" + fname);
                 else
                    Test_And_RemoveDir( src_Dir + "/" + fname, dirList_To_remove, motifList_To_Erase);
                }
        else if (fi->isFile() && isThisFileMustBeErase(fname, fileToRmList))
                { cur_dir.remove(src_Dir + "/" + fname);
                }
        ++it;
    }
}
//----------------------------------------- isThisDirMustBeErase ---------------------------------------------
bool C_Dlg_MainDialog::isThisDirMustBeErase( const QString &dname, const QStringList &dirToRmList)
{ 
  for (int i=0; i< (int)dirToRmList.count(); ++i)
      {QString motifMusBeInDirToErase = dirToRmList[i];
       if (motifMusBeInDirToErase.length()==0) return false;
       int modeTest = 3;                                                                                     // est egal
       if (motifMusBeInDirToErase.endsWith("*") && motifMusBeInDirToErase.startsWith("*"))   modeTest = 1;   // contient
       else if (motifMusBeInDirToErase.startsWith("*"))                                      modeTest = 2;   // finit par
       else if (motifMusBeInDirToErase.endsWith("*"))                                        modeTest = 0;   // commence par

       motifMusBeInDirToErase = motifMusBeInDirToErase.remove("*");
       if      (modeTest==0 && dname.startsWith(motifMusBeInDirToErase)) return TRUE; // commence par
       else if (modeTest==2 && dname.endsWith(motifMusBeInDirToErase))   return TRUE; // finit par
       else if (modeTest==1 && dname.find(motifMusBeInDirToErase)!=-1)   return TRUE; // contient
       else if (modeTest==3 && dname==motifMusBeInDirToErase)            return TRUE; // est egal
      }
 return FALSE;
}
//----------------------------------------- isThisFileMustBeErase ---------------------------------------------
bool C_Dlg_MainDialog::isThisFileMustBeErase( const QString &fname, const QStringList &fileToRmList)
{
  for (int i=0; i< (int)fileToRmList.count(); ++i)
      {QString motifMusBeInFileToErase = fileToRmList[i];
       int modeTest = 3;                                                                                      // est egal
       if (motifMusBeInFileToErase.endsWith("*") && motifMusBeInFileToErase.startsWith("*"))  modeTest = 1;   // contient
       else if (motifMusBeInFileToErase.startsWith("*"))                                      modeTest = 2;   // finit par
       else if (motifMusBeInFileToErase.endsWith("*"))                                        modeTest = 0;   // commence par

       motifMusBeInFileToErase = motifMusBeInFileToErase.remove("*");
       if      (modeTest==0 && fname.startsWith(motifMusBeInFileToErase)) return TRUE; // commence par
       else if (modeTest==2 && fname.endsWith(motifMusBeInFileToErase))   return TRUE; // finit par
       else if (modeTest==1 && fname.find(motifMusBeInFileToErase)!=-1)   return TRUE; // contient
       else if (modeTest==3 && fname==motifMusBeInFileToErase)            return TRUE; // est egal
      }
 return FALSE;
}

//----------------------------------------- EraseDirectory ---------------------------------------------
void C_Dlg_MainDialog::EraseDirectory( QString dir_to_erase)
{
    //........................ petit message .......................................;
    QDir cur_dir( dir_to_erase );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks | QDir::Hidden );
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {
        QString fname = fi->fileName().latin1();
        if      (fname == "..")
        {
        }
        else if (fi->isDir() && fname != "." )
        {EraseDirectory(dir_to_erase + "/" + fname);
        }
        else if (fi->isFile())
        {   QFile::remove (dir_to_erase + "/" + fname);
        }
        ++it;
    }
    cur_dir.rmdir(dir_to_erase );
}

