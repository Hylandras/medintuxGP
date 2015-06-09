/******************************** C_Dlg_ChercheRDV.cpp ********************
*  #include "C_Dlg_ChercheRDV.h"           QT4-ready                      *
* ........................................................................*
*   Project   :  MedinTux  (typographie made in france)                   *
*   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
*   by        :  Sevin Roland & co  from   MedinTux project               *
*   E-Mail    :  roland-sevin@medintux.org                                *
*   Web site  :  www.medintux.org                                         *
* ........................................................................*
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#include "C_Dlg_ChercheRDV.h"
#include "ui_C_Dlg_ChercheRDV.h"
#include "C_Dlg_Agenda.h"


//-------------------------------------------- C_Dlg_ChercheRDV -----------------------------
C_Dlg_ChercheRDV::C_Dlg_ChercheRDV(CMoteurAgenda* pCMoteurAgenda, QString nom_prenom_guid /* ="" */, QWidget *parent /* = 0 */) :
    QDialog(parent),
    m_ui(new Ui::C_Dlg_ChercheRDV)
{   m_pTreeWidgetPatients = 0;
    m_pQFrameListPatients = 0;
    m_pCMoteurAgenda      = pCMoteurAgenda;
    m_ui->setupUi(this);
    m_ui->label_loupe->setPixmap(Theme::getIcon("Agenda/loupe.png"));

    connect (m_ui->pushButton_fermer,       SIGNAL(clicked()),                      this, SLOT(Slot_fermer()));
    //connect (m_ui->pushButton_Imprimer,     SIGNAL(clicked()),                      this, SLOT(Slot_imprimer()));
    connect (m_ui->radioButton_futursRDV,   SIGNAL(clicked()),                      this, SLOT(Slot_actualiserListe()));
    connect (m_ui->radioButton_TousLesRDV,  SIGNAL(clicked()),                      this, SLOT(Slot_actualiserListe()));

    m_ui->treeWidget_RDV->setColumnWidth(0,150);
    m_ui->treeWidget_RDV->setColumnWidth(1,50);
    m_ui->treeWidget_RDV->setColumnWidth(2,80);
    m_ui->treeWidget_RDV->setColumnWidth(3,100);
    m_ui->treeWidget_RDV->setColumnWidth(4,50);
    m_ui->treeWidget_RDV->setColumnWidth(5,150);
    if (nom_prenom_guid.length())
       {QStringList lst = nom_prenom_guid.split(';');
        if (lst[0].length()) m_ui->lineEdit_Nom->setText(lst[0]);
        if (lst[1].length()) m_ui->lineEdit_Prenom->setText(lst[1]);
        if (lst[2].length()) m_ui->lineEdit_GUID->setText(lst[2]);
       }
    if (m_ui->lineEdit_GUID->text().length())
       { m_ui->lineEdit_Nom->setReadOnly ( true );
         m_ui->lineEdit_Prenom->setReadOnly ( true );
         m_ui->lineEdit_GUID->setReadOnly ( true );
       }
    else
       {m_ui->lineEdit_GUID->hide();
        m_ui->label_guid->hide();
       }
    Slot_actualiserListe();
    connect( m_ui->lineEdit_Nom,             SIGNAL(textChanged(const QString &)),          this, SLOT(Slot_lineEdit_Nom_textChanged(const QString &)) );       //CZA
    connect( m_ui->lineEdit_Prenom,          SIGNAL(textChanged(const QString &)),          this, SLOT(Slot_lineEdit_Prenom_textChanged(const QString &)) );    //CZA
}

//-------------------------------------------- ~C_Dlg_ChercheRDV -----------------------------
C_Dlg_ChercheRDV::~C_Dlg_ChercheRDV()
{ delete m_ui;
}

//-------------------------------------------- changeEvent -----------------------------
void C_Dlg_ChercheRDV::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-------------------------------------------- Slot_fermer -----------------------------
void C_Dlg_ChercheRDV::Slot_fermer()
{reject();
}
//---------------------------------Slot_createTreeViewPatients-------------------------------------------------
void C_Dlg_ChercheRDV::Slot_createTreeViewPatients()
{
    int widthListP      = 300;
    int heightListP     = 280;
    int nbColonneMax    = 2;

    m_pQFrameListPatients = new QFrame(this);
    m_pQFrameListPatients->move(m_ui->lineEdit_Nom->x(), m_ui->lineEdit_Prenom->y() + m_ui->lineEdit_Prenom->height());
    m_pQFrameListPatients->resize(widthListP,heightListP);
    m_pTreeWidgetPatients = new QTreeWidget(m_pQFrameListPatients);
    m_pTreeWidgetPatients->setObjectName(QString::fromUtf8("listModele")) ;
    m_pTreeWidgetPatients->move(0,0);
    m_pTreeWidgetPatients->resize(widthListP,heightListP);
    m_pTreeWidgetPatients->setColumnCount(nbColonneMax);
    m_pTreeWidgetPatients->setAlternatingRowColors(true);
    m_pTreeWidgetPatients->setColumnWidth(0,150);
    m_pTreeWidgetPatients->setColumnWidth(1,150);
    m_pTreeWidgetPatients->setHeaderHidden(true);

    connect( m_pTreeWidgetPatients,   SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )), this, SLOT(Slot_TreeWidgetPatients_DoubleClicked( QTreeWidgetItem * , int)) );
}
// ------------- A METTRE DANS UNE CLASSE COMMUNE AVEC MANAGER ......

//--------------------------------- initListePatient ---------------------------------------------------------------------------
void C_Dlg_ChercheRDV::initListePatient( const QString & qstr_nom, const QString & qstr_prenom, const QString & guid)
{
    // Creation du Treeview si existe pas
    if (!m_pTreeWidgetPatients)
        Slot_createTreeViewPatients();
    m_pQFrameListPatients->show();
    m_pCMoteurAgenda->GetPatientAgendaList(m_pTreeWidgetPatients, qstr_nom, qstr_prenom, guid, m_pQLabelStatus );
    m_pTreeWidgetPatients->sortItems (0, Qt::AscendingOrder );
   // effacement du treeview s'il est vide ...
   if (m_pTreeWidgetPatients->topLevelItemCount() == 0)
       m_pQFrameListPatients->hide();
}
//--------------------------------- Slot_lineEdit_Prenom_textChanged -------------------------------------------------------------
void C_Dlg_ChercheRDV::Slot_lineEdit_Prenom_textChanged(const QString &)
{   getListePatient();
}
//--------------------------------- Slot_lineEdit_Nom_textChanged -------------------------------------------------------------
void C_Dlg_ChercheRDV::Slot_lineEdit_Nom_textChanged(const QString &)
{   getListePatient();
}
//--------------------------------- getListePatient -------------------------------------------------------------
void C_Dlg_ChercheRDV::getListePatient()
{   QString nom    = m_ui->lineEdit_Nom->text().trimmed();
    QString prenom = m_ui->lineEdit_Prenom->text().trimmed();
   //.................. rechercher le separateur de nom ; prenom .......................
   initListePatient( nom, prenom, m_ui->lineEdit_GUID->text());
}

//-------------------------------------- keyPressEvent --------------------------------------------------------------
void C_Dlg_ChercheRDV::keyPressEvent ( QKeyEvent * event )
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
                {m_pQFrameListPatients->hide();;
                }
    default:
        break;
    }
}

//---------------------------------------- Slot_TreeWidgetPatients_DoubleClicked --------------------------------------------
void C_Dlg_ChercheRDV::Slot_TreeWidgetPatients_DoubleClicked( QTreeWidgetItem *pQTreeWidgetPatientItem , int)
{
    Valid_Patient_Selected(pQTreeWidgetPatientItem);
}
//--------------------------------- getSelectedListViewItem -------------------------------------------------------------
void C_Dlg_ChercheRDV::Valid_Patient_Selected(QTreeWidgetItem *pQTreeWidgetPatientItem)
{   //.......... recuperer les valeurs de pQTreeWidgetPatientItem........................................
    //           avant qu'il ne devienne  invalide
    QString nom    = pQTreeWidgetPatientItem->text(0);
    QString prenom = pQTreeWidgetPatientItem->text(1);
    m_ui->lineEdit_Nom->setText      (nom);
    m_ui->lineEdit_Prenom->setText   (prenom);
    m_pQFrameListPatients->hide();
    //......... ne pas affecter directement le resultat ....................
    //          de pQTreeWidgetPatientItem->text() car cela
    //          change lineEdit_Nom et lineEdit_Prenom
    //          ce qui entraine une reconstruction de la list view de par
    //          leur slot Slot_lineEdit_Nom_textChanged et Slot_lineEdit_Prenom_textChanged
    //          avec le pointeur pQTreeWidgetPatientItem qui devient invalide
    //
    m_pCMoteurAgenda->AfficherLesRdvDuPatient(m_ui->treeWidget_RDV,
                                              nom,
                                              prenom ,
                                              m_ui->radioButton_futursRDV->isChecked()?"FUTURS":"TOUS",
                                              m_ui->lineEdit_GUID->text());
}

//--------------------------------- Slot_actualiserListe -------------------------------------------------------------
void C_Dlg_ChercheRDV::Slot_actualiserListe()
{
 if (m_ui->lineEdit_Nom->text().length() || m_ui->lineEdit_Prenom->text().length() )
    {   m_pCMoteurAgenda->AfficherLesRdvDuPatient(m_ui->treeWidget_RDV,
                                                  m_ui->lineEdit_Nom->text(),
                                                  m_ui->lineEdit_Prenom->text(),
                                                  m_ui->radioButton_futursRDV->isChecked()?"FUTURS":"TOUS",
                                                  m_ui->lineEdit_GUID->text());
    }

}
//--------------------------------- imprimer -------------------------------------------------------------
void C_Dlg_ChercheRDV::Slot_imprimer()
{
}

