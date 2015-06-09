/********************************* C_QToolBox.cpp ********************************
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
#include "C_QToolBox.h"
#include <QMapIterator>
//--------------------------------- C_QToolBox -----------------------------------------------------------------------
C_QToolBox::C_QToolBox(QWidget *parent) :
    QToolBox(parent)
{
}

//--------------------------------- removeItem -----------------------------------------------------------------------
void C_QToolBox::removeItem ( int index )
{   removeItem (widget ( index ), index );
}

//--------------------------------- removeItem -----------------------------------------------------------------------
QWidget * C_QToolBox::removeItem (QWidget *pTab_QWidgetToRemove, int index )
{   if (pTab_QWidgetToRemove==0 ) return 0;
    pTab_QWidgetToRemove->hide();
    //........ recuperer le titre ..............................
    QString title = itemText ( index );
    //............ inserer dans la liste des absents ...........
    //             (pouvoir le retrouver par la suite)
    m_removedObject.insert(pTab_QWidgetToRemove, title);
    //............ le vierer de la pQToolBox ...................
    QToolBox::removeItem ( index );
    return pTab_QWidgetToRemove;
}

//--------------------------------- unactiveItemByObjectName -----------------------------------------------------------------------
QWidget * C_QToolBox::unactiveItemByObjectName(const QString & widget_page_object_name)
{//.......... rechercher l'objet et on index dans la QToolBox par son nom d'objet ...................
 int index = -1;
 QWidget *pTab_QWidgetToRemove = toolWidgetByTabObjectName(widget_page_object_name, &index);
 return removeItem (pTab_QWidgetToRemove, index );
}

//--------------------------------- activeItemByObjectName -----------------------------------------------------------------------
// LA CLEF est l'objet lui meme : on mape l'objet avec son titre
// ici la clef est l'objet (on recherche le titre du tab de la toolBox par l'objet)
QWidget * C_QToolBox::activeItemByObjectName(const QString & widget_page_object_name)
{
 //............ rechercher le widget de la page ....................
 //             dans la liste des removed
 QWidget *pTab_QWidgetToRetrieve = 0;
 QMapIterator< QWidget *, QString>  it (m_removedObject) ;
 while ( it.hasNext() )
       { it.next();
         QWidget *pTab_QWidget = it.key();
         if ( pTab_QWidget->objectName() == widget_page_object_name )
            { pTab_QWidgetToRetrieve = pTab_QWidget;
              break;
            }
       } // while (it.hasNext())
 if ( pTab_QWidgetToRetrieve==0 )  return 0;
 //............. si trouve l'ajouter au QToolBox ...........
 QString title =  it.value();           // la clef est l'objet (on recherche le titre du tab de la toolBox par l'objet)
 addItem ( pTab_QWidgetToRetrieve, title );
 //............. et l'enlever de la liste des removed ...........
 m_removedObject.remove( pTab_QWidgetToRetrieve );
 pTab_QWidgetToRetrieve->show();
 return pTab_QWidgetToRetrieve;
}
//------------------------------------ setCurrentItem --------------------------------------------------
QWidget * C_QToolBox::setCurrentItem (  const QString &pageName )
{   QWidget *pQWidget =  toolWidgetByTabObjectName(pageName);
    if (pQWidget==0) return 0;
    setCurrentWidget ( pQWidget );
    return pQWidget;
}

//------------------------------------ setItemIcon --------------------------------------------------
void C_QToolBox::setItemIcon(const QString pageName, const QPixmap &pixmap)
{   int index = -1;
    toolWidgetByTabObjectName(pageName, &index );
    if (index==-1) return;
    QToolBox::setItemIcon (index, pixmap);
}
//------------------------------------ setItemIcon --------------------------------------------------
void C_QToolBox::setItemIcon(int index, const QPixmap &pixmap)
{   if ( index == -1) return;
    QToolBox::setItemIcon (index, pixmap);
}
//------------------------------------ toolWidgetByTabObjectName --------------------------------------------------
QWidget * C_QToolBox::toolWidgetByTabObjectName(const QString &name, int *index  /* = 0 */)
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
//------------------------------------ toolWidgetIndexByTabObjectName --------------------------------------------------
int C_QToolBox::toolWidgetIndexByTabObjectName(const QString &name)
{int nb = count();
 for (int i=0; i<nb; ++i)
     {QWidget *pTab_QWidget = widget(i);
      if (pTab_QWidget->objectName()==name) return i;
     }
 return -1;
}
//------------------------------------ removedList --------------------------------------------------
QStringList C_QToolBox::removedList()
{   QStringList list;
    QMapIterator<QWidget*,QString> it(m_removedObject);
    while (it.hasNext())
         { it.next();
           list << it.value();
         }
    return list;
}

