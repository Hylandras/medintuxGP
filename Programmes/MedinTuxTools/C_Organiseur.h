// C++ Implementation:
/********************************* C_Organiseur.h *************************

    Project: MedinTux
    Copyright (C) 2005 by Roland Sevin  and Data Medical Design
    E-Mail: roland-sevin@medintux.org

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


#ifndef C_Organiseur_H
#define C_Organiseur_H

#include "ui/Form_Organiseur.h"

#include <qobject.h>
#include <qlistview.h>
#include <qsplitter.h>
#include <qdatetime.h>

#include "../drtux/src/CRubList.h"
#include "../../../MedinTuxTools/CPrtQListViewItem.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thÃšme de l'appli

#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))

#define GET_PK_DOS get_Prop_2
#define SET_PK_DOS set_Prop_2

#define GET_GUID get_Prop_3
#define SET_GUID set_Prop_3

#define GET_PK_DOC get_Prop_1
#define SET_PK_DOC set_Prop_1

#define GET_PK_EVNT get_Prop_0
#define SET_PK_EVNT set_Prop_0

#define IS_CLASSEUR 1


class Dlg_Calendar;
class C_FormGlossaire;
//===============================================================  C_PatientRootItem =========================================================================
/*! \class C_PatientRootItem
 *  \brief Classe de données pour stocker les classeurs racine de la vigie
*/
class C_PatientRootItem
       {
       public:
        C_PatientRootItem(){};
        C_PatientRootItem(const char *nom, const char *prenom, const char *guid, const char *pkDoss, /*, const char *age,*/ QListView *pQListView)
            {m_pC_DragQListViewItem = new C_DragQListViewItem(pQListView);
             m_Name                 = nom;
             m_Prenom               = prenom;
             m_guid                 = guid;
             m_pkDoss               = pkDoss;
             //m_Age                  = age;
             if (m_pC_DragQListViewItem)
                {m_pC_DragQListViewItem->setText(0, m_Name + " " + m_Prenom);
                 //m_pC_DragQListViewItem->setText(1, prenom);
                 m_pC_DragQListViewItem->SET_PK_DOS(pkDoss);
                 m_pC_DragQListViewItem->SET_GUID(guid);
                 m_pC_DragQListViewItem->SET_PK_EVNT("RO");
                 m_pC_DragQListViewItem->setOpen(TRUE);
                 m_pC_DragQListViewItem->setPixmap ( 0, Theme::getIcon("16x16/identity.png") );
                }
            }
        QString               m_Name;
        QString               m_Prenom;
        QString               m_guid;
        QString               m_pkDoss;
        //QString               m_Age;

        C_DragQListViewItem  *m_pC_DragQListViewItem;
       } ;

typedef QMap<QString, C_PatientRootItem> PK_PATIENT_MAP;
//===============================================================  C_VigieListView =========================================================================
/*! \class C_VigieListView
 *  \brief Widget QListView pour afficher les différents classeurs de la vigie
*/
class C_VigieListView : public C_DragQListView
{
    Q_OBJECT
public:
    C_VigieListView( QString *ptr, QWidget *parent = 0, const char *name = 0 );
    C_VigieListView( QWidget *parent = 0, const char *name = 0 );
   ~C_VigieListView(){};

  /*! \brief Retrouve un patient racine à partir de son Pk.
 * cette fonction examine la QMap des items racine : PK_PATIENT_MAP m_PatientPk.
 * et si le pk du dossier de ce patient existe il est retourné un pointeur sur
 * une pointeur de la classe : C_PatientRootItem qui donne les infos sur ce patient.
 * \code C_PatientRootItem  *pC_PatientRootItem   =  GetPatientRootItem( dossPk ); \endcode
 * \sa   C_VigieListView()
 * \param pk_Doss const QString & .
 */
    C_PatientRootItem  *GetPatientRootItem(const QString &pk_Doss);
    C_PatientRootItem  *CreatePatientRootItem( const QString &pk_Doss, bool mustBeOpenClose=TRUE);
    C_PatientRootItem  *FindOrCreatePatientRootItem(const QString &pk_Doss, bool mustBeOpenClose = TRUE);
    PK_PATIENT_MAP      m_PatientPk;
public slots:
    virtual void clear();
//........................ kit de connexion au glossaire ............................................
public:
    void    mouseEnter ();
    void    DoConnexionOnGossaire(C_FormGlossaire *pC_FormGlossaire);
    void    initialisePathGlossaire(const QString &pathDocuments, const QString &defaultMaskDirectory);

    QString m_PathDocuments;
    QString m_DefaultMaskDirectory;

protected:
    void    mouseLeave ();
    bool    eventFilter ( QObject * o, QEvent * e );
signals:
    void Sign_MouseEnter(QString & ,  QString & );
    void Sign_MouseLeave(QString & ,  QString & );
public slots:
    virtual void On_DirectoryGlossaireChanged(const QString &pathDocuments, const QString &defaultMaskDirectory);

};

typedef QMap<QString, QString> OpClProfiles;

//===============================================================  C_Organiseur =========================================================================
/*! \class C_Organiseur
 *  \brief Formulaire de la vigie
*/
class C_Organiseur : public Form_Organiseur 
{
    Q_OBJECT

 public:

     enum Hierarch_Mode { justtDir       = 1,
                          withFileName   = 0
                        };
     enum toExist_Mode  { toDelete       = 1,
                          toKeep         = 0
                        };

     //................... construct destruct ..............................
     C_Organiseur(EVNT_LIST *pEvnmtList, RUBREC_LIST *pRubList, const QString &dossPk, QWidget * parent = 0, const char * name = 0, WFlags f = 0);
    ~C_Organiseur();
     //................. Methodes ..........................................
     /*! \brief Fonction qui remplie les colonnes d'un item de la listview avec un enregistrement de la liste d'evennements pEvnmtList
     *   \param pQListViewItem  pQListViewItem* :  un pointeur sur l'item de la listview a renseigner
     *   \param pCRubRecord     CRubRecord* :  un pointeur sur l'enregistrement la liste des documents avec lequel renseigner l'item
     *   \param pCRubEvntPair   CRubEvntPair* :  un pointeur sur  l'enregistrement de la liste d'evennements avec lequel renseigner l'item
     *   \param pQListViewItemPere   C_DragQListViewItem : sur l'item du pere
     *   \return neant
     */
     void                 SetListViewItemFromRecord( C_DragQListViewItem *pQListViewItem, CRubRecord *pCRubRecord , CRubEvntPair *pCRubEvntPair);
     C_DragQListViewItem *GetListViewItem_FromPkEvnt(const QString &evntPk);
     C_DragQListViewItem* GetListViewItemFromPkDoc( const QString &pk);
     C_DragQListViewItem* GetListViewItemROOTFromPkDoss( const QString &pk);
     int                  GetPreselectDateList(QStringList & rQStringList);
     void                 Dlg_DateInit(Dlg_Calendar *pDlg_Calendar);
     int                  testErrorDate(int & isInvalid, const QString &date, const QString &heure, const QString & titre);
     int                  findOpenCloseProfile(const QString &name);
     QString              QueryDateHeureToNatural(const QString &date);
     QString              NaturalDateHeureToQuery(const QString &date);
     void                 CouCou(const QString &mess);
     void                 DoConnexionOnGossaire(C_FormGlossaire *pC_FormGlossaire);
     //QTime                hhmmssToQTime(  const QString & heure );
     QDate                ddMMyyyyToQDate(const QString & date );
     void                 Set_ZoomFontSize(int size);
     void                 initialisePathGlossaire(const QString &pathDocuments, const QString &defaultMaskDirectory);

     /*! \brief Retrouve un patient racine à partir de son Pk.
     * cette fonction examine la QMap des items racine : PK_PATIENT_MAP m_PatientPk.
     * et si le pk du dossier de ce patient existe il est retourné un pointeur sur
     * une pointeur de la classe : C_PatientRootItem qui donne les infos sur ce patient.
     * \code C_PatientRootItem  *pC_PatientRootItem   =  GetPatientRootItem( dossPk ); \endcode
     * \sa   C_VigieListView()
     * \param pk_Doss const QString & .
     */
     C_PatientRootItem*   GetPatientRootItem(const QString &pk_Doss);
    //................. profils ...........................................
     int                pushButton_SetIcone_clicked(QString &icone);
     QString            loadProfile(const QString &path, int just_profile  = 0);
     void               saveProfile(const QString &path, int just_profile  = 0);
     void               saveUserProfil();
     QString            GetProfile();
     QString            GetFilter();
     int                SetProfile(QString xml);
     int                SetFilter(QString xml);
     void               setProfileValue(const QString &name, const QString &xmltag, const QString & value);
     QString            getProfileValue(const QString &name, const QString &xmltag);
     void               setFilterValue(const QString &name, const QString &xmltag, const QString & value);
     QString            getFilterValue(const QString &name, const QString &xmltag);
     void               delFilter(const QString &filterName);
     void               setItemOnProfile(C_DragQListViewItem *pQListViewItem);
     QString            setMenuList(QString classeurName,    QString property);
     QString            removeMenuList(QString classeurName, QString property);
     void               clearListViewHeaderOnItem();
     void               setListViewHeaderOnItem(C_DragQListViewItem *pQListViewItem);
     int                isThisHierarchAuthorized(const QString &hierarch );
     void               setIniParamFromContext(QString &param);
     void               SetInterfaceOnDroits(const QString &droits);
     void               ChangeDateTime(C_DragQListViewItem *pC_DragQListViewItem, CRubRecord *pCRubRecord, const QString &debOrEnd);

     /*! \brief Fonction qui verifie si le chemin donne en entree correspond a une hirarchie de documents de l'organiseur
      *  \param hierarch : QString de la hierarchie a chercher (termes separes par /)
     */
     CRubEvntPair   *isThisHierarchieExist( QString & hierarch );

     /*! \brief Fonction qui place tous les descendant d'un item et les document allant avec en mode effacement ou non effacement
      *  \param pk_Pere  : QString  clef primaire du pere dans a hierarchie dont il faut positionner les enfants
      *  \param state  : int  si C_Organiseur::toDelete placer le mode effacement,  C_Organiseur::toKeep oter le mode effacement
      *  \return  zero
     */
     int                Evnmt_Set_StateOn_AllFilsAndDoc(QString pk_Pere, int state);
     //............... list version ........................................
     void               makeListeMonitorFromList (EVNT_LIST *pEvnmtList, RUBREC_LIST *pRubList, bool clearListView  = TRUE  );
     int                GetFilsFromPereInList(EVNT_LIST *pEvnmtList, RUBREC_LIST *pRubList, const QString &pk_pere, C_DragQListViewItem *pQListViewItemPere , const QString &hierarch);
     //............... sql version .........................................
     void               makeListeMonitorFromSQL(const QString &filtre, const QDateTime &dateTimeDeb, const QDateTime &dateTimeFin);
     CRubRecord         queryToRecord(QSqlQuery &query);
     CRubEvntPair       queryToEvnt(QSqlQuery &query);
     QListViewItem     *GetEvenementsFilsFromSQL(C_DragQListViewItem *pQListViewItem, const CRubEvntPair &crpair, const QString &headerRequete, const QString &hierarch);
     QString            MakeSQLFieldSelect();
     //.......................... methodes .................................
     int                  GetXMLValue(const QString &dataToExamine, const QString &xmlToken_in, QString &valueToGet, int pos  = -1 );
     void                 displayWarningItemInEraseState();
     /*! \brief Fonction qui retourne la hierarchie d'un item de la listview
      *  \param pC_DragQListViewItem  : C_DragQListViewItem*  un pointeur sur l'item de la listview dont on recherche la hierarchie
      *  \param pC_DragQListViewItem  : C_DragQListViewItem*  un pointeur sur l'item de la listview dont on recherche la hierarchie
      *  \return  QString de la hierarchie  (termes separes par /)
     */
     QString              GetItemHierarchie(C_DragQListViewItem *pC_DragQListViewItem, int justtDir = C_Organiseur::withFileName);
     /*! \brief Fonction qui ajoute un document medintux existant dans l'organiseur
      *  \param pCRubRecordToAdd  : CRubRecord*  un pointeur sur l'enregistrement à rajouter
      *  \param int justtDir = C_Organiseur::withFileName  : si C_Organiseur::withFileName si le dernier element du chemin est un fichir il est conservé C_Organiseur::justtDir le chemin n'est composé que des repertoires
      *  \param hierarch : QString de la hierarchie ou placer le documment (termes separes par /)
     */
     QString              Append_Doc_In_Hierarchie(CRubRecord* PCRubRecord, QString hierarch, int ifNoHierarchAppendDoc=0);
     /*! \brief Fonction qui recherche dans les classeurs racines, un ouvert (date de fin non fermee) avec le meme libelle
      *  \param PCRubRecord CRubRecord*  un pointeur sur l'enregistrement à rajouter
      *  \param hierarch QString hierarchie de classur ou devrait etre insere le document.
      *  \param ifNoHierarchAppendDoc int (valeur par defaut) si classeur non trouvé : si 0 le document n'est pas inséré si 1  le document est inséré
      *  \return CRubEvntPair* : pointeur sur l'enregistrement ou zero si pas trouve
     */
     CRubEvntPair*        GetRootEvntWithSameNameNameAndDate(RUBREC_LIST   *pRubList, const QString &libelle, const QString &date);
     /*! \brief Fonction qui explore la fratrie de la descendance (uniquement la fratrie pas les ils des fils etc...) d'un evenement à la recherche d'un nom semblable au libelle
      *  \param pRubList   RUBREC_LIST* :  un pointeur sur la liste des documents
      *  \param pEvntPere : CRubEvntPair* evenement pere dont il faut explorer la fratrie de sa descendance
      *  \param libelle : QString du nom dont il faut rechercher le meme element dans la fratrie
     */
     CRubEvntPair*        Find_Evnt_WhithSameName_InFratrie(RUBREC_LIST   *pRubList, CRubEvntPair* pEvntPere, const QString libelle);
     /*! \brief Fonction qui explore la descendance d'un evenement pour voir si il correspond a la hierarchie donnee en entree si trouve : m_pFindHierarchEvnt contient le pointeur de l'evenement final
      *  \param CRubEvntPair *hierarchEvntPere enregistrement du pere dont il faut explorer la descendance
      *  \param hierarch : QString de la hierarchie ou placer le documment (termes separes par /)
      *  \param date :  date du trux à placer (pour distinguer ceux avec la meme hierarchie mais de date differente)
      *  \param pos_deb : int de debut du terme pointe dans la hierarchie
      *  \param pos_end : int de fin du terme pointe dans la hierarchie
     */
     int                  Find_Evnt_In_Hierarchie(CRubEvntPair *hierarchEvntPere, const QString hierarch, const QDateTime &date, int pos_deb, int pos_end);
     /*! \brief Fonction qui positionne la liste sur un patient à partir de sa clef primaire
      *  \param dossPk clef primaire du patient sur lequel la liste doit se positionner
     */
     void    setListviewOnDossier(const QString &dossPk);
     //............... donnees .............................................
     EVNT_LIST     *m_pEvnmtList;
     RUBREC_LIST   *m_pRubList;
     QString        m_DossPk;
     QString        m_LastError;
     CRubEvntPair            *m_pFindHierarchEvnt;
     C_DragQListViewItem     *m_pFindHierarchItem;
     QPtrList<CRubRecord>     m_HierarchRubRecordList; /*!< stocke les documents liés à une hiérarchie utilisée dans : Append_Doc_In_Hierarchie() */
     C_VigieListView         *m_ListView_MonitorPatient;
     QString                  m_log;
protected:
    QSplitter       *m_splitter;
    QPixmap          m_ToDeletePixmap;
    QPixmap          m_ToDestroyPixmap;
    QPixmap          m_IsClasseurPixmap;
    QPixmap          m_IsMenuHerePixmap;
    QPixmap          m_DateDeb;
    QPixmap          m_DateFin;
    QString          m_PathProfiles;
    bool             m_IsDragedFile;
    QString          m_LastImgPath;            /*!< chemin des images  */
    QString          m_BaseProfils;            /*!< donn�es XML du profil local aux classeurs non li�s � l'utilisateur  */
    QString          m_Filtre;                 /*!< filtre d'affichage  */
    bool             m_hierarchFilter;
    //QString          m_TempHierarch;           /*!< hierarchie en cours  */
    QStringList      m_FiltreHierarchList;     /*!< filtre d'affichage n'autorisant que la hiérarchie indiquee dans cette liste  */
    OpClProfiles     m_UserClasseurProfiles;   /*!< stocke une paire : Nom d'un classeur, données XML de ce classeur  */
    //OpClProfiles     m_CommonClasseurProfiles; /*!< stocke une paire : Nom d'un classeur, données XML de base de ce classeur  */
    OpClProfiles     m_FilterProfiles;         /*!< stocke une paire : Nom d'un filtre, données texte de ce classeur  */
    int              m_ClasseursAllVisibles;   /*!< si à 1 : tous les classeurs sont visibles (administrateur seulement)  si 0 que si le profil le permet*/
public slots:
    void   On_but_ConfFiltre_HideShow();
    void   On_DragMoveEvent(QListViewItem *pQListViewItem, bool &dragBreak);
    void   On_IdentChange(const char* dossGUID,  const char* dossPk, const char* dossNom,  const char* dossPrenom);
    void   On_ItemClicked(QListViewItem *itemClicked, const QPoint & pnt, int c);
    void   On_Sign_ItemClicked(QListViewItem *itemClicked);
    void   On_ItemDraged_On_AnotherItem(QListViewItem *itemSrc, QListViewItem *itemDst,         const QString &verbe);
    void   On_FilesDraged_On_Item(QListViewItem *itemDst,             QStrList &fileList,       const QString &verbe);
    void   On_TextDraged_On_Item( QListViewItem *itemDst,       const QString  &dragedText,     const QString &verbe);
    void   On_ContextMenuRequested(QListViewItem *pQListViewItem , const QPoint &pos , int c);
    void               makeListeMonitor();
    void               makeListeMonitor(const QString &filtre);
    void   makeListeMonitorFromList();
    void   pushButtonDateDeb_clicked();
    void   pushButtonDateFin_clicked();
    void   On_but_ZoomPlus_clicked();
    void   On_but_ZoomMinus_clicked();
    void   On_but_ZoomDefault_clicked();
    void   On_but_SaveProfile_clicked();
    void   On_but_LoadProfile_clicked();
    void   On_checkBox_filtreActif_clicked();
    void   On_but_ConfFiltre_clicked();
    void   On_but_ConfFiltre_Plus_clicked();
    void   On_but_ConfFiltre_Moins_clicked();
    void   On_comboBox_FiltreVisu_activated(const QString &str);
signals:
    void Sign_ActiverDocument(QString, QString, int);
    void Sign_CreateNewRootInOrganiseur( QString);
    void Sign_DragFileInOrganiseur( QString, QString *);
    void Sign_IdentChangeInOrganiseur( const char*,  const char*, const char*,  const char*);
    void Sign_MouseEnter(QString &, QString &);
};

#endif
