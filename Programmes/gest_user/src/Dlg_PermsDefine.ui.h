/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli

//------------------------------------------pushButtonOk_clicked--------------------------------------------
void Dlg_PermsDefine::pushButtonOk_clicked()
{QString droits = "";         // rajouter d'office au debut
 QListViewItemIterator it( listViewPermsList );
 while ( it.current() )
       {QListViewItem *item = it.current();
        if (item->text(3)=="+")   droits += "-"+item->text(2);
        ++it;
       }
 *m_pDroitsIn = droits;
 accept();
}

//------------------------------------------InitDialog--------------------------------------------
void Dlg_PermsDefine::InitDialog(QString *droits, int mode)
{m_Droits                  = *droits;
 m_pDroitsIn               =  droits;
 QString     allDroitsStr  = "";
 QStringList listAllDroits = "";
 m_ListDroits = QStringList::split( "-", m_Droits);

 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli + "Images/ListeDroits.txt" , allDroitsStr);
 if (allDroitsStr.length())
    {allDroitsStr  = allDroitsStr.stripWhiteSpace();
     allDroitsStr  = allDroitsStr.remove("\n");
     allDroitsStr  = allDroitsStr.remove("\r");

     listAllDroits = QStringList::split("-",allDroitsStr );
     for ( QStringList::Iterator it = listAllDroits .begin(); it != listAllDroits .end(); ++it )
         {bool     ok = TRUE;
          QString str =  *it;
          int pos  = str.find("(");
          if (pos != -1)
             {QString str_droit = str.mid(pos+1,3);
              if (mode && (str_droit =="med" || str_droit =="sgn"|| str_droit =="adm")) ok= FALSE;
              if (ok)
                { QListViewItem *pQListViewItem = new QListViewItem( listViewPermsList,
                                                                 "",
                                                                 str.left(pos),
                                                                 str_droit);

                  if (m_Droits.find( pQListViewItem->text(2) ) != -1) {pQListViewItem->setText(3,"+");pQListViewItem->setPixmap (0, Theme::getIcon("16x16/CheckBoxOn.png" ) ); }
                  else                                                {pQListViewItem->setText(3,"-");pQListViewItem->setPixmap (0, Theme::getIcon("16x16/CheckBoxOff.png" ) ); }
                }
             }
         }
    }
}

//------------------------------------------ pushButtonAnnuler_clicked --------------------------------------------
void Dlg_PermsDefine::pushButtonAnnuler_clicked()
{*m_pDroitsIn = m_Droits;
 reject();
}

//------------------------------------------listViewPermsList_clicked--------------------------------------------
void Dlg_PermsDefine::listViewPermsList_clicked( QListViewItem *pQListViewItem, const QPoint &, int col)
{if (pQListViewItem==0) return;
 if (pQListViewItem->text(3) == "+")
    {pQListViewItem->setText(3,"-");
     pQListViewItem->setPixmap (0, Theme::getIcon("16x16/CheckBoxOff.png" ) );
    }
 else
    {pQListViewItem->setText(3,"+");
     pQListViewItem->setPixmap (0, Theme::getIcon("16x16/CheckBoxOn.png" ) );
    }

}

//------------------------------------------ pushButtonAllErase_clicked --------------------------------------------
void Dlg_PermsDefine::pushButtonAllErase_clicked()
{QListViewItemIterator it( listViewPermsList );
 while ( it.current() )
       {QListViewItem *pQListViewItem = it.current();
        if (pQListViewItem->text(2) == "med")
           { pQListViewItem->setText(3,"+");
             pQListViewItem->setPixmap (0, Theme::getIcon("16x16/CheckBoxOn.png" ) );
           }
        else
           { pQListViewItem->setText(3,"-");
             pQListViewItem->setPixmap (0, Theme::getIcon("16x16/CheckBoxOff.png" ) );
           }
        ++it;
       }
}

//------------------------------------------ pushButtonAllSelect_clicked --------------------------------------------
void Dlg_PermsDefine::pushButtonAllSelect_clicked()
{QListViewItemIterator it( listViewPermsList );
 while ( it.current() )
       {QListViewItem *pQListViewItem = it.current();
        pQListViewItem->setText(3,"+");
        pQListViewItem->setPixmap (0, Theme::getIcon("16x16/CheckBoxOn.png" ) );
        ++it;
       }

}

//------------------------------------------ pushButtonAllSelect_clicked --------------------------------------------
void Dlg_PermsDefine::listViewPermsList_spacePressed( QListViewItem *pQListViewItem )
{QPoint pt;
 listViewPermsList_clicked(pQListViewItem, pt, 0);
}
