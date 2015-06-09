/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <qfiledialog.h>
#include <qsqldriver.h>
#include <qmessagebox.h>
//---------------------------------------- init ------------------------------------------------
void Dlg_Patch::init()
{m_pCTheraBase   = 0;
 m_pCMoteurBase  = 0;
 m_pCMedicaBase  = 0;
 m_AutoTerminate = 0;
 m_Test          = "";

 textEdit_infos->hide();
 //...................... recuperer action ....................................................
 m_Action        = "";
 if (qApp->argc()>=2)  m_Action = QString(qApp->argv()[1]);
 //...................... recuperer chemin de l'application ....................................
 QFileInfo qfi(qApp->argv()[0]);                    //qApp->argv()[i]
 m_PathAppli    = CGestIni::Construct_PathBin_Module("mise_a_jour",QFileInfo (qApp->argv()[0]).dirPath (true));
 m_PatchParam   = "";
 m_PathDrTux    = CGestIni::Construct_PathBin_Module("drtux", QFileInfo (qApp->argv()[0]).dirPath (true));
 textLabelInfo->setText(tr("Bienvenue sur la mise à jour de MedinTux"));
 setIcon( m_PathAppli + "Ressources/Patch.png" );
 if (m_Action.left(5).lower() == "debug")         // ATTENTION il y a aussi 'Tester base' donc comparer 'test ' avec l'espace separateur
    {m_Action = m_Action.mid(5).stripWhiteSpace();
     m_Test   = "ok";
     textEdit_infos->show();
     pushButtonOk->show();
    }
 else
    {pushButtonOk->hide();
     pushButtonAnnuler->hide();
    }
 // m_Action = "Do Update";
 // m_Test   = "ok";

 QString val;
 QString VersionTxt = CGestIni::Construct_Name_File_Ini( "mise_a_jour",QFileInfo (qApp->argv()[0]).dirPath (true),"Ressources/Version.txt");
 // QString log;
 // CGestIni::Param_UpdateFromDisk(m_PathAppli + "Ressources/Log.txt", log);
 // log += QString (" Action :" ) + m_Action + "\n";
 // CGestIni::Param_UpdateToDisk(m_PathAppli + "Ressources/Log.txt", log);
 //................ action selon param d'entrée ..............................
 if (m_Action == "Do Update")
    {QString              err  = InitCMoteurBase();
     if (err.length()==0) err  = InitCMedicaBase();
     if (err.length()==0)
        {if ( m_Test   != "ok" )
           {QTimer::singleShot( 2000, this, SLOT(pushButtonOk_clicked()) );
            m_AutoTerminate = 2;              // interface non affihée
           }
         else
           {m_AutoTerminate = 1;              // on affiche l'interface
            pushButtonOk->show();
            pushButtonOk_clicked();
           }
            // interface affichée
        //CGestIni::Param_UpdateToDisk(m_PathAppli + "Ressources/Version.txt", "ok");
        CGestIni::Param_UpdateToDisk(VersionTxt, "ok");
        //m_pCMoteurBase->GotoDebug();
       }
     else
       {textLabelInfo->setText(tr("Les Bases de données n'ont pas pu être "
                                  "initialisées correctement.<br>"
                                  "Veuillez vérifier leur installation et à "
                                  " éventuellement reinstaller toutes les bases de données."
                                  ) );
        //pushButtonAnnuler->show();
        //CGestIni::Param_UpdateToDisk(m_PathAppli + "Ressources/Version.txt", "Erreur 00");
        CGestIni::Param_UpdateToDisk(VersionTxt, "Erreur 00");
        Sortir();
       }
    }
if (m_Action == "GetBasesList")
    {if ( m_Test   != "ok" )
        {QTimer::singleShot( 400, this, SLOT(Sortir()) );
         m_AutoTerminate = 2;              // interface non affihée
        }
     else
        {m_AutoTerminate = 1;              // on affiche l'interface
        }
     val = "";
     CGestIni::Param_UpdateToDisk(VersionTxt, val);  // recuperer le fichier d'echange
     QString err = InitCMoteurBase();
     if (err.length()==0)
        { Get_Bases_List(val);
          if ( m_Test   == "ok" ) textEdit_infos->setText(textEdit_infos->text()+tr("\nRésultat :\n'")+val+"'\n");
        }
     else
        {val           = "Erreur Get Bases List() initialisaton de la base non possible, installation des bases non correcte \n" + err;
         if ( m_Test   == "ok" ) textEdit_infos->setText(val+"\n");
        }
     textLabelInfo->setText(tr("Bienvenue sur la mise à jour de MedinTux ") + val);
     CGestIni::Param_UpdateToDisk(VersionTxt, val);
    }
 else if (m_Action=="MSDEV")
    {if ( m_Test   != "ok" )
        {QTimer::singleShot( 400, this, SLOT(Sortir()) );
         m_AutoTerminate = 2;              // interface non affihée
        }
     else
        {m_AutoTerminate = 1;              // on affiche l'interface
        }
     QString mess;
     QString path = QString(qApp->argv()[2]).stripWhiteSpace();
     if (QDir(path).isRelative()) path.prepend(m_PathAppli);
     path = QDir::cleanDirPath(path);
     if (!QFile::exists(path))
        {mess += tr("\nFichier non trouvé : ") + path ;
        }
     if (path.length())
        {CGestIni::Param_UpdateFromDisk(path, val);
         textEdit_infos->setText(mess+"\n Chemin : "+path);
         //int pos = val.find("!MESSAGE NMAKE /f \"");
         //if (pos != -1)
            {QString name = qApp->argv()[3];//val.mid(pos + 19);
             name = name.stripWhiteSpace();
             //name = name.replace(".mak\"." ,".exe");
             int pos  = val.find("# ADD LINK32");
             if (pos != -1)
                {int end = pos + 12;
                 while(end<val.length() && val[end] != '\r' && val[end] != '\n') ++end;
                 if (end<val.length())
                    {val = val.insert(end," /out:\"../bin/" + name);
                     pos = end + 13;
                    }
                }
             pos  = val.find("# ADD LINK32", pos);
             if (pos != -1)
                {int end = pos + 12;
                 while(end<val.length() && val[end] != '\r' && val[end] != '\n') ++end;
                 if (end<val.length())
                    {val = val.insert(end," /out:\"../bin/" + name);
                    }
                }
             CGestIni::Param_UpdateToDisk(path, val);
            }
        }
      else textEdit_infos->setText(mess+"\n Chemin non trouve : "+path);
    }
 else if (m_Action=="Get Version")
    {if ( m_Test   != "ok" )
        {QTimer::singleShot( 400, this, SLOT(Sortir()) );
         m_AutoTerminate = 2;              // interface non affihée
        }
     else
        {m_AutoTerminate = 1;              // on affiche l'interface
        }
     val = "";
     CGestIni::Param_UpdateToDisk(VersionTxt, val);
     QString err = InitCMoteurBase();
     if (err.length()==0)
        { m_pCMoteurBase->GetMedinTuxVersion(val);
        }
     else
        {val           = "Erreur Get Version() initialisaton de la base non possible, installation des bases non correcte \n" + err;
         if ( m_Test   == "ok" ) textEdit_infos->setText(val+"\n");
        }
     textLabelInfo->setText(tr("Bienvenue sur la mise à jour de MedinTux ") + val);
     CGestIni::Param_UpdateToDisk(VersionTxt, val);
    }
 else if (m_Action=="Set Datasemp Version")
       {m_AutoTerminate = 2;  // interface non affichée
        val = qApp->argv()[2];
        QString err = InitCMedicaBase();
        if (err.length()==0)
           {SetDatasempIndexVersion(val);
           }
        textLabelInfo->setText(tr("Bienvenue sur la mise à jour de MedinTux ") + val);
        CGestIni::Param_UpdateToDisk(VersionTxt, val);
        QTimer::singleShot( 400, this, SLOT(Sortir()) );
        }

 else if (m_Action=="Get Datasemp Version")
        {m_AutoTerminate = 2;  // interface non affichée
         val             = "0.00";
         QString     err = InitCMedicaBase();
         if (err.length()==0)
           {val = GetDatasempIndexVersion();
           }
         textLabelInfo->setText(tr("Bienvenue sur la mise à jour de MedinTux ") + val);
         CGestIni::Param_UpdateToDisk(VersionTxt, val);
         QTimer::singleShot( 400, this, SLOT(Sortir()) );
        }
 else if (m_Action=="Tester SQL Connexion")
       {if ( m_Test   != "ok" )
           {QTimer::singleShot( 400, this, SLOT(Sortir()) );
            m_AutoTerminate = 2;              // interface non affihée
           }
        else
           {m_AutoTerminate = 1;              // on affiche l'interface
           }
        val = "0.00";
        testBase(val);
        QString pass = qApp->argv()[5];
        if (pass=="_=OO=_") pass = "";
        textLabelInfo->setText(tr("Connexion à la base : ") +
                               qApp->argv()[2] +"::" +      // driver
                               qApp->argv()[3] +"::" +      // base
                               qApp->argv()[4] +"::" +      // user
                               pass            +"::" +      // pass
                               qApp->argv()[6] +"::" +      // host
                               qApp->argv()[7] +"::" +      // port
                               val                          // numero de version
                              );
        CGestIni::Param_UpdateToDisk(VersionTxt, val);
        if ( m_Test   == "ok" ) textEdit_infos->setText(val+"\n");
        }
 else
        {QString err               = InitCMoteurBase();
         if (err.length()==0) err  = InitCMedicaBase();
         if (err.length()==0)
                {pushButtonAnnuler->show();
                 pushButtonOk->show();
                 textLabelInfo->show();
                 textEdit_infos->show();
                 m_pCMoteurBase->GetMedinTuxVersion(val);
                 textLabelInfo->setText(tr("Bienvenue sur la mise à jour de MedinTux ") + val);
                 CGestIni::Param_UpdateToDisk(VersionTxt, val);
                }
        else
                {textLabelInfo->show();
                 textEdit_infos->show();
                 textLabelInfo->setText(tr("Les Bases de données n'ont pas pu être "
                                  "initialisées correctement.<br>"
                                  "Veuillez vérifier leur installation et à "
                                  "éventuellement toutes les reinstaller."
                                  ) );
                 pushButtonAnnuler->show();
               }
        }
  //................... infos des paramètres si debug mode ..........................
  if (m_Test   == "ok")
     {QString info = tr("========= messages ==========\r\n") + textEdit_infos->text()+tr("\r\n========= arguments ==========");
      for (int i = 0; i<qApp->argc(); ++i)
          {info += tr("\r\nArgument ") + QString::number(i) + " = \t\t" +qApp->argv()[i];
          }
      info += tr("\r\n========= Action ========= \r\n")  + "m_Action = \t\t" + m_Action;
      textEdit_infos->setText(info);
      textLabelInfo->setText(tr("Bienvenue sur la mise à jour de MedinTux (")+m_Action + ") " + QString::number(m_AutoTerminate));
    }
 else
    {textLabelInfo->setText(tr("Bienvenue sur la mise à jour de MedinTux"));
    }
}

//---------------------------------------- Get_Bases_List ------------------------------------------------
void Dlg_Patch::Get_Bases_List(QString &val)
{QSqlDriver *pQSqlDriver =  m_pCMoteurBase->m_DataBase->driver();
 if ( pQSqlDriver == 0)
    {textEdit_infos->setText(tr("pushButtonOk_clicked() :  Driver non instancié"));
     val           = tr("Erreur Get Bases List() Driver non instancié \n");
     return ;
    }
 if (m_pCMoteurBase->OpenBase()==0)
    {textEdit_infos->setText(tr("pushButtonOk_clicked() :  Driver non instancié"));
     val           = tr("Erreur Get Bases List() la base n'a pa pu être ouverte \n");
     return ;
    }
 QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);
 QString requete("SHOW DATABASES");
 query.exec(requete);
 if (query.isActive())
    {val="";
     while (query.next())  val += query.value(0).toString() +"|";                    // on a la liste des utilisateurs signataires
    } //endif (pSqlQuery && pSqlQuery->isActive())
  m_pCMoteurBase->CloseBase();
}

//---------------------------------------- pushButtonOk_clicked ------------------------------------------------
void Dlg_Patch::pushButtonOk_clicked()
{
 QString val;

//.................. renommer correctement les tables .....................................
//                   (ces foutus dump de windows ne respectent pas la casse)
m_pCMoteurBase->RenameTableWhithCorrectName();  // sous windows quelque soit le nom passe (majuscules ou minuscules) tout reste en minuscules !!
//double  val_ref  = QString("2.14").toDouble();   // il existe des erreurs d'approximation du style 2.13999999   !!!


QSqlDriver *pQSqlDriver =  m_pCMoteurBase->m_DataBase->driver();
if ( pQSqlDriver == 0) {textEdit_infos->setText(tr("pushButtonOk_clicked() :  Driver non instancié")); return ;}


//..................... verifier si les tables de la base indiquees par le fichier de configuration ..................
//                      est la bonne base en verifiant si elle comporte la table m_DOSS_RUB_HEAD_TBL_NAME
//                      car on peut tres bien indiquer dans le .ini une base bidon donc si elle existe la connexion
//                      se fera, et le N° de version incremental restera toujours à zero car pourra pas etre mis à jour donc PB
QStringList tablesList      = pQSqlDriver->tables("1");                    tablesList.sort();
#ifdef Q_WS_WIN
if (tablesList.findIndex (m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME.lower() )==-1)
   {QString message = tr("pushButtonOk_clicked() :  La table '%1' \n"
                         "n'est pas présente dans la liste des tables\n"
                         "vérifiez si vous pointez sur la bonne base : \n"
                         "'%2' indiquée dans les paramètres du 'drtux.ini'\n"
                         "section : [Connexion] après les labels : Nomade et Master\n"
                         "est celle qu'il faut."
                        ).arg(m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME, m_pCMoteurBase->m_DataBase->databaseName());
    textEdit_infos->setText(message);
    QMessageBox::critical (this, tr("ERREUR DE LA MISE A JOUR DE MEDINTUX"), message, QMessageBox::Abort , QMessageBox::NoButton, QMessageBox::NoButton );
    return ;
   }
#else
if (tablesList.findIndex (m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME )==-1)
   {QString message = tr("pushButtonOk_clicked() :  La table '%1' \n"
                         "n'est pas présente dans la liste des tables\n"
                         "vérifiez si vous pointez sur la bonne base : \n"
                         "'%2' indiquée dans les paramètres du 'drtux.ini'\n"
                         "section : [Connexion] après les labels : Nomade et Master\n"
                         "est celle qu'il faut."
                        ).arg(m_pCMoteurBase->m_DOSS_RUB_HEAD_TBL_NAME, m_pCMoteurBase->m_DataBase->databaseName());
    textEdit_infos->setText(message);
    QMessageBox::critical (this, tr("ERREUR DE LA MISE A JOUR DE MEDINTUX"), message, QMessageBox::Abort , QMessageBox::NoButton, QMessageBox::NoButton );
    return ;
   }
#endif


m_pCMoteurBase->GetMedinTuxVersion(val);   // fait plus grande valeur de  "SELECT NumVers FROM version "
int lastVersion   = normaliseVersion(val);
int resultVersion = 0;
while (lastVersion < 216000)
 {//...................................................................... V0 ==> V1 .......................................
  /*
  if (lastVersion == QString("0").toDouble())
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     textLabelInfo->setText(tr("MedinTux version : 0 vers version 1.00 en cours ..."));
     //...................... copier les nouveaux fichiers de configuration ................................................
     //                       pour que la base de donnée puisse etre initialisée
     CopieDataBaseCfg("DataBase.cfg");
     //.................. créer la table des versions.............................................
     if (m_pCMoteurBase->OpenBase()==0)  return;
     QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);

     //.................. créer la table de version .............................................
     query.exec("CREATE TABLE IF NOT EXISTS version ("
                "`NumVers` varchar(32) NOT NULL default ''"
                ") TYPE=MyISAM;");
     //.................. créer les nouveaux champs.............................................
     query.exec("INSERT INTO `version` VALUES (\"1.00\");");
     query.exec("ALTER TABLE `RubriquesBlobs` ADD `RbDate_IDDos` varchar(36);");
     query.exec("ALTER TABLE `RubriquesBlobs` ADD INDEX `RbDate_BlobsIDDosIndex`(`RbDate_IDDos`);");
     query.exec("ALTER TABLE `RubriquesHead`  ADD `RbDate_RefBlobs_PrimKey` bigint;");
     m_pCMoteurBase->ConvertToNew(textLabelInfo, progressBar);
     m_pCMoteurBase->CloseBase();

     m_pCMoteurBase->GetMedinTuxVersion(val);
     setCaption(tr("MedinTux version : ")+val);

     //................... créer le numéro de version Datasemp de MedicaBase ..............................
     //                    sert à verifier si l'index de MedicaBase est cohérent avec Datasemp
     if (m_pCMedicaBase->m_DataGet_Base->isOpen()==FALSE && m_pCMedicaBase->m_DataGet_Base->open()==FALSE)  return;

     QSqlQuery query_m(QString::null, m_pCMedicaBase->m_DataGet_Base);
     query_m.exec("CREATE TABLE IF NOT EXISTS `version` ("
                  "`DatasempVersion` varchar(32) NOT NULL default ''"
                  ") TYPE=MyISAM;"
                 );
     m_pCMedicaBase->OutSQL_error( query_m);
     query_m.exec("INSERT INTO `version` VALUES (\"110B02.2\");");
     m_pCMedicaBase->m_DataGet_Base->close();
     //............. verifions si l'upgrade est passe ..............................
     if (isUpgradeOk(resultVersion , lastVersion , "V0 => V1")==0) return;
    }
 */
 //...................................................................... V1 ==> V1.2 .......................................
 if ( lastVersion < 120000 ) /////////////////////////////////////////////////////////////////// V 1.20 //////////////////////////////////
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     textLabelInfo->setText(tr("MedinTux version : 1 vers version 1.2 en cours ..."));
     //..................................Ouverture base patient ........................................
     if ( ! (m_pCMoteurBase->OpenBase()==0))
        {QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);
         query.exec("INSERT INTO `version` VALUES (\"1.20\");");
         //   UPDATE UserDoc SET `NameDoc` = 'Parametres' WHERE `NameDoc`= 'Paramètres';
         query.exec(QString(" UPDATE ") + m_pCMoteurBase->m_USER_DATA_TBL_NAME +" SET `" +
                    m_pCMoteurBase->m_USER_DATA_NAMEDOC + "` = 'Parametres' WHERE `"     +  m_pCMoteurBase->m_USER_DATA_NAMEDOC + "` = 'Param�res' ;");
         query.exec(QString(" UPDATE ") + m_pCMoteurBase->m_USER_DATA_TBL_NAME +" SET `" +
                    m_pCMoteurBase->m_USER_DATA_NAMEDOC + "` = 'Parametres' WHERE `"     +  m_pCMoteurBase->m_USER_DATA_NAMEDOC + "` = 'Paramètres' ;");

         m_pCMoteurBase->CloseBase();
        }

     //..................................Ouverture  base therapeutique GET........................................
     if (m_pCMedicaBase->m_DataBaseSrce && ! (m_pCMedicaBase->m_DataBaseSrce->isOpen()==FALSE && m_pCMedicaBase->m_DataBaseSrce->open()==FALSE))
        {QSqlQuery query(QString::null, m_pCMedicaBase->m_DataBaseSrce);
         query.exec("ALTER TABLE `Posologies`       MODIFY  `Divers` varchar(128);");
         query.exec("ALTER TABLE `Posologies_Perso` MODIFY  `Divers` varchar(128);");

         m_pCMedicaBase->m_DataBaseSrce->close();
        }

     //..................................Ouverture  base therapeutique DATASEMP........................................
     if (m_pCMedicaBase->m_DataGet_Base && ! (m_pCMedicaBase->m_DataGet_Base->isOpen()==FALSE && m_pCMedicaBase->m_DataGet_Base->open()==FALSE))
        {QSqlQuery query(QString::null, m_pCMedicaBase->m_DataGet_Base);
         query.exec("ALTER TABLE `Posologies`       MODIFY  `Divers` varchar(128);");
         query.exec("ALTER TABLE `Posologies_Perso` MODIFY  `Divers` varchar(128);");

         m_pCMedicaBase->m_DataGet_Base->close();
        }
     //............. verifions si l'upgrade est passe ..............................
     if (isUpgradeOk(resultVersion , lastVersion , "V1 => V1.20")==0) return;
    }
 if (lastVersion < 125000 ) /////////////////////////////////////////////////////////////////// V 1.25 //////////////////////////////////
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     textLabelInfo->setText(tr("MedinTux version : 1 vers version 1.25 en cours ..."));
     //..................................Ouverture base patient ........................................
     if ( ! (m_pCMoteurBase->OpenBase()==0))
        {QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);
         query.exec("INSERT INTO `version` VALUES (\"1.25\");");
         query.exec("ALTER TABLE `Personnes`        MODIFY  `Droits` varchar(200);");
         //......................... rajout du sexe dans Personnes .........................................................
         //   UPDATE UserDoc SET `NameDoc` = 'Parametres' WHERE `NameDoc`= 'Paramètres';
         //   ALTER TABLE 'Personnes'  ADD 'Sexe' varchar(2) default ''  AFTER 'Droits'
         QString requete = QString(" ALTER TABLE ") + m_pCMoteurBase->m_USER_IDENT_TBL_NAME + " ADD `Sexe` varchar(2) default '' AFTER `"     +  m_pCMoteurBase->m_USER_IDENT_DROITS + "` ;";
         query.exec(requete);
         //......................... rajout du sexe dans Personnes .........................................................
         requete = QString(" ALTER TABLE ") + m_pCMoteurBase->m_USER_IDENT_TBL_NAME + " ADD `" +
                           m_pCMoteurBase->m_USER_IDENT_SEXE + "` varchar(2) default '' AFTER `"     +  m_pCMoteurBase->m_USER_IDENT_DROITS + "` ;";

         //................................. rajout des champs Utilisateur signataire........................................
         query.exec(requete);
         query.exec("ALTER TABLE `Verrous` ADD  `SignUserName` varchar(32) default '' AFTER `UserName`;");
         query.exec("ALTER TABLE `RubriquesHead` ADD  `RbDate_CreateSignUser` varchar(32) default '' AFTER `RbDate_CreateUser`;");
         //................................. création de la table de gestion des utilisateurs signataires ....................
         requete = QString(" CREATE TABLE `") + m_pCMoteurBase->m_USER_PERMS_TBL_NAME + "` (  `" +
                    m_pCMoteurBase->m_USER_PERMS_PK + "` bigint(20) unsigned NOT NULL auto_increment,  `" +
                    m_pCMoteurBase->m_USER_PERMS_SIGN_GUID+"` varchar(36) default NULL,  `" +
                    m_pCMoteurBase->m_USER_PERMS_FRIEND_GUID+"` varchar(36) default NULL,  `" +
                    m_pCMoteurBase->m_USER_PERMS_FRIEND_DROITS+"` varchar(200) default NULL, `" +
                    m_pCMoteurBase->m_USER_PERMS_FRIEND_BLOBS+"` BLOB ,  PRIMARY KEY(`"+m_pCMoteurBase->m_USER_PERMS_PK+"`),"+
                    "  KEY `user_perms_Index` (`"+m_pCMoteurBase->m_USER_PERMS_FRIEND_GUID+"`) )TYPE=MyISAM;";

         query.exec(requete);
         //..................... rajouter tous les droits aux utilisateurs afin que MedinTux fonctionne comme avant ................................................
         AddDroitsToAllUser(query, "-sgn-orv-orc-orp-orr-bmc-ccc-acc-cic-lac-obv-obc-obp-glc-gld-etc-ata-atv-atc-ttv-ttc-tvv-tvc", 1);   // 1 pour table  'Personnes'
         //.................................. copier les bonnes définitions de tables tenant compte des rajouts et création ..............
         CopieDataBaseCfg("DataBase.cfg");
         //................................... modifier la table  des
         m_pCMoteurBase->CloseBase();
        }
     //............. verifions si l'upgrade est passe ..............................
     if (isUpgradeOk(resultVersion , lastVersion , "V1.20 => V1.25")==0) return;
    }
 if ( lastVersion < 126000 ) /////////////////////////////////////////////////////////////////// V 1.26 //////////////////////////////////
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     textLabelInfo->setText(tr("MedinTux version : 1.25 vers version 1.26 en cours ..."));
     //..................................Ouverture base patient ........................................
     if ( ! (m_pCMoteurBase->OpenBase()==0))
        {QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);
         //......................... placer le numéro de version .........................................................
         query.exec("INSERT INTO `version` VALUES (\"1.26\");");
         //......................... Ralonger les droits .........................................................
         query.exec("ALTER TABLE `Personnes`        MODIFY  `Droits` varchar(200);");
         query.exec("ALTER TABLE `" + m_pCMoteurBase->m_USER_PERMS_TBL_NAME +"` MODIFY  `" + m_pCMoteurBase->m_USER_PERMS_FRIEND_DROITS + "` varchar(200);");
         //..................... rajouter les droit -agc-agv-idv-idc-idg aux utilisateurs ................................................
         AddDroitsToAllUser(query, "-agc-agv-idv-idc-idg",3);                                                 // 3 pour table  'Personnes' et 'perms_user'
         m_pCMoteurBase->CloseBase();
        }
     //.................................... copier le fichier de config pour modifier la base  des medoc et compte des Durees conseillees et de la secabilite ...........................
     QString dst    =  CGestIni::Construct_PathBin_Module("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
     QString src    =  m_PathAppli + "Ressources/Vers-1.26/MedicaBase.cfg";
     CHtmlTools::Copy_File(src, dst);
     //..................................... ouvrir les bases therapeutiques ................................................................
     if (m_pCTheraBase==0) m_pCTheraBase   = new (CTheraBase);
     if (m_pCTheraBase && m_pCTheraBase->m_AllIsOk)
        {if (m_pCTheraBase->m_DatasempIndexBase->isOpen()==TRUE && m_pCTheraBase->m_DatasempIndexBase->open()==TRUE)
            {QSqlQuery queryVidal(QString::null, m_pCTheraBase->m_DatasempIndexBase);
             queryVidal.exec("ALTER TABLE `Posologies` ADD  `Pendant` float default NULL AFTER `Divers`;");
             queryVidal.exec("ALTER TABLE `Posologies_Perso` ADD  `Pendant` float default NULL AFTER `Divers`;");
             queryVidal.exec("ALTER TABLE `Posologies` ADD  `Secabilite` char(1) default '' AFTER `Pendant`;");
             queryVidal.exec("ALTER TABLE `Posologies_Perso` ADD  `Secabilite` char(1) default '' AFTER `Pendant`;");
             m_pCTheraBase->m_DatasempIndexBase->close();
            }
         if (m_pCTheraBase->m_DataIndexGet_Base->isOpen()==TRUE && m_pCTheraBase->m_DataIndexGet_Base->open()==TRUE)
            {QSqlQuery   queryGet(QString::null, m_pCTheraBase->m_DataIndexGet_Base);
             queryGet.exec("ALTER TABLE `Posologies` ADD  `Pendant` float default NULL AFTER `Divers`;");
             queryGet.exec("ALTER TABLE `Posologies_Perso` ADD  `Pendant` float default NULL AFTER `Divers`;");
             queryGet.exec("ALTER TABLE `Posologies` ADD  `Secabilite` char(1) default '' AFTER `Pendant`;");
             queryGet.exec("ALTER TABLE `Posologies_Perso` ADD  `Secabilite` char(1) default '' AFTER `Pendant`;");
             m_pCTheraBase->m_DataIndexGet_Base->close();
            }
        }
     //............. verifions si l'upgrade est passe ..............................
     if (isUpgradeOk(resultVersion , lastVersion , "V1.25 => V1.26")==0) return;
    }
 if ( lastVersion <= 205000 ) /////////////////////////////////////////////////////////////////// V 2.00 //////////////////////////////////
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     textLabelInfo->setText(tr("MedinTux vers version 2.12 en cours ..."));
     QString     v;

     //..................................Ouverture base patient ........................................
     if ( ! (m_pCMoteurBase->OpenBase()==0))
        {QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);
         //......................... placer le numéro de version .........................................................
         query.exec("INSERT INTO `version` VALUES (\"2.12\");");
         //......................... Ralonger les droits .........................................................
         query.exec("ALTER TABLE `RubriquesHead`  ADD     `RbDate_SubTypeRub` varchar(50) default NULL AFTER `RbDate_NomDate`;");
         query.exec("ALTER TABLE `RubriquesHead`  MODIFY  `RbDate_NomDate` varchar(50);");
         query.exec("ALTER TABLE `RubriquesBlobs` MODIFY  `RbDate_DataRub` longblob;");

         //..................... rajouter les droit -doc-obc-orc aux utilisateurs ................................................
         AddDroitsToAllUser(query, "-doc-obc-orc",3);                                       // 3 pour table  'Personnes' et 'perms_user'
         //........................... creer les données utilisateur (parametres fenetres et documents type) de la rubrique Documents .....................................
         QStringList qstringList;
         QString     text;
         QString     pk_record;
         QString     us_pk;
         QString     doc_pk;

         QString      str = "=={{<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\"font-size:10pt;font-family:Sans\">\n"
                            "<p align=\"center\"><span style=\"font-style:italic;font-size:8pt\">Imprimé en date du : {{dd-MM-yyyy}} document "
                            "provenant du dossier de {{TITRE PATIENT}} {{NOM PRENOM PATIENT}}</span></p>\n</body></html>\n}}==/r/n=={{}}==/r/n";

         QString requete("SELECT ");
                 requete       += m_pCMoteurBase->m_USER_IDENT_LOGIN + " FROM " + m_pCMoteurBase->m_USER_IDENT_TBL_NAME
                               +  " WHERE " + m_pCMoteurBase->m_USER_IDENT_DROITS
                               +  " LIKE '%-sgn%' ORDER BY " + m_pCMoteurBase->m_USER_IDENT_LOGIN;
         query.exec(requete);
         if (query.isActive())
            {qstringList.clear();
             while (query.next())  qstringList.append(query.value(0).toString() );                    // on a la liste des utilisateurs signataires
            } //endif (pSqlQuery && pSqlQuery->isActive())
         m_pCMoteurBase->CloseBase();
         QStringList::Iterator it;
         for (  it = qstringList.begin(); it != qstringList.end(); ++it )        // pour tous des utilisateurs signataires ajouter aux parametres les elements
             {text = m_pCMoteurBase->GetUserDocument(*it,
                                                     tr("Parametres"),
                                                     TYP_PARAM,        // TYP_ORDONNANCE 2  TYP_CERTIFICAT 3 TYP_OBSERVATION 4 TYP_OBSERVATION  5 TYP_PARAM
                                                     0,                // adresse de la chaine où l'on veut recuperer le pied de page
                                                     0,                // 0 par defaut (RECTO) 1 si l'on veut le DUPLICATA (que pour TYP_ORDONNANCE)
                                                     &pk_record);      // = 0;
              if (text.length())
                 {if (CGestIni::Param_ReadParam(text ,"Documents", "Montrer", &v) !=0)
                     {text += "\r\n\r\n[Documents]\r\n  Zoom = 10\r\n  Montrer = Normal\r\n  Position =  110 , 40 , 600 , 200\r\n";
                     }
                  m_pCMoteurBase->UpdateUserDataFromPrimKey( pk_record, 0, 0 , 0,  text);
                 }
             }
        for (  it = qstringList.begin(); it != qstringList.end(); ++it )        // pour tous des utilisateurs signataires ajouter les documents impression
            { us_pk   = m_pCMoteurBase->GetUserPrimKey(*it);
              if (us_pk.length())
                 {doc_pk  = m_pCMoteurBase->IsThisUserDocExist(  TYP_CERTIFICAT, us_pk, "[Documents] * Documents" );
                  if (doc_pk.length()==0) m_pCMoteurBase->CreateUserDataFromPrimKey(TYP_CERTIFICAT, us_pk , "[Documents] * Documents",  str);
                 }
            }
     //query.exec("INSERT INTO `version` VALUES (\"2.00\");");            //             RubHead
     //query.exec("INSERT INTO `version` VALUES (\"2.00\");");            //             RubBlob
     //query.exec("INSERT INTO `version` VALUES (\"2.00\");");            //             userBlobs
     // RUBREC_LIST          rubList;
        }
     //.................................... corriger le fichier de config  drtux.ini ...........................
     QString outParam;
     QString dst;
     QString src;
     dst    =  CGestIni::Construct_PathBin_Module("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
     CGestIni::Param_UpdateFromDisk(dst+"drtux.ini", outParam );
     outParam.replace (tr("Mode de création dossier"),   "Mode de creation dossier");
     outParam.replace (tr("Mode de cr?tion dossier"),    "Mode de creation dossier");
     outParam.replace (tr("Mode de crï¿½tion dossier"),  "Mode de creation dossier");

     outParam.replace (tr("Accéder"),   "Acceder");
     outParam.replace (tr("Acc?er"),    "Acceder");
     outParam.replace (tr("Accï¿½er"),  "Acceder");

     outParam.replace (tr("Vérifier dossier"),   "Verifier dossier");
     outParam.replace (tr("V?ifier dossier"),    "Verifier dossier");
     outParam.replace (tr("Vï¿½ifier dossier"),  "Verifier dossier");

     outParam.replace (tr("Défaut"),   "Defaut");
     outParam.replace (tr("D?aut"),    "Defaut");
     outParam.replace (tr("Dï¿½aut"),  "Defaut");

     outParam.replace (tr("Vérification Verrou"), "Verification Verrou");
     outParam.replace (tr("V?ification Verrou"),  "Verification Verrou");
     outParam.replace (tr("Vï¿½ification Verrou"),  "Verification Verrou");

     CGestIni::Param_WriteParam( &outParam, "MenuContextuel", "ListDocuments", "Gestion des documents");
     CGestIni::Param_WriteParam( &outParam, "MenuContextuel", "ListImages",    "Gestion des images");
     if (CGestIni::Param_ReadParam(outParam ,"Documents", "MaxImageWidth", &v) != 0)
        {outParam += "\r\n\r\n[Documents]\r\n"
                     " Path documents = /home\r\n"
                     " MaxImageWidth = 550\r\n";
        }
     outParam.replace (tr("Masques du terrain"), "Terrain");
     outParam.replace (tr("Glossaire/Masques/"), "Glossaire/Observation/");
     CGestIni::Param_UpdateToDisk(dst+"drtux.ini", outParam );

     //.................................... corriger le fichier de config  Manager.ini ...........................
     dst    =  CGestIni::Construct_PathBin_Module("Manager",QFileInfo (qApp->argv()[0]).dirPath (true));
     CGestIni::Param_UpdateFromDisk(dst+"Manager.ini", outParam );
     outParam.replace (tr("Mode de création dossier"),   "Mode de creation dossier");
     outParam.replace (tr("Mode de cr?tion dossier"),    "Mode de creation dossier");
     outParam.replace (tr("Mode de crï¿½tion dossier"),  "Mode de creation dossier");

     outParam.replace (tr("Accéder"),   "Acceder");
     outParam.replace (tr("Acc?er"),    "Acceder");
     outParam.replace (tr("Accï¿½er"),  "Acceder");

     outParam.replace (tr("Vérifier dossier"),   "Verifier dossier");
     outParam.replace (tr("V?ifier dossier"),    "Verifier dossier");
     outParam.replace (tr("Vï¿½ifier dossier"),  "Verifier dossier");

     outParam.replace (tr("Défaut"),   "Defaut");
     outParam.replace (tr("D?aut"),    "Defaut");
     outParam.replace (tr("Dï¿½aut"),  "Defaut");

     outParam.replace (tr("Vérification Verrou"),   "Verification Verrou");
     outParam.replace (tr("V?ification Verrou"),    "Verification Verrou");
     outParam.replace (tr("Vï¿½ification Verrou"),  "Verification Verrou");

     CGestIni::Param_WriteParam( &outParam, "MenuContextuel", "ListDocuments", "Gestion des documents");
     CGestIni::Param_WriteParam( &outParam, "MenuContextuel", "ListImages",    "Gestion des images");
     if (CGestIni::Param_ReadParam(outParam ,"Documents", "MaxImageWidth", &v) != 0)
        {outParam += "\r\n\r\n[Documents]\r\n"
                     " Path documents = /home\r\n"
                     " MaxImageWidth = 550\r\n";
        }
     outParam.replace (tr("Masques du terrain"), "Terrain");
     outParam.replace (tr("Glossaire/Masques/"), "Glossaire/Observation/");
     CGestIni::Param_UpdateToDisk(dst+"Manager.ini", outParam );
     //.................................... copier les bons le fichier de DataBase.cfg ...........................
     dst    =  CGestIni::Construct_PathBin_Module("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
     src    =  m_PathAppli + "Ressources/Vers-2.12/drtux/DataBase.cfg";
     CHtmlTools::Copy_File(src, dst);

     dst    =  CGestIni::Construct_PathBin_Module("Manager",QFileInfo (qApp->argv()[0]).dirPath (true));
     src    =  m_PathAppli + "Ressources/Vers-2.12/Manager/DataBase.cfg";
     CHtmlTools::Copy_File(src, dst);
     //....................................... copier les bouts de glossaires utiles ......................................
     // outParam devrait contenir le bon fichier ini
     // dst      devrait contenir le bon chemin de manager
     if (CGestIni::Param_ReadParam(outParam ,"Glossaire", "Path", &val) ==0)              // recuperer path du Glossaire dans le .ini
        {if ( QDir::isRelativePath ( val ) ) val  = QDir::cleanDirPath (val.prepend( dst) ); // le derelativiser
         QDir dir( val );
         dir.rename( val + "/Masques du terrain",          val + "/Terrain" );
         dir.rename( val + "/Masques",                     val + "/Observation" );
         dir.rename( val + tr("/Prescriptions médicales"), val + "/Prescription" );
         dir.mkdir ( val + "/Documents");
         src    =  m_PathAppli + "Ressources/Vers-2.12/Glossaire";
         CHtmlTools::Copy_Dir(src,  val);
        }
     //............. verifions si l'upgrade est passe ..............................
     if (isUpgradeOk(resultVersion , lastVersion , "vers V2.12")==0) return;
    }
 if ( lastVersion < 213000 ) /////////////////////////////////////////////////////////////////// V 2.13 //////////////////////////////////
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     textLabelInfo->setText(tr("MedinTux vers version 2.13 en cours ..."));
     QString     v;
     //..................................Ouverture base patient ........................................
     if ( ! (m_pCMoteurBase->OpenBase()==0))
        {QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);
         qDebug(query.lastError().text());
         //......................... placer le numéro de version .........................................................
         query.exec("INSERT INTO `version` VALUES (\"2.13\");");
         //......................... inserer les champs manquants .........................................................
         //                          truc de fou : une des table � convertir dans une base avait un  RbDate_Date = 0000-05-28 00:00:00
         //                          donc invalide qui bloquait et empechait la requete de se realiser
         query.exec("ALTER TABLE `RubriquesHead`        ADD     `RbDate_Fin`                 datetime              NULL   AFTER `RbDate_CreateSignUser`;");        // qDebug(query.lastError().text());
         query.exec("ALTER TABLE `RubriquesHead`        ADD     `RbDate_DureeMod`            bigint(20)   default  NULL   AFTER `RbDate_Fin`;");                   // qDebug(query.lastError().text());
         query.exec("ALTER TABLE `RubriquesHead`        ADD     `RbDate_Prop_0`              varchar(40)  default  NULL   AFTER `RbDate_DureeMod`;");              // qDebug(query.lastError().text());
         query.exec("ALTER TABLE `RubriquesHead`        ADD     `RbDate_Prop_1`              varchar(40)  default  NULL   AFTER `RbDate_Prop_0`;");                // qDebug(query.lastError().text());
         query.exec("ALTER TABLE `RubriquesHead`        ADD     `RbDate_Prop_2`              varchar(40)  default  NULL   AFTER `RbDate_Prop_1`;");                // qDebug(query.lastError().text());
         query.exec("ALTER TABLE `RubriquesHead`        ADD     `RbDate_Prop_3`              varchar(40)  default  NULL   AFTER `RbDate_Prop_2`;");                // qDebug(query.lastError().text());
         query.exec("ALTER TABLE `RubriquesHead`        ADD     `RbDate_Prop_4`              varchar(40)  default  NULL   AFTER `RbDate_Prop_3`;");                // qDebug(query.lastError().text());
         query.exec("ALTER TABLE `fchpat_Intervenants`  ADD     `fchpat_Intervenants_Type`   char(50)     default  NULL   AFTER `fchpat_Intervenants_IntervPK`;"); // qDebug(query.lastError().text());
         query.exec("ALTER TABLE `fchpat`               ADD     `FchPat_Geme`                char(2)      default  NULL   AFTER `FchPat_RefPk`;");                 // qDebug(query.lastError().text());

         //................................. création de la table de gestion des utilisateurs signataires ....................
         QString requete = QString(" CREATE TABLE `");
         requete += m_pCMoteurBase->m_EVNT_LIST_TBL_NAME  + "` (             `" +
                    m_pCMoteurBase->m_EVNT_LIST_PK        +"` bigint(20) unsigned NOT NULL auto_increment,  `" +
                    m_pCMoteurBase->m_EVNT_DOSS_PK        +"` bigint(20)          NOT NULL default '0',     `" +
                    m_pCMoteurBase->m_EVNT_LIST_PERE      +"` bigint(20) unsigned NOT NULL default '0',     `" +
                    m_pCMoteurBase->m_EVNT_LIST_PKDOC     +"` bigint(20) unsigned NOT NULL default '0',  PRIMARY KEY(`" + m_pCMoteurBase->m_EVNT_LIST_PK + "`) )TYPE=MyISAM;";
         query.exec(requete);

         m_pCMoteurBase->CloseBase();
        }
              //....................................... copier les bouts de glossaires utiles ......................................
     // outParam devrait contenir le bon fichier ini
     // dst      devrait contenir le bon chemin de manager
     QString outParam;
     QString src;
     QString dst;
          //.................................... copier les bons le fichier de DataBase.cfg ...........................
     dst    =  CGestIni::Construct_PathBin_Module("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
     src    =  m_PathAppli + "Ressources/Vers-2.13/drtux/DataBase.cfg";
     CHtmlTools::Copy_File(src, dst);

     dst    =  CGestIni::Construct_PathBin_Module("Manager",QFileInfo (qApp->argv()[0]).dirPath (true));
     src    =  m_PathAppli + "Ressources/Vers-2.13/Manager/DataBase.cfg";
     CHtmlTools::Copy_File(src, dst);

     dst    =  CGestIni::Construct_PathBin_Module("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
     CGestIni::Param_UpdateFromDisk(dst+"drtux.ini", outParam );
     if (CGestIni::Param_ReadParam(outParam ,"Glossaire", "Path", &val) ==0)              // recuperer path du Glossaire dans le .ini
        {if ( QDir::isRelativePath ( val ) ) val  = QDir::cleanDirPath (val.prepend( dst) ); // le derelativiser
         QDir dir( val );
         dir.mkdir ( val + "/Identification");
         dir.mkdir ( val + "/Vigie");
         src    =  m_PathAppli + "Ressources/Vers-2.13/Glossaire";
         CHtmlTools::Copy_Dir(src,  val);
         CHtmlTools::Copy_Dir(m_PathAppli + "Ressources/Vers-2.13/drtux/Ressources/icones",  dst + "Ressources/icones");
         dir.mkdir (dst + "Ressources/Vigie");
         CHtmlTools::Copy_Dir(m_PathAppli + "Ressources/Vers-2.13/drtux/Ressources/Vigie",   dst + "Ressources/Vigie");
        }
         m_pCMoteurBase->TesteAndCorrigeDossIndexPkFromHeaders();
     //............. verifions si l'upgrade est passe ..............................
     if (isUpgradeOk(resultVersion , lastVersion , "vers V2.13")==0) return;
    }
 if ( lastVersion < 214000 ) /////////////////////////////////////////////////////////////////// V 2.14 //////////////////////////////////
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     textLabelInfo->setText(tr("MedinTux vers version 2.14 en cours ..."));
     QString     v;
     //..................................Ouverture base patient ........................................
     if ( ! (m_pCMoteurBase->OpenBase()==0))
        {QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);
         qDebug(query.lastError().text());
         //................... mise à jour SQL ...............................................................
         QString sql_txt;
         CGestIni::Param_UpdateFromDisk(m_PathAppli + "Ressources/Vers-2.14/DrTuxTestUpdates.sql", sql_txt );
         QStringList queryList = QStringList::split(";",sql_txt);
         for (int i=0; i<queryList.size(); ++i)
             { sql_txt = queryList[i];
               query.exec(sql_txt);
               qDebug(query.lastError().text());
             }
         //................... droits .................................................
         if (QFile::exists(m_PathAppli + "Ressources/Vers-2.14/droitsToAdd.txt"))
            {QString droitsToAdd;
             CGestIni::Param_UpdateFromDisk(m_PathAppli + "Ressources/Vers-2.14/droitsToAdd.txt", droitsToAdd );
             AddDroitsToAllUser(query, droitsToAdd, 3);
            }
         m_pCMoteurBase->CloseBase();
        }

     //............. verifions si l'upgrade est passe ..............................
     if (isUpgradeOk(resultVersion , lastVersion , "vers V2.14")==0) return;
    }
 if ( lastVersion < 215000 ) /////////////////////////////////////////////////////////////////// V 2.15 //////////////////////////////////
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     textLabelInfo->setText(tr("MedinTux vers version 2.15.000 en cours ..."));
     QString     v;
     //..................................Ouverture base patient ........................................
     if ( ! (m_pCMoteurBase->OpenBase()==0))
        {QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);
         qDebug(query.lastError().text());
         //................... mise à jour SQL ...............................................................
         QString sql_txt;
         CGestIni::Param_UpdateFromDisk(m_PathAppli + "Ressources/Vers-2.15/DrTuxTestUpdates.sql", sql_txt );
         QStringList queryList = QStringList::split(";",sql_txt);
         for (int i=0; i<queryList.size(); ++i)
             { sql_txt = queryList[i];
               query.exec(sql_txt);
               qDebug(query.lastError().text());
             }
         //................... droits .................................................
         if (QFile::exists(m_PathAppli + "Ressources/Vers-2.15/droitsToAdd.txt"))
            {QString droitsToAdd;
             CGestIni::Param_UpdateFromDisk(m_PathAppli + "Ressources/Vers-2.15/droitsToAdd.txt", droitsToAdd );
             AddDroitsToAllUser(query, droitsToAdd, 3);
            }
         m_pCMoteurBase->CloseBase();
        }

     //............. verifions si l'upgrade est passe ..............................
     if (isUpgradeOk(resultVersion , lastVersion , "vers V2.15")==0) return;
    }
 if ( lastVersion < 216000 ) /////////////////////////////////////////////////////////////////// V 2.15 //////////////////////////////////
    {QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
     textLabelInfo->setText(tr("MedinTux vers version 2.16.000 en cours ..."));
     QString     v;
     //..................................Ouverture base patient ........................................
     if ( ! (m_pCMoteurBase->OpenBase()==0))
        {QSqlQuery query(QString::null, m_pCMoteurBase->m_DataBase);
         qDebug(query.lastError().text());
         //................... mise à jour SQL ...............................................................
         QString sql_txt;
         CGestIni::Param_UpdateFromDisk(m_PathAppli + "Ressources/Vers-2.16/DrTuxTestUpdates.sql", sql_txt );
         QStringList queryList = QStringList::split(";",sql_txt);
         for (int i=0; i<queryList.size(); ++i)
             { sql_txt = queryList[i];
               query.exec(sql_txt);
               qDebug(query.lastError().text());
             }
         //................... droits .................................................
         if (QFile::exists(m_PathAppli + "Ressources/Vers-2.16/droitsToAdd.txt"))
            {QString droitsToAdd;
             CGestIni::Param_UpdateFromDisk(m_PathAppli + "Ressources/Vers-2.16/droitsToAdd.txt", droitsToAdd );
             AddDroitsToAllUser(query, droitsToAdd, 3);
            }
         m_pCMoteurBase->CloseBase();
        }

     //............. verifions si l'upgrade est passe ..............................
     if (isUpgradeOk(resultVersion , lastVersion , "vers V2.16")==0) return;
    }
 } // end while
//////////////////////////////////////////////////////////////////////////////////////////////////////
//.............................. fichiers locaux .....................................................
//////////////////////////////////////////////////////////////////////////////////////////////////////
//................ localiser le repertoire actuel du medintux en cours ...............................
m_pCMoteurBase->GotoDebug();
m_medinTuxDir    = lineEdit_MedinTuxFolder->text().stripWhiteSpace();
if (m_medinTuxDir.length()==0||m_medinTuxDir=="/")
   {pushButton_FindInstallation_clicked();
    if (m_medinTuxDir.length()==0||m_medinTuxDir=="/") return;
   }
if (!m_medinTuxDir.endsWith("/")) m_medinTuxDir += "/";

//............... mettre a jour le glossaire avec les differentes versions ..................
//                trouvees dans le repertoire de mise a jour
QString            lval = "";
QString         gloPath = "";
double           lvLst  = 0.0;
double              lv  = GetMedinTuxGlossaireVersion(lval, &gloPath);
QStringList versionList = CGestIni::listDirectory(m_PathAppli + "Ressources/", "", "Vers-",";",FALSE, TRUE);
for (int i = 0; i<versionList.count(); ++i)
    {QString tmp = versionList[i];
     lvLst       = toDouble(tmp.remove("Vers-"));  // on passe par tmp car le remove modifierai versionList[i] dont on a besoin en version non modifiee plus loin
     if (lv<lvLst)
        {CHtmlTools::Copy_Dir(m_PathAppli + "Ressources/"+versionList[i]+"/Glossaire",  gloPath);
         CGestIni::Param_UpdateToDisk( gloPath+"/Version-"+QString::number(lvLst), QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss") );
         lv     = GetMedinTuxGlossaireVersion(lval, 0);
        }
    }
                                                // 3 pour table  'Personnes' et 'perms_user'

//................ recuperer les    parametres de connexion originaux .................................
lv = GetMedinTuxLocalVersion(lval);
if (lv < QString("2.14").toDouble())
   {//.............. placer les anciens parametres de connexion en cours dans la nouvelle archive ..............................
    setConnexionParam("Manager",1);
    setConnexionParam("drtux",1);
    setConnexionParam("personnes",0);
    setConnexionParam("comptabilite",0);
    setConnexionParam("gest_user",0);
    setConnexionParam("med_stat",0);
    setConnexionParam("ccamview",0);
    setConnexionParam("qgetdatasemp",0);
    setConnexionParam("QLaboFTP",0);
    setConnexionParam("qmedicabase",0);
    CGestIni::Param_UpdateToDisk( m_medinTuxDir+"Version-2.14", QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss") );
   }
 if (m_AutoTerminate) accept();
}

//--------------------------------------- toDouble ------------------------------------
void Dlg_Patch::setConnexionParam(const QString &module, int mode)
{   bool isOriginalNomadismeParam = FALSE;       // sera vrai si les parametres de nomadisme existent dans l'ancienne archive en cours
    //........................ lire les parametres originaux pour les mettre dans la nouvelle archive ............................
    QString  driver,base,login,pass,host,port, value;
    QString  oriParam;
    CGestIni::Param_UpdateFromDisk(m_medinTuxDir + "Programmes/"+module+"/bin/"+module+".ini", oriParam ); // on va chercher le Manager.ini d'origine ici
    if (oriParam.find("Master")==-1 && oriParam.find("Master")==-1)
       {CGestIni::Param_ReadParam(      oriParam, "Connexion", "Parametres", &driver, &base, &login, &pass, &host, &port);  // param par defaut lus dans module d'origine
        normalizeConnexionParam(module, driver,  base ,  login,  pass,  host, port );
       }
    else
       {if   (CGestIni::Param_ReadParam(  oriParam, "Connexion", "Master", &driver, &base, &login, &pass, &host, &port)==0) isOriginalNomadismeParam = TRUE;
        else  CGestIni::Param_ReadParam(  oriParam, "Connexion", "Parametres", &driver, &base, &login, &pass, &host, &port);  // param par defaut lus dans module d'origine
        normalizeConnexionParam(module, driver,  base ,  login,  pass,  host, port );

       }
    if (port.stripWhiteSpace().length()==0) port = "3306";
    //........................  placer les parametres originaux dans la nouvelle archive ............................
    QString  dstParam;
    QString dst = QDir::cleanDirPath (m_PathAppli+"../../"+module+"/bin/"+module+".ini");
    CGestIni::Param_UpdateFromDisk(dst,  dstParam ); // on va chercher le .ini de destination ici
    if (mode==1||isOriginalNomadismeParam)
       {if (isOriginalNomadismeParam) CGestIni::Param_ReadParam(  oriParam, "Connexion", "Master",     &driver, &base, &login, &pass, &host, &port);
        normalizeConnexionParam(module, driver,  base ,  login,  pass,  host, port );
        CGestIni::Param_WriteParam(&dstParam, "Connexion", "Master",      driver,  base,  login,  pass,  host,  port);

        if (isOriginalNomadismeParam) CGestIni::Param_ReadParam(  oriParam, "Connexion", "Nomade",     &driver, &base, &login, &pass, &host, &port);
        normalizeConnexionParam(module, driver,  base ,  login,  pass,  host, port );
        CGestIni::Param_WriteParam(&dstParam, "Connexion", "Nomade",      driver,  base,  login,  pass,  host,  port);

        if (isOriginalNomadismeParam) CGestIni::Param_ReadParam(  oriParam, "Connexion", "Reverse",    &driver, &base, &login, &pass, &host, &port);
        normalizeConnexionParam(module, driver,  base ,  login,  pass,  host, port );
        CGestIni::Param_WriteParam(&dstParam, "Connexion", "Reverse",     driver,  base,  login,  pass,  host,  port);

        if (isOriginalNomadismeParam) CGestIni::Param_ReadParam(  oriParam, "Connexion", "nomadisme",  &value);
        else                          value = "non actif";
        CGestIni::Param_WriteParam(&dstParam, "Connexion", "nomadisme",   value);
        if (isOriginalNomadismeParam) CGestIni::Param_ReadParam(  oriParam, "Connexion", "Gestion nomadisme", &value);
        else                          value = "fullReplication";
        CGestIni::Param_WriteParam(&dstParam, "Connexion", "Gestion nomadisme",  value);
       }
    else
       {CGestIni::Param_WriteParam(&dstParam, "Connexion", "Parametres",  driver,   base,  login,  pass,  host,  port);
       }
    CGestIni::Param_UpdateToDisk(dst,  dstParam ); // on va placer le .ini modifie ici
}

//--------------------------------------- normalizeConnexionParam ------------------------------------
void Dlg_Patch::normalizeConnexionParam( const QString &moduleToSet, QString &driver,  QString &base ,  QString &login,  QString &pass,  QString &host,  QString &port )
{QString                          baseToSet = "DrTuxTest";
 if (moduleToSet=="ccamview")     baseToSet = "CCAMTest";
 if (moduleToSet=="QLaboFTP")     baseToSet = "QLaboFTP";
 if (moduleToSet=="comptabilite") baseToSet = "Comptabilite";
 if (driver.stripWhiteSpace().length()==0) {driver  = "QMYSQL3"; base  = baseToSet; }
 if (login.stripWhiteSpace().length()==0)  {login  = "root"; }
 if (pass.stripWhiteSpace().length()==0)   {pass  = ""; }
 if (host.stripWhiteSpace().length()==0)   {host  = "localhost"; }
 if (port.stripWhiteSpace().length()==0)   {port  = "3306"; }
}

//--------------------------------------- toDouble ------------------------------------
double Dlg_Patch::toDouble(const QString &str)
{
 int     end = str.length();
 QString ret = "";
 int       i = 0;
 while (i<end)
     {if (str[i]>='0' && str[i]<='9')  {ret += str[i];}
      else if (str[i]=='.')            {ret += ".";   }
      else if (str[i]==',')            {ret += ".";   }
      ++i;
     }
 return ret.toDouble();
}
//--------------------------------------- GetMedinTuxLocalVersion ------------------------------------
int Dlg_Patch::GetMedinTuxLocalVersion(QString &val)
{   int v     = 0;
    int vlast = 0;
    //........................ explorer la source .......................................
    QString dirMedinTux = m_medinTuxDir;
    if (dirMedinTux.endsWith("/")) dirMedinTux = dirMedinTux.left(dirMedinTux.length()-1);
    QDir cur_dir( dirMedinTux );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );

    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return 0;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {
        QString fname = fi->fileName();
        if ( fi->isFile() && fname.startsWith("Version-"))   //  vers-2.13
                {val = toNum(fname, "$allString");
	         v   = normaliseVersion(val);
                 if ( v > vlast ) vlast = v;
                }
        ++it;
    }
 return v;
}
//--------------------------------------- GetMedinTuxGlossaireVersion ------------------------------------
int Dlg_Patch::GetMedinTuxGlossaireVersion(QString &val, QString *pGloPath)
{   int                v  = 0;
    int            vlast  = 0;
    QString     outParam  = "";
    QString      gloPath  = "";
    QString   managerDir  = m_medinTuxDir+"Programmes/Manager/bin/";
    CGestIni::Param_UpdateFromDisk(managerDir+"Manager.ini", outParam );
    if (CGestIni::Param_ReadParam(outParam ,"Glossaire", "Path", &gloPath) ==0)                 // recuperer path du Glossaire dans le .ini
       {if ( QDir::isRelativePath ( gloPath ) ) gloPath  = QDir::cleanDirPath (gloPath.prepend( managerDir) ); // le derelativiser
       }

    //........................ explorer la source .......................................
    if (gloPath.endsWith("/")) gloPath = gloPath.left(gloPath.length()-1);
    if (pGloPath) *pGloPath = gloPath;
    QDir cur_dir( gloPath );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );

    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return 0;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {
        QString fname = fi->fileName();
        if ( fi->isFile() && fname.startsWith("Version-"))   //  vers-2.13
                {val = toNum(fname, "$allString");
                 v   = normaliseVersion(val);
                 if ( v > vlast ) vlast = v;
                }
        ++it;
    }
 return v;
}

//-------------------------------------- isUpgradeOk ---------------------------------------------------
int Dlg_Patch::isUpgradeOk(int &resultVersion , int &lastVersion , const QString &fromTo)
{    QString val;
     QApplication::restoreOverrideCursor();

     m_pCMoteurBase->GetMedinTuxVersion(val);
     resultVersion = normaliseVersion(val);

     if (resultVersion <= lastVersion)
        {QString message = tr("Erreur upgrade MedinTux : %2 Arrêt du processus.\n"
                              "Vérifiez si la base '%1' est celle devant être \n"
                              "indiqué dans 'drtux.ini' section : [Connexion] \n"
                              "après les labels : Nomade et Master '").arg(m_pCMoteurBase->m_DataBase->databaseName(), fromTo);
         textEdit_infos->setText(message);
         QMessageBox::critical (this, tr("ERREUR DE LA MISE A JOUR DE MEDINTUX"), message, QMessageBox::Abort , QMessageBox::NoButton, QMessageBox::NoButton );
         return 0;
        }
     lastVersion = resultVersion;
     return 1;
}

//-------------------------------------- normaliseVersion( ---------------------------------------------------
int Dlg_Patch::normaliseVersion(QString &version)
{if (version[1]=='.') version = version.prepend('0');   // cas du 1.20.001  --> 01.20.001     1.20 -->01.20
 version = version.remove('.');
 if (version.length() <= 3 )
    {version +="000";
    }
 return version.toInt();
}

//-------------------------------------- testBase ---------------------------------------------------
int Dlg_Patch::testBase(QString &val)
{
 QSqlDatabase *defaultDB = QSqlDatabase::addDatabase( qApp->argv()[2] );
 if (defaultDB==0)
    {val = tr("ERROR database driver not exist");
     return 0;
    }
 QString pass = qApp->argv()[5];
 if (pass=="_=OO=_") pass = "";

 defaultDB->setDatabaseName( qApp->argv()[3] );    // base name
 defaultDB->setUserName(     qApp->argv()[4] );    // connexion name
 defaultDB->setPassword(     pass );               // password
 defaultDB->setHostName(     qApp->argv()[6] );    // hostname (distant or local)

 if ( defaultDB->open() )
    {val = "OK";
     defaultDB->close();
     return 1;
    }
  else
    {if (defaultDB->lastError().type() != QSqlError::None)
        {val = defaultDB->lastError().text();
        }
     else
        {val = tr("testBase() connexion SQL unknow ERROR\r\n")+defaultDB->lastError().text();
        }
    }
 return 0;
}

//-------------------------------------- InitCMoteurBase -----------------------------------------
QString Dlg_Patch::InitCMoteurBase()
{   QString qstr        = "";
    QString val1        = "";
    QString driver      = "";
    QString base        = "";
    QString login       = "";
    QString pass        = "";
    QString host        = "";
    QString port        = "";
    QString err         = "";
 //................ recuperer param de connexion de DrTux.ini .................................
 //m_PathDrTuxIni = QDir::cleanDirPath ( m_PathAppli + "../../drtux/bin/DrTux.ini" );
 m_PathDrTuxIni = CGestIni::Construct_Name_File_Ini( "drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"");
 CGestIni::Param_UpdateFromDisk( m_PathDrTuxIni , m_PatchParam );
 CGestIni::Param_UpdateFromDisk( m_PathAppli + "Ressources/DataBase.cfg" , m_BaseCfg );


    //...................... nomadisme ......................................................
    bool isNomadeActif=FALSE;
    QString connexionVar = "";
    if (CGestIni::Param_ReadParam(m_PatchParam,"Connexion", "nomadisme", &val1)==0 && val1.lower()[0]=='a')  // zero = pas d'erreur
       {isNomadeActif = TRUE;
       }
    if (isNomadeActif)   connexionVar     = "Nomade";
    else                 connexionVar     = "Master";
    //.......................Charger les parametres de connexion ........................................
    //                       Local au programme (pas celui de l'utilisateur)
    if (CGestIni::Param_ReadParam( m_PatchParam, "Connexion", connexionVar, &driver, &base, &login, &pass, &host, &port) !=0 )  // zero = pas d'erreur
       { driver     = "QMYSQL3";
         base       = "DrTux";
         login      = "root";
         pass       = "";
         host       = "localhost";
         port       = "3306";
       }
 normalizeConnexionParam( "Manager", driver,  base ,  login,  pass,  host,  port );
 //....................... ouvrir et initialiser le moteur de base de données ....................................
 m_pCMoteurBase = new CMoteurBase(driver ,                 // nom du driver: "QODBC3" "QMYSQL3" "QPSQL7"
                                  base,                    // nom de la base: si QODBC3 -> nom de la source de données (userDSN)
                                  login,                   // = "root"       user
                                  pass,                    // = ""           password
                                  host,                    // = "localhost"  hostname
                                  port,
                                  m_BaseCfg,
                                  "*MISE_A_JOUR",
                                  &qstr);
 if (m_pCMoteurBase==0)        {err = tr("Echec de la connexion à   Driver : %1 base : %2 avec %3" ).arg(driver, base, login+"@"+host+":"+port);
                                textLabelInfo->setText(err);
                                pushButtonOk->hide();
                                return err + "\n"+qstr;
                               }
 if (m_pCMoteurBase->m_DataBase ==0)   {err = tr("Echec de la connexion à   Driver : %1 base : %2 avec %3" ).arg(driver, base, login+"@"+host+":"+port);
                                        textLabelInfo->setText(err);
                                        delete m_pCMoteurBase; m_pCMoteurBase=0;
                                        pushButtonOk->hide();
                                        return err + "\n"+qstr;
                                       }
 m_pCMoteurBase->GetMedinTuxVersion(val1);
 setCaption(tr("MedinTux version : ")+val1);
 return err;
}

//-------------------------------------- InitCMedicaBase -----------------------------------------
QString Dlg_Patch::InitCMedicaBase()
{//......................... va falloir aussi avoir la base medicamenteuse ........................................
 QString err           = "";
 QString MedicaBaseIni = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"MedicaBase.ini");
 QString MedicaBaseCfg = CGestIni::Construct_Name_File_Ini("drtux",QFileInfo (qApp->argv()[0]).dirPath (true),"MedicaBase.cfg");

 m_pCMedicaBase = new CMedicaBase(MedicaBaseIni ,MedicaBaseCfg );
 if (m_pCMedicaBase->m_MedicaBase==0)
    {delete m_pCMedicaBase;
     m_pCMedicaBase=0;
     err = tr("Echec de la connexion à la base thérapeutique ");
    }
 return err;
}

//---------------------------------------- Sortir ------------------------------------------------
void Dlg_Patch::Sortir()
{accept();
}

//---------------------------------------- destroy ------------------------------------------------
void Dlg_Patch::destroy()
{if (m_pCTheraBase ) delete m_pCTheraBase;
 if (m_pCMoteurBase) delete m_pCMoteurBase;
 if (m_pCMedicaBase) delete m_pCMedicaBase;
}

//---------------------------------------- CopieDataBaseCfg ------------------------------------------------
void Dlg_Patch::CopieDataBaseCfg(const QString &fileName)
{//...................... copier les nouveaux fichiers de configuration ................................................
     //                       pour que la base de donnée puisse etre initialisée
     QString dst    =  "";
     QString src    = m_PathAppli + "Ressources/" + fileName;

     dst    = CGestIni::Construct_PathBin_Module("drtux",QFileInfo (qApp->argv()[0]).dirPath (true));
     CHtmlTools::Copy_File(src, dst);

     dst    = CGestIni::Construct_PathBin_Module("Manager",QFileInfo (qApp->argv()[0]).dirPath (true));
     CHtmlTools::Copy_File(src, dst);

     dst    = CGestIni::Construct_PathBin_Module("gest_user",QFileInfo (qApp->argv()[0]).dirPath (true));
     CHtmlTools::Copy_File(src, dst);

}



//--------------------------------------- AddDroitsToAllUser ------------------------------------
void Dlg_Patch::AddDroitsToAllUser(QSqlQuery &qsqlQuery, const QString &droits, int table_type)
{        //..................... rajouter les droit -agc-agv-idv-idc-idg aux utilisateurs ................................................
         QString droitsAlreadySet = "";
         QString requete          = "SELECT  ID_PrimKey, Droits FROM Personnes where Droits like '%med%'";
    if (table_type & 1)
       { qsqlQuery.exec( requete );
         if (qsqlQuery.isActive())
            {QSqlQuery queryU (QString::null, m_pCMoteurBase->m_DataBase);
             while (qsqlQuery.next()) //                                 droits actuels            +  -sgn
                   {droitsAlreadySet  = qsqlQuery.value(1).toString();
		    droitsAlreadySet += droits;
		    if (droitsAlreadySet.left(1) != "-") droitsAlreadySet.prepend("-");
		    requete  = " UPDATE Personnes SET `Droits` = '"+ droitsAlreadySet + "' WHERE `ID_PrimKey` = '" + qsqlQuery.value(0).toString() + "' ;";
                    queryU.exec(requete);                                                        //-sgn-orv-orc-orp-orr-bmc-ccc-acc-cic-lac-obv-obc-obp-glc-gld-etc-ata-atv-atc-ttv-ttc-tvv-tvc
                   }
            } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
       }
    if (table_type & 2)
       {
         requete          = "SELECT  PrimKey, FriendUserDroits FROM `user_perms` ;";
         qsqlQuery.exec( requete );
         if (qsqlQuery.isActive())
            {QSqlQuery queryU (QString::null, m_pCMoteurBase->m_DataBase);
             while (qsqlQuery.next()) //                                 droits actuels            +  -sgn
                   {droitsAlreadySet  = qsqlQuery.value(1).toString();
                    droitsAlreadySet += droits;
                    if (droitsAlreadySet.left(1) != "-") droitsAlreadySet.prepend("-");
                    requete  = " UPDATE user_perms SET `FriendUserDroits` = '"+ droitsAlreadySet + "' WHERE `PrimKey` = '" + qsqlQuery.value(0).toString() + "' ;";
                    queryU.exec(requete);                                                        //-sgn-orv-orc-orp-orr-bmc-ccc-acc-cic-lac-obv-obc-obp-glc-gld-etc-ata-atv-atc-ttv-ttc-tvv-tvc
                   }
            } //if (pSqlQuery && pSqlQuery->isActive() && pSqlQuery->next())
       }
}
//------------------------------------------ toNum -----------------------------------------
/*! \brief ne retient d'une chaine de caracteres que les valeurs numeriques et le point (au passage les virgules seront transformees en point).
 *  \param text           const QString & texte a convertir en numerique
 *  \param cutFirstStr_in const QString & mode de conversion numerique :\
 SI   contient $stopIfNotNum la conversion s'arr�te au premier caract�re non num�rique.
 SI   contient $allString tous les caract�res num�riques de la cha�ne seront retenus.
 SI   contient $toInt la conversion ne conservera que la partie enti�re.
 SI   toute autre valeur, la conversion s'arr�tera � la premi�re occurence de cette valeur.
 *  \return une QString resultat du traitement
 */
QString Dlg_Patch::toNum(const QString &text, const QString &cutFirstStr_in)
{
 QString str               = text.stripWhiteSpace();
 QString cutFirstStr       = cutFirstStr_in;
 bool    cutFirstNonNum    = TRUE;
 bool    toInt             = (cutFirstStr.find("$toInt") != -1);
 if (toInt) cutFirstStr    = cutFirstStr.remove("$toInt");
 if (cutFirstStr.length())
    {
     if      (cutFirstStr.find("$stopIfNotNum") != -1)    cutFirstNonNum = TRUE;
     else if (cutFirstStr.find("$allString")    != -1)    cutFirstNonNum = FALSE;
     else {int pos = str.find(cutFirstStr);
           if (pos != -1) str.truncate(pos);
          }
    }
 int     end = str.length();
 QString ret = "";
 int       i = 0;
 while (i<end)
     {if (str[i]>='0' && str[i]<='9')  {ret += str[i];}
      else if (str[i]=='.')            {ret += ".";   }
      else if (str[i]==',')            {ret += ".";   }
      else if (cutFirstNonNum)         {i    = end;   }
      ++i;
     }
 if (toInt)
    {int pos  =     ret.find(".");
     if (pos != -1) ret.truncate(pos);
    }
 return ret;
}

//---------------------------------------- GetDatasempIndexVersion ------------------------------------------------
QString Dlg_Patch::GetDatasempIndexVersion()
{   return m_pCMedicaBase->Medica_GetDatasempVersion(CMedicaBase::Index);
}

//---------------------------------------- SetDatasempIndexVersion ------------------------------------------------
void Dlg_Patch::SetDatasempIndexVersion(const QString &version)
{   m_pCMedicaBase->Medica_SetDatasempVersion(QString(version), CMedicaBase::IndexAndData);
}

//---------------------------------------- RenameTableWhithCorrectName ------------------------------------------------
void Dlg_Patch::RenameTableWhithCorrectName(const QString &correct_name_table, QSqlQuery *pQSqlQuery)
{m_pCMoteurBase->RenameTableWhithCorrectName(correct_name_table, pQSqlQuery);
}

//---------------------------------------- pushButtonAnnuler_clicked ------------------------------------------------
void Dlg_Patch::pushButtonAnnuler_clicked()
{reject();
}

//---------------------------------------- pushButton_FindInstallation_clicked ------------------------------------------------
void Dlg_Patch::pushButton_FindInstallation_clicked()
{ QString startDir = m_PathAppli+"../../../";  // remonter Programmes/mise_a_jour/bin
  startDir  = QDir::cleanDirPath (startDir); // le derelativiser
  m_medinTuxDir   = QFileDialog::getExistingDirectory ( startDir, this, "localizeDir", tr("répertoire d'installation de MedinTux") );
  if (m_medinTuxDir.stripWhiteSpace().length()==0) {m_medinTuxDir=""; return;}
  if (!m_medinTuxDir.endsWith("/")) m_medinTuxDir += "/";
  lineEdit_MedinTuxFolder->setText(m_medinTuxDir);
  if ( !QFile::exists(m_medinTuxDir + "Programmes/Manager/bin/Manager.ini"))
     {QMessageBox::critical (this, tr("ERREUR DE LA MISE A JOUR DE MEDINTUX"), tr("Le répertoire indiqué '%1' ne semble pas correspondre à un répertoire de MedinTux").arg(m_medinTuxDir), QMessageBox::Abort , QMessageBox::NoButton, QMessageBox::NoButton );
      m_medinTuxDir="";
     }
}

