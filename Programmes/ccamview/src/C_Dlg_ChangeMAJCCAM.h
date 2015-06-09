//
// C++ Interface: 
//
// Description: 
//
//
// Author: Sevin Roland <ro@upsis>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef C_DLG_CHANGEMAJCCAM_H
#define C_DLG_CHANGEMAJCCAM_H
#include <Q3ListView>
#include <Q3ListViewItem>
#include <qapplication.h>
#include <qeventloop.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <stdlib.h>
#include <qstringlist.h>

#include "Global.h"
#include "CMoteurCCAM_Base.h"
#include "Dlg_ChargeMAJCCAM.h"

class C_Dlg_ChangeMAJCCAM : public Dlg_ChargeMAJCCAM
{
  Q_OBJECT

public:
  C_Dlg_ChangeMAJCCAM(CMoteurCCAM_Base* pCMoteurCCAM_Base,const QString &fichier,QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0 );
  ~C_Dlg_ChangeMAJCCAM();
  /*$PUBLIC_FUNCTIONS$*/
  virtual long          RechercheNombreEntitesTotales();
  virtual long          RechercheNombreEntite( QString numero, QString rubrique, QString etat );
  virtual int           RechercheIndex( QString & numero, QString & rubrique );
  virtual char *        ParseEntite( Q3ListView * pQListView, QString & ligne_a_decouper, QString & id_entite );
  virtual QVariant      ValeurDansEntite( QString & ligne_a_decouper, QString & section, QString var_name );
  virtual bool          LectureFichier( QFile * pQFile );
  virtual bool          VerificationCoherenceFichier( QFile * pQFile );
  virtual int           initDialog( CMoteurCCAM_Base * pCMoteurCCAM_Base, const QString & fichier );

public slots:
  /*$PUBLIC_SLOTS$*/
  virtual void          StockageDate( const QString & date );
  virtual void          EffaceBase();
  virtual void          pushButton_Integrer_clicked();
  virtual void          pushButton_Annuler_clicked();
  virtual void          DecoupeEntite( QString & ligne_a_decouper );
  virtual void          RAZ_Tableau( QString * tableau, int longueur );
  virtual void          RAZ_Compatibilite( ENT_COMPATIBILITE * compatibilite );
  virtual void          converti( char * source );
  virtual void          InformationsEntites( QString & ligne_a_decouper );
public:
  QString                 m_LastError;
  bool                    m_Maj_OK;

protected:
  /*$PROTECTED_FUNCTIONS$*/
     ENT_HISTO_ACTIVITE      m_HistoActivite;
     LISTE_MOTS              m_ListeMots;
     TAB_VAL_UNITE_OEUVRE    m_TabValUnitOeuvre[7];
     TAB_DMT                 m_TabDmt;
     TAB_SPEC                m_TabSpec;
     TAB_NUM_DENTS           m_TabNumDents;
     TAB_CODE_PAIEMENT       m_TabCodePaiement;
     TAB_AGR_RAD             m_TabAgrRad;
     TAB_TYPE_ACTE           m_TabTypeActe;
     TAB_FRAIS_DEP           m_TabFrais;
     TAB_NAT_ASS             m_TabNatAss;
     TAB_CLASSE_DMT          m_ClasseDmt;
     TAB_TB20                m_TabTb20[4];
     TAB_TB18                m_TabTb18[6];
     TAB_TB15                m_TabTb15[6];
     TAB_TB13                m_TabTb13[6];
     TAB_TB11                m_TabTb11[4];
     TAB_TB09                m_TabTb09[6];
     bool                    m_StopIntegration;
     TAB_TB06                m_TabTb06[4];
     TAB_TB04                m_TabTb04[6];
     TAB_TB03                m_TabTb03[5];
     TYPE_NOTE               m_TypesNotes;
     TAB_ASSO                m_TabAsso[5];
     ENT_PREC                m_EntitePrecedente;
     QString                 m_ActeIncompatible[8];
     QString                 m_DejaIntegre;
     VER_BASE                m_VersionBase;
     ENT_HISTO_ACTE          m_HistoActe;
     ENT_COMPATIBILITE       m_Compatibilite[8];
     ENT_ACTE                m_ActeEnCours;
     int                     m_Index_max;
     NB_ENT                  m_Nb_Entite[100];
     QString                 m_fichier_entite;
     long                    m_Position;
     QFile*                  m_pQFile;
     CMoteurCCAM_Base*       m_pCMoteurCCAM_Base;
     bool                    m_FichierOuvert;
     char *                  m_Buffer;
     QString                 m_TypeFichier;
     long                    m_Nb_EntitesTotales;
     long                    m_Nb_Entite002020;
     long                    m_Nb_Entite10101;
     long                    m_Nb_Entite20101;
     long                    m_Nb_Entite30101;
     QTime                   m_Duree_Traitement;
     ENT_ACTIVITE            m_Activite;
     QString                 m_CodeProcedure[8];
     QString                 m_ModificateurActe[10];
     NOTE_ACTE               m_NoteActe;
     ENT_PHASE               m_Phase;
     ENT_ARBO                m_Arborescence;
     ENT_NOTEARBO            m_NoteArbo;
     TAB_ACTIVITE            m_TabActivite;
     TAB_MODIFICATEUR        m_TabModificateur;
     TAB_PHASE               m_TabPhase;
     TAB_TB05                m_TabTb05[6];
     TAB_TB07                m_TabTb07[2];
     TAB_TB08                m_TabTb08[4];
     TAB_TB10                m_TabTb10[6];
     TAB_TB12                m_TabTb12[4];
     TAB_TB14                m_TabTb14[5];
     TAB_TB17                m_TabTb17[9];
     TAB_TB19                m_TabTb19[7];
     TAB_CONDGEN             m_CondGen;
     TAB_EXO_TM              m_ExoTm;
     TAB_REMBOURS            m_TabRembours;
     TAB_TYPE_FORFAIT        m_TabTypeForfait;
     TAB_EXT_DOC             m_TabExtDoc;
     TAB_CAT_MED             m_TabCatMed;
     TAB_COD_REGROUP         m_TabCodRegroup;
     TAB_CAT_SPEC            m_TabCatSpec;
     TAB_DOM                 m_TabDom;
     TAB_ASS_NPREV           m_TabAssNPrev;
     TAB_RGL_TARIF           m_TabRglTarif;
     TAB_COMPAT_EXOTM        m_TabCompatExoTM[60];
     COND_GEN_ACTE           m_CondGenActe[30];
     REC_MED                 m_RecMedActe;
     ENT_HISTO_PHASE         m_Historique;
     int                     m_lenEOL;
     QStringList             m_ListeDate;

protected slots:
  /*$PROTECTED_SLOTS$*/

};

#endif

