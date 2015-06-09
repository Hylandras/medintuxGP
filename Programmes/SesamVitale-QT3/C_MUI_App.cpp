/********************************** C_MUI_App.cpp *******************************
 *                                                                                *
 *   #include "C_MUI_App.h"                                                     *
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

#include "C_MUI_App.h"

//-------------------------------------- C_MUI_App ------------------------------
C_MUI_App::C_MUI_App(const QString &appName, int  argc, char ** argv)
    : QtSingleApplication(appName, argc, argv)
{
}

//-------------------------------------- GotoDebug ------------------------------
int C_MUI_App::GotoDebug()
{return 0;
}
