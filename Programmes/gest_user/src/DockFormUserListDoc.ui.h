/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
//#define TYP_ORDONNANCE  2
//#define TYP_CERTIFICAT  3
//#define TYP_PARAM       5

#define POS_NAME    0
#define POS_TYPE    1
#define POS_PRIMKEY 2

#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli

//------------------------------------- init ----------------------------------------------------------------
void DockFormUserListDoc::init()
{m_PkRef = "";
 pushButtonDelete->setIconSet ( QIconSet (Theme::getIcon("16x16/delete_Icon.png"))) ;
 pushButtonNewDoc->setIconSet ( QIconSet (Theme::getIcon("16x16/NewDoc.png"))) ;
 pushButton_ExportProfil->setIconSet ( QIconSet (Theme::getIcon("16x16/SaveProfile.png"))) ;
 pushButton_InportProfil->setIconSet ( QIconSet (Theme::getIcon("16x16/LoadProfile.png"))) ;

}

//------------------------------------- SetData ----------------------------------------------------------------
void DockFormUserListDoc::SetData(const char* prim_key)
{if (prim_key[0]==0) return;
 m_PkRef = prim_key;
 m_pCMoteurBase->GetUserDocList(ListView_Doc, m_PkRef);
}

//------------------------------------- ListView_Glossaire_clicked ----------------------------------------------------------------
void DockFormUserListDoc::ListView_Doc_clicked( QListViewItem *pQListViewItem )
{if (pQListViewItem<=0) return;
 emit Sign_UserDocSelected(m_PkRef, pQListViewItem->text(POS_NAME),
                                    pQListViewItem->text(POS_TYPE),
                                    pQListViewItem->text(POS_PRIMKEY));
}
//------------------------------------- ListView_Doc_currentChanged ----------------------------------------------------------------
void DockFormUserListDoc::ListView_Doc_currentChanged( QListViewItem *pQListViewItem )
{if (pQListViewItem<=0) return;
 emit Sign_UserDocSelected(m_PkRef, pQListViewItem->text(POS_NAME),
                                    pQListViewItem->text(POS_TYPE),
                                    pQListViewItem->text(POS_PRIMKEY));

}

//------------------------------------- ListViewDoc_FirstTypeFind  ----------------------------------------------------------------
// ACTION: retourne si il existele premier item trouvé correspondant au type et nom passé en entreé
QListViewItem *DockFormUserListDoc::ListViewDoc_FirstTypeFind( int type , const char* name)
{return ListViewDoc_FirstTypeFind(QString::number(type) ,  name);
}

//------------------------------------- ListViewDoc_FirstTypeFind  ----------------------------------------------------------------
// ACTION: retourne si il existe le premier item trouvé correspondant au type et nom passé en entreé
QListViewItem *DockFormUserListDoc::ListViewDoc_FirstTypeFind( const char* typeIn , const char* nameIn)
{char *type = (char*)typeIn;
 char *name = (char*)nameIn;

 if (type==0) type="";
 if (name==0) name="";
 if (type=="" && name=="") return 0;
 //................ iterer la liste de choix ........
 QListViewItemIterator it(ListView_Doc);
 while ( it.current() )
    {QListViewItem *item = it.current();
     ++it;
     if       ( type=="" && name!="") {if (item->text(POS_NAME)==name)     return item;}
     else if  ( name=="" && type!="") {if (item->text(POS_TYPE)==type)     return item;}
     else if  ( item->text(POS_NAME)==name && item->text(POS_TYPE)==type)  return item;
    }
 return 0;
}

//------------------------------------- pushButtonNewDoc_clicked ----------------------------------------------------------------
void DockFormUserListDoc::pushButtonNewDoc_clicked()
{QPopupMenu *pQPopupMenu = new QPopupMenu(this, "MyPopupMenu" );
 if (pQPopupMenu==0)    return ;
 //............................... chercher une zone de liste deroulante (bleue) .....................
 pQPopupMenu->insertItem( (tr("Nouvel en-tête et pied pour les observations")),     this, SLOT( NewObserv()),    CTRL+Key_Y );
 pQPopupMenu->insertItem( (tr("Nouvel en-tête et pied pour les certificats")),      this, SLOT( NewCertif()),    CTRL+Key_U );
 pQPopupMenu->insertItem( (tr("Nouvel en-tête et pied pour les ordonnances")),      this, SLOT( NewOrdo()),      CTRL+Key_N );
 pQPopupMenu->insertItem( (tr("Paramètres")),                                       this, SLOT( NewParam()),     CTRL+Key_P );
 pQPopupMenu->exec(QCursor::pos());
 delete  pQPopupMenu;
}

//------------------------------------- NewObserv ----------------------------------------------------------------
void DockFormUserListDoc::NewObserv()
{emit Sign_UserDocSelected(m_PkRef, tr("* Nouvel en-tête et pied d'observation"), QString::number(TYP_OBSERVATION), "0");
}

//------------------------------------- NewParam ----------------------------------------------------------------
void DockFormUserListDoc::NewParam()
{emit Sign_UserDocSelected(m_PkRef, tr("Parametres"), QString::number(TYP_PARAM), "0");
}

//------------------------------------- NewCertif ----------------------------------------------------------------
void DockFormUserListDoc::NewCertif()
{emit Sign_UserDocSelected(m_PkRef, tr("* Nouvel ent-tête et pied de certificat"), QString::number(TYP_CERTIFICAT), "0");
}

//------------------------------------- NewOrdo ----------------------------------------------------------------
void DockFormUserListDoc::NewOrdo()
{emit Sign_UserDocSelected(m_PkRef, tr("* Nouvel ent-tête et pied d'ordonnance"), QString::number(TYP_ORDONNANCE), "0");
}

//------------------------------------- pushButtonDelete_clicked ----------------------------------------------------------------
void DockFormUserListDoc::pushButtonDelete_clicked()
{QListViewItem *pQListViewItem = ListView_Doc->currentItem ();
 if (pQListViewItem <= 0) return;
 emit Sign_RubListMustBeDeleted( pQListViewItem->text(POS_PRIMKEY));
}


