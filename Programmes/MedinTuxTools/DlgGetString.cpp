/****************************************************************************
** Form implementation generated from reading ui file '..\..\MedinTuxTools\DlgGetString.ui'
**
** Created: jeu. 12. mai 07:19:22 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "DlgGetString.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtextbrowser.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "DlgGetString.ui.h"

/*
 *  Constructs a FormDlgGetString as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FormDlgGetString::FormDlgGetString( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setProperty( "name", "FormDlgGetString" );
    FormDlgGetStringLayout = new QGridLayout( this, 1, 1, 11, 6, "FormDlgGetStringLayout"); 

    pushButtonOK = new QPushButton( this, "pushButtonOK" );
    pushButtonOK->setProperty( "maximumSize", QSize( 100, 30 ) );

    FormDlgGetStringLayout->addWidget( pushButtonOK, 2, 0 );

    pushButtonCancel = new QPushButton( this, "pushButtonCancel" );
    pushButtonCancel->setProperty( "maximumSize", QSize( 100, 30 ) );

    FormDlgGetStringLayout->addWidget( pushButtonCancel, 2, 1 );
    spacer1 = new QSpacerItem( 302, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
    FormDlgGetStringLayout->addItem( spacer1, 2, 2 );

    lineEditString = new QLineEdit( this, "lineEditString" );

    FormDlgGetStringLayout->addMultiCellWidget( lineEditString, 1, 1, 0, 2 );

    textBrowserInfo = new QTextBrowser( this, "textBrowserInfo" );
    textBrowserInfo->setProperty( "paletteBackgroundColor", QColor( 255, 244, 198 ) );

    FormDlgGetStringLayout->addMultiCellWidget( textBrowserInfo, 0, 0, 0, 2 );
    languageChange();
    resize( QSize(496, 212).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButtonCancel, SIGNAL( pressed() ), this, SLOT( pushButtonCancel_pressed() ) );
    connect( pushButtonOK, SIGNAL( clicked() ), this, SLOT( pushButtonOK_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
FormDlgGetString::~FormDlgGetString()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FormDlgGetString::languageChange()
{
    setProperty( "caption", tr( "FormDlgGetString" ) );
    pushButtonOK->setProperty( "text", tr( "&Valider" ) );
    pushButtonOK->setProperty( "accel", QKeySequence( tr( "Alt+V" ) ) );
    pushButtonCancel->setProperty( "text", tr( "Annu&ler" ) );
    pushButtonCancel->setProperty( "accel", QKeySequence( tr( "Alt+L" ) ) );
}

