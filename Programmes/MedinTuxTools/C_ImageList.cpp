//
// C++ Interface:
//
/********************************* C_ImageList.cpp *****************************************
                                                                                
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
#include "C_ImageList.h"
#include <qobject.h>
#define TR  QObject::tr

//====================================== C_ImageList =======================================================

/*! \class C_ImageList
 *  Classe C_ImageList
*/

  //--------------------------------------- C_ImageList ---------------------------------------
  /*! \brief Constructeur  : le dictionnaire des références en autodelete
  */
  C_ImageList::C_ImageList()
          {m_C_ImageRefDict.setAutoDelete ( TRUE );
          };
  //--------------------------------------- ~C_ImageList ---------------------------------------
  /*! \brief Destructeur  : retire les images de la QMimeSourceFactory en fin seulement car elles doivent pouvoir être dispo pour le copier coller de référence html
  */
  C_ImageList::~C_ImageList()
          {C_ImgInf *pC_ImgInf;
           QDictIterator<C_ImgInf> it( m_C_ImageRefDict );
           for( ; it.current(); ++it )
              {pC_ImgInf = it.current();
               if (pC_ImgInf) QMimeSourceFactory::setPixmap( pC_ImgInf->m_Name, 0 );        // virer cette image de la factory
              }
          }
  //--------------------------------------- resetList ---------------------------------------
  /*! \brief remetla liste d'images à zero (efface les références et les images de la factory)
  */
  void C_ImageList::resetList()
          {C_ImgInf *pC_ImgInf;
           QDictIterator<C_ImgInf> it( m_C_ImageRefDict );
           for( ; it.current(); ++it )
              {pC_ImgInf = it.current();
               if (pC_ImgInf)  QMimeSourceFactory::setPixmap( pC_ImgInf->m_Name, 0 );        // virer cette image de la factory
              }
           m_C_ImageRefDict.clear();
          }
  //--------------------------------------- resetRef ---------------------------------------
  /*! \brief remet toutes les réferences à zero
  */
  void C_ImageList::resetRef()
          {C_ImgInf *pC_ImgInf;
           QDictIterator<C_ImgInf> it( m_C_ImageRefDict );
           for( ; it.current(); ++it )
              {pC_ImgInf = it.current();
               if (pC_ImgInf)  pC_ImgInf->m_NbRef = 0 ;      // mettre la référence à zéro
              }
          }
  //--------------------------------------- appendToList ---------------------------------------
  /*! \brief ajoute une image au QMimeSourceFactory. Si déjà présente incrémente juste la référence à cette image
  */
  int C_ImageList::appendToList(QPixmap *pQPixmap,  QString &name, const QString &path)
            {return appendToList(*pQPixmap, name, path );
            }
  int C_ImageList::appendToList(QPixmap pixmap,  QString &name, const QString &path)
            {C_ImgInf *pC_ImgInf = 0;
             //................ si une image venant du meme fichier disque existe .....................
             //                 alors incrémenter la reference
             /*  NE PAS ICREMENTER COMPTEUR SI UNE IMAGE A LE MEME FICHIER SOURCE
                 CAR CELA EST POSSIBLE AVEC LE FICHIER EN BASE MODIFIE DIFFERENT
             if (path.length())
                {QDictIterator<C_ImgInf> it( m_C_ImageRefDict );
                 for( ; it.current(); ++it )
                    {pC_ImgInf = it.current();
                     if (pC_ImgInf && path == pC_ImgInf->m_Path) 
                        {pC_ImgInf->m_NbRef += 1;
                         name = pC_ImgInf->m_Name;
                         return pC_ImgInf->m_NbRef;
                        }
                    }
                }
              */
             if (  (pC_ImgInf = m_C_ImageRefDict.find ( name )) )
                { pC_ImgInf->m_NbRef += 1;
                  return pC_ImgInf->m_NbRef;
                }
             else
                { pC_ImgInf = new C_ImgInf (name, path);
                  if (pC_ImgInf)
                     {QMimeSourceFactory::setPixmap( name, pixmap );
                      m_C_ImageRefDict.insert( name, pC_ImgInf );
                     }
                  return pC_ImgInf->m_NbRef;
                }
            }

  //--------------------------------------- incRef ---------------------------------------
  /*! \brief incrémente une reference à une image
  */
  int C_ImageList::incRef(const QString &name)
            {C_ImgInf *pC_ImgInf = 0;
             if (  (pC_ImgInf = m_C_ImageRefDict.find ( name )) )
                { pC_ImgInf->m_NbRef += 1;
                  return pC_ImgInf->m_NbRef;
                }
             else
                {
                }
             return -1;
            }
  //--------------------------------------- setNbRef ---------------------------------------
  /*! \brief intialise l'image avec le nom 'name' avec le nb de reference indiqué en nbRef
  */
   bool C_ImageList::setNbRef( const QString &name , int nbRef)
   {C_ImgInf *pC_ImgInf = 0;
    if (  (pC_ImgInf = m_C_ImageRefDict.find ( name )) )
       {  pC_ImgInf->m_NbRef = nbRef;
          return TRUE;
       }
    return FALSE;
   }

  //--------------------------------------- getPixmap ---------------------------------------
  /*! \brief intialise un pixmap sur l'image avec le nom 'name'
  */
   bool C_ImageList::getPixmap( const QString &name , QPixmap &retPixMap, QString *pFileName )
   {const QMimeSource *pQMimeSource = data( name );
    int pos = name.findRev('.');
    QString mimetype = "png";
    if (pos != -1)
       {mimetype=name.mid(pos+1).lower();
       }
    mimetype.prepend("image/");
    pQMimeSource = QMimeSourceFactory::defaultFactory()->data( name );
    if (pQMimeSource) 
       {retPixMap           = QPixmap(pQMimeSource->encodedData ( mimetype ));
        C_ImgInf *pC_ImgInf = 0;
        if (pFileName && (pC_ImgInf = m_C_ImageRefDict.find ( name )) ) 
           {*pFileName =  pC_ImgInf->m_Path;
           }
        //QImageDrag::decode( pQMimeSource, retPixMap );
        return TRUE;
       }
    return FALSE;
   }

  /*! \brief retourne et créé un  pixmap sur l'image avec le nom 'name' le pointeur sur l'image sera à détruire
  */
  //--------------------------------------- getPixmap ---------------------------------------
  QPixmap *C_ImageList::getPixmap( const QString &name , QString *pFileName)
   {const QMimeSource *pQMimeSource = data( name );
    if (pQMimeSource) 
       {C_ImgInf *pC_ImgInf = 0;
        if (pFileName && (pC_ImgInf = m_C_ImageRefDict.find ( name )) ) 
           {*pFileName =  pC_ImgInf->m_Path;
           }
        return new QPixmap(pQMimeSource->encodedData ( "image/png" ));
       }
    return 0;
   }

 //--------------------------------------- getSrcImageName ---------------------------------------
  QString C_ImageList::getSrcImageName( const QString &name)
   {const QMimeSource *pQMimeSource = data( name );
    if (pQMimeSource) 
       {C_ImgInf *pC_ImgInf = m_C_ImageRefDict.find ( name );
        if (pC_ImgInf )  return pC_ImgInf->m_Path;
       }
    return QString::null;
   }

 //--------------------------------------- setSrcImageName ---------------------------------------
  int C_ImageList::setSrcImageName( const QString &name, const QString &fileName)
   {const QMimeSource *pQMimeSource = data( name );
    if (pQMimeSource) 
       {C_ImgInf *pC_ImgInf = m_C_ImageRefDict.find ( name );
        if (pC_ImgInf )  {pC_ImgInf->m_Path = fileName; return 1;}
       }
    return 0;
   }
  //--------------------------------------- removeFromList ---------------------------------------
  /*! \brief retire une image décrémentant juste la référence à cette image car elles doivent pouvoir être dispo pour le copier coller de référence html
  */
  int C_ImageList::removeFromList(const QString &name)
            {C_ImgInf *pC_ImgInf;
             if (  (pC_ImgInf = m_C_ImageRefDict.find ( name )) )
                { if (pC_ImgInf->m_NbRef > 0 ) pC_ImgInf->m_NbRef -= 1;
                  return pC_ImgInf->m_NbRef;
                }
             return -1;
            }
  //--------------------------------------- destroyFromList ---------------------------------------
  /*! \brief détruit tout ce qui concerne cette image en effaçant tout, cette image ne sera plus dispo du tout même plus dans la QMimeSourceFactory 
  */
  void C_ImageList::destroyFromList(const QString &name)
            {C_ImgInf *pC_ImgInf;
             while (  (pC_ImgInf = m_C_ImageRefDict.find ( name )) )
                { m_C_ImageRefDict.remove ( name );
                  QMimeSourceFactory::setPixmap( name, 0 );        // virer cette image de la factory globale
                  setPixmap( name, 0 );                            // virer cette image de la factory locale (devrait être pareil qu'au dessus mais on sait jamais)
                }
            }

  //--------------------------------------- getNbRefForThisName ---------------------------------------
  int C_ImageList::getNbRefForThisName(const QString &name)
            {C_ImgInf *pC_ImgInf;
             if (  (pC_ImgInf = m_C_ImageRefDict.find ( name )) )
                { return pC_ImgInf->m_NbRef;
                }
             return -1;
            }




