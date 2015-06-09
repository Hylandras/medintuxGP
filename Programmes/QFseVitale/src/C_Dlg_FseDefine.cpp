/********************************* C_Dlg_FseDefine.cpp **********************************
\file .cpp
\brief (put some short descripion here).

    #include ".h"
    Project: MedinTux
    Copyright (C) 2003 2004 2005 2006 2007
    by Sevin Roland  and Data Medical Design
    E-Mail: medintux@medintux.org

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

#include "CVitaleReadThread.h"
#include "C_Dlg_FseDefine.h"
#include "C_Dlg_FseDefine.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "ui/Dlg_Calendar.h"
#include "../../SesamVitale-QT3/CSesam_API_InfoGroupe.h"
#include "C_VitaleListBoxItem.h"
#include "C_Fse.h"

#include <qeventloop.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qtextbrowser.h>
#include <qsplitter.h>
#include <qtabwidget.h>
#include <qmessagebox.h>
#include <qcursor.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qfiledialog.h>
#include <qwidgetstack.h>
#include <qtoolbox.h>
#include <qcheckbox.h>
#include <qcstring.h>
#include <qmemarray.h>
#include <qiconset.h>
#include <qwhatsthis.h>
#include <qdir.h>
#include <qfileinfo.h>
//----------------------------------- C_Dlg_FseDefine ------------------------------------------
C_Dlg_FseDefine::C_Dlg_FseDefine(CFseVitaleApp* pCFseVitaleApp, QWidget* parent, const char* name, bool modal, WFlags fl)
    : Dlg_FseDefine(parent, name, modal, fl)
{m_pCFseVitaleApp = pCFseVitaleApp;
 m_pC_Cps           = 0;
 m_pC_CpsRempla     = 0;
 m_pC_Vitale        = 0;
 //radioButtonSec_SesamSansVitale->hide();    // ce mode n'est possible que pour les labo, anapath, et pharmaciens.
 groupBox_PS_Titulaire->hide();
 groupBox_PS_Remplacant->hide();
 //................. regler les dates ...........................................
 lineEdit_FSE_Date->setText(QDate::currentDate().toString("dd-MM-yyyy"));
 lineEdit_ArretTravailDateDebut->setText(lineEdit_FSE_Date->text());
 lineEdit_Nbr_ArretTravail->setText("0");
 lineEdit_Nbr_ArretTravail_lostFocus();
 //....................... à priori pas d'accident de travail ..........................
 lineEdit_FSE_AccParTiers_Date->setEnabled(FALSE);
 pushButton_FSE_AccParTiers_Calendrier->setEnabled(FALSE);
 //..................... etat des boites de radio button ................................
 setSecurisationState(5);
 buttonGroup_FSE_FactureType->setButton(1);
 //..................... images des bouton de cartes.............................
 pushButton_CPS->setPixmap    ( QPixmap(m_pCFseVitaleApp->m_PathImages +"Carte_CPS_Butt.png") );
 pushButton_Vitale->setPixmap ( QPixmap(m_pCFseVitaleApp->m_PathImages +"VitaleButt.png") );
 pushButtonSaveCPS_Titulaire->setPixmap ( QPixmap(m_pCFseVitaleApp->m_PathImages +"filesave.png") );
 pushButtonLoadCPS_Titulaire->setPixmap ( QPixmap(m_pCFseVitaleApp->m_PathImages +"fileopen.png") );
 pushButtonSaveCPS_Titulaire->setEnabled(FALSE);

 pushButton_ActeEnCours_Ajouter->setIconSet  ( QIconSet (QPixmap(m_pCFseVitaleApp->m_PathImages +"3leftarrow.png") ) );
 pushButton_ActeEnCours_Valider->setIconSet  ( QIconSet (QPixmap(m_pCFseVitaleApp->m_PathImages +"3leftarrow.png") ) );

 pushButton_WhatIsIt->setPixmap    ( QPixmap(m_pCFseVitaleApp->m_PathImages +"help.png") );

 //..................... charger le fichier des caisses.............................
 CGestIni::Param_UpdateFromDisk(m_pCFseVitaleApp->m_PathImages +"tables/orgdest.csv",             m_CaisseFileData);
 CGestIni::Param_UpdateFromDisk(m_pCFseVitaleApp->m_PathImages +"tables/correspondance-0106.csv", m_CorrespTableData);
 //..................... allez un p'tit code porteur par defaut.............................
 lineEdit_CodePorteur->setText("1234");
 groupBox_PS_Remplacant->hide();
 //..................... connections.......................................................
 connect( pushButton_OK,                      SIGNAL( clicked() ),               this, SLOT( pushButton_OK_clicked() ) );
 connect( pushButton_Annuler,                 SIGNAL( clicked() ),               this, SLOT( pushButton_Annuler_clicked() ) );
 connect( pushButton_Vitale,                  SIGNAL( clicked() ),               this, SLOT( pushButton_Vitale_clicked() ) );
 connect( pushButton_CPS,                     SIGNAL( clicked() ),               this, SLOT( pushButton_CPS_clicked() ) );
 connect( pushButtonFormater,                 SIGNAL( clicked() ),               this, SLOT( pushButtonFormater_clicked() ) );
 connect( pushButton_FSE_ToListView,          SIGNAL( clicked() ),               this, SLOT( pushButtonFormater_clicked() ) );
 connect( listBox_Vitale,                     SIGNAL( clicked(QListBoxItem *) ), this, SLOT( listBox_Vitale_clicked(QListBoxItem *) ) );
 connect( pushButton_FSE_Calendrier,          SIGNAL( clicked() ),               this, SLOT( pushButton_FSE_Calendrier_clicked() ) );
 //connect( lineEdit_FSE_Date,                  SIGNAL( clicked() ),               this, SLOT( pushButton_FSE_Calendrier_clicked() ) );
 connect( pushButton_AT_DateDebut,            SIGNAL( clicked() ),               this, SLOT( pushButton_AT_DateDebut_clicked() ) );
 connect( pushButtonSaveCPS_Titulaire,        SIGNAL( clicked() ),               this, SLOT( pushButtonSaveCPS_Titulaire_clicked() ) );
 connect( pushButtonLoadCPS_Titulaire,        SIGNAL( clicked() ),               this, SLOT( pushButtonLoadCPS_Titulaire_clicked() ) );
 connect( pushButton_AT_DateFin,              SIGNAL( clicked() ),               this, SLOT( pushButton_AT_DateFin_clicked() ) );
 connect( pushButton_DoVirtual,               SIGNAL( clicked() ),               this, SLOT( pushButton_DoVirtual_clicked() ) );
 connect( pushButton_SaveVirtual,             SIGNAL( clicked() ),               this, SLOT( pushButton_SaveVirtual_clicked() ) );
 connect( listView_FseGroup,                  SIGNAL( doubleClicked ( QListViewItem *, const QPoint &, int ) ),
          this,                               SLOT(listView_FseGroup_doubleClicked(QListViewItem *, const QPoint &, int)));
 connect( lineEdit_Nbr_ArretTravail,          SIGNAL( lostFocus () ),                 this, SLOT(lineEdit_Nbr_ArretTravail_lostFocus ()));
 connect( lineEdit_AMC_CodeRoutage,           SIGNAL( textChanged (const QString&) ), this, SLOT(lineEdit_AMC_CodeRoutage_textChanged (const QString&)));
 connect( lineEdit_AMC_IdHote,                SIGNAL( textChanged (const QString&) ), this, SLOT(lineEdit_AMC_IdHote_textChanged (const QString&)));
 connect( lineEdit_AMC_NomDomaine,            SIGNAL( textChanged (const QString&) ), this, SLOT(lineEdit_AMC_NomDomaine_textChanged (const QString&)));
 connect( lineEdit_AMC_AiguillageSTS,         SIGNAL( textChanged (const QString&) ), this, SLOT(lineEdit_AMC_AiguillageSTS_textChanged (const QString&)));
 connect( comboBox_AMC_IndicTTT,              SIGNAL( activated (int )             ), this, SLOT(comboBox_AMC_IndicTTT_activated(int)));
 connect( toolBox_PEC,                        SIGNAL( currentChanged (int )        ), this, SLOT(toolBox_PEC_currentChanged(int)));
 connect( lineEdit_MUT_Numero,                SIGNAL( lostFocus () ),                 this, SLOT(lineEdit_MUT_Numero_lostFocus ()));
 connect( checkBox_ExoTicketMod_Non,          SIGNAL( clicked () ),                   this, SLOT(checkBox_ExoTicketMod_clicked_Non ()));
 connect( checkBox_ExoTicketMod_Oui,          SIGNAL( clicked () ),                   this, SLOT(checkBox_ExoTicketMod_clicked_Oui ()));
 connect( checkBox_AccParTiers_Non,           SIGNAL( clicked () ),                   this, SLOT(checkBox_AccParTiers_clicked_Non ()));
 connect( checkBox_AccParTiers_Oui,           SIGNAL( clicked () ),                   this, SLOT(checkBox_AccParTiers_clicked_Oui ()));
 connect( pushButton_ReadFileVitale,          SIGNAL( clicked () ),                   this, SLOT(pushButton_ReadFileVitale_clicked ()));
 connect( pushButton_WhatIsIt,                SIGNAL( clicked () ),                   this, SLOT(pushButton_WhatIsIt_clicked ()));
 connect( pushButton_ReadFileCPS,             SIGNAL( clicked () ),                   this, SLOT(pushButtonLoadCPS_Titulaire_clicked ()));
 connect( listBox_ActesDispos,                SIGNAL( onItem ( QListBoxItem *) ),          this, SLOT(listBox_ActesDispos_onItem ( QListBoxItem *)));
 connect( listBox_ActesDispos,                SIGNAL( currentChanged ( QListBoxItem *) ),  this, SLOT(listBox_ActesDispos_onItem ( QListBoxItem *)));

 //connect( lineEdit_ATMP_Date,                     SIGNAL( clicked () ),                   this, SLOT(ATMP_SetDate ()));
 connect( pushButton_ATMP_Date,                   SIGNAL( clicked () ),                   this, SLOT(ATMP_SetDate ()));
 connect( pushButton_FSE_AccParTiers_Calendrier,  SIGNAL( clicked () ),                   this, SLOT(AccParTiers_SetDate ()));
 //connect( lineEdit_FSE_AccParTiers_Date,          SIGNAL( clicked () ),                   this, SLOT(AccParTiers_SetDate ()));
 connect( pushButton_DebutGross_Calendrier,       SIGNAL( clicked () ),                   this, SLOT(Maternite_SetDate ()));
 //connect( lineEdit_DebutGross_Date,               SIGNAL( clicked () ),                   this, SLOT(Maternite_SetDate ()));
 connect( radioButton_GestionUnique ,             SIGNAL(stateChanged ( int  )),          this, SLOT(radioButton_GestionUnique_StateChanged (int )));
 connect( radioButton_GestionSeparee ,            SIGNAL(stateChanged ( int  )),          this, SLOT(radioButton_GestionSeparee_StateChanged (int )));

 //.................................... combo des indicateurs de traitement mutuelle ........................
 comboBox_AMC_IndicTTT->insertStringList( C_Vitale::CodeIndicTT_AMC_ToList() );
 comboBox_AMC_IndicTTT->insertItem("  ");
 clear_AMC_View();
 comboBox_MUT_IndicTTT->insertStringList(C_Vitale::CodeIndicTT_MUT_ToList());
 comboBox_MUT_IndicTTT->insertItem("  ");
 clear_MUT_View();
 comboBox_AMO_IndicTTT->insertStringList(C_Vitale::CodeService_AMO_ToList());
 //comboBox_MUT_IndicTTT->insertItem("  ");
 //.............................. initialiser les actes possibles en fonction du praticien .............
  listBox_ActesDispos->clear();
 //............................ Placer le spliter dans la bonne position .................................
  QValueList <int> list;
  list.append(130);
  list.append(width()-130);
  //splitterPrincipal->setSizes (list );
  //splitter_Vitale->setSizes (list );
  //splitter_CPS->setSizes (list );
  radioButton_GestionUnique_StateChanged ( QButton::Off );
  radioButton_GestionSeparee_StateChanged ( QButton::Off );
  int debug = 0;
  #if   defined(Q_WS_WIN)
     if (QFile::exists("C:/MedinTuxRo.txt"))                         debug = 1;
  #elif defined(Q_WS_X11)
     if (QFile::exists("/home/ro/MedinTuxRo.txt"))                   debug = 1;
  #elif defined(Q_WS_MAC)
     if (QFile::exists("/Utilisateurs/ro/Documents/MedinTuxRo.txt")) debug = 1;
  #endif

  if (!debug)
     {tabWidget_Beneficiaire->removePage ( tabWidget_Beneficiaire->page(3) );
      pushButtonSaveCPS_Titulaire->hide();
      pushButtonLoadCPS_Titulaire->hide();
      pushButton_ReadFileVitale->hide();
      pushButton_ReadFileCPS->hide();
      pushButton_DoVirtual->hide();
      pushButton_SaveVirtual->hide();
     }
}

//----------------------------------- ~C_Dlg_FseDefine ------------------------------------------
C_Dlg_FseDefine::~C_Dlg_FseDefine()
{if ( m_pC_Cps )          delete   m_pC_Cps;
 if ( m_pC_CpsRempla )    delete   m_pC_CpsRempla;
 if ( m_pC_Vitale )       delete   m_pC_Vitale;
}

/*$SPECIALIZATION$*/
//---------------------------------------- pushButton_SaveVirtual_clicked ---------------------------------------
void C_Dlg_FseDefine::pushButton_SaveVirtual_clicked()
{    QTimer  timer;
     QString data_Vitale;
     //..................... afficher le popup .......................................................................
     setEnabledReadState(FALSE);
     m_pCFseVitaleApp->CouCouUp("READ_VITALE","Lecture carte Vitale en cours....");
     //................... on cr�e un thread de lecture sesam-vitale .................................................

     CVitaleReadThread *pCVitaleReadThread = new CVitaleReadThread(m_pCFseVitaleApp->m_PathExchg,
                                                                   "READ_VITALE",
                                                                   m_pCFseVitaleApp->m_PathLog,
                                                                   m_pCFseVitaleApp->m_PathGalss,
                                                                   lineEdit_CodePorteur->text(),
                                                                   m_pCFseVitaleApp->m_RessPS,
                                                                   m_pCFseVitaleApp->m_RessLecteur);
     pCVitaleReadThread->start();                     // on le lance
     //....................... tant que le thread de lecture sesam ...................................................
     //                        n'a pas fini on execute les evenements
     //                        afin de laiser les messages syst�mes passer. (expl pour que le popup s'affiche)
     timer.start(1*1000, FALSE);
     while ( pCVitaleReadThread->IsThreadRun() )
           { m_pCFseVitaleApp->eventLoop()->processEvents ( QEventLoop::WaitForMore );
           }
     pCVitaleReadThread->wait();                     // on quitte proprement le thread
     delete pCVitaleReadThread;                      // on detruit le thread
     //........................ faire redescendre le popup ...........................................................
     m_pCFseVitaleApp->CouCouDown();
     setEnabledReadState(TRUE);

     //........................ recuperer donn�es de lecture ........................................................
     QString val1;
     CGestIni::Param_UpdateFromDisk(m_pCFseVitaleApp->m_PathExchg, data_Vitale);
     CGestIni::Param_ReadParam(data_Vitale, "ERR_CODE", "m_Erreur",&val1);
     if (val1.length())
        {QMessageBox::warning ( this, tr ("ERREUR DE LECTURE ! "),
                                tr ("<b><u>Erreur de lecture de la carte Vitale: </u></b><br><i>") + val1 +("</i>"),
                                tr ("&Op�ration annul�e"),0,0,
                                0, 1 );
         return;
        }
    C_Vitale_ToInterface(data_Vitale);
    C_Vitale vitale(data_Vitale);
    QString name = vitale.GetMember(1,   104, 1) + "_" + vitale.GetMember(3,   104, 1) + ".bin"; // renommer avec nom prenom
    CGestIni::Param_UpdateToDisk(m_pCFseVitaleApp->m_PathAppli + "Ressources/cartes_virtuelles/" + name, data_Vitale);
}

//---------------------------------------- pushButton_DoVirtual_clicked ---------------------------------------
void C_Dlg_FseDefine::pushButton_DoVirtual_clicked()
{   QString dir_to_erase = m_pCFseVitaleApp->m_PathAppli+"Ressources/cartes_virtuelles";
    QDir cur_dir( dir_to_erase );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {
        QString fname = fi->fileName().latin1();
        if      (fname == "..")
        {
        }
        else if (fi->isDir() && fname != "." )
        {//eraseDirectory(dir_to_erase + "/" + fname);
        }
        else if (fi->isFile())
        {   //cur_dir.remove(dir_to_erase + "/" + fname);
            QString data_Vitale = m_pCFseVitaleApp->LoadVirtualCarteVitale(dir_to_erase + "/" + fname);
            if (data_Vitale.length())
               {C_Vitale vitale(data_Vitale);
                QString newName = vitale.GetMember(1,   104, 1)+"_"+vitale.GetMember(3,   104, 1)+"_"+fi->baseName() + ".bin"; // renommer avec nom prenom
                cur_dir.rename ( dir_to_erase + "/" + fname, newName);
               }
        }
        ++it;
    }
}

//---------------------------------------- radioButton_GestionUnique_StateChanged ---------------------------------------
void C_Dlg_FseDefine::radioButton_GestionUnique_StateChanged (int state )
{ if (state == QButton::On)
     {groupBox_GestionUnique->show();
      radioButton_GestionSeparee_StateChanged ( QButton::Off );
     }
  else
     {groupBox_GestionUnique->hide();
     }
}
//---------------------------------------- radioButton_GestionSeparee_StateChanged ---------------------------------------
void C_Dlg_FseDefine::radioButton_GestionSeparee_StateChanged (int state )
{ if (state == QButton::On)
    {groupBoxAMC->show();
     radioButton_GestionUnique_StateChanged ( QButton::Off );
    }
 else
    {groupBoxAMC->hide();
    }
}
//---------------------------------------- pushButton_WhatIsIt_clicked ---------------------------------------
void C_Dlg_FseDefine::listBox_ActesDispos_onItem ( QListBoxItem *pQListBoxItem)
{if (pQListBoxItem == 0) return;
 QString data;
 CGestIni::Param_UpdateFromDisk(m_pCFseVitaleApp->m_PathImages +"tables/r1_table_1.csv", data);
 QStringList listActes;
 QString code = pQListBoxItem->text().stripWhiteSpace();
 C_Fse::Get_CSV_TableRef(data, code, 0, listActes);
 C_Fse::Get_CSV_TableRef(data, code, 0, listActes);
 if (listActes.count()==0)C_Fse::Get_CSV_TableRef(data, code+" *", 0, listActes);
 if (listActes.count())
    {textLabel_LibelleActeBase->setText(listActes[1]);
    }
 else
    {textLabel_LibelleActeBase->setText("");
    }
}

//---------------------------------------- init_listView_ActesDispos ---------------------------------------
// |1|#|C - CA CST  FPE - HN - ID - IK - IKM - IKS - K - KA - KC - HN - PRA - PRO - SCM - SES - STH - TDR -THR - UPH - V - VA -Z - ZM - ZN |
// |2|#|C - CS -CST HN - ID - IK - IKM - IKS - K - KA - KC -KCC  - KE - KFA - KFB- KTH - PRA - SES - STH - TDR -THR - V - VA - VS - Z - ZN|
// REMARQUE : la séparation des actes est variable : espaces, espaces tirets, etc...
void C_Dlg_FseDefine::init_listBox_ActesDispos()
{if (m_pC_Cps==0) return;
 QString data;
 CGestIni::Param_UpdateFromDisk(m_pCFseVitaleApp->m_PathImages +"tables/r1_table_2.csv", data);
 QString spe = m_pC_Cps->m_CodeSpecialite; if (spe[0]=='0') spe = spe.mid(1); // virer zéro de début
 QStringList listActes;
 C_Fse::Get_CSV_TableRef(data, spe, 0, listActes);
 if (listActes.count())
    {QString str = listActes[1].replace("-"," ");
     str         = str.replace("\r"," ");
     str         = str.replace("\n"," ");
     str         = str.simplifyWhiteSpace();
     listActes   = QStringList::split ( ' ', str, TRUE );
     if (listActes.count())
        {spe = m_pC_Cps->codeSpecialiteToString(spe.toInt());
         listBox_ActesDispos->clear();
         listBox_ActesDispos->insertStringList (listActes);
         textLabel_TitleLisActeBase->setText(tr("Actes de base pour\r") + spe);
        }
    }
}

//---------------------------------------- pushButton_WhatIsIt_clicked ---------------------------------------
void C_Dlg_FseDefine::pushButton_WhatIsIt_clicked ()
{
 QWhatsThis::enterWhatsThisMode ();
}

//---------------------------------------- ATMP_SetDate ---------------------------------------
void C_Dlg_FseDefine::ATMP_SetDate()
{lineEdit_ATMP_Date->setText(DoCalendrier(tr("Date de L'accident de travail"), lineEdit_ATMP_Date->text()));
}

//---------------------------------------- AccParTiers_SetDate ---------------------------------------
void C_Dlg_FseDefine::AccParTiers_SetDate()
{lineEdit_FSE_AccParTiers_Date->setText(DoCalendrier(tr("Date de l'accident"), lineEdit_FSE_AccParTiers_Date->text()));
}

//---------------------------------------- Maternite_SetDate ---------------------------------------
void C_Dlg_FseDefine::Maternite_SetDate()
{lineEdit_DebutGross_Date->setText(DoCalendrier(tr("Date de début de grossesse"), lineEdit_DebutGross_Date->text()));
}

//---------------------------------------- pushButton_ReadFileVitale_clicked ---------------------------------------
void C_Dlg_FseDefine::pushButton_ReadFileVitale_clicked()
{ QString data_Vitale   =  m_pCFseVitaleApp->LoadVirtualCarteVitale();
  if (data_Vitale.length()) C_Vitale_ToInterface(data_Vitale);
 /*
  //................. choisir le fichier image src à insérer .................................................
  QFileDialog *fd = new QFileDialog  ( m_pCFseVitaleApp->m_PathImages +"cartes_virtuelles/",
                                       tr( "Vitale-TestFiles (*.IN *.in);;All Files (*)" ),
                                       this,
                                       "OuvrirCpsDlg"
                                      ) ;
  if (fd==0)            return;
  if ( ! (fd->exec() == QDialog::Accepted) )
     {delete fd;        return;
     }
  QString fileName = fd->selectedFile();
  delete fd;
  if ( !fileName.isEmpty() && QFile::exists( fileName ))
     { QFile file(fileName );
       file.open ( IO_ReadOnly );
       int len = file.size();
       if (len==0)                               return;
       unsigned char *pBuffer = new unsigned char[ len ];                    // array of unsigned char
       if (pBuffer==0)                           return;
       long len_read = file.readBlock ( (char*)pBuffer, len );
       if ( len_read != len ) {delete []pBuffer; return;}
       file.close();
       int len_Groupe;
       int len_champ;
       unsigned short iDGroupeCourant;
       int numChamp;
       int occur;
       unsigned char *pt_debGroupe  = pBuffer;
       unsigned char *pt            = pt_debGroupe;
       unsigned char *end_file      = pBuffer + len;
       unsigned char *end           = pBuffer + len;
       QString data_Vitale          = "" ;
       while (pt_debGroupe < end_file)
         {//......................... rajouter le groupe ...................................
          iDGroupeCourant    = pt_debGroupe[0]; iDGroupeCourant = (iDGroupeCourant<<8) + pt_debGroupe[1];
          occur              = C_Vitale::GetNbOccurences(data_Vitale, iDGroupeCourant) + 1;
          data_Vitale       += QString("[VIT_GR-") + QString::number(iDGroupeCourant)  + "-OCC-" + QString::number(occur) + "]";
          data_Vitale       += TR("\r\n      ; Groupe : ") +
                               QString::number(iDGroupeCourant).rightJustify(4,'0')   +
                               "  (" + CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant) + ")";

          //......................... readapter les pointeurs ...................................
          if (pt_debGroupe[2] & 0x80)   // si bit de poids fort à 1 sauter le bouzin (sais pas pourquoi)
             {len_Groupe         = pt_debGroupe[3];
              pt_debGroupe       = pt_debGroupe + 4;
              data_Vitale       += "  truc : " + QString::number(pt_debGroupe[2] & 0x7F);
             }
          else
             {len_Groupe         = pt_debGroupe[2];
              pt_debGroupe       = pt_debGroupe + 3;
             }
          pt                 = pt_debGroupe;
          end                = pt + len_Groupe;
          int memberIndex    = 1;
          QString value      = "";

          //......................... rajouter les champs ...................................
          while (pt<end)
             {len_champ = pt[0];
              ++pt;               // passer la longueur du champ
              if (len_champ) value = QString::fromLatin1 ( (char*)pt, len_champ);
              else           value = "";
              data_Vitale         += QString("\r\n      m_Num-")  + QString::number(memberIndex) + " = " +
                                     value.replace(",","\\,").replace(";","\\;") + " , " + CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant, memberIndex);
              pt += len_champ;
              ++memberIndex;
             }
         //...................... goupe suivant .............................................
         data_Vitale  += "\r\n\r\n";
         pt_debGroupe += len_Groupe;
         }
       delete []pBuffer;
       C_Vitale_ToInterface(data_Vitale);
     }
 */
}

//---------------------------------------- checkBox_ExoTicketMod_clicked ---------------------------------------
void C_Dlg_FseDefine::checkBox_ExoTicketMod_clicked_Oui ()
{checkBox_ExoTicketMod_Non->setChecked ( FALSE );
 checkBox_ExoTicketMod_Oui->setChecked ( TRUE );
}

//---------------------------------------- checkBox_AccParTiers_clicked ---------------------------------------
void C_Dlg_FseDefine::checkBox_AccParTiers_clicked_Oui ()
{checkBox_AccParTiers_Non->setChecked ( FALSE );
 checkBox_AccParTiers_Oui->setChecked ( TRUE );
 lineEdit_FSE_AccParTiers_Date->setEnabled(TRUE);
 pushButton_FSE_AccParTiers_Calendrier->setEnabled(TRUE);
 lineEdit_FSE_AccParTiers_Date->setText(DoCalendrier(tr("Date de l'accident"), lineEdit_FSE_AccParTiers_Date->text()));
}

//---------------------------------------- checkBox_ExoTicketMod_clicked ---------------------------------------
void C_Dlg_FseDefine::checkBox_ExoTicketMod_clicked_Non ()
{checkBox_ExoTicketMod_Non->setChecked ( TRUE );
 checkBox_ExoTicketMod_Oui->setChecked ( FALSE );
}

//---------------------------------------- checkBox_AccParTiers_clicked ---------------------------------------
void C_Dlg_FseDefine::checkBox_AccParTiers_clicked_Non ()
{checkBox_AccParTiers_Non->setChecked ( TRUE );
 checkBox_AccParTiers_Oui->setChecked ( FALSE );
 lineEdit_FSE_AccParTiers_Date->setEnabled(FALSE);
 pushButton_FSE_AccParTiers_Calendrier->setEnabled(FALSE);
}
//---------------------------------------- comboBox_AMC_IndicTTT_activated ---------------------------------------
void C_Dlg_FseDefine::comboBox_AMC_IndicTTT_activated(int)
{textLabel_AMC_SesamVitale->setText(MakeSesamAMC_String());
}
//---------------------------------------- lineEdit_AMC_CodeRoutage_textChanged ---------------------------------------
void C_Dlg_FseDefine::lineEdit_AMC_CodeRoutage_textChanged(const QString&  /*text*/)
{textLabel_AMC_SesamVitale->setText(MakeSesamAMC_String());
}
//---------------------------------------- lineEdit_AMC_IdHote_textChanged ---------------------------------------
void C_Dlg_FseDefine::lineEdit_AMC_IdHote_textChanged(const QString&  /*text*/)
{textLabel_AMC_SesamVitale->setText(MakeSesamAMC_String());
}
//---------------------------------------- lineEdit_AMC_NomDomaine_textChanged ---------------------------------------
void C_Dlg_FseDefine::lineEdit_AMC_NomDomaine_textChanged(const QString& /*text*/)
{textLabel_AMC_SesamVitale->setText(MakeSesamAMC_String());
}
//---------------------------------------- lineEdit_AMC_AiguillageSTS_textChanged ---------------------------------------
void C_Dlg_FseDefine::lineEdit_AMC_AiguillageSTS_textChanged(const QString&  /*text*/)
{textLabel_AMC_SesamVitale->setText(MakeSesamAMC_String());
}

//---------------------------------------- toolBox_PEC_currentChanged ---------------------------------------
void C_Dlg_FseDefine::toolBox_PEC_currentChanged(int page)
{widgetStack_PEC->raiseWidget ( page );
}

//---------------------------------------- MakeSesamAMC_String ---------------------------------------
QString C_Dlg_FseDefine::MakeSesamAMC_String()
{return tr("<b>SESAM VITALE </b>(Aigui. / ind.Trait. / rout. / hôte / dom) : <font color=\"#001c83\"><b>")+
        lineEdit_AMC_AiguillageSTS->text() + "/" +
        GetIndicTraitement()               + "/" +
        lineEdit_AMC_CodeRoutage->text()   + "/" +
        lineEdit_AMC_IdHote->text()        + "/" +
        lineEdit_AMC_NomDomaine->text()    + "/</b></font>" ;
}

//---------------------------------------- GetIndicTraitement ---------------------------------------
QString C_Dlg_FseDefine::GetIndicTraitement()
{return comboBox_AMC_IndicTTT->currentText().left(2);
}

//---------------------------------------- DateTimeStr_jjMMyyyy_toDate ---------------------------------------
QDate C_Dlg_FseDefine::DateTimeStr_jjMMyyyy_toDate(const QString & dateTime )
{ int i= 0, len = dateTime.length();
  QString str = "";
  for (i= 0; i<len; ++i)
      {if (dateTime.at(i)>='0' && dateTime.at(i)<='9') str += dateTime.at(i);
      }
  QString date  = str.left(8);
  int jour  = date.mid(0,2).toInt();
  int mois  = date.mid(2,2).toInt();
  int annee = date.mid(4,4).toInt();
  return QDate(annee, mois, jour);
}
//----------------------------------- setEnabledReadState ------------------------------------------
void C_Dlg_FseDefine::setEnabledReadState(bool state)
{    pushButton_Vitale->setEnabled(state);
     pushButtonFormater->setEnabled(state);
     pushButton_CPS->setEnabled(state);
     pushButton_FSE_ToListView->setEnabled(state);
}

//----------------------------------- pushButtonSaveCPS_Titulaire_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButtonSaveCPS_Titulaire_clicked()
{ if (m_pC_Cps == 0) return;
  m_pC_Cps->m_CodePorteur="";
  CGestIni::Param_UpdateToDisk(m_pCFseVitaleApp->m_PathImages +"cps/"+m_pC_Cps->m_Nom+"_"+m_pC_Cps->m_Prenom+"-"+m_pC_Cps->m_NIR+".cps", m_pC_Cps->Serialize());
}

//----------------------------------- pushButtonLoadCPS_Titulaire_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButtonLoadCPS_Titulaire_clicked()
{ QString data_CPS = m_pCFseVitaleApp->LoadVirtualCarteCps();
  if (data_CPS.length()) C_CPS_ToInterface(data_CPS);
  /*
 //................. choisir le fichier image src à insérer .................................................
  QFileDialog *fd = new QFileDialog  ( m_pCFseVitaleApp->m_PathImages +"cps/",
                                       tr( "CPS-Files (*.cps);;All Files (*)" ),
                                       this,
                                       "OuvrirCpsDlg"
                                      ) ;
  if (fd==0)                                            return;
  if ( ! (fd->exec() == QDialog::Accepted) )
     {delete fd;
      return;
     }
  QString fileName = fd->selectedFile();
  delete fd;
  if ( !fileName.isEmpty() && QFile::exists( fileName ))
     { CGestIni::Param_UpdateFromDisk(fileName, data_CPS);
       if (data_CPS.length()==0) return;
       C_CPS_ToInterface(data_CPS);
       / *
       QString old_Spe = "" ;
       if (m_pC_Cps)
          {old_Spe = m_pC_Cps->m_CodeSpecialite;
           delete m_pC_Cps;
          }
       m_pC_Cps = new C_Cps("", data_CPS);
       textEdit_MonCPS->setText(data_CPS);
       textBrowser_CPS->setText(CPS_To_Html_Display(m_pC_Cps));
       if (m_pC_Cps && m_pC_Cps->m_CodeSpecialite != old_Spe) init_listBox_ActesDispos();
       * /
     }
 */
}

//----------------------------------- DoCalendrier ------------------------------------------
QString C_Dlg_FseDefine::DoCalendrier(const QString titre, QString date)
{lineEdit_Nbr_ArretTravail_lostFocus();
 Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |WStyle_MinMax | WStyle_SysMenu);
 if (dlg ==0)                                                   return date;
 dlg->setCaption(titre);
 dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
 QString text = date.remove('-');
 int jour, mois, annee;
 jour  = text.mid(0,2).toInt();
 mois  = text.mid(2,2).toInt();
 annee = text.mid(4,4).toInt();
 if (QDate::isValid ( annee, mois, jour ) )
    {dlg->setDate(QDate(annee, mois, jour));
    }
 if (dlg->exec()== QDialog::Accepted )
    {date = dlg->getDate().toString("dd-MM-yyyy");
    }
 delete dlg;
 return date;
}

//----------------------------------- pushButton_FSE_Calendrier_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButton_FSE_Calendrier_clicked()
{lineEdit_Nbr_ArretTravail_lostFocus();
 lineEdit_FSE_Date->setText(DoCalendrier(tr("Date de début"), lineEdit_FSE_Date->text()));
}

//----------------------------------- pushButton_AT_DateFin_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButton_AT_DateFin_clicked()
{lineEdit_Nbr_ArretTravail_lostFocus();
 // lineEdit_ArretTravailDateFin->setText(DoCalendrier(tr("Date de fin"), lineEdit_ArretTravailDateFin->text()));

 Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial_Fin", TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |WStyle_MinMax | WStyle_SysMenu);
 if (dlg ==0)                                                   return;
 dlg->setCaption(tr("Date de Fin"));
 dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
 QString text = lineEdit_ArretTravailDateFin->text().remove('-');
 int jour, mois, annee;
 jour  = text.mid(0,2).toInt();
 mois  = text.mid(2,2).toInt();
 annee = text.mid(4,4).toInt();
 QDate dtw = QDate(annee, mois, jour);
 QDate dtd = DateTimeStr_jjMMyyyy_toDate( lineEdit_ArretTravailDateDebut->text() );
 if (dtw.isValid () )
    {dlg->setDate(dtw);
     dlg->setDateDeb(&dtd);
    }
 if (dlg->exec()== QDialog::Accepted )
    {lineEdit_ArretTravailDateFin->setText(dlg->getDate().toString("dd-MM-yyyy"));
    }
 delete dlg;
 lineEdit_Nbr_ArretTravail->setText(DateATControl());
}

//----------------------------------- pushButton_AT_DateDebut_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButton_AT_DateDebut_clicked()
{lineEdit_Nbr_ArretTravail_lostFocus();
 Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial_Deb", TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |WStyle_MinMax | WStyle_SysMenu);
 if (dlg ==0)                                                   return;
 dlg->setCaption(tr("Date de début"));
 dlg->move ( QCursor::pos().x(), QCursor::pos().y() );
 QString text = lineEdit_ArretTravailDateDebut->text().remove('-');
 int jour, mois, annee;
 jour  = text.mid(0,2).toInt();
 mois  = text.mid(2,2).toInt();
 annee = text.mid(4,4).toInt();
 QDate dtw = QDate(annee, mois, jour);
 QDate dtd = DateTimeStr_jjMMyyyy_toDate( lineEdit_ArretTravailDateFin->text() );

 if (dtw.isValid () )
    {dlg->setDate(dtw);
     dlg->setDateDeb(&dtd);
    }
 if (dlg->exec()== QDialog::Accepted )
    {lineEdit_ArretTravailDateDebut->setText(dlg->getDate().toString("dd-MM-yyyy"));
    }
 delete dlg;
 lineEdit_Nbr_ArretTravail->setText(DateATControl());
}

//----------------------------------- DateATControl ------------------------------------------
QString C_Dlg_FseDefine::DateATControl()
{QDate dateDebut = DateTimeStr_jjMMyyyy_toDate(lineEdit_ArretTravailDateDebut->text() );
 QDate dateFin   = DateTimeStr_jjMMyyyy_toDate(lineEdit_ArretTravailDateFin->text() );
 if (! dateDebut.isValid())
    {QMessageBox mb( tr("Erreur date de début AT "),
                            tr("La date de début AT n'est pas correcte\n"),
                            QMessageBox::Critical,
                            QMessageBox::Ok | QMessageBox::Default,0,0);
     mb.exec();
     return tr("Invalide");
    }
 if (! dateFin.isValid())
    {QMessageBox mb( tr("Erreur date de fin AT "),
                            tr("La date de fin AT n'est pas correcte\n"),
                            QMessageBox::Critical,
                            QMessageBox::Ok | QMessageBox::Default,0,0);
     mb.exec();
     return tr("Invalide");
    }

 int nbJours     = dateDebut.daysTo (dateFin );
 if (nbJours<0)
    {QMessageBox mb( tr("Erreur date AT"),
                            tr("La date début ne peut être supérieure à la date de fin AT \n"),
                            QMessageBox::Critical,
                            QMessageBox::Ok | QMessageBox::Default,0,0);
     mb.exec();
     return tr("Invalide");
    }
  return QString::number(nbJours);
}

//----------------------------------- getDateAT ------------------------------------------
QString C_Dlg_FseDefine::getDateAT(QDate *pQDate /* = 0 */)
{QDate dateDebut = DateTimeStr_jjMMyyyy_toDate(lineEdit_ArretTravailDateDebut->text() );
 QDate dateFin   = dateDebut.addDays ( lineEdit_Nbr_ArretTravail->text().toInt() );
 if (pQDate) *pQDate = dateFin;
 if (! dateFin.isValid())
    {QMessageBox mb( tr("Erreur date de fin AT "),
                            tr("La date de fin AT n'est pas correcte\n"),
                            QMessageBox::Critical,
                            QMessageBox::Ok | QMessageBox::Default,0,0);
     mb.exec();
     return tr("");
    }
 return dateFin.toString("yyyyMMdd");
}
//----------------------------------- lineEdit_Nbr_ArretTravail_lostFocus ----------------
void C_Dlg_FseDefine::lineEdit_Nbr_ArretTravail_lostFocus()
{lineEdit_Nbr_ArretTravail_textChanged ( lineEdit_Nbr_ArretTravail->text());
}

//----------------------------------- lineEdit_MUT_Numero_lostFocus ----------------
void C_Dlg_FseDefine::lineEdit_MUT_Numero_lostFocus()
{QString  mutNum = lineEdit_MUT_Numero->text();
 if (mutNum.length())
    {QString  codeRegime     = m_pC_Vitale->GetMember(10,101, 1);
     QString  caisseGest     = m_pC_Vitale->GetMember(11,101, 1);
     QString  centreGest     = m_pC_Vitale->GetMember(12,101, 1);
     QString orgaComplName   = "";
     QString numOrgaCompl    = C_Vitale::Mutnum_To_Complementaire(m_CorrespTableData,
                                                                  mutNum,
                                                                  codeRegime,
                                                                  caisseGest,
                                                                  centreGest,
                                                                  &orgaComplName);
     lineEdit_MUT_Nom->setText(orgaComplName);
     lineEdit_MUT_NumeroOrga->setText(numOrgaCompl);
   }
 else
   {clear_MUT_View();
   }
}

//----------------------------------- lineEdit_Nbr_ArretTravail_textChanged ----------------
void C_Dlg_FseDefine::lineEdit_Nbr_ArretTravail_textChanged ( const QString &text )
{QDate dateDebut = DateTimeStr_jjMMyyyy_toDate(lineEdit_ArretTravailDateDebut->text() );
 QDate dateFin   = dateDebut.addDays ( text.toInt() );
 lineEdit_ArretTravailDateFin->setText(dateFin.toString("dd-MM-yyyy"));
}

//----------------------------------- getDateFSE ------------------------------------------
QString C_Dlg_FseDefine::getDateFSE()
{QDate dateFSE = DateTimeStr_jjMMyyyy_toDate(lineEdit_FSE_Date->text() );
 if (dateFSE.isValid()) return dateFSE.toString("yyyyMMdd");
 else                   return tr("Invalide");
}

//----------------------------------- pushButtonFormater_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButtonFormater_clicked()
{lineEdit_Nbr_ArretTravail_lostFocus();
 //.................... il faut l'API SESAM Vitale ....................................
 CSesam_API sesamAPI(lineEdit_CodePorteur->text()    ,
                     m_pCFseVitaleApp->m_RessPS      ,
                     m_pCFseVitaleApp->m_RessLecteur ,
                     m_pCFseVitaleApp->m_PathGalss   ,
                     m_pCFseVitaleApp->m_PathLog     ,
                     getDateFSE() );

 unsigned short  session  = 0XFFFF;
 //unsigned short  nbEssais = 3+1;
 if (m_pC_CpsRempla == 0 && m_pC_Cps == 0)
    {pushButton_CPS_clicked();
    }
 if (m_pC_CpsRempla && m_pC_Cps == 0)
    {if (QMessageBox::warning ( this, tr ("ATTENTION ! "),
                                tr ("La carte CPS en place : <b>") + m_pC_CpsRempla->m_Nom + " "+  m_pC_CpsRempla->m_Prenom +
                                tr ("</b> est une carte de <u>remplaçant</u>.<br>"
                                    "Pour créer une feuille de soins electronique avec votre carte<br>"
                                    "il vous faut définir quel <u>médecin est remplacé</u> actuellement. <br>"
                                    "Pour cela il vous est possible :<br><br>"
                                    " <b> - Soit </b> d'en choisir un précédemment enregistré dans une liste. <br>"
                                    " <b> - Soit </b> il vous est aussi possible de lire la carte  <br>"
                                    "                   du médecin à remplacer en premier, <br>"
                                    "                   puis à la suite celle du remplaçant. <br>"),
                                tr ("&Choisir le médecin remplacé dans une liste"),tr("Annuler"),0,
                                0, 1 ) == 0)
        {pushButtonLoadCPS_Titulaire_clicked();
        }
    }
 if ( m_pC_Cps==0)      return;
 if ( m_pC_Vitale ==0 ) pushButton_Vitale_clicked();
 if ( m_pC_Vitale ==0 ) return ;       // erreur lors lecture, m_pC_Vitale n'est pas instanciée
 C_Fse fse(&sesamAPI , session  , lineEdit_CodePorteur->text(), "");
 //...1110 ....................... Groupe identification de la facture .............................
 fse.SetMember(1, 1110, m_pCFseVitaleApp->GetNumeroFacture(), 0, 1);    // N° de facture
 fse.SetMember(2, 1110, "1", 0, 1);                                                      // Nature d'opération
 fse.SetMember(3, 1110, getDateFSE().leftJustify(12,'0'), 0, 1);                         // Date d'élaboration de la facture
 fse.SetMember(4, 1110, m_pC_Cps->m_NumLog, 0, 1);                                       // N° logique de la situation de facturation du PS
 fse.SetMember(5, 1110, "", 0, 1);                                                       // N° de facture rectifié
 fse.SetMember(6, 1110, "", 0, 1);                                                       // Date d'élaboration de la facture rectifié

 //...1120 ....................... Groupe identification du professionnel de santé .............................
 fse.SetMember(1,  1120, m_pC_Cps->m_NumIdentFactuPs, 0, 1);                    // N° d'identification de facturation du Professionnel de santé
 fse.SetMember(2,  1120, m_pC_Cps->m_NumIdentFactuPsClef, 0, 1);                // Clé du N° d'identification de facturation du Professionnel de santé
 fse.SetMember(3,  1120, m_pC_Cps->m_Nom, 0, 1);                                // Nom du Professionnel de santé
 fse.SetMember(4,  1120, m_pC_Cps->m_Prenom, 0, 1);                             // Prénom du Professionnel de santé
 fse.SetMember(5,  1120, m_pC_Cps->m_CodeConvention, 0, 1);                     // Code conventionnel sous lequel exerce le Professionnel de sant
 fse.SetMember(6,  1120, m_pC_Cps->m_CodeSpecialite, 0, 1);                     // Code spécialité
 fse.SetMember(7,  1120, m_pC_Cps->m_CodeZoneTarif, 0, 1);                      // Code zone tarifaire
 fse.SetMember(8,  1120, m_pC_Cps->m_CodeZoneIk, 0, 1);                         // Code Zone IK
 fse.SetMember(9,  1120, m_pC_Cps->m_CodeAgr1, 0, 1);                           // Code agrément 1
 fse.SetMember(10, 1120, m_pC_Cps->m_CodeAgr2, 0, 1);                           // Code agrément 2
 fse.SetMember(11, 1120, m_pC_Cps->m_CodeAgr3, 0, 1);                           // Code agrément 3
 //.............. Liberal 'L' ou Salarie 'S' ...........................
 // #???#  faut il placer le code ou 'L' ou 'S'
 QString res;
 switch (m_pC_Cps->m_ModeExe.toInt())
   {case  0  : res = "L" ; break ; // return TR("Libéral, exploitant ou commerçant");
    case  1  : res = "S" ; break ; // return TR("Salarié");
    case  4  : res = "L" ; break ; // return TR("Remplaçant");
    case  7  : res = "L" ; break ; // return TR("Non rétribué");
    default  : res = "L" ; break ; // return TR("code inconnu : ")+QString::number(code);
   }
 fse.SetMember(12, 1120, res, 0, 1);

 //...1130 ....................... Groupe identification du Bénéficiaire .............................
 QListBoxItem        *pQListBoxItem        = listBox_Vitale->selectedItem();
 C_VitaleListBoxItem *pC_VitaleListBoxItem = (C_VitaleListBoxItem*) pQListBoxItem;
 int occurence                             = pC_VitaleListBoxItem->m_Occurence.toInt();  // occurence à laquelle correspond l'item de la listbox

 fse.SetMember(1,  1130, m_pC_Vitale->GetMember(7, 101, 1), 0, 1);                       // Type d'identification du bénéficiaire
 fse.SetMember(2,  1130, m_pC_Vitale->GetMember(8, 101, 1), 0, 1);                       // N° National d'immatriculation NIR
 fse.SetMember(3,  1130, m_pC_Vitale->GetMember(9, 101, 1), 0, 1);                       // Clé du NIR
 fse.SetMember(4,  1130, m_pC_Vitale->GetMember(12,104, occurence), 0, 1);               // Date de naissance du bénéficiaire
 fse.SetMember(5,  1130, m_pC_Vitale->GetMember(13,104, occurence), 0, 1);               // Rang de naissance du bénéficiaire
 fse.SetMember(6,  1130, m_pC_Vitale->GetMember(14,104, occurence), 0, 1);               // Qualité
 fse.SetMember(7,  1130, m_pC_Vitale->GetMember(15,104, occurence), 0, 1);               // Code couverture, code ALD, Code situation (optionnel en non sécurisé)
 fse.SetMember(8,  1130, comboBox_CodeCarnetMedical->currentText().left(1) , 0, 1);      // Code de présentation du carnet Médical O ou N

 //...1140 ....................... Groupe Identification Structure .............................
 fse.SetMember(1, 1140, m_pC_Cps->m_TypIdentStruct, 0, 1);      // Type d'identification de Structure
 fse.SetMember(2, 1140, m_pC_Cps->m_NumIdentStruct, 0, 1);      // N° Identification de structure dans laquelle le professionnel de santé exerce
 fse.SetMember(3, 1140, m_pC_Cps->m_NumIdentStructClef, 0, 1);  // Clé du N° Identification de structure
 fse.SetMember(4, 1140, m_pC_Cps->m_RaisonSocialeStruct, 0, 1); // Nom de la structure

 //...1150 ....................... Groupe Identification Remplaçant .............................
 if (m_pC_CpsRempla)   // remplaçant
    { fse.SetMember(1, 1150, m_pC_CpsRempla->m_NumIdentPsRempla, 0, 1);      // N° d'identification de facturation du Professionnel de santé Remplaçant
      fse.SetMember(2, 1150, m_pC_CpsRempla->m_NumIdentPsRemplaClef, 0, 1);  // Clé du N° d'identification de facturation
      fse.SetMember(3, 1150, m_pC_CpsRempla->m_Nom, 0, 1);                   // Nom du Professionnel de santé Remplaçant
      fse.SetMember(4, 1150, m_pC_CpsRempla->m_Prenom, 0, 1);                // Prénom du Professionnel de santé Remplaçant
      fse.SetMember(5, 1150, "2", 0, 1);                               // Indicateur de signature (facture élaborée par un remplaçant ou sous sa responsabilité) dico P 37
    }

 //...1160 ....................... Groupe Identification Individu .............................
 res  = m_pC_Vitale->GetMember(9,104, occurence).stripWhiteSpace();;
 if (res.length())
    { fse.SetMember(1, 1160, res, 0, 1);                                        // N° national d'immatriculation individu
      fse.SetMember(2, 1160, m_pC_Vitale->GetMember(10,104, occurence), 0, 1);  // Clé du N° national d'immatriculation individu
    }

 //...1220 ....................... Groupe Arrêt de travail prescrit (facultatif).............................

 res = lineEdit_Nbr_ArretTravail->text();
 if (res.toInt() != 0)
    { fse.SetMember(1, 1220, res, 0, 1);                                   // Nombre de jours d'arrêt de travail prescrit
      fse.SetMember(2, 1220, getDateAT().leftJustify(12,'0'), 0, 1);       // Date de fin d'arrêt de travail
    }
 //...1310 ....................... Groupe Organismes AMO (page 18/75) .............................
 fse.SetMember(1, 1310, m_pC_Vitale->GetMember(10,101, 1), 0, 1);                                       // Code régime
 fse.SetMember(2, 1310, m_pC_Vitale->GetMember(11,101, 1), 0, 1);                                       // Caisse gestionnaire
 fse.SetMember(3, 1310, m_pC_Vitale->GetMember(12,101, 1), 0, 1);                                       // Centre gestionnaire
 fse.SetMember(4, 1310, m_pC_Vitale->CodeRegimeToOrganismeDestinataire(res, m_CaisseFileData), 0, 1);   // Code organisme destinataire
 fse.SetMember(5, 1310, res, 0, 1);                                                                     // Code centre informatique

 //...1321 ....................... Groupe Organismes AMC (page 18/75) .............................
 /*
 fse.SetMember(1, 1321, m_pC_Vitale->GetMember(10,101, 1), 0, 1);                     // Type de contrat
 fse.SetMember(2, 1321, m_pC_Vitale->GetMember(11,101, 1), 0, 1);                     // N° organisme complémentaire (carte vitale ou autre support)
 fse.SetMember(3, 1321, m_pC_Vitale->GetMember(12,101, 1), 0, 1);                     // N° d'adhérent à un organisme complémentaire
 fse.SetMember(4, 1321, m_pC_Vitale->CodeRegimeToOrganismeDestinataire(), 0, 1);      // Donées AMC Zone 1
 fse.SetMember(5, 1321, m_pC_Vitale->GetMember(10,101, 1), 0, 1);                     // Donées AMC Zone 2
 fse.SetMember(6, 1321, m_pC_Vitale->GetMember(11,101, 1), 0, 1);                     // N° organisme complémentaire EDI (carte vitale ou autre support)
 fse.SetMember(7, 1321, m_pC_Vitale->GetMember(12,101, 1), 0, 1);                     // Code application (99 ou 98)
 fse.SetMember(8, 1321, m_pC_Vitale->CodeRegimeToOrganismeDestinataire(), 0, 1);      // Code de routage flux AMC (carte vitale table conversion ou autre support)
 fse.SetMember(9, 1321, m_pC_Vitale->GetMember(10,101, 1), 0, 1);                     // Domaine (carte vitale table conversion ou autre support)
 fse.SetMember(10,1321, m_pC_Vitale->GetMember(11,101, 1), 0, 1);                     // Indicateur de traitement AMC(carte vitale ou autre support)
 fse.SetMember(11,1321, m_pC_Vitale->GetMember(12,101, 1), 0, 1);                     // Identifiant hote gestionnaire (carte vitale table conversion ou autre support)
 fse.SetMember(12,1321, m_pC_Vitale->CodeRegimeToOrganismeDestinataire(), 0, 1);      // Identifiant de l'assuré AMC (carte vitale ou autre support AMC)
 fse.SetMember(13,1321, m_pC_Vitale->GetMember(10,101, 1), 0, 1);                     // Zone supplémentaire non certifiée (Certifiée ou non certifiée si type de contrat 99"
 */
 //.............. Monitorer le resultat ...........................
 res = fse.Serialize();
// textEdit_MonFSE->setText(res);
 GroupToListView(listView_FseGroup, res);
}

//----------------------------------- puspushButton_Vitale_clicked ------------------------------------------
void C_Dlg_FseDefine::GroupToListView(QListView *pQListView, const QString &serializeData)
{pQListView->clear();
 char *pt                           = (char*)(const char*)serializeData;
 char *pt_var                       = 0;
 QListViewItem *pQListViewItemGroup = 0;
 QListViewItem *pQListViewItemChamp = 0;
 QString section                    = "";
 QString varName                    = "";
 QString s_Occ                      = "";
 QString s_chp                      = "";
 QString s_Val                      = "";
 QString libelle                    = "";

 while ( (pt = CGestIni::Param_GotoNextSection(pt, 0, &section))&&*pt)   // section [FSE_GR-1120-OCC-0001]
       {if (section.mid(3,3) == "_GR")
           {s_Val               = section.mid(7,4);
            s_Occ               = section.mid(16,4);
            pQListViewItemGroup = new QListViewItem(pQListView,
                                                    tr("Groupe N° ") + s_Val,        // type, Valeur,  Libellé , occurence
                                                    s_Val,
                                                    CSesam_API_InfoGroupe::NumGroupeToString(s_Val.toInt()),
                                                    s_Occ);
            if (pQListViewItemGroup)
               {int chp = 1;
                pt_var  = pt;                                   //m_Num-9 = 94 , 9 , Clé du NIR
                while ( (pt_var = CGestIni::Param_ExtraireNextValeurs(pt_var, varName, (char**)0, (long*)0,  &s_Val, &libelle)) && *pt_var && *pt_var !='[' )
                      {pQListViewItemChamp = new QListViewItem(pQListViewItemGroup,
                                                               tr("Champ N° ") + QString::number(chp++).rightJustify (2, '0'),        // type, Valeur,  Libellé , occurence
                                                               s_Val,
                                                               libelle,
                                                               "");
                       if (pQListViewItemChamp)  pQListViewItemChamp->setRenameEnabled ( 1, TRUE );
                      } //endwhile ( (pt_var = CGestIni::Param_ExtraireNextValeurs(
                pQListViewItemGroup->setOpen ( TRUE );
               } //endif (pQListViewItemGroup)
           }//endif (section.mid(3,3) == "_GR")
       } //endwhile ( (pt = CGestIni::Param_GotoNextSection(
}

//----------------------------------- listView_FseGroup_doubleClicked ------------------------------------------
void C_Dlg_FseDefine::listView_FseGroup_doubleClicked(QListViewItem *pQListViewItem, const QPoint &, int)
{if (pQListViewItem->text(0).left(1) != "G")
    {pQListViewItem->startRename ( 1 );
    }
}

//----------------------------------- puspushButton_Vitale_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButton_Vitale_clicked()
{    QTimer  timer;
     QString data_Vitale;
     //..................... afficher le popup .......................................................................
     setEnabledReadState(FALSE);
     m_pCFseVitaleApp->CouCouUp("READ_VITALE","Lecture carte Vitale en cours....");
     //................... on crée un thread de lecture sesam-vitale .................................................

     CVitaleReadThread *pCVitaleReadThread = new CVitaleReadThread(m_pCFseVitaleApp->m_PathExchg,
                                                                   "READ_VITALE",
                                                                   m_pCFseVitaleApp->m_PathLog,
                                                                   m_pCFseVitaleApp->m_PathGalss,
                                                                   lineEdit_CodePorteur->text(),
                                                                   m_pCFseVitaleApp->m_RessPS,
                                                                   m_pCFseVitaleApp->m_RessLecteur);
     pCVitaleReadThread->start();                     // on le lance
     //....................... tant que le thread de lecture sesam ...................................................
     //                        n'a pas fini on execute les evenements
     //                        afin de laiser les messages systèmes passer. (expl pour que le popup s'affiche)
     timer.start(1*1000, FALSE);
     while ( pCVitaleReadThread->IsThreadRun() )
           { m_pCFseVitaleApp->eventLoop()->processEvents ( QEventLoop::WaitForMore );
           }
     pCVitaleReadThread->wait();                     // on quitte proprement le thread
     delete pCVitaleReadThread;                      // on detruit le thread
     //........................ faire redescendre le popup ...........................................................
     m_pCFseVitaleApp->CouCouDown();
     setEnabledReadState(TRUE);

     //........................ recuperer données de lecture ........................................................
     QString val1;
     CGestIni::Param_UpdateFromDisk(m_pCFseVitaleApp->m_PathExchg, data_Vitale);
     CGestIni::Param_ReadParam(data_Vitale, "ERR_CODE", "m_Erreur",&val1);
     if (val1.length())
        {QMessageBox::warning ( this, tr ("ERREUR DE LECTURE ! "),
                                tr ("<b><u>Erreur de lecture de la carte Vitale: </u></b><br><i>") + val1 +("</i>"),
                                tr ("&Opération annulée"),0,0,
                                0, 1 );
         return;
        }
    C_Vitale_ToInterface(data_Vitale);

}

//----------------------------------- C_Vitale_ToInterface ------------------------------------------
void C_Dlg_FseDefine::C_Vitale_ToInterface(const QString data_Vitale)
{    if (m_pC_Vitale) delete m_pC_Vitale;
     m_pC_Vitale = new C_Vitale(data_Vitale);
     textEdit_MonVitale->setText(data_Vitale);

     //........................ remplir la listbox ........................................................
     int last_occurence = m_pC_Vitale->GetNbOccurences(104) + 1;   //option base 1
     int occurence;
     listBox_Vitale->clear();
     for (occurence=1; occurence<last_occurence; ++occurence)
         {/*C_VitaleListBoxItem *pC_VitaleListBoxItem = */new C_VitaleListBoxItem( listBox_Vitale,
                                                                               m_pC_Vitale->GetMember(1,   104, occurence),    // nom
                                                                               m_pC_Vitale->GetMember(3,   104, occurence),    // prenom
                                                                               m_pC_Vitale->GetMember(12,  104, occurence),    // date de naissance
                                                                               m_pC_Vitale->GetMember(13,  104, occurence),    // rang de naissance
                                                                               QString::number(   occurence)
                                                                            );
         }
     if (listBox_Vitale->count())
        {listBox_Vitale->setSelected ( 0, TRUE );
         QListBoxItem *pQListBoxItem =  listBox_Vitale->item (0);
         if (pQListBoxItem) {listBox_Vitale_clicked(pQListBoxItem);
                             setSecurisationState(3);
                            }
        }
}

//----------------------------------- listBox_Vitale_clicked ------------------------------------------
void C_Dlg_FseDefine::listBox_Vitale_clicked(QListBoxItem *pQListBoxItem)
{if (pQListBoxItem==0) return;
 C_VitaleListBoxItem *pC_VitaleListBoxItem = (C_VitaleListBoxItem*) pQListBoxItem;
 int occurence   = pC_VitaleListBoxItem->m_Occurence.toInt();    // occurence à laquelle correspond l'item de la listbox
 QString  dnss   = m_pC_Vitale->GetMember(12,   104, occurence);
 QString  nomP   = m_pC_Vitale->GetMember(2,    104, occurence);
 QString  adresseligne_1 = m_pC_Vitale->GetMember(4,    104, occurence);
 QString  adresseligne_2 = m_pC_Vitale->GetMember(5,    104, occurence);
 QString  adresseligne_3 = m_pC_Vitale->GetMember(6,    104, occurence);
 QString  adresseligne_4 = m_pC_Vitale->GetMember(7,    104, occurence);
 QString  villeCodePost  = m_pC_Vitale->GetMember(8,    104, occurence);
 QString  qualite        = m_pC_Vitale->GetMember(14,   104, occurence);
 QString  codeRegime     = m_pC_Vitale->GetMember(10,101, 1);
 QString  caisseGest     = m_pC_Vitale->GetMember(11,101, 1);
 QString  centreGest     = m_pC_Vitale->GetMember(12,101, 1);

 //................... determination de la nature du porteur de la carte ..............................
 QString str,codeCentreInfo;
 str += tr("<br><b>--- Le N° de sécurité sociale est celui de l'") + ((m_pC_Vitale->GetMember(7,101, 1)=="A")?TR("assuré"):TR("ayant droit")) + " ---</b>";
 str += tr(" (Occurence : ")                         + pC_VitaleListBoxItem->m_Occurence + ")";
 str += tr("<br><b>Le bénéficiaire actuel </b>")     + ((occurence == 1)?TR("est l'assuré"):TR("n'est pas l'assuré"));
 if (qualite.toInt()) str += tr("<br><b>Qualité (parenté) de l'assuré </b>") + m_pC_Vitale->CodeQualiteToName(qualite);
 str += tr("<br><b>Nom usuel : </b>")                + m_pC_Vitale->GetMember(1,    104, occurence);
 if (nomP.length()) str += tr("<br><b>Nom patronymique : </b>")  + nomP;
 str += tr("<br><b>Prénom : </b>")                   + m_pC_Vitale->GetMember(3,    104, occurence);
 if (adresseligne_1.length()||
     adresseligne_2.length()||
     adresseligne_3.length()||
     adresseligne_4.length()||
     villeCodePost.length()
    )
    {str += tr("<br><b>Adresse : </b>");
     if (adresseligne_1.length()) str += "<br>&nbsp;&nbsp;&nbsp;" + adresseligne_1;
     if (adresseligne_2.length()) str += "<br>&nbsp;&nbsp;&nbsp;" + adresseligne_2;
     if (adresseligne_3.length()) str += "<br>&nbsp;&nbsp;&nbsp;" + adresseligne_3;
     if (adresseligne_4.length()) str += "<br>&nbsp;&nbsp;&nbsp;" + adresseligne_4;
     if (villeCodePost.length())  str += "<br>&nbsp;&nbsp;&nbsp;" + villeCodePost;
    }
 str += tr("<br><b>Date de naissance : </b>")        + dnss.mid(6,2) + "/" + dnss.mid(4,2)+ "/" + dnss.mid(0,4) + tr(" <b>Rang :</b> ")  + m_pC_Vitale->GetMember(13, 104, occurence);
 str += tr("<br><b>Numéro SS : </b>")                + m_pC_Vitale->GetMember(8,    101, 1) + tr(" <b>Clé :</b> ")  + m_pC_Vitale->GetMember(9,    101, 1);
 str += tr("<br><b>Code Régime N° : </b>")           + m_pC_Vitale->CodeRegimeToName(m_CaisseFileData);
 str += tr("<br><b>Caisse Gestionnaire : </b>")      + caisseGest;
 str += tr("<br><b>Centre Gestionnaire : </b>")      + centreGest;
 str += tr("<br><b>Code Gestion : </b>")             + m_pC_Vitale->GetMember(13,101, 1);
 str += tr("<br><b>Organisme destinataire : </b>")   + m_pC_Vitale->CodeRegimeToOrganismeDestinataire(codeCentreInfo);
 str += tr("<br><b>Code centre informatique : </b>") + codeCentreInfo;


 str += tr("<br><b>.... Recherche complémentaire dans le fichier <i>'orgdest.csv'</i> ....</b>");
 C_Organisme organisme(m_CaisseFileData, m_pC_Vitale->GetMember(10,101, 1), m_pC_Vitale->GetMember(11,101, 1));
 if ( organisme.m_Error.length() )
    { str += tr("<br><b>Erreur : </b>") + organisme.m_Error;
    }
 else
    { str += tr("<br><b>Nom caisse : </b>")               + organisme.m_Name;
      str += tr("<br><b>Code Regime : </b>")              + organisme.m_CodeRegime;
      str += tr("<br><b>Caisse Gestionnaire : </b>")      + organisme.m_CaisseGestionnaire;
      str += tr("<br><b>Centre Gestionnaire : </b>")      + organisme.m_CentreGestionnaire;
      str += tr("<br><b>Organisme Destinataire : </b>")   + organisme.m_OrganismeDestinataire;
      str += tr("<br><b>Code centre informatique : </b>") + organisme.m_CodeCentreInformatique;
      if (organisme.m_Regime.length())  str += tr("<br><b>Régime : </b>") + organisme.m_Regime;
      if (organisme.m_Adresse_1.length() || organisme.m_Adresse_2.length())
         {str += tr("<br><b>Adresse : </b>") ;
          if (organisme.m_Adresse_1.length())    str += organisme.m_Adresse_1  + " ";
          if (organisme.m_Adresse_2.length())    str += organisme.m_Adresse_2;
         }
      if (organisme.m_CodePostal.length())   str += tr("<br><b>Code Postal : </b>") + organisme.m_CodePostal;
      if (organisme.m_Ville.length())        str += tr("<br><b>Ville : </b>")       + organisme.m_Ville;
      if (organisme.m_Cedex.length())        str += tr("<br><b>Cedex : </b>")       + organisme.m_Cedex;
      if (organisme.m_Tel_1.length())        str += tr("<br><b>Tél 1 : </b>")       + organisme.m_Tel_1;
      if (organisme.m_Fax.length())          str += tr("<br><b>Fax : </b>")         + organisme.m_Fax;
    }

setComboOnValue(comboBox_AMO_IndicTTT, m_pC_Vitale->CodeIndicTT_MUT_ToName(m_pC_Vitale->GetMember(15, 104, occurence)));

//lineEdit_AMO_CodeService

/*
 [VIT_GR-109-OCC-1]
; Groupe : 0109  (Données de la Complémentaire de santé (0 à 1 occurence par bénéficiaire))
  m_Num-1 = 0223490000 , 1 , Numéro complémentaire B2
  m_Num-2 =  , 2 , Numéro complémentaire EDI
  m_Num-3 =  , 3 , Numéro d'adhérent AMC
  m_Num-4 = 33 , 4 , Indicateur de traitement AMC (service accepté par AMC : Tiers payant, CMU, DRE etc..)
  m_Num-5 = 200001010000 , 5 , Date de début de validité de présence des données AMC
  m_Num-6 = 200512310000 , 6 , Date de fin de validité de présence des données AMC
  m_Num-7 = MA , 7 , Code de routage AMC
  m_Num-8 = 349 , 8 , Identifiant d'hôte
  m_Num-9 = RSS.FR , 9 , Nom de domaine AMC
  m_Num-10 =  , 10 , Code aiguillage STS
  m_Num-11 =  , 11 , Type de services associés
  m_Num-12 =  , 12 , Services associés au contrat
*/
 radioButton_GestionUnique_StateChanged ( QButton::Off );
 radioButton_GestionSeparee_StateChanged ( QButton::Off );

 clear_AMC_View();
 QString  amcNumComp = m_pC_Vitale->GetMember(1, 109, occurence);
 if (amcNumComp.length())
    { QString dtDeb, dtFin, codeServ, numAd, numOrg, codeAigu;
      str      += tr("<br><br><b>....Données AMC....</b>");
      numAd     =  m_pC_Vitale->GetMember(3,  109, occurence);
      codeServ  =  m_pC_Vitale->GetMember(4,  109, occurence);
      dtDeb     =  m_pC_Vitale->GetMember(5,  109, occurence);
      dtFin     =  m_pC_Vitale->GetMember(6,  109, occurence);
      codeAigu  =  m_pC_Vitale->GetMember(10, 109, occurence);
      str += tr("<br><b>Numéro d'organisme complémentaire B2 : </b>") + amcNumComp;
      str += tr("<br><b>Numéro d'adhérent : </b>") + ((numAd.length()) ? numAd : TR("non renseigné"));
      if (dtDeb.length()){ dtDeb = dtDeb.mid(6,2) + "/" + dtDeb.mid(4,2)+ "/" + dtDeb.mid(0,4);   str += tr("<br><b>Date de début des droits : </b>") + dtDeb;};
      if (dtFin.length()){ dtFin = dtFin.mid(6,2) + "/" + dtFin.mid(4,2)+ "/" + dtFin.mid(0,4);   str += tr("<br><b>Date de fin des droits : </b>")   + dtFin;};
      if (codeServ.length()) str += tr("<br><b>Indicateur de traitement : </b>") + m_pC_Vitale->CodeIndicTT_AMC_ToName(codeServ );
      if (codeAigu.length()) str += tr("<br><b>Code aiguillage STS : </b>") + m_pC_Vitale->Code_AiguillageSTS_ToName(codeAigu );
      lineEdit_AMC_NumOrga->setText(amcNumComp);
      lineEdit_AMC_Adherent->setText(numAd);
      lineEdit_AMC_DtDebDroits->setText(dtDeb);
      lineEdit_AMC_DtFinDroits->setText(dtFin);
      setComboOnValue(comboBox_AMC_IndicTTT, m_pC_Vitale->CodeIndicTT_AMC_ToName(codeServ ));
      lineEdit_AMC_CodeRoutage->setText(m_pC_Vitale->GetMember(7, 109, occurence));
      lineEdit_AMC_IdHote->setText(m_pC_Vitale->GetMember(8, 109, occurence));
      lineEdit_AMC_NomDomaine->setText(m_pC_Vitale->GetMember(9, 109, occurence));
      lineEdit_AMC_AiguillageSTS->setText(m_pC_Vitale->GetMember(10, 109, occurence));
      lineEdit_AMC_ServicesAssocType->setText(m_pC_Vitale->GetMember(11, 109, occurence));
      lineEdit_AMC_ServicesAssocContrat->setText(m_pC_Vitale->GetMember(12, 109, occurence));
      //radioButton_GestionSeparee_StateChanged ( QButton::On );
      //radioButton_GestionSeparee->setDown ( TRUE );
      buttonGroup_ModeGestion->setButton(1);
      radioButton_GestionUnique_StateChanged ( QButton::Off );
      radioButton_GestionSeparee_StateChanged ( QButton::On);
      //textLabel_AMC_SesamVitale->setText(tr("<b>SESAM VITALE </b> ") + );
    }
/*
  m_Num-1 = 02400000 , 1 , Identification mutuelle
  m_Num-2 = OOOOONON , 2 , Garanties effectives
  m_Num-3 =  , 3 , Indicateur de traitement mutuelle
  m_Num-4 =  , 4 , Type de services associés
  m_Num-5 =  , 5 , Services associés au contrat
  m_Num-6 =  , 6 , Code aiguillage STS
*/
 clear_MUT_View();
 QString  mutNum = m_pC_Vitale->GetMember(1, 107, occurence);
 if (mutNum.length())
    { QString dtDeb, codeServ, dtFin, garant, servTyp, servAsso, aigui;
      str      +=  tr("<br><br><b>....Données Mutuelle....</b>");
      garant    =  m_pC_Vitale->GetMember(2, 107, occurence);
      codeServ  =  m_pC_Vitale->GetMember(3, 107, occurence);
      servTyp   =  m_pC_Vitale->GetMember(4, 107, occurence);
      servAsso  =  m_pC_Vitale->GetMember(5, 107, occurence);
      aigui     =  m_pC_Vitale->GetMember(6, 107, occurence);
      str      +=  tr("<br><b>Numéro de mutuelle : </b>") + mutNum;

      //str += tr("<br><b>Numéro d'adhérent : </b>") + ((numAd.length()) ? numAd : TR("non renseigné"));
      //if (dtDeb.length()){ dtDeb = dtDeb.mid(6,2) + "/" + dtDeb.mid(4,2)+ "/" + dtDeb.mid(0,4);   str += tr("<br><b>Date de début des droits : </b>") + dtDeb;};
      //if (dtFin.length()){ dtFin = dtFin.mid(6,2) + "/" + dtFin.mid(4,2)+ "/" + dtFin.mid(0,4);   str += tr("<br><b>Date de fin des droits : </b>")   + dtFin;};
      //if (codeServ.length()) str += tr("<br><b>Indicateur de traitement : </b>") + m_pC_Vitale->CodeIndicTT_AMC_ToName(codeServ );
      QString orgaComplName = "";
      QString numOrgaCompl   = C_Vitale::Mutnum_To_Complementaire(m_CorrespTableData,
                                                                  mutNum,
                                                                  codeRegime,
                                                                  caisseGest,
                                                                  centreGest,
                                                                  &orgaComplName);
      str += tr("<br><b>.... Recherche dans la table de correspondance ....</b>");
      if (numOrgaCompl.length())
         {str      += tr("<br><b>N° d'organisme complémentaire : </b>") + numOrgaCompl;
          str      += tr("<br><b>Nom : </b>") + orgaComplName;
          lineEdit_MUT_Nom->setText(orgaComplName);
         }
      else
         {str      += "<br><b>" + orgaComplName + "</b>";
         }
      lineEdit_MUT_Numero->setText(mutNum);
      lineEdit_MUT_NumeroOrga->setText(numOrgaCompl);
      setComboOnValue(comboBox_MUT_IndicTTT, m_pC_Vitale->CodeIndicTT_MUT_ToName(codeServ ));
      lineEdit_MUT_Garanties->setText(garant);
      lineEdit_MUT_TypeService->setText(servTyp);
      lineEdit_MUT_ServicesAsso->setText(servAsso);
      lineEdit_MUT_AiguillageSTS->setText(aigui);
      //lineEdit_MUT_DtDebDroits->setText("");
      //lineEdit_MUT_DtFinDroits->setText("");
      //radioButton_GestionUnique_StateChanged ( QButton::On );
      //radioButton_GestionUnique->setDown ( TRUE );
      buttonGroup_ModeGestion->setButton(0);
      radioButton_GestionUnique_StateChanged ( QButton::On );
      radioButton_GestionSeparee_StateChanged ( QButton::Off);
    }
 // lineEdit_AMO_CodeService
 textBrowser_Beneficiaire->setText(str);
}

//----------------------------------- clear_MUT_View() ------------------------------------------
void C_Dlg_FseDefine::clear_MUT_View()
{
  lineEdit_MUT_Numero->setText("");
  lineEdit_MUT_NumeroOrga->setText("");
  lineEdit_MUT_Nom->setText("");
  lineEdit_MUT_Garanties->setText("");
  setComboOnValue(comboBox_MUT_IndicTTT, "  ");
  lineEdit_MUT_TypeService->setText("");
  lineEdit_MUT_ServicesAsso->setText("");
  lineEdit_MUT_AiguillageSTS->setText("");
  lineEdit_MUT_DtDebDroits->setText("");
  lineEdit_MUT_DtFinDroits->setText("");

}

//----------------------------------- clear_AMC_View() ------------------------------------------
void C_Dlg_FseDefine::clear_AMC_View()
{     lineEdit_AMC_NumOrga->setText("");
      lineEdit_AMC_Adherent->setText("");
      lineEdit_AMC_DtDebDroits->setText("");
      lineEdit_AMC_DtFinDroits->setText("");
      setComboOnValue(comboBox_AMC_IndicTTT, "  ");
      lineEdit_AMC_CodeRoutage->setText("");
      lineEdit_AMC_IdHote->setText("");
      lineEdit_AMC_NomDomaine->setText("");
      lineEdit_AMC_AiguillageSTS->setText("");
      lineEdit_AMC_ServicesAssocType->setText("");
      lineEdit_AMC_ServicesAssocContrat->setText("");
}

//----------------------------------- setComboOnValue ------------------------------------------
void C_Dlg_FseDefine::setComboOnValue(QComboBox* pQComboBox, const QString &value)
{QListBoxItem *pQListBoxItem  = pQComboBox->listBox()->findItem (value, Qt::ExactMatch );
 if (pQListBoxItem)
    {int index = pQComboBox->listBox()->index( pQListBoxItem );
     if (index !=-1) pQComboBox->setCurrentItem ( index );
    }
}

//----------------------------------- setSecurisationState ------------------------------------------
void C_Dlg_FseDefine::setSecurisationState(int state)
{buttonGroup_FSE_Securisation->setButton (state);
}

//----------------------------------- pushButton_CPS_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButton_CPS_clicked()
{    QTimer  timer;
     QString data_CPS;

     //..................... afficher le popup .......................................................................
     setEnabledReadState(FALSE);
     m_pCFseVitaleApp->CouCouUp("READ_CPS","Lecture carte CPS en cours....");
     //................... on crée un thread de lecture sesam-vitale .................................................

     CVitaleReadThread *pCVitaleReadThread = new CVitaleReadThread(m_pCFseVitaleApp->m_PathExchg,
                                                                   "READ_CPS",
                                                                   m_pCFseVitaleApp->m_PathLog,
                                                                   m_pCFseVitaleApp->m_PathGalss,
                                                                   lineEdit_CodePorteur->text(),
                                                                   m_pCFseVitaleApp->m_RessPS,
                                                                   m_pCFseVitaleApp->m_RessLecteur);
     pCVitaleReadThread->start();                     // on le lance
     //....................... tant que le thread de lecture sesam ...................................................
     //                        n'a pas fini on execute les evenements
     //                        afin de laiser les messages systèmes passer. (expl pour que le popup s'affiche)
     timer.start(1*1000, FALSE);
     while ( pCVitaleReadThread->IsThreadRun() )
           { m_pCFseVitaleApp->eventLoop()->processEvents ( QEventLoop::WaitForMore );
           }
     pCVitaleReadThread->wait();                     // on quitte proprement le thread
     delete pCVitaleReadThread;                      // on detruit le thread
     //........................ faire redescendre le popup ...........................................................
     m_pCFseVitaleApp->CouCouDown();
     setEnabledReadState(TRUE);

     //........................ recuperer données de lecture ........................................................
     CGestIni::Param_UpdateFromDisk(m_pCFseVitaleApp->m_PathExchg, data_CPS);
     C_CPS_ToInterface(data_CPS);
}


//----------------------------------- C_CPS_ToInterface ------------------------------------------
void C_Dlg_FseDefine::C_CPS_ToInterface(const QString &data_CPS )
{     /*
       QString old_Spe = "" ;
       if (m_pC_Cps)
          {old_Spe = m_pC_Cps->m_CodeSpecialite;
           delete m_pC_Cps;
          }
       m_pC_Cps = new C_Cps("", data_CPS);
       textEdit_MonCPS->setText(data_CPS);
       textBrowser_CPS->setText(CPS_To_Html_Display(m_pC_Cps));
       if (m_pC_Cps && m_pC_Cps->m_CodeSpecialite != old_Spe) init_listBox_ActesDispos();
       */

     QString val1;
     CGestIni::Param_ReadParam(data_CPS, "ERR_CODE", "m_Erreur",&val1);
     if (val1.length())
        {QMessageBox::warning ( this, tr ("ERREUR DE LECTURE ! "),
                                tr ("<b><u>Erreur de lecture de la carte CPS: </u></b><br><i>") + val1 +("</i>"),
                                tr ("&Opération annulée"),0,0,
                                0, 1 );
         return;
        }
     QString old_Spe = "" ;
     CGestIni::Param_ReadParam(data_CPS, "CPS_GR2", "m_ModeExe",&val1);
     if (val1=="4")
        {if (m_pC_CpsRempla) delete m_pC_CpsRempla;
         m_pC_CpsRempla = new C_Cps(lineEdit_CodePorteur->text(), data_CPS);
         textEdit_MonCPS->setText(data_CPS);
         textBrowser_CPS_Remplacant->setText(CPS_To_Html_Display(m_pC_CpsRempla));
         groupBox_PS_Remplacant->show();
         groupBox_PS_Titulaire->show();
         pushButtonLoadCPS_Titulaire->setEnabled(TRUE);
        }
     else
        {
         if (m_pC_Cps)
          {old_Spe = m_pC_Cps->m_CodeSpecialite;
           delete m_pC_Cps;
          }
         if (m_pC_CpsRempla) delete m_pC_CpsRempla;
         groupBox_PS_Remplacant->hide();
         groupBox_PS_Titulaire->show();
         m_pC_Cps = new C_Cps(lineEdit_CodePorteur->text(), data_CPS);
         textEdit_MonCPS->setText(data_CPS);
         textBrowser_CPS->setText(CPS_To_Html_Display(m_pC_Cps));
         pushButtonSaveCPS_Titulaire->setEnabled(TRUE);
         pushButtonLoadCPS_Titulaire->setEnabled(FALSE);
        }
    if (m_pC_Cps && m_pC_Cps->m_CodeSpecialite != old_Spe) init_listBox_ActesDispos();
}

//----------------------------------- CPS_To_Html_Display ------------------------------------------
QString C_Dlg_FseDefine::CPS_To_Html_Display(C_Cps *pC_Cps )
{
     QString str;
     str += tr("<br><b> --- Données de la carte CPS --- </b>");
     //str += tr("<br><b>Type de support : </b>")          + pC_Cps->codeTypeDeCarte_ToString(m_pC_Cps->GetMember(1,1).toInt());
     str += tr("<br><b>Civilité : </b>")                 + pC_Cps->codeCiviliteToString(pC_Cps->GetMember(5,1).toInt());
     str += tr("<br><b>Nom : </b>")                      + pC_Cps->GetMember(6,    1);
     str += tr("<br><b>Prénom : </b>")                   + pC_Cps->GetMember(7,    1);
     str += tr("<br><b>Type de carte PS: </b>")          + pC_Cps->codeTypeCartePS_ToString(pC_Cps->GetMember(1,1).toInt());
     str += tr("<br><b>")+pC_Cps->codeType_ID_Nationale_ToString(pC_Cps->GetMember(2,1).toInt())+" : </b>"+ pC_Cps->GetMember(3,1) + " "+pC_Cps->GetMember(4,1);
     QString spe = pC_Cps->GetMember(14,2);
     if (spe != "99") str += tr("<br><b>Spécialité : </b>") + pC_Cps->codeSpecialiteToString(spe.toInt());
     str += tr("<br><b>Secteur d'activité : </b>")          + pC_Cps->codeSecteurActivite_ToString(pC_Cps->GetMember(4,2).toInt());
     str += tr("<br><b>Mode d'exercice : </b>")             + pC_Cps->codeModeExercice_ToString(pC_Cps->GetMember(2,2).toInt());
     str += tr("<br><b>Peut signer une facture : </b>")     + ((pC_Cps->GetMember(20,2).toInt()==1)?TR("oui"):TR("non"));
     str += tr("<br><b>Peut signer un lot : </b>")          + ((pC_Cps->GetMember(21,2).toInt()==1)?TR("oui"):TR("non"));
     str += tr("<br><b>Convention : </b>")                  + pC_Cps->codeConvention_ToString(pC_Cps->m_CodeConvention.toInt());
     return str;

}

//----------------------------------- pushButton_Annuler_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButton_Annuler_clicked()
{reject();
}

//----------------------------------- pushButton_OK_clicked ------------------------------------------
void C_Dlg_FseDefine::pushButton_OK_clicked()
{
}

//----------------------------------- reject ------------------------------------------
void C_Dlg_FseDefine::reject()
{ QDialog::reject();
}

//----------------------------------- accept ------------------------------------------
void C_Dlg_FseDefine::accept()
{ QDialog::accept();
}



