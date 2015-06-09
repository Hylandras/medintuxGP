 /********************************* C_Dlg_DoList.cpp ******************************
 *  #include "../../MedinTuxTools/C_Dlg_DoList/C_Dlg_DoList.h"                    *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat ?  l'Energie Atomique                                           *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F�d�ration, 75752 PARIS cedex 15.   *
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
#include "C_Dlg_DoList.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
#include "C_KeyPressEater.h"

#include <QHeaderView>
#include <QTreeWidgetItem>

//---------------------------------------- C_Dlg_DoList ----------------------------------
C_Dlg_DoList::C_Dlg_DoList(const QStringList &list, const QString& sep /*= "|"*/, int posToHide /* = 0*/, int selectionMode /*=C_Dlg_DoList::SingleSelection*/, QWidget *parent) :
    QDialog(parent), m_selectionMode (selectionMode)
{   m_inputList     = list;
    m_posToHide     = posToHide;
    m_sep           = sep;
    m_selectionMode = selectionMode;

    m_ui.setupUi(this);
    m_ui.lineEdit_Filtre->setFocus();
    connect( m_ui.treeWidget_Choix,     SIGNAL( itemDoubleClicked ( QTreeWidgetItem * , int)), this, SLOT( Slot_treeWidget_Choix_DoubleClicked( QTreeWidgetItem * , int)) );
    connect( m_ui.pushButton_Ok,        SIGNAL( clicked() ),                                   this, SLOT( Slot_pushButton_Ok_clicked() ) );
    connect( m_ui.pushButton_Cancel,    SIGNAL( clicked() ),                                   this, SLOT( Slot_pushButton_Cancel_clicked() ) );
    connect( m_ui.lineEdit_Filtre,      SIGNAL( textChanged(const QString &)),                 this, SLOT( Slot_FiltrerListe(const QString &)) );

    //................... un petit bidouillage accelerateur ...........................................................
    //                    pour relier fleche haute, basse, return de la zonne d'edition
    //                    au QTreeWidget pour faire monter et descendre la selection
    C_KeyPressEater *keyPressEater = new C_KeyPressEater( m_ui.treeWidget_Choix, this , "C_Dlg_DoList::C_KeyPressEater");
    m_ui.lineEdit_Filtre->installEventFilter( keyPressEater );

    //................... effacer le header des identifiants ...................
    QHeaderView *pQHeaderView = m_ui.treeWidget_Choix->header();
    pQHeaderView->hideSection (1);
    pQHeaderView->hideSection (2);
    //m_ui.treeWidget_Choix->setAlternatingRowColors (TRUE );
    Slot_FiltrerListe("");
}

//---------------------------------------- Slot_FiltrerListe ---------------------------------------
void C_Dlg_DoList::Slot_FiltrerListe( const QString &textFilter)
{   //................... remplir la liste .....................................
    m_ui.treeWidget_Choix->clear();
    for (int i = 0; i < m_inputList.size(); ++i)
     { QString toInsert    = m_inputList[i];
       QString identifiant = "";
       QString icone       = "";
       bool    isSeparator = FALSE;
       if (m_sep.length())           // si chaque item de la liste est compos�e de plusieurs termes separes par un separateur
          {if (m_posToHide > 0)    // si l'on doit masquer les champs situes apres toMask
              {int   i = 0;
               int pos = 0;
               while ( i<m_posToHide && (pos = toInsert.indexOf(m_sep, pos + m_sep.length())) != -1)
                 {++i;
                 };
               if (pos != -1) toInsert = toInsert.left(pos);
             }
           identifiant = m_inputList[i];
           toInsert    = toInsert.replace(m_sep," ");   // on ne va pas afficher les separateurs dans le menu
         }
       if (toInsert[0]=='=')
          {int pc = toInsert.indexOf('=',1);
           if (pc != -1)
              {identifiant = toInsert.mid(1,pc-1);
               toInsert    = toInsert.mid(pc+1);
              }
          }

       if (toInsert[0]=='$')
          {setCaption(toInsert.mid(1));
           toInsert = "";
          }
       else if (toInsert.left(4) == "----")
               {isSeparator = TRUE;
               }
       else if (toInsert[0]=='#')
               {int pc = toInsert.indexOf('#',1);
                if (pc != -1)
                   {icone    = toInsert.mid(1,pc-1);
                    toInsert = toInsert.mid(pc+1);;
                   }
               }
       //....................... inserer les items selon filtre.........................................
       if (textFilter.length() && !toInsert.contains (textFilter, Qt::CaseInsensitive )) toInsert = "";
       if (toInsert.length())
          { QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem( );
            if (pQTreeWidgetItem)
               {m_ui.treeWidget_Choix->addTopLevelItem(pQTreeWidgetItem);
                pQTreeWidgetItem->setText(0, toInsert);
                pQTreeWidgetItem->setText(1, identifiant);
                pQTreeWidgetItem->setText(2, QString::number(i));
                if (icone.length()) pQTreeWidgetItem->setIcon (0,Theme::getIconSet(icone));
               }
          }
     }
}

//---------------------------------------- Slot_treeWidget_Choix_DoubleClicked ---------------------------------------
void C_Dlg_DoList::Slot_treeWidget_Choix_DoubleClicked( QTreeWidgetItem * , int)
{Slot_pushButton_Ok_clicked();
}

//---------------------------------------- Slot_pushButton_Ok_clicked --------------------------------------
void C_Dlg_DoList::Slot_pushButton_Ok_clicked()
{ accept();
}
//---------------------------------------- getListMultiSelectedResult -------------------------------------------------------
QStringList C_Dlg_DoList::getListMultiSelectedResult()
{    m_resultList.clear();
     QTreeWidgetItem *pQTreeWidgetItem=0;
     if (m_ui.treeWidget_Choix->topLevelItemCount()==1)
        {pQTreeWidgetItem = m_ui.treeWidget_Choix->topLevelItem (0);
         if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(TRUE);
        }
     QTreeWidgetItemIterator it(m_ui.treeWidget_Choix);
     while (*it)
       { pQTreeWidgetItem = (*it);
         if ( pQTreeWidgetItem->isSelected () )
            {int i          = pQTreeWidgetItem->text(2).toInt();
             m_resultList.append(m_inputList[i]);
            }
         ++it;
       }
    return m_resultList;
}
//---------------------------------------- getStringMultiSelectedResult -------------------------------------------------------
QString C_Dlg_DoList::getStringMultiSelectedResult(QString sep /*= ""*/)
{   m_resultString           = "";
    QTreeWidgetItem *pQTreeWidgetItem=0;
     if (m_ui.treeWidget_Choix->topLevelItemCount()==1)
        {pQTreeWidgetItem = m_ui.treeWidget_Choix->topLevelItem (0);
         if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(TRUE);
        }
    if (sep.length()==0) sep = m_sep;
    QTreeWidgetItemIterator it(m_ui.treeWidget_Choix);
    while (*it)
       { pQTreeWidgetItem = (*it);
         if ( pQTreeWidgetItem->isSelected () )
            {int i          = pQTreeWidgetItem->text(2).toInt();
             m_resultString = m_inputList[i] + sep;
            }
         ++it;
       }
    return m_resultString;
}
//---------------------------------------- getStringSingleSelectedResult -------------------------------------------------------
QString C_Dlg_DoList::getStringSingleSelectedResult()
{    m_resultString = "";
     QTreeWidgetItem *pQTreeWidgetItem=0;
     if (m_ui.treeWidget_Choix->topLevelItemCount()==1)
        {pQTreeWidgetItem = m_ui.treeWidget_Choix->topLevelItem (0);
         if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(TRUE);
        }
     QTreeWidgetItemIterator it(m_ui.treeWidget_Choix);
     while (*it)
       { pQTreeWidgetItem = (*it);
         if ( pQTreeWidgetItem->isSelected () )
            {int i          = pQTreeWidgetItem->text(2).toInt();
             m_resultString = m_inputList[i];
             break;
            }
         ++it;
       }
     return m_resultString;
}

//---------------------------------------- Slot_pushButton_Cancel_clicked ----------------------------------
void C_Dlg_DoList::Slot_pushButton_Cancel_clicked()
{m_resultString = "";
 reject();
}
//---------------------------------------- changeEvent ----------------------------------
void C_Dlg_DoList::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui.retranslateUi(this);
        break;
    default:
        break;
    }
}
