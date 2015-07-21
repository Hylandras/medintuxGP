/********************************* CApp.h *****************************************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: data.medical.design@club-internet.fr

    This program is free software; you can redistribute it and/or modify
    it under the terms of the CeCILL Version 1 du 21/06/2004
    License (GPL compatible)
                               http://www.cecill.info/
    as published by :

    Commissariat � l'Energie Atomique
    - CEA,
                             31-33 rue de la F�d�ration, 75752 PARIS cedex 15.
                             FRANCE
    Centre National de la Recherche Scientifique
    - CNRS,
                             3 rue Michel-Ange, 75794 Paris cedex 16.
                             FRANCE
    Institut National de Recherche en Informatique et en Automatique
    - INRIA,
                             Domaine de Voluceau, Rocquencourt, BP 105, 78153
                             FRANCE

          This program is distributed in the hope that it will be useful,
          but WITHOUT ANY WARRANTY; without even the implied warranty of
          MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
          CeCILL  License (GPL compatible) for more details.

*/

#ifndef C_APP_H
#define C_APP_H

#ifdef Q_WS_WIN
   #define F_EXE ".exe"
#else
   #define F_EXE ""
#endif


#include "../../drtux/src/CMoteur_Base.h"       // inclu aussi      #include "CRubList.h"
#include "../../MedinTuxTools/C_ImageList.h"    // Gestion des listes d'images
#include "../../MedinTuxTools/Theme.h"          // Gestion du thème de l'appli
#include "../../MedinTuxTools/ThemePopup.h"
#include "../../MedinTuxTools/CCoolPopup.h"     // Gestion des popups surgissants
#include "C_Atcd.h"

#ifdef SESAMVITALE_VERSION
    #include "../../SesamVitale-QT3/C_MUI_App.h"
    #include "../../SesamVitale-QT3/C_Cps.h"
    #include "../../SesamVitale-QT3/C_Vitale.h"
#else
    #include "../../MedinTuxTools/C_MUI_App.h"
#endif

#include <qobject.h>
#include <qmap.h>
#include <qiconset.h>

#define TR  QObject::tr
#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))

#define TYP_ORDO_CALC_CURENT    CMDI_Prescription::S_GetType()+999

#define WRITE_USER_PARAM   CGestIni::Param_WriteParam
#define READ_USER_PARAM    CGestIni::Param_ReadParam
#define READ_LOCAL_PARAM   CGestIni::Param_ReadParam
#define USER_PARAM         G_pCApp->m_UserDataParam
#define LOCAL_PARAM        G_pCApp->m_DrTuxParam
#define UPDATE_USER_PARAM  G_pCApp->m_pCMoteurBase->Param_SavParam
//..................... Liste de paires type nom d'une variable (clef) et son contenu...................................................
typedef QMap<QString, QString> VAR_MAP;

//............................... exportation des variables globales Uniques de l'application ....................................
//
class DrTux;                        // classe principale et parente de l'application
class CMoteurBase;                // classe d'outils de gestion des données en base de donnée
class CMedicaBase;                // classe d'outils de gestion des données en base de donnée therapeutique
class CMDI_ChoixPatient;        // classe de la fenetre de choix des patients
class CMDI_Ident;                // classe de la fenetre indentité du patient
class CMDI_Prescription;        // classe de la fenetre de gestion des prescriptions du patient
class CMDI_Observation;                // classe de la fenetre de gestion des observations du patient
class CMDI_Terrain;                // classe de la fenetre de gestion du terrain du patient
class CMDI_Documents;                // classe de la fenetre de gestion du terrain du patient
class CMDI_Generic;                // classe de la fenetre de gestion generique d'une rubrique
class MyEditText;
class CRubRecord;

//================================================= CApp ==============================================================
class CApp : public C_MUI_App
{

   Q_OBJECT

 public:


     // Quelques enum pour les méthodes de trie
     /*! \brief Enumération pour les fonctions de tri des variables et des antécédents.
      *  Peut prendre les valeurs SortByDate, SortByValue, SortByRubrique, SortBySection, SortByLibelle
     */
     enum SortBy { SortByDate=0, SortByValue=1, SortByRubrique=2, SortBySection=3, SortByLibelle=4 };
     enum FIND_TYPE { IfExist=0, Always=1};

     CApp(QString mui_name, int & argc, char ** argv);
    ~CApp();


     void           saveLastUserOn_Ini(const QString &user, const QString &signUser, const QString &password, bool passwordMustBeRecorded  = 1 );
     int            AuthentifyAndSelectUser(QWidget * parent, const QString &newUser, const QString &newSignUser, bool passwordMustBeRecorded  = 1 );
     bool           IsAppStartWithUser();
     bool           IsThisDroitExist(const char *listDroits, const char *droitToFind);
     void           Map_Prefix_Droits();
     QString        GetMap_Prefix_Droits(const QString &doc_Type);
     int            SetIconTherapeutique();
     CMDI_Generic  *GetCurrentRubrique(QString *pRubName  = 0 , int* rub_type_ret  = 0 , CRubRecord **ppCRubRecord = 0 );
     void           putCurrentRubriqueOnModifiedState();
     int            PathDocumentToType( const QString &path , QString *pRubNameToFind = 0 );
     int            ApproximativeRubNameToType( const QString &texte , QString *pRubNameToFind=0);
     QString        loadContentsIfBeginByTokenFile(QString &arg);
     //------------------------------- delAccents-----------------------------------------------------
     /*! \brief converti les caracteres accentues d'une chaine en caracteres non accentues
     */
     QString delAccents(const QString &src);
     //--------------------------------------------- debugMode -------------------------------------------------------------------
     /*! \brief retourne si l'application est  en mode debug. Le mode debug est determine dans la section [Connexion] par la variable DebugMode = 1
     */
     int debugMode();
     //--------------------------------- GetIDCurrentDoc -----------------------------------------------------
     /*! \brief Retourne le CRubRecord correspondant au document actuellement affiché dans la CMDI gérant le doc_type.
     */
     static CRubRecord    *GetIDCurrentDoc(const QString &doc_type_in, const DOCUMENT_DISPLAY_MAP  *currentDocDisplayMap);
     //--------------------------------------------- get_PossiblesRubNameList --------------------------------------
     /*! \brief retourne la liste des rubriques possibles (ne sont pas forcement toutes activées selon les profils)
     *  \return Renvoie une QStringList avec le nom des rubriques possibles
     */
     QStringList    get_PossiblesRubNameList();
     //--------------------------------------------- SetPathGlossaire --------------------------------------
     /*! \brief positionne le chemin du glossaire sur le path fourni en entree
     *  \param pathGlossaire : const QString chemin du glossaire à positionner
     *  \return Renvoie un chemin actif en absolu du glossaire
     */
     QString        SetPathGlossaire(QString pathGlossaire);
     //--------------------------------------------- ResolvPathGlossaire --------------------------------------
     /*! \brief resoud un chemin comportant les eventuelles macros $Glossaire $Vigie par le chemin absolu
      *  \param pathFile :  QString chemin d'entree
      *  \return Renvoie le chemin modifie
      */
     QString        resolvePath(QString pathFile);
     //--------------------------------------------- ResolvPathGlossaire --------------------------------------
     /*! \brief retourne sous forme textuelle le type de rubrique associée au nom de rubrique fourni en entrée
     *  \param pathGlossaire : const QString chemin du glossaire tel que trouvé dans le fichier de parametrage
     *  \return Renvoie un chemin actif en absolu du glossaire
     */
     QString        ResolvPathGlossaire(QString pathGlossaire);
     //--------------------------------------------- RubNameToStringType --------------------------------------
     /*! \brief retourne sous forme textuelle le type de rubrique associée au nom de rubrique fourni en entrée
     *  \param rubName : const QString & nom de la rubrique dont on recherche le type
     *  \param f_typ : FIND_TYPE  SI  FIND_TYPE::Always alors le type est toujours retourné  SI FIND_TYPE::ifExist alors le type n'est retourné que si la rubrique est présente dans DrTux
     *  \return Renvoie le type de rubrique
     */
     QString          RubNameToStringType( const QString &rubName , FIND_TYPE f_typ = CApp::Always);

    //--------------------------------------------- RubNameToType --------------------------------------
    /*! \brief retourne sous forme textuelle le type de rubrique associée au nom de rubrique fourni en entrée
     *  \param rubName : const QString & nom de la rubrique dont on recherche le type
     *  \param f_typ : FIND_TYPE  SI  FIND_TYPE::Always alors le type est toujours retourné  SI FIND_TYPE::ifExist alors le type n'est retourné que si la rubrique est présente dans DrTux
     *  \return Renvoie le type de rubrique
     */
     int              RubNameToType( const QString &rubName , FIND_TYPE f_typ = CApp::Always);

     QString          TypeToRubName( int typRub );
     static QString   GetImageFileName(QString *last_path  = 0 , QString stringPreselect ="", int prewiew =1, QWidget *pQWidget =0);
     QString          DoPopupList(QStringList &list);

     QString          getListNameRubriqueMenu(int typeRubrique);
     QString          getListNameRubriqueMenu(const QString &rubName);
     /*! \brief Affiche un popup amusant */
     void  CouCou(const QString &message, const QString &imagePath="", int tempo = 0);
     void  CouCou(const QString &message, const QColor &color, const QFont &ft , int tempo = 0, const QString &imagePath="");

//=====================================================================================================================
///////////////////////////////////////////// FONCTIONS COMMUNES AUX RUBRIQUES AVEC ///////////////////////////////////
/////////////////////////////////////////////       EDITEUR TEXTE MULTIMEDIA        ///////////////////////////////////
/////////////////////////////////////////   (a restructurer dans une classe commune ) /////////////////////////////////
//_____________________________________________________________________________________________________________________
      //--------------------------------------------- IsExistRubrique --------------------------------------------
      /*! \brief retourne un pointeur sur la rubrique courante (m_pDrTux doit avoir été initialisé ce qui est forcément le cas lors de l'appel)
       *  \param pRubName : pointeur sur une QString qui si il n'est pas nul contiendra le nom de la rubrique courante
       *  \return Renvoie un pointeur sur la rubrique en cours sinon vide (NULL)
       */
      CMDI_Generic *IsExistRubrique(const QString &pRubName);
      //--------------------------------------------- IsExistRubrique --------------------------------------------
      /*! \brief teste si une des fenêtres rubriques dont le type est fourni en entrée est ouverte dans le WorkSpace des rubriques.
       *  \return pointeur sur la rubrique si elle existe ou zero.
       */
      CMDI_Generic *IsExistRubrique(int rubTyp);

     //------------------------------------------- detectTypeRubFromDoc ----------------------------------------
     /*! \brief verifie si dans le document il n'y a pas l'instruction {{DOC_TYPE=Observation,1}}"
      *  \param text :      const QString & texte du document si vide alors le document sera charge a partir de pathDoc
      *  \param pathDoc :   const QString & chemin du document
      *  \param noInquire : int * qui si different de zero, sera mis a jour avec le parametre noInquire
      * \return int :   le type du document
     */
     int detectTypeRubFromDoc(const QString &text_in, const QString &pathDoc, int *noInquire =0);

      //--------------------------------------------- On_pushButtonAppend --------------------------------------------
      /*! \brief teste si une des fenêtres rubriques dont le type est fourni en entrée est ouverte dans le WorkSpace des rubriques.
       *  \return ajoute un document a une rubrique, en passant par le menu hierarchique des documents
       */
     int On_pushButtonAppend(CMDI_Generic* pCMDI);

     //------------------------------------------- directoryToPopup ----------------------------------------
     /*! \brief Créer l'arborescence de la liste et l'introduit dans le fichier cache.
      *  \param ThemePopup :   menu qui reçoit l'arborescence
      *  \param path :         Chemin à explorer
      *  \param numSousMenus : indice du sous menu dans la hierarchie (colonne).
      *  \param idMainMenu :   identifiant du menu principal.
      *  \param nb :           si indique ( !=0) comptera le nb item.
      * \return int :
      */
     int          directoryToPopup(ThemePopup* pPopup, QString path, int numSousMenus, int idMainMenu, int *nb=0);

     //----------------------------- AddNewDocument -----------------------------------
     /*! \brief insere un document dans une rubrique textuelle*/
     CRubRecord*   AddNewDocument(CMDI_Generic* pCMDI, const QString &strData_in, int typ, const char *path =0 , const QDateTime *pQDateTime = 0, const QString *libelle =0, const QString *subType =0, int noInquire =0);
     //----------------------------- DoActionWithThisDocument -----------------------------------
     /*! \brief fait avec le document donné en entrée une action dépendante du verbe  */
     void          DoActionWithThisDocument(CMDI_Generic* pCMDI, const QString &verbe, int typ, const char *path  =0 , const QDateTime * pQDateTime =0 , const QString * libelle =0 , const QString *subType =0 );
     //----------------------------- isThisImageInCache -----------------------------------
     /*! \brief verifie si un image est presente dans les caches /Glossaire/ImagesFolder et m_PathAppli/Ressources/RubriquesTextes*/
     bool          isThisImageInCache(const QString imgName);
     //----------------------------------------- makeData --------------------------------
     /*! \brief Fonction à partir du nom du fichier à insérer, et du masque d'insertion, les données à placer en base...
      *  \param imgName : &QString qui contient le nom de l'image à insérer
      *  \param  w : int contenant la largeur maximale d'affichage l'image à insérer
      *  \param  h : int contenant la hauteur maximale d'affichage l'image à insérer
      *  \param  fileMask : &QString contenant le chemin du fichier du masque d'insertion (si vide il sera utilisé m_InsertMask de la classe )
      *  \return QString qui contient les données.
     */
     QString makeData(const QString &imageFile, int h, int w, int maxImageWidth = 1024, const QString &fileMask  = "" );
    //----------------------------------------- makeData --------------------------------
    /*! \brief Fonction à partir du nom du fichier à insérer, et du masque d'insertion, les données à placer en base...
     *  \param imgName : &QString qui contient le nom de l'image à insérer
     *  \param  w : int contenant la largeur maximale d'affichage l'image à insérer
     *  \param  h : int contenant la hauteur maximale d'affichage l'image à insérer
     *  \param  html_Part :  une référence sur une QString à laquelle sera ajoutée  la partie html des données
     *  \param  image_Part : un pointeur sur une QString à laquelle (si pointeur non egal à zéro) sera ajoutée  la partie xml des données images (par défaut à zéro)
     *  \param  fileMask : &QString contenant le chemin du fichier du masque d'insertion (si vide il sera utilisé m_InsertMask de la classe ) (par défaut vide)
     *  \return néant.
     */
     void makeData(const QString &imageFile, int h, int w, QString &html_Part, int maxImageWidth=1024, QString *image_Part = 0 , const QString &fileMask   = ""  );

     //----------------------------- MixMaskWhithImage -----------------------------------
     /*! \brief Fonction à partir du nom du fichier à insérer, et du masque d'insertion, crée la chaine HTML à insérer...
      *  \param in : QCString ou QByteArray ou QString qui contient le nom de l'image à insérer
      *  \return Renvoie un pointeur vers le QPixmap décodé.
      */
      //   Image  {{IMAGE_NAME}}
      //   {{IMAGE_WIDTH=300}} {{IMAGE_HEIGHT=100}}
     static QString MixMaskWhithImageName( QString imgName , int w, int h, const QString &fileMask  = "" );
     //--------------------------------------- CouCouDestroy --------------------------------------------------------
     /*! \brief decode les donnees meditux et les place dans une  MyEditText pretes à l'affichage*/
     int       decodeMedinTuxData(QByteArray  &data, MyEditText* pMyEditText, int from);
     //----------------------------- interfaceToData -----------------------------------
     /*! \brief Fonction crée la chaine de caractères des données du MyEditText à enregistrer
      *  \param len : un pointeur sur int qui si il n'est pas egal à zero sera initialisé sur la longueur des données
      *  \return Renvoie un QString des données
     */
     QString   interfaceToData(MyEditText* pMyEditText, int *len = 0);
     QString   interfaceToData(const QString &text,     int *len = 0);
     //----------------------------- encodePixmapToBase64 -----------------------------------
     /*! \brief Encode en base64 la QPixmap passée en paramètre */
     static QString   encodePixmapToBase64(const QPixmap& pixIn);
     //----------------------------- dataToInterface -----------------------------------
     /*! \brief Fonction qui place les images dans la liste d'images et initialise un QString avec les donnees html
     */
     void      dataToInterface(const QString &data, QString &dstString);
     //----------------------------- dataToInterface -----------------------------------
     /*! \brief Fonction qui place les images dans la liste d'images et initialise un QTextEdit avec les donnees html
     */
     void      dataToInterface(const QString &data, MyEditText *pMyEditText);
     //----------------------------- dataToInterface -----------------------------------
     /*! \brief Fonction qui place les images dans la liste d'images et initialise les bornes pos_deb et pos_end du texte html
     *  \param  pos_deb : int contenant la position du debut des donnees  html
     *  \param  pos_end : int contenant la position de la fin des donnees html
     *  \return Renvoie 1 un si ok 0 zero sinon
     */
     int       dataToInterface(const QString &data, int &pos_deb, int &pos_end);

    //----------------------------- decodeStringTo_QPixmap -----------------------------------
    /*! \brief Fonction qui décode un texte base64 en image...
     *  \param in : QCString ou QByteArray ou QString qui contient l'image encodée en base64
     *  \return Renvoie un pointeur vers le QPixmap décodé.
    */
     static QPixmap   decodeStringTo_QPixmap( const QString& in );
public slots:
     void  Slot_FileSelected();
     void  Slot_OnMenuActivated(int id);
     void  Slot_On_MainMenuActivated(int id);
     //--------------------------------------------- quit -------------------------------------------------------------------
     /*! \brief surcharge du Slot quit afin d'envoyer le message Sign_QuitterRequired permettant a ceux qui s'y connectent de sauver les meubles \
     */
     void  quit();

public:


    DrTux                 *m_pDrTux;              /*!< Pointeur vers DrTux Objet principal */
    CMoteurBase           *m_pCMoteurBase;        /*!< Moteur de la base de données DrTux */
    CMedicaBase           *m_pCMedicaBase;        /*!< Moteur de la base de données médicamenteuse */
    QStringList                      m_ListRubName;                /*!< Liste des rubriques disponibles */
    QMap<QString, int>               m_mapNameRubType;             /*!< liste des nom de rubrique avec leur type marche pas en static au linkage ==> G_mapNameRubType */
    QMap<QString, QString>           m_mapNameRubInfos;            /*!< liste des nom de rubrique avec leur type marche pas en static au linkage ==> G_mapNameRubType */
    QMap<QString, CMDI_Observation*> m_mapNameRubPtr;

    //................. menu hierarcque a structure de .ini .....................................
    //------------------------------------ addPopupHierarchique --------------------------------------------------
    /*! \brief Ajoute le menu de selection du type Hierarchique a un menu quelconque a partir d'un fichier
     *  \param path :        QString qui contient le chemin du fichier .ini structurant les options du menu
     *  \param pQPopupMenu : QPopupMenu* est le menu auquel rajouter ce menu hierarchique
     *  \param pRetVar :     QString* est un pointeur sur la chaine de caractre dans laquelle retourner l'option selectionnee
     *  \note  avant appel de cette fonction il convient de la connecter comme suit a
     *                                un SLOT a actionner lors selection d'une option:
     *                     connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
     *         puis dans le slot de la deconnecter :
     *                  disconnect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
     *  LIMITATION : il ne peut y avoir qu'un menu de ce type dans un menu principal
    */
    void addPopupHierarchique(const QString& path, QPopupMenu* pQPopupMenu, QString* pRetVar);
    //------------------------------------ addPopupHierarchique --------------------------------------------------
    /*! \brief Ajoute le menu de selection du type Hierarchique a un menu quelconque a partir d'un fichier
     *  \param path :        QString qui contient le chemin du fichier .ini structurant les options du menu
     *  \param optionsList : QStringListliste des options du menu
     *  \param pRetVar :     QString* est un pointeur sur la chaine de caractre dans laquelle retourner l'option selectionnee
     *  \param subName :     QString& chaine de caractre du nom du sous menu
     *  \note  avant appel de cette fonction il convient de la connecter comme suit a
     *                                un SLOT a actionner lors selection d'une option:
     *                     connect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
     *         puis dans le slot de la deconnecter :
     *                  disconnect ( G_pCApp, SIGNAL(Sign_popup_HierarchOptionSelected()) , this, SLOT(Slot_OnMenuActionSelected()));
     *  LIMITATION : il ne peut y avoir qu'un menu de ce type dans un menu principal
    */

    void addPopupHierarchique(QStringList optionsList, QPopupMenu* pQPopupMenu, QString* pRetVar, const QString &subName, int *pMenuPrincipalID=0);
    QMap<int,ThemePopup* > m_PopupHeadMap;                /*!< liste des menus principaux lors des menus type [section] */
    QString               *m_pOptionHierarch_Selectionne; /*!< pointeur sur la variable de retour de l'option selectionnee a renseigner */
   //......................... contexte ......................................................
    // QString              m_dumy;
    QString              m_NUM_VERSION;          /*!< stockera le numéro de version */
    QString              m_PathGlossaireIsLocal; /*!< determine si le Glossaire doit �tre local */
    QString              m_PathGlossaire;        /*!< Chemin vers le Glossaire */
    QString              m_PathAppli;                /*!< Chemin vers l'appli en cours */
    QString              m_PathTerrain;                /*!< Chemin vers le répertoire Terrain */
    QString              m_PathImages;                /*!< Chemin vers le répertoire de sauvegarde des images */
    QString              m_PathCfg_Base;        /*!< Chemin vers le fichier de configuration de la base */
    QString              m_PathTheme;                /*!< Chemin vers le Thème à utiliser */
    //.................... fichiers de configuration .........................................
    QString              m_PathDrTuxIni;        /*!< Chemin du fichier drtux.ini */
    //..................... indentification utilisateur .....................................
    QString              m_User;                /*!< Login de l'utilisateur en cours */
    QString              m_SignUser;            /*!< Login de l'utilisateur signataire */
    //..................... identification dossier en cours .................................
    QString              m_ID_Doss;             /*!< Pk de l'index nom prenom du dossier en cours */
    QString              m_NumGUID;             /*!< GUID du dossier en cours */
    QString              m_DossPrenom;          /*!< Préom du dossier en cours */
    QString              m_DossNom;             /*!< Nom du dossier en cours */

    QString              m_UserDataParam;       /*!< parametres utilisateurs */
    QString              m_DrTuxParam;          /*!< variable de configuration locale du .ini */
    QString              m_CriptedPassWord;        /*!< Mot de passe crypté */
    QString              m_Droits;                /*!< Droits de l'utilisateur */
    QString              m_MessVisuDocNotPossible;        /*!< non documenté */
    QString              m_LastError;                /*!< variable de configuration locale du .ini */
    bool                 m_IsAppStartWithUser;        /*!< Appli démarrée avec un utilisateur oui/non */
    int                  m_TimerVerrouDelay;        /*!< Temps de verrouillage */
    bool                 m_IsNomadeActif;       /*!< flag indiquant si le mode nomade est actif */
    //.................... Configuration Pour le Terrain ...........................
    // Trie
    int                  m_Atcd_SortBy;         /*!< Utilisé par la classe Atcd_Code pour méthode de trie. \sa enum SortBy */
    int                  m_Var_SortBy;        /*!< Utilisé par la classe Var_Code pour méthode de trie. \sa enum SortBy */
    int                  m_VarValue_SortBy;        /*!< Utilisé par la classe Var_Code pour méthode de trie. \sa enum SortBy */
    // Objets du terrain
    Atcd_Code*           m_pAtcd_Code;        /*!< Objet de gestion du terrain pour les antécédents */


    //................. Theme .................................
    Theme                m_Theme;                /*!< Pointeur vers le thème (icones, choix des couleurs et fontes) */
    QIconSet             m_IconTherapeutique;   /*!< Icone pour la thérapeutique selon la base médicaments */

    //.................. prefixage des types de documents prescriptifs avec le type de droit .......................
    //                                key               value
    //  #define TYP_ORDO_CALC      20020100              or
    //  #define TYP_ORDONNANCE     20020200              ot
    //  #define TYP_CERTIFICAT     20020300              ce
    //  #define TYP_COURRIER       20020500              co

    QMap <QString, QString> m_MapMenuRubriques;           /*!< associe clef : nom de la rubrique avec liste du menu fixe pour cette rubrique */
    QMap <QString, QString> m_mapDroitPrefix;             /*!< associe clef : nom de la rubrique avec prefix du droit */

    VAR_MAP                m_VAR_MAP;                     /*!< Pour la gestion des variables de script */
    CCoolPopup            *m_pCCoolPopup;                 /*!< Pour afficher un message en popup surgissant en bas à droite */
    int                    m_CCoolPopupTempo;             /*!< temps de persistance du menu à l'écran en microsecondes */
    C_ImageList            m_C_ImageList;                 /*!< Pour la gestion des documents (CMDI_Documents) */
//......................... carte PS .............................
#ifdef SESAMVITALE_VERSION
    C_Cps  *m_pCps;
#endif
private:
    ThemePopup          *m_Popup;
    QString              m_LastMainOption;
    int                  m_IdMenu;
    QString              m_SelectedFile;

    // Map the letters which have accents and the letters which haven't.
    QMap <ushort, char>  m_AccentDic;                    /*!< Pour la gestion des accentuees */

public slots:
     void  CouCouUp(   CCoolPopup * pCCoolPopup);
     void  CouCouStop( CCoolPopup * pCCoolPopup);
     void  CouCouDestroy();
     void  Slot_popup_HierarchOptionSelected();
     void  Slot_popup_HierarchActivated(int id);

signals:
     void  Sign_popup_HierarchOptionSelected();
     void  Sign_QuitterRequired();
};

extern CApp* G_pCApp;                                           /*!< Instance globale de l'application */
#endif

