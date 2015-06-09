//
// C++ Implementation: 
//
// Description: 
//
//
// Author: Sevin Roland <ro@upsis>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "C_Dlg_ChangeMAJCCAM.h"
extern QString       Global_PathIni;        // Pointeur sur le chemin du fichier .ini
extern QString       Global_ParamData;      // Pointeur sur les parametre charges a partir du fichier .ini

//---------------------------- C_Dlg_ChangeMAJCCAM -------------------------------------------------
C_Dlg_ChangeMAJCCAM::C_Dlg_ChangeMAJCCAM(CMoteurCCAM_Base* pCMoteurCCAM_Base,const QString &fichier,QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
: Dlg_ChargeMAJCCAM(parent,name, modal,fl)
{   m_LastError          = "";
    connect( pushButton_Annuler,  SIGNAL( clicked() ), this, SLOT( pushButton_Annuler_clicked() ) );
    connect( pushButton_Integrer, SIGNAL( clicked() ), this, SLOT( pushButton_Integrer_clicked() ) );
    if (initDialog(pCMoteurCCAM_Base,fichier)==0) m_LastError = tr("type de fichier iconnu : ") + fichier;
}

//---------------------------- ~C_Dlg_ChangeMAJCCAM -------------------------------------------------
C_Dlg_ChangeMAJCCAM::~C_Dlg_ChangeMAJCCAM()
{if (m_pQFile) {
        delete m_pQFile;
    }
}

/*$SPECIALIZATION$*/
//---------------------------- initDialog -------------------------------------------------
int C_Dlg_ChangeMAJCCAM::initDialog(CMoteurCCAM_Base* pCMoteurCCAM_Base,const QString &fichier)
{
    QString bloc_lu("");
    m_pCMoteurCCAM_Base = pCMoteurCCAM_Base;

    m_TypeFichier = "";
    m_Maj_OK          = FALSE;
    m_StopIntegration = FALSE;
    // Definir le fichier à lire pour avoir les paramètres de chaque entités
    // m_fichier_entite = qstringLoad (GlobalPathAppli + "Structure_entites.ini");
    CGestIni::Param_UpdateFromDisk(GlobalPathAppli + "Structure_entites.ini", m_fichier_entite);

    // Barre de progression
    //BarreProgression->setIndicatorFollowsStyle(TRUE);
    BarreProgression->show();
    EtiqBarreProg->show();

    // Definition du pointeur sur le fichier CCAM à lire
    m_pQFile = new QFile(fichier);

    // Cree la table des versions intégrées de la base CCAM
    m_pCMoteurCCAM_Base->CheckTableVersion();

    // Test de la cohérence du fichier à intégrer
    if (VerificationCoherenceFichier(m_pQFile)) {
        pushButton_Integrer->setEnabled(TRUE);
    } else {
        if (m_TypeFichier != tr("Inconnu")) {
            pushButton_Integrer->setEnabled(FALSE);
        } else {
            return 0;
        }
    }
    return 1;
}

//---------------------------- RechercheNombreEntitesTotales -------------------------------------------------
long C_Dlg_ChangeMAJCCAM::RechercheNombreEntitesTotales()
{
    long entites = 0;
    int i;
    for (i=0; i<m_Index_max; i++){
        entites += m_Nb_Entite[i].m_Nombre;
    }
    return entites;
}

//---------------------------- RechercheNombreEntite -------------------------------------------------
long C_Dlg_ChangeMAJCCAM::RechercheNombreEntite( QString numero, QString rubrique, QString etat )
{long nb = 0;
    int i;
    for (i=0; i<100; i++){
        if ((m_Nb_Entite[i].m_Numero == numero) && (m_Nb_Entite[i].m_Rubrique == rubrique)) {
            nb = m_Nb_Entite[i].m_Nombre;
            if (etat == "RAZ") m_Nb_Entite[i].m_Nombre = 0;
            return nb;
        }
    }
    return nb;
}

//---------------------------- RechercheIndex -------------------------------------------------
int C_Dlg_ChangeMAJCCAM::RechercheIndex( QString & numero, QString & rubrique )
{int i;
    for (i=0; i<100; i++){
        if ((m_Nb_Entite[i].m_Numero == numero) && (m_Nb_Entite[i].m_Rubrique == rubrique)) {
            return i;
        }
    }
    return -1;
}

//---------------------------- ParseEntite -------------------------------------------------
char * C_Dlg_ChangeMAJCCAM::ParseEntite( Q3ListView * pQListView, QString & ligne_a_decouper, QString & id_entite )
{QString section("");
 QString section_voulue ("");
 QString var_name ("");
 QString nom_donnee, type_donnee, debut_donnee, longueur_donnee, contenu_fichier;
 char *pt = (char*)((const char*)m_fichier_entite);

 while( (pt = CGestIni::Param_GotoNextSection(pt, 0, &section)) && *pt)
  {if (section==id_entite)
      {new Q3ListViewItem( pQListView, "------","--------", section, "---------", "---------","-----------");
       while (*pt && *pt != '[')
             {pt = CGestIni::Param_ExtraireNextValeurs(pt, var_name, &nom_donnee, &type_donnee, &debut_donnee, &longueur_donnee);
              contenu_fichier = ligne_a_decouper.mid(atoi(debut_donnee), atoi(longueur_donnee));
              new Q3ListViewItem( pQListView, var_name, nom_donnee, type_donnee, debut_donnee, longueur_donnee,contenu_fichier);
             }
       section = "";
      }
  }
 return pt;
}

//---------------------------- ParseEntite -------------------------------------------------
QVariant C_Dlg_ChangeMAJCCAM::ValeurDansEntite( QString & ligne_a_decouper, QString & section, QString var_name )
{   QVariant valeur_entite ="";
    QString nom_donnee, type_donnee, debut_donnee, longueur_donnee;

    char *pt = (char*)((const char*)m_fichier_entite);
    if (CGestIni::Param_ReadParam(pt, section, var_name, &nom_donnee, &type_donnee, &debut_donnee, &longueur_donnee) != QString::null) {
        return (tr("Erreur Lecture dans le fichier ini : section = ") + section + tr(", variable = ") + var_name);
    }
    if (type_donnee == "AlphaNum") {
        valeur_entite = ligne_a_decouper.mid(atoi(debut_donnee), atoi(longueur_donnee));
    }
    if (type_donnee == "Num") {
        valeur_entite = atoi(ligne_a_decouper.mid(atoi(debut_donnee), atoi(longueur_donnee)));
    }
    if (type_donnee == "Dec2") {
        valeur_entite = atof(ligne_a_decouper.mid(atoi(debut_donnee), atoi(longueur_donnee)))/100;
    }
    if (type_donnee == "Dec3") {
        valeur_entite = atof(ligne_a_decouper.mid(atoi(debut_donnee), atoi(longueur_donnee)))/1000;
    }
    if (type_donnee == "Date") {
        valeur_entite = ligne_a_decouper.mid(atoi(debut_donnee), atoi(longueur_donnee));
    }
    return valeur_entite;
}

//---------------------------- LectureFichier -------------------------------------------------
bool C_Dlg_ChangeMAJCCAM::LectureFichier( QFile * pQFile )
{   if (pQFile==0)                          return FALSE; // si pas de fichier cassos
    if (pQFile->open( IO_ReadOnly )==FALSE) return FALSE; // si impossible ouvrir fichier cassos
    char *buffer_lecture = new char[130];
    buffer_lecture[0]    = 0;
    m_Position           = 0;
    long longueur_lu     = 0;
    QString ligne_lu     = "";

    // reinitialisation de la barre de progression ........................
    long enr_tot = m_pQFile->size()/ (128 + m_lenEOL);
    BarreProgression->setProgress(m_Position);

    // initialisation du tableau qui stocke le nombre de chaque entite
    for (int i=0; i<100; i++){
        m_Nb_Entite[i].m_Nombre = 0;
    }
    // initialisation de l'entité précedemment rencontrée : utile pour 101 52 et 091 01
    m_EntitePrecedente.m_Entite =0;
    m_EntitePrecedente.m_Rubrique =0;
    m_EntitePrecedente.m_Sequence =0;
    // initialisation de l'acte en cours
    m_ActeEnCours.m_CodeActe = "Non";
    // initialisation de l'historique d'un acte
    m_HistoActe.m_DateEffet ="";
    // initialisation de l'activité en cours
    m_Activite.m_CodeActe = "";
    m_Activite.m_CodeActivite = "";
    // initialisation des tableaux
    RAZ_Compatibilite(m_Compatibilite);
    RAZ_Tableau(m_ActeIncompatible,8);
    RAZ_Tableau(m_CodeProcedure,8);
    m_NoteActe.m_TypeNote = "";
    m_NoteActe.m_TexteNote = "";
    m_RecMedActe.m_NumRecMed = "";
    m_RecMedActe.m_TexteRecMed = "";
    m_ListeDate.clear();


    while ( ((longueur_lu = pQFile->readBlock(buffer_lecture, 128)) > 0) && (!m_StopIntegration)){
        buffer_lecture[longueur_lu] = 0;     // ajoute le \0 à la fin de la chaine
        converti(buffer_lecture);            // change le type d'apostrophe
        ligne_lu          = buffer_lecture;  // change la chaine char en QString
        DecoupeEntite(ligne_lu);	     // Decoupage des entités, le plus gros du travail
        pQFile->readBlock(buffer_lecture, m_lenEOL);// lit les deux suivants pour sauter le \r\n
        buffer_lecture[0] = 0;
        ligne_lu          = "";
        m_Position++;
        BarreProgression->setProgress(m_Position);
        qApp->processEvents();              // force le traitement des événements pour des questions d'esthétique
    }                                       // et pour voir la proggression
    pQFile->close();  // ferme le fichier
    delete []buffer_lecture; // detruit le buffer
    return  TRUE;
}

//---------------------------- VerificationCoherenceFichier -------------------------------------------------
bool C_Dlg_ChangeMAJCCAM::VerificationCoherenceFichier( QFile * pQFile )
{   QString id_entite       = "";
    QString rubrique_entite = "";
    int coherence           = 0;
    m_Nb_EntitesTotales     = 0;
    m_Nb_Entite002020       = 0;
    m_Nb_Entite10101        = 0;
    m_Nb_Entite20101        = 0;
    m_Nb_Entite30101        = 0;
    m_Duree_Traitement.start();

    // Definition du curseur de la souris...................................
    QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

    // Lecture des 130 premiers octets pour connaitre le type de fichier
    // m_lenEOL = 2 si octet 129  = 0a et octet 130  = 0d
    // m_lenEOL = 1 si octet 129  = 0a et octet 130 != 0d
    // m_lenEOL = 0 si octet 129 != 0a et octet 130 != 0d

    if (pQFile==0) return FALSE;                          // si pas de fichier cassos
    if (pQFile->open( IO_ReadOnly )==FALSE) return FALSE; // si impossible ouvrir fichier cassos
    char *buffer_test  = new char[130];
    buffer_test[0]     = 0;
    long longueur_test = pQFile->readBlock(buffer_test, 130);
    pQFile->close();

    if ( (buffer_test[128] == '\r') && (buffer_test[129] == '\n') ) {
        m_lenEOL = 2;
    } else if ( buffer_test[128] == '\n' ) {
        m_lenEOL = 1;
    } else {
        m_lenEOL = 0;
        // Restauration du curseur de la souris.................................
        QApplication::restoreOverrideCursor();
        return FALSE;
    }

    //QMessageBox::about(this,"m_lenEOL","m_lenEOL = " + QString::number(m_lenEOL));

    // initialisation du tableau qui stocke le nombre de chaque entite
    for (int i=0; i<100; i++){
        m_Nb_Entite[i].m_Numero   = "";
        m_Nb_Entite[i].m_Rubrique = "";
        m_Nb_Entite[i].m_Nombre   = 0;
    }
    m_Index_max = 0;
    // initialisation de la barre de progression .......................
    long enr_tot = m_pQFile->size() / (128 + m_lenEOL);
    BarreProgression->setTotalSteps(enr_tot);
    // Parcours du fichier.............................................
    if (pQFile==0) return FALSE;                          // si pas de fichier cassos
    if (pQFile->open( IO_ReadOnly )==FALSE) return FALSE; // si impossible ouvrir fichier cassos
    char *buffer_lecture = new char[130];
    buffer_lecture[0]    = 0;
    m_Position           = 0;
    long longueur_lu     = 0;
    QString ligne_lu     = "";
    while ( (longueur_lu = pQFile->readBlock(buffer_lecture, 128)) > 0){
        buffer_lecture[longueur_lu] = 0;
        ligne_lu          = buffer_lecture;
        id_entite       = ligne_lu.mid(0,3);
        rubrique_entite = ligne_lu.mid(3,2);
        if ((m_Position == 0 && id_entite !="000") || (enr_tot < 1)) { // Verification de la validité du fichier
                m_TypeFichier = "Inconnu";                             // Car en cas de taille de fichier < 128 octets
                break;                                                 // ou de fichier exe le programme plante
        }
        // Recherche les informations utiles dans les entités
        InformationsEntites(ligne_lu);
        // Continue la Lecture du fichier....................
        pQFile->readBlock(buffer_lecture, m_lenEOL);
        buffer_lecture[0] = 0;
        ligne_lu          = "";
        m_Position++;
        BarreProgression->setProgress(m_Position);
    }
    pQFile->close();
    delete []buffer_lecture;
    new Q3ListViewItem(listview_Affichage,tr("_Durée de la verification"),QString::number( m_Duree_Traitement.elapsed()) + tr(" millisecondes"));
    // Compare les informations .............................................
    long ent_tot = RechercheNombreEntitesTotales();
    long enr101 = RechercheNombreEntite("101","01","");
    long enr201 = RechercheNombreEntite("201","01","");
    long enr301 = RechercheNombreEntite("301","01","");
    // ................... type de fichier ................................
    if (m_TypeFichier != tr("Inconnu")) {
        new Q3ListViewItem(listview_Affichage,m_TypeFichier);
        new Q3ListViewItem(listview_Affichage,tr("longueur fin ligne"),QString::number(m_lenEOL));
        // Nb entites totales d'apres taille fichier VS Nb entites déclarées en 999
        if ((enr_tot) == m_Nb_EntitesTotales) coherence++;
        new Q3ListViewItem(listview_Affichage,tr("Nb entites totales d'apres taille fichier"),QString::number(enr_tot),tr("Nb entites déclarées en 999"),QString::number(m_Nb_EntitesTotales));
        // Comptage total entites lues VS Nb entites déclarées en 999
        if ((ent_tot) == m_Nb_EntitesTotales) coherence++;
        new Q3ListViewItem(listview_Affichage,tr("Nb entites totales lues"),QString::number(ent_tot),tr("Nb entites déclarées en 999"),QString::number(m_Nb_EntitesTotales));
        // Nb entites 101 - 01 d'apres fichier VS Nb entites déclarées en 999
        if (enr101 == m_Nb_Entite10101) coherence++;
        new Q3ListViewItem(listview_Affichage,tr("Nb entites 101 - 01 d'apres fichier"),QString::number(enr101),tr("Nb entites déclarées en 999"),QString::number(m_Nb_Entite10101));
        // Nb entites 201 - 01 d'apres fichier VS Nb entites déclarées en 999
        if (enr201 == m_Nb_Entite20101) coherence++;
        new Q3ListViewItem(listview_Affichage,tr("Nb entites 201 - 01 d'apres fichier"),QString::number(enr201),tr("Nb entites déclarées en 999"),QString::number(m_Nb_Entite20101));
        // Nb entites 301 - 01 d'apres fichier VS Nb entites déclarées en 999
        if (enr301 == m_Nb_Entite30101) coherence++;
        new Q3ListViewItem(listview_Affichage,tr("Nb entites 301 - 01 d'apres fichier"),QString::number(enr301),tr("Nb entites déclarées en 999"),QString::number(m_Nb_Entite30101));
        new Q3ListViewItem(listview_Affichage,tr("Cohérence"),QString::number(coherence));
        // Verifie si le fichier est déja integré...............................
        m_DejaIntegre = m_pCMoteurCCAM_Base->CheckIntegration(m_VersionBase);
    }
    // Restauration du curseur de la souris.................................
    QApplication::restoreOverrideCursor();
    if (coherence == 5) return TRUE;
    return FALSE;
}

//---------------------------- StockageDate -------------------------------------------------
void C_Dlg_ChangeMAJCCAM::StockageDate( const QString & date )
{ QStringList::Iterator it = m_ListeDate.find(date);
    if (it == m_ListeDate.end()) {
        m_ListeDate.append(date);
    }
}

//---------------------------- EffaceBase -------------------------------------------------
void C_Dlg_ChangeMAJCCAM::EffaceBase()
{// mise à jour pour tarification
    for (int i=0; i<100; i++){
        if (m_Nb_Entite[i].m_Nombre > 0) {
            if (m_Nb_Entite[i].m_Numero == "002" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("ASSOCIATION");
            }
            if (m_Nb_Entite[i].m_Numero == "003" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB03");
            }
            if (m_Nb_Entite[i].m_Numero == "004" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB04");
            }
            if (m_Nb_Entite[i].m_Numero == "005" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB05");
            }
            if (m_Nb_Entite[i].m_Numero == "006" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB06");
            }
            if (m_Nb_Entite[i].m_Numero == "007" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB07");
            }
            if (m_Nb_Entite[i].m_Numero == "008" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB08");
            }
            if (m_Nb_Entite[i].m_Numero == "009" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB09");
            }
            if (m_Nb_Entite[i].m_Numero == "010" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB10");
            }
            if (m_Nb_Entite[i].m_Numero == "011" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB11");
            }
            if (m_Nb_Entite[i].m_Numero == "012" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB12");
            }
            if (m_Nb_Entite[i].m_Numero == "013" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB13");
            }
            if (m_Nb_Entite[i].m_Numero == "014" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB14");
            }
            if (m_Nb_Entite[i].m_Numero == "015" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB15");
            }
            if (m_Nb_Entite[i].m_Numero == "017" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB17");
            }
            if (m_Nb_Entite[i].m_Numero == "018" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB18");
            }
            if (m_Nb_Entite[i].m_Numero == "019" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB19");
            }
            if (m_Nb_Entite[i].m_Numero == "020" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TB20");
            }
            if (m_Nb_Entite[i].m_Numero == "050" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TYPENOTE");
            }
            if (m_Nb_Entite[i].m_Numero == "051" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("COND_GEN");
            }
            if (m_Nb_Entite[i].m_Numero == "052" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("CLASSE_DMT");
            }
            if (m_Nb_Entite[i].m_Numero == "053" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("EXO_TM");
            }
            if (m_Nb_Entite[i].m_Numero == "054" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("NAT_ASS");
            }
            if (m_Nb_Entite[i].m_Numero == "055" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("ADM_REMB");
            }
            if (m_Nb_Entite[i].m_Numero == "056" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("FRAIS_DEP");
            }
            if (m_Nb_Entite[i].m_Numero == "057" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TYPE_ACTE");
            }
            if (m_Nb_Entite[i].m_Numero == "058" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("TYPE_FORFAIT");
            }
            if (m_Nb_Entite[i].m_Numero == "059" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("EXT_DOC");
            }
            if (m_Nb_Entite[i].m_Numero == "060" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("ACTIVITE");
            }
            if (m_Nb_Entite[i].m_Numero == "061" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("CAT_MED");
            }
            if (m_Nb_Entite[i].m_Numero == "062" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("COD_REGROUP");
            }
            if (m_Nb_Entite[i].m_Numero == "063" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("CAT_SPEC");
            }
            if (m_Nb_Entite[i].m_Numero == "064" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("AGR_RADIO");
            }
            if (m_Nb_Entite[i].m_Numero == "065" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("CODE_PAIEMENT");
            }
            if (m_Nb_Entite[i].m_Numero == "066" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("PHASE");
            }
            if (m_Nb_Entite[i].m_Numero == "067" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("NUM_DENTS");
            }
            if (m_Nb_Entite[i].m_Numero == "068" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("DOM");
            }
            if (m_Nb_Entite[i].m_Numero == "080" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("ASS_NPREV");
            }
            if (m_Nb_Entite[i].m_Numero == "081" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("RGL_TARIF");
            }
            if (m_Nb_Entite[i].m_Numero == "082" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("SPECIALITES");
            }
            if (m_Nb_Entite[i].m_Numero == "083" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("MODIFICATEUR");
            }
            if (m_Nb_Entite[i].m_Numero == "084" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("DMT");
            }
            if (m_Nb_Entite[i].m_Numero == "090" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("COMPAT_EXO_TM");
            }
            if (m_Nb_Entite[i].m_Numero == "091" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("ARBORESCENCE");
            }
            if (m_Nb_Entite[i].m_Numero == "092" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("NOTESARBORESCENCE");
            }
            if (m_Nb_Entite[i].m_Numero == "093" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("VAL_UNITE_OEUVRE");
            }
            if (m_Nb_Entite[i].m_Numero == "099" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("LISTE_MOTS");
            }
            if (m_Nb_Entite[i].m_Numero == "101" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("ACTES");
            }
            if (m_Nb_Entite[i].m_Numero == "101" && m_Nb_Entite[i].m_Rubrique == "51") {
                m_pCMoteurCCAM_Base->DeleteAllData("COND_GEN_ACTE");
            }
            if (m_Nb_Entite[i].m_Numero == "101" && m_Nb_Entite[i].m_Rubrique == "52") {
                m_pCMoteurCCAM_Base->DeleteAllData("NOTES");
            }
            if (m_Nb_Entite[i].m_Numero == "110" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("DATES_EFFET");
            }
            if (m_Nb_Entite[i].m_Numero == "120" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("PROCEDURES");
            }
            if (m_Nb_Entite[i].m_Numero == "130" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("INCOMPATIBILITE");
            }
            if (m_Nb_Entite[i].m_Numero == "201" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("ACTIVITEACTE");
            }
            if (m_Nb_Entite[i].m_Numero == "201" && m_Nb_Entite[i].m_Rubrique == "50") {
                m_pCMoteurCCAM_Base->DeleteAllData("REC_MED_ACTE");
            }
            if (m_Nb_Entite[i].m_Numero == "210" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("MODIFICATEURACTE");
            }
            if (m_Nb_Entite[i].m_Numero == "220" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("ASSOCIABILITE");
            }
            if (m_Nb_Entite[i].m_Numero == "301" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("PHASEACTE");
            }
            if (m_Nb_Entite[i].m_Numero == "310" && m_Nb_Entite[i].m_Rubrique == "01") {
                m_pCMoteurCCAM_Base->DeleteAllData("HISTO_PHASE");
            }
        }
    }
}

//---------------------------- pushButton_Integrer_clicked -------------------------------------------------
void C_Dlg_ChangeMAJCCAM::pushButton_Integrer_clicked()
{   bool ok = TRUE;

    if (m_DejaIntegre != "")
       {
        QMessageBox mb( "ATTENTION !!!",
                        tr("Ce fichier a déjà été integré dans la base\n"
                           "le " + m_DejaIntegre.mid(6,2) + "/" + m_DejaIntegre.mid(4,2) + "/" + m_DejaIntegre.mid(0,4) + "\n"
                           "\n"
                           "Etes vous sur de vouloir l'intégrer à nouveau ?"),
                        QMessageBox::Information,
                        QMessageBox::Yes,
                        QMessageBox::Cancel  | QMessageBox::Default,0);
        mb.setButtonText( QMessageBox::Yes,    tr("Oui") );
        mb.setButtonText( QMessageBox::Cancel, tr("Non") );
        if (mb.exec()  == QMessageBox::Cancel) ok = FALSE;
        }
    if (ok)
       {
        // Definition du curseur de la souris...................................
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
        listview_Affichage->clear();
         m_Duree_Traitement.start();
        // Préparation et initialisation des bases
        if (m_TypeFichier == "FC")
           {
            // on efface tout
            EffaceBase();
           }
        if (m_TypeFichier == "FR" && m_VersionBase.m_TypeFichier == "1")
           {
            //on efface que les tables dont les entités sont présentes
           }
        if (m_TypeFichier == "FR" && m_VersionBase.m_TypeFichier == "0")
           {
            //on efface rien on fait une mise à jour (update au lieu de insert)
           }
        // Integration des données
        if (LectureFichier(m_pQFile) && !m_StopIntegration)
           {
            m_Maj_OK = TRUE;
            new Q3ListViewItem(listview_Affichage,tr("Durée de l'intégration"),QString::number( m_Duree_Traitement.elapsed()) + " secondes");
                m_pCMoteurCCAM_Base->MAJAddDatesEffet(m_ListeDate);
           }
        else
           {
            new Q3ListViewItem(listview_Affichage,tr("L'intégration ne s'est pas effectuée correctement"),QString::number( m_Duree_Traitement.elapsed()) + " secondes");
            m_VersionBase.m_DateIntegration = QDate::currentDate().toString("yyyyMMdd");
            m_VersionBase.m_Status = "Inc";
            m_pCMoteurCCAM_Base->MAJAddVersionBase(m_VersionBase);
           }
        // Restauration du curseur de la souris.................................
        QApplication::restoreOverrideCursor();
       }  // endif ok
}

//---------------------------- DecoupeEntite -------------------------------------------------
void C_Dlg_ChangeMAJCCAM::pushButton_Annuler_clicked()
{ m_StopIntegration = TRUE;
}

//---------------------------- DecoupeEntite -------------------------------------------------
void C_Dlg_ChangeMAJCCAM::DecoupeEntite( QString & ligne_a_decouper )
{QString id_entite       = ligne_a_decouper.mid(0,3);
    QString rubrique_entite = ligne_a_decouper.mid(3,2);
    QString sequence_entite = ligne_a_decouper.mid(5,2);
    long nb_entites;
    int i;
    const int valeur_section = atoi(id_entite);
    QString section_voulue;

    // s'occupe du comptage des différentes entités............................
    m_Nb_Entite[RechercheIndex(id_entite,rubrique_entite)].m_Nombre++;

    // si l'entité précédente est de type 101 52
    // et SequenceEnCours est != SequencePrecedente + 1
    // Alors il faut enregistrer la note de l'acte
    if ((m_EntitePrecedente.m_Entite == 101) && (m_EntitePrecedente.m_Rubrique == 52) && ((m_EntitePrecedente.m_Sequence + 1) != atoi(sequence_entite))) {
        m_pCMoteurCCAM_Base->MAJAddNoteActe(m_ActeEnCours.m_CodeActe,m_NoteActe);
        m_NoteActe.m_TypeNote = "";
        m_NoteActe.m_TexteNote = "";
    }
    // si l'entité précédente est de type 201 50
    // et SequenceEnCours est != SequencePrecedente + 1
    // Alors il faut enregistrer la Recommandation médicale de l'acte
    if ((m_EntitePrecedente.m_Entite == 201) && (m_EntitePrecedente.m_Rubrique == 50) && ((m_EntitePrecedente.m_Sequence + 1) != atoi(sequence_entite))) {
        m_pCMoteurCCAM_Base->MAJAddRecMedActe(m_ActeEnCours.m_CodeActe,m_Activite.m_CodeActivite,m_RecMedActe);
        m_RecMedActe.m_NumRecMed = "";
        m_RecMedActe.m_TexteRecMed = "";
    }
    // de même si l'entité précédente est de type 091 01
    // et SequenceEnCours est != SequencePrecedente + 1
    // Alors il faut enregistrer l'arborescence
    if ((m_EntitePrecedente.m_Entite == 91) && (m_EntitePrecedente.m_Rubrique == 1) && ((m_EntitePrecedente.m_Sequence + 1) != atoi(sequence_entite))) {
        m_pCMoteurCCAM_Base->MAJAddArborescence(m_Arborescence);
        m_Arborescence.m_CodeMenu = "";
        m_Arborescence.m_CodePere = "";
        m_Arborescence.m_Rang = "";
        m_Arborescence.m_Libelle = "";
    }
    // de même si l'entité précédente est de type 092 01
    // et SequenceEnCours est != SequencePrecedente + 1
    // Alors il faut enregistrer la note de l'arborescence
    if ((m_EntitePrecedente.m_Entite == 92) && (m_EntitePrecedente.m_Rubrique == 1) && ((m_EntitePrecedente.m_Sequence + 1) != atoi(sequence_entite))) {
        m_pCMoteurCCAM_Base->MAJAddNoteArborescence(m_NoteArbo);
        m_NoteArbo.m_CodeMenu = "";
        m_NoteArbo.m_TypeNote = "";
        m_NoteArbo.m_TexteNote = "";
    }
    // de même si l'entité précédente est de type 099 01
    // et SequenceEnCours est != SequencePrecedente + 1
    // Alors il faut enregistrer le mot
    if ((m_EntitePrecedente.m_Entite == 99) && (m_EntitePrecedente.m_Rubrique == 1) && ((m_EntitePrecedente.m_Sequence + 1) != atoi(sequence_entite))) {
        m_pCMoteurCCAM_Base->MAJAddListeMots(m_ListeMots);
        m_ListeMots.m_Mot = "";
        m_ListeMots.m_Definition = "";
    }

    switch (valeur_section) {

    case 0: // Debut du fichier ..............................................................................

        break;
    case 1:
        //ParseEntite(listview_Affichage, ligne_a_decouper,  id_entite );
        break;
    case 2: // Table des associations
        for (i=0; i < 5; i++) {
            m_TabAsso[i].m_CodeAsso = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString().stripWhiteSpace();
            m_TabAsso[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString();
            m_TabAsso[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabAsso[i].m_Coefficient = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabAssociation(m_TabAsso);
        for (i=0; i < 5; i++) {
            m_TabAsso[i].m_CodeAsso = "";
        }
        break;
    case 3:
        for (i=0; i < 5; i++) {
            m_TabTb03[i].m_RegleTarifaire = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString().stripWhiteSpace();
            m_TabTb03[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString();
            m_TabTb03[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabTb03[i].m_Coefficient = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB03(m_TabTb03);
        for (i=0; i < 5; i++) {
            m_TabTb03[i].m_RegleTarifaire = "";
        }
        break;
    case 4:
        for (i=0; i < 6; i++) {
            m_TabTb04[i].m_Specialite = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString().stripWhiteSpace();
            m_TabTb04[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString();
            m_TabTb04[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabTb04[i].m_ClasseSpecialite = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB04(m_TabTb04);
        for (i=0; i < 6; i++) {
            m_TabTb04[i].m_Specialite = "";
        }
        break;
    case 5:
        for (i=0; i < 6; i++) {
            m_TabTb05[i].m_NatPrestation = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString().stripWhiteSpace();
            m_TabTb05[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString();
            m_TabTb05[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabTb05[i].m_TypeForfait = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB05(m_TabTb05);
        for (i=0; i < 6; i++) {
            m_TabTb05[i].m_NatPrestation = "";
        }
        break;
    case 6:
        for (i=0; i < 4; i++) {
            m_TabTb06[i].m_CodeModificateur = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (7*i))).toString().stripWhiteSpace();
            m_TabTb06[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (7*i))).toString();
            m_TabTb06[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (7*i))).toString();
            m_TabTb06[i].m_UniteAgeMini = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (7*i))).toString();
            m_TabTb06[i].m_AgeMini = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(8 + (7*i))).toString();
            m_TabTb06[i].m_UniteAgeMaxi = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(9 + (7*i))).toString();
            m_TabTb06[i].m_AgeMaxi = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(10 + (7*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB06(m_TabTb06);
        for (i=0; i < 4; i++) {
            m_TabTb06[i].m_CodeModificateur = "";
        }
        break;
    case 7:
        for (i=0; i < 2; i++) {
            m_TabTb07[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (6*i))).toString();
            m_TabTb07[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (6*i))).toString();
            m_TabTb07[i].m_S1 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (6*i))).toString();
            m_TabTb07[i].m_S2 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (6*i))).toString();
            m_TabTb07[i].m_S3 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(8 + (6*i))).toString();
            m_TabTb07[i].m_S4 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(9 + (6*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB07(m_TabTb07);
        for (i=0; i < 2; i++) {
            m_TabTb07[i].m_DateDebut = "";
        }
        break;
    case 8:
        for (i=0; i < 4; i++) {
            m_TabTb08[i].m_Caisse = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (5*i))).toString();
            m_TabTb08[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (5*i))).toString();
            m_TabTb08[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (5*i))).toString();
            m_TabTb08[i].m_TypeFerie = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (5*i))).toString().stripWhiteSpace();
            m_TabTb08[i].m_Jour = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(8 + (5*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB08(m_TabTb08);
        for (i=0; i < 4; i++) {
            m_TabTb08[i].m_TypeFerie = "";
        }
        break;
    case 9:
        for (i=0; i < 6; i++) {
            m_TabTb09[i].m_CodeModif = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString().stripWhiteSpace();
            m_TabTb09[i].m_ModifCompat = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString().stripWhiteSpace();
            m_TabTb09[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabTb09[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB09(m_TabTb09);
        for (i=0; i < 6; i++) {
            m_TabTb09[i].m_CodeModif = "";
        }
        break;
    case 10:
        for (i=0; i < 6; i++) {
            m_TabTb10[i].m_Modificateur = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (5*i))).toString().stripWhiteSpace();
            m_TabTb10[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (5*i))).toString();
            m_TabTb10[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (5*i))).toString();
            m_TabTb10[i].m_Coherence = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (5*i))).toString();
            m_TabTb10[i].m_PresMultiple = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(8 + (5*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB10(m_TabTb10);
        for (i=0; i < 6; i++) {
            m_TabTb10[i].m_Modificateur = "";
        }
        break;
    case 11:
        for (i=0; i < 4; i++) {
            m_TabTb11[i].m_Modificateur = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (5*i))).toString().stripWhiteSpace();
            m_TabTb11[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (5*i))).toString();
            m_TabTb11[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (5*i))).toString();
            m_TabTb11[i].m_Forfait = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (5*i))).toString();
            m_TabTb11[i].m_Coef = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(8 + (5*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB11(m_TabTb11);
        for (i=0; i < 4; i++) {
            m_TabTb11[i].m_Modificateur = "";
        }
        break;
    case 12:
        for (i=0; i < 4; i++) {
            m_TabTb12[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString();
            m_TabTb12[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString();
            m_TabTb12[i].m_Forfait = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabTb12[i].m_Coef = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB12(m_TabTb12);
        for (i=0; i < 4; i++) {
            m_TabTb12[i].m_DateDebut = "";
        }
        break;
    case 13:
        for (i=0; i < 6; i++) {
            m_TabTb13[i].m_Prestation = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString().stripWhiteSpace();
            m_TabTb13[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString();
            m_TabTb13[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabTb13[i].m_Equiv = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB13(m_TabTb13);
        for (i=0; i < 6; i++) {
            m_TabTb13[i].m_Prestation = "";
        }
        break;
    case 14:
        for (i=0; i < 5; i++) {
            m_TabTb14[i].m_Dmt = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString().stripWhiteSpace();
            m_TabTb14[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString();
            m_TabTb14[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabTb14[i].m_Classe = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB14(m_TabTb14);
        for (i=0; i < 5; i++) {
            m_TabTb14[i].m_Dmt = "";
        }
        break;
    case 15:
        for (i=0; i < 6; i++) {
            m_TabTb15[i].m_Modificateur = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString().stripWhiteSpace();
            m_TabTb15[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString();
            m_TabTb15[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabTb15[i].m_ModifOC = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB15(m_TabTb15);
        for (i=0; i < 6; i++) {
            m_TabTb15[i].m_Modificateur = "";
        }
        break;
    case 17:
        for (i=0; i < 9; i++) {
            m_TabTb17[i].m_Prestation = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (3*i))).toString().stripWhiteSpace();
            m_TabTb17[i].m_Specialite = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (3*i))).toString();
            m_TabTb17[i].m_DateObligation = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (3*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB17(m_TabTb17);
        for (i=0; i < 9; i++) {
            m_TabTb17[i].m_Prestation = "";
        }
        break;
    case 18:
        for (i=0; i < 6; i++) {
            m_TabTb18[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (3*i))).toString().stripWhiteSpace();
            m_TabTb18[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (3*i))).toString();
            m_TabTb18[i].m_Localisation = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (3*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB18(m_TabTb18);
        for (i=0; i < 6; i++) {
            m_TabTb18[i].m_DateDebut = "";
        }
        break;
    case 19:
        for (i=0; i < 7; i++) {
            m_TabTb19[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (3*i))).toString().stripWhiteSpace();
            m_TabTb19[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (3*i))).toString();
            m_TabTb19[i].m_NbModif = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (3*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB19(m_TabTb19);
        for (i=0; i < 7; i++) {
            m_TabTb19[i].m_DateDebut = "";
        }
        break;
    case 20:
        for (i=0; i < 4; i++) {
            m_TabTb20[i].m_Concept = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (4*i))).toString().stripWhiteSpace();
            m_TabTb20[i].m_DateDebut = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (4*i))).toString();
            m_TabTb20[i].m_DateFin = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (4*i))).toString();
            m_TabTb20[i].m_Valeur = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + (4*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddTabTB20(m_TabTb20);
        for (i=0; i < 4; i++) {
            m_TabTb20[i].m_Concept = "";
        }
        break;
    case 50:  // Table des types de notes et de leur libellé
        m_TypesNotes.m_TypeNote = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TypesNotes.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddTypesNotes(m_TypesNotes);
        break;
    case 51:
        m_CondGen.m_Condition = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_CondGen.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddCondGenerales(m_CondGen);
        break;
    case 52:
        m_ClasseDmt.m_ClasseDMT = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_ClasseDmt.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddClasseDmt(m_ClasseDmt);
        break;
    case 53:
        m_ExoTm.m_CodeExo = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_ExoTm.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddExoTm(m_ExoTm);
        break;
    case 54:
        m_TabNatAss.m_NatAss = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabNatAss.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddNatAssurance(m_TabNatAss);
        break;
    case 55:
        m_TabRembours.m_AdmRem = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabRembours.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddAdmRemboursement(m_TabRembours);
        break;
    case 56:
        m_TabFrais.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabFrais.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddFraisDeplacement(m_TabFrais);
        break;
    case 57:
        m_TabTypeActe.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabTypeActe.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddTypeActe(m_TabTypeActe);
        break;
    case 58:
        m_TabTypeForfait.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabTypeForfait.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddTypeForfait(m_TabTypeForfait);
        break;
    case 59:
        m_TabExtDoc.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabExtDoc.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddExtDoc(m_TabExtDoc);
        break;
    case 60: // Tables des activités et de leur libellé
        m_TabActivite.m_CodeActivite = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabActivite.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddTabActivite(m_TabActivite);
        break;
    case 61:
        m_TabCatMed.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabCatMed.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddCatMed(m_TabCatMed);
        break;
    case 62:
        m_TabCodRegroup.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabCodRegroup.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddCodRegroup(m_TabCodRegroup);
        break;
    case 63:
        m_TabCatSpec.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabCatSpec.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddCatSpec(m_TabCatSpec);
        break;
    case 64:
        m_TabAgrRad.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabAgrRad.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddAgrRadio(m_TabAgrRad);
        break;
    case 65:
        m_TabCodePaiement.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabCodePaiement.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddCodePaiement(m_TabCodePaiement);
        break;
    case 66:  // Table des phases et de leur libellé
        m_TabPhase.m_CodePhase = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabPhase.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddTabPhase(m_TabPhase);
        break;
    case 67:
        m_TabNumDents.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabNumDents.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddNumDents(m_TabNumDents);
        break;
    case 68:
        m_TabDom.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabDom.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddDom(m_TabDom);
        break;
    case 80:
        m_TabAssNPrev.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabAssNPrev.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddAssNonPrevue(m_TabAssNPrev);
        break;
    case 81:
        m_TabRglTarif.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabRglTarif.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddRglTarif(m_TabRglTarif);
        break;
    case 82:
        m_TabSpec.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabSpec.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddSpecialite(m_TabSpec);
        break;
    case 83:  // Table des modificateurs et de leur libellé
        m_TabModificateur.m_CodeModificateur = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabModificateur.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddTabModificateur(m_TabModificateur);
        break;
    case 84:
        m_TabDmt.m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_TabDmt.m_Libelle = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString().stripWhiteSpace();
        m_pCMoteurCCAM_Base->MAJAddDMT(m_TabDmt);
        break;
    case 90:
        for (i=0; i < 60; i++) {
            m_TabCompatExoTM[i].m_ExoTM = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (2*i))).toString();
            m_TabCompatExoTM[i].m_ExoTMCompat = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (2*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddCompatExoTM(m_TabCompatExoTM);
        for (i=0; i < 60; i++) {
            m_TabCompatExoTM[i].m_ExoTM = "";
        }
        break;
    case 91:
        m_Arborescence.m_CodeMenu = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_Arborescence.m_CodePere = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString();
        m_Arborescence.m_Rang = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur6").toString();
        m_Arborescence.m_Libelle += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur7").toString();
        break;
    case 92:
        m_NoteArbo.m_CodeMenu = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_NoteArbo.m_TypeNote = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString();
        m_NoteArbo.m_TexteNote += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur6").toString();
        break;
    case 93:
        for (i=0; i < 7; i++) {
            m_TabValUnitOeuvre[i].m_Code = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (3*i))).toString().stripWhiteSpace();
            m_TabValUnitOeuvre[i].m_DateEffet = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (3*i))).toString();
            m_TabValUnitOeuvre[i].m_Valeur = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(6 + (3*i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddValUnitOeuvre(m_TabValUnitOeuvre);
        for (i=0; i < 7; i++) {
            m_TabValUnitOeuvre[i].m_Code = "";
        }
        break;
    case 99: // Liste des mots
        m_ListeMots.m_Mot = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_ListeMots.m_Definition += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString();
        break;
    case 101:
        if (rubrique_entite == "01") { // Debut d'un acte
            section_voulue = id_entite + rubrique_entite;
            m_ActeEnCours.m_CodeActe = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
            m_ActeEnCours.m_LibelleCourt = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur5").toString().stripWhiteSpace();
            m_ActeEnCours.m_TypeActe = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur6").toString();
            m_ActeEnCours.m_Sexe = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur7").toString();
            m_ActeEnCours.m_DateCreation = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur8").toString();
            m_ActeEnCours.m_DateFin = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur9").toString();
        }
        if (rubrique_entite == "02") { // suite d'un acte
            section_voulue = id_entite + rubrique_entite;
            m_ActeEnCours.m_Assurance1 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
            m_ActeEnCours.m_Assurance2 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur5").toString();
            m_ActeEnCours.m_Assurance3 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur6").toString();
            m_ActeEnCours.m_Assurance4 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur7").toString();
            m_ActeEnCours.m_Assurance5 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur8").toString();
            m_ActeEnCours.m_Assurance6 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur9").toString();
            m_ActeEnCours.m_Assurance7 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur10").toString();
            m_ActeEnCours.m_Assurance8 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur11").toString();
            m_ActeEnCours.m_Assurance9 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur12").toString();
            m_ActeEnCours.m_Assurance10 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur13").toString();
            m_ActeEnCours.m_Deplacement = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur14").toString();
        }
        if (rubrique_entite == "03") { // suite d'un acte
            section_voulue = id_entite + rubrique_entite;
            m_ActeEnCours.m_Arborescence1 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
            m_ActeEnCours.m_Arborescence2 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur5").toString();
            m_ActeEnCours.m_Arborescence3 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur6").toString();
            m_ActeEnCours.m_Arborescence4 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur7").toString();
            m_ActeEnCours.m_Arborescence5 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur8").toString();
            m_ActeEnCours.m_Arborescence6 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur9").toString();
            m_ActeEnCours.m_Arborescence7 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur10").toString();
            m_ActeEnCours.m_Arborescence8 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur11").toString();
            m_ActeEnCours.m_Arborescence9 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur12").toString();
            m_ActeEnCours.m_Arborescence10 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur13").toString();
            m_ActeEnCours.m_PlaceArbo = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur14").toString();
            m_ActeEnCours.m_CodeStructure = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur15").toString();
            m_ActeEnCours.m_CodePrecedent = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur16").toString();
            m_ActeEnCours.m_CodeSuivant = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur17").toString();
        }
        if (rubrique_entite == "04") { // suite d'un acte

        }
        if (rubrique_entite == "50") { // suite d'un acte : LIBELLE LONG
            section_voulue = id_entite + rubrique_entite;
            m_ActeEnCours.m_LibelleLong += ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
        }
        if (rubrique_entite == "51") { // Debut d'un acte
            section_voulue = id_entite + rubrique_entite;
            for (i=0; i < 30; i++) {
                m_CondGenActe[i].m_Code = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur" + QString::number(4 + i)).toString().stripWhiteSpace();
            }
            m_pCMoteurCCAM_Base->MAJAddCondGenActe(m_ActeEnCours.m_CodeActe,m_CondGenActe);
            for (i=0; i < 30; i++) {
                m_CondGenActe[i].m_Code = "";
            }
        }
        if (rubrique_entite == "52") { // Debut d'un acte : NOTE DE L'ACTE
            section_voulue = id_entite + rubrique_entite;
            m_NoteActe.m_TypeNote = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
            m_NoteActe.m_TexteNote += ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur5").toString();
        }
        break;
    case 110: // suite de l'acte ..............................................................................

        //..... rajoute le 03022006.....
        m_HistoActe.m_DateEffet = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        StockageDate(m_HistoActe.m_DateEffet);
        //..... fin de rajoute le 03022006.....

        m_ActeEnCours.m_Ccam26 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        m_ActeEnCours.m_Ccam27 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString();
        m_ActeEnCours.m_Ccam28 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur6").toString();
        m_ActeEnCours.m_Ccam22 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur7").toString();
        m_ActeEnCours.m_Ccam24 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur8").toString();
        // addition des valeurs car dans base actuelle tout est stocké dans une seule colonne
        m_ActeEnCours.m_Ccam25 = "";
        m_ActeEnCours.m_Ccam25 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur9").toString();
        m_ActeEnCours.m_Ccam25 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur10").toString();
        m_ActeEnCours.m_Ccam25 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur11").toString();
        m_ActeEnCours.m_Ccam25 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur12").toString();
        m_ActeEnCours.m_Ccam25 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur13").toString();
        // addition des valeurs car dans base actuelle tout est stocké dans une seule colonne
        m_ActeEnCours.m_Ccam31 = "";
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur14").toString();
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur15").toString();
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur16").toString();
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur17").toString();
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur18").toString();
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur19").toString();
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur20").toString();
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur21").toString();
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur22").toString();
        m_ActeEnCours.m_Ccam31 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur23").toString();
        // addition des valeurs car dans base actuelle tout est stocké dans une seule colonne
        m_ActeEnCours.m_Ccam34 = "";
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur24").toString();
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur25").toString();
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur26").toString();
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur27").toString();
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur28").toString();
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur29").toString();
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur30").toString();
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur31").toString();
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur32").toString();
        m_ActeEnCours.m_Ccam34 += ValeurDansEntite(ligne_a_decouper,id_entite,"valeur33").toString();
        break;
    case 120: // Détail des actes de la procédure
        for (i=0; i < 8; i++) {
            m_CodeProcedure[i] = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + i)).toString().stripWhiteSpace();
        }
        m_pCMoteurCCAM_Base->MAJAddProcedure(m_ActeEnCours.m_CodeActe,m_HistoActe.m_DateEffet,m_CodeProcedure);
        RAZ_Tableau(m_CodeProcedure,8);
        break;
    case 130: // incompatibilités de l'acte
        for (i=0; i < 8; i++) {
            m_ActeIncompatible[i] = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + i)).toString().stripWhiteSpace();
        }
        m_pCMoteurCCAM_Base->MAJAddIncompatibilite(m_ActeEnCours.m_CodeActe,m_HistoActe.m_DateEffet,m_ActeIncompatible);
        RAZ_Tableau(m_ActeIncompatible,8);
        break;
    case 199: // Fin de l'acte ..............................................................................
        // Verifie la coherence du nombre d'entités rencontrées
        nb_entites = 0;
        nb_entites += RechercheNombreEntite("101","01","RAZ");
        nb_entites += RechercheNombreEntite("101","02","RAZ");
        nb_entites += RechercheNombreEntite("101","03","RAZ");
        nb_entites += RechercheNombreEntite("101","04","RAZ");
        nb_entites += RechercheNombreEntite("101","50","RAZ");
        nb_entites += RechercheNombreEntite("101","51","RAZ");
        nb_entites += RechercheNombreEntite("101","52","RAZ");
        nb_entites += RechercheNombreEntite("110","01","RAZ");
        nb_entites += RechercheNombreEntite("120","01","RAZ");
        nb_entites += RechercheNombreEntite("130","01","RAZ");
        nb_entites += RechercheNombreEntite("201","01","RAZ");
        nb_entites += RechercheNombreEntite("201","50","RAZ");
        nb_entites += RechercheNombreEntite("210","01","RAZ");
        nb_entites += RechercheNombreEntite("220","01","RAZ");
        nb_entites += RechercheNombreEntite("301","01","RAZ");
        nb_entites += RechercheNombreEntite("301","50","RAZ");
        nb_entites += RechercheNombreEntite("310","01","RAZ");
        nb_entites += RechercheNombreEntite("350","01","RAZ");
        nb_entites += RechercheNombreEntite("399","01","RAZ");
        nb_entites += RechercheNombreEntite("299","01","RAZ");
        nb_entites += RechercheNombreEntite("199","01","RAZ");

        // Enregistre acte dans la base
        if (nb_entites == atol(ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString())) {
            m_pCMoteurCCAM_Base->MAJAddActe(m_ActeEnCours);
        }

        m_ActeEnCours.m_CodeActe = "Non";
        m_ActeEnCours.m_LibelleLong = "";

        break;
    case 201:
        if (rubrique_entite == "01") { // Début code activité acte
            section_voulue = id_entite + rubrique_entite;
            m_Activite.m_CodeActe = m_ActeEnCours.m_CodeActe;
            m_Activite.m_CodeActivite = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
        }
        if (rubrique_entite == "50") { // Début code activité acte
            section_voulue = id_entite + rubrique_entite;
            m_RecMedActe.m_NumRecMed = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
            m_RecMedActe.m_TexteRecMed += ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur5").toString();
        }
        break;
    case 210: // Historique de l'acte/activité
        m_HistoActivite.m_DateEffet = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur4").toString();
        StockageDate(m_HistoActivite.m_DateEffet);
        m_HistoActivite.m_DateAM = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur5").toString();
        m_HistoActivite.m_DateJO = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur6").toString();
        //m_HistoActivite.m_CategorieMedicale = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur17").toString();
        //m_HistoActivite.m_SpeExecPermise1 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur18").toString();
        //m_HistoActivite.m_SpeExecPermise2 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur19").toString();
        //m_HistoActivite.m_SpeExecPermise3 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur20").toString();
        //m_HistoActivite.m_SpeExecPermise4 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur21").toString();
        //m_HistoActivite.m_SpeExecPermise5 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur22").toString();
        //m_HistoActivite.m_SpeExecPermise6 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur23").toString();
        //m_HistoActivite.m_SpeExecPermise7 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur24").toString();
        //m_HistoActivite.m_SpeExecPermise8 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur25").toString();
        //m_HistoActivite.m_SpeExecPermise9 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur26").toString();
        //m_HistoActivite.m_SpeExecPermise10 = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur27").toString();
        //m_HistoActivite.m_CodeRegroupement = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur28").toString();
        for (i=0; i < 10; i++) {
            m_ModificateurActe[i] = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(7 + i)).toString().stripWhiteSpace();
        }
        m_pCMoteurCCAM_Base->MAJAddModificateurActe(m_Activite.m_CodeActe,m_Activite.m_CodeActivite,m_HistoActivite.m_DateEffet,m_ModificateurActe);
        RAZ_Tableau(m_ModificateurActe,10);
        break;
    case 220: // Compatibilités de l'acte
        for (i=0; i < 8; i++) {
            m_Compatibilite[i].m_CodeActe = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(4 + (2 * i))).toString().stripWhiteSpace();
            m_Compatibilite[i].m_CodeActivite = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(5 + (2 * i))).toString();
            m_Compatibilite[i].m_RegleTarifaire = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur" + QString::number(20 + (1 * i))).toString();
        }
        m_pCMoteurCCAM_Base->MAJAddCompatibilite(m_Activite.m_CodeActe,m_Activite.m_CodeActivite,m_HistoActivite.m_DateEffet,m_Compatibilite);
        break;

    case 299: // Fin code activité acte ..............................................................................
        // Faire ici les verifications d'usage
        m_pCMoteurCCAM_Base->MAJAddActiviteActe(m_Activite.m_CodeActe,m_Activite.m_CodeActivite);
        m_Activite.m_CodeActe = "";
        m_Activite.m_CodeActivite = "";
        RAZ_Compatibilite(m_Compatibilite);
        break;
    case 301:
        if (rubrique_entite == "01") { // Début phase de traitement
            section_voulue = id_entite + rubrique_entite;
            m_Phase.m_CodeActe = m_Activite.m_CodeActe;
            m_Phase.m_CodeActivite = m_Activite.m_CodeActivite;
            m_Phase.m_CodePhase = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
            m_Phase.m_NbDents = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur5").toString();
            m_Phase.m_AgeMini = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur58").toString();
            m_Phase.m_AgeMaxi = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur59").toString();
        }
        if (rubrique_entite == "50") { // Suite phase de traitement
            section_voulue = id_entite + rubrique_entite;
            m_Phase.m_ICR = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
            m_Phase.m_Classant = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur5").toString();
        }
        break;
    case 310:
        if (rubrique_entite == "01") { // Historique niveau acte/activité/phase
            section_voulue = id_entite;
            m_Historique.m_CodeActe = m_Phase.m_CodeActe;
            m_Historique.m_CodeActivite = m_Phase.m_CodeActivite;
            m_Historique.m_CodePhase = m_Phase.m_CodePhase;
            m_Historique.m_DateEffet = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur4").toString();
            StockageDate(m_Historique.m_DateEffet);
            m_Historique.m_DateAM = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur5").toString();
            m_Historique.m_DateJO = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur6").toString();
            m_Historique.m_NbMaxSeances = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur7").toString();
            m_Historique.m_UniteOeuvre = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur8").toString();
            m_Historique.m_CoefUO = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur9").toString();
            m_Historique.m_CodePaiement = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur10").toString();
            m_Historique.m_PU = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur11").toString();
            m_Historique.m_CoefMAJDom1 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur12").toString();
            m_Historique.m_CoefMAJDom2 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur13").toString();
            m_Historique.m_CoefMAJDom3 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur14").toString();
            m_Historique.m_CoefMAJDom4 = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur15").toString();
            m_Historique.m_SuppChgCab = ValeurDansEntite(ligne_a_decouper,section_voulue,"valeur16").toString();
            m_pCMoteurCCAM_Base->MAJAddHistorique(m_Historique);
        }
        break;
    case 399: // Fin phase de traitement
        // Faire ici les verifications d'usage
        m_pCMoteurCCAM_Base->MAJAddPhaseActe(m_Phase);
        m_Phase.m_CodeActe = "";
        m_Phase.m_CodeActivite = "";
        m_Phase.m_CodePhase = "";
        m_Phase.m_NbDents = "";
        m_Phase.m_AgeMini = "";
        m_Phase.m_AgeMaxi = "";
        m_Phase.m_ICR = "";
        m_Phase.m_Classant = "";
        break;
    case 999: // Fin du fichier ..............................................................................
        // le fichier est intégré donc on stocke dans la base sa version
        m_VersionBase.m_DateIntegration = QDate::currentDate().toString("yyyyMMdd");
        m_VersionBase.m_Status = "Ok";
        m_pCMoteurCCAM_Base->MAJAddVersionBase(m_VersionBase);
        break;

    } // fin de switch (valeur_section)

    // memorisation de l'entité rencontrée
    m_EntitePrecedente.m_Entite   = atoi(id_entite);
    m_EntitePrecedente.m_Rubrique = atoi(rubrique_entite);
    m_EntitePrecedente.m_Sequence = atoi(sequence_entite);
}

//----------------------------------- RAZ_Tableau -------------------------------------------
void C_Dlg_ChangeMAJCCAM::RAZ_Tableau( QString * tableau, int longueur )
{  int i = 0;
    while (i < longueur)
    {
        tableau[i] = "";
        i++;
    }
}

//----------------------------------- RAZ_Compatibilite -------------------------------------------
void C_Dlg_ChangeMAJCCAM::RAZ_Compatibilite( ENT_COMPATIBILITE * compatibilite )
{ /* for (int i = 0; i < 8; i++) {
        compatibilite[i].m_CodeActe = "";
        compatibilite[i].m_CodeActivite = "";
        compatibilite[i].m_RegleTarifaire = "";
     }*/

    int i=0;
    while (i < 8 ){
        compatibilite->m_CodeActe = "";
        compatibilite->m_CodeActivite = "";
        compatibilite->m_RegleTarifaire = "";
        i++;
        compatibilite++;
    }
}

//----------------------------------- converti -------------------------------------------
void C_Dlg_ChangeMAJCCAM::converti( char * source )
{while (*source)
    {if ( (unsigned char) *source ==  0x92) *source = 0x27;
     source++;
    }
}

//----------------------------------- InformationsEntites -------------------------------------------
void C_Dlg_ChangeMAJCCAM::InformationsEntites( QString & ligne_a_decouper )
{ QString id_entite       = ligne_a_decouper.mid(0,3);
    QString rubrique_entite = ligne_a_decouper.mid(3,2);

    const int valeur_section = atoi(id_entite);

    switch (valeur_section) {

    case 0: // Debut du fichier ..............................................................................
        if (ligne_a_decouper.mid(49,3) == "CAM") {
            m_TypeFichier = "FR"; // si fichier au format reduit
        } else if (ligne_a_decouper.mid(49,3) == "CAC") {
            m_TypeFichier = "FC"; // si fichier au format base complète
        } else {
            m_TypeFichier = "Inconnu";
        }
        // Verifie si le fichier est déja intégré, pour avertir l'utilisateur
        if (m_TypeFichier != "Inconnu") {
            m_VersionBase.m_IdFichier = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur9").toString();
            m_VersionBase.m_DateCreation = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur10").toString();
            m_VersionBase.m_DateIntegration = "";
            m_VersionBase.m_Chrono = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur12").toString();
            m_VersionBase.m_TypeFichier = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur13").toString();
            m_VersionBase.m_NumVersion = ValeurDansEntite(ligne_a_decouper,id_entite,"valeur14").toString();
        }

        break;
    case 999: // Fin du fichier ..............................................................................
        if (m_TypeFichier == "FC") {
            m_Nb_EntitesTotales = atol(ligne_a_decouper.mid(52,10));
            m_Nb_Entite002020 = atol(ligne_a_decouper.mid(62,6));
            m_Nb_Entite10101 = atol(ligne_a_decouper.mid(68,6));
            m_Nb_Entite20101 = atol(ligne_a_decouper.mid(74,6));
            m_Nb_Entite30101 = atol(ligne_a_decouper.mid(80,6));
        } else if (m_TypeFichier == "FR") {
            m_Nb_EntitesTotales = atol(ligne_a_decouper.mid(52,8));
            m_Nb_Entite002020 = atol(ligne_a_decouper.mid(60,6));
            m_Nb_Entite10101 = atol(ligne_a_decouper.mid(66,6));
            m_Nb_Entite20101 = atol(ligne_a_decouper.mid(72,6));
            m_Nb_Entite30101 = atol(ligne_a_decouper.mid(78,6));
        }
        break;
    }
    // compte les différentes entités....................
    int index = RechercheIndex(id_entite,rubrique_entite);
    if ( index > -1) {
        m_Nb_Entite[index].m_Nombre++;
    } else {
        m_Nb_Entite[m_Index_max].m_Numero = id_entite;
        m_Nb_Entite[m_Index_max].m_Rubrique = rubrique_entite;
        m_Nb_Entite[m_Index_max].m_Nombre++;
        m_Index_max++;
    }
}



