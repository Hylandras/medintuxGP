//
// C++ Implementation:
//
// Description:
//
//
// Author: Sevin Roland <roland-sevin@medintux.org>, (C) 2004 - 2011
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "C_Dlg_FeuillePrincipale.h"
#include "C_Dlg_ChangeMAJCCAM.h"
#include "C_Dlg_PatientActeProperty.h"
#include "C_Dlg_Print.h"
#include "C_Dlg_Categories.h"
#include "C_Dlg_SigemsLogin.h"
#include "C_Dlg_MyAlertBox.h"
#include <QSqlError>
#include <QTimer>

#define THE_CATEGORIE   0
#define THE_CODE_USUEL  1
#define THE_CODE_CCAM   2
#define THE_LIBELLE     3

#ifdef Q_WS_X11
  #include <QX11Info>
#endif
static char NUM_VERSION[]     = "==##@@==2.16.001==@@##==";
static char CHANGEMENTS[]     = "==========================================================================================================<br>"
                                "Modif RS ccam_view du 16-12-2013 Version 2.16.000<br>"
                                "---------------------------------------------------------------------------------------------------------<br>"
                                "- <b><u>AMELIORATION</u></b> : Affichage correct du num\303\251rde version "
                                "<br>Mise en avant plan sous ubuntu unity correcte"
                                "<br>fonctionne parfaitement sous Mac et ubuntu 64 bits 13.10"
                                "<br>"
                                "==========================================================================================================<br>"
                                "Modif RS ccam_view du 29-03-2010 Version 2.14.003<br>"
                                "---------------------------------------------------------------------------------------------------------<br>"
                                "- <b><u>AMELIORATION</u></b> : Affichage correct du num\303\251rde version <br>"
                                "  et fonctionne parfaitement sous Mac correction de l'int\303\251gration des fichiers NX"
                                "<br>"
                                "==========================================================================================================<br>"
                                "Modif RS ccam_view du 05-10-2010 Version 2.14.002<br>"
                                "---------------------------------------------------------------------------------------------------------<br>"
                                "- <b><u>AMELIORATION</u></b> : d\303\251sormais la doc est dans <br>"
                                "  le r\303\251pertoire /Programmes/ccamview/Doc de MedinTux<br>"
                                "<br>"
                                "==========================================================================================================<br>"
                                "Modif RS ccam_view du 16-07-2010 Version 2.14.001<br>"
                                "---------------------------------------------------------------------------------------------------------<br>"
                                "- <b><u>AMELIORATION</u></b> : le filtre SQL fonctionne m\303\252me si les <br>"
                                "  donn\303\251es de la base m\303\251langent l'UTF8 avec le Latin1<br>"
                                "<br>"
                                "==========================================================================================================<br>"
                                "Modif RS ccam_view du 14-06-2010 Version 2.14.000<br>"
                                "---------------------------------------------------------------------------------------------------------<br>"
                                "- <b><u>FONCTIONNALITES</u></b> :  - Int\303\251gration du module de gestion APropos<br>"
                                "  de l'information A propos de la suite MedinTux<br>";
//------------------------------------------- C_Dlg_FeuillePrincipale ------------------------------------------------------------------------
C_Dlg_FeuillePrincipale::C_Dlg_FeuillePrincipale(QWidget* parent, const char* name, Qt::WindowFlags fl)
: FeuillePrincipale(parent,name,fl)
{
    m_NUM_VERSION     = NUM_VERSION;
    m_Apropos_Proc    = 0;
    QObject::connect(bouton_AjouterDossier, SIGNAL(clicked()), this, SLOT(bouton_AjouterDossier_clicked()));
    QObject::connect(bouton_AjouterThesaurus, SIGNAL(clicked()), this, SLOT(bouton_AjouterThesaurus_clicked()));
    QObject::connect(bouton_ImprimerThesaurus, SIGNAL(clicked()), this, SLOT(bouton_ImprimerThesaurus_clicked()));
    QObject::connect(bouton_ModifActeThesaurus, SIGNAL(clicked()), this, SLOT(bouton_ModifActeThesaurus_clicked()));
    QObject::connect(bouton_SupprimeActeThesaurus, SIGNAL(clicked()), this, SLOT(bouton_SupprimeActeThesaurus_clicked()));
    QObject::connect(cbox_Action, SIGNAL(activated(QString)), this, SLOT(critere_code(QString)));
    QObject::connect(cbox_ModeAcces, SIGNAL(activated(QString)), this, SLOT(critere_code(QString)));
    QObject::connect(cbox_SiteAnatomique, SIGNAL(activated(QString)), this, SLOT(critere_code(QString)));
    QObject::connect(cbox_SiteAnatomique, SIGNAL(highlighted(QString)), this, SLOT(cbox_SiteAnatomique_highlighted(QString)));
    QObject::connect(checkBoxTestMode, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTestMode_stateChanged(int)));
    QObject::connect(comboBoxActivite, SIGNAL(activated(QString)), this, SLOT(comboBoxActivite_activated(QString)));
    QObject::connect(combo_Categories, SIGNAL(activated(int)), this, SLOT(combo_Categories_activated(int)));
    QObject::connect(combo_Praticiens, SIGNAL(activated(int)), this, SLOT(combo_Praticiens_activated(int)));
    QObject::connect(edit_Code, SIGNAL(textChanged(QString)), this, SLOT(recherche_par_code(QString)));
    QObject::connect(edit_MotCle, SIGNAL(textChanged(QString)), this, SLOT(Modif_MotCle1(QString)));
    QObject::connect(edit_MotCle_2, SIGNAL(textChanged(QString)), this, SLOT(Modif_MotCle2(QString)));
    QObject::connect(fileExitAction, SIGNAL(activated()), this, SLOT(close()));
    QObject::connect(filePrintAction, SIGNAL(activated()), this, SLOT(Imprimer()));
    QObject::connect(fileSaveAction, SIGNAL(activated()), this, SLOT(HtmlExport()));
    QObject::connect(helpAboutAction, SIGNAL(activated()), this, SLOT(AboutCCAM_View()));
    QObject::connect(listview_ListeActePat, SIGNAL(doubleClicked(Q3ListViewItem*)), this, SLOT(listview_ListeActePat_doubleClicked(Q3ListViewItem*)));
    QObject::connect(listview_ListeDossPat, SIGNAL(clicked(Q3ListViewItem*)), this, SLOT(listview_ListeDossPat_clicked(Q3ListViewItem*)));
    QObject::connect(listview_ListePatient, SIGNAL(clicked(Q3ListViewItem*)), this, SLOT(listview_ListePatient_clicked(Q3ListViewItem*)));
    QObject::connect(listview_Thesaurus, SIGNAL(clicked(Q3ListViewItem*)), this, SLOT(listview_Thesaurus_clicked(Q3ListViewItem*)));
    QObject::connect(listview_Thesaurus, SIGNAL(selectionChanged()), this, SLOT(listview_Thesaurus_selectionChanged()));
    QObject::connect(lview_ActesTrouves, SIGNAL(clicked(Q3ListViewItem*)), this, SLOT(lview_ActesTrouves_clicked(Q3ListViewItem*)));
    QObject::connect(lview_ActesTrouves, SIGNAL(selectionChanged()), this, SLOT(lview_ActesTrouves_selectionChanged()));
    QObject::connect(lview_ActesTrouves, SIGNAL(doubleClicked(Q3ListViewItem*)), this, SLOT(lview_ActesTrouves_doubleClicked(Q3ListViewItem*)));
    QObject::connect(lview_Hierarchie, SIGNAL(clicked(Q3ListViewItem*)), this, SLOT(lview_Hierarchie_clicked(Q3ListViewItem*)));
    QObject::connect(lview_Hierarchie, SIGNAL(collapsed(Q3ListViewItem*)), this, SLOT(NoeudFerme(Q3ListViewItem*)));
    QObject::connect(lview_Hierarchie, SIGNAL(expanded(Q3ListViewItem*)), this, SLOT(NoeudOuvert(Q3ListViewItem*)));
    QObject::connect(lview_Hierarchie, SIGNAL(currentChanged(Q3ListViewItem*)), this, SLOT(lview_Hierarchie_currentChanged(Q3ListViewItem*)));
    QObject::connect(lview_Hierarchie, SIGNAL(selectionChanged(Q3ListViewItem*)), this, SLOT(lview_Hierarchie_selectionChanged(Q3ListViewItem*)));
    //QObject::connect(DROP_PraticienAction, SIGNAL(activated()), this, SLOT(menu_drop_praticiens_clicked()));
    //QObject::connect(PraticienAction, SIGNAL(activated()), this, SLOT(menu_praticien_clicked()));
    //QObject::connect(fileImporter_la_table_des_medecinsAction, SIGNAL(activated()), this, SLOT(fileImporter_la_table_des_medecinsAction_activated()));
    //QObject::connect(fileExporter_la_table_des_mdecinsAction, SIGNAL(activated()), this, SLOT(fileExporter_la_table_des_mdecinsAction_activated()));
    QObject::connect(pushButtonActePatEffacer, SIGNAL(clicked()), this, SLOT(pushButtonActePatEffacer_clicked()));
    QObject::connect(pushButtonActePatImprimer, SIGNAL(clicked()), this, SLOT(pushButtonActePatImprimer_clicked()));
    QObject::connect(pushButtonActePatModifier, SIGNAL(clicked()), this, SLOT(pushButtonActePatModifier_clicked()));
    QObject::connect(pushButtonChercherPatient, SIGNAL(clicked()), this, SLOT(pushButtonChercherPatient_clicked()));
    QObject::connect(pushButtonClear, SIGNAL(clicked()), this, SLOT(pushButtonClear_clicked()));
    QObject::connect(pushButtonDossRech, SIGNAL(clicked()), this, SLOT(pushButtonDossRech_clicked()));
    QObject::connect(pushButtonNumDossier, SIGNAL(clicked()), this, SLOT(pushButtonNumDossier_clicked()));
    QObject::connect(tabwidget_Recherche, SIGNAL(currentChanged(QWidget*)), this, SLOT(change_onglet_recherche(QWidget*)));
    QObject::connect(comboBox_Bases, SIGNAL(activated(int)), this, SLOT(comboBox_Bases_activated(int)));
    QObject::connect(listview_Thesaurus, SIGNAL(doubleClicked(Q3ListViewItem*)), this, SLOT(listview_Thesaurus_doubleClicked(Q3ListViewItem*)));
    QObject::connect(lview_GestesComplementaires, SIGNAL(doubleClicked(Q3ListViewItem*)), this, SLOT(lview_GestesComplementaires_doubleClicked(Q3ListViewItem*)));
    QObject::connect(textbrowser_procedures, SIGNAL(anchorClicked(QString,QString)), this, SLOT(textbrowser_procedures_anchorClicked(QString,QString)));
    QObject::connect(textbrowser_hierarchie, SIGNAL(anchorClicked(QString,QString)), this, SLOT(textbrowser_hierarchie_anchorClicked(QString,QString)));
    QObject::connect(comboBox_Bases, SIGNAL(highlighted(QString)), this, SLOT(comboBox_Bases_highlighted(QString)));
    QObject::connect(configurationMise__jour_de_la_Base_CCAMAction, SIGNAL(activated()), this, SLOT(configurationMise__jour_de_la_Base_CCAMAction_activated()));
    QObject::connect(lineedit_NumDoss, SIGNAL(returnPressed()), this, SLOT(lineedit_NumDoss_returnPressed()));
    QObject::connect(lineedit_NumDossAn, SIGNAL(returnPressed()), this, SLOT(lineedit_NumDossAn_returnPressed()));
    QObject::connect(lineedit_NumDossAn, SIGNAL(textChanged(QString)), this, SLOT(lineedit_NumDossAn_textChanged(QString)));
    QObject::connect(lineedit_NumDoss, SIGNAL(lostFocus()), this, SLOT(lineedit_NumDoss_lostFocus()));
    QObject::connect(lineedit_NumDoss, SIGNAL(textChanged(QString)), this, SLOT(lineedit_NumDoss_textChanged(QString)));
    QObject::connect(pushButtonRechAvancee, SIGNAL(clicked()), this, SLOT(pushButtonRechAvancee_clicked()));
    //QObject::connect(helpIndexAction, SIGNAL(activated()), this, SLOT(HelpIndexCCAM_View()));
    QObject::connect(pushButtonRecActesListToSigems, SIGNAL(clicked()), this, SLOT(pushButtonRecActesListToSigems_clicked()));
    QObject::connect(configurationExport_Table_ACTION1Action, SIGNAL(activated()), this, SLOT(configurationExport_Table_ACTION1Action_activated()));
    QObject::connect(configurationImport_Tables_EssentiellesAction, SIGNAL(activated()), this, SLOT(configurationImport_Tables_EssentiellesAction_activated()));
    QObject::connect(fileExporter_le_thsaurusAction, SIGNAL(activated()), this, SLOT(fileExporter_le_thsaurusAction_activated()));
    QObject::connect(fileImporter_un_thsaurusAction, SIGNAL(activated()), this, SLOT(fileImporter_un_thsaurusAction_activated()));
    QObject::connect(fileEffacer_le_thsaurusAction, SIGNAL(activated()), this, SLOT(fileEffacer_le_thsaurusAction_activated()));
    QObject::connect(edit_Code, SIGNAL(lostFocus()), this, SLOT(edit_Code_lostFocus()));
    QObject::connect(listview_ListeActePat, SIGNAL(clicked(Q3ListViewItem*,QPoint,int)), this, SLOT(listview_ListeActePat_clicked(Q3ListViewItem*,QPoint,int)));
    QObject::connect(comboBoxDateEffet,     SIGNAL(activated(const QString &))         , this, SLOT(   comboBoxDateEffet_activated(const QString &)));

    QString dt;
    edit_Code->setText("_______");
    m_pcSoapI                = 0;
    m_UrlSigemsWebServices   = "";                      // "212.234.37.97:1234/sigems/services/";
    m_SigemsLogin            = "";                      // "marignane";
    m_SigemsPassword         = "";                      // "mari789";
    m_UpdatePatList          = 0;
    m_pQListViewItemToRecord = 0;
    m_pQListViewItemToErase  = 0;
    m_NumKeyboardMap         = "";
    m_LastDate               = QDate::currentDate();
    m_LastTime               = QTime::currentTime();
    m_CurrentPopupItem       = "";
    m_RetPopupItem           = "";
    m_pQPopupMenu            = 0;
    bouton_AjouterThesaurus->setEnabled(FALSE);
    bouton_AjouterDossier->setEnabled(FALSE);
    int nb_arg = qApp->argc();                                                 // 0 = path programme   1 = fichier patient
    int      i = 2;
    if (nb_arg >= 3)  lineedit_Code->setText(qApp->argv()[ i++ ]);             // code patient 1
    if (nb_arg >= 4)  lineedit_Code1->setText(qApp->argv()[ i++ ]);            // code patient 2
    if (nb_arg >= 5)  lineedit_Nom->setText(QString(qApp->argv()[ i++ ]).replace("~@~","'"));              // nom usuel ou marital
    if (nb_arg >= 6)  lineedit_Nom_Naiss->setText(QString(qApp->argv()[ i++ ]).replace("~@~","'"));        // nom de jeune fille
    if (nb_arg >= 7)  lineedit_Prenom->setText(QString(qApp->argv()[ i++ ]).replace("~@~","'"));           // prenom
    if (nb_arg >= 8)
       {dt =  qApp->argv()[ i++ ];
        QDate  date (  atoi(dt.mid(4,4)),                                      // date naissance     10122003  --> YYYY  2003
                       atoi(dt.mid(2,2)),                                      //                    10122003  --> MM    12
                       atoi(dt.mid(0,2))                                       //                    10122003  --> DD    10
                     );
         if (date.isValid () ) dateEdit_DateNss->setDate (date );
       }
    if (nb_arg >= 9)  lineEdit_Sexe->setText(qApp->argv()[ i++ ]);             // sexe
    if (nb_arg >= 10) lineedit_Adr_1->setText(QString(qApp->argv()[ i++ ]).replace("~@~","'"));            // adresse 1
    if (nb_arg >= 11) lineedit_Adr_2->setText(QString(qApp->argv()[ i++ ]).replace("~@~","'"));            // adresse 2
    if (nb_arg >= 12) lineedit_CodePostal->setText(qApp->argv()[ i++ ]);       // code postal
    if (nb_arg >= 13) lineedit_Ville->setText(QString(qApp->argv()[ i++ ]).replace("~@~","'"));            // ville
    if (nb_arg >= 14) lineedit_Ville_Nss->setText(QString(qApp->argv()[ i++ ]).replace("~@~","'"));        // ville naissance
    if (nb_arg >= 15) lineedit_NumSS->setText(qApp->argv()[ i++ ]);            // numero de secu
    if (nb_arg >= 16) lineEdit_CleSS->setText(qApp->argv()[ i++ ]);            // clef N° secu
    if (nb_arg >= 17) lineedit_Tel->setText(qApp->argv()[ i++ ]);              // Tel 1
    if (nb_arg >= 18) i++; //lineedit_Te2->setText(qApp->argv()[ i++ ]);       // Tel 2
    if (nb_arg >= 19) i++; //lineedit_Te3->setText(qApp->argv()[ i++ ]);       // Tel 3
    if (nb_arg >= 20) i++; //lineedit_Email->setText(qApp->argv()[ i++ ]);     // Email
    if (nb_arg >= 21)
       {dt =  qApp->argv()[ i++ ];
        QDate  date (  atoi(dt.mid(4,4)),                                      // date Entree Doss   10122003  --> YYYY  2003
                       atoi(dt.mid(2,2)),                                      //                    10122003  --> MM    12
                       atoi(dt.mid(0,2))                                       //                    10122003  --> DD    10
                     );
         if (date.isValid () ) dateEdit_DateEntree->setDate (date );
       }
    if (nb_arg >= 22)
       {dt =  qApp->argv()[ i++ ];
        QDate  date (  atoi(dt.mid(4,4)),                                      // date Sortie Doss   10122003  --> YYYY  2003
                       atoi(dt.mid(2,2)),                                      //                    10122003  --> MM    12
                       atoi(dt.mid(0,2))                                       //                    10122003  --> DD    10
                     );
         if (date.isValid () ) dateEdit_DateSortie->setDate (date );
       }
    if (nb_arg >= 23) ++i;                                                     // praticien:password (trait\303\251 dans le main )

    if (nb_arg >= 24) SetNumDossierToEditables( qApp->argv()[ i++ ]);          // numero de dossier sigems
    if (nb_arg >= 25) m_GuidDrTux =  qApp->argv()[ i++ ];                      // numero de dossier DrTux
    else              m_GuidDrTux =  "";
    //........... remplir les combo box .........
    C_Dlg_FeuillePrincipale::critere_code(QString::null);
    QTimer::singleShot(500, this, SLOT(Slot_ActivateMainWindow())); // pour avoir la fenetre en avant plan sur ubuntu unity
}

//------------------------------------------ C_Dlg_FeuillePrincipale -------------------------------------------------------------------------------------------
C_Dlg_FeuillePrincipale::~C_Dlg_FeuillePrincipale()
{tryToStopAPropos();
 if (m_pcSoapI) delete m_pcSoapI;
}
//--------------------------------------- Slot_ActivateMainWindow ---------------------------------------------------
void C_Dlg_FeuillePrincipale::Slot_ActivateMainWindow()
{   //if (G_pCApp->getDroits().length()) setWindowFlags(Qt::WindowStaysOnTopHint);
    show();
    setWindowState(windowState() & ~Qt::WindowMinimized);
    activateWindow();
    raise();
}
/*$SPECIALIZATION$*/
//------------------------------ ReplaceThesaurusTokenByValue ----------------------------------------------------------------------------
int C_Dlg_FeuillePrincipale::ReplaceThesaurusTokenByValue( INFO_MED * pInfoMed, Q3ListViewItem * pQListViewItemActes, QString & document, int pos_deb, int len )
{QString  token = document.mid(pos_deb, len);
    QString  resolvToken(" ");
    QVariant qvar;
    //....................... fusion des lignes du th\303\251saurus lors impression .........................................
    if (pQListViewItemActes != 0 && token.left(10)=="thesaurus.")
    {if (token== "thesaurus.categorie")
        {resolvToken = pQListViewItemActes->text(0);
            document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
        else if (token== "thesaurus.LibelleUsuel")
        {resolvToken = pQListViewItemActes->text(1);
            document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
        else if (token== "thesaurus.codeCCAM")
        {resolvToken = pQListViewItemActes->text(2);
            document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
        else if (token== "thesaurus.LibelleCCAM")
        {resolvToken = pQListViewItemActes->text(3);
            document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
        else if (token== "thesaurus.Tarif")
        {resolvToken    = m_pCMoteurCCAM_Base->GetPrixActe( pQListViewItemActes->text(2), QDate::currentDate() );
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }

    }
    //....................... fusion des champs concernant le medecin .........................................
    if (pInfoMed==0) return pos_deb-2+resolvToken.length();   // cas de l'appel lors impression d'une ligne d'acte
    if (token== "DD-MM-YYYY"||token=="DATE COURANTE")
    {QDateTime qdt = QDateTime::currentDateTime ( Qt::LocalTime );
        QString format = "dd-MM-yyyy";
        document.replace ( pos_deb-2, len+4, qdt.toString ( format ));
    }
    else if (token== "IDENTIFICATION MEDECIN")
    {resolvToken = pInfoMed->m_Nid;
        document.replace ( pos_deb-2, len+4, resolvToken);
    }
    else if (token== "NOM PRENOM MEDECIN")
    {resolvToken = pInfoMed->m_Nom + " " + pInfoMed->m_Prenom;
        document.replace ( pos_deb-2, len+4, resolvToken);
    }
    else if (token== "NOM MEDECIN")
    {resolvToken = pInfoMed->m_Nom;
        document.replace ( pos_deb-2, len+4, resolvToken);
    }
    else if (token== "PRENOM MEDECIN")
    {resolvToken = pInfoMed->m_Prenom;
        document.replace ( pos_deb-2, len+4,resolvToken);
    }

    return pos_deb-2+resolvToken.length();
}

//-------------------------------------- Get_Head_Foot_Thesaurus --------------------------------------------------
QString C_Dlg_FeuillePrincipale::Get_Head_Foot_Thesaurus( QString * foot )
{QString path = GlobalPathAppli + SYS_FOLDER + "/";
 QString headFile("");
 if (CGestIni::Param_ReadParam(Global_ParamData,  "Impression Thesaurus", "PrintHead", &headFile) !=QString::null ) //???            // zero = pas d'erreur
    {headFile = "HeadThesaurus.html";
    }
 QString footFile("");
 if (CGestIni::Param_ReadParam(Global_ParamData,  "Impression Thesaurus", "PrintFoot", &footFile) !=QString::null )             // zero = pas d'erreur
    {footFile = "FootThesaurus.html";
    }
 QString str;
 CGestIni::Param_UpdateFromDisk(path + footFile, str);
 if (foot) *foot = str;
 CGestIni::Param_UpdateFromDisk(path + headFile, str);
 return str;
}

//------------------------------ ReplaceTokenByValue ----------------------------------------------------------------------------
int C_Dlg_FeuillePrincipale::ReplaceTokenByValue( IDENT_PAT * pIdentPat, Q3ListViewItem * pQListViewItemActes, QString & document, int pos_deb, int len )
{QString  token = document.mid(pos_deb, len);
 QString  resolvToken(" ");
 QVariant qvar;
 int         f;
 //....................... fusion des lignes d'actes lors impression .........................................

 if (pQListViewItemActes != 0 && token.left(5)=="acte.")
    {if (token== "acte.date")
        {resolvToken = pQListViewItemActes->text(LVA_DATE);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     if (token== "acte.heure")
        {resolvToken = pQListViewItemActes->text(LVA_HEURE);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.praticien")
        {resolvToken = pQListViewItemActes->text(LVA_PRATICIENNAME);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.code_ccam")
        {resolvToken = pQListViewItemActes->text(LVA_CODECCAM);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.libelle")
        {resolvToken = pQListViewItemActes->text(LVA_LIBELLEACTE);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.phase")
        {resolvToken = pQListViewItemActes->text(LVA_PHASE);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.activite")
        {resolvToken = pQListViewItemActes->text(LVA_ACTIVITE);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.Mod_1")
        {resolvToken = pQListViewItemActes->text(LVA_MODIFICATEUR1);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.Mod_2")
        {resolvToken = pQListViewItemActes->text(LVA_MODIFICATEUR2);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.Mod_3")
        {resolvToken = pQListViewItemActes->text(LVA_MODIFICATEUR3);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.Mod_4")
        {resolvToken = pQListViewItemActes->text(LVA_MODIFICATEUR4);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.nombre")
        {resolvToken = pQListViewItemActes->text(LVA_NB);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.NumDoss")
        {resolvToken = pQListViewItemActes->text(LVA_NUMDOSS);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.Nomenclature")
        {resolvToken = pQListViewItemActes->text(LVA_NOMENCLAT);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.IdPrat")
        {resolvToken = pQListViewItemActes->text(LVA_NIDPRATICIEN);
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
     else if (token== "acte.Tarif")
        {QString dtStr  = pQListViewItemActes->text(LVA_DATE);
         resolvToken    = m_pCMoteurCCAM_Base->GetPrixActe(pQListViewItemActes->text(LVA_CODECCAM),
                                                           QDate ( atoi(dtStr.mid(6,4)),     // 10/12/2003  --> YYYY  2003
                                                                   atoi(dtStr.mid(3,2)),     // 10/12/2003  --> MM    12
                                                                   atoi(dtStr.mid(0,2))      // 10/12/2003  --> DD    10
                                                                  ),
                                                            pQListViewItemActes->text(LVA_PHASE),
                                                            pQListViewItemActes->text(LVA_ACTIVITE),
                                                            pQListViewItemActes->text(LVA_MODIFICATEUR1),
                                                            pQListViewItemActes->text(LVA_MODIFICATEUR2),
                                                            pQListViewItemActes->text(LVA_MODIFICATEUR3),
                                                            pQListViewItemActes->text(LVA_MODIFICATEUR4)
                                                           );
         document.replace ( pos_deb-2, len+4, Q3StyleSheet::escape (resolvToken));
        }
    }

 if (pIdentPat==0) return pos_deb-2+resolvToken.length();   // cas de l'appel lors impression d'une ligne d'acte

 //....................... fusion du reste du document .........................................
 INFO_MED info_med;
 INFO_MED info_user;
 m_pCMoteurCCAM_Base->GetInfoMedecinsFromCode(pIdentPat->m_Utilisateur, info_user);
 m_pCMoteurCCAM_Base->GetInfoMedecinsFromNid(pIdentPat->m_PraticienNid, info_med);

 if (token== "DD-MM-YYYY"||token=="DATE COURANTE")
    {QDateTime qdt = QDateTime::currentDateTime ( Qt::LocalTime );
     QString format = "dd-MM-yyyy";
     document.replace ( pos_deb-2, len+4, qdt.toString ( format ));
    }
 else if (token== "IDENTIFICATION MEDECIN")
    {resolvToken = pIdentPat->m_PraticienNid;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "NOM PRENOM USER")
    {resolvToken = info_user.m_Nom + " " + info_med.m_Prenom;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "NOM USER")
    {resolvToken = info_user.m_Nom;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "PRENOM USER")
    {resolvToken = info_user.m_Prenom;
     document.replace ( pos_deb-2, len+4,resolvToken);
    }
 else if (token== "NOM PRENOM MEDECIN")
    {resolvToken = info_med.m_Nom + " " + info_med.m_Prenom;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "NOM MEDECIN")
    {resolvToken = info_med.m_Nom;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "PRENOM MEDECIN")
    {resolvToken = info_med.m_Prenom;
     document.replace ( pos_deb-2, len+4,resolvToken);
    }
 else if (token== "NUM DOSSIER")
    {resolvToken = pIdentPat->m_NumDossier;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "DATE ENTREE")
    {resolvToken = pIdentPat->m_DateEntree;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
  else if (token== "DATE SORTIE")
    {resolvToken = pIdentPat->m_DateSortie;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
  else if (token== "BASE TRAVAIL")
    {resolvToken = pIdentPat->m_BaseTravail;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
  else if (token== "NAME BASE")
    {CGestIni::Param_ReadParam(Global_ParamData,  pIdentPat->m_BaseTravail, "Name", &resolvToken);             // zero = pas d'erreur
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
  else if (token== "NUM PATIENT")
    {resolvToken = pIdentPat->m_NumPatient;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
  else if (token== "NUM. TEL1 PATIENT")
    {resolvToken = pIdentPat->m_Tel;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
  else if (token== "DATE NAISSANCE PATIENT")
    {resolvToken   = pIdentPat->m_DateNss;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
  else if (token== "AGE PATIENT")
    {if (pIdentPat->m_DateNss.length()>=10)
        {QDate date = QDate ( atoi(pIdentPat->m_DateNss.mid(6,4)),     // 10/12/2003  --> YYYY  2003
                              atoi(pIdentPat->m_DateNss.mid(3,2)),     // 10/12/2003  --> MM    12
                              atoi(pIdentPat->m_DateNss.mid(0,2))      // 10/12/2003  --> DD    10
                            );
         resolvToken   = m_pCMoteurCCAM_Base->CalculeDiffAge(&date);
        }
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
  else if (token== "SEXE PATIENT")
    {if (pIdentPat->m_Sexe=="F")       resolvToken=tr("F\303\251minin");
     else                              resolvToken=tr("Masculin");
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "TITRE PATIENT")
    {if (pIdentPat->m_DateNss.length()>=10)
        {QDate qDteNss = QDate ( atoi(pIdentPat->m_DateNss.mid(6,4)),     // 10/12/2003  --> YYYY  2003
                                 atoi(pIdentPat->m_DateNss.mid(3,2)),     // 10/12/2003  --> MM    12
                                 atoi(pIdentPat->m_DateNss.mid(0,2))      // 10/12/2003  --> DD    10
                               );
         int age = atoi(m_pCMoteurCCAM_Base->CalculeDiffAge(&qDteNss, 0, FALSE, 2));
         if (age < 15)                         resolvToken=tr("L'enfant");
         if (age >= 15 && age <= 21)
            {if (pIdentPat->m_Sexe=="F")       resolvToken=tr("Mademoiselle");
             else                              resolvToken=tr("Mr");
            }
         if (age>21)
            {if (pIdentPat->m_Sexe=="F")       resolvToken=tr("Mme");
             else                              resolvToken=tr("Mr");
            }
        }
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "VILLE PATIENT")
    {     resolvToken = pIdentPat->m_Ville;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "VILLE NAISSANCE PATIENT")
    {     resolvToken = pIdentPat->m_VilleNss;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "CODE POSTAL PATIENT")
    {resolvToken = pIdentPat->m_CodePostal;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "ADRESSE PATIENT 1")
    {resolvToken = pIdentPat->m_Adresse1;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "ADRESSE PATIENT 2")
    {resolvToken = pIdentPat->m_Adresse2;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "ADRESSE COMPLETE PATIENT")
    {resolvToken  = pIdentPat->m_Adresse1 + "\n";
     if (pIdentPat->m_Adresse2.stripWhiteSpace() != "" ) resolvToken += pIdentPat->m_Adresse2 + " \n";
     resolvToken += pIdentPat->m_CodePostal + " " + pIdentPat->m_Ville;
     resolvToken.replace("\n","<br />");
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "ADRESSE PATIENT UNE LIGNE")
    {resolvToken  = pIdentPat->m_Adresse1 + " ";
     if (pIdentPat->m_Adresse2.stripWhiteSpace() != "" ) resolvToken += pIdentPat->m_Adresse2 + "  ";
     resolvToken += pIdentPat->m_CodePostal + " " + pIdentPat->m_Ville;
     resolvToken.replace("\n"," ");
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "NUM. SECU PATIENT")
    {resolvToken = pIdentPat->m_NumSecu;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "NOM PATIENT")
    {resolvToken = pIdentPat->m_Nom;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "NOM NAISSANCE PATIENT")
    {resolvToken = pIdentPat->m_NomNaissance;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "PRENOM PATIENT")
    {resolvToken = pIdentPat->m_Prenom;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if (token== "NOM PRENOM PATIENT")
    {resolvToken  = pIdentPat->m_Nom + " " + pIdentPat->m_Prenom;
     document.replace ( pos_deb-2, len+4, resolvToken);
    }
 else if ((f=token.find('X'))!=-1)   // syntaxe {{ X20}}  ou {{\tX20}}
   {int        nb = atoi(token.mid(f+1));
    QString motif = token.left(f);
    if (motif != "" )
       {if (motif[0]=='\\')  // echapement exple "\t"
           {
            char escape_car = motif.at(1).latin1();
            switch(escape_car)
              {case 't':
                    resolvToken.fill('\t', nb);
                    break;
               case 'n':
                    resolvToken.fill('\n', nb);
                    break;
               case '\\':
                    resolvToken.fill('\\', nb);
                    break;
               case 'r':
                    resolvToken.fill('\r', nb);
                    break;
               default:
                    resolvToken.fill(motif[1], nb);
                    break;
              } // endswitch(token[1])
           } //endif (token[0]=='\\')  // echapement
        else
           {resolvToken.fill(motif[0], nb);
           }
        document.replace ( pos_deb-2, len+4, resolvToken);
       }// endif (motif != "" )
    else
       {document.replace ( pos_deb-2, len+4, resolvToken);
       }
   }

 return pos_deb-2+resolvToken.length();
}

//-------------------------------------- Get_Head_Foot --------------------------------------------------
QString C_Dlg_FeuillePrincipale::Get_Head_Foot( QString * foot )
{QString path = GlobalPathAppli + SYS_FOLDER + "/";
 QString headFile("");
 if (CGestIni::Param_ReadParam(Global_ParamData,  comboBox_Bases->currentText(), "PrintHead", &headFile) !=QString::null )             // zero = pas d'erreur
    {headFile = "Head.html";
    }
 QString footFile("");
 if (CGestIni::Param_ReadParam(Global_ParamData,  comboBox_Bases->currentText(), "PrintFoot", &footFile) !=QString::null )             // zero = pas d'erreur
    {footFile = "Foot.html";
    }
 QString str ;
 CGestIni::Param_UpdateFromDisk(path + footFile, str);
 if (foot) *foot = str;
 CGestIni::Param_UpdateFromDisk(path + headFile, str);
 return str;
}

//-------------------------------------- Get_Export_Head_Foot ------------------------------------------------------------------------------
QString C_Dlg_FeuillePrincipale::Get_Export_Head_Foot()
{QString path = GlobalPathAppli + SYS_FOLDER + "/";
 QString headFile("");
 if (CGestIni::Param_ReadParam(Global_ParamData,  comboBox_Bases->currentText(), "ExportHtml", &headFile) !=QString::null )             // zero = pas d'erreur
    {headFile = "ExportModele.html";
    }
 QString ret;
 CGestIni::Param_UpdateFromDisk(path + headFile, ret);
 return ret;
}

//-------------------------------------- SetToNum --------------------------------------------------
QString C_Dlg_FeuillePrincipale::SetToNum(QString txt)
{if (m_NumKeyboardMap.length()!=0)
    {int pos = -1;
     int len = txt.length();
     for (int i=0; i<len; ++i)
         {if ( (pos = m_NumKeyboardMap.find(txt[i])) != -1 )
             {txt[i] = pos+48;
             }
         }
    }
 return txt;
}

//-------------------------------- MessageAlert -------------------------------------------------------------------------
int C_Dlg_FeuillePrincipale::MessageAlert( const char * title, const char * message, const char * button1, const char * button2, const char * button3, const char * button4 )
{C_Dlg_MyAlertBox *dlg = new C_Dlg_MyAlertBox(0,0,TRUE);   // TRUE Pour modal
 if (dlg ==0) return -1;
 dlg->pushButton1->setShown(FALSE);
 dlg->pushButton2->setShown(FALSE);
 dlg->pushButton3->setShown(FALSE);
 dlg->pushButton4->setShown(FALSE);

 if (title)    dlg->setCaption(title);
 if (message)  dlg->textEditMesage->setText(message);
 if (button1) {dlg->pushButton1->setShown(TRUE);dlg->pushButton1->setText(button1);}
 if (button2) {dlg->pushButton2->setShown(TRUE);dlg->pushButton2->setText(button2);}
 if (button3) {dlg->pushButton3->setShown(TRUE);dlg->pushButton3->setText(button3);}
 if (button4) {dlg->pushButton4->setShown(TRUE);dlg->pushButton4->setText(button4);}

 int ret = dlg->exec();
 delete dlg;
 return ret;
}

//------------------------------------------- StrDateToQDate --------------------------------------------------------------------
QDate C_Dlg_FeuillePrincipale::StrDateToQDate( QString date_acte, int & error )
{error = 0;
 QDate date;
 //date_acte = date_acte.stripWhiteSpace();
 if (date_acte.length()>=10)
    {QDate date = QDate ( atoi(date_acte.mid(6,4)),     // 10/12/2003  --> YYYY  2003
                          atoi(date_acte.mid(3,2)),     // 10/12/2003  --> MM    12
                          atoi(date_acte.mid(0,2))      // 10/12/2003  --> DD    10
                        );
     if (! date.isValid())
        {error = 1;                // erreur date  " Date Acte invalide "
        }
    }
 else
    {error = 2;                    // erreur date " Date Acte invalide et ne comporte pas assez de caract\303\250res"
    }
 if (error) return QDate( );
 else       return date;
}

//------------------------------------------ GetDateEntreeSortie ---------------------------------------------------------------
int C_Dlg_FeuillePrincipale::GetDateEntreeSortie( QDate & dateEntree, QDate & dateSortie, QString numDoss )
{if (m_ModeFlag&LOCAL_MODE)
    {dateEntree = dateSortie = QDate::currentDate ();
     return 0;
    }

 if (numDoss=="") return 16;
 int error = 0;
 Q3ListViewItemIterator it ( listview_ListeDossPat );
 while ( it.current() )
     {Q3ListViewItem *item = it.current();
      if (item->text(0)== numDoss)
         {//................. recuperer date entree ....................
          //                  dans listview des dossiers hospi
          QString dt_Entree = item->text(2).stripWhiteSpace();
          if (dt_Entree.length()<10) return 12;
          dateEntree = QDate     ( atoi(dt_Entree.mid(6,4)),     // 10/12/2003  --> YYYY  2003
                                   atoi(dt_Entree.mid(3,2)),     // 10/12/2003  --> MM    12
                                   atoi(dt_Entree.mid(0,2))      // 10/12/2003  --> DD    10
                                 );
          //dateEntree = StrDateToQDate(item->text(2), error);
          //if (error)                    return error+10;
          //................. recuperer date sortie ....................
          //                  dans listview des dossiers hospi
          QString dt_Sortie = item->text(3).stripWhiteSpace();
          if (dt_Sortie==""||dt_Sortie.length()<10)   //...... si vide (patient en cours d'hospi) rajouter 1 an
             {dateSortie = dateEntree;
              dateSortie.setYMD(dateSortie.year()+1,dateSortie.month(),dateSortie.day());
             }
          else
             {dateSortie = QDate ( atoi(dt_Sortie.mid(6,4)),     // 10/12/2003  --> YYYY  2003
                                   atoi(dt_Sortie.mid(3,2)),     // 10/12/2003  --> MM    12
                                   atoi(dt_Sortie.mid(0,2))      // 10/12/2003  --> DD    10
                                 );
             }
          //................... si date de sortie invalide ............
          //                    entree plus 1 an
          if (! dateSortie.isValid())
             {dateSortie = dateEntree;
              dateSortie.setYMD(dateSortie.year()+1,dateSortie.month(),dateSortie.day());
             }
          return 0;
         }
      ++it;
     }
 return 15;          // pas de dossier correspondant \303\264 ce num\303\251ro de dossier

}

//------------------------------------------ GetSexePatient --------------------------------------------------------------------
QString C_Dlg_FeuillePrincipale::GetSexePatient()
{return lineEdit_Sexe->text();
}

//------------------------------------------ IsThisActeInListViewActe -------------------------------------------------------------
Q3ListViewItem * C_Dlg_FeuillePrincipale::IsThisActeInListViewActe( const QString & acte, const QDateTime dateTimeEntree, const QDateTime dateTimeSortie, CCAM_ACTE * pCCAMActe )
{Q3ListViewItemIterator it ( listview_ListeActePat );
 while ( it.current() )
     {Q3ListViewItem *item = it.current();
      //......................................... version tenant compte des heures ........................................................
      /*
      QDate dt_acte =  QDate     ( atoi(item->text(LVA_DATE).mid(6,4)),     // 10/12/2003  --> YYYY  2003
                                   atoi(item->text(LVA_DATE).mid(3,2)),     // 10/12/2003  --> MM    12
                                   atoi(item->text(LVA_DATE).mid(0,2))      // 10/12/2003  --> DD    10
                                 );

      QTime hr_acte =  QTime     ( atoi(item->text(LVA_HEURE).mid(0,2)),     // 10:12:20    --> hh    10
                                   atoi(item->text(LVA_HEURE).mid(3,2)),     // 10:12:20    --> mm    12
                                   atoi(item->text(LVA_HEURE).mid(6,2))      // 10:12:20    --> ss    20
                                 );

      QDateTime dateTimeActe( dt_acte, hr_acte );

      if (pCCAMActe)
         {if ( dateTimeActe >= dateTimeEntree && dateTimeActe <= dateTimeSortie && acte==item->text(LVA_CODECCAM) &&
               pCCAMActe->m_Activite==item->text(LVA_ACTIVITE))                                                         return item;
         }
      else
         {if ( dateTimeActe >= dateTimeEntree && dateTimeActe <= dateTimeSortie && acte==item->text(LVA_CODECCAM))      return item;
         }
      ++it;
     */
      //.............................. version ne tenant pascompte des heures .........................................................
      QDate dt_acte =  QDate     ( atoi(item->text(LVA_DATE).mid(6,4)),     // 10/12/2003  --> YYYY  2003
                                   atoi(item->text(LVA_DATE).mid(3,2)),     // 10/12/2003  --> MM    12
                                   atoi(item->text(LVA_DATE).mid(0,2))      // 10/12/2003  --> DD    10
                                 );
      if (pCCAMActe)
         {if ( dt_acte >= dateTimeEntree.date() && dt_acte <= dateTimeSortie.date() && acte==item->text(LVA_CODECCAM) &&
               pCCAMActe->m_Activite==item->text(LVA_ACTIVITE))                                                             return item;
         }
      else
         {if ( dt_acte >= dateTimeEntree.date() && dt_acte <= dateTimeSortie.date() && acte==item->text(LVA_CODECCAM))      return item;
         }
      ++it;

     }
 return 0;
}

//------------------------------------------ IsWarningActe ------------------------------------------------------------------------
// \brief ACTION: detection des erreurs relatives devant faire l'objet d'un warning avec confirmation
int C_Dlg_FeuillePrincipale::IsWarningActe( Q3ListView * pQListView, CCAM_ACTE & ccamActe, QDateTime dateTimeEntree, QDateTime dateTimeSortie, QString * pErrorStr )
{if (pErrorStr) *pErrorStr = "";
 int err = 0;
 //................................. verifier si cet acte est deja dans la listview ...................................................
 // un meme acte ne peut etre pr\303\251sent dans la
 Q3ListViewItem* pQListViewItem = IsThisActeInListViewActe(ccamActe.m_CodeCCAM, dateTimeEntree, dateTimeSortie, &ccamActe);
 if ( pQListViewItem )
    {err = 110;
     if (pErrorStr==0) return err; // "\nUn acte incompatible avec cet acte est d\303\251j\303\240 pr\303\251sent dans les actes de ce patient"
     else              pErrorStr->append (tr("\n<b>ATTENTION: Acte double :</b>\n\n                  <u>")             +
                                          ccamActe.m_CodeCCAM   + tr("</u>   Activit\303\251 : ") + pQListViewItem->text(LVA_ACTIVITE) + "    <i>" +
                                          pQListViewItem->text(LVA_LIBELLEACTE)                                                            +
                                          tr("\n\n</i><b>                  est d\303\251j\303\240 pr\303\251sent dans les actes de ce patient</b>"));

     }
 if (*pErrorStr != "")  return err;
 return 0;
}

//------------------------------------------ IsThisActeInListViewActeModifier -------------------------------------------------------------
Q3ListViewItem * C_Dlg_FeuillePrincipale::IsThisActeInListViewActeModifier( const QString & acte, const QDateTime dateTimeEntree, const QDateTime dateTimeSortie, CCAM_ACTE * pCCAMActe, Q3ListViewItem * item_to_modif )
{Q3ListViewItemIterator it ( listview_ListeActePat );
 while ( it.current() )
     {//......................................... version tenant compte des heures ........................................................
      /*
      QListViewItem *item = it.current();
      QDate dt_acte =  QDate     ( atoi(item->text(LVA_DATE).mid(6,4)),     // 10/12/2003  --> YYYY  2003
                                   atoi(item->text(LVA_DATE).mid(3,2)),     // 10/12/2003  --> MM    12
                                   atoi(item->text(LVA_DATE).mid(0,2))      // 10/12/2003  --> DD    10
                                 );
      QTime hr_acte =  QTime     ( atoi(item->text(LVA_HEURE).mid(0,2)),     // 10:12:20    --> hh    10
                                   atoi(item->text(LVA_HEURE).mid(3,2)),     // 10:12:20    --> mm    12
                                   atoi(item->text(LVA_HEURE).mid(6,2))      // 10:12:20    --> ss    20
                                 );
      QDateTime dateTimeActe( dt_acte, hr_acte );
      if (item != item_to_modif)
         {if (pCCAMActe)
             {if ( dateTimeActe >= dateTimeEntree && dateTimeActe <= dateTimeSortie && acte==item->text(LVA_CODECCAM) &&
                   pCCAMActe->m_Activite==item->text(LVA_ACTIVITE))                                                        return item;
             }
          else
             {if ( dateTimeActe >= dateTimeEntree && dateTimeActe <= dateTimeSortie && acte==item->text(LVA_CODECCAM))     return item;
             }
         }
      ++it;
      */
      //......................................... version ne tenant pas compte des heures ........................................................
      Q3ListViewItem *item = it.current();
      QDate dt_acte =  QDate     ( atoi(item->text(LVA_DATE).mid(6,4)),     // 10/12/2003  --> YYYY  2003
                                   atoi(item->text(LVA_DATE).mid(3,2)),     // 10/12/2003  --> MM    12
                                   atoi(item->text(LVA_DATE).mid(0,2))      // 10/12/2003  --> DD    10
                                 );
      if (item != item_to_modif)
         {if (pCCAMActe)
             {if ( dt_acte >= dateTimeEntree.date() && dt_acte <= dateTimeSortie.date() && acte==item->text(LVA_CODECCAM) &&
                   pCCAMActe->m_Activite==item->text(LVA_ACTIVITE))                                                            return item;
             }
          else
             {if ( dt_acte >= dateTimeEntree.date() && dt_acte <= dateTimeSortie.date() && acte==item->text(LVA_CODECCAM))     return item;
             }
         }
      ++it;
     }
 return 0;
}
//------------------------------------------ IsWarningActeModifier ------------------------------------------------------------------------
// \brief ACTION: detection des erreurs relatives devant faire l'objet d'un warning avec confirmation
int C_Dlg_FeuillePrincipale::IsWarningActeModifier( Q3ListView * pQListView, CCAM_ACTE & ccamActe, QDate dateEntree, QDate dateSortie, Q3ListViewItem * item_to_modif, QString * pErrorStr )
{if (pErrorStr) *pErrorStr = "";
 int err = 0;
 //................................. verifier si cet acte est deja dans la listview ...................................................
 // un meme acte ne peut etre pr\303\251sent dans la
 Q3ListViewItem* pQListViewItem = IsThisActeInListViewActeModifier(ccamActe.m_CodeCCAM, QDateTime(dateEntree), QDateTime(dateSortie), &ccamActe, item_to_modif);
 if ( pQListViewItem )
    {err = 110;
     if (pErrorStr==0) return err; // "\nUn acte incompatible avec cet acte est d\303\251j\303\240 pr\303\251sent dans les actes de ce patient"
     else              pErrorStr->append (tr("\n<b>ATTENTION: Acte double :</b>\n\n                  <u>")              +
                                          ccamActe.m_CodeCCAM   + tr("</u>   Activit\303\251 : ") + pQListViewItem->text(5) + "    <i>" +
                                          pQListViewItem->text(LVA_LIBELLEACTE)                                             +
                                          tr("\n\n</i><b>                  est d\303\251j\303\240 pr\303\251sent dans les actes de ce patient</b>"));

     }
 if (*pErrorStr != "")  return err;
 return 0;
}

//------------------------------------------ VerifieModificateur ------------------------------------------------------------------
bool C_Dlg_FeuillePrincipale::VerifieModificateur( QStringList & strList, QString mod )
{if (mod=="") return TRUE; // si zero c'est qu'il n'a pas \303\251t\303\251 plac\303\251 de modificateur donc pas d'erreur
 bool ok = FALSE;
 for ( QStringList::Iterator lit = strList.begin(); lit != strList.end(); ++lit )
     {if (*lit == mod) ok = TRUE;
     }
 return ok;
}
//------------------------------------------ VerifieActe --------------------------------------------------------------------------------
// \brief ACTION: detection des erreurs absolues blocantes
int C_Dlg_FeuillePrincipale::VerifieActe( Q3ListView * pQListView, CCAM_ACTE & ccamActe, QDate dateEntree, QDate dateSortie, QString * pErrorStr )

{if (pErrorStr) *pErrorStr = "";
 if  (!ccamActe.m_Date.isValid())
 {if (pErrorStr==0) return 100;
  else              pErrorStr->append (tr("\n<b>ERREUR dans la date : </b> la date de l'acte est invalide </b>"));
 }
 //................... verifier si date acte est dans la bonne fourchette ................................................
 if  (!(m_ModeFlag&LOCAL_MODE) && ( ccamActe.m_Date < dateEntree || ccamActe.m_Date > dateSortie))
     {if (pErrorStr==0)  return 101; // "Erreur date acte non comprise entre entree et sortie "
      else               pErrorStr->append (        tr("\n<b>ERREUR dans la date :</b>") +
                                                    tr("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;la date de l'acte :   ") + ccamActe.m_Date.toString("dd-MM-yyyy")  + tr(" est en dehors") +
                                                    tr("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;de la date d'entree : ") + dateEntree .toString("dd-MM-yyyy")      +
                                                    tr("<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;et de sortie :        ") + dateSortie .toString("dd-MM-yyyy")      + tr(" du patient "));
     }
 //........................... conversion de la date pour verif incompatibilites et  modificateurs possibles .............      // ajout\303\251 le 5/2/2006
 QString s_date = ccamActe.m_Date.toString("yyyyMMdd");                                                                         // ajout\303\251 le 5/2/2006
 //................... verifier si des actes incompatibles ne sont pas deja dans la liste du patient .....................
 QStringList  strList;
 if (m_pCMoteurCCAM_Base->GetActesIncompatibles(strList,  ccamActe.m_CodeCCAM, s_date))                                         // ajout\303\251 le 5/2/2006
    {for ( QStringList::Iterator lit = strList.begin(); lit != strList.end(); ++lit )
         {Q3ListViewItem* pQListViewItem = IsThisActeInListViewActe(*lit, QDateTime(dateEntree), QDateTime(dateSortie), 0);
          if ( pQListViewItem )
             {if (pErrorStr==0) return 102;
              else              pErrorStr->append (tr("\n<b>ERREUR Compatibilit\303\251 : </b> Un acte incompatible est pr\303\251sent: \n\n                  <u>")  +
                                                   ccamActe.m_CodeCCAM                   + "</u><i>   " +
                                                   pQListViewItem->text(LVA_LIBELLEACTE) + "</i>\n\n");
             }
         }
    }
 //................... verifier si le medecin peut pratiquer cet acte ......................................................
 INFO_MED info_med;
 m_pCMoteurCCAM_Base->GetInfoMedecinsFromNid(ccamActe.m_NidPraticien, info_med);
 if (ccamActe.m_Activite=="4" && info_med.m_Spec !="02")
    {if (pErrorStr==0) return 103;
     else              pErrorStr->append (tr("\n<b>ERREUR Activit\303\251 : </b> Vous ne pouvez pas pratiquer un acte d'anesth\303\251sie (Activit\303\251 4) "));
    }
 //................... verifier si la phase de cet acte est possible .......................................................
 strList.clear();
 if (m_pCMoteurCCAM_Base->GetPhasesActe(strList, ccamActe.m_CodeCCAM))
    {bool ok = FALSE;
     for ( QStringList::Iterator lit = strList.begin(); lit != strList.end(); ++lit )
         {if (*lit == ccamActe.m_Phase)   ok = TRUE;
         }
     if (ok==FALSE)
         {if (pErrorStr==0) return 104;
          else              pErrorStr->append (tr("\n<b>ERREUR phase : </b> cet acte : ne permet pas la phase : ")+ ccamActe.m_Phase);
         }
    }
 //................... verifier si l'activit\303\251 de cet acte est possible .....................................................
 strList.clear();
 if (m_pCMoteurCCAM_Base->GetActiviteActe(strList, ccamActe.m_CodeCCAM))
    {bool ok = FALSE;
     for ( QStringList::Iterator lit = strList.begin(); lit != strList.end(); ++lit )
         {if (*lit == ccamActe.m_Activite) ok = TRUE;
         }
     if (ok==FALSE)
        {if (pErrorStr==0) return 105;
         else              pErrorStr->append (tr("\n<b>ERREUR Activit\303\251 : </b> cet acte: ne permet pas l'activit\303\251 : ")+ ccamActe.m_Activite);
        }
    }
 //................... verifier si  cet acte est compatible avec le sexe du patient .....................................................
 //                    nos actes ont un sexe (Freud)
 QString sexe_acte;
 m_pCMoteurCCAM_Base->GetSexeActe(sexe_acte, ccamActe.m_CodeCCAM);
 if (sexe_acte != "0")
    {QString sexe_patient = GetSexePatient();
     if (sexe_acte == "1" && sexe_patient=="F")
        {if (pErrorStr==0) return 116;
         else              pErrorStr->append (tr("\n<b>ERREUR Sexe : </b> Acte incompatible avec le sexe F\303\251minin \n\n                  <u>") +
                                              ccamActe.m_CodeCCAM     + "</u><i>   " +
                                              ccamActe.m_LibelleActe  + "</i>\n\n"
                                             );
        }
     else if (sexe_acte == "2" && sexe_patient=="M")
        {if (pErrorStr==0) return 117;
         else              pErrorStr->append (tr("\n<b>ERREUR Sexe : </b> Acte incompatible avec le sexe Masculin \n\n                  <u>") +
                                              ccamActe.m_CodeCCAM     + "</u><i>   " +
                                              ccamActe.m_LibelleActe  + "</i>\n\n"
                                             );
        }
    }
 //................... verifier si les modificateurs de cet acte sont possibles ........................................................
 strList.clear();
 if (m_pCMoteurCCAM_Base->GetModificateurActe(strList, ccamActe.m_CodeCCAM, s_date))                                               // ajout\303\251 le 5/2/2006
    {if (VerifieModificateur(strList, ccamActe.m_Modificateur1) == FALSE)
        {if (pErrorStr==0) return 106;
         else              pErrorStr->append (tr("\n<b>ERREUR Modificateur 1 : </b> cet acte: ne permet pas le modificateur : ")+ ccamActe.m_Modificateur1);
        }
     if (VerifieModificateur(strList, ccamActe.m_Modificateur2) == FALSE)
        {if (pErrorStr==0) return 107;
         else              pErrorStr->append (tr("\n<b>ERREUR Modificateur 2 : </b> cet acte: ne permet pas le modificateur : ")+ ccamActe.m_Modificateur2);
        }
     if (VerifieModificateur(strList, ccamActe.m_Modificateur3) == FALSE)
        {if (pErrorStr==0) return 108;
         else              pErrorStr->append (tr("\n<b>ERREUR Modificateur 3 : </b> cet acte: ne permet pas le modificateur : ")+ ccamActe.m_Modificateur3);
        }
     if (VerifieModificateur(strList, ccamActe.m_Modificateur4) == FALSE)
        {if (pErrorStr==0) return 109;
         else              pErrorStr->append (tr("\n<b>ERREUR Modificateur 4 : </b> cet acte: ne permet pas le modificateur : ")+ ccamActe.m_Modificateur4);
        }
    }
 else
    {if (ccamActe.m_Modificateur1 != "" || ccamActe.m_Modificateur2 != "" || ccamActe.m_Modificateur3 != "" || ccamActe.m_Modificateur4 != "")
        {if (pErrorStr==0) return 115;
         else              pErrorStr->append (tr("\n<b>ERREUR Modificateurs : </b> cet acte: ne permet pas de modificateur et vous en avez plac\303\251. "));
        }
    }

 //................... verifier si les modificateurs sont en doublons ..............................................................
 if (ccamActe.m_Modificateur1 != "" && (ccamActe.m_Modificateur1==ccamActe.m_Modificateur2||
                                        ccamActe.m_Modificateur1==ccamActe.m_Modificateur3||
                                        ccamActe.m_Modificateur1==ccamActe.m_Modificateur4
                                       )
    )
    {if (pErrorStr==0) return 111;
     else              pErrorStr->append (tr("\n<b>ERREUR Modificateur 1 : </b> ce modificateur est plac\303\251 plusieurs fois sur le m\303\252me acte !!! "));
    }
 if (ccamActe.m_Modificateur2 != "" && (ccamActe.m_Modificateur2==ccamActe.m_Modificateur1||
                                        ccamActe.m_Modificateur2==ccamActe.m_Modificateur3||
                                        ccamActe.m_Modificateur2==ccamActe.m_Modificateur4
                                       )
    )
    {if (pErrorStr==0) return 112;
     else              pErrorStr->append (tr("\n<b>ERREUR Modificateur 2 : </b> ce modificateur est plac\303\251 plusieurs fois sur le m\303\252me acte !!! "));
    }
 if (ccamActe.m_Modificateur3 != "" && (ccamActe.m_Modificateur3==ccamActe.m_Modificateur1||
                                        ccamActe.m_Modificateur3==ccamActe.m_Modificateur2||
                                        ccamActe.m_Modificateur3==ccamActe.m_Modificateur4
                                       )
    )
    {if (pErrorStr==0) return 113;
     else              pErrorStr->append (tr("\n<b>ERREUR Modificateur 3 : </b> ce modificateur est plac\303\251 plusieurs fois sur le m\303\252me acte !!! "));
    }
 if (ccamActe.m_Modificateur4 != "" && (ccamActe.m_Modificateur4==ccamActe.m_Modificateur1||
                                        ccamActe.m_Modificateur4==ccamActe.m_Modificateur2||
                                        ccamActe.m_Modificateur4==ccamActe.m_Modificateur3
                                       )
    )
    {if (pErrorStr==0) return 114;
     else              pErrorStr->append (tr("\n<b>ERREUR Modificateur 4 : </b> ce modificateur est plac\303\251 plusieurs fois sur le m\303\252me acte !!! "));
    }

 if (*pErrorStr != "")  return 999;
 return 0;
}
//------------------------------------------ PutActeInListView --------------------------------------------------------------------------------
Q3ListViewItem * C_Dlg_FeuillePrincipale::PutActeInListView( Q3ListViewItem * element, Q3ListViewItem * pere, CCAM_ACTE & ccamActe, const QPixmap & pixmap )
{if (element==0)
    {if (pere)   element = new Q3ListViewItem(pere);
     else        element = new Q3ListViewItem(listview_ListeActePat);
    }
 if (element)
    {element->setText ( LVA_DATE,          ccamActe.m_Date.toString  ("dd/MM/yyyy"));
     element->setText ( LVA_HEURE,         ccamActe.m_Heure.toString ("hh:mm"));
     element->setText ( LVA_PRATICIENNAME, ccamActe.m_PraticienName.stripWhiteSpace());
     element->setText ( LVA_CODECCAM,      ccamActe.m_CodeCCAM);
     element->setText ( LVA_LIBELLEACTE,   ccamActe.m_LibelleActe);
     element->setText ( LVA_TARIF,         m_pCMoteurCCAM_Base->GetPrixActe(ccamActe));
     element->setText ( LVA_PHASE,         ccamActe.m_Phase);
     element->setText ( LVA_ACTIVITE,      ccamActe.m_Activite);
     element->setText ( LVA_MODIFICATEUR1, ccamActe.m_Modificateur1);
     element->setText ( LVA_MODIFICATEUR2, ccamActe.m_Modificateur2);
     element->setText ( LVA_MODIFICATEUR3, ccamActe.m_Modificateur3);
     element->setText ( LVA_MODIFICATEUR4, ccamActe.m_Modificateur4);
     element->setText ( LVA_NB,            "1");
     element->setText ( LVA_NUMDOSS,       GetNumDossierFromEditables());
     element->setText ( LVA_NOMENCLAT,     "CCAM");
     element->setText ( LVA_NIDPRATICIEN,  ccamActe.m_NidPraticien);
     element->setText ( LVA_TYP,  "N");
     element->setText ( LVA_ENFANT,  "0");
     element->setText ( LVA_PRESCRIPTEUR,  "");
     element->setText ( LVA_CODE_REJET,  "");
     element->setPixmap ( 0, pixmap );
    }
 return element;
}

//------------------------------------------------------- CalculePrixTotal -------------------------------------------------------------
QString C_Dlg_FeuillePrincipale::CalculePrixTotal()
{/*
  int nb             = 0;
  int isMainActeUsed = 0;
  double       tarif = 0;
  double    tarifMax = 0;
  double       taux  = 1;
  QListViewItemIterator it( listview_ListeActePat );
  while ( it.current() && nb < 4)
        {   tarif     +=
            QListViewItem *pQListViewItem = it.current();
            if ( pQListViewItem->childCount() )
               {QListViewItem * myChild = myItem->firstChild();
                while( myChild )
                     {

                      myChild = myChild->nextSibling();
                     }
               }
            else
               {
               }
            ++it;
        }

 */
return QString("");
}

//------------------------------------------ GetNumDossierFromEditables -------------------------------------------------------------
QString C_Dlg_FeuillePrincipale::GetNumDossierFromEditables()
{if (m_ModeFlag & LOCAL_MODE) return QString("");
 QString an = lineedit_NumDossAn->text();
 QString nu = lineedit_NumDoss->text();
 if (an.length() != 2)
    {QMessageBox::critical (this, tr(" Erreur CCAM_View") ,
                            tr("ERREUR dans la saisie du Num\303\251ro de Dossier :\n        la partie 'ann\303\251e' du num\303\251ro de dossier doit\n        comporter DEUX caract\303\250res\n"),
                            QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     SetFocusOnNumDossAn();
     return QString("0");
    }
 if ( nu.length() > 5)
    {QMessageBox::critical (this, tr(" Erreur CCAM_View") ,
                            tr("ERREUR dans la saisie du Num\303\251ro de Dossier :\n        la partie de droite du num\303\251ro de dossier doit\n        comporter au maximum CINQ caract\303\250res\n"),
                            QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     SetFocusOnNumDoss();
     return QString("0");
    }
 nu = QString("00000") + nu;
 nu = nu.right(5);
 lineedit_NumDoss->setText(nu);
 return an + nu;
}

//------------------------------------------ IsSameActe --------------------------------------------------------------
bool C_Dlg_FeuillePrincipale::IsSameActe( Q3ListViewItem * pQListViewItem, QStringList & data_list )
{if ( pQListViewItem == 0) return FALSE;

 int nb_columns = pQListViewItem->listView()->columns();
 for (int i=0; i<nb_columns; ++i)
     {if (i != LVA_TYP)
         { if (pQListViewItem->text(i) != data_list[i]) return FALSE;
         }
     }
 return TRUE;
}

//------------------------------------------ IsThisActeInThesaurus ------------------------------------------------
int C_Dlg_FeuillePrincipale::IsThisActeInThesaurus( Q3ListView * pQListView, QString codeActe )
{Q3ListViewItemIterator it ( pQListView );
 while ( it.current() )
     {Q3ListViewItem *item = it.current();
      if (item->text(2) == codeActe) return 1;
      ++it;
     }
 return 0;
}

//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString C_Dlg_FeuillePrincipale::DoPopupList( QStringList & list )
{m_CurrentPopupItem = "";
 m_RetPopupItem     = "";
 if (m_pQPopupMenu)          return m_RetPopupItem;
 m_pQPopupMenu      = new Q3PopupMenu(0, "MyPopupMenu" );
 if (m_pQPopupMenu==0)       return m_RetPopupItem;
 //............................... chercher une zone de liste deroulante (bleue) .....................
 for ( QStringList::Iterator it =list.begin(); it != list.end(); ++it )
    { if (*(it)=="---------")  m_pQPopupMenu->insertSeparator();
      else                     m_pQPopupMenu->insertItem( *(it),    this, SLOT( GetPopupItem()),  0 );
    }
 //................. connecter le menu .................................................................
 connect( m_pQPopupMenu, SIGNAL( activated( int)),
          this,          SLOT(OnMenuActivated( int))
        );

 m_pQPopupMenu->exec( QCursor::pos() /* mapToGlobal (  QPoint ( x, y) )*/ );
 delete  m_pQPopupMenu;
 m_pQPopupMenu = 0;
 return  m_RetPopupItem;

}

//------------------------------------------ GetNidComboPraticiens ------------------------------------------------
QString C_Dlg_FeuillePrincipale::GetNidComboPraticiens()
{
 int         index = combo_Praticiens->currentIndex(); if (index == -1) return QString::null;
 QVariant       qv = combo_Praticiens->itemData (index);
 QString      text = qv.toString();
 int             f = text.findRev("-");
 if (f != -1) return text.mid(f+1,9);
 return QString::null;
}

//------------------------------------------ GetCodeComboPraticiens ------------------------------------------------
// \brief echerche le code activit\303\251 dans le combobox praticien
QString C_Dlg_FeuillePrincipale::GetCodeComboPraticiens()
{int         index = combo_Praticiens->currentIndex(); if (index == -1) return QString::null;
 QVariant       qv = combo_Praticiens->itemData (index);
 QString      text = qv.toString();
 int             f = text.findRev("-");
 if (f != -1) return text.left(f);
 return QString::null;
}

//------------------------------------------ GetComboActivite ----------------------------------------------------------------------------------
// \brief recherche le code activit\303\251 dans le combobox d'activit\303\251
QString C_Dlg_FeuillePrincipale::GetComboActivite()
{QString            qtext = comboBoxActivite->currentText ();
    int                    f = qtext.findRev( '-' );   // avant dernier espace separe indice de la liste des rubriques du reste
    if (f==-1)  return QString("");
    return qtext.mid(f+1);   // recuperer indice dans la liste des rubriques
}

//------------------------------------------ initDialog ----------------------------------------------------------------------------------
int C_Dlg_FeuillePrincipale::initDialog( CMoteurCCAM_Base * pCMoteurCCAM_Base, const QString & code_utilisateur )
{   m_ModeFlag = 0;
    m_pCMoteurCCAM_Base = pCMoteurCCAM_Base;
    m_pCMoteurCCAM_Base->CreeTableMedecins();
    //.................................... interface sur recherche avancee ou non ..................................
    QString str;
    if (CGestIni::Param_ReadParam(Global_ParamData,  "SigemsWebServices", tr("RechercheAvanc\303\251e"), &str) == QString::null)  // zero = pas d'erreur
       {QString val = str.left(1).upper();
        if (val=="O"||val=="Y") m_ModeFlag |= FULL_FIND;
       }

    #ifdef Q_WS_X11
    //.................... ajuster resolution d'impression .........................................
    QString str2;
    //if (CGestIni::Param_ReadParam(Global_ParamData, "Impression", "Resolution", &str, &str2)==QString::null)  // zero = pas d'erreur
    //   {QPaintDevice::x11SetAppDpiX(atoi(str)-1);
    //    QPaintDevice::x11SetAppDpiY(atoi(str2)-1);
    //   }
    int screen = QX11Info().screen();
    //QString reso_x(""), reso_y("");
    if (CGestIni::Param_ReadParam(Global_ParamData, "Impression", "Resolution", &str, &str2)==QString::null)  // zero = pas d'erreur
       {QPaintDevice::x11SetAppDpiX(str.toInt()-1,screen);
        QPaintDevice::x11SetAppDpiY(str2.toInt()-1,screen);
       }
    #endif
     //.................................... mode temps reel Sigems Web-Services..............................
     if (CGestIni::Param_ReadParam(Global_ParamData,  "SigemsWebServices", tr("Enregistrement temps r\303\251el"), &str) ==QString::null )  // pas egal zero = erreur
       {QString val = str.left(1).upper();
        if (val=="O"||val=="Y")  m_ModeFlag |= SIGEMS_REAL_TIME;
       }
    //.................................... version Sigems Web-Services..............................
    if (CGestIni::Param_ReadParam(Global_ParamData,  "SigemsWebServices", "Version", &m_SigemsVers) !=QString::null )  // pas egal zero = erreur
       {m_SigemsVers = "00";
       }
    if (m_SigemsVers== "00") pushButtonRecActesListToSigems->setEnabled(FALSE);
    //......................... login et password Sigems ...........................................
    QString login;
    QString password;

    if (CGestIni::Param_ReadParam(Global_ParamData,  "SigemsWebServices", "Login", &login) ==QString::null )  // zero = pas d'erreur
       { m_SigemsLogin    = login;
       }
    if (CGestIni::Param_ReadParam(Global_ParamData,  "SigemsWebServices", "Password", &password) ==QString::null )  // zero = pas d'erreur
       { m_SigemsPassword = password;
       }
    // M\303\251morisation du code utilisateur ............................................................
    m_Code_Utilisateur = code_utilisateur;
    // M\303\251morise les droits utilisateurs ............................................................
    /*m_Droit_Utilisateur = GetDroitUtilisateur(m_Code_Utilisateur);*/
    // Rempli la combobox Praticiens ...............................................................
    //................................. lire le mapping numerique du clavier .......................
    CGestIni::Param_ReadParam(Global_ParamData,  "NumKeyboardMap", "MapField", &m_NumKeyboardMap);

    //.............. D\303\251fini le praticien en cours ..................................................
    QString nid="";
    if (m_Code_Utilisateur == "SuperMan-999999999")   //SuperMan-999999999
       {nid = "999999999";
       }
    else
       {nid = m_pCMoteurCCAM_Base->GetNidPraticiens(m_Code_Utilisateur);
       }
    if ( nid.left(3) == "999")
       { // ........... si secretaire: combobox avec tous les praticiens ..........................
        m_pCMoteurCCAM_Base->GetComboPraticiens(combo_Praticiens);
        m_Code_Praticien = GetCodeComboPraticiens(); // le praticien est le premier de la liste
        combo_Praticiens->setEnabled(TRUE);
       }
    else
       { //............ sinon medecin: combobox avec seulement lui ................................
        m_Code_Praticien = m_Code_Utilisateur;       // le praticien est l'utilisateur
        INFO_MED info_med;
        m_pCMoteurCCAM_Base->GetInfoMedecinsFromCode(m_Code_Praticien, info_med);
        combo_Praticiens->clear();
        combo_Praticiens->insertItem( 0, info_med.m_Nom    + " " + info_med.m_Prenom,
                                         info_med.m_Code   + "-" +  info_med.m_Nid
                                    );
        //statusBar()->message(tr("Bienvenue sur CCAM View: ") + name, 50000 );
       }
    //Initialisation de la listview pour le retour des codes trouv\303\251s par la recherche
    lview_ActesTrouves->clear();
    edit_MotCle->setFocus();
    //.................. remplir combo box d'activit\303\251 ........................
    FillComboActivite();
    //.................. remplir combo box de date d'effet ........................
    FillComboDateEffet();
    //.................. remplir listview hierarchie .............................................................................
    m_pCMoteurCCAM_Base->initArbo01(lview_Hierarchie);

    //.................. remplir listview hierarchie enti\303\251rement ............ ...................................................
    // ajouter un param\303\250tre dans le fichier ini
    // car peut ramer sur certaines machines
    QString arbo_complete_demarrage="";
    CGestIni::Param_ReadParam(Global_ParamData,  "Arborescence", "Complete", &arbo_complete_demarrage);  // zero = pas d'erreur

    if (arbo_complete_demarrage =="1"){
        Q3ListViewItemIterator it(lview_Hierarchie);
        while ( it.current() ) {
            Q3ListViewItem *item = it.current();
            m_pCMoteurCCAM_Base->initArbo02(item,item->text(3));
            ++it;
        }
    }

    //........................ recuperer URL SigemwWebServices ...........................................................
    if (CGestIni::Param_ReadParam(Global_ParamData,  "SigemsWebServices", "Url", &str) ==QString::null )             // zero = pas d'erreur
       {m_UrlSigemsWebServices  = str;
       }
    //........................ activation du monitoring XML ..............................................................
    if (CGestIni::Param_ReadParam(Global_ParamData,  "SigemsWebServices", "XML_Monitoring", &str) ==QString::null )  // zero = pas d'erreur
       { QString val = str.left(1).upper();
        if (val=="O"||val=="Y") m_ModeFlag  |= XML_MONITORING;
       }

    //........................ activation du Mode test pour Sigems ..............................................................
    if (CGestIni::Param_ReadParam(Global_ParamData,  "SigemsWebServices", "Test", &str) ==QString::null )             // zero = pas d'erreur
       {
        if (str.left(1).upper()=="O" || str.left(1).upper()=="Y")
           {checkBoxTestMode->setChecked (TRUE);
            checkBoxTestMode->show();
           }
        else
           {checkBoxTestMode->setChecked (FALSE);
            checkBoxTestMode->hide();
           }
       }
    //.................................... interface sur mode local  ...........................................................
    // le mode local permet un fonctionnement sans Sigems et autorise la saisie d'un nom dans les champs de saisie renseignement
    // patient et l'impression avec.
    if (CGestIni::Param_ReadParam(Global_ParamData,  "SigemsWebServices", "Local", &str) ==QString::null )  // zero = pas d'erreur
       {QString val = str.left(1).upper();
        if (val=="O"||val=="Y")   m_ModeFlag |= LOCAL_MODE;
       }
    SetInterface();
    //......................... Petit Message coucou ............................................................................
    //tabwidget_Informations->removePage (tabwidget_Informations->page (4)); // LV_ASSOCIES
    tabwidget_Informations->setCurrentPage(TB_HIERARCHIE+1);   // pourquoi faut il + 1 pour que cela fonctionne ???

    // statusBar()->message("Bienvenue sur CCAM View ", 50000 );

    //..................................... ajuster spliter ...........................................................
    //............................ Placer le spliter dans la bonne position .................................
    Q3ValueList <int> list;
    int w = width() / 3;
    list.append(w);
    list.append(width() - w);
    splitterMedian->setSizes (list );
    if (qApp->argc() >= 26)
       {list.clear();
        int h = height()/5;
        list.append( height() - h);
        list.append(h);
        int id = getCurrentPageID_ByTitle(tabwidget_Recherche, qApp->argv()[25 ]);
        tabwidget_Recherche->setCurrentPage (id );  // nom du pannel \303\264 activer
        //change_onglet_recherche( tabwidget_Recherche->page ( id ) );
        //tabwidget_Recherche->setCurrentPage (getCurrentPageID_ByTitle(tabwidget_Recherche, "&Thesaurus") );  // nom du pannel \303\264 activer
        splitterBas->setSizes (list );
       }
#ifndef QT_NO_DEBUG
   // int id = getCurrentPageID_ByTitle(tabwidget_Recherche, "&Thesaurus");
   // tabwidget_Recherche->setCurrentPage (id );  // nom du pannel \303\264 activer
#endif
    //Global_DebugStr.append(QString("-------- Entr\303\251e : ---------------- ")+code_medecin+"\n");
    //Global_DebugStr.append(QString("-------- Entr\303\251e : ---------------- ") + qApp->argv()[ 25 ] + "\n");
    //CGestIni::Param_UpdateToDisk( "/home/ro/debug.txt", Global_DebugStr       );
    FillComboSitesAnatomiques();
    FillComboAction();
    FillComboModesAcces();
    return 1;
}

//------------------------------------------ getCurrentPageID_ByTitle ----------------------------------------------------------------------------------
int C_Dlg_FeuillePrincipale::getCurrentPageID_ByTitle( QTabWidget * pQTabWidget, const QString & title )
{int nb = pQTabWidget->count();
 for ( int i=0; i<nb; ++i)
     {if (pQTabWidget->label(i) == title) return i;
     }
 return -1;
}

//------------------------------------------ GetComboDateEffet ----------------------------------------------------------------------------------
//  \brief recherche la date d'effet dans la combobox date d'effet
QString C_Dlg_FeuillePrincipale::GetComboDateEffet()
{int         index = comboBoxDateEffet->currentIndex(); if (index == -1) return QString::null;
 QVariant       qv = comboBoxDateEffet->itemData (index);
 return         qv.toString();
}

// -------------------------------------- edit_Code_lostFocu -------------------------------------
void C_Dlg_FeuillePrincipale::edit_Code_lostFocus()
{//if (edit_Code->text().length()<=2) return;
    if (m_pCMoteurCCAM_Base==0) return;
    QString code_acte = edit_Code->text()+"_______";
    m_pCMoteurCCAM_Base->GetActesParMotsClef(lview_ActesTrouves, "", "", code_acte.left(7), "" );
    lview_ActesTrouves_selectionChanged();// modifie le 26052004
}

// -------------------------------------- fileExporter_la_table_des_mdecinsAction_activated -------------------------------------
void C_Dlg_FeuillePrincipale::fileExporter_la_table_des_mdecinsAction_activated()
{if (m_pCMoteurCCAM_Base==0) return;
    QString filtre_selectionne = "*.tmd";
    QString nom_fichier = QFileDialog::getSaveFileName (QString::null, // startWith
                                                        tr("Table des M\303\251decins (*.tmd)"), // filter
                                                        this, // parent
                                                        0, // name
                                                        tr("Exporter"), // Caption
                                                        &filtre_selectionne);// selected filter
    if ( nom_fichier.isEmpty() ) return;  // si pas de nom de fichier alors cassos

    QString document = m_pCMoteurCCAM_Base->GetTableMedecins();

    if (document !=""){
        //SaveString(nom_fichier, document, ".tmd" );
        QFileInfo fif(nom_fichier);
        CGestIni::Param_UpdateToDisk(fif.absolutePath()+"/"+fif.baseName()+".tmd", document);

    } else {
        QMessageBox::critical (0, NAME_APPLI ,
                               tr("Il n'y a aucunes donn\303\251es \303\240 exporter"),
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
    }
}

// -------------------------------------- fileImporter_la_table_des_medecinsAction_activated -------------------------------------
void C_Dlg_FeuillePrincipale::fileImporter_la_table_des_medecinsAction_activated()
{if (m_pCMoteurCCAM_Base==0) return;
    QString filtre_selectionne = "*.tmd";
    QString path               = CGestIni::Construct_PathBin_Module("ccamview", QFileInfo (qApp->argv()[0]).dirPath (true)) + "Ressources";
    QString nom_fichier = QFileDialog::getOpenFileName (path, // startWith
                                                        tr("Table de M\303\251decin Ccamview (*.tmd)"), // filter
                                                        this, // parent
                                                        0, // name
                                                        tr("Importer"), // Caption
                                                        &filtre_selectionne);// selected filter
    if ( nom_fichier.isEmpty() ) return;  // si pas de nom de fichier alors cassos

    if (m_pCMoteurCCAM_Base->ImportTableMedecins(this,nom_fichier) != -1) {
        m_pCMoteurCCAM_Base->GetComboPraticiens(combo_Praticiens);
        return;
    }

    QMessageBox::critical (0, NAME_APPLI ,
                           tr("Il n'y a aucunes donn\303\251es \303\240 importer\n"
                              "ou le format de fichier est invalide"),
                           QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
}

// -------------------------------------- fileEffacer_le_thsaurusAction_activated -------------------------------------
void C_Dlg_FeuillePrincipale::fileEffacer_le_thsaurusAction_activated()
{   m_pCMoteurCCAM_Base->SupprimeThesaurusPraticien(m_Code_Praticien);
    combo_Categories->clear();
    combo_Categories->insertItem(tr("(Toutes)"));
    m_pCMoteurCCAM_Base->GetComboCategorie(combo_Categories,m_Code_Praticien);
    m_pCMoteurCCAM_Base->GetThesaurus(listview_Thesaurus, m_Code_Praticien,combo_Categories->currentText());
}

// -------------------------------------- fileImporter_un_thsaurusAction_activated -------------------------------------
void C_Dlg_FeuillePrincipale::fileImporter_un_thsaurusAction_activated()
{QString ret;
    QString filtre_selectionne = "*.txt";
    QString path               = CGestIni::Construct_PathBin_Module("ccamview", QFileInfo (qApp->argv()[0]).dirPath (true)) + "Ressources";
    QString nom_fichier        = QFileDialog::getOpenFileName (path, // startWith
                                                               tr("Th\303\251saurus (*.ths *.txt )"), // filter
                                                               this, // parent
                                                               0, // name
                                                               tr("Importer un th\303\251saurus"), // Caption
                                                               &filtre_selectionne);// selected filter
    if ( nom_fichier.isEmpty() ) return;  // si pas de nom de fichier alors cassos
    CGestIni::Param_UpdateFromDisk(nom_fichier, ret);
    QStringList lineThesaurusList;
    lineThesaurusList = QStringList::split("\r\n", ret);
    char *pt  = (char*)(const char*)ret;
    char *deb = pt;
    char *end = pt + ret.length();
    while (pt<end)
        { if (pt[0]=='\r' || pt[0]=='\n')
             {QString str = QString::fromLatin1 (deb, pt-deb );
              QStringList itemThesaurusList;
                  itemThesaurusList = QStringList::split("|=|", str, TRUE);
              m_pCMoteurCCAM_Base->AjoutActesThesaurus(m_Code_Praticien, itemThesaurusList[2], &itemThesaurusList[0], &itemThesaurusList[1]);
              while ( (pt[0]=='\r' || pt[0]=='\n') && pt<end) ++pt;
              deb=pt;
             }
          else
             {
              ++pt;
             }
        }
    combo_Categories->clear();
    combo_Categories->insertItem(tr("(Toutes)"));
    m_pCMoteurCCAM_Base->GetComboCategorie(combo_Categories,m_Code_Praticien);
    m_pCMoteurCCAM_Base->GetThesaurus(listview_Thesaurus, m_Code_Praticien,combo_Categories->currentText());
}

// -------------------------------------- fileExporter_le_thsaurusAction_activated -------------------------------------
void C_Dlg_FeuillePrincipale::fileExporter_le_thsaurusAction_activated()
{QString ret="";

 if (tabwidget_Recherche->tabLabel ( tabwidget_Recherche->currentPage () )!= "&Thesaurus")
    {QMessageBox::critical (0, NAME_APPLI ,
                           tr("Cette op\303\251ration ne peut se faire\nqu'\303\240 partir de l'onglet th\303\251saurus\nVeuillez l'activer"),
                           QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     return;
    }
 if (listview_Thesaurus->childCount()<=0)
    {QMessageBox::critical (0, NAME_APPLI ,
                           tr("Le th\303\251saurus est vide\nop\303\251ration annul\303\251e"),
                           QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     return;
    }
 QString path = CGestIni::Construct_PathBin_Module("ccamview", QFileInfo (qApp->argv()[0]).dirPath (true))+"Ressources";
 QString    s = QFileDialog::getSaveFileName(path,
                                             tr("Th\303\251saurus (*.ths *.txt )"),
                                             this,
                                             tr("Exporter le th\303\251saurus"),
                                             tr("Choisissez le nom du fichier \303\240 exporter" ));
 if (s.length()==0) return;
 Q3ListViewItemIterator it ( listview_Thesaurus );
 while ( it.current() )
     {Q3ListViewItem *item = it.current();
      ret += item->text(THE_CATEGORIE)+"|=|"+item->text(THE_CODE_USUEL)+"|=|"+item->text(THE_CODE_CCAM)+"|=|"+item->text(THE_LIBELLE)+"\r\n";
      ++it;
     }
 CGestIni::Param_UpdateToDisk(s, ret);
}

// -------------------------------------- configurationExport_Table_ACTION1Action_activated -------------------------------------
void C_Dlg_FeuillePrincipale::configurationExport_Table_ACTION1Action_activated()
{if (m_pCMoteurCCAM_Base==0) return;
    QString filtre_selectionne = "*.cvt";
    QString nom_fichier = QFileDialog::getSaveFileName (QString::null, // startWith
                                                        tr("Table de donn\303\251es Ccamview (*.cvt)"), // filter
                                                        this, // parent
                                                        0, // name
                                                        "Exporter", // Caption
                                                        &filtre_selectionne);// selected filter
    if ( nom_fichier.isEmpty() ) return;  // si pas de nom de fichier alors cassos

    QString document = m_pCMoteurCCAM_Base->GetTablesEssentielles();

    if (document !=""){
        //SaveString(nom_fichier, document, ".cvt" );
        QFileInfo fif(nom_fichier);
        CGestIni::Param_UpdateToDisk(fif.absolutePath()+"/"+fif.baseName()+".cvt", document);
    } else {
        QMessageBox::critical (0, NAME_APPLI ,
                               tr("Il n'y a aucune donn\303\251e \303\240 exporter"),
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
    }

}

// -------------------------------------- configurationImport_Tables_EssentiellesAction_activated -------------------------------------
void C_Dlg_FeuillePrincipale::configurationImport_Tables_EssentiellesAction_activated()
{
    if (m_pCMoteurCCAM_Base==0) return;
    QString path               = CGestIni::Construct_PathBin_Module("ccamview", QFileInfo (qApp->argv()[0]).dirPath (true)) + "FichiersNX";
    QString filtre_selectionne = "*.cvt";
    QString nom_fichier = QFileDialog::getOpenFileName (path, // startWith
                                                        tr("Table de donn\303\251es Ccamview (*.cvt)"), // filter
                                                        this, // parent
                                                        0, // name
                                                        tr("Importer"), // Caption
                                                        &filtre_selectionne);// selected filter
    if ( nom_fichier.isEmpty() ) return;  // si pas de nom de fichier alors cassos

    if (m_pCMoteurCCAM_Base->ImportTablesEssentielles(nom_fichier) != -1) {
        cbox_SiteAnatomique->clear();
        cbox_Action->clear();
        cbox_ModeAcces->clear();
        FillComboSitesAnatomiques();
        FillComboAction();
        FillComboModesAcces();
        C_Dlg_FeuillePrincipale::critere_code(QString::null);
        return;
    }

    QMessageBox::critical (0, NAME_APPLI ,
                           tr("Il n'y a aucune donn\303\251e \303\240 importer\n"
                              "ou le format de fichier est invalide"),
                           QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
}


//-------------------------------------- Get_BodyPartsThesaurus --------------------------------------------------------------------------------
// NOTE : l'impression du thesaurus ne partage pas les memes templates que l'impression de la liste des actes
//        car l'origine des infos provient d'une listView dif\303\251rente de celle de la liste des actes
//        de ce fait les templates d'impression sont diff\303\251rents.
void C_Dlg_FeuillePrincipale::Get_BodyPartsThesaurus( QString & bodyHead, QString & bodyActe, QString & bodyfoot )
{QString path = GlobalPathAppli + SYS_FOLDER + "/";
 CGestIni::Param_UpdateFromDisk(path + "BodyHeadThesaurus.html", bodyHead);
 CGestIni::Param_UpdateFromDisk(path + "BodyActeThesaurus.html", bodyActe);
 CGestIni::Param_UpdateFromDisk(path + "BodyFootThesaurus.html", bodyfoot);
}

//-------------------------------------- CreateBodyThesaurus ---------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::CreateBodyThesaurus( Q3ListView * pQListViewActes, QString & body, int repeat_nb, int flag )
{int i = 0;
 if (pQListViewActes->childCount()<=0) return;
 //..................... relever le nb de fois qu'il faut repeter le motif ..........................................
 QString repeat;
 if (repeat_nb==0)
    {CGestIni::Param_UpdateFromDisk(Global_PathIni, Global_ParamData);
     if (CGestIni::Param_ReadParam(Global_ParamData,  "Page Param", "RepeatActes", &repeat) == QString::null )             // zero = pas d'erreur
        {repeat_nb = atoi(repeat);
        }
     else repeat_nb=1;
    }
 Q3ListViewItemIterator it ( pQListViewActes );
 QString head(""), foot(""), acte(""), str("");
 Get_BodyPartsThesaurus(head, acte,  foot);
 //........................ entete HTML ...................................
 if (flag)
     body  = "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
             "<body style=\"font-size:6pt;font-family:Arial Narrow\">";
 else
     body  ="";
 //....................... entete de groupe motif .........................
 body += head;
 while ( it.current() )
     {Q3ListViewItem *item = it.current();
      str    =  acte ;
      ++i;
      FusionneThesaurus(0, item, str);
      body +=  str ;
      if (i==repeat_nb)
         {body += foot + head;
          i=0;
         }
      ++it;
     }
 body +=  foot;
 if (flag) body += "</body></html>";
}

//-------------------------------------- FusionneThesaurus ------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::FusionneThesaurus( INFO_MED * pInfoMed, Q3ListViewItem * pQListViewItemActes, QString & document )
{int          pos_deb = 0;
 int          pos_end = 0;
 const char*       pt = 0;

 //........................... faire les remplacements ................................................
 pos_deb = document.find("{{", pos_end);
 pt = (const char*)document + pos_deb;
 while (pos_deb != -1)
       {pos_end = document.find("}}", pos_deb);
        pt = (const char*) document + pos_end;
        if (pos_end != -1 &&  pos_deb != -1)
           {pos_end = ReplaceThesaurusTokenByValue(pInfoMed, pQListViewItemActes, document, pos_deb+2, pos_end - pos_deb-2);
            pt = (const char*) document + pos_end;
           }
        pos_deb = document.find("{{", pos_end);
        pt = (const char*) document + pos_deb;
       }
}

//------------------------------------------ bouton_ImprimerThesaurus_clicked ---------------------------------------------------------------
void C_Dlg_FeuillePrincipale::bouton_ImprimerThesaurus_clicked()
{
    /*typedef struct _INFO_MED       Strucuture d'\303\251change du praticien
      {QString m_Code;
       QString m_Password;
       QString m_Nom;
       QString m_Prenom;
       QString m_Nid;
       QString m_Spec;
      }INFO_MED;

      Fonction qui rempli la structure pr\303\251c\303\251dente
    GetInfoMedecinsFromCode(const QString &codeMEDECIN, INFO_MED &info_med);*/

 QString head(""), body(""), foot("");
 //IDENT_PAT identPat;

 //............................ info medecin ..............................
 INFO_MED                  info_med;
 m_pCMoteurCCAM_Base->GetInfoMedecinsFromCode(m_Code_Praticien, info_med); // recup dans info_med les informations du praticien
 //.......................... recuperer l'entete et le pied de page du document .....................................
 head = Get_Head_Foot_Thesaurus(&foot);
 //.......................... fusionner l'entete et le pied ...............................................
 FusionneThesaurus(&info_med,0, head);
 FusionneThesaurus(&info_med,0, foot);
 //.......................... Creer les lignes d'actes ....................................................
 CreateBodyThesaurus(listview_Thesaurus, body, 0, 1);
 C_Dlg_Print* dlg = new C_Dlg_Print (textbrowser_hierarchie,
                                     &head,
                                     &body,
                                     &foot,
                                     "Impression Thesaurus");
 if (dlg==0) return;
 if (dlg->exec()== QDialog::Accepted) {};
 delete dlg;
}

//------------------------------------------ GetDateEntreeSortie ---------------------------------------------------------------
void C_Dlg_FeuillePrincipale::GetDateEntreeSortie( QString & dateEntree, QString & dateSortie )
{QString numDoss = GetNumDossierFromEditables();
 if (numDoss==""||numDoss=="0") return;
 Q3ListViewItemIterator it ( listview_ListeDossPat );
 while ( it.current() )
     {Q3ListViewItem *item = it.current();
      if (item->text(0)== numDoss)
         {dateEntree = item->text(2);
          dateSortie = item->text(3);
          return;
         }
      ++it;
     }
 dateEntree = dateEdit_DateEntree->date().toString ("dd/MM/yyyy");
 dateSortie = dateEdit_DateSortie->date().toString ("dd/MM/yyyy");
}

//-------------------------------------- FusionneDocument ------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::FusionneDocument( IDENT_PAT * pIdentPat, Q3ListViewItem * pQListViewItemActes, QString & document )
{int          pos_deb = 0;
 int          pos_end = 0;
 const char*       pt = 0;

 //........................... faire les remplacemenst ................................................
 pos_deb = document.find("{{", pos_end);
 pt = (const char*)document + pos_deb;
 while (pos_deb != -1)
       {pos_end = document.find("}}", pos_deb);
        pt = (const char*) document + pos_end;
        if (pos_end != -1 &&  pos_deb != -1)
           {pos_end = ReplaceTokenByValue(pIdentPat, pQListViewItemActes, document, pos_deb+2, pos_end - pos_deb-2);
            pt = (const char*) document + pos_end;
           }
        pos_deb = document.find("{{", pos_end);
        pt = (const char*) document + pos_deb;
       }
}

//-------------------------------------- CreateBody ---------------------------------------------------------------------------
// ACTION: cr\303\251e le corps du document par assemblage de trois parties:
//         un header
//         un milieu r\303\251p\303\251t\303\251 autant de fois qu'il y a des actes \303\264 imprimer
//         un pied de page
// ENTREE: QListView* pQListViewActes: listview des actes d'o\303\231 seront extraites les informations sur les actes
//         QString& body: reference sur la chaine de caract\303\250res dans laquelle sera construit le corps du document
//         int repeat_nb: nbre de fois au bout duquel le pied de page et un nouveau header seront inser\303\251s
//                        Si zero: alors ce nombre sera extrait du fichier ccam_view.ini dans l'entr\303\251e suivante:
//                                 [Page Param]
//                                        RepeatActes = 10000
//         int flag:      si un: il ne sera ajout\303\251 le header et pied de page conventionnel d'une page html \303\264 savoir
//                            pour le header: "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
//                                            "<body style=\"font-size:6pt;font-family:Arial Narrow\">
//                            pour le pied:   "</body></html>"
//                            la page html ainsi produite est compl\303\252te et pr\303\250te \303\264 \303\252tre utilis\303\251e.
//                        si zero: le header et pied de page conventionnel d'une page html ne seront pas ajout\303\251s
//                                 cela permet d'integrer le resultat dans une page html en construction
void C_Dlg_FeuillePrincipale::CreateBody( Q3ListView * pQListViewActes, QString & body, int repeat_nb, int flag )
{int i = 0;
 if (pQListViewActes->childCount()<=0) return;
 //..................... relever le nb de fois qu'il faut repeter le motif ..........................................
 QString repeat;
 if (repeat_nb==0)
    {CGestIni::Param_UpdateFromDisk(Global_PathIni, Global_ParamData);
     if (CGestIni::Param_ReadParam(Global_ParamData,  "Page Param", "RepeatActes", &repeat) == QString::null )             // zero = pas d'erreur
        {repeat_nb = atoi(repeat);
        }
     else repeat_nb=1;
    }
 Q3ListViewItemIterator it ( pQListViewActes );
 QString head(""), foot(""), acte(""), str("");
 Get_BodyParts(head, acte,  foot);
 //........................ entete HTML Document...................................
 if (flag)
     body  = "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
             "<body style=\"font-size:6pt;font-family:Arial Narrow\">";
 else
     body  ="";
 //....................... entete de groupe motif .........................
 body += head;
 //....................... lignes de groupe motif .........................
 while ( it.current() )
     {Q3ListViewItem *item = it.current();
      str    =  acte ;
      ++i;
      FusionneDocument(0, item, str);
      body +=  str ;
      if (i==repeat_nb)
         {body += foot + head;
          i=0;
         }
      ++it;
     }
 body +=  foot;
 if (flag) body += "</body></html>";
}

//-------------------------------------- Get_BodyParts --------------------------------------------------
// NOTE : l'impression de la liste des actes  ne partage pas les memes templates que l'impression du thesaurus
//        car l'origine des infos provient d'une listView diff\303\251rente de celle de la liste du thesaurus
//        de ce fait les templates d'impression sont diff\303\251rents.

void C_Dlg_FeuillePrincipale::Get_BodyParts( QString & bodyHead, QString & bodyActe, QString & bodyfoot )
{QString path = GlobalPathAppli + SYS_FOLDER + "/";
 //bodyHead  = qstringLoad(path + "BodyHeadThesaurus.html");
 //bodyActe  = qstringLoad(path + "BodyActeThesaurus.html");
 //bodyfoot  = qstringLoad(path + "BodyFootThesaurus.html");
 CGestIni::Param_UpdateFromDisk(path + "BodyHead.html", bodyHead);
 CGestIni::Param_UpdateFromDisk(path + "BodyActe.html", bodyActe);
 CGestIni::Param_UpdateFromDisk(path + "BodyFoot.html", bodyfoot);
}


//------------------------------------------ pushButtonActePatImprimer_clicked --------------------------------------------------------------
/*
 typedef struct _IDENT_PAT
    {QString m_Nom;
     QString m_Prenom;
     QString m_NomNaissance;
     QString m_Adresse1;
     QString m_Adresse2;
     QString m_CodePostal;
     QString m_Ville;
     QString m_VilleNss;
     QString m_Tel;
     QString m_DateNss;
     QString m_Sexe;
     QString m_NumSecu;
     QString m_NumPatient;
     QString m_NumDossier;
     QString m_DateEntree;
     QString m_DateSortie;
     QString m_BaseTravail;
     QString m_PraticienNid;
     QString m_Utilisateur;
    }IDENT_PAT;
*/

void C_Dlg_FeuillePrincipale::pushButtonActePatImprimer_clicked()
{QString head(""), body(""), foot("");
 QString numDoss = GetNumDossierFromEditables();
 if ((numDoss==""||numDoss=="0") && !(m_ModeFlag & LOCAL_MODE))
    {
     QMessageBox::critical (0, tr(" Erreur CCAM_View") ,
                            tr(MESSAGE_DOSS_NO_SELECTED),
                            QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     if (numDoss=="") SetFocusOnNumDossAn();
     return;
    }

 //............................ identit\303\251 patient ..............................
 IDENT_PAT        identPat;
 identPat.m_Nom            = lineedit_Nom->text();
 identPat.m_Prenom         = lineedit_Prenom->text();
 identPat.m_NomNaissance   = lineedit_Nom_Naiss->text();
 identPat.m_Adresse1       = lineedit_Adr_1->text();
 identPat.m_Adresse2       = lineedit_Adr_2->text();
 identPat.m_CodePostal     = lineedit_CodePostal->text();
 identPat.m_Ville          = lineedit_Ville->text();
 identPat.m_VilleNss       = lineedit_Ville_Nss->text();
 identPat.m_Tel            = lineedit_Tel->text();
 identPat.m_DateNss        = dateEdit_DateNss->date().toString ("dd/MM/yyyy");
 identPat.m_Sexe           = lineEdit_Sexe->text();
 identPat.m_NumSecu        = lineedit_NumSS->text() + " " + lineEdit_CleSS->text();
 identPat.m_NumPatient     = lineedit_Code->text();
 identPat.m_NumDossier     = numDoss;
 identPat.m_DateEntree     = "";
 identPat.m_DateSortie     = "";
 if (!(m_ModeFlag & LOCAL_MODE)) GetDateEntreeSortie(identPat.m_DateEntree, identPat.m_DateSortie);
 identPat.m_BaseTravail    = comboBox_Bases->currentText();
 identPat.m_PraticienNid   = m_pCMoteurCCAM_Base->GetNidPraticiens(m_Code_Praticien);
 identPat.m_Utilisateur    = m_Code_Utilisateur;
 //.......................... recuperer les trois parties du document .....................................
 head = Get_Head_Foot(&foot);
 //.......................... fusionner l'entete et le pied ...............................................
 FusionneDocument(&identPat,0, head);
 FusionneDocument(&identPat,0, foot);

 //.......................... Creer les lignes d'actes ....................................................
 CreateBody(listview_ListeActePat, body, 0, 1);
 C_Dlg_Print* dlg = new C_Dlg_Print(textbrowser_hierarchie,
                                    &head,
                                    &body,
                                    &foot,
                                    "Impression Actes"
                                   );
 if (dlg==0)     return;
 if (dlg->exec()== QDialog::Accepted) {};
 delete dlg;
}

//------------------------------------------ HtmlExport --------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::HtmlExport()
{QString numDoss = GetNumDossierFromEditables();
 //.................... verifier si un numero de dossier est valide ..............................................................
 if ((numDoss==""|| numDoss=="0") && !(m_ModeFlag & LOCAL_MODE))
    {
     QMessageBox::critical (0, tr(" Erreur CCAM_View") ,
                            tr(MESSAGE_DOSS_NO_SELECTED),
                            QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     if (numDoss=="") SetFocusOnNumDossAn();
     return;
    }
 //........................ selecteur de fichiers ................................................................................
 QString fn = QFileDialog::getSaveFileName( QString::null, tr( "Fichiers-HTML (*.htm *.html);;Tous les fichiers (*)" ), this );
 if ( fn.isEmpty() ) return;
 //............................ identit\303\251 patient ..............................
 IDENT_PAT        identPat;
 identPat.m_Nom            = lineedit_Nom->text();
 identPat.m_Prenom         = lineedit_Prenom->text();
 identPat.m_NomNaissance   = lineedit_Nom_Naiss->text();
 identPat.m_Adresse1       = lineedit_Adr_1->text();
 identPat.m_Adresse2       = lineedit_Adr_2->text();
 identPat.m_CodePostal     = lineedit_CodePostal->text();
 identPat.m_Ville          = lineedit_Ville->text();
 identPat.m_VilleNss       = lineedit_Ville_Nss->text();
 identPat.m_Tel            = lineedit_Tel->text();
 identPat.m_DateNss        = dateEdit_DateNss->date().toString ("dd/MM/yyyy");
 identPat.m_Sexe           = lineEdit_Sexe->text();
 identPat.m_NumSecu        = lineedit_NumSS->text() + " " + lineEdit_CleSS->text();
 identPat.m_NumPatient     = lineedit_Code->text();
 identPat.m_NumDossier     =  numDoss;
 identPat.m_DateEntree     = "";
 identPat.m_DateSortie     = "";
 if (!(m_ModeFlag & LOCAL_MODE)) GetDateEntreeSortie(identPat.m_DateEntree, identPat.m_DateSortie);
 identPat.m_BaseTravail    = comboBox_Bases->currentText();
 identPat.m_PraticienNid   = m_pCMoteurCCAM_Base->GetNidPraticiens(m_Code_Praticien);
 identPat.m_Utilisateur    = m_Code_Utilisateur;

 QString document = Get_Export_Head_Foot();   // {{--InsertListeActes--}}

 //.......................... Creer les lignes d'actes ....................................................
 QString body("");
 CreateBody(listview_ListeActePat, body, 10000, 0);
 //.......................... les inserer dans le document maitre .........................................
 document.replace("{{--InsertListeActes--}}", body);
 //.......................... faire la fusion du reste du document maitre .................................
 FusionneDocument(&identPat,0, document);
 //.......................... enregistrer le bouzin .............................................................
 //SaveString(nom_fichier, document, ".html" );
 QFileInfo fif(fn);
 CGestIni::Param_UpdateToDisk(fif.absolutePath()+"/"+fif.baseName()+".html", document);
}

//-------------------------------------------------------------------------------- Imprimer ------------------------------------------------
void C_Dlg_FeuillePrincipale::Imprimer( int page )
{
    switch (page)
    {case ONGLET_RECHERCHE_CODE_ACTE:
          break;
     case ONGLET_THESAURUS:
          bouton_ImprimerThesaurus_clicked();
          break;
     case ONGLET_RECHERCHE_PATIENT:
          pushButtonActePatImprimer_clicked();
          break;
    }
    return;
}

//-------------------------------------------------------------------------------- Imprimer ------------------------------------------------
void C_Dlg_FeuillePrincipale::Imprimer()
{Imprimer(tabwidget_Recherche->currentPageIndex ());
}

//------------------------------------------ pushButtonActePatEffacer_clicked ------------------------------------------------
// cod\303\251 avec les pieds
void C_Dlg_FeuillePrincipale::pushButtonActePatEffacer_clicked()
{
 Q3ListViewItem* pQListViewItem = listview_ListeActePat->selectedItem();
 if (pQListViewItem==0) return;
 //......................... acte simple (il a un parent ou pas de fils) on l'efface .................................
 //                          simplement sans tenir compte de la descendance
 if (pQListViewItem->parent ()!=0 || pQListViewItem->childCount()==0)
    {if (pushButtonRecActesListToSigems->text()==tr("Enregistrer sur Sigems"))
        {if (pQListViewItem->text(LVA_TYP)=="V")
            {QString str_err  = tr("<b>ERREUR: Acte non effa\303\247able :</b><br>Cet acte est un acte d\303\251j\303\264 valid\303\251 et ne peut \303\252tre effac\303\251 que par le m\303\251decin DIM<br>Seuls les actes avec une pastille Orange ou rouge, peuvent \303\252tre modifi\303\251s :<br>");
              MessageAlert(tr("CCAM_View :  Acte non effa\303\247able"), str_err.replace("\n","<br>"), tr("Annuler"), 0, 0, 0);
            }
         else if (pQListViewItem->text(LVA_TYP)=="T")
            {EffacerActeSigems(pQListViewItem);
            }
         else if (pQListViewItem->text(LVA_TYP)=="N")
            { delete pQListViewItem;
            }
         }
      else
         {delete pQListViewItem;
         }
     return;
    }
 //......................... acte avec actes associ\303\251 on l'efface .............................................
 //                          avec la descendance
 QString acteAsso = pQListViewItem->text(LVA_ASSO_WITH);
 // se sera ici le temps de la routine, le crit\303\250re d'effacement
 // on force afin qu'un item selectionn\303\251 sans acte associ\303\251 soit aussi effac\303\251
 if (acteAsso .length()<=0)
    {acteAsso = pQListViewItem->text(LVA_CODECCAM);
     pQListViewItem->setText(LVA_ASSO_WITH, acteAsso);
    }
 //................ parcourir toute la listview des actes ....................
 //                 noter ceux \303\264 effacer
 Q3PtrList<Q3ListViewItem> list;
 list.setAutoDelete( TRUE ); // comme cela les contenus seront automatiquement d\303\251truits en sortie

 Q3ListViewItemIterator it( listview_ListeActePat );
 while ( it.current() )
       {Q3ListViewItem *pQListViewItem = it.current();
        if (pQListViewItem->text(LVA_ASSO_WITH)==acteAsso)
            {if (pushButtonRecActesListToSigems->text()==tr("Enregistrer sur Sigems"))
                {if (pQListViewItem->text(LVA_TYP)=="V")
                    {QString str_err  = tr("<b>ERREUR : Acte non effa\303\247able:</b><br>Cet acte est un acte d\303\251j\303\240 valid\303\251 et ne peut \303\252tre effac\303\251 que par le m\303\251decin DIM<br>Seuls les actes avec une pastille Orange ou rouge, peuvent \303\252tre modifi\303\251s :<br>");
                     MessageAlert(tr("CCAM_View :  Acte non effa\303\247able"), str_err.replace("\n","<br>"), tr("Annuler"), 0, 0, 0);
                    }
                 else if (pQListViewItem->text(LVA_TYP)=="T")
                    {EffacerActeSigems(pQListViewItem);
                    }
                 else if (pQListViewItem->text(LVA_TYP)=="N")
                   {if (pQListViewItem->parent ()==0) list.append(pQListViewItem);
                   }
                }
             else
               {if (pQListViewItem->parent ()==0) list.append(pQListViewItem);
               }
           }
       ++it;
      }
 //   comme le contenu de la liste de pointeurs d'item est auto effa\303\247able,
 //   en sortie de routine, tous ceux qui ont \303\251t\303\251 rajout\303\251s dedans s'effaceront
 //   lors de la destruction de la variable locale : list   g\303\251nial Qt non ??
}

//------------------------------------------ AboutCCAM_View ---------------------------------------------------------------
void C_Dlg_FeuillePrincipale::AboutCCAM_View()
{QTimer::singleShot ( 100, this,SLOT(Slot_actionAproposDisplay()) );
}

//----------------------------------- Slot_actionAproposDisplay -----------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::Slot_actionAproposDisplay()
{        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
         CGestIni::Param_UpdateToDisk(GlobalPathAppli + "Ressources/Changements.html", QString::fromUtf8 ( CHANGEMENTS));
         QString pathExeAPropos     = CGestIni::Construct_Name_Exe("APropos", QFileInfo (qApp->argv()[0]).path());
         QString pathBinRessources  = CGestIni::Construct_PathBin_Module("APropos", QFileInfo (qApp->argv()[0]).path())+"Ressources/";
         QStringList argList;
         QProcess::ProcessState procState;
         //......................... completer les autres arguments .........................................
         argList << "ccamview";                                                      // 1  nom du module
         argList << tr("Module de codage CCAM");                                     // 2  description courte
         argList << m_NUM_VERSION.remove("@").remove("#").remove("=")+"  Qt : " + QT_VERSION_STR;  // 3  numero de version epure des balises de debut et fin
         argList << GlobalPathAppli +"Ressources/Changements.html";                  // 4  fichiers decrivant les changements
         argList << GlobalPathAppli +"Ressources/DocHTML/ccam_view_icn.png";         // 5  Icone du programme
         //argList << GlobalPathAppli +"../../Doc/ccamview/index.html";                    // 6  doc du programme

         if (m_Apropos_Proc==0)
            {helpAboutAction->setDisabled(TRUE);
             m_Apropos_Proc = new QProcess(this);
             m_Apropos_Proc->start(pathExeAPropos, argList);
             SLEEP(3);
             qApp->processEvents ();
             while ( (procState = m_Apropos_Proc->state())== QProcess::Running /* && QFile::exists(pathBinRessources+"~A_propos.html")*/)
                   { //qDebug(QString::number(procState).toAscii());
                     QApplication::processEvents ( QEventLoop::WaitForMoreEvents );
                   }
             if (m_Apropos_Proc) delete m_Apropos_Proc;
             m_Apropos_Proc = 0;
             QFile::remove(pathBinRessources+"~A_propos.html");
             helpAboutAction->setDisabled(FALSE);
            }
         QApplication::restoreOverrideCursor();
}
//--------------------------------------- tryToStopAPropos ----------------------------------------------------------
void C_Dlg_FeuillePrincipale::tryToStopAPropos()
{if (m_Apropos_Proc==0) return;
 m_Apropos_Proc->kill();   // terminate() ne fonctionne pas
}
//------------------------------------------ listview_ListeActePat_doubleClicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::listview_ListeActePat_doubleClicked( Q3ListViewItem * pQListViewItem )
{if (pQListViewItem==0) return;
 pushButtonActePatModifier_clicked();
}

//------------------------------------------ listview_ListeDossPat_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::listview_ListeDossPat_clicked( Q3ListViewItem * pQListViewItem )
{if (pQListViewItem==0) return;
 SetNumDossierToEditables(pQListViewItem->text(0));
 listview_ListeActePat->clear();

 QString numDoss  = GetNumDossierFromEditables();
 if (numDoss==""||numDoss=="0")  return;
 listview_ListeActePat->clear();
 ListerSigemsActesPatient(numDoss, "", "searchActe");      // IdPrat par pour l'instant
}

//------------------------------------------ listview_ListePatient_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::listview_ListePatient_clicked( Q3ListViewItem * pQListViewItem )
{ if (pQListViewItem==0) return;
  listview_ListeDossPat->clear();
  listview_ListeActePat->clear();
  lineedit_NumDoss->clear();
  lineedit_NumDossAn->clear();
  SelectPatientSigems(pQListViewItem->text(0));
}

//------------------------------------------ pushButtonDossRech_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::pushButtonDossRech_clicked()
{if (lineedit_NumDoss->text()==""||atol(lineedit_NumDoss->text())==0)
    {QMessageBox::critical (this, tr(" Erreur CCAM_View") ,
                            tr("ERREUR dans la saisie du Num\303\251ro de Dossier :\n        la partie de droite du num\303\251ro de dossier est vide ou nulle "),
                            QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     SetFocusOnNumDoss();
     return;
    }
 QString numDoss  = GetNumDossierFromEditables();
 if (numDoss==""||numDoss=="0")  return;

 ClearAll();
 SelectDossierHospiSigems(numDoss);
 if (listview_ListeDossPat->childCount()>0)
    {if (listview_ListeDossPat->childCount()==1)
        {listview_ListeDossPat->setSelected (listview_ListeDossPat->firstChild () , TRUE);
         Q3ListViewItem * pQListViewItem = listview_ListeDossPat->selectedItem ();
         numDoss = pQListViewItem->text(0);
        }
     SetNumDossierToEditables(numDoss);
    }
 SetFocusOnNumDossAn();
}

//------------------------------------------ pushButtonRechAvancee_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::pushButtonRechAvancee_clicked()
{if (pushButtonClear->isVisible())m_ModeFlag &= ~FULL_FIND;
 else                             m_ModeFlag |=  FULL_FIND;
 SetInterface();
}

//-------------------------------------- lineedit_NumDoss_lostFocus --------------------------------------------------
void C_Dlg_FeuillePrincipale::lineedit_NumDoss_lostFocus()
{
}

//-------------------------------------- lineedit_NumDoss_textChanged --------------------------------------------------
void C_Dlg_FeuillePrincipale::lineedit_NumDoss_textChanged( const QString & st )
{QString str = st;
 if (str.length()> 5) str = str.left(5);
 if (str.length()) lineedit_NumDoss->setText(SetToNum(str));

 //if (lineedit_NumDoss->text().length()==5) pushButtonDossRech_clicked();  // boucle infinie !!!
}

//-------------------------------------- lineedit_NumDossAn_textChanged --------------------------------------------------
void C_Dlg_FeuillePrincipale::lineedit_NumDossAn_textChanged( const QString & str )
{if (str.length()) lineedit_NumDossAn->setText(SetToNum(str));
 if (lineedit_NumDossAn->text().length()==2) SetFocusOnNumDoss();
}

//-------------------------------------- SetFocusOnNumDoss --------------------------------------------------
void C_Dlg_FeuillePrincipale::SetFocusOnNumDoss()
{lineedit_NumDoss->setFocus();
 lineedit_NumDoss->setCursorPosition (0);
 lineedit_NumDoss->selectAll();
}

//-------------------------------------- SetFocusOnNumDossAn --------------------------------------------------
void C_Dlg_FeuillePrincipale::SetFocusOnNumDossAn()
{lineedit_NumDossAn->setFocus();
 lineedit_NumDossAn->setCursorPosition (0);
 lineedit_NumDossAn->selectAll();
}

//-------------------------------------- lineedit_NumDoss_returnPressed --------------------------------------------------
void C_Dlg_FeuillePrincipale::lineedit_NumDoss_returnPressed()
{pushButtonDossRech_clicked();
}

//-------------------------------------- lineedit_NumDossAn_returnPressed --------------------------------------------------
void C_Dlg_FeuillePrincipale::lineedit_NumDossAn_returnPressed()
{SetFocusOnNumDoss();
}

//------------------------------------------ pushButtonChercherPatient_clicked -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::pushButtonChercherPatient_clicked()
{ClearAllIdentiteWithout4RechFields();
     listview_ListePatient->clear();
     listview_ListeDossPat->clear();
     listview_ListeActePat->clear();
     lineedit_NumDoss->clear();
     lineedit_NumDossAn->clear();
     lineedit_Code->clear();
     ChercherPatient();
}

//------------------------------------------ pushButtonClear_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::pushButtonClear_clicked()
{    ClearAll();
     //................ imterruption de la requete SOAP en cours ............
     if (m_pcSoapI) delete m_pcSoapI;
     m_pcSoapI = 0;
     QApplication::restoreOverrideCursor();
     lineedit_NumDossAn->setFocus();
}

//------------------------------------------ pushButtonNumDossier_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::pushButtonNumDossier_clicked()
{if (lineedit_Code->text() != "")
    {QString ipp = lineedit_Code->text(); // noter le code avant de tout effacer
     ClearAll();
     m_UpdatePatList = 1;                 // faudra mettre \303\264 jour la liste des patients
     SelectPatientSigems(ipp);            // chercher le patient
    }
}

//------------------------------------------- comboBox_Bases_highlighted -------------------------------------------
void C_Dlg_FeuillePrincipale::comboBox_Bases_highlighted( const QString & base )
{QString str;
 if (CGestIni::Param_ReadParam(Global_ParamData,  base, "Name", &str) ==QString::null )             // zero = pas d'erreur
    { statusBar()->message(str, 100000 );
    }
}

//-------------------------------------- comboBox_Bases_activated --------------------------------------------------
void C_Dlg_FeuillePrincipale::comboBox_Bases_activated( int )
{QString str;
 ClearAll();
 //............ arreter toute recherche SOAP en cours ............
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI = 0;
 QApplication::restoreOverrideCursor();
 if (CGestIni::Param_ReadParam(Global_ParamData,  comboBox_Bases->currentText(), "Name", &str) ==QString::null )             // zero = pas d'erreur
    {INFO_MED info_med;
     m_pCMoteurCCAM_Base->GetInfoMedecinsFromCode(m_Code_Utilisateur, info_med);
     setCaption( QString("CCAM View Utilis\303\251e par :  ") + info_med.m_Nom + " " + info_med.m_Prenom + " \303\240 : " + str);
    }
}

//------------------------------------------ ClearAll ---------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::ClearAll()
{ClearAllIdentite();
 listview_ListePatient->clear();
 listview_ListeDossPat->clear();
 listview_ListeActePat->clear();
 lineedit_NumDoss->clear();
 lineedit_NumDossAn->clear();
 lineedit_Code->clear();
}

//------------------------------------------ ClearAllIdentiteWithout4RechFields --------------------------------------------------
void C_Dlg_FeuillePrincipale::ClearAllIdentiteWithout4RechFields()
{    lineedit_Code1->clear();
     lineedit_Code->clear();
     lineedit_NumSS->clear();
     lineEdit_CleSS->clear();
     lineedit_Adr_1->clear();
     lineedit_Adr_2->clear();
     lineEdit_Sexe->clear();
     lineedit_Tel->clear();
     lineedit_CodePostal->clear();
     lineedit_Ville->clear();
     lineedit_Ville_Nss->clear();
}

//------------------------------------------ ClearAllIdentite -------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::ClearAllIdentite()
{ClearAllIdentiteWithout4RechFields();
 //................. effacer les 4 champs de recherche .......................
 lineedit_Nom_Naiss->clear();
 lineedit_Nom->clear();
 lineedit_Prenom->clear();
 dateEdit_DateNss->setDate (QDate ( ) );
}

//------------------------------------------ pushButtonRecActesListToSigems_clicked ---------------------------------------------
/*
enum LVACTES {LVA_DATE            ,    // 0
              LVA_HEURE           ,    // 1
              LVA_PRATICIENNAME   ,    // 2
              LVA_CODECCAM        ,    // 3
              LVA_LIBELLEACTE     ,    // 4
              LVA_PHASE           ,    // 5
              LVA_ACTIVITE        ,    // 6
              LVA_MODIFICATEUR1   ,    // 7
              LVA_MODIFICATEUR2   ,    // 8
              LVA_MODIFICATEUR3   ,    // 9
              LVA_MODIFICATEUR4   ,    // 10
              LVA_NB              ,    // 11
              LVA_NUMDOSS         ,    // 12
              LVA_NOMENCLAT       ,    // 14
              LVA_NIDPRATICIEN         // 14
           };
*/
void C_Dlg_FeuillePrincipale::pushButtonRecActesListToSigems_clicked()
{
 if (pushButtonRecActesListToSigems->text()==tr("Enregistrer sur Sigems"))
    {int nb = 0;
     Q3ListViewItemIterator ut( listview_ListeActePat );
     while ( ut.current() )
           {Q3ListViewItem *item = ut.current();
            ++ut;
            if ( item->text(LVA_TYP)=="N" ) ++nb;
           }

     if (nb > 1)
        {EnregistrerActeParDossierSigems();
        }
     else if (nb ==1)
        {Q3ListViewItemIterator it( listview_ListeActePat );
         while ( it.current() )
           {Q3ListViewItem *item = it.current();
            ++it;
            if ( item->text(LVA_TYP)=="N" )
               {EnregistrerActeSigems(item);
               }
           }
        }
     return;
    }


 if (m_GuidDrTux != "")
    {
     QString path(qApp->argv()[ 1 ]);
     int           i =  path.findRev ('/');
     if (i!=-1) path =  path.left(i+1);
     QString    text = "";



     Q3ListViewItemIterator it ( listview_ListeActePat );
     while ( it.current() )
     {Q3ListViewItem *item = it.current();
      text+= item->text(LVA_CODECCAM)      + " | " +
             item->text(LVA_ACTIVITE)      + " | " +
             item->text(LVA_PHASE)         + " | " +
             item->text(LVA_MODIFICATEUR1) + " | " +
             item->text(LVA_MODIFICATEUR2) + " | " +
             item->text(LVA_MODIFICATEUR3) + " | " +
             item->text(LVA_MODIFICATEUR4) + " | " +
             item->text(LVA_LIBELLEACTE)   + " | " +
             item->text(LVA_CODE_ASSO)     + " | " +
             item->text(LVA_ASSO_WITH)     + " | " +
             item->text(LVA_REGLE_ASSO)    + " | " +
             item->text(LVA_TARIF)         + " | " +
             item->text(LVA_TRF_CALC)      + "\r\n" ;
      ++it;
     }
     path += m_GuidDrTux + ".txt";
     //SaveString(path, text);
     QFileInfo fif(path);
     CGestIni::Param_UpdateToDisk(fif.absolutePath()+"/"+fif.baseName()+".txt", text);
     close();
    }

}

//------------------------------------------ checkBoxTestMode_stateChanged ------------------------------------------------
void C_Dlg_FeuillePrincipale::checkBoxTestMode_stateChanged( int state )
{QString str;
 if (state == Qt::Unchecked)
    {GetBaseSigems();
     if (CGestIni::Param_ReadParam(Global_ParamData,  comboBox_Bases->currentText(), "Name", &str) ==QString::null )             // zero = pas d'erreur
        {INFO_MED info_med;
         m_pCMoteurCCAM_Base->GetInfoMedecinsFromCode(m_Code_Utilisateur, info_med);
         setCaption( tr("CCAM View Utilis\303\251e par :  ") + info_med.m_Nom + " " + info_med.m_Prenom + tr(" \303\240 : ") + str);
        }
    }
}

//-------------------------------------- configurationMise__jour_de_la_Base_CCAMAction_activated --------------------------------------------------
void C_Dlg_FeuillePrincipale::configurationMise__jour_de_la_Base_CCAMAction_activated()
{ if (m_pCMoteurCCAM_Base==0) return;
    QString path               = CGestIni::Construct_PathBin_Module("ccamview", QFileInfo (qApp->argv()[0]).dirPath (true)) + tr("FichiersNX");
    QString filtre_selectionne = "";
    QString nom_fichier = QFileDialog::getOpenFileName (path, // startWith
                                                        tr("Tous les fichiers ()"), // filter
                                                        this, // parent
                                                        0, // name
                                                        tr("Importer dans la base : ") + m_pCMoteurCCAM_Base->m_BaseName, // Caption
                                                        &filtre_selectionne);// selected filter
    if ( nom_fichier.isEmpty() ) return;  // si pas de nom de fichier alors cassos

    //.................... Appel de la fen\303\252tre importation de la base CCAM ....................
    C_Dlg_ChangeMAJCCAM *dlg_majbase = new C_Dlg_ChangeMAJCCAM(m_pCMoteurCCAM_Base,nom_fichier);
    if (dlg_majbase==0)      return ; // Erreur initialisation

    if (dlg_majbase->m_LastError.length())
       {
        QMessageBox mb( tr("Erreur de lecture"),
                        tr("Ce fichier ne semble pas \303\252tre un fichier au format valide\n"),
                        QMessageBox::Critical,
                        QMessageBox::Cancel  | QMessageBox::Default,0,0);
        mb.setButtonText( QMessageBox::Cancel, tr("Annuler"));
        mb.exec();
       }
    else if (dlg_majbase->exec()== QDialog::Rejected)
       {if (dlg_majbase->m_Maj_OK)
           {
            // reinitialiser les objets de la feuille principale
            m_pCMoteurCCAM_Base->initArbo01(lview_Hierarchie);
            FillComboActivite();
            FillComboDateEffet();
           }
       }
    delete dlg_majbase;
    return ;
}
//------------------------------------------ AjouterActeAuDossier -------------------------------------------------------------
/*
typedef struct _CCAM_ACTE
    {QString m_CodeCCAM;
     QString m_Activite;
     QString m_Phase;
     QString m_Modificateur1;
     QString m_Modificateur2;
     QString m_Modificateur3;
     QString m_Modificateur4;
     QString m_NidPraticien;
     QDate   m_Date;
     QTime   m_Heure;
     QString m_PraticienName;
     QString m_LibelleActe;
    }CCAM_ACTE;
*/

void C_Dlg_FeuillePrincipale::AjouterActeAuDossier( int flag, QString codeCCAM )
{if (codeCCAM=="") return;
 QString str_err("");
 CCAM_ACTE ccamActe;
 QString numDoss = GetNumDossierFromEditables();
 //...................... verifier si numero de dossier est present ...................................
 if ( !(m_ModeFlag & LOCAL_MODE) && (numDoss==""||numDoss=="0"))
    {QMessageBox::critical (this, tr(" Erreur CCAM_View") ,
                            tr(MESSAGE_DOSS_NO_SELECTED),
                            QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     tabwidget_Recherche->setCurrentPage(ONGLET_RECHERCHE_PATIENT);
     //lineedit_NumDossAn->setFocus ();
     if (numDoss!="0")SetFocusOnNumDossAn();
     return;
    }
 //.......................... voir si droit \303\264 modifier l'acte .........................................
 //                si utilisateur sans droit de secretaire donc forcement meme que user
 //                si n'est pas le meme que celui de l'acte \303\264 modifier alors lui interdire la
 //                modification
 ccamActe.m_Droits = "";
 if (m_Code_Utilisateur == m_Code_Praticien )
    {ccamActe.m_Droits += "UC=0";     // pas le droit \303\264 changer d'utilisateur
    }
 else
    {ccamActe.m_Droits += "UC=1";     // avec le droit \303\264 changer d'utilisateur
    }
 //........................ recuperer date Entree Sortie du N° dossier ................................
 int     error = 0;
 QDate dateEntree;
 QDate dateSortie;
 error = GetDateEntreeSortie(dateEntree, dateSortie, numDoss);
 if (error)
    {switch (error)
       {case 1:   str_err = tr(" StrDateToQDate() Date Acte invalide ");                                                  break;
        case 2:   str_err = tr(" StrDateToQDate() Date Acte incompl\303\250te (ne comporte pas assez de caract\303\250res) ");          break;
        case 11:  str_err = tr(" GetDateEntreeSortie() Date Entree invalide ");                                           break;
        case 12:  str_err = tr(" GetDateEntreeSortie() Date Entree incompl\303\252te (ne comporte pas assez de caract\303\250res) ");   break;
        case 15:  str_err = tr(" GetDateEntreeSortie() pas de dossier correspondant \303\240 ce num\303\251ro dans la liste actuelle"); break;
        case 16:  str_err = tr(" GetDateEntreeSortie() Num\303\251ro de dossier vide !!!");                                      break;
       }
     QMessageBox::critical (this, tr(" Erreur CCAM_View") ,
                            str_err,
                            QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     return;
    }

 //................. placer code selectionne dans la listview du dossier patient.....................
 ccamActe.m_CodeCCAM       = codeCCAM;
 ccamActe.m_Activite       = "1";
 ccamActe.m_Phase          = "0";
 ccamActe.m_Modificateur1  = "0";
 ccamActe.m_Modificateur2  = "0";
 ccamActe.m_Modificateur3  = "0";
 ccamActe.m_Modificateur4  = "0";
 ccamActe.m_NidPraticien   = m_pCMoteurCCAM_Base->GetNidPraticiens(m_Code_Praticien);
 ccamActe.m_Date           = QDate::currentDate ();
 ccamActe.m_Heure          = QTime::currentTime ();
 if (ccamActe.m_Date < dateEntree || ccamActe.m_Date > dateSortie) ccamActe.m_Date =  dateEntree;

 C_Dlg_PatientActeProperty* dlg = new C_Dlg_PatientActeProperty(&ccamActe, m_pCMoteurCCAM_Base, 0, m_LastDate, m_LastTime);
 if (dlg ==0) return;
 if (dlg->exec() == QDialog::Accepted )
    {
     error = VerifieActe(listview_ListeActePat, ccamActe, dateEntree, dateSortie, &str_err);
     if (error)
        {MessageAlert(tr("CCAM_View :  ERREUR: Pr\303\251sence de cet acte impossible"), str_err.replace("\n","<br>"), tr("Annuler"), 0, 0, 0);
        }
     else
        {int ret = 1;
         error = IsWarningActe(listview_ListeActePat, ccamActe, QDateTime(dateEntree), QDateTime(dateSortie), &str_err);
         if (error)
            {ret = MessageAlert(tr("CCAM_View :  ATTENTION erreur possible pour cet acte"), str_err.replace("\n","<br>"), tr("Ajouter"), tr("Annuler"), 0, 0);
            }
         if (ret!=200)
            {//............................. Inserer l'acte dans la list_view ........................................................
             Q3ListViewItem *pere = PutActeInListView(0, 0, ccamActe, QPixmap (NewActeXPM_xpm));
             if (pere)
                {m_LastDate = ccamActe.m_Date;
                 m_LastTime = ccamActe.m_Heure;
                 //.......................... rajouter les actes associ\303\251s ............................................................
                 if (ccamActe.m_ActesAssocies.count())
                    { QString assoWith = ccamActe.m_CodeCCAM;
                      pere->setText ( LVA_ASSO_WITH,      assoWith);             // marquer l'acte pere avec  lui meme pour signaler qu'il a des actes associ\303\251s
                      for ( QStringList::Iterator it = ccamActe.m_ActesAssocies.begin(); it != ccamActe.m_ActesAssocies.end(); ++it )
                          {ccamActe.m_CodeCCAM       = (*it);
                           ccamActe.m_LibelleActe    = m_pCMoteurCCAM_Base->CodeCCAMToLibelleLong(ccamActe.m_CodeCCAM);
                           Q3ListViewItem *element    = PutActeInListView(0, pere, ccamActe, QPixmap( GlobalPathAppli + SYS_FOLDER + "/PlusVertLitle.png" ));
                           if (element) element->setText ( LVA_ASSO_WITH, assoWith);// marquer l'acte associ\303\251 avec l'acte pere pour signaler avec qi il est associ\303\251
                          }
                      pere->setOpen ( TRUE );
                    }
                 if (flag && tabwidget_Recherche->currentPageIndex () !=  ONGLET_RECHERCHE_PATIENT)
                    {tabwidget_Recherche->setCurrentPage(ONGLET_RECHERCHE_PATIENT);
                    } //endif (flag && tabwidget_Recherche->currentPageIndex () !=  ONGLET_RECHERCHE_PATIENT)
                } //endif (element)

            if (m_ModeFlag & SIGEMS_REAL_TIME && pushButtonRecActesListToSigems->text()==tr("Enregistrer sur Sigems"))
               {EnregistrerActeSigems(pere );
               }
            } //endif (ret!=200)
        } //end else if(error)
    } //endif (dlg->exec() == QDialog::Accepted )
 listview_ListeActePat->setColumnWidth (LVA_LIBELLEACTE, LIBELLE_WIDTH);
 delete dlg;
// textLabel_Prix->setText("");
}

//------------------------------------------ SetNumDossierToEditables -------------------------------------------------------------
void C_Dlg_FeuillePrincipale::SetNumDossierToEditables( QString numDoss )
{if (numDoss.length() != 7) return;
 lineedit_NumDossAn->setText(numDoss.left(2));
 lineedit_NumDoss->setText(numDoss.mid(2));
}

//------------------------------------------ QListViewItemActe_To_QStringList --------------------------------------------------------------
void C_Dlg_FeuillePrincipale::QStringList_To_QListViewItemActe( Q3ListViewItem * pQListViewItem, QStringList & list )
{if ( pQListViewItem == 0) return;
 int          i = 0;
 int nb_columns = pQListViewItem->listView()->columns();
 for (i=0; i<nb_columns-1; ++i) pQListViewItem->setText(i,list[i]);
 QString childStr = list[nb_columns-1];
 if (childStr != "no_list")
    {QStringList childList = QStringList::split (",", childStr);
     for ( QStringList::Iterator it = childList.begin(); it != childList.end(); ++it )
         {Q3ListViewItem * pNewQListViewItem = new Q3ListViewItem(pQListViewItem);
          if (pNewQListViewItem)
             { for (i=0; i<nb_columns-1; ++i) pNewQListViewItem->setText(i,list[i]);
               pNewQListViewItem->setText(LVA_CODECCAM,*it);
             }
         }
    }
}

//------------------------------------------ QListViewItemActe_To_QStringList --------------------------------------------------------------
void C_Dlg_FeuillePrincipale::QListViewItemActe_To_QStringList( Q3ListViewItem * pQListViewItem, QStringList & list )
{if ( pQListViewItem == 0) return;
 int nb_columns = pQListViewItem->listView()->columns();
 for (int i=0; i<nb_columns; ++i) list.append(pQListViewItem->text(i));
 if (pQListViewItem->childCount ())
    {Q3ListViewItem * myChild = pQListViewItem->firstChild();
     QString       childList = "";
     while( myChild )
          {childList += myChild->text(LVA_CODECCAM) + ",";
           myChild    = myChild->nextSibling();
          }
     list.append(childList);
    }
 else
   {list.append("no_list");
   }
}

//------------------------------------------ EraseAllSameActes ------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::EraseAllSameActes( Q3ListViewItem * pQListViewItem )
{if ( pQListViewItem == 0) return;
 QStringList data_list;
 QListViewItemActe_To_QStringList(pQListViewItem, data_list);
 //................ marquer tous les items semblables ................................
 Q3ListViewItemIterator it( listview_ListeActePat );
 while ( it.current() )
       {Q3ListViewItem *item = it.current();
        if (IsSameActe(item, data_list )) delete item;
        else ++it;
        }
}

//------------------------------------------ deleteAllChilds ------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::deleteAllChilds( Q3ListViewItem * pQListViewItem )
{Q3PtrList<Q3ListViewItem> listItemToDelete;
 listItemToDelete.setAutoDelete( TRUE ); // comme cela les contenus seront automatiquement d\303\251truits en sortie

 Q3ListViewItem * myChild =pQListViewItem->firstChild();
 while( myChild )
    {listItemToDelete.append(myChild);
     myChild = myChild->nextSibling();
    }
}

//------------------------------------------ pushButtonActePatModifier_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::pushButtonActePatModifier_clicked()
{Q3ListViewItem* pQListViewItem = listview_ListeActePat->selectedItem();
 if (pQListViewItem==0)                  return;
 QString str_err("");
 if (pQListViewItem->parent()) pQListViewItem = pQListViewItem->parent();
 if (pQListViewItem->text(LVA_TYP)=="V")
    {str_err  = tr("<b>ERREUR : Acte non modifiable :</b><br>Seuls les actes avec une pastille Orange ou rouge, peuvent \303\252tre modifi\303\251s :<br>");
     MessageAlert(tr("CCAM_View :  Acte non modifiable"), str_err.replace("\n","<br>"), tr("Annuler"), 0, 0, 0);
     return;
    }

 CCAM_ACTE ccamActe;
 QString numDoss = pQListViewItem->text(LVA_NUMDOSS);
 //.......................... voir si droit \303\264 modifier l'acte .........................................
 //                si utilisateur sans droit de secretaire donc forcement meme que user
 //                si n'est pas le meme que celui de l'acte \303\264 modifier alors lui interdire la
 //                modification
 ccamActe.m_Droits = "";
 if (m_Code_Utilisateur == m_Code_Praticien )
    {INFO_MED info_med;
     m_pCMoteurCCAM_Base->GetInfoMedecinsFromCode(m_Code_Praticien, info_med);
     if (info_med.m_Nid != pQListViewItem->text(LVA_NIDPRATICIEN))
        {str_err  = tr("<b>ERREUR: Utilisateur interdit :</b><br>Vous n'avez pas le droit de modifier un acte plac\303\251 par le Dr :<br>");
         str_err += pQListViewItem->text(LVA_PRATICIENNAME);
         MessageAlert(tr("CCAM_View :  UTILISATEUR non autoris\303\251"), str_err.replace("\n","<br>"), tr("Annuler"), 0, 0, 0);
         return;
        }
     ccamActe.m_Droits += "UC=0";     // pas le droit \303\264 changer d'utilisateur
    }
 else
    {ccamActe.m_Droits += "UC=1";     // avec le droit \303\264 changer d'utilisateur
    }
 //........................ recuperer date Entree Sortie du N° dossier .......................................................
 //                         si mode connect\303\251 SIGEMS
 int     error = 0;
 QDate dateEntree;
 QDate dateSortie;
 if (!(m_ModeFlag & LOCAL_MODE)) error = GetDateEntreeSortie(dateEntree, dateSortie, numDoss);
 if (error)
    {switch (error)
       {case 1:   str_err = tr(" StrDateToQDate() Date Acte invalide ");                                                  break;
        case 2:   str_err = tr(" StrDateToQDate() Date Acte incompl\303\252te (ne comporte pas assez de caract\303\250res) ");          break;
        case 11:  str_err = tr(" GetDateEntreeSortie() Date Entree invalide ");                                           break;
        case 12:  str_err = tr(" GetDateEntreeSortie() Date Entree incompl\303\252te (ne comporte pas assez de caract\303\250res) ");   break;
        case 15:  str_err = tr(" GetDateEntreeSortie() pas de dossier correspondant \303\240 ce num\303\251ro dans la liste actuelle "); break;
        case 16:  str_err = tr(" GetDateEntreeSortie() Num\303\251ro de dossier vide !!! ");                                      break;
       }
     QMessageBox::critical (this, tr(" Erreur CCAM_View") ,
                            str_err,
                            QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     return;
    }


 ccamActe.m_CodeCCAM      = pQListViewItem->text(LVA_CODECCAM);
 ccamActe.m_Activite      = pQListViewItem->text(LVA_ACTIVITE);
 ccamActe.m_Phase         = pQListViewItem->text(LVA_PHASE);
 ccamActe.m_Modificateur1 = pQListViewItem->text(LVA_MODIFICATEUR1);
 ccamActe.m_Modificateur2 = pQListViewItem->text(LVA_MODIFICATEUR2);
 ccamActe.m_Modificateur3 = pQListViewItem->text(LVA_MODIFICATEUR3);
 ccamActe.m_Modificateur4 = pQListViewItem->text(LVA_MODIFICATEUR4);
 ccamActe.m_NidPraticien  = pQListViewItem->text(LVA_NIDPRATICIEN);

 GetActesAssocies(ccamActe.m_CodeCCAM, ccamActe.m_ActesAssocies);

 QString str_date = pQListViewItem->text(LVA_DATE);

 ccamActe.m_Date = QDate ( atoi(str_date.mid(6,4)),     // 10/12/2003  --> YYYY  2003
                           atoi(str_date.mid(3,2)),     // 10/12/2003  --> MM    12
                           atoi(str_date.mid(0,2))      // 10/12/2003  --> DD    10
                         );
 ccamActe.m_Heure = QTime ( atoi(pQListViewItem->text(LVA_HEURE).mid(0,2)),     // 10:12:20    --> hh    10
                            atoi(pQListViewItem->text(LVA_HEURE).mid(3,2)),     // 10:12:20    --> mm    12
                            0
                            //atoi(pQListViewItem->text(LVA_HEURE).mid(6,2))      // 10:12:20    --> ss    20
                          );
 C_Dlg_PatientActeProperty* dlg = new C_Dlg_PatientActeProperty(&ccamActe, m_pCMoteurCCAM_Base, 1, m_LastDate, m_LastTime);
 if (dlg ==0) return;
 int dlg_ret = dlg->exec();
 //........................................ ajouter acte anesthesie.......................................................................
 if (dlg_ret == 111)
    {
     error = VerifieActe(listview_ListeActePat, ccamActe, dateEntree, dateSortie, &str_err);
     if (error)
        {MessageAlert(tr("CCAM_View :  ERREUR: Pr\303\251sence de cet acte impossible"), str_err.replace("\n","<br>"), tr("Annuler"), 0, 0, 0);
        }
     else
        {int ret = 1;
         error = IsWarningActe(listview_ListeActePat, ccamActe, QDateTime(dateEntree), QDateTime(dateSortie), &str_err);
         if (error)
            {ret = MessageAlert(tr("CCAM_View :  ATTENTION erreur possible pour cet acte"), str_err.replace("\n","<br>"), tr("Ajouter"), tr("Annuler"), 0, 0);
            }
         if (ret!=200)
            {Q3ListViewItem *element = PutActeInListView(0, 0 , ccamActe,QPixmap (NewActeXPM_xpm));
             if (element)
                {m_LastDate = ccamActe.m_Date;
                 m_LastTime = ccamActe.m_Heure;
                 if (m_ModeFlag & SIGEMS_REAL_TIME && pushButtonRecActesListToSigems->text()==tr("Enregistrer sur Sigems"))
                    {EnregistrerActeSigems(element );
                    }
                } //endif (element)
            } //if (ret!=200)
        }//end else if(error)
    } //endif (dlg_ret == 111)
 //........................................ Modifier acte ....................................................................................
 else if (dlg_ret == QDialog::Accepted )
    {
     error = VerifieActe(listview_ListeActePat, ccamActe, dateEntree, dateSortie, &str_err);
     if (error)
        {MessageAlert(tr("CCAM_View :  ERREUR: modification de l'acte impossible"), str_err.replace("\n","<br>"), tr("Annuler"), 0, 0, 0);
        }
     else
        {
         int ret = 1;
         error = IsWarningActeModifier(listview_ListeActePat, ccamActe, dateEntree, dateSortie, pQListViewItem, &str_err);
         if (error )
            {ret = MessageAlert(tr("CCAM_View :  ATTENTION erreur possible lors modification acte"), str_err.replace("\n","<br>"), tr("Valider"), tr("Annuler"), 0, 0);
            }
         if (ret!=200)
            {//..................... relever anciennes valeurs ...........................................
             //                      pour pouvoir cr\303\251er par la suite un faux acte
             //                      \303\264 effacer avec ses semblables
             QStringList data_list;
             deleteAllChilds(pQListViewItem);                                          // effacer tous les actes enfants associ\303\251s
             QListViewItemActe_To_QStringList(pQListViewItem, data_list);              // relever anciennes valeurs
             PutActeInListView(pQListViewItem, 0, ccamActe, QPixmap (NewActeXPM_xpm)); // modifier l'acte avec les nouvelles valeur
             //.................... placer les actes associ\303\251s ..........................................

             if ( ccamActe.m_ActesAssocies.count() )
                { QString assoWith = ccamActe.m_CodeCCAM;
                  pQListViewItem->setText ( LVA_ASSO_WITH,      assoWith);             // marquer l'acte pere avec  lui meme pour signaler qu'il a des actes associ\303\251s
                  for ( QStringList::Iterator it = ccamActe.m_ActesAssocies.begin(); it != ccamActe.m_ActesAssocies.end(); ++it )
                      {ccamActe.m_CodeCCAM       = (*it);
                       ccamActe.m_LibelleActe    = m_pCMoteurCCAM_Base->CodeCCAMToLibelleLong(ccamActe.m_CodeCCAM);
                       Q3ListViewItem *element    = PutActeInListView(0, pQListViewItem, ccamActe, QPixmap( GlobalPathAppli + SYS_FOLDER + "/PlusVertLitle.png" ));
                       if (element) element->setText ( LVA_ASSO_WITH, assoWith);       // marquer l'acte associ\303\251 avec l'acte pere pour signaler avec qi il est associ\303\251
                      }
                  pQListViewItem->setOpen ( TRUE );
                 }

             m_LastDate = ccamActe.m_Date;
             m_LastTime = ccamActe.m_Heure;

             //................ eventuellement effacer l'acte sigems ....................................................
             if ( pushButtonRecActesListToSigems->text()==tr("Enregistrer sur Sigems") &&
                  data_list[LVA_TYP] == "T"
                )
                {//.................. si m\303\252me acte ne rien faire ............................................
                 if (IsSameActe(pQListViewItem, data_list ))
                    {pQListViewItem->setText ( LVA_TYP,           "T");           // retablir type temporaire Sigems
                     pQListViewItem->setPixmap ( 0, QPixmap (TempActeXPM_xpm) );  // avec son icone
                    }
                 //.................. si acte modifi\303\251 effacer l'ancien chez Sigems ...........................
                 else
                    {if (m_ModeFlag & SIGEMS_REAL_TIME )
                        {m_pQListViewItemToRecord = pQListViewItem;
                        }
                     Q3ListViewItem *element = new Q3ListViewItem(listview_ListeActePat); // creer faux acte \303\264 effacer avec ses semblables \303\264
                     if (element)                                                       // partir de l'ancien acte
                        {QStringList_To_QListViewItemActe(element, data_list);
                         element->setText ( LVA_TYP,           "T");             // acte temporaire car existe tjrs chez Sigems
                         element->setPixmap ( 0, QPixmap (ModifActeXPM_xpm) );   // icone speciale pour reconnaitre si \303\247a ne s'est pas effac\303\251
                         EffacerActeSigems(element); // efface tous les actes semblables \303\264 element et rajoute l'acte m_pQListViewItemToRecord
                        }
                    } //end elseif (IsSameActe(pQListViewItem, data_list )
                } // endif ( pushButtonRecActesListToSigems->text()=="Enregistrer sur Sigems" &&
            } //endif (ret)
        } //end else if(error)
    } //endif (dlg->exec() == QDialog::Accepted )
 listview_ListeActePat->setColumnWidth (LVA_LIBELLEACTE, LIBELLE_WIDTH);
 delete dlg;
}

//------------------------------------------ pushButtonActePatModifier_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::GetActesAssocies( const QString & codeCCAM, QStringList & codeAssoList )
{     codeAssoList.clear();
      Q3ListViewItemIterator it( listview_ListeActePat );
      while ( it.current() )
      { Q3ListViewItem *pQListViewItem = it.current();
        if (pQListViewItem && pQListViewItem->parent()!=0 && pQListViewItem->text(LVA_ASSO_WITH)== codeCCAM)
           {codeAssoList.append(pQListViewItem->text(LVA_CODECCAM));
           }
        ++it;
      }
}

//------------------------------------------ textbrowser_hierarchie_anchorClicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::textbrowser_hierarchie_anchorClicked( const QString &, const QString & link )
{QString            codeCCAM("");
 int                pos_deb;

 //......................... si click sur code CCAM ......................................
 //
 if ( (pos_deb  = link.find("#CCAM_Code")) != -1 )
    {codeCCAM   = link.mid(pos_deb+10);
    }
 if (codeCCAM != "")  AjouterActeAuDossier(1, codeCCAM);
}

//------------------------------------------ textbrowser_hierarchie_anchorClicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::textbrowser_procedures_anchorClicked( const QString &, const QString & link )
{QString            codeCCAM("");
 int                pos_deb;

 //......................... si click sur code CCAM ......................................
 //
 if ( (pos_deb  = link.find("#CCAM_Code")) != -1 )
    {codeCCAM      = link.mid(pos_deb+10);
    }
 if (codeCCAM != "")  AjouterActeAuDossier(1, codeCCAM);
}

//------------------------------------------ bouton_AjouterDossier_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::bouton_AjouterDossier_clicked()
{//..................... recuperer acte ............................................................
 QString              codeCCAM = "";
 int                      page = tabwidget_Recherche->currentPageIndex ();
 Q3ListViewItem *pQListViewItem = 0;
 //................. rechercher code selectionne dans les listview..................................
 if (page==ONGLET_RECHERCHE_CODE_ACTE)
    {pQListViewItem = lview_ActesTrouves->currentItem ();
     if (pQListViewItem) codeCCAM = pQListViewItem->text(0);
    }
 else if (page==ONGLET_THESAURUS)
    {pQListViewItem = listview_Thesaurus->currentItem ();
     if (pQListViewItem) codeCCAM = pQListViewItem->text(2);
    }
 AjouterActeAuDossier(1, codeCCAM);
}

//-------------------------------------- lview_GestesComplementaires_doubleClicked --------------------------------------------------
void C_Dlg_FeuillePrincipale::lview_GestesComplementaires_doubleClicked( Q3ListViewItem * pQListViewItem )
{if (pQListViewItem==0) return;
 QString             codeCCAM = "";
 if (pQListViewItem) codeCCAM = pQListViewItem->text(0);
 AjouterActeAuDossier(1, codeCCAM);
}

//------------------------------------------ lview_ActesTrouves_doubleClicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::lview_ActesTrouves_doubleClicked( Q3ListViewItem * pQListViewItem )
{if (pQListViewItem==0) return;
 QString             codeCCAM = "";
 if (pQListViewItem) codeCCAM = pQListViewItem->text(0);
 AjouterActeAuDossier(0, codeCCAM);
}

//------------------------------------------ listview_Thesaurus_doubleClicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::listview_Thesaurus_doubleClicked( Q3ListViewItem * pQListViewItem )
{if (pQListViewItem==0) return;
 QString             codeCCAM = "";
 if (pQListViewItem) codeCCAM = pQListViewItem->text(2);
 AjouterActeAuDossier(0, codeCCAM);
}

//------------------------------------------ combo_Praticiens_activated ------------------------------------------------
void C_Dlg_FeuillePrincipale::combo_Praticiens_activated( int )
{  m_Code_Praticien = GetCodeComboPraticiens();
    combo_Categories->clear();
    combo_Categories->insertItem(tr("(Toutes)"));
    m_pCMoteurCCAM_Base->GetComboCategorie(combo_Categories, m_Code_Praticien);
    m_pCMoteurCCAM_Base->GetThesaurus(listview_Thesaurus, m_Code_Praticien,combo_Categories->currentText());
}

//------------------------------------------ combo_Categories_activated ------------------------------------------------
void C_Dlg_FeuillePrincipale::combo_Categories_activated( int )
{m_pCMoteurCCAM_Base->GetThesaurus(listview_Thesaurus, m_Code_Praticien,combo_Categories->currentText());
}

//------------------------------------------ bouton_SupprimeActeThesaurus_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::bouton_SupprimeActeThesaurus_clicked()
{ //.................... Verifie si un acte est s\303\251lectionn\303\251 .................................
    if (listview_Thesaurus->selectedItem()==0) return;
    QString acte_selectionne = listview_Thesaurus->selectedItem()->text(4);
    m_pCMoteurCCAM_Base->SupprimeActeThesaurus(acte_selectionne);
    combo_Categories->clear();
    combo_Categories->insertItem(tr("(Toutes)"));
    m_pCMoteurCCAM_Base->GetComboCategorie(combo_Categories,m_Code_Praticien);
    m_pCMoteurCCAM_Base->GetThesaurus(listview_Thesaurus, m_Code_Praticien,combo_Categories->currentText());
    return;
}

//------------------------------------------ bouton_ModifActeThesaurus_clicked() ------------------------------------------------
void C_Dlg_FeuillePrincipale::bouton_ModifActeThesaurus_clicked()
{ //.................... Verifie si un acte est s\303\251lectionn\303\251 .................................
    if (listview_Thesaurus->selectedItem()==0) return;
    QString acte_selectionne = listview_Thesaurus->selectedItem()->text(4);
    //.................... Appel de la fen\303\252tre modification acte thesaurus ....................
    C_Dlg_Categories* dlg_cat = new C_Dlg_Categories(m_pCMoteurCCAM_Base,acte_selectionne,m_Code_Praticien);
    if (dlg_cat==0)      return ; // Erreur initialisation
    //dlg_cat->initDialog(m_pCMoteurCCAM_Base,acte_selectionne,m_Code_Praticien);
    if (dlg_cat->exec()== QDialog::Rejected){
        delete dlg_cat;
        return ; // On quitte sans modifier l'acte
    }
    delete dlg_cat;
    combo_Categories->clear();
    combo_Categories->insertItem(tr("(Toutes)"));
    m_pCMoteurCCAM_Base->GetComboCategorie(combo_Categories, m_Code_Praticien);
    m_pCMoteurCCAM_Base->GetThesaurus(listview_Thesaurus, m_Code_Praticien,combo_Categories->currentText());
    return ;
}

//---// modifie le 26052004--------------------------------------- listview_Thesaurus_selectionChanged() ------------------------------------------------
void C_Dlg_FeuillePrincipale::listview_Thesaurus_selectionChanged()
{ChangeEtatBoutonDossierPatient(ONGLET_THESAURUS); // active ou d\303\251sactive bouton ajouter au dossier patient
}

//---// modifie le 26052004--------------------------------------- ChangeEtatBoutonsDossierPatient ------------------------------------------------
void C_Dlg_FeuillePrincipale::ChangeEtatBoutonDossierPatient( int appelant )
{   switch (appelant){
    case ONGLET_RECHERCHE_CODE_ACTE:
        if (lview_ActesTrouves->selectedItem()!=0){ // si un noueud est s\303\251lectionn\303\251 alors bouton vers Dossier est activ\303\251
            bouton_AjouterDossier->setEnabled(TRUE);
        } else { // sinon il est d\303\251sactiv\303\251
            bouton_AjouterDossier->setEnabled(FALSE);
        }
        break;
    case ONGLET_THESAURUS:
        if (listview_Thesaurus->selectedItem()!=0){ // si un noueud est s\303\251lectionn\303\251 alors bouton vers Dossier est activ\303\251
            bouton_AjouterDossier->setEnabled(TRUE);
            bouton_ModifActeThesaurus->setEnabled(TRUE);
            bouton_SupprimeActeThesaurus->setEnabled(TRUE);
        } else { // sinon il est d\303\251sactiv\303\251
            bouton_AjouterDossier->setEnabled(FALSE);
            bouton_ModifActeThesaurus->setEnabled(FALSE);
            bouton_SupprimeActeThesaurus->setEnabled(FALSE);
        }
        break;
    }
    return;
}

//--// modifie le 26052004---------------------------------------- ChangeEtatBoutonsThesaurus ------------------------------------------------
void C_Dlg_FeuillePrincipale::ChangeEtatBoutonThesaurus()
{ if (lview_ActesTrouves->selectedItem()!=0){ // si un noueud est s\303\251lectionn\303\251 alors bouton vers Dossier est activ\303\251
        bouton_AjouterThesaurus->setEnabled(TRUE);
    } else { // sinon il est d\303\251sactiv\303\251
        bouton_AjouterThesaurus->setEnabled(FALSE);
    }
    return;
}

//--// modifie le 26052004---------------------------------------- lview_ActesTrouves_selectionChanged() ------------------------------------------------
void C_Dlg_FeuillePrincipale::lview_ActesTrouves_selectionChanged()
{
}

//------------------------------------------ lview_Hierarchie_selectionChanged ------------------------------------------------
void C_Dlg_FeuillePrincipale::lview_Hierarchie_selectionChanged( Q3ListViewItem * )
{//if (pQListViewItem==0)
    {ChangeEtatBoutonThesaurus();
     ChangeEtatBoutonDossierPatient(ONGLET_RECHERCHE_CODE_ACTE); // active ou d\303\251sactive bouton ajouter au dossier patient
     ClearAllInformations();
    }
}

//------------------------------------------ lview_Hierarchie_currentChanged ------------------------------------------------
void C_Dlg_FeuillePrincipale::lview_Hierarchie_currentChanged( Q3ListViewItem * )
{//if (pQListViewItem==0)
    {ChangeEtatBoutonThesaurus();
     ChangeEtatBoutonDossierPatient(ONGLET_RECHERCHE_CODE_ACTE); // active ou d\303\251sactive bouton ajouter au dossier patient
     ClearAllInformations();
    }
}

//------------------------------------------ bouton_AjouterThesaurus_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::bouton_AjouterThesaurus_clicked()
{   if (lview_ActesTrouves->selectedItem()==0) return;
    if (m_pCMoteurCCAM_Base==0) return;
    if (IsThisActeInThesaurus(listview_Thesaurus, lview_ActesTrouves->selectedItem()->text(0)))
       {QMessageBox::critical (this, tr(" Erreur CCAM_View") ,
                               tr("Cet acte est d\303\251j\303\240 pr\303\251sent dans le th\303\251saurus "),
                               QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
       }
    else
       {
        m_pCMoteurCCAM_Base->AjoutActesThesaurus(m_Code_Praticien, lview_ActesTrouves->selectedItem()->text(0));
        combo_Categories->clear();
        combo_Categories->insertItem(tr("(Toutes)"));
        m_pCMoteurCCAM_Base->GetComboCategorie(combo_Categories, m_Code_Praticien);
        m_pCMoteurCCAM_Base->GetThesaurus(listview_Thesaurus,    m_Code_Praticien,combo_Categories->currentText());
       }
}

//------------------------------------------ menu_drop_praticiens_clicked ------------------------------------------------
void C_Dlg_FeuillePrincipale::menu_drop_praticiens_clicked()
{
    if (m_Code_Utilisateur == "SIVE99"||m_Code_Utilisateur == "SuperMan-999999999")
       {  // a revoir quand gestion des droits utilisateurs
        QMessageBox mb( tr("ATTENTION !!!"),
                        tr("Cette action effacera toutes les donn\303\251es de la table des PRATICIENS\n"
                        "le programme CCAMVIEW sera alors inutilisable\n"
                        "\n"
                        "Etes vous sur de vouloir effacer tous les Praticiens ?"),
                        QMessageBox::Information,
                        QMessageBox::Yes,
                        QMessageBox::Cancel  | QMessageBox::Default,0);
        mb.setButtonText( QMessageBox::Yes, tr("Oui") );
        mb.setButtonText( QMessageBox::Cancel, tr("Non"));
        switch( mb.exec() ) {
        case QMessageBox::Yes:
            if (m_pCMoteurCCAM_Base==0) return;
            m_pCMoteurCCAM_Base->DropTablePraticiens();
            break;
        case QMessageBox::Cancel:
            // ne fait rien
            break;
        }
    }
}

//------------------------------------------- menu_praticien_clicked -------------------------------------------
void C_Dlg_FeuillePrincipale::menu_praticien_clicked()
{  //.................... Appel de la fen\303\252tre saisie/modification fiche praticien ....................
    Dlg_Praticiens* dlg_prat = new Dlg_Praticiens;
    if (dlg_prat==0)      return ; // Erreur initialisation
    dlg_prat->initDialog(m_pCMoteurCCAM_Base);
    if (dlg_prat->exec()== QDialog::Rejected){
        delete dlg_prat;
        return ; // On quitte sans cr\303\251er de praticien
    }
    delete dlg_prat;
    return ;
}

//------------------------------------------ ChangeEtatOnglet ----------------------------------------------------------------------------------
// *********************************
// *** FONCTION ChangeEtatOnglet ***
// ************************************************************************************************
// * Appel\303\251 apr\303\251s les fonctions qui remplissent les objets situ\303\251s dans les onglets d'informations *
// ************************************************************************************************
// pour changer l'\303\251tat des onglet (enable ou disable) selon si ils sont pleins ou vides
void C_Dlg_FeuillePrincipale::ChangeEtatOnglet( int objet )
{   switch (objet){
    case TB_NOTES:
        if ( textbrowser_notes->text() ==""){
            tabwidget_Informations->setTabEnabled(tab_Notes,FALSE);
        } else {
            tabwidget_Informations->setTabEnabled(tab_Notes,TRUE);
        }
        break;
    case TB_PHASES:
        if ( textbrowser_phases->text() ==""){
            tabwidget_Informations->setTabEnabled(tab_Phases,FALSE);
        } else {
            tabwidget_Informations->setTabEnabled(tab_Phases,TRUE);
        }
        break;
    case TB_HIERARCHIE:
        if ( textbrowser_hierarchie->text() ==""){
            tabwidget_Informations->setTabEnabled(tab_AffHierarchie,FALSE);
        } else {
            tabwidget_Informations->setTabEnabled(tab_AffHierarchie,TRUE);
        }
        break;
    case TB_PROCEDURES:
        if ( textbrowser_procedures->text() ==""){
            tabwidget_Informations->setTabEnabled(tab_Actes,FALSE);
        } else {
            tabwidget_Informations->setTabEnabled(tab_Actes,TRUE);
        }
        break;

    case LV_ASSOCIES:
        if ( lview_ActesAssocies->childCount() == 0){
            tabwidget_Informations->setTabEnabled(tab_Associations,FALSE);
        } else {
            tabwidget_Informations->setTabEnabled(tab_Associations,TRUE);
        }
        break;

    case LV_INCOMPATIBLES:
        if ( lview_ActesIncompatibles->childCount() == 0){
            tabwidget_Informations->setTabEnabled(tab_Incompatibilite,FALSE);
        } else {
            tabwidget_Informations->setTabEnabled(tab_Incompatibilite,TRUE);
        }
        break;
    case LV_COMPLEMENTAIRES:
        if ( lview_GestesComplementaires->childCount() == 0){
            tabwidget_Informations->setTabEnabled(tab_GestesCompelmentaires,FALSE);
        } else {
            tabwidget_Informations->setTabEnabled(tab_GestesCompelmentaires,TRUE);
        }
        break;
    case TB_MODIFICATEURS:
        if ( textbrowser_modificateurs->text() ==""){
            tabwidget_Informations->setTabEnabled(tab_Modificateurs,FALSE);
        } else {
            tabwidget_Informations->setTabEnabled(tab_Modificateurs,TRUE);
        }
        break;
    }
}

//------------------------------------------ NoeudOuvert ---------------------------------------------------------------------------------------
// *****************************
// *** CONNECTEUR NoeudFerme ***
// ****************************************
// * Appel\303\251 quand on clic sur le signe +  *
// ****************************************
// pour changer l'image du noeud
void C_Dlg_FeuillePrincipale::NoeudOuvert( Q3ListViewItem * item )
{
    if (item!=0){
        QImage livre_ouvert(LivreOuvert_xpm);      // d\303\251finition l'image
        QPixmap pix_lo;                        // d\303\251finition l'image
        pix_lo.convertFromImage(livre_ouvert);   // d\303\251finition de l'image
        item->setPixmap(0,pix_lo);             // affectation de l'image au noeud
    }
}

//------------------------------------------ NoeudFerme -----------------------------------------------------------------------------------------
// *****************************
// *** CONNECTEUR NoeudFerme ***
// ****************************************
// * Appel\303\251 quand on clic sur le signe -  *
// ****************************************
// pour changer l'image du noeud
void C_Dlg_FeuillePrincipale::NoeudFerme( Q3ListViewItem * item )
{ if (item!=0){
        QImage livre_ferme(LivreFerme_xpm);       // d\303\251finition l'image
        QPixmap pix_lf;                        // d\303\251finition l'image
        pix_lf.convertFromImage(livre_ferme);  // d\303\251finition de l'image
        item->setPixmap(0,pix_lf);             // affectation de l'image au noeud
    }
}

//------------------------------------------ cbox_SiteAnatomique_highlighted --------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::cbox_SiteAnatomique_highlighted( const QString & qstr )
{QToolTip::add(cbox_SiteAnatomique,qstr);
}

//------------------------------------------ lview_Hierarchie_clicked ---------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::lview_Hierarchie_clicked( Q3ListViewItem * pQListViewItem )
{   if (m_pCMoteurCCAM_Base==0) return;
    if (pQListViewItem==0) {
        lview_ActesTrouves->clear();
        ClearAllInformations();
        return;
       }
    m_pCMoteurCCAM_Base->initArbo02(pQListViewItem, pQListViewItem->text(3));
    //........................... retrouve l'arborescence jusqu'a la racine...........................
    QStringList arborescence;                              // d\303\251claration d'une liste de texte pour stocker l'arborescence compl\303\250te de l'objet
    Q3ListViewItem *noeud = pQListViewItem;                // d\303\251claration d'un pointeur sur le noeud s\303\251lectionn\303\251 pour retrouver l'arborescence jusqu'a la racine
    Q3ListViewItem *noeudparent = pQListViewItem->parent();// d\303\251claration d'un pointeur qui retrouvera le parent du noeud en cours
    arborescence.prepend (pQListViewItem->text(4));              // passage du texte de l'objet vis\303\251
    while (noeudparent != 0) { // tant que le noeud poss\303\251de un parent
        arborescence.prepend(noeudparent->text(4));
        noeud = noeudparent;
        noeudparent = noeud->parent();
    }
    //........................... effectue la recherche ...............................................
    m_pCMoteurCCAM_Base->GetActesParArborescence(lview_ActesTrouves,arborescence);
    lview_ActesTrouves_selectionChanged();// modifie le 26052004
    //on verifie si le noeud poss\303\250de des enfants et on l'ouvre
    if (pQListViewItem->childCount() >0){
        pQListViewItem->setOpen(TRUE);
    }
}

//------------------------------------------ comboBoxActivite_activated ------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::comboBoxActivite_activated( const QString & )
{if (m_pCMoteurCCAM_Base==0) return;
 QString code_acte = edit_Code->text()+"_______";
 m_pCMoteurCCAM_Base->GetActesParMotsClef(lview_ActesTrouves, edit_MotCle->text(), edit_MotCle_2->text(), code_acte.left(7), GetComboActivite()  );
 lview_ActesTrouves_selectionChanged();// modifie le 26052004
}

//--------------------------------- GetPopupItem  -------------------------------------------------------------
void C_Dlg_FeuillePrincipale::GetPopupItem()
{m_RetPopupItem  = m_CurrentPopupItem;
}

//--------------------------------- OnMenuActivated  -------------------------------------------------------------
void C_Dlg_FeuillePrincipale::OnMenuActivated( int id )
{if (id != -1 && m_pQPopupMenu)
    {m_CurrentPopupItem = m_pQPopupMenu->text(id);
    }
}

//------------------------------------------ listview_ListeActePat_clicked ------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::listview_ListeActePat_clicked( Q3ListViewItem * pQListViewItem, const QPoint & p, int c )
{QString ret = "";
 //........... si click en dehors d'un item (deselectionne) ..................
 qDebug(QString::number(c));
 if (pQListViewItem==0)
    {
     bouton_AjouterDossier->setEnabled(FALSE);
     ClearAllInformations();
     return;
    }

 if (m_pCMoteurCCAM_Base==0) return;
 if (c==LVA_CODE_ASSO)
    {QStringList listPopupItem;
     listPopupItem.append("1");
     listPopupItem.append("2");
     listPopupItem.append("3");
     listPopupItem.append("4");
     listPopupItem.append("5");
     ret = DoPopupList(listPopupItem);
     if (ret.length()) pQListViewItem->setText(LVA_CODE_ASSO,ret);
    }
    QWidget * currentPage = tabwidget_Informations->currentPage();  // noter page active
    int index =  tabwidget_Informations->indexOf(currentPage);
    m_pCMoteurCCAM_Base->GetActesIncompatibles(lview_ActesIncompatibles, pQListViewItem->text(LVA_CODECCAM), m_DateEffet); // il s'agira de verifier par rapport \303\264 la date de l'acte
    ChangeEtatOnglet(LV_INCOMPATIBLES);

    // ........................... remonte hierarchie ..................................
    //  (QTextBrowser* pQTextBrowser,const QString &codeCCAM, const QString &libelle, const QString &placeArbo)
    m_pCMoteurCCAM_Base->GetHierarchieActe(textbrowser_hierarchie, pQListViewItem->text(LVA_CODECCAM), pQListViewItem->text(LVA_LIBELLEACTE) );
    ChangeEtatOnglet(TB_HIERARCHIE);
    // ........................... recherche des notes .................................
    m_pCMoteurCCAM_Base->GetNotes(textbrowser_notes, pQListViewItem->text(LVA_CODECCAM));
    ChangeEtatOnglet(TB_NOTES);
    // ........................... recherche des actes isol\303\251s et proc\303\251dures ................................
    m_pCMoteurCCAM_Base->GetActesIsolesProcedures(textbrowser_procedures,pQListViewItem->text(LVA_CODECCAM), m_DateEffet);
    ChangeEtatOnglet(TB_PROCEDURES);
    // ........................... recherche les modificateurs ....................................
    m_pCMoteurCCAM_Base->GetModificateurs(textbrowser_modificateurs,pQListViewItem->text(LVA_CODECCAM), m_DateEffet);
    ChangeEtatOnglet(TB_MODIFICATEURS);
    // ........................... recherche les phases et les activit\303\251s ...................................
    m_pCMoteurCCAM_Base->GetPhases(textbrowser_phases,pQListViewItem->text(LVA_CODECCAM), m_DateEffet);
    ChangeEtatOnglet(TB_PHASES);
    // ........................... recherche les actes associ\303\251s et les gestes compl\303\251mentaires ...................................
    m_pCMoteurCCAM_Base->GetGestesComplementaires(lview_ActesAssocies,lview_GestesComplementaires,pQListViewItem->text(LVA_CODECCAM), m_DateEffet);
    ChangeEtatOnglet(LV_ASSOCIES);
    ChangeEtatOnglet(LV_COMPLEMENTAIRES);
    //bouton_AjouterDossier->setEnabled(TRUE);

    if (tabwidget_Informations->isTabEnabled (currentPage))
        tabwidget_Informations->setCurrentPage(index);
    else
        tabwidget_Informations->setCurrentPage(TB_HIERARCHIE);


}

//------------------------------------------ listview_Thesaurus_clicked ------------------------------------------------------------------------
void C_Dlg_FeuillePrincipale::listview_Thesaurus_clicked( Q3ListViewItem * pQListViewItem )
{//........... si click en dehors d'un item (deselectionne) ..................
 if (pQListViewItem==0){
     bouton_AjouterDossier->setEnabled(FALSE);
     ClearAllInformations();
     return;
    }
 if (m_pCMoteurCCAM_Base==0) return;
    QWidget * currentPage = tabwidget_Informations->currentPage();  // noter page active
    int index =  tabwidget_Informations->indexOf(currentPage);
    m_pCMoteurCCAM_Base->GetActesIncompatibles(lview_ActesIncompatibles, pQListViewItem->text(2), m_DateEffet);
    ChangeEtatOnglet(LV_INCOMPATIBLES);
    // ........................... remonte hierarchie ..................................
    m_pCMoteurCCAM_Base->GetHierarchieActe(textbrowser_hierarchie,pQListViewItem->text(2), pQListViewItem->text(3));
    ChangeEtatOnglet(TB_HIERARCHIE);
    // ........................... recherche des notes .................................
    m_pCMoteurCCAM_Base->GetNotes(textbrowser_notes,pQListViewItem->text(2));
    ChangeEtatOnglet(TB_NOTES);
    // ........................... recherche des actes isol\303\251s et proc\303\251dures ................................
    m_pCMoteurCCAM_Base->GetActesIsolesProcedures(textbrowser_procedures,pQListViewItem->text(2), m_DateEffet);
    ChangeEtatOnglet(TB_PROCEDURES);
    // ........................... recherche les modificateurs ....................................
    m_pCMoteurCCAM_Base->GetModificateurs(textbrowser_modificateurs,pQListViewItem->text(2), m_DateEffet);
    ChangeEtatOnglet(TB_MODIFICATEURS);
    // ........................... recherche les phases et les activit\303\251s ...................................
    m_pCMoteurCCAM_Base->GetPhases(textbrowser_phases,pQListViewItem->text(2), m_DateEffet);
    ChangeEtatOnglet(TB_PHASES);
    // ........................... recherche les actes associ\303\251s et les gestes compl\303\251mentaires ...................................
    m_pCMoteurCCAM_Base->GetGestesComplementaires(lview_ActesAssocies,lview_GestesComplementaires,pQListViewItem->text(2), m_DateEffet);
    ChangeEtatOnglet(LV_ASSOCIES);
    ChangeEtatOnglet(LV_COMPLEMENTAIRES);
    bouton_AjouterDossier->setEnabled(TRUE);

    if (tabwidget_Informations->isTabEnabled (currentPage))
        tabwidget_Informations->setCurrentPage(index);
    else
        tabwidget_Informations->setCurrentPage(TB_HIERARCHIE);
}

//------------------------------------------- ClearAllInformations -------------------------------------------
void C_Dlg_FeuillePrincipale::ClearAllInformations()
{   QWidget * currentPage = tabwidget_Informations->currentPage();  // noter page active
    int index =  tabwidget_Informations->indexOf(currentPage);
    textbrowser_notes->setText("");
    ChangeEtatOnglet(TB_NOTES);
    textbrowser_phases->setText("");
    ChangeEtatOnglet(TB_PHASES);
    textbrowser_hierarchie->setText("");
    ChangeEtatOnglet(TB_HIERARCHIE);
    textbrowser_procedures->setText("");
    ChangeEtatOnglet(TB_PROCEDURES);
    lview_ActesAssocies->clear();
    ChangeEtatOnglet(LV_ASSOCIES);
    lview_ActesIncompatibles->clear();
    ChangeEtatOnglet(LV_INCOMPATIBLES);
    lview_GestesComplementaires->clear();
    ChangeEtatOnglet(LV_COMPLEMENTAIRES);
    textbrowser_modificateurs->setText("");
    ChangeEtatOnglet(TB_MODIFICATEURS);
    tabwidget_Informations->setCurrentPage(index);
}
//------------------------------------------ lview_ActesTrouves_clicked ------------------------------------------------------------------------
// \brief Connecteur appel\303\251 quand il a \303\251t\303\251 cliqu\303\251 sur listview des actes trouv\303\251s     *
// Pour lancer la recherche par le Code Acte
void C_Dlg_FeuillePrincipale::lview_ActesTrouves_clicked( Q3ListViewItem * pQListViewItem )
{//........... si click en dehors d'un item (deselectionne) .................
 if (pQListViewItem==0)
    {bouton_AjouterThesaurus->setEnabled(FALSE);
     bouton_AjouterDossier->setEnabled(FALSE);
     ClearAllInformations();
     return;
    }
 if (m_pCMoteurCCAM_Base==0) return;
    QWidget * currentPage = tabwidget_Informations->currentPage();  // noter page active
    int index =  tabwidget_Informations->indexOf(currentPage);

    m_pCMoteurCCAM_Base->GetActesIncompatibles(lview_ActesIncompatibles, pQListViewItem->text(0), m_DateEffet);
    ChangeEtatOnglet(LV_INCOMPATIBLES);
    // ........................... remonte hierarchie .................................
    m_pCMoteurCCAM_Base->GetHierarchieActe(textbrowser_hierarchie,pQListViewItem->text(0), pQListViewItem->text(1));
    ChangeEtatOnglet(TB_HIERARCHIE);
    // ........................... recherche des notes ................................
    m_pCMoteurCCAM_Base->GetNotes(textbrowser_notes,pQListViewItem->text(0));
    ChangeEtatOnglet(TB_NOTES);
    // ........................... recherche des actes isol\303\251s et proc\303\251dures ................................
    m_pCMoteurCCAM_Base->GetActesIsolesProcedures(textbrowser_procedures,pQListViewItem->text(0), m_DateEffet);
    ChangeEtatOnglet(TB_PROCEDURES);
    // ........................... recherche les modificateurs ...................................
    m_pCMoteurCCAM_Base->GetModificateurs(textbrowser_modificateurs,pQListViewItem->text(0), m_DateEffet);
    ChangeEtatOnglet(TB_MODIFICATEURS);
    // ........................... recherche les phases et les activit\303\251s ...................................
    m_pCMoteurCCAM_Base->GetPhases(textbrowser_phases,pQListViewItem->text(0), m_DateEffet);
    ChangeEtatOnglet(TB_PHASES);
    // ........................... recherche les actes associ\303\251s et les gestes compl\303\251mentaires ...................................
    m_pCMoteurCCAM_Base->GetGestesComplementaires(lview_ActesAssocies,lview_GestesComplementaires,pQListViewItem->text(0), m_DateEffet);
    ChangeEtatOnglet(LV_ASSOCIES);
    ChangeEtatOnglet(LV_COMPLEMENTAIRES);

    if (tabwidget_Informations->isTabEnabled (currentPage))
        tabwidget_Informations->setCurrentPage(index);
    else
        tabwidget_Informations->setCurrentPage(TB_HIERARCHIE);

    bouton_AjouterThesaurus->setEnabled(TRUE);
    bouton_AjouterDossier->setEnabled(TRUE);

}

//------------------------------------------ recherche_par_code ---------------------------------------------------------------------------------
// \brief Connecteur appel\303\251 quand le texte saisi dans 'QLineEdit : edit_Code' change *
// Pour lancer la recherche par le Code Acte
void C_Dlg_FeuillePrincipale::recherche_par_code( const QString & )
{    if (m_pCMoteurCCAM_Base==0) return;
    QString code_acte = edit_Code->text()+"_______";
    m_pCMoteurCCAM_Base->GetActesParMotsClef(lview_ActesTrouves, edit_MotCle->text() , edit_MotCle_2->text(), code_acte.left(7), GetComboActivite() );
    lview_ActesTrouves_selectionChanged();// modifie le 26052004

}

//------------------------------------------ Modif_MotCle1 --------------------------------------------------------------------------------------
// \brief Connecteur appel\303\251 quand le texte saisi dans 'QLineEdit : edit_MotCle' change *
// Pour avoir le retour du mot cl\303\251 \303\264 rechercher
// On appelera d'ici la recherche par mot cl\303\251
void C_Dlg_FeuillePrincipale::Modif_MotCle1( const QString & mot_cle_saisi1 )
{if (m_pCMoteurCCAM_Base==0) return;
 QString code_acte = edit_Code->text()+"_______";
 m_pCMoteurCCAM_Base->GetActesParMotsClef(lview_ActesTrouves, mot_cle_saisi1, edit_MotCle_2->text(), code_acte.left(7), GetComboActivite() , 2);
 lview_ActesTrouves_selectionChanged();// modifie le 26052004
}

//------------------------------------------ Modif_MotCle2 --------------------------------------------------------------------------------------
// \brief Connecteur appel\303\251 quand le texte saisi dans 'QLineEdit : edit_MotCle_2' change  Pour avoir le retour du mot cl\303\251 \303\264 rechercher On appelera d'ici la recherche par mot cl\303\251
void C_Dlg_FeuillePrincipale::Modif_MotCle2( const QString & mot_cle_saisi2 )
{if (m_pCMoteurCCAM_Base==0) return;
 QString code_acte = edit_Code->text()+"_______";
 m_pCMoteurCCAM_Base->GetActesParMotsClef(lview_ActesTrouves, edit_MotCle->text(), mot_cle_saisi2, code_acte.left(7), GetComboActivite() ,2);
 lview_ActesTrouves_selectionChanged();// modifie le 26052004
}

//------------------------------------------ critere_code --------------------------------------------------------------------------------------
// \brief  Connecteur appel\303\251 quand la selection d'une des QComboBox de l'onglet Recherche par Code change   Pour remettre \303\264 jour le crit\303\250re de recherche   qui sera stock\303\251 dans 'QLineEdit : edit_Code'
void C_Dlg_FeuillePrincipale::critere_code( const QString & )
{   QString critere_a_chercher="";
    if (cbox_SiteAnatomique->currentItem() == 0){
        critere_a_chercher = "__";      // 2 underscores pour recherche dans tous les sites
    } else {
        QString site =cbox_SiteAnatomique->text(cbox_SiteAnatomique->currentItem());
        critere_a_chercher = site.left(2);
    }
    if (cbox_Action ->currentItem() == 0){
        critere_a_chercher += "_";      // 1 underscore pour recherche de toutes les actions
    } else {
        QString action =cbox_Action ->text(cbox_Action ->currentItem());
        critere_a_chercher += action.left(1);
    }
    if (cbox_ModeAcces->currentItem() == 0){
        critere_a_chercher += "_";      // 1 underscore pour recherche de tous les acc\303\250s
    } else {
        QString acces =cbox_ModeAcces->text(cbox_ModeAcces->currentItem());
        critere_a_chercher += acces.left(1);
    }
    edit_Code->setText(critere_a_chercher);
}

//------------------------------------------ change_onglet_recherche ---------------------------------------------------------------------------
// \brief On initialise les zones de l'onglet s\303\251lectionn\303\251
void C_Dlg_FeuillePrincipale::change_onglet_recherche( QWidget * onglet_selectionne )
{   QSqlQuery query(QString::null, *m_pCMoteurCCAM_Base->m_pCCAM_Base);
    Q3ListViewItem *pQListViewItem = 0;

    switch (tabwidget_Recherche->indexOf(onglet_selectionne)){
    case ONGLET_RECHERCHE_CODE_ACTE:
        /*
        tabwidget_Recherche->setGeometry(tabwidget_Recherche->x(),
                                         tabwidget_Recherche->y(),
                                         tabwidget_Recherche->width(),
                                         (lview_Hierarchie->y())-10);
        */

        lview_Hierarchie->show();
        lview_ActesTrouves->show();

        pQListViewItem = lview_ActesTrouves->currentItem ();
        if (pQListViewItem && lview_ActesTrouves->isSelected (pQListViewItem))
           {bouton_AjouterThesaurus->setEnabled(TRUE);
            bouton_AjouterDossier->setEnabled(TRUE);
           }
        else
           {bouton_AjouterThesaurus->setEnabled(FALSE);
            bouton_AjouterDossier->setEnabled(FALSE);
           }
        edit_MotCle->setFocus();
        // remplissage de QComboBox : cbox_SiteAnatomique par requete
        if (cbox_SiteAnatomique->count() != 0) return; // si d\303\251j\303\240 fait cassos
        FillComboSitesAnatomiques();
        FillComboAction();
        FillComboModesAcces();
        FillComboDateEffet();
        C_Dlg_FeuillePrincipale::critere_code(QString::null);
       //*/
        break;
    case ONGLET_THESAURUS:
        //m_pCMoteurCCAM_Base->CheckThesaurus(listview_Thesaurus);
        if (cbox_SiteAnatomique->count() <= 0)// si d\303\251j\303\240 fait cassos
           {FillComboSitesAnatomiques();
            FillComboAction();
            FillComboModesAcces();
            FillComboDateEffet();
            C_Dlg_FeuillePrincipale::critere_code(QString::null);
           }
        /*
        tabwidget_Recherche->setGeometry(tabwidget_Recherche->x(),
                                         tabwidget_Recherche->y(),
                                         tabwidget_Recherche->width(),
                                         lview_Hierarchie->y()+lview_Hierarchie->height()+1);
        */
        lview_Hierarchie->hide();
        lview_ActesTrouves->hide();
        // ..... r\303\251cup\303\250re les cat\303\251gories pour la combobox filtre cat\303\251gories .....
        combo_Categories->clear();
        combo_Categories->insertItem(tr("(Toutes)"));
        m_pCMoteurCCAM_Base->GetComboCategorie(combo_Categories,m_Code_Praticien);

        if (listview_Thesaurus->childCount ()==0)
           {QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
            m_pCMoteurCCAM_Base->CheckThesaurus(listview_Thesaurus);
            //m_pCMoteurCCAM_Base->GetThesaurus(listview_Thesaurus, m_Code_Praticien);
            m_pCMoteurCCAM_Base->GetThesaurus(listview_Thesaurus, m_Code_Praticien, tr("(Toutes)"));
            QApplication::restoreOverrideCursor();
           }
        pQListViewItem = listview_Thesaurus->currentItem ();
        if (pQListViewItem && listview_Thesaurus->isSelected (pQListViewItem))
           {bouton_AjouterThesaurus->setEnabled(TRUE);
            bouton_AjouterDossier->setEnabled(TRUE);
           }
        else
           {bouton_AjouterThesaurus->setEnabled(FALSE);
            bouton_AjouterDossier->setEnabled(FALSE);
           }
        ChangeEtatBoutonDossierPatient(ONGLET_THESAURUS); // active ou d\303\251sactive bouton ajouter au dossier patient// modifie le 26052004
        break;
    case ONGLET_RECHERCHE_PATIENT:
        //......... si premi\303\250re fois (pas de base list\303\251e) ............
        if (comboBox_Bases->count()==0)
           {QString str;
            if (m_SigemsLogin==""||m_SigemsPassword=="")
               {C_Dlg_SigemsLogin* dlg = new C_Dlg_SigemsLogin;
                if (dlg)
                   {if (dlg->exec()== QDialog::Accepted)
                       {comboBox_Bases->clear();
                        m_SigemsPassword = dlg->lineEdit_Password->text().stripWhiteSpace();
                        m_SigemsLogin    = dlg->lineEdit_Login->text().stripWhiteSpace();
                       }
                    delete dlg;
                   }
               }
            GetBaseSigems();
            if (CGestIni::Param_ReadParam(Global_ParamData,  comboBox_Bases->currentText(), "Name", &str) ==QString::null )             // zero = pas d'erreur
               {INFO_MED info_med;
                m_pCMoteurCCAM_Base->GetInfoMedecinsFromCode(m_Code_Utilisateur, info_med);
                setCaption( tr("CCAM View Utilis\303\251e par :  ") + info_med.m_Nom + " " + info_med.m_Prenom + tr(" \303\240 : ") + str);
               }
           }
        /*
        tabwidget_Recherche->setGeometry(tabwidget_Recherche->x(),
                                         tabwidget_Recherche->y(),
                                         tabwidget_Recherche->width(),
                                         lview_Hierarchie->y()+lview_Hierarchie->height()+1);
        */
        lview_Hierarchie->hide();
        lview_ActesTrouves->hide();
        bouton_AjouterThesaurus->setEnabled(FALSE);
        bouton_AjouterDossier->setEnabled(FALSE);
        //lineedit_NumDossAn->setFocus ();
        SetFocusOnNumDossAn();
        break;
    }
}

//------------------------------------------ FillComboModesAcces ----------------------------------------------------------------------------------
// \brief rempli le combobox des modes d'acces
void C_Dlg_FeuillePrincipale::FillComboModesAcces()
{   QSqlQuery query(QString::null, *m_pCMoteurCCAM_Base->m_pCCAM_Base);
    cbox_ModeAcces->clear();
    cbox_ModeAcces->insertItem("(Tous)"); // premier item pour la recherche de tous les modes d'acc\303\250s
    query.prepare("SELECT CODE,ACCES FROM ACCES1;");
    query.exec();
    while ( query.next() ) {
        cbox_ModeAcces->insertItem(query.value(0).toString() + " - " + CGestIni::Utf8_Query(query,1));
    }

}

//------------------------------------------ FillComboAction() ----------------------------------------------------------------------------------
// \brief rempli le combobox des actions
void C_Dlg_FeuillePrincipale::FillComboAction()
{  QSqlQuery query(QString::null, *m_pCMoteurCCAM_Base->m_pCCAM_Base);
    cbox_Action->clear();
    cbox_Action->insertItem("(Toutes)"); // premier item pour la recherche de toutes les actions
    query.prepare("SELECT CODE,VERBE FROM ACTION1;");
    query.exec();
    while ( query.next() ) {
        cbox_Action->insertItem(query.value(0).toString() + " - " +     CGestIni::Utf8_Query(query,1));
    }

}

//------------------------------------------ FillComboSitesAnatomiques() ----------------------------------------------------------------------------------
// \brief rempli le combobox des sites anatomiques
void C_Dlg_FeuillePrincipale::FillComboSitesAnatomiques()
{
    QSqlQuery query(QString::null, *m_pCMoteurCCAM_Base->m_pCCAM_Base);
    cbox_SiteAnatomique->clear();
    cbox_SiteAnatomique->insertItem("(Tous)"); // premier item pour la recherche dans tous les sites
    query.prepare("SELECT CODE,LIBELLE FROM TOPOGRAPHIE2;");
    query.exec();
    while ( query.next() ) {
        cbox_SiteAnatomique->insertItem(query.value(0).toString() + " - " +     CGestIni::Utf8_Query(query,1));
    }

}

//------------------------------------------ FillComboDateEffet() ----------------------------------------------------------------------------------
// \brief rempli le combobox de la date d'effet
void C_Dlg_FeuillePrincipale::FillComboDateEffet()
{
    comboBoxDateEffet->clear();
    QString date;
        long l_date          = QDate::currentDate().toString("yyyyMMdd").toLong();
        long l_date_req      = 0;
        long l_date_last     = 0;
        int  last_date_index = -1;
    QSqlQuery query(QString::null, *m_pCMoteurCCAM_Base->m_pCCAM_Base);
    query.prepare("SELECT DATE_EFFET FROM DATES_EFFET ORDER BY DATE_EFFET;");
    query.exec(); qDebug(query.lastError().text ());
    int nb = 0;
    while ( query.next() )
          {
           date       = query.value(0).toString();
           l_date_req = date.toLong();
           if (l_date >= l_date_req)   {l_date_last     = l_date_req;
                                        last_date_index = nb;          // noter l'index de la derni\303\250re date
                                       }
           comboBoxDateEffet->insertItem(nb, date.mid(6,2) + "/" + date.mid(4,2) + "/" + date.left(4),
                                         query.value(0).toString());
           ++nb;
          }
        //........ positionner le combo sur la derni\303\250re date trouv\303\251e ....................
        if (last_date_index != -1)
           {comboBoxDateEffet->setCurrentIndex (last_date_index);
            QString date = QString::number(last_date_index);
            comboBoxDateEffet_activated(date.mid(6,2) + "/" + date.mid(4,2) + "/" + date.left(4));  // ajout de cette ligne pour forcer la m\303\251morisation de la date
           }
}

//------------------------------------------ FillComboActivite ----------------------------------------------------------------------------------
// \brief rempli le combobox d'activit\303\251
void C_Dlg_FeuillePrincipale::FillComboActivite()
{   comboBoxActivite->clear();
    comboBoxActivite->insertItem(tr("Toutes les activit\303\251s           -"));
    QSqlQuery query(QString::null, *m_pCMoteurCCAM_Base->m_pCCAM_Base);
    query.prepare("SELECT CODE,LIBELLE FROM ACTIVITE;");
    query.exec();

    qDebug(query.lastError().text ());
    while ( query.next() ) {
                comboBoxActivite->insertItem(   CGestIni::Utf8_Query(query,1) + "  -" + CGestIni::Utf8_Query(query,0));
            }
}

//---------------------------------------------- closeEvent --------------------------------------------------------------------------------
// \brief Si ce->accept();  alors ensuite va dans: OnQuitterDrTux()
// \brief \303\264 cause de : a.connect( &a,SIGNAL(lastWindowClosed()),  pDrTux, SLOT(OnQuitterDrTux()) );
void C_Dlg_FeuillePrincipale::closeEvent( QCloseEvent * ce )
{//#ifdef Q_WS_WIN
//    int x  =  this->geometry().x();
//    int y  =  this->geometry().y();
//    int w  =  this->width();
//    int h  =  this->height();

//#else
    int x  =  this->x();
    int y  =  this->y();
    int w  =  this->width();
    int h  =  this->height();
//#endif
 tryToStopAPropos();
 CGestIni::Param_WriteParam(&Global_ParamData, "Session", "Position", QString::number(x), QString::number(y), QString::number(w), QString::number(h));
 CGestIni::Param_UpdateToDisk(Global_PathIni, Global_ParamData);
 ce->accept();
 //else                                      ce->ignore();

}

// ---------------------------------- SetInterface ------------------------------------
void C_Dlg_FeuillePrincipale::SetInterface()
{
 if (m_ModeFlag & FULL_FIND)
    {pushButtonClear->show();
     pushButtonNumDossier->show();
     lineedit_Code->setReadOnly(FALSE);
     lineedit_Code1->setReadOnly(FALSE);
     lineedit_Code->show();
     lineedit_Code1->show();
     textLabelTitle1->show();
     textLabelTitle2->show();
     textLabelDateInf->show();
     textLabelDateSup->show();
     pushButtonChercherPatient->show();
     lineedit_Nom->setReadOnly(FALSE);
     lineedit_Nom_Naiss->setReadOnly(FALSE);
     lineedit_Prenom->setReadOnly(FALSE);
     dateEdit_DateNss->setEnabled(TRUE);
     lineedit_Adr_1->setReadOnly(FALSE);
     lineedit_Adr_2->setReadOnly(FALSE);
     lineedit_CodePostal->setReadOnly(FALSE);
     lineedit_Ville->setReadOnly(FALSE);
     lineedit_Ville_Nss->setReadOnly(FALSE);
     lineedit_NumSS->setReadOnly(FALSE);
     lineEdit_CleSS->setReadOnly(FALSE);
     lineedit_Tel->setReadOnly(FALSE);
     lineEdit_Sexe->setReadOnly(FALSE);
     listview_ListePatient->show();
     dateEdit_DateEntree->show();
     dateEdit_DateSortie->show();
     textLabelFlecheDroite->show();
     pushButtonRechAvancee->setText(tr("Recherche simple"));
    }
 else
    {pushButtonClear->hide();
     pushButtonNumDossier->hide();
     lineedit_Code->setReadOnly(TRUE);
     lineedit_Code1->setReadOnly(TRUE);
     lineedit_Code->hide();
     lineedit_Code1->hide();
     textLabelTitle1->hide();
     textLabelTitle2->hide();
     textLabelDateInf->hide();
     textLabelDateSup->hide();
     pushButtonChercherPatient->hide();
     lineedit_Nom->setReadOnly(TRUE);
     lineedit_Nom_Naiss->setReadOnly(TRUE);
     lineedit_Prenom->setReadOnly(TRUE);
     dateEdit_DateNss->setEnabled(FALSE);
     lineedit_Adr_1->setReadOnly(TRUE);
     lineedit_Adr_2->setReadOnly(TRUE);
     lineedit_CodePostal->setReadOnly(TRUE);
     lineedit_Ville->setReadOnly(TRUE);
     lineedit_Ville_Nss->setReadOnly(TRUE);
     lineedit_NumSS->setReadOnly(TRUE);
     lineEdit_CleSS->setReadOnly(TRUE);
     lineedit_Tel->setReadOnly(TRUE);
     lineEdit_Sexe->setReadOnly(TRUE);
     listview_ListePatient->hide();
     dateEdit_DateEntree->hide();
     dateEdit_DateSortie->hide();
     textLabelFlecheDroite->hide();
     pushButtonRechAvancee->setText(tr("Recherche Avanc\303\251e"));
    }

 if (m_GuidDrTux != "" ||
     m_ModeFlag &LOCAL_MODE
    )                   {pushButtonRecActesListToSigems->show();
                         pushButtonRecActesListToSigems->setText(tr("Confirmez"));
                         pushButtonNumDossier->hide();
                         lineedit_Code->hide();
                         lineedit_Code1->hide();
                         comboBox_Bases->hide();
                         textLabelBaseTravail->hide();
                         checkBoxTestMode->hide();
                         pushButtonChercherPatient->hide();
                         pushButtonRechAvancee->hide();
                         listview_ListePatient->hide();
                         textLabelTitle1->hide();
                         textLabelTitle2->hide();
                         textLabelDateInf->hide();
                         textLabelDateSup->hide();
                         groupBox3->hide();
                         textLabelFlecheDroite->hide();
                         dateEdit_DateEntree->hide();
                         dateEdit_DateSortie->hide();
                         listview_ListeDossPat->hide();
                         textLabel1_2->hide();
                         textLabel2_2_2_3_2->hide();
                         tabWidgetRechPatient->changeTab (tabWidgetRechPatient->page (0 ), tr("Liste des Actes Patients ") );
                        }
 else if (m_ModeFlag & SIGEMS_REAL_TIME)
                        {pushButtonRecActesListToSigems->setEnabled(FALSE);
                        }
 if ( !(m_ModeFlag & XML_MONITORING))
       {tabWidgetRechPatient->removePage (tabWidgetRechPatient->page (1));
        tabWidgetRechPatient->removePage (tabWidgetRechPatient->page (1)); // 1 car apres avoir enlever la pr\303\251c\303\251dente elle se retrouvee en 1
       }
}

// ---------------------------------- comboBoxDateEffet_activated ------------------------------------
// \brief m\303\251morisation de la date d'effet
void C_Dlg_FeuillePrincipale::comboBoxDateEffet_activated( const QString & )
{
        m_DateEffet = GetComboDateEffet();
        if (lview_ActesTrouves->childCount() < 1) return;
        lview_ActesTrouves_clicked(lview_ActesTrouves->selectedItem());
}





/****************************************************************************************************************************************
 ****************************************************************************************************************************************
 *                                                         S O A P                                                                      *
 ****************************************************************************************************************************************
 ****************************************************************************************************************************************/
//------------------------------------------ DisplayColoredXML_Request -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::DisplayColoredXML_Request( QString & result )
{QString xml;
 QString http_header;
 if (m_pcSoapI->GetXML_Part(result, xml, &http_header))
    {m_pcSoapI->XML_To_Colored(xml, result );
     textEditSoapMessage->setText(tr( "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                                            "<title>CCAM_View-Sigems-Request</title>"
                                            "</head>"
                                            "<body ><b>")
                                  + http_header.replace("\n","<br />")
                                  + "</b>"
                                  + result
                                  + "</body></html>"
                                 );

    }
 else
    {textEditSoapMessage->setText( result );
    }
}

//------------------------------------------ DisplayColoredXML_Response -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::DisplayColoredXML_Response( QString & xml, CSoapInterface * pcSoapI )
{ QString result = "";
  QString header = pcSoapI->GetResponseHeader();
  pcSoapI->XML_To_Colored(xml, result );
  textEditSoapRespMessage->setText(tr( "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                                            "<title>CCAM_View-Sigems-Response</title>"
                                            "</head>"
                                            "<body ><b>")
                                  + header.replace("\n","<br />")
                                  + "</b>"
                                  + result
                                  + "</body></html>"
                                 );
}

//------------------------------------------ EnregistrerActeParDossierSigems -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::EnregistrerActeParDossierSigems()
{//................................. instancier l'interface SOAP ...............................................
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI    = new CSoapInterface;
 if (m_pcSoapI==0) return;
 QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 statusBar()->message(tr("Enregistrement des actes patient : Transfert des donn\303\251es en cours..... "), 100000 );
 //................................. connecter l'interface SOAP ................................................
 connect(  m_pcSoapI, SIGNAL( done( bool ) ),
           this,      SLOT( Slot_SOAP_EnregistrerActeParDossierSigems_done( bool ) ) );
 connect(  m_pcSoapI, SIGNAL( responseHeaderReceived(     const QHttpResponseHeader& ) ),
           this,      SLOT( Slot_responseHeaderReceived(  const QHttpResponseHeader& ) ) );
 //.................................. la mettre en mode simulation selon checkBoxTestMode ......................
 m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigeGateActeCcam-createActesTempParDossier-reponseCCAMVIEW.txt", checkBoxTestMode->isChecked());

 //................................. URL et autres joyeuset\303\251es ................................................
 QString  url("http://");
 QString  urn = "SigeGateActeCcam";
 url    +=  m_SigemsLogin + ":" + m_SigemsPassword + "@" + m_UrlSigemsWebServices + urn;


 //.............................. message de base ..............................................................
       QString message("");
               message += "<idBase xsi:type=\"xsd:string\">" + comboBox_Bases->currentText() + "</idBase>"
                          "<listeActes href=\"#id0\"/>";

       QString multirefHead_Deb("");
               multirefHead_Deb += "<multiRef id=\"id0\" soapenc:root=\"0\" "
                                               " SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\""
                                               " xsi:type=\"ns1:WsActeCcamListe\""
                                               " xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding/\""
                                               " xmlns:ns1=\"urn:" + urn + "\">" +
                                   "<liste xsi:type=\"soapenc:Array\" soapenc:arrayType=\"ns1:WsActeCcam[";   // rajouter l\303\264 le nb d'actes
       QString multirefHead_Bdy  = "";               //<item href="#id1"/>
       QString multirefHead_End  = "</liste>"
                                   "</multiRef>";

 //............................. iterer la listview des actes ......................................
 int i = 0;
 Q3ListViewItemIterator it( listview_ListeActePat );
 while ( it.current() )
  {Q3ListViewItem *pQListViewItem = it.current();
   ++it;
   if ( pQListViewItem->text(LVA_TYP)=="N" )
      {QString ns = QString::number(i+2);
       ++i;
       multirefHead_Bdy += "<item href=\"#id" + QString::number(i) + "\"/>";
       multirefHead_End +=     "<multiRef id=\"id" + QString::number(i) + "\" soapenc:root=\"0\"" +
                                           " SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\""
                                           " xsi:type=\"ns"+ns+":WsActeCcam\""       // <=== Ici nom du tableau ===> ns2
                                           " xmlns:ns"+ns+"=\"urn:" + urn + "\""                             +
                                           " xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding\">"    +
                                   "<activite xsi:type=\"xsd:string\">"+                  pQListViewItem->text(LVA_ACTIVITE)+      "</activite>"                   // record
                                   "<associationNonPrevue xsi:type=\"xsd:string\">""</associationNonPrevue>"
                                   "<chargeSupplementaire xsi:type=\"xsd:string\">""</chargeSupplementaire>"
                                   "<codeActe xsi:type=\"xsd:string\">"+                  pQListViewItem->text(LVA_CODECCAM)+      "</codeActe>"                   // record
                                   "<codeRejet xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                                   "<date xsi:type=\"xsd:string\">"+                      pQListViewItem->text(LVA_DATE)+          "</date>"                       // record
                                   //"<depassementHonoraire xsi:type=\"xsd:float\" xsi:nil=\"true\"/>"
                                   "<depassementHonoraire xsi:type=\"xsd:float\" xsi:nil=\"true\"/>"
                                   "<enfant xsi:type=\"xsd:int\">"+                       pQListViewItem->text(LVA_ENFANT)+        "</enfant>"                     // record
                                   //"<extensionDocumentaire xsi:type=\"xsd:string\"></extensionDocumentaire>"
                                   "<extensionDocumentaire xsi:type=\"xsd:string\">""</extensionDocumentaire>"
                                   "<heureDebut xsi:type=\"xsd:string\">"+                pQListViewItem->text(LVA_HEURE).left(5)+ "</heureDebut>"                 // record
                                   "<idPraticien xsi:type=\"xsd:string\">"+               pQListViewItem->text(LVA_NIDPRATICIEN)+  "</idPraticien>"                // record
                                   "<ippSigems xsi:type=\"xsd:int\">"+                    lineedit_Code->text().stripWhiteSpace()+ "</ippSigems>"                  // record
                                   //"<lienAvecActeExonereAnterieur xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                                   "<lienAvecActeExonereAnterieur xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                                   "<modificateur1 xsi:type=\"xsd:string\">"+             pQListViewItem->text(LVA_MODIFICATEUR1)+ "</modificateur1>"              // record
                                   "<modificateur2 xsi:type=\"xsd:string\">"+             pQListViewItem->text(LVA_MODIFICATEUR2)+ "</modificateur2>"              // record
                                   "<modificateur3 xsi:type=\"xsd:string\">"+             pQListViewItem->text(LVA_MODIFICATEUR3)+ "</modificateur3>"              // record
                                   "<modificateur4 xsi:type=\"xsd:string\">"+             pQListViewItem->text(LVA_MODIFICATEUR4)+ "</modificateur4>"              // record
                                   "<nbOccurence xsi:type=\"xsd:int\">"+                  pQListViewItem->text(LVA_NB)+            "</nbOccurence>"                // record
                                   "<numDossier xsi:type=\"xsd:string\">"+                pQListViewItem->text(LVA_NUMDOSS)+       "</numDossier>"                 // record
                                   "<phase xsi:type=\"xsd:string\">"+                     pQListViewItem->text(LVA_PHASE)+         "</phase>"                      // record
                                   //"<prescripteur xsi:type=\"xsd:string\">"+              pQListViewItem->text(LVA_PRESCRIPTEUR)+  "</prescripteur>"
                                   "<prescripteur xsi:type=\"xsd:string\">"+              pQListViewItem->text(LVA_PRESCRIPTEUR)+  "</prescripteur>"
                                   "<remboursementExceptionnel xsi:type=\"xsd:string\">"                                           "</remboursementExceptionnel>"
                                   "<uniteMedicale xsi:type=\"xsd:string\">"                                                       "</uniteMedicale>"
                               "</multiRef>";
      } //if ( item->text(LVA_TYP)=="N" )
  } //end while ( it.current() )
 multirefHead_Deb += QString::number(i) +"]\">" +  multirefHead_Bdy + multirefHead_End ;
 //.............................. Envoyer une requ\303\250te avec ce message ..........................................
 //                              le retour est: l'entete http + message soap envoy\303\251
 QString result = m_pcSoapI->SendMessage(url,  "createActesTempParDossier",  message , multirefHead_Deb);

 //............................. afficher de fa\303\247on sympa la requete XML ........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Request(result);
}
//------------------------------------------ Slot_SOAP_EnregistrerActeParDossierSigems_done -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::Slot_SOAP_EnregistrerActeParDossierSigems_done( bool state )
{m_IsEraseActeOK = FALSE;
 //............................. Recuperer la reponse .........................................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 statusBar()->message(tr("Enregistrement d'un acte patient : Traitement des donn\303\251es en cours.... "), 100000 );

 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Remplir la structure: WsPatient ..............................................
 //                              avec le contenu des champs retourn\303\251es (apres parsing XML)
 WsActeDel                      WsActeDel("createActesTempParDossierReturn");
 m_pcSoapI->XML_To_SigemsResult(xml, &WsActeDel );

 //................... envoyer la reponse dans la listview des patients .......................................
 if (WsActeDel.m_RetCode=="0")
    { m_IsEraseActeOK = TRUE;
    }
 else if (WsActeDel.m_RetCode=="-1")
    {m_IsEraseActeOK = FALSE;
    }
 else
    {if (WsActeDel.m_RetCode!="") Display_createActeTemp_SigemsError(WsActeDel.m_RetCode);
     m_IsEraseActeOK = FALSE;
    }
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);
 //...................... on en a plus besoin --> poubelle !!! ..............................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;
 if (m_IsEraseActeOK) statusBar()->message(tr("Enregistrer les actes patient : Traitement des donn\303\251es termin\303\251 "), 100000 );
 else                 statusBar()->message(tr("Echec de l'enregistrements des actes patient "), 100000 );
 //........................ mettre \303\240 jour la list box des actes ...............................................
 if (m_IsEraseActeOK)
    { Q3ListViewItemIterator it( listview_ListeActePat );
      while ( it.current() )
         {Q3ListViewItem *pQListViewItem = it.current();
          ++it;
          if ( pQListViewItem->text(LVA_TYP)=="N" )
             {pQListViewItem->setPixmap ( 0, QPixmap (TempActeXPM_xpm) );
              pQListViewItem->setText(LVA_TYP,"T");
             } //if ( item->text(LVA_TYP)=="N" )
         } //end while ( it.current() )
    } //endif (m_IsEraseActeOK)
 m_pQListViewItemToRecord = 0;
}

//------------------------------------------ EnregistrerActeSigems -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::EnregistrerActeSigems( Q3ListViewItem * pQListViewItem )
{if (pQListViewItem==0) return;
 //................................. instancier l'interface SOAP ...............................................
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI    = new CSoapInterface;
 if (m_pcSoapI==0) return;
 QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 statusBar()->message(tr("Enregistrement d'un acte patient : Transfert des donn\303\251es en cours..... "), 100000 );
 //................................. connecter l'interface SOAP ................................................
 connect(  m_pcSoapI, SIGNAL( done( bool ) ),
           this,      SLOT( Slot_SOAP_EnregistrerActeSigems_done( bool ) ) );
 connect(  m_pcSoapI, SIGNAL( responseHeaderReceived(     const QHttpResponseHeader& ) ),
           this,      SLOT( Slot_responseHeaderReceived(  const QHttpResponseHeader& ) ) );
 //.................................. la mettre en mode simulation selon checkBoxTestMode ......................
 m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigeGateActeCcam-createActeTemp-reponseCCAMVIEW.txt", checkBoxTestMode->isChecked());
//................................. URL et autres joyeuset\303\251es ................................................
 QString  url("http://");
 QString  urn = "SigeGateActeCcam";
 url    +=  m_SigemsLogin + ":" + m_SigemsPassword + "@" + m_UrlSigemsWebServices + urn;

 //.............................. message de base ..............................................................
 QString message = "<idBase xsi:type=\"xsd:string\">"+comboBox_Bases->currentText()+"</idBase>"
                   "<acte href=\"#id0\"/>";

 QString multiref ="<multiref id=\"id0\" soapenc:root=\"0\""
                             " SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\""
                             " xsi:type=\"ns1:WsActeCcam\""       // <=== Ici nom du tableau ===>
                             " xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding\""
                             " xmlns:ns1=\"urn:";
         multiref +=  urn +  "\">"
                                   "<activite xsi:type=\"xsd:string\">"+                  pQListViewItem->text(LVA_ACTIVITE)+      "</activite>"                   // record
                                   "<associationNonPrevue xsi:type=\"xsd:string\">""</associationNonPrevue>"
                                   "<chargeSupplementaire xsi:type=\"xsd:string\">""</chargeSupplementaire>"
                                   "<codeActe xsi:type=\"xsd:string\">"+                  pQListViewItem->text(LVA_CODECCAM)+      "</codeActe>"                   // record
                                   "<codeRejet xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                                   "<date xsi:type=\"xsd:string\">"+                      pQListViewItem->text(LVA_DATE)+          "</date>"                       // record
                                   //"<depassementHonoraire xsi:type=\"xsd:float\" xsi:nil=\"true\"/>"
                                   "<depassementHonoraire xsi:type=\"xsd:float\" xsi:nil=\"true\"/>"
                                   "<enfant xsi:type=\"xsd:int\">"+                       pQListViewItem->text(LVA_ENFANT)+        "</enfant>"                     // record
                                   //"<extensionDocumentaire xsi:type=\"xsd:string\"></extensionDocumentaire>"
                                   "<extensionDocumentaire xsi:type=\"xsd:string\"></extensionDocumentaire>"
                                   "<heureDebut xsi:type=\"xsd:string\">"+                pQListViewItem->text(LVA_HEURE).left(5)+ "</heureDebut>"                 // record
                                   "<idPraticien xsi:type=\"xsd:string\">"+               pQListViewItem->text(LVA_NIDPRATICIEN)+  "</idPraticien>"                // record
                                   "<ippSigems xsi:type=\"xsd:int\">"+                    lineedit_Code->text().stripWhiteSpace()+ "</ippSigems>"                  // record
                                   //"<lienAvecActeExonereAnterieur xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                                   "<lienAvecActeExonereAnterieur xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                                   "<modificateur1 xsi:type=\"xsd:string\">"+             pQListViewItem->text(LVA_MODIFICATEUR1)+ "</modificateur1>"              // record
                                   "<modificateur2 xsi:type=\"xsd:string\">"+             pQListViewItem->text(LVA_MODIFICATEUR2)+ "</modificateur2>"              // record
                                   "<modificateur3 xsi:type=\"xsd:string\">"+             pQListViewItem->text(LVA_MODIFICATEUR3)+ "</modificateur3>"              // record
                                   "<modificateur4 xsi:type=\"xsd:string\">"+             pQListViewItem->text(LVA_MODIFICATEUR4)+ "</modificateur4>"              // record
                                   "<nbOccurence xsi:type=\"xsd:int\">"+                  pQListViewItem->text(LVA_NB)+            "</nbOccurence>"                // record
                                   "<numDossier xsi:type=\"xsd:string\">"+                pQListViewItem->text(LVA_NUMDOSS)+       "</numDossier>"                 // record
                                   "<phase xsi:type=\"xsd:string\">"+                     pQListViewItem->text(LVA_PHASE)+         "</phase>"                      // record
                                   //"<prescripteur xsi:type=\"xsd:string\">"+              pQListViewItem->text(LVA_PRESCRIPTEUR)+  "</prescripteur>"
                                   "<prescripteur xsi:type=\"xsd:string\">"+              pQListViewItem->text(LVA_PRESCRIPTEUR)+  "</prescripteur>"
                                   "<remboursementExceptionnel xsi:type=\"xsd:string\">"                                           "</remboursementExceptionnel>"
                                   "<uniteMedicale xsi:type=\"xsd:string\">"                                                       "</uniteMedicale>"
                   "</multiref>";

 //.............................. Envoyer une requ\303\250te avec ce message ..........................................
 //                              le retour est: l'entete http + message soap envoy\303\251
 m_pQListViewItemToRecord = pQListViewItem;
 QString result = m_pcSoapI->SendMessage(url,  "createActeTemp",  message , multiref);

 //............................. afficher de fa\303\247on sympa la requete XML ........................................
 if ( m_ModeFlag & XML_MONITORING) DisplayColoredXML_Request(result);
}

//------------------------------------------ Slot_SOAP_EnregistrerActeSigems_done -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::Slot_SOAP_EnregistrerActeSigems_done( bool state )
{m_IsEraseActeOK = FALSE;
 //............................. Recuperer la reponse .........................................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 statusBar()->message(tr("Enregistrement d'un acte patient : Traitement des donn\303\251es en cours.... "), 100000 );

 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Remplir la structure: WsPatient ..............................................
 //                              avec le contenu des champs retourn\303\251es (apres parsing XML)
 WsActeDel                      WsActeDel("createActeTempReturn");
 m_pcSoapI->XML_To_SigemsResult(xml, &WsActeDel );

 //................... envoyer la reponse dans la listview des patients .......................................
 if (WsActeDel.m_RetCode=="0")
    { m_IsEraseActeOK = TRUE;
    }
 else if (WsActeDel.m_RetCode=="-1")
    {m_IsEraseActeOK = FALSE;
    }
 else
    {if (WsActeDel.m_RetCode!="") Display_createActeTemp_SigemsError(WsActeDel.m_RetCode);
     m_IsEraseActeOK = FALSE;
    }
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);
 //...................... on en a plus besoin --> poubelle !!! ..............................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;
 if (m_IsEraseActeOK) statusBar()->message(tr("Enregistrer un acte patient : Traitement des donn\303\251es termin\303\251 "), 100000 );
 else                 statusBar()->message(tr("Echec de l'enregistrement d'un acte patient "), 100000 );
 //........................ mettre \303\264 jour la list box des actes ...............................................
 if (m_IsEraseActeOK)
    {if (m_pQListViewItemToRecord)
        {m_pQListViewItemToRecord->setPixmap ( 0, QPixmap (TempActeXPM_xpm) );
         m_pQListViewItemToRecord->setText(LVA_TYP,"T");
        }
    }
 else if (m_ModeFlag&SIGEMS_REAL_TIME)
    {if (m_pQListViewItemToRecord)
        {delete m_pQListViewItemToRecord;
        }
    }
 m_pQListViewItemToRecord = 0;
}

//------------------------------------------ Display_createActeTemp_SigemsError -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::Display_createActeTemp_SigemsError( QString error_str )
{
 char *pt  = 0;
 switch (error_str.toInt())
    {case -1:  pt = ("Votre requ\303\252te s'est mal pass\303\251e ou ne retourne aucun r\303\251sultat.");                    break;
     case -2:  pt = ("Les param\303\250tres d'entr\303\251e sont tous vides.");                                         break;
     case -6:  pt = ("Erreur interne du service, veuillez en informer la soci\303\251t\303\251 Sigems.");               break;
     case -7:  pt = ("Acc\303\251s refus\303\251.");                                                                    break;
     case -21: pt = ("Num\303\251ro national de praticien inconnu.");                                            break;
     case -50: pt = ("Le champ numDossier doit obligatoirement \303\252tre rempli.");                            break;
     case -51: pt = ("Le champ ippPatient doit obligatoirement \303\252tre rempli.");                            break;
     case -52: pt = ("Le champ date doit obligatoirement \303\252tre rempli au format jj/mm/aaaa.");             break;
     case -53: pt = ("Le champ heure doit obligatoirement \303\252tre rempli au format hh:mm.");                 break;
     case -58: pt = ("Le champ heure n'est pas valide. Le format attendu est hh:mm.");                    break;
     case -59: pt = ("Le num\303\251ro de dossier fourni n'est pas enregistr\303\251 dans la base Sigems.");            break;
     case -60: pt = ("Le patient dont vous aves fourni l'ipp, dans le champ ippSigems,\n"
                     "n'a pas de dossier du num\303\251ro fourni dans le champ numDossier.");                    break;
     case -64: pt = ("Vous ne pouvez travailler:\n"
                       "     -  ni apr\303\250s la date de sortie d\303\251finitive de l'\303\251tablissement, du patient\n "
                       "     -  ni apr\303\250s la date de sortie pr\303\251vue, si la date de sortie d\303\251finitive n'est pas renseign\303\251e\n "
                       "     -  ni apr\303\250s la date de sortie d\303\251finitive de l'\303\251tablissement, du patient. ");   break;
     case -66:  pt = ("Vous ne pouvez travailler avant la date d'etr\303\251e du patient dans l'\303\251tablisement."); break;
     case -101: pt = ("Le champ enfant doit obligatoirement \303\252tre rempli.");                               break;
     case -102: pt = ("Le champ activite doit obligatoirement \303\252tre rempli.");                             break;
     case -103: pt = ("Le champ phase doit obligatoirement \303\252tre rempli.");                                break;
     case -104: pt = ("Le champ enfant doit obligatoirement \303\252tre rempli.");                               break;
     case -105: pt = ("Le champ nbOccurence doit obligatoirement \303\252tre rempli.");                          break;
     case -106: pt = ("Le champ idPraticien doit obligatoirement \303\252tre rempli.");                          break;
     case -108: pt = ("Le champ enfant est un entier positif:\n"
                        "   - 0 par d\303\251faut ou pour une m\303\250re\n"
                        "   - 1 pour son premier nouveau-n\303\251\n"
                        "   - 2 pour son second jumeau dans le cas o\303\231 les nouveaux-n\303\251s sont jumeaux\n"
                        "   - etc.");                                                                       break;
     case -109: pt = ("le code acte est un champ sur 7 positions:\n"
                        "   - les 4 premiers caract\303\250res sont des lettres,\n"
                        "   - les 3 derniers caract\303\250res sont des chiffres");                                break;
     case -110: pt = ("Le code activit\303\251 est une chaine de caract\303\250res representant\n"
                        " un entier compris entre 1 et 5 inclus.");                                         break;
     case -111: pt = ("Le code activit\303\251 est une chaine de caract\303\250res representant\n"
                        "un entier positif compris entre 0 et 9 inclus.");                                  break;
     case -112: pt = ("Le champ nbOccurence est un entier strictement positif.");                         break;
     case -113: pt = ("Le champ extensionDocumentaire est un caract\303\250re (1 position).");                   break;
     case -114: pt = ("Les champs modificateur s'\303\251crivent sur 1 position (1 caract\303\250re).");                break;
     case -115: pt = ("Le champ associationNonPrevue s'\303\251crit sur 1 position (1 caract\303\250re).");             break;
     case -116: pt = ("Le champ remboursementExceptionnel s'\303\251crit sur 1 position (1 caract\303\250re).");        break;
     case -117: pt = ("Le champ chargeSupplementaire s'\303\251crit sur 1 position (1 caract\303\250re).");             break;
     case -118: pt = ("createActeTempParDossier: aucun acte concernant ce dossier n'a \303\251t\303\251 cr\303\251\303\251\n"
                        "car une erreur interne est survenue");                                             break;
     case -119: pt = ("createActeTempParDossier: les actes que vous voulez cr\303\251er n'appartiennent\n"
                        "pas tous au m\303\252me dossier");                                                        break;
     case -120: pt = ("deleteActeTemp: l'acte que vous voulez supprimer, n'existe pas ou plus\n"
                        "dans la table temporaire des actes Sigems");                                       break;
     case -121: pt = ("Vous ne pouvez pas ins\303\251rer un acte sur un dossier de pr\303\251-admission");              break;
     case -122: pt = ("Le prescripteur est obligatoire lorsque l'ex\303\251cutant, de code Ad\303\251li de\n"
                        "idPraticien contient 0, 3 ou 7 en 3\303\250me position.\n");                              break;
     case -123: pt = ("Le champ facultatif lienAvecActeExonereAnterieur prend sa valeur parmi {O,N}");    break;
     case -124: pt = ("Le dossier dans lequel vouv voulez ajouter un acte a \303\251t\303\251 contr\303\264l\303\251 et valid\303\251\n"
                        "par le m\303\251decin DIM. Vous ne pouvez plus ajouter ou supprimer d'actes ou de\n"
                        "diagnostics \303\240 ce dossier.");                                                      break;
     default:   pt = ("Erreur inconnue.");
    }
 QString resultStr ("");
 resultStr += tr("Erreur SIGEMS N° ") + error_str + "\n";
 resultStr.append(tr(pt));
 QMessageBox::warning( 0,tr("Erreur"), resultStr );
}

//------------------------------------------ EffacerActeSigems -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::EffacerActeSigems( Q3ListViewItem * pQListViewItem )
{if (pQListViewItem==0) return;
 QStringList data_list;
 QListViewItemActe_To_QStringList(pQListViewItem, data_list);
 m_pQListViewItemToErase = pQListViewItem;
 EffacerActeSigems(data_list);
}

//------------------------------------------ EffacerActeSigems -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::EffacerActeSigems( QStringList & data_list )
{//................................. instancier l'interface SOAP ...............................................
 m_IsEraseActeOK = FALSE;
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI    = new CSoapInterface;
 if (m_pcSoapI==0) return;
 QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 statusBar()->message(tr("Effacement d'un acte patient : Transfert des donn\303\251es en cours..... "), 100000 );
 //................................. connecter l'interface SOAP ................................................
 connect(  m_pcSoapI, SIGNAL( done( bool ) ),
           this,      SLOT( Slot_SOAP_EffacerActeSigems_done( bool ) ) );
 connect(  m_pcSoapI, SIGNAL( responseHeaderReceived(     const QHttpResponseHeader& ) ),
           this,      SLOT( Slot_responseHeaderReceived(  const QHttpResponseHeader& ) ) );
 //.................................. la mettre en mode simulation selon checkBoxTestMode ......................
 m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigeGateActeCcam-deleteActeTemp-reponseCCAMVIEW.txt", checkBoxTestMode->isChecked());
//................................. URL et autres joyeuset\303\251es ................................................
 QString  url("http://");
 QString  urn = "SigeGateActeCcam";
 url    +=  m_SigemsLogin + ":" + m_SigemsPassword + "@" + m_UrlSigemsWebServices + urn;

 //.............................. message de base ..............................................................
 QString message = "<idBase xsi:type=\"xsd:string\">"+comboBox_Bases->currentText()+"</idBase>"
                   "<acte href=\"#id0\"/>";

 QString multiref ="<multiref id=\"id0\" soapenc:root=\"0\""
                             " SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\""
                             " xsi:type=\"ns1:WsActeCcam\""       // <=== Ici nom du tableau ===>
                             " xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding\""
                             " xmlns:ns1=\"urn:";
        multiref +=  urn + "\">"
                   "<activite xsi:type=\"xsd:string\">"     + data_list[LVA_ACTIVITE]       +"</activite>"              // del
                   "<associationNonPrevue xsi:type=\"xsd:string\"></associationNonPrevue>"
                   "<chargeSupplementaire xsi:type=\"xsd:string\"></chargeSupplementaire>"
                   "<codeActe xsi:type=\"xsd:string\">"     + data_list[LVA_CODECCAM]       +"</codeActe>"              // del
                   "<codeRejet xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<date xsi:type=\"xsd:string\">"         + data_list[LVA_DATE]           +"</date>"                          // del
                   "<depassementHonoraire xsi:type=\"xsd:float\" xsi:nil=\"true\"/>"
                   "<enfant xsi:type=\"xsd:int\">"          + data_list[LVA_ENFANT]         +"</enfant>"                       // del
                   "<extensionDocumentaire xsi:type=\"xsd:string\"></extensionDocumentaire>"
                   "<heureDebut xsi:type=\"xsd:string\">"   + data_list[LVA_HEURE].left(5)  + "</heureDebut>"        // del
                   "<idPraticien xsi:type=\"xsd:string\"></idPraticien>"
                   "<ippSigems xsi:type=\"xsd:int\" xsi:nil=\"true\"/>"
                   "<lienAvecActeExonereAnterieur xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<modificateur1 xsi:type=\"xsd:string\"></modificateur1>"
                   "<modificateur2 xsi:type=\"xsd:string\"></modificateur2>"
                   "<modificateur3 xsi:type=\"xsd:string\"></modificateur3>"
                   "<modificateur4 xsi:type=\"xsd:string\"></modificateur4>"
                   "<nbOccurence xsi:type=\"xsd:int\" xsi:nil=\"true\"/>"
                   "<numDossier xsi:type=\"xsd:string\">"   + data_list[LVA_NUMDOSS]        +"</numDossier>"          // del
                   "<phase xsi:type=\"xsd:string\">"        + data_list[LVA_PHASE]          +"</phase>"                         // del
                   "<prescripteur xsi:type=\"xsd:string\">" + data_list[LVA_PRESCRIPTEUR]   +"</prescripteur>"
                   "<remboursementExceptionnel xsi:type=\"xsd:string\"></remboursementExceptionnel>"
                   "<uniteMedicale xsi:type=\"xsd:string\"></uniteMedicale>"
                   "</multiref>\n";

 //.............................. Envoyer une requ\303\250te avec ce message ..........................................
 //                              le retour est: l'entete http + message soap envoy\303\251
 QString result = m_pcSoapI->SendMessage(url,  "deleteActeTemp",  message , multiref);

 //............................. afficher de fa\303\247on sympa la requete XML ........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Request(result);
}

//------------------------------------------ Slot_SOAP_EffacerActeSigems_done -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::Slot_SOAP_EffacerActeSigems_done( bool state )
{//............................. Recuperer la reponse .........................................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 statusBar()->message(tr("Effacer un acte patient : Traitement des donn\303\251es en cours.... "), 100000 );

 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Remplir la structure: WsPatient ..............................................
 //                              avec le contenu des champs retourn\303\251es (apres parsing XML)
 WsActeDel                      WsActeDel;
 m_pcSoapI->XML_To_SigemsResult(xml, &WsActeDel );

 //................... envoyer la reponse dans la listview des patients .......................................
 if (WsActeDel.m_RetCode=="0")
    { m_IsEraseActeOK = TRUE;
    }
 else if (WsActeDel.m_RetCode=="-1")
    {m_IsEraseActeOK = FALSE;
    }
 else
    {if (WsActeDel.m_RetCode!="") Display_createActeTemp_SigemsError(WsActeDel.m_RetCode);
     m_IsEraseActeOK = FALSE;
    }
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);
 //...................... on en a plus besoin --> poubelle !!! ..............................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;
 if (m_IsEraseActeOK) statusBar()->message(tr("Effacer un acte patient : Traitement des donn\303\251es termin\303\251 "), 100000 );
 else                 statusBar()->message(tr("Echec de l'effacement d'un acte patient "), 100000 );
 //........................ mettre \303\240 jour la list box des actes ...............................................
 if (m_IsEraseActeOK && m_pQListViewItemToErase)
    { EraseAllSameActes(m_pQListViewItemToErase);
    }

 m_pQListViewItemToErase = 0;
 m_IsEraseActeOK=FALSE;
 //................................ lors update il faut effacer ancien acte et enregistrer le nouveau .............................
 if (m_pQListViewItemToRecord)
    {EnregistrerActeSigems(m_pQListViewItemToRecord);
    }
}

//------------------------------------------ ListerSigemsActesPatient -----------------------------------------------------------------
// mode = "searchActeTemp"
//        "searchActe"
void C_Dlg_FeuillePrincipale::ListerSigemsActesPatient( QString numDoss, QString idPrat, QString mode )
{//................................. instancier l'interface SOAP ...............................................
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI    = new CSoapInterface;
 if (m_pcSoapI==0) return;
 QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 statusBar()->message(tr("Rechercher les actes d'un patient : Transfert des donn\303\251es en cours..... "), 100000 );

 //................................. connecter l'interface SOAP ................................................
if (mode=="searchActeTemp")
   {//.................................. la mettre en mode simulation selon checkBoxTestMode ......................
    m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigeGateActeCcam-searchActeTemp-reponseCCAMVIEW.txt", checkBoxTestMode->isChecked());
    connect(  m_pcSoapI, SIGNAL( done( bool ) ),
              this,      SLOT( Slot_SOAP_ListerSigemsActesTempPatient_done( bool ) ) );
   }
else //        "searchActe"
   {//.................................. la mettre en mode simulation selon checkBoxTestMode ......................
    m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigeGateActeCcam-searchActe-reponseCCAMVIEW.txt", checkBoxTestMode->isChecked());
    connect(  m_pcSoapI, SIGNAL( done( bool ) ),
              this,      SLOT( Slot_SOAP_ListerSigemsActesPatient_done( bool ) ) );
   }
 connect(  m_pcSoapI, SIGNAL( responseHeaderReceived(     const QHttpResponseHeader& ) ),
           this,      SLOT( Slot_responseHeaderReceived(  const QHttpResponseHeader& ) ) );

 //................................. URL et autres joyeuset\303\251es ................................................
 QString  url("http://");
 QString  urn = "SigeGateActeCcam";
 url    +=  m_SigemsLogin + ":" + m_SigemsPassword + "@" + m_UrlSigemsWebServices + urn;

 //.............................. message de base ..............................................................
 QString message("");
         message +="<idBase xsi:type=\"xsd:string\">" +comboBox_Bases->currentText()+"</idBase>"
                   "<page xsi:type=\"xsd:int\">1</page>"
                   "<nbLigne xsi:type=\"xsd:int\">50</nbLigne>"
                   "<dossier href=\"#id0\"/>"
                   "<dateInf xsi:type=\"xsd:string\">"+dateEdit_DateEntree->date().toString ("dd/MM/yyyy")+"</dateInf>"
                   "<dateSup xsi:type=\"xsd:string\">"+dateEdit_DateSortie->date().toString ("dd/MM/yyyy")+"</dateSup>";

 QString multiref ="<multiref id=\"id0\" soapenc:root=\"0\""
                             " SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\""
                             " xsi:type=\"ns1:WsActeCcam\""       // <=== Ici nom du tableau ===>
                             " xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding\""
                             " xmlns:ns1=\"urn:";
        multiref +=   urn + "\">"
                   "<activite xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<associationNonPrevue xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<chargeSupplementaire xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<codeActe xsi:type=\"xsd:string\"></codeActe>"
                   "<codeRejet xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<date xsi:type=\"xsd:string\"></date>"
                   "<depassementHonoraire xsi:type=\"xsd:float\" xsi:nil=\"true\"/>"
                   "<enfant xsi:type=\"xsd:int\" xsi:nil=\"true\"/>"
                   "<extensionDocumentaire xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<heureDebut xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<idPraticien xsi:type=\"xsd:string\">"+idPrat+"</idPraticien>"
                   "<ippSigems xsi:type=\"xsd:int\" xsi:nil=\"true\"/>"
                   "<lienAvecActeExonereAnterieur xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<modificateur1 xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<modificateur2 xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<modificateur3 xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<modificateur4 xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<nbOccurence xsi:type=\"xsd:int\" xsi:nil=\"true\"/>"
                   "<numDossier xsi:type=\"xsd:string\">"+numDoss+"</numDossier>"
                   "<phase xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<prescripteur xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<remboursementExceptionnel xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "<uniteMedicale xsi:type=\"xsd:string\" xsi:nil=\"true\"/>"
                   "</multiref>\n";
 //.............................. Envoyer une requ\303\250te avec ce message ..........................................
 //                              le retour est: l'entete http + message soap envoy\303\251
 // mode = "searchActeTemp"
 //        "searchActe"
 QString result = m_pcSoapI->SendMessage(url,  mode,  message , multiref);

 //............................. afficher de fa\303\247on sympa la requete XML ........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Request(result);
}

//------------------------------------------ Slot_SOAP_ListerSigemsActesTempPatient_done -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::Slot_SOAP_ListerSigemsActesTempPatient_done( bool state )
{//............................. Recuperer la reponse .........................................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 statusBar()->message(tr("Chercher les actes d'un patient : Traitement des donn\303\251es en cours.... "), 100000 );

 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Remplir la structure: WsPatient ..............................................
 //                              avec le contenu des champs retourn\303\251es (apres parsing XML)
 WsActeCcamListe                      wsActeCcamListe("searchActeTempReturn");
 m_pcSoapI->XML_To_SigemsResult(xml, &wsActeCcamListe );

 //................... envoyer la reponse dans la listview des patients .......................................
 if (wsActeCcamListe.m_RetCode=="0")
    {
      for (int i=0; i<wsActeCcamListe.m_Nb; ++i)
          {INFO_MED info_med;
           m_pCMoteurCCAM_Base->GetInfoMedecinsFromNid(wsActeCcamListe.m_pWsActeCcam[i].m_idPraticien, info_med);

           Q3ListViewItem *element = new Q3ListViewItem(listview_ListeActePat);
           if (element)
              {element->setText ( LVA_DATE,          wsActeCcamListe.m_pWsActeCcam[i].m_date );
               element->setText ( LVA_HEURE,         wsActeCcamListe.m_pWsActeCcam[i].m_heureDebut);
               element->setText ( LVA_PRATICIENNAME, info_med.m_Nom    + " " + info_med.m_Prenom );
               element->setText ( LVA_CODECCAM,      wsActeCcamListe.m_pWsActeCcam[i].m_codeActe);
               element->setText ( LVA_LIBELLEACTE,   m_pCMoteurCCAM_Base->CodeCCAMToLibelleLong(wsActeCcamListe.m_pWsActeCcam[i].m_codeActe));
               element->setText ( LVA_TARIF,         m_pCMoteurCCAM_Base->GetPrixActe(wsActeCcamListe.m_pWsActeCcam[i].m_codeActe,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_date,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_activite,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_phase,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_modificateur1,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_modificateur2,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_modificateur3,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_modificateur4));
               element->setText ( LVA_PHASE,         wsActeCcamListe.m_pWsActeCcam[i].m_phase);
               element->setText ( LVA_ACTIVITE,      wsActeCcamListe.m_pWsActeCcam[i].m_activite);
               element->setText ( LVA_MODIFICATEUR1, wsActeCcamListe.m_pWsActeCcam[i].m_modificateur1);
               element->setText ( LVA_MODIFICATEUR2, wsActeCcamListe.m_pWsActeCcam[i].m_modificateur2);
               element->setText ( LVA_MODIFICATEUR3, wsActeCcamListe.m_pWsActeCcam[i].m_modificateur3);
               element->setText ( LVA_MODIFICATEUR4, wsActeCcamListe.m_pWsActeCcam[i].m_modificateur4);
               element->setText ( LVA_NB,            wsActeCcamListe.m_pWsActeCcam[i].m_nbOccurence);
               element->setText ( LVA_NUMDOSS,       wsActeCcamListe.m_pWsActeCcam[i].m_numDossier);
               element->setText ( LVA_NOMENCLAT,     "CCAM");
               element->setText ( LVA_NIDPRATICIEN,  wsActeCcamListe.m_pWsActeCcam[i].m_idPraticien);
               element->setText ( LVA_TYP,           "T");              // temporaire
               element->setPixmap ( 0, QPixmap (TempActeXPM_xpm) );
               element->setText ( LVA_ENFANT,        wsActeCcamListe.m_pWsActeCcam[i].m_enfant);
               element->setText ( LVA_PRESCRIPTEUR,  wsActeCcamListe.m_pWsActeCcam[i].m_prescripteur);
               element->setText ( LVA_CODE_REJET,    wsActeCcamListe.m_pWsActeCcam[i].m_codeRejet);
               listview_ListeActePat->insertItem ( element );
              } //endif (element)
         } // end for (int i=0; i<wsActeCcamListe.m_Nb; ++i)
    }
 else if (wsActeCcamListe.m_RetCode=="-1")
    {//listview_ListeActePat->clear();
    }
 else m_pcSoapI->DisplaySigemsError(wsActeCcamListe.m_RetCode);
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);
 //...................... on en a plus besoin --> poubelle !!! ..............................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;
 statusBar()->message(tr("Chercher les actes d'un patient : Traitement des donn\303\251es termin\303\251 "), 100000 );
}


//------------------------------------------ Slot_responseHeaderReceived ---------------------------------------------------------------------
// ****************************************************
// *** CONNECTEUR Slot_responseHeaderReceived       ***
// *******************************************************
// * Appel\303\251 http a fini d'envoyer le header              *
// *******************************************************
// pour envoyer requete de rcherche patient Sigems
void C_Dlg_FeuillePrincipale::Slot_responseHeaderReceived( const Q3HttpResponseHeader & response )
{m_pcSoapI->ResponseHeaderToStr(response ); // en mode debug cet appel est simul\303\251
}

//------------------------------------------ Slot_SOAP_SelectDossierHospi_done ------------------------------------------------
// *******************************************************
// *** CONNECTEUR Slot_SOAP_GetDatabasesList_done      ***
// *******************************************************
// * Appel\303\251 http a fini d'envoyer les donn\303\251es            *
// *******************************************************
// pour envoyer requete de rcherche patient Sigems
void C_Dlg_FeuillePrincipale::Slot_SOAP_SelectDossierHospi_done( bool state )
{int updatePatList = m_UpdatePatList;
  m_UpdatePatList   = 0;
 //............................. Recuperer la reponse .........................................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 statusBar()->message(tr("S\303\251lection d'un dossier patient : Traitement des donn\303\251es en cours.... "), 100000 );

 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Remplir la structure: WsPatient ..............................................
 //                              avec le contenu des champs retourn\303\251es (apres parsing XML)
 WsDossier                            dossier;
 m_pcSoapI->XML_To_SigemsResult(xml, &dossier);

 //................... envoyer la reponse dans la listview des dossiers patient ..............................
 listview_ListeDossPat->clear();
 if (dossier.m_RetCode=="0")
    {new Q3ListViewItem(listview_ListeDossPat,
                       dossier.m_numero,
                       dossier.m_service,
                       dossier.m_dateEntree,
                       dossier.m_dateSortieReelle,
                       dossier.m_motif,
                       dossier.m_idChirurgien);
    }
 else if (dossier.m_RetCode != "-1") m_pcSoapI->DisplaySigemsError(dossier.m_RetCode);
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);
 //...................... on en a plus besoin --> poubelle !!! ................................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;
 //....................... mettre \303\264 jour le patient si besoin .................................................
 if (dossier.m_RetCode=="0")
    {m_UpdatePatList = 2;    // 2 -> mettre \303\264 jour liste patient et identit\303\251 patient mais pas sa liste de dossiers d'hospi (on part de l\303\264 !!)
     SelectPatientSigems( dossier.m_ippSigemsPatient);
    }
 statusBar()->message(tr("S\303\251lection d'un dossier patient : Traitement des donn\303\251es termin\303\251 "), 100000 );
}

//------------------------------------------ SelectDossierHospiSigems ----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::SelectDossierHospiSigems( const QString & numDoss )
{QString str("");
 //................................. instancier l'interface SOAP ...............................................
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI    = new CSoapInterface;
 if (m_pcSoapI==0) return;
 QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 statusBar()->message(tr("S\303\251lection d'un dossier patient : Transfert des donn\303\251es en cours.... "), 100000 );

 //.................................. la mettre en mode simulation selon checkBoxTestMode ......................
 m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigemsRet-SelectDossier.txt", checkBoxTestMode->isChecked()); // ##SelectPatient

 //................................. connecter l'interface SOAP ................................................

 connect(  m_pcSoapI,   SIGNAL( done( bool ) ),
           this,        SLOT( Slot_SOAP_SelectDossierHospi_done( bool ) ) );      // ## SelectPatient
 connect(  m_pcSoapI,   SIGNAL( responseHeaderReceived(    const QHttpResponseHeader& ) ),
           this,        SLOT( Slot_responseHeaderReceived( const QHttpResponseHeader& ) ) );

 //.............................. message de base ..............................................................
 QString message("");
         message += "<idBase xsi:type=\"xsd:string\">"+ comboBox_Bases->currentText() +"</idBase>\n"
                    "<numero xsi:type=\"xsd:string\">"+ numDoss                       +"</numero>\n";

 //.............................. Envoyer une requ\303\250te avec ce message ..........................................
 //                              le retour est: l'entete http + message soap envoy\303\251
 QString url("http://");  url   +=  m_SigemsLogin + ":" + m_SigemsPassword + "@" + m_UrlSigemsWebServices;
 if (m_SigemsVers=="01")  url   +=  "SigeGateDos";
 else                     url   +=  "SigeGateMed";

 QString result = m_pcSoapI->SendMessage(url,  "selectDossier",  message );             // ## SelectPatient

 //............................. afficher de fa\303\247on sympa la requete XML ........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Request(result);
}

//------------------------------------------ Slot_SOAP_SearchDossier_done ------------------------------------------------
// *******************************************************
// *** CONNECTEUR Slot_SOAP_SearchDossier_done      ***
// *******************************************************
// * Appel\303\251 http a fini d'envoyer les donn\303\251es            *
// *******************************************************
// pour recuperer le resultat de la requete de recherche Dossier Sigems
void C_Dlg_FeuillePrincipale::Slot_SOAP_SearchDossier_done( bool state )
{//............................. Recuperer la reponse .........................................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 statusBar()->message(tr("Chercher les dossiers d'un patient : Traitement des donn\303\251es en cours.... "), 100000 );

 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Remplir la structure: WsPatient ..............................................
 //                              avec le contenu des champs retourn\303\251es (apres parsing XML)
 WsDossierListe                       wsDossierListe;
 m_pcSoapI->XML_To_SigemsResult(xml, &wsDossierListe );

 //................... envoyer la reponse dans la listview des patients .......................................
 if (wsDossierListe.m_RetCode=="0")
    { listview_ListeDossPat->clear();
      for (int i=0; i<wsDossierListe.m_Nb; ++i)
          {new Q3ListViewItem(listview_ListeDossPat,
                             wsDossierListe.m_pWsDossier[i].m_numero,
                             wsDossierListe.m_pWsDossier[i].m_service,
                             wsDossierListe.m_pWsDossier[i].m_dateEntree,
                             wsDossierListe.m_pWsDossier[i].m_dateSortieReelle,
                             wsDossierListe.m_pWsDossier[i].m_motif,
                             wsDossierListe.m_pWsDossier[i].m_idChirurgien);
          }
    }
 else if (wsDossierListe.m_RetCode=="-1")
    {listview_ListeDossPat->clear();
    }
 else m_pcSoapI->DisplaySigemsError(wsDossierListe.m_RetCode);
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);
 //...................... on en a plus besoin --> poubelle !!! ..............................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;
 statusBar()->message(tr("Chercher les dossiers d'un patient : Traitement des donn\303\251es termin\303\251 "), 100000 );
}

//------------------------------------------ SearchDossierSigems ----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::SearchDossierSigems( const QString & ippSigemsPatient )
{QString urn("");
 //................................. instancier l'interface SOAP ...............................................
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI    = new CSoapInterface;
 if (m_pcSoapI==0) return;
 QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 statusBar()->message(tr("Chercher les dossiers d'un patient : Transfert des donn\303\251es en cours.... "), 100000 );
 //.................................. la mettre en mode simulation selon checkBoxTestMode ......................
 m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigemsRet-SearchDossier00.txt", checkBoxTestMode->isChecked()); // ##SelectPatient

 //................................. connecter l'interface SOAP ................................................
 connect(  m_pcSoapI,   SIGNAL( done( bool ) ),
           this,        SLOT( Slot_SOAP_SearchDossier_done( bool ) ) );      // ## SelectPatient
 connect(  m_pcSoapI,   SIGNAL( responseHeaderReceived(    const QHttpResponseHeader& ) ),
           this,        SLOT( Slot_responseHeaderReceived( const QHttpResponseHeader& ) ) );

 //................................. appel urn different selon version Sigems ..................................
 QString url("http://");
 if (m_SigemsVers=="01")  urn = "SigeGateDos";
 else                     urn = "SigeGateMed";

 url    +=  m_SigemsLogin + ":" + m_SigemsPassword + "@" + m_UrlSigemsWebServices + urn;
 //.............................. message de base ..............................................................
 QString message("");
         message = "<idBase xsi:type=\"xsd:string\">"+comboBox_Bases->currentText()+"</idBase>"
                   "<page xsi:type=\"xsd:int\">1</page>"
                   "<nbLigne xsi:type=\"xsd:int\">50</nbLigne>"
                   "<dossier href=\"#id0\"/>"
                   "<dateEntreeInf xsi:type=\"xsd:string\">"      + dateEdit_DateEntree->date().toString ("dd/MM/yyyy") +"</dateEntreeInf>"
                   "<dateEntreeSup xsi:type=\"xsd:string\">"      + dateEdit_DateSortie->date().toString ("dd/MM/yyyy") +"</dateEntreeSup>"
                   "<dateInterventionInf xsi:type=\"xsd:string\">"+ dateEdit_DateEntree->date().toString ("dd/MM/yyyy") +"</dateInterventionInf>"
                   "<dateInterventionSup xsi:type=\"xsd:string\">"+ dateEdit_DateSortie->date().toString ("dd/MM/yyyy") +"</dateInterventionSup>\n";

 QString multiref ="<multiref id=\"id0\" soapenc:root=\"0\""
                             " SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\""
                             " xsi:type=\"ns1:WsDossier\""
                             " xmlns:soapenc=\"http://schemas.xmlsoap.org/soap/encoding\""
                   " xmlns:ns1=\"urn:";
        multiref +=  urn + "\">"
                   "<ippSigemsPatient xsi:type=\"xsd:int\">"+ippSigemsPatient+"</ippSigemsPatient>"
                   "<dateEntree xsi:type=\"xsd:string\"></dateEntree>"
                   "<idChirurgien xsi:type=\"xsd:string\"></idChirurgien>"
                   "<idAnesthesiste xsi:type=\"xsd:string\"></idAnesthesiste>"
                   "<idMedecinTraitant xsi:type=\"xsd:string\"></idMedecinTraitant>"
                   "</multiref>\n";

 //.............................. Envoyer une requ\303\250te avec ce message ..........................................
 //                              le retour est: l'entete http + message soap envoy\303\251
 QString result = m_pcSoapI->SendMessage(url,  "searchDossier",  message , multiref);             // ## SelectPatient

 //............................. afficher de fa\303\247on sympa la requete XML ........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Request(result);
}

//------------------------------------------ Slot_SOAP_SelectPatient_done ------------------------------------------------
// *******************************************************
// *** CONNECTEUR Slot_SOAP_GetDatabasesList_done      ***
// *******************************************************
// * Appel\303\251 http a fini d'envoyer les donn\303\251es            *
// *******************************************************
// pour envoyer requete de rcherche patient Sigems
void C_Dlg_FeuillePrincipale::Slot_SOAP_SelectPatient_done( bool state )
{ int updatePatList = m_UpdatePatList;
  m_UpdatePatList   = 0;
 //............................. Recuperer la reponse .........................................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 statusBar()->message(tr("S\303\251lectionner un patient : Traitement des donn\303\251es en cours.... "), 100000 );

 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Remplir la structure: WsPatient ..............................................
 //                              avec le contenu des champs retourn\303\251es (apres parsing XML)
 WsPatient                            patient;
 m_pcSoapI->XML_To_SigemsResult(xml, &patient );

 //................... envoyer la reponse dans la listview des patients .......................................
 if (patient.m_RetCode=="0")
    {lineedit_Code1->setText(patient.m_ippExtern );
     lineedit_Code->setText(patient.m_ippSigems );
     lineedit_Nom->setText(patient.m_nomUsuel );
     lineedit_Nom_Naiss->setText(patient.m_nomNaissance );
     if (patient.m_nomUsuel=="") lineedit_Nom->setText(patient.m_nomNaissance );
     lineedit_Prenom->setText(patient.m_prenom );
     lineedit_NumSS->setText(patient.m_numSecu );
     lineEdit_CleSS->setText(patient.m_cleSecu );
     lineedit_Adr_1->setText(patient.m_adresse1 );
     lineedit_Adr_2->setText(patient.m_adresse2 );
     lineEdit_Sexe->setText(patient.m_sexe );
     lineedit_Tel->setText(patient.m_telephone );
     lineedit_CodePostal->setText(patient.m_codePostal );
     lineedit_Ville->setText(patient.m_ville );
     lineedit_Ville_Nss->setText(patient.m_villeNaissance);
     //............... traitement de la date ................................
     if (patient.m_dateNaissance.length()==10)
        {QDate  date ( atoi(patient.m_dateNaissance.mid(6,4)),     // 10/12/2003  --> YYYY  2003
                       atoi(patient.m_dateNaissance.mid(3,2)),     // 10/12/2003  --> MM    12
                       atoi(patient.m_dateNaissance.mid(0,2))      // 10/12/2003  --> DD    10
                     );
         if (date.isValid () ) dateEdit_DateNss->setDate (date );
         else                  dateEdit_DateNss->setDate (QDate ());
        }
     else
        {dateEdit_DateNss->setDate (QDate (  ) );
        }
    }
 else m_pcSoapI->DisplaySigemsError(patient.m_RetCode);
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);
 //...................... on en a plus besoin --> poubelle !!! ................................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;
 //....................... mettre \303\264 jour la liste des patient si besoin .......................................
 //                        cette particularite est lorsque il y a
 //                        selection du patient par son num\303\251ro:  pushButtonNumDossier_clicked()
 if (updatePatList && lineedit_Code->text() !="")      // si inscription toujours valide c'est que le patient existe et a ete trouve
    {listview_ListePatient->clear();
     new Q3ListViewItem(listview_ListePatient,
                       lineedit_Code->text(),
                       lineedit_Nom->text(),
                       lineedit_Prenom->text(),
                       lineedit_Nom_Naiss->text(),
                       dateEdit_DateNss->date().toString ("dd/MM/yyyy"));
    }
 //...................... lister les dossiers d'hospi de ce patient ...........................................
 if (patient.m_RetCode=="0" && updatePatList<2) SearchDossierSigems(patient.m_ippSigems);
 statusBar()->message(tr("S\303\251lectionner un patient : Traitement des donn\303\251es termin\303\251 "), 100000 );
}

//------------------------------------------ SelectPatientSigems ----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::SelectPatientSigems( const QString & ipp )
{//................................. instancier l'interface SOAP ...............................................
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI    = new CSoapInterface;
 if (m_pcSoapI==0) return;
 QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 statusBar()->message(tr("S\303\251lectionner un patient : Transfert des donn\303\251es en cours.... "), 100000 );
 //.................................. la mettre en mode simulation selon checkBoxTestMode ......................
 m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigemsRet-SelectPatient.txt", checkBoxTestMode->isChecked()); // ##SelectPatient

 //................................. connecter l'interface SOAP ................................................

 connect(  m_pcSoapI,   SIGNAL( done( bool ) ),
           this,        SLOT( Slot_SOAP_SelectPatient_done( bool ) ) );      // ## SelectPatient
 connect(  m_pcSoapI,   SIGNAL( responseHeaderReceived(    const QHttpResponseHeader& ) ),
           this,        SLOT( Slot_responseHeaderReceived( const QHttpResponseHeader& ) ) );

 //.............................. message de base ..............................................................
 QString message("");
         message += " <idBase xsi:type=\"xsd:string\">"+ comboBox_Bases->currentText() +"</idBase>\n"
                    " <ipp xsi:type=\"xsd:int\">"      + ipp                           +"</ipp>\n";

 //.............................. Envoyer une requ\303\250te avec ce message ..........................................
 //                              le retour est: l'entete http + message soap envoy\303\251
 QString url    = QString("http://") + m_SigemsLogin + ":" + m_SigemsPassword + "@" + m_UrlSigemsWebServices + "SigeGatePat";
 QString result = m_pcSoapI->SendMessage(url,  "selectPatient",  message );             // ## SelectPatient

 //............................. afficher de fa\303\247on sympa la requete XML ........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Request(result);
}

//------------------------------------------ Slot_SOAP_SelectPatient_done ------------------------------------------------
// *******************************************************
// *** CONNECTEUR Slot_SOAP_GetDatabasesList_done      ***
// *******************************************************
// * Appel\303\251 http a fini d'envoyer les donn\303\251es            *
// *******************************************************
// pour envoyer requete de rcherche patient Sigems
void C_Dlg_FeuillePrincipale::Slot_SOAP_GetDatabasesList_done( bool state )
{
 //............................. Recuperer la reponse .........................................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Extraire les donn\303\251es du tableau de patient ....................................
 DatabaseIdentificationListe          baseListe;
 m_pcSoapI->XML_To_SigemsResult(xml, &baseListe );

 //................... envoyer la reponse dans la listview des patients .......................................
 if (baseListe.m_RetCode=="0")
    { comboBox_Bases->clear();
      for (int i=0; i<baseListe.m_Nb; ++i)
          {comboBox_Bases->insertItem(baseListe.m_pDatabaseIdentification[i].m_identifiant);
          }
     }
 else m_pcSoapI->DisplaySigemsError(baseListe.m_RetCode);
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);

 //...................... on en a plus besoin --> poubelle !!! ..............................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;
}


//------------------------------------------ GetBaseSigems ----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::GetBaseSigems()
{//................................. instancier l'interface SOAP ...............................................
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI    = new CSoapInterface;
 if (m_pcSoapI==0) return;
 QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 //.................................. la mettre en mode simulation selon checkBoxTestMode ......................
 m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigemsRet-GetDatabases.txt", checkBoxTestMode->isChecked());

 //................................. connecter l'interface SOAP ................................................

 connect( m_pcSoapI,   SIGNAL( done( bool ) ),
          this,        SLOT( Slot_SOAP_GetDatabasesList_done( bool ) ) );
 connect( m_pcSoapI,   SIGNAL( responseHeaderReceived( const QHttpResponseHeader& ) ),
          this,        SLOT( Slot_responseHeaderReceived(      const QHttpResponseHeader& ) ) );

 //.............................. Envoyer une requ\303\250te avec ce message ..........................................
 //                              le retour est: l'entete http + message soap envoy\303\251
 QString url    = QString("http://") + m_SigemsLogin + ":" + m_SigemsPassword + "@" + m_UrlSigemsWebServices + "SigeGateConf";
 QString result = m_pcSoapI->SendMessage(url,  "getDatabases",  " " );

 //............................. afficher de fa\303\247on sympa la requete XML ........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Request(result);
}

//------------------------------------------ Slot_SOAP_GetPatientList_done ------------------------------------------------
// *******************************************************
// *** CONNECTEUR Slot_SOAPdone                        ***
// *******************************************************
// * Appel\303\251 http a fini d'envoyer les donn\303\251es            *
// *******************************************************
// pour envoyer requete de rcherche patient Sigems
void C_Dlg_FeuillePrincipale::Slot_SOAP_GetPatientList_done( bool state )
{
 //............................. Recuperer la reponse du serveur ...............................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 statusBar()->message(tr("Rechercher un patient : Traitement des donn\303\251es en cours.... "), 100000 );

 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Extraire les donn\303\251es du tableau de patient ....................................
 WsPatientIdentificationListe         wsPatListe;
 m_pcSoapI->XML_To_SigemsResult(xml, &wsPatListe );

 //................... envoyer la reponse dans la listview des patients .......................................
 if (wsPatListe.m_RetCode=="0")
    { listview_ListePatient->clear();
      for (int i=0; i<wsPatListe.m_Nb; ++i)
          {new Q3ListViewItem(listview_ListePatient,
                             wsPatListe.m_pWsPatientIdentification[i].m_ippSigems,
                             wsPatListe.m_pWsPatientIdentification[i].m_nomUsuel,
                             wsPatListe.m_pWsPatientIdentification[i].m_prenom,
                             wsPatListe.m_pWsPatientIdentification[i].m_nomNaissance,
                             wsPatListe.m_pWsPatientIdentification[i].m_dateNaissance);
          }
     }
 else m_pcSoapI->DisplaySigemsError(wsPatListe.m_RetCode);
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);

 //...................... on en a plus besoin --> poubelle !!! ..............................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;           // pourquoi cela plante ??? car pas deconnect\303\251 avant !!!
 statusBar()->message(tr("Rechercher un patient : Traitement des donn\303\251es termin\303\251 "), 100000 );
}

//------------------------------------------ ChercherPatient -----------------------------------------------------------------
// ****************************************************
// *** CONNECTEUR pushButtonChercherPatient_clicked ***
// *******************************************************
// * Appel\303\251 quand on clic sur le bouton ChercherPatient  *
// *******************************************************
// pour envoyer requete de rcherche patient Sigems
void C_Dlg_FeuillePrincipale::ChercherPatient()
{//................................. instancier l'interface SOAP ...............................................
 if (m_pcSoapI) delete m_pcSoapI;
 m_pcSoapI    = new CSoapInterface;
 if (m_pcSoapI==0) return;
 QApplication::restoreOverrideCursor();
 QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
 statusBar()->message(tr("Rechercher un patient : Transfert des donn\303\251es en cours..... "), 100000 );
 //.................................. la mettre en mode simulation selon checkBoxTestMode ......................
 m_pcSoapI->SetDebugMode(GlobalPathAppli + "SimFiles/SigemsRet-SearchPatient00.txt", checkBoxTestMode->isChecked());

 //................................. connecter l'interface SOAP ................................................

 connect(  m_pcSoapI, SIGNAL( done( bool ) ),
           this,      SLOT( Slot_SOAP_GetPatientList_done( bool ) ) );
 connect(  m_pcSoapI, SIGNAL( responseHeaderReceived(     const QHttpResponseHeader& ) ),
           this,      SLOT( Slot_responseHeaderReceived(  const QHttpResponseHeader& ) ) );

 //.............................. message de base ..............................................................
 QString message("");
         message += "<idBase xsi:type=\"xsd:string\">"        + comboBox_Bases->currentText()  + "</idBase>\n"
                    "<nomNaissance xsi:type=\"xsd:string\">"  + lineedit_Nom_Naiss->text()     + "</nomNaissance>\n"
                    "<nomUsuel xsi:type=\"xsd:string\">"      + lineedit_Nom->text()           + "</nomUsuel>\n"
                    "<prenom xsi:type=\"xsd:string\">"        + lineedit_Prenom->text()        + "</prenom>\n"
                    "<dateNaissance xsi:type=\"xsd:string\">" + (checkBox_DateNss->isChecked() ? (const char*) dateEdit_DateNss->date().toString ("dd/MM/yyyy") : (const char*)"") +"</dateNaissance>\n";

 //.............................. Envoyer une requ\303\252te avec ce message ..........................................
 //                              le retour est: l'entete http + message soap envoy\303\251
 QString url    = QString("http://") + m_SigemsLogin + ":" + m_SigemsPassword + "@" + m_UrlSigemsWebServices + "SigeGatePat";
 QString result = m_pcSoapI->SendMessage(url, "searchPatient", message );

 //............................. afficher de fa\303\247on sympa la requete XML ........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Request(result);
}

//------------------------------------------ Slot_SOAP_ListerSigemsActesPatient_done -----------------------------------------------------------------
void C_Dlg_FeuillePrincipale::Slot_SOAP_ListerSigemsActesPatient_done( bool state )
{//............................. Recuperer la reponse .........................................................
 QString xml    = m_pcSoapI->GetResponseBody(state);
 QApplication::restoreOverrideCursor();
 statusBar()->message(tr("Chercher les actes d'un patient: Traitement des donn\303\251es en cours.... "), 100000 );

 if (xml=="Error") {textEditSoapRespMessage->setText(""); return;}

 //............................. Remplir la structure: WsPatient ..............................................
 //                              avec le contenu des champs retourn\303\251es (apres parsing XML)
 WsActeCcamListe                      wsActeCcamListe("searchActeReturn");
 m_pcSoapI->XML_To_SigemsResult(xml, &wsActeCcamListe );

 //................... envoyer la reponse dans la listview des patients .......................................
 if (wsActeCcamListe.m_RetCode=="0")
    {
      for (int i=0; i<wsActeCcamListe.m_Nb; ++i)
          {INFO_MED info_med;
           m_pCMoteurCCAM_Base->GetInfoMedecinsFromNid(wsActeCcamListe.m_pWsActeCcam[i].m_idPraticien, info_med);

           Q3ListViewItem *element = new Q3ListViewItem(listview_ListeActePat);
           if (element)
              {element->setText ( LVA_DATE,          wsActeCcamListe.m_pWsActeCcam[i].m_date );
               element->setText ( LVA_HEURE,         wsActeCcamListe.m_pWsActeCcam[i].m_heureDebut);
               element->setText ( LVA_PRATICIENNAME, info_med.m_Nom    + " " + info_med.m_Prenom );
               element->setText ( LVA_CODECCAM,      wsActeCcamListe.m_pWsActeCcam[i].m_codeActe);
               element->setText ( LVA_LIBELLEACTE,   m_pCMoteurCCAM_Base->CodeCCAMToLibelleLong(wsActeCcamListe.m_pWsActeCcam[i].m_codeActe));
               element->setText ( LVA_TARIF,         m_pCMoteurCCAM_Base->GetPrixActe(wsActeCcamListe.m_pWsActeCcam[i].m_codeActe,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_date,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_activite,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_phase,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_modificateur1,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_modificateur2,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_modificateur3,
                                                                                      wsActeCcamListe.m_pWsActeCcam[i].m_modificateur4));
               element->setText ( LVA_PHASE,         wsActeCcamListe.m_pWsActeCcam[i].m_phase);
               element->setText ( LVA_ACTIVITE,      wsActeCcamListe.m_pWsActeCcam[i].m_activite);
               element->setText ( LVA_MODIFICATEUR1, wsActeCcamListe.m_pWsActeCcam[i].m_modificateur1);
               element->setText ( LVA_MODIFICATEUR2, wsActeCcamListe.m_pWsActeCcam[i].m_modificateur2);
               element->setText ( LVA_MODIFICATEUR3, wsActeCcamListe.m_pWsActeCcam[i].m_modificateur3);
               element->setText ( LVA_MODIFICATEUR4, wsActeCcamListe.m_pWsActeCcam[i].m_modificateur4);
               element->setText ( LVA_NB,            wsActeCcamListe.m_pWsActeCcam[i].m_nbOccurence);
               element->setText ( LVA_NUMDOSS,       wsActeCcamListe.m_pWsActeCcam[i].m_numDossier);
               element->setText ( LVA_NOMENCLAT,     "CCAM");
               element->setText ( LVA_NIDPRATICIEN,  wsActeCcamListe.m_pWsActeCcam[i].m_idPraticien);
               element->setText ( LVA_TYP,           "V");              // valid\303\251
               element->setPixmap ( 0, QPixmap (ValidActeXPM_xpm) );
               element->setText ( LVA_ENFANT,        wsActeCcamListe.m_pWsActeCcam[i].m_enfant);
               element->setText ( LVA_PRESCRIPTEUR,  wsActeCcamListe.m_pWsActeCcam[i].m_prescripteur);
               element->setText ( LVA_CODE_REJET,    wsActeCcamListe.m_pWsActeCcam[i].m_codeRejet);
               listview_ListeActePat->insertItem ( element );
              } //endif (element)
         } // end for (int i=0; i<wsActeCcamListe.m_Nb; ++i)
    }
 else if (wsActeCcamListe.m_RetCode=="-1")  // pas d'actes pour ce dossier
    {//listview_ListeDossPat->clear();
    }
 else Display_createActeTemp_SigemsError(wsActeCcamListe.m_RetCode);
 //............................. afficher le resultat de la requete ...........................................
 if (m_ModeFlag & XML_MONITORING) DisplayColoredXML_Response(xml, m_pcSoapI);
 //...................... on en a plus besoin --> poubelle !!! ..............................................
 if (m_pcSoapI) delete m_pcSoapI;   m_pcSoapI = 0;
 statusBar()->message(tr("Chercher les actes d'un patient : Traitement des donn\303\251es termin\303\251 "), 100000 );
 //......................... enchainer sur les temporaires .............................................
 QString numDoss  = GetNumDossierFromEditables();
 if (numDoss.length()<=0)  return;
 ListerSigemsActesPatient(numDoss, "", "searchActeTemp");  // IdPrat par pour l'instant
}
