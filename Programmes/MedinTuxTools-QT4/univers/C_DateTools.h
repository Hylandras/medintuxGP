/***************************** C_DateTools.h ******************************
*  #include "C_DateTools.h"        QT4-ready                              *
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
#ifndef C_DATETOOLS_H
#define C_DATETOOLS_H
#include <QList>
#include <QString>

class QDate;
class C_DateTools
{

public:
    C_DateTools();
    //--------------------------------- getFreeDays --------------------------------------------------------------------------------
    /*! \brief compute french free days   list from a date to number years after;
     *  \param const QDate   &startDate   date from witch compute free days
     *  \param int            nb_years    number of years after start date to compute
     *  \param const QString &country     if contains "FR" frenchs specifics free days, if contains "CR"  chretians free date
     *  \return QList<QDate> : list of free QDate.
    */
    static QList<QDate> getFreeDays(const QDate &startDate, int nb_years=3, const QString &country="CRFR");

    //-------------------------------- CalculeDiffAge ---------------------------------------------------
    /*! \brief compute age from date birth. Exemple : QString age = CalculeDiffAge(QDtate::fromString("05-06-1956","dd-MM-yyyy");
     *  \param const QDate &dateBirth:   date birth
     *  \param bool bTexte    true/return format is as : "4 years 6 months 3 days" false/return format is as : "4.6.3"
     *  \param int  bAbrege:  0/age is with years month and days "4 years 6 months 3 days" or "4.6.3"    // YMD
     *                        1/age is with years and month only "4 years 6 months"        or "4.6"      // YM
     *                        2/age is with years only           "4 years"                 or "4"        // Y
     *  \return QString   format: if (bTexte==true) 4 years 6 months 3 days   if (bTexte==false) 4.6.3
    */
    static QString CalculeDiffAge(const QDate &dateBirth, bool bTexte=true, int bAbrege=1);

    //-------------------------------- CalculeDiffAge ---------------------------------------------------
    /*! \brief compute age between two dates.
     *  \param QDate * &startDate:   pointer to start date
     *  \param QDate * &endDate:     pointer to end date if zero current date will be used.
     *  \param bool bTexte    true/return format is as : "4 years 6 months 3 days" false/return format is as : "4.6.3"
     *  \param int  bAbrege:  0/age is with years month and days "4 years 6 months 3 days" or "4.6.3"    // YMD
     *                        1/age is with years and month only "4 years 6 months"        or "4.6"      // YM
     *                        2/age is with years only           "4 years"                 or "4"        // Y
     *  \return QString   format: if (bTexte==true) 4 years 6 months 3 days   if (bTexte==false) 4.6.3
    */
    static QString CalculeDiffAge(const QDate *startDate, QDate *endDate =0, bool bTexte=true, int bAbrege=1);

    //--------------------------------- dateOfBirthControl -------------------------------------------------------------
    /*! \brief try to make a valid birth date with string which can bee with polymorph structures:\
     * a date : dd-MM-yy ddMMyy dd-MM-yyyy ddMMyyyy (separator can be any non numeric caracter except '.' ',')\
     * a age  : string less than three numbers as : 35 for 35 years, 101 for 101 years, 10 for 10 years\
     * a age  : string as : aaa.MM.dd   (age.months.days) 0.36.45  age is 36 month 45 days\
     *  \param  QString _dnss   input string from wich date of birth will be extracted
     *  \return  QDate  (can be not valid if no valid date can be extracted from input string)
    */
    static QDate dateOfBirthControl(const QString &_dnss);
};

#endif // C_DATETOOLS_H
