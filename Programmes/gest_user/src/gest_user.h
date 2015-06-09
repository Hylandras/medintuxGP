/***************************************************************************
 *   Copyright (C) 2003 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
 *   gest_user.h                                                        *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef GEST_USER_H
#define GEST_USER_H

#include <qmainwindow.h>
#include <qobject.h>
#include <qtextedit.h>
#include <qworkspace.h>
#include <qaction.h>
#include <qpushbutton.h>
#include <qsizegrip.h>
#include <qfontdatabase.h>
#include <qlineedit.h>
#include <qcolor.h>
#include <qcolordialog.h>
#include <qdir.h>
#include <qsimplerichtext.h>
#include <qsplitter.h>
#include <qfile.h>
#include <qvbox.h>
#include <qlayout.h>
#include <qtabwidget.h>
#include <qworkspace.h>
#include <qlabel.h>
#include <qprocess.h> 

#include "../../drtux/src/CMoteur_Base.h"
#include "../../MedinTuxTools/CHtmlTools.h"


#include "MyEditText.h"
#include "FormOngletRubrique.h"

#include "ui/RubIdentite.h"
#include "ui/FormRubOrdo.h"
#include "ui/FormRubCertificat.h"
#include "ui/FormRubObservation.h"
#include "ui/FormRubParam.h"
#include "ui/DockFormUserListDoc.h"
#include "ui/DockFormUserList.h"

class CMDI_Generic;
class CMDI_Ident;
class CMDI_Ordo;
class CMDI_Certificat;
class CMDI_Observation;
class CMDI_Param;
class BigShrinker;

//====================================== Gest_User =======================================================


class QTextEdit;

class Gest_User: public QMainWindow
{
    Q_OBJECT

public:
     Gest_User();
    ~Gest_User();
//....................... necesaire à la gestion des textes ......................
    QComboBox           *m_comboStyle;
    QComboBox           *m_comboFont;
    QComboBox           *m_comboSize;
    QAction  *m_pActionTextBold,
             *m_pActionTextUnderline,
             *m_pActionTextItalic,
             *m_pActionTextColor,
             *m_pActionAlignLeft,
             *m_pActionAlignCenter,
             *m_pActionAlignRight,
             *m_pActionEnregistrer,
             *m_pActionAlignJustify;

    void setupFileActions();       // cree la barre d'outils et menu lié aux fonctions disque
    void setupTextActions();       // cree la barre d'outils et menu lié aux attributs texte (gras italique etc..)
    void setupEditActions();       // cree la barre d'outils et menu lié aux actions (couper coller etc...)
    void SetInterfaceOnDroits();
    QString    PluginExe(       QObject    *pQObject,
                                const QString    &pathPlugin,  // Chemin de l'executable plugin à actionner sans l'extension .exe
                                const QString    &pathAppli,   // Chemin de l'executable appelant (DrTux)
                                const QString    &pathIni,     // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                const QString    &maskExch,    // texte du fichier d'exchange
                                const QString    &guid,
                                const QString    &dosPk,
                                const QString    &terPk,
                                const QString    &ordPk ,
                                QStringList      &param,
                                int               waitFlag  = CMoteurBase::endWait );
    QString     UnSerializeAll();
    QString     SerializeAll();

    QString     SerializeIdentite();
    int         UnSerializeIdentite(const QString& file);
    int         UnSerializeIdentite(const QString &serializeData, const QString & justForDiff);
    QString     SerializeDocOnly();
    QString     SerializeDataDocument();
    void        UnSerializeDocuments(const QString& file);
    QString     UnSerializeDocuments(const QString& data, const QString & pk);
    void        tryToStopAPropos();
    //.............. formatage du texte ......................................................
    MyEditText *currentEditor()const;
public slots:
    void        textFamily( const QString & );     //......... on positionne un atribut de l'editeur
    void        textItalic();
    void        textBold();
    void        textUnderline();
    void        textColor();
    void        textSize( const QString & );
    void        textAlign( QAction* );
    void        textStyle( int );
    void        editUndo();
    void        editRedo();
    void        editCut();
    void        editCopy();
    void        editPaste();
    void        editorChanged( QWidget * );        //......... l'editeur signale un changement
    void        fontChanged( const QFont &f );
    void        colorChanged( const QColor &c );
    void        alignmentChanged( int a );
    void        doConnections( QTextEdit *e );     // connecte un editeur de texte aux fonctions de mise a jour des boutons
    void        HelpMe();
    void     OnActiverRubrique(const char* rubName);
    void     OnDeleteRubrique(const char* rubName);
    void     OnOngletRubriquesChanged(QWidget * pQwidget);
    void     On_pushButton_ExportProfilClicked();
    void     On_pushButton_InportProfilClicked();
    void     On_pushButton_PlaceProfilClicked();

    void RestaureUserListConnexion();
    void OnNewWhithExistantClicked();
    void ClearInterface();
    //........................ creation des fenêtres MDI ......................................
    //                         dans l'espace rubrique:  m_pQWorkSpaceRub
    CMDI_Ident*          CMDI_IdentCreate (const char* prim_key);
    CMDI_Ordo*           CMDI_OrdoCreate (const char* prim_key);
    CMDI_Certificat*     CMDI_CertificatCreate (const char* prim_key);
    CMDI_Observation*    CMDI_ObservationCreate(const char* prim_key);
    CMDI_Param*          CMDI_ParamCreate (const char* prim_key);

    void          OnPassWordChanged(const char* password);
    void          OnUserDeleteClicked();
    void          OnNewUserClicked(void);
    void          OnNewUserAbort();
    void          OnUserChanged(const char* prim_key);      // demande de confirmation de sauvegarde
    void          SetOnUserChanged(const char* prim_key);   // pas de demande de confirmation de sauvegarde
    void          OnUserListDocClicked(const char *ref_UserPrimKey, const char* name, const char*  typ, const char *prim_key);
    void          OnSign_NewUserIsCreated(CMDI_Ident* pCMDI_Ident, const QString &ident_prim_key);
    void          OnSign_RubChanged();
    void          OnRubIsUpdated();
    void          On_pushButton_SaveIdentOnDiskClicked();
    void          On_pushButton_NewFromFileOnDiskClicked();
    CMDI_Generic* IsExistRubrique(const char* prim_key, int type);
    void          doYouVantSaveAllBeforeAction();
protected slots:
    void    closeEvent( QCloseEvent* );
    void    fileSave();
    void    fileClose();
    QString GetUserPrimKey();
    void    InsererImage();
    void    InsererChamp();
    void     Slot_SauverLesMeubles();
    void     Slot_actionAproposDisplay();
    void     Slot_actionApropos();
private slots:

    void about();
    void aboutQt();

private:
    CMoteurBase         *m_pCMoteurBase;
    QWorkspace          *m_pQWorkSpaceRub;        // workspace CMDI contenant les rubriques
    FormOngletRubrique  *m_pFormOngletRubrique;   // widget d'onglets
    DockFormUserListDoc *m_pDockFormUserListDoc;
    DockFormUserList    *m_pDockFormUserList;
    QString              m_ImagePath;
    QString              m_OldUser;
    QString              m_PluginRun;
    QProcess            *m_Apropos_Proc;
signals:
    void  Sign_OnIdentChange(const char* prim_key);
    void  Sign_ActiverRubrique(const char*);
    void  Sign_PassWordChanged(const char* password); // lors d'un changement d' identité le programme doit pouvoir
                                                      // prevenir que le mot de passe, n'est plus le même

};

//====================================== CMDI_Generic =======================================================
class CMDI_Generic : public QMainWindow
{
    Q_OBJECT

public:
   CMDI_Generic(QWidget* parent, const char* name, int wflags, CMoteurBase *pCMoteurBase)
       :QMainWindow( parent, name, wflags )
       {m_pCMoteurBase = pCMoteurBase;
        m_IsModified   = FALSE;
       }
   int     GetType()
              {return m_Type;
              }
   void    SetType(int type)
              {m_Type = type;
              }
   QString GetUserPrimKey()
             {return m_ref_UserPrimKey;
             }
   void    SetUserPrimKey(const char* ref_UserPrimKey)
             { m_ref_UserPrimKey = ref_UserPrimKey;
             }

public slots:
   virtual MyEditText *GetEditor()         {return 0;}
   virtual void SetText(QString )          {}
   virtual void SetNameDoc(QString )       {}

public:
   CMoteurBase     *m_pCMoteurBase;
   bool             m_IsModified;
private:
   QString          m_ref_UserPrimKey;
   int              m_Type;


};

//====================================== CMDI_Certificat =======================================================
#define CERTIF_NAME     "Certificat"
class CMDI_Certificat: public CMDI_Generic
{
    Q_OBJECT
public:
    CMDI_Certificat( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, const char* prim_key=0);
   ~CMDI_Certificat();
   virtual MyEditText *GetEditor();
   virtual void SetText(QString text);
   virtual void SetNameDoc(QString text);

public slots:
    //............. appelé par DrTux pour mettre à jour les ..................
    //              données en fonction de l'identité du patient
    void Slot_EditGotFocus( MyEditText *pMyEdit){m_LastMyEditFocus = pMyEdit;}
    void OnIdentChange(const char* prim_key );
    void ActiverRubrique(const char *rubName);
    void VerifyPassWordAndSaveDocument();
    void SaveDocument();
    void CreateDocStandard (const QString &modeleName="");
    void CreateDocFromFile();
    void DeleteDocument(const char* primKey);
    void CMDI_DoConnection(MyEditText* e);
    void OnSign_CertificatChanged();
public:
    FormRubCertificat    *m_pFormRubCertificat;
    MyEditText           *m_pMyEditTextHead;
    MyEditText           *m_pMyEditTextFoot;

    MyEditText           *m_LastMyEditFocus;

private:

signals:
    void Sign_RubIsDeleted(const char* name);
    void Sign_RubListMustBeUpdated(const char* primkey);
    void Sign_RubChanged();
    void Sign_RubIsUpdated();
};

//====================================== CMDI_Observation =======================================================
#define OBSERV_NAME     "Observation"
class CMDI_Observation: public CMDI_Generic
{
    Q_OBJECT
public:
    CMDI_Observation( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, const char* prim_key=0);
   ~CMDI_Observation();
   virtual MyEditText *GetEditor();
   virtual void SetText(QString text);
   virtual void SetNameDoc(QString text);

public slots:
    //............. appelé par DrTux pour mettre à jour les ..................
    //              données en fonction de l'identité du patient
    void Slot_EditGotFocus( MyEditText *pMyEdit){m_LastMyEditFocus = pMyEdit;}
    void OnIdentChange(const char* prim_key );
    void ActiverRubrique(const char *rubName);
    void VerifyPassWordAndSaveDocument();
    void SaveDocument();
    void CreateDocStandard ();
    void CreateDocFromFile();
    void DeleteDocument(const char* primKey);
    void CMDI_DoConnection(MyEditText* e);
    void OnSign_ObservationChanged();
public:
    FormRubObservation   *m_pFormRubObservation;
    MyEditText           *m_pMyEditTextHead;
    MyEditText           *m_pMyEditTextFoot;

    MyEditText           *m_LastMyEditFocus;

private:

signals:
    void Sign_RubIsDeleted(const char* name);
    void Sign_RubListMustBeUpdated(const char* primkey);
    void Sign_RubChanged();
    void Sign_RubIsUpdated();
};

//====================================== CMDI_Param =======================================================
#define PARAM_NAME      "Parametrages"
class CMDI_Param : public  CMDI_Generic
{
    Q_OBJECT
public:
    CMDI_Param( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, const char* prim_key=0);
   ~CMDI_Param();
   virtual MyEditText  *GetEditor();
   virtual void         SetText(QString text);
   virtual void         SetNameDoc(QString text);
public:
    FormRubParam  *m_pFormRubParam;
    MyEditText    *m_pMyEditText;
    MyEditText    *m_LastMyEditFocus;

public slots:
    //............. appelé par DrTux pour mettre à jour les ..................
    //              données en fonction de l'identité du patient
    void Slot_EditGotFocus( MyEditText *pMyEdit){m_LastMyEditFocus = pMyEdit;}
    void OnIdentChange(const char* prim_key );
    void ActiverRubrique(const char *rubName);
    void VerifyPassWordAndSaveDocument();
    void SaveDocument();
    void CreateDocStandard ();
    void DeleteDocument(const char* primKey);
    void CMDI_DoConnection(MyEditText* e);
    void OnSign_ParamChanged();
private:

signals:
    void Sign_RubIsDeleted(const char* name);
    void Sign_RubListMustBeUpdated(const char* primkey);
    void Sign_RubChanged();
    void Sign_RubIsUpdated();
};

//====================================== CMDI_Ordo =======================================================
#define ORDO_NAME       "Ordonnance"
class CMDI_Ordo : public  CMDI_Generic
{
    Q_OBJECT
public:
    CMDI_Ordo( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, const char* prim_key=0);
   ~CMDI_Ordo();
    virtual MyEditText *GetEditor();
    virtual void SetText(QString text);
    virtual void SetNameDoc(QString text);

public:

    FormRubOrdo   *m_pFormRubOrdo;
    MyEditText    *m_pMyEditRecto_Entete;
    MyEditText    *m_pMyEditRecto_Pied;
    MyEditText    *m_pMyEditDupli_Entete;
    MyEditText    *m_pMyEditDupli_Pied;
    MyEditText    *m_LastMyEditFocus;

public slots:
    //............. appelé par DrTux pour mettre à jour les ..................
    //              données en fonction de l'identité du patient
     void Slot_EditGotFocus( MyEditText *pMyEdit){m_LastMyEditFocus = pMyEdit;}
     void OnIdentChange(const char* prim_key);
     void ActiverRubrique(const char *rubName);
     void SetData();
     void CMDI_DoConnection(MyEditText* e);
     void VerifyPassWordAndSaveDocument();
     void SaveDocument();
     void CreateDocStandard ();
     void CreateDocFromFile ();
     void DeleteDocument(const char* primKey);
     void OnSign_OrdonnanceChanged();
    //void Sign_OnDrTuxIdentChange(const char* , const char* ,  const char*  ,  const char*  );
private:

signals:
    void Sign_RubIsDeleted(const char* name);
    void Sign_RubListMustBeUpdated(const char* primkey);
    void Sign_RubChanged();
    void Sign_RubIsUpdated();
};


//====================================== CMDI_Ident =======================================================
class CMDI_Ident : public  CMDI_Generic
{
    Q_OBJECT
public:
    CMDI_Ident( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase, const char* prim_key=0);
   ~CMDI_Ident();
   virtual MyEditText  *GetEditor(){return 0;}

public:
    FormRubIdent  *m_pFormRubIdent;
    QString        m_CriptedPassWord;

public slots:
    //............. appelé par DrTux pour mettre à jour les ..................
    //              données en fonction de l'identité du patient
     void OnIdentChange(const char* prim_key);
     void ActiverRubrique(const char *rubName);
     void ClearData();
     void PrepareNewGUID();
     int  IsValidToCreateOrUpdate();
     int  VerifyPassWordAndSaveDocument();
     int  SaveDocument();
     int  IsToCreate();
     int  CreateNewUser();
     int  UpdateUser();
     void SetPassWord(const char* password);
     void OnSign_IdentiteChanged();
    //void Sign_OnDrTuxIdentChange(const char* , const char* ,  const char*  ,  const char*  );
private:

signals:
    void message(const QString&, int );
    void Sign_RubIsDeleted(const char* name);
    void Sign_PassWordChanged(const char* password);  // la rubrique identité est un endroit où l'on peut modifier
                                                      // le mot de passe, donc elle emet ce signal pour prevenir
                                                      // l'appli de ce changement
    void Sign_NewUserIsCreated(CMDI_Ident* pCMDI_Ident, const QString &ident_prim_key);
    void Sign_RubChanged();
    void Sign_RubIsUpdated();
};

//====================================== BigShrinker ================================================================
class BigShrinker : public QFrame
{
    Q_OBJECT
public:
    //................ BigShrinker ...................................
    BigShrinker(QWidget* parent, int w, int h) :
        QFrame(parent)
       {setFrameStyle(QFrame::Box|QFrame::Sunken);
        resize(w, h);
       }

    //................. mouseReleaseEvent .............................
    void mouseReleaseEvent(QMouseEvent* e)
    {
        emit clicked(e->x(), e->y());
    }

signals:
    void clicked(int,int);
};



#endif
