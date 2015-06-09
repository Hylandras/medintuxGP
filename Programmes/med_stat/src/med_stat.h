#ifndef MAD_STAT_MEDSTAT_H
#define MAD_STAT_MEDSTAT_H

#include <qmainwindow.h>
#include <qvbox.h>
#include <qaction.h>
#include <qworkspace.h>
#include <qprocess.h>
#include <qdir.h>
#include "../../drtux/src/CMoteur_Base.h"

class CMDI_Identification;
class Wdg_Monitor;
class Wdg_User;
class Wdg_Central;

//====================================== med_stat =======================================================
class med_stat: public QMainWindow
{
    Q_OBJECT

public:
    //............................ methodes .............................
    med_stat();
   ~med_stat();

    //............................. datas ...............................
    CMDI_Identification *m_CMDI_Identification;
    void    SetNoUserState();
    QString    PluginExe(       QObject         * pQObject,
                                const char      * pluginScript,
                                int               waitFlag  = CMoteurBase::endWait );

    QString    PluginExe(       QObject          *pQObject,
                                const QString    &pathPlugin,  // Chemin de l'executable plugin à actionner sans l'extension .exe
                                const QString    &pathAppli,   // Chemin de l'executable appelant (DrTux)
                                const QString    &pathIni,     // Chemin d'un eventuel fichier de configuration pour XXXX.ini pour l'executable plugin ou %
                                const QString    &maskExch,    // texte du fichier d'exchange
                                const QString    &guid,
                                const QString    &dosPk,
                                const QString    &terPk,
                                const QString    &ordPk ,
                                QStringList      &param,
                                int               waitFlag = CMoteurBase::endWait );
    void DemarrerAccessoires();
    void StartWithUser(const QString &user, const QString &sign_user);
protected:
    //............................ methodes .............................
    void closeEvent( QCloseEvent* ce );
    void setupFileActions();

    void DemarrerUnAccessoire(QString accessoire_path);
    //............................. datas ...............................
    Wdg_Monitor         *m_pWdg_Monitor;
    Wdg_User            *m_pWdg_User;
    QWorkspace          *m_pQWorkSpace;
public slots:
    void OnUserSelected( QListViewItem *pQListViewItem , QListViewItem *pQListViewItemParent);
    void setCaption(const QString &caption);
    void Slot_fileOpen();
    void Slot_fileSave();
    void Slot_run();
    void Slot_export();
    void Slot_EditUser();
    void SlotMakeVisible(const QString&);
    void Slot_CPS_IsClicked ();
    void Slot_RecordPos();
    void Slot_ComboUserTypClicked(const QString &userQualite);
private slots:
    void fileSave(){};
    void filePrint(){};
    void fileClose(){};
    void OnOngletRubriquesChanged(QWidget * pCMDI_Qwidget);
    void OnActiverRubrique(const char* rubName);
    void OnDeleteRubrique(const char* rubName);
    void Slot_actionAproposDisplay();
    void Slot_actionApropos();
    void Slot_SauverLesMeubles();
    void tryToStopAPropos();
private:
   QString   m_PluginRun;
   QProcess *m_Apropos_Proc;
signals:
    void  Sign_ActiverRubrique(const char*);

};

//====================================== CMDI_Generic =======================================================
class CMDI_Generic : public QMainWindow
{
    Q_OBJECT

public:
   CMDI_Generic(QWidget* parent, const char* name, int wflags, CMoteurBase *pCMoteurBase)
       :QMainWindow( parent, name, wflags ), m_pCMoteurBase(pCMoteurBase), m_Name(name)
       {
       }

public slots:
   int     GetType()
              {return m_Type;
              }
   void    SetType(int type)
              {m_Type = type;
              }
   QString     GetName()
              {return m_Name;
              }
public:
   CMoteurBase     *m_pCMoteurBase;

private:
   QString          m_Name;
   int              m_Type;

};

//====================================== CMDI_Identification =======================================================
class Wdg_Central;

class CMDI_Identification : public CMDI_Generic
{
    Q_OBJECT
public:
    CMDI_Identification( QWidget* parent, const char* name, int wflags , CMoteurBase *pCMoteurBase);
    /*                    :CMDI_Generic(parent, name,  wflags, pCMoteurBase);

    {QVBox *pQVCentral = new QVBox( this );
     pQVCentral->setMargin( 0 );
     pQVCentral->setSpacing(0);
     pQVCentral->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
     m_pWdg_Central = new Wdg_Central(pQVCentral, "Wdg_Central");
     setCentralWidget( pQVCentral );
     setIcon( G_pCApp->m_PathAppli+"Images"+QDir::separator()+"malehead.png" );
    }
 */
    ~CMDI_Identification(){};

   void closeEvent( QCloseEvent* ce )
        {ce->ignore();
        }
public:
    Wdg_Central *m_pWdg_Central;

public slots:
    void SetUser(const QString &user, const QString &signUser);
    /*
    {m_pWdg_Central->setUser( user);
    };
   */

private:

signals:

};

#endif
