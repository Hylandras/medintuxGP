/***************************** C_UserCtx.cpp ***************************
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

#include "C_UserCtx.h"
#include "../C_PatientBase/C_PatientBase.h"
#include "../CGestIni.h"
#include <QDebug>
#include <QObject>

//=========================================== C_UserCtx ====================================================
C_UserCtx::C_UserCtx()
{unserialize(this, "");
}

C_UserCtx::C_UserCtx(const QString &dataXml )
{unserialize(this, dataXml);
}

//--------------------------------- unserialize -----------------------------------------------------
void  C_UserCtx::unserialize(const QString &dataXml)
{unserialize(this, dataXml);
}
//--------------------------------- unserialize -----------------------------------------------------
/*
    "   <m_version>1</m_version>\n"
    "   <m_id>{{USER_GUID}}</m_id>\n"
    "   <m_id_type>GUID</m_id_type>\n"
    "   <m_Titre>{{USER_TITRE_PS}}</m_Titre>\n"
    "   <m_Specialite>{{USER_SPECIALITE_MEDECIN}}</m_Specialite>\n"
    "   <m_usual_name>{{USER_NOM_MEDECIN}}</m_usual_name>\n"
    "   <m_forename>{{USER_NOM_PRENOM_MEDECIN}}</m_forename>\n"
    "   <m_sexe>{{USER_SEXE_MEDECIN}}</m_sexe>\n"
    "   <m_Nu_ordinal>{{USER_IDENTIFICATION_MEDECIN}}</m_Nu_ordinal>\n"
    "   <m_Nu_RPPS>{{USER_RPPS}}</m_Nu_RPPS>\n"
    "   <m_Nu_ClefRPPS>{{USER_CLEF_RPPS}}</m_Nu_ClefRPPS>\n"
    "   <m_Adresse>{{USER_ADRESSE_MEDECIN}}</m_Adresse>\n"
    "   <m_CodePostal>{{USER_CP_MEDECIN}}</m_CodePostal>\n"
    "   <m_Ville>{{USER_VILLE_MEDECIN}}</m_Ville>\n"
    "   <m_Tel1>{{USER_TEL_1_MEDECIN}}</m_Tel1>\n"
    "   <m_Tel2>{{USER_TEL_2_MEDECIN}}</m_Tel2>\n"
    "   <m_Tel3>{{USER_TEL_3_MEDECIN}}</m_Tel3>\n"
    "   <m_Email>{{USER_EMAIL_MEDECIN}}</m_Email>\n"
    "   <m_Login>{{USER_LOGIN_MEDECIN}}</m_Login>\n"

 */
C_UserCtx  *C_UserCtx::unserialize(C_UserCtx *obj, const QString &dataXml)
{
    int nextPos = 0;
    //............................. C_UserCtx ..............................
    obj->set_id(                 CGestIni::getXmlData("m_id",                  dataXml, &nextPos));
    obj->set_id_type(            CGestIni::getXmlData("m_id_type",             dataXml, &nextPos));
    obj->set_Titre(              CGestIni::getXmlData("m_Titre",               dataXml, &nextPos));
    obj->set_Specialite(         CGestIni::getXmlData("m_Specialite",          dataXml, &nextPos));
    obj->set_usual_name(         CGestIni::getXmlData("m_usual_name",          dataXml, &nextPos));
    obj->set_forename(           CGestIni::getXmlData("m_forename",            dataXml, &nextPos));
    obj->set_sexe(               CGestIni::getXmlData("m_sexe",                dataXml, &nextPos));
    obj->set_Nu_ordinal(         CGestIni::getXmlData("m_Nu_ordinal",          dataXml, &nextPos));
    obj->set_Nu_RPPS(            CGestIni::getXmlData("m_Nu_RPPS",             dataXml, &nextPos));
    obj->set_Nu_ClefRPPS(        CGestIni::getXmlData("m_Nu_ClefRPPS",         dataXml, &nextPos));
    obj->set_Adresse(            CGestIni::getXmlData("m_Adresse",             dataXml, &nextPos));
    obj->set_CodePostal(         CGestIni::getXmlData("m_CodePostal",          dataXml, &nextPos));
    obj->set_Ville(              CGestIni::getXmlData("m_Ville",               dataXml, &nextPos));
    obj->set_Tel1(               CGestIni::getXmlData("m_Tel1",                dataXml, &nextPos));
    obj->set_Tel2(               CGestIni::getXmlData("m_Tel2",                dataXml, &nextPos));
    obj->set_Tel3(               CGestIni::getXmlData("m_Tel3",                dataXml, &nextPos));
    obj->set_Email(              CGestIni::getXmlData("m_Email",               dataXml, &nextPos));
    obj->set_Login(              CGestIni::getXmlData("m_Login",               dataXml, &nextPos));

    return obj;
}

