/***************************** C_PatientBase_Var.h ************************
*  #include "C_PatientBase_Var.h"        QT4-ready                        *
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
#ifndef C_PATIENTBASE_H
#define C_PATIENTBASE_H
#include <QObject>
#include "../../MedinTuxTools-QT4/C_BaseCommon.h"
#include "../../MedinTuxTools-QT4/C_PatientBase/C_PatientBase_Var.h"
//============================================= C_PatientBase ===========================================================================
class C_PatientBase: public C_BaseCommon,            // le gestionnaire generique de bases de donnee
                            C_PatientBase_Var        // le nom des champs de la base de donnees
{

    Q_OBJECT

public:
    C_PatientBase(const QString   &confData,                        // definition des champs de la base de donnee (ceux de la liste des champs)
                  const QString   &params ,                         // parametres de connexion a la base de donnees
                  const QString   &pathIni,
                  QObject         *parent
                 );
    ~C_PatientBase();
    QString guid_To_Pk_Name_Forename(const QString &guid, QString *u_name = 0, QString *p_name = 0 );
    int pk_to_BaseIdent(const QString &pk,
                        QString *pm_DOSS_IDENT_NSS,
                        QString *pm_DOSS_IDENT_SEX,
                        QString *pm_DOSS_IDENT_JFNOM);
    int     isValid(){return m_isValid;}
    QString pathIni(){return m_pathIni;}
    int createTablesDatees(const QString tableName, const QStringList fieldsList);
private:
    int        m_isValid;
    QString    m_pathIni;
    QSqlQuery *m_pQSqlQuery;
signals:
    void Sign_onError(const QSqlQuery &qsqlquery, const QString &from, const QString &sql_query, const QString &file, int line);

};

#endif // C_PATIENTBASE_H
