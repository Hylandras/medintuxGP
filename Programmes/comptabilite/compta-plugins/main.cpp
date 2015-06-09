/***************************************************************************
 *   Copyright (C) 2007, 2008 by Eric Maeker   *
 *   eric.maeker@free.fr   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <qapplication.h>
#include <qobject.h>
#include "../comptabilite/ui/FormPlugins.h"
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qstringlist.h> 
#include "CApp_Plug.h"
#include <string.h>
#include <qcstring.h> 
#include <qfileinfo.h> 
#define TR QObject::tr
#define  PLUGAPP

// {{[Execute]          
//                                                               pathPlugin=../../Plug/Compta2/bin/compta-plugins         
//                                                               maskExch=           
//                                                               pathIni=           
//                                                               waitEnd=y          [Parametres]          
//                                                               NomPatient={{NOM PRENOM PATIENT}}          
//                                                               Date={{DATE OBSERVATION}}          
//                                                               Acte=C          
//                                                               Tiers=non          
//                                                               Payement=Chq        }}

int main( int argc, char ** argv )
{ // Construit le pointeur général de l'appli et récupère les données
  int       i          = 0;
  QString ret          = "";
  CApp_Plug a(argc, argv);
  QString pathDebugParam = CGestIni::Construct_PathBin_Module("comptabilite",  QFileInfo(argv[0]).dirPath (true));
  CGestIni::Param_UpdateToDisk("/Applications/MedinTuxV2/Programmes/comptabilite/bin/bug.txt", pathDebugParam );
  pathDebugParam +=  "toTest.txt";
  QStringList G_lstArg ;
  if (argc < 12)
     {QString strToRecord = "";
      CGestIni::Param_UpdateFromDisk(pathDebugParam, strToRecord );
      G_lstArg = QStringList::split("@=#=@\r\n",  strToRecord, TRUE); 
      argc     = G_lstArg.count();    
     }
  else if (argc >= 12)
    {QString    strToRecord = "";
     for ( i=0; i<argc; ++i)
         {G_lstArg.append(argv[i]);
          strToRecord.append(argv[i]+QString("@=#=@\r\n"));
          //CGestIni::Param_UpdateToDisk("C:\\toTest.txt", strToRecord );
          CGestIni::Param_UpdateFromDisk(pathDebugParam, strToRecord );
         }
    }

  if (argc>=12)
     {a.setParam(G_lstArg);
      if (G_pCApp_Plug)
         {QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
              // Lance le plugins de concaténation d'honoraires
          FormPlugins *pForm = new FormPlugins();
          if (pForm)
             { pForm->setParams(  G_pCApp_Plug->m_Param_Patient,
                                  G_pCApp_Plug->m_Param_GUID,
                                  G_pCApp_Plug->m_pUtilisateurs,
                                  G_pCApp_Plug->m_Param_Date,
                                  G_pCApp_Plug->m_Param_Acte,
                                  G_pCApp_Plug->m_Param_Tiers,
                                  G_pCApp_Plug->m_Param_ModePaiement,
                                  G_pCApp_Plug->m_ComptaBase
                               );
               pForm->exec();
               ret = pForm->getMessage();               
               delete pForm;
             }
         }
     CGestIni::Param_UpdateToDisk(G_pCApp_Plug->m_PathExchg, ret );
   }
  else 
  {     QMessageBox b;
        QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
        b.setCaption(TR("ERREUR"));
        b.setText(TR("Cette application est un plugin qui doit être appelé par <br><b>MedinTux</b> selon la documentation fournie. <br />Vous ne pouvez pas démarrrer cette application directement."));
        b.setIcon(QMessageBox::Warning);
        b.adjustSize();
        b.exec();
  }

  return 0;

}
