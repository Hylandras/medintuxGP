 /*********************************************************************************
 *                                                                                *
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

#include "C_QTabWidget.h"
#include "../Theme/ThemePopup.h"
#include <QMapIterator>
#include <QMenu>

//--------------------------------- C_QTabWidget -----------------------------------------------------------------------
C_QTabWidget::C_QTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
  connect( this,                SIGNAL(tabCloseRequested (int )) , this,   SLOT (Slot_CloseRequested( int )) );
}

//--------------------------------- activeTabWidgetByObjectName -----------------------------------------------------------------------
// ici la clef est le titre du tab (on recherche l'objet par son titre dans un menu)
QWidget * C_QTabWidget::activeTabWidgetByObjectName( const QString & widget_page_object_name)
{
 //............ rechercher le widget de la page ....................
 //             dans la liste des removed
 QWidget *pTab_QWidgetToRetrieve = 0;
 QMapIterator<QString, QWidget *>  it (m_removedObject) ;
 while ( it.hasNext() )
       { it.next();
         QWidget *pTab_QWidget = it.value();
         if ( pTab_QWidget->objectName() == widget_page_object_name )
            { pTab_QWidgetToRetrieve = pTab_QWidget;
              break;
            }
       } // while (it.hasNext())
 if ( pTab_QWidgetToRetrieve==0 )  return 0;
 //............. si trouve l'ajouter au QTabWidget ...........
 QString key =  it.key();           // la clef est le titre de l'objet dans la QTabWidget et non son objectName()
 addTab( pTab_QWidgetToRetrieve, key );
 //............. l'enlever de la liste des removed ...........
 m_removedObject.remove( key );
 return pTab_QWidgetToRetrieve;
}
//------------------------------------ setCurrentItem --------------------------------------------------
QWidget * C_QTabWidget::setCurrentItem (  const QString &pageName )
{   QWidget *pQWidget =  tabWidgetByTabObjectName(pageName);
    if (pQWidget==0) return 0;
    setCurrentWidget ( pQWidget );
    return pQWidget;
}

//------------------------------------ removedList --------------------------------------------------
QStringList C_QTabWidget::removedList()
{   QStringList list;
    QMapIterator<QString, QWidget*> it(m_removedObject);
    while (it.hasNext())
         { it.next();
           list << it.key();
         }
    return list;
}

//--------------------------------- doPageChoiceMenu -----------------------------------------------------------------------
void C_QTabWidget::doPageChoiceMenu( QWidget * parent )
{   int nbToFind      = 0;
    //........ parser la QMap des removed pour creer le menu ................
    //         des rubriques a activer
    QMenu *pQMenu     = new QMenu(parent);  if (pQMenu==0) return;

    QMapIterator<QString, QWidget*> it (m_removedObject);
    while (it.hasNext())
      { it.next();
        ThemePopup::addOptionMenu(pQMenu, tr("  %1  - Activer : %2").arg(it.key(),  it.key()));
        ++nbToFind;
       }

    //........ actionner le menu ................
    if (nbToFind)
       { QAction *QAction = pQMenu->exec(this->cursor().pos());
         if (QAction)
            { QString key = QAction->data().toString();
              QWidget *pTab_QWidget = m_removedObject[key];
              if (pTab_QWidget)
                 { addTab(pTab_QWidget, key);
                   m_removedObject.remove(key);
                   setCurrentWidget(pTab_QWidget);
                 }
            }
       }
    delete pQMenu;
}

//--------------------------------- Slot_tabWidget_Main_CloseRequested -----------------------------------------------------------------------
void C_QTabWidget::Slot_CloseRequested( int index)
{   if ( count () <= 1)  return;               // laisser au moins un onglet
    if (index < 0)       return;
    QWidget *pTab_QWidget =  widget(index);
    if (pTab_QWidget==0) return;
    m_removedObject.insert( tabText ( index ), pTab_QWidget );
    removeTab (index );
}
//------------------------------------ tabWidgetByTabText --------------------------------------------------
QWidget * C_QTabWidget::tabWidgetByTabText( const QString &text, int *index  /* = 0 */)
{int nb = count();
 for (int i=0; i<nb; ++i)
     {if (tabText(i)==text)
         {if (index) *index = i;
          return widget(i);
         }
     }
 if (index) *index = -1;
 return 0;
}

//------------------------------------ tabWidgetByTabObjectName --------------------------------------------------
QWidget * C_QTabWidget::tabWidgetByTabObjectName(const QString &name, int *index  /* = 0 */)
{int nb = count();
 for (int i=0; i<nb; ++i)
     {QWidget *pTab_QWidget = widget(i);
      if (pTab_QWidget->objectName()==name)
         { if (index) *index = i;
           return pTab_QWidget;
         }
     }
 return 0;
}

//------------------------------------ tabWidgetIndexByTabObjectName --------------------------------------------------
int C_QTabWidget::tabWidgetIndexByTabObjectName(const QString &name)
{int nb = count();
 for (int i=0; i<nb; ++i)
     {QWidget *pTab_QWidget = widget(i);
      if (pTab_QWidget->objectName()==name) return i;
     }
 return -1;
}
