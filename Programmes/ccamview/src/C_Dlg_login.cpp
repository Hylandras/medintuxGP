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


#include "C_Dlg_login.h"

//----------------------- C_Dlg_login -----------------------
C_Dlg_login::C_Dlg_login(CMoteurCCAM_Base* pCMoteurCCAM_Base, const QString &last_user, const QString &password,
                         QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
: Dlg_Login(parent,name, modal,fl)
{
    // signals and slots connections
    connect( bouton_OK,            SIGNAL( clicked() ),                   this, SLOT( bouton_OK_clicked() ) );
    connect( bouton_Quitter,       SIGNAL( clicked() ),                   this, SLOT( bouton_Quitter_clicked() ) );
    connect( lineEdit_MotDePasse,  SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_MotDePasse_textChanged(const QString&) ) );

    lineEdit_Utilisateur->setText(last_user);
    lineEdit_MotDePasse->setText(password);
    if (last_user.length()) lineEdit_MotDePasse->setFocus();
    else                    lineEdit_Utilisateur->setFocus();
    if (password.length() && last_user.length())  bouton_OK->setFocus();
    m_pCMoteurCCAM_Base = pCMoteurCCAM_Base;
}

//----------------------- C_Dlg_login -----------------------
C_Dlg_login::~C_Dlg_login()
{

}

/*$SPECIALIZATION$*/

//----------------------- lineEdit_MotDePasse_textChanged -----------------------
void C_Dlg_login::lineEdit_MotDePasse_textChanged( const QString & )
{// .................. astuce pour créer les utilisateurs ..................................
    if ( (lineEdit_MotDePasse->text() == "SENSEI") && (lineEdit_Utilisateur->text() == "") ){
        //.................... Appel de la fenêtre saisie/modification fiche praticien ....................
        Dlg_Praticiens* dlg_prat = new Dlg_Praticiens;
        if (dlg_prat==0)      return; // Erreur initialisation
        dlg_prat->initDialog(m_pCMoteurCCAM_Base);
        if (dlg_prat->exec()== QDialog::Rejected){
            delete dlg_prat;
            return; // On quitte sans créer de praticien
        }
        delete dlg_prat;
    }
}

//----------------------- bouton_Quitter_clicked -----------------------
void C_Dlg_login::bouton_Quitter_clicked()
{  reject();
}

//----------------------- bouton_OK_clicked -----------------------
void C_Dlg_login::bouton_OK_clicked()
{   QString utilisateur = lineEdit_Utilisateur->text();
    QString password = lineEdit_MotDePasse->text();

    if ( (lineEdit_Utilisateur->text() != "") )
       {
        bool droit_ouvert = m_pCMoteurCCAM_Base->VerificationLogin(utilisateur, password);
        if (droit_ouvert)
           {accept();
           }
    }
}



