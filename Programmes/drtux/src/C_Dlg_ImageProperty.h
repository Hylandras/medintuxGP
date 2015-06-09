//
// C++ Interface: 
//
// Description: 
//
//
// Author: Sevin Roland <upsis@club-internet.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef C_DLG_IMAGEPROPERTY_H
#define C_DLG_IMAGEPROPERTY_H

#include "ui/Dlg_ImageProperty.h"
#include <qiconset.h>
class C_ImageList;
class MyEditText;

class C_Dlg_ImageProperty : public Dlg_ImageProperty
{
  Q_OBJECT

public:
  C_Dlg_ImageProperty(MyEditText* pMyEditText,
                      const QString& imageName,
                      C_ImageList *pC_ImageList,
                      QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~C_Dlg_ImageProperty();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/
    MyEditText    *m_pMyEditText;
    QString        m_ImageName;
    C_ImageList   *m_pC_ImageList;
    QIconSet       m_IconIsNotChecked;
    QIconSet       m_IconIsChecked;
protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          reject();
  virtual void          accept();
  virtual void          Exporter();
  virtual void          VoirSource();
};

#endif

