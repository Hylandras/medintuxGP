/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#define MENUHONO 	0
#define MENUBANQUE	1
#define MENUVOITURE	2
#define MENUACTES 	3
#define MENULIVRES	4
#define MENUPARAM	5
#define MENUABOUT	6



#define TR QObject::tr

//------------------------------------- init ----------------------------------------------------------------
void FormListMenu::init()
{
}


void FormListMenu::ListView_Doc_clicked( QListBoxItem *pItem )
{ if (pItem <= 0) return;
  ListView_Doc_currentChanged(pItem);
}


void FormListMenu::ListView_Doc_currentChanged( QListBoxItem *pItem )
{ if (pItem <= 0) return;
  if ( pItem->text() == TR("Honoraires") ) emit honorairesSelected();
  else if ( pItem->text() == TR("Remise") ) emit banqueSelected();
  else if ( pItem->text() == TR("Mouvements") ) emit mouvementsSelected();
  else if ( pItem->text() == TR("Actes") ) emit actesSelected();
  else if ( pItem->text() == TR("Gestion DAF") ) emit gestionDAFSelected();
  else if ( pItem->text() == TR("Gestion CMU") ) emit gestionCMUSelected();
  else if ( pItem->text() == TR("Relevés de comptes") ) emit releveSelected();
  else if ( pItem->text() == TR("Livre et Bilan") ) emit livresSelected();
  else if ( pItem->text() == TR("Paramètres") ) emit paramSelected();
  else if ( pItem->text() == TR("A propos") ) emit aboutSelected();
}
