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

 * Définition de la classe CDevilGraph.
*/

/*! \class CDevilGraph
 * \brief Classe dépendante de la classe CDevilCrucible qui permet d'effectuer les fonctions de graphiques (taille, poids, bmi, courbe de croissance) de drtux.

 * Exemple pour les masques :
 \code {{:: Graph ( ) }}
 \endcode

 * Exemple pour le code (dans CDevilCrucible.cpp):
 *\code CDevilGraph *pPDF = new CDevilGraph (this);
 * QString resultat_du_calcul = pPDF->Graph (arg1);
  \endcode

 * Le code est commenté pour une meilleure lisibilité.

 * \param *pCDevil : pointer vers la classe CDevilCrucible appelante.
 * \todo Créer un modèle pour appeler CDevilGraph à l'extérieur de CDevilCrucible ??? Intérêt ???
*/
#include "CDevilGraph.h"
#include "C_TokenInterpret.h"

#ifdef Q_WS_WIN

/*
float round ( double in , double prec )
   {int in2=in*pow(10, prec);
    in=in2/pow(10,prec);
    return in;
   }

*/
 #define round
#endif
CDevilGraph::CDevilGraph (CDevilCrucible *pCDevil)
{  m_pCDevil 	= pCDevil;
   C_TokenInterpret::init(pCDevil);
   m_pMB      = pCDevil->GetMB();  // récupère les fonctions de la base de données de CDevilCrucible
   m_IdentPrimKey = pCDevil->GetMIdentPrimKey();
   m_UserDocPk = pCDevil->GetUserDocPk();
}

//------------------------------------------ Graph ---------------------------------------------------------------
/*! \brief Fonction à appeler pour construire le graphique. Le code est commenté pour une meilleure lisibilité.

 * Cette fonction trace automatiquement selon l'âge du patient deux types de graphiques :
 * - Taille, Poids et BMI de 0 à 3 ans
 * - Taille, Poids, BMI, Croissance de 3 à 22 ans\n
 * \n
 * C'est la fonction principale de cette classe. Son seul appel suffit à créer et afficher le graphique dans son intégralité.

 * Exemple pour les masques :
 \code {{:: Graph ( ) }}
 \endcode

 * Exemple pour le code (dans CDevilCrucible.cpp):
 *\code CDevilGraph *pPDF = new CDevilGraph (this);
 * QString resultat_du_calcul = pPDF->Graph (arg1);
  \endcode

 * \param arg1 : encore inutilisé

 * \version 1.00
 * \todo Créer un modèle pour appeler CDevilGraph à l'extérieur de CDevilCrucible ??? Intérêt ???
*/
//--------------------------------------------------------------------------------------------------------------------
QString CDevilGraph::Graph( QString &/*arg1*/)  				// testé OK
{ //...... récupère date de NAISSANCE DU PATIENT !!!!
  QVariant qvar;
  m_pMB->GetFieldValue(m_pMB->m_DOSS_IDENT_TBL_NAME, m_pMB->m_DOSS_IDENT_NSS, m_pMB->m_DOSS_IDENT_REF_PK , m_IdentPrimKey, &qvar);
  QDateTime qDteNss = qvar.toDate();
  //...... calcul l'âge en mois
  double age = qDteNss.daysTo(QDateTime::currentDateTime()) / 365.25;

  if (age > 22) return QString::null;	// age > 22 ans    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  A  FAIRE
  else if (age > 3) Graph_22_Years();	// age entre 36 mois et 22 Ans  --> courbe 22 Ans
  else Graph_3_Years();			// age < 36 mois --> triple courbe des 36 premiers mois

  return QString::null;
}


//------------------------------------------ PutLabel ----------------------------------------------------------------
/*! \brief Ajoute à l'image un label à la position données. Le code est commenté pour une meilleure lisibilité.

 * \param X :	Position X
 * \param Y :	Position Y
 * \param lbl :	contenu de la string à ajouter
 * \param &p :	Painter dans lequel il faut ajouter le label

 * Le label est ajouté avec un style simple et un encadrement.
*/
//--------------------------------------------------------------------------------------------------------------------
void CDevilGraph::PutLabel(int X, int Y, QString lbl, QPainter &p)
{//...... Quelques variables
 QColor c;
 QFont  fnt;
 QPen   pen;
 QRect r;
 int w;
 //int h;
 w = lbl.length() * 10;

 // revient en noir
 c.setRgb (0,0,0);
 pen.setColor(c);
 p.setPen(pen);
 // change de police
 fnt.setFamily("Arial");
 fnt.setPointSize(10);
 fnt.setBold(FALSE);
 p.setFont(fnt);
 c.setRgb(255,255,255);
 p.setBrush(c);

 // positionne l'ombre
 c.setRgb(125,125,125);
 pen.setColor(c);
 pen.setWidth(2);
 p.setPen(pen);
 p.drawRect(X-w+2,Y+2,w,20);

 // positionne le label
 c.setRgb(0,0,0);
 pen.setColor(c);
 pen.setWidth(1);
 p.setPen(pen);
 r.setRect (X-w,Y,w,20);   //setRect ( int x, int y, int w, int h )
 p.drawRect( r ) ;
 p.drawText( r , Qt::AlignHCenter | Qt::AlignVCenter, lbl);
}


//------------------------------------------ Graph_Quadrillage -------------------------------------------------------
/*! \brief Ajoute à l'image un label à la position données. Le code est commenté pour une meilleure lisibilité.

\param	xMax	: valeur maximale pour X
\param	xMin	: valeur maximale pour Y
\param	yMax	: valeur minimale pour X
\param	yMin	: valeur minimale pour Y
\param	xPas	: pas pour x
\param	yPas	: pas pour y
\param	xZero	: coordonnées du 0 en X
\param	yZero	: coordonnées du 0 en Y
\param	distX	: nb de pixels entre chaque colonnes
\param	distY	: nb de pixels entre chaque lignes
\param	legendXPas	: pas pour les légendes en X
\param	legendYPas	: pas pour les légendes en Y
\param	lbl		: l'intitulé du tableau
\param	&paint		: là où nous dessinons
\param	colorQuad	: couleur du quadrillage

 * Le quadrillage est tracé en partant de xZero yZero du painter. Le reste est simple à comprendre.
*/
//--------------------------------------------------------------------------------------------------------------------
void CDevilGraph::Graph_Quadrillage(	double xMax, double xMin, double yMax, double yMin, double xPas, double yPas,
					int xZero, int yZero, int distX, int distY,
					double legendXPas, double legendYPas, QString lbl, QPainter &paint, QColor colorQuad)
{ //..... Quelques variables bien utiles
  QPen	pen;
  QColor color;
  QFont  fnt;
  int X , Y, i, nbLines, nbCol, nbColLeg, nbLinesLeg, widthQuad, heightQuad, nb;


  //..... Quelques calculs bien utiles
  nbCol		= round ( (xMax - xMin) / xPas );
  nbLines	= round ( (yMax - yMin) / yPas );
  widthQuad 	= round ( nbCol * distX);
  heightQuad	= round ( nbLines * distY);

  //..... change l'aspect du crayon
  pen.setWidth(1);
  pen.setColor(colorQuad);
  paint.setPen(pen);

  //..... dessine les lignes verticales
  int lX , lY;
  Y = yZero;
  lY  = yZero-heightQuad;
  for (i = 0; i < nbCol+1; ++i)
      { X = round ( xZero + (i * distX) );
	paint.drawLine( X , Y , X , lY );
      }
  //..... dessine les lignes horizontales
  X  = xZero;
  lX = xZero+widthQuad;
  for (i = 0; i < nbLines+1; ++i)
      { Y = round ( yZero - (i * distY) );
	paint.drawLine( X , Y , lX , Y );
      }

  //...... prépare le crayon, la police pour les légendes du graphique
  // revient en noir
  color.setRgb (0,0,0);
  pen.setColor(color);
  paint.setPen(pen);
  // change de police
  fnt.setFamily("Arial");
  fnt.setPointSize(10);
  fnt.setBold(FALSE);
  paint.setFont(fnt);
  color.setRgb(200,200,200);
  paint.setBrush(color);

  //..... place les légendes
  nbColLeg	= round ( (xMax - xMin) / legendXPas);
  nbLinesLeg	= round ( (yMax - yMin) / legendYPas);

  //..... légendes verticales
  int w,h;
  X = round(xZero - distX*2);
  Y = round(yZero - (distY/2) );
  w = round(distX*2);
  for (i = 0; i < nbLinesLeg+1; ++i)
   { 	lY  = Y - round ( i * distY / yPas * legendYPas);
	h = round ( (distY) );
	nb = (yMin + (i * legendYPas));
	paint.drawText( X,lY,w,h,Qt::AlignHCenter|Qt::AlignVCenter, QString::number(nb));
   }
  //..... légendes horizontales
  X = xZero;
  Y = yZero;
  w = round(distX*2);
  for (i = 0; i < nbColLeg+1; ++i)
   { 	h = round ( (distY) );
	lX = X + round (  i * (distX / xPas * legendXPas) - distX);
	nb = (xMin + (i * legendXPas));
	paint.drawText( lX,Y,w,h,Qt::AlignHCenter|Qt::AlignVCenter, QString::number(nb));
   }

  // place l'intitulé du graphique en bas à droite
  //  X = xZero - ((xMax - xMin - 7)*distX) - (distX/2);
  //  Y = yZero - (1 *distY) - (distY/2);
  X = xZero + widthQuad;
  Y = yZero - heightQuad - distY;
  PutLabel(X,Y,lbl,paint);
}

//------------------------------------------ Graph_3_Years ----------------------------------------------------------
/*! \brief Trace le graphique prévu pour les patients de 0 à 3 ans. Le code est commenté pour une meilleure lisibilité.
*/
//--------------------------------------------------------------------------------------------------------------------
void CDevilGraph::Graph_3_Years()
{ //...... défini les variables indispensables aux tracés
  // Intervalles Max et Min
  int intervallePC[2]	= { 60,30 };   	// Max et Min pour la courbe périmètre crânien
  int intervalleT [2]	= { 120, 30 };  // idem pour la taille
  int intervalleP [2]	= { 22,0 };	// idem pour le poids
  // Pas des courbes pour le quadrillage
  int pas_PC		= 5;		// pas du quadrillage pour la mesure des PC  (une ligne tous les x cm) : 5 cm
  int pas_T		= 5;		// idem pour taille : 5 cm
  int pas_P		= 1;		// idem pour poids : 1 kg
  int pas_Mois		= 1;		// pas du quadrillage pour les mois : 3 mois
  int distX		= 10;		// distance en pixels entre chaque lignes verticales
  int distY		= 15;		// distance en pixels entre chaque lignes horizontales
  // Marges du dessin
  int leftMarg   	= 30; 		// Marge de gauche en pixels;
  int rightMarg  	= 30; 		// Marge de droite en pixels;
  int topMarg		= 50; 		// Marge du haut en pixels;
  int bottomMarg 	= 50; 		// Marge du bas en pixels;
  // Taille des points à dessiner
  int pointWidth	= 4;		// Longueur du rectangle repère
  int pointHeight	= 4;		// Hauteur du rectangle repère
  // espacement
  double espGrille	= 3.0;  	// entre chaques grilles -> espacement de  x * distX
  // pour les calculs
  double nbDaysPerMonth = 365.25 / 12;	// nombre de jours par mois en moyenne, tiens compte des années bisextiles

  //...... variables de calcul
  int X, Y , nb, i, idx;			// coordonnées à tracer
  double valVar;
  //...... variables pour la récupération des valeurs du terrain
  QString     section;  //  = "VAR Poids";
  QString     variable; // = "Poids";

  // variables qui contiendront les coordonnées du {zéro,zéro} de chaque courbes
  int xPC	= leftMarg;
  int yPC	= round ( topMarg + ((intervallePC[0] - intervallePC[1]) / pas_PC * distY) );
  int xT	= leftMarg;
  int yT	= round ( yPC + (2 * distY) + ((intervalleT[0] - intervalleT[1]) / pas_T * distY) );
  int xP	= round ( leftMarg + (36 / pas_Mois * distX) + (espGrille * distX) );
  int yP	= round ( topMarg + (2 * distY) + ((intervalleP[0] - intervalleP[1]) / pas_P * distY) );// amélioration prévisible remplacer le 2

  // Taille de l'image
  int heightPict= round ( yT + bottomMarg );
  int widthPict = xP + rightMarg + (36 / pas_Mois * distX);

  //  Calcul des coordonnées pour le périmètre crânien
  //  X = round ( xPC + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yPC - (( valeur - intervallePC[1] ) / pas_PC * distY ) ) ;	// valeur doit être en CM

  //  Calcul des coordonnées pour la taille
  //  X = round ( xT + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yT - (( valeur - intervalleT[1] ) / pas_T * distY ) ) ;	// valeur doit être en CM

  //  Calcul des coordonnées pour le poids
  //  X = round ( xP + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yP - (( valeur - intervalleP[1] ) / pas_P * distY ) ) ;	// valeur doit être en KG

  //...... Painter , Pixmap , Widget...
  QWidget 	*myWgt = new QWidget;
  QPixmap 	drawnPixmap;
  QPainter	paint;
  QPen		pen;
  QColor	color, sexColorGrid , sexColorBkgd;
  QStringList	tmp;			// contiendra les valeurs de chaques variables (poids, taille, pc)
  QDateTime	qDteNss;		// date de naissance du patient en QDateTime
  QDateTime	dateVal;		// date des valeurs en QDateTime
  long		age;			// age en jours du patient
  QPointArray   ptAr;			// pour tracer les polygones de normalité et les lignes de poids, taille et pc

  // Courbes de normalité pour les 3 premières années
  double poidsDelta2Max [18] = { 4.1,5.6,6.9,7.7,8.4,9.1,9.7,10.2,10.7,11.2,11.8,12.8,13.5,14.1,14.7,16,17,17 };
  double poidsDelta2Min [18] = { 2.4,3,3.5,4.1,4.6,5.1,5.6,5.9,6.2,6.6,6.9,7.4,8,8.5,8.9,9.4,10.3,11 };
  int    poidsAge       [18] = { 0,1,2,3,4,5,6,7,8,9,10,12,15,18,21,24,31,36 }; // en MOIS

  double tailleDelta2Max [10] = { 54, 64,   67, 69,   71,   75.5, 84,   91,   96, 101 };
  double tailleDelta2Min [10] = { 46, 54.5, 57, 58.5, 60.5, 64.5, 71.5, 77.4, 82, 86 };
  int    tailleAge       [10] = { 0,  3,    4,  5,    6,    9,    16,   23,   29, 36 };  // en MOIS

  double pcDelta2Max [12] = { 37.5,41.5,43,44,45,46.5,47.8,49.2,50,51,52,52.5 };
  double pcDelta2Min [12] = { 32.5,35.8,37,38,39,40.5,42,43,44,44.7,45.8,45.5 };
  int    pcAge       [12] = { 0,2,3,4,5,7,9,12,15,20,28,36 }; // en MOIS

  //...... redéfini hauteur et largeur du pixmap
  drawnPixmap.resize( widthPict, heightPict);

  //...... débute le dessin
  paint.begin(&drawnPixmap);

  //...... sélectionne les couleurs selon le sexe
  QString sex = m_pMB->GetFieldValue(m_pMB->m_DOSS_IDENT_TBL_NAME, m_pMB->m_DOSS_IDENT_SEX, m_pMB->m_DOSS_IDENT_REF_PK , m_IdentPrimKey);
  if (sex == "F")
      {	sexColorGrid.setRgb(179,1,164);   // rose : (255,167,248)   pour les filles
	sexColorBkgd.setRgb(255,225,254);
      }
  else
      {	sexColorGrid.setRgb (1, 122, 179);   // bleu : (50, 147, 192) / 227,241,247   pour les garçons :)
	sexColorBkgd.setRgb(225,245,255);
      }
  pen.setColor(sexColorBkgd);

  //...... prépare le fond d'image
  color.setRgb(255,255,255);
  paint.setBrush(color);
  pen.setColor(color);
  paint.setPen(pen);
  paint.drawRect(0,0,widthPict, heightPict);
  paint.setBackgroundColor(color);

  //...... trace les courbes de normalité
  // Poids
  paint.setBrush(sexColorBkgd);
  nb = 18;
  ptAr.resize(nb*2,QGArray::MemOptim);
  for (i = 0; i < nb; ++i)
    {	X = round ( xP + (( poidsAge[i] ) / pas_Mois * distX ) ) ;
	Y = round ( yP - (( poidsDelta2Max[i] - intervalleP[1] ) / pas_P * distY ) ) ;
	ptAr.setPoint(i, X, Y);
	Y = round ( yP - (( poidsDelta2Min[i] - intervalleP[1] ) / pas_P * distY ) ) ;
	idx = nb + (nb-1-i);
	ptAr.setPoint(idx, X, Y);
    }
  paint.drawPolygon(ptAr);
  // Taille
  paint.setBrush(sexColorBkgd);
  nb = 10;
  ptAr.resize(nb*2,QGArray::MemOptim);
  for (i = 0; i < nb; ++i)
    {	X = round ( xT + (( tailleAge[i] ) / pas_Mois * distX ) ) ;
	Y = round ( yT - (( tailleDelta2Max[i] - intervalleT[1] ) / pas_T * distY ) ) ;
	ptAr.setPoint(i, X, Y);
	Y = round ( yT - (( tailleDelta2Min[i] - intervalleT[1] ) / pas_T * distY ) ) ;
	idx = nb + (nb-1-i);
	ptAr.setPoint(idx, X, Y);
    }
  paint.drawPolygon(ptAr);
  // PC
  paint.setBrush(sexColorBkgd);
  nb = 12;
  ptAr.resize(nb*2,QGArray::MemOptim);
  for (i = 0; i < nb; ++i)
    {	X = round ( xPC + (( pcAge[i] ) / pas_Mois * distX ) ) ;			// valeur doit être en MOIS
	Y = round ( yPC - (( pcDelta2Max[i] - intervallePC[1] ) / pas_PC * distY ) ) ;	// valeur doit être en KG
	ptAr.setPoint(i, X, Y);
	Y = round ( yPC - (( pcDelta2Min[i] - intervallePC[1] ) / pas_PC * distY ) ) ;	// valeur doit être en KG
	idx = nb + (nb-1-i);
	ptAr.setPoint(idx, X, Y);
    }
  paint.drawPolygon(ptAr);


  //...... trace les quadrillages
  // Poids
  Graph_Quadrillage	(36,0,intervalleP[0], intervalleP[1], pas_Mois, pas_P,
			 xP, yP, distX, distY,
			 3,2, TR("Poids"), paint, sexColorGrid);
  Graph_Quadrillage	(36,0,intervalleT[0], intervalleT[1], pas_Mois, pas_T,
			 xT, yT, distX, distY,
			 3,10, TR("Taille"),  paint, sexColorGrid);
  Graph_Quadrillage	(36,0,intervallePC[0], intervallePC[1], pas_Mois, pas_PC,
			 xPC, yPC, distX, distY,
			 3,10, TR("P.C."), paint, sexColorGrid);

  //...... insère bas de page (graphique réalisé par Medin'Tux le )
  QString nom = TR("Graphique réalisé par DrTux le : ") + QDateTime::currentDateTime().toString(Qt::LocalDate);
  X = (widthPict / 2) - 200 + (nom.length()*10);
  Y = (heightPict - 25);
  PutLabel(X,Y,nom,paint);

  //...... insère nom, prénom, âge actuel du patient
  nom  = m_pMB->GetFieldValue(m_pMB->m_DOSS_INDEX_TBL_NAME, m_pMB->m_DOSS_INDEX_NOM,
					 m_pMB->m_DOSS_INDEX_PRIM_KEY , m_IdentPrimKey)+" ";
  nom += m_pMB->GetFieldValue(m_pMB->m_DOSS_INDEX_TBL_NAME, m_pMB->m_DOSS_INDEX_PRENOM,
					 m_pMB->m_DOSS_INDEX_PRIM_KEY , m_IdentPrimKey);
  X = leftMarg + (nom.length()*10);
  Y = round(distX*1.5);
  PutLabel(X,Y,nom,paint);
  myWgt->setCaption(nom);

  //...... récupère date de NAISSANCE DU PATIENT !!!!
  QVariant qvar;
  m_pMB->GetFieldValue(m_pMB->m_DOSS_IDENT_TBL_NAME, m_pMB->m_DOSS_IDENT_NSS, m_pMB->m_DOSS_IDENT_REF_PK , m_IdentPrimKey, &qvar);
  qDteNss = qvar.toDate();

  //...... récupère les valeurs de poids, taille et PC
  //...... vérifie que les valeurs ne sont pas plus âgées que 3 ans     //////////////// A FAIRE /////////////////
  //...... dessine les valeurs
  section  = "VAR Poids";
  variable = "Poids";
  QStringList valeurs = C_TokenInterpret::GetVar(section,variable);
  //...... calcul pour chaque valeurs de poids l'âge (en mois) à la mesure
  pen.setWidth(2);
  color.setRgb(0,0,0);
  pen.setColor(color);
  paint.setPen(pen);
  paint.setBrush(color);
  ptAr.resize(valeurs.count(),QGArray::MemOptim);
  valeurs.sort();
  // dessine les points
  for ( i = 0; i != (int)valeurs.count(); ++i )
    {	tmp = QStringList::split("/;/", valeurs[i]);
        dateVal = dateVal.fromString(tmp[0] , Qt::ISODate);
        age     = qDteNss.daysTo(dateVal);
	valVar = tmp[1].toDouble();
        X = round( xP + (( age / nbDaysPerMonth ) / pas_Mois * distX ) ) ;
        Y = round( yP - (( valVar - intervalleP[1] ) / pas_P * distY ) );
	paint.drawRect(X-1,Y-1,pointWidth,pointHeight);
	ptAr.setPoint(i, X, Y);
    }
  // trace la ligne entre les points
  paint.drawPolyline(ptAr);


  //...... récupère les valeurs de taille
  section  = "VAR Poids";
  variable = "Taille";
  valeurs  = C_TokenInterpret::GetVar(section,variable);
  //...... calcul pour chaque valeurs de poids l'âge (en mois) à la mesure
  pen.setWidth(2);
  paint.setPen(pen);
  color.setRgb(0,0,0);
  paint.setBrush(color);
  ptAr.resize(valeurs.count(),QGArray::MemOptim);
  valeurs.sort();
  // dessine les points
  for ( i = 0; i != (int)valeurs.count(); ++i )
    {	tmp = QStringList::split("/;/", valeurs[i]);
        dateVal = dateVal.fromString(tmp[0] , Qt::ISODate);
        age     = qDteNss.daysTo(dateVal);
	valVar = tmp[1].toDouble();
	Y = round ( yT - (( valVar - intervalleT[1] ) / pas_T * distY ) );
	X = round ( xT + (( age / nbDaysPerMonth ) / pas_Mois * distX ) ) ;
	paint.drawRect(X-1,Y-1,pointWidth,pointHeight);
	ptAr.setPoint(i, X, Y);
    }
  // trace la ligne entre les points
  paint.drawPolyline(ptAr);

  //...... récupère les valeurs de Périmètre Crânien
  section  = "VAR Poids";
  variable = "PC";
  valeurs = C_TokenInterpret::GetVar(section,variable);
  //...... calcul pour chaque valeurs de poids l'âge (en mois) à la mesure
  pen.setWidth(2);
  paint.setPen(pen);
  color.setRgb(0,0,0);
  paint.setBrush(color);
  ptAr.resize(valeurs.count(),QGArray::MemOptim);
  valeurs.sort();
  // dessine les points
  for ( i = 0; i != (int)valeurs.count(); ++i )
    {	tmp = QStringList::split("/;/", valeurs[i]);
        dateVal = dateVal.fromString(tmp[0] , Qt::ISODate);
        age     = qDteNss.daysTo(dateVal);
	valVar  = tmp[1].toDouble();
	Y = yPC - round ( (( valVar - intervallePC[1] ) / pas_PC ) * distY );
	X = xPC + round ( (( age / nbDaysPerMonth ) / pas_Mois * distX ) ) ;
	paint.drawRect(X-1,Y-1,pointWidth,pointHeight);
	ptAr.setPoint(i, X, Y);
    }
  // trace la ligne entre les points
  paint.drawPolyline(ptAr);

  //...... Fin du dessin
  paint.end();
  QLabel *lblPix = new QLabel (myWgt);

  lblPix->setPixmap(drawnPixmap);
  lblPix->adjustSize ();
  myWgt->adjustSize ();
  myWgt->show();
  // DANGER MEMORY where delete myWgt ?
}


//------------------------------------------ Graph_22_Years ----------------------------------------------------------
/*! \brief Trace l'intégraplité du graphique prévu pour les patients de 3 à 22 ans. Le code est commenté pour une meilleure lisibilité.

 * Les zones de normalité sont tracés en fond pour aider le clinicien à se repérer rapidement dans ses courbes.\n
 * Le graphique comprend les courbes de :
 * - Poids
 * - Taille
 * - BMI
 * - Croissance annuelle
*/
//--------------------------------------------------------------------------------------------------------------------
void CDevilGraph::Graph_22_Years()
{ //...... défini les variables indispensables aux tracés
  // Intervalles Max et Min
  int intervalleT [2]		= { 200, 0 };	// Max et Min pour la taille
  int intervalleP [2]		= { 100, 0 };	// idem pour le poids
  int intervalleBMI[2]		= { 30,  6 };	// idem pour BMI
  int intervalleGrowth[2]	= { 30,  0};	// idem pour courbe de croissance
  // Pas des courbes pour le quadrillage
  int pas_T		= 10;		// pas du quadrillage pour la mesure des T  (une ligne tous les x cm) : 5 cm
  int pas_P		= 5;		// idem pour poids : 1 kg
  int pas_Ans		= 1;		// pas du quadrillage pour les mois : 3 mois
  int pas_BMI		= 2;		// idem pour BMI
  double pas_Growth	= 2.5;		// idem pour courbe de croissance
  int distX		= 15;		// distance en pixels entre chaque lignes verticales
  int distY		= 15;		// distance en pixels entre chaque lignes horizontales
  // Marges du dessin
  int leftMarg   	= 30; 		// Marge de gauche en pixels;
  int rightMarg  	= 30; 		// Marge de droite en pixels;
  int topMarg		= 50; 		// Marge du haut en pixels;
  int bottomMarg 	= 50; 		// Marge du bas en pixels;
  // espacement
  double espGrille	= 3.0;  	// entre chaques grilles -> espacement de  x * distX
  // pour les calculs
  double nbDaysPerYears = 365.25;	// nombre de jours par mois en moyenne, tiens compte des années bisextiles
  // nb d'années ou de mois
  int nbYears		= 22;
  //...... variables de calcul
  int X, Y , nb, i, idx;			// coordonnées à tracer

  // variables qui contiendront les coordonnées du {zéro,zéro} de chaque courbes
  int xT	= leftMarg;
  int yT	= round ( topMarg + ((intervalleT[0] - intervalleT[1]) / pas_T * distY) );

  int xP	= round ( leftMarg + (nbYears / pas_Ans * distX) + (espGrille * distX) );
  int yP	= round ( topMarg + ((intervalleP[0] - intervalleP[1]) / pas_P * distY) );

  int xBMI	= round ( leftMarg );
  int yBMI	= round ( yT + (espGrille * distY) + ((intervalleBMI[0] - intervalleBMI[1]) / pas_BMI * distY ) );

  int xGrowth	= round ( leftMarg + (espGrille * distX) + (nbYears / pas_Ans * distX)  );
  int yGrowth	= round ( yT + (espGrille * distY) + ((intervalleBMI[0] - intervalleBMI[1]) / pas_BMI * distY ) );

  // Taille de l'image
  int heightPict= round ( yBMI + bottomMarg );
  int widthPict = xP + rightMarg + (nbYears / pas_Ans * distX);

  //  Calcul des coordonnées pour la taille
  //  X = round ( xT + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yT - (( valeur - intervalleT[1] ) / pas_T * distY ) ) ;	// valeur doit être en CM

  //  Calcul des coordonnées pour le poids
  //  X = round ( xP + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yP - (( valeur - intervalleP[1] ) / pas_P * distY ) ) ;	// valeur doit être en KG

  //...... Painter , Pixmap , Widget...
  QWidget 	*myWgt = new QWidget;
  QPixmap 	drawnPixmap;
  QPainter	paint;
  QPen		pen;
  QColor	color, sexColorGrid , sexColorBkgd;
  QStringList	tmp;			// contiendra les valeurs de chaques variables (poids, taille, pc)
  QDateTime	qDteNss;		// date de naissance du patient en QDateTime
  QDateTime	dateVal;		// date des valeurs en QDateTime
  long		age;			// age en jours du patient
  QPointArray   ptAr;			// pour tracer les polygones de normalité et les lignes de poids, taille et pc

  // Courbes de normalité pour les 3 premières années
  double poidsDelta2Max_H [35] = { 4.1,8,10.5,13,14,15,16,17,18,19.5,20,21.5,22.5,24.5,25.5,27,28.5,30,32,34,35.5,
				   37.5,39.5,42,44,47,50,54,58,62,65.5,71,74,76,84 };
  double poidsDelta2Min_H [35] = { 2.4,5.5,8,9,10,11.5,12,13,14,14.5,15,16,17,18,19,19.5,20.5,22,23,23.5,25,
				   25.5,26.5,28,29,30.5,32.5,34.5,37,40,42,45,48,50.5,53 };
  double poidsAge_H       [35] = { 0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,
				   10,10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,16,17,18,22 };

  double tailleDelta2Max_H [35] = { 54,70,79,85.5,91,97,101,105,109.5,112,116,119,123,126,129,132.5,135,138.5,
				    141,144,146,149,152,155,158,162,167,172,176,180,182,184,185.5,186.5,187 };
  double tailleDelta2Min_H [35] = { 46,60,69,75.5,79,83.5,87,90,93,96,99,102,104.5,107,110,112.5,115,117,120,122.5,
				    125,127,129,131,133.5,136,138,140,144,147.5,151,153.5,161.5,162.5,164 };
  double tailleAge_H       [35] = { 0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,
				   10,10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,16,17,18,22 };

  double bmiDelta2Max_H [42] = { 16.5,17.5,19.4,20.2,20.7,20.5,20.3,19.8,19.5,19.1,18.9,18.6,18.4,18.2,
				 18.1,18,17.95,17.95,18,18.1,18.3,18.5,18.8,19.1,19.5,
				 19.8,20.2,20.7,21.2,21.6,22,22.4,22.8,23.1,23.5,24,24.4,25.1,25.7,26.3,27.3,27.7 };
  double bmiDelta2Min_H [42] = { 11,12.3,14,14.9,15.4,15.2,15,14.9,14.8,14.5,14.4,14.2,
				 14.2,14.1,13.9,13.9,13.85,13.8,13.8,13.8,13.8,13.8,13.82,13.9,13.95,
				 13.98,14.05,14.2,14.3,14.55,14.7,14.8,15,15.2,15.5,15.8,16,16.5,17,17.3,17.8,18 };
  double bmiAge_H       [42] = { 0,0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.25,2.5,2.75,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,
				 10,10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,16,17,18,20,22 };

  double growthDelta2Max_H [38] = { 28.5,17.7,14.5,12.5,11.3,10,9,8.8,8.3,7.9,7.7,7.6,7.6,7.2,7,6.9,6.6,6.5,6.3,
				    6.2,6.2,6.1,6,6.2,6.5,7.8,10.5,11.4,10.6,8,6,4,2.9,2,1.5,1.2,1,0 };
  double growthDelta2Min_H [38] = { 20,10.4,8.7,7.3,6.1,5.7,5,4.8,4.6,4.5,4.4,4.3,4.3,4.3,4.1,4,3.9,3.8,3.8,
				    3.7,3.5,3.4,3.2,3.2,3.4,4,5.5,6.5,6,3.5,1,0,0,0,0,0,0,0 };
  double growthAge_H       [38] = { 1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,10,
				    10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,15.5,16,16.5,17,17.5,18,19,20,22 };


  double poidsDelta2Max_F [35] = { 4.1,8,10.5,12,13,14,14.5,16.5,17.5,18.5,19.5,21,22,23.5,25,26.5,28,30,31.5,33,36,
				   38,41,44,47,50,54.5,56,59,61,62,63,63.5,64,64 };
  double poidsDelta2Min_F [35] = { 2.4,5,7.5,8.5,9.5,10.5,11.5,12,13,13.5,14.5,15,16,17,17.5,18,19.5,20.5,
				   22,22.5,23,24.5,25.5,27,28.5,30.5,32.5,34.5,36,37.5,39,41,41.3,41.5,41 };
  double poidsAge_F       [35] = { 0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,
				   10,10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,16,17,18,22 };

  double tailleDelta2Max_F [35] = { 54,69,77,85,90,95,100,103,107,111,114,117,120,124,127,130,133,136,139,142,145,148,
				    152,156,160,164,165.5,169,170,171.5,172.5,173.5,174,174,175 };
  double tailleDelta2Min_F [35] = { 46,60,68,73.5,78,82,86,89,92,95,98,101,103,106,108,111,114,117,119,121,124,126.5,
				    129,132,135,136,142,145,147,148.5,150,151,152,152,152 };
  double tailleAge_F       [35] = { 0,0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,
				   10,10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,16,17,18,22 };

  double bmiDelta2Max_F [46] = { 15.2,17,18.8,20,20.2,20,19.5,19.2,18.5,18.5,18.4,18.2,18.2,18,17.9,17.8,17.8,17.8,17.9,18,18.1,18.4,
				 18.6,18.9,19.2,19.6,19.9,20.5,21,21.6,22.1,22.8,23.4,23.8,24.3,24.7,25,25.2,25.5,25.8,26,26.1,
				 26.2,26.5,26.6,27 };
  double bmiDelta2Min_F [46] = { 11,12.5,13.2,14.5,15,14.9,14.5,14.1,14,13.9,13.8,13.6,13.5,13.4,13.3,13.3,13.2,13.1,13.1,13.1,
				 13.1,13.2,13.3,13.4,13.5,13.6,13.8,13.9,14.1,14.3,14.5,14.7,14.9,15.1,15.4,15.6,15.8,16,16.2,16.4,
				 16.6,16.7,16.8,16.8,16.9,16.9 };
  double bmiAge_F       [46] = { 0,0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.25,2.5,2.75,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,
				 10,10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,15.5,16,16.5,17,17.5,18,19,20,22 };

  double growthDelta2Max_F [37] = { 28,18,14.5,12.7,10.7,10,9.5,8.7,8.4,8,7.6,7.7,7.8,7.5,7.4,7,8.9,8.8,
				    6.7,6.7,7.7,9.2,10.2,10.3,9.9,7.5,5.1,4.2,2.6,2,1.5,1.1,0.8,0.7,0.5,0.2,0 };
  double growthDelta2Min_F [37] = { 23,10.3,8.5,7,6.4,5.5,5,4.7,4.4,4.3,4.2,4.3,4.2,4,3.8,3.8,3.8,3.7,
				    3.6,3.5,3.6,4.2,5.5,5.8,5,3.6,2.1,1.4,0.6,0.3,0,0,0,0,0,0,0 };
  double growthAge_F       [37] = { 1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8,8.5,9,9.5,10,
				    10.5,11,11.5,12,12.5,13,13.5,14,14.5,15,15.5,16,16.5,17,17.5,18,19,20 };

  // Sélectionne selon le sexe, prépare les couleurs par de même
  QString sex = m_pMB->GetFieldValue(m_pMB->m_DOSS_IDENT_TBL_NAME, m_pMB->m_DOSS_IDENT_SEX, m_pMB->m_DOSS_IDENT_REF_PK , m_IdentPrimKey);
  if (sex == "F")
      {	sexColorGrid.setRgb(179,1,164);   // rose : (255,167,248)   pour les filles
	sexColorBkgd.setRgb(255,225,254);
      }
  else
      {	sexColorGrid.setRgb (1, 122, 179);   // bleu : (50, 147, 192) / 227,241,247   pour les garçons :)
	sexColorBkgd.setRgb(225,245,255);
      }

  //...... redéfini hauteur et largeur du pixmap
  drawnPixmap.resize( widthPict, heightPict);

  //...... débute le dessin
  paint.begin(&drawnPixmap);
  pen.setColor(sexColorBkgd);

  //...... prépare le fond d'image
  color.setRgb(255,255,255);
  paint.setBrush(color);
  pen.setColor(color);
  paint.setPen(pen);
  paint.drawRect(0,0,widthPict, heightPict);
  paint.setBackgroundColor(color);

  //...... trace les courbes de normalité selon le sexe
  // Poids
  //  Calcul des coordonnées pour le poids
  //  X = round ( xP + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yP - (( valeur - intervalleP[1] ) / pas_P * distY ) ) ;	// valeur doit être en KG
  paint.setBrush(sexColorBkgd);
  if (sex=="F")
    {	nb = 35;
	ptAr.resize(nb*2,QGArray::MemOptim);
	for (i = 0; i < nb; ++i)
	    {	X = round ( xP + (( poidsAge_F[i] ) / pas_Ans * distX ) ) ;			// valeur doit être en MOIS
		Y = round ( yP - (( poidsDelta2Max_F[i] - intervalleP[1] ) / pas_P * distY ) ) ;	// valeur doit être en KG
		ptAr.setPoint(i, X, Y);
		Y = round ( yP - (( poidsDelta2Min_F[i] - intervalleP[1] ) / pas_P * distY ) ) ;	// valeur doit être en KG
		idx = nb + (nb-1-i);
		ptAr.setPoint(idx, X, Y);
	    }
     }
  else
    {	nb = 35;
	ptAr.resize(nb*2,QGArray::MemOptim);
	for (i = 0; i < nb; ++i)
	    {	X = round ( xP + (( poidsAge_H[i] ) / pas_Ans * distX ) ) ;			// valeur doit être en MOIS
		Y = round ( yP - (( poidsDelta2Max_H[i] - intervalleP[1] ) / pas_P * distY ) ) ;	// valeur doit être en KG
		ptAr.setPoint(i, X, Y);
		Y = round ( yP - (( poidsDelta2Min_H[i] - intervalleP[1] ) / pas_P * distY ) ) ;	// valeur doit être en KG
		idx = nb + (nb-1-i);
		ptAr.setPoint(idx, X, Y);
	    }
     }
  paint.drawPolygon(ptAr);
  // Taille
  //  Calcul des coordonnées pour la taille
  //  X = round ( xT + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yT - (( valeur - intervalleT[1] ) / pas_T * distY ) ) ;	// valeur doit être en CM
  paint.setBrush(sexColorBkgd);
  if (sex=="F")
     {	nb = 35;
	ptAr.resize(nb*2,QGArray::MemOptim);
	for (i = 0; i < nb; ++i)
	    {	X = round ( xT + (( tailleAge_F[i] ) / pas_Ans * distX ) ) ;			// valeur doit être en MOIS
		Y = round ( yT - (( tailleDelta2Max_F[i] - intervalleT[1] ) / pas_T * distY ) ) ;	// valeur doit être en KG
		ptAr.setPoint(i, X, Y);
		Y = round ( yT - (( tailleDelta2Min_F[i] - intervalleT[1] ) / pas_T * distY ) ) ;	// valeur doit être en KG
		idx = nb + (nb-1-i);
		ptAr.setPoint(idx, X, Y);
	    }
     }
  else
     {	nb = 35;
	ptAr.resize(nb*2,QGArray::MemOptim);
	for (i = 0; i < nb; ++i)
	    {	X = round ( xT + (( tailleAge_H[i] ) / pas_Ans * distX ) ) ;			// valeur doit être en MOIS
		Y = round ( yT - (( tailleDelta2Max_H[i] - intervalleT[1] ) / pas_T * distY ) ) ;	// valeur doit être en KG
		ptAr.setPoint(i, X, Y);
		Y = round ( yT - (( tailleDelta2Min_H[i] - intervalleT[1] ) / pas_T * distY ) ) ;	// valeur doit être en KG
		idx = nb + (nb-1-i);
		ptAr.setPoint(idx, X, Y);
	    }
     }
  paint.drawPolygon(ptAr);
  // BMI
  //  Calcul des coordonnées pour la taille
  //  X = round ( xBMI + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yBMI - (( valeur - intervalleBMI[1] ) / pas_BMI * distY ) ) ;
  paint.setBrush(sexColorBkgd);
  if (sex=="F")
     {	nb = 46;
	ptAr.resize(nb*2,QGArray::MemOptim);
	for (i = 0; i < nb; ++i)
	    {	X = round ( xBMI + (( bmiAge_F[i] ) / pas_Ans * distX ) ) ;
		Y = round ( yBMI - (( bmiDelta2Max_F[i] - intervalleBMI[1] ) / pas_BMI * distY ) ) ;
		ptAr.setPoint(i, X, Y);
		Y = round ( yBMI - (( bmiDelta2Min_F[i] - intervalleBMI[1] ) / pas_BMI * distY ) ) ;
		idx = nb + (nb-1-i);
		ptAr.setPoint(idx, X, Y);
	    }
     }
  else
     {	nb = 42;
	ptAr.resize(nb*2,QGArray::MemOptim);
	for (i = 0; i < nb; ++i)
	    {	X = round ( xBMI + (( bmiAge_H[i] ) / pas_Ans * distX ) ) ;
		Y = round ( yBMI - (( bmiDelta2Max_H[i] - intervalleBMI[1] ) / pas_BMI * distY ) ) ;
		ptAr.setPoint(i, X, Y);
		Y = round ( yBMI - (( bmiDelta2Min_H[i] - intervalleBMI[1] ) / pas_BMI * distY ) ) ;
		idx = nb + (nb-1-i);
		ptAr.setPoint(idx, X, Y);
	    }
     }
  paint.drawPolygon(ptAr);
  // CROISSANCE
  paint.setBrush(sexColorBkgd);
  if (sex=="F")
     {	nb = 37;
	ptAr.resize(nb*2,QGArray::MemOptim);
	for (i = 0; i < nb; ++i)
	    {	X = round ( xGrowth + (( growthAge_F[i] ) / pas_Ans * distX ) ) ;
		Y = round ( yGrowth - (( growthDelta2Max_F[i] - intervalleGrowth[1] ) / pas_Growth * distY ) ) ;
		ptAr.setPoint(i, X, Y);
		Y = round ( yGrowth - (( growthDelta2Min_F[i] - intervalleGrowth[1] ) / pas_Growth * distY ) ) ;
		idx = nb + (nb-1-i);
		ptAr.setPoint(idx, X, Y);
	    }
     }
  else
     {	nb = 38;
	ptAr.resize(nb*2,QGArray::MemOptim);
	for (i = 0; i < nb; ++i)
	    {	X = round ( xGrowth + (( growthAge_H[i] ) / pas_Ans * distX ) ) ;
		Y = round ( yGrowth - (( growthDelta2Max_H[i] - intervalleGrowth[1] ) / pas_Growth * distY ) ) ;
		ptAr.setPoint(i, X, Y);
		Y = round ( yGrowth - (( growthDelta2Min_H[i] - intervalleGrowth[1] ) / pas_Growth * distY ) ) ;
		idx = nb + (nb-1-i);
		ptAr.setPoint(idx, X, Y);
	    }
     }
  paint.drawPolygon(ptAr);

  //...... trace les quadrillages
  Graph_Quadrillage	(nbYears,0,intervalleP[0], intervalleP[1], pas_Ans, pas_P,
			 xP, yP, distX, distY,
			 2,10, TR("Poids"), paint, sexColorGrid);
  Graph_Quadrillage	(nbYears,0,intervalleT[0], intervalleT[1], pas_Ans, pas_T,
			 xT, yT, distX, distY,
			 2,20, TR("Taille"),  paint, sexColorGrid);
  Graph_Quadrillage	(nbYears,0,intervalleBMI[0], intervalleBMI[1], pas_Ans, pas_BMI,
			 xBMI, yBMI, distX, distY,
			 2,2, TR("BMI"),  paint, sexColorGrid);
  Graph_Quadrillage	(nbYears,0,intervalleGrowth[0], intervalleGrowth[1], pas_Ans, pas_Growth,
			 xGrowth, yGrowth, distX, distY,
			 2,5, TR("Croissance"),  paint, sexColorGrid);

  //...... insère bas de page (graphique réalisé par Medin'Tux le )
  QString nom = TR("Graphique réalisé par DrTux le : ") + QDateTime::currentDateTime().toString(Qt::LocalDate);
  X = (widthPict / 2) - 200 + (nom.length()*10);
  Y = (heightPict - 25);
  PutLabel(X,Y,nom,paint);

  //...... insère nom, prénom, âge actuel du patient
  nom  = m_pMB->GetFieldValue(m_pMB->m_DOSS_INDEX_TBL_NAME, m_pMB->m_DOSS_INDEX_NOM,
					 m_pMB->m_DOSS_INDEX_PRIM_KEY , m_IdentPrimKey)+" ";
  nom += m_pMB->GetFieldValue(m_pMB->m_DOSS_INDEX_TBL_NAME, m_pMB->m_DOSS_INDEX_PRENOM,
					 m_pMB->m_DOSS_INDEX_PRIM_KEY , m_IdentPrimKey);
  X = leftMarg + (nom.length()*10);
  Y = round(distX*1.5);
  PutLabel(X,Y,nom,paint);
  myWgt->setCaption(nom);

  //...... récupère date de NAISSANCE DU PATIENT !!!!
  QVariant qvar;
  m_pMB->GetFieldValue(m_pMB->m_DOSS_IDENT_TBL_NAME, m_pMB->m_DOSS_IDENT_NSS, m_pMB->m_DOSS_IDENT_REF_PK , m_IdentPrimKey, &qvar);
  qDteNss = qvar.toDate();

  //...... récupère les valeurs de poids, taille et PC
  //...... vérifie que les valeurs ne sont pas plus âgées que 22 ans     //////////////// A FAIRE /////////////////
  //...... dessine les valeurs
  double valVar;
  QMemArray<double> 	age_poids(1); 	 	// variable utilisée pour le calcul du BMI;  en [i] = age en jours ; en [i+1] = poids
  QMemArray<double> 	age_taille(1);  	// variable utilisée pour le calcul du BMI; idem avec taille
  QString     section  = "VAR Poids";
  QString     variable = "Poids";
  QStringList valeurs  = C_TokenInterpret::GetVar(section,variable);

  //...... calcul pour chaque valeurs de poids l'âge (en mois) à la mesure
  pen.setWidth(2);
  color.setRgb(0,0,0);
  pen.setColor(color);
  paint.setPen(pen);
  paint.setBrush(color);
  ptAr.resize(valeurs.count(),QGArray::MemOptim);
  age_poids.resize(valeurs.count()*2,QGArray::MemOptim);
  valeurs.sort();
  // dessine les points
  //  Calcul des coordonnées pour le poids
  //  X = round ( xP + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yP - (( valeur - intervalleP[1] ) / pas_P * distY ) ) ;	// valeur doit être en KG
  for (i = 0; i != (int)valeurs.count(); ++i )
    {	tmp = QStringList::split("/;/", valeurs[i]);
        dateVal = dateVal.fromString(tmp[0] , Qt::ISODate);
        age     = qDteNss.daysTo(dateVal);
	valVar = tmp[1].toDouble();
	age_poids[i*2]	 	= (double)(age);
	age_poids[i*2+1]	= valVar;
        X = round( xP + (( age_poids[i*2] / nbDaysPerYears ) / pas_Ans * distX ) ) ;
        Y = round( yP - (( age_poids[i*2+1] - intervalleP[1] ) / pas_P * distY ) );
 	paint.drawRect(X-2,Y-2,5,5);
	ptAr.setPoint(i, X, Y);
    }
  // trace la ligne entre les points
  paint.drawPolyline(ptAr);


  //...... récupère les valeurs de taille
  section  = "VAR Poids";
  variable = "Taille";
  valeurs  = C_TokenInterpret::GetVar(section,variable);
  //...... calcul pour chaque valeurs de poids l'âge (en mois) à la mesure
  pen.setWidth(2);
  paint.setPen(pen);
  color.setRgb(0,0,0);
  paint.setBrush(color);
  ptAr.resize(valeurs.count(),QGArray::MemOptim);
  age_taille.resize(valeurs.count()*2,QGArray::MemOptim);
  valeurs.sort();
  // dessine les points
  //  Calcul des coordonnées pour la taille
  //  X = round ( xT + (( valeur / nbDaysPerMonth ) / pas_Mois * distX ) ) ;	// valeur doit être en JOURS
  //  Y = round ( yT - (( valeur - intervalleT[1] ) / pas_T * distY ) ) ;	// valeur doit être en CM
  for (i = 0; i != (int)valeurs.count(); ++i )
    {	tmp = QStringList::split("/;/", valeurs[i]);
        dateVal = dateVal.fromString(tmp[0] , Qt::ISODate);
        age     = qDteNss.daysTo(dateVal);
	valVar = tmp[1].toDouble();
	age_taille[i*2] 	= (double)(age);
	age_taille[i*2+1]	= valVar;
	Y = round ( yT - (( age_taille[i*2+1] - intervalleT[1] ) / pas_T * distY ) );
 	X = round ( xT + (( age_taille[i*2] / nbDaysPerYears ) / pas_Ans * distX ) ) ;
 	paint.drawRect(X-2,Y-2,5,5);
	ptAr.setPoint(i, X, Y);
    }
  // trace la ligne entre les points
  paint.drawPolyline(ptAr);



  //...... calcul les BMI et trace la courbe
  int    j = 0;
  int    k = 0;
  int    limite = 60;
  double ageMin, ageMax, findTaille;
  QMemArray<double> age_bmi(age_poids.count());

  // calcul les bmi
  for (i = 0; i < (int)age_poids.count(); i = i + 2)		// pour toutes les valeurs de poids
   { ageMin = age_poids[i] - limite;
     ageMax = age_poids[i] + limite;
     findTaille = 0;
     for (j = 0; j < (int)age_taille.count(); j = j + 2)	// teste toutes les valeurs de taille pour trouver la meilleure
      {	if (age_taille[j] > ageMin)
	  if (age_taille[j] < ageMax)
		{ findTaille = age_taille[j+1];  break; }
      } // fin for j
     if (findTaille != 0)				// taille correspondante au poids trouvée
      { age_bmi[k] = age_poids[i];			// age bmi et poids sont identiques
	age_bmi[k+1] = age_poids[i+1] / pow((findTaille/100),2);
	k += 2;
      } // fin if
    } // fin for i

   // redéfini la taille des tableaux
   age_bmi.resize(k,QGArray::MemOptim);
   ptAr.resize(k/2,QGArray::MemOptim);

  // trace la courbe
  for (i = 0; i != k; i = i + 2)
   {	X = round ( xBMI + (( age_bmi[i] / nbDaysPerYears ) / pas_Ans * distX ) ) ;
	Y = round ( yBMI - (( age_bmi[i+1] - intervalleBMI[1] ) / pas_BMI * distY ) ) ;
 	paint.drawRect(X-2,Y-2,5,5);
        idx = i/2;
	ptAr.setPoint(idx, X, Y);
    }
  paint.drawPolyline(ptAr);



  //...... calcul la croissance annuelle
  if (age_taille.count() >= 2)
   {limite = 400;
    i = 0;
    j = 0;
    k = 0;
    QMemArray<double> age_growth(age_taille.count());

    // trouve les valeurs
    for (i = 2; i < (int)age_taille.count(); i = i + 2)	// pour toutes les valeurs de taille (hormis la première)
     { ageMin = age_taille[i] - limite;
       ageMax = ageMin + (limite/5);
       findTaille = 0;

       for (j = 0; j < i; j = j + 2)		// teste toutes les valeurs antérieures de taille pour trouver celle de l'année précédente
        { if (age_taille[j] > ageMin)
           if (age_taille[j] < ageMax)
	    { findTaille = age_taille[j+1];  break; }
        }  // for j

       if (findTaille != 0)				// taille correspondante au poids trouvée
        { age_growth[k]   = age_taille[i];		// age growth et taille sont identiques
          age_growth[k+1] = age_taille[i+1] - age_taille[j+1];
  	  k += 2;
        } // fin if

      } // fin for i

     // redéfini la taille des tableaux
     age_growth.resize(k,QGArray::MemOptim);
     ptAr.resize(k/2,QGArray::MemOptim);

    // trace la courbe
    for (i = 0; i != k; i = i + 2)
     {	X = round ( xGrowth + (( age_growth[i] / nbDaysPerYears ) / pas_Ans * distX ) ) ;
	Y = round ( yGrowth - (( age_growth[i+1] - intervalleGrowth[1] ) / pas_Growth * distY ) ) ;
 	paint.drawRect(X-2,Y-2,5,5);
        idx = i/2;
	ptAr.setPoint(idx, X, Y);
      }
    paint.drawPolyline(ptAr);
   } // fin if age_taille.count() >= 2



  //...... Fin du dessin
  paint.end();
  QLabel *lblPix = new QLabel (myWgt);

  lblPix->setPixmap(drawnPixmap);
  lblPix->adjustSize ();
  myWgt->adjustSize ();
  myWgt->show();
  // DANGER MEMORY where delete myWgt ?
}

