/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


/*! \class FormRubTerrain
 * Gestion des param\303\250tres de terrain :
 * - Ant\303\251c\303\251dents
 * - Variables
 * - Traitement en cours
*/

//=============================================== IMPLEMENTTION =========================================================
#include <math.h>
#include <qdragobject.h>
#include "C_Var.h"
#include "C_Atcd.h"
#include "CMDI_Prescription.h"

//--------------------------------- Init ----------------------------------------------------
void FormRubTerrain::init()
{
 pushButtonNewATCD->setIconSet( Theme::getIconListAdd() );
 pushButtonNewTTT->setIconSet( Theme::getIconListAdd() );
 pushButtonNewVAR->setIconSet( Theme::getIconListAdd() );
 pushButtonDellAllTTT->setIconSet( Theme::getIconListDelete() );
 pushButtonDellAllATCD->setIconSet( Theme::getIconListDelete() );
 pushButtonDellAllVAR->setIconSet( Theme::getIconListDelete() );

 m_IsModifiable            = 0;
 m_TypeATCD_Selectionne    = "";
 m_pAtcd_Code              = 0;
 m_pAtcd_Element_Selected  = 0;
 m_IsModified              = 0;
 m_IsAllDroitsOn           = FALSE;
 m_LAP_Ordo                = "";
 m_StateModifBeforeLAP     = 0;
 
 if (G_pCApp->m_pAtcd_Code)
    {m_pAtcd_Code = G_pCApp->m_pAtcd_Code;
     connect(m_pAtcd_Code, SIGNAL( ATCD_Changed() ), this, SLOT( ATCD_HaveChanged() ));
    }
 //connect(textLabel_NbRub, SIGNAL( pressed() ), this, SLOT( On_buttonDragClicked() ));
  m_pC_ListViewATCDManager  = new C_ListViewATCDManager(this, "FormRubTerrainGestListViewATCD",listView_ATCD, G_pCApp->m_pAtcd_Code, &m_IsModified, &m_IsModifiable, &m_TypeATCD_Selectionne);

    // signals and slots connections
    connect( pushButtonSave,          SIGNAL( clicked() ), this, SLOT( pushButtonSave_clicked() ) );
    connect( listView_ATCD,           SIGNAL( contextMenuRequested(QListViewItem*,const QPoint&,int) ), this,  SLOT( listView_ATCD_contextMenuRequested(QListViewItem*,const QPoint&,int) ) );
    connect( listView_ATCD,           SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT(  listView_ATCD_doubleClicked(QListViewItem*) ) );
    connect( listView_TTT,            SIGNAL( contextMenuRequested(QListViewItem*,const QPoint&,int) ), this,  SLOT( listView_TTT_contextMenuRequested(QListViewItem*,const QPoint&,int) ) );
    connect( listView_TTT,            SIGNAL( doubleClicked(QListViewItem*) ), this, SLOT(  listView_TTT_doubleClicked(QListViewItem*) ) );
    connect( checkBoxGrossesse,       SIGNAL( clicked() ), this, SLOT( checkBoxGrossesse_clicked() ) );
    connect( checkBoxAllaitement,     SIGNAL( clicked() ), this, SLOT( checkBoxAllaitement_clicked() ) );
    connect( pushButtonNewVAR,        SIGNAL( clicked() ), this, SLOT( pushButtonNewVAR_clicked() ) );
    connect( pushButtonNew,           SIGNAL( clicked() ), this, SLOT( pushButtonNew_clicked() ) );
    connect( pushButtonDelete,        SIGNAL( clicked() ), this, SLOT( pushButtonDelete_clicked() ) );
    connect( pushButtonDellAllVAR,    SIGNAL( clicked() ), this, SLOT( pushButtonDellAllVAR_clicked() ) );
    connect( pushButtonNewATCD,       SIGNAL( clicked() ), this, SLOT( pushButtonNewATCD_clicked() ) );
    connect( pushButtonDellAllATCD,   SIGNAL( clicked() ), this, SLOT( pushButtonDellAllATCD_clicked() ) );
    connect( pushButtonNewTTT,        SIGNAL( clicked() ), this, SLOT( pushButtonNewTTT_clicked() ) );
    connect( pushButtonDellAllTTT,    SIGNAL( clicked() ), this, SLOT( pushButtonDellAllTTT_clicked() ) );
    connect( pushButtonRubDateChange, SIGNAL( clicked() ), this, SLOT( pushButtonRubDateChange_clicked() ) );
    connect( comboBox_RubName,        SIGNAL( activated(int) ), this, SLOT( comboBox_RubName_activated(int) ) );
    connect( comboBox_RubName,        SIGNAL( highlighted(int) ), this, SLOT( comboBox_RubName_highlighted(int) ) );
    connect( QPushButton_DDR,         SIGNAL( clicked() ), this, SLOT( QPushButton_DDR_clicked() ) );

 #ifdef Q_OS_MACX
    pushButtonSave->setFlat (TRUE);
    pushButtonNewVAR->setFlat (TRUE);
    pushButtonNew->setFlat (TRUE);
    pushButtonDelete->setFlat (TRUE);
    pushButtonDellAllVAR->setFlat (TRUE);
    pushButtonNewATCD->setFlat (TRUE);
    pushButtonDellAllATCD->setFlat (TRUE);
    pushButtonNewTTT->setFlat (TRUE);
    pushButtonDellAllTTT->setFlat (TRUE);
    pushButtonRubDateChange->setFlat (TRUE);
#endif


 textLabelDDR->hide();
 QPushButton_DDR->hide(); //qDebug("init() hide");
 listView_ATCD->setSelectionMode ( QListView::Extended );
 listView_TTT->setSelectionMode ( QListView::Extended );

 pushButtonNew->setIconSet            (Theme::getIconNewDoc());
 pushButtonSave->setIconSet           (Theme::getIconSave());
 //pushButtonPrint->setIconSet          (Theme::getIconPrint);

 //............. creer le Layout des Variables ...........................
 m_pLayout_VAR      = new QVBoxLayout( 0, 0, 7, "layout9_2");

 //............... Y placer une ScrollView ................................
 m_pQScrollView  = new QScrollView(this);
 //............... avec un  CBigShrinker qui recevra les widgets ..........
 m_pCBigShrinker = new CBigShrinker(0,1280,4000);//(vp->viewport());
 m_pQScrollView->addChild(m_pCBigShrinker);
 m_pLayout_VAR->addWidget(m_pQScrollView );
 //................ Ajouter ce layout aux autres .........................
 FormRubTerrainLayout->addLayout( m_pLayout_VAR, 5, 0 ); // il faut que l'ensemble du dialogue soit en layout pour avoir: FormRubTerrainLayout
 if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"ata") ) m_IsAllDroitsOn = TRUE;
}

//--------------------------------- ButtonSaveDisplay ------------------------------------------------------
void  FormRubTerrain::ButtonSaveDisplay(const QString &/*droits*/)
{if ( m_IsModifiable)
    { pushButtonSave->show();
    }
 else
    { pushButtonSave->hide();
    }
}

//--------------------------------- sexeAndParturienteDisplay ------------------------------------------------------
void  FormRubTerrain::sexeAndParturienteDisplay()
{  if (m_pAtcd_Code->GetSexe() == 'F')
    {  if (m_pAtcd_Code->m_Grossesse)
          { checkBoxGrossesse->setChecked(TRUE);
            QPushButton_DDR->setText (m_pAtcd_Code->m_DDR.toString("dd-MM-yyyy"));
            textLabelDDR->show();
            QPushButton_DDR->show(); //qDebug("sexeAndParturienteDisplay() show");
          }
       else
          { checkBoxGrossesse->setChecked(FALSE);
            QPushButton_DDR->setText (tr("Choisir"));
            textLabelDDR->hide();
            QPushButton_DDR->hide(); //qDebug("sexeAndParturienteDisplay() hide");
          }
       if (m_pAtcd_Code->m_Allaitement)      checkBoxAllaitement->setChecked(TRUE);
       else                                  checkBoxAllaitement->setChecked(FALSE);
    }
}
//--------------------------------- setModifiedState --------------------------------------------------
void  FormRubTerrain::setModifiedState(int state)
{m_IsModified = state;
}

//--------------------------------- SetInterfaceOnDroits -------------------------------------------------
void  FormRubTerrain::SetInterfaceOnDroits(const QString &droits)
{if (m_IsAllDroitsOn)
    {pushButtonDelete->show();
     pushButtonNew->show();
     pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate() );
     Set_ATCD_Visible(1);
     Set_TTT_Visible(1);
     Set_VAR_Visible(1);
     ButtonSaveDisplay(droits);
     return;
    }
 if (G_pCApp->IsThisDroitExist(droits,"atc"))      Set_ATCD_Visible(1);        //atv atc ttc  tvv tvc etc
 else                                              Set_ATCD_Visible(0);
 if (G_pCApp->IsThisDroitExist(droits,"ttv"))      Set_TTT_Visible(1);
 else                                              Set_TTT_Visible(0);
 if (G_pCApp->IsThisDroitExist(droits,"tvv"))     { Set_VAR_Visible(1); }     // qDebug(QString ("is visible   ")+droits);
 else                                             { Set_VAR_Visible(0); }     // qDebug(QString ("no visible   ")+droits);
 if (G_pCApp->IsThisDroitExist(droits,"etc"))
    {pushButtonDelete->show();
     pushButtonNew->show();
     pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate() );
    }
 else
    {pushButtonDelete->hide();
     pushButtonNew->hide();
     pushButtonRubDateChange->setIconSet(Theme::getIconCadenas() );
    }
 ButtonSaveDisplay(droits);
}
//--------------------------------- Set_ATCD_Visible ------------------------------------------------------
void  FormRubTerrain::Set_ATCD_Visible(int state)
{if (state==1)
    {listView_ATCD->show();
     textLabel_ATCD->show();
     pushButtonDellAllATCD->show();
     pushButtonNewATCD->show();
     if (m_pAtcd_Code->GetSexe() == 'F')
        { checkBoxGrossesse->show();
          checkBoxAllaitement->show();
          textLabelDDR->show();
          QPushButton_DDR->show(); //qDebug("Set_ATCD_Visible(state==1) show");
        }
     else
       { checkBoxGrossesse->hide();
         checkBoxAllaitement->hide();
         textLabelDDR->hide();
         QPushButton_DDR->hide();  //qDebug("Set_ATCD_Visible(state!=1) hide");
       }
    }
 else
    {listView_ATCD->hide();
     textLabel_ATCD->hide();
     pushButtonDellAllATCD->hide();
     pushButtonNewATCD->hide();
     checkBoxGrossesse->hide();
     checkBoxAllaitement->hide();
     textLabelDDR->hide();
     QPushButton_DDR->hide(); //qDebug("Set_ATCD_Visible() !m_IsFeminin   hide");
    }
}
//--------------------------------- Set_TTT_Visible ------------------------------------------------------
void  FormRubTerrain::Set_TTT_Visible(int state)
{if (state==1)
    {listView_TTT->show();
     textLabel_TTT_Encours->show();
     pushButtonDellAllTTT->show();
     pushButtonNewTTT->show();
    }
 else
    {listView_TTT->hide();
     textLabel_TTT_Encours->hide();
     pushButtonDellAllTTT->hide();
     pushButtonNewTTT->hide();
    }
}
//--------------------------------- Set_VAR_Visible ------------------------------------------------------
void  FormRubTerrain::Set_VAR_Visible(int state)
{if (state==1)
    {textLabel_VAR->show();
     pushButtonDellAllVAR->show();
     pushButtonNewVAR->show();
     m_pQScrollView->show();
    }
 else
    {textLabel_VAR->hide();
     pushButtonDellAllVAR->hide();
     pushButtonNewVAR->hide();
     m_pQScrollView->hide();
    }
}

//--------------------------------- SetModifiableState ------------------------------------------------------
void  FormRubTerrain::SetModifiableState(int state)
{m_IsModifiable = state;
 SetInterfaceOnDroits(G_pCApp->m_Droits);
}
//--------------------------------- cacher--------------------------------------------------
void FormRubTerrain::cacher()
{    comboBox_RubName->hide();
     pushButtonSave->hide();
     pushButtonDelete->hide();
     pushButtonNew->hide();
     textLabel_ATCD->hide();
     pushButtonNewATCD->hide();
     pushButtonDellAllATCD->hide();
     listView_ATCD->hide();
     checkBoxGrossesse->hide();
     checkBoxAllaitement->hide();
     QPushButton_DDR->hide(); //qDebug("cacher() hide");
     textLabelDDR->hide();
     pushButtonNewTTT->hide();
     pushButtonDellAllTTT->hide();
     textLabel_TTT_Encours->hide();
     listView_TTT->hide();
     pushButtonNewVAR->hide();
     pushButtonDellAllVAR->hide();
     textLabel_VAR->hide();
     m_pQScrollView->hide();
     pushButtonRubDateChange->hide();
}

//--------------------------------- montrer --------------------------------------------------
void FormRubTerrain::montrer(int nb)
{    if (m_IsModifiable)
        {pushButtonRubDateChange->show();
         pushButtonSave->show();
         pushButtonDelete->show();
         pushButtonNew->show();
         pushButtonNewATCD->show();
         pushButtonDellAllATCD->show();
         pushButtonNewTTT->show();
         pushButtonDellAllTTT->show();
         pushButtonNewVAR->show();
         pushButtonDellAllVAR->show();

        }
     comboBox_RubName->show();
     textLabel_ATCD->show();
     listView_ATCD->show();
     checkBoxGrossesse->show();
     checkBoxAllaitement->show();
     QPushButton_DDR->show(); //qDebug("montrer() show");
     textLabelDDR->show();
     textLabel_TTT_Encours->show();
     listView_TTT->show();
     textLabel_VAR->show();
     show();
    // Il faut se synchroniser avec Dock_Menu ICI
     if (m_pAtcd_Code->getPkDocTerrain()=="")
     { comboBox_RubName->setCurrentItem (nb-1);            // se mettre sur derni\303\250re prescription
     }
     else
     { // Il faut trouver le pkdoc a afficher et le s\303\251lectionner
       RUBREC_LIST::iterator it;
       QString tmp = "";
       CPrtQListBoxItem* pCPrt = 0;
       for (int i=0; i < (int) comboBox_RubName->listBox()->count();++i)
       { pCPrt = (CPrtQListBoxItem*)  comboBox_RubName->listBox()->item ( i );
         if (pCPrt!=0) tmp = pCPrt->GetUser();  // Bin ouais tu sauves la pkdoc dans user faut le savoir ?!
         else          tmp = "";
         //it = m_pRubList->at( tmp.toInt() ) ;
         if ( tmp == m_pAtcd_Code->getPkDocTerrain() )
            { comboBox_RubName->setCurrentItem(i);
              m_Selected_Id_RubList = pCPrt->GetPk().toInt();
              break;
            }
       }
     }
     comboBox_RubName_activated( comboBox_RubName->currentItem ());
}

//--------------------------------- SetData--------------------------------------------------
int FormRubTerrain::SetData()
{//............................... si il n'y a pas de terrain mettre celui par defaut ................
 long nb = initComboRubName();
 if (nb <= 0)
    {G_pCApp->m_pAtcd_Code->clear();
     cacher();
    }
 else         montrer(nb);
 return nb;
}

//------------------------------------------ nbDate ----------------------------------------------------------
int FormRubTerrain::nbDate()
{return comboBox_RubName->count();
}

//--------------------------------- GetSurface---------------------------------------------------
//   Surface corporelle (Mosteller) = (Taille(cm)*poids(kg)/3600)0.5
//   Surface corporelle(Dubois) = 0.007184 x Taille(cm)0.725 x Poids(kg)0.425
//   Surface corporelle(Gehan et George) = 0.0235 x Taille(cm)0.42246 x Poids(kg)0.51456
//   lorentz homme  poids = taille - 100 - (taille - 150)/4
//   lorentz femme  poids = taille - 100 - (taille - 150)/2
QString FormRubTerrain::GetSurface()
{QString surface("");

 QString poids  = GetPoids();
 if (poids.length()==0)  return surface;
 QString taille = GetTaille(atof(poids));
 if (taille.length()==0) return surface;

 surface = QString::number(sqrt(atof(taille) * atof(poids) / 3600));   //(Mosteller)
 return surface;
}
//--------------------------------- PoidsToTaille--------------------------------------------------
//   lorentz homme  poids = taille - 100 - (taille - 150)/4
//   lorentz femme  poids = taille - 100 - (taille - 150)/2
//   lorentz invers\303\251e    taille = ( (Poids + 100) x coef - 150) / ( coef -1)   // coef 4 pour homme 2 pour la femme
float FormRubTerrain::PoidsToTaille(float poids, QString sexe)
{float           coef = 2;
 if (sexe=="M")  coef = 4;
 return (  ((poids+100)*coef - 150) / (coef-1));
}

//--------------------------------- GetTaille----------------------------------------------------------
QString FormRubTerrain::GetTaille(float poids)
{QString taille = GetTaille();
 if (taille.length()==0) // si taille non renseign\303\251e => en calculer une id\303\251ale par rapport au poids ...
    {taille = QString::number(PoidsToTaille(poids, m_pAtcd_Code->GetSexe()));
    }
 return taille;
}

//--------------------------------- GetTaille------------------------------------------------------
QString FormRubTerrain::GetTaille()
{return VAR_GetLastValeurVarTable(tr("Poids"), tr("Taille"));
}

//--------------------------------- GetPoids--------------------------------------------------
QString FormRubTerrain::GetPoids()
{return VAR_GetLastValeurVarTable(tr("Poids"), tr("Poids"));
}

//--------------------------------- pushButtonNew_clicked ---------------------------------------------------
void FormRubTerrain::pushButtonNew_clicked()
{if (m_IsModifiable==0) return;
 QPopupMenu *pQPopupMenu = new ThemePopup(this, "MyPopupMenu" );

 pQPopupMenu->insertItem( tr("Avec copie de &tout"),                                  this, SLOT( NEW_MenuActionWithCopy()),        CTRL+Key_M );
 pQPopupMenu->insertItem( tr("Avec copie de tout &sans les valeurs des variables"),   this, SLOT( NEW_MenuActionWithoutVarValue()), CTRL+Key_R );
 pQPopupMenu->insertItem( tr("Avec copie &vide"),                                     this, SLOT( NEW_MenuActionWithEmptyCopy()),   CTRL+Key_M );
 pQPopupMenu->insertItem( tr("&Avec copie vierge (sans les variables)"),              this, SLOT( NEW_MenuActionWithBlankCopy()),   CTRL+Key_M );


 pQPopupMenu->exec(QCursor::pos());
 delete pQPopupMenu;
}

//--------------------------------- NEW_MenuActionWithCopy -----------------------------------------------
void FormRubTerrain::NEW_MenuActionWithCopy()
{if (m_IsModifiable==0) return;
 QString  stringDST = "";
 FormToData(stringDST);                                                   // recuperer les donn\303\251es actuelles
 NEW_AddNewTerrain(stringDST);
}

//--------------------------------- NEW_MenuActionWithoutVarValue -----------------------------------------
void FormRubTerrain::NEW_MenuActionWithoutVarValue()
{if (m_IsModifiable==0) return;
 QString  stringDST = "";
 FormToData(stringDST);                                                   // recuperer les donn\303\251es actuelles
 NEW_AddNewTerrain(stringDST);
 ClearAllTableValues();                                                // effacer le contenu des tables
}

//--------------------------------- NEW_MenuActionWithEmptyCopy -------------------------------------------
void FormRubTerrain::NEW_MenuActionWithEmptyCopy()
{if (m_IsModifiable==0) return;
 QString  stringDST = "";
 VAR_GetModeleList(stringDST, 0);                                      // recuperer juste les mod\303\250les des tables actuelles
 NEW_AddNewTerrain(stringDST);
}

//--------------------------------- NEW_MenuActionWithBlankCopy ---------------------------------------------
void FormRubTerrain::NEW_MenuActionWithBlankCopy()
{if (m_IsModifiable==0) return;
 QString  stringDST = " ";
 NEW_AddNewTerrain(stringDST);
}

//--------------------------------- NEW_AddNewTerrain --------------------------------------------------
void FormRubTerrain::NEW_AddNewTerrain(QString &data)
{if (m_IsModifiable==0) return;
 QDateTime      qdt =   QDateTime::currentDateTime();
 QString       date = qdt.toString ("yyyy-MM-ddThh:mm:ss");            // date est celle courante
 IfModified_SaveInRubList();                                           // sauvegarder les donn\303\251es actuelles dans la liste

 m_pRubList->append(CRubRecord("0",                                    // PrimKey=0 pour signaler que pas sur disque
                               "Terrain",                              // libelle
                               QString::number(TYP_TERRAIN),           // type
                               "Default SubType",                      // sous type
                               date,                                   // date l'enregistrement
                               G_pCApp->m_User,                        // utilisateur
                               G_pCApp->m_SignUser,                    // utilisateur signataire
                               data,                                   // donn\303\251es texte
                               data.length()                           // longueurdes donn\303\251es
                              )
                   );
 long nb = initComboRubName();   // reactualiser le combo avec nouvelle liste
 if (nb > 0)
    {comboBox_RubName->setCurrentItem (nb-1);            // se mettre sur derni\303\250re prescription
     //comboBox_RubName_activated( comboBox_RubName->currentItem ());
    }
 m_IsModified = 1;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}


//--------------------------------- comboBox_RubName_highlighted --------------------------------------------------
void FormRubTerrain::comboBox_RubName_highlighted( int item)
{comboBox_RubName_activated(item);
}

//--------------------------------- comboBox_RubName_activated--------------------------------------------------
/*! \brief Slot activ\303\251 lorsque l'utilisateur modifie le terrain a voir.
*/
void FormRubTerrain::comboBox_RubName_activated( int item)
{
 QString               stringDST;
 QString               stringStruct;

 m_Selected_Id_RubList = -1;
 //........................... si rubrique qui etait avant celle a afficher, a ete modifi\303\251e ...................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if (m_IsModifiable && m_IsModified && m_LastRub != -1)
    {FormToData(stringDST);
     m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
    }

  RUBREC_LIST::iterator it;
  CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( item );
  if (pCPrtQListBoxItem == 0) return;
  m_Selected_Id_RubList = pCPrtQListBoxItem->GetPk().toInt();
  it                    = m_pRubList->at(m_Selected_Id_RubList);
  DataToForm();

 //..................... signaler le changement de la rubrique ............................
 // emit Sign_RubChanged(it);
 //........................ r\303\251gler droits utilisateurs .................................................
 QString user              = (*it).m_User;
 QString doc_sign_user     = (*it).m_SignUser;

 if (G_pCApp->m_User != doc_sign_user)  // si l'utilisateur actuel n'est pas le m\303\252me que le signataire du document v\303\251rifier les droits
    {QString friend_droits     = G_pCApp->m_pCMoteurBase->PermsUserIsThisUserFriendFromName(  doc_sign_user , G_pCApp->m_User);
     SetInterfaceOnDroits(friend_droits);
    }
 else
    {SetInterfaceOnDroits(G_pCApp->m_Droits);
    }

 //..................... si ordo a effacer afficher bouton effacement ...............................
 if ((*it).m_State == RUB_IS_TO_DELETE) {pushButtonDelete->setIconSet (Theme::getIconIsDeleted() ); pushButtonDelete->setDown(TRUE);}
 else                                   {pushButtonDelete->setIconSet (Theme::getIconDelete() );    pushButtonDelete->setDown(FALSE);}

 // Si Dock_Menu existe lui ordonner un changement de terrain
 if (G_pCApp->m_pDrTux != 0 && G_pCApp->m_pDrTux->m_pDock_Menu != 0)
 { //if ((*it).m_PrimKey != G_pCApp->m_pDrTux->m_pForm_Menu->m_PkDocTerrain)
    { // Resynchronise dock_Menu
      G_pCApp->m_pDrTux->m_pForm_Menu->getTerrain((*it).m_PrimKey);
    }
   //G_pCApp->m_pDrTux->m_pForm_Menu->setSexeAndParturienteDisplay();
 }
 //sexeAndParturienteDisplay();qDebug ("comboBox_RubName_activated() sexeAndParturienteDisplay()");
}

//--------------------------------- ATCD_HaveChanged --------------------------------------------------
/*! \brief Slot connect\303\251 au Signal de Atcd_Code :: ATCD_Changed. Met a jour l'affichage du boutton de sauvegarde et met a jour le terrain.
 * \sa ATCD_setInForm().
*/
void FormRubTerrain::ATCD_HaveChanged()
{ if (m_IsModifiable==0) return;

  ButtonSaveDisplay(G_pCApp->m_Droits);
  ATCD_setInForm();
  //m_IsModified |= 2;
  //................ mettre a jour les datas de la rubrique ...................
 QString stringDST;
 FormToData(stringDST);
 if (m_LastRub != -1)
    {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
    }
}

//--------------------------------- ATCD_setInForm --------------------------------------------------
/*! \brief Met a jour le listview des ATCD avec les ATCD de Atcd_Code.*
*/
void FormRubTerrain::ATCD_setInForm()
{ if (!m_pAtcd_Code) return;
  // Met a jour la listview des ATCD
   //................. positionner l'interface sur les valeurs ....................
 if (m_pAtcd_Code->GetSexe() == 'F')
    {checkBoxAllaitement->show();
     checkBoxGrossesse->show();
     QPushButton_DDR->show();   //qDebug("DataToForm() show");
    }

  m_pAtcd_Code->atcd_To_ListView(*listView_ATCD);
  // Met a jour les ATCD obst\303\251tricaux
  if (m_pAtcd_Code->m_Grossesse)
   { checkBoxGrossesse->setChecked(TRUE);
     QPushButton_DDR->setText (m_pAtcd_Code->m_DDR.toString("dd-MM-yyyy"));
     textLabelDDR->show();
     QPushButton_DDR->show();  //qDebug("ATCD_setInForm() show");
   }
  else
   { checkBoxGrossesse->setChecked(FALSE);
     QPushButton_DDR->setText ("Choisir");
     textLabelDDR->hide();
     QPushButton_DDR->hide();  //qDebug("ATCD_setInForm() hide");
   }
  if (m_pAtcd_Code->m_Allaitement)      checkBoxAllaitement->setChecked(TRUE);
  else                                  checkBoxAllaitement->setChecked(FALSE);
}
/* OLD
//--------------------------------- FormToData --------------------------------------------------
void FormRubTerrain::FormToData(QString &strDST)
{//.................... contenu des ATCD ............................................................
 if (!m_pAtcd_Code) return;
 strDST = "";
 strDST = m_pAtcd_Code->toOldIni();

 //.................... contenu Variables ...............................................................
 //QDateTime    qdt       =   QDateTime::currentDateTime();
 //QString     date       =   qdt.toString ("yyyy-MM-ddThh:mm:ss");
 VAR_GetModeleList(strDST, 1);

 //.................... Etat Renouvelable/Intercurent ...................................................
 {strDST += tr("[Propri\303\251t\303\251 Ordonnance]\r\n");
  QListViewItemIterator it( listView_TTT );
  while ( it.current() )
        { strDST += "    " + it.current()->text (0) + " = " + it.current()->text (1) + " , " + it.current()->text (2) + "\r\n";
         ++it;
        }
 }
 //.................... contenu des TTT .................................................................
 // IMPORTANT: doit toujours etre a la fin
 strDST += tr("[Traitement]\r\n");
 strDST.append (G_pCApp->m_pCMedicaBase->Medica_PosologieListToXMLOrdoStruct( m_OrdoList, GetPoids(), GetTaille() ));
// CGestIni::Param_UpdateToDisk( "C:/terrain.txt", strDST);
}
*/
//--------------------------------- FormToData --------------------------------------------------
void FormRubTerrain::FormToData(QString &strDST)
{//.................... contenu des ATCD ............................................................
 if (!m_pAtcd_Code) return;
 strDST = "";
 strDST = m_pAtcd_Code->toOldIni();

 //.................... contenu Variables ...............................................................
 //QDateTime    qdt       =   QDateTime::currentDateTime();
 //QString     date       =   qdt.toString ("yyyy-MM-ddThh:mm:ss");
 VAR_GetModeleList(strDST, 1);

 //.................... Etat Renouvelable/Intercurent ...................................................
 {strDST += tr("[Propri\303\251t\303\251 Ordonnance]\r\n");
  QListViewItemIterator it( listView_TTT );
  while ( it.current() )
        { strDST += "    " + it.current()->text (0) + " = " + it.current()->text (1) + " , " + it.current()->text (2) + "\r\n";
         ++it;
        }
 }
 //.................... contenu des TTT .................................................................
 // IMPORTANT: doit toujours etre a la fin
 strDST += tr("[Traitement]\r\n");
 strDST.append (G_pCApp->m_pCMedicaBase->Medica_PosologieListToXMLOrdoStruct( m_OrdoList, GetPoids(), GetTaille(), m_LAP_Ordo));
// CGestIni::Param_UpdateToDisk( "C:/terrain.txt", strDST);
}

//--------------------------------- clearForm--------------------------------------------------
/*! \brief efface l'interface.
 *  \todo Utiliser Atcd_Code, Var_Code
*/
void FormRubTerrain::clearForm()   // texte de configuration avec zero de fin
{//...........effacement de l'interface .....................
 listView_ATCD->clear();
 listView_TTT->clear();
 m_OrdoList.clear();
 if (m_pCBigShrinker != 0) delete m_pCBigShrinker;  m_pCBigShrinker        = new CBigShrinker(0,1280,4000);
 m_pQScrollView->addChild(m_pCBigShrinker);
 checkBoxAllaitement->setChecked(FALSE);
 checkBoxGrossesse->setChecked(FALSE);
 textLabelDDR->hide();
 QPushButton_DDR->hide();
 checkBoxAllaitement->hide();
 checkBoxGrossesse->hide();
 m_ptrListQTableVar.clear();
}

//--------------------------------- DataToForm--------------------------------------------------
/*! \brief R\303\251cup\303\250re les donn\303\251es du terrain dans le blob de RubList et affiche les donn\303\251es.
 *  \todo Utiliser Atcd_Code, Var_Code
*/
void FormRubTerrain::DataToForm()   // texte de configuration avec zero de fin
{
  if (m_Selected_Id_RubList == -1) return; // aucun item s\303\251lectionn\303\251
  RUBREC_LIST::iterator it = m_pRubList->at(m_Selected_Id_RubList);;
  QByteArray  data_rubrique;
  const char* data;
  long        len         = 0;
  QString     stringDST   = "";
  m_LAP_Ordo              = "";
  //.............................. effacer la vue .......................................................
  clearForm();
  //........................... recuperer les donnees de la base de donnees..............................
  m_pCMoteurBase->GetDataFromRubList(data_rubrique, it);
  m_LastRub  = m_Selected_Id_RubList;                   // c'est maintenant la derni\303\250re affich\303\251e
  data       = data_rubrique.data();
  len        = data_rubrique.size();
  if (data && len>0)
     {
      if (CGestIni::IsUtf8( data, len ))
         {stringDST = QString::fromUtf8 ( data );
          data = (char*)(const char*) stringDST;
         }
     }
  //........................... postionner les ATCD sur les donnees..............................
  if (!m_pAtcd_Code) // creer si pas deja fait (on ne sait jamais)
     { // Cr\303\251er et D\303\251finir le terrain de l'objet
       G_pCApp->m_pAtcd_Code = new Atcd_Code(m_pCMoteurBase);
       G_pCApp->m_pAtcd_Code->setTerrain(data, m_pCMoteurBase);
       m_pAtcd_Code          = G_pCApp->m_pAtcd_Code;
       connect(m_pAtcd_Code, SIGNAL( ATCD_Changed() ), this, SLOT( ATCD_HaveChanged() ));
     }
  else if (data)
     { // V\303\251rifie que les atcd sont ceux du document s\303\251lectionn\303\251
       // sinon informe Atcd_Code du chgt de terrain
       if (m_pAtcd_Code->getPkDocTerrain() != (*it).m_PrimKey)
          { //m_pAtcd_Code->setTerrain( it, m_pCMoteurBase );                   // ici la listview du terrain est initialis\303\251e
            m_pAtcd_Code->setTerrain(data, m_pCMoteurBase);
          }
    }
  //........................... postionner l'interface sur les antecedents..............................
  ATCD_setInForm();
 if (data==0) return;
 //............................. positionner le reste (variables et traitement courant ) ..............
 QString val1, val2, val3, val4, val5, val6, val7, val8, val9, val10;
 QString       strTab[10];
 QString      section   = "";
 QString     var_name   = "";
 const char      *txt   = data;
 char             *pt   = (char*) txt;
 char     *prop_section = 0;       // permet de noter le pointeur sur la section propriete renouvelable/intercurent des produits


 while((pt=CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
    {//.................................. VAR ........................................................
     if (section.left(3)=="VAR" )                  // a ce stade pt pointe au debut de la nouvelle ligne
        {QString name_section = section.mid(4);
         // [VAR Poids]     1      2       3       4    5   6        7                                      8
         //    Mod\303\250le  = Date, Valeur, 31536000, 100, 50, Kg(s), Attention a la surcharge pond\303\251rale, Attention a l'amaigrissement
         //    2004-03-15T17:14:48 =  68
         //                                   val1                             ,    val2               val3      val4 val5 val6  val7       val8
         QFont fnt(font());
         fnt.setPointSize(9);
         QStringList   tableLabels;    // titre des colonnes de la table
         CMyQTable    *pQTable = 0;
         int              row  = 0;
         QLabel    *pTextLabel = 0;
         int        n_var      = 0;
         QString    title      = "";
         while (*pt && *pt != '[')
               {var_name = "";
                pt   = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &strTab[0], &strTab[1], &strTab[2], &strTab[3], &strTab[4], &strTab[5], &strTab[6], &strTab[7], &strTab[8]);
                //...................... DEFINITION DE VARIABLE .....................................
                //                       contruire le dialogue des variables
                if (var_name==tr("Mod\303\250le"))
                   {//...................... premier mod\303\250le: creation listview .....................
                    if (pQTable==0)
                       {int    w  = 1278;

                        //............... titre de la variable ......................................
                        pTextLabel = new QLabel( m_pCBigShrinker, name_section);
                        pTextLabel->setFrameShape( QLabel::MenuBarPanel );
                        pTextLabel->setAlignment( int( QLabel::AlignLeft ) );
                        title = name_section;
                        if (strTab[5] != "") title += tr("  Unit\303\251: ") + strTab[5];
                        pTextLabel->resize(w, 20 );
                        pTextLabel->setFont(fnt);

                        //............... QTable des valeurs .......................................
                        pQTable = new CMyQTable ( 0, 2, m_pCBigShrinker, name_section );
                        pQTable->m_Title = pTextLabel;
                        pQTable->setFont(fnt);

                        tableLabels.append( tr("Date & Heure") );
                        tableLabels.append( strTab[1] );
                        //.................. stocker modele de la table ..............................................
                        pQTable->AddModele (var_name + " = " + strTab[0] + ", " + strTab[1] + ", " + strTab[2] + ", "
                                                             + strTab[3] + ", " + strTab[4] + ", " + strTab[5] + ", "
                                                             + strTab[6] + ", " + strTab[7] );
                        n_var++;
                       }
                    //...................... mod\303\250les suivants: ajout d'une variable ...................
                    else
                       {//.................. stocker modele de la table ..............................................
                        pQTable->AddModele (var_name + " = " + strTab[0] + ", " + strTab[1] + ", " + strTab[2] + ", "
                                                             + strTab[3] + ", " + strTab[4] + ", " + strTab[5] + ", "
                                                             + strTab[6] + ", " + strTab[7] );
                        pQTable->insertColumns (1);
                        if (strTab[5] != "") title += ",  " + strTab[5] ;
                        tableLabels.append( strTab[1] );
                        n_var++;
                       }
                   } // endif (var_name=="Mod\303\250le")
                 //................ VALEURS DATEES DE VARIABLE ........................................
                 else if (var_name!="" && pQTable)
                   {pQTable->insertRows (row);
                    pQTable->setText (row, 0, var_name.left(10) + "  " + var_name.mid(11));
                    for (int i=1; i<=n_var;i++)
                        {pQTable->setText (row, i, strTab[i-1]);
                        }
                    row++;
                   }
               } // end while (*pt && *pt != '[')
         if (pQTable)
            {CMyButton* qw = new CMyButton( m_pCBigShrinker, tr("Outils"), pQTable);
             if (qw)
                {connect( qw, SIGNAL( Sign_ButtonClickedPtr(const char*, void* ) ),this, SLOT( Slot_TableButtonClicked(const char*, void*) ) );
                 qw->resize(90, 22 );
                 qw->setFont(fnt);
                 pQTable->m_ButtonTools  = qw;
                }
             qw = new CMyButton( m_pCBigShrinker, tr("&Nouveau"), pQTable);
             if (qw)
                {connect( qw, SIGNAL( Sign_ButtonClickedPtr(const char*, void* ) ),this, SLOT( Slot_TableNewDateClicked(const char*, void*) ) );
                 qw->resize(22, 22 );
                 qw->setFont(fnt);
                 qw->setPixmap ( Theme::getIcon( "22x22/today.png") );
                 pQTable->m_ButtonNew = qw;
                }

             pQTable->setColumnLabels (tableLabels);
             pQTable->adjustColumn (0);
             for (int i=1; i<=n_var;i++) pQTable->adjustColumn (i);
             QRect rectTable = pQTable->cellGeometry (0, n_var );
             pQTable->resize( QSize(rectTable.right ()+50, 120) );
             pQTable->ensureCellVisible (row, 1 );
             pTextLabel->setText( title);
             m_ptrListQTableVar.append( pQTable );
             //.................. connecter le changement de valeur ............
             connect(pQTable, SIGNAL( Sign_ValueChanged        (int,  int, CMyQTable*)),
                     this,    SLOT  ( Slot_OnValueTableChanged (int,  int, CMyQTable*))
                    );
             //................. faut deriver QTable ...........................
             pQTable->show();
             pTextLabel->show();
            } // endif (pQTable)
         section="";
        } // endif (section.left(3)=="VAR" )

     if (section==tr("Propri\303\251t\303\251 Ordonnance"))                  // a ce stade pt pointe au debut de la nouvelle ligne
        {prop_section = pt;                                // noter section des proprietes (a traiter apres liste med)
        }
     //.................................. TTT ........................................................
     // IMPORTANT: cette section doit tjrs etre en dernier dans le fichier pour des raisons de fin !!!
     if (section==tr("Traitement"))                  // a ce stade pt pointe au debut de la nouvelle ligne
        {TTT_PutBlobPrescriptionInTerrain(pt);
         break;
        }
    } // end while((pt=CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
 //................... mettre a jour propriete renouvelable/intercurent des produits ....................
 //                    ne peut se faire qu'apres avoir construit la liste des medicaments
 if (prop_section && listView_TTT->childCount()>0)
    {pt        =   prop_section;
     int nbMax =   listView_TTT->childCount();
     int     i = 0;
     QListViewItemIterator it( listView_TTT );
     while (*pt && *pt != '[' && i<nbMax)
           {var_name = "";
            pt   = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &strTab[0], &strTab[1] );
            it.current()->setText (0, var_name);
            it.current()->setText (1, strTab[0]);
            it.current()->setText (2, strTab[1]);
            ++it;
            ++i;
           }
    }
 //............ reactualiser affichage des listes de tables ..........................
 m_pCBigShrinker->show();
 m_pQScrollView->show();
 ButtonSaveDisplay(G_pCApp->m_Droits);
 VAR_RearangeTable();
}

//--------------------------------- Slot_OnValueTableChanged --------------------------------------------------
void FormRubTerrain::Slot_OnValueTableChanged (int /*row*/, int /*col*/,  CMyQTable * /*pCMyQTable*/)
{if (m_IsModifiable==0) return;
 m_IsModified |= 64;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}

//--------------------------------- IfModified_SaveInRubList --------------------------------------------------
/*! \brief Si une modification dans le terrain est a sauvegarder cette fonction le fait.
 * Utilise une variable d'analyse de l'\303\251tat de modification et la fonction d'analyse de l'\303\251tat de modification de la classe Atcd_Code.
*/
int FormRubTerrain::IfModified_SaveInRubList()
{
 if (m_IsModifiable==0) return FALSE;
 QString              stringDST;
 /*
QString            qtext = comboBox_RubName->currentText ();
 int                    f = qtext.findRev( ' ' );   // avant dernier espace separe indice de la liste des rubriques du reste
 if (f==-1)                              return FALSE;
 */
 //........................... si rubrique qui etait avant celle a afficher, a ete modifi\303\251e ...................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 //FormToData(stringDST);
 if ((m_IsModified || m_pAtcd_Code->isModified()) && m_LastRub != -1)
    {FormToData(stringDST);
     m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
     m_IsModified = 0;
     ButtonSaveDisplay(G_pCApp->m_Droits);

     return TRUE;
    }
 m_IsModified = 0;
 ButtonSaveDisplay(G_pCApp->m_Droits);

 return FALSE;
}


//--------------------------------- VAR_SetPoids --------------------------------------------------
// [VAR Constantes de Base]
//      Modele = Date, Pouls, 31536000, 130, 40, pps, Tachycardie, Bradycardie
//      Modele = Date, SAO2, 31536000, 0, 92, %, , Hypoxie
//      Modele = Date, Temp, 31536000, 40.5, 35, ?, Hyporthermie, Hyperthermie
// [VAR Poids]
//      Modele = Date, Poids, 31536000, 100, 50, Kg(s), Obesite, Amaigrissement
//      Modele = Date, Taille, 31536000, 0, 0, cm(s), ,
void FormRubTerrain::VAR_SetPoids(QString poids)
{if (m_IsModifiable==0) return;
 QDateTime  dt_deb  = QDateTime::fromString ("1900-01-01T00:00:00", Qt::ISODate ); //Qt::ISODate - ISO 8601 extended format (YYYY-MM-DD, or with time, YYYY-MM-DDTHH:MM:SS)
 QDateTime  dt_max  = QDateTime::fromString ("2900-01-01T00:00:00", Qt::ISODate );
 QString      value = "";
 int          col   = -1;
 CMyQTable   *pCMyQTable = VAR_getTableValue(&dt_deb,
                                             &dt_max,
                                             "",
                                             "Poids",
                                             value,
                                             &col);
 dt_deb  = QDateTime::currentDateTime ();
 if (pCMyQTable==0)
    {VAR_CreateTable( tr ("[VAR Poids]\n"
                          "  Mod\303\250le = Date, Poids, 31536000, 100, 50, Kg(s), Ob\303\251sit\303\251, Amaigrissement\n"
                          "  Mod\303\250le = Date, Taille, 31536000, 0, 0, cm(s), , \n") +
                          dt_deb.toString("yyyy-MM-dd hh:mm:ss") + "  = " + poids + ", \n"   // 2005-02-01T19:24:19
                    );
    }
 else if (value != poids)
    {QStringList valueList;
     valueList.append(dt_deb.toString("yyyy-MM-dd hh:mm:ss"));
     valueList.append(poids);
     valueList.append(" ");
     VAR_addValueInTable(pCMyQTable, valueList);
    }
}
//--------------------------------- VAR_SetTaille --------------------------------------------------
// [VAR Constantes de Base]
//      Modele = Date, Pouls, 31536000, 130, 40, pps, Tachycardie, Bradycardie
//      Modele = Date, SAO2, 31536000, 0, 92, %, , Hypoxie
//      Modele = Date, Temp, 31536000, 40.5, 35, ?, Hyporthermie, Hyperthermie
// [VAR Poids]
//      Modele = Date, Poids, 31536000, 100, 50, Kg(s), Obesite, Amaigrissement
//      Modele = Date, Taille, 31536000, 0, 0, cm(s), ,
void FormRubTerrain::VAR_SetTaille(QString taille)
{if (m_IsModifiable==0) return;
 QDateTime  dt_deb  = QDateTime::fromString ("1900-01-01T00:00:00", Qt::ISODate ); //Qt::ISODate - ISO 8601 extended format (YYYY-MM-DD, or with time, YYYY-MM-DDTHH:MM:SS)
 QDateTime  dt_max  = QDateTime::fromString ("2900-01-01T00:00:00", Qt::ISODate );
 QString      value = "";
 int          col   = -1;
 CMyQTable   *pCMyQTable = VAR_getTableValue(&dt_deb,
                                             &dt_max,
                                             "",
                                             "Taille",
                                             value,
                                             &col);
 dt_deb  = QDateTime::currentDateTime ();
 if (pCMyQTable==0)
    {VAR_CreateTable( tr ("[VAR Poids]\n"
                          "  Mod\303\250le = Date, Poids, 31536000, 100, 50, Kg(s), Ob\303\251sit\303\251, Amaigrissement\n"
                          "  Mod\303\250le = Date, Taille, 31536000, 0, 0, cm(s), , \n") +
                          dt_deb.toString("yyyy-MM-dd hh:mm:ss") + "  = ," + taille + "\n"   // 2005-02-01T19:24:19
                    );
    }
 else if (value != taille)
    {QStringList valueList;
     valueList.append(dt_deb.toString("yyyy-MM-dd hh:mm:ss"));
     valueList.append("");
     valueList.append(taille);
     VAR_addValueInTable(pCMyQTable, valueList);
    }
}
//--------------------------------- VAR_SetPoidsTaille --------------------------------------------------
// [VAR Constantes de Base]
//      Modele = Date, Pouls, 31536000, 130, 40, pps, Tachycardie, Bradycardie
//      Modele = Date, SAO2, 31536000, 0, 92, %, , Hypoxie
//      Modele = Date, Temp, 31536000, 40.5, 35, ?, Hyporthermie, Hyperthermie
// [VAR Poids]
//      Modele = Date, Poids, 31536000, 100, 50, Kg(s), Obesite, Amaigrissement
//      Modele = Date, Taille, 31536000, 0, 0, cm(s), ,
void FormRubTerrain::VAR_SetPoidsTaille(QString poids, QString taille)
{if (m_IsModifiable==0) return;
 QDateTime  dt_deb            = QDateTime::fromString ("1900-01-01T00:00:00", Qt::ISODate ); //Qt::ISODate - ISO 8601 extended format (YYYY-MM-DD, or with time, YYYY-MM-DDTHH:MM:SS)
 QDateTime  dt_max            = QDateTime::fromString ("2900-01-01T00:00:00", Qt::ISODate );
 QString      valuePoids      = "";
 QString      valueTaille     = "";
 int          colPoids        = -1;
 int          colTaille       = -1;
 CMyQTable   *pCMyQTablePoids = VAR_getTableValue(&dt_deb,
                                             &dt_max,
                                             "",
                                             "Poids",
                                             valuePoids,
                                             &colPoids);
 CMyQTable   *pCMyQTableTaille = VAR_getTableValue(&dt_deb,
                                             &dt_max,
                                             "",
                                             "Taille",
                                             valueTaille,
                                             &colTaille);
 dt_deb  = QDateTime::currentDateTime ();
 if (pCMyQTablePoids==0 || pCMyQTableTaille==0)    // si table a variable unique => pas bon pour en mettre 2 => creer une a deux
    {VAR_CreateTable( tr ("[VAR Poids]\n"
                          "  Mod\303\250le = Date, Poids, 31536000, 100, 50, Kg(s), Ob\303\251sit\303\251, Amaigrissement\n"
                          "  Mod\303\250le = Date, Taille, 31536000, 0, 0, cm(s), , \n") +
                          dt_deb.toString("yyyy-MM-dd hh:mm:ss") + "=" + poids + "," + taille + "\n"   // 2005-02-01T19:24:19
                    );
    }
 else if ( (valueTaille != taille || valuePoids != poids) && (pCMyQTablePoids==pCMyQTableTaille))
    {QStringList valueList;
     valueList.append(dt_deb.toString("yyyy-MM-dd hh:mm:ss"));
     valueList.append(poids);
     valueList.append(taille);
     VAR_addValueInTable(pCMyQTableTaille, valueList);
    }
else if (valueTaille != taille && pCMyQTableTaille)
    {QStringList valueList;
     valueList.append(dt_deb.toString("yyyy-MM-dd hh:mm:ss"));
     valueList.append("");
     valueList.append(taille);
     VAR_addValueInTable(pCMyQTableTaille, valueList);
    }
else if (valuePoids != poids && pCMyQTablePoids)
   {QStringList valueList;
     valueList.append(dt_deb.toString("yyyy-MM-dd hh:mm:ss"));
     valueList.append(poids);
     valueList.append("");
     VAR_addValueInTable(pCMyQTablePoids, valueList);
   }
}
//---------------------------------- VAR_addValueInTable --------------------------------------------------
void FormRubTerrain::VAR_addValueInTable( CMyQTable *pCMyQTable, QStringList &valueList)
{if (m_IsModifiable==0) return;
     //QString    text = "";
     //QDateTime  qdt  = QDateTime::currentDateTime();
     //QString    date = qdt.toString ("yyyy-MM-dd hh:mm:ss");
     int         row = pCMyQTable->numRows();
     //...... tester si la derniere n'est pas vide ..............................................
     //for (int i=1; i<pCMyQTable->numCols(); i++)
     //    {text = pCMyQTable->text(row-1,i);
     //     if (text !="") break;
     //    }
     //............... si derniere ligne pas vide rajouter une ligne vide .......................
     //if (text =="" || pCMyQTable->numRows() ==0 )
       {pCMyQTable->insertRows (row);
        //pCMyQTable->setText (row, 0, date);
        //......... la rendre visible ..............
        pCMyQTable->ensureCellVisible (row, 0 );
        QRect rect = pCMyQTable->cellGeometry (row, 0 );
        m_pQScrollView->ensureVisible (rect.left () + pCMyQTable->x(), rect.top() + pCMyQTable->y());
        //.......... la mettre en edition ..........
        //pCMyQTable->setCurrentCell ( row, 1 );
        //pCMyQTable->editCell (row, 1);
        if (valueList.count())
           {int nb_c = pCMyQTable->numCols();
            int nb_v = valueList.count();
            if (nb_c<nb_v) nb_v = nb_c;    // nb_v = minimum des deux
            for (int col=0; col<nb_v; col++)
                {pCMyQTable->setText (row, col, valueList[col] );
                }
            m_IsModified |= 128;
            ButtonSaveDisplay(G_pCApp->m_Droits);
          }
      }
}

//--------------------------------- VAR_getTableValue--------------------------------------------------
/*! \brief Retourne un pointeur sur une table de variable dont le nom est : table_name et la valeur de la colonne dont le nom est : col_name a la derni\303\250re date comprise entre les deux dates : dt_deb et dt_max
* ENTREE : dt_deb        date de debut, si zero elle sera du  1 Jan 1970
*          dt_max        date de fin, si elle sera dt_deb + 200 ans
*          table_name    nom de la table dans laquelle la recherche doit se faire si ""
*                        alors il sera recherche une valeur : col_name  dans toutes les tables
*          col_name      colonne dont la valeur est a rechercher
*          value         variable de retour dans laquelle sera placee la valeur trouvee a la colonne : col_name
*
*          CMyQTable*    pointeur retourne
*          int *col_ret  indice de colonne :  col_name   (si trouv\303\251e)
*/
CMyQTable* FormRubTerrain::VAR_getTableValue(QDateTime  *dt_deb,
                                             QDateTime  *dt_max,
                                             const QString &table_name,
                                             const QString &col_name,
                                             QString &value,
                                             int *col_ret)
{CMyQTable *pCMyQTableOut    = 0;
 int dt_max_must_be_deleted  = 0;
 int dt_deb_must_be_deleted  = 0;

 QDateTime dt;
 if (dt_deb == 0) {dt_deb    = new QDateTime;  dt_deb_must_be_deleted = 1;  dt_deb->setTime_t(0); } // 1 Jan 1970 UTC}
 if (dt_max == 0) {dt_max    = new QDateTime;  dt_max_must_be_deleted = 1; *dt_max = dt_deb->addYears (200); }
 if (dt_deb == 0 || dt_max == 0) return 0;
 value = "";
 for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
     {CMyQTable *pCMyQTable = (CMyQTable *) m_ptrListQTableVar.at( i );
      if (table_name  == pCMyQTable->m_name || table_name=="")
         {//................ rep\303\251rer colonne  correspondant a :a col_name .............................
          int col    = 1;   // sauter la date
          int nb_col = pCMyQTable->numCols();
          while (col < nb_col)
                {if ( pCMyQTable->horizontalHeader()->label(col) == col_name ) break;
                 col++;
                } // endwhile (col < nb_col)
          //...................... si une colone de ce nom est trouvee .....................
          //                       ok on note les valeurs
          if (col < nb_col)
             {pCMyQTableOut = pCMyQTable;  // cette table est OK pour la variable recherchee
              *col_ret      = col;         // retourner la colonne de la variable
             }
          //................ rechercher dans cette table la valeur correspondant aux dates.............................
          if (pCMyQTableOut)
             { int  nb_row = pCMyQTableOut->numRows();
               if (nb_row>0)
                  {//................. parcourir les rangees .........................
                   for (int row=0; row<nb_row ; row++)
                       {//............ recuperer la date ...........................................
                        QString txt = pCMyQTable->text(row,0);    // recuperer la cellule de date
                        if (txt.length())
                           {dt.setDate(  QDate ( atoi(txt.mid(0,4)),     atoi(txt.mid(5,2)),  atoi(txt.mid(8,2))  )  );
                            dt.setTime(  QTime ( atoi(txt.mid(12,2)),    atoi(txt.mid(15,2)), atoi(txt.mid(18,2))  )  ); // 2004-03-31  19:56:17
                           }
                        //............. si la date est plus recente, retenir la valeur .............
                        if (dt>=*dt_deb && dt<=*dt_max)
                           {*dt_deb    = dt;
                            value      = pCMyQTable->text(row, col);
                           } // endif (dt>dt_max)
                       } // endfor (int row=0; row<nb_row ; row++)
                  } // endif (nb_row>0)
             } // endif pCMyQTableOut
         } //endif (table_name  == pCMyQTable->m_name || table_name=="")
     } // for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
 if (dt_deb_must_be_deleted)  delete dt_deb;
 if (dt_max_must_be_deleted)  delete dt_max;
 return pCMyQTableOut ;
}

//--------------------------------- VAR_GetLastValeurVarTable--------------------------------------------------
QString FormRubTerrain::VAR_GetLastValeurVarTable(const QString &table_name, const QString &col_name)
{QString strOut     = "";
 QDateTime  *dt     = new QDateTime;
 QDateTime  *dt_max = new QDateTime;
 if (dt == 0 || dt_max == 0) return 0;

 dt_max->setTime_t(0);  // 1 Jan 1970 UTC
 //QTime ( int h, int m, int s = 0, int ms = 0 )
 for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
     {CMyQTable *pCMyQTable = (CMyQTable *) m_ptrListQTableVar.at( i );
      if (table_name  == pCMyQTable->m_name )
         {//................. reperer la colonne correspondant a col_name .........................
          int col    = 1;
          int nb_col = pCMyQTable->numCols();
          while (col < nb_col)
                {if ( pCMyQTable->horizontalHeader()->label (col) == col_name ) break;
                 ++col;
                }
          //................ valeur de la variable .............................
          int  nb_row = pCMyQTable->numRows();
          if (nb_row>0 && col < nb_col)
             {
              //................. contenu d'une rangee .........................
              for (int row=0; row<nb_row ; row++)
                  {//............ recuperer la date ...........................................
                   QString txt = pCMyQTable->text(row,0);    // recuperer le cellule de date
                   //val1        = txt.mid(0,4);
                   //val2        = txt.mid(5,2);
                   //val3        = txt.mid(8,2);
                   if (txt.length())
                           {dt->setDate(  QDate ( atoi(txt.mid(0,4)),    atoi(txt.mid(5,2)),  atoi(txt.mid(8,2))  )  );
                            //val1        = txt.mid(12,2);
                            //val2        = txt.mid(15,2);
                            //val3        = txt.mid(18,2);
                            dt->setTime(  QTime ( atoi(txt.mid(12,2)),    atoi(txt.mid(15,2)), atoi(txt.mid(18,2))  )  ); // 2004-03-31  19:56:17
                            //dt.setDate(  QDate ( atoi(txt.mid(4,')),  atoi(txt.mid(5,2)),  atoi(txt.mid(8,2))  )  );
                            //dt.setTime(  QTime ( atoi(txt.mid(12,2)), atoi(txt.mid(15,2)), atoi(txt.mid(18,2)) )  ); // 2004-03-31  19:56:17
                           }
                   //............. si la date est plus recente, retenir la valeur .............
                   if (*dt>*dt_max)
                      {*dt_max = *dt;
                       strOut  = pCMyQTable->text(row, col);
                      } // endif (dt>dt_max)
                  } // endfor (int row=0; row<nb_row ; row++)
             } // endif (nb_row>0)
          } // endif (areValueToBeListed)
     } // for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
 delete dt;
 delete dt_max;
 return strOut;
}
//------------------------------------ VAR_CreateTable ----------------------------------------------------
void FormRubTerrain::VAR_CreateTable(const char* text)
{ if (m_IsModifiable==0) return;
  char *pt = (char*) text;
  QString strTab[10];
  QString section, var_name;
  QFont fnt(font());
  fnt.setPointSize(9);
  while((pt=CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
    {//.................................. VAR ........................................................
     if (section.left(3)=="VAR" )                  // a ce stade pt pointe au debut de la nouvelle ligne
        {
        QString name_section = section.mid(4);
         // [VAR Poids]     1      2       3       4    5   6        7                                      8
         //    Mod\303\250le  = Date, Valeur, 31536000, 100, 50, Kg(s), Attention a la surcharge pond\303\251rale, Attention a l'amaigrissement
         //    2004-03-15T17:14:48 =  68
         //                                   val1                             ,    val2               val3      val4 val5 val6  val7       val8
         QStringList   tableLabels;    // titre des colonnes de la table
         CMyQTable    *pQTable = 0;
         int              row  = 0;
         QLabel    *pTextLabel = 0;
         int        n_var      = 0;
         QString    title      = "";
         while (*pt && *pt != '[')
               {var_name = "";
                pt   = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &strTab[0], &strTab[1], &strTab[2], &strTab[3], &strTab[4], &strTab[5], &strTab[6], &strTab[7], &strTab[8]);
                //...................... DEFINITION DE VARIABLE .....................................
                //                       contruire le dialogue des variables
                if (var_name==tr("Mod\303\250le"))
                   {//...................... premier mod\303\250le: creation listview .....................
                    if (pQTable==0)
                       {int    w  = 1278;

                        //............... titre de la variable ......................................
                        pTextLabel = new QLabel( m_pCBigShrinker, name_section);
                        pTextLabel->setFont(fnt);
                        pTextLabel->setFrameShape( QLabel::MenuBarPanel );
                        pTextLabel->setAlignment( int( QLabel::AlignLeft ) );
                        title = name_section;
                        if (strTab[5]!="") title += tr("  Unit\303\251: ") + strTab[5];
                        pTextLabel->resize(w, 20 );

                        //............... QTable des valeurs .......................................
                        pQTable = new CMyQTable ( 0, 2, m_pCBigShrinker, name_section );
                        pQTable->setFont(fnt);
                        pQTable->m_Title = pTextLabel;
                        tableLabels.append( tr("Date & Heure") );
                        tableLabels.append( strTab[1] );
                        //.................. stocker modele de la table ..............................................
                        pQTable->AddModele (var_name + " = " + strTab[0] + ", " + strTab[1] + ", " + strTab[2] + ", " + strTab[3] + ", " + strTab[4] + ", " + strTab[5] + ", "+ strTab[6] + ", " + strTab[7] );
                        n_var++;
                       }
                    //...................... mod\303\250les suivants: ajout d'une variable ...................
                    else
                       {//.................. stocker modele de la table ..............................................
                        pQTable->AddModele (var_name + " = " + strTab[0] + ", " + strTab[1] + ", " + strTab[2] + ", "
                                                             + strTab[3] + ", " + strTab[4] + ", " + strTab[5] + ", "
                                                             + strTab[6] + ", " + strTab[7] );
                        pQTable->insertColumns (1);
                        if (strTab[5]!="") title += ",  " + strTab[5] ;
                        tableLabels.append( strTab[1] );
                        n_var++;
                       }
                   } // endif (var_name=="Mod\303\250le")
                 //................ VALEURS DATEES DE VARIABLE ........................................
                 else if (var_name!="" && pQTable)
                   {pQTable->insertRows (row);
                    pQTable->setText (row, 0, var_name.left(10) + "  " + var_name.mid(11));
                    for (int i=1; i<=n_var;++i)
                        {pQTable->setText (row, i, strTab[i-1]);
                        }
                    ++row;
                   }
               } // end while (*pt && *pt != '[')
         if (pQTable)
            {CMyButton* qw = new CMyButton( m_pCBigShrinker, tr("&Outils"), pQTable);
             if (qw)
                {connect( qw, SIGNAL( Sign_ButtonClickedPtr(const char*, void* ) ),this, SLOT( Slot_TableButtonClicked(const char*, void*) ) );
                 qw->setFont(fnt);
                 qw->resize(90, 22 );
                 qw->show();
                 pQTable->m_ButtonTools  = qw;
                }
             qw = new CMyButton( m_pCBigShrinker, tr("&Nouveau"), pQTable);
             if (qw)
                {connect( qw, SIGNAL( Sign_ButtonClickedPtr(const char*, void* ) ),this, SLOT( Slot_TableNewDateClicked(const char*, void*) ) );
                 qw->resize(22, 22 );
                 qw->setPixmap ( Theme::getIcon( "22x22/today.png")  );
                 qw->show();
                 pQTable->m_ButtonNew = qw;
                }

             pQTable->setColumnLabels (tableLabels);
             pQTable->adjustColumn (0);
             for (int i=1; i<=n_var;++i) pQTable->adjustColumn (i);
             QRect rectTable = pQTable->cellGeometry (0, n_var );
             pQTable->resize( QSize(rectTable.right ()+50, 120) );
             pQTable->ensureCellVisible (row, 1 );
             pTextLabel->setText( title);
             m_ptrListQTableVar.append( pQTable );
             pQTable->show();
             pTextLabel->show();
            } // endif (pQTable)
         section="";
        } // endif (section.left(3)=="VAR" )
    } //end while((pt=CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
 VAR_RearangeTable();
 //............ reactualiser affichage des listes de tables ..........................
 m_pCBigShrinker->show();
 m_pQScrollView->show();
 m_IsModified |= 32;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}

//------------------------------- VAR_RearangeTable ---------------------------------------------------------
long  FormRubTerrain::VAR_RearangeTable()
{int pos_y = 1;
 for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
     {CMyQTable *pCMyQTable = (CMyQTable *) m_ptrListQTableVar.at( i );
      if (pCMyQTable)
         {if (pCMyQTable->m_Title)       pCMyQTable->m_Title->move( QPoint(155, pos_y) );
          if (pCMyQTable->m_ButtonTools) pCMyQTable->m_ButtonTools->move( QPoint(28, pos_y) );
          if (pCMyQTable->m_ButtonNew)   pCMyQTable->m_ButtonNew->move( QPoint(3, pos_y) );
          pCMyQTable->move( QPoint(2, pos_y + 22) );
          pos_y += 150;
         }
     } //for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
 return 1;
}

//---------------------------------- ClearAllTableValues---------------------------------------------------------
void FormRubTerrain::ClearAllTableValues()
{if (m_IsModifiable==0) return;
 for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
     {CMyQTable *pCMyQTable = (CMyQTable *) m_ptrListQTableVar.at( i );
      if (pCMyQTable)
         {int max_row = pCMyQTable->numRows();
          for ( int row = 0; row < max_row; ++row )
              {pCMyQTable->removeRow (row );
              }
         }
     } //for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
}

//--------------------------------- initComboRubName --------------------------------------------------
long  FormRubTerrain::initComboRubName(void)
{long nb = m_pCMoteurBase->initConboBoxWithRubList(m_pRubList, comboBox_RubName, QString::number(TYP_TERRAIN));
 return nb;
}

//---------------------------------- pushButtonNewVAR_clicked --------------------------------------------------
void FormRubTerrain::pushButtonNewVAR_clicked()
{if (m_IsModifiable==0) return;
 QFileDialog *dlg = new QFileDialog ( G_pCApp->m_PathTerrain, "*.txt", this, "ChercherMasqueTerrain", TRUE);
 if (dlg==0)                           return;
 if (dlg->exec() != QDialog::Accepted) {delete dlg; return;}
 QString fileName = dlg->selectedFile();
 delete dlg;
 newVAR_Add(fileName);
}

//---------------------------------- newVAR_Add --------------------------------------------------
void FormRubTerrain::newVAR_Add(const char *fileName)
{QString qstr;
 CGestIni::Param_UpdateFromDisk(fileName , qstr);
 VAR_CreateTable(qstr);
 m_IsModified |= 2;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}

//------------------------------------ pushButtonDellAllVAR_clicked --------------------------------------------------
void FormRubTerrain::pushButtonDellAllVAR_clicked()
{if (m_IsModifiable==0) return;
 int ret = QMessageBox::warning ( this, tr ("Effacer toutes les variables ") + QString(PROG_NAME),
                                        tr ("Voulez vous vraiment effacer\r\n"
                                            "tous les tableaux de variables ?\r\n"
                                            "CETTE ACTION EST IRREVERSIBLE"),
                                        tr ("&Effacer"), tr("Annu&ler"), 0,
                                   1, 1 );
 if (ret >=1 ) return;
 //........... effacer les anciennes tables de variables ................
 if (m_pCBigShrinker != 0) delete m_pCBigShrinker;
 m_pCBigShrinker = new CBigShrinker(0,1280,4000);
 m_pQScrollView->addChild(m_pCBigShrinker);
 m_pCBigShrinker->show();
 m_pQScrollView->show();
 m_IsModified |= 1;
 ButtonSaveDisplay(G_pCApp->m_Droits);
 m_ptrListQTableVar.clear();
}

//---------------------------------- Slot_TableButtonClicked --------------------------------------------------
void FormRubTerrain::Slot_AllTableDelete()
{if (m_IsModifiable==0) return;
 m_ptrListQTableVar.clear();
 m_IsModified |= 2;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}

//---------------------------------- Slot_TableButtonClicked --------------------------------------------------
void FormRubTerrain::Slot_TableButtonClicked(const char * /*name*/, void* ptr)
{if (m_IsModifiable==0) return;
 QPopupMenu *pQPopupMenu = new ThemePopup(this, "MyPopupMenu" );
 m_pQTable = (CMyQTable*) ptr;

 pQPopupMenu->insertItem(Theme::getIconListDelete(), tr("Effacer cette date"), this, SLOT( VAR_MenuActionDelete()),      CTRL+Key_D );
 pQPopupMenu->insertItem( Theme::getIcon( "16x16/edit.png"), tr("Changer cette date"), this, SLOT( VAR_MenuActionChangeDate()),  CTRL+Key_K );
 pQPopupMenu->insertItem( Theme::getIconListAdd(), tr("Nouvelle date"),      this, SLOT( VAR_MenuActionNewDate()),     CTRL+Key_N );
 pQPopupMenu->insertSeparator ();
 pQPopupMenu->insertItem( Theme::getIcon("16x16/configure.png"), tr("Configurer"),         this, SLOT( VAR_MenuActionConfig()),      CTRL+Key_J );
 pQPopupMenu->insertItem( Theme::getIcon("16x16/delete_Icon.png"), tr("D\303\251truire cette variable"), this, SLOT( VAR_MenuActionDetruireVar()), CTRL+Key_E );

 pQPopupMenu->exec(QCursor::pos());
 delete pQPopupMenu;
 m_pQTable = 0;

}

//---------------------------------- VAR_MenuActionDelete --------------------------------------------------
void FormRubTerrain::VAR_MenuActionDelete()
{if (m_IsModifiable==0) return;
 if (m_pQTable==0) return;
 m_pQTable->removeRow ( m_pQTable->currentRow () );
 m_IsModified |= 2;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}

//---------------------------------- VAR_MenuActionChangeDate --------------------------------------------------
void FormRubTerrain::VAR_MenuActionChangeDate()
{if (m_IsModifiable==0) return;
}

//---------------------------------- VAR_MenuActionNewDate --------------------------------------------------
void FormRubTerrain::VAR_MenuActionNewDate()
{if (m_IsModifiable==0) return;
 Slot_TableNewDateClicked(tr("&Nouveau"), m_pQTable);
}

//---------------------------------- VAR_MenuActionConfig --------------------------------------------------
void FormRubTerrain::VAR_MenuActionConfig()
{if (m_IsModifiable==0) return;
}

//---------------------------------- VAR_MenuActionDetruireVar --------------------------------------------------
void FormRubTerrain::VAR_MenuActionDetruireVar()
{if (m_IsModifiable==0) return;
 if (m_pQTable==0)      return;

 for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
     {CMyQTable *pCMyQTable = (CMyQTable *) m_ptrListQTableVar.at( i );

     if (m_pQTable == pCMyQTable)
        { m_ptrListQTableVar.remove (m_pQTable);
          if (m_pQTable->m_ButtonNew)   delete   m_pQTable->m_ButtonNew;
          if (m_pQTable->m_ButtonTools) delete   m_pQTable->m_ButtonTools;
          if (m_pQTable->m_Title)       delete   m_pQTable->m_Title;
          delete m_pQTable;
          break;
        }
     } //for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
 VAR_RearangeTable();
 m_IsModified |= 32;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}

//---------------------------------- Slot_TableNewDateClicked --------------------------------------------------
void FormRubTerrain::Slot_TableNewDateClicked(const char *name, void* ptr)
{if (m_IsModifiable==0) return;
 if (name==tr("&Nouveau") && ptr)
    {CMyQTable *pCMyQTable = (CMyQTable*) ptr;
     QString    text = "";
     QDateTime  qdt  = QDateTime::currentDateTime();
     QString    date = qdt.toString ("yyyy-MM-dd  hh:mm:ss");
     int         row = pCMyQTable->numRows();
     //...... tester si la derni\303\250re n'est pas vide ..............................................
     for (int i=1; i<pCMyQTable->numCols(); i++)
         {text = pCMyQTable->text(row-1,i);
          if (text !="") break;
         }
    //............... si derni\303\250re ligne pas vide rajouter une ligne vide .......................
    if (text !="" || pCMyQTable->numRows() ==0 )
       {pCMyQTable->insertRows (row);
        pCMyQTable->setText (row, 0, date);
        //......... la rendre visible ..............
        pCMyQTable->ensureCellVisible (row, 0 );
        QRect rect = pCMyQTable->cellGeometry (row, 0 );
        m_pQScrollView->ensureVisible (rect.left () + pCMyQTable->x(), rect.top() + pCMyQTable->y());
        //.......... la mettre en edition ..........
        pCMyQTable->setCurrentCell ( row, 1 );
        pCMyQTable->editCell (row, 1);
       }
    } // endif (name==tr("nouveau") && ptr)
}

//---------------------------------- OnCMyQTableClicked --------------------------------------------------
void FormRubTerrain::OnCMyQTableClicked(CMyQTable *pCMyQTable, const char* name, int/* row*/, int /*col*/, int /*button*/, const QPoint & /*mousePos*/)
{if (m_IsModifiable==0) return;
 Slot_TableNewDateClicked(name, pCMyQTable);
}

//--------------------------------- getAtcdList  --------------------------------------------------
/*
   <m_allergies>
     3007528|CIP|ASPIRINE|ALD|Allergie(Allergie)|choc anaphylactique grâve |06-06-1956|
     3515495|CIP|LAMALINE|Sport|Allergie(Allergie)|urticaire généralisé |06-06-1956|
     3507745|CIP|AMOXICILINE ET INHIBITEUR D'ENZYME|ALD|Allergie(Allergie)|bronchospasme allergique sévère|06-06-1956|
   </m_allergies>
   <m_antecedents>
     K27|CIM|ulcère gastro duodénal|ALD|Médical(Gastro)|pas de commentaire|06-06-1956|
     N19|CIM|Insuf renale|ALD|Médical(uro néphro)|pas de commentaire|06-06-1956|
     W78|CISP|Grossesse||Médical(Obstétrique)|pas de commentaire|10-10-2012|
     Z32|CIM|Grossesse||Médical(Obstétrique)|pas de commentaire|10-10-2012|
     Z39.1|CIM|Allaitement||Médical(Puerpéralité)|pas de commentaire|10-10-2012|
     |MD|Pratique du judo à haut niveau|Sport|Habitudes(Activité Sportive)|Attention aux produits dopants|10-10-2012|
     |MD|chauffeur de bus scolaire|Vigilance|Habitudes(Profession)|Attention aux produits altérant la vigilance|10-10-2012|
 */
QStringList FormRubTerrain::getAtcdList( const QString &tableName )
{  QListViewItemIterator it( listView_ATCD );
   QStringList retList;
   QString     element;
   m_pCMoteurBase->GotoDebug();
   while ( it.current() )
       { CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
         QString code = pCPrt->text(6);
         if ( tableName=="allergies")
            { if (!code.startsWith("(")) {++it;continue;}
            }
         else
            { if ( code.startsWith("(")) {++it;continue;}
            }
         /*
         QString str_0 = pCPrt->text(0); qDebug(str_0);
         QString str_1 = pCPrt->text(1); qDebug(str_1);
         QString str_2 = pCPrt->text(2); qDebug(str_2);
         QString str_3 = pCPrt->text(3); qDebug(str_3);
         QString str_4 = pCPrt->text(4); qDebug(str_4);
         QString str_5 = pCPrt->text(5); qDebug(str_5);
         */
         QDate   dateDeb;
         QDate   dateEnd;
         QString s_dateDeb = CGenTools::NormaliseDateToIso(pCPrt->text(2));
         QString s_dateEnd = CGenTools::NormaliseDateToIso(pCPrt->text(3));

         dateDeb  = QDate::fromString(s_dateDeb,Qt::ISODate);
         if (! dateDeb.isValid ())  dateDeb = QDate::currentDate().addDays (-2);
         s_dateDeb = dateDeb.toString("dd-MM-yyyy");

         dateEnd   = QDate::fromString(s_dateEnd,Qt::ISODate);
         if (! dateEnd.isValid ()) s_dateEnd = "";
         else                      s_dateEnd = dateEnd.toString("dd-MM-yyyy");


         //.................. code de l'atcd ou allergie ........................................
         // N19|CIM|Insuf renale|ALD|Médical(uro néphro)|pas de commentaire|06-06-1956|
         element = "";

         if (code.startsWith("-("))
            { element += code.remove('-').remove('(').remove(')') + "|CISP|";
            }
         else if (code.startsWith("(-"))
            { element += code.remove("(-").remove("-)")           + "|CIP|";
            }
         else if (code.startsWith("~"))
            { element += code.remove('~')                         + "|CIM|";
            }
         else if (code.startsWith("(."))
            { element += code.remove("(.").remove(".)")           + "|ATC|";
            }
         else if (code.startsWith("("))
            { element += code.remove('(').remove(')')             + "|DS_E|";   // excipient
            }
         else
            { element += code                                     + "|MD|";
            }
         element += pCPrt->text(0)                 + "|";                              // libelle
         element += pCPrt->text(5)                 + "|";                              // statut ALD / SPORT / VIGILANCE
         element += pCPrt->text(1)                 + "|";                              // classse medintux
         element += pCPrt->text(4).remove('|')     + "|";                              // commentaire
         element += s_dateDeb                      + "|";                              // date debut
         element += s_dateEnd                           ;                              // date fin
         retList.append(element);
        ++it;
       }
    return retList;
}

//--------------------------------- pushButtonSave_clicked --------------------------------------------------
void FormRubTerrain::pushButtonSave_clicked()
{if (m_IsModifiable==0) return;
 //QString strDST;
 //FormToData(strDST);
 //m_IsModified = 0;
 //disconnect(m_pAtcd_Code, SIGNAL( ATCD_Changed() ), this, SLOT( ATCD_HaveChanged() ));
 //.......... on sauvegarde les antecedents et le traitemment en cours dans les tables ..........................
 m_pCMoteurBase->save_ListAtcd(G_pCApp->m_ID_Doss, G_pCApp->m_NumGUID, G_pCApp->m_User, G_pCApp->m_SignUser,"antecedents", listView_ATCD );
 m_pCMoteurBase->save_ListAtcd(G_pCApp->m_ID_Doss, G_pCApp->m_NumGUID, G_pCApp->m_User, G_pCApp->m_SignUser,"allergies"  , listView_ATCD );
 G_pCApp->m_pCMedicaBase->save_TraitementEnCours(G_pCApp->m_ID_Doss, G_pCApp->m_NumGUID, G_pCApp->m_User, G_pCApp->m_SignUser, m_OrdoList, G_pCApp->m_pCMoteurBase->m_DataBase );
 emit Sign_SaveButtonClicked();
 //connect(m_pAtcd_Code, SIGNAL( ATCD_Changed() ), this, SLOT( ATCD_HaveChanged() ));
 //sexeAndParturienteDisplay();  qDebug ("pushButtonSave_clicked() sexeAndParturienteDisplay()");
}

//--------------------------------- VAR_GetModeleList --------------------------------------------------
// ACTION: retourne une chaine de caract\303\250re avec la liste des mod\303\251les des tables et  leur valeurs si areValueToBeListed==1
void FormRubTerrain::VAR_GetModeleList(QString &strDST, int areValueToBeListed)
{
 for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
     {CMyQTable *pCMyQTable = (CMyQTable *) m_ptrListQTableVar.at( i );
      strDST  += "[VAR " +pCMyQTable->m_name + "]\r\n";   // nom de la variable
      //................ mod\303\250les de la variable ............................
      for ( QStringList::Iterator it = pCMyQTable->m_ModeleList.begin(); it != pCMyQTable->m_ModeleList.end(); ++it )
          {strDST  += "   " + (*it) + "\r\n";
          }
      if (areValueToBeListed)
         {//................ valeur de la variable .............................
          int  nb_row = pCMyQTable->numRows();
          if (nb_row>0)
             {//................. contenu d'une rang\303\251e .........................
              for (int row=0; row<nb_row ; row++)
                  {strDST  += "    " + pCMyQTable->text(row,0).replace("  ","T") + " = "; // premiere colonne est la date
                   int col    = 1;
                   int nb_col = pCMyQTable->numCols();
                   while (col < nb_col)
                     {strDST  +=  pCMyQTable->text(row, col);
                      col++;
                      if (col < nb_col) strDST  += ", ";
                     }
                   strDST  += "\r\n";
                  } // endfor (int row=0; row<nb_row ; row++)
             } // endif (nb_row>0)
          } // endif (areValueToBeListed)
     } // for ( uint i = 0; i < m_ptrListQTableVar.count(); ++i )
}


////////////////////////////////////// LIST VIEW TTT ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------ listView_TTT_contextMenuRequested --------------------------------------------------
void FormRubTerrain::listView_TTT_contextMenuRequested( QListViewItem *qlistViewItem, const QPoint &pos, int )
{if (m_IsModifiable==0) return;

 QPopupMenu *pQPopupMenu = new ThemePopup(this, "MyPopupMenu" );

 m_pQListViewItem = qlistViewItem;

pQPopupMenu->insertItem( Theme::getIconListEdit(),  tr("Modifier le traitement de fond"),                             this, SLOT( TTT_SlotMenuActionModifier()),      CTRL+Key_M );
pQPopupMenu->insertItem( Theme::getIcon("16x16/listview_renouv.png"), tr("Renouveler le traitement de fond"),         this, SLOT( TTT_SlotMenuActionRenouveler()),        CTRL+Key_R );
// pQPopupMenu->insertItem( Theme::getIcon("16x16/list_all_ttt.png"), tr("Prescrire le traitement en cours"),            this, SLOT( TTT_MenuActionPrescrire()),         CTRL+Key_L );
pQPopupMenu->insertSeparator ();
pQPopupMenu->insertItem( tr("Traitement \303\240 renouveler"),                      this, SLOT( TTT_SlotMenuActionSetRenouveler()),     CTRL+Key_H );
pQPopupMenu->insertItem( tr("Traitement intercurrent"),                             this, SLOT( TTT_SlotMenuActionSetIntercurent()),    CTRL+Key_I );

if (qlistViewItem!=0)
    {// pQPopupMenu->insertSeparator ();
     // pQPopupMenu->insertItem( tr("Effacer une prescription de la liste"),          this, SLOT( TTT_MenuActionDel()),           CTRL+Key_F );
     // pQPopupMenu->insertSeparator ();
     // pQPopupMenu->insertItem( tr("Cette preescription est renouvelable"),          this, SLOT( TTT_MenuActionSetRenouv()),     CTRL+Key_H );
     // pQPopupMenu->insertItem( tr("Cette preescription est Non renouvelable"),      this, SLOT( TTT_MenuActionSetNonRenouv()),  CTRL+Key_L );
    }
 pQPopupMenu->exec(pos);
 delete pQPopupMenu;
 m_pQListViewItem = 0;
}
void FormRubTerrain::Alert_WriteOnly()
{QMessageBox::warning ( this, tr(PROG_NAME" Modification d'un document"),
                              tr ( "La modification de ce document n'est pas\r\n"
                                   "permise sur un dossier en mode lecture seule.\r\n"),
                                   tr("Annuler"), 0, 0,
                                             1, 1 );
}

//------------------------------------ TTT_PutPrescriptionInTerrain --------------------------------------------------
void FormRubTerrain::TTT_PutPrescriptionInTerrain(RUBREC_LIST::iterator it)
{if (m_IsModifiable==0)
    {Alert_WriteOnly();
     return;
    }
 
 if (((*it).m_Type).toInt()!=TYP_ORDO_CALC) return;
 //........ recuperer le BLOB et convertir UTF8 .............
 QString stringDST;
 QByteArray         ba_data;
 m_pCMoteurBase->GetDataFromRubList(ba_data, it);       // recuperer donn\303\251es soit dans liste cache soit sur disque
 char *ptr        = ba_data.data();
 long len         = ba_data.size();
 if  (ptr && len>0)
     {if ( CGestIni::IsUtf8( ptr, len ) ) stringDST = QString::fromUtf8 ( ptr );
      else                                stringDST = ba_data.data();
     }
 //.............. le placer ...................................
 TTT_PutBlobPrescriptionInTerrain(stringDST);
 m_IsModified |= 1;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}
//------------------------------------ TTT_PutBlobPrescriptionInTerrain --------------------------------------------------
void FormRubTerrain::TTT_PutBlobPrescriptionInTerrain(const QString &datas)
{        m_LAP_Ordo               = "";
         QString stringStruct     = "";
         QString stringHasStruct  = "";
         CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData     (datas, 0, &stringStruct);       // y isoler et recuperer les données calculables
         CMedicaBase::Medica_DiskDataSplitIn_HtmlData_HAS_StructData (datas, 0, &stringHasStruct);    // y isoler et recuperer les données calculables
         if (stringHasStruct.length())
            { m_LAP_Ordo = datas;
              TTT_LapXmlToListView(listView_TTT, m_LAP_Ordo );
	          CMedicaBase::Medica_PosologieListDeserialize(G_pCApp->m_pCMedicaBase->Medica_Has_StructDataToOldXmlStruct(m_LAP_Ordo), m_OrdoList);
            }
         else
            { CMedicaBase::Medica_PosologieListDeserialize(stringStruct, m_OrdoList);
              TTT_OrdoListToListView(listView_TTT, m_OrdoList);
            }
}
//------------------------------------ TTT_SlotMenuActionModifier() --------------------------------------------------
void FormRubTerrain::TTT_SlotMenuActionModifier()
{ TTT_Modifier("MODIF_TERRAIN");
}
//------------------------------------ TTT_Modifier --------------------------------------------------
int FormRubTerrain::TTT_Modifier(const QString &mode)
{if (m_IsModifiable==0)
    {Alert_WriteOnly();
     return 0;
    }
  QString ordo = "";
  if (m_LAP_Ordo.length()!=0 ||  m_OrdoList.size()!=0)
     { ordo = G_pCApp->m_pCMedicaBase->Medica_PosologieListToXMLOrdoStruct( m_OrdoList, GetPoids(), GetTaille() , m_LAP_Ordo);  // recuperer l'ordo (soit ancien format soit nouveau)
     } 
  int retLap  =  G_pCApp->m_pDrTux->Lap_Lauch(ordo, mode);        // si ordo ancienne forme       retLap != DrTux::IS_ALREADY_LAUCH  ==> on va ds mode degrade
  if (retLap == DrTux::IS_ALREADY_LAUCH) {  return 1;}            // Ok tout s'est bien passe et le LAP repondra dans le Slot_LapExited()

 //................ forme degradee du LAP avec ancien assistant ............................
 int ret = 0;
 Dlg_MedicaTux *dlg = new Dlg_MedicaTux(this,"Therapeutique_Dial",TRUE);
 if (dlg ==0)                                                          return 0;
 if (dlg->initDialog(G_pCApp->m_pCMedicaBase, m_OrdoList)==0) {delete dlg; return 0;};  // poids du malade = 70 kgs   m_PosoList;
 if (dlg->exec()== QDialog::Accepted)
    {QPL_CPosologie ordoList = dlg->GetOrdoList();
     m_OrdoList = ordoList;
     TTT_OrdoListToListView(listView_TTT, m_OrdoList);
     m_IsModified |= 1;
     ButtonSaveDisplay(G_pCApp->m_Droits);
     ret = 1;
    }
 delete dlg;
 return ret;
}
//------------------------------------ Slot_LapExited --------------------------------------------------
void FormRubTerrain::TTT_Slot_LapExited()
{ QString ordoBlob     = G_pCApp->m_pDrTux->Lap_ExchangesFilesToDataBlob();
  if ( ordoBlob.length() )  
     { TTT_PutBlobPrescriptionInTerrain(ordoBlob);
       m_IsModified |= 1;
       ButtonSaveDisplay(G_pCApp->m_Droits);
     }
  G_pCApp->m_pDrTux->Lap_StopProcess();   // le mettre a la fin car test de fin de proces doit se faire apre avoir recupere les fichiers d'exchange  
}

//------------------------------------ TTT_MenuActionRenouveler --------------------------------------------------
void FormRubTerrain::TTT_SlotMenuActionRenouveler()
{if (m_IsModifiable==0)
    {Alert_WriteOnly();
     return;
    }
 //............... creer la liste de medicaments uniquement renouvelables ....................
 QPL_CPosologie ordo_list;
 QListViewItemIterator ut( listView_TTT );
 while ( ut.current() )
       {if (  ut.current()->text (1) == tr("Renouvelable"))
           {int id = ut.current()->text (2).toInt();           // recuperer indice
            QPL_CPosologie::iterator it = m_OrdoList.at (id);
            ordo_list.append((*it));
           }
        ++ut;
       }
 //.............. methode directe qui ne passe pas par le LAP .................
 // TYP_ORDO_CALC_CURENT pour que date ordo soit la date courante lors appel DrTux::AddNewRecordToRubrique()
 //  int sav = m_IsModified;
 //  #define TYP_ORDO_CALC_CURENT    CMDI_Prescription::S_GetType()+999     dans CApp.h
 //  QString ordo = G_pCApp->m_pCMedicaBase->Medica_PosologieListToXMLOrdoStruct(ordo_list, GetPoids(), GetTaille() , m_LAP_Ordo);
 //  emit Sign_Renouveler(ordo , TYP_ORDO_CALC_CURENT, 0 );
 //  m_IsModified = sav;
 //................. methode qui passe par le LAP ..............................
 m_StateModifBeforeLAP = m_IsModified;
 TTT_Modifier("RENOUV_TERRAIN");
 //  La sortie,  recuperation et fin  de cette action se fera dans  Slot_LapExitedRenouv()
}
//------------------------------------ Slot_LapExitedRenouv --------------------------------------------------
void FormRubTerrain::TTT_Slot_LapExitedRenouv()
{ G_pCApp->m_pDrTux->Lap_StopProcess();
  QString ordoBlob     = G_pCApp->m_pDrTux->Lap_ExchangesFilesToDataBlob();
  if ( ordoBlob.length() )  
     { TTT_PutBlobPrescriptionInTerrain(ordoBlob);
       m_IsModified |= 1;
       ButtonSaveDisplay(G_pCApp->m_Droits);
       // TYP_ORDO_CALC_CURENT pour que date ordo soit la date courante lors appel DrTux::AddNewRecordToRubrique()
       emit Sign_Renouveler(ordoBlob, TYP_ORDO_CALC_CURENT, 0 );
     }
  m_IsModified = m_StateModifBeforeLAP;
}
//------------------------------------ TTT_MenuActionPrescrire --------------------------------------------------
void FormRubTerrain::TTT_SlotMenuActionPrescrire()
{if (m_IsModifiable==0)
    {Alert_WriteOnly();
     return;
    }
 int sav = m_IsModified;
 // TYP_ORDO_CALC_CURENT pour que date ordo soit la date courante lors appel DrTux::AddNewRecordToRubrique()
 QString ordo = G_pCApp->m_pCMedicaBase->Medica_PosologieListToXMLOrdoStruct( m_OrdoList, GetPoids(), GetTaille() , m_LAP_Ordo);
 emit Sign_Renouveler(ordo , TYP_ORDO_CALC_CURENT, 0 );
 m_IsModified = sav;
}

//------------------------------------ TTT_MenuActionSetRenouveler --------------------------------------------------
void FormRubTerrain::TTT_SlotMenuActionSetRenouveler()
{if (m_IsModifiable==0)
    {Alert_WriteOnly();
     return;
    }
 if (m_pQListViewItem == 0) return;
 m_pQListViewItem->setSelected (TRUE);   // reselectionner l'item au dessus duquel se retrouve la souris
 QListViewItemIterator it( listView_TTT );
 while ( it.current() )
       {if ( it.current()->isSelected() ) it.current()->setText ( 1, tr("Renouvelable"));
        ++it;
       }
 m_pQListViewItem = 0;
 m_IsModified |= 1;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}

//------------------------------------ TTT_MenuActionSetIntercurent --------------------------------------------------
void FormRubTerrain::TTT_SlotMenuActionSetIntercurent()
{if (m_IsModifiable==0)
    {Alert_WriteOnly();
     return;
    }
 if (m_pQListViewItem == 0) return;
 m_pQListViewItem->setSelected (TRUE);   // reselectionner l'item au dessus duquel se retrouve la souris
 QListViewItemIterator it( listView_TTT );
 while ( it.current() )
       {if ( it.current()->isSelected() ) it.current()->setText ( 1, tr("Intercurent"));
        ++it;
       }
 m_pQListViewItem = 0;
 m_IsModified |= 1;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}
//--------------------------------- TTT_LapXmlToListView --------------------------------------------------
void FormRubTerrain::TTT_LapXmlToListView(QListView *pQListView, const QString &lap_Ordo )
{ QStringList ordoLinesList = CGestIni::getXmlDataList("OrdoLine", lap_Ordo);
  QString ordoLine          = "";
  QString product_name      = "";
  int                    id = 0;
  pQListView->clear();
  for (int i=0; i < (int) ordoLinesList.size();++i)
      {ordoLine         = ordoLinesList[i].stripWhiteSpace();;
       product_name     = CGestIni::getXmlData("gph_na", ordoLine);               // recuperer la date de fin de la ligne prescriptive
       new QListViewItem( pQListView,
                          product_name.replace('[','(').replace(']',')'),
                          tr("Renouvelable"),
                          QString::number(id)
                        );
       id++;
      }  
}
//--------------------------------- TTT_OrdoListToListView --------------------------------------------------
void FormRubTerrain::TTT_OrdoListToListView(QListView *pQListView, QPL_CPosologie &ordo_list)
{QPL_CPosologie::iterator it;
 int                  id = 0;
 pQListView->clear();
 for (it = ordo_list.begin(); it != ordo_list.end(); ++it )
     {new QListViewItem( pQListView,
                         G_pCApp->m_pCMedicaBase->Medica_GetMedicamentNameByCIP((*it).m_MEDICA_POSO_CIP).replace('[','(').replace(']',')'),
                         tr("Renouvelable"),
                         QString::number(id)
                       );
     id++;
    } //end for (it = ordo_List.begin(); it != ordo_List.end(); ++it )
}

//--------------------------------- listView_TTT_doubleClicked --------------------------------------------------
void FormRubTerrain::listView_TTT_doubleClicked( QListViewItem * )
{TTT_Modifier("MODIF_TERRAIN");
 //  La sortie,  recuperation et fin  de cette action se fera dans  Slot_LapExited()
}

//--------------------------------- pushButtonNewTTT_clicked --------------------------------------------------
void FormRubTerrain::pushButtonNewTTT_clicked()
{TTT_Modifier("MODIF_TERRAIN");
 //  La sortie,  recuperation et fin  de cette action se fera dans  Slot_LapExited()
}

//--------------------------------- pushButtonDellAllTTT_clicked --------------------------------------------------
void FormRubTerrain::pushButtonDellAllTTT_clicked()
{if (m_IsModifiable==0) return;
 int ret = QMessageBox::warning ( this, tr ("Effacer tous les Traitements ") + QString(PROG_NAME),
                                        tr ("Voulez vous vraiment effacer\r\n"
                                            "toutes les donn\303\251es du traitement de fond\r\n"
                                            "CETTE ACTION EST IRREVERSIBLE"),
                                        tr ("&Effacer"), tr("Annu&ler"), 0,
                                  1, 1 );
 if (ret >=1 ) return;
 listView_TTT->clear();
 m_OrdoList.clear();
 m_LAP_Ordo    = "";
 m_IsModified |= 2;
 ButtonSaveDisplay(G_pCApp->m_Droits);
}

////////////////////////////////////// LIST VIEW ATCD //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------ Add_popMenu_ATCD_Type --------------------------------------------------
/*! \brief Ajoute le menu de selection du type d'ant\303\251cedent a un menu quelconque
*/
void FormRubTerrain::Add_popMenu_ATCD_Type(QPopupMenu* pQPopupMenu)
{connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(ATCD_MenuActionSetFamilleGenre()));
 G_pCApp->addPopupHierarchique(G_pCApp->m_PathAppli + "Ressources/MenuATCD.txt", pQPopupMenu, &m_TypeATCD_Selectionne);
}

//------------------------------------ listView_ATCD_contextMenuRequested --------------------------------------------------
/*! \brief action bouton droit sur la listeview ATCD
*/

   // 0 Libelle
   // 1 famille Genre
   // 2 etat Actif/gueri
   // 3 commentaire
   // 4 date
   // 5 ald
   // 6 code

void FormRubTerrain::listView_ATCD_contextMenuRequested( QListViewItem *qlistViewItem, const QPoint &pos, int c )
{if (m_IsModifiable==0) return;
 //======
 m_TypeATCD_Selectionne              = "";
 m_pQListViewItem                    = qlistViewItem;
 CPrtQListViewItem* pCPrt            = (CPrtQListViewItem*) qlistViewItem;
 if (pCPrt) m_pAtcd_Element_Selected = m_pAtcd_Code->getAtcd_byID( pCPrt->get_ID_Rublist().toInt() );
 ThemePopup *pQPopupMenu             = 0;
 QString ret                         = "";
 QStringList optionList;
 switch(c)
 {case LV_NAME:  // libelle
     {
      ThemePopup *pThemePopup   = new ThemePopup(this, "MyPopupMenu" );
      pThemePopup->insertItem(Theme::getIcon( "Cim10All_Icon.png"),     tr("Ajouter un  Ant\303\251c\303\251dent CIM10"),                  this, SLOT( ATCD_MenuActionNewCIM10()),       CTRL+Key_Dollar );
      pThemePopup->insertItem(Theme::getIcon( "16x16/CispItemTab.png"), tr("Ajouter un  Ant\303\251c\303\251dent CISP"),                   this, SLOT( ATCD_MenuActionNewCISP()),        CTRL+Key_Percent );
      pThemePopup->insertItem(G_pCApp->m_IconTherapeutique, tr("Ajouter une intol\303\251rance m\303\251dicamenteuse"),                    this, SLOT( ATCD_MenuActionNewAllergie()),    CTRL+Key_Exclam );
      pThemePopup->insertItem(Theme::getIconListItem(), tr("Ajouter un ant\303\251c\303\251dent texte libre"),                             this, SLOT( ATCD_MenuActionNewTexteLibre()),  CTRL+Key_E );
      pThemePopup->insertItem(Theme::getIcon("16x16/ThesaurusTab.png"), tr("Ajouter un ant\303\251c\303\251dent \303\240 partir des favoris"),   this, SLOT( ATCD_MenuActionNewFromFavori()),  CTRL+Key_M );
      if (qlistViewItem!=0)
        {// Construit le menu g\303\251n\303\251ral
         pThemePopup->insertSeparator ();
         pThemePopup->insertItem( Theme::getIconListDelete() , tr("Effacer les ant\303\251c\303\251dents s\303\251lectionn\303\251s"),                 this, SLOT( ATCD_MenuActionMultiDel()),       CTRL+Key_F );
         pThemePopup->insertSeparator ();
         pThemePopup->insertItem( Theme::getIconListDateTime(),        tr("Modifier la date de d\303\251but de cet Ant\303\251c\303\251dent"),             this, SLOT( ATCD_MenuActionSetDateDeb()),        CTRL+Key_D );
         pThemePopup->insertItem( Theme::getIcon("16x16/timeDel.png"), tr("Effacer la date de d\303\251but  de cet Ant\303\251c\303\251dent"),             this, SLOT( ATCD_MenuActionDelDateDeb()),        SHIFT+Key_D );
         pThemePopup->insertItem( Theme::getIconListDateTime(),        tr("Modifier la date de fin de cet Ant\303\251c\303\251dent"),             this, SLOT( ATCD_MenuActionSetDateFin()),        CTRL+Key_D );
         pThemePopup->insertItem( Theme::getIcon("16x16/timeDel.png"), tr("Effacer la date de fin  de cet Ant\303\251c\303\251dent"),             this, SLOT( ATCD_MenuActionDelDateFin()),        SHIFT+Key_D );
         pThemePopup->insertSeparator ();
         Add_popMenu_ATCD_Type(pThemePopup);
         pThemePopup->insertSeparator ();
         if (m_pAtcd_Element_Selected->m_Commentaire != "")
            { pThemePopup->insertItem( Theme::getIcon("16x16/commentaire.png"),  m_pAtcd_Element_Selected->m_Commentaire,      this, SLOT( ATCD_MenuActionSetCommentaire()), CTRL+Key_K );
            }
         else
            { pThemePopup->insertItem( Theme::getIcon("16x16/commentaire.png"),  tr("D\303\251finir un commentaire"),          this, SLOT( ATCD_MenuActionSetCommentaire()), CTRL+Key_T );
            }
        pThemePopup->insertSeparator ();
        pThemePopup->insertItem( Theme::getIcon("16x16/ald_on.png"),       tr("En rapport avec une pathologie ALD"),       this, SLOT( ATCD_setAldOn()),         CTRL+Key_I );
        pThemePopup->insertItem( Theme::getIcon("16x16/sport_on.png"),     tr("En rapport avec une activité sportive"),    this, SLOT( ATCD_setSportOn()),       ALT+Key_S );
        pThemePopup->insertItem( Theme::getIcon("16x16/vigilance_on.png"), tr("En rapport avec une vigilance intacte"),    this, SLOT( ATCD_setVigilanceOn()),   ALT+Key_V );
        pThemePopup->insertItem( Theme::getIcon("16x16/ald_off.png"),      tr("Sans rapport ALD ou activité sportive"),    this, SLOT( ATCD_setAldOff()),        CTRL+Key_O );
        }
      pThemePopup->exec(pos);
      delete pThemePopup;
      m_pQListViewItem = 0;
     }
     break;
  case LV_TYPE:         // med/chir
     { pQPopupMenu   = new ThemePopup(this, "MyPopupMenu" );
       Add_popMenu_ATCD_Type(pQPopupMenu);
       pQPopupMenu->exec(QCursor::pos());
       delete pQPopupMenu;
     }
     break;
  case LV_DFIN:         // etat passe transitoire
     { ThemePopup *pThemePopup   = new ThemePopup((QWidget*)this->parent(), "MyPopupMenu" );
       pThemePopup->insertItem( Theme::getIconListDateTime(),        tr("Modifier la date de fin de cet Ant\303\251c\303\251dent"),       this, SLOT( ATCD_MenuActionSetDateFin()),        CTRL+Key_G );
       pThemePopup->insertItem( Theme::getIcon("16x16/timeDel.png"), tr("Effacer la date de fin de cet Ant\303\251c\303\251dent"),        this, SLOT( ATCD_MenuActionDelDateFin()),        CTRL+Key_G );
       pThemePopup->exec(QCursor::pos());
       delete pThemePopup;
     }
     break;
  case LV_COMM:         // commentaire
     { ATCD_MenuActionSetCommentaire();
     }
     break;
  case LV_DDEB:         // date
     { ThemePopup *pThemePopup   = new ThemePopup((QWidget*)this->parent(), "MyPopupMenu" );
       pThemePopup->insertItem( Theme::getIconListDateTime(),        tr("Modifier la date de d\303\251but de cet Ant\303\251c\303\251dent"),       this, SLOT( ATCD_MenuActionSetDateDeb()),        CTRL+Key_G );
       pThemePopup->insertItem( Theme::getIcon("16x16/timeDel.png"), tr("Effacer la date de d\303\251but de cet Ant\303\251c\303\251dent"),        this, SLOT( ATCD_MenuActionDelDateDeb()),        CTRL+Key_G );
       pThemePopup->exec(QCursor::pos());
       delete pThemePopup;
     }
     break;
  case LV_ALSP:         // ALD
     { ThemePopup *pThemePopup   = new ThemePopup(this, "MyPopupMenu" );
       pThemePopup->insertItem( Theme::getIcon("16x16/ald_on.png"),       tr("En rapport avec une pathologie ALD"),      this, SLOT( ATCD_setAldOn()),         CTRL+Key_A );
       pThemePopup->insertItem( Theme::getIcon("16x16/sport_on.png"),     tr("En rapport avec une activité sportive"),   this, SLOT( ATCD_setSportOn()),       ALT+Key_S );
       pThemePopup->insertItem( Theme::getIcon("16x16/vigilance_on.png"), tr("En rapport avec une vigilance intacte"),   this, SLOT( ATCD_setVigilanceOn()),   ALT+Key_V );
       pThemePopup->insertItem( Theme::getIcon("16x16/ald_off.png"),      tr("Sans rapport avec une pathologie ALD"),    this, SLOT( ATCD_setAldOff()),        SHIFT+Key_A );
       pThemePopup->exec(QCursor::pos());
       delete pThemePopup;
     }
     break;
  case LV_CODE:          // code de l'antecedent
     {listView_ATCD_doubleClicked( qlistViewItem);
     }
     break;

 }
}
//------------------------------------ ATCD_setAldOn --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur en rapport avec les ALD
*/
void FormRubTerrain::ATCD_setAldOn()
{ATCD_setAldOnOff(tr("ALD"));
}
//------------------------------------ ATCD_setSportOn --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur sans rapport avec les ALD
*/
void FormRubTerrain::ATCD_setSportOn()
{ATCD_setAldOnOff(tr("Sport"));
}
//------------------------------------ ATCD_setVigilanceOn() --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur en rapport avec une vigilaance non altérée
*/
void FormRubTerrain::ATCD_setVigilanceOn()
{ATCD_setAldOnOff(tr("Vigilance"));
}
//------------------------------------ ATCD_setAldOff --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur sans rapport avec les ALD
*/
void FormRubTerrain::ATCD_setAldOff()
{ATCD_setAldOnOff(tr(""));
}

//------------------------------------ ATCD_setAldOn --------------------------------------------------
/*! \brief positionne les ATCD selectionnes dans la listview des ATCD sur en/sans rapport avec les ALD selon le parametre state "":"ALD"
*/
void FormRubTerrain::ATCD_setAldOnOff(const QString &state)
{
  //===
  if (m_IsModifiable==0)     return;
  if (!m_pAtcd_Code)         return;
  if (m_pQListViewItem == 0) return;

  m_pQListViewItem->setSelected (TRUE);   // reselectionner l'item au dessus duquel se retrouve la souris
  m_pC_ListViewATCDManager->ATCD_setAldOnOff(state);
  // if (m_pAtcd_Code) connect(m_pAtcd_Code, SIGNAL( ATCD_Changed() ), this, SLOT( ATCD_HaveChanged() ));
  m_pQListViewItem         = 0;
  m_pAtcd_Element_Selected = 0;
  //................ mettre a jour les datas de la rubrique ...................
  QString stringDST;
  FormToData(stringDST);
  if (m_LastRub != -1)
     {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
     }
}

//------------------------------------ listView_ATCD_doubleClicked -------------------------------------------
/*! \brief Fonction qui appelle Atcd_Code::modifyAtcd() pour la modification de l'ant\303\251c\303\251dent s\303\251lectionn\303\251. Tr\303\250s efficace !
*/
void FormRubTerrain::listView_ATCD_doubleClicked( QListViewItem * pQListViewItem)
{//CMoteurBase::GotoDebug();
 if (m_IsModifiable==0) return;
 if (pQListViewItem==0) {ATCD_MenuActionNewFromFavori();return;}
 CPrtQListViewItem* pCPrt = (CPrtQListViewItem*) pQListViewItem;
 m_pAtcd_Element_Selected = m_pAtcd_Code->getAtcd_byID( pCPrt->get_ID_Rublist().toInt() );
 // Appel le Widget d'ajout de l'ATCD
 if (!m_pAtcd_Element_Selected) return;
 m_pAtcd_Code->modifyAtcd((QWidget*)this, m_pAtcd_Element_Selected, Atcd_Code::sendModifMessage);
 m_pQListViewItem = 0;
  //................ mettre a jour les datas de la rubrique ...................
 QString stringDST;
 FormToData(stringDST);
 if (m_LastRub != -1)
    {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
    }
 }

//------------------------------------ ATCD_MenuActionNewFromFavori --------------------------------------------------
/*! \brief Ajoute un ATCD par le biais des favoris de Atcd_Code de CApp
*/
void FormRubTerrain::ATCD_MenuActionNewFromFavori()
{
 if (m_IsModifiable==0) return;
 if (!m_pAtcd_Code)     return;
  // Appel le Widget d'ajout de l'ATCD
 m_pAtcd_Code->addATCD_CIM10(3);     // thesaurus
 m_pQListViewItem = 0;
  //................ mettre a jour les datas de la rubrique ...................
 QString stringDST;
 FormToData(stringDST);
 if (m_LastRub != -1)
    {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
    }
}

//------------------------------------ ATCD_MenuActionNewCIM10 --------------------------------------------------
/*! \brief Ajoute un ATCD CIM 10 par le biais de Atcd_Code de CApp
*/
void FormRubTerrain::ATCD_MenuActionNewCIM10()
{
 if (m_IsModifiable==0) return;
 if (!m_pAtcd_Code)     return;
  // Appel le Widget d'ajout de l'ATCD
 m_pAtcd_Code->addATCD_CIM10(0);
 m_pQListViewItem = 0;
  //................ mettre a jour les datas de la rubrique ...................
 QString stringDST;
 FormToData(stringDST);
 if (m_LastRub != -1)
    {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
    }
}
//------------------------------------ ATCD_MenuActionNewCISP --------------------------------------------------
/*! \brief Ajoute un ATCD CISP par le biais de Atcd_Code de CApp
*/
void FormRubTerrain::ATCD_MenuActionNewCISP()
{if (m_IsModifiable==0) return;
 if (!m_pAtcd_Code)     return;
  // Appel le Widget d'ajout de l'ATCD
 m_pAtcd_Code->addATCD_CIM10(4);
 m_pQListViewItem = 0;
  //................ mettre a jour les datas de la rubrique ...................
 QString stringDST;
 FormToData(stringDST);
 if (m_LastRub != -1)
    {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
    }
}

//------------------------------------ ATCD_MenuActionSetDateDeb --------------------------------------------------
/*! \brief Modifie la date de l'ATCD s\303\251lectionn\303\251 par le biais de la classe Atcd_Code
*/
void FormRubTerrain::ATCD_MenuActionSetDateDeb()
{ if (m_pQListViewItem == 0)       return;
  if (m_IsModifiable==0)           return;
  if (!m_pAtcd_Code)               return;
  if (!m_pAtcd_Element_Selected)   return;
  m_pAtcd_Code->modifyDateDeb((QWidget*)this, m_pAtcd_Element_Selected);
  // L'affichage se remet a jour grace au signal de Atcd_Code.
}
//------------------------------------ ATCD_MenuActionSetDateFin --------------------------------------------------
/*! \brief Modifie la date de l'ATCD s\303\251lectionn\303\251 par le biais de la classe Atcd_Code
*/
void FormRubTerrain::ATCD_MenuActionSetDateFin()
{ if (m_pQListViewItem == 0)       return;
  if (m_IsModifiable==0)           return;
  if (!m_pAtcd_Code)               return;
  if (!m_pAtcd_Element_Selected)   return;
  m_pAtcd_Code->modifyDateFin((QWidget*)this, m_pAtcd_Element_Selected);
  // L'affichage se remet a jour grace au signal de Atcd_Code.
}
//------------------------------------ ATCD_MenuActionDelDateDeb --------------------------------------------------
/*! \brief Efface la date de l'ATCD s\303\251lectionn\303\251 par le biais de la classe Atcd_Code
*/
void FormRubTerrain::ATCD_MenuActionDelDateDeb()
{
  if (m_IsModifiable==0)     return;
  if (!m_pAtcd_Code)         return;
  if (m_pQListViewItem==0)   return;

 m_pQListViewItem->setSelected (TRUE);   // reselectionner l'item au dessus duquel se retrouve la souris
 m_pC_ListViewATCDManager->ATCD_MenuActionDelDateDeb();
 m_pQListViewItem         = 0;
 m_pAtcd_Element_Selected = 0;
 //................ mettre a jour les datas de la rubrique ...................
 QString stringDST;
 FormToData(stringDST);
 if (m_LastRub != -1)
    {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
    }
}
//------------------------------------ ATCD_MenuActionDelDateFin --------------------------------------------------
/*! \brief Efface la date de l'ATCD s\303\251lectionn\303\251 par le biais de la classe Atcd_Code
*/
void FormRubTerrain::ATCD_MenuActionDelDateFin()
{
  if (m_IsModifiable==0)     return;
  if (!m_pAtcd_Code)         return;
  if (m_pQListViewItem==0)   return;

 m_pQListViewItem->setSelected (TRUE);   // reselectionner l'item au dessus duquel se retrouve la souris
 m_pC_ListViewATCDManager->ATCD_MenuActionDelDateFin();
 m_pQListViewItem         = 0;
 m_pAtcd_Element_Selected = 0;
 //................ mettre a jour les datas de la rubrique ...................
 QString stringDST;
 FormToData(stringDST);
 if (m_LastRub != -1)
    {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
    }
}
//------------------------------------ ATCD_MenuActionNewAllergie ---------------------------------------------------
/*! \brief Ajoute un ATCD allergique par le biais de Atcd_Code de CApp
*/
void FormRubTerrain::ATCD_MenuActionNewAllergie()
{ if (m_IsModifiable==0) return;
  if (!m_pAtcd_Code) return;
  m_pAtcd_Code->addATCD_Allergie();
}
//------------------------------------ ATCD_MenuActionNewTexteLibre --------------------------------------------------
/*! \brief Ajoute un ATCD texte libre par le biais de Atcd_Code de CApp
*/
void FormRubTerrain::ATCD_MenuActionNewTexteLibre()
{ if (m_IsModifiable==0)  return;
  if (!m_pAtcd_Code)      return;
  m_pAtcd_Code->addATCD_Textuel((QWidget*)this);
}

//------------------------------------ ATCD_MenuActionDel --------------------------------------------------
/*! \brief Efface un ATCD par le biais de Atcd_Code de CApp
*/
void FormRubTerrain::ATCD_MenuActionDel()
{if (m_pQListViewItem == 0)       return;
 if (m_pAtcd_Element_Selected==0) return;
 if (m_IsModifiable==0)           return;
 m_pQListViewItem->setSelected (TRUE);   // reselectionner l'item au dessus duquel se retrouve la souris
 m_pAtcd_Code->eraseAtcd(m_pAtcd_Element_Selected);
 // L'affichage se met a jour automatiquement grace au signal ATCD_Changed de Atcd_Code
}

//------------------------------------ ATCD_MenuActionMultiDel --------------------------------------------------
/*! \brief Efface un ATCD par le biais de Atcd_Code de CApp
*/
void FormRubTerrain::ATCD_MenuActionMultiDel()
{if (m_pQListViewItem == 0)       return;
 if (m_IsModifiable==0)           return;
 m_pC_ListViewATCDManager->ATCD_MenuActionMultiDel();
 if (G_pCApp->m_pAtcd_Code)
    {G_pCApp->m_pAtcd_Code->updateFromListViewTerrain( listView_ATCD );
     G_pCApp->m_pAtcd_Code->createAtcdListView();
    }
 m_pQListViewItem         = 0;
 m_pAtcd_Element_Selected = 0;
 //................ mettre a jour les datas de la rubrique ...................
 QString stringDST;
 FormToData(stringDST);
 if (m_LastRub != -1)
    {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
    }
}

//------------------------------------ ATCD_MenuActionSetFamilleGenre --------------------------------------------------
/*! \brief Modifie la Famille et Genre d'un ATCD par le biais de la classe Atcd_Code.
*/void FormRubTerrain::ATCD_MenuActionSetFamilleGenre()
{ m_pC_ListViewATCDManager->ATCD_MenuActionSetFamilleGenre();
  if (m_pQListViewItem == 0)      return;
  if (m_IsModifiable==0)          return;
  if (!m_pAtcd_Code)              return;
  m_pQListViewItem         = 0;
  m_pAtcd_Element_Selected = 0;
  //................ mettre a jour les datas de la rubrique ...................
  QString stringDST;
  FormToData(stringDST);
  if (m_LastRub != -1)
     {m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
     }
}

//------------------------------------ ATCD_MenuActionSetCommentaire --------------------------------------------------
void FormRubTerrain::ATCD_MenuActionSetCommentaire()
{if (m_pQListViewItem == 0)     return;
 if (m_IsModifiable==0)         return;
 if (!m_pAtcd_Code)             return;
 if (!m_pAtcd_Element_Selected) return;
 m_pAtcd_Code->changeCommentaire(m_pAtcd_Element_Selected);
 // L'affichage se remet a jour grace au signal de Atcd_Code.
}


//------------------------------------ pushButtonNewATCD_clicked --------------------------------------------------
void FormRubTerrain::pushButtonNewATCD_clicked()
{if (m_IsModifiable==0) return;
 listView_ATCD_contextMenuRequested(0, QCursor::pos(), 0 );
}


//------------------------------------ pushButtonDellAllATCD_clicked --------------------------------------------------
void FormRubTerrain::pushButtonDellAllATCD_clicked()
{if (m_IsModifiable==0) return;
 QStringList optList;
 optList.append(tr ("Effacer les ant\303\251c\303\251dents s\303\251lectionn\303\251s"));
 optList.append(tr ("Effacer tous les ant\303\251c\303\251dents"));
 QString result = G_pCApp->DoPopupList(optList);
 if (result.length() == 0) return;

 if (result.find("Effacer tous")!= -1)
    {int ret = QMessageBox::warning ( this, tr ("Effacer tous les ant\303\251c\303\251dents ") + QString(PROG_NAME),
                                        tr ("Voulez vous vraiment effacer\r\n"
                                            "toutes les donn\303\251es des ant\303\251c\303\251dents\r\n"
                                            "CETTE ACTION EST IRREVERSIBLE"),
                                        tr ("&Effacer"), tr("Annu&ler"), 0,
                                  1, 1 );
     if (ret >=1 ) return;
     //................. parcourir tous les items et les selectionner ............
     QListViewItemIterator it( listView_ATCD );
     while ( it.current() )
       {CPrtQListViewItem* pCPrt = (CPrtQListViewItem*)it.current();
        pCPrt->setSelected(TRUE);
        ++it;
       }
    }
 //........... effacer la selection ....................
 m_pQListViewItem = listView_ATCD->currentItem ();
 ATCD_MenuActionMultiDel();
}

//------------------------------------ StringToEscapeString --------------------------------------------------
void FormRubTerrain::StringToEscapeString(QString &str)
{str.replace("\\","\\\\");
 str.replace("\r","\\\r");
 str.replace("\n","\\\n");
 str.replace("[","\\[");
 str.replace("]","\\]");
 str.replace(",","\\,");
 str.replace(";","\\;");
}
//------------------------------------ EscapeStringToString --------------------------------------------------
void FormRubTerrain::EscapeStringToString(QString &str)
{ str.replace("\\,",",");
  str.replace("\\;",";");
  str.replace("\\[","[");
  str.replace("\\]","]");
  str.replace("\\\r","\r");
  str.replace("\\\n","\n");
  str.replace("\\\\","\\");
}

//------------------------------------ checkBoxGrossesse_clicked --------------------------------------------------
void FormRubTerrain::checkBoxGrossesse_clicked()
/*! \brief slot associ\303\251 au checkbox grossesse -> d\303\251riv\303\251 vers Atcd_Code */
{if (!m_pAtcd_Code) return;
 if (m_IsModifiable==0) return;
 if (checkBoxGrossesse->isChecked ())
    {//QPushButton_DDR->show();
     //textLabelDDR->show();
         QPushButton_DDR_clicked();
    }
 else
    {QPushButton_DDR->hide();
     textLabelDDR->hide(); //qDebug("checkBoxGrossesse_clicked() hide");
    }
 m_pAtcd_Code->setGrossesse(checkBoxGrossesse->isChecked());
 m_IsModified |= 4;
}

//------------------------------------ checkBoxAllaitement_clicked --------------------------------------------------
/*! \brief slot associ\303\251 au checkbox allaitement -> d\303\251riv\303\251 vers Atcd_Code */
void FormRubTerrain::checkBoxAllaitement_clicked()
{if (!m_pAtcd_Code) return;
 if (m_IsModifiable==0) return;
 m_pAtcd_Code->setAllaitement(checkBoxAllaitement->isChecked());
 m_IsModified |= 8;
}


//------------------------------------ QPushButton_DDR_clicked --------------------------------------------------
void FormRubTerrain::QPushButton_DDR_clicked()
{if (!m_pAtcd_Code) return;
 if (m_IsModifiable==0) return;
 m_pAtcd_Code->changeDDR(this);
 if (m_pAtcd_Code->m_Grossesse) m_IsModified |= 16;
}

//--------------------------------- Current_RubList_ID --------------------------------------------------
int FormRubTerrain::Current_RubList_ID()
{CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (pCPrtQListBoxItem) return pCPrtQListBoxItem->GetPk().toInt();
 return -1;
}

//--------------------------------- Current_RubList_Iterator --------------------------------------------------
RUBREC_LIST::iterator FormRubTerrain::Current_RubList_Iterator()
{int id = Current_RubList_ID();
 if (id==-1) return  m_pRubList->end();
             return  m_pRubList->at(id);
}

//--------------------------------- SetCurentDocByPrimkey --------------------------------------------------
/*! \brief d\303\251finit le document a afficher par sa cl\303\251 primaire.
 *  \sa DrTux::Sign_ActiverDocument , Dock_Menu::m_ListView_AccesRapide_clicked ,
*/
int FormRubTerrain::SetCurentDocByPrimkey(const QString &primKey)
{QListBox *pQListBox = comboBox_RubName->listBox();
 int last            = comboBox_RubName->count ();
 int i;
 for (i=0; i<last; i++)
     {CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) pQListBox->item ( i );
     if (pCPrtQListBoxItem->GetUser() == primKey)
        {comboBox_RubName->setCurrentItem(i);
         comboBox_RubName_activated(i);
         return i;
        }
     }
 return -1; // pas trouv\303\251
}

//--------------------------------- pushButtonDelete_clicked --------------------------------------------------
void FormRubTerrain::pushButtonDelete_clicked()
{if (m_IsModifiable==0) return;

 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem) return;
 int id  =  pCPrtQListBoxItem->GetPk().toInt();

 RUBREC_LIST::iterator it = m_pRubList->at(id);
 //........................... si rubrique qui etait avant celle a afficher, a ete modifi\303\251e ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if ((*it).m_State & RUB_IS_TO_DELETE)
    {m_pCMoteurBase->RubList_SetItemNotDeleted(m_pRubList, id);
    }
 else
    {m_pCMoteurBase->RubList_SetItemDeleted(m_pRubList, id);
    }
 if ((*it).m_State == RUB_IS_TO_DELETE) {pushButtonDelete->setIconSet (Theme::getIconIsDeleted()); pushButtonDelete->setDown(TRUE);}
 else                                   {pushButtonDelete->setIconSet (Theme::getIconDelete());    pushButtonDelete->setDown(FALSE);}
 m_IsModified |= 4;
 ButtonSaveDisplay(G_pCApp->m_Droits);

}

//--------------------------------- pushButtonRubDateChange_clicked --------------------------------------------------
void FormRubTerrain::pushButtonRubDateChange_clicked()
{if (m_IsModifiable==0)                  return;
 if (! pushButtonNew->isVisible() )      return;

 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem) return;

 int                  id    =  pCPrtQListBoxItem->GetPk().toInt();
 //QString             date   =  pCPrtQListBoxItem->GetData_2();
 RUBREC_LIST::iterator it   =  m_pRubList->at(id);
 DlgDateNom *dlg = new DlgDateNom(this,"DateNOm_Dlg",TRUE);        // TRUE Pour modal
 if (dlg ==0)                            return ;
 dlg->initDialog( &(*it).m_Date, &(*it).m_Libelle, &(*it).m_SubType);

 if ( dlg->exec() == QDialog::Accepted )
    { //................... remettre a jour le comboBox ................................
      pCPrtQListBoxItem->SetDate((*it).m_Date);
      pCPrtQListBoxItem->setText((*it).m_Date.left(10) + " " +(*it).m_Libelle );
      //................... mettre le document en modified (en replacant les data ds rubrique liste) ......................
      if (m_LastRub != -1)
         {QString stringDST;
          FormToData(stringDST);
          m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
         }
     //................... mettre le document en modified (en replacant les data ds rubrique liste) ......................
     m_IsModified = 1;
     ButtonSaveDisplay(G_pCApp->m_Droits);
    }
 delete dlg;
}







