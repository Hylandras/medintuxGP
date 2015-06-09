//
// C++ Interface: C_ImageList
//
/********************************* C_ImageList.h *****************************************
                                                                                
    Project: MedinTux                                                            
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design            
    E-Mail: data.medical.design@club-internet.fr                                 
                                                                                 
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

#ifndef C_IMAGELIST_H
#define C_IMAGELIST_H

#include <qmime.h> 
#include <qimage.h>
#include <qpixmap.h> 
#include <qdict.h>

//=============================================== DEFINE =========================================================


#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))


//====================================== C_ImgInf =======================================================
class C_ImgInf {public:
                C_ImgInf(const QString &name, const QString &path="")
                        : m_Name(name), m_Path(path), m_NbRef(1)
                        {
                        }
                ~C_ImgInf(){};
                QString m_Name;
                QString m_Path;
                int m_NbRef;
                } ;

typedef QDict < C_ImgInf > C_IMAGE_REF_DICT;

//====================================== C_ImageList =======================================================
class C_ImageList : public QMimeSourceFactory
{ 
  public:
  C_ImageList();
  ~C_ImageList();
  void  resetRef();
  void  resetList();
  int appendToList(QPixmap *pQPixmap, QString &name, const QString &path = "");
  int appendToList(QPixmap pixmap, QString &name, const QString &path = "");
  int incRef(const QString &name);
  bool setNbRef( const QString &name , int nbRef=0);
  bool getPixmap( const QString &name , QPixmap &retPixMap, QString *pFileName = 0);
  QPixmap *getPixmap( const QString &name , QString *pFileName = 0);
  int removeFromList(const QString &name);
  int getNbRefForThisName(const QString &name);
  QString getSrcImageName( const QString &name);
  int     setSrcImageName( const QString &name, const QString &fileName);
  void destroyFromList(const QString &name);
  QDict < C_ImgInf > m_C_ImageRefDict;
};


#endif

