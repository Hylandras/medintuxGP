/********************************** C_KeyPressEater.cpp ***************************
 *                                                                                *
 *   #include "C_KeyPressEater.h"                                                 *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@medintux.org                                     *
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

#include "C_KeyPressEater.h" 

//-------------------------------- C_KeyPressEater --------------------------------------

C_KeyPressEater::C_KeyPressEater (  QListView *pQListView , QObject * parent, const char * name )
 : QObject ( parent , name  ) 
{ m_pQListView = pQListView ;
}

//-------------------------------- eventFilter --------------------------------------
bool C_KeyPressEater::eventFilter( QObject * /*o*/, QEvent *e )
    {
        if ( e->type() == QEvent::KeyPress ) 
           {
            // special processing for key press
            QKeyEvent *k = (QKeyEvent *)e;
            if ( k->key() == Qt::Key_Up)
               {QListViewItem *pQListViewItem  =  m_pQListView->currentItem ();
                if (pQListViewItem) { m_pQListView->selectAll (FALSE ); 
                                      pQListViewItem = pQListViewItem->itemAbove();
                                    }
                if (pQListViewItem==0 && m_pQListView->childCount()==1) pQListViewItem = m_pQListView->firstChild();
                if (pQListViewItem) { m_pQListView->setCurrentItem ( pQListViewItem ); 
                                      m_pQListView->ensureItemVisible ( pQListViewItem );
                                      m_pQListView->setSelected ( pQListViewItem, TRUE );                                       
                                    }
          
                emit Sign_Up_Pressed(pQListViewItem);
                return TRUE;
               }
            else if ( k->key() == Qt::Key_Down)
               {QListViewItem *pQListViewItem  =  m_pQListView->currentItem ();
                if (pQListViewItem) {m_pQListView->selectAll (FALSE ); 
                                     pQListViewItem = pQListViewItem->itemBelow();
                                    }
                if (pQListViewItem==0 && m_pQListView->childCount()==1) pQListViewItem = m_pQListView->firstChild();
                if (pQListViewItem) {
                                     m_pQListView->setCurrentItem ( pQListViewItem ); 
                                     m_pQListView->ensureItemVisible ( pQListViewItem );
                                     m_pQListView->setSelected ( pQListViewItem, TRUE );
                                    }
                emit Sign_Down_Pressed(pQListViewItem);
                return TRUE;
               }
           else if ( k->key() == Qt::Key_F1)
               {emit Sign_F1_Pressed();
                return TRUE;
               }
           else if ( k->key() == Qt::Key_F2)
               {emit Sign_F2_Pressed();
                return TRUE;
               }
           else if ( k->key() == Qt::Key_F6)
               {emit Sign_F6_Pressed();
                return TRUE;
               }

            // qDebug( "Ate key press %d", k->key() );
            return FALSE; // eat event
          }
       else
          {  // standard event processing
            return FALSE;
          }
    }
