/******************** C_Dlg_FamilleGenre.cpp **************************************
 *                                                                                *
 *   #include "C_Dlg_FamilleGenre.h"                                              *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 ->2009 by Sevin Roland  and Data Medical Design *
 *   E-Mail: roland-sevin@medintux.org                                            *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat �  l'Energie Atomique                                            *
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
#include "C_Dlg_FamilleGenre.h"
#include <qmessagebox.h>
#include "CApp.h"
#include "../../MedinTuxTools/CGenTools.h"

//-----------------------------------------------------  C_Dlg_FamilleGenre -------------------------------------------
C_Dlg_FamilleGenre::C_Dlg_FamilleGenre(QString* pStructStr, QWidget* parent, const char* name, bool modal, WFlags fl)
    : Dlg_FamilleGenre(parent, name, modal, fl)
{   m_pStructStr     =  pStructStr;
    m_isModified     = FALSE;
    pushButton_save->setPixmap(Theme::getIcon( "16x16/save.png"));
    pushButton_del->setPixmap(Theme::getIcon( "16x16/DelDoc.png"));
    pushButton_add_Famille->setPixmap(Theme::getIcon( "16x16/NewDir.png"));
    pushButton_add_Genre->setPixmap(Theme::getIcon( "16x16/NewDoc.png"));

    connect( pushButton_save,          SIGNAL( clicked() ), this, SLOT( Slot_pushButton_save() ));
    connect( pushButton_del,           SIGNAL( clicked() ), this, SLOT( Slot_pushButton_del() ));
    connect( pushButton_add_Famille,   SIGNAL( clicked() ), this, SLOT( Slot_pushButton_add_Famille() ));
    connect( pushButton_add_Genre,     SIGNAL( clicked() ), this, SLOT( Slot_pushButton_add_Genre() ));
    connect( pushButton_Quit,          SIGNAL( clicked() ), this, SLOT( Slot_accept() ));

    unserialize(*m_pStructStr);
    *m_pStructStr = serialize();   // juste pour etre certain que la sortie sera identique si pas de modifications
}

//-----------------------------------------------------  C_Dlg_FamilleGenre -------------------------------------------
C_Dlg_FamilleGenre::~C_Dlg_FamilleGenre()
{
}

/*$SPECIALIZATION$*/
//-----------------------------------------------------  Slot_reject -------------------------------------------
void C_Dlg_FamilleGenre::Slot_reject()
{QDialog::reject();
}

//-----------------------------------------------------  Slot_accept -------------------------------------------
void C_Dlg_FamilleGenre::Slot_accept()
{QString str =  serialize();
 if (str != *m_pStructStr)
    {if (QMessageBox::information( this, tr("Enregistrer les modifications"),
                                     tr ( " <b><u>ATTENTION</b></u> ! quitter sans enregistrer les modifications ?")  ,
                                     tr("&Quitter"), tr("Enregistrer"), 0 /*tr("Annuler")*/,
                                     0, 1 ) != 0
        )
        {Slot_pushButton_save();
        }
    }
  QDialog::accept();
}

//-----------------------------------------------------  Slot_pushButton_save -------------------------------------------
void C_Dlg_FamilleGenre::Slot_pushButton_save()
{ *m_pStructStr = serialize();
   m_isModified = TRUE;
   CGestIni::Param_UpdateToDisk(G_pCApp->m_PathAppli + "Ressources/MenuATCD.txt", *m_pStructStr);
}

//-----------------------------------------------------  unserialize -------------------------------------------
void C_Dlg_FamilleGenre::unserialize(const QString &data)
{   QString section         = "";
    int posDebSection       = -1;
    int posEndSection       = 0;
    listView_FamilleGenre->clear();
    while ( (posDebSection  = data.find("[",posEndSection)) != -1 )
       {++posDebSection;
        if ( (posEndSection = data.find("]",posDebSection)) != -1 )
           {section = data.mid(posDebSection,posEndSection-posDebSection).stripWhiteSpace();
            if (section.length())
               {QListViewItem* pQListViewItemFamille = new QListViewItem( listView_FamilleGenre, section);
                if (pQListViewItemFamille)
                   {QStringList genreList;
                    CGestIni::Param_GetList(*m_pStructStr, section, "",  genreList );
                    for (int i=0; i < (int) genreList.size(); ++i)
                        {QListViewItem* pQListViewItemGenre = new QListViewItem( pQListViewItemFamille, genreList[i].stripWhiteSpace());
                         pQListViewItemGenre->setRenameEnabled (0, TRUE );
                         pQListViewItemGenre->setPixmap ( 0,Theme::getIcon( "16x16/genre.png") );
                        }
                    pQListViewItemFamille->setRenameEnabled (0, TRUE );
                    pQListViewItemFamille->setPixmap ( 0,Theme::getIcon( "16x16/famille.png") );
                    pQListViewItemFamille->setOpen (TRUE);
                   }
               }
           }
       }
}

//-----------------------------------------------------  serialize -------------------------------------------
QString C_Dlg_FamilleGenre::serialize()
{ QString str   = "";
  int        i  = 0;
  QListViewItemIterator it( listView_FamilleGenre );
  while ( it.current() )
        {   QListViewItem *pQListViewItemFamille = it.current();
            if (pQListViewItemFamille->depth()==0)
               {QListViewItem * pQListViewItemGenre = pQListViewItemFamille->firstChild();
                while( pQListViewItemGenre )
                     { CGestIni::Param_WriteParam( &str, pQListViewItemFamille->text(0).stripWhiteSpace(), QString::number(i++), pQListViewItemGenre->text(0).stripWhiteSpace());
                      pQListViewItemGenre = pQListViewItemGenre->nextSibling();
                     }
                i=0;
               }
            ++it;
        }
  return str;
}

//-----------------------------------------------------  Slot_pushButton_del -------------------------------------------
void C_Dlg_FamilleGenre::Slot_pushButton_del()
{QListViewItem* pQListViewItem = listView_FamilleGenre->selectedItem ();
 if (pQListViewItem==0) return;

 if (pQListViewItem->depth()==0 && pQListViewItem->firstChild())
    { if (QMessageBox::information( this, tr("Effacement d'une famille entière"),
                                     tr ( " <b><u>ATTENTION</b></u> ! Cette famille <b>")  + pQListViewItem->text(0) +
                                     tr(  "</b> contient des genres. " ) +
                                     tr(  "<br> Faut-il tout de même l'effacer avec<br>"
                                          "tous les genres en faisant partie ?"
                                       ) ,
                                     tr("&Annuler"), tr("Effacer"), 0 /*tr("Annuler")*/,
                                     0, 1 )
          == 0) return;
    }
 delete pQListViewItem;
}

//-----------------------------------------------------  Slot_pushButton_add_Famille -------------------------------------------
void C_Dlg_FamilleGenre::Slot_pushButton_add_Famille()
{QString text;
 if (CGenTools::DialogGetString(0, TR("Saisir le nom de la nouvelle famille d'antécédent") ,text) == QDialog::Accepted)
     { QListViewItem* pQListViewItemFamille = new QListViewItem( listView_FamilleGenre, text);
       if (pQListViewItemFamille)
          {pQListViewItemFamille->setPixmap ( 0,Theme::getIcon( "16x16/famille.png") );
           pQListViewItemFamille->setOpen(TRUE);
           pQListViewItemFamille->setRenameEnabled(0, TRUE );
           listView_FamilleGenre->setSelected ( pQListViewItemFamille, TRUE );
           listView_FamilleGenre->ensureItemVisible ( pQListViewItemFamille );
          }
     }
}

//-----------------------------------------------------  Slot_pushButton_add_Genre -------------------------------------------
void C_Dlg_FamilleGenre::Slot_pushButton_add_Genre()
{QListViewItem* pQListViewItemFamille = listView_FamilleGenre->selectedItem ();
 if (pQListViewItemFamille==0) return;
 if (pQListViewItemFamille->depth()>0)
    {pQListViewItemFamille = pQListViewItemFamille->parent();
    }
 if (pQListViewItemFamille==0) return;
 QString text;
 if (CGenTools::DialogGetString(0, TR("Saisir le genre �  ajouter �  la famille : ")+ pQListViewItemFamille->text(0),text) == QDialog::Accepted)
     { QListViewItem* pQListViewItemGenre = new QListViewItem( pQListViewItemFamille, text);
       if (pQListViewItemGenre)
          {pQListViewItemGenre->setPixmap ( 0,Theme::getIcon( "16x16/genre.png") );
           pQListViewItemGenre->setRenameEnabled (0, TRUE );
           listView_FamilleGenre->setSelected ( pQListViewItemGenre, TRUE );
           listView_FamilleGenre->ensureItemVisible ( pQListViewItemGenre );
          }
     }
}



