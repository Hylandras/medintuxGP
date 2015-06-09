/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSettings>
#include <QtSql>
#include <QMap>
#include <QApplication>
#include <QCoreApplication>
#include <QEventLoop>

#include "C_Wdg_Box.h"
#include "dlgbox.h"
#include "dlgetat.h"
#include "dlgresp.h"
#include "dlgtache.h"
#include "dlgparam.h"
#include "dlgdesti.h"
#include "ui_dlgdesti.h"

typedef QList<QObject*> QObjectList;

QT_BEGIN_NAMESPACE
class MdiBox;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QProcess;

class C_ClickableLed;

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void    closeEvent(QCloseEvent *event);
    QString makeRequeteMedResponsable();
    //.......... gestion de létat des timers ................
    void    setTimerActionEnabled(bool state = TRUE);
    void    setTimerActionOn();
    void    setTimerActionOff();
    void    setLedStateOnTimerState();
    void    onSqlError();
    int     areBasesInstalled();
signals:
    void triggered(QAction *actEtat);

private slots:
    void GestionBox();
    void Controle_Alarmes();
    void Controle_Entrees();
    void FaireClignoterLaTache();
    void GestionTaches();
    void GestionEtats();
    void GestionResponsables();
    void GestionParam(QString typeParam);
    void GestionCSS();
    void GestionINI();
    void Enregistrer_Positions();
    void Actualiser(QString partielOUtotal);
    void ActualiserCad();
    void ActualiserFen();
    void tilerLesW();
    void RestaureBase();
    void masquerNomPatient(QWidget *UnWidget);
    void rentrerUnPatient(QWidget *UnWidget);
    void bougerLePatient(QWidget *UnWidget);
    void Recap_Tache(QWidget *UnWidget);
    void Note_Tache(QWidget *UnWidget);
    void Annule_Tache(QWidget *UnWidget);
    void modif_Etat(QWidget *UnWidget);
    void modif_Resp(QWidget *UnWidget);
    void Lancer_DrTux(QWidget *UnWidget);
    void modif_Etat_menu(QString StrEtat);
    void modif_Resp_menu(QString StrResp);
    void selectionDunResponsable(QString StrResp);
    void PlierDeplier(QWidget *UnWidget);
    void GestionDestinationsSortie();
    void GestionDestinationsAbsence();
    //----------------------------------- Slot_pushButton_Apropos_clicked -----------------------------------------------------------------------
    /*! \brief lauch ../../APropos/bin/APropos for display informations about the programme
     *  the application version is defined in .pro  after mention  NUM_VERS =
     *  when this mention change  C_AppCore must be recompiled.
     */
    void Slot_pushButton_Apropos_clicked();
    void Slot_timerStateIndicator_clicked();
    void Slot_modif_Comment_Tache();
    void Slot_byebyeBox(C_Wdg_Box *);

private:
    void        SortirLesZombies();
    bool        RecupInit();
    bool        LireleCSS();
    int         Afficher_Les_Box();
    void        sortieAppli();
    void        creerActionsGene();
    void        creerActionsMdi();
    void        creerMenusGene();
    void        creerMenusMdi();
    void        createToolBars();
    void        createStatusBar();
    void        Recuperer_Positions();
    void        PopUp_Tache(QString Message, int largeur, int hauteur, QString Style);
    void        RemplirLeBox(C_Wdg_Box *dlgBox, QString BoxEnCours);
    C_Wdg_Box*  newBox(QString CodeBox, QString NomBox, QString CouleurBox, QString CouleurTitre, QString NbMaxiPat,  QString TypeBox);
    void        AfficherLesTaches(C_Wdg_Box *dlgBox, QString CodeBox, QString NumEnCours, bool StatusReplier  );
    void        afficheNomPatient(C_Wdg_Box *dlgBox, Ui_Box *Box_ui, QString NomPatient, QString BoxEnCours, QString NumEnCours, QString DateEntree, QString DateSortie, QString CommentairePatient, bool Anonyme, bool StatusReplier );
    void        afficheNomMedecin(C_Wdg_Box *dlgBox, QString CodeBox, QString NumEnCours, bool StatusReplier , QString Medecin);
    void        Alarme(QString Num_Encours, QString Code_Tache, QString TypeAlerte, QString Replier);
    void        majTableEncours(QString NumEncours, QString ChampsMisaJour, QString Commentaire);
    void        majTableEtatEncours(QString NumEnCours, QString NumTache, QString ChampsMisaJour, QString Commentaire);
    void        majDeLaTache(QString NumEncours, QString NumTache);
    void        majDateFinTache(QString NumEnCours, QString NumTache, QString DernietEtat);
    void        majHistorique(QString NumEnCours, QString CodeTache, QString Commentaire, QString LibelleTache);
    void        SortirLePatient(QString NumEnCours);
    void        saisieDestinationDuPatient(QString typeDeSortie, QString TypeSaisie);
    void        AppliquerUnStyle(QString TypeDeTruc, QWidget *Letruc, QString NomDuStyle, QString LaCouleurDuTruc);
    void        creerMenuSelectionResponsable();
    void        ActualiserUnBox(C_Wdg_Box *LeBox);
    QString     lectureBlobDrtux(int PrimKeyBlob);
    QString     RecupStyle(QString TypeObjet);

    QMap <QString, C_Wdg_Box *> m_Map_C_Wdg_Box;
    QMap <QString, QString> m_Map_Action;
    //C_Wdg_Box     *m_pC_Wdg_Box[NB_BOX_MAX];

    QDir           m_DirDossierEntree;
    QMdiArea      *m_mdiArea;
    QSignalMapper *m_windowMapper;
    QSignalMapper *m_mapperPatient;
    QSignalMapper *m_BoxMapper;
    QSignalMapper *m_PatientMapper;
    QSignalMapper *m_BougerMapper;
    QSignalMapper *m_TacheMapper;
    QSignalMapper *m_AnnuleTacheMapper;
    QSignalMapper *m_RecapMapper;
    QSignalMapper *m_EtatMapper;
    QSignalMapper *m_MenuMapper;
    QSignalMapper *m_ReplierMapper;
    QSignalMapper *m_RespMapper;
    QSignalMapper *m_MenuRespMapper;
    QSignalMapper *m_MenuSelectionRespMapper;
    QSignalMapper *m_DrTuxMapper;
    QMenu           *m_fileMenu;
    QMenu           *m_editMenu;
    QMenu           *m_windowMenu;
    QMenu           *m_helpMenu;
    QMenu           *m_menuEtat;
    QMenu           *m_menuResp;
    QMenu           *m_SelectionResponsable;
    QToolBar        *m_fileToolBar;
    QToolBar        *m_editToolBar;
    QPushButton     *m_BoutonPatientEnCours;
    QPushButton     *m_BoutonBougerEnCours;
    C_Wdg_Box       *m_BoxBougerEnCours;
    QPushButton     *m_BoutonEtat;
    QPushButton     *m_BoutonResp;
    QFrame          *m_popupTache;
    QAction         *m_boxAct;
    QAction         *m_tacheAct;
    QAction         *m_etatAct;
    QAction         *m_respAct;
    QAction         *m_cssAct;
    QAction         *m_iniAct;
    QAction         *m_sortieAct;
    QAction         *m_absenceAct;
    QAction         *m_RestaureBaseAct;
    QAction         *m_exitAct;
    QAction         *m_PosAct;
    QAction         *m_ReafficherFenAct;
    QAction         *m_ReafficherCadAct;
    QAction         *m_tileAct;
    QAction         *m_cascadeAct;
    QAction         *m_separatorAct;
    QAction         *m_aboutAct;

    QTimer          *m_timerAlarme;
    QTimer          *m_timerClignote;
    QTimer          *m_timerEntrees;
    C_ClickableLed  *m_TimerStateIndicator;
    int              m_notAction;
    QSettings       *m_settingsIni;

    QList<QPushButton *> m_ListeBoutonsAlarme;
    QList<int>  m_ListePositions;
    int         m_zombieLifeTime;
    QString     m_nomFicIni;
    QString     m_Dossier_Entrees;
    QString     m_BoxEnCours;
    QString     m_PK_encours;
    QString     m_Couleur_Patients;
    QString     m_Couleur_PatientsClignote;
    QString     m_Couleur_Comment_Patients;
    QString     m_Couleur_BougerPatients;
    QString     m_Couleur_Medecins;
    QString     m_Couleur_HeuresEntree;
    QString     m_Couleur_HeuresSortie;
    QString     m_CouleurDesti;
    QString     m_ModeAffichage;
    QString     m_MessageAlarme;
    QString     m_CodeBoxParDefaut;
    QString     m_BoutonDrtux;
    QString     m_ResponsableSelectionne;
    QString     m_ConfirmationSortie;
    QString     m_ExeCalendrier;
    QByteArray  m_FicCSS;
    QProcess   *m_Apropos_Proc;
    bool        m_OnColleTout;
    int         m_Largeur_Heures;
    int         m_Hauteur_Taches;
    int         m_Hauteur_Medecins;
    int         m_Hauteur_Heures;
    int         m_Hauteur_Patients;
    int         m_Largeur_Boutons;
    int         m_Hauteur_Comment_Patients;
    int         m_Largeur_Annule_Tache;
    int         m_Hauteur_Box;
    int         m_Largeur_Box;
    int         m_Hauteur_NomBox;
    int         m_Largeur_Tache;
    int         m_PeriodeAlarme;
    int         m_PeriodeClignote;
    int         m_PeriodeEntrees;
    int         m_DernierPkencours_taches;
    int         m_DernierPkencours;
    int         m_DelaiDeSortie;

};
#endif
