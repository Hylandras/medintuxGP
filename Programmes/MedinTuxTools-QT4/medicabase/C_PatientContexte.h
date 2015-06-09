/************************** C_PatientContexte.h ***************************
*  #include "C_PatientContexte.h"        QT4-ready                        *
* ........................................................................*
*   Project   :  MedinTux  (typographie made in france)                   *
*   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
*   by        :  Sevin Roland     from   MedinTux project                 *
*   E-Mail    :  roland-sevin@medintux.org                                *
*   Web site  :  www.medintux.org                                         *
* ........................................................................*
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#ifndef C_PATIENTCONTEXTE_H
#define C_PATIENTCONTEXTE_H

#include <QObject>
//========================================= C_PatientContexte ============================
class C_PatientContexte : public QObject
{
    Q_OBJECT
public:
    explicit C_PatientContexte(const QString & patient_guid, const QString &patient_pk, QObject *parent = 0);

public:
    QString get_poids();
    QString get_taille();
    QString get_age();


signals:

public slots:

};

#endif // C_PATIENTCONTEXTE_H
