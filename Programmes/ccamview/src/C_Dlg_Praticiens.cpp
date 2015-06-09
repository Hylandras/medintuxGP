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


#include "C_Dlg_Praticiens.h"
#include <Q3ListViewItem>

//------------------------------------- C_Dlg_Praticiens -------------------------------------
C_Dlg_Praticiens::C_Dlg_Praticiens(CMoteurCCAM_Base* pCMoteurCCAM_Base, QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
: Dlg_Praticiens(parent,name, modal,fl)
{
    connect( bouton_Quitter, SIGNAL( clicked() ), this, SLOT( bouton_Quitter_clicked() ) );
    connect( listview_praticiens, SIGNAL( clicked(Q3ListViewItem*) ), this, SLOT( listview_praticiens_clicked(Q3ListViewItem*) ) );
    connect( lineEdit_Code, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Code_textChanged(const QString&) ) );
    connect( lineEdit_Nom, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Nom_textChanged(const QString&) ) );
    connect( lineEdit_Prenom, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Prenom_textChanged(const QString&) ) );
    connect( lineEdit_Nid, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Nid_textChanged(const QString&) ) );
    connect( lineEdit_Spec, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Spec_textChanged(const QString&) ) );
    connect( lineEdit_MotDePasse, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_MotDePasse_textChanged(const QString&) ) );
    connect( lineEdit_ConfMot, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_ConfMot_textChanged(const QString&) ) );
    connect( bouton_OK, SIGNAL( clicked() ), this, SLOT( bouton_OK_clicked() ) );
    connect( bouton_New, SIGNAL( clicked() ), this, SLOT( bouton_New_clicked() ) );
    connect( pushButtonDelete, SIGNAL( clicked() ), this, SLOT( pushButtonDelete_clicked() ) );


    m_pCMoteurCCAM_Base = pCMoteurCCAM_Base;
    // ....... Verifie si la table medecins est créée sinon la crée .......
    m_pCMoteurCCAM_Base->CreeTableMedecins();
    // ....... Rempli la listview avec les medecins existant .......
    m_pCMoteurCCAM_Base->GetMedecins(listview_praticiens);
    EffaceSaisie();
    //bouton_OK->setEnabled(FALSE);
    if (m_pCMoteurCCAM_Base->m_USER_IDENT_TYPE_TABLE=="MedinTux")
       {lineEdit_MotDePasse_MedinTux->setEnabled(FALSE);
        lineEdit_ConfMot_MedinTux->setEnabled(FALSE);
       }
    if (m_pCMoteurCCAM_Base->m_USER_IDENT_TYPE_TABLE=="SIGEMS")
       {groupBoxMedinTux->hide();
       }
}

//------------------------------------- C_Dlg_Praticiens -------------------------------------
C_Dlg_Praticiens::~C_Dlg_Praticiens()
{
}

/*$SPECIALIZATION$*/
//------------------------------------- VerifSIGEMSValiditation -------------------------------------
int C_Dlg_Praticiens::VerifSIGEMSValiditation()
{int ok = 0;
    /*
    if (m_pCMoteurCCAM_Base->m_USER_IDENT_TYPE_TABLE=="MedinTux")
        {
         if (lineEdit_Code->text().length() != 0) ok += 1;
         if (lineEdit_Nom->text() != "")          ok += 1;
         if (lineEdit_Prenom->text() != "")       ok += 1;
         if (lineEdit_Nid->text() != "")          ok += 1;
         if (lineEdit_Spec->text() != "")         ok += 1;
         / *if ((lineEdit_MotDePasse->text() == lineEdit_ConfMot->text())  )* / ok += 1;
        }
    else if (m_pCMoteurCCAM_Base->m_USER_IDENT_TYPE_TABLE=="SIGEMS")
    */
        {
         if (lineEdit_Code->text().length() == 6) ok += 1;
         if (lineEdit_Nom->text() != "")          ok += 1;
         if (lineEdit_Prenom->text() != "")       ok += 1;
         if (lineEdit_Nid->text() != "")          ok += 1;
         if (lineEdit_Spec->text() != "")         ok += 1;
         if ((lineEdit_MotDePasse->text() == lineEdit_ConfMot->text()) && (lineEdit_MotDePasse->text().length() > 5)) ok += 1;
        }
    if (ok == 6) {
        bouton_OK->setEnabled(TRUE);
    } else {
        bouton_OK->setEnabled(FALSE);
    }
    return ok;
}


//------------------------------------- pushButtonDelete_clicked -------------------------------------
void C_Dlg_Praticiens::pushButtonDelete_clicked()
{int ret = QMessageBox::information( this, tr("EFFACEMENT"),
                                      tr ( "Confirmez-vous l'effacement\r\n"
                                            "d'un utilisateur ??\n"
                                            "Après effacement, la récupération sera impossible\n"),
                                      tr("E&ffacer"), tr("Annu&ler"), 0,  1, 1 );
 if (ret == 1)                           return;
 Q3ListViewItem   *pQListViewItem = listview_praticiens->currentItem();
 if (pQListViewItem == 0)                return;
 if (VerifSIGEMSValiditation() == 0)     return;
 m_pCMoteurCCAM_Base->DeleteMedecin(pQListViewItem->text(6));
 EffaceSaisie();
 m_pCMoteurCCAM_Base->GetMedecins(listview_praticiens);
}

//------------------------------------- bouton_New_clicked -------------------------------------
void C_Dlg_Praticiens::bouton_New_clicked()
{if (bouton_New->text() == tr("Annu&ler création"))  // cas ou un nouvel utilisateur est en cours de creation
    {  listview_praticiens->setEnabled(TRUE);
       bouton_OK->show();
       bouton_New->setText(tr("Créer un nouvel &utilisateur"));
       EffaceSaisie();
       pushButtonDelete->show();
    }
 else
    {  listview_praticiens->setEnabled(FALSE);
       bouton_OK->show();
       EffaceSaisie();
       bouton_New->setText(tr("Annu&ler création"));
       pushButtonDelete->hide();
    }
}

//------------------------------------- EffaceSaisie -------------------------------------
void C_Dlg_Praticiens::EffaceSaisie()
{   lineEdit_Code->setText("");
    lineEdit_MotDePasse->setText("");
    lineEdit_ConfMot->setText("");
    lineEdit_Nom->setText("");
    lineEdit_Prenom->setText("");
    lineEdit_Nid->setText("");
    lineEdit_Spec->setText("");
    //bouton_OK->setEnabled(FALSE);
    return;
}

//------------------------------------- bouton_OK_clicked -------------------------------------
void C_Dlg_Praticiens::bouton_OK_clicked()
{bool ok = FALSE;
    INFO_MED info_med;
    Q3ListViewItem   * pQListView;
    if (VerifSIGEMSValiditation() == 0)     return;
    if (bouton_New->text() == tr("Annu&ler création"))  // cas ou un nouvel utilisateur est en cours de creation
       {info_med.m_Code             = lineEdit_Code->text();
        info_med.m_Password         = lineEdit_MotDePasse->text();
        info_med.m_CodeMedinTux     = lineEdit_LogMedinTux->text();
        info_med.m_PasswordMedinTux = lineEdit_MotDePasse_MedinTux->text();
        info_med.m_Nom              = lineEdit_Nom->text();
        info_med.m_Prenom           = lineEdit_Prenom->text();
        info_med.m_Nid              = lineEdit_Nid->text();
        info_med.m_Spec             = lineEdit_Spec->text();
        if (m_pCMoteurCCAM_Base->m_USER_IDENT_TYPE_TABLE=="SIGEMS") info_med.m_Pk = lineEdit_Spec->text();
        else                                                        info_med.m_Pk = "";

         ok = m_pCMoteurCCAM_Base-> CreateMedecins(info_med);
        if (ok)  m_pCMoteurCCAM_Base->GetMedecins(listview_praticiens);
        listview_praticiens->setEnabled(TRUE);
        bouton_OK->show();
        bouton_New->setText(tr("Créer un nouvel &utilisateur"));
        EffaceSaisie();
        pushButtonDelete->show();
       }
    else if ( (pQListView = listview_praticiens->currentItem()) != 0)
       {
        info_med.m_Code             = lineEdit_Code->text();
        info_med.m_Password         = lineEdit_MotDePasse->text();
        info_med.m_CodeMedinTux     = lineEdit_LogMedinTux->text();
        info_med.m_PasswordMedinTux = lineEdit_MotDePasse_MedinTux->text();
        info_med.m_Nom              = lineEdit_Nom->text();
        info_med.m_Prenom           = lineEdit_Prenom->text();
        info_med.m_Nid              = lineEdit_Nid->text();
        info_med.m_Spec             = lineEdit_Spec->text();
        info_med.m_Pk               = pQListView->text(6);

        ok = m_pCMoteurCCAM_Base->MAJMedecins(info_med);
        if (ok) { m_pCMoteurCCAM_Base->GetMedecins(listview_praticiens);
                  EffaceSaisie();
                }
       }
    return;
}

//------------------------------------- lineEdit_ConfMot_textChanged -------------------------------------
void C_Dlg_Praticiens::lineEdit_ConfMot_textChanged( const QString & )
{ VerifSIGEMSValiditation();
    return;
}

//------------------------------------- lineEdit_MotDePasse_textChanged -------------------------------------
void C_Dlg_Praticiens::lineEdit_MotDePasse_textChanged( const QString & )
{   lineEdit_ConfMot->setText("");
    lineEdit_ConfMot->setEnabled(TRUE);
    VerifSIGEMSValiditation();
    return;
}

//------------------------------------- lineEdit_Spec_textChanged -------------------------------------
void C_Dlg_Praticiens::lineEdit_Spec_textChanged( const QString & )
{   VerifSIGEMSValiditation();
    return;
}

//------------------------------------- lineEdit_Nid_textChanged -------------------------------------
void C_Dlg_Praticiens::lineEdit_Nid_textChanged( const QString & text )
{QString code_saisi = text;
    // 131703613
    if ( (lineEdit_Code->text().length() == 6 && lineEdit_Nid->text().length()>=9)||
         (m_pCMoteurCCAM_Base->m_USER_IDENT_TYPE_TABLE=="MedinTux"))
       {
        bool medecin_existe = FALSE;
        //..... recherche si le code existe .....
        medecin_existe = m_pCMoteurCCAM_Base->MedecinExiste(lineEdit_Nid->text());
        //.....on affiche le praticien selectionné dans les cases d'édition.....
        if (medecin_existe) {
            AfficheInfosMedecins(lineEdit_Nid->text());
            lineEdit_ConfMot->setText(lineEdit_MotDePasse->text());
            lineEdit_ConfMot->setEnabled(FALSE);
            return;
        } else {
            // lineEdit_MotDePasse->setText("");
            // lineEdit_ConfMot->setText("");
            // lineEdit_Nom->setText("");
            // lineEdit_Prenom->setText("");
            // lineEdit_Spec->setText("");
        }
    }

    //bouton_OK->setEnabled(FALSE);

    return;
}

//------------------------------------- lineEdit_Prenom_textChanged -------------------------------------
void C_Dlg_Praticiens::lineEdit_Prenom_textChanged( const QString & )
{ VerifSIGEMSValiditation();
    return;
}

//------------------------------------- lineEdit_Nom_textChanged -------------------------------------
void C_Dlg_Praticiens::lineEdit_Nom_textChanged( const QString & )
{ VerifSIGEMSValiditation();
    return;
}

//------------------------------------- AfficheInfosMedecins -------------------------------------
void C_Dlg_Praticiens::AfficheInfosMedecins( const QString & nidMEDECIN )
{   INFO_MED info_med;
    m_pCMoteurCCAM_Base->GetInfoMedecinsFromNid(nidMEDECIN, info_med);
    lineEdit_Code->setText(info_med.m_Code);
    lineEdit_MotDePasse->setText(info_med.m_Password);
    lineEdit_LogMedinTux->setText(info_med.m_CodeMedinTux);
    lineEdit_MotDePasse_MedinTux->setText(info_med.m_PasswordMedinTux);
    lineEdit_ConfMot_MedinTux->setText(info_med.m_PasswordMedinTux);
    lineEdit_Nom->setText(info_med.m_Nom);
    lineEdit_Prenom->setText(info_med.m_Prenom);
    lineEdit_Nid->setText(info_med.m_Nid);
    lineEdit_Spec->setText(info_med.m_Spec);
}

//------------------------------------- lineEdit_Code_textChanged -------------------------------------
void C_Dlg_Praticiens::lineEdit_Code_textChanged( const QString & )
{    VerifSIGEMSValiditation();
    return;
}

//------------------------------------- listview_praticiens_clicked -------------------------------------
void C_Dlg_Praticiens::listview_praticiens_clicked( Q3ListViewItem * pQListViewItem )
{   //.....si ce n'est pas un noeud valide cassos.....
    if (pQListViewItem==0)      return;
    //.....on affiche le praticien selectionné dans les cases d'édition.....
    AfficheInfosMedecins(pQListViewItem->text(4));
    //bouton_OK->setEnabled(FALSE);
    lineEdit_ConfMot->setEnabled(FALSE);
    return;
}

//------------------------------------- bouton_Quitter_clicked -------------------------------------
void C_Dlg_Praticiens::bouton_Quitter_clicked()
{ reject();
  return;
}



