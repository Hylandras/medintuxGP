//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Sevin Roland <ro@upsis>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

/*
typedef struct _CCAM_ACTE
    {QString m_CodeCCAM;
     QString m_Activite;
     QString m_Phase;
     QString m_Modificateur1;
     QString m_Modificateur2;
     QString m_Modificateur3;
     QString m_Modificateur4;
     QString m_NidPraticien;
     QDate   m_Date;
     QString m_PraticienName;
     QString m_LibelleActe;
     QString m_Droits;
    }CCAM_ACTE;

typedef struct _INFO_MED
      {QString m_Code;
       QString m_Password;
       QString m_Nom;
       QString m_Prenom;
       QString m_Nid;
       QString m_Spec;
      }INFO_MED;
*/

#include "C_Dlg_PatientActeProperty.h"
#include <Q3ListViewItem>

//------------------------------------------------------- C_Dlg_PatientActeProperty -------------------------------------------------------------------
C_Dlg_PatientActeProperty::C_Dlg_PatientActeProperty(CCAM_ACTE*         pCCAM_Acte,
                                                     CMoteurCCAM_Base*  pCMoteurCCAM_Base,
                                                     int mode                            ,  // 0/nouvel acte   1/modifier acte
                                                     QDate &last_Date                    ,
                                                     QTime &last_Time,
                                                     QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
: Dlg_PatientActeProperty(parent,name, modal,fl)
{

    // signals and slots connections
    connect( pushButtonCancel,         SIGNAL( clicked() ),                 this, SLOT( pushButtonCancel_clicked() ) );
    connect( pushButtonOK,             SIGNAL( clicked() ),                 this, SLOT( pushButtonOK_clicked() ) );
    connect( comboBoxPraticien,        SIGNAL( activated(const QString&) ), this, SLOT( comboBoxPraticien_activated(const QString&) ) );
    connect( pushButtonCreerNewActe,   SIGNAL( clicked() ),                 this, SLOT( pushButtonCreerNewActe_clicked() ) );
    connect( pushButtonLastDate,       SIGNAL( clicked() ),                 this, SLOT( pushButtonLastDate_clicked() ) );
    connect( pushButtonLastDate_Jour,  SIGNAL( clicked() ),                 this, SLOT( pushButtonLastDate_Jour_clicked() ) );
    connect( combo_Mod01, SIGNAL( highlighted(const QString&) ), this, SLOT( combo_Mod01_highlighted(const QString&) ) );
    connect( combo_Mod02, SIGNAL( highlighted(const QString&) ), this, SLOT( combo_Mod02_highlighted(const QString&) ) );
    connect( combo_Mod03, SIGNAL( highlighted(const QString&) ), this, SLOT( combo_Mod03_highlighted(const QString&) ) );
    connect( combo_Mod04, SIGNAL( highlighted(const QString&) ), this, SLOT( combo_Mod04_highlighted(const QString&) ) );
    connect( combo_Mod01, SIGNAL( activated(const QString&) ),   this, SLOT( combo_Mod01_activated(const QString&) ) );
    connect( combo_Mod02, SIGNAL( activated(const QString&) ),   this, SLOT( combo_Mod02_activated(const QString&) ) );
    connect( combo_Mod03, SIGNAL( activated(const QString&) ),   this, SLOT( combo_Mod03_activated(const QString&) ) );
    connect( combo_Mod04, SIGNAL( activated(const QString&) ),   this, SLOT( combo_Mod04_activated(const QString&) ) );
    connect( comboBox_Activite, SIGNAL( activated(const QString&) ),                  this, SLOT( comboBox_Activite_activated(const QString&) ) );
    connect( listViewCodeAsso,  SIGNAL( clicked(Q3ListViewItem*,const QPoint&,int) ), this, SLOT( listViewCodeAsso_clicked(Q3ListViewItem*,const QPoint&,int) ) );

 listViewCodeAsso->hide();
 QStringList strList;
 m_pCMoteurCCAM_Base             = pCMoteurCCAM_Base;
 m_pCCAM_Acte                    = pCCAM_Acte;
 m_LastDate                      = last_Date;
 m_LastTime                      = last_Time;
 m_WorkCCAM_Acte                 = *pCCAM_Acte;
 m_WorkCCAM_Acte.m_ActesAssocies = pCCAM_Acte->m_ActesAssocies;
 //............... recuperer informations sur le praticien ...................................................
 INFO_MED info_med;
 m_pCMoteurCCAM_Base->GetInfoMedecinsFromNid(pCCAM_Acte->m_NidPraticien, info_med);

 //............... si droit de changer les utilisateurs alors remplir la combobox avec tous ..................
 if (m_pCCAM_Acte->m_Droits.find("UC=1") != -1)
    {m_pCMoteurCCAM_Base->GetComboPraticiens(comboBoxPraticien);
    }
 //............... si pas le droit de changer les utilisateurs alors remplir la combobox avec un .............
 else
    {comboBoxPraticien->clear();
     comboBoxPraticien->insertItem(0,  info_med.m_Nom    + " " + info_med.m_Prenom ,
                                       info_med.m_Code   + "-" + info_med.m_Nid
                                  );
    }
 //........................... positionner la date et heure de l'acte ........................................
 dateEdit_DateActe->setDate(pCCAM_Acte->m_Date);
 timeEdit_HeureActe->setTime(pCCAM_Acte->m_Heure);
 //........................... conversion de la date pour verif modificateurs possibles ......................
 QString s_date = pCCAM_Acte->m_Date.toString("yyyyMMdd");
 //........................... positionner les modificateurs sur etats possibles .............................
 strList.clear();
 if (m_pCMoteurCCAM_Base->GetModificateurActe(strList, pCCAM_Acte->m_CodeCCAM, s_date))
    {ComboModificateurSetList(combo_Mod01, strList);
     ComboModificateurSetList(combo_Mod02, strList);
     ComboModificateurSetList(combo_Mod03, strList);
     ComboModificateurSetList(combo_Mod04, strList);
     SetComboOnLastStr(combo_Mod01,        pCCAM_Acte->m_Modificateur1);
     SetComboOnLastStr(combo_Mod02,        pCCAM_Acte->m_Modificateur2);
     SetComboOnLastStr(combo_Mod03,        pCCAM_Acte->m_Modificateur3);
     SetComboOnLastStr(combo_Mod04,        pCCAM_Acte->m_Modificateur4);

    }
 else
    {SetComboOnLastStr(combo_Mod01,        "");
     SetComboOnLastStr(combo_Mod02,        "");
     SetComboOnLastStr(combo_Mod03,        "");
     SetComboOnLastStr(combo_Mod04,        "");
     groupBoxComboMod->hide();
    }
 //........................... positionner les combos activité sur etats possibles .............................
 strList.clear();
 if (m_pCMoteurCCAM_Base->GetActiviteActe(strList, pCCAM_Acte->m_CodeCCAM))
    {ComboActiviteSetList(comboBox_Activite, strList, info_med.m_Spec);
     SetComboOnLastStr(comboBox_Activite,  pCCAM_Acte->m_Activite);
    }
 else
    {comboBox_Activite->hide();   // ne devrait jamais arriver !!!
    }
 //........................... positionner les combos Phases sur etats possibles .............................
 strList.clear();
 if (m_pCMoteurCCAM_Base->GetPhasesActe(strList, pCCAM_Acte->m_CodeCCAM))
    {ComboPhasesSetList(comboBox_Phase, strList);
     SetComboOnLastStr(comboBox_Phase,  pCCAM_Acte->m_Phase);
    }
 else
    {comboBox_Phase->hide();   // ne devrait jamais arriver !!!
    }
 textLabel_CodeCCAM->setText(pCCAM_Acte->m_CodeCCAM);
 pCCAM_Acte->m_LibelleActe = m_pCMoteurCCAM_Base->CodeCCAMToLibelleLong(pCCAM_Acte->m_CodeCCAM);
 textEdit_LibelleActe->setText(pCCAM_Acte->m_LibelleActe);
 m_pCCAM_Acte = pCCAM_Acte;
 m_pCMoteurCCAM_Base->SetComboPraticiensOnNID(comboBoxPraticien, pCCAM_Acte->m_NidPraticien);

 //..................... si anesthesiste proposer activité 4 par defaut .....................................
 if (info_med.m_Spec == "02") SetComboOnLastStr(comboBox_Activite,  "4");
 if (mode)
    {pushButtonOK->setText(tr("Modifier acte en cours"));
     if (! (info_med.m_Spec == "02")) pushButtonCreerNewActe->hide();
    }
 else
    {pushButtonCreerNewActe->hide();
    }
 m_mode = mode;
 //................... si pas d'activité 4 desactiver ajout acte anesthesie ........................................
 if (IsExistComboString(comboBox_Activite, tr("Activité 4")) == -1)
    {pushButtonCreerNewActe->hide();
    }
 comboBoxPraticien->setFocus();
 fillListViewAsso( &m_WorkCCAM_Acte, s_date);
 computeAndDisplayTarif();
}

//------------------------------------------------------- ~C_Dlg_PatientActeProperty -------------------------------------------------------------------
C_Dlg_PatientActeProperty::~C_Dlg_PatientActeProperty()
{
}

/*$SPECIALIZATION$*/
//------------------------------------------------------- GetComboPraticienName -------------------------------------------------------------------
QString C_Dlg_PatientActeProperty::GetComboPraticienName()
{return comboBoxPraticien->currentText();
}

//------------------------------------------------------- GetComboPraticienNid -------------------------------------------------------------------
QString C_Dlg_PatientActeProperty::GetComboPraticienNid()
{int         index = comboBoxPraticien->currentIndex(); if (index == -1) return QString::null;
 QVariant       qv = comboBoxPraticien->itemData (index);
 QString      text = qv.toString();
 int             f = text.findRev("-");
 if (f != -1) return text.mid(f+1);
 return QString::null;
}

//------------------------------------------------------- GetComboLastStrMod -------------------------------------------------------------------
QString C_Dlg_PatientActeProperty::GetComboLastStrMod( QComboBox * pQComboBox )
{QString      text = pQComboBox->currentText();
 int             f = text.findRev(" ");
 QString       ret = "";
 if (f != -1)
    {ret = text.mid(f+1);
     if (ret=="0") ret = "";
    }
 return ret;
}

//------------------------------------------------------- GetComboLastStr -------------------------------------------------------------------
QString C_Dlg_PatientActeProperty::GetComboLastStr( QComboBox * pQComboBox )
{QString      text = pQComboBox->currentText();
 int             f = text.findRev(" ");
 if (f != -1) return text.mid(f+1);
 return QString("");
}

//------------------------------------------------------- SetComboOnLastStr -------------------------------------------------------------------
int C_Dlg_PatientActeProperty::SetComboOnLastStr( QComboBox * pQComboBox, const char * str )
{QString str_f = "";
 for (int i = 0; i < pQComboBox->count(); i++)
     {QString      text = pQComboBox->text(i);
      int             f = text.findRev(" ");
      if (f != -1)
         {str_f = text.mid(f+1);
          if (str_f == str)
             {pQComboBox->setCurrentItem (i);
              return i;
             }
         } //endif (f != -1)
     } //endfor (int i = 0; i <= pQComboBox->count(); i++)
 return -1;
}

//------------------------------------------------------- computeAndDisplayTarif -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::computeAndDisplayTarif()
{    CCAM_ACTE  nCCAM_Acte;
     nCCAM_Acte.m_CodeCCAM       = textLabel_CodeCCAM->text();
     nCCAM_Acte.m_Activite       = GetComboLastStr(comboBox_Activite);
     nCCAM_Acte.m_Phase          = GetComboLastStr(comboBox_Phase);
     nCCAM_Acte.m_Modificateur1  = GetComboLastStrMod(combo_Mod01);
     nCCAM_Acte.m_Modificateur2  = GetComboLastStrMod(combo_Mod02);
     nCCAM_Acte.m_Modificateur3  = GetComboLastStrMod(combo_Mod03);
     nCCAM_Acte.m_Modificateur4  = GetComboLastStrMod(combo_Mod04);
     nCCAM_Acte.m_NidPraticien   = GetComboPraticienNid();
     nCCAM_Acte.m_Date           = dateEdit_DateActe->date() ;
     nCCAM_Acte.m_Heure          = timeEdit_HeureActe->time() ;
     nCCAM_Acte.m_PraticienName  = GetComboPraticienName();
     QString  tarif = m_pCMoteurCCAM_Base->GetPrixActe(nCCAM_Acte.m_CodeCCAM,
                                                       nCCAM_Acte.m_Date,
                                                       nCCAM_Acte.m_Code_asso,
                                                       nCCAM_Acte.m_Activite,
                                                       nCCAM_Acte.m_Phase,
                                                       nCCAM_Acte.m_Modificateur1,
                                                       nCCAM_Acte.m_Modificateur2,
                                                       nCCAM_Acte.m_Modificateur3,
                                                       nCCAM_Acte.m_Modificateur4,
                                                       "");
     textLabel_TarifCCAM->setText(tarif);
}

//------------------------------------------------------- comboBox_Activite_activated -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::comboBox_Activite_activated( const QString & )
{fillListViewAsso( &m_WorkCCAM_Acte, dateEdit_DateActe->date().toString("yyyyMMdd"));
 computeAndDisplayTarif();
}

//------------------------------------------------------- combo_Mod04_activated -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::combo_Mod04_activated( const QString & )
{computeAndDisplayTarif();
}

//------------------------------------------------------- combo_Mod03_activated -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::combo_Mod03_activated( const QString & )
{computeAndDisplayTarif();
}

//------------------------------------------------------- combo_Mod02_activated -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::combo_Mod02_activated( const QString & )
{computeAndDisplayTarif();
}

//------------------------------------------------------- combo_Mod01_activated -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::combo_Mod01_activated( const QString & )
{computeAndDisplayTarif();
}

//------------------------------------------------------- pushButtonLastDate_Jour_clicked -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::pushButtonLastDate_Jour_clicked()
{dateEdit_DateActe->setDate(QDate::currentDate());
 timeEdit_HeureActe->setTime(QTime::currentTime());
}

//------------------------------------------------------- pushButtonLastDate_clicked -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::pushButtonLastDate_clicked()
{dateEdit_DateActe->setDate(m_LastDate);
 timeEdit_HeureActe->setTime(m_LastTime);
}

//------------------------------------------------------- pushButtonCancel_clicked -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::pushButtonCancel_clicked()
{reject();
}

//------------------------------------------------------- pushButtonOK_clicked -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::pushButtonOK_clicked()
{  //m_pCCAM_Acte->m_CodeCCAM       = ; inchangé
     m_pCCAM_Acte->m_Activite       = GetComboLastStr(comboBox_Activite);
     m_pCCAM_Acte->m_Phase          = GetComboLastStr(comboBox_Phase);
     m_pCCAM_Acte->m_Modificateur1  = GetComboLastStrMod(combo_Mod01);
     m_pCCAM_Acte->m_Modificateur2  = GetComboLastStrMod(combo_Mod02);
     m_pCCAM_Acte->m_Modificateur3  = GetComboLastStrMod(combo_Mod03);
     m_pCCAM_Acte->m_Modificateur4  = GetComboLastStrMod(combo_Mod04);
     m_pCCAM_Acte->m_NidPraticien   = GetComboPraticienNid();
     m_pCCAM_Acte->m_Date           = dateEdit_DateActe->date() ;
     m_pCCAM_Acte->m_Heure          = timeEdit_HeureActe->time() ;
     m_pCCAM_Acte->m_PraticienName  = GetComboPraticienName();
     m_pCCAM_Acte->m_ActesAssocies  = m_WorkCCAM_Acte.m_ActesAssocies;
     accept();
}

//------------------------------------------------------- pushButtonCreerNewActe_clicked -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::pushButtonCreerNewActe_clicked()
{//m_pCCAM_Acte->m_CodeCCAM       = ; inchangé
     m_pCCAM_Acte->m_Activite       = "4";
     m_pCCAM_Acte->m_Phase          = GetComboLastStr(comboBox_Phase);
     m_pCCAM_Acte->m_Modificateur1  = GetComboLastStrMod(combo_Mod01);
     m_pCCAM_Acte->m_Modificateur2  = GetComboLastStrMod(combo_Mod02);
     m_pCCAM_Acte->m_Modificateur3  = GetComboLastStrMod(combo_Mod03);
     m_pCCAM_Acte->m_Modificateur4  = GetComboLastStrMod(combo_Mod04);
     m_pCCAM_Acte->m_NidPraticien   = GetComboPraticienNid();
     m_pCCAM_Acte->m_Date           = dateEdit_DateActe->date() ;
     m_pCCAM_Acte->m_Heure          = timeEdit_HeureActe->time() ;
     m_pCCAM_Acte->m_PraticienName  = GetComboPraticienName();
     m_pCCAM_Acte->m_ActesAssocies  = m_WorkCCAM_Acte.m_ActesAssocies;
     done (111);
}

//------------------------------------------------------- IsExistComboString -------------------------------------------------------------------
int C_Dlg_PatientActeProperty::IsExistComboString( QComboBox * pQComboBox, const char * str )
{int nb = pQComboBox->count();
 for (int i=0; i<nb; ++i)
    {if (pQComboBox->text(i) == str) return i;
    }
 return -1;
}

//------------------------------------------------------- combo_Mod_highlighted -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::combo_Mod_highlighted( const QString & text )
{int                   f = text.findRev(" ");
 QString       codeModif = "";
 if (f != -1)
    {codeModif = text.mid(f+1);
     if (codeModif=="0") codeModif = "";
    }
 if (codeModif == "")
      text_Libelle_Modif->setText(tr("Pas de modificateur placé sur cet acte"));
 else
      text_Libelle_Modif->setText(m_pCMoteurCCAM_Base->GetLibelleModificateur(codeModif));
}

//------------------------------------------------------- combo_Mod04_highlighted -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::combo_Mod04_highlighted( const QString & text )
{combo_Mod_highlighted( text );
}

//------------------------------------------------------- combo_Mod03_highlighted -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::combo_Mod03_highlighted( const QString & text )
{combo_Mod_highlighted( text );
}

//------------------------------------------------------- combo_Mod02_highlighted -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::combo_Mod02_highlighted( const QString & text )
{combo_Mod_highlighted( text );
}

//------------------------------------------------------- combo_Mod01_highlighted -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::combo_Mod01_highlighted( const QString & text )
{combo_Mod_highlighted( text );
}

//------------------------------------------------------- ComboModificateurSetList -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::ComboModificateurSetList( QComboBox * pQComboBox, QStringList & strList )
{pQComboBox->clear();
 pQComboBox->insertItem (tr("Modif 0"));  // "Modif 0"
 QString str("");
 for ( QStringList::Iterator it = strList.begin(); it != strList.end(); ++it )
     {str = "Modif "; str += *it; // + " "+m_pCMoteurCCAM_Base->GetLibelleModificateur(*it);
      if (IsExistComboString(pQComboBox, str)==-1) pQComboBox->insertItem (str);
     }
 if (strList.count()==0)
    {groupBoxComboMod->hide();
    }
 else
    {groupBoxComboMod->show();
    }
}

//------------------------------------------------------- ComboPhasesSetList -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::ComboPhasesSetList( QComboBox * pQComboBox, QStringList & strList )
{pQComboBox->clear();
 QString str("");
 for ( QStringList::Iterator it = strList.begin(); it != strList.end(); ++it )
     {str = "Phase "; str += *it;
      if (IsExistComboString(pQComboBox, str)==-1) pQComboBox->insertItem (str);
     }
}

//------------------------------------------------------- ComboActiviteSetList -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::ComboActiviteSetList( QComboBox * pQComboBox, QStringList & strList, QString spec )
{pQComboBox->clear();
 QString str("");
 for ( QStringList::Iterator it = strList.begin(); it != strList.end(); ++it )
     {str = tr("Activité "); str += *it;
      /*
      if (spec == "02")
         {if (IsExistComboString(pQComboBox, str)==-1 ) pQComboBox->insertItem (str);
         }
      else
         {if (*it != "4" && IsExistComboString(pQComboBox, str)==-1 ) pQComboBox->insertItem (str);
         }
      */
      if (IsExistComboString(pQComboBox, str)==-1 ) pQComboBox->insertItem (str);
     }
}

//------------------------------------------------------- comboBoxPraticien_activated -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::comboBoxPraticien_activated( const QString & )
{QString nid = GetComboPraticienNid();
 INFO_MED info_med;
 m_pCMoteurCCAM_Base->GetInfoMedecinsFromNid(nid, info_med);
 QStringList strList;
 if (m_pCMoteurCCAM_Base->GetActiviteActe(strList, m_pCCAM_Acte->m_CodeCCAM))
    {ComboActiviteSetList(comboBox_Activite, strList, info_med.m_Spec);
     SetComboOnLastStr(comboBox_Activite,  m_pCCAM_Acte->m_Activite);
     //..................... si anesthesiste proposer activité 4 par defaut .....................................
     if (info_med.m_Spec == "02") SetComboOnLastStr(comboBox_Activite,  "4");
     if (m_mode)    // modifier
        {if (! (info_med.m_Spec == "02"))            // si pas anesthesiste cacher bouton : "ajouter acte anesthésie"
            {pushButtonCreerNewActe->hide();
            }
         else
            {pushButtonCreerNewActe->show();      // si pas anesthesiste montrer bouton : "ajouter acte anesthésie"
            }
        }
     else //.............. si pas modification cacher "ajouter acte anesthésie" .......................
        {pushButtonCreerNewActe->hide();
        }
    }
 else
    {comboBox_Activite->hide();   // ne devrait jamais arriver !!!
    }
 //................... si pas d'activité 4 desactiver ajout acte anesthesie ........................................
 if (IsExistComboString(comboBox_Activite, tr("Activité 4")) == -1)
    {pushButtonCreerNewActe->hide();
    }
}

//------------------------------------------------------- listViewCodeAsso_clicked -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::listViewCodeAsso_clicked( Q3ListViewItem * pQListViewItem, const QPoint &, int )
{if (pQListViewItem==0) return;
 QString codeCCAM = pQListViewItem->text(0);
 int index = m_WorkCCAM_Acte.m_ActesAssocies.findIndex ( codeCCAM );
 if (index != -1)
    {m_WorkCCAM_Acte.m_ActesAssocies.remove ( m_WorkCCAM_Acte.m_ActesAssocies.at(index) );
     pQListViewItem->setPixmap (0, QPixmap( GlobalPathAppli + SYS_FOLDER + "/CheckBoxOff.png" ) );
    }
 else
    {m_WorkCCAM_Acte.m_ActesAssocies.append(codeCCAM);
     pQListViewItem->setPixmap (0, QPixmap( GlobalPathAppli + SYS_FOLDER + "/CheckBoxOn.png" ) );
    }
 //fillListViewAsso( &m_WorkCCAM_Acte,  dateEdit_DateActe->date().toString("yyyyMMdd"));
}

//------------------------------------------------------- fillListViewAsso -------------------------------------------------------------------
void C_Dlg_PatientActeProperty::fillListViewAsso( CCAM_ACTE * pCCAM_Acte, const QString & dateActe )
{//......................... recherche des codes associés ...............................................
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 listViewCodeAsso->clear();
 CODE_ASSO_LIST codeAssoList;
 if (m_pCMoteurCCAM_Base->GetActesAssocies( pCCAM_Acte->m_CodeCCAM, GetComboLastStr(comboBox_Activite) , dateActe, codeAssoList)>0)
    {CODE_ASSO_LIST::iterator it;
     for ( it = codeAssoList.begin(); it != codeAssoList.end(); ++it )
         {QString codeCCAM = (*it).m_CodeCCAM;
          Q3ListViewItem *pQListViewItem = new Q3ListViewItem(listViewCodeAsso,
                                                            codeCCAM,
                                                            (*it).m_Activite,
                                                            m_pCMoteurCCAM_Base->GetPrixBaseActe((*it).m_CodeCCAM, (*it).m_Activite, "0", dateActe ),
                                                            (*it).m_IsCompOrAsso,
                                                            (*it).m_LibelleActe,
                                                            (*it).m_regleAsso
                                                           );
          int index = pCCAM_Acte->m_ActesAssocies.findIndex ( codeCCAM );
          if ( index != -1) pQListViewItem->setPixmap (0, QPixmap( GlobalPathAppli + SYS_FOLDER + "/CheckBoxOn.png" ) );
          else              pQListViewItem->setPixmap (0, QPixmap( GlobalPathAppli + SYS_FOLDER + "/CheckBoxOff.png" ) );
         }
     listViewCodeAsso->show();
     lineRight->show();
     lineLeft->show();
     textLabelAsso->show();
    }
 else
    {listViewCodeAsso->hide();
     lineRight->hide();
     lineLeft->hide();
     textLabelAsso->hide();
    }
 QApplication::restoreOverrideCursor();
}



