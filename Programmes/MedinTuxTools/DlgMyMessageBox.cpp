/****************************************************************************
** Form implementation generated from reading ui file '..\..\MedinTuxTools\DlgMyMessageBox.ui'
**
** Created: jeu. 12. mai 07:19:22 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "DlgMyMessageBox.h"

#include <qvariant.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "DlgMyMessageBox.ui.h"

/*
 *  Constructs a FormDlgMyMessageBox as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FormDlgMyMessageBox::FormDlgMyMessageBox( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setProperty( "name", "FormDlgMyMessageBox" );
    FormDlgMyMessageBoxLayout = new QGridLayout( this, 1, 1, 11, 6, "FormDlgMyMessageBoxLayout"); 

    textEditMesage = new QTextEdit( this, "textEditMesage" );
    textEditMesage->setProperty( "paletteForegroundColor", QColor( 0, 0, 0 ) );
    textEditMesage->setProperty( "paletteBackgroundColor", QColor( 255, 253, 202 ) );
    QFont textEditMesage_font(  textEditMesage->font() );
    textEditMesage->setFont( textEditMesage_font ); 
    textEditMesage->setProperty( "textFormat", "RichText" );
    textEditMesage->setProperty( "undoDepth", 0 );
    textEditMesage->setProperty( "readOnly", QVariant( TRUE, 0 ) );

    FormDlgMyMessageBoxLayout->addWidget( textEditMesage, 0, 0 );

    layout1 = new QVBoxLayout( 0, 0, 6, "layout1"); 

    pushButton1 = new QPushButton( this, "pushButton1" );
    layout1->addWidget( pushButton1 );

    pushButton2 = new QPushButton( this, "pushButton2" );
    layout1->addWidget( pushButton2 );

    pushButton3 = new QPushButton( this, "pushButton3" );
    layout1->addWidget( pushButton3 );

    pushButton4 = new QPushButton( this, "pushButton4" );
    layout1->addWidget( pushButton4 );

    FormDlgMyMessageBoxLayout->addLayout( layout1, 0, 1 );
    languageChange();
    resize( QSize(450, 170).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButton1, SIGNAL( clicked() ), this, SLOT( pushButton1_clicked() ) );
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( pushButton2_clicked() ) );
    connect( pushButton3, SIGNAL( clicked() ), this, SLOT( pushButton3_clicked() ) );
    connect( pushButton4, SIGNAL( clicked() ), this, SLOT( pushButton4_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
FormDlgMyMessageBox::~FormDlgMyMessageBox()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FormDlgMyMessageBox::languageChange()
{
    setProperty( "caption", tr( "MessageBox" ) );
    pushButton1->setProperty( "text", tr( "pushButton1" ) );
    pushButton2->setProperty( "text", tr( "pushButton1" ) );
    pushButton3->setProperty( "text", tr( "pushButton1" ) );
    pushButton4->setProperty( "text", tr( "pushButton1" ) );
}

