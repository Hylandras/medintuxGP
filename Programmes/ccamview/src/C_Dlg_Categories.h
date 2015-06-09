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

#ifndef C_DLG_CATEGORIES_H
#define C_DLG_CATEGORIES_H

#include "Dlg_Categories.h"
#include "CMoteurCCAM_Base.h"

class C_Dlg_Categories : public Dlg_Categories
{
  Q_OBJECT

public:
  C_Dlg_Categories(CMoteurCCAM_Base* pCMoteurCCAM_Base,const QString &numero_serie,const QString &code_utilisateur,
                   QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
  ~C_Dlg_Categories();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          combo_Categorie_textChanged( const QString & );
  virtual void          bouton_OK_clicked();
  virtual void          bouton_Annuler_clicked();

protected:
  /*$PROTECTED_FUNCTIONS$*/
   CMoteurCCAM_Base* m_pCMoteurCCAM_Base;
   QString           m_Numero_Serie;
   QString           m_Utilisateur;
protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

