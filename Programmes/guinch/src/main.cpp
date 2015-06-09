/***************************************************************************
 *   Copyright (C) 2005 by Sevin Roland                                    *
 *   roland-sevin@medintux.org                                             *
 *                                                                         *
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

#include <QProcess>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

//#include "../../MedinTuxTools-QT4/CGestIni.h"

#ifdef Q_OS_MACX
#define PATH_SPEC_MAC "../../../"
#define ARCHI_DEP_MAC ".app/Contents/MacOS/" + module
#else
#define PATH_SPEC_MAC ""
#define ARCHI_DEP_MAC ""
#endif
#ifdef Q_WS_WIN
#define F_EXT  ".exe"
#else
#define F_EXT ""
#endif

QString Construct_Name_Exe(QString module, QString start_Argv, const QString &alternateExecName ="" );

//-------------------------------------------------------------------------- main -------------------------------------------------------------------------------------
int main( int argc, char ** argv )
{   //................ Lancer le process .................................................................
    QString pathExe  = Construct_Name_Exe("drtux", QFileInfo (argv[0]).path ());
    int i=1 ;
    QStringList argList;
    //......................... completer les arguments de lancement.........................................
    while ( i<argc)
        { argList <<(argv[i]);
          ++i;
        }
    bool ret = QProcess::startDetached (pathExe, argList);
    // if (ret == 0) qDebug() << "ok";
    return 0;
}

//------------------------------ Construct_Name_Exe -----------------------------------------
/*! \brief construit le chemin vers l'executable du module (module) de MendinTux. Gère la compatibilité Windows/Linux/Mac.
*/
QString Construct_Name_Exe(QString module, QString start_Argv, const QString &alternateExecName /*="" */)
{
/*      Signature :  String         X String                              --> String
                     nom du module  X Chemin complet de demarrage du proc --> nom  ini complet du module
    Permet la construction d'un nom complet de fichier executable en =
relation avec le module correspondant
*/

        QString path;
        QString exeName;
        if (alternateExecName.length()) exeName = alternateExecName;
        else                            exeName = module;
        //Création du nom de fichier
       if (module=="compta-plugins" || module=="check_dus"  || module=="comptabilite" )
          { path   = start_Argv + "/" + PATH_SPEC_MAC + "../../comptabilite/bin/" + exeName + F_EXT;
            module = exeName;
            path  +=  ARCHI_DEP_MAC;
          }
       else
          { path   = start_Argv + "/" + PATH_SPEC_MAC + "../../"+module+"/bin/"  + exeName + F_EXT;
            module = exeName;
            path  +=  ARCHI_DEP_MAC;
          }
        path = QDir::cleanPath (path);
        return path;

}
