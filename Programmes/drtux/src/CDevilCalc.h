/********************************* CDevilCrucible.cpp *****************************************
                                                                                
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


#ifndef  CDEVILCALC_H
#define  CDEVILCALC_H

#include <math.h>
#include "CDevilCrucible.h"

class CDevilCrucible;


class CDevilCalc
{

public:

/*! \brief Constructeur de la classe.

 * Exemple pour le code (dans CDevilCrucible.cpp):
 *\code CDevilCalc *pCalc = new CDevilCalc (this);
 * QString resultat_du_calcul = pCalc->Calc(arg1,arg2,arg3);
  \endcode
 * \param *pCDevil : pointer vers la classe CDevilCrucible appelante.
*/
     CDevilCalc		(CDevilCrucible *pCDevil);

/*! \brief Destructeur de la classe.
 */
     ~CDevilCalc(){};

     QString		Calc ( QString &v1, QString &op, QString &v2);

     QString		GetResult		();   // retourne le résultat du calcul ou du calendrier (au cas où...)

private:
     CDevilCrucible	*m_pCDevil;
     QString 		resultToReturn;

};

#endif



