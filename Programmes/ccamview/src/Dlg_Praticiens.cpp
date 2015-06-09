#include "Dlg_Praticiens.h"

#include <qvariant.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a Dlg_Praticiens as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
Dlg_Praticiens::Dlg_Praticiens(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
Dlg_Praticiens::~Dlg_Praticiens()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Dlg_Praticiens::languageChange()
{
    retranslateUi(this);
}

void Dlg_Praticiens::bouton_Quitter_clicked()
{
    qWarning("Dlg_Praticiens::bouton_Quitter_clicked(): Not implemented yet");
}

void Dlg_Praticiens::listview_praticiens_clicked( Q3ListViewItem *)
{
    qWarning("Dlg_Praticiens::listview_praticiens_clicked( Q3ListViewItem *): Not implemented yet");
}

void Dlg_Praticiens::lineEdit_Code_textChanged( const QString &)
{
    qWarning("Dlg_Praticiens::lineEdit_Code_textChanged( const QString &): Not implemented yet");
}

void Dlg_Praticiens::AfficheInfosMedecins( const QString &)
{
    qWarning("Dlg_Praticiens::AfficheInfosMedecins( const QString &): Not implemented yet");
}

void Dlg_Praticiens::lineEdit_Nom_textChanged( const QString &)
{
    qWarning("Dlg_Praticiens::lineEdit_Nom_textChanged( const QString &): Not implemented yet");
}

void Dlg_Praticiens::lineEdit_Prenom_textChanged( const QString &)
{
    qWarning("Dlg_Praticiens::lineEdit_Prenom_textChanged( const QString &): Not implemented yet");
}

void Dlg_Praticiens::lineEdit_Nid_textChanged( const QString &)
{
    qWarning("Dlg_Praticiens::lineEdit_Nid_textChanged( const QString &): Not implemented yet");
}

void Dlg_Praticiens::lineEdit_Spec_textChanged( const QString &)
{
    qWarning("Dlg_Praticiens::lineEdit_Spec_textChanged( const QString &): Not implemented yet");
}

void Dlg_Praticiens::lineEdit_MotDePasse_textChanged( const QString &)
{
    qWarning("Dlg_Praticiens::lineEdit_MotDePasse_textChanged( const QString &): Not implemented yet");
}

void Dlg_Praticiens::lineEdit_ConfMot_textChanged( const QString &)
{
    qWarning("Dlg_Praticiens::lineEdit_ConfMot_textChanged( const QString &): Not implemented yet");
}

void Dlg_Praticiens::bouton_OK_clicked()
{
    qWarning("Dlg_Praticiens::bouton_OK_clicked(): Not implemented yet");
}

void Dlg_Praticiens::EffaceSaisie()
{
    qWarning("Dlg_Praticiens::EffaceSaisie(): Not implemented yet");
}

void Dlg_Praticiens::bouton_New_clicked()
{
    qWarning("Dlg_Praticiens::bouton_New_clicked(): Not implemented yet");
}

void Dlg_Praticiens::pushButtonDelete_clicked()
{
    qWarning("Dlg_Praticiens::pushButtonDelete_clicked(): Not implemented yet");
}

int Dlg_Praticiens::initDialog( CMoteurCCAM_Base *)
{
    qWarning("Dlg_Praticiens::initDialog( CMoteurCCAM_Base *): Not implemented yet");
    return 0;
}

int Dlg_Praticiens::VerifSIGEMSValiditation()
{
    qWarning("Dlg_Praticiens::VerifSIGEMSValiditation(): Not implemented yet");
    return 0;
}

