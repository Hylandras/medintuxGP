 /********************************* Tools.cpp **********************************
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 2                                   *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
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
/*! \file Tools.cpp
    \brief (put some short descripion here).

     #include "Tools.h"
     Project: Comptabilité pour MedinTux
     Copyright (C) 2007
     by Eric Maeker , eric.maeker@free.fr
*/

#include "Tools.h"


Tools::Tools()
{
}


Tools::~Tools()
{
}

/*! \brief Crée à partir des deux QStringList un tableau HTML avec les paramètres de padding et de spacing des cellules.
 *  La QStringList HEADER (hd) définit le nombre de colonnes du tableau. Les lignes du tableau (lines) sont calculés et affichées en fonction de ce nombre de colonnes.
*   Si hd.count() == 3 alors les lignes seront calculées par groupes de trois.
*  \todo insérer un style CSS ? améliorer l'affichage en basculant une ligne sur deux sur une couleur différente.
*/
bool Tools::createHTMLTable(QStringList& hd, QStringList& lines, QString& retour, int padding, int spacing)
{ if (hd.count() == 0) return FALSE;
  if (lines.count() == 0) return FALSE;

  int nbCol = hd.count();

  retour += "\n\n<table  width=100% border=1 cellpadding="+QString::number(padding)+" cellspacing="+QString::number(spacing)+"><tr>\n";
  for (int i=0; i < (int)hd.count(); ++i)
  { retour += "<td align=\"center\"><b>"+hd[i]+"</b></td>\n"; }
  retour += "</tr>\n<tr>\n";

  int nbLines = lines.count() / nbCol;
  int nbPerLines = lines.count() / nbLines;
  for (int r=0; r < nbLines; r++)
   { for (int i=0; i < nbPerLines; ++i)
       { retour += "<td align=\"center\">"+lines[r*nbPerLines + i]+"</td>\n"; }
     retour += "</tr>\n<tr>\n";
     }

  retour += "</tr>\n</table>\n\n";
  return TRUE;
}


/*! \brief Recherche dans le champs remarque passé en paramètre la string toFind et renvoie la valeur qui lui est associée dans value.
 *  \return TRUE si trouvé, FALSE cas contraire.
 * Cette fonction est utilisé pour "décrypter" les champs remarque.\n
 * Exemple :  |tx=33.33|id=23_|esp \n
 * rechercher |tx= renvoie TRUE et 33.33 dans value \n
 * rechercher |id= renvoie TRUE et 23_ dans value \n
 * rechercher |esp renvoie TRUE et "" dans value
*/
bool Tools::getIntoRemarque(const char* champs_Remarque, const char* toFind, QString& value)
{ QString tmp = champs_Remarque;
  int posdeb = tmp.find(toFind, 0);
  if (posdeb != -1) // Trouvé
  { // Récupère le nom
    posdeb += 4;
    int posfin = posdeb;
    posfin = tmp.find( "|", posdeb-1);
    if (posfin == -1)
    { posfin = tmp.length();  }
    if (posdeb == posfin) value = "";
    else value = tmp.mid(posdeb,posfin-posdeb);
    return TRUE;
  }
  else
    return FALSE;
}

