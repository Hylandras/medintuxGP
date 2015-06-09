/********************************* $FILE$ *****************************************
                                                                                
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
                                                                                 
 **********************************************************************************/

/********************************* $FILE$ *****************************************
                                                                                
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
                                                                                 
 **********************************************************************************/


#ifndef  CDEVILGRAPH_H
#define  CDEVILGRAPH_H

#include "CDevilCrucible.h"
#include "CMoteur_Base.h"

#include <math.h>
#include <qdatetime.h>
#include <qmemarray.h> 
#include <qpixmap.h>
#include <qpainter.h>

class CDevilCrucible;

class CDevilGraph
{

public: 

/*! \brief Constructeur de la classe.

 * Exemple pour le code (dans CDevilCrucible.cpp):
 *\code CDevilGraph *pPDF = new CDevilGraph (this);
 * QString resultat_du_calcul = pPDF->Graph (arg1);
  \endcode
 * \param *pCDevil : pointer vers la classe CDevilCrucible appelante.
*/
     CDevilGraph		(CDevilCrucible *pCDevil);


/*! \brief Destructeur de la classe.
*/
     ~CDevilGraph(){};

     QString 		Graph			(QString &arg1);  // argument non encore utilisé
     void 		Graph_22_Years		();
     void 		Graph_3_Years		();
     void		PutLabel		(int X, int Y, QString lbl, QPainter &p);
     void 		Graph_Quadrillage	(double xMax, double xMin, double yMax, double yMin, double xPas, double yPas,
						int xZero, int yZero, int distX, int distY, 
						double legendXPas, double legendYPas, QString lbl, QPainter &paint, 
						QColor colorQuad);

private:
     CDevilCrucible	*m_pCDevil;
     CMoteurBase	*m_pMB;			// Récupère le Moteur de le Base de données
     QString		m_IdentPrimKey;		// patient identity primary key on m_DOSS_IDENT_TBL_NAME 
     QString		m_UserDocPk;		// current  user primary key on m_USER_IDENT_TBL_NAME from document user 
};

#endif
