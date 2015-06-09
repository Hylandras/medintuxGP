 /********************************* .cpp **********************************
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


#include <qlistbox.h>
#include <qlineedit.h>
#include <qlistview.h>

#include "CApp.h"
#include "CDlg_newclassdenombre.h"

CDlg_NewClassDenombre::CDlg_NewClassDenombre(QString *pnom, QString *pchamp, QListView *pQListViewMetaField ,QWidget* parent, const char* name, bool modal, WFlags fl)
: MyDialog1(parent,name, modal,fl)
{m_pName  = pnom;
 m_pChamp = pchamp;
 QStringList popItem;
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_NOM);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_INDEX_PRENOM);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NSS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_SEX);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_JFNOM);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NB_ENF);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_RUE);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_CODE_POST);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_VILLE);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_CEDEX);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_PAYS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL1);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL2);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL3);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL1_TYP);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL2_TYP);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TEL3_TYP);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_EMAIL);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NUMSS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_IS_ASS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_NOM_ASS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_PRENOM_ASS);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_PROF);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_IDENT_TITRE);
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_NOM_DATE);        // Court texte descriptif de l'enregistrement.
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_DATE);   // date de l'enregistrement
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_FIN);  // duree de fin de l'enregistrement
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_TYPE);   // Type de rubrique
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_USER);   // utilisateur associé à rubrique
 popItem.append(tr( "° ") + G_pCApp->m_pCMoteurBase->m_DOSS_RUB_HEAD_SIGN_USER);       // utilisateur associé signataire de la rubrique

 QListViewItemIterator it( pQListViewMetaField );
 while ( it.current() )
    { QListViewItem *pQListViewItem = it.current();
      popItem.append( tr("* ") + pQListViewItem->text(0) );
      ++it;
    }

 comboBox_FieldName->insertStringList ( popItem );
 QListBoxItem* pQListBoxItem =  comboBox_FieldName->listBox()->findItem ( *m_pChamp, Qt::ExactMatch );
 if (pQListBoxItem)
    { comboBox_FieldName->listBox()->setCurrentItem (pQListBoxItem );
      comboBox_FieldName->setCurrentText (*m_pChamp );
    }
 lineEdit_ClassName->setText(*m_pName);
}

CDlg_NewClassDenombre::~CDlg_NewClassDenombre()
{
}

/*$SPECIALIZATION$*/
void CDlg_NewClassDenombre::reject()
{ QDialog::reject();
}

void CDlg_NewClassDenombre::accept()
{ *m_pChamp = comboBox_FieldName->currentText().stripWhiteSpace();
  *m_pName   = lineEdit_ClassName->text().stripWhiteSpace();
  QDialog::accept();
}



