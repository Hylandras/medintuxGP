/***************************** C_UserCtx.h *****************************
*  #include "C_UserCtx.h"        QT4-ready                             *
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

#ifndef C_UserCtx_H
#define C_UserCtx_H
#include <QMap>
#include <QStringList>
#include <QObject>
#include <QDebug>

#include "../univers/C_Universal_Ident.h"
#include "../univers/C_Human_Properties.h"



//=========================================== C_UserCtx =================================================
/*! \class  C_UserCtx
 *  \brief  user prescription context if guid is valid data's will be extract from patient attached to this guid, otherwise input datas will be used
 *  \note   date of birth is sometimes unknown. So it is always possible to determine aproximative or precise age. So when date of birth is unknown,
 *          age is used and date of birth set to invalid date (to indicate that only age is known)
 */

class C_UserCtx:  public C_Human_Ident
{

public:
    C_UserCtx();
    C_UserCtx(const QString &dataXml);
    ~C_UserCtx(){qDebug()<< "C_UserCtx is deleted";}

    void                         unserialize(const QString &dataXml);
    static  C_UserCtx           *unserialize(C_UserCtx *pC_UserCtx, const QString &dataXml);
    void        set_id(QString value)                                   { set_guid(value);}      // numero RPPS
    void        set_id_type(QString value)                              { m_id_type = value;}    // "RPPS"

    void        set_Titre(QString value)                                { m_Titre = value;}
    void        set_Specialite(QString value)                           { m_Specialite = value;}
    void        set_Nu_ordinal(QString value)                           { m_Nu_ordinal = value;}
    void        set_Nu_RPPS(QString value)                              { m_Nu_RPPS = value;}
    void        set_Nu_ClefRPPS(QString value)                          { m_Nu_ClefRPPS = value;}
    void        set_Adresse(QString value)                              { m_Adresse = value;}
    void        set_CodePostal(QString value)                           { m_CodePostal = value;}
    void        set_Ville(QString value)                                { m_Ville = value;}
    void        set_Tel1(QString value)                                 { m_Tel1 = value;}
    void        set_Tel2(QString value)                                 { m_Tel2 = value;}
    void        set_Tel3(QString value)                                 { m_Tel3 = value;}
    void        set_Email(QString value)                                { m_Email = value;}
    void        set_Login(QString value)                                { m_Login = value;}

    QString        get_id_type()        {return                             m_id_type ;}
    QString        get_Titre()          {return                             m_Titre ;}
    QString        get_Specialite()     {return                             m_Specialite;}
    QString        get_Nu_ordinal()     {return                             m_Nu_ordinal ;}
    QString        get_Nu_RPPS()        {return                             m_Nu_RPPS;}
    QString        get_Nu_ClefRPPS()    {return                             m_Nu_ClefRPPS;}
    QString        get_Adresse()        {return                             m_Adresse;}
    QString        get_CodePostal()     {return                             m_CodePostal;}
    QString        get_Ville()          {return                             m_Ville;}
    QString        get_Tel1()           {return                             m_Tel1;}
    QString        get_Tel2()           {return                             m_Tel2;}
    QString        get_Tel3()           {return                             m_Tel3;}
    QString        get_Email()          {return                             m_Email;}
    QString        get_Login()          {return                             m_Login;}
private:

   QString m_id_type;
   QString m_Titre;
   QString m_Specialite;
   QString m_Nu_ordinal;
   QString m_Nu_RPPS;
   QString m_Nu_ClefRPPS;
   QString m_Adresse;
   QString m_CodePostal;
   QString m_Ville;
   QString m_Tel1;
   QString m_Tel2;
   QString m_Tel3;
   QString m_Email;
   QString m_Login;
};

#endif // C_UserCtx_H
