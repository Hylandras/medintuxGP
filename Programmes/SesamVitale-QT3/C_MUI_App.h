/********************************** CDrTuxAppCo.h *********************************
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
#ifndef C_MUI_App_H
#define C_MUI_App_H

#include <qtsingleapplication.h>
//#include "../drtux/src/CDrTuxAppCtx.h"

//............................................. quelques définitions ..........................................................
class QString;


//================================================= C_MUI_App ==============================================================
class C_MUI_App : public QtSingleApplication 
{
    
   Q_OBJECT
    
public:
    C_MUI_App(const QString &appName="", int  argc=0, char ** argv=0);
    int  GotoDebug();
};

//extern CDrTuxAppCo* G_pCDrTuxApp;

#endif            // #define CDRTUXAPP_H
