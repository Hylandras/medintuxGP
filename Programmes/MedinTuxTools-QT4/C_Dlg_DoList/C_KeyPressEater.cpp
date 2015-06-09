/********************************** C_KeyPressEater.cpp ***************************
 *                                                                                *
 *   #include "C_KeyPressEater.h"            QT4-ready                            *
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
 *   Commissariat �  l'Energie Atomique                                           *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15. *
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

#include "C_KeyPressEater.h"

//-------------------------------- C_KeyPressEater --------------------------------------

C_KeyPressEater::C_KeyPressEater (  QTreeWidget *pQTreeWidget , QObject * parent /*=0*/, const QString & name /*="C_KeyPressEater"*/)
 : QObject ( parent )
{setObjectName ( name );
 m_pQTreeWidget = pQTreeWidget ;
}
//--------------------------------- getSelectedListViewItem -------------------------------------------------------------
QTreeWidgetItem * C_KeyPressEater::getSelectedListViewItem(QTreeWidget *pQTreeWidget)
{   QList<QTreeWidgetItem *> list = pQTreeWidget->selectedItems ();
    if (list.count()) return list[0];
    else              return 0;
}
//-------------------------------- eventFilter --------------------------------------
bool C_KeyPressEater::eventFilter( QObject * /*o*/, QEvent *e )
{if ( e->type() != QEvent::KeyPress ) return FALSE;
 QKeyEvent *key_evnt = (QKeyEvent *)e;
 switch(key_evnt->key())
   {case Qt::Key_Return:
    case Qt::Key_Enter:
         return FALSE;
         break;
    case Qt::Key_Down:
         {QTreeWidgetItem * pQTreeWidgetItem = getSelectedListViewItem(m_pQTreeWidget);
          if (pQTreeWidgetItem)
             {QTreeWidgetItem * pQTreeWidgetItemBelow = m_pQTreeWidget->itemBelow( pQTreeWidgetItem );
              if (pQTreeWidgetItemBelow)
                 {m_pQTreeWidget->clearSelection ();
                  pQTreeWidgetItemBelow->setSelected(TRUE);
                 }
             }
          else if (m_pQTreeWidget->topLevelItemCount ())
             {pQTreeWidgetItem = m_pQTreeWidget->topLevelItem(0);
              if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(TRUE);
             }
          emit Sign_Down_Pressed(pQTreeWidgetItem);
          return TRUE;
         }
         break;
    case Qt::Key_Up:
         {QTreeWidgetItem * pQTreeWidgetItem = getSelectedListViewItem(m_pQTreeWidget);
          if (pQTreeWidgetItem)
             {QTreeWidgetItem * pQTreeWidgetItemAbove = m_pQTreeWidget->itemAbove( pQTreeWidgetItem );
              if (pQTreeWidgetItemAbove)
                 {m_pQTreeWidget->clearSelection ();
                  pQTreeWidgetItemAbove->setSelected(TRUE);
                 }
             }
          else if (m_pQTreeWidget->topLevelItemCount ())
             {pQTreeWidgetItem = m_pQTreeWidget->topLevelItem(0);
              if (pQTreeWidgetItem) pQTreeWidgetItem->setSelected(TRUE);
             }
          emit Sign_Up_Pressed(pQTreeWidgetItem);
          return TRUE;
         }
         break;
    case Qt::Key_F1:
         {emit Sign_F1_Pressed();
          return TRUE;
         }
         break;
    case Qt::Key_F2:
         {emit Sign_F2_Pressed();
          return TRUE;
         }
         break;
    case Qt::Key_F6:
         {emit Sign_F6_Pressed();
          return TRUE;
         }
         break;
    }
return FALSE; // standard event processing
}
