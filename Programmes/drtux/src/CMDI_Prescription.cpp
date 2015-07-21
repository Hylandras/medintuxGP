/********************************* CMDI_Prescription.cpp **************************
 *                                                                                *
 *   #include "CMDI_Prescription.h"                                                *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
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
#include <qvbox.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include "CMDI_Prescription.h"
#include "CMDI_Observation.h"
#include "CApp.h"
#include "drtux.h"
#include "../../MedinTuxTools/CPrtQListBoxItem.h"
#include "../../MedinTuxTools/CHtmlTools.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//====================================== CMDI_Prescription =======================================================================


//---------------------------------- CMDI_Prescription -----------------------------------------------------------------------------
CMDI_Prescription::CMDI_Prescription( QWidget* parent, const char* name, int wflags,  CMoteurBase *pCMoteurBase, RUBREC_LIST *pRubList,
                                      const char* num_GUID, const char* id_doss, const char* dossNom, const char* dossPrenom, const char* user, int type)
   :CMDI_Generic( parent, name, wflags , pCMoteurBase, pRubList, type) // : QMainWindow( parent, name, wflags )
{

    SetType(TYP_PRESCRIPTION);
    SetPrintableState(TRUE);
    m_CustomMenuRequestState = 0;
    //............... placer en premier un gestionnaire vertical de QWidget ..............................
    //                en fils de la fenetre mère:
    //                les enfants de ce gestionnaire seront:
    //                             le dialogue: m_pFormRubPrescription    qui sera en haut
    //                             l'éditeur de texte: m_pMyEditText      qui sera en dessous
    QVBox *pQVCentral = new QVBox( this );
    pQVCentral->setMargin(0);
    pQVCentral->setSpacing(0);
    pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );

    //......................... creation du dialogue:  m_pC_RubObservation .............................
    m_pFormRubPrescription = new FormRubPrescription(pQVCentral, QString("Form") + name );
    if (m_pFormRubPrescription)
       {m_pFormRubPrescription->m_pCMoteurBase = pCMoteurBase;
        m_pFormRubPrescription->m_pRubList     = pRubList;
        if (num_GUID)   m_pFormRubPrescription->m_NumGUID    = num_GUID;
        else            m_pFormRubPrescription->m_NumGUID    = "";
        if (id_doss)    m_pFormRubPrescription->m_ID_Doss    = id_doss;
        else            m_pFormRubPrescription->m_ID_Doss    = "";
        if (dossNom)    m_pFormRubPrescription->m_DossNom    = dossNom;
        else            m_pFormRubPrescription->m_DossNom    = "";
        if (dossPrenom) m_pFormRubPrescription->m_DossPrenom = dossPrenom;
        else            m_pFormRubPrescription->m_DossPrenom = "";
        if (user)       m_pFormRubPrescription->m_User       = user;
        else            m_pFormRubPrescription->m_User       = "";

        //.................... creation de l'éditeur de texte: m_pMyEditText...............................
        //                     à ratacher au membre: MyEditText *m_pMyEditText de :  m_pFormRubPrescription
        //                     afin d'en disposer à l'interieur de sa classe.
        m_pMyEditText = new MyEditText(pCMoteurBase->m_DataBase, pQVCentral);
        m_pMyEditText->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
        m_pMyEditText->SetTypeRubrique(TYP_PRESCRIPTION);

        //m_pMyEditText->setPaletteBackgroundColor( QColor( 58, 141, 141 ) );
        m_pFormRubPrescription->InitText(m_pMyEditText);
        m_pFormRubPrescription->SetData();

        //.................. connection servant à mettre à jour .............................................
        //                   le type lors du changement de document dans le combo
        //                   en effet il peut y avoir soit des certificats: 20020300
        //                                            soit des ordonnances: 20020200
        connect(m_pFormRubPrescription,   SIGNAL( Sign_RubChanged(RUBREC_LIST::iterator)),
                 this,                    SLOT  ( OnRubChanged(RUBREC_LIST::iterator ))
               );
        //..................... connecter impression ......................................................
        connect( m_pFormRubPrescription->pushButtonPrint,     SIGNAL( clicked()),
                 this,                                        SLOT  ( OnPrint())
               );
        //.................. connecter les plugin de l'editeur de texte.............................................
        //                   au systeme de menu Custom

        connect( m_pMyEditText,   SIGNAL( Sign_ExeCommand(QString&)),
                this,             SLOT  ( Slot_ExeCommand(QString&))
               );
        connect( m_pMyEditText,   SIGNAL( Sign_Get_Custom_Menu(const QString &, QString&, QString &, QKeySequence &)),
                 this,            SLOT  ( Slot_Get_Custom_Menu(const QString &, QString&, QString &, QKeySequence &))
               );

       }
    setIcon(Theme::getIconListTypePrescription().pixmap(QIconSet::Automatic,TRUE,QIconSet::On) );
    setFocusProxy( m_pFormRubPrescription );
    setCentralWidget( pQVCentral );
}

//---------------------------------- ~CMDI_Prescription ------------------------------------------------------------------------
CMDI_Prescription::~CMDI_Prescription()
{emit Sign_RubIsDeleted(CMDI_Prescription::S_GetRubName());
 if (G_pCApp->m_pDrTux) G_pCApp->m_pDrTux->m_pCMDI_Prescription = 0;
}

//---------------------------------- GetCurrent_RubList_ID -----------------------------------------------------------------------------
int     CMDI_Prescription::GetCurrent_RubList_ID()
{return m_pFormRubPrescription->Current_RubList_ID();   // noter rubrique affichée;
}
//---------------------------------- Current_RubList_Iterator -----------------------------------------------------------------------------
RUBREC_LIST::iterator CMDI_Prescription::Current_RubList_Iterator()
{return m_pFormRubPrescription->Current_RubList_Iterator();
}
//----------------------------- reinitComboBoxWithRubList -----------------------------------
/*! \brief initialise la combo box avec les donnees de la rublist */
int  CMDI_Prescription::reinitComboBoxWithRubList(RUBREC_LIST  *pRubList, const QString &pkToDisplay /*=""*/)
{ int index = 0;
  int nb = m_pCMoteurBase->initConboBoxWithRubList(pRubList, m_pFormRubPrescription->comboBox_RubName, QString::number(CMDI_Prescription::S_GetType()), pkToDisplay, &index);
  if (nb<=0)
     {m_pFormRubPrescription->hide();
      m_pMyEditText->hide();
     }
  else
     {m_pFormRubPrescription->show();
      m_pMyEditText->show();
     }
  if (index == -1) index =  nb-1;
  m_pFormRubPrescription->m_LastRub = -1;
  m_pFormRubPrescription->comboBox_RubName_activated(index);
  m_pFormRubPrescription->comboBox_RubName->setCurrentItem ( index );
  m_pFormRubPrescription->textLabel_NbRub->setText(QString::number(nb));
  return nb;
}

//----------------------------- GetDocListComboBox -----------------------------------
/*! \brief retourne la combobox affectee a al liste des documents */
QComboBox    *CMDI_Prescription::GetDocListComboBox()
{return m_pFormRubPrescription->comboBox_RubName;
}
//----------------------------- initData -----------------------------------
/*! \brief initialise la rubrique avec les données la concernant */
int  CMDI_Prescription::initData()
{m_pFormRubPrescription->m_LastRub    = -1;   // tres tres important
 m_pFormRubPrescription->SetData();
 return 0;
}

//----------------------------- ChangeCurrentComboBoxItemName -----------------------------------
/*! \brief SLOT qui change le nom de l'item courant de la comboBox */
void CMDI_Prescription::ChangeCurrentComboBoxItemName(const QString &name)
{if (name.length()==0)               return;
 CPrtQListBoxItem* pCPrtQListBoxItem  = (CPrtQListBoxItem*) m_pFormRubPrescription->comboBox_RubName->listBox()->item (  m_pFormRubPrescription->comboBox_RubName->currentItem ()  );
 if (!pCPrtQListBoxItem)             return;
 pCPrtQListBoxItem->setText ( name );
}

//----------------------------- SetCurentDocByPrimkey -----------------------------------
/*! \brief initialise l'affichage sur le document désigné par const QString &pk */
int  CMDI_Prescription::SetCurentDocByPrimkey(const QString &pk)
{return m_pFormRubPrescription->SetCurentDocByPrimkey(pk);
}


//----------------------------- GetCurrentEditor -----------------------------------
/*! \brief retourne l'éditeur de texte courant avancé type MyEditText */
MyEditText  *CMDI_Prescription::GetCurrentEditor()
{return m_pMyEditText;
}

//---------------------------------- SetInterfaceOnDroits ----------------------------------------------------------------------------------------
/*! \brief positionne l'interface sur les droits */
void CMDI_Prescription::SetInterfaceOnDroits(const QString &/*droits*/)
{m_pFormRubPrescription->comboBox_RubName_activated( m_pFormRubPrescription->comboBox_RubName->currentItem() );
}

//---------------------------------- setSaveButtonVisible -----------------------------------------------------------------------------
/*! \brief fonction qui met le botuon de sauvegarde visible */
void CMDI_Prescription:: setSaveButtonVisible(int state )
{if (state==1)         m_pFormRubPrescription->pushButtonSave->show();
 else if (state ==0)   m_pFormRubPrescription->pushButtonSave->hide();
}

//---------------------------------- DoActionWithThisDocument -----------------------------------------------------------------------------
/*! \brief fait avec le document donné en entrée une action dépendante du verbe  */
void   CMDI_Prescription::DoActionWithThisDocument(const QString &verbe, int /*typ*/, const char *path /* =0 */, const QDateTime * /*pQDateTime =0 */, const QString * /*libelle =0 */, const QString * /*subType =0 */)
{
  if (verbe==tr("Tout") || verbe==tr("Ajouter"))
     {QFile file( path );
      if ( file.open( IO_ReadOnly ) )
         {QString   txt_f = "";
          CGestIni::Param_UpdateFromDisk(path, txt_f);
          if (CHtmlTools::IsRtf(txt_f))
             {QString   txt;
              CHtmlTools::RtfToQtextEdit_HTML(txt_f, txt_f.length(), txt, 1);
              txt_f = txt;
             }
          file.close();
          //txt_f.replace('0xa0','0x20');
          QString libelle = QFileInfo(path).baseName();
          int rubDoc_id   = -1;
          int rubObs_id   = -1;
          CMDI_Observation   *pQwdgRubObserv       = (CMDI_Observation*)  G_pCApp->m_pDrTux->IsExistRubrique(RUBNAME_OBSERVATION);
          //.................... cas de figures ............................................
          //   remplacement prescription: pQwdgRubPrescr->m_pMyEditText == pMyEditText
          if ( (rubDoc_id = m_pFormRubPrescription->Current_RubList_ID()) != -1)
             {                                     rubObs_id = G_pCApp->m_pDrTux->GetPossibleObservationDate((*m_pRubList->at(rubDoc_id)).m_Date);
              if (rubObs_id==-1 && pQwdgRubObserv) rubObs_id = pQwdgRubObserv->m_pC_RubObservation->Current_RubList_ID();
             }
          if (rubDoc_id==-1)
             {if (QMessageBox::warning ( this, tr("MedinTux solicite votre attention"),
                                           tr ( "Il n'existe pas de document valide pour cette action.<br>"
                                                "Faut-il en créer un avec celui :<br><b>«")
                                                +QFileInfo(path).fileName()+tr(" »</b><br>sélectionné dans le Glossaire "),
                                           tr ("Annu&ler"), tr ("&Créer"), 0,
                                           1, 1 )==1
                  )
                  {AddNewDocument(txt_f, 0, path);
                  }
             }
          else
             {
              G_pCApp->m_VAR_MAP.insert("$SCRIPT_STATUS",CMDI_Prescription::S_GetRubName()+"_$FUSION_ADD_"+verbe);
              G_pCApp->m_pDrTux->FusionneDocument(&txt_f, G_pCApp->m_SignUser, &(*m_pRubList->at(rubDoc_id)),G_pCApp->m_pDrTux->GetMapActiveID_Doc());
              G_pCApp->m_VAR_MAP.insert("$SCRIPT_STATUS",CMDI_Prescription::S_GetRubName()+"_$FUSION_END");
              //....................... reajuster les combobox et editeurs de texte ...................................
              m_pMyEditText->setModified(TRUE);
              if (verbe==tr("Tout"))
                 {if (QMessageBox::warning ( this, QString (PROG_NAME),
                                           tr ( "Êtes vous certain de tout remplacer<br>"
                                                "par ce document :<br><b>«")
                                                +QFileInfo(path).fileName()+tr(" »</b><br>sélectionné dans le Glossaire "),
                                           tr ("Annu&ler"), tr ("&Tout remplacer"), 0,
                                           0, 1 )==1
                     )
                     {if (QFileInfo(path).extension().lower()=="ord")
                         {if (m_pFormRubPrescription->m_LastRub != -1)
                             {RUBREC_LIST::iterator it = Current_RubList_Iterator();
                             //............... SI PAS DE  document affiché ...................................................
                              if (it == m_pRubList->end()) return;
                              m_pCMoteurBase->ReplaceDataInRubList(txt_f, it );
                              (*it).m_Type = QString::number(TYP_ORDO_CALC);
                              QString stringDST;
                              CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(txt_f, &stringDST, 0);    // y isoler et recuperer les données textuelle
                              m_pMyEditText->setText(stringDST);
                              m_pFormRubPrescription->pushButtonVidal->show();
                             }
                         }
                      else
                         {m_pMyEditText->setText(txt_f);
                         }
                       m_pMyEditText->setModified(TRUE); // indispensable à placer juste apres: setText(txt) !!!
                       m_pFormRubPrescription->SetRubDateName(libelle, (*m_pRubList->at(rubDoc_id)).m_Date );
                     }
                 }
              else if (verbe==tr("Ajouter"))
                 {CHtmlTools::insertHtml(m_pMyEditText, txt_f);
                 }//end else if (action=="Remplacer")
             } //end else if if (rubDoc_id==-1)
         }//end if ( file.open( IO_ReadOnly ) )
     }//end else if (action=="Remplacer" || action=="Ajouter")
}

//---------------------------------- AddNewDocument -----------------------------------------------------------------------------
/*! \brief replace le document en cours en replaçant le contenu dans la liste des rubriques, et ajoute un nouveau document à la liste */
CRubRecord*  CMDI_Prescription::AddNewDocument(const QString &strData, int typ, const char *path , const QDateTime *pQDateTime, const QString *libelle_in, const QString *subType, int  noInquire/*=0*/)
{if ( strData.length()==0 )                                   return 0;
 int                is_renouvellement    = 0;
 CMDI_Observation  *pQwdgRubObserv       = (CMDI_Observation*)  G_pCApp->m_pDrTux->IsExistRubrique(RUBNAME_OBSERVATION);     // rubName doit etre: "Observation" "Prescription" "Choix patient" "Identité"   ;
 QTextEdit       *pQTextEditObserv       = 0;
 QDateTime                     qdt       = QDateTime::currentDateTime();
 QString                      user       = "";
 QString                 sign_user       = "";
 bool                     conflictVerify = TRUE;
 QString                         libelle = "";
 QString                      date;
 if (pQwdgRubObserv) pQTextEditObserv    =  pQwdgRubObserv->GetCurrentEditor();  //m_LastRub
 if (libelle_in) libelle = *libelle_in;
 if (libelle.startsWith("==::==")) {libelle.remove("==::=="); conflictVerify = FALSE;}          // verifier si la verification des conflit doit être activée
 if (pQDateTime)               qdt       = *pQDateTime;
 //.................... lors de l'appel avec: typ==TYP_ORDO_CALC_CURENT ..................................
 //                     creation d'une ordonnance: TYP_ORDO_CALC avec date courante (pour renouvellement)
 //  #define TYP_ORDO_CALC_CURENT    CMDI_Prescription::S_GetType()+999     dans CApp.h
 if (typ== TYP_ORDO_CALC_CURENT ) {is_renouvellement = 1;       typ = TYP_ORDO_CALC;}
 if (path && QFileInfo(path).extension().lower()=="ord")        typ = TYP_ORDO_CALC;

 if (G_pCApp->IsThisDroitExist(G_pCApp->m_Droits, "orc") )          // si droit pour créer des prescriptions nouvelles
          {int      rubObs_id             = -1;
           //int      rubPrs_id             = m_pRubList->count();     // n'est pas encore dans la liste mais y sera
           //....................... si pas d'observation la date est celle de la creation et l'utilisateur ....................
           //                        celui en cours
           user      = G_pCApp->m_User;
           sign_user = G_pCApp->m_SignUser;
           date      = qdt.toString ("yyyy-MM-ddThh:mm:ss");
           //........................ A priori la date et l'utilisateur de la nouvelle prescription sont celles ...............................
           //                         de l'observation en cours sauf si renouvellement
           if (pQwdgRubObserv )
              {rubObs_id = pQwdgRubObserv->m_pC_RubObservation->Current_RubList_ID();
               if ( noInquire==0 && rubObs_id != -1 )                  // peut y avoir rubrique sans dossier si ce patient n'en a pas
                  {if (conflictVerify && G_pCApp->m_pDrTux->DateUserConflictResolve(user, sign_user, date,
                                                                                    (*m_pRubList->at(rubObs_id)).m_SignUser, (*m_pRubList->at(rubObs_id)).m_Date) ==2
                      ) return 0;
                  }
              }
           //............. placer les données de la rubrique en cours d'edition, si necessaire .............................
           //              dans la liste des rubriques, donc à sauver (que si necessaire CAD modifiées)
           IfModified_SaveInRubList();
           //............................ placer les données dans la liste des rubriques ............................................
           //                             avec comme nom le nom du fichier epuré du path et extension
           //           #define TYP_PRESCRIPTION   20020000
           //           #define TYP_ORDONNANCE     20020200
           //           #define TYP_CERTIFICAT     20020300
           QString rubName(tr("Prescription"));          // si pas plus de renseignement nom rubrique = Prescription
           if (typ==TYP_PRESCRIPTION && path )
              {rubName = path;
               //................. preciser si besoin le sous type de prescription .............................................
               //                  (en voyant de quel repertoire il provient)
               if      (rubName.find(tr("rdonnance"))!=-1)            typ = TYP_ORDONNANCE;
               else if (rubName.find(tr("ertificats"))!=-1)           typ = TYP_CERTIFICAT;
               else if (rubName.find(tr("ourrier"))!=-1)              typ = TYP_COURRIER;
               else                                                   typ = TYP_CERTIFICAT;
               //....... placer nom fichier comme nom rubrique .................................................................
               rubName = QFileInfo( path ).baseName();
             }
           else if (typ==TYP_ORDONNANCE)
             {rubName = tr("Ordonnance textuelle");
             }
           else if (typ==TYP_ORDO_LAP)
             {if (is_renouvellement) rubName = tr("Renouvellement d'ordonnance LAP");
              else                   rubName = tr("Ordonnance ajustable LAP");
             }
            else if (typ==TYP_ORDO_CALC || typ==TYP_ORDO_LAP)
             {if (is_renouvellement) rubName = tr("Renouvellement d'ordonnance");
              else                   rubName = tr("Ordonnance ajustable");
             }
           else if (typ==TYP_CERTIFICAT)
             {rubName = tr("Certificat");
             }
           else if (typ==TYP_COURRIER)
             {rubName = tr("Courrier");
             }
           //..................... on enregistre deja dans la liste les données essentielles ...............................
           //                      à la fusion (date, nom user , type)
           if (libelle.length()) rubName       = libelle;
           RUBREC_LIST::Iterator last = m_pRubList->append(CRubRecord(G_pCApp->m_pCMoteurBase->GetPkDoc_Provisoire(m_pRubList),                                     // PrimKey
                                                                      rubName,                                 // libelle
                                                                      QString::number(typ),                    // type
                                                                      subType ? *subType : "Default SubType",  // sous type
                                                                      date,                                    // date l'enregistrement
                                                                      user,                                    // utilisateur
                                                                      sign_user
                                                                      //strData,                               // données texte à placer apres fusion
                                                                      //strData.length()                       // longueur des données
                                                                     )
                                                          );
           //.......................... le fusionner .......................................................................
           (*last).m_LastErr = "";
           QString data(strData);
           G_pCApp->m_VAR_MAP.insert("$SCRIPT_STATUS",CMDI_Prescription::S_GetRubName()+"_$FUSION_CREATE");
           G_pCApp->m_pDrTux->FusionneDocument(&data, G_pCApp->m_User, &(*last), G_pCApp->m_pDrTux->GetMapActiveID_Doc());
           G_pCApp->m_VAR_MAP.insert("$SCRIPT_STATUS",CMDI_Prescription::S_GetRubName()+"_$FUSION_END");
           if ((*last).m_LastErr[0]=='_'||data.find("__ERREUR Fusion :__") != -1)
              {m_pRubList->remove ( last );
               return 0;
              }
           else
              {//.......................... on replace les données dans la liste apres fusion ....
               G_pCApp->m_pCMoteurBase->ReplaceDataInRubList(data, last);
               //.......................... on replace les données dans la rubrique ..............
               SetType(typ);
               m_pFormRubPrescription->SetData();  // cela permet de reinitialiser les combos box & cie
               if ( m_pMyEditText && strData.find("<Posologie_LAP_List>") != -1  ) 
                  { m_pMyEditText->setReadOnly(true);     // les ordonnances securisees ne peuvent etre modifiees que dans le LAP
                    m_pMyEditText->setPaper ( QBrush ( QColor("#fffef1"), Qt::SolidPattern   )  );
                  }
               return &*(last);
              }
          }
 return 0;
}

//---------------------------------- IfModified_SaveInRubList -----------------------------------------------------------------------------
void CMDI_Prescription::IfModified_SaveInRubList()
{m_pFormRubPrescription->IfModified_SaveInRubList();
}

//---------------------------------- Slot_Get_Custom_Menu -----------------------------------------------------------------------------
void CMDI_Prescription::Slot_Get_Custom_Menu(const QString &state, QString &menuIcon, QString &menuItem, QKeySequence &accel)
{if (state == "$StartRequest")    m_CustomMenuRequestState = 0;
 else                           ++m_CustomMenuRequestState;
 switch(m_CustomMenuRequestState)
   {case 0:
         menuIcon = G_pCApp->m_PathAppli + "Ressources/icones/backup_section.png";
         menuItem = tr("Enregi&strer comme document type");
         accel    = CTRL+Key_S;
         break;
    default:
         menuIcon = "";
         menuItem = "";
   }
}

//---------------------------------- Slot_ExeCommand -----------------------------------------------------------------------------
void CMDI_Prescription::Slot_ExeCommand(QString &command)
{  RUBREC_LIST::iterator it = Current_RubList_Iterator();
   if (it ==  m_pFormRubPrescription->m_pRubList->end() )  return;

   if (command==tr("Enregi&strer comme document type"))
       { QString   path    = G_pCApp->m_PathGlossaire + S_GetRubName();
         int        typ    = (*it).m_Type.toInt();
         QString    ext    = ".html";
         QString    txt    = "";
         QString  filtre   = tr( " Fichiers HTML (*.htm *.html);;  Tous les fichiers (*)");
         switch( typ )
            {case TYP_ORDO_CALC:
                  ext    = ".ord";
                  path  +=  tr("/Ordonnances codex");
                  filtre =  tr( " Fichiers Medica (*.ord);;  Tous les fichiers (*)");
                  break;
             case TYP_PRESCRIPTION:
             case TYP_ORDONNANCE:
                  path +=  tr("/Ordonnances types");
                  txt   = m_pMyEditText->text();
                  break;
             case TYP_COURRIER:
                  path +=  tr("/Courriers");
                  txt   = m_pMyEditText->text();
                  break;
             case TYP_CERTIFICAT:
                  path +=  tr("/Certificats");
                  txt   = m_pMyEditText->text();
                  break;
            }
        path = path.replace('/',QDir::separator () );
        QFileDialog fd (path,  filtre,  this, "SaveDlg" ) ;
        fd.setDir ( path ) ;
        fd.setMode(QFileDialog::AnyFile );    // selection multiple
        if ( fd.exec() != QDialog::Accepted )           return;
        QString fn = fd.selectedFile();
        if ( fn.isEmpty() )                             return;
        fn         = QFileInfo(fn).dirPath() + "/" + QFileInfo(fn).baseName() + ext;
        QFile file( fn);
        if ( !file.open( IO_WriteOnly ) )               return;
        QTextStream ts( &file );
        if (typ == TYP_ORDO_CALC)
           {QByteArray           data;
            char                *ptr = 0;
            QString              stringStruct;
            QString              stringDST;
            //.................. si données composée ordonnance calculable ...........................
            m_pCMoteurBase->GetDataFromRubList(data, it);       // recuperer données soit dans liste cache soit sur disque
            ptr        = data.data();
            if (CGestIni::IsUtf8( ptr , data.size()))
               {  stringStruct = QString::fromUtf8 ( ptr );     // CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(QString::fromUtf8 ( ptr ), 0, &stringStruct);    // y isoler et recuperer les données calculables
               }
            else  
               {  stringStruct = ptr;                           // CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData( ptr , 0, &stringStruct);
               }
            ts << stringStruct;
           }
        else
           {CHtmlTools::ElimineAncresRedondantes(txt);
            CHtmlTools::setTextDefaultAtribut(txt, "text",          m_pMyEditText->m_DefaultTextColor);
            CHtmlTools::setTextDefaultAtribut(txt, "bgcolor",       m_pMyEditText->m_DefaultBackgroundColor);
            CHtmlTools::QRichTextToHtml(txt);
            ts << txt;
           }
       }
}

//---------------------------------- OnPrint -----------------------------------------------------------------------------
void CMDI_Prescription::OnPrint()
{m_pFormRubPrescription->IfModified_SaveInRubList();
 emit Sign_Print();
}
//---------------------------------- OnRubChanged -----------------------------------------------------------------------------
// ACTION: Slot permettant à notre rubrique de mettre à jour ce qui est relatif au changement de document affiché
//         lors de l'activation du combo box des documents. Reponse au signal: Sign_RubChanged(RUBREC_LIST::iterator)
//         Ici on en profite pour mettre à jour le type de rubrique.

void CMDI_Prescription::OnRubChanged(RUBREC_LIST::iterator it)
{SetType(atoi((*it).m_Type));
}

//---------------------------------- RubOnUserChange -----------------------------------------------------------------------------
void CMDI_Prescription::RubOnUserChange(const char*user, const char* /*sign_user*/)
{m_pFormRubPrescription->m_User       = user;
}

//---------------------------------- OnIdentChange -----------------------------------------------------------------------------
// ACTION: ce slot est appelé par qui veut, pour signaler un changement d'identité à cette fenetre MDI
//         qui doit donc adapter son contenu à un nouveau malade.

void CMDI_Prescription::OnIdentChange(const char* num_GUID,  const char* id_doss,
                                      const char* dossNom ,  const char* dossPrenom )
{ m_pFormRubPrescription->m_NumGUID    = num_GUID;
  m_pFormRubPrescription->m_ID_Doss    = id_doss;
  m_pFormRubPrescription->m_DossNom    = dossNom;
  m_pFormRubPrescription->m_DossPrenom = dossPrenom;
  m_pFormRubPrescription->SetData();
}

//---------------------------------- SetZoom ----------------------------------------------------------------------------------------
void CMDI_Prescription::SetZoom(int /*zoom*/)
{//m_pFormRubPrescription->m_pMyEditText->SetZoom(zoom);
}



//---------------------------------- setWordWrap ----------------------------------------------------------------------------------------
void CMDI_Prescription::setWordWrap(int width)
{if (m_pMyEditText==0) return;
 if (width == -1)
    {m_pMyEditText->setWordWrap(QTextEdit::WidgetWidth);   //NoWrap, WidgetWidth, FixedPixelWidth, FixedColumnWidth
     //setWrapColumnOrWidth (793-37 );
    }
 else
    {m_pMyEditText->setWordWrap (QTextEdit::FixedPixelWidth);
     m_pMyEditText->setWrapColumnOrWidth (width );  //   (793-37 );   //( corresponds A4 width  21*72/2.54 )
    }
}
//---------------------------------- SetModifiableState -----------------------------------------------------------------------------
void CMDI_Prescription::SetModifiableState(int state)
{m_pFormRubPrescription->SetModifiableState(state);
}

//---------------------------------- setDataToModifiedState -----------------------------------------------------------------------------
/*! \brief place le document en etat modifié, afin qu'au prochain enregistrement elles soient mises à jour */
void CMDI_Prescription::setDataToModifiedState()
{ if (m_pFormRubPrescription->m_pMyEditText && m_pFormRubPrescription->m_LastRub != -1)
     {m_pFormRubPrescription->SaveTextInRubList();
     }
}
