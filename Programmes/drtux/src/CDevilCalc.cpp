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

/*! \file

 * Définition de la classe CDevilCalc.
*/

/*! \class CDevilCalc
 * \brief Classe dépendante de la classe CDevilCrucible qui permet d'effectuer les fonctions de calcul.

 * Exemple pour les masques :
 \code {{:: Calc ( 1, * , 1) }}.  \endcode

 * Exemple pour le code (dans CDevilCrucible.cpp):
 *\code
 * CDevilCalc *pCalc = new CDevilCalc (this);
 * QString resultat_du_calcul = pCalc->Calc(arg1,arg2,arg3);
 \endcode

 * \param *pCDevil : pointer vers la classe CDevilCrucible appelante.
 * \todo Créer un modèle pour appeler CDevilCalc à l'extérieur de CDevilCrucible ??? Intérêt ???
*/
#include "CDevilCalc.h"
#include "C_TokenInterpret.h"


//------------------------------------------ CDevilCalc ----------------------------------------------------------
//	CLASSE : Donne accès depuis CDevilCrucible aux fonctions de calcul et de calendrier (obstétrical)
CDevilCalc::CDevilCalc(CDevilCrucible *pCDevil)
{       C_TokenInterpret::init(pCDevil);
	m_pCDevil = pCDevil;  // sauvegarde le pointer de CDevilCrucible pour accéder à ses données et fonctions
}

//------------------------------------------ Calc ---------------------------------------------------------------
/*! \fn QString CDevilCalc::Calc(QString &v1, QString &op, QString &v2)
 * \brief Fonction de calcul accessible depuis les masques de MedinTux par : {{::Calc (TAILLE,*,POIDS) }}
 * \param v1 : première valeur de l'opération (dans notre exemple : TAILLE).
 * \param op : l'opérateur.
 * \param v1 : deuxième valeur de l'opération (dans notre exemple : POIDS).
 * \return Retourne le résultat de l'opération sous le format QString.

 * Les variables du calcul peuvent être indifféremment des chiffres ou des variables de la base de données tels que : \n
  *- POIDS, TAILLE; ou même des variables préalablement calculées : 
  *- BMI, SURFACE.\n
 * L'opérateur peut être '*', '/', '+', '-'.

 * \todo Ajouter les opérateurs 'DIV', et 'MOD'.
*/
//--------------------------------------------------------------------------------------------------------------------
QString CDevilCalc::Calc(QString &v1, QString &op, QString &v2)
{
  QString        mostRecent;
  QStringList    varDateVal;
  QString        findVal;
  double         valeur1  = 0;
  double         valeur2  = 0;
  double         resultat = 0;

  //........ assainit les paramètres passés à la fonction
  v1 = v1.stripWhiteSpace();
  v2 = v2.stripWhiteSpace();
  op = op.stripWhiteSpace();

  //........ récupère la date de l'observation
  QString sDt = C_TokenInterpret::GetDateOfActiveRubrique();
  if (sDt == " ") sDt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");

  //........ récupère les valeurs de poids, taille et surface enregistrées et sélectionne la plus proche de la date d'observation

  //........ rédigé comme suit : ( Poids, * , 25)  ou   (25, * , Poids)  ou  ( 1, *, 1 )
  if (!v1[0].isNumber())
   	{ findVal = v1.lower();     valeur2 = v2.toDouble(); }
  else if (!v2[0].isNumber())
	{ findVal = v2.lower();     valeur1 = v1.toDouble(); }
  else 
	{ 	findVal = "";  	valeur1 = v1.toDouble();		valeur2 = v2.toDouble();  }

  if (findVal == "poids")               // testé OK
   {    QString      section = "VAR Poids";
        QString      variable = "Poids";
        if (valeur2 == 0)    valeur2 = C_TokenInterpret::GetVarNeerer ( section, variable , sDt).toDouble();
        else                 valeur1 = C_TokenInterpret::GetVarNeerer ( section, variable , sDt).toDouble();
   }
  else if (findVal == "taille")         // testé OK
   {    QString  section = "VAR Poids";
        QString  variable = "Taille";
        if (valeur2 == 0)    valeur2 = C_TokenInterpret::GetVarNeerer ( section, variable , sDt).toDouble();
        else                 valeur1 = C_TokenInterpret::GetVarNeerer ( section, variable , sDt).toDouble();
   }
  else if (findVal == "surface")        // testé OK
   {    QString      section = "VAR Poids";
        QString      variable = "Poids";
        double poids = C_TokenInterpret::GetVarNeerer ( section, variable , sDt).toDouble();

        section = "VAR Poids";
        variable = "Taille";
        double taille_cm = C_TokenInterpret::GetVarNeerer ( section, variable, sDt).toDouble();

        double tailleMetre = taille_cm / ((double)100);

        if (taille_cm < 40) tailleMetre = 1.50;
        // reste à choisir la formule de calcul de surface corporelle
        // Formule de Dubois et Dubois  = 0.20247 x Taille(m)^0.725 x Poids(kg)^0.425
        // pour enfants                 = [4 x Poids(kg) +7] / [Poids(kg) + 90]
        // Formule de Boyd
                // # Surface corporelle (m²) =  0,0003207 x (Poids)0,7285-0,0188 x log(Poids) x (Taille)0,3
                // # Le poids est en gramme ; la taille est en cm ; le Log est décimal.
                // # Limite : Poids de 15 à 200 Kg ; taille de 99 à 250 cm.
        tailleMetre = pow(tailleMetre,0.725);
        poids       = pow(poids,0.425);
        if (valeur2 == 0)    valeur2 = 0.20247 * tailleMetre * poids;
        else                 valeur1 = 0.20247 * tailleMetre * poids;
   }
  else if (findVal == "bmi")    // testé OK
   {    QString      section = "VAR Poids";
        QString      variable = "Poids";
        double poids =C_TokenInterpret::GetVarNeerer ( section, variable , sDt).toDouble();

        section = "VAR Poids";
        variable = "Taille";
        double taille_cm = C_TokenInterpret::GetVarNeerer ( section, variable, sDt).toDouble();

        double tailleMetre = taille_cm / ((double)100);

        if (taille_cm < 40) tailleMetre = 1.50;
        if (valeur2 == 0)    valeur2 = poids / pow(tailleMetre, 2);
        else                 valeur1 = poids / pow(tailleMetre, 2);
   }

  if (op == "*" || op.lower() == "x")   // testé OK
    {   resultat = valeur1 * valeur2;
        resultToReturn = QString::number(resultat);
    }
  else if (op == "/")                   // testé OK
    {   resultat = valeur1 / valeur2;
        resultToReturn = QString::number(resultat);
    }
  else if (op == "+")                   // testé OK
    {   resultat = valeur1 + valeur2;
        resultToReturn =  QString::number(resultat);
    }
  else if (op == "-")                   // testé OK
    {   resultat = valeur1 - valeur2;
        resultToReturn = QString::number(resultat);
    }
  else if (op.lower() == "div")
    {	int div_entiere = 0;
        div_entiere = (int)(valeur1 / valeur2);
	//resultat = (valeur1 - (valeur1 % valeur2)) / valeur2;
	return QString::number(div_entiere);
    }
  else if (op.lower() == "mod")
    {	//int div_entiere = 0;
        //div_entiere = (int)(valeur1 / valeur2);
	resultat = ((int)valeur1 % (int)valeur2);
	return QString::number(resultat);
    }

  return resultToReturn;
}


QString CDevilCalc::GetResult()
{
   return resultToReturn;
}

