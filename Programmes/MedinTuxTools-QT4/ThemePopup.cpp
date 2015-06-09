 /********************************* ThemePopup.cpp ********************************
 *  #include "ThemePopup.h"                                                       *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *                from Eric MAEKER <eric.maeker.fr> original code                 *
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
#include <QCursor>
#include <QMenu>
#include <QDebug>
#include "ThemePopup.h"

//--------------------------------- ThemePopup -----------------------------------------------------
ThemePopup::ThemePopup(QWidget * parent/*=0*/ , const QString & style /*=""*/)
 : QMenu(parent)
{if (style.length()) setStyleSheet(style);
 if (Theme::getPath()!="")
  { QFont f = QMenu::font();
    int size = Theme::getFontSize_Popups();
    if (size != -1)
       {f.setPointSize( Theme::getFontSize_Popups() );
        QMenu::setFont( f );
       }
  }
}
//------------------------ addOptionMenu ---------------------------------------
void ThemePopup::addOptionMenu(QMenu* pQMenu, const QString &optionsTitle)
{   if (optionsTitle.startsWith("----"))
       { pQMenu->addSeparator();
       }
    else
       { QAction *pQAction = 0;
         QString      num  = "";
         QString    title  = optionsTitle;
         int pos           = title.indexOf('-');
         if (pos != -1)
            { num   = title.left(pos).trimmed();
              title = title.mid(pos+1).trimmed();
            }
         pQAction = pQMenu->addAction(title);
         pQAction->setObjectName( tr("%1 menu ").arg(pQMenu->objectName()+"@"+title));
         pQAction->setData(num);
       }
}
//--------------------------------- ThemePopup -----------------------------------------------------
// une option de sous menu <ceci est une option>
// un indentifiant specifique =1=
// une icone #nom de l'icone#
ThemePopup::ThemePopup(QStringList &list, QWidget * parent/*=0*/ , const QString & style /*=""*/, const QString& sep /*= "|"*/, int toMask /*= 0*/ )
 : QMenu(parent)
{QStringList subList;
 //......... relever les listes de sous menus .......................................
 QMap<QString, QStringList> subPopupMap;
 for ( QStringList::Iterator it =list.begin(); it != list.end(); ++it )
     {QString option    = *(it);
      QString nameSub   = "";
      if (option.startsWith('<'))
         {int i = option.indexOf('>');
          if (i==-1) continue;
          nameSub             = option.mid(1,i-1);
          option              = option.mid(i+1);
          subList             = subPopupMap[nameSub];
          subList.append(option);
          subPopupMap[nameSub]=subList;
         }
     }
 if (style.length()) setStyleSheet(style);
 if (Theme::getPath()!="")
  { QFont f  = QMenu::font();
    int size = Theme::getFontSize_Popups();
    if (size != -1)
       {f.setPointSize( Theme::getFontSize_Popups() );
        QMenu::setFont( f );
       }
  }
 //............................... chercher une zone de liste deroulante (bleue) .....................
 //       resolution du probleme des icones sous ubuntu 10.10
 // I found out from fedora documentation that Gnome >= 2.28 does not display menu icons as a default.
 // These commands fixed the issue:
 // gconftool-2 --type boolean --set /desktop/gnome/interface/buttons_have_icons true
 // gconftool-2 --type boolean --set /desktop/gnome/interface/menus_have_icons true

 for ( QStringList::Iterator it =list.begin(); it != list.end(); ++it )
     { QString  nameSub     = "";
       QString  toInsert    = *(it);
       QAction *pQAction    = 0;
       QString  identifiant = getIconIdentifiantOption(toInsert);
       QIcon    icon        = getIconFromOption(toInsert);

       if (toInsert.startsWith('<')) continue; // c'est une sous option donc ne pas inclure ici
       //.... c'est une entree de sous menu la creer ............
       if (toInsert.startsWith('>'))
          {int i = toInsert.indexOf('<');
           if (i==-1)             continue;
           nameSub             = toInsert.mid(1,i-1);
           toInsert            = toInsert.mid(i+1);
           subList             = subPopupMap[nameSub];
           if (subList.size()<=0) continue;

           ThemePopup *subPopup = new ThemePopup(subList, this );
           pQAction = addMenu ( subPopup );
           subPopup->setTitle(toInsert);
           pQAction->setText(toInsert);
           pQAction->setIcon(icon);
           if ( identifiant.length() ) pQAction->setData ( identifiant );
           continue;
          }

       if (sep.length())        // si chaque item de la liste est compose de plusieurs termes separes par un separateur
          {if (toMask > 0)      // si l'on doit masquer les champs situes apres toMask
              {int   i = 0;
               int pos = 0;
               while ( i<toMask && (pos = toInsert.indexOf(sep, pos + sep.length())) != -1)
                 {++i;
                 };
               if (pos != -1) toInsert = toInsert.left(pos);
             }
           toInsert    = toInsert.replace(sep," ");   // on ne va pas afficher les separateurs dans le menu
         }

       if (toInsert[0]=='$')                 setTitle(toInsert.mid(1));
       else if (toInsert.left(4) == "----")  pQAction = addSeparator();
       else
          { pQAction = addAction( icon, toInsert );
            if ( identifiant.length() )
               { pQAction->setData ( identifiant );
               }
          }
     }
}
//--------------------------------- getIconFromOption -----------------------------------------------------
QIcon ThemePopup::getIconFromOption(QString & optionText)
{   QIcon   retIcon;
    QString iconName = "";
    int deb  = optionText.indexOf('#');
    if (deb == -1) return retIcon;
    int end  = optionText.indexOf('#',deb+1);
    if (end == -1) return retIcon;
    iconName = optionText.mid(deb+1, end-deb-1);
    retIcon  = Theme::getIconSet(iconName);
    optionText = optionText.remove(deb,end+1-deb);
    return retIcon;
}
//--------------------------------- getIconIdentifiantOption -----------------------------------------------------
QString ThemePopup::getIconIdentifiantOption(QString & optionText)
{   QString identifiant = "";
    int deb  = optionText.indexOf('=');
    if (deb == -1) return identifiant;
    int end  = optionText.indexOf('=',deb+1);
    if (end == -1) return identifiant;
    identifiant = optionText.mid(deb+1, end-deb-1);
    optionText  = optionText.remove(deb,end+1-deb);
    return identifiant;
}
//--------------------------------- DoPopupList -----------------------------------------------------
QString ThemePopup::DoPopupList(const QString & style/*=""*/)
{
 if (style.length()) setStyleSheet(style);
 QAction *pQAction  =  exec( QCursor::pos() /* mapToGlobal (  QPoint ( x, y) )*/ );
 if (pQAction)
    {QString identifiant = pQAction->data().toString();
     if (identifiant.length()) return identifiant;
     else                      return pQAction->text();
    }
 else          return QString::null;
}

