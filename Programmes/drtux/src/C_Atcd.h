//
// C++ Interface: C_Atcd
//
// Description:
//
//
// Author: Eric MAEKER eric.maeker@free.fr, (C) 2008
// Author: Roland SEVIN roland-sevin@medintux.org (C) 2009 2010
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef ATCD_CODE_H
#define ATCD_CODE_H

#include <qstring.h>
#include <qstringlist.h>
#include <qdatetime.h>
#include <qvaluelist.h>
#include <qobject.h>
#include <qwidget.h>

#include <qlistview.h>

#include "CMoteur_Base.h"
#include "../../MedinTuxTools/CPrtQListViewItem.h"
#include "../../MedinTuxTools/CGenTools.h"
#include "../../MedinTuxTools/Theme.h"

#include "ui/DlgAtcd_txt.h"
#include "ui/Dlg_Calendar.h"
#define  TR        QObject::tr


#define LV_NAME 0
#define LV_TYPE 1
#define LV_DDEB 2
#define LV_DFIN 3
#define LV_COMM 4
#define LV_ALSP 5
#define LV_CODE 6

//class QObject;

/**
        @author Sevin Roland <roland-sevin@medintux.org>
*/

/*! \class Atcd_Element
 *  \brief Classe Atcd et C_Atcd : Deux classes qui gèrent les ATCD pour toute l'application. Il suffit d'injecter le fichier de Terrain et le reste suit.
 *  Cette classe contient les items de chaque ATCD :
 *  - libellé
 *  - date debut
 *  - code CIM10
 *  - rubrique
 *  - commentaire
 *  - date fin
 */

class Atcd_Element
{
public :
        Atcd_Element()
                { m_Rubrique    = "";
                  m_Libelle     = "";
                  m_Code        = "";
                  m_DateDeb     = QDate();
                  m_DateFin     = QDate();
                  m_Commentaire = "";
                  m_Id_ATCD     = -1;
                  m_ald         = "";
                }

        Atcd_Element(const char*           rubrique,
                     const char*           libelle,
                     const QDate&          dateDeb,
                     const char*           code,
                     const QDate&          dateFin,
                     const char*           commentaire,
                     const QString&        ald_Sport,
                     int                   id_ATCD=-1)
                { set_Atcd_Element(rubrique, libelle,  dateDeb,  code, dateFin,  commentaire, ald_Sport, id_ATCD);
                }

        ~Atcd_Element() {}
        void set_Atcd_Element(const char*           rubrique,
                              const char*           libelle,
                              const QDate&          dateDeb,
                              const char*           code,
                              const QDate&          dateFin,
                              const char*           commentaire,
                              const QString&        ald_Sport,
                              int                   id_ATCD)
                { m_Rubrique    = rubrique;
                  m_Libelle     = libelle;
                  m_Code        = code;
                  m_DateFin     = dateFin;
                  m_DateDeb     = dateDeb;
                  m_Commentaire = commentaire;
                  m_ald         = ald_Sport;
                  m_Id_ATCD     = id_ATCD;
                }

        // Implémentation de l'opérateur inférieur pour accéder aux fonction qHeapSort
        // Trie selon le nom de la rubrique
        bool operator <  (const Atcd_Element& e) const ;
        bool operator == (const Atcd_Element& e) const ;
        bool                 is_ATC();
        bool                 isAllergie();
        bool                 isCIM10();
        bool                 isCisp();

static  bool                 is_ATC(const QString &code);
static  bool                 isAllergie(const QString &code);
static  bool                 isCIM10(const QString &code);
static  bool                 isCisp(const QString &code);
        //bool                 isVidal();
        QString              getLightCIM10();
        QPixmap              getPixmap();
static  QPixmap              getPixmap(const QString &code);
        void                 setId            (int n)        { m_Id_ATCD = n ;   }
        int                  getId()                         { return m_Id_ATCD; }
        QString              getAld()                        { return m_ald;     }
public:
        QString              m_Libelle;                 /*!< libellé de l'ATCD */
        QString              m_Rubrique;                /*!< Rubrique de l'ATCD */
        QDate                m_DateDeb;                 /*!< Date de l'ATCD */
        QString              m_Code;                    /*!< codage CIM10, allergie */
        QString              m_Commentaire;             /*!< Commentaire de l'ATCD */
        QString              m_ald;                     /*!< si est en rapport avec l'ALD du patient */
        QDate                m_DateFin;                 /*!< Passé(P) ou Actif(A) ou date*/
        int                  m_Id_ATCD;                 /*!< Id de l'ATCD défini lors de la lecture du terrain, non modifié par la suite */

};
// Définition d'un alias vers une liste d'antécédents
typedef QValueList<Atcd_Element> ATCD_LISTE;



class Atcd_Code : public QObject
{
  Q_OBJECT

public:
   enum changeMode {
        sendNotModifMessage = 0,
        sendModifMessage    = 1

    };


    Atcd_Code(CMoteurBase  *pCMoteurBase, QWidget *parent=0, const char *name=0);
    ~Atcd_Code();

        ////////////////////////////////////////////
        ///////////////  METHODES  /////////////////
        ////////////////////////////////////////////

        bool                isModified();
        void                setModifiedState( bool state );

        // RECUPERATION DU TERRAIN
        void                setTerrain(CRubRecord* pCRubRecord, CMoteurBase* pCMoteurBase);
        int                 setTerrain  (RUBREC_LIST::iterator it, CMoteurBase* pCMoteurBase);

        // RECUPERATION DES ELEMENTS
        void                 createAtcdListView                   ();
        void                 createAtcdListView                   (CPrtQListViewItem& retour);
        void                 atcd_Element_To_ListViewItem(Atcd_Element& pAtcd_Element, QListView* pQListView);
        void                 atcd_To_ListView                     (QListView& retour);
        QString              get_ATCD_Rubname                     (Atcd_Element& pAtcd);
        //bool                 getIdRubrique(Atcd_Element& pAtcd, bool& isMed, bool& isChir, bool& isAllergie, int& id, QString  *pRubname  = 0 );
        //QString              getRubriqueName_Medical              (int id);
        //QString              getRubriqueName_Chirurgical          (int id);
        QDate                getDDR                               ();
        QString              getTermeObstetrical                  (QDate dt = QDate());
        Atcd_Element*        getAtcd_byID                         (int idAtcd);
        QString              getPkDocTerrain                      ();
        CPrtQListViewItem*   getListViewItemFromAtcd(Atcd_Element* pAtcd);
        Atcd_Element*        get_lastElementAdded();
        QString              ATCD_To_Text(  const QString &typePrefix = "<b>°&nbsp;<u>::TYPE:: :</u></b>",
                                            const QString &linePatern = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;::LIBELLE:: ::NOTE:: ::ALD::  ::DATE::",
                                            const QString &datePrefix = "en date du : ",
                                            const QString &aldPrefix  = "ALD");
        QString              GetSexe_FromBD();
        QChar                GetSexe_FMI_FromBD();
        QChar                GetSexe();

        //------------------------------ datesDebFinToPixmap --------------------------------------------------
        /*! \brief retourne le type d'icone correspondant aux dates de debut et fin
         *  \param dateDeb : date de debut
         *  \param dateDeb : date de fin
         *  \return QPixmap correspondant (actif inactif)
        */
        static QPixmap datesDebFinToPixmap(const QDate &dateDeb, const QDate &dateFin);
        //------------------------------ ald_sport_codeToPixmap --------------------------------------------------
        /*! \brief retourne le type d'icone correspondant au code ald_sport
         *  \param ald_sport_code : code ald_sport 'ALD' 'Sport' ''
         *  \return QPixmap correspondant (icone ALD ou sport)
        */
        static QPixmap ald_sport_codeToPixmap(const QString & ald_sport_code);

        //------------------------------ setDatesToNewVersion --------------------------------------------------
        /*! \brief converti l'ancien format (date debut, etat) d'un atcd en s'assurant que les dates de fin et debut soient correctes.
         *  \param dateDeb = texte de la date de debut au format dd-MM-yyyy
         *  \param dateFin = texte de la date de fin   au format dd-MM-yyyy ou 'Passe' ou 'Actif'
         *  \param dt_deb  = date de debut  calculee en fonction de elements fournis par dateDeb et dateFin
         *  \param dt_fin  = date de fin    calculee en fonction de elements fournis par dateDeb et dateFin
        */
        static void setDatesToNewVersion(QString &dateDeb, QString &dateFin, QDate &dt_deb, QDate &dt_fin);

        //--------------------------------- valid_DateStringOrEmptyString -------------------------------------------------------------
        /*! \brief retourne une chaine vide si date invalide sinon une de la forme "dd-MM-yyyy"
         *  \param date_in = date dont il faut tester la validite
         *  \param format  = format de retour
         *  \return QString qui est vide si date invalide sinon la date avec le format d'entree
        */
        static QString valid_DateStringOrEmptyString(QString date_in, const QString &format = "dd-MM-yyyy"  );
        //------------------------------ setNewState_FromOldState --------------------------------------------------
        /*! \brief l'etat ancien stocké est 'Passé' 'Actif' le nouveau devant etre une date qui si invalide l'atcd est actif si valide il est termine a la date présente
         *  \param old_state = ancien etat a interpreter (il tient compte d'un eventuel etat date deja positionne)
         *  \param date_to_set = pointeur sur un QDate qui si != zero sera positionne sur la date de fin de l'atcd (nouvelle valeur de l'etat)
         *  \return QString qui est le nouvel etat sous forme nouvelle (date valide ou invalide) selon actif ou inactif
        */
        static QString  setNewState_FromOldState(const QString &old_state, QDate *date_to_set=0);
        // FONCTIONS DE TRI
        void                 setSort(int mode);
        void                 sort();

        // AJOUTS / MODIFICATIONS
        void                viewATCD                  (QWidget *parent, int id_ATCD);
        //------------------------------ modifyDateDeb --------------------------------------------------
        /*! \brief Affiche le widget calendar pour modifier la date de debut de l'antécédent passé en paramètre.
        */
        void                modifyDateDeb                (QWidget *parent, Atcd_Element* pAtcd,  int sendModifMessage = Atcd_Code::sendModifMessage);
        //------------------------------ modifyDateFin --------------------------------------------------
        /*! \brief Affiche le widget calendar pour modifier la date de fin de l'antécédent passé en paramètre.
        */
        void                modifyDateFin                (QWidget *parent, Atcd_Element* pAtcd,  int sendModifMessage = Atcd_Code::sendModifMessage);
        void                addATCD_Textuel           (QWidget *parent, int sendModifMessage = Atcd_Code::sendModifMessage);
        void                addATCD                   (QWidget *parent, const QString &libelle, const QString &code, const QString &familyGenre, const QString &comment, int sendModifMessage  = Atcd_Code::sendModifMessage );
        void                modifyAtcd                (QWidget *parent, Atcd_Element* pAtcd,  int sendModifMessage = Atcd_Code::sendModifMessage);
        //------------------------------ setDateDeb --------------------------------------------------
        /*! \brief modifie la date de debut et emet le signal de changement.
        */
        void                setDateDeb                (Atcd_Element* pAtcd,  const QDate &date,  int sendModifMessage = Atcd_Code::sendModifMessage);
        //------------------------------ setDateFin --------------------------------------------------
        /*! \brief modifie la date de fin et emet le signal de changement.
        */
        void                setDateFin                (Atcd_Element* pAtcd,  const QDate &date,  int sendModifMessage = Atcd_Code::sendModifMessage);
        //------------------------------ deleteDateDeb --------------------------------------------------
        /*! \brief efface la date de debut de l'atcd passe en parametre.
        */
        void                deleteDateDeb(Atcd_Element* pAtcd,  int sendModifMessage = Atcd_Code::sendModifMessage);
        //------------------------------ deleteDateFin --------------------------------------------------
        /*! \brief efface la date de fin de l'atcd passe en parametre.
        */
        void                deleteDateFin(Atcd_Element* pAtcd,  int sendModifMessage = Atcd_Code::sendModifMessage);
        //------------------------------ changeDateFin --------------------------------------------------
        /*! \brief Change la date de fin de l'antécédent, passé en paramètre.
        */
        void                changeDateFin             (QWidget *parent, Atcd_Element* pAtcd);
        //void                setEtat                   (Atcd_Element* pAtcd,  QString etat,  int sendModifMessage = Atcd_Code::sendModifMessage);
        void                changeCommentaire         (Atcd_Element* pAtcd,  int sendModifMessage = Atcd_Code::sendModifMessage);
        void                eraseAtcd                 (Atcd_Element* pAtcd,  int sendModifMessage = Atcd_Code::sendModifMessage);
        void                setRubrique               (Atcd_Element* pAtcd, QString& rubrique,   int sendModifMessage = Atcd_Code::sendModifMessage);
        void                setALD                    (Atcd_Element* pAtcd, const QString& ald,  int sendModifMessage = Atcd_Code::sendModifMessage);
        void                setGrossesse              (bool etat);
        void                setAllaitement            (bool etat);
        void                setDDRTo                  (QDate dt);
        void                clear                     ();
        void                updateFromListViewTerrain(QListView* pQListView, int sendModifMessage = Atcd_Code::sendModifMessage);
        // GESTION DE LA SAUVEGARDE LECTURE
        QString             updateToBD();
        QString             updateFromBD();
        QString             toOldIni();
        void                setTerrain(const char* strTerrain, CMoteurBase* pCMoteurBase);


        ////////////////////////////////////////////
        ///////////////   SLOTS    /////////////////
        ////////////////////////////////////////////

public slots:
        // AJOUTS / MODIFICATIONS
        void                emitATCD_Changed        ();
        void                addATCD_CIM10           (int tabToSet=-1);
        void                addATCD_Allergie        ();
        void                changeDDR               (QWidget *parent);



        ////////////////////////////////////////////
        ///////////////   SIGNALS  /////////////////
        ////////////////////////////////////////////

signals:
        void                ATCD_Changed();                /*!< \brief Signal émit �  chaque modification des Antécédents. Attention ne modifie pas la propriété m_TerrainIsModified avant. Pour cela il faut appeler le Slot emitATCD_Changed(). */
private:
Atcd_Element*       m_pLastElementAdded;                   /*!< \brief copie du dernier element ajouté. */
CMoteurBase        *m_pCMoteurBase;

        ////////////////////////////////////////////
        ///////////////   DONNEES  /////////////////
        ////////////////////////////////////////////

public :

        QString            m_Terrain;                    /*!< Le fichier terrain �  analyser */
        ATCD_LISTE         m_Atcd_Liste;                 /*!< Liste des ATCD */
        bool               m_Grossesse;                  /*!< Contient TRUE si grossesse en cours. */
        QDate              m_DDR;                        /*!< Date des dernieres règles. */
        bool               m_Allaitement;                /*!< Contient TRUE si allaitement en cours. */
        QString            m_PkDocTerrain;               /*!< Contient le PkDoc du Terrain en cours, utile pour tester la synchronisation des terrains */
        bool               m_TerrainIsModified;          /*!< Se modifie en cas de modification des antécédents */
        QListView*         m_pAtcdFatherListView;        /*!< ListView qui affichera le menu rapide*/
        CPrtQListViewItem* m_pAtcdListViewItem;          /*!< ListViewItem qui affichera les atcd Allergiques Cim10 et textuels*/
        QChar              m_sexe;                       /*!< sexe du patient */
};

#endif
