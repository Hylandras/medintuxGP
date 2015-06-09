/********************************* CIndexMakeThread.cpp ***************************
 *                                                                                *
 *   #include "CIndexMakeThread.h"                                                *
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

#include "CIndexMakeThread.h"
#include "../../MedinTuxTools/CMedicaBase.h"

#include <qobject.h>
//------------------------------------------------ Datasemp_Init --------------------------------------------
CIndexMakeThread::CIndexMakeThread(CMedicaBase *pCMedicaBase, QTextEdit *logWidget /* = 0 */)
{m_pCMedicaBase        = pCMedicaBase;
 m_Status              = QObject::tr( "Thread non lancé" );
 m_NbTotal             = 0;
 m_pQTextEditLogWidget = logWidget;
}

//------------------------------------------------ ~CIndexMakeThread ------------------------------------------------------
CIndexMakeThread::~CIndexMakeThread()
{//QMutexLocker ml( &m_mutex );
 if (m_pCMedicaBase)  m_pCMedicaBase->Medica_Stop();
}

//------------------------------------------------ run ------------------------------------------------------
void CIndexMakeThread::run()
{//QMutexLocker ml( &m_mutex );
 m_Run     = 1;
 if (m_pCMedicaBase)
     {m_NbTotal        = m_pCMedicaBase->Datasemp_GetNbRecord("t00").toLong();
      m_Status         = QObject::tr( "-1- Index accélérateur en cours de création : " );
      if (m_pCMedicaBase->Datasemp_MakeAllIndexProduit(0, m_pQTextEditLogWidget)>0)
         {//..................... placer le numero de version du datasemp au niveau de l'index ..........................
          QString numEdition;
          QString ods         = m_pCMedicaBase->Datasemp_GetBaseVersion( &numEdition, 0, 0);
          if (ods.length())
             {numEdition      =   numEdition + ods.mid(6,2)+ "." + ods.mid(7,1);
             }
          m_pCMedicaBase->Medica_SetDatasempVersion(numEdition, CMedicaBase::Index);
         }
      m_NbTotal        = m_pCMedicaBase->Medica_GetNbRecord(m_pCMedicaBase->m_MEDICA_INDEX_MED_TBL_NAME).toLong();
      m_Status         = QObject::tr( "-2- Table de spécifications produits en cours de création : " );
      if (m_pCMedicaBase->Datasemp_ToMedicaTux(0)>0)
         {//..................... placer le numero de version du datasemp au niveau des donnees ..........................
          QString numEdition;
          QString ods         = m_pCMedicaBase->Datasemp_GetBaseVersion( &numEdition, 0, 0);
          if (ods.length())
             {numEdition      =   numEdition + ods.mid(6,2)+ "." + ods.mid(7,1);
             }
          m_pCMedicaBase->Medica_SetDatasempVersion(numEdition, CMedicaBase::Data);
         }
      m_Status         = QObject::tr( "-3- Fin " );
     }
 m_Run     = 0;
}

//------------------------------------------------ stop ------------------------------------------------------
void CIndexMakeThread::stop()
{//QMutexLocker ml( &m_mutex );
 if (m_pCMedicaBase)  m_pCMedicaBase->Medica_Stop();
}

//------------------------------------------------ IsRun ------------------------------------------------------
int CIndexMakeThread::IsRun()
{//QMutexLocker ml( &m_mutex );
 return  m_Run;
}

//------------------------------------------------ start -------------------------------------------------------------------
void  CIndexMakeThread::start()
{//QMutexLocker ml( &m_mutex );
 m_Run     = 1;
 QThread::start(QThread::LowPriority);
}

//-----------------------------------------------------  GetCurrentStatus -------------------------------------------
QString  CIndexMakeThread::GetCurrentStatus()
{//QMutexLocker ml( &m_mutex );
 return m_Status;
}
//-----------------------------------------------------  GetTotalProgress -------------------------------------------
long  CIndexMakeThread::GetTotalProgress()
{//QMutexLocker ml( &m_mutex );
 return m_NbTotal;
}
//-----------------------------------------------------  GetCurrentProgress -------------------------------------------
long  CIndexMakeThread::GetCurrentProgress()
{//QMutexLocker ml( &m_mutex );
 if (m_pCMedicaBase)  return m_pCMedicaBase->Medica_GetProgress();
 return 0;
}

