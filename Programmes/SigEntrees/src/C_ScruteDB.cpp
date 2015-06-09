/******************************* C_ScruteDB.cpp ***************************
*  #include "CGestIni.h"         QT4-ready                                *
* ........................................................................*
*   Project   :  MedinTux  (typographie made in france)                   *
*   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
*   by        :  Sevin Roland     from   MedinTux project                 *
*   E-Mail    :  roland-sevin@medintux.org                                *
*   Web site  :  www.medintux.org                                         *
* ........................................................................*
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

#include "C_ScruteDB.h"
#include <QTreeWidgetItemIterator>
#include "CApp.h"
#include "C_Dlg_SigEntrees.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"
//----------------------------------------- C_ScruteDB -------------------------------
C_ScruteDB::C_ScruteDB(C_Dlg_SigEntrees *pC_Dlg_SigEntrees, QObject *parent) :
    QObject(parent)
{
 m_IsAlreadyView       = Theme::getIcon("16x16/eyes.png");
 m_newPatient          = Theme::getIcon("16x16/newPatient.png");;
 m_resultsChanged      = Theme::getIcon("16x16/list-warning.png");;
 m_pC_Dlg_SigEntrees   = pC_Dlg_SigEntrees;
 m_pC_ScruteDB_Thread  = new C_ScruteDB_Thread(this);
 m_ScrutationTimer     = new QTimer(this);
 m_PatientPk_CPatient_Map.clear();
 setScrutationInterval (qMax(15,(CApp::pCApp()->readUniqueParam("sigentrees session" , "DataBase scrutation").toInt())));
 connect(m_ScrutationTimer,    SIGNAL(timeout())          , this, SLOT(Slot_on_ScrutationTimerOut()));
}
//--------------------------------------- ~C_ScruteDB -------------------------------------------
C_ScruteDB::~C_ScruteDB()
{if (m_ScrutationTimer)     delete m_ScrutationTimer;
 if (m_pC_ScruteDB_Thread)  delete m_pC_ScruteDB_Thread;
}

//--------------------------------------- traiteLaBaseDeDonnee ----------------------------------
// appele par le thread
// Verifie si les entrants ont ete pris en charge
//   - il y a un document posterireur a la date d'entree
// Verifi si une fiche patient a ete creee ??? A VOIR DANS MANAGER
void C_ScruteDB::traiteLaBaseDeDonnee()
{   if (CApp::pCApp()->getDB() == 0)
       {  CApp::pCApp()->outMessage(tr("°       <font color=#ff0000><b> C_ScruteDB::traiteLaBaseDeDonnee() CApp::m_pC_BaseSigEntrees is not instancied, may be database server is unreachable. </b></font>."));    // <font color=#ff5400><b>%1</b></font>
          return;
       }
    CApp::pCApp()->getDB()->MAJ_Patients_PrisEnCharge();

    /*************************************************
    int                 nbNewPatients = 0;
    int                 nbChgPatients = 0;
    QTreeWidgetItem* pQTreeWidgetItem = 0;
    QString                   message = "";
    QString                       str = "";
    //............... relever l'eventuel selectionne pour ...............................
    //                le reselectionner apres reconstruction du treeWidget
    QString id_patientSelected = "";
    QList<QTreeWidgetItem *>  listItemSelected = m_pC_Dlg_SigEntrees->m_pGUI->treeWidget_Patients->selectedItems();
    if (listItemSelected.size())
       {QTreeWidgetItem *pQTreeWidgetItem = listItemSelected[0];
        id_patientSelected = pQTreeWidgetItem->text(7);
       }
    if (m_pC_ScruteDB_Thread->m_clearQTreeWidget) m_pC_Dlg_SigEntrees->m_pGUI->treeWidget_Patients->clear();
    //............ on va cherche la liste des patients en cours .........................
    int limit = m_pC_Dlg_SigEntrees->Slot_fill_CPatientMap(m_PatientPk_CPatient_Map);
    if (limit >= CApp::pCApp()->getResultsLimit().toInt())
       {str = tr("<font color=#ff0000><b>WARNING: Results number %1 exced limits.</b></font>").arg(QString::number(limit));
       }
    else
       {str = tr("<font color=#ff8400><b>%1 results found.</b></font>").arg(QString::number(limit));
       }
    m_pC_Dlg_SigEntrees->setStatusLabeMessage(str);
    CApp::pCApp()->outMessage(str);
    //............. on la compare aux elements du treeWidget_Patients ...................
    //              pour detecter les changements et la modifier en consequence
    foreach (CPatient patient, m_PatientPk_CPatient_Map)
    {//.......... voir si l'item de la liste patient est présent dans le treeWidget (on matche sur le Pk) .............
     QList<QTreeWidgetItem *> itemList = m_pC_Dlg_SigEntrees->m_pGUI->treeWidget_Patients->findItems ( patient.getPkRecord(), Qt::MatchExactly, 7 );
     //.......... si cet item est deja present dans le treeWidget voir si changements ................
     //           on regarde si le nbr de resultats change
     if (itemList.size())
        {pQTreeWidgetItem  = itemList[0];
         if (pQTreeWidgetItem->text(6).toInt() != patient.getNbObx().toInt())
            { ++nbChgPatients;
              pQTreeWidgetItem->setText(6, patient.getNbObx());
              pQTreeWidgetItem->setIcon(0, m_resultsChanged);
              message += tr("<br>°  <font color=#ff8400><b>%1 %2</b></font>  ").arg(patient.m_nom, patient.m_prenom);
            }
        }
     //.......... si cet item est non present dans le treeWidget alors le rajouter ................
     else
        {
          pQTreeWidgetItem = new QTreeWidgetItem(m_pC_Dlg_SigEntrees->m_pGUI->treeWidget_Patients);
          if (pQTreeWidgetItem)
             {pQTreeWidgetItem->setText(0, patient.m_nom);             // nom
              pQTreeWidgetItem->setText(1, patient.m_prenom);          // prenom
              pQTreeWidgetItem->setText(2, patient.getDateExam());     // date
              pQTreeWidgetItem->setText(3, patient.getHeureExam());    // heure
              pQTreeWidgetItem->setText(4, patient.m_dateNaissance);   // Dt Naiss
              pQTreeWidgetItem->setText(5, patient.m_sexe);            // Sexe
              pQTreeWidgetItem->setText(6, patient.getNbObx());        // nb OBX
              pQTreeWidgetItem->setText(7, patient.getPkRecord());     // pk
    //          pQTreeWidgetItem->setText(8, patient.m_Service);         // service
              pQTreeWidgetItem->setIcon(0, m_newPatient);

              ++nbNewPatients;
              message += tr("<br>°  <font color=#ff0000><b>%1 %2</b></font>  ").arg(patient.m_nom, patient.m_prenom);
            }
        }
    } // end foreach (CPatient patient, m_PatientPk_CPatient_Map)
    //.............. reselectionner l'éventuel item sélectionné.............................................
    if (id_patientSelected.length())
      {listItemSelected = m_pC_Dlg_SigEntrees->m_pGUI->treeWidget_Patients->findItems(id_patientSelected, Qt::MatchExactly,7);
       if (listItemSelected.size())
          {m_pC_Dlg_SigEntrees->m_pGUI->treeWidget_Patients->setCurrentItem(listItemSelected[0]);
           listItemSelected[0]->setSelected(true);
           m_pC_Dlg_SigEntrees->Slot_treeWidget_Patients_itemClicked( listItemSelected[0],0);
          }
      }

    emit Sign_DB_AnalyseFinished(message);
    ******/
}
/*****
//Ajout d'une entree dans la liste
//--------------------------------------- Slot_on_ScrutationTimerOut -------------------------------------------
void C_ScruteDB::addEntryIntreeWidget_Patients(CPatient &patient)
{
    QTreeWidgetItem* pQTreeWidgetItem = 0;
    pQTreeWidgetItem = new QTreeWidgetItem(m_pC_Dlg_SigEntrees->m_pGUI->treeWidget_Patients);
    if (pQTreeWidgetItem)
       {pQTreeWidgetItem->setText(0, patient.m_nom);             // nom
        pQTreeWidgetItem->setText(1, patient.m_prenom);          // prenom

        pQTreeWidgetItem->setText(2, patient.getDateExam());     // date
        pQTreeWidgetItem->setText(3, patient.getHeureExam());    // heure
        pQTreeWidgetItem->setText(4, patient.m_dateNaissance);   // Dt Naiss
        pQTreeWidgetItem->setText(5, patient.m_sexe);            // Sexe
        pQTreeWidgetItem->setText(6, patient.getNbObx());        // nb OBX
        pQTreeWidgetItem->setText(7, patient.getPkRecord());     // pk

        pQTreeWidgetItem->setIcon(0, m_newPatient);
    }
}
//
******/
//--------------------------------------- Slot_on_ScrutationTimerOut -------------------------------------------
void C_ScruteDB::Slot_on_ScrutationTimerOut()
{scruteEtTraiteLaBaseDeDonnee();
}

//--------------------------------------- scruteEtTraiteLaBaseDeDonnee ----------------------------------
void C_ScruteDB::scruteEtTraiteLaBaseDeDonnee(int clearQTreeWidget  /* = 0 */)
{ if (m_pC_ScruteDB_Thread == 0)
     { CApp::pCApp()->outMessage(tr("°       <font color=#ff0000><b>m_pC_ScruteDB_Thread is not instancied.</b></font>"));
       return;
     }
  //................. si travail précedent non fini on se casse .....................
  if (m_pC_ScruteDB_Thread->isRunning())
     { CApp::pCApp()->outMessage(tr("°       <font color=#ff0000><b>m_pC_ScruteDB_Thread is running.</b></font>"));
       return;
     }
  CApp::pCApp()->outMessage(tr("================= scruteEtTraiteLaBaseDeDonnee( interval = %1) ================================").arg(QString::number(m_ScrutationTimer->interval()/1000)));
  m_pC_ScruteDB_Thread->m_clearQTreeWidget = clearQTreeWidget;
  m_pC_ScruteDB_Thread->run();
}

//--------------------------------------- setScrutationInterval -----------------------------------------------
void C_ScruteDB::setScrutationInterval(int interval)
{   interval      =  qMax(interval,30);
    bool isActive = m_ScrutationTimer->isActive();
    m_ScrutationTimer->stop();
    m_ScrutationTimer->setInterval(interval*1000);
    if (isActive) m_ScrutationTimer->start();
}

//------------------------------------ startScrutation -------------------------------
void C_ScruteDB::startScrutation()
{setScrutationInterval (CApp::pCApp()->readUniqueParam("sigentrees session" , "DataBase scrutation").toInt());
 scruteEtTraiteLaBaseDeDonnee();
 m_ScrutationTimer->start();
}

//------------------------------------ stopScrutation -------------------------------
void C_ScruteDB::stopScrutation()
{m_ScrutationTimer->stop();
 CApp::pCApp()->outMessage(tr("================= stop DB scrutation ================================"));
 while (m_pC_ScruteDB_Thread->isRunning()) {qApp->processEvents(); }
}

//======================================= C_ScruteDB_Thread ============================================================
//--------------------------------------- C_ScruteDB_Thread -------------------------------------------
C_ScruteDB_Thread::C_ScruteDB_Thread(C_ScruteDB *pC_ScruteDB)
{ m_pC_ScruteDB      = pC_ScruteDB;
  m_clearQTreeWidget = 0;
}

//--------------------------------------- run ----------------------------------------------------------
void C_ScruteDB_Thread::run()
{   CApp::pCApp()->outMessage(tr("C_ScruteDB_Thread::run() thread start "));
    m_pC_ScruteDB->traiteLaBaseDeDonnee();
    CApp::pCApp()->outMessage(tr("C_ScruteDB_Thread::run() thread finished "));
}
