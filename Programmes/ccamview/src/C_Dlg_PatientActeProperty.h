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

#ifndef C_DLG_PATIENTACTEPROPERTY_H
#define C_DLG_PATIENTACTEPROPERTY_H

#include "Dlg_PatientActeProperty.h"
#include <Q3ListViewItem>
#include "CMoteurCCAM_Base.h"
#include "Global.h"
#include "qlineedit.h"
#include "qapplication.h"

class C_Dlg_PatientActeProperty : public Dlg_PatientActeProperty
{
  Q_OBJECT

public:
  C_Dlg_PatientActeProperty(CCAM_ACTE*         pCCAM_Acte,
                            CMoteurCCAM_Base*  pCMoteurCCAM_Base,
                            int mode                            ,  // 0/nouvel acte   1/modifier acte
                            QDate &last_Date                    ,
                            QTime &last_Time,
                            QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
  ~C_Dlg_PatientActeProperty();
  /*$PUBLIC_FUNCTIONS$*/
  virtual QString       GetComboPraticienName();
  virtual QString       GetComboPraticienNid();
  virtual QString       GetComboLastStrMod( QComboBox * pQComboBox );
  virtual QString       GetComboLastStr( QComboBox * pQComboBox );
  virtual int           SetComboOnLastStr( QComboBox * pQComboBox, const char * str );
  //virtual int           initDialog( CCAM_ACTE * pCCAM_Acte, CMoteurCCAM_Base * pCMoteurCCAM_Base, int mode, QDate & last_Date, QTime & last_Time );
  //virtual void          init();

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          computeAndDisplayTarif();
  virtual void          comboBox_Activite_activated( const QString & );
  virtual void          combo_Mod04_activated( const QString & );
  virtual void          combo_Mod03_activated( const QString & );
  virtual void          combo_Mod02_activated( const QString & );
  virtual void          combo_Mod01_activated( const QString & );
  virtual void          pushButtonLastDate_Jour_clicked();
  virtual void          pushButtonLastDate_clicked();
  virtual void          pushButtonCancel_clicked();
  virtual void          pushButtonOK_clicked();
  virtual void          pushButtonCreerNewActe_clicked();
  virtual int           IsExistComboString( QComboBox * pQComboBox, const char * str );
  virtual void          combo_Mod_highlighted( const QString & text );
  virtual void          combo_Mod04_highlighted( const QString & text );
  virtual void          combo_Mod03_highlighted( const QString & text );
  virtual void          combo_Mod02_highlighted( const QString & text );
  virtual void          combo_Mod01_highlighted( const QString & text );
  virtual void          ComboModificateurSetList( QComboBox * pQComboBox, QStringList & strList );
  virtual void          ComboPhasesSetList( QComboBox * pQComboBox, QStringList & strList );
  virtual void          ComboActiviteSetList( QComboBox * pQComboBox, QStringList & strList, QString spec );
  virtual void          comboBoxPraticien_activated( const QString & );
  virtual void          listViewCodeAsso_clicked( Q3ListViewItem * pQListViewItem, const QPoint &, int );
  virtual void          fillListViewAsso( CCAM_ACTE * pCCAM_Acte, const QString & dateActe );

protected:
  /*$PROTECTED_FUNCTIONS$*/
 CCAM_ACTE m_WorkCCAM_Acte;
 QTime m_LastTime;
 int m_mode;
 CMoteurCCAM_Base* m_pCMoteurCCAM_Base;
 CCAM_ACTE* m_pCCAM_Acte;
 QDate m_LastDate;

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

