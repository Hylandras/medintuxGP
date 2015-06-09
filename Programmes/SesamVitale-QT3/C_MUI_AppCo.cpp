/********************************** CDrTuxAppCo.cpp *******************************
 *                                                                                *
 *   #include "CDrTuxAppCo.h"                                                     *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This source file is not free; you can't redistribute it and/or modify        *
 *   it under the terms of the DataMedicalDesign Licence Version 1 du 12/12/2005  *
 *   E-Mail: data.medical.design@club-internet.fr                                 *                                                                             
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/

#include <qstring.h>
#include <qdir.h>

#include "CDrTuxAppCo.h" 

//CDrTuxAppCo* G_pCDrTuxApp = 0;

//-------------------------------------- C_MUI_AppCo ------------------------------
C_MUI_AppCo::C_MUI_AppCo(const QString &appName, int  argc, char ** argv)
    : QtSingleApplication(appName, argc, argv)
{//G_pCDrTuxApp = this;
}

