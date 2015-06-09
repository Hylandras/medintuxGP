/****************************************************************************
** Form implementation generated from reading ui file 'Dlg_GetCodePorteur.ui'
**
** Created: dim. 17. avr. 09:34:49 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Dlg_GetCodePorteur.h"

#include <qvariant.h>
#include <qdir.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "Dlg_GetCodePorteur.ui.h"

/*
 *  Constructs a Dlg_GetCodePorteur as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Dlg_GetCodePorteur::Dlg_GetCodePorteur( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setProperty( "name", "Dlg_GetCodePorteur" );
    Dlg_GetCodePorteurLayout = new QGridLayout( this, 1, 1, 11, 6, "Dlg_GetCodePorteurLayout"); 

    labelIconePs = new QLabel( this, "labelIconePs" );
    labelIconePs->setProperty( "minimumSize", QSize( 242, 153 ) );
    labelIconePs->setProperty( "maximumSize", QSize( 242, 153 ) );

    Dlg_GetCodePorteurLayout->addMultiCellWidget( labelIconePs, 0, 2, 0, 0 );

    layout1 = new QGridLayout( 0, 1, 1, 0, 6, "layout1"); 

    pushButtonCancel = new QPushButton( this, "pushButtonCancel" );

    layout1->addWidget( pushButtonCancel, 0, 1 );

    pushButtonOk = new QPushButton( this, "pushButtonOk" );
    pushButtonOk->setProperty( "autoDefault", QVariant( TRUE, 0 ) );
    pushButtonOk->setProperty( "default", QVariant( TRUE, 0 ) );

    layout1->addWidget( pushButtonOk, 0, 0 );

    Dlg_GetCodePorteurLayout->addLayout( layout1, 2, 1 );

    lineEditGetCode = new QLineEdit( this, "lineEditGetCode" );
    lineEditGetCode->setProperty( "echoMode", "Password" );

    Dlg_GetCodePorteurLayout->addWidget( lineEditGetCode, 1, 1 );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setProperty( "paletteBackgroundColor", QColor( 182, 221, 220 ) );
    QFont textLabel2_font(  textLabel2->font() );
    textLabel2_font.setBold( TRUE );
    textLabel2->setFont( textLabel2_font ); 
    textLabel2->setProperty( "frameShape", "Box" );
    textLabel2->setProperty( "frameShadow", "Raised" );
    textLabel2->setProperty( "lineWidth", 1 );
    textLabel2->setProperty( "alignment", int( QLabel::WordBreak | QLabel::AlignCenter ) );

    Dlg_GetCodePorteurLayout->addWidget( textLabel2, 0, 1 );
    languageChange();
    resize( QSize(440, 175).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButtonOk, SIGNAL( clicked() ), this, SLOT( pushButtonOk_clicked() ) );
    connect( pushButtonCancel, SIGNAL( clicked() ), this, SLOT( pushButtonCancel_clicked() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
Dlg_GetCodePorteur::~Dlg_GetCodePorteur()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Dlg_GetCodePorteur::languageChange()
{
    setProperty( "caption", tr( "Carte CPS" ) );
    labelIconePs->setProperty( "text", QString::null );
    pushButtonCancel->setProperty( "text", tr( "Annuler" ) );
    pushButtonOk->setProperty( "text", tr( "Confirmer" ) );
    textLabel2->setProperty( "text", trUtf8( "\x56\x65\x75\x69\x6c\x6c\x65\x7a\x20\x73\x61\x69\x73\x69\x72\x20\x6c\x65\x20\x20\x63\x6f\x64\x65\x20\x70\x6f\x72\x74\x65\x75\x72\x20\x64\x65\x20\x76\x6f\x74\x72\x65\x20\x63\x61\x72\x74\x65\x20\x50\x72\x61\x74\x69\x63\x69\x65\x6e\x20\x64\x65\x20\x53\x61\x6e\x74\xc3\xa9" ) );
}

