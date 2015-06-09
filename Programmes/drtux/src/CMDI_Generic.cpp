/********************************* CMDI_Generic.cpp *******************************
 *                                                                                *
 *   #include "CMDI_Terrain.h"                                                    *
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
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
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

#include "CMDI_Generic.h"
#include "CMDI_Prescription.h"
#include "drtux.h"
#include "../../MedinTuxTools/CPrtQListBoxItem.h"
#include "C_FormGlossaire.h"

/*! \class CMDI_Generic
 *  Classe dont hériteront toutes les autres CMDI.
*/

//---------------------------------- CMDI_Generic ----------------------------------------------------------------------------------
CMDI_Generic::CMDI_Generic(QWidget* parent, const char* name, int wflags, CMoteurBase *pCMoteurBase, RUBREC_LIST  *pRubList, int type)
        :QMainWindow( parent, name, wflags ), m_pCMoteurBase(pCMoteurBase) , m_RubName(name), m_pRubList(pRubList)
        {setMouseTracking ( TRUE );
         SetRubName(name);
         SetType(type);
         m_DefaultMaskDirectory = GetDefaultMaskDirectory();
         m_PathDocuments        = m_DefaultMaskDirectory;
         m_PrefixDroit          = "xx";
        }

//---------------------------------- DoActionWithThisDocument ----------------------------------------------------------------------------------
void CMDI_Generic::DoActionWithThisDocument(const QString &/*action*/, int /*typ*/, const char * /*path =0*/, const QDateTime * /*pQDateTime=0*/, const QString * /*libelle=0*/, const QString * /*subType=0*/)
{return ;
}

//---------------------------------- ChangeCurrentComboBoxItemName ----------------------------------------------------------------------------------
void CMDI_Generic::ChangeCurrentComboBoxItemName(const QString &name)
{if (name.length()==0)               return;
 CPrtQListBoxItem* pCPrtQListBoxItem  = GetCurrentDocItem();
 if (pCPrtQListBoxItem==0)           return;
 pCPrtQListBoxItem->setText ( name );
}
//---------------------------------- GetCurrentDocItem ----------------------------------------------------------------------------------
CPrtQListBoxItem    *CMDI_Generic::GetCurrentDocItem()
{ QComboBox    *pQComboBox = GetDocListComboBox();
  if (pQComboBox==0)            return 0;
  if (pQComboBox->count ()==0)  return 0;
  int currentItem = pQComboBox->currentItem();
  if (currentItem >= 0)
     { return (CPrtQListBoxItem*) pQComboBox->listBox()->item ( currentItem );
     }
  return 0;
}

//---------------------------------- AddNewDocument ----------------------------------------------------------------------------------
CRubRecord*   CMDI_Generic::AddNewDocument(const QString &/*strData*/, int /*typ*/, const char * /*path =0*/, const QDateTime * /*pQDateTime=0*/, const QString * /*libelle=0*/,  const QString * /*subType=0*/, int  /*=0*/)
{return 0;
}

//---------------------------------- GetType ----------------------------------------------------------------------------------
int CMDI_Generic::GetType()
               {return m_Type;
               }
//---------------------------------- SetType ----------------------------------------------------------------------------------
void CMDI_Generic::SetType(int type)
               {m_Type = type;
               }

//---------------------------------- GetDefaultMaskDirectory ----------------------------------------------------------------------------------
QString CMDI_Generic::GetDefaultMaskDirectory()
{return (G_pCApp->m_PathGlossaire + m_RubName);
}
//---------------------------------- GetDefaultMaskDirectory ----------------------------------------------------------------------------------
QString CMDI_Generic::GetCurrentMaskDirectory()
{return (m_PathDocuments);
}

//---------------------------------- GetDefaultMaskFile ----------------------------------------------------------------------------------
QString CMDI_Generic::GetDefaultMaskFile()
{return (GetDefaultMaskDirectory()+"/default.html");
}
//---------------------------------- setGeometry ----------------------------------------------------------------------------------
void CMDI_Generic::setGeometry(QRect rect, int flag)
{
 setCaption(m_RubName);
 //................................. un peu de geometrie ........................................
 QString val1, val2, val3, val4;
 if (flag) QMainWindow::setGeometry (rect);
 if ( READ_USER_PARAM(USER_PARAM ,m_RubName, tr("Position"), &val1, &val2, &val3, &val4)==0) // zero = pas d'erreur
    {  QPoint p(atoi(val1),atoi(val2));        // store position
       QSize  s(atoi(val3),atoi(val4));        // store size
       resize( s );        // restore size
       move( p );          // restore position
    }

 if ( READ_USER_PARAM(USER_PARAM, m_RubName, "Espace de travail", &val1 )==0)  // zero = pas d'erreur
    {    CGestIni::retrieveStringForParam(val1);
         QCString cStr  = val1.utf8 ();
         long  len_data =  cStr.length ();
         QByteArray array( len_data ) ;
         memcpy( array.data(), (const char*) cStr, len_data);
         QTextStream tsa( array, IO_ReadOnly );
         tsa >> *this;
    }
 }

//---------------------------------- saveGeometry ----------------------------------------------------------------------------------
void CMDI_Generic::saveGeometry()
 {
   QPoint cmdi_pos  = pos() ;                    // permet de relever hauteur et largeur decorations
   QPoint p_pos     = parentWidget ()->pos() ;   // store position
   QSize  p_size    = parentWidget ()->size();   // store size
   WRITE_USER_PARAM( &USER_PARAM , m_RubName, tr("Montrer"), tr("Normal"));
   WRITE_USER_PARAM( &USER_PARAM , m_RubName, tr("Position"),
                                                 QString::number(p_pos.x()),                   QString::number(p_pos.y()),
                                                 QString::number(p_size.width()-cmdi_pos.x()), QString::number(p_size.height()-cmdi_pos.y()),
                                                 QString::number(x()),                         QString::number(y()),
                                                 QString::number(width()),                     QString::number(height()));
    QByteArray  array ;
    QTextStream tsa( array, IO_WriteOnly);
    tsa << *this;
    QString str = "";
    str = str.insert(0,array);
    CGestIni::formatStringForParam(str);
    WRITE_USER_PARAM( &USER_PARAM , m_RubName, tr("Espace de travail"), str);
}

//---------------------------------- ActiverRubrique ----------------------------------------------------------------------------------
void    CMDI_Generic::ActiverRubrique(const char *rubName,int)
{   if (rubName != m_RubName) return;
    showNormal();
}


//-------------------------------- On_DirectoryGlossaireChanged ----------------------------------------------------
void CMDI_Generic::On_DirectoryGlossaireChanged(const QString &pathDocuments, const QString &defaultMaskDirectory)
{if (defaultMaskDirectory == "?"){ m_DefaultMaskDirectory = pathDocuments;
                                   m_PathDocuments        = pathDocuments;
                                 }
 else if (defaultMaskDirectory == m_DefaultMaskDirectory) m_PathDocuments = pathDocuments;
}

//----------------------------------------------- On_DragMoveEvent ------------------------------------------------------------------------
void CMDI_Generic::DoConnexionOnGossaire(C_FormGlossaire *pC_FormGlossaire)
{if (pC_FormGlossaire==0) return;
 connect( pC_FormGlossaire, SIGNAL(Sign_DirectoryChanged(const QString &, const QString &)),  this,   SLOT(  On_DirectoryGlossaireChanged(const QString &,const QString &)) );
 connect( this, SIGNAL(Sign_MouseEnter(QString &, QString &)),  pC_FormGlossaire,   SLOT(  On_Sign_MouseEnter(QString &, QString &)) );
 connect( this, SIGNAL(Sign_MouseLeave(QString &, QString &)),  pC_FormGlossaire,   SLOT(  On_Sign_MouseLeave(QString &, QString &)) );
}

//----------------------------------------------- eventFilter ---------------------------------------------------
bool CMDI_Generic::eventFilter( QObject *o, QEvent *e )
{if (e->type ()==QEvent::Enter) mouseEnter ();  //Enter = 10, Leave = 11
 if (e->type ()==QEvent::Leave) mouseLeave ();  //Enter = 10, Leave = 11
 return QMainWindow::eventFilter(o,e);
}

//----------------------------------------------- mouseEnter ---------------------------------------------------
/*! \brief recupere l'evenement lorsque la souris rentre dans la rubrique
 */
void CMDI_Generic::mouseEnter ()
{emit Sign_MouseEnter(m_PathDocuments, m_DefaultMaskDirectory);
}

//----------------------------------------------- mouseLeave ---------------------------------------------------
/*! \brief recupere l'evenement lorsque la souris sort de la rubrique
 */
void CMDI_Generic::mouseLeave ()
{emit Sign_MouseLeave(m_PathDocuments, m_DefaultMaskDirectory);
}
