/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "IsExtraPk.xpm"
#include <qinputdialog.h>

#define LV_PROD_LIBELLE    0
#define LV_PROD_CODE_PROD  1
#define LV_PROD_PKT00      2
#define LV_PROD_PKT4B      3
#define LV_PROD_CIP        4
#define LV_PROD_CODE_UV    5

//--------------------------------------------- init -------------------------------------------------------------------------------
void Dlg_PosoManager::init()
{if (G_pCApp->m_pCMedicaBase==0)G_pCApp->m_pCMedicaBase = new CMedicaBase; if (G_pCApp->m_pCMedicaBase==0) return;
 //...................... creer les bases personnelles ............................................................
 G_pCApp->m_pCMedicaBase->Medica_CreateTableFormeProduit(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_PERSO_TBL_NAME, 0);
 G_pCApp->m_pCMedicaBase->Medica_CreateTableFormeProduit(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_PERSO_TBL_NAME, 0);
 G_pCApp->m_pCMedicaBase->Medica_CreateTablePosologies(G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PERSO_TBL_NAME, 0);
 pushButtonListPosoColler->hide();
 pushButtonDefPosoColler->hide();
 G_pCApp->m_pCMedicaBase->Medica_initComboDuree(comboBoxPendant);
 //...................... une connexion ...........................................................................
 connect( lineEditQUP1byUP2,   SIGNAL( textChanged(const QString&)) ,  this, SLOT(   lineEditQUP1byUP2_textChanged(const QString&)) );
 m_pDlg_HtmlView      = new Dlg_HtmlView(this, "My_QTextBrowser", FALSE);

}

//--------------------------------------------- InitDialog -----------------------------------------------------------------------------------------
void Dlg_PosoManager::initDialog( QString cip , const QString &tableSpecIndex, const QString &/*monographie*/, QListView *pQListView)
{m_TableSpecIndex = tableSpecIndex;

 QStringList unitList;
 if (G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, unitList)>0)comboBoxUP_1->insertStringList (unitList);
 if (G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, unitList)>0)comboBoxUP_2->insertStringList (unitList);
 m_CIP            = cip;
 ListViewProduitCopy(pQListView);      // copie la listView des produits du module Medica dans celuis de PosoManager

 connect( lineEdit_PoidsTest,  SIGNAL( textChanged(const QString&)) ,  this, SLOT(   lineEdit_PoidsTest_textChanged(const QString&)) );
 connect( lineEdit_TailleTest, SIGNAL( textChanged(const QString&)) ,  this, SLOT(   lineEdit_TailleTest_textChanged(const QString&)) );
 connect( lineEdit_QMin_UP1,   SIGNAL( textChanged(const QString&)) ,  this, SLOT(   lineEdit_QMin_UP1_textChanged(const QString&)) );
 connect( lineEdit_QMax_UP1,   SIGNAL( textChanged(const QString&)) ,  this, SLOT(   lineEdit_QMax_UP1_textChanged(const QString&)) );
 connect( lineEditFactCorpQU,  SIGNAL( textChanged(const QString&)) ,  this, SLOT(   lineEditFactCorpQU_textChanged(const QString&)) );
}

//--------------------------------------- destroy -----------------------------------------------------------------------------------------
void Dlg_PosoManager::destroy ()
{if (m_pDlg_HtmlView ) delete m_pDlg_HtmlView ;
 m_pDlg_HtmlView  = 0;
}

//--------------------------------------------- lineEdit_PoidsTest_textChanged -------------------------------------------------------------------------
void Dlg_PosoManager::lineEdit_PoidsTest_textChanged(const QString&)
{DisplayResult();
}

//--------------------------------------------- lineEdit_TailleTest_textChanged -------------------------------------------------------------------------
void Dlg_PosoManager::lineEdit_TailleTest_textChanged(const QString&)
{DisplayResult();
}

//--------------------------------------------- lineEdit_QMin_UP1_textChanged -------------------------------------------------------------------------
void Dlg_PosoManager::lineEdit_QMin_UP1_textChanged(const QString&)
{DisplayResult();
}

//--------------------------------------------- lineEdit_QMax_UP1_textChanged -------------------------------------------------------------------------
void Dlg_PosoManager::lineEdit_QMax_UP1_textChanged(const QString&)
{DisplayResult();
}

//--------------------------------------------- lineEditFactCorpQU_textChanged -------------------------------------------------------------------------
void Dlg_PosoManager::lineEditFactCorpQU_textChanged(const QString&)
{DisplayResult();
}

//--------------------------------------------- ListViewProduitCopy -----------------------------------------------------------------------------------------
void Dlg_PosoManager::ListViewProduitCopy(QListView *pQListView)
{listView_Produits->clear();
 QListViewItemIterator it( pQListView );
 QListViewItem *pQListViewSel = 0;
 while ( it.current() )
       {QListViewItem *item = it.current();
        ++it;
        QListViewItem *pQListViewItem = new QListViewItem(listView_Produits);
        if (pQListViewItem)
           {pQListViewItem->setText(0, item->text(0) );
            pQListViewItem->setText(1, item->text(1) );
            pQListViewItem->setText(2, item->text(2) );
            pQListViewItem->setText(3, item->text(3) );
            pQListViewItem->setText(4, item->text(4) );
            pQListViewItem->setText(5, item->text(5) );
            pQListViewItem->setText(6, item->text(6) );
            pQListViewItem->setText(7, item->text(7) );
           }
        if ( item->isSelected() ) pQListViewSel = pQListViewItem;
        }
 if (pQListViewSel != 0)
    {listView_Produits->setSelected( pQListViewSel , TRUE);
     listView_Produits->setCurrentItem(pQListViewSel );
     listView_Produits_clicked(pQListViewSel );
    }
}

//------------------------------------------- listView_Produits_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::listView_Produits_clicked( QListViewItem *pQListViewItem )
{listView_Produits_Changed(pQListViewItem );
}


//------------------------------------------- listView_Produits_currentChanged -------------------------------------------------------------------------
void Dlg_PosoManager::listView_Produits_currentChanged( QListViewItem *pQListViewItem )
{listView_Produits_Changed(pQListViewItem );
}

//------------------------------------------- listView_Produits_Changed -------------------------------------------------------------------------
void Dlg_PosoManager::listView_Produits_Changed( QListViewItem *pQListViewItem )
{if (pQListViewItem==0) return;
 m_CIP        = pQListViewItem->text(LV_PROD_CIP);
 //.............................. extraire et afficher la monographie ............................................
 //QString monographie(G_pCApp->m_pCMedicaBase->Datasemp_GetMonographie(pQListViewItem->text(LV_PROD_CODE_UV), pQListViewItem->text(LV_PROD_PKT4B)));
 QString monographie(G_pCApp->m_pCMedicaBase->Datasemp_GetMonographieByCIP(pQListViewItem->text(LV_PROD_CIP)));
 if (m_pDlg_HtmlView)
    {if (monographie.length()==0)
        {delete m_pDlg_HtmlView;
         m_pDlg_HtmlView = 0;
        }
     else
        {m_pDlg_HtmlView->textBrowser->setText(monographie);
         m_pDlg_HtmlView->show();
        }
    }
 //................................. nom du medoc dans titre.......................................................
 QString name = G_pCApp->m_pCMedicaBase->Medica_GetMedicamentNameByCIP(m_CIP, m_TableSpecIndex.toInt());
 setCaption(tr("Modifier la posologie pour: ") + name );
 //................................. extraire liste des posologies pour ce medoc ..................................
 listView_MedicaPoso->clear();
 QPL_CPosologie list_CPosologie;
 G_pCApp->m_pCMedicaBase->Medica_GetPosologies(m_CIP, list_CPosologie );
 for ( QPL_CPosologie::iterator it = list_CPosologie.begin(); it !=  list_CPosologie.end(); ++it )
     {QListViewItem *element = new QListViewItem(listView_MedicaPoso);
      if (element)
         {QString unit;
          element->setText ( 0, (*it).m_MEDICA_POSO_PK );            // 0  clef primaire
          element->setText ( 1, (*it).m_MEDICA_POSO_CIP);            // 1  code CIP
          element->setText ( 2, (*it).m_MEDICA_POSO_LIBELLE);        // 2  Libelle précisant l'indication particulière au contexte
          element->setText ( 3, (*it).m_MEDICA_POSO_DOSE_MIN);       // 9  dose minimum de la posologie pour cette indication
          element->setText ( 4, (*it).m_MEDICA_POSO_DOSE_MAX);       // 10 dose maximum de la posologie pour cette indication
          unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, (*it).m_MEDICA_POSO_DOSE_UNIT);
          element->setText ( 5, unit);                               // 11 unite de la dose posologie pour cette indication
          element->setText ( 6, (*it).m_MEDICA_POSO_FACT_CORP_QU);   // 12 quantité d'unité de facteur corporel pour cette dose
          unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, (*it).m_MEDICA_POSO_FACT_CORP_UNIT);
          element->setText ( 7, unit);                               // 13 unité de facteur corporel pour cette dose
          element->setText ( 8,  (*it).m_MEDICA_POSO_PERIODE);       // 14 période en secondes pour lauqelle est exprimée cette posologie
          element->setText ( 9,  (*it).m_MEDICA_POSO_EQUI_COEFF);    // 15 coefficient pour transformer l'unite de prise 1 en unite de prise 2
          unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, (*it).m_MEDICA_POSO_UNIT_COEFF);
          element->setText ( 10, unit);                              // 16 unite deuxième forme de la dose posologie pour cette indication
          if ((*it).m_MEDICA_POSO_EXTRA_PK.length() != 0 &&
              (*it).m_MEDICA_POSO_EXTRA_PK != "0"
             ) element->setPixmap ( 2, QPixmap (IsExtraPk_xpm) );    // si perso Placer l'icone qui va bien
         }
    }
 //...................... selectionner première poso .............................................
 if (listView_MedicaPoso->childCount () >0)
    {QListViewItem * item = listView_MedicaPoso->firstChild ();
     if (item != 0)
        {listView_MedicaPoso->setSelected( item , TRUE);
         listView_MedicaPoso->setCurrentItem(item );
         listView_MedicaPoso_clicked(item );

        }  //endif (item != 0)
     pushButton_RecordPoso->setEnabled(TRUE);
     pushButton_ErasePoso->setEnabled(TRUE);
    }//endif (listView_MedicaPoso->count()>0)
 else
    {pushButton_RecordPoso->setEnabled(FALSE);
     pushButton_ErasePoso->setEnabled(FALSE);
    }
 listView_Produits->ensureItemVisible (pQListViewItem );
}


//--------------------------------------------- listView_MedicaPoso_currentChanged -------------------------------------------------------------------------
void Dlg_PosoManager::listView_MedicaPoso_currentChanged( QListViewItem *pQListViewItem )
{listView_ItemToInterface( pQListViewItem );
}

//--------------------------------------------- listView_MedicaPoso_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::listView_MedicaPoso_clicked( QListViewItem * pQListViewItem )
{listView_ItemToInterface( pQListViewItem );
}

//--------------------------------------------- listView_MedicaPoso_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::listView_ItemToInterface( QListViewItem * pQListViewItem )
{if (pQListViewItem==0) return;
 CPosologie poso;
 G_pCApp->m_pCMedicaBase->Medica_GetPosologies(pQListViewItem->text(0), poso );   // recuperer la poso selectionnée dans la listview
 CPosologieToInterface(poso);                                                 // la placer dans l'interface
}

//------------------------------------------------ CPosologieToInterface -------------------------------------------------------------------------
void Dlg_PosoManager::CPosologieToInterface(CPosologie &poso)
{ QString unit("");
 m_CPosologie = poso;
 //.............. Placer coeff au debut  ..................................
 //               sinon car lors setText des autres champs --> cela connecte sur
 //               des routines qui se servent de cette valeur
 lineEditQUP1byUP2->setText(m_CPosologie.m_MEDICA_POSO_EQUI_COEFF);
 //................... placer age et poids ................................
 lineEdit_AgeMin->setText(m_CPosologie.m_MEDICA_POSO_AGE_MIN);
 lineEdit_AgeMax->setText(m_CPosologie.m_MEDICA_POSO_AGE_MAX);
 lineEdit_EdPo_PoidsMin->setText(m_CPosologie.m_MEDICA_POSO_POIDS_MIN);
 lineEdit_EdPo_PoidsMax->setText(m_CPosologie.m_MEDICA_POSO_POIDS_MAX);
 //.................. doses min et max .....................................
 lineEdit_Indication->setText(m_CPosologie.m_MEDICA_POSO_LIBELLE);
 lineEdit_Terrain->setText(m_CPosologie.m_MEDICA_POSO_TERRAIN);
 lineEdit_QMin_UP1->setText(m_CPosologie.m_MEDICA_POSO_DOSE_MIN);
 lineEdit_QMax_UP1->setText(m_CPosologie.m_MEDICA_POSO_DOSE_MAX);
 unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, m_CPosologie.m_MEDICA_POSO_DOSE_UNIT);
 comboBoxUP_1->setEditText (unit);
 SetComboOnThisLibelle(comboBoxUP_1 , unit);
 //.............. maxi possible ...........................................
 lineEdit_UP1_Max->setText(m_CPosologie.m_MEDICA_POSO_DOSE_LIMIT_MAX);
 //............. facteur corporel .........................................
 lineEditFactCorpQU->setText(m_CPosologie.m_MEDICA_POSO_FACT_CORP_QU);
 unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, m_CPosologie.m_MEDICA_POSO_FACT_CORP_UNIT);
 //SetComboOnLibelle(comboBox_FactCorpU, unit);
 if      (unit == "m2")        comboBoxFactCorpUnit->setCurrentItem (1);
 else if (unit == "Kg")        comboBoxFactCorpUnit->setCurrentItem (0);
 else                          comboBoxFactCorpUnit->setCurrentItem (2);

 lineEdit_Periode->setText(m_CPosologie.m_MEDICA_POSO_PERIODE);
 lineEdit_SpecPrise->setText(m_CPosologie.m_MEDICA_POSO_REPART_PRISES);
 //............................. sexe ..................................................
 if        (m_CPosologie.m_MEDICA_POSO_SEXE[0]=='F')  comboBox_Sexe->setCurrentItem ( 0 );  // 0
 else if   (m_CPosologie.m_MEDICA_POSO_SEXE[0]=='M')  comboBox_Sexe->setCurrentItem ( 1 );  // 1
 else                                                 comboBox_Sexe->setCurrentItem ( 2 );  // 2
 //.............................. note ........................................................
 //lineEdit_Note->setText(m_CPosologie.m_MEDICA_POSO_DIVERS);
 //......................unite coeff ...........................................................
 unit         = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, m_CPosologie.m_MEDICA_POSO_UNIT_COEFF);
 comboBoxUP_2->setEditText (unit);
 SetComboOnThisLibelle(comboBoxUP_2 , unit);
 DisplayResult();
 //.............................. note ........................................................
 if (m_CPosologie.m_MEDICA_POSO_DIVERS.left(2)=="Ok")            m_CPosologie.m_MEDICA_POSO_DIVERS="";
 else if (m_CPosologie.m_MEDICA_POSO_DIVERS.left(2)=="No" &&
          m_CPosologie.m_MEDICA_POSO_DIVERS.find("E:")!=-1)      m_CPosologie.m_MEDICA_POSO_DIVERS="";
 lineEdit_Note->setText(m_CPosologie.m_MEDICA_POSO_DIVERS);
 //.............................. pendant ........................................................
 G_pCApp->m_pCMedicaBase->Medica_initComboDuree(comboBoxPendant);
 if (m_CPosologie.m_MEDICA_POSO_PENDANT.length())
    { //comboBoxPendant->setEditText (QString::number(m_CPosologie.m_MEDICA_POSO_PENDANT.toFloat()/86400.0)+ " Jour(s)");
      G_pCApp->m_pCMedicaBase->Medica_SetComboOnDuree(comboBoxPendant, m_CPosologie.m_MEDICA_POSO_PENDANT);
    }
 //.............................. secabilite ........................................................
 if (m_CPosologie.m_MEDICA_POSO_SECABILITE.length()==0)
    {m_CPosologie.m_MEDICA_POSO_SECABILITE = G_pCApp->m_pCMedicaBase->Medica_GetSecabilite( m_CPosologie.m_MEDICA_POSO_CIP);
    }
 int secabilite = m_CPosologie.m_MEDICA_POSO_SECABILITE.toInt();

 radioButtonSecable1->setChecked(FALSE);
 radioButtonSecable2->setChecked(FALSE);
 radioButtonSecable4->setChecked(FALSE);

 if      (secabilite==4) radioButtonSecable4->setChecked(TRUE);
 else if (secabilite==2) radioButtonSecable2->setChecked(TRUE);
 else                    radioButtonSecable1->setChecked(TRUE);

}

//------------------------------------------------ InterfaceToCPosologie -------------------------------------------------------------------------
void Dlg_PosoManager::InterfaceToCPosologie(CPosologie &poso)
{int posoAdjust;
 if (pushButton_A->state () == QButton::On ) posoAdjust = -slider_VarPoso->value()-1; // m_PosoAdjust 1 à 100  slider_VarPoso 0 à 99
 else                                        posoAdjust =  slider_VarPoso->value()+1; // m_PosoAdjust 1 à 100  slider_VarPoso 0 à 99
 poso.setData (  m_CIP,
                 "",                                     // pk
                 lineEdit_Indication->text(),            // libelle
                 lineEdit_AgeMin->text(),
                 lineEdit_AgeMax->text(),
                 ComboSexeToVal(),
                 lineEdit_Terrain->text(),
                 lineEdit_EdPo_PoidsMin->text(),
                 lineEdit_EdPo_PoidsMax->text(),
                 lineEdit_QMin_UP1->text(),
                 lineEdit_QMax_UP1->text(),
                 G_pCApp->m_pCMedicaBase->Medica_GetUniteID(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, comboBoxUP_1->currentText()),
                 lineEdit_UP1_Max->text(),
                 lineEditFactCorpQU->text(),
                 G_pCApp->m_pCMedicaBase->Medica_GetUniteID(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, ComboFactCorpUnitToUnit()),
                 lineEdit_Periode->text(),
                 lineEditQUP1byUP2->text(),               // coeff
                 G_pCApp->m_pCMedicaBase->Medica_GetUniteID(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, comboBoxUP_2->currentText()),
                 QString::number(ComputeNbPrises(lineEdit_SpecPrise->text())),
                 lineEdit_SpecPrise->text(),
                 lineEdit_Note->text() ,                                        // divers  Medica_GetComboCPrtDuree(QComboBox *comboBoxPendant);
                 QString::number( G_pCApp->m_pCMedicaBase->Medica_GetComboCPrtDuree(comboBoxPendant)),               // duree conseillée du traitement (Pendant)
                 QString::number(getSecabilite())  ,
                 "" ,                                                           // extraPk
                 "" ,    // note
                 posoAdjust ,
                 1  ,                       // numero d'ordre du produit dans l'ordo ou la liste (utile pour affichage)
                 "" ,                       // poso textuelle
                 0  ,                       // ald
                 CPosologie::Structuree,    // CPosologie::Nothing  CPosologie::Textuelle  CPosologie::Structuree  CPosologie::AllMode
                 1                          // Id Table
              );
}

//--------------------------------------------- DisplayResult -------------------------------------------------------------------------
void Dlg_PosoManager::DisplayResult()
{
 CPosologie poso;
 InterfaceToCPosologie(poso);
 //........................... deuxième forme.........................................................
 QString upMin, upMax;
 ComputeSecondForm(poso, upMin, upMax);
 lineEdit_QMin_UP2->setText(upMin);
 lineEdit_QMax_UP2->setText(upMax);
 //..................... afficher le resumé dans la vue html ...............................................................
 textBrowser_Result->setText(G_pCApp->m_pCMedicaBase->Medica_PosologieToPrescriptionHtml(  poso                        ,
                                                                                       lineEdit_PoidsTest->text()  ,
                                                                                       lineEdit_TailleTest->text()
                                                                                    )
                           );
 //........................... afficher le nbr de UP1 par UP2 ........................................................................
 if (lineEditQUP1byUP2->text().toFloat() != 0)
     textLabelCoeffResult->setText(QString("<b>") + comboBoxUP_1->currentText() + "</b> par <b>" + comboBoxUP_2->currentText()+"</b>");
 else
     textLabelCoeffResult->setText("");
}

//-------------------------------------- ComputeNbPrises -------------------------------------------------------
int Dlg_PosoManager::ComputeNbPrises(const QString &spec)
{int pos;
 if ( (pos = spec.find("XJO")) != -1)
    {return spec.left(pos).toInt();
    }
 int nb    = spec.length();
 int nbRet = 1;
 for (int i=0; i<nb; ++i) if (spec[i]==';') ++nbRet;
 return nbRet;
}

//-------------------------------------- ComputeSecondForm -------------------------------------------------------
//
void Dlg_PosoManager::ComputeSecondForm(const CPosologie &/*poso*/, QString &upMin, QString &upMax)
{upMin = upMax =  "0";
 if (lineEditQUP1byUP2->text() == "0" || lineEditQUP1byUP2->text().length()==0) return;
 float poids(  lineEdit_PoidsTest->text().toFloat() );
 float taille( lineEdit_TailleTest->text().toFloat() );
 float fMin(0), fMax(0), coeff(1/lineEditQUP1byUP2->text().toFloat());
 //..................... multiplier par le coefficient par le poids ou la surface ............................
 QString factCorpU = ComboFactCorpUnitToUnit();
 if (lineEditFactCorpQU->text() != "0")
    {if      (factCorpU=="Kg" && poids>0)
             {coeff *= poids / lineEditFactCorpQU->text().toFloat();
             }
     else if (factCorpU=="m2" && taille>0 && poids >0)
             {coeff *= sqrt(taille * poids / 3600) / lineEditFactCorpQU->text().toFloat();  //(Mosteller)
             }
    }
 //..................... calculer les valeurrs finales Min et Max ............................
 fMin = lineEdit_QMin_UP1->text().toFloat() * coeff;
 if (lineEdit_QMax_UP1->text() != 0)
    {fMax = lineEdit_QMax_UP1->text().toFloat() * coeff;
    }
 upMin  = QString::number( G_pCApp->m_pCMedicaBase->Medica_ArondirUniteDePrise(fMin, getSecabilite() ));
 upMax  = QString::number( G_pCApp->m_pCMedicaBase->Medica_ArondirUniteDePrise(fMax, getSecabilite() ));
}

//------------------------------------------- slider_VarPoso_valueChanged -------------------------------------------------------------------------
void Dlg_PosoManager::slider_VarPoso_valueChanged( int )
{DisplayResult();
}

//-------------------------------------- ExtractNumber ----------------------------------------------------------------
float Dlg_PosoManager::ExtractNumber(const QString text)
{QString str("");
 int end = text.length();
 int   i = -1;
 while (++i < end)
       {if ( (text[i]>='0' && text[i]<='9') || text[i]=='.'|| text[i]==',') str += text[i];
       }
 return str.toFloat();
}

//-------------------------------------- ComboFactCorpUnitToUnit --------------------------------------------------------------
QString Dlg_PosoManager::ComboFactCorpUnitToUnit()
{int i = comboBoxFactCorpUnit->currentItem ();
 if      (i ==0) return QString("Kg");
 else if (i ==1) return QString("m2");
 return QString("");
}

//-------------------------------------- ComboSexeToVal --------------------------------------------------------------
QString Dlg_PosoManager::ComboSexeToVal()
{QString sexe = comboBox_Sexe->currentText().left(1);
 return sexe;
}

//-------------------------------------- comboBoxMomentToSpecPrises --------------------------------------------------------------
QString Dlg_PosoManager::comboBoxMomentToSpecPrises(QString &spec)
{QString nbPrises(""), nbPrisesToRet("");
 int action = 0;
 if      (comboBoxMoment->currentText()==tr("(vide)"))
         {nbPrisesToRet     = "1";
         }
 else    {nbPrisesToRet = comboBoxMoment->currentText().left(1);
         }
 action = comboBoxMoment->currentItem();
 switch(action)
    {case 1:  spec = "1XJO";           break;
     case 2:  spec = "MA";             break;
     case 3:  spec = "MI";             break;
     case 4:  spec = "SO";             break;
     case 5:  spec = "CO";             break;
     case 6:  spec = "#rDJ";           break;
     case 7:  spec = "#rMI";           break;
     case 8:  spec = "#rSO";           break;
     case 9:  spec = "MA;SO";          break;
     case 10: spec = "MA;MI";          break;
     case 11: spec = "MI;SO";          break;
     case 12: spec = "MA;CO";          break;
     case 13: spec = "#rDJ;#rSO";      break;
     case 14: spec = "#rDJ;#rMI";      break;
     case 15: spec = "#rMI;#rSO";      break;
     case 16: spec = "MA;MI;SO";       break;
     case 17: spec = "#rDJ;#rMI;#rSO"; break;
     case 18: spec = "MA;MI;AM;SO";    break;
     case 19: spec = "MA;MI;AM;SO;CO"; break;
     default:
              spec = nbPrisesToRet + "XJO";
    }
 return nbPrisesToRet;
}

//-------------------------------------- radioButtonSecable1_clicked -------------------------------------------------------
void Dlg_PosoManager::radioButtonSecable1_clicked()
{radioButtonSecable1->setChecked(FALSE);
 radioButtonSecable2->setChecked(FALSE);
 radioButtonSecable4->setChecked(FALSE);
 radioButtonSecable1->setChecked(TRUE);
 DisplayResult();
}

//-------------------------------------- radioButtonSecable2_clicked -------------------------------------------------------
void Dlg_PosoManager::radioButtonSecable2_clicked()
{radioButtonSecable1->setChecked(FALSE);
 radioButtonSecable2->setChecked(FALSE);
 radioButtonSecable4->setChecked(FALSE);
 radioButtonSecable2->setChecked(TRUE);
 DisplayResult();
}

//-------------------------------------- radioButtonSecable4_clicked -------------------------------------------------------
void Dlg_PosoManager::radioButtonSecable4_clicked()
{radioButtonSecable1->setChecked(FALSE);
 radioButtonSecable2->setChecked(FALSE);
 radioButtonSecable4->setChecked(FALSE);
 radioButtonSecable4->setChecked(TRUE);
 DisplayResult();
}

//-------------------------------------- radioButtonSecable4_clicked -------------------------------------------------------
int Dlg_PosoManager::getSecabilite()
{if (radioButtonSecable1->isChecked()) return 1;
 if (radioButtonSecable2->isChecked()) return 2;
 if (radioButtonSecable4->isChecked()) return 4;
 return 0;
}

//-------------------------------------- comboBoxMoment_activated -------------------------------------------------------
void Dlg_PosoManager::comboBoxMoment_activated( const QString & )
{QString schemaPrises;
 /*QString nbPrises = */ comboBoxMomentToSpecPrises(schemaPrises);
 lineEdit_SpecPrise->setText(schemaPrises);
 DisplayResult();
}

//--------------------------------------------- comboBoxUP_1_textChanged -------------------------------------------------------------------------
void Dlg_PosoManager::comboBoxUP_1_textChanged( const QString & )
{DisplayResult();
}

//--------------------------------------------- comboBoxUP_1_activated -------------------------------------------------------------------------
void Dlg_PosoManager::comboBoxUP_1_activated( int )
{lineEditQUP1byUP2_textChanged(lineEditQUP1byUP2->text());
 DisplayResult();
}

//--------------------------------------------- comboBoxUP_2_textChanged -------------------------------------------------------------------------
void Dlg_PosoManager::comboBoxUP_2_textChanged( const QString & )
{DisplayResult();
}

//--------------------------------------------- comboBoxUP_2_activated -------------------------------------------------------------------------
void Dlg_PosoManager::comboBoxUP_2_activated( int )
{DisplayResult();
}

//--------------------------------------------- lineEditQUP1byUP2_textChanged -------------------------------------------------------------------------
void Dlg_PosoManager::lineEditQUP1byUP2_textChanged(const QString &)
{ DisplayResult();
}

//------------------------------------------- SetComboOnThisLibelle -------------------------------------------------------------------------
void Dlg_PosoManager::SetComboOnThisLibelle(QComboBox *pQComboBox, const QString &value)
{int nb = pQComboBox->count();
 for (int i=0; i<nb; ++i)
     {QString str = pQComboBox->text(i);
      if (str== value)
         {pQComboBox->setCurrentItem (i);
          return;
         }
     }
}

//------------------------------------------- SetComboOnLibelle -------------------------------------------------------------------------
void Dlg_PosoManager::SetComboOnLibelle(QComboBox *pQComboBox, const QString &value)
{int nb = pQComboBox->count();
 for (int i=0; i<nb; ++i)
     {QString str = pQComboBox->text(i);
      int     pos = str.find("[~]");
      if (pos != -1 && str.left(pos).stripWhiteSpace()== value)
         {pQComboBox->setCurrentItem (i);
          //pQComboBox->setLineEdit (str.left(pos));
          return;
         }
     }
}

//------------------------------------------- comboBoxPeriode_activated -------------------------------------------------------------------------
void Dlg_PosoManager::comboBoxPeriode_activated( const QString &text )
{if (text.find("our") != -1)
    {int pos = text.find(' ');
     if (pos != -1)  lineEdit_Periode->setText(QString::number(text.left(pos).toFloat()*86400));
    }
 else if (text.find("moi") != -1)
    {int pos = text.find(' ');
     if (pos != -1)  lineEdit_Periode->setText(QString::number(text.left(pos).toFloat()*86400*31));
    }
DisplayResult();
}

//------------------------------------------- comboBoxFactCorpUnit_activated -------------------------------------------------------------------------
void Dlg_PosoManager::comboBoxFactCorpUnit_activated( int )
{DisplayResult();
}

//------------------------------------------- comboBoxPendant_activated -------------------------------------------------------------------------
void Dlg_PosoManager::comboBoxPendant_activated( const QString & )
{DisplayResult();
}

//------------------------------------------- lineEdit_SpecPrise_textChanged -------------------------------------------------------------------------
void Dlg_PosoManager::lineEdit_SpecPrise_textChanged( const QString & )
{DisplayResult();
}

//------------------------------------------- pushButtonNewUP1_Unit_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::pushButtonNewUP1_Unit_clicked()
{   UnitTablesSynchronise();
    bool ok;
    QString libelle = QInputDialog::getText( "Thérapeutique",
					     "Entrez le libellé d'unité de prise:",
					     QLineEdit::Normal,
                                             QString::null, &ok, this ).stripWhiteSpace();
    if ( ok && !libelle.isEmpty() )
       {QString pk      = G_pCApp->m_pCMedicaBase->Medica_GetUniteID(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, libelle);
         if (pk.length()==0) // si n'existe pas dans la base rajouter cet item
            {comboBoxUP_1->insertItem ( libelle  );
             int nb = comboBoxUP_1->count();
             G_pCApp->m_pCMedicaBase->Medica_UnitePriseAdd(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, libelle, QString::number(nb) );
             long lastPk = G_pCApp->m_pCMedicaBase->Medica_GetLastPrimKey(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_PERSO_TBL_NAME, G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_PK);
             G_pCApp->m_pCMedicaBase->Medica_UnitePriseAdd(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_PERSO_TBL_NAME, libelle, QString::number(lastPk+1)  );
             comboBoxUP_1->setCurrentItem (nb-1 );
            }
       }
}

//------------------------------------------- UnitTablesSynchronise -------------------------------------------------------------------------
void Dlg_PosoManager::UnitTablesSynchronise()
{//................. recuperer les listes d'unités destination avec les valeurs dèjà existantes ................................
 //                  (l'ajout d'une nouvelle poso la complete au fur et à mesure)
 QStringList formeUPList;
 QStringList formeADList;
 G_pCApp->m_pCMedicaBase->Medica_GetUnitesList(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, formeUPList, CMedicaBase::JustLibelle);  // clear() dans la fonction
 G_pCApp->m_pCMedicaBase->Medica_GetUnitesList(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, formeADList, CMedicaBase::JustLibelle);
 //.................................. synchroniser la liste perso avec la liste d'unité principale ............................
 //                                   il faut qu'un ajout d'une unite ai le meme PK
 G_pCApp->m_pCMedicaBase->Medica_CreateTableFormeProduit(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_PERSO_TBL_NAME, formeUPList);
 G_pCApp->m_pCMedicaBase->Medica_CreateTableFormeProduit(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_PERSO_TBL_NAME, formeADList);
}

//------------------------------------------- pushButtonNewUP2_Unit_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::pushButtonNewUP2_Unit_clicked()
{
 UnitTablesSynchronise();
 bool ok;
 QString libelle = QInputDialog::getText( "Thérapeutique",
                                             "Entrez le libellé d'unité de prise:",
                                             QLineEdit::Normal,
                                             QString::null, &ok, this ).stripWhiteSpace();
 if ( ok && !libelle.isEmpty() )
    {
         QString pk      = G_pCApp->m_pCMedicaBase->Medica_GetUniteID(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, libelle);
         if (pk.length()==0) // si n'existe pas dans la base rajouter cet item
            {comboBoxUP_2->insertItem ( libelle  );
             int nb = comboBoxUP_2->count();
             G_pCApp->m_pCMedicaBase->Medica_UnitePriseAdd(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, libelle, QString::number(nb) );
             long lastPk = G_pCApp->m_pCMedicaBase->Medica_GetLastPrimKey(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_PERSO_TBL_NAME, G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_PK);
             G_pCApp->m_pCMedicaBase->Medica_UnitePriseAdd(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_PERSO_TBL_NAME, libelle, QString::number(lastPk+1) );
             comboBoxUP_2->setCurrentItem (nb-1 );
            }

    }
}

//------------------------------------------- pushButton_AddPoso_clicked -------------------------------------------------------------------------
/*
    //......................... données structurelles .............................................
    //                          provenant de la base médicamenteuse
    QString       m_MEDICA_POSO_CIP;
    QString       m_MEDICA_POSO_PK;
    QString       m_MEDICA_POSO_LIBELLE;
    QString       m_MEDICA_POSO_AGE_MIN;
    QString       m_MEDICA_POSO_AGE_MAX;
    QString       m_MEDICA_POSO_SEXE;
    QString       m_MEDICA_POSO_TERRAIN;
    QString       m_MEDICA_POSO_POIDS_MIN;
    QString       m_MEDICA_POSO_POIDS_MAX;
    QString       m_MEDICA_POSO_DOSE_MIN;
    QString       m_MEDICA_POSO_DOSE_MAX;
    QString       m_MEDICA_POSO_DOSE_UNIT;
    QString       m_MEDICA_POSO_DOSE_LIMIT_MAX;
    QString       m_MEDICA_POSO_FACT_CORP_QU;
    QString       m_MEDICA_POSO_FACT_CORP_UNIT;
    QString       m_MEDICA_POSO_PERIODE;
    QString       m_MEDICA_POSO_EQUI_COEFF;
    QString       m_MEDICA_POSO_UNIT_COEFF;
    QString       m_MEDICA_POSO_NB_PRISES;           // nombre de prises conseillées
    QString       m_MEDICA_POSO_REPART_PRISES;       // description structurée de la répartition des prises
    QString       m_MEDICA_POSO_DIVERS;              // un peu ce que l'on veut
    QString       m_MEDICA_POSO_EXTRA_PK;            // si present = Pk de la table de poso personnelle d'ou provient la donnée
    QString       m_Note;                            // commentaire concernant cette posologie
    //......................... données fonctionnelles .............................................
    //                          non dépendantes de la base médicamenteuse mais des
    //                          ajustements de la prescription
    int           m_PosoAdjust;                     // ajustement par rapport à fourchette de doses initiales conseillées de 1 à 100 %
    int           m_Secabilite;                     // ajustement de la secabilite
    int           m_NumOrdre;                       // numero d'ordre du produit dans l'ordo ou la liste (utile pour affichage)
    QString       m_PosoTextuelle;                  // posologie manuelle ajoutée par l'utilisateur (complément textuel)
    int           m_ALD;                            // à 0 si pas en rapport avec longue maladie
    int           m_PosoType;                       // CPosologie::Nothing  CPosologie::Textuelle  CPosologie::Structuree  CPosologie::AllMode
    float         m_Duree;                          // durée de la prescription en secondes
    int           m_IdTable;                        // indice de la table d'index
*/
void Dlg_PosoManager::pushButton_AddPoso_clicked()
{//................... recuperer la posologie définie par notre boîte de dialogue .................................
 CPosologie poso;
 InterfaceToCPosologie(poso);
 AddPosoToBase(poso);
}

//------------------------------------------- AddPosoToBase ----------------------------------------------------------------------------------------
void Dlg_PosoManager::AddPosoToBase(CPosologie &poso)
{UnitTablesSynchronise();
 //................... recuperer prochain pk de la table personnelle des posologies .................................
 long lastPk = G_pCApp->m_pCMedicaBase->Medica_GetLastPrimKey(G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PERSO_TBL_NAME, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PK);
 //................... Doit être aussi ajoutée à la table personnelle ...............................................
 poso.m_MEDICA_POSO_EXTRA_PK = QString::number(lastPk+1);       // indiquer que cette posologie fait partie aussi de la table de posologies personnelles

 //................... enregistrer ..................................................................................
 G_pCApp->m_pCMedicaBase->Medica_PosologieAppendToBase(poso, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PERSO_TBL_NAME ); // enregistrer dans la table personnelle
 G_pCApp->m_pCMedicaBase->Medica_PosologieAppendToBase(poso, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_TBL_NAME );       // enregistrer dans la table globale
//................... insérer dans la liste ..........................................................................
 QListViewItem *element = new QListViewItem(listView_MedicaPoso);
 if (element)
    {lastPk = G_pCApp->m_pCMedicaBase->Medica_GetLastPrimKey(G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_TBL_NAME, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PK);
     QString unit;
     element->setText ( 0, QString::number(lastPk)  );         // 0  clef primaire
     element->setText ( 1, poso.m_MEDICA_POSO_CIP);            // 1  code CIP
     element->setText ( 2, poso.m_MEDICA_POSO_LIBELLE);        // 2  Libelle précisant l'indication particulière au contexte
     element->setText ( 3, poso.m_MEDICA_POSO_DOSE_MIN);       // 9  dose minimum de la posologie pour cette indication
     element->setText ( 4, poso.m_MEDICA_POSO_DOSE_MAX);       // 10 dose maximum de la posologie pour cette indication
     unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, poso.m_MEDICA_POSO_DOSE_UNIT);
     element->setText ( 5, unit);                               // 11 unite de la dose posologie pour cette indication
     element->setText ( 6, poso.m_MEDICA_POSO_FACT_CORP_QU);    // 12 quantité d'unité de facteur corporel pour cette dose
     unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, poso.m_MEDICA_POSO_FACT_CORP_UNIT);
     element->setText ( 7, unit);                                    // 13 unité de facteur corporel pour cette dose
     element->setText ( 8, poso.m_MEDICA_POSO_PERIODE);      // 14 période en secondes pour lauqelle est exprimée cette posologie
     element->setText ( 9, poso.m_MEDICA_POSO_EQUI_COEFF);      // 15 coefficient pour transformer l'unite de prise 1 en unite de prise 2
     unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, poso.m_MEDICA_POSO_UNIT_COEFF);
     element->setText ( 10, unit);                                   // 16 unite deuxième forme de la dose posologie pour cette indication
     if (lastPk!=0) element->setPixmap ( 2, QPixmap (IsExtraPk_xpm) );
     listView_MedicaPoso->setCurrentItem (element );
     listView_MedicaPoso->setSelected ( element, TRUE);
     listView_MedicaPoso->ensureItemVisible ( element );
     //............... ne pas oublier de mettre à jour la dernière poso selectionnée ...................
     m_CPosologie.m_MEDICA_POSO_PK       = lastPk;
     m_CPosologie.m_MEDICA_POSO_EXTRA_PK = poso.m_MEDICA_POSO_EXTRA_PK;
    }
}

//------------------------------------------- pushButton_RecordPoso_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::pushButton_RecordPoso_clicked()
{QListViewItem * pQListViewItem = listView_MedicaPoso->currentItem();
 if (pQListViewItem == 0)                                                               return;  // si pas d'item cassos
 if (pQListViewItem->text(0) != m_CPosologie.m_MEDICA_POSO_PK)                          return;  // si item listview et le dernier affiché ne correspondesnt pas cassos
 CPosologie poso;

 InterfaceToCPosologie(poso);     //G_pCApp->m_pCMedicaBase->Medica_GetComboCPrtDuree(comboBoxPendant)
 if (m_CPosologie.m_MEDICA_POSO_PK.length()== 0 || m_CPosologie.m_MEDICA_POSO_PK =="0") return;
 UnitTablesSynchronise();
 //...................... mise à jour des clefs primaires ........................................................................................
 poso.m_MEDICA_POSO_EXTRA_PK = m_CPosologie.m_MEDICA_POSO_EXTRA_PK;
 poso.m_MEDICA_POSO_PK       = m_CPosologie.m_MEDICA_POSO_PK;
 //.................... si il y a déjà un enregistrement perso correspondant  à la table globale .................................................
 if (poso.m_MEDICA_POSO_EXTRA_PK.length() !=0 && poso.m_MEDICA_POSO_EXTRA_PK != "0")
    {G_pCApp->m_pCMedicaBase->Medica_PosologieUpdateToBase(poso, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_TBL_NAME );       // mettre à jour table globale
     poso.m_MEDICA_POSO_PK = poso.m_MEDICA_POSO_EXTRA_PK;    // changer le pk pour celui de la table perso
     G_pCApp->m_pCMedicaBase->Medica_PosologieUpdateToBase(poso, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PERSO_TBL_NAME ); // mettre à jour table perso
    }
 //.................... si il y a pas d'enregistrement perso correspondant à la table globale ...................................................
 //                     en creer un (noter la modif)
 else
    {long lastPk = G_pCApp->m_pCMedicaBase->Medica_GetLastPrimKey(G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PERSO_TBL_NAME, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PK);
     poso.m_MEDICA_POSO_EXTRA_PK         = QString::number(lastPk+1);
     m_CPosologie.m_MEDICA_POSO_EXTRA_PK =  poso.m_MEDICA_POSO_EXTRA_PK;    //  mettre à jour la dernière poso selectionnée
     G_pCApp->m_pCMedicaBase->Medica_PosologieAppendToBase(poso, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PERSO_TBL_NAME ); // enregistrer dans la table personnelle
     G_pCApp->m_pCMedicaBase->Medica_PosologieUpdateToBase(poso, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_TBL_NAME );
    }
 //pQListViewItem->setPixmap ( 2, QPixmap (IsExtraPk_xpm) );
 //................ remettre à jour l'item de la listview ...............................................
 QString unit;
 pQListViewItem->setText ( 0, m_CPosologie.m_MEDICA_POSO_PK  );    // 0  clef primaire
 pQListViewItem->setText ( 1, poso.m_MEDICA_POSO_CIP);             // 1  code CIP
 pQListViewItem->setText ( 2, poso.m_MEDICA_POSO_LIBELLE);         // 2  Libelle précisant l'indication particulière au contexte
 pQListViewItem->setText ( 3, poso.m_MEDICA_POSO_DOSE_MIN);        // 9  dose minimum de la posologie pour cette indication
 pQListViewItem->setText ( 4, poso.m_MEDICA_POSO_DOSE_MAX);        // 10 dose maximum de la posologie pour cette indication
 unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, poso.m_MEDICA_POSO_DOSE_UNIT);
 pQListViewItem->setText ( 5, unit);                               // 11 unite de la dose posologie pour cette indication
 pQListViewItem->setText ( 6, poso.m_MEDICA_POSO_FACT_CORP_QU);    // 12 quantité d'unité de facteur corporel pour cette dose
 unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, poso.m_MEDICA_POSO_FACT_CORP_UNIT);
 pQListViewItem->setText ( 7, unit);                               // 13 unité de facteur corporel pour cette dose
 pQListViewItem->setText ( 8, poso.m_MEDICA_POSO_PERIODE);         // 14 période en secondes pour lauqelle est exprimée cette posologie
 pQListViewItem->setText ( 9, poso.m_MEDICA_POSO_EQUI_COEFF);      // 15 coefficient pour transformer l'unite de prise 1 en unite de prise 2
 unit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, poso.m_MEDICA_POSO_UNIT_COEFF);
 pQListViewItem->setText ( 10, unit);                              // 16 unite deuxième forme de la dose posologie pour cette indication
 if (poso.m_MEDICA_POSO_EXTRA_PK.length() != 0 && poso.m_MEDICA_POSO_EXTRA_PK != "0") pQListViewItem->setPixmap ( 2, QPixmap (IsExtraPk_xpm) );
}


//------------------------------------------- pushButton_ErasePoso_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::pushButton_ErasePoso_clicked()
{QListViewItem * pQListViewItem = listView_MedicaPoso->currentItem();
 if (pQListViewItem == 0)                                             return;  // si pas d'item cassos
 if (pQListViewItem->text(0) != m_CPosologie.m_MEDICA_POSO_PK)        return;  // si item listview et le dernier affiché ne correspondesnt pas cassos
 if (m_CPosologie.m_MEDICA_POSO_EXTRA_PK.length() !=0 && m_CPosologie.m_MEDICA_POSO_EXTRA_PK != "0")
    {G_pCApp->m_pCMedicaBase->Medica_PosologieDeleteToBase(m_CPosologie.m_MEDICA_POSO_EXTRA_PK, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PERSO_TBL_NAME );
    }
 G_pCApp->m_pCMedicaBase->Medica_PosologieDeleteToBase(m_CPosologie.m_MEDICA_POSO_PK, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_TBL_NAME );
 delete pQListViewItem;
 m_CPosologie.m_MEDICA_POSO_PK       = ""; // mettre à jour la dernière poso selectionnée
 m_CPosologie.m_MEDICA_POSO_EXTRA_PK = ""; // mettre à jour la dernière poso selectionnée

 if (listView_MedicaPoso->childCount () >0)
    {pQListViewItem = listView_MedicaPoso->firstChild ();
     if ( pQListViewItem != 0)
        {listView_MedicaPoso->setSelected( pQListViewItem , TRUE);
         listView_MedicaPoso->setCurrentItem(pQListViewItem );
         listView_MedicaPoso_clicked(pQListViewItem );
        }  //endif (item != 0)
    }//endif (listView_MedicaPoso->count()>0)
}

//------------------------------------------- pushButtonAdvancedTerrain_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::pushButtonAdvancedTerrain_clicked()
{
 Dlg_AdvancedTerrain* dlg = new Dlg_AdvancedTerrain(this,"MyDlg_AdvancedTerrain", TRUE);
 if (dlg==0) return;
 QString ageT("");
 QString pdsT("");
 //..................................... Placer Age Poids comme variables de terrain ...................................
 //                                     (sera inutile lorsque age et poids seront intégrés dans le terrain)
 if ((lineEdit_AgeMin->text().stripWhiteSpace().length() != 0 && lineEdit_AgeMin->text() !="0") ||
     (lineEdit_AgeMax->text().stripWhiteSpace().length() != 0 && lineEdit_AgeMax->text() !="0")
    )
    {ageT += "$AG:" + lineEdit_AgeMin->text() + ":" + lineEdit_AgeMax->text() + ":mois";
    }
 if ((lineEdit_EdPo_PoidsMin->text().stripWhiteSpace().length() != 0 && lineEdit_EdPo_PoidsMin->text() !="0") ||
     (lineEdit_EdPo_PoidsMax->text().stripWhiteSpace().length() != 0 && lineEdit_EdPo_PoidsMax->text() !="0")
    )
    {pdsT += "$PS:" + lineEdit_EdPo_PoidsMin->text() + ":" + lineEdit_EdPo_PoidsMax->text() + ":kg";
    }
 QString terrain = lineEdit_Terrain->text() + ageT + pdsT;
 dlg->initDialog( terrain  );
 if (dlg->exec()== QDialog::Accepted)
    {QString terrain(dlg->dialogToTerrain());
     delete dlg;
     int pos;
     int end;
     int deb;
     //..................................... Extraire Age Poids des variables de terrain ...................................
     //                                     (sera inutile lorsque age et poids seront intégrés dans le terrain)
     if ( (pos = terrain.find("$AG")) != -1 )
        {  deb  = pos;
           pos += 4;                                              // passer le premier    ':'
           end  = terrain.find(':',pos);                          // trouver le deuxième  ':'
           lineEdit_AgeMin->setText(terrain.mid(pos, end-pos));
           pos  = end + 1;  end = terrain.find(':',pos);           // trouver le troisième ':'
           lineEdit_AgeMax->setText(terrain.mid(pos, end-pos));
           //........ virer la donnée: $AG:XX:XX:mois de la chaine terrrain ..............
           end  = terrain.find("mois", pos);
           if (end != -1) terrain.remove(deb, end-deb+4);
        }
     if ( (pos  = terrain.find("$PS")) != -1 )
        {  deb  = pos;
           pos += 4; // passer le premier ':'
           end  = terrain.find(':',pos);
           lineEdit_EdPo_PoidsMin->setText(terrain.mid(pos, end-pos));
           pos  = end + 1;  end = terrain.find(':',pos);
           lineEdit_EdPo_PoidsMax->setText(terrain.mid(pos, end-pos));
           //........ virer la donnée: $PS:XX:XX:Kg de la chaine terrrain ..............
           end  = terrain.find("Kg", pos);
           if (end != -1) terrain.remove(deb, end-deb+2);
        }
     lineEdit_Terrain->setText(terrain);
    }
}

//------------------------------------------- pushButtonListPosoCopier_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::pushButtonListPosoCopier_clicked()
{m_CPosologieListMem.clear();
 G_pCApp->m_pCMedicaBase->Medica_GetPosologies(m_CIP, m_CPosologieListMem );
 if (m_CPosologieListMem.count())
    {pushButtonListPosoColler->show();
    }
}

//------------------------------------------- pushButtonListPosoColler_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::pushButtonListPosoColler_clicked()
{if (m_CPosologieListMem.count()==0)
    {pushButtonListPosoColler->hide();
     return;
    }
 switch( QMessageBox::information( this, tr(" Coller une liste de posologie "),
                                             tr ( " Désirez vous placer la liste de posologies du produit : ")     +
                                             G_pCApp->m_pCMedicaBase->Medica_GetMedicamentNameByCIP(m_CPosologieListMem[0].m_MEDICA_POSO_CIP, m_TableSpecIndex.toInt()) +
                                             tr(  "\r\n sur celui en cours : \r\n") +
                                             G_pCApp->m_pCMedicaBase->Medica_GetMedicamentNameByCIP(m_CIP, m_TableSpecIndex.toInt()) ,
                                             tr("Oui"), tr("Non"), 0,
                                             0, 1 ) )
            {case 0:
                   AddPosoListToBase(m_CPosologieListMem);
                   return;
             case 1:
                   return;
             case 2:
             default: // just for sanity
                   return;
              }

}

//-------------------------------------------------- AddPosoListToBase --------------------------------------------------------------------------------
void Dlg_PosoManager::AddPosoListToBase(QPL_CPosologie &list_CPosologie)
{QListViewItem *pQListViewItem = listView_Produits->currentItem();
 if (pQListViewItem==0)                                                      return;
 if (pQListViewItem->text(LV_PROD_CIP) != m_CIP)                             return; // si produit sélectionné n'a pas le méme CIP que interface
 CPosologie poso;
 long lastPk = G_pCApp->m_pCMedicaBase->Medica_GetLastPrimKey(G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PERSO_TBL_NAME, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PK)+1;

 for ( QPL_CPosologie::iterator it = list_CPosologie.begin(); it !=  list_CPosologie.end(); ++it )
     {poso = (*it);
      poso.m_MEDICA_POSO_CIP      = m_CIP;                                           // attribuer CIP du produit de destination
      //................... recuperer prochain pk de la table personnelle des posologies .................................
      //................... Doit être aussi ajoutée à la table personnelle ...............................................
      poso.m_MEDICA_POSO_EXTRA_PK = QString::number(lastPk);       // indiquer que cette posologie fait partie aussi de la table de posologies personnelles
      //................... enregistrer ..................................................................................
      G_pCApp->m_pCMedicaBase->Medica_PosologieAppendToBase(poso, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_PERSO_TBL_NAME ); // enregistrer dans la table personnelle
      G_pCApp->m_pCMedicaBase->Medica_PosologieAppendToBase(poso, G_pCApp->m_pCMedicaBase->m_MEDICA_POSO_TBL_NAME );       // enregistrer dans la table globale
      lastPk++; // on peut supposer qu'il est incrémenté ??
    }
 listView_Produits_Changed( pQListViewItem ); // reinitialiser l'interface
}

//------------------------------------------- pushButtonDefPosoCopier_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::pushButtonDefPosoCopier_clicked()
{InterfaceToCPosologie(m_CPosologieMem);
 pushButtonDefPosoColler->show();
}

//------------------------------------------- pushButtonDefPosoColler_clicked -------------------------------------------------------------------------
void Dlg_PosoManager::pushButtonDefPosoColler_clicked()
{CPosologie poso             = m_CPosologieMem;
 poso.m_MEDICA_POSO_CIP      = m_CPosologie.m_MEDICA_POSO_CIP;
 poso.m_MEDICA_POSO_PK       = m_CPosologie.m_MEDICA_POSO_PK;
 poso.m_MEDICA_POSO_EXTRA_PK = m_CPosologie.m_MEDICA_POSO_EXTRA_PK;
 CPosologieToInterface(poso);
}


//------------------------------------------- pushButton_A_clicked ----------------------------------------------------------------------------------------
void Dlg_PosoManager::pushButton_A_clicked()
{if (pushButton_A->state () == QButton::On )  slider_VarPoso->setEnabled(FALSE);
 else                                         slider_VarPoso->setEnabled(TRUE);
 DisplayResult();
}
