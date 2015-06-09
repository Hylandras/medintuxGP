/********************************* CFseVitaleApp.cpp ******************************
 *                                                                                *
 *                                                                                *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
 *   E-Mail: dat medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/

#include "CFseVitaleApp.h"
#include "C_Dlg_FseDefine.h"
#include "C_Dlg_GetCodePorteur.h"
#include "../../MedinTuxTools/CGestIni.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli
#include "../../SesamVitale-QT3/CSesam_API_InfoGroupe.h"

#include <qevent.h>
#include <qlineedit.h>
#include <qpixmap.h>
#include <qtextcodec.h>
#include <qeventloop.h>
#include <qdatetime.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
CFseVitaleApp* G_pCFseVitaleApp = 0;  // contiendra l'instance globale de l'application


//================================================= CFseVitaleApp ==============================================================
//-------------------------------------------- CFseVitaleApp ---------------------------------------------
CFseVitaleApp::CFseVitaleApp(int & argc, char ** argv, const QString &appName)
    : QApplication (  argc, argv ) , CFseVitaleAppCtx(argc, argv, appName)
{QTextCodec::setCodecForTr( QTextCodec::codecForName("utf8") );
 m_pCCoolPopup           = 0;
 m_pCVitaleReadThread    = 0;
 m_count                 = 0;

//....................... systheme du theme ..............................................
   if (CGestIni::Param_ReadParam( m_PathAppli + m_NameAppli + ".ini", "Theme", "Path", &m_PathTheme) !=0 )  // zero = pas d'erreur
       { m_PathTheme     =  "../../Themes/Default/";    // valeur par défaut
       }
   if (!QDir(m_PathTheme).exists()) m_PathTheme     =  "../../Themes/Default/";         // valeur par défaut
   if (QDir(m_PathTheme).isRelative()) m_PathTheme.prepend(m_PathAppli);
   m_PathTheme = QDir::cleanDirPath(m_PathTheme) + "/";
   Theme::setPath(m_PathTheme);

 G_pCFseVitaleApp        = this;
}

//-------------------------------------------- initialize --------------------------------------------------
void CFseVitaleApp::Slot_OnTimer()
{m_pCCoolPopup->setText(m_Display + QString::number(m_count) );
 m_count++;
}

//-------------------------------------------- initialize --------------------------------------------------
void CFseVitaleApp::initialize( bool /*activate*/)
{return ;
}

//-------------------------------------------- Run ---------------------------------------------------------
int CFseVitaleApp::Run()
{   connect( this, SIGNAL(lastWindowClosed()), this, SLOT(quit()) );
    // a.connect( &timer, SIGNAL(timeout()), &a, SLOT(Slot_OnTimer()) ); // juste pour créer un évenement toutes les secondes
    //................... affichage des arguments dans une messageBox .......................................
    /*QString text;
    for (int i=0; i<argc(); ++i)
        {text += "Arg["+QString::number(i)+"] : " + QString(argv()[i])+"\r\n";
        }
    QMessageBox::information (0, "voila", text, 1 );
    */
 if ( m_Command == TR("MAKE_FSE"))
    {C_Dlg_FseDefine c_Dlg_FseDefine(this);
     setMainWidget(&c_Dlg_FseDefine);
     c_Dlg_FseDefine.show();
     return exec();
    }
 else if ( m_Command==TR("READ_CONFIG")|| m_Command==TR("READ_CPS")|| m_Command==TR("READ_VITALE"))
    {
     if ( m_Command==TR("READ_CPS"))
        {C_Dlg_GetCodePorteur *dlg = new C_Dlg_GetCodePorteur(0);
         if (dlg == 0)               return 0;
         int ret                   = dlg->exec();
         QString              str  = dlg->lineEditGetCode->text();
         delete dlg;
         if (ret == QDialog::Accepted) m_CodePorteur = str;
         else                          return 0;
        }
     if ( CGestIni::Param_ReadUniqueParam(m_LocalParam, "Sesam-Vitale", "Emulation")=="ok")  // zero = pas d'erreur
       { if ( m_Command==TR("READ_CPS"))
            {QString data_cps   =  LoadVirtualCarteCps();
             if (data_cps.length())
                {C_Cps cps(m_CodePorteur, data_cps);
                 CGestIni::Param_UpdateToDisk(m_PathExchg, cps.Serialize());
                 return 0;
                }
            }
         else if ( m_Command==TR("READ_VITALE"))
            {QString data_Vitale   =  LoadVirtualCarteVitale();
             if (data_Vitale.length())
                {C_Vitale vitale(data_Vitale);
                 CGestIni::Param_UpdateToDisk(m_PathExchg, QString("\r\n")+vitale.Serialize()+"\r\n");
                 //CGestIni::Param_UpdateToDisk("/home/ro/vitale.txt", QString("\r\n")+vitale.Serialize()+"\r\n");
                 return 0;
                }
            }
       }
     QTimer  timer;
     //..................... afficher le popup .......................................................................
     CouCouUp(m_Command);
     //................... on crée un thread de lecture sesam-vitale .................................................
     m_pCVitaleReadThread = new CVitaleReadThread(m_PathExchg, m_Command, m_PathLog, m_PathGalss, m_CodePorteur, m_RessPS, m_RessLecteur);
     m_pCVitaleReadThread->start();                     // on le lance
     //....................... tant que le thread de lecture sesam ...................................................
     //                        n'a pas fini on execute les evenements
     //                        afin de laiser les messages systèmes passer. (expl pour que le popup s'affiche)
     timer.start(1*1000, FALSE);
     while ( m_pCVitaleReadThread->IsThreadRun() )
           { eventLoop()->processEvents ( QEventLoop::WaitForMore );
           }
     m_pCVitaleReadThread->wait();                     // on quitte proprement le thread
     delete m_pCVitaleReadThread;                      // on detruit le thread
     m_pCVitaleReadThread = 0;
     //........................ faire redescendre le popup ...........................................................
     CouCouDown();                            // on intime l'ordre au popup de baisser
     return exec();
    }
 return -1;
}

//-------------------------------------------- LoadVirtualCarteCps --------------------------------------------------
QString CFseVitaleApp::LoadVirtualCarteCps()
{ QString data_CPS = "";
  //................. choisir le fichier image src à insérer .................................................
  QFileDialog *fd = new QFileDialog  ( m_PathImages +"cps/",
                                       tr( "CPS-Files (*.cps);;All Files (*)" ),
                                       0,
                                       "OuvrirCpsDlg"
                                      ) ;
  if (fd==0)                return data_CPS;
  if ( ! (fd->exec() == QDialog::Accepted) )
     {delete fd;            return data_CPS;
     }
  QString fileName = fd->selectedFile();
  delete fd;
  if ( !fileName.isEmpty() && QFile::exists( fileName ))
     {CGestIni::Param_UpdateFromDisk(fileName, data_CPS);
     }
  return data_CPS;
}

//-------------------------------------------- LoadVirtualCarteVitale --------------------------------------------------
QString CFseVitaleApp::LoadVirtualCarteVitale()
{QFileDialog *fd = new QFileDialog  (  m_PathImages +"cartes_virtuelles/",
                                       tr( "Vitale-TestFiles (*.IN *.in *.bin *.BIN);;All Files (*)" ),
                                       0,
                                       "OuvrirCpsDlg"
                                      ) ;
  if (fd==0)            return "";
  if ( ! (fd->exec() == QDialog::Accepted) )
     {delete fd;        return "";
     }
  QString fileName = fd->selectedFile();
  delete fd;
  return LoadVirtualCarteVitale(fileName);
}

//-------------------------------------------- LoadVirtualCarteVitale --------------------------------------------------
QString CFseVitaleApp::LoadVirtualCarteVitale(const QString &fileName)
{ QString data_Vitale          = "" ;
  if ( !fileName.isEmpty() && QFile::exists( fileName ))
     { QFile file(fileName );
       file.open ( IO_ReadOnly );
       int len = file.size();
       if (len==0)                               return data_Vitale;
       unsigned char *pBuffer = new unsigned char[ len +1];                    // array of unsigned char
       if (pBuffer==0)                           return data_Vitale;
       long len_read = file.readBlock ( (char*)pBuffer, len );
       file.close();
       if ( len_read != len ) {delete []pBuffer; return data_Vitale;}
       pBuffer[len]=0;
       QString str = CGestIni::CharStrToQStringUtf8( (char*) pBuffer ).stripWhiteSpace();
       if (str.find("[VIT_GR-101-OCC-1]") != -1)
          {delete []pBuffer;
           return str;
          }

       int len_Groupe;
       int len_champ;
       unsigned short iDGroupeCourant;
       int occur;
       unsigned char *pt_debGroupe  = pBuffer;
       unsigned char *pt            = pt_debGroupe;
       unsigned char *end_file      = pBuffer + len;
       unsigned char *end           = pBuffer + len;


       while (pt_debGroupe < end_file)
         {//......................... rajouter le groupe ...................................
          iDGroupeCourant    = pt_debGroupe[0]; iDGroupeCourant = (iDGroupeCourant<<8) + pt_debGroupe[1];
          occur              = C_Vitale::GetNbOccurences(data_Vitale, iDGroupeCourant) + 1;
          data_Vitale       += QString("[VIT_GR-") + QString::number(iDGroupeCourant)  + "-OCC-" + QString::number(occur) + "]";
          data_Vitale       += TR("\r\n      ; Groupe : ") +
                               QString::number(iDGroupeCourant).rightJustify(4,'0')   +
                               "  (" + CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant) + ")";

          //......................... readapter les pointeurs ...................................
          if (pt_debGroupe[2] & 0x80)   // si bit de poids fort à 1 sauter le bouzin (sais pas pourquoi)
             {len_Groupe         = pt_debGroupe[3];
              pt_debGroupe       = pt_debGroupe + 4;
              data_Vitale       += "  truc : " + QString::number(pt_debGroupe[2] & 0x7F);
             }
          else
             {len_Groupe         = pt_debGroupe[2];
              pt_debGroupe       = pt_debGroupe + 3;
             }
          pt                 = pt_debGroupe;
          end                = pt + len_Groupe;
          int memberIndex    = 1;
          QString value      = "";

          //......................... rajouter les champs ...................................
          while (pt<end)
             {len_champ = pt[0];
              ++pt;               // passer la longueur du champ
              if (len_champ) value = QString::fromLatin1 ( (char*)pt, len_champ);
              else           value = "";
              data_Vitale         += QString("\r\n      m_Num-")  + QString::number(memberIndex) + " = " +
                                     value.replace(",","\\,").replace(";","\\;") + " , " + CSesam_API_InfoGroupe::NumGroupeToString(iDGroupeCourant, memberIndex);
              pt += len_champ;
              ++memberIndex;
             }
         //...................... goupe suivant .............................................
         data_Vitale  += "\r\n\r\n";
         pt_debGroupe += len_Groupe;
         }
       delete []pBuffer;
     }
 return data_Vitale;
}

//-------------------------------------------- GetNumeroFacture --------------------------------------------------
QString CFseVitaleApp::GetNumeroFacture()
{QString data, numero;
 int num = 0;
 CGestIni::Param_UpdateFromDisk(m_PathAppli +  "Ressources/Facture.ini", data);
 CGestIni::Param_ReadParam(data,  "Facture",  "Numero",       &numero);

 num = numero.toInt() + 1;
 if (data.find("[Facture]")==-1) data += "\r\n[Facture]\r\n";
 numero = QString::number(num);
 CGestIni::Param_WriteParam(&data, "Facture",  "Numero",     numero  );
 CGestIni::Param_WriteParam(&data, "Facture",  "LastDate",  (QDate::currentDate()).toString("dd-MM-yyyy")   );

 CGestIni::Param_UpdateToDisk(m_PathAppli +  "Ressources/Facture.ini", data);
 return numero;
}

//-------------------------------------------- sendMessage --------------------------------------------------
bool CFseVitaleApp::sendMessage( const QString &/*message*/, int /*timeout */)
{return 0;
}

//-------------------------------------------- activateMainWidget --------------------------------------------------
void CFseVitaleApp::activateMainWidget()
{
    QWidget *wdg = mainWidget();
    if (wdg==0)    return;
    wdg->raise();
    wdg->setActiveWindow();
}

//-------------------------------------------- stopVitaleThread --------------------------------------------------
void CFseVitaleApp::stopVitaleThread()
{if (m_pCVitaleReadThread) m_pCVitaleReadThread->stop();
}

//-------------------------------------------- CouCouUp --------------------------------------------------
void CFseVitaleApp::CouCouUp(const QString command, QString text)
{   if (text.length()==0) text = m_Display;
    //..................... afficher le popup .......................................................................
    if ( command=="READ_CPS")
       { CouCouUp( m_PathImages + "Carte_CPS.png",                     // Image à afficher
                   9,32,150,17,                                       // coordonnées du rectangle texte d'affichage du message
                   "<span style=\"font-family:Arial;font-size:10pt;font-weight:600;\">"  // message à afficher (html)
                   "<center><b>" +  text + "</b></center></span>",
                   QColor(255,0,0),                // couleur Jaune du texte vitale :    QColor("#F0B332")
                   QColor("#FBDA7E")             // couleur verte de la carte vitale : QColor("#40973F")
              );
       }
    else
       { CouCouUp( m_PathImages + "Vitale01.png",                    // Image à afficher
                   9,32,225,17,                                      // coordonnées du rectangle texte d'affichage du message
                   "<span style=\"font-family:Arial;font-size:10pt;font-weight:600;\">"  // message à afficher (html)
                   "<center><b>" +  text + "</b></center></span>",
                   QColor(255,0,0),              // couleur Jaune du texte vitale :    QColor("#F0B332")
                   QColor("#2FC828")             // couleur verte de la carte vitale : QColor("#40973F")
              );
        }
}
//------------------------------------------------- CouCouUp ----------------------------------------------------------------
void CFseVitaleApp::CouCouUp(const QString& pathImage, int x, int y, int w, int h,const QString &text, const QColor &fore, const QColor &back)
{if (m_pCCoolPopup) delete m_pCCoolPopup;
 m_pCCoolPopup = new CCoolPopup( pathImage , 0, x,  y,  w,  h, text, fore, back);
 m_pCCoolPopup->setIcon(QPixmap(m_PathImages+"QFseVitale.png"));
 connect( m_pCCoolPopup,        SIGNAL( Sign_PopupUp(CCoolPopup *) ),
          this,                 SLOT(CouCouIsUp (CCoolPopup *))
        );
 connect( m_pCCoolPopup,        SIGNAL( Sign_PopupDown(CCoolPopup *) ),
          this,                 SLOT(CouCouStop(CCoolPopup *))
        );
 connect( m_pCCoolPopup,        SIGNAL(Sign_Clicked() ),
          this,                 SLOT(stopVitaleThread())
        );

}

//--------------------------------------- CouCouIsUp ------------------------------------------------------------------------
void CFseVitaleApp::CouCouIsUp(CCoolPopup * )
{//pCCoolPopup->Disappear();      // decommenter cette ligne pour qu'une fois  en haut le menu redescende tout seul
}
//--------------------------------------- CouCouUp ------------------------------------------------------------------------
void CFseVitaleApp::CouCouDown()
{if (m_pCCoolPopup!=0) m_pCCoolPopup->Disappear();
}

//--------------------------------------- CouCouStop ------------------------------------------------------------------------
void CFseVitaleApp::CouCouStop(CCoolPopup * pCCoolPopup)
{if (m_pCCoolPopup==pCCoolPopup)
    {pCCoolPopup->disconnect( SIGNAL(Sign_PopupUp(CCoolPopup *)) );
     pCCoolPopup->disconnect( SIGNAL(Sign_PopupDown(CCoolPopup *)) );
     QTimer::singleShot(0, this, SLOT(CouCouDestroy()) );
    }
}

//--------------------------------------- CouCouDestroy ------------------------------------------------------------------------
void CFseVitaleApp::CouCouDestroy()
{if (m_pCCoolPopup) delete m_pCCoolPopup;
 m_pCCoolPopup = 0;
 if ( m_Command==TR("READ_CONFIG")|| m_Command==TR("READ_CPS")|| m_Command==TR("READ_VITALE"))
    { emit lastWindowClosed();
    }
}


