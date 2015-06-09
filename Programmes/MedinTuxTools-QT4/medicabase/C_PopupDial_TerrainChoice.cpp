/**********************************************************************************
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

#include "C_PopupDial_TerrainChoice.h"
#include "ui_C_PopupDial_TerrainChoice.h"
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QTreeWidgetItem>
#include <QMenu>
#include "../../MedinTuxTools-QT4/medicabase/C_LevelItemDelegate.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"


//------------------------------------ C_PopupDial_TerrainChoice -------------------------------------
C_PopupDial_TerrainChoice::C_PopupDial_TerrainChoice(C_Frm_PosologieHitList *pC_Frm_PosologieHitList,
                                                     CMyQLineEdit           *pCMyQLineEdit,
                                                     const QColor            &backGround,
                                                     const QRect             &initialRect,
                                                     const QRect             &deployRect) :
 C_PopupDial(pC_Frm_PosologieHitList, backGround, initialRect, deployRect)
{   setupUi(this);
    m_cur_grammar             = pC_Frm_PosologieHitList->grammar();
    m_pC_Frm_PosologieHitList = pC_Frm_PosologieHitList;
    m_pCMyQLineEdit           = pCMyQLineEdit;
    m_IsPopupRun              = false;
    m_add_Menu                = new ThemePopup(this);
    m_add_Menu->setObjectName("HitList : pushButton_add menu");
    ThemePopup::addOptionMenu(m_add_Menu,tr("Age   en : ans"));
    ThemePopup::addOptionMenu(m_add_Menu,tr("Age   en : mois"));
    ThemePopup::addOptionMenu(m_add_Menu,tr("Age   en : semaines"));
    ThemePopup::addOptionMenu(m_add_Menu,tr("Poids en :  kg"));
    ThemePopup::addOptionMenu(m_add_Menu,tr("Sexe f\303\251minin"));
    ThemePopup::addOptionMenu(m_add_Menu,tr("Sexe masculin"));
    ThemePopup::addOptionMenu(m_add_Menu,tr("Allaitement"));
    QStringList additionnalOptions = m_pC_Frm_PosologieHitList->bitMapCollection()->getFileList("add_PopupVariables.txt");
    for (int i=0; i<additionnalOptions.size(); ++i)
        {ThemePopup::addOptionMenu(m_add_Menu,additionnalOptions.at(i));
        }
    QRect dRect = QRect (deployRect.x(),
                         deployRect.y(),
                         width(),
                         deployRect.height());

    setFont(m_pC_Frm_PosologieHitList->font());

    setRectanngles(initialRect, dRect);
    setToInitialRect(C_PopupDial::FAST);
    connect( this,                    SIGNAL( Sign_deployed (int&)),                   this , SLOT( Slot_Dial_TerrainChoiceDeployed(int&) ));
    connect( this,                    SIGNAL( Sign_minimized (int&)),                  this , SLOT( Slot_Dial_TerrainChoiceMinimized(int&) ));
    connect( pushButton_add,          SIGNAL(clicked ( bool )),                        this,  SLOT( Slot_pushButton_add_clicked ( bool )) );
    connect( pushButton_del,          SIGNAL(clicked ( bool )),                        this,  SLOT( Slot_pushButton_del_clicked ( bool )) );
    connect( treeWidget_Variables,    SIGNAL(itemChanged ( QTreeWidgetItem *, int )),  this,  SLOT( Slot_treeWidget_Variables_itemChanged ( QTreeWidgetItem *, int )) );

}
//------------------------------------ Slot_pushButton_add_clicked --------------------------------------------------------
void C_PopupDial_TerrainChoice::Slot_pushButton_add_clicked ( bool )
{ m_IsPopupRun        =   true;
  QString optionTexte = m_add_Menu->DoPopupList();
  m_IsPopupRun        =   false;
  menuAddVariableTriggered( optionTexte );
}
//------------------------ menuAddVariableTriggered ---------------------------------------
/*
BNP;        unité mg;   code 1821
Azotémie;   unité g/l;  code 591
Kaliémimie; unité g/l;  code 1608
 */
void C_PopupDial_TerrainChoice::menuAddVariableTriggered( const QString &optionTexte  )
{   QString name        = "";
    QString binf        = "";
    QString bsup        = "";
    QString unit        = "";

    if (optionTexte=="Allaitement")
       {  name        = optionTexte;
          binf        = 'A';
          bsup        = 'A';
          unit        = 'A';
       }
    else if (optionTexte.contains("Sexe"))
       {  name        = "Sexe";
          unit        = 'S';
          if (optionTexte.contains("asculin"))
             { binf        = "Masculin";
               bsup        = binf;
             }
          else
             {binf        = tr("f\303\251minin");
              bsup        = binf;
             }
       }
    else
      { QStringList lst = optionTexte.split("en :");
        if (lst.length()<2) return;
        name = lst[0].trimmed();
        unit = lst[1].trimmed();
        binf = "<saisir>";
        bsup = "<saisir>";
      }
    if (name.length()==0) return;
    QTreeWidgetItem   *item  = new QTreeWidgetItem (treeWidget_Variables);
     if (item)
        { item->setText(0,name);                  // libelle
          item->setText(1,binf);                  // borne inferieure
          item->setText(2,bsup);                  // borne superieure
          item->setText(3,unit);                  // unite
          item->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
          treeWidget_Variables->addTopLevelItem(item);
       }
}

//------------------------------------ Slot_pushButton_add_clicked --------------------------------------------------------
void C_PopupDial_TerrainChoice::Slot_Dial_TerrainChoiceMinimized(int &ok)
{if (m_IsPopupRun) ok = 0;
 else              ok = 1;
 Slot_treeWidget_Variables_itemChanged (0, 0 );

}

//------------------------ Slot_MenuAddVariableTriggered ---------------------------------------
/*
-----------------
BNP (NGAP=1821)          en : mg
Azotémie (NGAP= 591)     en : g/l
Kaliémimie (NGAP= 1608)  en : g/l
*/
void C_PopupDial_TerrainChoice::Slot_MenuAddVariableTriggered( QAction *pQAction  )
{QString optionTexte =  pQAction->text();
 menuAddVariableTriggered( optionTexte );
}
//------------------------------------ Slot_treeWidget_Variables_itemChanged --------------------------------------------------------
void C_PopupDial_TerrainChoice::Slot_treeWidget_Variables_itemChanged ( QTreeWidgetItem * item, int column )
{
  Q_UNUSED(item);
  Q_UNUSED(column);
  QString  terrainStr = interfaceToGrammar();
  if (terrainStr.length()==0) return;
  m_pCMyQLineEdit->setFreeData(terrainStr);
  m_pCMyQLineEdit->setText(C_PosologieGrammar::get_terrainReadyDisplayString(terrainStr));
}

//------------------------------------ Slot_pushButton_del_clicked --------------------------------------------------------
void C_PopupDial_TerrainChoice::Slot_pushButton_del_clicked ( bool )
{    QTreeWidgetItem   *item  = treeWidget_Variables->currentItem();
     if (item)   delete item;
     QString  terrainStr = interfaceToGrammar();
     m_pCMyQLineEdit->setFreeData(terrainStr);
     m_pCMyQLineEdit->setText(C_PosologieGrammar::get_terrainReadyDisplayString(terrainStr));
}

//------------------------------------ interfaceToGrammar --------------------------------------------------------
QString C_PopupDial_TerrainChoice::interfaceToGrammar()
{ QString terrain = "";
  int	nb        = treeWidget_Variables->topLevelItemCount();
  for (int i=0; i<nb; ++i)
      { QTreeWidgetItem *item = treeWidget_Variables->topLevelItem(i);
        if (item)
           {
             QString name        = item->text(0);
             QString binf        = item->text(1);
             QString bsup        = item->text(2);
             QString unit        = item->text(3);
             if (name.toLower()=="sexe")
                { QString sexe = "";
                  if       ((binf[0].toLatin1()|32)=='f') sexe = "F";
                  else if  ((binf[0].toLatin1()|32)=='m') sexe = "M";
                  if (sexe.length())
                     { terrain += "  <gph_trl>\n"
                                  "    <gph_trn>"+tr("Sexe")+"</gph_trn><gph_tri>"+sexe+"</gph_tri><gph_trs>"+sexe+"</gph_trs><gph_tru>S</gph_tru>\n"
                                  "  </gph_trl>\n";
                     }
                }
             else if (name.toLower()=="allaitement")
                { terrain += "  <gph_trl>\n"
                             "    <gph_trn>"+tr("Allaitement")+"</gph_trn><gph_tri>A</gph_tri><gph_trs>A</gph_trs><gph_tru>A</gph_tru>\n"
                             "  </gph_trl>\n";
                }
             else
                { if (isNumValid(binf)  && isNumValid(bsup))
                     { terrain +=  "  <gph_trl>\n"
                                   "    <gph_trn>" + name    +"</gph_trn>"               // nom de la variable terrain
                                       "<gph_tri>" + binf    +"</gph_tri>"               // borne inferieure
                                       "<gph_trs>" + bsup    +"</gph_trs>"               // borne superieure
                                       "<gph_tru>" + unit    +"</gph_tru>\n"             // unite des bornes
                                   "  </gph_trl>\n";
                    }
                } // endif (name.toLower()=="sexe")
           } // endif (item)
      } //end for (int i=0; i<nb; ++i)
   return terrain;
}

//------------------------------------ isNumValid --------------------------------------------------------
bool C_PopupDial_TerrainChoice::isNumValid(const QString &borne)
{bool ret = false;
 int i = 0;
 for (i = 0; i<borne.length(); ++i)
     {if ( !((borne[i]>='0' && borne[i]<='9')||borne[i]=='.'||borne[i]==',') ) break;
     }
 ret = (i >= borne.length());
 return   ret;
}

//------------------------------------ grammarToInterface --------------------------------------------------------
void C_PopupDial_TerrainChoice::grammarToInterface()
{   QString terrainStr      = m_pCMyQLineEdit->getFreeData();
    QStringList terrainList = C_PosologieGrammar::get_TerrainList(terrainStr);
    QTreeWidgetItem   *item = 0;
    treeWidget_Variables->clear();
    for (int i = 0; i < terrainList.size(); ++i )
        { int     nextPos_t   = 0;
          QString terrainLine = terrainList[i];
          QString name        = CGestIni::getXmlData("gph_trn", terrainLine, &nextPos_t);
          QString binf        = CGestIni::getXmlData("gph_tri", terrainLine, &nextPos_t);
          QString bsup        = CGestIni::getXmlData("gph_trs", terrainLine, &nextPos_t);
          QString unit        = CGestIni::getXmlData("gph_tru", terrainLine, &nextPos_t);

          /*
          if (unit=="S")      terrain += QObject::tr(" %1 : %2;").arg( name, (bsup=="F")?QObject::tr("F\303\251minin"):QObject::tr("Masculin") );
          else if (unit=="A") terrain += " "+name +"; ";
          else                terrain += QObject::tr( "%1 : de %2 \303\240 %3 %4;").arg( name, binf, bsup, unit );
          */
          item                = new QTreeWidgetItem (treeWidget_Variables);
          if (item)
             {
               item->setText(0,name);    // libelle
               item->setText(1,binf);    // borne inferieure
               item->setText(2,bsup);    // borne superieure
               item->setText(3,unit);    // unite
               item->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
             }
       }
}

//------------------------------------ setFont --------------------------------------------------------
void C_PopupDial_TerrainChoice::setFont(const QFont &fnt)
{
 C_PopupDial::setFont(fnt);
 m_fGrd  =  fnt;
 m_add_Menu->setFont(m_fGrd);
 pushButton_add->setFont(m_fGrd);
 pushButton_del->setFont(m_fGrd);
 treeWidget_Variables->setFont(m_fGrd);
 label_title->setFont(m_fGrd);
}
//----------------------------------- ~C_PopupDial_IndicationChoice -------------------------------------------
C_PopupDial_TerrainChoice::~C_PopupDial_TerrainChoice()
{
}
//------------------------------------------------------ Slot_DialDateDeployed ----------------------------------------------------------------------
void C_PopupDial_TerrainChoice::Slot_Dial_TerrainChoiceDeployed(int &ok)
{
    ok=1;
    pushButton_add->setIcon( Theme::getIcon("list-add.png"));
    pushButton_del->setIcon( Theme::getIcon("list-remove.png"));
    grammarToInterface();
}
