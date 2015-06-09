//
// C++ Interface: 
//
// Description: 
//
//
// Author: Sevin Roland <ro@upsis>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef C_DLG_PRATICIENS_H
#define C_DLG_PRATICIENS_H

#include "Dlg_Praticiens.h"
#include <Q3ListViewItem>
#include "CMoteurCCAM_Base.h"
#include "qmessagebox.h"

class C_Dlg_Praticiens : public Dlg_Praticiens
{
  Q_OBJECT

public:
  C_Dlg_Praticiens(CMoteurCCAM_Base* pCMoteurCCAM_Base, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
  ~C_Dlg_Praticiens();
  /*$PUBLIC_FUNCTIONS$*/
  virtual int           VerifSIGEMSValiditation();
  //virtual int           initDialog( CMoteurCCAM_Base * pCMoteurCCAM_Base );

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          pushButtonDelete_clicked();
  virtual void          bouton_New_clicked();
  virtual void          EffaceSaisie();
  virtual void          bouton_OK_clicked();
  virtual void          lineEdit_ConfMot_textChanged( const QString & );
  virtual void          lineEdit_MotDePasse_textChanged( const QString & );
  virtual void          lineEdit_Spec_textChanged( const QString & );
  virtual void          lineEdit_Nid_textChanged( const QString & text );
  virtual void          lineEdit_Prenom_textChanged( const QString & );
  virtual void          lineEdit_Nom_textChanged( const QString & );
  virtual void          AfficheInfosMedecins( const QString & nidMEDECIN );
  virtual void          lineEdit_Code_textChanged( const QString & );
  virtual void          listview_praticiens_clicked( Q3ListViewItem * pQListViewItem );
  virtual void          bouton_Quitter_clicked();

protected:
  /*$PROTECTED_FUNCTIONS$*/
  CMoteurCCAM_Base* m_pCMoteurCCAM_Base;

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

