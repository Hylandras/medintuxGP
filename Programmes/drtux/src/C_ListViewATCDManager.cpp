//
// C++ Implementation: C_ListViewATCDManager
//
// Description:
//
//
// Author: Sevin Roland <upsis@club-internet.fr>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "C_ListViewATCDManager.h"
#include <qobject.h>
#include <qmessagebox.h>
#include "CApp.h"

//------------------------------------ C_ListViewATCDManager --------------------------------------------------
/*! \brief constructeur
*/
C_ListViewATCDManager::C_ListViewATCDManager(QObject * parent, const char * name , QListView*   pQListView, Atcd_Code* pAtcd_Code, int *pIsModified, int *pIsModifiable, QString *pTypeATCD_Selectionne)
: QObject ( parent, name )
{m_pQListView             = pQListView;
 m_pAtcd_Code             = pAtcd_Code;
 m_pIsModified            = pIsModified;
 m_pIsModifiable          = pIsModifiable;
 m_pTypeATCD_Selectionne  = pTypeATCD_Selectionne;
}

//------------------------------------ Add_popMenu_ATCD_Type --------------------------------------------------
/*! \brief Ajoute le menu de selection du type d'ant\303\251cedent �  un menu quelconque
*/
void C_ListViewATCDManager::Add_popMenu_ATCD_Type(QPopupMenu* pQPopupMenu)
{connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(ATCD_MenuActionSetFamilleGenre()));
 G_pCApp->addPopupHierarchique(G_pCApp->m_PathAppli + "Ressources/MenuATCD.txt", pQPopupMenu, m_pTypeATCD_Selectionne);
}

//------------------------------------ m_pQListView_contextMenuRequested --------------------------------------------------
/*! \brief action bouton droit sur la listeview ATCD
*/

   // 0 Libelle
   // 1 famille Genre
   // 2 etat Actif/gueri
   // 3 commentaire
   // 4 date
   // 5 ald
   // 6 code

void C_ListViewATCDManager::Slot_listView_ATCD_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &pos, int c )
{
 if (*m_pIsModifiable==0)   return;
*m_pTypeATCD_Selectionne    = "";
 ThemePopup *pQPopupMenu    = 0;
 QString ret                = "";
 QStringList optionList;
 switch(c)
 {case LV_NAME:  // libelle
     {
      ThemePopup *pThemePopup   = new ThemePopup((QWidget*)this->parent(), "MyPopupMenu" );
      //pThemePopup->insertItem(Theme::getIcon( "Cim10All_Icon.png"), tr("Ajouter un  Ant\303\251c\303\251dent CIM10"),                this, SLOT( ATCD_MenuActionNewCIM10()),       CTRL+Key_M );
      //pThemePopup->insertItem(G_pCApp->m_IconTherapeutique, tr("Ajouter une intol\303\251rance m\303\251dicamenteuse"),              this, SLOT( ATCD_MenuActionNewAllergie()),    CTRL+Key_N );
      pThemePopup->insertItem(Theme::getIconListItem(), tr("Ajouter un ant\303\251c\303\251dent texte libre"),                       this, SLOT( ATCD_MenuActionNewTexteLibre()),  CTRL+Key_T );
      if (pQListViewItem!=0)
        {// Construit le menu g\303\251n\303\251ral
         pThemePopup->insertSeparator ();
         pThemePopup->insertItem( Theme::getIconListDelete() , tr("Effacer les ant\303\251c\303\251dents s\303\251lectionn\303\251s"),               this, SLOT( ATCD_MenuActionMultiDel()),       CTRL+Key_F );
         pThemePopup->insertSeparator ();
         pThemePopup->insertItem( Theme::getIconListDateTime(),        tr("Modifier la date de d\303\251but de cet Ant\303\251c\303\251dent"),       this, SLOT( ATCD_MenuActionSetDateDeb()),        CTRL+Key_G );
         pThemePopup->insertItem( Theme::getIcon("16x16/timeDel.png"), tr("Effacer la date de d\303\251but de cet Ant\303\251c\303\251dent"),        this, SLOT( ATCD_MenuActionDelDateDeb()),        CTRL+Key_G );
         pThemePopup->insertItem( Theme::getIconListDateTime(),        tr("Modifier la date de fin de cet Ant\303\251c\303\251dent"),                this, SLOT( ATCD_MenuActionSetDateFin()),        CTRL+Key_G );
         pThemePopup->insertItem( Theme::getIcon("16x16/timeDel.png"), tr("Effacer la date de fin de cet Ant\303\251c\303\251dent"),                 this, SLOT( ATCD_MenuActionDelDateFin()),        CTRL+Key_G );
         pThemePopup->insertSeparator ();
         Add_popMenu_ATCD_Type(pThemePopup);
         pThemePopup->insertSeparator ();
         if (pQListViewItem->text(LV_COMM).length())
            { pThemePopup->insertItem( Theme::getIcon("16x16/commentaire.png"),  pQListViewItem->text(LV_COMM),                   this, SLOT( ATCD_MenuActionSetCommentaire()), CTRL+Key_K );
            }
         else
            { pThemePopup->insertItem( Theme::getIcon("16x16/commentaire.png"),  tr("D\303\251finir un commentaire"),             this, SLOT( ATCD_MenuActionSetCommentaire()), CTRL+Key_K );
            }
        pThemePopup->insertSeparator ();
        pThemePopup->insertItem( Theme::getIcon("16x16/ald_on.png"),        tr("En rapport avec une pathologie ALD"),         this, SLOT( ATCD_setAldOn()),         CTRL+Key_L );
        pThemePopup->insertItem( Theme::getIcon("16x16/sport_on.png"),      tr("En rapport avec une activité sportive"),      this, SLOT( ATCD_setSportOn()),       ALT+Key_S );
        pThemePopup->insertItem( Theme::getIcon("16x16/vigilance_on.png"),  tr("En rapport avec une vigilance intacte"),      this, SLOT( ATCD_setVigilanceOn()),   ALT+Key_V );
        pThemePopup->insertItem( Theme::getIcon("16x16/ald_off.png"),       tr("Sans rapport ALD ou activité sportive"),      this, SLOT( ATCD_setAldOff()),        CTRL+Key_O );
        }
      pThemePopup->exec(pos);
      delete pThemePopup;
      //m_pQListViewItem = 0;
     }
     break;
  case LV_TYPE:         // med/chir
     {pQPopupMenu   = new ThemePopup((QWidget*)this->parent(), "MyPopupMenu" );
      Add_popMenu_ATCD_Type(pQPopupMenu);
      pQPopupMenu->exec(QCursor::pos());
      delete pQPopupMenu;
     }
     break;
  case LV_DFIN:         // date de fin
     {ThemePopup *pThemePopup   = new ThemePopup((QWidget*)this->parent(), "MyPopupMenu" );
      pThemePopup->insertItem( Theme::getIconListDateTime(),        tr("Modifier la date de fin de cet Ant\303\251c\303\251dent"),                this, SLOT( ATCD_MenuActionSetDateFin()),        CTRL+Key_G );
      pThemePopup->insertItem( Theme::getIcon("16x16/timeDel.png"), tr("Effacer la date de fin de cet Ant\303\251c\303\251dent"),                 this, SLOT( ATCD_MenuActionDelDateFin()),        CTRL+Key_G );
      pThemePopup->exec(QCursor::pos());
      delete pThemePopup;
     }
     break;
  case LV_COMM:         // commentaire
     {ATCD_MenuActionSetCommentaire();
     }
     break;
  case LV_DDEB:
     {ThemePopup *pThemePopup   = new ThemePopup((QWidget*)this->parent(), "MyPopupMenu" );
      pThemePopup->insertItem( Theme::getIconListDateTime(),        tr("Modifier la date de d\303\251but de cet Ant\303\251c\303\251dent"),       this, SLOT( ATCD_MenuActionSetDateDeb()),        CTRL+Key_G );
      pThemePopup->insertItem( Theme::getIcon("16x16/timeDel.png"), tr("Effacer la date de d\303\251but de cet Ant\303\251c\303\251dent"),        this, SLOT( ATCD_MenuActionDelDateDeb()),        CTRL+Key_G );
      pThemePopup->exec(QCursor::pos());
      delete pThemePopup;
     }
     break;
  case LV_ALSP:         // ALD
     {ThemePopup *pThemePopup   = new ThemePopup((QWidget*)this->parent(), "MyPopupMenu" );
      pThemePopup->insertItem( Theme::getIcon("16x16/ald_on.png"),        tr("En rapport avec une pathologie ALD"),         this, SLOT( ATCD_setAldOn()),         ALT+Key_A );
      pThemePopup->insertItem( Theme::getIcon("16x16/sport_on.png"),      tr("En rapport avec une activité sportive"),      this, SLOT( ATCD_setSportOn()),       ALT+Key_S );
      pThemePopup->insertItem( Theme::getIcon("16x16/vigilance_on.png"),  tr("En rapport avec une vigilance intacte"),      this, SLOT( ATCD_setVigilanceOn()),   ALT+Key_V );
      pThemePopup->insertItem( Theme::getIcon("16x16/ald_off.png"),       tr("Sans rapport ALD ou activité sportive"),      this, SLOT( ATCD_setAldOff()),        ALT+Key_O );
      pThemePopup->exec(QCursor::pos());
      delete pThemePopup;
     }
     break;
  case LV_CODE:          // code de l'antecedent
     {listView_ATCD_doubleClicked( pQListViewItem);
     }
     break;

 }
}
//------------------------------------ ATCD_setAldOn --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur en rapport avec les ALD
*/
void C_ListViewATCDManager::ATCD_setAldOn()
{ATCD_setAldOnOff(tr("ALD"));
}
//------------------------------------ ATCD_setSportOn --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur en rapport avec une activite sportive
*/
void C_ListViewATCDManager::ATCD_setSportOn()
{ATCD_setAldOnOff(tr("Sport"));
}

//------------------------------------ ATCD_setVigilanceOn --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur en rapport avec une vigilance
*/
void C_ListViewATCDManager::ATCD_setVigilanceOn()
{ATCD_setAldOnOff(tr("Vigilance"));
}
//------------------------------------ ATCD_setAldOff --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur sans rapport avec les ALD
*/
void C_ListViewATCDManager::ATCD_setAldOff()
{ATCD_setAldOnOff(tr(""));
}

//------------------------------------ ATCD_setAldOn --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur en/sans rapport avec les ALD selon le parametre state "":"ALD"
*/
void C_ListViewATCDManager::ATCD_setAldOnOff(const QString &state)
{QListViewItemIterator it( m_pQListView );
 while ( it.current() )
    {
      CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
      if (pCPrt->isSelected())
         {if (m_pAtcd_Code)
             { Atcd_Element* pAtcd_Element_Selected = m_pAtcd_Code->getAtcd_byID( pCPrt->get_ID_Rublist().toInt() );
               if (pAtcd_Element_Selected) m_pAtcd_Code->setALD(pAtcd_Element_Selected, state, Atcd_Code::sendNotModifMessage);
             }
          pCPrt->setText( LV_ALSP, state );
          pCPrt->setPixmap(LV_TYPE, Atcd_Code::ald_sport_codeToPixmap(state) );
          *m_pIsModified = 1;
         }
      ++it;
    }
}

//------------------------------------ listView_ATCD_doubleClicked -------------------------------------------
/*! \brief Fonction qui appelle Atcd_Code::modifyAtcd() pour la modification de l'ant\303\251c\303\251dent s\303\251lectionn\303\251. Très efficace !
*/
void C_ListViewATCDManager::listView_ATCD_doubleClicked( QListViewItem * pQListViewItem)
{
 if (*m_pIsModifiable==0)        return;
 if (!m_pAtcd_Code)              return;
 m_pAtcd_Code->modifyAtcd((QWidget*)this, get_Selected_Atcd_Element((CPrtQListViewItem*)pQListViewItem));
}

//------------------------------------ ATCD_MenuActionSetDateDeb --------------------------------------------------
/*! \brief Modifie la date de l'ATCD s\303\251lectionn\303\251 par le biais de la classe Atcd_Code
*/
void C_ListViewATCDManager::ATCD_MenuActionSetDateDeb()
{
  if (*m_pIsModifiable==0)        return;
  if (!m_pAtcd_Code)              return;
  Atcd_Element        *pAtcd_Element      = get_Selected_Atcd_Element();
  CPrtQListViewItem   *pCPrtQListViewItem = get_CurrentItem();
  if (pCPrtQListViewItem == 0)    return;

  QDate ret = ATCD_selectDate(pCPrtQListViewItem->text(LV_DDEB), TR("Date de d\303\251but de l'ant\303\251c\303\251dent"));

  if ( ret.isValid() )
     { if (pAtcd_Element)
          {m_pAtcd_Code->setDateDeb(pAtcd_Element, ret, Atcd_Code::sendNotModifMessage);
          }
       pCPrtQListViewItem->setText(LV_DDEB, ret.toString("dd-MM-yyyy"));
       pCPrtQListViewItem->setPixmap(LV_DDEB, Atcd_Code::datesDebFinToPixmap(ret, CGenTools::dd_MM_yyyy_ToIsoDate(pCPrtQListViewItem->text(LV_DFIN))) );
     }
}
//------------------------------------ ATCD_MenuActionSetDateFin --------------------------------------------------
/*! \brief Modifie la date de l'ATCD s\303\251lectionn\303\251 par le biais de la classe Atcd_Code
*/
void C_ListViewATCDManager::ATCD_MenuActionSetDateFin()
{
  if (*m_pIsModifiable==0)        return;
  if (!m_pAtcd_Code)              return;
  Atcd_Element        *pAtcd_Element      = get_Selected_Atcd_Element();
  CPrtQListViewItem   *pCPrtQListViewItem = get_CurrentItem();
  if (pCPrtQListViewItem == 0)    return;

  QDate ret = ATCD_selectDate(pCPrtQListViewItem->text(LV_DFIN), TR("Date de fin de l'ant\303\251c\303\251dent"));

  if ( ret.isValid() )
     { if (pAtcd_Element)
          {m_pAtcd_Code->setDateFin(pAtcd_Element, ret, Atcd_Code::sendNotModifMessage);
          }
       pCPrtQListViewItem->setText(LV_DFIN, ret.toString("dd-MM-yyyy"));
       pCPrtQListViewItem->setPixmap(LV_DDEB, Atcd_Code::datesDebFinToPixmap(CGenTools::dd_MM_yyyy_ToIsoDate(pCPrtQListViewItem->text(LV_DDEB)),ret) );
     }
}
//------------------------------------ ATCD_selectDate --------------------------------------------------
/*! \brief selectionne une date de l'ATCD s\303\251lectionn\303\251 par le biais de la classe Atcd_Code
*/
QDate C_ListViewATCDManager::ATCD_selectDate(const QString &date, const QString &title)
{
  QDate retDate = QDate();
  Dlg_Calendar* dlg  = new Dlg_Calendar((QWidget*)this->parent(), "Calendar_Dial", FALSE);
  if (dlg==0)                     return retDate;
  dlg->setCaption(title);
  int isInvalid     = 0;
  QDate          dt = CGenTools::setDate(date, isInvalid);
  if (isInvalid) dt = QDate::currentDate();
  dlg->setDate(dt);
  dlg->setComboAnOnDate_Offset(dt, 25,0);
  dlg->SelectDateAll();

  if (dlg->exec()== QDialog::Accepted )
     {retDate =   dlg->getDate();
     }
 if (dlg) delete dlg;
 return retDate;
}
//------------------------------------ ATCD_MenuActionDelDateDeb --------------------------------------------------
/*! \brief Efface la date de l'ATCD s\303\251lectionn\303\251 par le biais de la classe Atcd_Code
*/
void C_ListViewATCDManager::ATCD_MenuActionDelDateDeb()
{
  if (*m_pIsModifiable==0)     return;
  if (!m_pAtcd_Code)           return;
 //................. parcourir tous les items ............
 QListViewItemIterator it( m_pQListView );
 while ( it.current() )
    {
      CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
      if (pCPrt->isSelected())
         {if (m_pAtcd_Code)
              {Atcd_Element* pAtcd_Element_Selected = m_pAtcd_Code->getAtcd_byID( pCPrt->get_ID_Rublist().toInt() );
               if (pAtcd_Element_Selected) m_pAtcd_Code->deleteDateDeb(pAtcd_Element_Selected, Atcd_Code::sendNotModifMessage);
              }
          pCPrt->setText( LV_DDEB,"" );
          pCPrt->setPixmap(LV_DDEB, Atcd_Code::datesDebFinToPixmap(CGenTools::dd_MM_yyyy_ToIsoDate(pCPrt->text(LV_DDEB)), CGenTools::dd_MM_yyyy_ToIsoDate(pCPrt->text(LV_DFIN))) );
          *m_pIsModified = 1;
         }
      ++it;
    }
}
//------------------------------------ ATCD_MenuActionDelDateFin --------------------------------------------------
/*! \brief Efface la date de l'ATCD s\303\251lectionn\303\251 par le biais de la classe Atcd_Code
*/
void C_ListViewATCDManager::ATCD_MenuActionDelDateFin()
{
  if (*m_pIsModifiable==0)     return;
  if (!m_pAtcd_Code)           return;
 //................. parcourir tous les items ............
 QListViewItemIterator it( m_pQListView );
 while ( it.current() )
    {
      CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
      if (pCPrt->isSelected())
         {if (m_pAtcd_Code)
              {Atcd_Element* pAtcd_Element_Selected = m_pAtcd_Code->getAtcd_byID( pCPrt->get_ID_Rublist().toInt() );
               if (pAtcd_Element_Selected) m_pAtcd_Code->deleteDateFin(pAtcd_Element_Selected, Atcd_Code::sendNotModifMessage);
              }
          pCPrt->setText( LV_DFIN,"" );
          pCPrt->setPixmap(LV_DDEB, Atcd_Code::datesDebFinToPixmap(CGenTools::dd_MM_yyyy_ToIsoDate(pCPrt->text(LV_DDEB)), CGenTools::dd_MM_yyyy_ToIsoDate(pCPrt->text(LV_DFIN))) );
          *m_pIsModified = 1;
         }
      ++it;
    }
}

//------------------------------------ ATCD_MenuActionNewCIM10 --------------------------------------------------
/*! \brief Ajoute un ATCD CIM 10 par le biais de Atcd_Code de CApp
*/
void C_ListViewATCDManager::ATCD_MenuActionNewCIM10()
{ if (*m_pIsModifiable==0) return;
  if (!m_pAtcd_Code)       return;
  m_pAtcd_Code->addATCD_CIM10();
}
//------------------------------------ ATCD_MenuActionNewAllergie ---------------------------------------------------
/*! \brief Ajoute un ATCD allergique par le biais de Atcd_Code de CApp
*/
void C_ListViewATCDManager::ATCD_MenuActionNewAllergie()
{ if (*m_pIsModifiable==0) return;
  if (!m_pAtcd_Code)       return;
  m_pAtcd_Code->addATCD_Allergie();
}
//------------------------------------ ATCD_MenuActionNewTexteLibre --------------------------------------------------
/*! \brief Ajoute un ATCD texte libre par le biais de Atcd_Code de CApp
*/
void C_ListViewATCDManager::ATCD_MenuActionNewTexteLibre()
{ if (*m_pIsModifiable==0)         return;
  if (!m_pAtcd_Code)               return;
  //......... si item atcd rajoute rajouter celui de la listView allant avec ......................
  Atcd_Element*        pAtcd_Element      = m_pAtcd_Code->get_lastElementAdded();
  if (pAtcd_Element==0)
     { CPrtQListViewItem* pCPrtQListViewItem  = new CPrtQListViewItem ( m_pQListView, 0,  "");
       ATCD_EditListViewItem(pCPrtQListViewItem);
    }
 else
    {  CPrtQListViewItem*   pCPrtQListViewItem = get_CurrentItem();
       if (pCPrtQListViewItem==0)       return;
       m_pAtcd_Code->addATCD_Textuel((QWidget*)this, Atcd_Code::sendNotModifMessage);    // si OK met a jour le dernier element rajoute
       m_pAtcd_Code->atcd_Element_To_ListViewItem(*pAtcd_Element, m_pQListView);
    }
}

//------------------------------------ ATCD_EditListViewItem --------------------------------------------------
/*! \brief Ajoute un ATCD texte libre par le biais de Atcd_Code de CApp
*/
int C_ListViewATCDManager::ATCD_EditListViewItem(CPrtQListViewItem* pCPrtQListViewItem)
{
       DlgAtcd_txt* pDlgAtcd_txt = new DlgAtcd_txt((QWidget*)this->parent());
       if (!pDlgAtcd_txt)          return 0;
       pDlgAtcd_txt->setComboOnValue(pDlgAtcd_txt->m_Combo_Family,tr("M\303\251dicaux"));
       pDlgAtcd_txt->exec();
       if (pDlgAtcd_txt->result() == QDialog::Accepted)
          { // R\303\251cupère les donn\303\251es du widget set_Atcd_Element(rubrique, libelle,  dt,  code, etat,  commentaire, ald,  -1);
            QString libelle, commentaire, familleGenre, dateDeb, dateFin, ald_sport;
            QDate dt_deb ;
            QDate dt_fin ;
            pDlgAtcd_txt->getInfos(libelle, familleGenre, dateDeb, dateFin, commentaire);
            if (dateDeb.length()) dt_deb = QDate::fromString(dateDeb, Qt::TextDate);
            else                  dt_deb = QDate();
            if (dateFin.length()) dt_fin = QDate::fromString(dateFin, Qt::TextDate);
            else                  dt_fin = QDate();
            ald_sport  = pDlgAtcd_txt->getSate_Ald_Sport();
            pCPrtQListViewItem->setText(LV_NAME,libelle);
            pCPrtQListViewItem->setText(LV_TYPE,familleGenre);
            pCPrtQListViewItem->setText(LV_DFIN,dt_fin.toString("dd-MM-yyyy"));
            pCPrtQListViewItem->setText(LV_COMM,commentaire);
            pCPrtQListViewItem->setText(LV_DDEB,dt_deb.toString("dd-MM-yyyy"));
            pCPrtQListViewItem->setText(LV_ALSP,ald_sport);

            pCPrtQListViewItem->setPixmap (LV_NAME, Atcd_Element::getPixmap("") );       // le texte libre est sans code
            pCPrtQListViewItem->setPixmap (LV_TYPE, Atcd_Code::ald_sport_codeToPixmap(ald_sport) );
            pCPrtQListViewItem->setPixmap (LV_DFIN, Atcd_Code::datesDebFinToPixmap(dt_deb, dt_fin) );
            delete pDlgAtcd_txt;
            return 1;
          }
     delete pDlgAtcd_txt;
     return 0;
}

//------------------------------------ get_Selected_Atcd_Element --------------------------------------------------
/*! \brief recupere un ATCD a partir de l'item selectionne dans la listview
*/
Atcd_Element* C_ListViewATCDManager::get_Selected_Atcd_Element(CPrtQListViewItem *pCPrtQListViewItem /*=0*/)
{if (pCPrtQListViewItem==0) pCPrtQListViewItem = get_CurrentItem();
 if (pCPrtQListViewItem==0) return 0;
 Atcd_Element* pAtcd_Element_Selected = m_pAtcd_Code->getAtcd_byID( pCPrtQListViewItem->get_ID_Rublist().toInt() );
 return pAtcd_Element_Selected;
}
//------------------------------------ get_CurrentItem --------------------------------------------------
/*! \brief recuperel'item selectionne dans la listview
*/
CPrtQListViewItem * C_ListViewATCDManager::get_CurrentItem()
{CPrtQListViewItem *pCPrtQListViewItem =  (CPrtQListViewItem*) m_pQListView->currentItem ();
 if (pCPrtQListViewItem) return pCPrtQListViewItem;
 return  (CPrtQListViewItem*) m_pQListView->selectedItem ();
}

//------------------------------------ ATCD_MenuActionMultiDel --------------------------------------------------
/*! \brief Efface un ATCD par le biais de Atcd_Code de CApp
*/
void C_ListViewATCDManager::ATCD_MenuActionMultiDel()
{ATCD_MenuActionMultiDel( Atcd_Code::sendNotModifMessage);
}

//------------------------------------ ATCD_MenuActionMultiDel --------------------------------------------------
void C_ListViewATCDManager::ATCD_MenuActionMultiDel(int sendModifMessage)
{//m_pQListViewItem->setSelected (TRUE);   // reselectionner l'item au dessus duquel se retrouve la souris
 //............... liste des items � effacer..................................
 //                comme elle est autodelete ils s'effaceront automatiquement
 //                a la fin lors destruction de l'objet : listItemsToDelete
 //                avec le raffraichissement graphique qui va avec
 QPtrList<CPrtQListViewItem> listItemsToDelete;
 listItemsToDelete.setAutoDelete( TRUE );
 //............... le disconnect fait que le raffraichissement de la listview nous incombe .......................
 //                et ne sera pas fait par :  m_pAtcd_Code->eraseAtcd(pAtcd_Element_Selected);
 if (m_pAtcd_Code) disconnect(m_pAtcd_Code, SIGNAL( ATCD_Changed() ), this, SLOT( ATCD_HaveChanged() ));
 //................. parcourir tous les items ............
 QListViewItemIterator it( m_pQListView );
 while ( it.current() )
    {
      CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
      if (pCPrt->isSelected())
         {if (m_pAtcd_Code)
              {Atcd_Element* pAtcd_Element_Selected = m_pAtcd_Code->getAtcd_byID( pCPrt->get_ID_Rublist().toInt() );
               if (pAtcd_Element_Selected) m_pAtcd_Code->eraseAtcd(pAtcd_Element_Selected, sendModifMessage);
              }
          listItemsToDelete.append(pCPrt);
          *m_pIsModified = 1;
         }
      ++it;
    }
}

//------------------------------------ ATCD_MenuActionSetFamilleGenre --------------------------------------------------
/*! \brief Modifie la Famille et Genre d'un ATCD par le biais de la classe Atcd_Code.
*/
void C_ListViewATCDManager::ATCD_MenuActionSetFamilleGenre()
{disconnect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(ATCD_MenuActionSetFamilleGenre()));
 if (*m_pIsModifiable==0)          return;
 if (!m_pAtcd_Code)                return;
 //................. parcourir tous les items ............
 QListViewItemIterator it( m_pQListView );
 while ( it.current() )
    {
      CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
      if (pCPrt->isSelected())
         {if (m_pAtcd_Code)
              {Atcd_Element* pAtcd_Element_Selected = m_pAtcd_Code->getAtcd_byID( pCPrt->get_ID_Rublist().toInt() );
               if (pAtcd_Element_Selected) m_pAtcd_Code->setRubrique(pAtcd_Element_Selected, *m_pTypeATCD_Selectionne, Atcd_Code::sendNotModifMessage);
              }
          pCPrt->setText( LV_TYPE, *m_pTypeATCD_Selectionne);
          *m_pIsModified = 1;
         }
      ++it;
    }
}

//------------------------------------ ATCD_MenuActionSetCommentaire --------------------------------------------------
void C_ListViewATCDManager::ATCD_MenuActionSetCommentaire()
{if (*m_pIsModifiable==0)           return;
 if (!m_pAtcd_Code)                 return;
 CPrtQListViewItem* pCPrtQListViewItem = get_CurrentItem();
 if (pCPrtQListViewItem == 0)       return;
 QString         commentaire = pCPrtQListViewItem->text(LV_COMM);
 Atcd_Element* pAtcd_Element = get_Selected_Atcd_Element();
 if (pAtcd_Element == 0)
    {  if (CGenTools::DialogGetString(0, TR("Saisir un commentaire (mode texte libre)") , commentaire) == QDialog::Accepted)
          {pCPrtQListViewItem->setText(LV_COMM,commentaire);
          }
    }
 else
    { m_pAtcd_Code->changeCommentaire(pAtcd_Element, Atcd_Code::sendNotModifMessage);
      pCPrtQListViewItem->setText(LV_COMM, pAtcd_Element->m_Commentaire);
    }
}

//------------------------------------ pushButtonDellAllATCD_clicked --------------------------------------------------
void C_ListViewATCDManager::dellAllSelectedATCD()
{if (*m_pIsModifiable==0) return;
 QStringList optList;
 optList.append(tr ("Effacer les ant\303\251c\303\251dents s\303\251lectionn\303\251s"));
 optList.append(tr ("Effacer tous les ant\303\251c\303\251dents"));
 QString result = G_pCApp->DoPopupList(optList);
 if (result.length() == 0) return;

 if (result.find("Effacer tous")!= -1)
    {int ret = QMessageBox::warning ( 0, tr ("Effacer tous les ant\303\251c\303\251dents "),
                                        tr ("Voulez vous vraiment effacer\r\n"
                                            "toutes les donn\303\251es des ant\303\251c\303\251dents\r\n"
                                            "CETTE ACTION EST IRREVERSIBLE"),
                                        tr ("&Effacer"), tr("Annu&ler"), 0,
                                  1, 1 );
     if (ret >=1 ) return;
     //................. parcourir tous les items et les selectionner ............
     QListViewItemIterator it( m_pQListView );
     while ( it.current() )
       {CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
        pCPrt->setSelected(TRUE);
        ++it;
       }
    }
 //........... effacer la selection ....................
 ATCD_MenuActionMultiDel();
}
