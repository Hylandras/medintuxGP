/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

//#include "Ez_edit.xpm"
#include <qdragobject.h>
#include <qprocess.h>
#include <qeventloop.h> 
#include "drtux.h"

//=============================================== DEFINE =========================================================

#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))


//=============================================== PROTOTYPRS ========================================================
extern void RtfToQtextEdit_ASCII(const char *txt, long len, QString &stringDST);

//=============================================== IMPLEMENTTION =====================================================

//--------------------------------- Init -------------------------------------------------------------
void FormRubPrescription::init()
{m_IsModifiable  = 0;
 m_pMyEditText   = 0;
 m_LastRub       = -1;
 pushButtonSave->setIconSet  (Theme::getIconSave());
 pushButtonPrint->setIconSet (Theme::getIconPrint());
 pushButtonVidal->setIconSet (G_pCApp->m_IconTherapeutique);
 pushButtonPlus->setIconSet  (Theme::getIconRightArrow());
 //........................ initialiser les popups ...............................
 m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 m_pQPopupMenu      = 0;
 if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"bmc"))           m_IsBaseMedEnabled = TRUE;
 else                                                              m_IsBaseMedEnabled = FALSE;
 connect(textLabel_NbRub, SIGNAL( pressed() ), this, SLOT( On_buttonDragClicked() ));
#ifdef Q_OS_MACX
    pushButtonSave->setFlat (TRUE);
	pushButtonDelete->setFlat (TRUE);
	pushButtonVidal->setFlat (TRUE);
    pushButtonRubDateChange->setFlat (TRUE);
    pushButtonPlus->setFlat (TRUE);
    pushButtonPrint->setFlat (TRUE);

	textLabel_NbRub->setFlat (TRUE);
	textLabel_NbRub->setMinimumSize (50, 22);
	textLabel_NbRub->setFont(G_pCApp->font());
#endif
}
 //--------------------------------- pushButtonPlus_clicked -------------------------------------------------------------
void FormRubPrescription::pushButtonPlus_clicked()
{pushButtonPlus->setIconSet (Theme::getIconDownArrow());
 int id                     = Current_RubList_ID();
 if (id<= -1)    return;
 RUBREC_LIST::iterator it   = m_pRubList->at(id);
 //.................. creer liste des items du menu .......................
 QStringList popItem;
 if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"orr") == FALSE && (*it).m_Type.toInt()!=TYP_ORDO_CALC)
    {popItem.append(tr("Pas d'action prévue pour ce contexte (droits utilisateur insuffisants)"));
    }
 if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"orr"))
    {popItem.append(tr("Renouveler ce document à la date du jour"));
     popItem.append(tr("Renouveler ce document à une date choisie"));
    }
 if ((*it).m_Type.toInt()==TYP_ORDO_CALC)
    {popItem.append(tr("---------"));
     popItem.append(tr("Placer cette ordonnance comme traitement de fond"));
     
     if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits,"orr") ) 
        { popItem.append(tr("Renouveler le traitement de fond"));
	  popItem.append(tr("---------"));
          popItem.append(tr("Voir la liste de tous les traitements en cours (intercurrents et de fond)"));
        }
    }
 QString popRet       =   DoPopupList(popItem);
 QDateTime  qdt       =   QDateTime::currentDateTime();

 //.................... action selon retour menu ..................................
 if (popRet.find(tr("Renouveler le traitement de fond"))!=-1 )
    {emit Sign_Renouveler_TTT_Fond();
    }
 else if (popRet.find(tr("Placer"))!=-1)
    {emit Sign_PutPrescriptionInTerrain(it);
    }
 else if (popRet.find(tr("date du jour"))!=-1)
    {emit Sign_DuplicateData(it, qdt, (*it).m_Libelle, (*it).m_SubType);
    }
 else if (popRet.startsWith(tr("Voir")))
    {emit Sign_CreateAllCurrentsTTT();  
    }
 else if (popRet.find(tr("date choisie"))!=-1)
    {DlgDateNom *dlg = new DlgDateNom(this,"Therapeutique_Dial",TRUE);   // TRUE Pour modal
     if (dlg ==0)                            return ;
     QString    date_time  = qdt.toString("yyyy-MM-dd hh:mm:ss");//1999-10-08 00:00:00
     QString libelle       = (*it).m_Libelle;
     QString subType       = (*it).m_SubType;
     dlg->initDialog( &date_time, &libelle, &subType);
     if ( dlg->exec() != QDialog::Accepted )
        {delete dlg;
         pushButtonPlus->setIconSet (Theme::getIconRightArrow());
         return;
        }
     QDate  date ( atoi(date_time.mid(0,4)),  atoi(date_time.mid(5,2)),  atoi(date_time.mid(8,2))  );
     QTime  time ( atoi(date_time.mid(11,2)), atoi(date_time.mid(14,2)), atoi(date_time.mid(17,2)) );
     qdt.setDate (date );
     qdt.setTime (time );
     delete dlg;
     emit Sign_DuplicateData(m_pRubList->at(id), qdt, libelle, subType);
    }
 pushButtonPlus->setIconSet (Theme::getIconRightArrow());
}

//--------------------------------- On_buttonDragClicked ------------------------------------------------------
// connect(textLabel_NbRub, SIGNAL( clicked() ), this, SLOT( On_buttonDragClicked() ));
//   virtual void          On_buttonDragClicked();
void  FormRubPrescription::On_buttonDragClicked()
{   RUBREC_LIST::iterator it = Current_RubList_Iterator();
    if ( it != m_pRubList->end() ) (*it).dragRubrique(this);
}


//------------------------------------------- pushButtonByPrincipe_clicked ------------------------------------------------------------------
void FormRubPrescription::pushButtonByPrincipe_clicked()
{
}

//----------------------------------------------------- pushButton_Ok_clicked ----------------------------------------------------------------------------------
void FormRubPrescription::listView_Produits_rightButtonClicked( QListViewItem *, const QPoint &, int )
{pushButtonByPrincipe_clicked();
}

//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString FormRubPrescription::DoPopupList(QStringList &list)
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
 connect( m_pQPopupMenu, SIGNAL( activated( int)), this,          SLOT(OnMenuActivated( int))  );

 m_pQPopupMenu->exec( QCursor::pos() /* mapToGlobal (  QPoint ( x, y) )*/ );
 delete  m_pQPopupMenu;
 m_pQPopupMenu = 0;
 return  m_RetPopupItem;
}

//--------------------------------- OnMenuActivated  -------------------------------------------------------------
void FormRubPrescription::OnMenuActivated (int id)
{if (id != -1 && m_pQPopupMenu)
    {m_CurrentPopupItem = m_pQPopupMenu->text(id);
    }
}

//--------------------------------- GetPopupItem  -------------------------------------------------------------
void FormRubPrescription::GetPopupItem ()
{m_RetPopupItem  = m_CurrentPopupItem;
}

//--------------------------------- SetModifiableState -----------------------------------------------------------------------
void  FormRubPrescription::SetModifiableState(int state)
{m_IsModifiable = state;
 if (m_IsModifiable)
    {pushButtonSave->show();
     pushButtonRubDateChange->show();
     pushButtonDelete->show();
     if (comboBox_RubName->count())
       {RUBREC_LIST::iterator it =  Current_RubList_Iterator();
        if ((atoi((*it).m_Type) == TYP_ORDO_CALC) && m_IsBaseMedEnabled)  pushButtonVidal->show();
        else                                                              pushButtonVidal->hide();
       }
    }
 else
    {pushButtonSave->hide();
     pushButtonRubDateChange->hide();
     pushButtonDelete->hide();
     pushButtonVidal->hide();
    }
}

//--------------------------------- InitText -------------------------------------------------------------------------
// ACTION: ratache à la variable membre: MyEditText *m_pMyEditText  l'editeur de texte passé en entrée
//         et initialise cet editeur sur les valeurs adéquates
//         Il est imperatif d'apeler cette fonction apres le constructeur de: FormRubObservation
void FormRubPrescription::InitText(MyEditText *pMyEditText)
{if (pMyEditText==0)     return;
 m_pMyEditText = pMyEditText;
 m_pMyEditText->setGeometry (QRect  (0,100,100,400));
 m_pMyEditText->setTextFormat(RichText);
}
//--------------------------------- FormPatientList_destroyed -------------------------------------------------------------
void FormRubPrescription::FormPatientList_destroyed( QObject * )
{
}

//--------------------------------- SetData-------------------------------------------------------------
int FormRubPrescription::SetData()
{QString qstr;
 if (m_pCMoteurBase==0) return 0;
 //................. mettre à jour le texte ............................
 long nb = initComboListOrdo();
 if (nb>0)
    {show();
     if (m_pMyEditText!=0) {m_pMyEditText->m_State = 1;
                            m_pMyEditText->show();
                           }
     comboBox_RubName->setCurrentItem (nb-1);            // se mettre sur dernière prescription
     comboBox_RubName_activated( nb-1);
    }
 else
    {if (m_pMyEditText!=0) {m_pMyEditText->m_State = 0;
                            m_pMyEditText->hide();
                            m_pMyEditText->setText("");
                           }
     textLabel_UserName->setText("");
     hide();
    }
 textLabel_NbRub->setText(qstr.setNum(nb));
 return nb;
}

//--------------------------------- initComboListOrdo -------------------------------------------------------------

long  FormRubPrescription::initComboListOrdo(void)
{return m_pCMoteurBase->initConboBoxWithRubList(m_pRubList, comboBox_RubName, QString::number(TYP_PRESCRIPTION) );
}

//--------------------------------- comboBox_RubName_highlighted -------------------------------------------------------------
void FormRubPrescription::comboBox_RubName_highlighted( int item)
{comboBox_RubName_activated( item );
}


//--------------------------------- comboBox_RubName_activated -------------------------------------------------------------
void FormRubPrescription::comboBox_RubName_activated( int item)
{
 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( item );
 if (pCPrtQListBoxItem == 0) return;
 int     id  =  pCPrtQListBoxItem->GetPk().toInt();

 QByteArray           data;
 long                 len;
 int                  typ = 0;
 QString              stringDST;
 QString              stringStruct;
 char                *ptr = 0;
 int                    f = 0;
 RUBREC_LIST::iterator it;
 bool mustBeReadOnly = false;
 //........................... si rubrique qui etait avant celle a afficher (m_LastRub), a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if (m_IsModifiable && m_pMyEditText && m_pMyEditText->isModified() && m_LastRub != -1)
    {it = m_pRubList->at(m_LastRub);
     PutDataInRubList(it);
    }
 //........................... retirer le texte de la nouvelle rubrique a afficher ..............................
 //
 it        = m_pRubList->at(id);
 int ret   = m_pCMoteurBase->GetDataFromRubList(data, it);
 m_LastRub = id;                   // c'est maintenant la dernière affichée
 if (ret == RUB_READ_ON_DISK)      // en fait cela veut dire que ça été lu sur le disque
    {ptr        = data.data();
     len        = data.size();
     if (ptr && len>6)
        {
         if (strncmp(ptr, "<?xml", 5)==0)
            {if (CGestIni::IsUtf8( ptr, len ))
                { CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(QString::fromUtf8 ( ptr ), &stringDST,0);
                }
             else CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(data.data(), &stringDST,0);
             if (m_pMyEditText!=0)
                {m_pMyEditText->setText(stringDST);
                 m_pMyEditText->setModified (FALSE);
                }
            }
         else if (strncmp(ptr, "<html>", 6))
            {CHtmlTools::RtfToQtextEdit_HTML(ptr+4, len-4, stringDST,1);
             if (m_pMyEditText!=0)
                {m_pMyEditText->setText(stringDST);
                 m_pMyEditText->setModified (FALSE);
                }
            }
         else
            {if (m_pMyEditText!=0)
                {m_pMyEditText->setText(ptr);
                 m_pMyEditText->setModified (FALSE);
                }
            }
         f = 1;
        }
    }
 else if (ret == RUB_READ_IN_LIST)   // en fait cela veut dire que ça été lu dans les données de liste
    {ptr      = data.data();
     typ      = atoi((*it).m_Type);
     if (ptr && m_pMyEditText!=0)
        {if (typ==TYP_ORDO_CALC)
            {if (CGestIni::IsUtf8( ptr , data.size()))
                { CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(QString::fromUtf8 ( ptr ), &stringDST,0);
                }
             else CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(data.data(), &stringDST,0);
             m_pMyEditText->setText(stringDST);
            }
         else
            {m_pMyEditText->setText(ptr);
            }
         m_pMyEditText->setModified (FALSE);
        }
     f = 1;
    }
if ( strstr(ptr, "<Posologie_LAP_List>") ) // data.find("<Posologie_LAP_List>") != -1 )
   { mustBeReadOnly = true;
   }

 //..................... signaler le changement de rubrique ............................
 emit Sign_RubChanged(it);
 //.................................................... régler droits utilisateurs ......................................................................
 //
 QString user              = (*it).m_User;
 QString doc_sign_user     = (*it).m_SignUser;
 QString doc_Type          = (*it).m_Type;

 if (G_pCApp->m_User != doc_sign_user)  // si l'utilisateur actuel n'est pas le même que le signataire du document vérifier les droits
    {QString friend_droits     = G_pCApp->m_pCMoteurBase->PermsUserIsThisUserFriendFromName(  doc_sign_user , G_pCApp->m_User);
     //.............. recuperer le prefixage de droit du document ..............................................................
     //               ce prefix sera suffixé par 'v' pour voir 'c' pour créer 'm' pour modifier 'p' pour imprimer  'r' pour renouveler
     QString prefix = G_pCApp->GetMap_Prefix_Droits(doc_Type);               //
     if (G_pCApp->IsThisDroitExist(friend_droits, prefix+"v") ) {                                                                                  pushButtonDelete->setEnabled(TRUE);  pushButtonVidal->setEnabled(TRUE);  pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate());}
     else                                                       { if (m_pMyEditText!=0) m_pMyEditText->setText(G_pCApp->m_MessVisuDocNotPossible); pushButtonDelete->setEnabled(FALSE); pushButtonVidal->setEnabled(FALSE); pushButtonRubDateChange->setIconSet(Theme::getIconCadenas());}
     if (G_pCApp->IsThisDroitExist(friend_droits, prefix+"m") ) { m_pMyEditText->setReadOnly (FALSE );pushButtonDelete->setEnabled(TRUE); pushButtonVidal->setEnabled(TRUE); pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate());}
     else                                                       { m_pMyEditText->setReadOnly (TRUE ); pushButtonDelete->setEnabled(FALSE);pushButtonVidal->setEnabled(FALSE);pushButtonRubDateChange->setIconSet(Theme::getIconCadenas());}
     if (G_pCApp->IsThisDroitExist(friend_droits, prefix+"p") ) { pushButtonPrint->setEnabled(TRUE); }
     else                                                       { pushButtonPrint->setEnabled(FALSE);}
     if (G_pCApp->IsThisDroitExist(friend_droits,"bmc") )       { pushButtonVidal->setEnabled(TRUE); }
     else                                                       { pushButtonVidal->setEnabled(FALSE);}
    }
 else
    {m_pMyEditText->setReadOnly (FALSE );pushButtonDelete->setEnabled(TRUE); pushButtonPrint->setEnabled(TRUE);pushButtonRubDateChange->setIconSet(Theme::getIconChangeDate());pushButtonVidal->setEnabled(TRUE);
    }

 if ( mustBeReadOnly ) 
    { m_pMyEditText->setReadOnly(true);     // les ordonnances securisees ne peuvent etre modifiees que dans le LAP
      m_pMyEditText->setPaper ( QBrush ( QColor("#fffef1"), Qt::SolidPattern   )  );
    }
 textLabel_UserName->setText(user + "::" + doc_sign_user);
 //..................... si ordo à effacer afficher bouton effacement ......................................................
 if ((*it).m_State == RUB_IS_TO_DELETE) {pushButtonDelete->setIconSet (Theme::getIconIsDeleted()); pushButtonDelete->setDown(TRUE);}
 else                                   {pushButtonDelete->setIconSet (Theme::getIconDelete());    pushButtonDelete->setDown(FALSE);}
 if (m_pMyEditText!=0 && f==0) m_pMyEditText->setText("");
 //.................... si ordo calculable afficher l'icone Vidal ..........................................................
 //  si l'on met TYP_ORDO_CALC selon les compils ce sera pas traduit comme une chaine
 if ((doc_Type == "20020100"||doc_Type == "20020150") && m_IsModifiable && m_IsBaseMedEnabled ){ pushButtonVidal->show();}
 else                                                                                          { pushButtonVidal->hide();}
 
}

//--------------------------------- IfModified_SaveInRubList -------------------------------------------------------------
int FormRubPrescription::IfModified_SaveInRubList()
{
 if (m_IsModifiable==0)  return FALSE;
 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem) return FALSE;
 RUBREC_LIST::iterator it = m_pRubList->at(pCPrtQListBoxItem->GetPk().toInt());

 //........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if (m_pMyEditText && m_pMyEditText->isModified() && m_LastRub != -1)
    {PutDataInRubList(it);
     return TRUE;
    }
 return FALSE;
}

//--------------------------------- PutDataInRubList----------------------------------------------------------------------
void FormRubPrescription::PutDataInRubList(RUBREC_LIST::iterator it)
{    QByteArray           data;
     char                *ptr = 0;
     QString              stringStruct;
     QString              stringDST;
     //.................. si données composée ordonnance calculable ...........................
     //                   alors recomposer les données avec le texte modifié et les données calculables
     if (atoi((*it).m_Type)==TYP_ORDO_CALC)
        {m_pCMoteurBase->GetDataFromRubList(data, it);       // recuperer données soit dans liste cache soit sur disque
         ptr        = data.data();
         if (CGestIni::IsUtf8( ptr , data.size()))
            {  CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(QString::fromUtf8 ( ptr ), 0, &stringStruct);    // y isoler et recuperer les données calculables
            }
         else  CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData( ptr , 0, &stringStruct);
         stringDST       = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\" ?>\n<ordotext>\n";
         stringDST.append (m_pMyEditText->text());           // ajouter le texte modifé
         stringDST.append ( "\n</ordotext>\n\n<ordoMedicaStruct>");
         stringDST.append (stringStruct);                    // ajouter la structure calculable non modifiée
         stringDST.append ( "</ordoMedicaStruct>\n");
         m_pCMoteurBase->ReplaceDataInRubList(stringDST, m_pRubList, m_LastRub );
        }
     else
        {m_pCMoteurBase->ReplaceDataInRubList(m_pMyEditText->text(), m_pRubList, m_LastRub );
        }
}

//--------------------------------- pushButtonDelete_clicked -------------------------------------------------------------
void FormRubPrescription::pushButtonDelete_clicked()
{
 if (m_IsModifiable==0) return;

 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem) return;
 int id  =  pCPrtQListBoxItem->GetPk().toInt();

 RUBREC_LIST::iterator it = m_pRubList->at(id);
 //........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if ((*it).m_State & RUB_IS_TO_DELETE)
    {m_pCMoteurBase->RubList_SetItemNotDeleted(m_pRubList, id);
    }
 else
    {m_pCMoteurBase->RubList_SetItemDeleted(m_pRubList, id);
    }
 if ((*it).m_State == RUB_IS_TO_DELETE) {pushButtonDelete->setIconSet (Theme::getIconIsDeleted()); pushButtonDelete->setDown(TRUE);}
 else                                   {pushButtonDelete->setIconSet (Theme::getIconDelete());    pushButtonDelete->setDown(FALSE);}
}


//--------------------------------- pushButtonSave_clicked -------------------------------------------------------------
void FormRubPrescription::pushButtonSave_clicked()
{if (m_IsModifiable) emit Sign_SaveButtonClicked();
}

//--------------------------------- Current_RubList_ID -------------------------------------------------------------
int FormRubPrescription::Current_RubList_ID()
{CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (pCPrtQListBoxItem) return pCPrtQListBoxItem->GetPk().toInt();
 return -1;
}

//--------------------------------- Current_RubList_Iterator -------------------------------------------------------------
RUBREC_LIST::iterator FormRubPrescription::Current_RubList_Iterator()
{int id = Current_RubList_ID();
 if (id==-1) return  m_pRubList->end();
             return  m_pRubList->at(id);
}

//--------------------------------- SetCurentDocByPrimkey -------------------------------------------------------------
int FormRubPrescription::SetCurentDocByPrimkey(const QString &primKey)
{
 QListBox *pQListBox = comboBox_RubName->listBox();
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
 return -1; // pas trouvé
}

//--------------------------------- Slot_LapExited -------------------------------------------------------------
void FormRubPrescription::Slot_LapExited()
{ G_pCApp->m_pDrTux->Lap_StopProcess();
  QString htm     = "";
  QString xml     = G_pCApp->m_pDrTux->Lap_ExchangesFilesToDataBlob( &htm );
  pushButtonVidal->setEnabled(true);
  if (xml.length() == 0||htm.length()==0) return;

  m_pCMoteurBase->ReplaceDataInRubList(xml, m_pRubList, m_LastRub );
  if ( m_pMyEditText !=0 )
     { m_pMyEditText->setText(htm);
       m_pMyEditText->setModified (FALSE);
       m_pMyEditText->setReadOnly(true);     // les ordonnances securisees ne peuvent etre modifiees que dans le LAP
       m_pMyEditText->setPaper ( QBrush ( QColor("#fffef1"), Qt::SolidPattern   )  );
     }
}

//--------------------------------- pushButtonVidal_clicked -------------------------------------------------------------
void FormRubPrescription::pushButtonVidal_clicked()
{    QString  str_data         = "";
     //........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
     //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
     if (m_LastRub == -1)                                                               return;
      //.................. si données composées ordonnance calculable ...........................
      //                   alors recomposer les données avec le texte modifié et les données calculables
     m_pCMoteurBase->GotoDebug();
     int type  =  (*(m_pRubList->at(m_LastRub))).m_Type.toInt(); 
     if ( type == TYP_ORDO_CALC )
        { m_pCMoteurBase->GetDataFromRubList(str_data, m_pRubList, m_LastRub);                    // recuperer données soit dans liste cache soit sur disque
          CGestIni::Param_UpdateToDisk("/home/ro/ordoData-01.txt",str_data);
          int retLap  =  G_pCApp->m_pDrTux->Lap_Lauch(str_data, "MODIFY_PRESCRIPTION");           //  si ordo ancienne forme       retLap != DrTux::IS_ALREADY_LAUCH  ==> on va ds mode degrade
	  if (retLap == DrTux::IS_ALREADY_LAUCH) {pushButtonVidal->setEnabled(false);   return;}  //  Ok tout s'est bien passe et le LAP repondra dans le Slot_LapExited()

          //............. si le LAP n'est pas lance (cas des anciennes donnees) alors mode degrade ..................
          QString  stringStruct     = "";
          CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(str_data, 0, &stringStruct);        // y isoler et recuperer les données calculables
          if (stringStruct.length()==0)                                                 return;       // anciennes format de données structurées cassos
          Dlg_MedicaTux *dlg = new Dlg_MedicaTux(this,"Therapeutique_Dial",TRUE);
          if (dlg ==0)                                                                  return;
          if (dlg->initDialog(G_pCApp->m_pCMedicaBase, &stringStruct )==0) {delete dlg; return;};     // poids du malade = 70 kgs   m_PosoList;
          if (dlg->exec()== QDialog::Accepted && m_IsModifiable)
             { QString ordo = dlg->Get_OrdoXmlStruct();

               CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(ordo,  &stringStruct,0);
               m_pCMoteurBase->ReplaceDataInRubList(ordo, m_pRubList, m_LastRub );
               if (m_pMyEditText!=0)
                  { m_pMyEditText->setText(stringStruct);
                    m_pMyEditText->setModified (FALSE);
                  }
             }
          delete dlg;
       }
}

//--------------------------------- GetCurrentOrdoList -------------------------------------------------------------
int FormRubPrescription::GetCurrentOrdoList( QPL_CPosologie &ordo_List )
{QString str_data      = "";
 QString  stringStruct = "";
 int               ret = 0;
 ordo_List.clear();
 //........................... si rubrique qui etait avant celle a afficher, a ete modifiée ................................................
 //                            replacer son contenu dans la liste des rubriques avant d'afficher la nouvelle
 if (m_LastRub != -1)
    {//.................. si données composées ordonnance calculable ...........................
     //                   alors recomposer les données avec le texte modifié et les données calculables
     if ( (*(m_pRubList->at(m_LastRub))).m_Type.toInt() == TYP_ORDO_CALC )
        {m_pCMoteurBase->GetDataFromRubList(str_data, m_pRubList, m_LastRub);       // recuperer données soit dans liste cache soit sur disque
         CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(str_data,  0, &stringStruct);    // y isoler et recuperer les données calculables
         CMedicaBase::Medica_PosologieListDeserialize(stringStruct,  ordo_List);
         ret = 1;
        }
    }
 return ret;
}

//--------------------------------- pushButtonRubDateChange_clicked -------------------------------------------------------------
void FormRubPrescription::pushButtonRubDateChange_clicked()
{if (m_IsModifiable==0)                  return;
 if (m_pMyEditText->isReadOnly())        return;
 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)                 return;

 RUBREC_LIST::iterator it   =  m_pRubList->at(pCPrtQListBoxItem->GetPk().toInt());       // indice de liste
 DlgDateNom *dlg            =  new DlgDateNom(this,"DateNOm_Dlg",TRUE);                      // TRUE Pour modal
 if (dlg ==0)                            return ;
 dlg->initDialog( &(*it).m_Date, &(*it).m_Libelle, &(*it).m_SubType);

 if ( dlg->exec() == QDialog::Accepted )
    { //................... remettre à jour le comboBox ................................
//       pCPrtQListBoxItem->SetType((*it).m_Date);
      pCPrtQListBoxItem->SetDate((*it).m_Date);
      pCPrtQListBoxItem->setText((*it).m_Date.left(10) + " " +(*it).m_Libelle );
      //................... mettre le document en modified (en replaçant les data ds rubrique liste) ......................
      if (m_pMyEditText && m_LastRub != -1)
         {PutDataInRubList(it);
         }
    }
 delete dlg;
}

//--------------------------------- SetRubDateName -------------------------------------------------------------
void FormRubPrescription::SetRubDateName( const QString &name, const QString &date )
{
 if (m_IsModifiable==0)                   return;
 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) comboBox_RubName->listBox()->item ( comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)                  return;

 RUBREC_LIST::iterator it   =  m_pRubList->at(pCPrtQListBoxItem->GetPk().toInt());  // indice de liste
 //................... remettre à jour les valeurs de la liste ................................
 if (name.length()<=0 && date.length()<19) return; //2004-03-10 19:21:19
 if (name.length()>0)   (*it).m_Libelle = name;
 if (date.length()==19) (*it).m_Date    = date;

 //................... remettre à jour le comboBox ................................
//  pCPrtQListBoxItem->SetType((*it).m_Date);
 pCPrtQListBoxItem->SetDate((*it).m_Date);
 pCPrtQListBoxItem->setText((*it).m_Date.left(10) + " " +(*it).m_Libelle );
 //................... mettre le document en modified (en replaçant les data ds rubrique liste) ......................
 if (m_pMyEditText && m_LastRub != -1)
    {PutDataInRubList(it);
    }
}

//--------------------------------- SaveTextInRubList -------------------------------------------------------------
void FormRubPrescription::SaveTextInRubList()
{if (m_IsModifiable==0) return;
 if (m_pMyEditText && m_LastRub != -1)
    {PutDataInRubList(m_pRubList->at(m_LastRub));
    }
}
