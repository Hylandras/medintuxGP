/********************************* CPreview.h *************************************
 *                                                                                *
 *   #include "CPreview.h"                                                        *
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

#ifndef CPreview_H
#define CPreview_H


#include <qobject.h>
#include <q3filedialog.h> 
#include <qlabel.h>
#include <qpixmap.h>
#include <qimage.h>

#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))

//===============================================================  Preview ============================================================================  

//NOTE: nefonctionne pas avec le mode sélection multiple: fd->setMode(QFileDialog::ExistingFiles);
class CPreview : public QLabel, public Q3FilePreview
{
 public:
        CPreview(QWidget *parent=0 , int maxDim=200) : QLabel( parent ),m_maxDim(maxDim) {}

        void previewUrl( const Q3Url &u )
        {
            QString path = u.path();
            QPixmap pix( path );
            if ( pix.isNull() )
                {setText( tr("This is not a pixmap") );
                }
            else
                {int w           = pix.width ();
                 int h           = pix.height();
                 float coefReduc = 1.00;
                 if (w > h)
                    {if (w > m_maxDim) coefReduc = (float)m_maxDim / (float) w;
                    }
                 else if (h >= w)
                    {if (h > m_maxDim) coefReduc = (float)m_maxDim / (float) h;
                    }
                 if (coefReduc < 1.00)
                    {w = (int) ((float) w * coefReduc);
                     h = (int) ((float) h * coefReduc);
                     QImage   qi    = pix.convertToImage().scale (w, h );
                     pix = QPixmap (qi );
                    }
                 setPixmap( pix );
                }
        }
      int m_maxDim;
    };

#endif


