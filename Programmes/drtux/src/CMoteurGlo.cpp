/********************************* CMoteurGlo.cpp *********************************
 *                                                                                *
 *   #include "CMoteurGlo.h"                                                      *
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
 *   Commissariat �l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F��ation, 75752 PARIS cedex 15.   *
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
#include <qobject.h>

#include "CApp.h"
#include "CMoteurGlo.h"
#include "CDevilCrucible.h"
#include "drtux.h"
#include "CMDI_Documents.h"

#include "../../MedinTuxTools/CGestIni.h"
#include "../../MedinTuxTools/CHtmlTools.h"
#include "../../MedinTuxTools/CGenTools.h"
#include "../../MedinTuxTools/CMedicaBase.h"

/*! \class CMoteurGlo
 *  \brief toutes les fonctions pour gérer le glossaire
*/


//------------------------------ CMoteurGlo ----------------------------------------------------
/*! \brief Constructeur de la classe. Récupère le path des documents.
*/
CMoteurGlo::CMoteurGlo()
{m_curPath = G_pCApp->m_PathGlossaire + CMDI_Documents::S_GetRubName();
 m_RubType = TYP_OBSERVATION;
 m_QDir.setPath ( m_curPath );
 m_curPath = m_QDir.absPath ();
}



//------------------------------------- SetListTyp -------------------------------------------
int CMoteurGlo::SetListTyp( int typ , const QString &path, QListView *pQlistView /* =0 */ )
{if (m_RubType == typ) return typ;   // pas la peine de changer car on y est deja et peut etre positionn�dans un sous dossier
 int   old_typ = m_RubType;
 m_RubType     = typ;
 m_curPath     = path;
 if (pQlistView) GetMasquesList(pQlistView , m_curPath);
 return old_typ;
}



//------------------------------------- GetMasquesObsList ------------------------------------
/*! \brief non documenté.
*/
long CMoteurGlo::GetMasquesList(  QListView *pQlistView , const char *path /*=0*/)
{   int        i = 0;
    int   isHome = 0;

    //.............. ne pas reculer au delas du permis ..............................
    //if (folder_name && strcmp(folder_name,".." )==0 && home == m_curPath)   return 0;
    //if (folder_name&& *folder_name!=0) m_curPath  = m_curPath + QDir::separator() + folder_name;
    //else                               m_curPath  = home;

    m_QDir.setPath ( path );
    m_QDir.convertToAbs();
    m_curPath = m_QDir.path();
    pQlistView->clear();
    m_QDir.setFilter( QDir::All | QDir::NoSymLinks );
    //if (home == m_curPath) isHome = TRUE;
    //dir.setSorting( QDir::DirsFirst|QDir::Size | QDir::Reversed );

    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = m_QDir.entryInfoList();
    if (pQFileInfoList==0) return 0;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo     *fi;
    QListViewItem *element;
    while ( (fi = it.current()) != 0 )
          {element = 0;
           QString fname = fi->fileName().latin1();
           QString ext   = QFileInfo(fname).extension().lower();
           if      (fname == "..")
                   {if (isHome==0)
                       {element = new QListViewItem( pQlistView, fname , "B" );
                        element->setPixmap ( 0, Theme::getIcon("22x22/previous.png")   );
                        m_curPath = m_QDir.absPath ();
                       }
                   }
           else if (fi->isDir() && fname[0] != '.' && fname.lower() != "cvs")
                   {element = new QListViewItem( pQlistView, fname , "D" );
                    element->setPixmap ( 0, Theme::getIconListFolder().pixmap(QIconSet::Automatic,TRUE,QIconSet::On) );
                   }
           else if (fi->isFile() && fname[0] != '.')
                   {if (ext.length())
                       {if (ext=="html"||ext=="htm"||ext=="txt"||ext=="xml"||ext=="rtf")
                           {element = new QListViewItem( pQlistView, fname , "F" );
                            element->setPixmap ( 0, Theme::getIconListHTML().pixmap(QIconSet::Automatic,TRUE,QIconSet::On)   );
                           }
                        else if (ext=="ord")
                           {element = new QListViewItem( pQlistView, fname , "F" );
                            element->setPixmap ( 0, Theme::getIcon("16x16/type_prescription.png")   );
                           }
                       }
                   }

           if (element) {pQlistView->insertItem ( element ); element->setDragEnabled ( TRUE );}
           ++it;
           i++;
          }
    pQlistView->setSorting (1);
    return 1;
}

//-------------------------------- PathGlossaireToTyp ----------------------------------------------------
/*! \brief Retourne le type: GLO_TYP_RUB_OBSERV ou GLO_TYP_RUB_PRESCR selon le path courant en cours utile pour permettre d'envoyer un document as le bon editeur de  texte en foction de sa localisation d'origine.
*/
int CMoteurGlo::PathGlossaireToTyp()
{int typ = G_pCApp->PathDocumentToType( m_curPath  );
 if (typ) return typ;
 else     return m_RubType;
}



//------------------------------------- SetTextEditWithData -----------------------------------------------
/*! \brief non documenté */
void CMoteurGlo::SetTextEditWithData(  QString file , CW_EditText *pQTextEdit)
{
 QString stringDST, str;
 QString qspath =  m_curPath + QDir::separator() + file;
 CGestIni::Param_UpdateFromDisk(qspath, str);
 QString ext = CGenTools::GetExtension(qspath).lower();
 if (ext == ".rtf" )
    {CHtmlTools::RtfToQtextEdit_HTML(str, str.length(), stringDST);
    }
 else if ( ext == ".ord" )
    {CMedicaBase::Medica_DiskDataSplitIn_HtmlData_StructData(str, &stringDST, 0);    // y isoler et recuperer les données textuelle
    }
 else
    {stringDST = str;
    }
 //............. creer la liste des documents �l'affichage ...........................
 DOCUMENT_DISPLAY_MAP  currentDocDisplayMap;
 QString user   = G_pCApp->m_pDrTux->MapActiveID_Doc(currentDocDisplayMap);   // retour = utilisateur le plus probable

 QString userPk = G_pCApp->m_pCMoteurBase->GetUserPrimKey(user);

 CDevilCrucible *pfusion = new CDevilCrucible(&stringDST                     ,          // 0 texte du document �fusionner
                                               G_pCApp->m_pCMoteurBase       ,          // 1 moteur de base de donn�s (faut bien acc�er aux fonctions)
                                               G_pCApp->m_pCMedicaBase       ,          // 2 moteur de base de données medicamenteuses(faut bien accéder aux fonctions)
                                              &G_pCApp->m_pDrTux->m_RubList  ,          // 3 liste des documents composant le dossier patient
                                               G_pCApp->m_pDrTux->m_ID_Doss  ,          // 4 il faut les renseignements sur le patient
                                              &currentDocDisplayMap          ,          // 5 liste des rubriques courantes (affich�s)
                                               userPk                        ,          // 6 utilisateur responsable du document
                                               0                             ,          // 7 pointeur sur la rubrique en cours de modif (si c'est une rubrique) zero sinon
                                               &G_pCApp->m_VAR_MAP           ,          // 8 pointeur sur la liste des variables (zero par defaut)
                                               CDevilCrucible::DoAll                    // 9 defaut = CDevilCrucible::DoAll indique que c'est juste pour la visualisation ou to faire

                                             );
 if (  pfusion )
    {pfusion-> DoMixture();
     delete pfusion;
    }
 pQTextEdit->setText(stringDST);
}

