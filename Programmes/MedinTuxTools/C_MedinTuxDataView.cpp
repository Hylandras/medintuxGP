// C++ Implementation:
/********************************* C_MedinTuxDataView.cpp ***********************
                                                                                 
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

#include "C_MedinTuxDataView.h"
#include "Codecs/qmdcodec.h"


//===============================================================  C_MedinTuxDataView =========================================================================

//----------------------------- prepareDataToView -----------------------------------
/*! \brief Fonction qui transforme les donnees multimedia brute de medintux en donnees affichables... 
 *  \param in :  QString qui contient les donnes brutes html et base64
 *  \return QString  donness html permettant l'affichage.
*/
QString C_MedinTuxDataView::prepareDataToView(const QString &data)
{

 resetRef();
 QString imgName, imageFile;
 QPixmap   pixmap;
 int pos_deb = data.find("<HTML_Data>",0);
 if (pos_deb==-1)         return QString::null;
 pos_deb += 11;
 int pos_end = data.find("</HTML_Data>", pos_deb);
 if (pos_deb==-1)         return QString::null;
 int pos_deb_img = data.find("<ImageListe>", pos_deb+12);
 if (pos_deb_img==-1)     return QString::null;
 int pos_end_img;
 while ( (pos_deb_img  = data.find("<name>",  pos_deb_img)) != -1 )
       {  imageFile = "";
          pos_deb_img += 6;
          if ( (pos_end_img  = data.find("</name>", pos_deb_img)) != -1)
             {imgName     = data.mid(pos_deb_img, pos_end_img-pos_deb_img).stripWhiteSpace();
              int pos;
              if ( (pos = data.find("<fileName>", pos_deb_img)) != -1)
                 {pos += 10;
                  if ( (pos_end_img  = data.find("</fileName>", pos)) != -1)
                     {imageFile = data.mid(pos, pos_end_img-pos).stripWhiteSpace();
                     }
                 }
              if ( (pos_deb_img = data.find("<base64>", pos_deb_img)) != -1)
                 {pos_deb_img += 8;
                  if ( (pos_end_img  = data.find("</base64>", pos_deb_img)) != -1)
                     { if (getNbRefForThisName(imgName) ==-1)           // est elle déjà dans la factory ??
                          {pixmap =  decodeStringTo_QPixmap( data.mid(pos_deb_img, pos_end_img-pos_deb_img ));
                           appendToList( pixmap, imgName, imageFile );
                           pos_deb_img = pos_end_img + 9;
                           //QImage   qi = pixmap.convertToImage().scale ( 60, 60 );
                           //new QIconViewItem( m_iconViewDocuments, imgName, QPixmap (qi )  );
                          }
                     }
                  else return QString::null;
                 }
               else return QString::null;
             }
          else return QString::null;
       }
 return data.mid(pos_deb, pos_end-pos_deb);
}

//----------------------------- decodeStringTo_pQPixmap -----------------------------------
/*! \brief Fonction qui décode un texte base64 en image... 
 *  \param in : QCString ou QByteArray ou QString qui contient l'image encodée en base64
 *  \return Renvoie un pointeur vers le QPixmap décodé.
*/
QPixmap *C_MedinTuxDataView::decodeStringTo_pQPixmap( const QString& in )
{ QByteArray ba_dec;
  QCString base64encoded = QCString( in );
  QCodecs::base64Decode( base64encoded , ba_dec);
  return new QPixmap ( ba_dec );
}

//----------------------------- decodeStringTo_QPixmap -----------------------------------
/*! \brief Fonction qui décode un texte base64 en image... 
 *  \param in : QCString ou QByteArray ou QString qui contient l'image encodée en base64
 *  \return Renvoie un pointeur vers le QPixmap décodé.
*/
QPixmap C_MedinTuxDataView::decodeStringTo_QPixmap( const QString& in )
{ QByteArray ba_dec;
  QCString base64encoded = QCString( in );
  QCodecs::base64Decode( base64encoded , ba_dec);
  return QPixmap ( ba_dec );
}