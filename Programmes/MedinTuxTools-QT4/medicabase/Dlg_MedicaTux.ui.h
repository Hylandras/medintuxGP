/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
/*
#define PAGE_INFO_GENE     "Généralités"
#define PAGE_INFO_COMPO    "Composition"
#define PAGE_INFO_EXTRA    "Extra"
#define PAGE_EDIT_POSO     "Editeur de Posologie"
#define PAGE_EDIT_MEDIC    "Editeur de médicaments"
#define PAGE_PRESCRIPTION  "P&rescription"
#define PAGE_MONOGRAPHIE   "&Monographie"
#define PAGE_POSOLOGIES    "&Posologies"
*/

#define LV_PROD_LIBELLE    0
#define LV_PROD_CODE_PROD  1
#define LV_PROD_PKT00      2
#define LV_PROD_PKT4B      3
#define LV_PROD_CIP        4
#define LV_PROD_CODE_UV    5

#include <qinputdialog.h>

//------------------------------------------- init -----------------------------------------------------------------------------------
void Dlg_MedicaTux::init()
{
 PAGE_INFO_GENE    = tr("Généralités");
 PAGE_INFO_COMPO   = tr("Composition");
 PAGE_INFO_EXTRA   = tr("Extra");
 PAGE_EDIT_POSO    = tr("Editeur de Posologie");
 PAGE_EDIT_MEDIC   = tr("Editeur de médicaments");
 PAGE_PRESCRIPTION = tabWidget_Tables->tabLabel ( tabWidget_Tables->page(2) );  //"P&rescription";
 PAGE_MONOGRAPHIE  = tabWidget_Tables->tabLabel ( tabWidget_Tables->page(1) );  //"&Monographie";
 PAGE_POSOLOGIES   = tabWidget_Tables->tabLabel ( tabWidget_Tables->page(0) );  //"&Posologies";
}
//------------------------------------------- destroy -----------------------------------------------------------------------------------
void Dlg_MedicaTux::destroy()
{textBrowser_Prescription->mimeSourceFactory()->addFilePath (m_PathImages  );
 textBrowser_Posologies->mimeSourceFactory()->addFilePath (m_PathImages   );
 textBrowser_Monographie->mimeSourceFactory()->addFilePath (m_PathImages  );
}
//------------------------------------------- initDialog -------------------------------------------------------------------------------
// ACTION:  initialisation générale du dialogue
// ENTREE:  CMedicaBase *pCMedBase: pointeur sur l'API de gestion des bases medicamenteuses
//          QString *ordoStruct:  pointeur sur la forme XML d'une ordonnance
// RETOUR:  int à zero si erreur d'initialisation

int Dlg_MedicaTux::initDialog(CMedicaBase *pCMedBase,
                              QString *ordoStruct,
			      QString pathImage)
{int ret;

 if ( (ret=initDialog( pCMedBase , pathImage)) ==0) return 0;

 m_pCMedBase->Medica_PosologieListDeserialize(*ordoStruct,  m_CPosologieOrdoList);
 textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
 tabWidget_Tables->setCurrentPage (getCurrentPageID_ByTitle(tabWidget_Tables, PAGE_PRESCRIPTION) );
 return ret;
}

//------------------------------------------- initDialog -------------------------------------------------------------------------------
// ACTION:  initialisation générale du dialogue
// ENTREE:  CMedicaBase *pCMedBase: pointeur sur l'API de gestion des bases medicamenteuses
//           QString *ordoStruct:  reference sur la collection ou liste des objets: CPosologie
//                                 d'une ordonnance
// RETOUR:  int à zero si erreur d'initialisation

int Dlg_MedicaTux::initDialog(CMedicaBase *pCMedBase, const QPL_CPosologie &posologieOrdoList,
			      QString pathImage)
{int ret;
 if ( (ret=initDialog( pCMedBase , pathImage)) ==0) return 0;

 m_CPosologieOrdoList = posologieOrdoList;
 textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
 tabWidget_Tables->setCurrentPage (getCurrentPageID_ByTitle(tabWidget_Tables, PAGE_PRESCRIPTION) );
 return ret;
}

//------------------------------------------- initDialog -------------------------------------------------------------------------------
// ENTREE:  CMedicaBase *pCMedBase: pointeur sur l'API de gestion des bases medicamenteuses
// RETOUR:  int à zero si erreur d'initialisation

int Dlg_MedicaTux::initDialog(CMedicaBase *pCMedBase,
			      QString pathImage )
{if (pCMedBase == 0) return 0;
 m_pCMedBase    = pCMedBase;
 m_PathImages   = pathImage;

 //................ on va pas l'afficher car doublon avec pictogrammes de modification des poso .........
 pushButtonEditPoso->hide();
 //................. chemin des pictogrammes ....................................................
 textBrowser_Prescription->mimeSourceFactory()->setFilePath (m_PathImages );
 textBrowser_Posologies->mimeSourceFactory()->setFilePath (m_PathImages    );
 textBrowser_Monographie->mimeSourceFactory()->setFilePath (m_PathImages  );
 //................. ameliorer les fonds ....................................................

 QPixmap image;
 image.load (m_PathImages+"/CuirMaron.png");
 textBrowser_Monographie->setPaletteBackgroundPixmap (image);
 image.load (m_PathImages+"/bois bleu.png");
 textBrowser_Posologies->setPaletteBackgroundPixmap (image );
 image.load (m_PathImages+"/granit bleu.png");
 textBrowser_Prescription->setPaletteBackgroundPixmap (image );
 pushButtonByReferent->setIconSet  ( QIconSet (QPixmap(m_PathImages  + "/referent.png"))) ;
 pushButtonByGenerique->setIconSet ( QIconSet (QPixmap(m_PathImages  + "/generiques.png"))) ;
 pushButtonByPrincipe->setIconSet  ( QIconSet (QPixmap(m_PathImages  + "/BySubstance.png"))) ;
 pushButtonByObsolete->setIconSet  ( QIconSet (QPixmap(m_PathImages  + "/supprimes.png"))) ;
 pushButtonByStandard->setIconSet  ( QIconSet (QPixmap(m_PathImages  + "/standards.png"))) ;
 //.............................. le logo ...............................................................
 if ( (int)(m_pCMedBase->Medica_GetBaseMode() & CMedicaBase::Datasemp) )
    {image.load (m_PathImages+"/DataSempLogo.png");
    }
 else
    {image.load (m_PathImages+"/LogoMachin.png");
     comboBoxClassifications->hide();
     checkBox_Dispo->hide();
     checkBox_Generiques->hide();
     tabWidgetSelect->removePage (tabWidgetSelect->page (1));
     tabWidgetSelect->removePage (tabWidgetSelect->page (1));
    }
 textLabelLogo->setPixmap(image);
 //................. generer la premiere liste ...........................................................................................
 QString title = tr("MedicaTux est connecté à : ") + m_pCMedBase->m_DriverName + ":: " + m_pCMedBase->m_BaseName
                 + tr("  Version : ") + m_pCMedBase->Medica_GetDatasempVersion();
 setCaption(title);
 int nb = m_pCMedBase->Medica_GetMedicamentList( listView_Produits ,
                                                         "A",
                                                         (checkBox_Generiques->isChecked() ? QString("G") : QString("")),
                                                         (checkBox_Dispo->isChecked()      ? QString("0") : QString(""))
                                                       );
 textLabel_Status->setText(QString::number(nb) + tr (" produits trouvés"));
 //.................. selectionner le premier de la liste ........................................................................
 if (nb >0)
    {QListViewItem * pQListViewItem = listView_Produits->firstChild();
     if (pQListViewItem != 0)
        {listView_Produits->setSelected (pQListViewItem, TRUE );
         listView_Produits->setCurrentItem(pQListViewItem);
         listView_Produits_clicked( pQListViewItem  );
       }
    }
 //......................... Liste ATC .............................................................................................
 if (listView_EdMed_ATC->childCount() <10 ) m_pCMedBase->Medica_FillQListView_ATC(listView_EdMed_ATC);
 progressBar_EDMed->setIndicatorFollowsStyle(TRUE);
 lineEdit_Produit->show();
 textLabel_CIP_Find->show();
 lineEdit_EdMed__CIP_Find->show();
 pushButton_CIP_Find->show();
 //................................. icone vidal dans l'onglet monographie .................................................
 int id = getCurrentPageID_ByTitle(tabWidget_Tables, PAGE_MONOGRAPHIE);

 if (id != -1)
    {image.load (m_PathImages + "/logo_vidal_petit.png");
     tabWidget_Tables->setTabIconSet (tabWidget_Tables->page(id), QIconSet (image) );
    }


//................................. ComboBox de specialité .................................................
// Le premier caractère représente la "section" : 1 = Spécialités ;
//                                                2 = Diététiques ;
//                                                3 = Vétérinaires ;
//                                                4 = Parapharmacie ;
//                                                5 = Accessoires ;
//                                                6 = Divers (herboristerie, etc.) ;
//                                                7 = Homéopathie...
comboBox_Specialites->clear();
comboBox_Specialites->insertItem(tr("Spécialités       ") ); //                 + SPACES_SEPARATOR  +  "1");
comboBox_Specialites->insertItem(tr("Diététique        ") ); //                 + SPACES_SEPARATOR  +  "2");
comboBox_Specialites->insertItem(tr("Prod .Vétérinaires") ); //                 + SPACES_SEPARATOR  +  "3");
comboBox_Specialites->insertItem(tr("Parapharmacie     ") ); //                 + SPACES_SEPARATOR  +  "4");
comboBox_Specialites->insertItem(tr("Accessoires       ") ); //                 + SPACES_SEPARATOR  +  "5");
comboBox_Specialites->insertItem(tr("Divers (herboristerie, etc.)       "));   // + SPACES_SEPARATOR  +  "6");
comboBox_Specialites->insertItem(tr("Homéopathie       ") ); //                .. + SPACES_SEPARATOR  +  "7");
//........................ initialiser les popups ...............................
m_CurrentPopupItem = "";
m_RetPopupItem     = "";
m_pQPopupMenu      = 0;
lineEdit_Produit->setFocus();

//............................ encore un petit effort placer le poids .....................................................
/*
if (G_pCApp->m_pDrTux->m_pCMDI_Terrain)
   {textLabel_Poids->setText(tr("Poids : ")   + G_pCApp->m_pDrTux->m_pCMDI_Terrain->GetPoids()  + tr(" kg"));
    textLabel_Taille->setText(tr("Taille : ") + G_pCApp->m_pDrTux->m_pCMDI_Terrain->GetTaille() + tr(" cm"));
   }
*/
 m_pCMedBase->Medica_initComboDuree(comboBoxPendant);
 return 1;
}

//------------------------------------------- initComboDuree --------------------------------------------------------------
void Dlg_MedicaTux::initComboDuree()
{
}

//------------------------------------------- pushButton_CIP_Find_clicked --------------------------------------------------------------
void Dlg_MedicaTux::pushButton_CIP_Find_clicked()
{m_pCMedBase->Medica_GetMedicamentListByCIP( listView_Produits , comboBox_Specialites->currentItem()+1, lineEdit_EdMed__CIP_Find->text());
}


//------------------------------------------- tabWidget_Tables_currentChanged --------------------------------------------------------
void Dlg_MedicaTux::tabWidget_Tables_currentChanged( QWidget * )
{listView_Produits_clicked( listView_Produits->currentItem ()  );
}

//------------------------------------------- listView_Produits_currentChanged -----------------------------------------------------------------
void Dlg_MedicaTux::listView_Produits_currentChanged( QListViewItem * pQListViewItem)
{listView_Produits_clicked( pQListViewItem  );
}

//------------------------------------------- listView_Produits_doubleClicked ------------------------------------------------------------------
void Dlg_MedicaTux::listView_Produits_doubleClicked( QListViewItem * )
{tabWidget_Tables->setCurrentPage (getCurrentPageID_ByTitle(tabWidget_Tables, PAGE_POSOLOGIES) );
}

//------------------------------------------- listView_Produits_clicked --------------------------------------------------------------
void Dlg_MedicaTux::listView_Produits_clicked( QListViewItem *pQListViewItem  )
{ if (pQListViewItem==0) return;
  QStringList list;
  QString     txt("");

  QApplication::restoreOverrideCursor();
  QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );

  QString tab_name = tabWidget_Tables->tabLabel ( tabWidget_Tables->currentPage () );

  //.................... positionner les classifications sur le produit ................................
  int classID = comboBoxClassifications->currentItem ();
  switch (classID)
    {case 0:         //............. classes ATC .........................................
           {QString atc = m_pCMedBase->Medica_GetCodeATC(pQListViewItem->text( LV_PROD_CIP),
                                                                 pQListViewItem->text( LV_PROD_PKT4B));
           //lineEdit_EdMed_ATC->setText(m_pCMedBase->Medica_GetLibelleATC(atc));
           m_pCMedBase->Medica_SetQListViewOnATC( listView_EdMed_ATC , atc );
           }
           break;
     case 1:        //............... classes thérapeutiques Vidal ........................
           {m_pCMedBase->Datasemp_GetCodesClassTherapVidal(pQListViewItem->text( LV_PROD_CODE_PROD),
                                                                    list
                                                                   );
            m_pCMedBase->Datasemp_SetQListViewOnClassTherapVidal( listViewTherapClassVidal , list  );
           }
            break;
    }
  lineEdit_EdMed__CIP_Find->setText(pQListViewItem->text( LV_PROD_CIP));
  //.................... positionner le reste de l'interface sur le produit ................................
  if ( tab_name == PAGE_POSOLOGIES )
     {
      m_pCMedBase->Medica_GetPosologies(pQListViewItem->text( LV_PROD_CIP), m_QPL_CPosologie );          // recuperer la liste des posologies
      QString txt = m_pCMedBase->Medica_GetHtmlPosoSpecPage(  pQListViewItem->text( LV_PROD_CIP) ,
                                                                      pQListViewItem->text( LV_PROD_LIBELLE) ,
                                                                      m_QPL_CPosologie,
                                                                      comboBox_Specialites->currentItem()+1,
                                                                      pQListViewItem->text( LV_PROD_PKT00) ,
                                                                      pQListViewItem->text( LV_PROD_PKT4B) ,
                                                                      pQListViewItem->text( LV_PROD_CODE_PROD) ,
                                                                      pQListViewItem->text( LV_PROD_CODE_UV)
                                                            );
      textBrowser_Posologies->setText(txt+"<br><center>"+m_pCMedBase->Datasemp_GetComposition(pQListViewItem->text( LV_PROD_CODE_PROD))+"</center>");
      m_PosoCIP = pQListViewItem->text( LV_PROD_CIP);
      //textEdit_Composition->setText( m_pCMedBase->Datasemp_GetComposition(pQListViewItem->text( LV_PROD_CODE_PROD)));
     }
  else if ( tab_name == PAGE_MONOGRAPHIE )
     {
      if ( (int)(m_pCMedBase->Medica_GetBaseMode() & CMedicaBase::Datasemp) )
         { textBrowser_Monographie->setText(m_pCMedBase->Datasemp_GetMonographie(pQListViewItem->text( LV_PROD_CODE_UV), pQListViewItem->text( LV_PROD_PKT4B)));
         }
      else
         { textBrowser_Monographie->setText(m_pCMedBase->Datasemp_GetMonographieByCIP(pQListViewItem->text(LV_PROD_CIP)));
         }
     }
//end_listView_Produits_clicked:
QApplication::restoreOverrideCursor();
}


//------------------------------------------- comboBox_Specialites_activated ------------------------------------------------------------------
void Dlg_MedicaTux::comboBox_Specialites_activated( int id)
{int nb = m_pCMedBase->Medica_GetMedicamentListByFamily( listView_Produits ,
                                                                 id + 1, // Vidal indicé base 1
                                                                 lineEdit_Produit->text(),
                                                                 (checkBox_Generiques->isChecked() ? QString("G") : QString("")),
                                                                 (checkBox_Dispo->isChecked()      ? QString("0") : QString(""))
                                                       );
  textLabel_Status->setText(QString::number(nb) + tr (" produits trouvés"));
  lineEdit_Produit->setFocus();
  lineEdit_Produit->selectAll ( );
}

//----------------------------------------------------- pushButtonDown_clicked ----------------------------------------------------------------------------------
void Dlg_MedicaTux::pushButtonDown_clicked()
{ comboBox_Specialites->setFocus();
  QString text = lineEdit_Produit->text();
  if (text.at(0) != '%') text.prepend("%");
  lineEdit_Produit->setText(text);
  comboBox_Specialites->setCurrentItem ( 4 );     // activer les accessoires
  comboBox_Specialites->popup ();
}
//----------------------------------------------------- pushButtonDownProd_clicked ----------------------------------------------------------------------------------
void Dlg_MedicaTux::pushButtonDownProd_clicked()
{ comboBox_Specialites->setFocus();
  QString text = lineEdit_Produit->text();
  if (text.at(0) == '%') text = text.mid(1);
  lineEdit_Produit->setText(text);
  comboBox_Specialites->setCurrentItem ( 0 );  // activer le s produits
  comboBox_Specialites->popup ();
}
//------------------------------------------- lineEdit_Produit_textChanged --------------------------------------------------------------
void Dlg_MedicaTux::lineEdit_Produit_textChanged( const QString &text )
{//if (text.length()<2) return;
 int nb = m_pCMedBase->Medica_GetMedicamentListByFamily(         listView_Produits ,
                                                                 comboBox_Specialites->currentItem()+1, // Vidal indicé base 1
                                                                 text,
                                                                 (checkBox_Generiques->isChecked() ? QString("G") : QString("")),
                                                                 (checkBox_Dispo->isChecked()      ? QString("0") : QString(""))
                                                       );
 textLabel_Status->setText(QString::number(nb) + tr (" produits trouvés"));
}

//------------------------------------------- checkBox_Generiques_stateChanged --------------------------------------------
void Dlg_MedicaTux::checkBox_Generiques_stateChanged( int )
{InitListMedicament();
}

//------------------------------------------- checkBox_Dispo_stateChanged --------------------------------------------
void Dlg_MedicaTux::checkBox_Dispo_stateChanged( int )
{InitListMedicament();
}

//------------------------------------------- InitListMedicament -------------------------------------------------------------------
void Dlg_MedicaTux::InitListMedicament()
{int prodFamily = comboBox_Specialites->currentItem()+1;
 if (prodFamily != 1)
    {comboBox_Specialites_activated( prodFamily);
     return;
    }

 //...................................... ci-dessous que pour medicaments ...............................
 QListViewItem *pQListViewItem;
 int nb = 0;
 int id =  comboBoxClassifications->currentItem ();
 if (id==0)
    {pQListViewItem = listView_EdMed_ATC->currentItem();
     if (pQListViewItem==0) return;
     QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     nb = m_pCMedBase->Medica_GetMedicamentListByATC(   listView_Produits ,
                                                                pQListViewItem->text(1), // 1 = code ATC de la listview ATC
                                                                lineEdit_Produit->text(),
                                                                (checkBox_Generiques->isChecked() ? QString("G") : QString("")),
                                                                (checkBox_Dispo->isChecked()      ? QString("0") : QString(""))
                                                                );
    }
 else if (id==1)
    {pQListViewItem = listViewTherapClassVidal->currentItem();
     if (pQListViewItem==0) return;
     QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     nb = m_pCMedBase->Datasemp_GetMedicamentListByClassTheraVidal(   listView_Produits ,
                                                                               pQListViewItem ,    //pQListViewItem->text(1)
                                                                               lineEdit_Produit->text(),
                                                                               (checkBox_Generiques->isChecked() ? QString("G") : QString("")),
                                                                               (checkBox_Dispo->isChecked()      ? QString("0") : QString("")),
                                                                               progressBar_EDMed
                                                                             );
     progressBar_EDMed->setProgress(0);
    }
 textLabel_Status->setText(QString::number(nb) + tr (" Produits trouvés"));
 QApplication::restoreOverrideCursor();
}

//------------------------------------------- listView_EdMed_ATC_clicked ------------------------------
void Dlg_MedicaTux::listView_EdMed_ATC_clicked( QListViewItem *pQListViewItem )
{if (pQListViewItem==0) return;
 if (comboBox_Specialites->currentItem() != 0) comboBox_Specialites->setCurrentItem(0);
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 int nb = m_pCMedBase->Medica_GetMedicamentListByATC(   listView_Produits ,
                                                                pQListViewItem->text(1), // 1 = code ATC de la listview ATC
                                                                lineEdit_Produit->text(),
                                                                (checkBox_Generiques->isChecked() ? QString("G") : QString("")),
                                                                (checkBox_Dispo->isChecked()      ? QString("0") : QString(""))
                                                             );
 textLabel_Status->setText(QString::number(nb) + tr (" produits trouvés"));
 QApplication::restoreOverrideCursor();
}

//------------------------------------------- listViewTherapClassVidal_clicked ------------------------------
void Dlg_MedicaTux::listViewTherapClassVidal_clicked( QListViewItem *pQListViewItem )
{ if (pQListViewItem==0) return;
  if (comboBox_Specialites->currentItem() != 0) comboBox_Specialites->setCurrentItem(0);
  QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
  QDateTime dtDeb= QDateTime::currentDateTime();
  int nb = m_pCMedBase->Datasemp_GetMedicamentListByClassTheraVidal( listView_Produits ,
                                                                               pQListViewItem ,    //pQListViewItem->text(1)
                                                                               lineEdit_Produit->text(),
                                                                               (checkBox_Generiques->isChecked() ? QString("G") : QString("")),
                                                                               (checkBox_Dispo->isChecked()      ? QString("0") : QString("")),
                                                                               progressBar_EDMed
                                                                             );
 progressBar_EDMed->setProgress(0);
 QDateTime dtEnd= QDateTime::currentDateTime();
 textLabel_Status->setText(QString::number(nb) + tr (" produits trouvés en ") + QString::number(dtDeb.secsTo (dtEnd)) + tr(" secondes"));
 QApplication::restoreOverrideCursor();
}

//------------------------------------------- getCurrentPageID_ByTitle ------------------------------------------------------------------------
int Dlg_MedicaTux::getCurrentPageID_ByTitle(QTabWidget *pQTabWidget, const QString &title)
{int nb = pQTabWidget->count();
 for ( int i=0; i<nb; ++i)
     {if (pQTabWidget->label(i) == title) return i;
     }
 return -1;
}

//------------------------------------------- pushButtonByGenerique_clicked --------------------------------------------
void Dlg_MedicaTux::pushButtonByGenerique_clicked()
{listView_Produits->setSorting ( 6, FALSE );    // generic 6  Dispo 7
}
//------------------------------------------- pushButtonByReferent_clicked --------------------------------------------
void Dlg_MedicaTux::pushButtonByReferent_clicked()
{listView_Produits->setSorting ( 6, TRUE );    // generic 6  Dispo 7
}
//------------------------------------------- pushButtonByObsolete_clicked --------------------------------------------
void Dlg_MedicaTux::pushButtonByObsolete_clicked()
{listView_Produits->setSorting ( 7, FALSE );   // generic 6  Dispo 7
}
//------------------------------------------- pushButtonByStandard_clicked --------------------------------------------
void Dlg_MedicaTux::pushButtonByStandard_clicked()
{listView_Produits->setSorting ( 6, TRUE );    // generic 6  Dispo 7
 listView_Produits->setSorting ( 7, TRUE );    // generic 6  Dispo 7
}

//------------------------------------------- comboBoxClassifications_activated( ------------------------------------------------------------------
void Dlg_MedicaTux::comboBoxClassifications_activated( int id)
{QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 widgetStack_Classifications->raiseWidget ( id );
  if (id==1 && listViewTherapClassVidal->childCount() ==0 )    // clases therapeutique vidal
     {m_pCMedBase->Datasemp_FillQListView_ClassTherapSemp( listViewTherapClassVidal );
     }
 QApplication::restoreOverrideCursor();
 InitListMedicament();
}

//------------------------------------------- lineEdit_Find_Indic_textChanged ------------------------------------------------------------------
void Dlg_MedicaTux::lineEdit_Find_Indic_textChanged( const QString &txt )
{m_pCMedBase->Datasemp_GetIndicationList(listView_IndicList,   txt);;
}


//------------------------------------------- listView_IndicList_clicked ------------------------------------------------------------------
void Dlg_MedicaTux::listView_IndicList_clicked( QListViewItem  *pQListViewItem)
{if (pQListViewItem==0) return;
 if (comboBox_Specialites->currentItem() != 0) comboBox_Specialites->setCurrentItem(0);
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 long nb = m_pCMedBase->Datasemp_GetMedicamentListByCodeIndic( listView_Produits,
                                                                       pQListViewItem->text(1),
                                                                       (checkBox_Generiques->isChecked() ? QString("G") : QString("")),
                                                                       (checkBox_Dispo->isChecked()      ? QString("0") : QString(""))
                                                                     );
 textLabel_Status->setText(QString::number(nb) + tr (" produits trouvés"));
 QApplication::restoreOverrideCursor();
}

//------------------------------------------- pushButtonByPrincipe_clicked ------------------------------------------------------------------
void Dlg_MedicaTux::pushButtonByPrincipe_clicked()
{QListViewItem *pQListViewItem = listView_Produits->currentItem ();
 if (pQListViewItem==0) return;
 QStringList popItem;
 popItem.append(tr("Lister les substances de ce produit"));
 popItem.append(tr("Lister les indications de ce produit"));
 popItem.append(tr("---------"));
 popItem.append(tr("Afficher la monographie de ce produit"));
 popItem.append(tr("Afficher les posologies de ce produit"));

 QString popRet = DoPopupList(popItem);
 if (popRet.find(tr("substances"))!=-1)
    {long nb = m_pCMedBase->Datasemp_GetSubtancesListByCodeProd(listView_Substance, pQListViewItem->text( LV_PROD_CODE_PROD));
     if (nb!=0)tabWidgetSelect->setCurrentPage ( 1 );
    }
 else if (popRet.find(tr("indications"))!=-1)
    {long nb = m_pCMedBase->Datasemp_GetIndicationsListByCodeProd(listView_IndicList, pQListViewItem->text( LV_PROD_CODE_PROD));
     if (nb!=0)tabWidgetSelect->setCurrentPage ( 2 );
    }
 else if (popRet.find(tr("monographie"))!=-1)
    {tabWidget_Tables->setCurrentPage (getCurrentPageID_ByTitle(tabWidget_Tables, PAGE_MONOGRAPHIE) );
    }
 else if (popRet.find(tr("posologies"))!=-1)
    {tabWidget_Tables->setCurrentPage (getCurrentPageID_ByTitle(tabWidget_Tables, PAGE_POSOLOGIES) );
    }
}

//----------------------------------------------------- pushButton_Ok_clicked ----------------------------------------------------------------------------------
void Dlg_MedicaTux::listView_Produits_rightButtonClicked( QListViewItem *, const QPoint &, int )
{pushButtonByPrincipe_clicked();
}

//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString Dlg_MedicaTux::DoPopupList(QStringList &list)
{m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 if (m_pQPopupMenu)          return m_RetPopupItem;
 m_pQPopupMenu      = new QPopupMenu(0, "MyPopupMenu" );
 if (m_pQPopupMenu==0)       return m_RetPopupItem;
 //............................... chercher une zone de liste deroulante (bleue) .....................
 for ( QStringList::Iterator it =list.begin(); it != list.end(); ++it )
    { if (*(it)=="---------")  m_pQPopupMenu->insertSeparator();
      else                     m_pQPopupMenu->insertItem( *(it),    this, SLOT( GetPopupItem()),  0 );
    }
 //................. connecter le menu .................................................................
 connect( m_pQPopupMenu, SIGNAL( activated( int)),
          this,          SLOT(OnMenuActivated( int))
        );

 m_pQPopupMenu->exec( QCursor::pos() /* mapToGlobal (  QPoint ( x, y) )*/ );
 delete  m_pQPopupMenu;
 m_pQPopupMenu = 0;
 return  m_RetPopupItem;
}

//--------------------------------- OnMenuActivated  -------------------------------------------------------------
void Dlg_MedicaTux::OnMenuActivated (int id)
{if (id != -1 && m_pQPopupMenu)
    {m_CurrentPopupItem = m_pQPopupMenu->text(id);
    }
}

//--------------------------------- GetPopupItem  -------------------------------------------------------------
void Dlg_MedicaTux::GetPopupItem ()
{m_RetPopupItem  = m_CurrentPopupItem;
}

//-------------------------------------- PopupQuantiteAccessoire -------------------------------------------------------
void Dlg_MedicaTux::PopupQuantiteAccessoire(CPosologie &poso)
{// [Duree]
 //   DureePrescription = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   DureeUnite = Jours
 //   NombreAccessoires = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   NombreUnite= élément(s)
 QStringList popItem;
 QString str      = "";
 QString unite    = "";
 CGestIni::Param_ReadParam(m_pCMedBase->m_IniParam, "Duree", "NombreAccessoires", &str);
 CGestIni::Param_ReadParam(m_pCMedBase->m_IniParam, "Duree", "NombreUnite",        &unite);
 if (str.length()<= 0)    return;
 QStringList list = QStringList::split("-", str);
 for (   QStringList::Iterator it = list.begin(); it != list.end(); ++it )
     {popItem.append( (*it) + " " +unite );
     }

 QString popRet = DoPopupList(popItem);
 if (popRet.length())
    {int pos                      = popRet.find(" ");        if (pos==-1) return ;
     poso.m_MEDICA_POSO_DOSE_MIN  = popRet.left(pos);
    }
}

//-------------------------------------- PopupDureeTraitement -------------------------------------------------------
void Dlg_MedicaTux::PopupDureeTraitement(CPosologie &poso, int num)
{QStringList popItem;
 if (num==-1) popItem.append( tr(" 0 Bon pour achat ") );
 // [Duree]
 //   DureePrescription = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   DureeUnite = Jours
 //   NombreAccessoires = 1-2-3-4-5-6-7-8-9-10-12-15-21-28-31-62-92-182-365
 //   NombreUnite= élément(s)
 QString str    = "";
 CGestIni::Param_ReadParam(m_pCMedBase->m_IniParam, "Duree", "DureePrescription", &str);
 if (str.length()<= 0)    return;
 QStringList list = QStringList::split("-", str);
 for (   QStringList::Iterator it = list.begin(); it != list.end(); ++it )
         {popItem.append( m_pCMedBase->Medica_PosologieDureeJourToHtml(*it) );
         }

 QString popRet = DoPopupList(popItem);
 if (popRet.length())
    {popRet                         = popRet.replace(tr("un"),"1");
     poso.m_MEDICA_POSO_PENDANT     = QString::number(m_pCMedBase->Medica_dureeMenuTextToSecond(popRet));
    }
}


//------------------------------------------- lineEdit_FindNomSubce_textChanged ------------------------------------------------------------------
void Dlg_MedicaTux::lineEdit_FindNomSubce_textChanged( const QString &txt )
{m_pCMedBase->Datasemp_GetSubstanceList(listView_Substance,  txt );
}

//------------------------------------------- listView_Substance_clicked ------------------------------------------------------------------
void Dlg_MedicaTux::listView_Substance_clicked( QListViewItem *pQListViewItem )
{if (pQListViewItem==0) return;
 if (comboBox_Specialites->currentItem() != 0) comboBox_Specialites->setCurrentItem(0);
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 long nb = m_pCMedBase->Datasemp_GetMedicamentListBySubstance( listView_Produits,
                                                                       pQListViewItem->text(1),
                                                                       (checkBox_Generiques->isChecked() ? QString("G") : QString("")),
                                                                       (checkBox_Dispo->isChecked()      ? QString("0") : QString(""))
                                                                     );
 textLabel_Status->setText(QString::number(nb) + tr (" produits trouvés"));
 QApplication::restoreOverrideCursor();
}

//------------------------------------------- textBrowser_Posologies_anchorClicked ------------------------------------------------------------------
// #PosoList
// #PosoText
// #PosoProg
void Dlg_MedicaTux::textBrowser_Posologies_anchorClicked( const QString &, const QString &link )
{int     pos(0);
 if (link.left(5) != "#Poso")      return;
 //...................................................... ajout d'un produit non medicamenteux ..................................................
 // page += "<br><b>1)<a href=\"#PosoText"+ cip + "==" + QString::number(tableIndex) + "\"> Pas de données sur la posologie, les préciser textuellement</b></a></span>";
 //
 if (link.mid(5,4) == "Text")
    {QString cip("");
     int tableIndex = 5;        //case 5: table = m_MEDICA_INDEX_ACCS_TBL_NAME;  break;
     pos = link.find("==");
     if (pos != -1)
        {cip          = link.mid(9,pos-9);
         tableIndex   = link.mid(pos+2).toInt();
        }
     QPL_CPosologie::iterator it = m_CPosologieOrdoList.append(CPosologie(cip,                                           // cip
                                                                           "",                                           // pk
                                                                           tr("Posologie non extraite de la base"),      // libelle
                                                                           "",                                           // age min
                                                                           "",                                           // age max
                                                                           "",                                           // sexe
                                                                           "",                                           // terrain
                                                                           "",                                           // poids min
                                                                           "",                                           // poids max
                                                                           "1",                                          // dose min
                                                                           "",                                           // dose max
                                                                           "",                                           // dose unit
                                                                           "0",                                          // factcorpQU
                                                                           "",                                           // factCorpUnit
                                                                           "",                                           // doseLimitMax
                                                                           "",                                           // periode
                                                                           "0",                                          // equiCoeff
                                                                           "0",                                          // unitCoeff
                                                                           "1",                                          // nbPrises
                                                                           "",                                           // repart
                                                                           "",                                           // divers
                                                                           (tableIndex==5)?"0":"691200",                 // Pendant (691200 = 8 jours) si accessoire et durée zero -> bon pour achat
                                                                           "0",                                          // secabilité
                                                                           "",                                           // extraPk
                                                                           "",                                           // note
                                                                           0,                                            // posoAdjust
                                                                           1,                                            // numOrdre
                                                                           "",                                           // posoTextuelle
                                                                           0,                                            // ALD
                                                                           CPosologie::Structuree,                       // posoType
                                                                           tableIndex                                    // Id de la table d'index
                                                                          )
                                                              );
     textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
     tabWidget_Tables->setCurrentPage (getCurrentPageID_ByTitle(tabWidget_Tables, PAGE_PRESCRIPTION) );
    }
 //...................................................... ajout d'un produit medicamenteux ..................................................
 else if (link.mid(5,4) == "List")
    {QString spoids  = QString::number( m_pCMedBase->Medica_ExtractNumber( textLabel_Poids->text()));
     QString staille = QString::number( m_pCMedBase->Medica_ExtractNumber( textLabel_Taille->text()));
     Dlg_MedicaAdjustPoso* dlg = new Dlg_MedicaAdjustPoso(this, "AdjustPoso");
     if (dlg==0)                   return;
     int pos = link.find("==");
     if (pos==-1)                  return;
     //page += "<br><b>1)<a href=\"#PosoText"+ cip + "==" + QString::number(tableIndex) + "\"> Pas de données sur la posologie, les préciser textuellement</b></a></span>";
     //
     //.................. ajouter cette nouvelle posologie à l'ordonnance .......................................
     CPosologie poso = m_QPL_CPosologie[link.mid(9,pos-9).toInt()-1];
     dlg->initDialog( poso,  /*G_pCApp->m_pDrTux->m_pCMDI_Terrain,*/ &spoids, &staille);
     if (dlg->exec()== QDialog::Accepted)
        {
         textLabel_Poids->setText(tr("Poids : ")   + spoids  + tr(" kg"));
         textLabel_Taille->setText(tr("Taille : ") + staille + tr(" cm"));
         QPL_CPosologie::iterator it = m_CPosologieOrdoList.append(poso);
         *it = dlg->m_PosoIn;
         textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
         tabWidget_Tables->setCurrentPage (getCurrentPageID_ByTitle(tabWidget_Tables, PAGE_PRESCRIPTION) );
        }
     delete dlg;
     lineEdit_Produit->setFocus();
     lineEdit_Produit->selectAll ( );
    }
 else if (link.mid(5,3) == "New")
    {QString cip("");
     QString tableIndex("5");        //case 5: table = m_MEDICA_INDEX_ACCS_TBL_NAME;  break;
     pos = link.find("==");
     if (pos != -1)
        {cip          = link.mid(8,pos-8);
         tableIndex   = link.mid(pos+2);
        }
     //............................. dialogue de modification des posologies ..........................................................
     /*                   m_MEDICA_INDEX_LIBELLE  + "," +            // 0
                          m_MEDICA_INDEX_CD_PROD  + "," +            // 1
                          m_MEDICA_INDEX_PK_T00   + "," +            // 2
                          m_MEDICA_INDEX_PK_T4B   + "," +            // 3
                          m_MEDICA_INDEX_CIP      + "," +            // 4
                          m_MEDICA_INDEX_CD_UV    + "," +            // 5
                          m_MEDICA_INDEX_IS_GENE  + "," +            // 6
                          m_MEDICA_INDEX_IS_DISPO + " " +            // 7
     */

     //................. recuperer code datasemp pour monographie ........................................
     QString     monographie("");
     QStringList codesDatasemp;
     if (m_pCMedBase->Medica_GetMedicamentDatasempTablesCodesByCIP(cip, codesDatasemp, tableIndex.toInt()))
        { monographie = m_pCMedBase->Datasemp_GetMonographie( codesDatasemp[5], codesDatasemp[3]);
        }
     Dlg_PosoManager *dlg = new Dlg_PosoManager(this, "ManagerPoso");
     if (dlg==0)                   return;
     dlg->initDialog(m_PosoCIP ,  tableIndex, monographie , listView_Produits);
     if (dlg->exec()== QDialog::Accepted)
        {
        }
     delete dlg;
    }
}

//------------------------------------------- pushButtonEditPoso_clicked ------------------------------------------------------------------
void Dlg_MedicaTux::pushButtonEditPoso_clicked()
{    //................. recuperer code datasemp pour monographie ........................................
     QString     monographie("");
     QStringList codesDatasemp;
     if (m_pCMedBase->Medica_GetMedicamentDatasempTablesCodesByCIP(m_PosoCIP, codesDatasemp, comboBox_Specialites->currentItem ()+1) )
        { monographie = m_pCMedBase->Datasemp_GetMonographie( codesDatasemp[5], codesDatasemp[3]);
        }

     // page += "<br><b>2)<a href=\"#PosoNew"+ cip + "==" +QString::number(tableIndex)+ "\"> Pas de données sur la posologie, en créer de nouvelles</b></a></span>";
     Dlg_PosoManager *dlg = new Dlg_PosoManager(this, "ManagerPoso");
     if (dlg==0)                   return;
     dlg->initDialog(m_PosoCIP,  QString::number(comboBox_Specialites->currentItem ()+1) , monographie, listView_Produits);
     if (dlg->exec()== QDialog::Accepted)
        {
        }
     delete dlg;
     listView_Produits_clicked( listView_Produits->currentItem () );
}


//------------------------------------------- textBrowser_Posologies_anchorClicked ------------------------------------------------------------------
void Dlg_MedicaTux::textBrowser_PrescriptionRefresh(QPL_CPosologie &posoList)
{QString spoids(""), staille("");
 spoids       = textLabel_Poids->text();
 staille      = textLabel_Taille->text();
 float poids  = m_pCMedBase->Medica_ExtractNumber(spoids);
 float taille = m_pCMedBase->Medica_ExtractNumber(staille);
 spoids       = QString::number(poids);
 staille      = QString::number(taille);
 //............................ encore un petit effort placer le poids .....................................................
 /*
 if (G_pCApp->m_pDrTux->m_pCMDI_Terrain)
    {if (poids<=0 ) spoids  = G_pCApp->m_pDrTux->m_pCMDI_Terrain->GetPoids()  ;                  // cela permet de sauver le poids
     if (taille<=0 )staille = G_pCApp->m_pDrTux->m_pCMDI_Terrain->GetTaille() ;
    }
*/
 textBrowser_Prescription->setText(m_pCMedBase->Medica_PosologieToHtmlOrdo(posoList , spoids , staille ));
}

//------------------------------------------- pushButtonPrescErase_clicked ------------------------------------------------------------------
void Dlg_MedicaTux::pushButtonPrescErase_clicked()
{m_CPosologieOrdoList.clear();
 textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
}

//----------------------------------------------------- pushButtonALD_Off_clicked ----------------------------------------------------------------------------------
void Dlg_MedicaTux::pushButtonALD_Off_clicked()
{    for (QPL_CPosologie::iterator it = m_CPosologieOrdoList.begin(); it !=  m_CPosologieOrdoList.end(); ++it )
         {(*it).m_ALD = 0;
         }
     textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
}

//----------------------------------------------------- pushButtonALD_clicked ----------------------------------------------------------------------------------
void Dlg_MedicaTux::pushButtonALD_clicked()
{    for (QPL_CPosologie::iterator it = m_CPosologieOrdoList.begin(); it !=  m_CPosologieOrdoList.end(); ++it )
         {(*it).m_ALD = 1;
         }
     textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
}

//----------------------------------------------------- comboBoxPendant_activated ----------------------------------------------------------------------------------
void Dlg_MedicaTux::comboBoxPendant_activated( int item)
{    CPrtQListBoxItem *pCPrtQListBoxItem = (CPrtQListBoxItem*) comboBoxPendant->listBox()->item(item);
     if (pCPrtQListBoxItem == 0) return;
     float n   = m_pCMedBase->Medica_ExtractNumber(pCPrtQListBoxItem->GetPk());
     if (n<=0)                   return;
     for (QPL_CPosologie::iterator it = m_CPosologieOrdoList.begin(); it !=  m_CPosologieOrdoList.end(); ++it )
         {(*it).m_MEDICA_POSO_PENDANT = QString::number(n*86400);
         }
     textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
}

//----------------------------------------------------- comboBoxPendant_textChanged ----------------------------------------------------------------------------------
void Dlg_MedicaTux::comboBoxPendant_textChanged( const QString &text )
{float n = m_pCMedBase->Medica_dureeMenuTextToSecond( text );
 if (n<=0) return;
 for (QPL_CPosologie::iterator it = m_CPosologieOrdoList.begin(); it !=  m_CPosologieOrdoList.end(); ++it )
     {
      (*it).m_MEDICA_POSO_PENDANT = QString::number(n);
     }
     textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
}

//------------------------------------------- textBrowser_Prescription_anchorClicked ------------------------------------------------------------------
void Dlg_MedicaTux::textBrowser_Prescription_anchorClicked( const QString &, const QString &link )
{int              pos_deb;
 int              pos_end;
 QString            value;
 QString            tmpQStr;
 int           action_typ;

 //......................... si click sur nom du medicament ......................................
 //                         alors dialogue de modification des posologies
 if ( (pos_deb  = link.find("#OrdoMed")) != -1 && (pos_end = link.find("=="))!= -1)
    {value      = link.mid(pos_deb+8, pos_end-pos_deb);
     action_typ = 'G';                                    // dialogue de modification général des posologies
    }
 //......................... si click sur posologie manuelle ......................................
 //                         alors dialogue de modification des posologies
 else if ( (pos_deb  = link.find("#OrdoMan")) != -1 && (pos_end = link.find("=="))!= -1)
    {value      = link.mid(pos_deb+8, pos_end-pos_deb);
     action_typ = 'U';                                    // dialogue de modification général des posologies
    }
 //......................... si click sur "Durée" ou " ALD: " ou "Quantité" ......................
 //                          l'action est determinée par la lettre suivant: "#Modif" du lien
 else if ( (pos_deb = link.find("#Modif")) != -1)
    {value      = link.mid(pos_deb+7);
     action_typ = link.at(6).latin1();
    }

 int  num = atoi(value);
 if (num >= (int)m_CPosologieOrdoList.count()) return;

 switch(action_typ )
  {case 'U':
       {bool ok;
	tmpQStr = m_CPosologieOrdoList[num].m_PosoTextuelle;
        tmpQStr.replace("<br />", "\n");
	tmpQStr = QInputDialog::getText(     tr("Saisie d'un texte libre"),
					     tr("Saisissez un texte libre :"),
					     QLineEdit::Normal,
                                             tmpQStr, &ok, this );
        if ( ok )
           {Convert_CRLF_To_BR(tmpQStr, m_CPosologieOrdoList[num].m_PosoTextuelle, 1);
           }
        if (m_CPosologieOrdoList[num].m_PosoTextuelle.isEmpty())
           {m_CPosologieOrdoList[num].m_PosoType       &= ~ (int)CPosologie::Textuelle;
           }
        else
           {m_CPosologieOrdoList[num].m_PosoType       |=  (int)CPosologie::Textuelle;
           }
        textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
        break;
       }
   case 'G':
       {
        if (m_CPosologieOrdoList[num].m_IdTable > 1)
           {PopupQuantiteAccessoire(m_CPosologieOrdoList[num]);
            textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
           }
        else
           {QString spoids  = QString::number( m_pCMedBase->Medica_ExtractNumber( textLabel_Poids->text()));
            QString staille = QString::number( m_pCMedBase->Medica_ExtractNumber( textLabel_Taille->text()));
            Dlg_MedicaAdjustPoso* dlg = new Dlg_MedicaAdjustPoso(this, "AdjustPoso");
            if (dlg==0)                   return;
            dlg->initDialog( m_CPosologieOrdoList[num],  /*G_pCApp->m_pDrTux->m_pCMDI_Terrain, */ &spoids, &staille);
            if (dlg->exec()== QDialog::Accepted)
               {textLabel_Poids->setText(tr("Poids : ")   + spoids  + tr(" kg"));
                textLabel_Taille->setText(tr("Taille : ") + staille + tr(" cm"));
                m_CPosologieOrdoList[num] = dlg->m_PosoIn;
                textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
               }
            delete dlg;
            lineEdit_Produit->setFocus();
            lineEdit_Produit->selectAll ( );
           }
        break;
       }
   case 'S':
        m_CPosologieOrdoList.remove( m_CPosologieOrdoList.at(num) );
        textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
        break;
   case 'D':
        if (m_CPosologieOrdoList[num].m_IdTable > 1) PopupDureeTraitement(m_CPosologieOrdoList[num], -1);
        else                                         PopupDureeTraitement(m_CPosologieOrdoList[num],  0);
        textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
        break;
   case 'W':
        m_CPosologieOrdoList[num].m_ALD=0;
        textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
        break;
   case 'N':
        m_CPosologieOrdoList[num].m_ALD=1;
        textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
        break;
   case 'X':
        m_CPosologieOrdoList[num].m_PosoType    &= ~ (int)CPosologie::Structuree;
        textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
        break;
   case 'Y':
        m_CPosologieOrdoList[num].m_PosoType    |=  (int)CPosologie::Structuree;
        textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
        break;
   case 'Q':
        m_CPosologieOrdoList[num].m_PosoType    &= ~(int)CPosologie::Textuelle;
        textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
        break;
   case 'Z':
        value = m_pCMedBase->Medica_GetMedicamentNameByCIP(m_CPosologieOrdoList[num].m_MEDICA_POSO_CIP,m_CPosologieOrdoList[num].m_IdTable);
        tabWidget_Tables->setCurrentPage (getCurrentPageID_ByTitle(tabWidget_Tables, PAGE_MONOGRAPHIE) );
        if (lineEdit_Produit->text()==value)
           { lineEdit_Produit_textChanged( value );
             listView_Produits_clicked( listView_Produits->currentItem()  );
           }
        else
           { lineEdit_Produit->setText(value);
             listView_Produits_clicked( listView_Produits->currentItem()  );
           }
        break;
   case 'T':
        //......... si texte vide on affiche dialogue de saisie texte ...................
        if (m_CPosologieOrdoList[num].m_PosoTextuelle=="")
           {bool ok;
            tmpQStr = m_CPosologieOrdoList[num].m_PosoTextuelle;
            tmpQStr.replace("<br />", "\n");
            tmpQStr = QInputDialog::getText(     tr("Saisie d'un texte libre"),
                                                 tr("Saisissez un texte libre :"),
                                                  QLineEdit::Normal,
                                                  tmpQStr, &ok, this );
            if ( ok && !tmpQStr.isEmpty())
               {Convert_CRLF_To_BR(tmpQStr, m_CPosologieOrdoList[num].m_PosoTextuelle, 1);
                m_CPosologieOrdoList[num].m_PosoType       |=  (int)CPosologie::Textuelle;
               }
            else
               {m_CPosologieOrdoList[num].m_PosoType       &= ~(int)CPosologie::Textuelle;
               }
           }
        else
           {m_CPosologieOrdoList[num].m_PosoType       |=  (int)CPosologie::Textuelle;
           }
        textBrowser_PrescriptionRefresh(m_CPosologieOrdoList);
        break;
  }
}

//----------------------------------------------------- GetOrdoList ----------------------------------------------------------------------------------
QPL_CPosologie Dlg_MedicaTux::GetOrdoList()
{return m_CPosologieOrdoList;
}

/*------------------------------ Convert_CRLF_To_BR --------------------------------------------------------------------*/
int Dlg_MedicaTux::Convert_CRLF_To_BR(const char *text, QString &dstQStr, int flag /*=0*/)
{int nb_ligne = 0;
 char     *pt = (char*) text;
 //rQTextEdit   = "<html>";
 dstQStr   = "";
 while (*pt)
    { if (*pt=='\r' || *pt=='\n')
         {if (flag==0) dstQStr += "\n";
          else         dstQStr += "<br />";
          if      (*pt=='\r' &&  pt[1]=='\n' ) pt+=2;
          else if (*pt=='\n' &&  pt[1]=='\r' ) pt+=2;
          else                                 ++pt;
          nb_ligne++;
         }
     else
         {dstQStr += pt[0];
          ++pt;
         }
    }
 //rQTextEdit   += "</html>";
 return nb_ligne;
}

//----------------------------------------------------- GetOrdoXmlStruct ----------------------------------------------------------------------------------
QString Dlg_MedicaTux::Get_OrdoXmlStruct()
{QString spoids(""), staille("");
 //QString result("");
 /*
 if (G_pCApp->m_pDrTux->m_pCMDI_Terrain)
    {spoids  = G_pCApp->m_pDrTux->m_pCMDI_Terrain->GetPoids()  ;                  // cela permet de sauver le poids
     staille = G_pCApp->m_pDrTux->m_pCMDI_Terrain->GetTaille() ;
    }
 */
 spoids  = textLabel_Poids->text();
 staille = textLabel_Taille->text();
 return m_pCMedBase->Medica_PosologieListToXMLOrdoStruct(m_CPosologieOrdoList,
                                                         QString::number(m_pCMedBase->Medica_ExtractNumber(spoids)) ,
                                                         QString::number(m_pCMedBase->Medica_ExtractNumber(staille))
                                                        );
}

//----------------------------------------------------- pushButton_Ok_clicked ----------------------------------------------------------------------------------
void Dlg_MedicaTux::pushButton_Ok_clicked()
{QString spoids(""), staille("");
 spoids       = textLabel_Poids->text();
 staille      = textLabel_Taille->text();
 float poids  = m_pCMedBase->Medica_ExtractNumber(spoids);
 float taille = m_pCMedBase->Medica_ExtractNumber(staille);
 spoids       = QString::number(poids);
 staille      = QString::number(taille);
 //............................ encore un petit effort placer le poids .....................................................
/*
 if (G_pCApp->m_pDrTux->m_pCMDI_Terrain)
   {if (poids>0 && taille>0)    G_pCApp->m_pDrTux->m_pCMDI_Terrain->SetPoidsTaille(spoids, staille);
    else if (poids>0)           G_pCApp->m_pDrTux->m_pCMDI_Terrain->SetPoids(spoids);
    else if (taille>0)          G_pCApp->m_pDrTux->m_pCMDI_Terrain->SetTaille(staille);
   }
*/
 accept();
}
