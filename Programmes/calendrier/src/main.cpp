
#include <qapplication.h>
#include <qcursor.h>
#include <qdatetime.h>
#include <qdialog.h>
#include <qwidget.h>
#include <qstyle.h>
#include <qfile.h>
#include <qmessagebox.h>

#include "ui/Dlg_Calendar.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "CApp.h"

#define  TR	QObject::tr

int main( int argc, char ** argv )
{// débute l'application
 CApp a( argc, argv );

 // ouvre une fenêtre choix de date
  Dlg_Calendar *dlg  = new Dlg_Calendar(0, "Calendar_Dial", FALSE, Qt::WStyle_Customize | Qt::WStyle_NormalBorder | Qt::WStyle_Title |Qt::WStyle_MinMax | Qt::WStyle_SysMenu);
 if (dlg ==0)  return -1;
 dlg->setCaption("Calendrier");
 dlg->show();

 // String pour récupérer les paramètres passés au plugins
 QString date_cur, date_inf,  date_sup;
 QString ret_format = "dd-MM-yyyy";

 //................ Premier paramètre supplémentaire = change la date courante
 if (G_pCApp && G_pCApp->argc() > 13)
   {	date_cur = G_pCApp->argv()[13];
    // un peu de nettoyage de la string date
    date_cur.replace("-",""); date_cur.replace("/",""); date_cur.replace(".","");
    dlg->AdjustInterfaceToDate(QDate ( date_cur.mid(4,4).toInt(),       // année
                       date_cur.mid(2,2).toInt(),       // mois
                       date_cur.mid(0,2).toInt() ) );   // jour
   }

  //............... Deuxième paramètre = format de retour pour la date
  if (G_pCApp && G_pCApp->argc() > 14) { ret_format = G_pCApp->argv()[14];  }

  //.................. Troisième paramètre = date limite inférieure au calendrier
  if (G_pCApp && G_pCApp->argc() > 15)
   {	date_inf = G_pCApp->argv()[15];
    // un peu de nettoyage de la string date
    date_inf.replace("-",""); date_inf.replace("/",""); date_inf.replace(".","");
        QDate dt (date_inf.mid(4,4).toInt(),       // année
                  date_inf.mid(2,2).toInt(),       // mois
                  date_inf.mid(0,2).toInt() );
    dlg->setDateDeb( &dt );   // jour
   }

  //....................... Quatrième paramètre = date limite supérieure au calendrier
  if (G_pCApp && G_pCApp->argc() > 16)
    {	date_sup = G_pCApp->argv()[16];
    // un peu de nettoyage de la string date
    date_sup.replace("-",""); date_sup.replace("/",""); date_sup.replace(".","");
    dlg->setDate(QDate ( date_sup.mid(4,4).toInt(),       // année
                 date_sup.mid(2,2).toInt(),       // mois
                 date_sup.mid(0,2).toInt() ) );   // jour
        QDate dt (date_sup.mid(4,4).toInt(),       // année
                  date_sup.mid(2,2).toInt(),       // mois
                  date_sup.mid(0,2).toInt() );
        dlg->setDateDeb( &dt );   // jour
    }
  //....................... Cinquieme parametre = fichier du menu de preselection
  if (G_pCApp && G_pCApp->argc() > 17)
    {	QString      pathPreselectMenu = G_pCApp->argv()[17];
        QString      listTxt;
        QStringList  itemList;
        if ( QDir::isRelativePath ( pathPreselectMenu ) )  pathPreselectMenu = QDir::cleanDirPath (pathPreselectMenu.prepend(G_pCApp->m_PathAppli) );
        if ( QFile::exists(pathPreselectMenu) )
           {CGestIni::Param_UpdateFromDisk(pathPreselectMenu, listTxt);
            listTxt = listTxt.remove("\r");
            if (listTxt.length())
               { itemList = QStringList::split ("\n", listTxt);
                 dlg->setMenuPreSelectDate(itemList);
               }
           }
        else
           {QMessageBox::information( dlg,"Cela ne va pas du tout",  TR("Le répertoire du menu de présélection des dates est non accessible : ").arg(pathPreselectMenu));
           }
    }
  //....................... Sixieme parametre = mode de fonctionnement days|debDate|endDate|hours
  if (G_pCApp && G_pCApp->argc() > 18)
    {QString mode = G_pCApp->argv()[18];
     dlg->setMode(mode);
    }


 a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
 int ret = a.exec();

 // Sauvegarde le résultat dans le fichier d'échange avec DrTux
 CGestIni::Param_UpdateToDisk(G_pCApp->m_PathExchg, dlg->getDate().toString(ret_format) );
 return ret;
}
