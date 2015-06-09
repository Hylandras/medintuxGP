/***************************** C_DateTools.cpp ****************************
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

#include "C_DateTools.h"
#include <QDate>
#include <QString>
#include <QList>
#include <QtCore/qmath.h>
#include <QObject>
#include <QMessageBox>

//-------------------------------- C_DateTools ---------------------------------------------------
C_DateTools::C_DateTools()
{
}
//--------------------------------- getFreeDays --------------------------------------------------------------------------------
QList<QDate> C_DateTools::getFreeDays(const QDate &deb, int nb_years, const QString &country /* ="CRFR"*/)
{   QList<QDate>   listeJoursFeries;
   --nb_years;    // base zero
   for (int i=0; i<nb_years; i++)     // on calcule les dates pour 3 ans
       {int     an  = deb.year() + i;
        QString san = QString::number(an);
        listeJoursFeries << QDate::fromString(san + "-01-01","yyyy-MM-dd");   // 1er an
        if (country.toUpper().contains("FR"))
           { listeJoursFeries << QDate::fromString(san + "-05-01","yyyy-MM-dd");   // 1er mai
             listeJoursFeries << QDate::fromString(san + "-05-08","yyyy-MM-dd");   // vict 1945
             listeJoursFeries << QDate::fromString(san + "-11-11","yyyy-MM-dd");   // armis 1918
             listeJoursFeries << QDate::fromString(san + "-07-14","yyyy-MM-dd");   // bastille
           }
        if (country.toUpper().contains("CR"))
           {
            listeJoursFeries << QDate::fromString(san + "-08-15","yyyy-MM-dd");   // assomption
            listeJoursFeries << QDate::fromString(san + "-11-01","yyyy-MM-dd");   // toussaint
            listeJoursFeries << QDate::fromString(san + "-12-25","yyyy-MM-dd");   // noel

            int G = an%19;
            int C = qFloor(an / 100);
            int H = (C - qFloor(C/4) - qFloor( (8*C+13)/25) + 19*G + 15)%30 ;
            int I = H - qFloor(H/28) * (1- qFloor(H/28) * qFloor(29 / (H+1)) * qFloor((21-G)/11));
            int J = (an*1 + qFloor(an/4) + I + 2 - C + qFloor(C/4)) %7 ;
            int L = I - J;
            int moisPaques = 3 + qFloor( (L+40) / 44);
            int jourPaques = L + 28 - 31*qFloor(moisPaques/4);
            QString sm = QString::number(moisPaques);
            if (moisPaques < 10) sm = "0"+sm;
            QString sj = QString::number(jourPaques);
            if (jourPaques < 10) sj = "0"+sj;
            QDate datePaque = QDate::fromString(san + "-" + sm + "-" + sj,"yyyy-MM-dd");
            listeJoursFeries << datePaque;                                // paques
            listeJoursFeries << datePaque.addDays(1);                     // lundi paques
            listeJoursFeries << datePaque.addDays(39);                    // ascension
            listeJoursFeries << datePaque.addDays(49);                    // pentecote
           }
       } // fin for an
   // for (int i=0;i<listeJoursFeries.size(); i++)
   //     qDebug()<< listeJoursFeries.at(i).toString("dd-MM-yyyy");
   return listeJoursFeries;
}
//-------------------------------- CalculeDiffAge ---------------------------------------------------
/*! \brief compute age from date birth. Exemple : QString age = CalculeDiffAge(QDtate::fromString("05-06-1956","dd-MM-yyyy");
 *  \param const QDate &dateBirth:   date birth
 *  \param bool bTexte    true/return format is as : "4 years 6 months 3 days" false/return format is as : "4.6.3"
 *  \param int  bAbrege:  0/age is with years month and days "4 years 6 months 3 days" or "4.6.3"    // YMD
 *                        1/age is with years and month only "4 years 6 months"        or "4.6"      // YM
 *                        2/age is with years only           "4 years"                 or "4"        // Y
 *  \return QString   format: if (bTexte==true) 4 years 6 months 3 days   if (bTexte==false) 4.6.3
*/
// USAGE QString age = CalculeDiffAge(dateNss);
QString C_DateTools::CalculeDiffAge(const QDate &pDebut, bool bTexte/*=true*/, int bAbrege/*=1*/)
{
    return C_DateTools::CalculeDiffAge(&pDebut,0,  bTexte,  bAbrege);
}

//-------------------------------- CalculeDiffAge ---------------------------------------------------
/*! \brief compute age between two dates.
 *  \param QDate * &startDate:   pointer to start date
 *  \param QDate * &endDate:     pointer to end date if zero current date will be used.
 *  \param bool bTexte    true/return format is as : "4 years 6 months 3 days" false/return format is as : "4.6.3"
 *  \param int  bAbrege:  0/age is with years month and days "4 years 6 months 3 days" or "4.6.3"    // YMD
 *                        1/age is with years and month only "4 years 6 months"        or "4.6"      // YM
 *                        2/age is with years only           "4 years"                 or "4"        // Y
 *                        3/age is in number of month only   123                                     // M
 *  \return QString   format: if (bTexte==true) 4 years 6 months 3 days   if (bTexte==false) 4.6.3
*/
// USAGE QString age = CalculeDiffAge(&dateNss);
QString C_DateTools::CalculeDiffAge(const QDate *pDebut, QDate *pFin/*=0*/, bool bTexte/*=true*/, int bAbrege/*=1*/)
{
 char finMois[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
 int an=0;
 int mois=0;
 int jour=0;
 int dayYearDebut;
 int dayYearFin;
 QString buffer;
 QString sRes("");
 bool bFinAlloue = false;
 long lEtatDayYear;

 if (pDebut && pDebut->isValid ())
    {
     if (pFin==NULL)
        {pFin       = new QDate;
         *pFin      = QDate::currentDate ();
         bFinAlloue = true;
        }//if

     if ( pFin->isValid())
        {
         //.........................................Partie calcul ..........................................
         //........Le jour de l'annee .................
         dayYearDebut = pDebut->dayOfYear ();
         dayYearFin   = pFin->dayOfYear ();

         //................................ bisextiles and cie ..................................................
         // ATTENTION A L'ANNEE BISEXTILLE (sauf en l'an 1900, car les debuts de siecles ne sont pas bisextille sauf les debuts de millenaire
         // 60eme jour => 29 Fevrier
         if (pDebut->year()%4==0 && pDebut->year()%100 && pDebut->year()%1000    &&  dayYearDebut>60 )
             dayYearDebut--;
         if (pFin->year()%4==0   && pFin->year()%100   && pFin->year()%1000      &&  dayYearFin>60)
             dayYearFin--;

          //....................Position des jours l'un par rapport a l'autre ....................................
          if      (dayYearFin<dayYearDebut)     lEtatDayYear = -1;
          else if (dayYearFin>dayYearDebut)     lEtatDayYear =  1;
          else                                  lEtatDayYear =  0;

          //...................Calcul annee (-1 si le jour de l'annee en cours est <) .............................
          an = pFin->year() - pDebut->year() - (lEtatDayYear<0? 1 :0);

          if (lEtatDayYear!=0)
             {
              //.......................Calcul mois .....................................
              if (lEtatDayYear<0)
                  mois = 12 - pDebut->month() + pFin->month()   - (pFin->day()<pDebut->day()? 1 :0);
              else
                  mois = pFin->month()        - pDebut->month() - (pFin->day()<pDebut->day()? 1 :0);

              //...................... Calcul du jour ...................................
              if (pFin->day() < pDebut->day())
                  jour=(finMois[pFin->month()>1 ? pFin->month()-2 : 11] - pDebut->day()) + pFin->day();
              else
                  jour=pFin->day() - pDebut->day();
             } //if(lEtatDayYear!=0)

          //.............................................. Partie texte .........................................
          if (bAbrege==3)
             {int nbM = (an*12) + mois;
              return QString::number(nbM);
             }
          if (an)
             {
              if (bTexte)
                  //buffer.sprintf("%d an%s ", an, an>1 ? "s" : "");
                  buffer = QString::number(an) + QObject::tr(" an") + ((an>1) ? "s " : " ");
              else
                  buffer.sprintf("%d ", an);
              sRes = buffer;
             } //if (an)
          //........................ MODE ABREGE 2  si bAbrege==2 alors on ne sort que les annees .......................................
          if (bAbrege==2)
             {if (an==0) sRes="0";
              if(bFinAlloue)  delete pFin;
              return sRes;
             }
          //........................ MODE NON ABREGE 1 (que les mois) 0 (les jours aussi)....................................................
          else
             {
              if (mois)
                 {
                  if (bTexte)       // 4 ans 6 mois
                     {QString ms = QObject::tr(" mois");            // va bien y avoir un dialecte ou ca ne va pas marcher
                      buffer     = QString::number(mois)+ ms + ((mois>1&&ms[4]!='s') ? "s " : " ");
                     }
                  else
                     {buffer.sprintf(".%d ", mois);
                     }
                  sRes += buffer;
                } // if (mois)

              if (jour && bAbrege==0)
                 {
                  if (bTexte)
                      //buffer.sprintf("%d jour%s ", jour, jour>1 ? "s" : "");
                      buffer = QString::number(jour) + QObject::tr(" jour") + ((jour>1) ? "s " : " ");
                  else
                      buffer.sprintf(".%d ", jour);
                  sRes += buffer;
                 } // if  (jour || !bTexte)
              }// if (!bAbrege || an<3) //Inferieure a 3 ans
          }// if (pFin->m_status==COleDateTime::valid)
     }// if (pDebut && pDebut->m_status==COleDateTime::valid)
     if(bFinAlloue)  delete pFin;
     return sRes;
}

//--------------------------------- dateOfBirthControl -------------------------------------------------------------
/*! \brief try to make a valid birth date with string which can bee with polymorph structures:\
 * a date : dd-MM-yy ddMMyy dd-MM-yyyy ddMMyyyy (separator can be any non numeric caracter except '.' ',')\
 * a age  : string less than three numbers as : 35 for 35 years, 101 for 101 years, 10 for 10 years\
 * a age  : string as : aaa.MM.dd  (separator can be '.' ',') (age.months.days) 0.36.45  age is 36 month 45 days\
 *  \param  QString _dnss   input string from wich date of birth will be extracted
 *  \return  QDate  (can be not valid if no valid date can be extracted from input string)
*/
QDate C_DateTools::dateOfBirthControl(const QString &_dnss)
{   QDate dt;
    QString dnss =_dnss;
    if (dnss.length()==0) return dt;
    QStringList dnssList;

    //........... format (aaa) (aaa.mm.jj) ..............
    dnss = dnss.replace(',','.');
    if (dnss.length()<=3 || dnss.contains('.'))
       { dt       = QDate::currentDate();
         dnssList = dnss.split('.'); dnssList<<""<<""<<"";
         if (dnssList[0].toInt())  dt  = dt.addYears( -dnssList[0].toInt());
         if (dnssList[1].toInt())  dt  = dt.addMonths(-dnssList[1].toInt());
         if (dnssList[2].toInt())  dt  = dt.addDays(  -dnssList[2].toInt());
         return dt;
       }
    //........... format jjmmaaaa ou jjmmaa..............
    //            exemple 060656 ou 05061956
    //            on ajoute un separateur
    if (dnss.length()==6 || dnss.length()==8)
       {dnss = dnss.left(2)+"-"+dnss.mid(2,2)+"-"+dnss.mid(4);
       }
    //........... format jj/mm/aaaa ..............
    QString sep = "";
    for (int i=0; i<dnss.length();++i)
        { if ( !(dnss[i]>='0'&& dnss[i]<='9') ) {sep=dnss[i]; break;}
        }
    dnssList = dnss.split(sep); dnssList<<""<<""<<"";
    dnss     = dnss.remove(sep);
    //.................. completer le siecle si besoin..........................
    if (dnssList[0].toInt() && dnss.length()==6 && dnssList[0].length()==2 && dnssList[1].length()==2 && dnssList[2].length()==2)
       {   QString dnss20 = dnssList[0]+"-"+dnssList[1]+"-20"+dnssList[2];
           QString dnss19 = dnssList[0]+"-"+dnssList[1]+"-19"+dnssList[2];
           dt  = QDate::fromString(dnss20,"dd-MM-yyyy");   // on part sur l'an 2000
           if ( dt < QDate::currentDate() )
              { int ret = QMessageBox::warning ( 0   , QObject::tr("date of birth:"),
                                                       QObject::tr ( "<u><b>WARNING :</u></b><br/>"
                                                                     "&nbsp;&nbsp;&nbsp;The year is exprimed with only two numbers.<br/>"
                                                                     "&nbsp;&nbsp;&nbsp;The date can be <b>%1</b> or <b>%2</b> <br/>"
                                                                     "&nbsp;&nbsp;&nbsp;please choose the correct date of birth.").arg(dnss20,dnss19),
                                                       dnss20, dnss19, 0,
                                                       1, 1
                                               );
                if (ret==0) dnss = dnss20;
                else        dnss = dnss19;
              }
           else
              { dnss = dnss19;
              }
           dt  = QDate::fromString(dnss,"dd-MM-yyyy");
       }
    else
       {   dt  = QDate::fromString(dnss,"ddMMyyyy");
       }
    return dt;
}
