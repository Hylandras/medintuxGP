/******************** C_Dlg_GestionATCD.h *****************************************
 *                                                                                *
 *   #include "C_Dlg_GestionATCD.h"                                               *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 ->2009 by Sevin Roland  and Data Medical Design *
 *   E-Mail: roland-sevin@medintux.org                                            *
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

#ifndef C_DLG_GESTIONATCD_H
#define C_DLG_GESTIONATCD_H
#include <qobject.h>
#include <qevent.h>
#include <qsqldatabase.h>
#include <qpixmap.h>
/*
    class KeyPressEater : public QObject
    {

    Q_OBJECT
    public:
       KeyPressEater(QWidget* parent = 0);
      ~KeyPressEater(){};
        bool eventFilter( QObject *o, QEvent *e );
    };

    bool KeyPressEater::eventFilter( QObject *o, QEvent *e )
    {
        if ( e->type() == QEvent::KeyPress ) {
            // special processing for key press
            QKeyEvent *k = (QKeyEvent *)e;
            qDebug( "Ate key press %d", k->key() );
            return TRUE; // eat event
        } else {
            // standard event processing
            return FALSE;
        }
    }

*/

 #include "ui/Dlg_GestionATCD.h"
class C_ListViewATCDManager;
class QPopupMenu;
class QFile;
class QPixmap;
/*! \class C_Dlg_GestionATCD
 * classe du formulaire de gestion des ATCD appelee par :  void Atcd_Code::addATCD_CIM10()
*/

class C_Dlg_GestionATCD : public Dlg_GestionATCD
{
  Q_OBJECT

public:
   C_Dlg_GestionATCD(int tab=-1, QString mode="ATCD", QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~C_Dlg_GestionATCD();

  /*$PUBLIC_FUNCTIONS$*/
    int             OpenBase();
    QString         Utf8_Query( QSqlQuery & cur, int field );
    QSqlDatabase*   BaseConnect( const char * driver, const char * DataBaseName, const char * user, const char * password, const char * hostname, QString port, QString * errMess );
    long            FiltrerListe( QListView * pQlistView, QString str1, QString str2);
    QListViewItem*  AddItemToListViewUrg( QListView * pQlistView, QString itemText );
    long            CIM10GetLibellesList( QListView * pQlistView, QString mot_cle_saisie1, QString mot_cle_saisie2, QLabel * statutMess, QLabel * errMess );
    long            CIM10GetRubriquesList( QListView * pQlistView, QLabel * statutMess, QLabel * errMess );
    long            CIM10GetSubLevelLibelleList( QListViewItem * qlistViewItem, QString ID_Father, QLabel * statutMess, QLabel * errMess );
    QString         SID_to_CIM10( QString qsSID, QLabel * errMess=0 );
    QListView*      GetDlgListViewFromTab(int tab_index);
    void            setLineEditFocusFromTab(int tab_index);
    void            initCisp();
    void            writeWindowPos();
    void            readAndSetWindowPos();
    QListViewItem*  GetDlgListCode( int tab_index, QListViewItem * qlistViewItem );
    void            AddItemToListChoix( const QString & libelle, const QString & code, QString family );
    void            setOpenCloseMode( int mode );
    void            CloseBase();
    void            setOnglet( int onglet );
    void            setListCode( const QString & str_listCode );
    void            dataBaseSet( QSqlDatabase * pQSqlDatabase );
    void            Load_HTML_Files();
    void            Load_HTML_Target_Page( const QString & target_page );
    void            initThesaurusListe();
    void            filtrerListeThesaurus(QListView *pQlistView);
    void            appendToThesaurus(const QString &libelle, const QString &code);
    void            add_popMenu_ATCD_Type(QPopupMenu* pQPopupMenu);
    QString         thesaurusQListViewItemToText(QListViewItem * pQListViewItem);
    void            listViewThesaurusToThesaurusList();
    void            initListChoixATCD();
    void            initComboFamilleGenre();
    void            initThesaurus();
    void            common_listView_doubleClicked( QListViewItem *pQListViewItem );
    QString         serialiseFavoris();
    void            unSerialiseFavoris(QString & data);
    QString         DoPopupList(QStringList &list, QWidget *parent);
    int             initTableCisp();
    QString         getCimxLibelleFromCimxCode(const QString &codeCimx, int stopToFirstResponse  = 0 );
    QString         getCispLibelleFromCispCode(const QString &codeCisp);
    int             fillCispItemWithAssociatedCimx( QListViewItem *pQTreeWidgetItem, const QString &libelleCisp, const QString &codeCisp);
    QString         getCispToCim10Str(const QString &codeCisp);
    QString         getCispChapitreFromCodeCisp(QString cispCode);
    void            setCispFilterFromCombosStates(const QString &chapiText, int classIndex);
    int             listView_Cisp_filter( const QString &filter_chapitre  ="-FDABHKLNPRSTUWXYZ"  ,
                                          const QString &filter_class     = "SINTCD" ,
                                          QListView     *pQTreeWidget     = 0 );
    QListViewItem  *getCispParentItemFromCodeAndClasse( QListView *pQTreeWidget,        // QListView ou doit se faire l'affichage des mentions cisp
                                                        const QString &cispCode,        // code cisp X70 A99 de la mention
                                                        int   i_class_code,             // classe de la mention (0-"SYMPTÔMES ET PLAINTES" 1-"INFECTION" ...)
                                                        const QString &filter_chapitre, // chaine du filtre d'entree : un ou plusieurs caracteres de "-FDABHKLNPRSTUWXYZ"
                                                        const QString &filter_class);    // chaine du filtre d'entree : un ou plusieurs caracteres de "SINTCD"
    unsigned long   readLine(QFile *pQFile, char *buffer, QString &outParam, unsigned long nbMax);
    QListViewItem*  findListViewItem(QListView* pQListView, int depth, const QString &text, int pos_text_in_item = 0);
    int m_TAB_LIBELLE;
    int m_TAB_RUBRIQUE;
    int m_TAB_ALLERGIE;
    int m_TAB_THESAURUS;
    int m_TAB_CISP;

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_DATAS*/
     QStringList            m_StringList;
     QString                m_DriverName;
     QString                m_BaseName;
     QString                m_UserName;
     QString                m_PassWord;
     QString                m_HostName;
     QString                m_Port;
     QSqlDatabase*          m_DataBase;
     QString                m_Code;
     QString                m_Libelle;
     QString                m_Comment;
     QString                m_Family;
     int                    m_CloseAfterQuery;
     QString                m_structStr;
     QStringList            m_ThesaurusList;
     QStringList            m_ATCD_FamilyList;
     QStringList            m_ATCD_GenreList;
     QListViewItem*         m_pQListViewItem;
     QString                m_TypeATCD_Selectionne;
     QString                m_SaveInitialState;
     C_ListViewATCDManager *m_pC_ListViewATCDManager;
     int                    m_IsModified;
     int                    m_IsModifiable;
     bool                   m_isCispInitialised;
     QString                m_Cisp_ClassCodes;
     // KeyPressEater         *m_keyPressEater;
     QMap<QChar, QString> m_CispChapitresMap;
     QStringList          m_CispClassesList;
     QPixmap              m_CispMiniPixmap;
     QPixmap              m_CimxMiniPixmap;
     QPixmap              m_CimxCodefinal;
     QPixmap              m_CimxChapitre;
     QListViewItem       *m_pLastIntolItem;
protected slots:
  /*$PROTECTED_SLOTS$*/
    void close();
    void Slot_checkBox_filter_chapitres_cimxChanged(int);
    void Slot_lineEditAutolcator_Cisp1_textChanged(const QString&);
    void Slot_lineEditAutolcator_Cisp2_textChanged(const QString&);
    void Slot_listView_Cisp_clicked(QListViewItem*);
    void Slot_listView_Cisp_doubleClicked(QListViewItem*);
    void Slot_comboBox_Cisp_filter_Chapi_highlighted(const QString &);
    void Slot_comboBox_Cisp_filter_Class_highlighted(int);
    void Slot_checkBox_Cisp_filter_cimx_stateChanged(int);

    void Slot_lineEditThesaurusFind1_F2_Pressed();
    void Slot_lineEditThesaurusFind1_F6_Pressed();
    void Slot_ThesaurusImport();
    void Slot_ThesaurusExport();
    void Slot_pushButtonFamilleGenreEdit();
    void Slot_pushButtonThesaurusAdd_clicked();
    void Slot_pushButtonThesaurusFreeAdd_clicked();
    void Slot_pushButtonAddFreeToChoix_clicked();
    void Slot_pushButton_AddToChoixFast_clicked();
    void Slot_pushButtonAddMedicament_clicked();
    void Slot_pushButtonThesaurus_Edit_clicked();
    void Slot_listViewAllergies_clicked(QListViewItem*);
    void Slot_listView_Produits_contextMenuRequested( QListViewItem *, const QPoint &, int  );
    void Slot_listViewAllergies_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &pt, int c );
    void Slot_lineEditAutolcatorLibelle_textChanged( const QString & );
    void Slot_lineEditAutolcatorLibelle_2_textChanged( const QString & );
    void Slot_listViewCim10_rubriques_clicked( QListViewItem * qlistViewItem );
    void Slot_tabWidgetDicoATCD_currentChanged( QWidget * pQWidget );
    void Slot_listViewCim10_Libelles_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &pt, int c );
    void Slot_listViewCim10_Libelles_doubleClicked( QListViewItem * qlistViewItem );
    void Slot_listViewCim10_rubriques_doubleClicked( QListViewItem * qlistViewItem );
    void Slot_listViewCim10_rubriques_contextMenuRequested( QListViewItem *pQListViewItem, const QPoint &pt, int c );
    void Slot_pushButton_AddToChoix_clicked();
    void Slot_pushButtonRemoveFromChoix_clicked();
    void Slot_comboBox_Family_highlighted( const QString& section );
    void Slot_combo_Family_activated(const QString   &text);
    void Slot_listViewThesaurus_doubleClicked( QListViewItem *qlistViewItem );
    void Slot_listViewThesaurus_contextMenuRequested( QListViewItem *qlistViewItem, const QPoint &pos, int c );
    void Slot_listViewThesaurus_returnPressed( QListViewItem * );
    void Slot_lineEditThesaurusFind1_textChanged(const QString &str);
    void Slot_lineEditThesaurusFind1_returnPressed();
    void Slot_menuActionSetFamilleGenre();
    void Slot_SaveThesaurus();
    void Slot_listViewThesaurus_itemRenamed ( QListViewItem * pQListViewItem , int col, const QString &text  );
    void Slot_ThesaurusDel();
    void Slot_lineEditNomAllergie_textChanged( const QString &qtext );
    void Slot_lineEditNomMedicament_textChanged(const QString &qtext);
    void Slot_listView_ATC_clicked(QListViewItem*);
    void Slot_listView_ATC_doubleClicked(QListViewItem*);
    void Slot_listView_Produits_clicked(QListViewItem *pQListViewItem);
    void Slot_listView_Produits_doubleClicked( QListViewItem *pQListViewItem );
    void Slot_listViewAllergies_doubleClicked( QListViewItem *pQListViewItem );
    void Slot_listViewCim10_Choix_doubleClicked( QListViewItem * );
    void Slot_comboBox_Genre_highlighted( const QString&  genre  );
    void Slot_combo_Genre_activated( const QString&  section );
    void Slot_reject();
    void Slot_accept();


};

#endif

