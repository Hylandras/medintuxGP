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
#include <qmessagebox.h>
#include <qtextcodec.h>
#include "CApp_Dus.h"

#define TR QObject::tr
#define  PLUGAPP

int main( int argc, char ** argv )
{ // Construit le pointeur général de l'appli et récupère les données
  if (argc >= 12)
  { CApp_Dus a( argc, argv );
    if (G_pCApp_Dus)
     {	QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );

	CGestIni::Param_UpdateToDisk(G_pCApp_Dus->m_PathExchg, G_pCApp_Dus->m_msgToDrTux );
	return 0;
     }
    QString tmp = " ";
    CGestIni::Param_UpdateToDisk(G_pCApp_Dus->m_PathExchg, tmp );
   }
  else 
  { 	QApplication a (argc, argv);
	QMessageBox b;
	QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
	b.setCaption(TR("ERREUR"));
	b.setText(TR("Cette application est un plugins qui doit être appelé par <br><b>MedinTux</b> selon la documentation fournie. <br />Vous ne pouvez pas démarrrer cette application directement."));
	b.setIcon(QMessageBox::Warning);
	b.adjustSize();
	b.exec();
  }
  return 0;

}
