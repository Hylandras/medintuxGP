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

#ifndef C_DLG_PRINT_H
#define C_DLG_PRINT_H

#include <Q3SimpleRichText>
#include <Q3TextEdit>
//#include <QX11Info>
#include <qdir.h>
#include <qapplication.h>
#include <QDesktopWidget>
#include <Q3SimpleRichText>
#include "CMoteurCCAM_Base.h"
#include "Global.h"
#include "DlgPrint.h"
#include "CPrinter.h"
class C_Dlg_Print : public DlgPrint
{
  Q_OBJECT

public:
  C_Dlg_Print(Q3TextEdit*       pQTextEdit,
              QString*          pHeadText,
              QString*          pBodyText,
              QString*          pFootText,
              const QString   &printSectionName,
              QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
  ~C_Dlg_Print();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          pushButtonNext_clicked();
  virtual void          pushButtonPrevious_clicked();
  virtual void          pushButtonConfigurer_clicked();
  virtual void          pushButtonQuitter_clicked();
  virtual void          pushButtonPrint_clicked();
  virtual void          pushButtonOk_clicked();
  virtual void          initRichFootText( Q3SimpleRichText * pRichText );
  virtual void          initRichHeadText( Q3SimpleRichText * pRichText );
  virtual void          initRichBodyText( Q3SimpleRichText * pRichText );
  virtual void          AdjustHeightFootAndHead();
  virtual void          SetDialogOnHeadAndFoot( int mode );
  virtual void          pushButtonSave_clicked();
  virtual void          UpdateTextePageInfo( int nb_page );
  virtual void          slider_Foot_H_sliderMoved( int value );
  virtual void          slider_Marg_Bottom_sliderMoved( int value );
  virtual void          slider_Marg_Right_sliderMoved( int value );
  virtual void          slider_Marg_Left_sliderMoved( int value );
  virtual void          slider_Head_H_sliderMoved( int value );
  virtual void          slider_Marg_Top_sliderMoved( int value );
 //virtual void          initDialog( Q3TextEdit * pQTextEdit, QString * pHeadText, QString * pBodyText, QString * pFootText );

protected:
  /*$PROTECTED_FUNCTIONS$*/
    QString           * m_pBodyText;
    QString           * m_pFootText;
    QString           * m_pHeadText;
    Q3SimpleRichText  * m_pFootRichText;
    Q3SimpleRichText  * m_pBodyRichText;
    Q3SimpleRichText  * m_pHeadRichText;
    QPrinter*           m_pQPrinter;
    float               m_PageH;
    float               m_PageW;
    CViewPageWidget   * m_pCViewPageWidget;
    Q3TextEdit        * m_pBodyTextEdit;
    Q3ListView        * m_pQListViewActes;
    IDENT_PAT         * m_pIdentPat;
    QString             m_PrintSectionName;
protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

