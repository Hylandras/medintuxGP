 /********************************* C_Dlg_RdvTypeConfig.cpp ***********************
 *  #include "C_Dlg_RdvTypeConfig.h"                                              *
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
 *   Commissariat Ã  l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F\303\251d\303\251ration, 75752 PARIS cedex 15.   *
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

#include <QLabel>
#include <QTreeWidget>
#include <QPixmap>
#include <QIcon>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QColorDialog>
#include <QDebug>

#include "C_Dlg_RdvTypeConfig.h"
#include "C_Dlg_Agenda.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"

//---------------------------------------- C_Dlg_RdvTypeConfig --------------------------------------------
C_Dlg_RdvTypeConfig::C_Dlg_RdvTypeConfig(MAP_COLOR* pColorProfils, CMoteurAgenda* pCMoteurAgenda, QWidget *parent, C_RendezVous *pC_RendezVous, int isNew) :
    QDialog(parent),
    m_ui(new Ui::C_Dlg_RdvTypeConfig)
{   m_pCMoteurAgenda = pCMoteurAgenda;
    m_pColorProfils  = pColorProfils;
    m_pC_RendezVous  = pC_RendezVous;
    m_IsNew          = isNew;
    m_ui->setupUi(this);
    m_pLastQActionHovered = 0;
    m_pTreeWidgetPatients = 0; // CZA
    m_pQFrameListPatients = 0; // CZA
    m_ui->pushButton_Anonymize->setIcon( QIcon( Theme::getIcon("Agenda/identityDelete.png")));
    m_ui->pushButton_Heure->setIcon(     QIcon( Theme::getIcon("Agenda/datetime.png")));
    m_ui->pushButton_Duree->setIcon(     QIcon( Theme::getIcon("Agenda/Duree.png")));
    if (m_pC_RendezVous)
       {m_ui->lineEdit_Nom->setText(      m_pC_RendezVous->m_Nom.toUpper());
        m_ui->lineEdit_Prenom->setText(   m_pC_RendezVous->m_Prenom.toUpper());
        m_ui->lineEdit_Tel->setText(      m_pC_RendezVous->m_Tel);
        m_ui->lineEdit_GUID->setText(     m_pC_RendezVous->m_GUID);
        m_ui->textEdit_note->setText(     m_pC_RendezVous->m_Note);
        m_ui->timeEdit_Heure->setTime (   m_pC_RendezVous->getDateTime().time() );
        m_ui->timeEdit_Duree->setTime (   QTime(0,0,0).addSecs(m_pC_RendezVous->m_Duree*60) );
        m_ui->lineEdit_PrisPar->setText(  m_pC_RendezVous->m_PrisPar);
        m_ui->lineEdit_PrisPour->setText( m_pC_RendezVous->m_PrisAvec);
        //................ placer l'index ..........................................
        QStringList list  = CGestIni::listDirectory(Theme::getPath()+"Agenda/Statuts", ".png");
        QString statutRdv = m_pC_RendezVous->m_State;
        int        index  = 0;
        if (statutRdv.length()==0) statutRdv = tr("Undefined status");

        for (int i = 0; i < list.size(); ++i)
            {QString file   = Theme::getPath()+"Agenda/Statuts/"+list[i];
             QString statut = QFileInfo(file).baseName();
             if (statut==statutRdv) index = i;
             m_ui->comboBox_Statut->addItem(QIcon(QPixmap(file)), statut );
            }
        m_ui->comboBox_Statut->setCurrentIndex(index);
        if (m_ui->lineEdit_Nom->text().length() == 0)     // CZA
            m_ui->lineEdit_Nom->setFocus();              // CZA
        else
            m_ui->textEdit_note->setFocus();
        m_ui->pushButton_Plus->hide();          // PRINTRDV
        m_ui->pushButton_Moins->hide();         // PRINTRDV
       }
    else
       {m_ui->frame_RdvEdit->hide();
        setMaximumWidth(200);
        resize(200,height());
        //connect(  m_ui->treeWidget_RdvTypeList,          SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),                   this, SLOT(Slot_treeWidget_RdvTypeList_DoubleClicked( QTreeWidgetItem * , int)) );
       }
    initRdvTypeList();   // retourne 1 si l'initialisaion de la liste preselectionne le memee type que celui du RDV (si il est trouve)
    QTreeWidgetItem *pQTreeWidgetItem = getSelectedListViewItem(m_ui->treeWidget_RdvTypeList);
    if (pQTreeWidgetItem && isNew)
       {Slot_treeWidget_RdvTypeList_Clicked( pQTreeWidgetItem , 0 );
       }
    setCaption(tr("Modify types of appointments"));
    connect( m_ui->pushButton_Quitter,       SIGNAL( clicked()  ),     this ,     SLOT(   Slot_pushButton_Quitter_clicked ()  )  );
    connect( m_ui->pushButton_Ok,            SIGNAL( clicked()  ),     this ,     SLOT(   Slot_pushButton_Ok_clicked ()  )  );
    connect( m_ui->pushButton_Moins,         SIGNAL( clicked()  ),     this ,     SLOT(   Slot_pushButton_Moins_clicked ()  )  );
    connect( m_ui->pushButton_Plus,          SIGNAL( clicked()  ),     this ,     SLOT(   Slot_pushButton_Plus_clicked ()  )  );
    connect( m_ui->pushButton_Anonymize,     SIGNAL( clicked()  ),     this ,     SLOT(   Slot_pushButton_Anonymize_clicked ()  )  );
    connect( m_ui->pushButton_Heure,         SIGNAL( clicked()  ),     this ,     SLOT(   Slot_pushButton_Heure_clicked ()  )  );
    connect( m_ui->pushButton_Duree,         SIGNAL( clicked()  ),     this ,     SLOT(   Slot_pushButton_Duree_clicked ()  )  );
    connect( m_ui->treeWidget_RdvTypeList,   SIGNAL(itemClicked ( QTreeWidgetItem * ,       int  )), this, SLOT(Slot_treeWidget_RdvTypeList_Clicked( QTreeWidgetItem * , int)) );
    connect( m_ui->treeWidget_RdvTypeList,   SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )), this, SLOT(Slot_treeWidget_RdvTypeList_DoubleClicked( QTreeWidgetItem * , int)) );
    connect( m_ui->lineEdit_Nom,             SIGNAL(textChanged(const QString &)),          this, SLOT(Slot_lineEdit_Nom_textChanged(const QString &)) );       //CZA
    connect( m_ui->lineEdit_Prenom,          SIGNAL(textChanged(const QString &)),          this, SLOT(Slot_lineEdit_Prenom_textChanged(const QString &)) );       //CZA

    m_ui->pushButton_Moins->setIcon ( Theme::getIcon("Agenda/Moins.png") );
    m_ui->pushButton_Plus->setIcon  ( Theme::getIcon("Agenda/Plus.png") );
    QHeaderView *pQHeaderView = m_ui->treeWidget_RdvTypeList->header();               // cacher colonne ID
    pQHeaderView->resizeSection ( 0,20 );
    pQHeaderView->hideSection (2);
}

//---------------------------------------- initRdvTypeList --------------------------------------------
int C_Dlg_RdvTypeConfig::initRdvTypeList()
{ m_ui->treeWidget_RdvTypeList->clear();
  int isTypeSet = 0;
  QLabel *grabLabel = new QLabel();
  grabLabel->setGeometry (0, 0, 16, 16);
  QMap<QString, C_ColorType>::const_iterator it = m_pColorProfils->constBegin();

  grabLabel->setStyleSheet(QString("border: 2px solid #010101; background-color: #FFFFFF;"));
  QPixmap pix = QPixmap::grabWidget (grabLabel, 0, 0, 16, 16 );
  while (it != m_pColorProfils->constEnd())
        {C_ColorType ct = it.value();
         grabLabel->setStyleSheet(QString("border: 2px solid #010101; background-color: ")+ct.getColor()+";");
         pix = QPixmap::grabWidget (grabLabel, 0, 0, 16, 16 );
         if (it.key().trimmed().length())
            {QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem();
             if (pQTreeWidgetItem)
                {pQTreeWidgetItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                 pQTreeWidgetItem->setIcon (TYP_ICON, QIcon(pix)    );
                 pQTreeWidgetItem->setText (TYP_NAME, ct.getType()  );
                 pQTreeWidgetItem->setText (TYP_COUL, ct.getColor() );
                 pQTreeWidgetItem->setText (TYP_TIME, ct.getDuree() );
                 m_ui->treeWidget_RdvTypeList->addTopLevelItem(pQTreeWidgetItem);
                 if (m_pC_RendezVous && m_pC_RendezVous->m_Type == ct.getType())
                    {pQTreeWidgetItem->setSelected(TRUE);
                     isTypeSet = 1;
                    }
                } // end if (pQTreeWidgetItem)
            } // if (it.key().trimmed().length())
         ++it;
       }
  delete grabLabel;
  return isTypeSet;
}

//---------------------------------------- Slot_pushButton_Heure_clicked --------------------------------------------
QMenu* C_Dlg_RdvTypeConfig::makeMenuDuree(menuMode mode /*= C_Dlg_RdvTypeConfig::Long*/)
{QMenu *pmenu = new QMenu(this);
 QString libelle = tr("%1 hour %2 minutes");
 if (mode == C_Dlg_RdvTypeConfig::Short)
    { libelle = tr("%1 mn");
      pmenu->addAction ( libelle.arg("00") );
      pmenu->addAction ( libelle.arg("05") );
      pmenu->addAction ( libelle.arg("10") );
      pmenu->addAction ( libelle.arg("15") );
      pmenu->addAction ( libelle.arg("20") );
      pmenu->addAction ( libelle.arg("25") );
      pmenu->addAction ( libelle.arg("30") );
      pmenu->addAction ( libelle.arg("35") );
      pmenu->addAction ( libelle.arg("40") );
      pmenu->addAction ( libelle.arg("45") );
      pmenu->addAction ( libelle.arg("50") );
      pmenu->addAction ( libelle.arg("55") );
      return pmenu;
    }
 pmenu->addAction ( libelle.arg("00","05") );
 pmenu->addAction ( libelle.arg("00","10") );
 pmenu->addAction ( libelle.arg("00","15") );
 pmenu->addAction ( libelle.arg("00","20") );
 pmenu->addAction ( libelle.arg("00","25") );
 pmenu->addAction ( libelle.arg("00","30") );
 pmenu->addAction ( libelle.arg("00","35") );
 pmenu->addAction ( libelle.arg("00","40") );
 pmenu->addAction ( libelle.arg("00","45") );
 pmenu->addAction ( libelle.arg("00","50") );
 pmenu->addAction ( libelle.arg("00","55") );
 pmenu->addAction ( libelle.arg("01","00") );
 pmenu->addAction ( libelle.arg("01","05") );
 pmenu->addAction ( libelle.arg("01","10") );
 pmenu->addAction ( libelle.arg("01","15") );
 pmenu->addAction ( libelle.arg("01","20") );
 pmenu->addAction ( libelle.arg("01","25") );
 pmenu->addAction ( libelle.arg("01","30") );
 pmenu->addAction ( libelle.arg("01","35") );
 pmenu->addAction ( libelle.arg("01","40") );
 pmenu->addAction ( libelle.arg("01","45") );
 pmenu->addAction ( libelle.arg("01","50") );
 pmenu->addAction ( libelle.arg("01","55") );
 return pmenu;
}

//---------------------------------------- Slot_pushButton_Heure_clicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_pushButton_Heure_clicked()
{QMenu menu(this);
 QMenu *pmenu = makeMenuDuree(C_Dlg_RdvTypeConfig::Short);
 menu.addAction ( "07"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "08"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "09"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "10"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "11"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "12"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "13"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "14"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "15"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "16"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "17"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "18"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "19"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "20"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "21"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "22"+ tr(" hour") )->setMenu ( pmenu);
 menu.addAction ( "23"+ tr(" hour") )->setMenu ( pmenu);
 connect( &menu,       SIGNAL( hovered ( QAction *)  ), this , SLOT(   Slot_menuHeure_hovered (QAction *)  )  );

 QAction *pQAction =  menu.exec (QCursor::pos());
 if (pQAction && m_pLastQActionHovered)
    {QString  text = m_pLastQActionHovered->text() + " " +pQAction->text();
     qDebug()<<text.toLatin1();

     text = text.remove(tr(" mn"));
     QStringList list = text.split(tr(" hour "));
     m_ui->timeEdit_Heure->setTime(QTime(list[0].toInt(),list[1].toInt(),0));
    }
 delete pmenu;
}

//---------------------------------------- Slot_menuHeure_hovered --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_menuHeure_hovered (QAction *pQAction)
{if (pQAction->menu()) m_pLastQActionHovered = pQAction;    // on ne tient compte que des menus possedant des sous menus.
}

//---------------------------------------- Slot_pushButton_Duree_clicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_pushButton_Duree_clicked()
{QMenu      *pmenu = makeMenuDuree();
 QAction *pQAction = pmenu->exec (QCursor::pos());
 if (pQAction)
    {QString  text = pQAction->text();
     text = text.remove(tr(" minutes"));
     QStringList list = text.split(tr(" hour "));
     m_ui->timeEdit_Duree->setTime(QTime(list[0].toInt(), list[1].toInt(),0));
    }
 delete pmenu;
}
//---------------------------------------- Slot_pushButton_Anonymize_clicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_pushButton_Anonymize_clicked ()
{m_ui->lineEdit_Nom->setText("");
 m_ui->lineEdit_Prenom->setText("");
 m_ui->lineEdit_GUID->setText("");
 m_ui->lineEdit_Tel->setText("");
 m_ui->textEdit_note->setText("");
}

//---------------------------------------- Slot_treeWidget_RdvTypeList_Clicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_treeWidget_RdvTypeList_Clicked( QTreeWidgetItem *pQTreeWidgetItem , int )
{if (pQTreeWidgetItem==0) return;
 if (m_IsNew)
    { int duree     = pQTreeWidgetItem->text(TYP_TIME).toInt()*60;
      int heure     = duree/3600;
      int minutes   = (duree-(heure*3600))/60;
      m_ui->timeEdit_Duree->setTime(QTime(heure, minutes, 0));
    }
}

//---------------------------------------- Slot_treeWidget_RdvTypeList_DoubleClicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_treeWidget_RdvTypeList_DoubleClicked( QTreeWidgetItem *pQTreeWidgetItem , int c)
{if (pQTreeWidgetItem==0) return;
 if (c==1 || c==2)
           {m_ui->treeWidget_RdvTypeList->editItem ( pQTreeWidgetItem, c );
           }
 if (c==0) {QLabel *grabLabel = new QLabel();
            grabLabel->setGeometry (0, 0, 16, 16);
            grabLabel->setStyleSheet(QString("border: 2px solid #010101; background-color: #FFFFFF;"));
            QPixmap pix = QPixmap::grabWidget (grabLabel, 0, 0, 16, 16 );

            QColor color = QColorDialog::getColor ( pQTreeWidgetItem->text(2), this ,tr("Colour of the type of appointment") );
            if (color.isValid())
               { QString coulStr = color.name();
                 grabLabel->setStyleSheet(QString("border: 2px solid #010101; background-color: ")+coulStr+";");
                 pix = QPixmap::grabWidget (grabLabel, 0, 0, 16, 16 );
                 pQTreeWidgetItem->setIcon (0, QIcon(pix) );
                 pQTreeWidgetItem->setText (2, coulStr );
               }
            delete grabLabel;
           }
 if (m_IsNew)
    { int duree     = pQTreeWidgetItem->text(TYP_TIME).toInt()*60;
      int heure     = duree/3600;
      int minutes   = (duree-(heure*3600))/60;
      m_ui->timeEdit_Duree->setTime(QTime(heure, minutes, 0));
    }
}

//---------------------------------------- Slot_pushButton_Quitter_clicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_pushButton_Quitter_clicked()
{reject();
}

//---------------------------------------- Slot_pushButton_Ok_clicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_pushButton_Ok_clicked()
{ m_pColorProfils->clear();
  //............ regenerer la map Ã  partir de la QTreeWidget ..............
  QTreeWidgetItem *pQTreeWidgetItem = 0;
  QTreeWidgetItemIterator it (m_ui->treeWidget_RdvTypeList);
  QString key = "";
  while (*it)
        { pQTreeWidgetItem = (*it);
          key              = pQTreeWidgetItem->text(TYP_NAME);
          //(*m_pColorProfils)[pQTreeWidgetItem->text(TYP_NAME)] = C_ColorType(pQTreeWidgetItem->text(TYP_NAME),pQTreeWidgetItem->text(TYP_COUL),pQTreeWidgetItem->text(TYP_TIME));
          m_pColorProfils->insert(key, C_ColorType(key, pQTreeWidgetItem->text(TYP_COUL), pQTreeWidgetItem->text(TYP_TIME)) );
          ++it;
        }
  if (! m_pC_RendezVous)    // PRINTRDV deb
     {
        m_pCMoteurAgenda->COL_AllDelete();
        m_pCMoteurAgenda->COL_RecordAllColorMap(m_pColorProfils);
     } // PRINTRDV fin
  if ( m_pC_RendezVous )
     {  m_pC_RendezVous->m_Nom      = m_ui->lineEdit_Nom->text().toUpper();
        m_pC_RendezVous->m_Prenom   = m_ui->lineEdit_Prenom->text().toUpper();
        m_pC_RendezVous->m_Tel      = m_ui->lineEdit_Tel->text();
        m_pC_RendezVous->m_GUID     = m_ui->lineEdit_GUID->text();
        m_pC_RendezVous->m_Note     = m_ui->textEdit_note->text();
        m_pC_RendezVous->setDateTime( QDateTime( m_pC_RendezVous->getDateTime().date(), m_ui->timeEdit_Heure->time()) );
        m_pC_RendezVous->m_Duree    = QTime(0,0,0).secsTo ( m_ui->timeEdit_Duree->time() ) /60;
        m_pC_RendezVous->m_PrisPar  = m_ui->lineEdit_PrisPar->text();
        m_pC_RendezVous->m_PrisAvec = m_ui->lineEdit_PrisPour->text();
        m_pC_RendezVous->m_State    = m_ui->comboBox_Statut->currentText();
        QTreeWidgetItem *pQTreeWidgetItem                 = getSelectedListViewItem(m_ui->treeWidget_RdvTypeList);
        if (pQTreeWidgetItem) m_pC_RendezVous->m_Type     = pQTreeWidgetItem->text(TYP_NAME);
     }
  accept();
}

//---------------------------------------- Slot_pushButton_Moins_clicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_pushButton_Moins_clicked()
{QTreeWidgetItem *pQTreeWidgetItem = getSelectedListViewItem(m_ui->treeWidget_RdvTypeList);
 if (pQTreeWidgetItem==0) return;
 delete pQTreeWidgetItem;
}

//--------------------------------- getSelectedListViewItem -------------------------------------------------------------
QTreeWidgetItem * C_Dlg_RdvTypeConfig::getSelectedListViewItem(QTreeWidget *pQTreeWidget)
{    QTreeWidgetItemIterator it(pQTreeWidget);
     while (*it)
       { QTreeWidgetItem *pQListViewItem = (*it);
         if ( pQListViewItem->isSelected () )
            {return pQListViewItem;
            }
         ++it;
       }
 return 0;
}

//---------------------------------------- Slot_pushButton_Plus_clicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_pushButton_Plus_clicked()
{           QLabel *grabLabel = new QLabel();
            grabLabel->setGeometry (0, 0, 16, 16);
            grabLabel->setStyleSheet(QString("border: 2px solid #010101; background-color: #FFFFFF;"));
            QPixmap pix = QPixmap::grabWidget (grabLabel, 0, 0, 16, 16 );

            QColor color = QColorDialog::getColor ( "#FFFFFF", this,tr("Colour of the type of appointment") );
            if (color.isValid())
               { QString coulStr = color.name();
                 grabLabel->setStyleSheet(QString("border: 2px solid #010101; background-color: ")+coulStr+";");
                 pix = QPixmap::grabWidget (grabLabel, 0, 0, 16, 16 );
                 QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem();
                 if (pQTreeWidgetItem)
                    {pQTreeWidgetItem->setText (TYP_NAME, tr("<new type>") );
                     pQTreeWidgetItem->setIcon (TYP_ICON, QIcon(pix) );
                     pQTreeWidgetItem->setText (TYP_COUL, coulStr );
                     pQTreeWidgetItem->setText (TYP_TIME, "15" );
                     pQTreeWidgetItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
                     m_ui->treeWidget_RdvTypeList->addTopLevelItem(pQTreeWidgetItem);
                     m_ui->treeWidget_RdvTypeList->editItem ( pQTreeWidgetItem, 1 );
                    }
               }
            delete grabLabel;
}
//---------------------------------------- changeEvent --------------------------------------------
void C_Dlg_RdvTypeConfig::changeEvent(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//---------------------------------Slot_createTreeViewPatients-------------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_createTreeViewPatients()
{
    QStringList titreColonne;
    titreColonne << tr("Name") << tr("First name") << "Ident" << "GUID" << tr("Birth") << tr("Phone");
    int widthListP      = 575;
    int heightListP     = 305;
    int nbColonneMax    = 6;


    m_pQFrameListPatients = new QFrame(this);
    m_pQFrameListPatients->resize(widthListP,heightListP);
    m_pTreeWidgetPatients = new QTreeWidget(m_pQFrameListPatients);
    m_pTreeWidgetPatients->setObjectName(QString::fromUtf8("listModele")) ;
    m_pTreeWidgetPatients->move(0,0);
    m_pTreeWidgetPatients->resize(widthListP,heightListP);
    m_pTreeWidgetPatients->setColumnCount(nbColonneMax);
    m_pTreeWidgetPatients->setAlternatingRowColors(true);
    m_pTreeWidgetPatients->setColumnWidth(0,150);
    m_pTreeWidgetPatients->setColumnWidth(1,120);
    m_pTreeWidgetPatients->setColumnWidth(4,100);
    m_pTreeWidgetPatients->setHeaderLabels(titreColonne);
    m_pTreeWidgetPatients->header()->setDefaultAlignment(Qt::AlignHCenter);
    m_pQFrameListPatients->move(5, m_ui->lineEdit_Prenom->y() + m_ui->lineEdit_Prenom->height()+m_pTreeWidgetPatients->header()->height());

    connect( m_pTreeWidgetPatients,   SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )), this, SLOT(Slot_TreeWidgetPatients_DoubleClicked( QTreeWidgetItem * , int)) );
}
// ------------- A METTRE DANS UNE CLASSE COMMUNE AVEC MANAGER ......
//--------------------------------- initListePatient ---------------------------------------------------------------------------
void C_Dlg_RdvTypeConfig::initListePatient( const QString & qstr_nom, const QString & qstr_prenom)
{
    // Cr?ation du Treeview si existe pas
    if (!m_pTreeWidgetPatients)
        Slot_createTreeViewPatients();
    m_pQFrameListPatients->show();
    m_pCMoteurAgenda->GetPatientList(m_pTreeWidgetPatients, qstr_nom, qstr_prenom, m_pQLabelStatus );
    m_pTreeWidgetPatients->sortItems (0, Qt::AscendingOrder );
   // effacement du treeview s'il est vide ...
   if (m_pTreeWidgetPatients->topLevelItemCount() == 0)
       m_pQFrameListPatients->hide();

   //m_pLastQListViewItem = 0;
}
//--------------------------------- Slot_lineEdit_Prenom_textChanged -------------------------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_lineEdit_Prenom_textChanged(const QString &)
{   getListePatient();
}
//--------------------------------- Slot_lineEdit_Nom_textChanged -------------------------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_lineEdit_Nom_textChanged(const QString &)
{   getListePatient();
}
//--------------------------------- getListePatient -------------------------------------------------------------
void C_Dlg_RdvTypeConfig::getListePatient()
{   QString nom    = m_ui->lineEdit_Nom->text().trimmed();
    QString prenom = m_ui->lineEdit_Prenom->text().trimmed();
    if ((nom.length()+prenom.length())<3) return;
   //.................. rechercher le separateur de nom ; prenom .......................
   initListePatient( nom, prenom);
}
//-------------------------------------- keyPressEvent --------------------------------------------------------------
void C_Dlg_RdvTypeConfig::keyPressEvent ( QKeyEvent * event )
{
    QTreeWidgetItem * pQTreeWidgetItem = 0;

    switch (event->key()) {
    case Qt::Key_Down:
            if (m_ui->lineEdit_Nom->hasFocus()||m_ui->lineEdit_Prenom->hasFocus())              // Que pour la gestion de la liste patient
               { m_pTreeWidgetPatients->setFocus();
                 pQTreeWidgetItem = m_pTreeWidgetPatients->currentItem();
                 pQTreeWidgetItem->setSelected(true);
               }
            return;

    case Qt::Key_Return:
            if (m_ui->lineEdit_Nom->hasFocus()||m_ui->lineEdit_Prenom->hasFocus())          // si Return sur le nom on ferme la liste
               { if (m_pQFrameListPatients)
                    { if (m_pTreeWidgetPatients->topLevelItemCount()==1)
                         {Valid_Patient_Selected(m_pTreeWidgetPatients->topLevelItem(0));
                         }
                      else
                         {m_pQFrameListPatients->hide();
                         }
                      if (m_ui->lineEdit_Nom->hasFocus()) m_ui->lineEdit_Prenom->setFocus();
                    }
                 return;
               }
            if (m_pTreeWidgetPatients->hasFocus())      // Que pour la gestion de la liste patient
                Valid_Patient_Selected(m_pTreeWidgetPatients->currentItem());
            return;
    case Qt::Key_Escape:
            if (! m_pQFrameListPatients->isHidden())
                {m_pQFrameListPatients->hide();
                }
    default:
        break;
    }
}
//---------------------------------------- Slot_TreeWidgetPatients_DoubleClicked --------------------------------------------
void C_Dlg_RdvTypeConfig::Slot_TreeWidgetPatients_DoubleClicked( QTreeWidgetItem *pQTreeWidgetPatientItem , int)
{
    Valid_Patient_Selected(pQTreeWidgetPatientItem);
}
//--------------------------------- Valid_Patient_Selected -------------------------------------------------------------
void C_Dlg_RdvTypeConfig::Valid_Patient_Selected(QTreeWidgetItem *pQTreeWidgetPatientItem)
{   if (pQTreeWidgetPatientItem==0) return;

    //.......... recuperer les valeurs de pQTreeWidgetPatientItem........................................
    //           avant qu'il ne devienne  invalide
    QString nom    = pQTreeWidgetPatientItem->text(0);
    QString prenom = pQTreeWidgetPatientItem->text(1);
    // QString pk     = pQTreeWidgetPatientItem->text(2);
    QString guid   = pQTreeWidgetPatientItem->text(3);
    // QString dnss   = pQTreeWidgetPatientItem->text(4);
    QString tel    = pQTreeWidgetPatientItem->text(5);

    //......... ne pas affecter directement le resultat ....................
    //          de pQTreeWidgetPatientItem->text() car cela
    //          change lineEdit_Nom et lineEdit_Prenom
    //          ce qui entraine une reconstruction de la list view de par
    //          leur slot Slot_lineEdit_Nom_textChanged et Slot_lineEdit_Prenom_textChanged
    //          avec le pointeur pQTreeWidgetPatientItem qui devient invalide
    //
    m_ui->lineEdit_Nom->setText      (nom);        // apres ca Slot_lineEdit_Nom_textChanged     => pQTreeWidgetPatientItem devient invalide
    m_ui->lineEdit_Prenom->setText   (prenom);     // apres ca Slot_lineEdit_Prenom_textChanged  => pQTreeWidgetPatientItem devient invalide
    m_ui->lineEdit_GUID->setText     (guid);
    m_ui->lineEdit_Tel->setText      (tel);

    m_pQFrameListPatients->hide();
    m_ui->textEdit_note->setFocus();
}
