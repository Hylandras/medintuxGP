/********************************* C_Dlg_personnes.cpp ****************************
 *  #include "C_Dlg_personnes.h"        QT4-ready                                 *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a  l'Energie Atomique                                           *
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


#include "C_Dlg_personnes.h"
#include "ui/ui_Dlg_personnes.h"
#include "../../MedinTuxTools-QT4/C_Dlg_DoList/C_KeyPressEater.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"

#include "C_App.h"
#include <QSqlQuery>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QList>
#include <QMessageBox>
#include <QFileInfo>
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QHeaderView>
#include <QProcess>
#include <QFileInfo>
#include <QIcon>

#define ST_LIST_MAX      "2000"
//--------------------------------------- C_Dlg_personnes ----------------------------------------------------------
C_Dlg_personnes::C_Dlg_personnes(QWidget *parent)
    : QDialog(parent), m_pUI(new Ui::Dlg_personnes)
{
    m_pUI->setupUi(this);
    m_LastPk = "";
    m_Apropos_Proc = 0;
    m_pUI->lineEdit_CodePostal->setInputMask ("99 999");
    m_pUI->lineEdit_Tel_1->setInputMask ("99 99 99 99 99 99 99");
    m_pUI->lineEdit_Tel_2->setInputMask ("99 99 99 99 99 99 99");
    m_pUI->lineEdit_Tel_3->setInputMask ("99 99 99 99 99 99 99");
    //........................ une petite deco (le bandeau) ...............................
    m_pUI->label_Bandeau->setPixmap (QPixmap( G_pCApp->getPathAppli() + "/Ressources/Bandeau Personnes.png" ));
    m_pUI->label_Bandeau->setScaledContents ( TRUE );
    m_pUI->m_action_A_Propos->setIcon ( QIcon (Theme::getIcon("22x22/help.png"))) ;
    //........................ initaliser les listes ......................................
    InitAllComboTelType();
    InitComboSpec();
    initListesMedecin("", "");
    //......................... brancher ..................................................
    connect( m_pUI->lineEdit_FiltreListe,      SIGNAL( textChanged(const QString &)),                   this, SLOT( Slot_FiltrerListe(const QString &)) );
    connect( m_pUI->comboBox_FiltreListe,      SIGNAL( currentIndexChanged(int)),                       this, SLOT( Slot_comboBox_FiltreListe_currentIndexChanged(int)) );
    connect( m_pUI->treeWidget_ListeMedecins,  SIGNAL( itemClicked ( QTreeWidgetItem * , int  )),       this, SLOT( Slot_treeWidget_ListeMedecins_itemClicked( QTreeWidgetItem * , int)) );
    connect( m_pUI->treeWidget_ListeMedecins,  SIGNAL( itemDoubleClicked ( QTreeWidgetItem * , int  )), this, SLOT( Slot_treeWidget_ListeMedecins_itemDoubleClicked( QTreeWidgetItem * , int)) );
    connect( m_pUI->treeWidget_ListeMedecins,  SIGNAL( itemSelectionChanged ()) ,                       this, SLOT( Slot_treeWidget_ListeMedecins_itemSelectionChanged()) );
    connect( m_pUI->pushButtonOK,              SIGNAL( clicked ()) ,                                    this, SLOT( Slot_pushButtonOK_clicked()) );
    connect( m_pUI->quitPushButton,            SIGNAL( clicked ()) ,                                    this, SLOT( Slot_quitPushButton_clicked()) );
    connect( m_pUI->pushButton_Effacer,        SIGNAL( clicked ()) ,                                    this, SLOT( Slot_pushButton_Effacer_clicked()) );
    connect( m_pUI->pushButton_NouvelleFiche,  SIGNAL( clicked ()) ,                                    this, SLOT( Slot_pushButton_NouvelleFiche_clicked()) );
    connect( m_pUI->pushButton_Enregistrer,    SIGNAL( clicked ()) ,                                    this, SLOT( Slot_pushButton_Enregistrer_clicked()) );
    connect( m_pUI->m_action_A_Propos,         SIGNAL( clicked()),                                      this, SLOT( Slot_pushButton_Apropos_clicked()) );
    //................... effacer le header des identifiants ...................
    QHeaderView *pQHeaderView = m_pUI->treeWidget_ListeMedecins->header();
    pQHeaderView->hideSection (2);
    pQHeaderView->hideSection (3);

    //........................ connecter les slot de verification ...........................................................................................
    connect( m_pUI->lineEdit_Ville,      SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),         this,     SLOT(   Slot_OutFocusVille(QFocusEvent*, int &))  );
    connect( m_pUI->lineEdit_CodePostal, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),         this,     SLOT(   Slot_OutFocusCodePostal(QFocusEvent*, int &))  );
    connect( m_pUI->lineEdit_CodePostal, SIGNAL( Sign_keyPressEvent(QKeyEvent *,  int &)),         this,     SLOT(   Slot_CharEventCodePostal(QKeyEvent *, int &)) );
    //................... un petit bidouillage accelerateur ...........................................................
    //                    pour relier fleche haute, basse, return de la zonne d'edition
    //                    au QTreeWidget pour faire monter et descendre la selection
    C_KeyPressEater *keyPressEater = new C_KeyPressEater( m_pUI->treeWidget_ListeMedecins, this , "C_Dlg_personnes::C_KeyPressEater");
    m_pUI->lineEdit_FiltreListe->installEventFilter( keyPressEater );
    if (m_pUI->treeWidget_ListeMedecins->topLevelItemCount())
       {if (selectMedecinById(G_pCApp->getStartID())==0)
           {m_pUI->treeWidget_ListeMedecins->topLevelItem ( 0 )->setSelected(TRUE);
           }
       }
    m_pUI->textEdit_Changements->hide();    // juste la pour contenir le texte ï¿œ envoyer au gestionnaire de  a propos
    QList <int> list;                                                   // reduire le spliter des listes
    list.append(200);
    list.append(width()-200);
    m_pUI->splitter->setSizes(list);
    setWindowTitle(tr("Directory - Data Medical Design - Version:")+G_pCApp->m_NUM_VERSION.remove("@").remove("#").remove("="));
    m_pUI->pushButtonOK->setEnabled(G_pCApp->argc() > 1);
    getAndSetPositions();
    QTimer::singleShot(500, this, SLOT(Slot_ActivateMainWindow())); // pour avoir la fenetre en avant plan sur ubuntu unity
}
//--------------------------------------- Slot_ActivateMainWindow ---------------------------------------------------
void C_Dlg_personnes::Slot_ActivateMainWindow()
{   //if (G_pCApp->getDroits().length()) setWindowFlags(Qt::WindowStaysOnTopHint);
    show();
    setWindowState(windowState() & ~Qt::WindowMinimized);
    activateWindow();
    raise();
}

//--------------------------------------- ~C_Dlg_personnes ----------------------------------------------------------
C_Dlg_personnes::~C_Dlg_personnes()
{   delete m_pUI;
}


//----------------------------------- Slot_pushButton_Apropos_clicked -----------------------------------------------------------------------
void C_Dlg_personnes::Slot_pushButton_Apropos_clicked()
{QTimer::singleShot ( 100, this,SLOT(Slot_actionAproposDisplay()) );
}

//------------------------ Slot_Apropos_Proc_finished -----------------------------------------
void C_Dlg_personnes::Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus)
{if (m_Apropos_Proc)
    {m_Apropos_Proc->terminate();
     m_Apropos_Proc->waitForFinished (5000);
     delete m_Apropos_Proc;
     m_Apropos_Proc = 0;
     m_pUI->m_action_A_Propos->setDisabled(FALSE);
    }
}
//----------------------------------- Slot_actionAproposDisplay -----------------------------------------------------------------------
void C_Dlg_personnes::Slot_actionAproposDisplay()
{       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        QString macAdr;
        //QString ipAdr  = G_pCApp->get_Current_IP_Adr(&macAdr);

        //CGestIni::Param_UpdateToDisk(G_pCApp->pathAppli()+"Ressources/Changements.html",m_pGUI->textEdit_Changements->toHtml()
        //                                                                                .replace("{{IP_ADR}}",  ipAdr)
        //                                                                                .replace("{{MAC_ADR}}", macAdr)
        //                                                                                .replace("{{PATH_INI}}",G_pCApp->pathIni()));
        QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).path());
        //QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).path())+"Ressources/";
        QStringList argList;

        //......................... completer les autres arguments .........................................
        argList << "personnes";                                                     // 1  nom du module
        argList << tr("Module for directory management");                           // 2  description courte
        argList << G_pCApp->m_NUM_VERSION.remove("@").remove("#").remove("=")+ "  Qt : " + QT_VERSION_STR;      // 3  numero de version
        argList << G_pCApp->getPathAppli()+"Ressources/Changements.html";                                       // 4  fichiers d�crivant les changements
        argList <<"";                                                                                           // 5  Icone par defaut
        argList <<"";                                                                                           // 6  aide en ligne (vide pour prendre celle par defaut)
        argList <<"";                                                                                           // 7  apropos (on met une chaine vide pour qu'il prenne celui par d?faut)
        argList << G_pCApp->getBDVersionNumber();                                                               // 8  numero de version de la base de donnee
        //QProcess::startDetached (pathExeAPropos, argList);

        if (m_Apropos_Proc==0)
           {m_pUI->m_action_A_Propos->setDisabled(TRUE);
            m_Apropos_Proc = new QProcess(this);
            connect( m_Apropos_Proc, SIGNAL(finished ( int,  QProcess::ExitStatus)),  this, SLOT(Slot_Apropos_Proc_finished (int,  QProcess::ExitStatus)) );
            connect( m_Apropos_Proc,  SIGNAL(error ( QProcess::ProcessError  )), G_pCApp, SLOT(Slot_error ( QProcess::ProcessError  )) );
            m_Apropos_Proc->start(pathExeAPropos, argList);
            m_Apropos_Proc->waitForStarted  (4000);
            //m_Apropos_Proc->waitForFinished (-1);     // crash crash bug QT connu sauf si -1 comme paramètre
            //..... pour contourner le bug on fait une boucle d'attente un peu sale ....
            /*
            G_pCApp->processEvents ();
            while ( (procState = m_Apropos_Proc->state())== QProcess::Running ) // && QFile::exists(pathBinRessources+"~A_propos.html")
                  { //qDebug(QString::number(procState).toAscii());
                    QApplication::processEvents ( QEventLoop::ExcludeUserInput );
                  }
            */
            //if (m_Apropos_Proc) delete m_Apropos_Proc;
            //m_Apropos_Proc = 0;
            //QFile::remove(pathBinRessources+"~A_propos.html");
            //m_action_A_Propos->setDisabled(FALSE);
           }
       QApplication::restoreOverrideCursor();
}

//--------------------------------------- tryToStopAPropos ----------------------------------------------------------
void C_Dlg_personnes::tryToStopAPropos()
{Slot_Apropos_Proc_finished (0,  QProcess::NormalExit);
 //if (m_Apropos_Proc==0) return;
 //m_Apropos_Proc->kill();   // terminate() ne fonctionne pas
}

/*



//----------------------------------- Slot_pushButton_Apropos_clicked -----------------------------------------------------------------------
void C_Dlg_personnes::Slot_pushButton_Apropos_clicked()
{        CGestIni::Param_UpdateToDisk(G_pCApp->getPathAppli()+"Ressources/Changements.html",m_pUI->textEdit_Changements->toHtml());
         QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).path());
         QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).path())+"Ressources/";
         QStringList argList;
         //......................... completer les autres arguments .........................................
         argList << "personnes";                                                     // 1  nom du module
         argList << tr("Module for directory management");                           // 2  description courte
         argList << G_pCApp->m_NUM_VERSION.remove("@").remove("#").remove("=")+ "  Qt : " + QT_VERSION_STR;      // 3  numero de version
         argList << G_pCApp->getPathAppli()+"Ressources/Changements.html";                                       // 4  fichiers d�crivant les changements
         argList <<"";                                                                                           // 5  Icone par defaut
         argList <<"";                                                                                           // 6  aide en ligne (vide pour prendre celle par defaut)
         argList <<"";                                                                                           // 7  apropos (on met une chaine vide pour qu'il prenne celui par d?faut)
         argList << G_pCApp->getBDVersionNumber();                                                               // 8  numero de version de la base de donnee
         if (m_Apropos_Proc==0)
            {m_Apropos_Proc = new QProcess(this);
             m_Apropos_Proc->start(pathExeAPropos, argList);
             m_Apropos_Proc->waitForStarted  (4000);
             m_Apropos_Proc->waitForFinished ();
             if (m_Apropos_Proc) delete m_Apropos_Proc;
             m_Apropos_Proc = 0;
             QFile::remove(pathBinRessources+"~A_propos.html");
            }
}

//--------------------------------------- tryToStopAPropos ----------------------------------------------------------
void C_Dlg_personnes::tryToStopAPropos()
{if (m_Apropos_Proc==0) return;
 m_Apropos_Proc->kill();   // terminate() ne fonctionne pas
}
*/
//--------------------------------------- Slot_pushButtonOK_clicked ----------------------------------------------------------
void C_Dlg_personnes::Slot_pushButtonOK_clicked()
{setExchangeFile();
 accept();
}

//--------------------------------------- Slot_quitPushButton_clicked ----------------------------------------------------------
void C_Dlg_personnes::Slot_quitPushButton_clicked()
{reject();
}

//-- CODE POSTAL --------------------------------- Slot_OutFocusCodePostal -----------------------------------------------------------------------
void C_Dlg_personnes::Slot_OutFocusCodePostal(QFocusEvent*, int &)
{tryToSetVilleFromCodePostal();
}

//------------------------------------------- Slot_CharEventCodePostal -----------------------------------------------------------------------
void C_Dlg_personnes::Slot_CharEventCodePostal(QKeyEvent *event, int &ret)
{ // on incorpore le caractere dans la QLineEdit car ce n'est pas encore fait
  // lors de l'appel de ce Slot par  C_QLineEdit
  m_pUI->lineEdit_CodePostal->doParentPressEvent(event);
  ret = 0;    // du coup plus besoin d'Ãªtre traitÃ© en retour de slot par C_QLineEdit
  ret = 0;    // du coup plus besoin d'être traité en retour de slot par C_QLineEdit
  //.......... maintenant  on  traite le code postal .................
  //           en appelant directement la fonction de contrôle
  tryToSetVilleFromCodePostal();
}

//------------------------------------------- DoPopupList -----------------------------------------------------------------------
QString  C_Dlg_personnes::DoPopupList(QLineEdit *pQLineEdit, QStringList &list)
{ QAction *pQAction;
  QMenu   *pQMenu = pQLineEdit->createStandardContextMenu ();
  pQMenu->clear ();
  for (int i=0; i<list.size(); ++i) pQAction  = pQMenu->addAction( list[i] );
  pQAction  =  pQMenu->exec( QCursor::pos() /* mapToGlobal (  QPoint ( x, y) )*/ );
  if (pQAction) return  pQAction->text();
  return QString::null;
}

//------------------------------------------- tryToSetVilleFromCodePostal -----------------------------------------------------------------------
void C_Dlg_personnes::tryToSetVilleFromCodePostal()
{   QStringList villeList;
    QString codePostal = m_pUI->lineEdit_CodePostal->text().remove(' ');
    if (codePostal.length()==5)
    {   G_pCApp->GetVilleFromCodePostal(codePostal, villeList);
        int nb = villeList.count();
        if (nb==1)
           {m_pUI->lineEdit_Ville->setText(villeList[0]);
           }
        else if (nb>1 && nb<35 )
        {if (villeList.indexOf ( m_pUI->lineEdit_Ville->text()) == -1 )  // si la ville ne correspond pas au code postal
            {   //............... deconnecter car lors popup on va quitter le champ lineEdit_CodePostal .................
                //                => message focusOutEvent() => rerentrer ici en boucle => boum
                QObject::disconnect( m_pUI->lineEdit_CodePostal, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),    this,     SLOT(   Slot_OutFocusCodePostal(QFocusEvent*, int &))  );
                QString        ville = DoPopupList(m_pUI->lineEdit_CodePostal, villeList);
                if (ville.length()>0) m_pUI->lineEdit_Ville->setText(ville);
                connect( m_pUI->lineEdit_CodePostal, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)),                this,     SLOT(   Slot_OutFocusCodePostal(QFocusEvent*, int &))  );
            }
        }
    }
    else
    {m_pUI->lineEdit_Ville->clear();
    }
}
//-- VILLE --------------------------------- Slot_OutFocusVille -----------------------------------------------------------------------
void C_Dlg_personnes::Slot_OutFocusVille(QFocusEvent*, int &)
{tryToSetCodePostalFromVille();
}

//------------------------------------------- tryToSetCodePostalFromVille -----------------------------------------------------------------------
void C_Dlg_personnes::tryToSetCodePostalFromVille()
{
    QString codePostal("");
    QString ville = m_pUI->lineEdit_Ville->text().trimmed();
    //.......... si pas de ville saisie on tente avec le code postal .........................
    if (ville.length()==0)
       {tryToSetVilleFromCodePostal();
        return;
       }
    //............... chercher liste de codepostaux pouvant correspondre ï¿œ  cette ville .............
    QStringList codePostalList;
    G_pCApp->GetCodePostalFromVille( ville.replace("SAINT ","ST "),  codePostalList);
    if (codePostalList.indexOf ( m_pUI->lineEdit_CodePostal->text().remove(' ') + " " + m_pUI->lineEdit_Ville->text() ) != -1 ) return;

    int nb = codePostalList.count();
    if (nb==1)
       {codePostal = codePostalList[0];
       }
    else if (nb>1 && nb<35 )
       {//............... deconnecter car lors popup on va quitter le champ lineEditVille .................
        //                => message focusOutEvent() => rerentrer ici en boucle => boum
        QObject::disconnect( m_pUI->lineEdit_Ville, SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)), this,     SLOT(   Slot_OutFocusVille(QFocusEvent*, int &))  );
        codePostal = DoPopupList(m_pUI->lineEdit_Ville, codePostalList);
        connect( m_pUI->lineEdit_Ville,             SIGNAL( Sign_focusOutEvent(QFocusEvent*, int &)), this,     SLOT(   Slot_OutFocusVille(QFocusEvent*, int &))  );
       }
    //............ separer la ville du code postal ...................................................
    int pos = codePostal.indexOf(' ');
    if (pos != -1)
       {ville      = codePostal.mid(pos+1);
        codePostal = codePostal.left(pos);
       }
    if (//lineEdit_CdPostal->text().length()<5           ||                               // si code postal incomplet
        (codePostal.length()!=0 /*&& lineEdit_CdPostal->text() != codePostal */)            // ou non conforme avec celui de la ville
       )
       {m_pUI->lineEdit_CodePostal->setText(codePostal );                                        // placer celui de la ville
        m_pUI->lineEdit_Ville->setText(ville);
        return;
       }
    //.............. si pas de code postal trouvÃ© pour cette ville tenter de placer la ville du code postal .............
    if (codePostal.length()==0 )
       {ville = G_pCApp->GetVilleFromCodePostal(m_pUI->lineEdit_CodePostal->text().remove(' '));
        if (ville.length()!=0) m_pUI->lineEdit_Ville->setText(ville);
       }
}

//--------------------------------------- selectMedecinById ----------------------------------------------------------
int C_Dlg_personnes::selectMedecinById(const QString &start_ID)
{   if (start_ID.length()==0) return 0;
    QList<QTreeWidgetItem *> listItem = m_pUI->treeWidget_ListeMedecins->findItems ( start_ID, Qt::MatchExactly,  3 );
    QTreeWidgetItem *pQTreeWidgetItem = 0;
    if (listItem.count())
       {int i;
        m_pUI->treeWidget_ListeMedecins->clearSelection ();
        for (i = 0; i < listItem.size(); ++i)
            { pQTreeWidgetItem = listItem[i];
              if (pQTreeWidgetItem)
                 {pQTreeWidgetItem->setSelected(TRUE);
                  m_pUI->treeWidget_ListeMedecins->scrollToItem( pQTreeWidgetItem);
                 }
            }
        return i;
       }
    return 0;
}

//--------------------------------------- getSelectedMedecinById ----------------------------------------------------------
QTreeWidgetItem* C_Dlg_personnes::getSelectedMedecinById(const QString &start_ID)
{   if (start_ID.length()==0) return 0;
    QList<QTreeWidgetItem *> listItem = m_pUI->treeWidget_ListeMedecins->findItems ( start_ID, Qt::MatchExactly,  3 );
    if (listItem.count()) return listItem[0];
    else                  return 0;
}

//--------------------------------- Slot_pushButton_NouvelleFiche_clicked ------------------------------------------------------------
void C_Dlg_personnes::Slot_pushButton_NouvelleFiche_clicked()
{//.......... si Mode = Creation d'une fiche en cours ............................
 //           (listBoxChoixNom est alors cachÃ©e) et que ce
 //           bouton est appuyÃ© --> ANNULER CREATION EN COURS (tout remettre en visible)
 if (m_pUI->frame_Liste->isHidden ())
    {m_pUI->frame_Liste->show();
     m_pUI->pushButton_Enregistrer->setText(tr("&Save modifications"));
     m_pUI->pushButton_NouvelleFiche->setText(tr("&Create new file"));
     m_pUI->pushButton_Effacer->show();
     m_pUI->pushButtonOK->show();
     ItemToForm( m_LastPk );
    }
 //.......... si Mode = Consultation des fiches en cours ............................
 //           (listBoxChoixNom est alors visible) et que ce
 //           bouton est appuyÃ© --> SE PLACER EN MODE CREATION (cacher les outils de consultation)
 else
    {m_pUI->frame_Liste->hide();
     //........ "Enregistrer les modifications" ....................
     m_pUI->pushButton_Enregistrer->setText(tr("Create and add this file"));
     //........ "CrÃ©er une nouvelle Fiche" ....................
     m_pUI->pushButton_NouvelleFiche->setText(tr("Cance&l file creation"));
     m_pUI->pushButton_Effacer->hide();
     m_pUI->pushButtonOK->hide();
     ClearSaisie();
     //................. pour JMR positionner la specialite  sur un type predetermine .............
     QByteArray ba;
     QString path  = CGestIni::Construct_Name_File_Ini("Manager", QFileInfo (G_pCApp->argv()[0]).canonicalPath(),"");
     CGestIni::Param_UpdateFromDisk(path, ba);
     QString value = CGestIni::Param_ReadUniqueParam(ba, "Gestion des contacts","specialite par defaut");
     if (value.length()==0)
        { path  = CGestIni::Construct_Name_File_Ini("personnes", QFileInfo (G_pCApp->argv()[0]).canonicalPath(),"");
          CGestIni::Param_UpdateFromDisk(path, ba);
          value = CGestIni::Param_ReadUniqueParam(ba, "Configuration","specialite par defaut");
        }
     setComboBoxOnValue(m_pUI->comboBox_Specialite, value);
     m_pUI->lineEdit_Nom->setFocus();
    }
}
//--------------------------------- Slot_pushButton_Enregistrer_clicked ------------------------------------------------------------
void C_Dlg_personnes::Slot_pushButton_Enregistrer_clicked()
{//.......... si Mode = Creation d'une fiche en cours ............................
 //           (listBoxChoixNom est alors cachÃ©e) et que ce
 //           bouton est appuyÃ© --> confirmer la creation en cours
 if (m_pUI->frame_Liste->isHidden ())
    {QString newPk = FormToDisk("-1");                 // creer la fiche dans la base de donnÃ©es
     if (newPk != "-1") m_LastPk = newPk;              // le nouveau n'est retenu que si pas d'erreur de creation
     //........... replacer les outils de consultation en visible ..........................
     m_pUI->frame_Liste->show();
     m_pUI->pushButton_Enregistrer->setText(tr("&Save modifications"));
     m_pUI->pushButton_NouvelleFiche->setText(tr("&Create new file"));
     m_pUI->pushButton_Effacer->show();
     m_pUI->pushButtonOK->show();
     //........... reinitialiser les combo-box ..................
     //            et l'interface sur l'eventuel (si pas d'erreur) nouveau cree
     InitComboSpec();
     InitAllComboTelType();
     selectMedecinById(m_LastPk);   // si erreur de creation alors m_LastPk sera sur la valeur ancienne donc pas de PB
    }
 //.......... si Mode = Consultation des fiches en cours ............................
 //           (listBoxChoixNom est alors visible) et que ce
 //           bouton est appuyÃ© --> mettre ï¿œ  jour la fiche en cours
 else if (m_LastPk.length())
    { FormToDisk(m_LastPk);
      InitAllComboTelType();
      InitComboSpec();
      selectMedecinById(m_LastPk);
    }
}

//------------------------------------------- ClearSaisie ------------------------------------------------------------
void C_Dlg_personnes::ClearSaisie()
{m_pUI->lineEdit_Nom->setText("");
 m_pUI->lineEdit_Prenom->setText("");
 m_pUI->lineEdit_Titre->setText("");
 m_pUI->lineEdit_politesse->setText("");
 m_pUI->lineEdit_NumOrd->setText("");
 m_pUI->lineEdit_CodePostal->setText("");
 m_pUI->lineEdit_Ville->setText("");
 m_pUI->lineEdit_Tel_1->setText("");
 m_pUI->lineEdit_Tel_2->setText("");
 m_pUI->lineEdit_Tel_3->setText("");
 m_pUI->lineEdit_Code_1->setText("");
 m_pUI->lineEdit_Code_2->setText("");
 m_pUI->lineEdit_Code_3->setText("");
 m_pUI->lineEdit_FiltreListe->setText("");
 m_pUI->textEdit_Rue->setText("");
 m_pUI->textEdit_Note->setText("");
 m_pUI->lineEdit_Email->setText("");
 //InitAllComboTelType();
 m_pUI->comboBoxTelTyp_1->lineEdit()->setText("");
 m_pUI->comboBoxTelTyp_2->lineEdit()->setText("");
 m_pUI->comboBoxTelTyp_3->lineEdit()->setText("");
}

//--------------------------------- Slot_pushButton_Effacer_clicked ------------------------------------------------------------
void C_Dlg_personnes::Slot_pushButton_Effacer_clicked()
{QTreeWidgetItem* pQTreeWidgetItem = getSelectedMedecinById(m_LastPk);
 if (pQTreeWidgetItem) // a ce stade on a obligatoirement  pQTreeWidgetItem->text(3) = m_LastPk
    {
     int ret = 0;
     if (G_pCApp->getDroits().indexOf("med")!= -1)
        {QMessageBox::warning (this,"Annuaire",
                                 tr("<u><b>WARNING  </b></u> <b> MedinTux </b> users <br>"
                                 " can be deleted only from user manager <br>"
                                 " <b>\"gest_user\"</b> and only if you are"
                                 " <b> administrator </b> ."),
                                 0,0,0,1);
         ret = 1;
        }
     if (ret==1)   return;
     ret = QMessageBox::warning (this,"Annuaire",
                                 tr("WARNING ! Deleting this file: <br><b>%1"
                                 "</b><br>is definitive.<br> Please confirm this action.").arg(pQTreeWidgetItem->text(0)+" "+pQTreeWidgetItem->text(1)),
                                 tr("&Delete"),tr("Do &nothing"),0,1);
     if (ret==1)   return;
     EraseRecord(m_LastPk); // m_LastPk y est initialise ï¿œ ""
     QString pk = "";
     if (m_pUI->treeWidget_ListeMedecins->topLevelItemCount()>1)
        {QTreeWidgetItem *                 pQTreeWidgetItemToSelect = m_pUI->treeWidget_ListeMedecins->itemBelow( pQTreeWidgetItem );
         if (pQTreeWidgetItemToSelect==0)  pQTreeWidgetItemToSelect = m_pUI->treeWidget_ListeMedecins->itemAbove( pQTreeWidgetItem );
         if (pQTreeWidgetItemToSelect)     pk = pQTreeWidgetItemToSelect->text(3);
        }
     delete pQTreeWidgetItem;
     //............ a faire en dernier car modifie la liste ..............
     InitAllComboTelType();
     InitComboSpec();
     selectMedecinById(pk);// m_LastPk y est positionne
    }
}
//--------------------------------- Slot_treeWidget_ListeMedecins_itemDoubleClicked ------------------------------------------------------------
void C_Dlg_personnes::Slot_treeWidget_ListeMedecins_itemDoubleClicked( QTreeWidgetItem *pQTreeWidgetItem, int  )
{if (pQTreeWidgetItem== 0)  return;
 setExchangeFile();
}
//--------------------------------------- recordPositions ----------------------------------------------------------
void C_Dlg_personnes::recordPositions()
{   int x  =  this->x();
    int y  =  this->y();
    int w  =  this->width();
    int h  =  this->height();
    //................. enregistrer position fenetre principale .........................
    CGestIni::Param_WriteParam(   G_pCApp->pIniParam(), (char*)"Geometry", (char*)"MainWindow",
                                  QString::number(x).toAscii(),      QString::number(y).toAscii(),
                                  QString::number(w).toAscii(),      QString::number(h).toAscii());
    //................. enregistrer spliter nom prenom .........................
    if (! m_pUI->frame_Liste->isHidden ())
       {
         QList<int>	list = m_pUI->splitter->sizes();
         CGestIni::Param_WriteParam(   G_pCApp->pIniParam(), (char*)"Geometry", (char*)"SpliterNomPrenom",
                                       QString::number(list[0]).toAscii(),      QString::number(list[1]).toAscii());
        }
    CGestIni::Param_UpdateToDisk(G_pCApp->getPathAppli() + "personnes.ini", G_pCApp->iniParam());

}
//--------------------------------------- getAndSetPositions ----------------------------------------------------------
void C_Dlg_personnes::getAndSetPositions()
{   QString val1, val2, val3, val4;
    if (CGestIni::Param_ReadParam(G_pCApp->iniParam().toAscii(),"Geometry", "MainWindow", &val1, &val2, &val3, &val4)==QString::null)  // zero = pas d'erreur
       {move(   val1.toInt(),   val2.toInt() );
        resize( val3.toInt(),   val4.toInt() );
       }
    if (CGestIni::Param_ReadParam(G_pCApp->iniParam().toAscii(),"Geometry", "SpliterNomPrenom", &val1, &val2)==QString::null)  // zero = pas d'erreur
       {QList <int> list;
        list.append(val1.toInt());
        list.append(val2.toInt());
        m_pUI->splitter->setSizes(list);
       }
    //else
    //   {QRect rect =  QApplication::desktop()->availableGeometry();   // juste pour faire bouger les choses ajustement du bas)
    //    setGeometry ( rect );
    //   }

}

//--------------------------------------- accept ----------------------------------------------------------
void C_Dlg_personnes::accept()
{tryToStopAPropos();
 recordPositions();
 QDialog::accept();
}
//--------------------------------------- reject ----------------------------------------------------------
void C_Dlg_personnes::reject()
{tryToStopAPropos();
 recordPositions();
 QDialog::reject();
}

//--------------------------------------- setExchangeFile ----------------------------------------------------------
void C_Dlg_personnes::setExchangeFile()
{int                        nb_arg = G_pCApp->argc();                   // 0 = path programme   1 = fichier d'echange
 QTreeWidgetItem* pQTreeWidgetItem = getSelectedMedecinById(m_LastPk);
 if (nb_arg >= 2 && pQTreeWidgetItem)
    {QString path ( G_pCApp->argv()[ 1 ] );
     if (G_pCApp->argv()[ 2 ]==QString("Record"))                       // verbe
        {QString num_rec =     pQTreeWidgetItem->text(3)         +"|"+  // pk
                               pQTreeWidgetItem->text(0)         +"|"+  // nom
                               pQTreeWidgetItem->text(1)         +"|"+  // prenom
                               pQTreeWidgetItem->text(2);               // specialite
         CGestIni::Param_UpdateToDisk(path, num_rec);
         accept();
        }
    }
}
//--------------------------------- EraseRecord ----------------------------------------------------------------------
void C_Dlg_personnes::EraseRecord( const QString &pk)
{if (G_pCApp->OpenBase()==0)    return ;
 QString requete("DELETE FROM ");
 requete += G_pCApp->m_INTERVENANTS_TBL_NAME + " WHERE " + G_pCApp->m_INTERVENANTS_PK + "='" + pk + "'";
 QSqlQuery sqlQuery(requete, G_pCApp->getDatabase());
 G_pCApp->CloseBase();
 m_LastPk   = "";
}
//--------------------------------------- Slot_comboBox_FiltreListe_currentIndexChanged ----------------------------------------------------------
void C_Dlg_personnes::Slot_comboBox_FiltreListe_currentIndexChanged(int index)
{QTreeWidgetItem *pQTreeWidgetItem = initListesMedecin(index, m_pUI->lineEdit_FiltreListe->text());
 if (pQTreeWidgetItem) {m_LastPk = pQTreeWidgetItem->text(3);selectMedecinById(m_LastPk);}
}
//--------------------------------------- Slot_FiltrerListe ----------------------------------------------------------
void C_Dlg_personnes::Slot_FiltrerListe(const QString &text_nomPrenom)
{QTreeWidgetItem *pQTreeWidgetItem = initListesMedecin(m_pUI->comboBox_FiltreListe->currentIndex(), text_nomPrenom);
 if (pQTreeWidgetItem) {m_LastPk = pQTreeWidgetItem->text(3);selectMedecinById(m_LastPk);}
}
//--------------------------------------- initListesMedecin -----------------------------------------------------------------
QTreeWidgetItem *C_Dlg_personnes::initListesMedecin(int index, QString q_nom)
{QString spec;
 if (index==0)
    spec = "";
 else
    spec = m_pUI->comboBox_FiltreListe->currentText();
 return initListesMedecin(spec,  q_nom);
}

//--------------------------------------- initListesMedecin -----------------------------------------------------------------
QTreeWidgetItem *C_Dlg_personnes::initListesMedecin(QString spec, QString q_nom)
{ //............... extraire l'eventuel prenom (separe par ';').....................................
  QString  q_prenom = "";
  if (q_nom.length())
     {int pos = q_nom.indexOf(";");
      if (pos != -1)
         {q_prenom = q_nom.mid(pos+1);
          q_nom    = q_nom.left(pos);
         }
     }
  q_nom.replace("\'","\\\'");
  q_prenom.replace("\'","\\\'");
  //................. Preparer la requete .................................
  QString requete;
  requete += "SELECT "     + G_pCApp->m_INTERVENANTS_NOM               + ","
                           + G_pCApp->m_INTERVENANTS_PRENOM            + ","
                           + G_pCApp->m_INTERVENANTS_QUALITE           + ","
                           + G_pCApp->m_INTERVENANTS_PK                + " FROM "
                           + G_pCApp->m_INTERVENANTS_TBL_NAME          + " WHERE "
                           + G_pCApp->m_INTERVENANTS_NOM               + " LIKE '" + q_nom         + "%' AND "
                           + G_pCApp->m_INTERVENANTS_PRENOM            + " LIKE '" + q_prenom      + "%' ";

  //............... filtrer selon specialite .....................................
  if (spec.length()) requete    += " AND (" + G_pCApp->m_INTERVENANTS_QUALITE + " ='"+ spec.toUtf8()+"' OR " + G_pCApp->m_INTERVENANTS_QUALITE + " ='"+ spec+ "')";
  //................ un peu d'ordre .............................................
  requete    += " ORDER BY "   + G_pCApp->m_INTERVENANTS_NOM + "," + G_pCApp->m_INTERVENANTS_PRENOM + " LIMIT "+ ST_LIST_MAX;

  if (G_pCApp->OpenBase()==0)    return 0;
  QSqlQuery  sqlQuery(requete , G_pCApp->getDatabase() );
  //................ scanner les enregistrements ..........................
  //                 pour remplir le tableau d'indexs Utf8_Query(sqlQuery,
  int                             i = 0;
  QTreeWidgetItem *pQTreeWidgetItem = 0;
  if (sqlQuery.isActive())
     {m_pUI->treeWidget_ListeMedecins->clear();
      while (sqlQuery.next())
        {   pQTreeWidgetItem = new QTreeWidgetItem( );
            if (pQTreeWidgetItem)
               {QString spec = CGestIni::Utf8_Query(sqlQuery,2).trimmed();
                m_pUI->treeWidget_ListeMedecins->addTopLevelItem(pQTreeWidgetItem);
                pQTreeWidgetItem->setText(0, CGestIni::Utf8_Query(sqlQuery,0));
                pQTreeWidgetItem->setText(1, CGestIni::Utf8_Query(sqlQuery,1));
                pQTreeWidgetItem->setText(2, spec);
                pQTreeWidgetItem->setText(3, sqlQuery.value(3).toString());
                //if (icone.length()) pQTreeWidgetItem->setIcon (0,Theme::getIconSet(icone));
               }
          i++;
        } //end while (sqlQuery.next())
     } //end if (pSqlQuery && sqlQuery.isActive())
  G_pCApp->OutSQL_error( sqlQuery, 0, requete.toAscii());
  G_pCApp->CloseBase() ;
  m_pUI->label_trouves->setText(tr("Displayed %1 on %2").arg(QString::number(i), GetNbRecord(G_pCApp->m_INTERVENANTS_TBL_NAME)));
  if (i==1) return pQTreeWidgetItem;  // on ne retourne l'item selectionne que si il y en a un pour le slectionner dans
  return 0;
}

//--------------------------------- InitAllComboTelType -----------------------------------------------------------------------
void  C_Dlg_personnes::InitAllComboTelType()
{    QStringList itemList = initListesTypesTel();
     m_pUI->comboBoxTelTyp_1->clear();
     m_pUI->comboBoxTelTyp_2->clear();
     m_pUI->comboBoxTelTyp_3->clear();
     m_pUI->comboBoxTelTyp_1->insertItems (0, itemList );
     m_pUI->comboBoxTelTyp_2->insertItems (0, itemList );
     m_pUI->comboBoxTelTyp_3->insertItems (0, itemList );
}

//--------------------------------------- initListes ----------------------------------------------------------
QStringList C_Dlg_personnes::initListesTypesTel()
{       QStringList itemList;
        if (G_pCApp->OpenBase()==0)   return itemList;
        QString val_1, val_2, val_3;
        //.................. fabriquer la requete ........................
        //                   de selection de l'enregistrement
        //                   correspondant ï¿œ  la cle
        QString requete ("SELECT ");
        requete      +=  G_pCApp->m_INTERVENANTS_TEL_TYPE_1      + ","
                     +   G_pCApp->m_INTERVENANTS_TEL_TYPE_2      + ","
                     +   G_pCApp->m_INTERVENANTS_TEL_TYPE_3      + " " ;

        requete += " FROM " + G_pCApp->m_INTERVENANTS_TBL_NAME;
        QSqlQuery query(requete, G_pCApp->getDatabase());
        if (query.isActive())
           {while (query.next())
                  { val_1 = G_pCApp->Utf8_Query(query,0).trimmed();
                    val_2 = G_pCApp->Utf8_Query(query,1).trimmed();
                    val_3 = G_pCApp->Utf8_Query(query,2).trimmed();

                    if (val_1.length() && itemList.indexOf ( val_1 )==-1) itemList.append(val_1);
                    if (val_2.length() && itemList.indexOf ( val_2 )==-1) itemList.append(val_2);
                    if (val_3.length() && itemList.indexOf ( val_3 )==-1) itemList.append(val_3);
                  }
           }
        else  G_pCApp->OutSQL_error(query , "InitAllComboTelType()", requete.toAscii() );
     G_pCApp->CloseBase() ;
     return itemList;
}

//--------------------------------- InitComboSpec ------------------------------------------------------------
//  ACTION: parcoure la liste d'index et initialise le combobox avec toutes les valeurs trouvÃ©es de specialitÃ©s
//          Il ne faut pas utiliser cette fonction si la liste n'est pas complete donc l'utiliser apres
//          appel ï¿œ  la fonction: InitAllList("Toutes SpÃ©cialitÃ©s");

void C_Dlg_personnes::InitComboSpec()
{ if (G_pCApp->OpenBase()==0)   return ;

  QString requete("");
  requete    += "SELECT "     + G_pCApp->m_INTERVENANTS_QUALITE      + " FROM ";
  requete    += G_pCApp->m_INTERVENANTS_TBL_NAME + " ORDER BY "      + G_pCApp->m_INTERVENANTS_QUALITE;
  QSqlQuery sqlQuery(requete , G_pCApp->getDatabase() );

  //................ scanner les enregistrements ..........................
  //                 pour remplir le tableau d'indexs
  QStringList itemsList;
  if (sqlQuery.isActive())
     { m_pUI->comboBox_FiltreListe->clear();
       m_pUI->comboBox_Specialite->clear();
       while (sqlQuery.next())
        {QString toAppend = CGestIni::Utf8_Query(sqlQuery, 0).trimmed(); //   sqlQuery.value(0).toString().trimmed();
          if (toAppend.length())
             {if (itemsList.indexOf (toAppend)== -1) itemsList.append(toAppend);
             }
        } //end while (sqlQuery.next())
     }
  itemsList.sort();
  m_pUI->comboBox_Specialite->clear();
  m_pUI->comboBox_Specialite->insertItems(0, itemsList);
  itemsList.prepend(tr("All specialities"));
  m_pUI->comboBox_FiltreListe->clear();
  m_pUI->comboBox_FiltreListe->insertItems(0, itemsList);
  G_pCApp->CloseBase();
}

//--------------------------------- IsThisValueInComboBox ------------------------------------------------------------
int C_Dlg_personnes::IsThisValueInComboBox(QComboBox *pQComboBox, const QString& value)
{return pQComboBox->findText (value,  Qt::MatchExactly );  //Qt::MatchExactly | Qt::MatchCaseSensitive
}
//--------------------------------- setComboBoxOnValue ------------------------------------------------------------
int C_Dlg_personnes::setComboBoxOnValue(QComboBox *pQComboBox, const QString& value)
{if (value.length()==0)return -1;
 int index = pQComboBox->findText (value,  Qt::MatchExactly );  //Qt::MatchExactly | Qt::MatchCaseSensitive
 if (index != -1) pQComboBox->setCurrentIndex  (index);
 return index;
}

//---------------------------------  ------------------------------------------------------------
QString  C_Dlg_personnes::GetNbRecord(const QString &table)
{ QString result = "";
  if (G_pCApp->OpenBase()==0) return QString::null;

  //................. SI un nom ou prenom est donnÃ© en entrÃ©e alors filtrer selon ...................
  QString   requete = QString("SELECT COUNT(*) FROM ") + table;
  QSqlQuery query(requete , G_pCApp->getDatabase() );
  if (query.isActive() && query.next())
     {result = query.value(0).toString();
     }
  G_pCApp->CloseBase();
  return result;
}
//---------------------------------------- getFirstSelectedItem -------------------------------------------------------
QTreeWidgetItem* C_Dlg_personnes::getFirstSelectedItem(QTreeWidget *pQTreeWidget)
{   QList<QTreeWidgetItem *> list = pQTreeWidget->selectedItems ();
    if (list.count()) return list[0];

     QTreeWidgetItem *pQTreeWidgetItem=0;
     if (pQTreeWidget->topLevelItemCount()==1)
        {pQTreeWidgetItem = pQTreeWidget->topLevelItem (0);
         if (pQTreeWidgetItem)
            {pQTreeWidgetItem->setSelected(TRUE);
             return pQTreeWidgetItem;
            }
        }
     return 0;
}

//--------------------------------- Slot_treeWidget_ListeMedecins_itemSelectionChanged -------------------------------------------------------
void C_Dlg_personnes::Slot_treeWidget_ListeMedecins_itemSelectionChanged()
{QTreeWidgetItem*  pQTreeWidgetItem = getFirstSelectedItem(m_pUI->treeWidget_ListeMedecins);
 if (pQTreeWidgetItem) ItemToForm( pQTreeWidgetItem->text(3));
}
//--------------------------------- Slot_treeWidget_ListeMedecins_itemClicked -------------------------------------------------------
void C_Dlg_personnes::Slot_treeWidget_ListeMedecins_itemClicked( QTreeWidgetItem *pQTreeWidgetItem , int)
{if (pQTreeWidgetItem==0) return;
 ItemToForm( pQTreeWidgetItem->text(3));
}

//--------------------------------- ItemToForm -------------------------------------------------------
void C_Dlg_personnes::ItemToForm( const QString& pk )
{    if (G_pCApp->OpenBase()==0)   return ;
     if (pk.length())
       {
        //.................. fabriquer la requete ........................
        //                   de selection de l'enregistrement
        //                   correspondant ï¿œ  la cle

        QString requete ("SELECT ");
        requete      += G_pCApp->m_INTERVENANTS_NOM             + ","     // 0
                     +  G_pCApp->m_INTERVENANTS_PRENOM          + ","     // 1
                     +  G_pCApp->m_INTERVENANTS_ADRESSE         + ","     // 2
                     +  G_pCApp->m_INTERVENANTS_CODE_POST       + ","     // 3
                     +  G_pCApp->m_INTERVENANTS_VILLE           + ","     // 4
                     +  G_pCApp->m_INTERVENANTS_TEL_1           + ","     // 5
                     +  G_pCApp->m_INTERVENANTS_TEL_2           + ","     // 6
                     +  G_pCApp->m_INTERVENANTS_TEL_3           + ","     // 7
                     +  G_pCApp->m_INTERVENANTS_EMAIL           + ","     // 8
                     +  G_pCApp->m_INTERVENANTS_NOTE            + ","     // 9
                     +  G_pCApp->m_INTERVENANTS_QUALITE         + ","     // 10
                     +  G_pCApp->m_INTERVENANTS_TITRE           + ","     // 11
                     +  G_pCApp->m_INTERVENANTS_TEL_ABR_1       + ","     // 12
                     +  G_pCApp->m_INTERVENANTS_TEL_ABR_2       + ","     // 13
                     +  G_pCApp->m_INTERVENANTS_TEL_ABR_3       + ","     // 14
                     +  G_pCApp->m_INTERVENANTS_DROITS          + ","     // 15
                     +  G_pCApp->m_INTERVENANTS_TEL_TYPE_1      + ","     // 16
                     +  G_pCApp->m_INTERVENANTS_TEL_TYPE_2      + ","     // 17
                     +  G_pCApp->m_INTERVENANTS_TEL_TYPE_3      + ","     // 18
                     +  G_pCApp->m_INTERVENANTS_SEXE            + ","     // 19
                     +  G_pCApp->m_INTERVENANTS_NUM_ORDRE       + ","     // 20
                     +  G_pCApp->m_INTERVENANTS_CHER                      // 21
                  ;

        requete += " FROM " + G_pCApp->m_INTERVENANTS_TBL_NAME + " WHERE " + G_pCApp->m_INTERVENANTS_PK + "='";
        requete +=  pk + "'";

        QSqlQuery query(requete, G_pCApp->getDatabase());
        if (query.isActive())
           {while (query.next())
                  { m_LastPk = pk;
                    m_pUI->lineEdit_Nom->setText(G_pCApp->Utf8_Query(query,0));
                    m_pUI->lineEdit_Prenom->setText(G_pCApp->Utf8_Query(query,1));
                    m_pUI->textEdit_Rue->setText(G_pCApp->Utf8_Query(query,2));
                    m_pUI->lineEdit_CodePostal->setText(G_pCApp->Utf8_Query(query,3));
                    m_pUI->lineEdit_Ville->setText(G_pCApp->Utf8_Query(query,4));
                    m_pUI->lineEdit_Tel_1->setText(G_pCApp->Utf8_Query(query,5));
                    m_pUI->lineEdit_Tel_2->setText(G_pCApp->Utf8_Query(query,6));
                    m_pUI->lineEdit_Tel_3->setText(G_pCApp->Utf8_Query(query,7));
                    m_pUI->lineEdit_Email->setText(G_pCApp->Utf8_Query(query,8));
                    m_pUI->textEdit_Note->setText(G_pCApp->Utf8_Query(query,9));
                    setComboBoxOnValue(m_pUI->comboBox_Specialite, G_pCApp->Utf8_Query(query,10));
                    m_pUI->lineEdit_Titre->setText(G_pCApp->Utf8_Query(query,11));
                    m_pUI->lineEdit_Code_1->setText(G_pCApp->Utf8_Query(query,12));
                    m_pUI->lineEdit_Code_2->setText(G_pCApp->Utf8_Query(query,13));
                    m_pUI->lineEdit_Code_3->setText(G_pCApp->Utf8_Query(query,14));
                    G_pCApp->setDroits(G_pCApp->Utf8_Query(query,15));
                    if (setComboBoxOnValue(m_pUI->comboBoxTelTyp_1, G_pCApp->Utf8_Query(query,16))==-1) m_pUI->comboBoxTelTyp_1->QComboBox::lineEdit ()->setText("");
                    if (setComboBoxOnValue(m_pUI->comboBoxTelTyp_2, G_pCApp->Utf8_Query(query,17))==-1) m_pUI->comboBoxTelTyp_2->QComboBox::lineEdit ()->setText("");
                    if (setComboBoxOnValue(m_pUI->comboBoxTelTyp_3, G_pCApp->Utf8_Query(query,18))==-1) m_pUI->comboBoxTelTyp_3->QComboBox::lineEdit ()->setText("");
                    QString sex = G_pCApp->Utf8_Query(query,19);
                    if (sex.toUpper()[0]=='M') m_pUI->comboBoxSexe->setCurrentIndex  (0);
                    else                       m_pUI->comboBoxSexe->setCurrentIndex  (1);
                    m_pUI->lineEdit_NumOrd->setText(G_pCApp->Utf8_Query(query,20));
                    m_pUI->lineEdit_politesse->setText(G_pCApp->Utf8_Query(query,21));
                  }
           }
        else  G_pCApp->OutSQL_error(query , "ItemToForm()", requete.toAscii() );
       }
     G_pCApp->CloseBase();
}
//--------------------------------- FormToDisk------------------------------------------------------------------------
// ACTION: enregistre la fiche courante dans la base:
// ENTREE: rec_num: cle primaire de l'enregistrement: si -1 il faut en creer un sinon il faut mettre a jour
// RETOUR le pk modifie soit le nouveau cree
QString C_Dlg_personnes::FormToDisk(QString pk )
{   QString requete;

    //................ si le champ obligatoire nom est vide .................................
    //                 cassos aprÃšs avertissement
    if (m_pUI->lineEdit_Nom->text().trimmed()=="")
        {QMessageBox::critical(this,"Annuaire",
                               tr("The field <b>Family name</b> cannot be empty <br>"
                                  "Please fill it before creating a new file."));
         return 0;
        }
     //................ si le champ specialite est vide .....................................
     //                        y mettre "Divers"
    if (m_pUI->comboBox_Specialite->lineEdit()->text().trimmed()=="") m_pUI->comboBox_Specialite->lineEdit()->setText("Divers");
    //................... creer un GUID pour si creation d'une fiche .........................
    QString guid = G_pCApp->GUID_Create();
    //................... CREATION D'UNE FICHE ..............................................
    if ( pk == "-1")
       {
        requete  = "INSERT INTO ";
        requete +=  G_pCApp->m_INTERVENANTS_TBL_NAME        + " ("
                  + G_pCApp->m_INTERVENANTS_NOM             + ","          // 0
                  + G_pCApp->m_INTERVENANTS_PRENOM          + ","          // 1
                  + G_pCApp->m_INTERVENANTS_ADRESSE         + ","          // 2
                  + G_pCApp->m_INTERVENANTS_CODE_POST       + ","          // 3
                  + G_pCApp->m_INTERVENANTS_VILLE           + ","          // 4
                  + G_pCApp->m_INTERVENANTS_TEL_1           + ","          // 5
                  + G_pCApp->m_INTERVENANTS_TEL_2           + ","          // 6
                  + G_pCApp->m_INTERVENANTS_TEL_3           + ","          // 7
                  + G_pCApp->m_INTERVENANTS_EMAIL           + ","          // 8
                  + G_pCApp->m_INTERVENANTS_NOTE            + ","          // 9
                  + G_pCApp->m_INTERVENANTS_QUALITE         + ","          // 10
                  + G_pCApp->m_INTERVENANTS_TITRE           + ","          // 11
                  + G_pCApp->m_INTERVENANTS_TEL_ABR_1       + ","          // 12
                  + G_pCApp->m_INTERVENANTS_TEL_ABR_2       + ","          // 13
                  + G_pCApp->m_INTERVENANTS_TEL_ABR_3       + ","          // 14
                  + G_pCApp->m_INTERVENANTS_TEL_TYPE_1      + ","          // 15
                  + G_pCApp->m_INTERVENANTS_TEL_TYPE_2      + ","          // 16
                  + G_pCApp->m_INTERVENANTS_TEL_TYPE_3      + ","          // 17
                  + G_pCApp->m_INTERVENANTS_SEXE            + ","          // 18
                  + G_pCApp->m_INTERVENANTS_GUID            + ","          // 19
                  + G_pCApp->m_INTERVENANTS_NUM_ORDRE       + ","          // 20
                  + G_pCApp->m_INTERVENANTS_CHER            + ") VALUES "; // 21

        requete +=  "('" + m_pUI->lineEdit_Nom->text().replace("'","\\'")                     + "', '";       //    0
        requete +=         m_pUI->lineEdit_Prenom->text().replace("'","\\'")                  + "', '";       //    1
        requete +=         m_pUI->textEdit_Rue->toPlainText().replace("'","\\'")              + "', '";       //    2
        requete +=         m_pUI->lineEdit_CodePostal->text().remove(' ').left(5)             + "', '";       //    3
        requete +=         m_pUI->lineEdit_Ville->text().replace("'","\\'")                   + "', '";       //    4
        requete +=         m_pUI->lineEdit_Tel_1->text()                                      + "', '";       //    5
        requete +=         m_pUI->lineEdit_Tel_2->text()                                      + "', '";       //    6
        requete +=         m_pUI->lineEdit_Tel_3->text()                                      + "', '";       //    7
        requete +=         m_pUI->lineEdit_Email->text()                                      + "', '";       //    8
        requete +=         m_pUI->textEdit_Note->toPlainText().replace("'","\\'")             + "', '";       //    9
        requete +=         m_pUI->comboBox_Specialite->lineEdit()->text()                     + "', '";       //    10
        requete +=         m_pUI->lineEdit_Titre->text().replace("'","\\'")                   + "', '";       //    11
        requete +=         m_pUI->lineEdit_Code_1->text()                                     + "', '";       //    12
        requete +=         m_pUI->lineEdit_Code_2->text()                                     + "', '";       //    13
        requete +=         m_pUI->lineEdit_Code_3->text()                                     + "', '";       //    14
        requete +=         m_pUI->comboBoxTelTyp_1->lineEdit()->text().replace("'","\\'")     + "', '";       //    15
        requete +=         m_pUI->comboBoxTelTyp_2->lineEdit()->text().replace("'","\\'")     + "', '";       //    16
        requete +=         m_pUI->comboBoxTelTyp_3->lineEdit()->text().replace("'","\\'")     + "', '";       //    17
        QString sex = "M"; if (m_pUI->comboBoxSexe->currentText()[0]!='M')  sex = "F";
        requete +=         sex                                                                + "', '";       //    18
        requete +=         guid                                                               + "', '";       //    19
        requete +=         m_pUI->lineEdit_NumOrd->text()                                     + "', '";       //    20
        requete +=         m_pUI->lineEdit_politesse->text()                                  + "')  ";       //    21

       }
    //................... MIS A JOUR D'UNE FICHE ..............................................
    else
       {requete  = "UPDATE ";
        requete += G_pCApp->m_INTERVENANTS_TBL_NAME      + " SET ";
        requete += G_pCApp->m_INTERVENANTS_NOM           + " = '"    + m_pUI->lineEdit_Nom->text().replace("'","\\'")                  + "', ";
        requete += G_pCApp->m_INTERVENANTS_PRENOM        + " = '"    + m_pUI->lineEdit_Prenom->text().replace("'","\\'")               + "', ";
        requete += G_pCApp->m_INTERVENANTS_ADRESSE       + " = '"    + m_pUI->textEdit_Rue->toPlainText().replace("'","\\'")           + "', ";
        requete += G_pCApp->m_INTERVENANTS_CODE_POST     + " = '"    + m_pUI->lineEdit_CodePostal->text().remove(' ').left(5)          + "', ";
        requete += G_pCApp->m_INTERVENANTS_VILLE         + " = '"    + m_pUI->lineEdit_Ville->text().replace("'","\\'")                + "', ";
        requete += G_pCApp->m_INTERVENANTS_TEL_1         + " = '"    + m_pUI->lineEdit_Tel_1->text()                                   + "', ";
        requete += G_pCApp->m_INTERVENANTS_TEL_2         + " = '"    + m_pUI->lineEdit_Tel_2->text()                                   + "', ";
        requete += G_pCApp->m_INTERVENANTS_TEL_3         + " = '"    + m_pUI->lineEdit_Tel_3->text()                                   + "', ";
        requete += G_pCApp->m_INTERVENANTS_EMAIL         + " = '"    + m_pUI->lineEdit_Email->text()                                   + "', ";
        requete += G_pCApp->m_INTERVENANTS_NOTE          + " = '"    + m_pUI->textEdit_Note->toPlainText().replace("'","\\'")          + "', ";
        requete += G_pCApp->m_INTERVENANTS_QUALITE       + " = '"    + m_pUI->comboBox_Specialite->lineEdit()->text()                  + "', ";
        requete += G_pCApp->m_INTERVENANTS_TITRE         + " = '"    + m_pUI->lineEdit_Titre->text().replace("'","\\'")                + "', ";
        requete += G_pCApp->m_INTERVENANTS_TEL_ABR_1     + " = '"    + m_pUI->lineEdit_Code_1->text()                                  + "', ";
        requete += G_pCApp->m_INTERVENANTS_TEL_ABR_2     + " = '"    + m_pUI->lineEdit_Code_2->text()                                  + "', ";
        requete += G_pCApp->m_INTERVENANTS_TEL_ABR_3     + " = '"    + m_pUI->lineEdit_Code_3->text()                                  + "', ";
        requete += G_pCApp->m_INTERVENANTS_TEL_TYPE_1    + " = '"    + m_pUI->comboBoxTelTyp_1->lineEdit()->text().replace("'","\\'")  + "', ";
        requete += G_pCApp->m_INTERVENANTS_TEL_TYPE_2    + " = '"    + m_pUI->comboBoxTelTyp_2->lineEdit()->text().replace("'","\\'")  + "', ";
        requete += G_pCApp->m_INTERVENANTS_TEL_TYPE_3    + " = '"    + m_pUI->comboBoxTelTyp_3->lineEdit()->text().replace("'","\\'")  + "', ";
        QString sex = "M"; if (m_pUI->comboBoxSexe->currentText()!=tr("Male"))  sex = "F";
        requete += G_pCApp->m_INTERVENANTS_SEXE          + " = '"    + sex                                                             + "', ";
        requete += G_pCApp->m_INTERVENANTS_NUM_ORDRE     + " = '"    + m_pUI->lineEdit_NumOrd->text()                                  + "', ";
        requete += G_pCApp->m_INTERVENANTS_CHER          + " = '"    + m_pUI->lineEdit_politesse->text()                               + "'  ";

        requete += "WHERE " + G_pCApp->m_INTERVENANTS_PK + " ='" + pk +"'";
       }
    if (G_pCApp->OpenBase()==0)    return 0;

    //.................. Executer la requete ...............................................
    QSqlQuery query (requete, G_pCApp->getDatabase());
    G_pCApp->OutSQL_error( query, "FormToDisk() =1=", requete.toAscii() );
    //.................. si creation recuperer numero d'enregistrement .....................
    if (pk == "-1")
       {requete  = "SELECT ";
        requete += G_pCApp->m_INTERVENANTS_NOM               + ","           // 0 nom
                +  G_pCApp->m_INTERVENANTS_PRENOM            + ","           // 1 prenom
                +  G_pCApp->m_INTERVENANTS_QUALITE           + ","           // 2 specialite
                +  G_pCApp->m_INTERVENANTS_PK                + " FROM "      // 3 pk
                +  G_pCApp->m_INTERVENANTS_TBL_NAME          + " WHERE ";
        requete += G_pCApp->m_INTERVENANTS_GUID              + " = '" + guid + "'";
        query.exec(requete );

        if (query.isActive() && query.next())
           {QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem( );
            if (pQTreeWidgetItem)
               {QString spec = G_pCApp->Utf8_Query(query,2).trimmed();
                pk           = query.value(3).toString();
                m_pUI->treeWidget_ListeMedecins->addTopLevelItem(pQTreeWidgetItem);
                pQTreeWidgetItem->setText(0, G_pCApp->Utf8_Query(query,0));
                pQTreeWidgetItem->setText(1, G_pCApp->Utf8_Query(query,1));
                pQTreeWidgetItem->setText(2, spec);
                pQTreeWidgetItem->setText(3, pk);
               }
           } //endif (pSqlQuery->isActive())
      else G_pCApp->OutSQL_error( query, "FormToDisk() =2=", requete.toAscii() );
       } //end if (rec_num == -1L)
    G_pCApp->CloseBase();
    return pk;
}
