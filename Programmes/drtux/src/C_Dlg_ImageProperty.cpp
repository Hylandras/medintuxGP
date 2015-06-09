//
// C++ Implementation:
/*********************************** C_Dlg_ImageProperty.cpp *********************
 *                                                                                *
 *   #include "C_Dlg_ImageProperty.h"                                             *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
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

#include <qpushbutton.h>
#include <qlineedit.h>
#include <qslider.h>
#include <qimage.h>
#include <qsize.h>


#include "C_Dlg_ImageProperty.h"
#include "CApp.h"
#include "MyEditText.h"
//#include "C_RubDocuments.h"

//---------------------------------------------------------------- C_Dlg_ImageProperty -------------------------------------------------------------------------------------
C_Dlg_ImageProperty::C_Dlg_ImageProperty(MyEditText* pMyEditText,
                                         const QString& imageName,
                                         C_ImageList *pC_ImageList,
                                         QWidget* parent, const char* name, bool modal, WFlags fl)
: Dlg_ImageProperty(parent, name, modal, fl) , m_pMyEditText(pMyEditText), m_ImageName(imageName), m_pC_ImageList (pC_ImageList)
{
     textLabel_Name->setText(tr("Nom de l'image : ")+m_ImageName);
     QString imageSrcFile = m_pC_ImageList->getSrcImageName( m_ImageName).stripWhiteSpace();

     if      (imageSrcFile.length()==0)      {textEdit_FileName->setText(tr("Fichier source non indiqué ") );                      pushButton_VoirSource->setEnabled(FALSE); }
     else if (QFile::exists( imageSrcFile )) {textEdit_FileName->setText(tr("Fichier source : ") + imageSrcFile);                  pushButton_VoirSource->setEnabled(TRUE);  }
     else                                    {textEdit_FileName->setText(tr("Fichier source (non trouvé) : ") + imageSrcFile);     pushButton_VoirSource->setEnabled(FALSE); }
  //  }
 m_IconIsChecked.setPixmap        (QPixmap(G_pCApp->m_PathAppli + "Ressources/icones/button_Ok.png"),     QIconSet::Automatic);
 m_IconIsNotChecked.setPixmap     (QPixmap(G_pCApp->m_PathAppli + "Ressources/icones/button_NotOk.png"),  QIconSet::Automatic);


 slider_Width->hide();
 slider_height->hide();
 pushButton_Save->hide();
 pushButton_Annuler->hide();
 connect( pushButton_Ok,               SIGNAL( clicked()),         this,    SLOT  ( accept())  );
 connect( pushButton_Annuler,          SIGNAL( clicked()),         this,    SLOT  ( reject())  );
 connect( pushButton_Save,             SIGNAL( clicked()),         this,    SLOT  ( Exporter())  );
 connect( pushButton_VoirSource,       SIGNAL( clicked()),         this,    SLOT  ( VoirSource())  );
 VoirSource();
}

//---------------------------------------------------------------- ~C_Dlg_ImageProperty -------------------------------------------------------------------------------------
C_Dlg_ImageProperty::~C_Dlg_ImageProperty()
{
}

/*$SPECIALIZATION$*/
//---------------------------------------------------------------- reject -------------------------------------------------------------------------------------
void C_Dlg_ImageProperty::reject()
{
  QDialog::reject();
}

//---------------------------------------------------------------- accept -------------------------------------------------------------------------------------
void C_Dlg_ImageProperty::accept()
{
  QDialog::accept();
}

//---------------------------------------------------------------- VoirSource -------------------------------------------------------------------------------------
void C_Dlg_ImageProperty::VoirSource()
{   QPixmap retPixMap;
    if (pushButton_VoirSource->isOn () )
      { retPixMap = QPixmap(m_pC_ImageList->getSrcImageName(m_ImageName));
        pushButton_VoirSource->setIconSet (m_IconIsChecked);
      }
    else
      { m_pC_ImageList->getPixmap(m_ImageName, retPixMap);
        pushButton_VoirSource->setIconSet (m_IconIsNotChecked);
      }

    lineEdit_width->setText(  QString::number(retPixMap.width()) );
    lineEdit_height->setText( QString::number(retPixMap.height()) );
    int w           = retPixMap.width ();
    int h           = retPixMap.height();
    float coefReduc = 1.00;
    int      maxDim = 600;
    if (w > h)
       {if (w > maxDim) coefReduc = (float)maxDim / (float) w;
       }
    else if (h >= w)
      {if (h > maxDim) coefReduc = (float)maxDim / (float) h;
      }
    if (coefReduc < 1.00)
       {w = (int) ((float) w * coefReduc);
        h = (int) ((float) h * coefReduc);
        QImage   qi    = retPixMap.convertToImage().scale (w, h );
        retPixMap = QPixmap (qi );
       }
    pixmapLabel_Image->resize ( QSize (w,h) );
    pixmapLabel_Image-> setPixmap ( retPixMap );
}

//---------------------------------------------------------------- Exporter -------------------------------------------------------------------------------------
void C_Dlg_ImageProperty::Exporter()
{QPixmap retPixMap;
 if (m_pC_ImageList->getPixmap( m_ImageName , retPixMap))
    {   QImageIO iio;
        QImage   im;
        im = retPixMap; // convert to image
        iio.setImage( im );
        iio.setFileName( "/home/ro/test.png" );
        iio.setFormat( "PNG" );
        iio.write();
     }
}

