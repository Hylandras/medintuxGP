//
// C++ Implementation: ThemePopup
//
// Description:
//
//
// Author: Roland Sevin <data.medical.design@club-internet.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <qcursor.h>
#include "ThemePopup.h"

//--------------------------------- ThemePopup -----------------------------------------------------
ThemePopup::ThemePopup(QWidget * parent, const char * name )
 : QPopupMenu(parent, name)
{ if (Theme::getPath()!="")
  { QFont f = QPopupMenu::font();
    int size = Theme::getFontSize_Popups();
    if (size != -1)
       {f.setPointSize( Theme::getFontSize_Popups() );
        QPopupMenu::setFont( f );
       }
  }
 m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 m_CurrentPopupID   = -1;
 m_RetPopupID       = -1;
 m_pListItem        =  0;
 connect( this, SIGNAL( activated( int)),            this, SLOT(memorisePopupItem( int))  );
}

//--------------------------------- ThemePopup -----------------------------------------------------
ThemePopup::ThemePopup(QStringList &list, QWidget * parent, const char * name,  const QString& sep /*= "|"*/, int toMask /*= 0*/ )
 : QPopupMenu(parent, name)
{m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 m_CurrentPopupID   = -1;
 m_RetPopupID       = -1;
 m_pListItem        = &list;
 bool isDisabled    = FALSE;
 int  idItem        = -1;
 if (Theme::getPath()!="")
     { QFont f  = QPopupMenu::font();
       int size = Theme::getFontSize_Popups();
       if (size != -1)
          {f.setPointSize( Theme::getFontSize_Popups() );
           QPopupMenu::setFont( f );
          }
     }
 int id = 0;
 //............................... chercher une zone de liste deroulante (bleue) .....................
 for ( QStringList::Iterator it =list.begin(); it != list.end(); ++it )
     { QString toInsert = *(it);
       if (toInsert[0]=='~') {isDisabled=TRUE; toInsert=toInsert.mid(1);}
       else                   isDisabled=FALSE;
       if (sep.length())        // si chaque item de la liste est composée de plusieurs termes separes par un separateur
          {if (toMask > 0)      // si l'on doit masquer les champs situes apres toMask
              {int   i = 0;
               int pos = 0;
               while ( i<toMask && (pos = toInsert.find(sep, pos + sep.length())) != -1)
                 {++i;
                 };
               if (pos != -1) toInsert = toInsert.left(pos);
             }
           toInsert = toInsert.replace(sep," ");   // on ne va pas afficher les separateurs dans le menu
         }
       if (toInsert.left(4) == "----")
               {insertSeparator();
               }
       else if (toInsert[0]=='#')
               {int pc = toInsert.find('#', 1);
                if (pc != -1)
                   {QString icone = toInsert.mid(1,pc-1);
                    idItem = insertItem(Theme::getIconSet(icone), toInsert.mid(pc+1), this, SLOT( OnSelectPopupItem()),  0 , id);
                   }
                else idItem = insertItem( toInsert,    this, SLOT( OnSelectPopupItem()),  0 , id);
               }
       else    {idItem = insertItem( toInsert,    this, SLOT( OnSelectPopupItem()),  0 , id);
               }
       if (isDisabled) setItemEnabled ( idItem, FALSE );
       ++id;
     }
 connect( this, SIGNAL( activated( int)),            this, SLOT(memorisePopupItem( int))  );
}

//--------------------------------- DoPopupList -----------------------------------------------------
QString ThemePopup::DoPopupList(int *pID_to_Ret /*=0*/)
{m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 m_CurrentPopupID   = -1;
 m_RetPopupID       = -1;
 exec( QCursor::pos() /* mapToGlobal (  QPoint ( x, y) )*/ );
 if (pID_to_Ret)  *pID_to_Ret = m_CurrentPopupID;
 return  m_RetPopupItem;
}

//--------------------------------- OnMenuActivated  --------------------------------------------------
void ThemePopup::memorisePopupItem (int id)
{if (id != -1 )
    {if (m_pListItem) m_CurrentPopupItem =  (*m_pListItem)[id];
     else             m_CurrentPopupItem = text(id);
     m_CurrentPopupID                    = id;
    }
}

//--------------------------------- OnSelectPopupItem  --------------------------------------------------
void ThemePopup::OnSelectPopupItem ()
{m_RetPopupID    = m_CurrentPopupID;
 if (m_pListItem) m_RetPopupItem     =  (*m_pListItem)[m_CurrentPopupID];
 else             m_RetPopupItem     =    m_CurrentPopupItem;
 if (m_RetPopupItem[0]=='#')
    {int pc = m_RetPopupItem.find('#', 1);
     if (pc != -1) m_RetPopupItem = m_RetPopupItem.mid(pc+1);
    }
}
//--------------------------------- GetPopupItemID  --------------------------------------------------
int ThemePopup::GetPopupItemID ()
{return m_CurrentPopupID;
}
