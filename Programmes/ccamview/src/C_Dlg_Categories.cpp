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


#include "C_Dlg_Categories.h"

//------------------------------------- C_Dlg_Categories -------------------------------------
C_Dlg_Categories::C_Dlg_Categories(CMoteurCCAM_Base * pCMoteurCCAM_Base, const QString & numero_serie, const QString & code_utilisateur,
                                   QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
: Dlg_Categories(parent,name, modal,fl)
{   // signals and slots connections
    connect( bouton_Annuler, SIGNAL( clicked() ), this, SLOT( bouton_Annuler_clicked() ) );
    connect( bouton_OK, SIGNAL( clicked() ), this, SLOT( bouton_OK_clicked() ) );
    connect( combo_Categorie, SIGNAL( textChanged(const QString&) ), this, SLOT( combo_Categorie_textChanged(const QString&) ) );

    // récupère les variables passées par l'appel
    m_pCMoteurCCAM_Base = pCMoteurCCAM_Base;
    m_Numero_Serie = numero_serie;
    m_Utilisateur = code_utilisateur;
    // rempli la combo box categorie
    m_pCMoteurCCAM_Base->GetComboCategorie(combo_Categorie,m_Utilisateur);
    // positionne la combo box categorie sur l'élément categorie et rempli le lineEdit_LibelleUsuel
    m_pCMoteurCCAM_Base->GetInfoActeThesaurus(combo_Categorie,lineEdit_LibelleUsuel,m_Numero_Serie);
}

//------------------------------------- C_Dlg_Categories -------------------------------------
C_Dlg_Categories::~C_Dlg_Categories()
{
}

/*$SPECIALIZATION$*/

//------------------------------------- combo_Categorie_textChanged -------------------------------------
void C_Dlg_Categories::combo_Categorie_textChanged( const QString & )
{   QString texte_combo = combo_Categorie->currentText();
    if (texte_combo.length() > 40){
        texte_combo = texte_combo.right(40);
        combo_Categorie->setCurrentText(texte_combo);
    }
}

//------------------------------------- bouton_OK_clicked -------------------------------------
void C_Dlg_Categories::bouton_OK_clicked()
{QString categorie = combo_Categorie->currentText();
    QString lib_usuel = lineEdit_LibelleUsuel->text();
    m_pCMoteurCCAM_Base->MAJActeThesaurus(m_Numero_Serie,categorie,lib_usuel);
    accept();
}

//------------------------------------- combo_Categorie_textChanged -------------------------------------
void C_Dlg_Categories::bouton_Annuler_clicked()
{
    reject();
    return;
}



