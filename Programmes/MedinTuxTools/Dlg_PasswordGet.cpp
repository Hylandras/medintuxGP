/****************************************************************************
** Form implementation generated from reading ui file '..\..\MedinTuxTools\Dlg_PasswordGet.ui'
**
** Created: jeu. 12. mai 07:19:21 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Dlg_PasswordGet.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a Dlg_PasswordGet as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Dlg_PasswordGet::Dlg_PasswordGet( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setProperty( "name", "Dlg_PasswordGet" );
    Dlg_PasswordGetLayout = new QGridLayout( this, 1, 1, 11, 6, "Dlg_PasswordGetLayout"); 

    layout4 = new QGridLayout( 0, 1, 1, 0, 6, "layout4"); 

    checkBoxMemorise = new QCheckBox( this, "checkBoxMemorise" );

    layout4->addWidget( checkBoxMemorise, 1, 0 );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

    pushButtonOK = new QPushButton( this, "pushButtonOK" );
    pushButtonOK->setProperty( "default", QVariant( TRUE, 0 ) );
    layout3->addWidget( pushButtonOK );

    pushButtonAnnuler = new QPushButton( this, "pushButtonAnnuler" );
    layout3->addWidget( pushButtonAnnuler );

    layout4->addLayout( layout3, 0, 0 );

    Dlg_PasswordGetLayout->addLayout( layout4, 2, 0 );

    lineEditPassword = new QLineEdit( this, "lineEditPassword" );
    lineEditPassword->setProperty( "maxLength", 8 );
    lineEditPassword->setProperty( "echoMode", "Password" );

    Dlg_PasswordGetLayout->addWidget( lineEditPassword, 0, 0 );

    textLabel_NbCarPass = new QLabel( this, "textLabel_NbCarPass" );
    QFont textLabel_NbCarPass_font(  textLabel_NbCarPass->font() );
    textLabel_NbCarPass_font.setPointSize( 9 );
    textLabel_NbCarPass_font.setBold( TRUE );
    textLabel_NbCarPass_font.setItalic( TRUE );
    textLabel_NbCarPass->setFont( textLabel_NbCarPass_font ); 
    textLabel_NbCarPass->setProperty( "alignment", int( QLabel::AlignCenter ) );

    Dlg_PasswordGetLayout->addWidget( textLabel_NbCarPass, 1, 0 );
    languageChange();
    resize( QSize(185, 116).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Dlg_PasswordGet::~Dlg_PasswordGet()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Dlg_PasswordGet::languageChange()
{
    setProperty( "caption", tr( "Saisie du mot de passe" ) );
    checkBoxMemorise->setProperty( "text", trUtf8( "\x26\x4d\xc3\xa9\x6d\x6f\x72\x69\x73\x65\x72\x20\x6c\x65\x20\x6d\x6f\x74\x20\x64\x65\x20\x70\x61\x73\x73\x65" ) );
    checkBoxMemorise->setProperty( "accel", QKeySequence( tr( "Alt+M" ) ) );
    pushButtonOK->setProperty( "text", tr( "&Confirmer" ) );
    pushButtonOK->setProperty( "accel", QKeySequence( tr( "Alt+C" ) ) );
    pushButtonAnnuler->setProperty( "text", tr( "&Annuler" ) );
    pushButtonAnnuler->setProperty( "accel", QKeySequence( tr( "Alt+A" ) ) );
    textLabel_NbCarPass->setProperty( "text", trUtf8( "\x28\x20\x68\x75\x69\x74\x20\x63\x61\x72\x61\x63\x74\xc3\xa8\x72\x65\x73\x20\x6d\x61\x78\x69\x6d\x75\x6d\x20\x29" ) );
}

