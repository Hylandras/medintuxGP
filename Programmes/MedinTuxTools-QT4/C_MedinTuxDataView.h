// C++ Implementation:
/********************************* C_MedinTuxDataView.h *************************
                                                                                 
    Project: MedinTux                                                            
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design            
    E-Mail: roland-sevin@medintux.org                                     
                                                                                 
    This program is free software; you can redistribute it and/or modify         
    it under the terms of the CeCILL Version 1 du 21/06/2004                     
    License (GPL compatible)                                                     
                               http://www.cecill.info/                           
    as published by :                                                            
                                                                                 
    Commissariat � l'Energie Atomique                                            
    - CEA,                                                                       
                             31-33 rue de la F�d�ration, 75752 PARIS cedex 15.   
                             FRANCE                                              
    Centre National de la Recherche Scientifique                                 
    - CNRS,                                                                      
                             3 rue Michel-Ange, 75794 Paris cedex 16.            
                             FRANCE                                              
    Institut National de Recherche en Informatique et en Automatique             
    - INRIA,                                                                     
                             Domaine de Voluceau, Rocquencourt, BP 105, 78153    
                             FRANCE                                              
                                                                                 
          This program is distributed in the hope that it will be useful,        
          but WITHOUT ANY WARRANTY; without even the implied warranty of         
          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   
          CeCILL  License (GPL compatible) for more details.                     
                                                                                 
*/

#ifndef C_MedinTuxDataView_H
#define C_MedinTuxDataView_H

#include <qtextbrowser.h>
#include "C_ImageList.h"

#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))

//===============================================================  C_MedinTuxDataView =========================================================================
class C_MedinTuxDataView : public C_ImageList
{
 public:
     C_MedinTuxDataView(){};
    ~C_MedinTuxDataView(){};
     QString prepareDataToView(const QString &data);
     QPixmap *decodeStringTo_pQPixmap( const QString& in );
     QPixmap decodeStringTo_QPixmap( const QString& in );

};

#endif
