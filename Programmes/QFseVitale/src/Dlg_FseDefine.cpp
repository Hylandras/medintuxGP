/****************************************************************************
** Form implementation generated from reading ui file 'Dlg_FseDefine.ui'
**
** Created: dim. 17. avr. 09:36:47 2011
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "Dlg_FseDefine.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qtextbrowser.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qheader.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qtoolbox.h>
#include <qwidgetstack.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "Dlg_FseDefine.ui.h"

/*
 *  Constructs a Dlg_FseDefine as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Dlg_FseDefine::Dlg_FseDefine( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Dlg_FseDefine" );
    QFont f( font() );
    setFont( f ); 
    Dlg_FseDefineLayout = new QGridLayout( this, 1, 1, 0, 0, "Dlg_FseDefineLayout"); 

    splitter2 = new QSplitter( this, "splitter2" );
    splitter2->setOrientation( QSplitter::Horizontal );

    QWidget* privateLayoutWidget = new QWidget( splitter2, "layout66" );
    layout66 = new QGridLayout( privateLayoutWidget, 1, 1, 11, 6, "layout66"); 

    layout67 = new QGridLayout( 0, 1, 1, 0, 6, "layout67"); 

    pushButton_Vitale = new QPushButton( privateLayoutWidget, "pushButton_Vitale" );
    pushButton_Vitale->setMinimumSize( QSize( 60, 45 ) );
    pushButton_Vitale->setMaximumSize( QSize( 60, 45 ) );

    layout67->addWidget( pushButton_Vitale, 1, 1 );

    pushButton_ReadFileVitale = new QPushButton( privateLayoutWidget, "pushButton_ReadFileVitale" );
    pushButton_ReadFileVitale->setMaximumSize( QSize( 32767, 32767 ) );

    layout67->addMultiCellWidget( pushButton_ReadFileVitale, 0, 0, 0, 2 );
    spacer2 = new QSpacerItem( 18, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout67->addItem( spacer2, 1, 2 );
    spacer1 = new QSpacerItem( 21, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout67->addItem( spacer1, 1, 0 );

    listBox_Vitale = new QListBox( privateLayoutWidget, "listBox_Vitale" );

    layout67->addMultiCellWidget( listBox_Vitale, 2, 2, 0, 2 );

    layout66->addLayout( layout67, 2, 0 );

    pushButton_WhatIsIt = new QPushButton( privateLayoutWidget, "pushButton_WhatIsIt" );

    layout66->addWidget( pushButton_WhatIsIt, 0, 0 );

    layout65 = new QGridLayout( 0, 1, 1, 0, 6, "layout65"); 

    pushButton_OK = new QPushButton( privateLayoutWidget, "pushButton_OK" );

    layout65->addWidget( pushButton_OK, 0, 0 );

    pushButtonFormater = new QPushButton( privateLayoutWidget, "pushButtonFormater" );

    layout65->addWidget( pushButtonFormater, 3, 0 );

    pushButton_SaveVirtual = new QPushButton( privateLayoutWidget, "pushButton_SaveVirtual" );

    layout65->addWidget( pushButton_SaveVirtual, 4, 0 );

    pushButton_Annuler = new QPushButton( privateLayoutWidget, "pushButton_Annuler" );

    layout65->addWidget( pushButton_Annuler, 2, 0 );

    pushButton_DoVirtual = new QPushButton( privateLayoutWidget, "pushButton_DoVirtual" );

    layout65->addWidget( pushButton_DoVirtual, 1, 0 );

    layout66->addLayout( layout65, 3, 0 );

    layout66_2 = new QGridLayout( 0, 1, 1, 0, 6, "layout66_2"); 

    textLabel1_4 = new QLabel( privateLayoutWidget, "textLabel1_4" );
    textLabel1_4->setMaximumSize( QSize( 32767, 20 ) );
    textLabel1_4->setFrameShape( QLabel::Box );
    textLabel1_4->setAlignment( int( QLabel::AlignCenter ) );

    layout66_2->addMultiCellWidget( textLabel1_4, 2, 2, 0, 2 );
    spacer4 = new QSpacerItem( 17, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout66_2->addItem( spacer4, 1, 2 );
    spacer3 = new QSpacerItem( 22, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout66_2->addItem( spacer3, 1, 0 );

    pushButton_CPS = new QPushButton( privateLayoutWidget, "pushButton_CPS" );
    pushButton_CPS->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, pushButton_CPS->sizePolicy().hasHeightForWidth() ) );
    pushButton_CPS->setMinimumSize( QSize( 60, 45 ) );
    pushButton_CPS->setMaximumSize( QSize( 60, 45 ) );

    layout66_2->addWidget( pushButton_CPS, 1, 1 );

    lineEdit_CodePorteur = new QLineEdit( privateLayoutWidget, "lineEdit_CodePorteur" );
    lineEdit_CodePorteur->setFrameShape( QLineEdit::LineEditPanel );
    lineEdit_CodePorteur->setFrameShadow( QLineEdit::Sunken );
    lineEdit_CodePorteur->setEchoMode( QLineEdit::Password );

    layout66_2->addMultiCellWidget( lineEdit_CodePorteur, 3, 3, 0, 2 );

    pushButton_ReadFileCPS = new QPushButton( privateLayoutWidget, "pushButton_ReadFileCPS" );

    layout66_2->addMultiCellWidget( pushButton_ReadFileCPS, 0, 0, 0, 2 );

    layout66->addLayout( layout66_2, 1, 0 );

    tabWidget_Beneficiaire = new QTabWidget( splitter2, "tabWidget_Beneficiaire" );
    tabWidget_Beneficiaire->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, tabWidget_Beneficiaire->sizePolicy().hasHeightForWidth() ) );
    QFont tabWidget_Beneficiaire_font(  tabWidget_Beneficiaire->font() );
    tabWidget_Beneficiaire_font.setPointSize( 9 );
    tabWidget_Beneficiaire->setFont( tabWidget_Beneficiaire_font ); 

    Onglet_Lecture_cartes = new QWidget( tabWidget_Beneficiaire, "Onglet_Lecture_cartes" );
    Onglet_Lecture_cartesLayout = new QGridLayout( Onglet_Lecture_cartes, 1, 1, 11, 6, "Onglet_Lecture_cartesLayout"); 

    groupBox_PS_Remplacant = new QGroupBox( Onglet_Lecture_cartes, "groupBox_PS_Remplacant" );
    groupBox_PS_Remplacant->setColumnLayout(0, Qt::Vertical );
    groupBox_PS_Remplacant->layout()->setSpacing( 6 );
    groupBox_PS_Remplacant->layout()->setMargin( 11 );
    groupBox_PS_RemplacantLayout = new QGridLayout( groupBox_PS_Remplacant->layout() );
    groupBox_PS_RemplacantLayout->setAlignment( Qt::AlignTop );

    textBrowser_CPS_Remplacant = new QTextBrowser( groupBox_PS_Remplacant, "textBrowser_CPS_Remplacant" );

    groupBox_PS_RemplacantLayout->addWidget( textBrowser_CPS_Remplacant, 0, 0 );

    Onglet_Lecture_cartesLayout->addWidget( groupBox_PS_Remplacant, 1, 0 );

    groupBox_PS_Titulaire = new QGroupBox( Onglet_Lecture_cartes, "groupBox_PS_Titulaire" );
    groupBox_PS_Titulaire->setColumnLayout(0, Qt::Vertical );
    groupBox_PS_Titulaire->layout()->setSpacing( 6 );
    groupBox_PS_Titulaire->layout()->setMargin( 11 );
    groupBox_PS_TitulaireLayout = new QGridLayout( groupBox_PS_Titulaire->layout() );
    groupBox_PS_TitulaireLayout->setAlignment( Qt::AlignTop );

    layout13 = new QGridLayout( 0, 1, 1, 0, 6, "layout13"); 

    pushButtonSaveCPS_Titulaire = new QPushButton( groupBox_PS_Titulaire, "pushButtonSaveCPS_Titulaire" );
    pushButtonSaveCPS_Titulaire->setMinimumSize( QSize( 32, 32 ) );
    pushButtonSaveCPS_Titulaire->setMaximumSize( QSize( 32, 32 ) );

    layout13->addWidget( pushButtonSaveCPS_Titulaire, 0, 0 );

    pushButtonLoadCPS_Titulaire = new QPushButton( groupBox_PS_Titulaire, "pushButtonLoadCPS_Titulaire" );
    pushButtonLoadCPS_Titulaire->setMinimumSize( QSize( 32, 32 ) );
    pushButtonLoadCPS_Titulaire->setMaximumSize( QSize( 32, 32 ) );

    layout13->addWidget( pushButtonLoadCPS_Titulaire, 1, 0 );

    groupBox_PS_TitulaireLayout->addLayout( layout13, 0, 0 );

    textBrowser_CPS = new QTextBrowser( groupBox_PS_Titulaire, "textBrowser_CPS" );

    groupBox_PS_TitulaireLayout->addWidget( textBrowser_CPS, 0, 1 );

    Onglet_Lecture_cartesLayout->addWidget( groupBox_PS_Titulaire, 0, 0 );
    tabWidget_Beneficiaire->insertTab( Onglet_Lecture_cartes, QString::fromLatin1("") );

    Beneficiaire = new QWidget( tabWidget_Beneficiaire, "Beneficiaire" );
    BeneficiaireLayout = new QGridLayout( Beneficiaire, 1, 1, 11, 6, "BeneficiaireLayout"); 

    buttonGroup_ModeGestion = new QButtonGroup( Beneficiaire, "buttonGroup_ModeGestion" );
    buttonGroup_ModeGestion->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)5, 0, 0, buttonGroup_ModeGestion->sizePolicy().hasHeightForWidth() ) );
    buttonGroup_ModeGestion->setExclusive( TRUE );
    buttonGroup_ModeGestion->setColumnLayout(0, Qt::Vertical );
    buttonGroup_ModeGestion->layout()->setSpacing( 6 );
    buttonGroup_ModeGestion->layout()->setMargin( 11 );
    buttonGroup_ModeGestionLayout = new QGridLayout( buttonGroup_ModeGestion->layout() );
    buttonGroup_ModeGestionLayout->setAlignment( Qt::AlignTop );

    radioButton_GestionSeparee = new QRadioButton( buttonGroup_ModeGestion, "radioButton_GestionSeparee" );
    buttonGroup_ModeGestion->insert( radioButton_GestionSeparee, 1 );

    buttonGroup_ModeGestionLayout->addWidget( radioButton_GestionSeparee, 1, 0 );

    radioButton_GestionUnique = new QRadioButton( buttonGroup_ModeGestion, "radioButton_GestionUnique" );
    buttonGroup_ModeGestion->insert( radioButton_GestionUnique, 0 );

    buttonGroup_ModeGestionLayout->addWidget( radioButton_GestionUnique, 0, 0 );

    BeneficiaireLayout->addWidget( buttonGroup_ModeGestion, 0, 1 );

    groupBox6 = new QGroupBox( Beneficiaire, "groupBox6" );
    groupBox6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, groupBox6->sizePolicy().hasHeightForWidth() ) );
    groupBox6->setColumnLayout(0, Qt::Vertical );
    groupBox6->layout()->setSpacing( 6 );
    groupBox6->layout()->setMargin( 11 );
    groupBox6Layout = new QGridLayout( groupBox6->layout() );
    groupBox6Layout->setAlignment( Qt::AlignTop );

    layout21_2_2 = new QHBoxLayout( 0, 0, 0, "layout21_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_2_2_2 = new QLabel( groupBox6, "textLabel1_4_2_3_2_2_2_2_2_2_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout21_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_2_2_2 );

    comboBox_AMO_IndicTTT = new QComboBox( FALSE, groupBox6, "comboBox_AMO_IndicTTT" );
    comboBox_AMO_IndicTTT->setMaximumSize( QSize( 430, 20 ) );
    comboBox_AMO_IndicTTT->setSizeLimit( 15 );
    layout21_2_2->addWidget( comboBox_AMO_IndicTTT );

    groupBox6Layout->addLayout( layout21_2_2, 1, 0 );

    BeneficiaireLayout->addWidget( groupBox6, 0, 2 );

    groupBox_GestionUnique = new QGroupBox( Beneficiaire, "groupBox_GestionUnique" );
    groupBox_GestionUnique->setMinimumSize( QSize( 663, 0 ) );
    groupBox_GestionUnique->setMaximumSize( QSize( 663, 32767 ) );
    groupBox_GestionUnique->setColumnLayout(0, Qt::Vertical );
    groupBox_GestionUnique->layout()->setSpacing( 0 );
    groupBox_GestionUnique->layout()->setMargin( 11 );
    groupBox_GestionUniqueLayout = new QGridLayout( groupBox_GestionUnique->layout() );
    groupBox_GestionUniqueLayout->setAlignment( Qt::AlignTop );

    layout31_2 = new QHBoxLayout( 0, 0, 6, "layout31_2"); 

    layout13_2_2_2_2_3 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_3"); 

    textLabel1_4_2_3_2_2_2_2_3 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_3" );
    textLabel1_4_2_3_2_2_2_2_3->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_3->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_3->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_3->addWidget( textLabel1_4_2_3_2_2_2_2_3 );

    lineEdit_MUT_DtDebDroits = new QLineEdit( groupBox_GestionUnique, "lineEdit_MUT_DtDebDroits" );
    lineEdit_MUT_DtDebDroits->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_3->addWidget( lineEdit_MUT_DtDebDroits );
    layout31_2->addLayout( layout13_2_2_2_2_3 );

    layout13_2_2_2_2_2_3 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_3"); 

    textLabel1_4_2_3_2_2_2_2_2_4 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_2_4" );
    textLabel1_4_2_3_2_2_2_2_2_4->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_4->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_4->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_3->addWidget( textLabel1_4_2_3_2_2_2_2_2_4 );

    lineEdit_MUT_DtFinDroits = new QLineEdit( groupBox_GestionUnique, "lineEdit_MUT_DtFinDroits" );
    lineEdit_MUT_DtFinDroits->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_3->addWidget( lineEdit_MUT_DtFinDroits );
    layout31_2->addLayout( layout13_2_2_2_2_2_3 );

    groupBox_GestionUniqueLayout->addMultiCellLayout( layout31_2, 7, 7, 0, 1 );

    layout13_2_2_2_2_2_2_3_2_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_3_2_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2_2 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_3_2_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2_2 );

    lineEdit_MUT_AiguillageSTS = new QLineEdit( groupBox_GestionUnique, "lineEdit_MUT_AiguillageSTS" );
    lineEdit_MUT_AiguillageSTS->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_3_2_2_2_2->addWidget( lineEdit_MUT_AiguillageSTS );

    groupBox_GestionUniqueLayout->addMultiCellLayout( layout13_2_2_2_2_2_2_3_2_2_2_2, 6, 6, 0, 1 );

    layout13_2_2_2_2_2_2_3_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_3_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_3_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2 );

    lineEdit_MUT_ServicesAsso = new QLineEdit( groupBox_GestionUnique, "lineEdit_MUT_ServicesAsso" );
    lineEdit_MUT_ServicesAsso->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_3_2_2_2->addWidget( lineEdit_MUT_ServicesAsso );

    groupBox_GestionUniqueLayout->addMultiCellLayout( layout13_2_2_2_2_2_2_3_2_2_2, 5, 5, 0, 1 );

    layout13_2_2_2_2_2_2_3_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_3_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_2_3_3_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_3_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_3_2_2 );

    lineEdit_MUT_TypeService = new QLineEdit( groupBox_GestionUnique, "lineEdit_MUT_TypeService" );
    lineEdit_MUT_TypeService->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_3_2_2->addWidget( lineEdit_MUT_TypeService );

    groupBox_GestionUniqueLayout->addMultiCellLayout( layout13_2_2_2_2_2_2_3_2_2, 4, 4, 0, 1 );

    layout13_2_2_2_2_2_2_3 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_3"); 

    textLabel1_4_2_3_2_2_2_2_2_3_3 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_2_3_3" );
    textLabel1_4_2_3_2_2_2_2_2_3_3->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_3->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_3->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_3 );

    lineEdit_MUT_Numero = new QLineEdit( groupBox_GestionUnique, "lineEdit_MUT_Numero" );
    lineEdit_MUT_Numero->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_3->addWidget( lineEdit_MUT_Numero );

    groupBox_GestionUniqueLayout->addMultiCellLayout( layout13_2_2_2_2_2_2_3, 0, 0, 0, 1 );

    layout13_2_2_2_2_2_2_3_3 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_3_3"); 

    textLabel1_4_2_3_2_2_2_2_2_3_3_3 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_2_3_3_3" );
    textLabel1_4_2_3_2_2_2_2_2_3_3_3->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_3->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_3_3->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_3_3->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_3_3 );

    lineEdit_MUT_NumeroOrga = new QLineEdit( groupBox_GestionUnique, "lineEdit_MUT_NumeroOrga" );
    lineEdit_MUT_NumeroOrga->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_3_3->addWidget( lineEdit_MUT_NumeroOrga );

    groupBox_GestionUniqueLayout->addLayout( layout13_2_2_2_2_2_2_3_3, 1, 0 );

    layout13_2_2_2_2_2_2_3_3_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_3_3_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_3_3_2 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_2_3_3_3_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_3_3_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_3_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_3_3_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_3_3_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_3_3_2 );

    lineEdit_MUT_Nom = new QLineEdit( groupBox_GestionUnique, "lineEdit_MUT_Nom" );
    lineEdit_MUT_Nom->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_3_3_2->addWidget( lineEdit_MUT_Nom );

    groupBox_GestionUniqueLayout->addLayout( layout13_2_2_2_2_2_2_3_3_2, 1, 1 );

    layout13_2_2_2_2_2_2_3_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_3_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_3_2 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_2_3_3_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_3_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_3_2 );

    lineEdit_MUT_Garanties = new QLineEdit( groupBox_GestionUnique, "lineEdit_MUT_Garanties" );
    lineEdit_MUT_Garanties->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_3_2->addWidget( lineEdit_MUT_Garanties );

    groupBox_GestionUniqueLayout->addMultiCellLayout( layout13_2_2_2_2_2_2_3_2, 2, 2, 0, 1 );

    layout41 = new QHBoxLayout( 0, 0, 6, "layout41"); 

    textLabel1_4_2_3_2_2_2_2_2_2_2 = new QLabel( groupBox_GestionUnique, "textLabel1_4_2_3_2_2_2_2_2_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout41->addWidget( textLabel1_4_2_3_2_2_2_2_2_2_2 );

    comboBox_MUT_IndicTTT = new QComboBox( FALSE, groupBox_GestionUnique, "comboBox_MUT_IndicTTT" );
    comboBox_MUT_IndicTTT->setMaximumSize( QSize( 500, 20 ) );
    comboBox_MUT_IndicTTT->setSizeLimit( 15 );
    layout41->addWidget( comboBox_MUT_IndicTTT );

    groupBox_GestionUniqueLayout->addMultiCellLayout( layout41, 3, 3, 0, 1 );

    BeneficiaireLayout->addMultiCellWidget( groupBox_GestionUnique, 1, 1, 1, 2 );

    groupBoxAMC = new QGroupBox( Beneficiaire, "groupBoxAMC" );
    groupBoxAMC->setMinimumSize( QSize( 663, 0 ) );
    groupBoxAMC->setMaximumSize( QSize( 663, 32767 ) );
    groupBoxAMC->setColumnLayout(0, Qt::Vertical );
    groupBoxAMC->layout()->setSpacing( 0 );
    groupBoxAMC->layout()->setMargin( 11 );
    groupBoxAMCLayout = new QGridLayout( groupBoxAMC->layout() );
    groupBoxAMCLayout->setAlignment( Qt::AlignTop );

    layout13_2_2_2_3 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_3"); 

    textLabel1_4_2_3_2_2_2_3 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_3" );
    textLabel1_4_2_3_2_2_2_3->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_3->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_3->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_3->addWidget( textLabel1_4_2_3_2_2_2_3 );

    lineEdit_AMC_NumOrga = new QLineEdit( groupBoxAMC, "lineEdit_AMC_NumOrga" );
    lineEdit_AMC_NumOrga->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_3->addWidget( lineEdit_AMC_NumOrga );

    groupBoxAMCLayout->addLayout( layout13_2_2_2_3, 0, 0 );

    layout31 = new QHBoxLayout( 0, 0, 6, "layout31"); 

    layout13_2_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_2" );
    textLabel1_4_2_3_2_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2 );

    lineEdit_AMC_DtDebDroits = new QLineEdit( groupBoxAMC, "lineEdit_AMC_DtDebDroits" );
    lineEdit_AMC_DtDebDroits->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2->addWidget( lineEdit_AMC_DtDebDroits );
    layout31->addLayout( layout13_2_2_2_2 );

    layout13_2_2_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_2_2" );
    textLabel1_4_2_3_2_2_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2 );

    lineEdit_AMC_DtFinDroits = new QLineEdit( groupBoxAMC, "lineEdit_AMC_DtFinDroits" );
    lineEdit_AMC_DtFinDroits->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2->addWidget( lineEdit_AMC_DtFinDroits );
    layout31->addLayout( layout13_2_2_2_2_2 );

    groupBoxAMCLayout->addLayout( layout31, 2, 0 );

    layout13_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2"); 

    textLabel1_4_2_3_2_2_2 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2" );
    textLabel1_4_2_3_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2->addWidget( textLabel1_4_2_3_2_2_2 );

    lineEdit_AMC_Adherent = new QLineEdit( groupBoxAMC, "lineEdit_AMC_Adherent" );
    lineEdit_AMC_Adherent->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2->addWidget( lineEdit_AMC_Adherent );

    groupBoxAMCLayout->addLayout( layout13_2_2_2, 1, 0 );

    layout13_2_2_2_2_2_2_2_2_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_2_2_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2_2 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_2_2_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2_2 );

    lineEdit_AMC_ServicesAssocContrat = new QLineEdit( groupBoxAMC, "lineEdit_AMC_ServicesAssocContrat" );
    lineEdit_AMC_ServicesAssocContrat->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_2_2_2_2_2->addWidget( lineEdit_AMC_ServicesAssocContrat );

    groupBoxAMCLayout->addLayout( layout13_2_2_2_2_2_2_2_2_2_2_2, 10, 0 );

    layout13_2_2_2_2_2_2_2_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_2_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_2_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2 );

    lineEdit_AMC_ServicesAssocType = new QLineEdit( groupBoxAMC, "lineEdit_AMC_ServicesAssocType" );
    lineEdit_AMC_ServicesAssocType->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_2_2_2_2->addWidget( lineEdit_AMC_ServicesAssocType );

    groupBoxAMCLayout->addLayout( layout13_2_2_2_2_2_2_2_2_2_2, 9, 0 );

    layout13_2_2_2_2_2_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_2_2 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_2_2_3_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_2_2 );

    lineEdit_AMC_NomDomaine = new QLineEdit( groupBoxAMC, "lineEdit_AMC_NomDomaine" );
    lineEdit_AMC_NomDomaine->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_2_2->addWidget( lineEdit_AMC_NomDomaine );

    groupBoxAMCLayout->addLayout( layout13_2_2_2_2_2_2_2_2, 8, 0 );

    layout13_2_2_2_2_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_2 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_2_2_3_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_2 );

    lineEdit_AMC_IdHote = new QLineEdit( groupBoxAMC, "lineEdit_AMC_IdHote" );
    lineEdit_AMC_IdHote->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_2->addWidget( lineEdit_AMC_IdHote );

    groupBoxAMCLayout->addLayout( layout13_2_2_2_2_2_2_2, 7, 0 );

    layout13_2_2_2_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_2_2_3" );
    textLabel1_4_2_3_2_2_2_2_2_3->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3 );

    lineEdit_AMC_CodeRoutage = new QLineEdit( groupBoxAMC, "lineEdit_AMC_CodeRoutage" );
    lineEdit_AMC_CodeRoutage->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2->addWidget( lineEdit_AMC_CodeRoutage );

    groupBoxAMCLayout->addLayout( layout13_2_2_2_2_2_2, 6, 0 );

    layout21 = new QHBoxLayout( 0, 0, 6, "layout21"); 

    textLabel1_4_2_3_2_2_2_2_2_2 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_2_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout21->addWidget( textLabel1_4_2_3_2_2_2_2_2_2 );

    comboBox_AMC_IndicTTT = new QComboBox( FALSE, groupBoxAMC, "comboBox_AMC_IndicTTT" );
    comboBox_AMC_IndicTTT->setMinimumSize( QSize( 0, 0 ) );
    comboBox_AMC_IndicTTT->setMaximumSize( QSize( 500, 32767 ) );
    comboBox_AMC_IndicTTT->setSizeLimit( 15 );
    layout21->addWidget( comboBox_AMC_IndicTTT );

    groupBoxAMCLayout->addLayout( layout21, 5, 0 );

    layout13_2_2_2_2_2_2_2_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_2_2_2_2_2_2_2"); 

    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2 = new QLabel( groupBoxAMC, "textLabel1_4_2_3_2_2_2_2_2_3_2_2_2" );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_2_2_2_2_2_2_2->addWidget( textLabel1_4_2_3_2_2_2_2_2_3_2_2_2 );

    lineEdit_AMC_AiguillageSTS = new QLineEdit( groupBoxAMC, "lineEdit_AMC_AiguillageSTS" );
    lineEdit_AMC_AiguillageSTS->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_2_2_2_2_2_2_2->addWidget( lineEdit_AMC_AiguillageSTS );

    groupBoxAMCLayout->addLayout( layout13_2_2_2_2_2_2_2_2_2, 4, 0 );

    textLabel_AMC_SesamVitale = new QLabel( groupBoxAMC, "textLabel_AMC_SesamVitale" );
    textLabel_AMC_SesamVitale->setFrameShape( QLabel::TabWidgetPanel );

    groupBoxAMCLayout->addWidget( textLabel_AMC_SesamVitale, 3, 0 );

    BeneficiaireLayout->addMultiCellWidget( groupBoxAMC, 2, 2, 1, 2 );

    textBrowser_Beneficiaire = new QTextBrowser( Beneficiaire, "textBrowser_Beneficiaire" );
    textBrowser_Beneficiaire->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, textBrowser_Beneficiaire->sizePolicy().hasHeightForWidth() ) );
    textBrowser_Beneficiaire->setMinimumSize( QSize( 200, 0 ) );

    BeneficiaireLayout->addMultiCellWidget( textBrowser_Beneficiaire, 0, 2, 0, 0 );
    tabWidget_Beneficiaire->insertTab( Beneficiaire, QString::fromLatin1("") );

    Onglet_FSE = new QWidget( tabWidget_Beneficiaire, "Onglet_FSE" );
    Onglet_FSELayout = new QGridLayout( Onglet_FSE, 1, 1, 11, 6, "Onglet_FSELayout"); 

    layout73 = new QGridLayout( 0, 1, 1, 0, 6, "layout73"); 

    layout66_3 = new QHBoxLayout( 0, 0, 6, "layout66_3"); 

    groupBox1 = new QGroupBox( Onglet_FSE, "groupBox1" );
    groupBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, groupBox1->sizePolicy().hasHeightForWidth() ) );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 0 );
    groupBox1->layout()->setMargin( 5 );
    groupBox1Layout = new QGridLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    layout11_2 = new QGridLayout( 0, 1, 1, 0, 6, "layout11_2"); 

    pushButton_AT_DateDebut = new QPushButton( groupBox1, "pushButton_AT_DateDebut" );
    pushButton_AT_DateDebut->setMaximumSize( QSize( 19, 19 ) );

    layout11_2->addWidget( pushButton_AT_DateDebut, 0, 2 );

    lineEdit_ArretTravailDateDebut = new QLineEdit( groupBox1, "lineEdit_ArretTravailDateDebut" );
    lineEdit_ArretTravailDateDebut->setReadOnly( TRUE );

    layout11_2->addWidget( lineEdit_ArretTravailDateDebut, 0, 1 );

    textLabel1_4_2_2_2 = new QLabel( groupBox1, "textLabel1_4_2_2_2" );
    textLabel1_4_2_2_2->setMinimumSize( QSize( 40, 0 ) );
    textLabel1_4_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_2_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    layout11_2->addWidget( textLabel1_4_2_2_2, 0, 0 );

    groupBox1Layout->addLayout( layout11_2, 0, 0 );

    layout11_2_2 = new QGridLayout( 0, 1, 1, 0, 6, "layout11_2_2"); 

    pushButton_AT_DateFin = new QPushButton( groupBox1, "pushButton_AT_DateFin" );
    pushButton_AT_DateFin->setMaximumSize( QSize( 19, 19 ) );

    layout11_2_2->addWidget( pushButton_AT_DateFin, 0, 2 );

    lineEdit_ArretTravailDateFin = new QLineEdit( groupBox1, "lineEdit_ArretTravailDateFin" );
    lineEdit_ArretTravailDateFin->setReadOnly( TRUE );

    layout11_2_2->addWidget( lineEdit_ArretTravailDateFin, 0, 1 );

    textLabel1_4_2_2_2_2 = new QLabel( groupBox1, "textLabel1_4_2_2_2_2" );
    textLabel1_4_2_2_2_2->setMinimumSize( QSize( 40, 0 ) );
    textLabel1_4_2_2_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_2_2_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    layout11_2_2->addWidget( textLabel1_4_2_2_2_2, 0, 0 );

    groupBox1Layout->addLayout( layout11_2_2, 0, 1 );

    layout13_2_2 = new QHBoxLayout( 0, 0, 6, "layout13_2_2"); 

    textLabel1_4_2_3_2_2 = new QLabel( groupBox1, "textLabel1_4_2_3_2_2" );
    textLabel1_4_2_3_2_2->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2->addWidget( textLabel1_4_2_3_2_2 );

    lineEdit_Nbr_ArretTravail = new QLineEdit( groupBox1, "lineEdit_Nbr_ArretTravail" );
    lineEdit_Nbr_ArretTravail->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2->addWidget( lineEdit_Nbr_ArretTravail );

    groupBox1Layout->addLayout( layout13_2_2, 0, 2 );
    layout66_3->addWidget( groupBox1 );

    groupBox1_2 = new QGroupBox( Onglet_FSE, "groupBox1_2" );
    groupBox1_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, groupBox1_2->sizePolicy().hasHeightForWidth() ) );
    groupBox1_2->setColumnLayout(0, Qt::Vertical );
    groupBox1_2->layout()->setSpacing( 0 );
    groupBox1_2->layout()->setMargin( 5 );
    groupBox1_2Layout = new QGridLayout( groupBox1_2->layout() );
    groupBox1_2Layout->setAlignment( Qt::AlignTop );

    comboBox_CodeCarnetMedical = new QComboBox( FALSE, groupBox1_2, "comboBox_CodeCarnetMedical" );
    comboBox_CodeCarnetMedical->setMinimumSize( QSize( 0, 20 ) );
    comboBox_CodeCarnetMedical->setMaximumSize( QSize( 146, 23 ) );

    groupBox1_2Layout->addWidget( comboBox_CodeCarnetMedical, 0, 0 );
    layout66_3->addWidget( groupBox1_2 );

    layout73->addLayout( layout66_3, 3, 0 );

    groupBox9 = new QGroupBox( Onglet_FSE, "groupBox9" );
    groupBox9->setColumnLayout(0, Qt::Vertical );
    groupBox9->layout()->setSpacing( 0 );
    groupBox9->layout()->setMargin( 2 );
    groupBox9Layout = new QGridLayout( groupBox9->layout() );
    groupBox9Layout->setAlignment( Qt::AlignTop );

    layout70 = new QHBoxLayout( 0, 0, 6, "layout70"); 

    layout69 = new QVBoxLayout( 0, 0, 0, "layout69"); 

    textLabel6 = new QLabel( groupBox9, "textLabel6" );
    layout69->addWidget( textLabel6 );

    textLabel7 = new QLabel( groupBox9, "textLabel7" );
    layout69->addWidget( textLabel7 );

    textLabel8 = new QLabel( groupBox9, "textLabel8" );
    layout69->addWidget( textLabel8 );
    layout70->addLayout( layout69 );

    layout68 = new QVBoxLayout( 0, 0, 0, "layout68"); 

    lineEdit_TPAMO = new QLineEdit( groupBox9, "lineEdit_TPAMO" );
    lineEdit_TPAMO->setMaximumSize( QSize( 32767, 20 ) );
    lineEdit_TPAMO->setFrameShape( QLineEdit::MenuBarPanel );
    lineEdit_TPAMO->setReadOnly( TRUE );
    layout68->addWidget( lineEdit_TPAMO );

    lineEdit_TPAMC = new QLineEdit( groupBox9, "lineEdit_TPAMC" );
    lineEdit_TPAMC->setMaximumSize( QSize( 32767, 20 ) );
    lineEdit_TPAMC->setFrameShape( QLineEdit::MenuBarPanel );
    lineEdit_TPAMC->setFrameShadow( QLineEdit::Plain );
    lineEdit_TPAMC->setReadOnly( TRUE );
    layout68->addWidget( lineEdit_TPAMC );

    lineEdit_ChargeAssure = new QLineEdit( groupBox9, "lineEdit_ChargeAssure" );
    lineEdit_ChargeAssure->setMaximumSize( QSize( 32767, 20 ) );
    lineEdit_ChargeAssure->setFrameShape( QLineEdit::MenuBarPanel );
    lineEdit_ChargeAssure->setReadOnly( TRUE );
    layout68->addWidget( lineEdit_ChargeAssure );
    layout70->addLayout( layout68 );

    groupBox9Layout->addLayout( layout70, 0, 0 );

    layout71 = new QVBoxLayout( 0, 0, 0, "layout71"); 

    textLabel9 = new QLabel( groupBox9, "textLabel9" );
    textLabel9->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, textLabel9->sizePolicy().hasHeightForWidth() ) );
    textLabel9->setMinimumSize( QSize( 110, 0 ) );
    textLabel9->setTextFormat( QLabel::AutoText );
    textLabel9->setScaledContents( TRUE );
    textLabel9->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout71->addWidget( textLabel9 );

    textLabel9_2 = new QLabel( groupBox9, "textLabel9_2" );
    textLabel9_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, textLabel9_2->sizePolicy().hasHeightForWidth() ) );
    textLabel9_2->setTextFormat( QLabel::AutoText );
    textLabel9_2->setScaledContents( TRUE );
    textLabel9_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout71->addWidget( textLabel9_2 );

    textLabel9_2_2 = new QLabel( groupBox9, "textLabel9_2_2" );
    textLabel9_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, textLabel9_2_2->sizePolicy().hasHeightForWidth() ) );
    textLabel9_2_2->setScaledContents( TRUE );
    textLabel9_2_2->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout71->addWidget( textLabel9_2_2 );

    groupBox9Layout->addLayout( layout71, 0, 1 );

    layout72 = new QVBoxLayout( 0, 0, 0, "layout72"); 

    lineEdit_FactTotal = new QLineEdit( groupBox9, "lineEdit_FactTotal" );
    lineEdit_FactTotal->setFrameShape( QLineEdit::MenuBarPanel );
    lineEdit_FactTotal->setReadOnly( TRUE );
    layout72->addWidget( lineEdit_FactTotal );

    lineEdit_FactVersement = new QLineEdit( groupBox9, "lineEdit_FactVersement" );
    lineEdit_FactVersement->setFrameShape( QLineEdit::MenuBarPanel );
    lineEdit_FactVersement->setReadOnly( TRUE );
    layout72->addWidget( lineEdit_FactVersement );

    lineEdit_FactVersement_2 = new QLineEdit( groupBox9, "lineEdit_FactVersement_2" );
    lineEdit_FactVersement_2->setFrameShape( QLineEdit::MenuBarPanel );
    lineEdit_FactVersement_2->setReadOnly( TRUE );
    layout72->addWidget( lineEdit_FactVersement_2 );

    groupBox9Layout->addLayout( layout72, 0, 2 );

    textLabel9_2_3 = new QLabel( groupBox9, "textLabel9_2_3" );

    groupBox9Layout->addWidget( textLabel9_2_3, 0, 3 );

    comboBox_CodeCarnetMedical_2 = new QComboBox( FALSE, groupBox9, "comboBox_CodeCarnetMedical_2" );
    comboBox_CodeCarnetMedical_2->setMinimumSize( QSize( 0, 20 ) );
    comboBox_CodeCarnetMedical_2->setMaximumSize( QSize( 70, 23 ) );

    groupBox9Layout->addWidget( comboBox_CodeCarnetMedical_2, 0, 4 );

    layout73->addWidget( groupBox9, 2, 0 );

    listView_Actes = new QListView( Onglet_FSE, "listView_Actes" );
    listView_Actes->addColumn( tr( "Date" ) );
    listView_Actes->addColumn( tr( "Codes CCAM" ) );
    listView_Actes->addColumn( tr( "C,CS,CNPSY" ) );
    listView_Actes->addColumn( tr( "V,VS,VNPSY" ) );
    listView_Actes->addColumn( tr( "Autres (K,Z,Cs,CP...)" ) );
    listView_Actes->addColumn( tr( "Montant" ) );
    listView_Actes->addColumn( trUtf8( "\x44\xc3\xa9\x70\x2e" ) );
    listView_Actes->addColumn( tr( "ID" ) );
    listView_Actes->addColumn( tr( "IK (nbre)" ) );
    listView_Actes->addColumn( tr( "IK (Montant)" ) );

    layout73->addWidget( listView_Actes, 1, 0 );

    textLabel2_2 = new QLabel( Onglet_FSE, "textLabel2_2" );
    textLabel2_2->setFrameShape( QLabel::NoFrame );
    textLabel2_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    layout73->addWidget( textLabel2_2, 0, 0 );

    Onglet_FSELayout->addMultiCellLayout( layout73, 2, 2, 0, 1 );

    tabWidget3 = new QTabWidget( Onglet_FSE, "tabWidget3" );

    tab_Actes = new QWidget( tabWidget3, "tab_Actes" );
    tab_ActesLayout = new QGridLayout( tab_Actes, 1, 1, 0, 0, "tab_ActesLayout"); 

    groupBox10 = new QGroupBox( tab_Actes, "groupBox10" );
    groupBox10->setMaximumSize( QSize( 315, 32767 ) );
    groupBox10->setColumnLayout(0, Qt::Vertical );
    groupBox10->layout()->setSpacing( 0 );
    groupBox10->layout()->setMargin( 5 );
    groupBox10Layout = new QGridLayout( groupBox10->layout() );
    groupBox10Layout->setAlignment( Qt::AlignTop );

    layout68_2 = new QGridLayout( 0, 1, 1, 0, 6, "layout68_2"); 

    layout77 = new QVBoxLayout( 0, 0, 0, "layout77"); 

    textLabel_ActeEnCours_Tarif = new QLabel( groupBox10, "textLabel_ActeEnCours_Tarif" );
    layout77->addWidget( textLabel_ActeEnCours_Tarif );

    comboBox_ActeEnCours_Code = new QComboBox( FALSE, groupBox10, "comboBox_ActeEnCours_Code" );
    comboBox_ActeEnCours_Code->setMaximumSize( QSize( 131, 32767 ) );
    layout77->addWidget( comboBox_ActeEnCours_Code );

    layout68_2->addLayout( layout77, 0, 0 );

    layout67_2 = new QVBoxLayout( 0, 0, 6, "layout67_2"); 

    textLabel15_2 = new QLabel( groupBox10, "textLabel15_2" );
    layout67_2->addWidget( textLabel15_2 );

    textLabel_ActeEnCours_Prix = new QLabel( groupBox10, "textLabel_ActeEnCours_Prix" );
    textLabel_ActeEnCours_Prix->setMinimumSize( QSize( 50, 0 ) );
    QFont textLabel_ActeEnCours_Prix_font(  textLabel_ActeEnCours_Prix->font() );
    textLabel_ActeEnCours_Prix_font.setBold( TRUE );
    textLabel_ActeEnCours_Prix->setFont( textLabel_ActeEnCours_Prix_font ); 
    textLabel_ActeEnCours_Prix->setFrameShape( QLabel::GroupBoxPanel );
    textLabel_ActeEnCours_Prix->setAlignment( int( QLabel::AlignCenter ) );
    layout67_2->addWidget( textLabel_ActeEnCours_Prix );

    layout68_2->addLayout( layout67_2, 0, 3 );

    layout79 = new QVBoxLayout( 0, 0, 0, "layout79"); 

    textLabel12 = new QLabel( groupBox10, "textLabel12" );
    layout79->addWidget( textLabel12 );

    comboBox_ActeEnCours_Nb = new QComboBox( FALSE, groupBox10, "comboBox_ActeEnCours_Nb" );
    comboBox_ActeEnCours_Nb->setMaximumSize( QSize( 41, 20 ) );
    comboBox_ActeEnCours_Nb->setEditable( TRUE );
    layout79->addWidget( comboBox_ActeEnCours_Nb );

    layout68_2->addLayout( layout79, 0, 1 );

    layout80 = new QVBoxLayout( 0, 0, 0, "layout80"); 

    textLabel15 = new QLabel( groupBox10, "textLabel15" );
    layout80->addWidget( textLabel15 );

    lineEdit_ActeEnCours_Coef = new QLineEdit( groupBox10, "lineEdit_ActeEnCours_Coef" );
    lineEdit_ActeEnCours_Coef->setAlignment( int( QLineEdit::AlignHCenter ) );
    layout80->addWidget( lineEdit_ActeEnCours_Coef );

    layout68_2->addLayout( layout80, 0, 2 );

    groupBox10Layout->addMultiCellLayout( layout68_2, 1, 1, 0, 1 );

    layout69_2 = new QHBoxLayout( 0, 0, 6, "layout69_2"); 

    layout77_2 = new QVBoxLayout( 0, 0, 0, "layout77_2"); 

    textLabel_ActeEnCours_Tarif_2 = new QLabel( groupBox10, "textLabel_ActeEnCours_Tarif_2" );
    layout77_2->addWidget( textLabel_ActeEnCours_Tarif_2 );

    comboBox_ActeEnCours_Majoration = new QComboBox( FALSE, groupBox10, "comboBox_ActeEnCours_Majoration" );
    layout77_2->addWidget( comboBox_ActeEnCours_Majoration );
    layout69_2->addLayout( layout77_2 );
    spacer10_2 = new QSpacerItem( 31, 10, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout69_2->addItem( spacer10_2 );

    textLabel_ActeEnCours_Prix_2 = new QLabel( groupBox10, "textLabel_ActeEnCours_Prix_2" );
    textLabel_ActeEnCours_Prix_2->setMinimumSize( QSize( 50, 0 ) );
    textLabel_ActeEnCours_Prix_2->setMaximumSize( QSize( 50, 20 ) );
    QFont textLabel_ActeEnCours_Prix_2_font(  textLabel_ActeEnCours_Prix_2->font() );
    textLabel_ActeEnCours_Prix_2_font.setBold( TRUE );
    textLabel_ActeEnCours_Prix_2->setFont( textLabel_ActeEnCours_Prix_2_font ); 
    textLabel_ActeEnCours_Prix_2->setFrameShape( QLabel::GroupBoxPanel );
    textLabel_ActeEnCours_Prix_2->setAlignment( int( QLabel::AlignCenter ) );
    layout69_2->addWidget( textLabel_ActeEnCours_Prix_2 );

    groupBox10Layout->addMultiCellLayout( layout69_2, 2, 2, 0, 1 );

    textBrowser_ActeEnCours = new QTextBrowser( groupBox10, "textBrowser_ActeEnCours" );

    groupBox10Layout->addWidget( textBrowser_ActeEnCours, 3, 1 );

    textBrowserHelpActeEnCours = new QTextBrowser( groupBox10, "textBrowserHelpActeEnCours" );
    textBrowserHelpActeEnCours->setFrameShape( QTextBrowser::Box );
    textBrowserHelpActeEnCours->setFrameShadow( QTextBrowser::Plain );

    groupBox10Layout->addMultiCellWidget( textBrowserHelpActeEnCours, 0, 0, 0, 1 );

    layout77_2_2 = new QVBoxLayout( 0, 0, 0, "layout77_2_2"); 

    textLabel_ActeEnCours_Tarif_2_2 = new QLabel( groupBox10, "textLabel_ActeEnCours_Tarif_2_2" );
    textLabel_ActeEnCours_Tarif_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)4, 0, 0, textLabel_ActeEnCours_Tarif_2_2->sizePolicy().hasHeightForWidth() ) );
    layout77_2_2->addWidget( textLabel_ActeEnCours_Tarif_2_2 );

    comboBox_ActeEnCours_Nature = new QComboBox( FALSE, groupBox10, "comboBox_ActeEnCours_Nature" );
    layout77_2_2->addWidget( comboBox_ActeEnCours_Nature );

    groupBox10Layout->addLayout( layout77_2_2, 3, 0 );

    pushButton_ActeEnCours_Valider = new QPushButton( groupBox10, "pushButton_ActeEnCours_Valider" );

    groupBox10Layout->addWidget( pushButton_ActeEnCours_Valider, 7, 0 );

    pushButton_ActeEnCours_Ajouter = new QPushButton( groupBox10, "pushButton_ActeEnCours_Ajouter" );

    groupBox10Layout->addWidget( pushButton_ActeEnCours_Ajouter, 6, 0 );

    layout82 = new QHBoxLayout( 0, 0, 6, "layout82"); 

    textLabel9_3 = new QLabel( groupBox10, "textLabel9_3" );
    textLabel9_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)5, 0, 0, textLabel9_3->sizePolicy().hasHeightForWidth() ) );
    textLabel9_3->setMinimumSize( QSize( 0, 0 ) );
    textLabel9_3->setTextFormat( QLabel::AutoText );
    textLabel9_3->setScaledContents( TRUE );
    textLabel9_3->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );
    layout82->addWidget( textLabel9_3 );

    lineEdit_ActeEnCours_Total = new QLineEdit( groupBox10, "lineEdit_ActeEnCours_Total" );
    lineEdit_ActeEnCours_Total->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)0, 0, 0, lineEdit_ActeEnCours_Total->sizePolicy().hasHeightForWidth() ) );
    layout82->addWidget( lineEdit_ActeEnCours_Total );

    groupBox10Layout->addMultiCellLayout( layout82, 4, 4, 0, 1 );

    layout71_2 = new QVBoxLayout( 0, 0, 6, "layout71_2"); 

    checkBox_ActeEnCours_Entente = new QCheckBox( groupBox10, "checkBox_ActeEnCours_Entente" );
    layout71_2->addWidget( checkBox_ActeEnCours_Entente );

    checkBox_ActeEnCours_Cabinet = new QCheckBox( groupBox10, "checkBox_ActeEnCours_Cabinet" );
    layout71_2->addWidget( checkBox_ActeEnCours_Cabinet );

    checkBox_ActeEnCours_RMO = new QCheckBox( groupBox10, "checkBox_ActeEnCours_RMO" );
    layout71_2->addWidget( checkBox_ActeEnCours_RMO );

    groupBox10Layout->addMultiCellLayout( layout71_2, 5, 5, 0, 1 );

    tab_ActesLayout->addWidget( groupBox10, 0, 0 );
    tabWidget3->insertTab( tab_Actes, QString::fromLatin1("") );

    TabActeParam = new QWidget( tabWidget3, "TabActeParam" );

    QWidget* privateLayoutWidget_2 = new QWidget( TabActeParam, "layout66" );
    privateLayoutWidget_2->setGeometry( QRect( 20, 19, 107, 270 ) );
    layout66_4 = new QVBoxLayout( privateLayoutWidget_2, 11, 6, "layout66_4"); 

    textLabel_TitleLisActeBase = new QLabel( privateLayoutWidget_2, "textLabel_TitleLisActeBase" );
    textLabel_TitleLisActeBase->setFrameShape( QLabel::Box );
    textLabel_TitleLisActeBase->setFrameShadow( QLabel::Plain );
    textLabel_TitleLisActeBase->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
    layout66_4->addWidget( textLabel_TitleLisActeBase );

    listBox_ActesDispos = new QListBox( privateLayoutWidget_2, "listBox_ActesDispos" );
    layout66_4->addWidget( listBox_ActesDispos );

    textLabel_LibelleActeBase = new QLabel( TabActeParam, "textLabel_LibelleActeBase" );
    textLabel_LibelleActeBase->setGeometry( QRect( 20, 295, 260, 21 ) );
    tabWidget3->insertTab( TabActeParam, QString::fromLatin1("") );

    TabAide = new QWidget( tabWidget3, "TabAide" );

    textBrowser_FSE_Aide = new QTextBrowser( TabAide, "textBrowser_FSE_Aide" );
    textBrowser_FSE_Aide->setGeometry( QRect( 11, 11, 301, 366 ) );
    tabWidget3->insertTab( TabAide, QString::fromLatin1("") );

    Onglet_FSELayout->addMultiCellWidget( tabWidget3, 2, 2, 2, 3 );

    groupBox7 = new QGroupBox( Onglet_FSE, "groupBox7" );
    groupBox7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, groupBox7->sizePolicy().hasHeightForWidth() ) );
    groupBox7->setAlignment( int( QGroupBox::AlignHCenter ) );
    groupBox7->setColumnLayout(0, Qt::Vertical );
    groupBox7->layout()->setSpacing( 0 );
    groupBox7->layout()->setMargin( 2 );
    groupBox7Layout = new QGridLayout( groupBox7->layout() );
    groupBox7Layout->setAlignment( Qt::AlignTop );

    toolBox_PEC = new QToolBox( groupBox7, "toolBox_PEC" );
    toolBox_PEC->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)7, 0, 0, toolBox_PEC->sizePolicy().hasHeightForWidth() ) );
    toolBox_PEC->setMinimumSize( QSize( 90, 0 ) );
    toolBox_PEC->setMaximumSize( QSize( 32767, 126 ) );
    toolBox_PEC->setFrameShape( QToolBox::NoFrame );
    toolBox_PEC->setCurrentIndex( 0 );

    Maladie = new QWidget( toolBox_PEC, "Maladie" );
    Maladie->setBackgroundMode( QWidget::PaletteBrightText );
    toolBox_PEC->addItem( Maladie, QString::fromLatin1("") );

    Maternite = new QWidget( toolBox_PEC, "Maternite" );
    Maternite->setBackgroundMode( QWidget::PaletteBrightText );
    toolBox_PEC->addItem( Maternite, QString::fromLatin1("") );

    Accident_de_travail = new QWidget( toolBox_PEC, "Accident_de_travail" );
    Accident_de_travail->setBackgroundMode( QWidget::PaletteBrightText );
    toolBox_PEC->addItem( Accident_de_travail, QString::fromLatin1("") );

    groupBox7Layout->addWidget( toolBox_PEC, 0, 0 );

    widgetStack_PEC = new QWidgetStack( groupBox7, "widgetStack_PEC" );
    widgetStack_PEC->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)3, 0, 0, widgetStack_PEC->sizePolicy().hasHeightForWidth() ) );
    widgetStack_PEC->setMaximumSize( QSize( 32767, 123 ) );
    widgetStack_PEC->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    widgetStack_PEC->setBackgroundOrigin( QWidgetStack::WidgetOrigin );
    widgetStack_PEC->setFrameShape( QWidgetStack::Box );
    widgetStack_PEC->setLineWidth( 1 );

    Maladie_2 = new QWidget( widgetStack_PEC, "Maladie_2" );
    MaladieLayout = new QGridLayout( Maladie_2, 1, 1, 0, 0, "MaladieLayout"); 

    textLabel5_2_2 = new QLabel( Maladie_2, "textLabel5_2_2" );
    textLabel5_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)4, 0, 0, textLabel5_2_2->sizePolicy().hasHeightForWidth() ) );
    textLabel5_2_2->setMaximumSize( QSize( 32767, 19 ) );

    MaladieLayout->addWidget( textLabel5_2_2, 0, 0 );
    spacer14 = new QSpacerItem( 16, 67, QSizePolicy::Minimum, QSizePolicy::Expanding );
    MaladieLayout->addItem( spacer14, 1, 0 );

    layout68_3 = new QHBoxLayout( 0, 0, 6, "layout68_3"); 

    layout50 = new QVBoxLayout( 0, 0, 6, "layout50"); 

    textLabel1_5 = new QLabel( Maladie_2, "textLabel1_5" );
    layout50->addWidget( textLabel1_5 );

    textLabel1_5_2 = new QLabel( Maladie_2, "textLabel1_5_2" );
    layout50->addWidget( textLabel1_5_2 );
    layout68_3->addLayout( layout50 );

    layout46 = new QVBoxLayout( 0, 0, 6, "layout46"); 

    checkBox_ExoTicketMod_Non = new QCheckBox( Maladie_2, "checkBox_ExoTicketMod_Non" );
    layout46->addWidget( checkBox_ExoTicketMod_Non );

    checkBox_AccParTiers_Non = new QCheckBox( Maladie_2, "checkBox_AccParTiers_Non" );
    checkBox_AccParTiers_Non->setAutoMask( FALSE );
    layout46->addWidget( checkBox_AccParTiers_Non );
    layout68_3->addLayout( layout46 );

    layout45 = new QVBoxLayout( 0, 0, 6, "layout45"); 

    checkBox_ExoTicketMod_Oui = new QCheckBox( Maladie_2, "checkBox_ExoTicketMod_Oui" );
    layout45->addWidget( checkBox_ExoTicketMod_Oui );

    checkBox_AccParTiers_Oui = new QCheckBox( Maladie_2, "checkBox_AccParTiers_Oui" );
    layout45->addWidget( checkBox_AccParTiers_Oui );
    layout68_3->addLayout( layout45 );

    layout67_3 = new QVBoxLayout( 0, 0, 6, "layout67_3"); 

    textLabel2 = new QLabel( Maladie_2, "textLabel2" );
    layout67_3->addWidget( textLabel2 );

    layout65_2 = new QHBoxLayout( 0, 0, 6, "layout65_2"); 

    pushButton_FSE_AccParTiers_Calendrier = new QPushButton( Maladie_2, "pushButton_FSE_AccParTiers_Calendrier" );
    pushButton_FSE_AccParTiers_Calendrier->setMaximumSize( QSize( 51, 19 ) );
    layout65_2->addWidget( pushButton_FSE_AccParTiers_Calendrier );

    lineEdit_FSE_AccParTiers_Date = new QLineEdit( Maladie_2, "lineEdit_FSE_AccParTiers_Date" );
    lineEdit_FSE_AccParTiers_Date->setMinimumSize( QSize( 0, 20 ) );
    lineEdit_FSE_AccParTiers_Date->setReadOnly( TRUE );
    layout65_2->addWidget( lineEdit_FSE_AccParTiers_Date );
    layout67_3->addLayout( layout65_2 );
    layout68_3->addLayout( layout67_3 );

    MaladieLayout->addLayout( layout68_3, 2, 0 );
    widgetStack_PEC->addWidget( Maladie_2, 0 );

    Maternite_2 = new QWidget( widgetStack_PEC, "Maternite_2" );
    MaterniteLayout = new QGridLayout( Maternite_2, 1, 1, 11, 6, "MaterniteLayout"); 

    textLabel5 = new QLabel( Maternite_2, "textLabel5" );
    textLabel5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)4, 0, 0, textLabel5->sizePolicy().hasHeightForWidth() ) );
    textLabel5->setMaximumSize( QSize( 32767, 19 ) );

    MaterniteLayout->addWidget( textLabel5, 0, 0 );
    spacer10 = new QSpacerItem( 16, 21, QSizePolicy::Minimum, QSizePolicy::Expanding );
    MaterniteLayout->addItem( spacer10, 1, 0 );
    spacer11 = new QSpacerItem( 16, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    MaterniteLayout->addItem( spacer11, 3, 0 );

    layout70_2 = new QHBoxLayout( 0, 0, 6, "layout70_2"); 

    pushButton_DebutGross_Calendrier = new QPushButton( Maternite_2, "pushButton_DebutGross_Calendrier" );
    pushButton_DebutGross_Calendrier->setMaximumSize( QSize( 32767, 19 ) );
    layout70_2->addWidget( pushButton_DebutGross_Calendrier );

    lineEdit_DebutGross_Date = new QLineEdit( Maternite_2, "lineEdit_DebutGross_Date" );
    lineEdit_DebutGross_Date->setMinimumSize( QSize( 0, 20 ) );
    lineEdit_DebutGross_Date->setReadOnly( TRUE );
    layout70_2->addWidget( lineEdit_DebutGross_Date );

    MaterniteLayout->addLayout( layout70_2, 2, 0 );
    widgetStack_PEC->addWidget( Maternite_2, 1 );

    AccidentTravail = new QWidget( widgetStack_PEC, "AccidentTravail" );
    AccidentTravailLayout = new QGridLayout( AccidentTravail, 1, 1, 0, 0, "AccidentTravailLayout"); 

    layout13_2_2_3 = new QHBoxLayout( 0, 0, 6, "layout13_2_2_3"); 

    textLabel1_4_2_3_2_2_3 = new QLabel( AccidentTravail, "textLabel1_4_2_3_2_2_3" );
    textLabel1_4_2_3_2_2_3->setMinimumSize( QSize( 0, 20 ) );
    textLabel1_4_2_3_2_2_3->setFrameShape( QLabel::Box );
    textLabel1_4_2_3_2_2_3->setAlignment( int( QLabel::AlignCenter ) );
    layout13_2_2_3->addWidget( textLabel1_4_2_3_2_2_3 );

    lineEdit_ATMP_Numero = new QLineEdit( AccidentTravail, "lineEdit_ATMP_Numero" );
    lineEdit_ATMP_Numero->setMinimumSize( QSize( 0, 20 ) );
    layout13_2_2_3->addWidget( lineEdit_ATMP_Numero );

    AccidentTravailLayout->addMultiCellLayout( layout13_2_2_3, 2, 2, 0, 1 );

    layout64 = new QHBoxLayout( 0, 0, 6, "layout64"); 

    pushButton_ATMP_Date = new QPushButton( AccidentTravail, "pushButton_ATMP_Date" );
    pushButton_ATMP_Date->setMaximumSize( QSize( 51, 19 ) );
    layout64->addWidget( pushButton_ATMP_Date );

    lineEdit_ATMP_Date = new QLineEdit( AccidentTravail, "lineEdit_ATMP_Date" );
    lineEdit_ATMP_Date->setReadOnly( TRUE );
    layout64->addWidget( lineEdit_ATMP_Date );

    AccidentTravailLayout->addMultiCellLayout( layout64, 3, 3, 0, 1 );

    textLabel5_2 = new QLabel( AccidentTravail, "textLabel5_2" );
    textLabel5_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)4, 0, 0, textLabel5_2->sizePolicy().hasHeightForWidth() ) );
    textLabel5_2->setMaximumSize( QSize( 32767, 46 ) );

    AccidentTravailLayout->addMultiCellWidget( textLabel5_2, 0, 0, 0, 1 );
    spacer12 = new QSpacerItem( 16, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    AccidentTravailLayout->addItem( spacer12, 1, 1 );
    spacer13 = new QSpacerItem( 16, 10, QSizePolicy::Minimum, QSizePolicy::Expanding );
    AccidentTravailLayout->addItem( spacer13, 4, 0 );
    widgetStack_PEC->addWidget( AccidentTravail, 2 );

    groupBox7Layout->addWidget( widgetStack_PEC, 0, 1 );

    Onglet_FSELayout->addMultiCellWidget( groupBox7, 0, 1, 1, 2 );

    buttonGroup_FSE_FactureType = new QButtonGroup( Onglet_FSE, "buttonGroup_FSE_FactureType" );
    buttonGroup_FSE_FactureType->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)0, 0, 0, buttonGroup_FSE_FactureType->sizePolicy().hasHeightForWidth() ) );
    buttonGroup_FSE_FactureType->setColumnLayout(0, Qt::Vertical );
    buttonGroup_FSE_FactureType->layout()->setSpacing( 0 );
    buttonGroup_FSE_FactureType->layout()->setMargin( 11 );
    buttonGroup_FSE_FactureTypeLayout = new QGridLayout( buttonGroup_FSE_FactureType->layout() );
    buttonGroup_FSE_FactureTypeLayout->setAlignment( Qt::AlignTop );

    radioButtonDRE_FSE = new QRadioButton( buttonGroup_FSE_FactureType, "radioButtonDRE_FSE" );
    buttonGroup_FSE_FactureType->insert( radioButtonDRE_FSE, 3 );

    buttonGroup_FSE_FactureTypeLayout->addWidget( radioButtonDRE_FSE, 2, 0 );

    radioButtonDRE_Seule = new QRadioButton( buttonGroup_FSE_FactureType, "radioButtonDRE_Seule" );
    buttonGroup_FSE_FactureType->insert( radioButtonDRE_Seule, 2 );

    buttonGroup_FSE_FactureTypeLayout->addWidget( radioButtonDRE_Seule, 1, 0 );

    radioButtonFSE_Seule = new QRadioButton( buttonGroup_FSE_FactureType, "radioButtonFSE_Seule" );
    buttonGroup_FSE_FactureType->insert( radioButtonFSE_Seule, 1 );

    buttonGroup_FSE_FactureTypeLayout->addWidget( radioButtonFSE_Seule, 0, 0 );

    Onglet_FSELayout->addMultiCellWidget( buttonGroup_FSE_FactureType, 0, 1, 3, 3 );

    layout12 = new QHBoxLayout( 0, 0, 6, "layout12"); 

    textLabel1_4_2_2 = new QLabel( Onglet_FSE, "textLabel1_4_2_2" );
    textLabel1_4_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)5, 0, 0, textLabel1_4_2_2->sizePolicy().hasHeightForWidth() ) );
    textLabel1_4_2_2->setMaximumSize( QSize( 32767, 20 ) );
    textLabel1_4_2_2->setFrameShape( QLabel::Box );
    textLabel1_4_2_2->setAlignment( int( QLabel::AlignCenter ) );
    layout12->addWidget( textLabel1_4_2_2 );

    lineEdit_FSE_Date = new QLineEdit( Onglet_FSE, "lineEdit_FSE_Date" );
    lineEdit_FSE_Date->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, lineEdit_FSE_Date->sizePolicy().hasHeightForWidth() ) );
    lineEdit_FSE_Date->setMinimumSize( QSize( 0, 20 ) );
    lineEdit_FSE_Date->setMaximumSize( QSize( 32767, 32767 ) );
    lineEdit_FSE_Date->setReadOnly( TRUE );
    layout12->addWidget( lineEdit_FSE_Date );

    pushButton_FSE_Calendrier = new QPushButton( Onglet_FSE, "pushButton_FSE_Calendrier" );
    pushButton_FSE_Calendrier->setMaximumSize( QSize( 19, 19 ) );
    layout12->addWidget( pushButton_FSE_Calendrier );

    Onglet_FSELayout->addLayout( layout12, 0, 0 );

    buttonGroup_FSE_Securisation = new QButtonGroup( Onglet_FSE, "buttonGroup_FSE_Securisation" );
    buttonGroup_FSE_Securisation->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, buttonGroup_FSE_Securisation->sizePolicy().hasHeightForWidth() ) );
    buttonGroup_FSE_Securisation->setMaximumSize( QSize( 200, 32767 ) );
    buttonGroup_FSE_Securisation->setColumnLayout(0, Qt::Vertical );
    buttonGroup_FSE_Securisation->layout()->setSpacing( 0 );
    buttonGroup_FSE_Securisation->layout()->setMargin( 11 );
    buttonGroup_FSE_SecurisationLayout = new QGridLayout( buttonGroup_FSE_Securisation->layout() );
    buttonGroup_FSE_SecurisationLayout->setAlignment( Qt::AlignTop );

    radioButtonSec_SESAM_VItale = new QRadioButton( buttonGroup_FSE_Securisation, "radioButtonSec_SESAM_VItale" );
    radioButtonSec_SESAM_VItale->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)0, 0, 0, radioButtonSec_SESAM_VItale->sizePolicy().hasHeightForWidth() ) );
    buttonGroup_FSE_Securisation->insert( radioButtonSec_SESAM_VItale, 3 );

    buttonGroup_FSE_SecurisationLayout->addWidget( radioButtonSec_SESAM_VItale, 1, 0 );

    radioButtonSec_Desynchro_Vitale = new QRadioButton( buttonGroup_FSE_Securisation, "radioButtonSec_Desynchro_Vitale" );
    radioButtonSec_Desynchro_Vitale->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, radioButtonSec_Desynchro_Vitale->sizePolicy().hasHeightForWidth() ) );
    buttonGroup_FSE_Securisation->insert( radioButtonSec_Desynchro_Vitale, 4 );

    buttonGroup_FSE_SecurisationLayout->addWidget( radioButtonSec_Desynchro_Vitale, 2, 0 );

    radioButtonSec_SesamDegrade = new QRadioButton( buttonGroup_FSE_Securisation, "radioButtonSec_SesamDegrade" );
    radioButtonSec_SesamDegrade->setChecked( FALSE );
    buttonGroup_FSE_Securisation->insert( radioButtonSec_SesamDegrade, 5 );

    buttonGroup_FSE_SecurisationLayout->addWidget( radioButtonSec_SesamDegrade, 3, 0 );

    Onglet_FSELayout->addWidget( buttonGroup_FSE_Securisation, 1, 0 );
    tabWidget_Beneficiaire->insertTab( Onglet_FSE, QString::fromLatin1("") );

    Onglet_Monitoring = new QWidget( tabWidget_Beneficiaire, "Onglet_Monitoring" );
    Onglet_MonitoringLayout = new QGridLayout( Onglet_Monitoring, 1, 1, 11, 6, "Onglet_MonitoringLayout"); 

    splitter3 = new QSplitter( Onglet_Monitoring, "splitter3" );
    splitter3->setOrientation( QSplitter::Horizontal );

    QWidget* privateLayoutWidget_3 = new QWidget( splitter3, "layout1" );
    layout1 = new QVBoxLayout( privateLayoutWidget_3, 11, 6, "layout1"); 

    textLabel1 = new QLabel( privateLayoutWidget_3, "textLabel1" );
    textLabel1->setFrameShape( QLabel::Box );
    textLabel1->setAlignment( int( QLabel::AlignCenter ) );
    layout1->addWidget( textLabel1 );

    textEdit_MonCPS = new QTextEdit( privateLayoutWidget_3, "textEdit_MonCPS" );
    layout1->addWidget( textEdit_MonCPS );

    QWidget* privateLayoutWidget_4 = new QWidget( splitter3, "layout2" );
    layout2 = new QVBoxLayout( privateLayoutWidget_4, 11, 6, "layout2"); 

    textLabel1_2 = new QLabel( privateLayoutWidget_4, "textLabel1_2" );
    textLabel1_2->setFrameShape( QLabel::Box );
    textLabel1_2->setAlignment( int( QLabel::AlignCenter ) );
    layout2->addWidget( textLabel1_2 );

    textEdit_MonVitale = new QTextEdit( privateLayoutWidget_4, "textEdit_MonVitale" );
    layout2->addWidget( textEdit_MonVitale );

    QWidget* privateLayoutWidget_5 = new QWidget( splitter3, "layout14" );
    layout14 = new QGridLayout( privateLayoutWidget_5, 1, 1, 11, 6, "layout14"); 

    layout12_2 = new QGridLayout( 0, 1, 1, 0, 6, "layout12_2"); 

    pushButton_FSE_ToListView = new QPushButton( privateLayoutWidget_5, "pushButton_FSE_ToListView" );
    pushButton_FSE_ToListView->setMinimumSize( QSize( 17, 17 ) );
    pushButton_FSE_ToListView->setMaximumSize( QSize( 17, 17 ) );

    layout12_2->addWidget( pushButton_FSE_ToListView, 0, 1 );

    textLabel1_3 = new QLabel( privateLayoutWidget_5, "textLabel1_3" );
    textLabel1_3->setFrameShape( QLabel::Box );
    textLabel1_3->setAlignment( int( QLabel::AlignCenter ) );

    layout12_2->addWidget( textLabel1_3, 0, 0 );

    layout14->addLayout( layout12_2, 0, 0 );

    listView_FseGroup = new QListView( privateLayoutWidget_5, "listView_FseGroup" );
    listView_FseGroup->addColumn( tr( "Type" ) );
    listView_FseGroup->addColumn( tr( "Valeur" ) );
    listView_FseGroup->addColumn( trUtf8( "\x4c\x69\x62\x65\x6c\x6c\xc3\xa9" ) );
    listView_FseGroup->addColumn( tr( "Occurence" ) );
    listView_FseGroup->setSelectionMode( QListView::Single );
    listView_FseGroup->setAllColumnsShowFocus( FALSE );
    listView_FseGroup->setRootIsDecorated( TRUE );
    listView_FseGroup->setDefaultRenameAction( QListView::Accept );

    layout14->addWidget( listView_FseGroup, 1, 0 );

    Onglet_MonitoringLayout->addWidget( splitter3, 0, 0 );
    tabWidget_Beneficiaire->insertTab( Onglet_Monitoring, QString::fromLatin1("") );

    Dlg_FseDefineLayout->addWidget( splitter2, 0, 0 );
    languageChange();
    resize( QSize(1083, 618).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Dlg_FseDefine::~Dlg_FseDefine()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Dlg_FseDefine::languageChange()
{
    setCaption( trUtf8( "\x44\xc3\xa9\x66\x69\x6e\x69\x72\x20\x75\x6e\x65\x20\x46\x53\x45" ) );
    pushButton_Vitale->setText( tr( "Lecture carte vitale" ) );
    QToolTip::add( pushButton_Vitale, trUtf8( "\x43\x6c\x69\x71\x75\x65\x7a\x20\x73\x75\x72\x20\x63\x65\x20\x62\x6f\x75\x74\x6f\x6e\x20\x70\x6f\x75\x72\x20\x6c\x69\x72\x65\x20\x75\x6e\x65\x20\x63\x61\x72\x74\x65\x20\x64\x65\x20\x70\x72\x6f\x66\x65\x73\x73\x69\x6f\x6e\x6e\x65\x6c\x20\x64\x65\x20\x73\x61\x6e\x74\xc3\xa9\x20\x28\x20\x63\x61\x72\x74\x65\x20\x43\x50\x53\x29" ) );
    pushButton_ReadFileVitale->setText( tr( "Fichier de test" ) );
    QToolTip::add( listBox_Vitale, tr( "Liste des ayants droits inscrits sur la carte Vitale" ) );
    QWhatsThis::add( listBox_Vitale, trUtf8( "\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x63\x65\x6e\x74\x65\x72\x22\x3e\x3c\x66\x6f\x6e\x74\x20\x73\x69\x7a\x65\x3d\x22\x2b\x31\x22\x3e\x3c\x75\x3e\x3c\x62\x3e\x4c\x69\x73\x74\x65\x20\x64\x65\x73\x20\x61\x79\x61\x6e\x74\x73\x20\x20\x64\x72\x6f\x69\x74\x20\x64\x65\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65\x3c\x2f\x62\x3e\x3c\x2f\x75\x3e\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x70\x3e\xd\xa\x43\x65\x74\x74\x65\x20\x6c\x69\x73\x74\x65\x20\x73\x65\x20\x6d\x65\x74\x20\xc3\xa0\x20\x6a\x6f\x75\x72\x20\x61\x70\x72\xc3\xa9\x73\x20\x6c\x65\x63\x74\x75\x72\x65\x20\x64\x27\x75\x6e\x65\x20\x63\x61\x72\x74\x65\x20\x76\x69\x74\x61\x6c\x65\x20\x2e\x3c\x62\x72\x3e\xd\xa\x41\x20\x6c\x27\x61\x69\x64\x65\x20\x64\x65\x20\x6c\x61\x20\x73\x6f\x75\x72\x69\x73\x20\x69\x6c\x20\x76\x6f\x75\x73\x20\x66\x61\x75\x74\x20\x73\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72\x20\x6c\x27\x61\x79\x61\x6e\x74\x20\x20\x64\x72\x6f\x69\x74\x20\x70\x6f\x75\x72\x20\x6c\x65\x71\x75\x65\x6c\x20\x76\x6f\x75\x73\x20\x64\xc3\xa9\x73\x69\x72\x65\x7a\x20\x66\x61\x69\x72\x65\x20\x75\x6e\x65\x20\x66\x65\x75\x69\x6c\x6c\x65\x20\x64\x65\x20\x73\x6f\x69\x6e\x73\x20\xc3\xa9\x6c\x65\x63\x74\x72\x6f\x6e\x69\x71\x75\x65\x20\x28\x46\x53\x45\x29\x20\x65\x74\x2f\x6f\x75\x20\x75\x6e\x65\x20\x64\x65\x6d\x61\x6e\x64\x65\x20\x64\x65\x20\x72\x65\x6d\x62\x6f\x75\x72\x73\x65\x6d\x65\x6e\x74\x20\xc3\xa9\x6c\x65\x63\x74\x72\x6f\x6e\x69\x71\x75\x65\x20\x28\x44\x52\x45\x29" ) );
    pushButton_WhatIsIt->setText( tr( "??" ) );
    QToolTip::add( pushButton_WhatIsIt, trUtf8( "\x43\x6c\x69\x71\x75\x65\x7a\x20\x73\x75\x72\x20\x63\x65\x20\x62\x6f\x75\x74\x6f\x6e\x2c\x20\x70\x75\x69\x73\x20\x70\x6c\x61\x63\x65\x7a\x20\x6c\x65\x20\x70\x6f\x69\x6e\x74\x20\x64\x27\x69\x6e\x74\x65\x72\x72\x6f\x67\x61\x74\x69\x6f\x6e\x20\x73\x75\x72\x20\x6c\x61\x20\x7a\x6f\x6e\x65\x20\x6f\xc3\xb9\x20\x76\x6f\x75\x73\x20\x64\xc3\xa9\x73\x69\x72\x65\x7a\x20\x64\x65\x20\x6c\x27\x61\x69\x64\x65\x2e" ) );
    pushButton_OK->setText( tr( "Actualiser la FSE" ) );
    pushButtonFormater->setText( tr( "Valider la FSE" ) );
    pushButton_SaveVirtual->setText( tr( "Save Virtual" ) );
    pushButton_Annuler->setText( tr( "Quitter" ) );
    pushButton_DoVirtual->setText( tr( "DoVirtual" ) );
    textLabel1_4->setText( tr( "Code porteur" ) );
    pushButton_CPS->setText( tr( "Lecture carte CPS" ) );
    QToolTip::add( pushButton_CPS, tr( "Cliquez sur ce bouton pour lire une carte Vitale" ) );
    QToolTip::add( lineEdit_CodePorteur, trUtf8( "\x45\x6e\x74\x72\x65\x7a\x20\x69\x63\x69\x20\x6c\x65\x20\x63\x6f\x64\x65\x20\x70\x6f\x72\x74\x65\x75\x72\x20\x28\x6d\x6f\x74\x20\x64\x65\x20\x70\x61\x73\x73\x65\x29\x20\x64\x65\x20\x76\x6f\x74\x72\x65\x20\x63\x61\x72\x74\x65\x20\x64\x65\x20\x70\x72\x6f\x66\x65\x73\x73\x69\x6f\x6e\x6e\x65\x6c\x20\x64\x65\x20\x73\x61\x6e\x74\xc3\xa9\x20\x28\x63\x61\x72\x74\x65\x20\x43\x50\x53\x29" ) );
    pushButton_ReadFileCPS->setText( tr( "Fichier Test CPS" ) );
    groupBox_PS_Remplacant->setTitle( trUtf8( "\x50\x72\x61\x74\x69\x63\x69\x65\x6e\x20\x72\x65\x6d\x70\x6c\x61\xc3\xa7\x61\x6e\x74" ) );
    groupBox_PS_Titulaire->setTitle( tr( "Praticien titulaire du poste de travail" ) );
    pushButtonSaveCPS_Titulaire->setText( QString::null );
    QToolTip::add( pushButtonSaveCPS_Titulaire, trUtf8( "\x45\x6e\x72\x65\x67\x69\x73\x74\x72\x65\x72\x20\x6c\x65\x73\x20\x64\x6f\x6e\x6e\xc3\xa9\x65\x73\x20\x64\x65\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x43\x50\x53\x20\x2e\x2e\x2e" ) );
    pushButtonLoadCPS_Titulaire->setText( QString::null );
    QToolTip::add( pushButtonLoadCPS_Titulaire, trUtf8( "\x43\x68\x61\x72\x67\x65\x72\x20\x6c\x65\x73\x20\x64\x6f\x6e\x6e\xc3\xa9\x65\x73\x20\x64\x65\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x43\x50\x53\x20\x2e\x2e\x2e" ) );
    tabWidget_Beneficiaire->changeTab( Onglet_Lecture_cartes, tr( "Praticiens" ) );
    buttonGroup_ModeGestion->setTitle( tr( "Mode de gestion" ) );
    radioButton_GestionSeparee->setText( trUtf8( "\x47\x65\x73\x74\x69\x6f\x6e\x20\x73\xc3\xa9\x70\x61\x72\xc3\xa9\x65" ) );
    QToolTip::add( radioButton_GestionSeparee, trUtf8( "\x53\x69\x20\x63\x6f\x64\x65\x20\x72\x6f\x75\x74\x61\x67\x65\x20\x52\x4f\x2c\x20\x65\x74\x20\x6d\x65\x6e\x74\x69\x6f\x6e\x20\x47\x65\x73\x74\x69\x6f\x6e\x20\x75\x6e\x69\x71\x75\x65\x20\x6e\x6f\x6e\x20\x70\x72\xc3\xa9\x73\x65\x6e\x74\x65\x73\x20\x73\x75\x72\x20\x6c\x27\x61\x74\x74\x65\x73\x74\x61\x74\x69\x6f\x6e\x20\x6f\x75\x20\x65\x6e\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65" ) );
    QWhatsThis::add( radioButton_GestionSeparee, trUtf8( "\x3c\x70\x3e\x3c\x75\x3e\x3c\x62\x3e\x4c\x61\x20\x67\x65\x73\x74\x69\x6f\x6e\x20\x73\xc3\xa9\x70\x61\x72\xc3\xa9\x65\x20\x65\x73\x74\x20\x64\xc3\xa9\x74\x65\x72\x6d\x69\x6e\xc3\xa9\x65\x3c\x2f\x62\x3e\x3c\x2f\x75\x3e\x3c\x2f\x70\x3e\xd\xa\x9\xc2\xb0\x20\x3c\x75\x3e\x3c\x62\x3e\x53\x6f\x69\x74\x3c\x2f\x62\x3e\x20\x61\x75\x74\x6f\x6d\x61\x74\x69\x71\x75\x65\x6d\x65\x6e\x74\x3c\x2f\x75\x3e\x20\x70\x61\x72\x20\x6c\x65\x73\x20\x64\x6f\x6e\x6e\xc3\xa9\x65\x73\xd\xa\x9\x66\x6f\x75\x72\x6e\x69\x65\x73\x20\x70\x61\x72\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65\x20\x64\x75\x20\x62\xc3\xa9\x6e\xc3\xa9\x66\x69\x63\x69\x61\x69\x72\x65\x2e\x3c\x62\x72\x3e\x3c\x62\x72\x3e\xd\xa\xd\xa\x9\xc2\xb0\x20\x3c\x75\x3e\x3c\x62\x3e\x53\x6f\x69\x74\x3c\x2f\x62\x3e\x20\x6d\x61\x6e\x75\x65\x6c\x6c\x65\x6d\x65\x6e\x74\x3c\x2f\x75\x3e\x20\x70\x61\x72\x20\x6c\x65\x20\x70\x72\x61\x74\x69\x63\x69\x65\x6e\x20\x64\x65\x20\x73\x61\x6e\x74\xc3\xa9\xd\xa\x9\xc3\xa0\x20\x70\x61\x72\x74\x69\x72\x20\x64\x65\x73\x20\x61\x74\x74\x65\x73\x74\x61\x74\x69\x6f\x6e\x73\x20\x70\x61\x70\x69\x65\x72\x73\x20\x64\x27\x41\x4d\x43\x20\x73\x69\xd\xa\x9\x6c\x65\x73\x20\x64\x6f\x6e\x6e\xc3\xa9\x65\x73\x20\x73\x75\x69\x76\x61\x6e\x74\x65\x73\x20\x73\x6f\x6e\x74\x20\x3c\x75\x3e\x61\x62\x73\x65\x6e\x74\x65\x73\x3c\x2f\x75\x3e\x20\x3a\x3c\x62\x72\x3e\xd\xa\x9\x9\xd\xa\x9\x9\x2d\x20\x4d\x65\x6e\x74\x69\x6f\x6e\x20\x20\x3c\x62\x3e\x47\x65\x73\x74\x69\x6f\x6e\x20\x75\x6e\x69\x71\x75\x65\x20\x6f\x75\x20\xc3\xa9\x71\x75\x69\x76\x61\x6c\x65\x6e\x74\x3c\x2f\x62\x3e\x3c\x62\x72\x3e\xd\xa\x9\x9\x2d\x20\x43\x6f\x64\x65\x20\x72\x6f\x75\x74\x61\x67\x65\x20\x3c\x62\x3e\x52\x4f\x3c\x2f\x62\x3e\x3c\x62\x72\x3e" ) );
    radioButton_GestionUnique->setText( tr( "Gestion unique" ) );
    QToolTip::add( radioButton_GestionUnique, trUtf8( "\x53\x69\x20\x63\x6f\x64\x65\x20\x72\x6f\x75\x74\x61\x67\x65\x20\x52\x4f\x2c\x20\x6f\x75\x20\x6d\x65\x6e\x74\x69\x6f\x6e\x20\x47\x65\x73\x74\x69\x6f\x6e\x20\x75\x6e\x69\x71\x75\x65\x20\x20\x70\x72\xc3\xa9\x73\x65\x6e\x74\x65\x73\x20\x73\x75\x72\x20\x6c\x27\x61\x74\x74\x65\x73\x74\x61\x74\x69\x6f\x6e\x20\x6f\x75\x20\x65\x6e\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65" ) );
    QWhatsThis::add( radioButton_GestionUnique, trUtf8( "\x3c\x70\x3e\x3c\x75\x3e\x3c\x62\x3e\x4c\x61\x20\x67\x65\x73\x74\x69\x6f\x6e\x20\x75\x6e\x69\x71\x75\x65\x20\x65\x73\x74\x20\x64\xc3\xa9\x74\x65\x72\x6d\x69\x6e\xc3\xa9\x65\x3c\x2f\x62\x3e\x3c\x2f\x75\x3e\x3c\x2f\x70\x3e\xd\xa\x9\x3c\x75\x3e\x3c\x62\x3e\xc2\xb0\x20\x53\x6f\x69\x74\x3c\x2f\x62\x3e\x20\x61\x75\x74\x6f\x6d\x61\x74\x69\x71\x75\x65\x6d\x65\x6e\x74\x3c\x2f\x75\x3e\x20\x70\x61\x72\x20\x6c\x65\x73\x20\x64\x6f\x6e\x6e\xc3\xa9\x65\x73\xd\xa\x9\x66\x6f\x75\x72\x6e\x69\x65\x73\x20\x70\x61\x72\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65\x20\x64\x75\x20\x62\xc3\xa9\x6e\xc3\xa9\x66\x69\x63\x69\x61\x69\x72\x65\x2e\x3c\x62\x72\x3e\x3c\x62\x72\x3e\xd\xa\xd\xa\x9\x3c\x75\x3e\x3c\x62\x3e\xc2\xb0\x20\x53\x6f\x69\x74\x3c\x2f\x62\x3e\x20\x6d\x61\x6e\x75\x65\x6c\x6c\x65\x6d\x65\x6e\x74\x3c\x2f\x75\x3e\x20\x70\x61\x72\x20\x6c\x65\x20\x70\x72\x61\x74\x69\x63\x69\x65\x6e\x20\x64\x65\x20\x73\x61\x6e\x74\xc3\xa9\xd\xa\x9\xc3\xa0\x20\x70\x61\x72\x74\x69\x72\x20\x64\x65\x73\x20\x61\x74\x74\x65\x73\x74\x61\x74\x69\x6f\x6e\x73\x20\x70\x61\x70\x69\x65\x72\x73\x20\x64\x27\x41\x4d\x43\x20\x73\x69\xd\xa\x9\x6c\x65\x73\x20\x64\x6f\x6e\x6e\xc3\xa9\x65\x73\x20\x73\x75\x69\x76\x61\x6e\x74\x65\x73\x20\x3c\x75\x3e\x73\x6f\x6e\x74\x20\x70\x72\xc3\xa9\x73\x65\x6e\x74\x65\x73\x3c\x2f\x75\x3e\x20\x3a\x3c\x62\x72\x3e\xd\xa\x9\x9\xd\xa\x9\x9\x2d\x20\x4d\x65\x6e\x74\x69\x6f\x6e\x20\x20\x3c\x62\x3e\x47\x65\x73\x74\x69\x6f\x6e\x20\x75\x6e\x69\x71\x75\x65\x20\x6f\x75\x20\xc3\xa9\x71\x75\x69\x76\x61\x6c\x65\x6e\x74\x3c\x2f\x62\x3e\x3c\x62\x72\x3e\xd\xa\x9\x9\x2d\x20\x43\x6f\x64\x65\x20\x72\x6f\x75\x74\x61\x67\x65\x20\x3c\x62\x3e\x52\x4f\x3c\x2f\x62\x3e\x3c\x62\x72\x3e" ) );
    groupBox6->setTitle( tr( "Assurance Maladie Obligatoire (AMO)" ) );
    textLabel1_4_2_3_2_2_2_2_2_2_2_2->setText( tr( "Service AMO" ) );
    groupBox_GestionUnique->setTitle( tr( "Mutuelle (Gestion unique)" ) );
    textLabel1_4_2_3_2_2_2_2_3->setText( trUtf8( "\x44\x61\x74\x65\x20\x64\x65\x20\x20\x64\xc3\xa9\x62\x75\x74\x20\x64\x65\x73\x20\x64\x72\x6f\x69\x74\x73" ) );
    textLabel1_4_2_3_2_2_2_2_2_4->setText( tr( "Date de fin des droits" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2_2->setText( tr( "code aiguillage STS" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2_2->setText( trUtf8( "\x53\x65\x72\x76\x69\x63\x65\x73\x20\x61\x73\x73\x6f\x63\x69\xc3\xa9\x73" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2_2->setText( tr( "Type de services" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3->setText( trUtf8( "\x4e\x75\x6d\xc3\xa9\x72\x6f\x20\x6d\x75\x74\x75\x65\x6c\x6c\x65" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_3->setText( trUtf8( "\x4e\xc2\xb0\x20\x64\x27\x6f\x72\x67\x61\x6e\x69\x73\x6d\x65\x20\x63\x6f\x6d\x70\x6c\xc3\xa9\x6d\x65\x6e\x74\x61\x69\x72\x65" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_3_2->setText( tr( "Nom" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_3_2->setText( tr( "Garanties" ) );
    textLabel1_4_2_3_2_2_2_2_2_2_2->setText( tr( "Indicateur de traitement MUT" ) );
    groupBoxAMC->setTitle( trUtf8( "\x41\x73\x73\x75\x72\x61\x6e\x63\x65\x20\x4d\x61\x6c\x61\x64\x69\x65\x20\x43\x6f\x6d\x70\x6c\xc3\xa9\x6d\x65\x6e\x74\x61\x69\x72\x65\x20\x28\x41\x4d\x43\x29" ) );
    textLabel1_4_2_3_2_2_2_3->setText( trUtf8( "\x4e\xc2\xb0\x20\x52\x4e\x4d\x20\x64\x27\x6f\x72\x67\x61\x6e\x69\x73\x6d\x65\x20\x63\x6f\x6d\x70\x6c\xc3\xa9\x6d\xc3\xa9\x6e\x74\x61\x69\x72\x65\x20\x3a" ) );
    QToolTip::add( lineEdit_AMC_NumOrga, trUtf8( "\x4e\x75\x6d\xc3\xa9\x72\x6f\x20\x52\x4e\x4d\x20\x28\x4e\xc2\xb0\x20\x64\x27\x69\x6d\x6d\x61\x74\x72\x69\x63\x75\x6c\x61\x74\x69\x6f\x6e\x20\x61\x75\x20\x52\x65\x67\x69\x73\x74\x72\x65\x20\x4e\x61\x74\x69\x6f\x6e\x61\x6c\x20\x64\x65\x73\x20\x4d\x75\x74\x75\x65\x6c\x6c\x65\x73\x29" ) );
    textLabel1_4_2_3_2_2_2_2->setText( trUtf8( "\x44\x61\x74\x65\x20\x64\x65\x20\x20\x64\xc3\xa9\x62\x75\x74\x20\x64\x65\x73\x20\x64\x72\x6f\x69\x74\x73" ) );
    textLabel1_4_2_3_2_2_2_2_2->setText( tr( "Date de fin des droits" ) );
    textLabel1_4_2_3_2_2_2->setText( trUtf8( "\x4e\xc2\xb0\x20\x61\x64\x68\xc3\xa9\x72\x65\x6e\x74\x20\x3a" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2_2->setText( trUtf8( "\x53\x65\x72\x76\x69\x63\x65\x73\x20\x61\x73\x73\x6f\x63\x69\xc3\xa9\x73\x20\x61\x75\x20\x63\x6f\x6e\x74\x72\x61\x74" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2_2->setText( trUtf8( "\x74\x79\x70\x65\x20\x64\x65\x20\x73\x65\x72\x76\x69\x63\x65\x73\x20\x61\x73\x73\x6f\x63\x69\xc3\xa9\x73" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2->setText( tr( "Nom de domaine" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2->setText( trUtf8( "\x49\x64\x65\x6e\x74\x69\x66\x69\x61\x6e\x74\x20\x64\x27\x68\xc3\xb4\x74\x65" ) );
    textLabel1_4_2_3_2_2_2_2_2_3->setText( tr( "Code de routage" ) );
    textLabel1_4_2_3_2_2_2_2_2_2->setText( tr( "Indicateur de traitement AMC" ) );
    textLabel1_4_2_3_2_2_2_2_2_3_2_2_2->setText( tr( "code aiguillage STS" ) );
    textLabel_AMC_SesamVitale->setText( trUtf8( "\x53\x45\x53\x41\x4d\x20\x56\x49\x54\x41\x4c\x45\x20\x28\x41\x69\x67\x75\x69\x2e\x20\x2f\x20\x69\x6e\x64\x2e\x54\x72\x61\x69\x74\x2e\x20\x2f\x20\x72\x6f\x75\x74\x2e\x20\x2f\x20\x68\xc3\xb4\x74\x65\x20\x2f\x20\x64\x6f\x6d\x29\x20\x3a" ) );
    tabWidget_Beneficiaire->changeTab( Beneficiaire, trUtf8( "\x42\xc3\xa9\x6e\xc3\xa9\x66\x69\x63\x69\x61\x69\x72\x65" ) );
    groupBox1->setTitle( trUtf8( "\x41\x72\x72\xc3\xaa\x74\x20\x64\x65\x20\x74\x72\x61\x76\x61\x69\x6c\x20\x28\x66\x61\x63\x75\x6c\x74\x61\x74\x69\x66\x29" ) );
    pushButton_AT_DateDebut->setText( tr( "..." ) );
    QToolTip::add( pushButton_AT_DateDebut, trUtf8( "\x4d\x6f\x64\x69\x66\x69\x65\x72\x20\x6c\x61\x20\x64\x61\x74\x65\x20\x64\x65\x20\x64\xc3\xa9\x62\x75\x74\x20\x64\x27\x41\x63\x63\x69\x64\x65\x6e\x74\x20\x64\x65\x20\x54\x72\x61\x76\x61\x69\x6c" ) );
    textLabel1_4_2_2_2->setText( trUtf8( "\x44\xc3\xa9\x62\x75\x74\x20\x3a" ) );
    pushButton_AT_DateFin->setText( tr( "..." ) );
    QToolTip::add( pushButton_AT_DateFin, tr( "Modifier la date de fin d'Accident de Travail" ) );
    textLabel1_4_2_2_2_2->setText( tr( "Fin" ) );
    textLabel1_4_2_3_2_2->setText( tr( "Nbr de Jours" ) );
    QToolTip::add( lineEdit_Nbr_ArretTravail, tr( "Modifier  le nombre de jours d'Accident de Travail" ) );
    groupBox1_2->setTitle( trUtf8( "\x43\x61\x72\x6e\x65\x74\x20\x6d\xc3\xa9\x64\x69\x63\x61\x6c" ) );
    comboBox_CodeCarnetMedical->clear();
    comboBox_CodeCarnetMedical->insertItem( trUtf8( "\x50\x72\xc3\xa9\x73\x65\x6e\x74\xc3\xa9" ) );
    comboBox_CodeCarnetMedical->insertItem( trUtf8( "\x4e\x6f\x6e\x20\x70\x72\xc3\xa9\x73\x65\x6e\x74\xc3\xa9" ) );
    groupBox9->setTitle( trUtf8( "\x52\xc3\xa8\x67\x6c\x65\x6d\x65\x6e\x74" ) );
    textLabel6->setText( tr( "Part obligatoire (Tiers-Payant AMO)" ) );
    textLabel7->setText( trUtf8( "\x50\x61\x72\x74\x20\x63\x6f\x6d\x70\x6c\xc3\xa9\x6d\x65\x6e\x74\x61\x69\x72\x65\x20\x28\x54\x69\x65\x72\x73\x2d\x50\x61\x79\x61\x6e\x74\x20\x41\x4d\x43\x29" ) );
    textLabel8->setText( trUtf8( "\x41\x20\x6c\x61\x20\x63\x68\x61\x72\x67\x65\x20\x64\x65\x20\x6c\x27\x61\x73\x73\x75\x72\xc3\xa9" ) );
    textLabel9->setText( tr( "<u><h3>Total de la facture&nbsp;</h3></u>" ) );
    textLabel9_2->setText( trUtf8( "\x3c\x68\x33\x3e\x20\x20\x2d\x20\x73\x6f\x6d\x6d\x65\x20\x72\xc3\xa9\x67\x6c\xc3\xa9\x65\x26\x6e\x62\x73\x70\x3b\x3c\x2f\x68\x33\x3e" ) );
    textLabel9_2_2->setText( trUtf8( "\x3c\x68\x33\x3e\x20\x20\x52\x65\x73\x74\x65\x20\x64\xc3\xbb\x26\x6e\x62\x73\x70\x3b\x3c\x2f\x68\x33\x3e" ) );
    textLabel9_2_3->setText( tr( "<h3>&nbsp;&nbsp;par </h3>" ) );
    comboBox_CodeCarnetMedical_2->clear();
    comboBox_CodeCarnetMedical_2->insertItem( trUtf8( "\x45\x73\x70\xc3\xa8\x63\x65\x73" ) );
    comboBox_CodeCarnetMedical_2->insertItem( trUtf8( "\x43\x68\xc3\xa8\x71\x75\x65" ) );
    comboBox_CodeCarnetMedical_2->insertItem( tr( "Mandat" ) );
    listView_Actes->header()->setLabel( 0, tr( "Date" ) );
    listView_Actes->header()->setLabel( 1, tr( "Codes CCAM" ) );
    listView_Actes->header()->setLabel( 2, tr( "C,CS,CNPSY" ) );
    listView_Actes->header()->setLabel( 3, tr( "V,VS,VNPSY" ) );
    listView_Actes->header()->setLabel( 4, tr( "Autres (K,Z,Cs,CP...)" ) );
    listView_Actes->header()->setLabel( 5, tr( "Montant" ) );
    listView_Actes->header()->setLabel( 6, trUtf8( "\x44\xc3\xa9\x70\x2e" ) );
    listView_Actes->header()->setLabel( 7, tr( "ID" ) );
    listView_Actes->header()->setLabel( 8, tr( "IK (nbre)" ) );
    listView_Actes->header()->setLabel( 9, tr( "IK (Montant)" ) );
    QToolTip::add( listView_Actes, tr( "Liste des actes composant la FSE et/ou DRE" ) );
    QWhatsThis::add( listView_Actes, trUtf8( "\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x63\x65\x6e\x74\x65\x72\x22\x3e\x3c\x66\x6f\x6e\x74\x20\x73\x69\x7a\x65\x3d\x22\x2b\x31\x22\x3e\x3c\x62\x3e\x3c\x75\x3e\x4c\x69\x73\x74\x65\x20\x64\x65\x73\x20\x61\x63\x74\x65\x73\x20\x63\x6f\x6d\x70\x6f\x73\x61\x6e\x74\x20\x6c\x61\x20\x46\x53\x45\x3c\x2f\x75\x3e\x3c\x2f\x62\x3e\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x70\x3e\xd\xa\x3c\x62\x3e\x50\x6f\x75\x72\x20\xc3\xa9\x64\x69\x74\x65\x72\x20\x75\x6e\x20\x61\x63\x74\x65\x20\x3a\x20\x3c\x2f\x62\x3e\x63\x6c\x69\x71\x75\x65\x7a\x20\x61\x76\x65\x63\x20\x6c\x65\x20\x62\x6f\x75\x74\x6f\x6e\x20\x67\x61\x75\x63\x68\x65\x20\x73\x75\x72\x20\x6c\x61\x20\x6c\x69\x67\x6e\x65\x20\x6f\xc3\xb9\x20\x69\x6c\x20\x73\x65\x20\x73\x69\x74\x75\x65\x20\x70\x6f\x75\x72\x20\x6c\x65\x20\x73\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x72\x2c\x20\x63\x65\x6c\x61\x20\x6c\x65\x20\x70\x6c\x61\x63\x65\x20\x64\x61\x6e\x73\x20\x6c\x61\x20\x7a\x6f\x6e\x65\x20\x64\x27\xc3\xa9\x64\x69\x74\x69\x6f\x6e\x20\x64\x65\x73\x20\x61\x63\x74\x65\x73\x2e\x3c\x62\x72\x3e\xd\xa\x3c\x62\x3e\x50\x6f\x75\x72\x20\x6d\x6f\x64\x69\x66\x69\x65\x72\x20\x64\x69\x72\x65\x63\x74\x65\x6d\x65\x6e\x74\x20\x75\x6e\x65\x20\x76\x61\x6c\x65\x75\x72\x20\x3a\x20\x3c\x2f\x62\x3e\x20\x64\x6f\x75\x62\x6c\x65\x20\x63\x6c\x69\x71\x75\x65\x7a\x20\x73\x75\x72\x20\x63\x65\x6c\x6c\x65\x2d\x63\x69\x2e\x3c\x62\x72\x3e\xd\xa\x3c\x62\x3e\x50\x6f\x75\x72\x20\x66\x61\x69\x72\x65\x20\x61\x70\x70\x61\x72\x61\xc3\xae\x74\x72\x65\x20\x6c\x65\x20\x6d\x65\x6e\x75\x20\x63\x6f\x6e\x74\x65\x78\x74\x75\x65\x6c\x20\x3a\x20\x3c\x2f\x62\x3e\x20\x63\x6c\x69\x71\x75\x65\x7a\x20\x61\x76\x65\x63\x20\x6c\x65\x20\x62\x6f\x75\x74\x6f\x6e\x20\x64\x72\x6f\x69\x74\x20\x64\x65\x20\x6c\x61\x20\x73\x6f\x75\x72\x69\x73\x20\x61\x75\x20\x64\x65\x73\x73\x75\x73\x20\x64\x65\x20\x6c\x61\x20\x6c\x69\x67\x6e\x65\x20\x63\x6f\x6e\x63\x65\x72\x6e\xc3\xa9\x65\x2e\x3c\x62\x72\x3e\xd\xa\x3c\x62\x3e\x50\x6f\x75\x72\x20\x65\x66\x66\x61\x63\x65\x72\x20\x75\x6e\x20\x61\x63\x74\x65\x20\x3a\x20\x3c\x2f\x62\x3e\x20\x73\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x6e\x65\x7a\x20\x6c\x65\x20\x20\xc3\xa0\x20\x6c\x27\x61\x69\x64\x65\x20\x64\x75\x20\x62\x6f\x75\x74\x6f\x6e\x20\x64\x72\x6f\x69\x74\x20\x64\x65\x20\x6c\x61\x20\x73\x6f\x75\x72\x69\x73\x20\x28\x63\x65\x6c\x61\x20\x66\x61\x69\x74\x20\x61\x70\x70\x61\x72\x61\xc3\xae\x74\x72\x65\x20\x75\x6e\x20\x6d\x65\x6e\x75\x20\x63\x6f\x6e\x74\x65\x78\x74\x75\x65\x6c\x29\x20\x65\x74\x20\x63\x68\x6f\x69\x73\x69\x73\x73\x65\x7a\x20\x6c\x27\x6f\x70\x74\x69\x6f\x6e\x20\x3c\x69\x3e\x3c\x66\x6f\x6e\x74\x20\x63\x6f\x6c\x6f\x72\x3d\x22\x23\x30\x30\x30\x30\x37\x66\x22\x3e\x22\x45\x66\x66\x61\x63\x65\x72\x20\x63\x65\x74\x20\x61\x63\x74\x65\x22\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x69\x3e\x20\x64\x65\x20\x63\x65\x20\x6d\x65\x6e\x75\x2e\x3c\x62\x72\x3e\xd\xa\x3c\x62\x3e\x41\x73\x74\x75\x63\x65\x20\x3a\x20\x3c\x2f\x62\x3e\x49\x6c\x20\x65\x73\x74\x20\x70\x6f\x73\x73\x69\x62\x6c\x65\x20\x64\x65\x20\x66\x61\x69\x72\x65\x20\x75\x6e\x65\x20\x73\xc3\xa9\x6c\x65\x63\x74\x69\x6f\x6e\x20\x6d\x75\x6c\x74\x69\x70\x6c\x65\x20\xc3\xa0\x20\x6c\x27\x61\x69\x64\x65\x20\x64\x65\x20\x6c\x61\x20\x74\x6f\x75\x63\x68\x65\x20\x3c\x66\x6f\x6e\x74\x20\x63\x6f\x6c\x6f\x72\x3d\x22\x23\x30\x30\x30\x30\x37\x66\x22\x3e\x3c\x69\x3e\x22\x43\x6f\x6e\x74\x72\x6f\x6c\x22\x3c\x2f\x69\x3e\x3c\x2f\x66\x6f\x6e\x74\x3e\x20\x6f\x75\x20\x3c\x66\x6f\x6e\x74\x20\x63\x6f\x6c\x6f\x72\x3d\x22\x23\x30\x30\x30\x30\x37\x66\x22\x3e\x3c\x69\x3e\x22\x53\x68\x69\x66\x74\x22\x3c\x2f\x69\x3e\x3c\x2f\x66\x6f\x6e\x74\x3e\x20\x61\x70\x70\x75\x79\xc3\xa9\x65\x20\x74\x6f\x75\x74\x20\x65\x6e\x20\x63\x6c\x69\x71\x75\x61\x6e\x74\x20\x61\x76\x65\x63\x20\x6c\x61\x20\x73\x6f\x75\x72\x69\x73\x2e\x20\x43\x65\x6c\x61\x20\x70\x65\x72\x6d\x65\x74\x20\x64\x27\x61\x66\x66\x65\x63\x74\x65\x72\x20\x6c\x27\x6f\x70\x74\x69\x6f\x6e\x20\x64\x75\x20\x6d\x65\x6e\x75\x20\x63\x6f\x6e\x74\x65\x78\x74\x75\x65\x6c\x20\xc3\xa0\x20\x70\x6c\x75\x73\x69\x65\x75\x72\x73\x20\x6c\x69\x67\x6e\x65\x73\x20\x65\x6e\x20\x6d\xc3\xaa\x6d\x65\x20\x74\x65\x6d\x70\x73\x20\x28\x6e\x6f\x6e\x20\x70\x6f\x73\x73\x69\x62\x6c\x65\x20\x70\x6f\x75\x72\x20\x63\x65\x72\x74\x61\x69\x6e\x73\x20\x63\x68\x61\x6d\x70\x73\x20\xc3\xa0\x20\x76\x61\x6c\x65\x75\x72\x20\x75\x6e\x69\x71\x75\x65\x29\x2e" ) );
    textLabel2_2->setText( tr( "<u><h3>Liste des actes composant cette FSE</h3></u>" ) );
    groupBox10->setTitle( QString::null );
    textLabel_ActeEnCours_Tarif->setText( tr( "<b><p align=\"center\">Prix  de base : 20,00 Euros</p></b>" ) );
    comboBox_ActeEnCours_Code->clear();
    comboBox_ActeEnCours_Code->insertItem( tr( "C (consultation simple)" ) );
    textLabel15_2->setText( trUtf8( "\x3c\x62\x3e\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x63\x65\x6e\x74\x65\x72\x22\x3e\x52\xc3\xa9\x73\x75\x6c\x74\x61\x74\x2e\x3c\x2f\x70\x3e\x3c\x2f\x62\x3e" ) );
    textLabel_ActeEnCours_Prix->setText( tr( "20" ) );
    textLabel12->setText( tr( "<b><p align=\"center\">Nbr</p></b>" ) );
    comboBox_ActeEnCours_Nb->clear();
    comboBox_ActeEnCours_Nb->insertItem( tr( "1" ) );
    comboBox_ActeEnCours_Nb->insertItem( tr( "2" ) );
    comboBox_ActeEnCours_Nb->insertItem( tr( "3" ) );
    comboBox_ActeEnCours_Nb->insertItem( tr( "4" ) );
    comboBox_ActeEnCours_Nb->insertItem( tr( "5" ) );
    comboBox_ActeEnCours_Nb->insertItem( tr( "6" ) );
    comboBox_ActeEnCours_Nb->insertItem( tr( "7" ) );
    textLabel15->setText( tr( "<b><p align=\"center\">Coeff.</p></b>" ) );
    lineEdit_ActeEnCours_Coef->setText( tr( "1" ) );
    QToolTip::add( lineEdit_ActeEnCours_Coef, trUtf8( "\x4c\x65\x20\x63\x6f\x65\x66\x66\x69\x63\x69\x65\x6e\x74\x20\x65\x73\x74\x20\x70\x61\x72\x20\x64\xc3\xa9\x66\x61\x75\x74\x20\xc3\xa0\x20\x31\x20\x6d\x61\x69\x73\x20\x63\x65\x72\x74\x61\x69\x6e\x73\x20\x61\x63\x74\x65\x73\x20\x64\x65\x6d\x61\x6e\x64\x65\x6e\x74\x20\x64\x27\x61\x75\x74\x72\x65\x73\x20\x76\x61\x6c\x65\x75\x72\x73\x20\x65\x78\x65\x6d\x70\x6c\x65\x20\x3a\x20\x4b\x20\x61\x76\x65\x63\x20\x63\x6f\x65\x66\x66\x69\x63\x69\x65\x6e\x74\x20\x36\x2c\x35" ) );
    QWhatsThis::add( lineEdit_ActeEnCours_Coef, trUtf8( "\x4c\x65\x20\x63\x6f\x65\x66\x66\x69\x63\x69\x65\x6e\x74\x20\x65\x73\x74\x20\x70\x61\x72\x20\x64\xc3\xa9\x66\x61\x75\x74\x20\xc3\xa0\x20\x31\x20\x6d\x61\x69\x73\x20\x63\x65\x72\x74\x61\x69\x6e\x73\x20\x61\x63\x74\x65\x73\x20\x64\x65\x6d\x61\x6e\x64\x65\x6e\x74\x20\x64\x27\x61\x75\x74\x72\x65\x73\x20\x76\x61\x6c\x65\x75\x72\x73\x20\x65\x78\x65\x6d\x70\x6c\x65\x20\x3a\x3c\x62\x72\x3e\xd\xa\x20\x4b\x20\x61\x76\x65\x63\x20\x63\x6f\x65\x66\x66\x69\x63\x69\x65\x6e\x74\x20\x36\x2c\x35" ) );
    textLabel_ActeEnCours_Tarif_2->setText( tr( "<b><p align=\"center\">Majoration</p></b>" ) );
    comboBox_ActeEnCours_Majoration->clear();
    comboBox_ActeEnCours_Majoration->insertItem( trUtf8( "\x4e\xc3\xa9\x61\x6e\x74" ) );
    comboBox_ActeEnCours_Majoration->insertItem( tr( "Nuit" ) );
    comboBox_ActeEnCours_Majoration->insertItem( trUtf8( "\x44\x69\x6d\x61\x6e\x63\x68\x65\x20\x65\x74\x20\x66\xc3\xa9\x72\x69\xc3\xa9\x73" ) );
    comboBox_ActeEnCours_Majoration->insertItem( tr( "Visite" ) );
    textLabel_ActeEnCours_Prix_2->setText( tr( "20" ) );
    textBrowser_ActeEnCours->setText( tr( "<b>AMO taux %</b>      80<br>\n"
"        <b>base</b> 20,00<br>\n"
"<b>part AMO</b> 16" ) );
    textBrowserHelpActeEnCours->setText( trUtf8( "\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x63\x65\x6e\x74\x65\x72\x22\x3e\x3c\x66\x6f\x6e\x74\x20\x73\x69\x7a\x65\x3d\x22\x2b\x31\x22\x3e\x3c\x62\x3e\x5a\x6f\x6e\x65\x20\x64\x27\xc3\xa9\x64\x69\x74\x69\x6f\x6e\x20\x64\x65\x73\x20\x61\x63\x74\x65\x73\x3c\x2f\x62\x3e\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x70\x3e\xd\xa\x4c\x6f\x72\x73\x71\x75\x65\x20\x76\x6f\x75\x73\x20\x63\x6c\x69\x71\x75\x65\x7a\x20\x73\x75\x72\x20\x75\x6e\x20\x61\x63\x74\x65\x20\x64\x65\x20\x6c\x61\x20\x3c\x62\x3e\x6c\x69\x73\x74\x65\x20\x64\x65\x73\x20\x61\x63\x74\x65\x73\x3c\x2f\x62\x3e\x2c\x20\x63\x65\x6c\x75\x69\x2d\x63\x69\x20\x76\x69\x65\x6e\x74\x20\x73\x65\x20\x70\x6c\x61\x63\x65\x72\x20\x64\x61\x6e\x73\x20\x6c\x61\x20\x7a\x6f\x6e\x65\x20\x64\x27\xc3\xa9\x64\x69\x74\x69\x6f\x6e\x20\x63\x69\x2d\x64\x65\x73\x73\x6f\x75\x73\x20\x69\x63\x69\x20\x6d\xc3\xaa\x6d\x65\x2e\x20\x3c\x62\x72\x3e\xd\xa\x3c\x75\x3e\x50\x6f\x75\x72\x20\x76\x61\x6c\x69\x64\x65\x72\x20\x6c\x65\x73\x20\x6d\x6f\x64\x69\x66\x69\x63\x61\x74\x69\x6f\x6e\x73\x3c\x2f\x75\x3e\x20\x63\x6c\x69\x71\x75\x65\x7a\x20\x73\x75\x72\x20\x6c\x65\x20\x62\x6f\x75\x74\x6f\x6e\x20\x3c\x62\x3e\x56\x61\x6c\x69\x64\x65\x72\x20\x6c\x27\x61\x63\x74\x65\x3c\x2f\x62\x3e\x2e\x3c\x62\x72\x3e\xd\xa\x3c\x75\x3e\x50\x6f\x75\x72\x20\x61\x6a\x6f\x75\x74\x65\x72\x20\x75\x6e\x20\x61\x63\x74\x65\x20\xc3\xa0\x20\x6c\x61\x20\x6c\x69\x73\x74\x65\x3c\x2f\x75\x3e\x20\x64\x65\x73\x20\x61\x63\x74\x65\x73\x20\x63\x6c\x69\x71\x75\x65\x7a\x20\x73\x75\x72\x20\x6c\x65\x20\x62\x6f\x75\x74\x6f\x6e\x20\x3c\x62\x3e\x41\x6a\x6f\x75\x74\x65\x72\x20\x6c\x27\x61\x63\x74\x65\x3c\x2f\x62\x3e\x2e" ) );
    textLabel_ActeEnCours_Tarif_2_2->setText( tr( "<b><p align=\"center\">nature de l'acte</p></b>" ) );
    comboBox_ActeEnCours_Nature->clear();
    comboBox_ActeEnCours_Nature->insertItem( trUtf8( "\x4e\xc3\xa9\x61\x6e\x74" ) );
    comboBox_ActeEnCours_Nature->insertItem( tr( "Acte Gratuit" ) );
    comboBox_ActeEnCours_Nature->insertItem( tr( "Exigence du patient" ) );
    comboBox_ActeEnCours_Nature->insertItem( tr( "Non remboursable" ) );
    pushButton_ActeEnCours_Valider->setText( tr( "Valider l'acte" ) );
    pushButton_ActeEnCours_Ajouter->setText( tr( "Ajouter l'acte" ) );
    textLabel9_3->setText( tr( "<u><h3>Total de l'acte : </h3></u>" ) );
    checkBox_ActeEnCours_Entente->setText( trUtf8( "\x45\x6e\x74\x65\x6e\x74\x65\x20\x70\x72\xc3\xa9\x61\x6c\x61\x62\x6c\x65" ) );
    checkBox_ActeEnCours_Cabinet->setText( tr( "En cabinet" ) );
    checkBox_ActeEnCours_RMO->setText( tr( "RMO" ) );
    tabWidget3->changeTab( tab_Actes, tr( "Edition de l'acte " ) );
    textLabel_TitleLisActeBase->setText( tr( "Actes de base<br>dermatologie" ) );
    textLabel_LibelleActeBase->setText( QString::null );
    tabWidget3->changeTab( TabActeParam, trUtf8( "\x50\x61\x72\x61\x6d\xc3\xa9\x74\x72\x61\x67\x65\x20\x64\x65\x73\x20\x61\x63\x74\x65\x73" ) );
    tabWidget3->changeTab( TabAide, tr( "Aide" ) );
    groupBox7->setTitle( QString::null );
    QToolTip::add( toolBox_PEC, trUtf8( "\x43\x6c\x69\x71\x75\x65\x7a\x20\x73\x75\x72\x20\x6c\x65\x20\x74\x79\x70\x65\x20\x64\x65\x20\x70\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x20\x72\x65\x6c\x61\x74\x69\x66\x20\xc3\xa0\x20\x63\x65\x74\x74\x65\x20\x66\x61\x63\x74\x75\x72\x65" ) );
    QWhatsThis::add( toolBox_PEC, trUtf8( "\x3c\x68\x33\x3e\x4d\x6f\x64\x65\x20\x64\x65\x20\x70\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x3c\x2f\x68\x33\x3e\xd\xa\x43\x6c\x69\x71\x75\x65\x7a\x20\x73\x75\x72\x20\x6c\x65\x20\x74\x79\x70\x65\x20\x64\x65\x20\x70\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x20\x72\x65\x6c\x61\x74\x69\x66\x20\xc3\xa0\x20\x63\x65\x74\x74\x65\x20\x66\x61\x63\x74\x75\x72\x65\x2c\xd\xa\x65\x74\x20\x72\x65\x6e\x73\x65\x69\x67\x6e\x65\x7a\x20\x6c\x65\x73\x20\xc3\xa9\x6c\xc3\xa9\x6d\x65\x6e\x74\x73\x20\x72\x65\x6c\x61\x74\x69\x66\x73\x20\xc3\xa0\x20\x63\x65\x74\x74\x65\x20\x70\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x2e" ) );
    toolBox_PEC->setItemLabel( toolBox_PEC->indexOf(Maladie), tr( "Maladie" ) );
    toolBox_PEC->setItemLabel( toolBox_PEC->indexOf(Maternite), trUtf8( "\x4d\x61\x74\x65\x72\x6e\x69\x74\xc3\xa9" ) );
    toolBox_PEC->setItemLabel( toolBox_PEC->indexOf(Accident_de_travail), tr( "AT ou MP Accident de travail" ) );
    QWhatsThis::add( widgetStack_PEC, trUtf8( "\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x63\x65\x6e\x74\x65\x72\x22\x3e\x3c\x75\x3e\x3c\x62\x3e\x3c\x66\x6f\x6e\x74\x20\x73\x69\x7a\x65\x3d\x22\x2b\x32\x22\x3e\x4e\x61\x74\x75\x72\x65\x20\x64\x27\x61\x73\x73\x75\x72\x61\x6e\x63\x65\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x62\x3e\x3c\x2f\x75\x3e\x3c\x2f\x70\x3e\xd\xa\xd\xa\x3c\x62\x3e\x3c\x66\x6f\x6e\x74\x20\x73\x69\x7a\x65\x3d\x22\x2b\x31\x22\x3e\x43\x6f\x6e\x64\x69\x74\x69\x6f\x6e\x73\x20\x64\x65\x20\x70\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x20\x4d\x61\x6c\x61\x64\x69\x65\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x62\x3e\x3c\x62\x72\x3e\xd\xa\xc2\xb0\x20\x4c\x6f\x72\x73\x71\x75\x65\x20\x6c\x65\x73\x20\x61\x63\x74\x65\x73\x20\x73\x6f\x6e\x74\x20\x6e\x65\x20\x73\x6f\x6e\x74\x20\x70\x61\x73\x20\x65\x6e\x20\x72\x61\x70\x70\x6f\x72\x74\x20\x61\x76\x65\x63\x20\x75\x6e\x20\x3c\x69\x3e\x27\x61\x63\x63\x69\x64\x65\x6e\x74\x20\x64\x65\x20\x74\x72\x61\x76\x61\x69\x6c\x27\x3c\x2f\x69\x3e\x20\x6e\x69\x20\x75\x6e\x65\x20\x70\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x20\x3c\x69\x3e\x27\x4d\x61\x74\x65\x72\x6e\x69\x74\xc3\xa9\x27\x3c\x2f\x69\x3e\x2e\x3c\x62\x72\x3e\xd\xa\xc2\xb0\x20\x43\x6f\x63\x68\x65\x7a\x20\x6c\x61\x20\x63\x61\x73\x65\x3c\x69\x3e\x20\x27\x41\x63\x63\x69\x64\x65\x6e\x74\x20\x63\x61\x75\x73\xc3\xa9\x20\x70\x61\x72\x20\x75\x6e\x20\x74\x69\x65\x72\x73\x27\x3c\x2f\x69\x3e\x20\x73\x69\x20\x63\x65\x6c\x61\x20\x65\x73\x74\x20\x6c\x65\x20\x63\x61\x73\x20\x65\x74\x20\x69\x6e\x64\x69\x71\x75\x65\x7a\x20\x61\x6c\x6f\x72\x73\x20\x6c\x61\x20\x64\x61\x74\x65\x20\x64\x65\x20\x6c\x27\x61\x63\x63\x69\x64\x65\x6e\x74\x20\x64\x61\x6e\x73\x20\x6c\x61\x20\x63\x61\x73\x65\x20\x70\x72\xc3\xa9\x76\x75\x65\x20\xc3\xa0\x20\x63\x65\x74\x74\x65\x20\x65\x66\x66\x65\x74\x2e\x3c\x62\x72\x3e\x3c\x62\x72\x3e\xd\xa\xd\xa\x3c\x62\x3e\x3c\x66\x6f\x6e\x74\x20\x73\x69\x7a\x65\x3d\x22\x2b\x31\x22\x3e\x43\x6f\x6e\x64\x69\x74\x69\x6f\x6e\x73\x20\x64\x65\x20\x70\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x20\x41\x63\x63\x69\x64\x65\x6e\x74\x20\x64\x65\x20\x74\x72\x61\x76\x61\x69\x6c\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x62\x3e\x3c\x62\x72\x3e\xc2\xb0\x20\x4c\x6f\x72\x73\x71\x75\x65\x20\x6c\x65\x73\x20\x61\x63\x74\x65\x73\x20\x73\x6f\x6e\x74\x20\x65\x6e\x20\x72\x61\x70\x70\x6f\x72\x74\x20\x61\x76\x65\x63\x20\x75\x6e\x20\x61\x63\x63\x69\x64\x65\x6e\x74\x20\x64\x65\x20\x74\x72\x61\x76\x61\x69\x6c\x2e\x3c\x62\x72\x3e\xd\xa\xc2\xb0\x20\x49\x6e\x64\x69\x71\x75\x65\x7a\x20\x6c\x61\x20\x64\x61\x74\x65\x20\x64\x65\x20\x6c\x27\x61\x63\x63\x69\x64\x65\x6e\x74\x20\x64\x61\x6e\x73\x20\x6c\x61\x20\x63\x61\x73\x65\x20\x70\x72\xc3\xa9\x76\x75\x65\x20\xc3\xa0\x20\x63\x65\x74\x74\x65\x20\x65\x66\x66\x65\x74\x2c\x20\x6c\x65\x20\x6e\x75\x6d\xc3\xa9\x72\x6f\x20\x65\x73\x74\x20\x66\x61\x63\x75\x6c\x74\x61\x74\x69\x66\x3c\x62\x72\x3e\x3c\x62\x72\x3e\xd\xa\xd\xa\x3c\x62\x3e\x3c\x66\x6f\x6e\x74\x20\x73\x69\x7a\x65\x3d\x22\x2b\x31\x22\x3e\x43\x6f\x6e\x64\x69\x74\x69\x6f\x6e\x73\x20\x64\x65\x20\x70\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x20\x4d\x61\x74\x65\x72\x6e\x69\x74\xc3\xa9\x3c\x2f\x66\x6f\x6e\x74\x3e\x3c\x2f\x62\x3e\x3c\x62\x72\x3e\xd\xa\xc2\xb0\x20\x4c\x6f\x72\x73\x20\x64\x65\x20\x6c\x27\x65\x78\x61\x6d\x65\x6e\x20\x69\x6e\x69\x74\x69\x61\x6c\x20\x63\x6f\x6e\x73\x74\x61\x74\x61\x6e\x74\x20\x6c\x61\x20\x67\x72\x6f\x73\x73\x65\x73\x73\x65\x2e\x3c\x62\x72\x3e\xd\xa\xc2\xb0\x20\x4c\x6f\x72\x73\x20\x64\x65\x20\x64\x65\x20\x6c\x27\x65\x78\x61\x6d\x65\x6e\x20\x6d\xc3\xa9\x64\x69\x63\x61\x6c\x20\x70\x72\xc3\xa9\x6e\x61\x74\x61\x6c\x20\x70\x6f\x75\x72\x20\x6c\x65\x20\x70\xc3\xa8\x72\x65\x2e\x3c\x62\x72\x3e\xd\xa\xc2\xb0\x20\x4c\x6f\x72\x73\x20\x64\x65\x73\x20\x73\x65\x70\x74\x73\x20\x65\x78\x61\x6d\x65\x6e\x73\x20\x70\x72\xc3\xa9\x2d\x6e\x61\x74\x61\x75\x78\x20\x3a\x20\x3c\x62\x72\x3e\xd\xa\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x2d\x20\x6c\x65\x20\x31\x65\x72\x20\x61\x76\x61\x6e\x74\x20\x6c\x61\x20\x66\x69\x6e\x20\x64\x75\x20\x33\xc3\xa8\x6d\x65\x20\x6d\x6f\x69\x73\x2e\x3c\x62\x72\x3e\xd\xa\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x2d\x20\x6c\x65\x73\x20\x73\x69\x78\x20\x61\x75\x74\x72\x65\x73\x20\x61\x75\x20\x63\x6f\x75\x72\x73\x20\x64\x75\x20\x34\xc3\xa8\x6d\x65\x2c\x20\x35\xc3\xa8\x6d\x65\x2c\x20\x36\xc3\xa8\x6d\x65\x2c\x20\x37\xc3\xa8\x6d\x65\x2c\x20\x38\xc3\xa8\x6d\x65\x2c\x20\x39\xc3\xa8\x6d\x65\x20\x6d\x6f\x69\x73\x20\x61\x76\x61\x6e\x74\x20\x6c\x27\x61\x63\x63\x6f\x75\x63\x68\x65\x6d\x65\x6e\x74\x3c\x62\x72\x3e\xd\xa\xc2\xb0\x20\x20\x4c\x6f\x72\x73\x20\x64\x65\x73\x20\x73\x6f\x69\x6e\x73\x20\x64\x69\x73\x70\x65\x6e\x73\xc3\xa9\x73\x20\xc3\xa0\x20\x6c\x61\x20\x6d\xc3\xa8\x72\x65\x20\x20\x64\x75\x20\x36\xc3\xa8\x6d\x65\x20\x6d\x6f\x69\x73\x20\x6a\x75\x73\x71\x75\x27\x61\x75\x20\x74\x65\x72\x6d\x65\x3c\x62\x72\x3e\xd\xa\xc2\xb0\x20\x20\x6c\x27\x65\x78\x61\x6d\x65\x6e\x20\x70\x6f\x73\x74\x2d\x6e\x61\x74\x61\x6c\x20\x64\x65\x20\x6c\x61\x20\x6d\xc3\xa8\x72\x65\x20\x64\x61\x6e\x73\x20\x6c\x65\x73\x20\x38\x20\x73\x65\x6d\x61\x69\x6e\x65\x73\x20\x71\x75\x69\x20\x73\x75\x69\x76\x65\x6e\x74\x20\x6c\x27\x61\x63\x63\x6f\x75\x63\x68\x65\x6d\x65\x6e\x74\x3c\x62\x72\x3e\x20\xd\xa\xc2\xb0\x20\x20\x4c\x65\x73\x20\x65\x78\x61\x6d\x65\x6e\x73\x20\x64\x75\x20\x6e\x6f\x75\x72\x72\x69\x73\x73\x6f\x6e\x20\x3a\x3c\x62\x72\x3e\x20\x20\xd\xa\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x20\x2d\x20\x61\x75\x20\x63\x6f\x75\x72\x73\x20\x64\x65\x20\x6c\x61\x20\x31\xc3\xa8\x72\x65\x20\x61\x6e\x6e\xc3\xa9\x65\x20\x3a\x20\x3c\x62\x72\x3e\xd\xa\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x20\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x20\x75\x6e\x20\x70\x65\x6e\x64\x61\x6e\x74\x20\x6c\x65\x73\x20\x38\x20\x70\x72\x65\x6d\x69\x65\x72\x73\x20\x6a\x6f\x75\x72\x73\x3c\x62\x72\x3e\xd\xa\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x20\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x20\x70\x65\x6e\x64\x61\x6e\x74\x20\x6c\x65\x73\x20\x31\x65\x72\x2c\x20\x32\xc3\xa8\x6d\x65\x2c\x20\x33\xc3\xa8\x6d\x65\x2c\x20\x34\xc3\xa8\x6d\x65\x20\x35\xc3\xa8\x6d\x65\x2c\x20\x36\xc3\xa8\x6d\x65\x2c\x20\x39\xc3\xa8\x6d\x65\x20\x65\x74\x20\x31\x32\xc3\xa8\x6d\x65\x20\x6d\x6f\x69\x73\x3c\x62\x72\x3e\xd\xa\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x20\x2d\x20\x61\x75\x20\x63\x6f\x75\x72\x73\x20\x64\x65\x20\x6c\x61\x20\x32\xc3\xa8\x20\x61\x6e\x6e\xc3\xa9\x65\x20\x3a\x20\x31\x36\xc3\xa8\x6d\x65\x2c\x20\x32\x30\xc3\xa8\x6d\x65\x20\x65\x74\x20\x32\x34\xc3\xa8\x6d\x65\x20\x6d\x6f\x69\x73\x3c\x62\x72\x3e\xd\xa\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x26\x6e\x62\x73\x70\x3b\x20\x2d\x20\x6c\x6f\x72\x73\x20\x64\x65\x73\x20\x65\x78\x61\x6d\x65\x6e\x73\x20\x73\x65\x6d\x65\x73\x74\x72\x69\x65\x6c\x73\x20\x70\x6f\x75\x72\x20\x6c\x65\x73\x20\x34\x20\x61\x6e\x6e\xc3\xa9\x65\x73\x20\x73\x75\x69\x76\x61\x6e\x74\x65\x73\x2e" ) );
    textLabel5_2_2->setText( tr( "<h2><p align=\"center\">Prise en charge maladie</p></h2>" ) );
    textLabel1_5->setText( trUtf8( "\x65\x78\x6f\x6e\xc3\xa9\x72\x61\x74\x69\x6f\x6e\x20\x64\x75\x20\x74\x69\x63\x6b\x65\x74\x20\x6d\x6f\x64\xc3\xa9\x72\x61\x74\x65\x75\x72\x20\x3a" ) );
    textLabel1_5_2->setText( trUtf8( "\x41\x63\x63\x69\x64\x65\x6e\x74\x20\x63\x61\x75\x73\xc3\xa9\x20\x70\x61\x72\x20\x75\x6e\x20\x74\x69\x65\x72\x73\x20\x3a" ) );
    checkBox_ExoTicketMod_Non->setText( tr( "Non" ) );
    checkBox_AccParTiers_Non->setText( tr( "Non" ) );
    checkBox_ExoTicketMod_Oui->setText( tr( "Oui" ) );
    checkBox_AccParTiers_Oui->setText( tr( "Oui" ) );
    textLabel2->setText( QString::null );
    pushButton_FSE_AccParTiers_Calendrier->setText( tr( "Date" ) );
    QToolTip::add( pushButton_FSE_AccParTiers_Calendrier, tr( "Modifier la date de fin d'Accident de Travail" ) );
    textLabel5->setText( trUtf8( "\x3c\x68\x32\x3e\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x63\x65\x6e\x74\x65\x72\x22\x3e\x50\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x20\x6c\x69\xc3\xa9\x65\x20\xc3\xa0\x20\x6c\x61\x20\x6d\x61\x74\x65\x72\x6e\x69\x74\xc3\xa9\x3c\x2f\x70\x3e\x3c\x2f\x68\x32\x3e" ) );
    pushButton_DebutGross_Calendrier->setText( trUtf8( "\x44\x61\x74\x65\x20\x70\x72\xc3\xa9\x73\x75\x6d\xc3\xa9\x65\x20\x64\x75\x20\x64\xc3\xa9\x62\x75\x74\x20\x64\x65\x20\x67\x72\x6f\x73\x73\x65\x73\x73\x65\x20\x3a" ) );
    QToolTip::add( pushButton_DebutGross_Calendrier, tr( "Modifier la date de la facture" ) );
    textLabel1_4_2_3_2_2_3->setText( trUtf8( "\x4e\x75\x6d\xc3\xa9\x72\x6f" ) );
    QToolTip::add( lineEdit_ATMP_Numero, tr( "Modifier  le nombre de jours d'Accident de Travail" ) );
    pushButton_ATMP_Date->setText( tr( "Date" ) );
    QToolTip::add( pushButton_ATMP_Date, tr( "Modifier la date de fin d'Accident de Travail" ) );
    textLabel5_2->setText( trUtf8( "\x3c\x68\x32\x3e\x3c\x70\x20\x61\x6c\x69\x67\x6e\x3d\x22\x63\x65\x6e\x74\x65\x72\x22\x3e\x50\x72\x69\x73\x65\x20\x65\x6e\x20\x63\x68\x61\x72\x67\x65\x20\x6c\x69\xc3\xa9\x65\x20\xc3\xa0\x20\x75\x6e\x20\x41\x63\x63\x69\x64\x65\x6e\x74\x20\x64\x65\x20\x74\x72\x61\x76\x61\x69\x6c\x20\x6f\x75\x20\x75\x6e\x65\x20\x4d\x61\x6c\x61\x64\x69\x65\x20\x70\x72\x6f\x66\x65\x73\x73\x69\x6f\x6e\x6e\x65\x6c\x6c\x65\x3c\x2f\x70\x3e\x3c\x2f\x68\x32\x3e" ) );
    buttonGroup_FSE_FactureType->setTitle( tr( "Type de facture" ) );
    radioButtonDRE_FSE->setText( tr( "DRE et FSE (3)" ) );
    radioButtonDRE_Seule->setText( tr( "DRE Seule (2)" ) );
    radioButtonFSE_Seule->setText( tr( "FSE Seule (1)" ) );
    textLabel1_4_2_2->setText( tr( "Date de la facture" ) );
    pushButton_FSE_Calendrier->setText( tr( "..." ) );
    QToolTip::add( pushButton_FSE_Calendrier, tr( "Modifier la date de la facture" ) );
    buttonGroup_FSE_Securisation->setTitle( trUtf8( "\x4d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e" ) );
    radioButtonSec_SESAM_VItale->setText( tr( "SESAM Vitale (3)" ) );
    QToolTip::add( radioButtonSec_SESAM_VItale, trUtf8( "\x43\x65\x20\x6d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x6e\xc3\xa9\x63\x65\x73\x73\x69\x74\x65\x20\x6c\x61\x20\x70\x72\xc3\xa9\x73\x65\x6e\x63\x65\x20\x73\x69\x6d\x75\x6c\x74\x61\x6e\xc3\xa9\x65\x20\x64\x65\x73\x20\x63\x61\x72\x74\x65\x73\x20\x43\x50\x53\x20\x65\x74\x20\x56\x69\x74\x61\x6c\x65\x20\x64\x61\x6e\x73\x20\x6c\x65\x20\x6c\x65\x63\x74\x65\x75\x72\x2e" ) );
    QWhatsThis::add( radioButtonSec_SESAM_VItale, trUtf8( "\x3c\x68\x33\x3e\x4d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x53\x45\x53\x41\x4d\x20\x56\x69\x74\x61\x6c\x65\x3c\x2f\x68\x33\x3e\xd\xa\x43\x65\x20\x6d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x6e\xc3\xa9\x63\x65\x73\x73\x69\x74\x65\x20\x6c\x61\x20\x70\x72\xc3\xa9\x73\x65\x6e\x63\x65\x20\x73\x69\x6d\x75\x6c\x74\x61\x6e\xc3\xa9\x65\x20\x64\x65\x73\x20\x63\x61\x72\x74\x65\x73\x20\x43\x50\x53\x20\x65\x74\x20\x56\x69\x74\x61\x6c\x65\x20\x64\x61\x6e\x73\x20\x6c\x65\x20\x6c\x65\x63\x74\x65\x75\x72\x2e\x3c\x62\x72\x3e\xd\xa\x4c\x65\x73\x20\x64\x6f\x6e\x6e\xc3\xa9\x65\x73\x20\x73\x65\x6e\x73\x69\x62\x6c\x65\x73\x20\x73\x6f\x6e\x74\x20\x63\x68\x69\x66\x66\x72\xc3\xa9\x65\x73\x20\x65\x74\x20\x6c\x65\x73\x20\x66\x61\x63\x74\x75\x72\x65\x73\x20\x73\x69\x67\x6e\xc3\xa9\x65\x73\x2e" ) );
    radioButtonSec_Desynchro_Vitale->setText( trUtf8( "\x44\xc3\xa9\x73\x79\x6e\x63\x68\x72\x6f\x2e\x20\x61\x76\x65\x63\x20\x56\x69\x74\x61\x6c\x65\x20\x28\x34\x29" ) );
    QToolTip::add( radioButtonSec_Desynchro_Vitale, trUtf8( "\x4d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x65\x6e\x20\x64\x65\x75\x78\x20\x74\x65\x6d\x70\x73\x20\x3a\x20\x70\x72\x65\x6d\x69\xc3\xa8\x72\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x61\x76\x65\x63\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65\x20\x73\x65\x75\x6c\x65\x2c\x20\x6c\x61\x20\x64\x65\x75\x78\x69\xc3\xa8\x6d\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x65\x6e\x20\x70\x72\xc3\xa9\x73\x65\x6e\x63\x65\x20\x64\x65\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x43\x50\x53\x20\x73\x65\x75\x6c\x65\x2e" ) );
    QWhatsThis::add( radioButtonSec_Desynchro_Vitale, trUtf8( "\x3c\x68\x33\x3e\x4d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x44\xc3\xa9\x73\x79\x6e\x63\x68\x72\x6f\x2e\x20\x61\x76\x65\x63\x20\x56\x69\x74\x61\x6c\x65\x20\x3c\x2f\x68\x33\x3e\xd\xa\x4d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x65\x6e\x20\x64\x65\x75\x78\x20\x74\x65\x6d\x70\x73\x20\x3a\x20\x3c\x62\x72\x3e\x50\x72\x65\x6d\x69\xc3\xa8\x72\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x61\x76\x65\x63\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65\x20\x73\x65\x75\x6c\x65\x3c\x62\x72\x3e\x20\x4c\x61\x20\x64\x65\x75\x78\x69\xc3\xa8\x6d\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x65\x6e\x20\x70\x72\xc3\xa9\x73\x65\x6e\x63\x65\x20\x64\x65\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x43\x50\x53\x20\x73\x65\x75\x6c\x65\x2e\x3c\x62\x72\x3e\x4c\x65\x73\x20\x64\x6f\x6e\x6e\xc3\xa9\x65\x73\x20\x73\x65\x6e\x73\x69\x62\x6c\x65\x73\x20\x73\x6f\x6e\x74\x20\x63\x68\x69\x66\x66\x72\xc3\xa9\x65\x73\x20\x65\x74\x20\x6c\x65\x73\x20\x66\x61\x63\x74\x75\x72\x65\x73\x20\x73\x69\x67\x6e\xc3\xa9\x65\x73\x2e" ) );
    radioButtonSec_SesamDegrade->setText( trUtf8( "\x53\x45\x53\x41\x4d\x20\x44\xc3\xa9\x67\x72\x61\x64\xc3\xa9\x20\x28\x35\x29" ) );
    QToolTip::add( radioButtonSec_SesamDegrade, trUtf8( "\x4d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x64\x61\x6e\x73\x20\x6c\x61\x71\x75\x65\x6c\x6c\x65\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65\x20\xc3\xa9\x74\x61\x6e\x74\x20\x61\x62\x73\x65\x6e\x74\x65\x2c\x20\x6c\x61\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x65\x73\x74\x20\x72\xc3\xa9\x61\x6c\x69\x73\xc3\xa9\x65\x20\x70\x61\x72\x20\x6c\x61\x20\x43\x50\x53\x20\x73\x65\x75\x6c\x65\x2e\x20" ) );
    QWhatsThis::add( radioButtonSec_SesamDegrade, trUtf8( "\x3c\x68\x33\x3e\x4d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x53\x45\x53\x41\x4d\x20\x44\xc3\xa9\x67\x72\x61\x64\xc3\xa9\x3c\x2f\x68\x33\x3e\x20\xd\xa\x4d\x6f\x64\x65\x20\x64\x65\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x64\x61\x6e\x73\x20\x6c\x61\x71\x75\x65\x6c\x6c\x65\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65\x20\xc3\xa9\x74\x61\x6e\x74\x20\x61\x62\x73\x65\x6e\x74\x65\x2c\x20\x3c\x62\x72\x3e\x6c\x61\x20\x73\xc3\xa9\x63\x75\x72\x69\x73\x61\x74\x69\x6f\x6e\x20\x65\x73\x74\x20\x72\xc3\xa9\x61\x6c\x69\x73\xc3\xa9\x65\x20\x70\x61\x72\x20\x6c\x61\x20\x43\x50\x53\x20\x73\x65\x75\x6c\x65\x2e\x3c\x62\x72\x3e\x4c\x65\x73\x20\x64\x6f\x6e\x6e\xc3\xa9\x65\x73\x20\x73\x65\x6e\x73\x69\x62\x6c\x65\x73\x20\x73\x6f\x6e\x74\x20\x63\x68\x69\x66\x66\x72\xc3\xa9\x65\x73\x20\x65\x74\x20\x6c\x65\x73\x20\x66\x61\x63\x74\x75\x72\x65\x73\x20\x73\x69\x67\x6e\xc3\xa9\x65\x73\x20\x70\x61\x72\x20\x6c\x61\x20\x43\x50\x53\x2e\x3c\x62\x72\x3e\xd\xa\x4d\x6f\x64\x65\x20\xc3\xa0\x20\x75\x74\x69\x6c\x69\x73\x65\x72\x20\x65\x6e\x20\x63\x61\x73\x20\x64\x27\x61\x62\x73\x65\x6e\x63\x65\x20\x6f\x75\x20\x64\x79\x73\x66\x6f\x6e\x63\x74\x69\x6f\x6e\x6e\x65\x6d\x65\x6e\x74\x20\x64\x65\x20\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x56\x69\x74\x61\x6c\x65\x2c\x20\x6f\x75\x20\x46\x53\x45\x20\x41\x54\x20\x6c\x6f\x72\x73\x71\x75\x65\x20\x6c\x27\x6f\x72\x67\x61\x6e\x69\x73\x6d\x65\x20\x64\x65\x20\x6c\x61\x20\x66\x61\x63\x74\x75\x72\x65\x3c\x62\x72\x3e\x20\x65\x73\x74\x20\x64\x69\x66\x66\xc3\xa9\x72\x65\x6e\x74\x20\x64\x65\x20\x63\x65\x6c\x75\x69\x20\x64\x65\x6c\x61\x20\x63\x61\x72\x74\x65\x20\x76\x69\x74\x61\x6c\x65\x2e\x3c\x62\x72\x3e\x43\x65\x20\x6d\x6f\x64\x65\x20\x6e\xc3\xa9\x63\x65\x73\x73\x69\x74\x65\x20\x6c\x27\x65\x6e\x76\x6f\x69\x20\x64\x27\x75\x6e\x65\x20\x66\x65\x75\x69\x6c\x6c\x65\x20\x64\x65\x20\x73\x6f\x69\x6e\x73\x20\x70\x61\x70\x69\x65\x72\x20\x65\x6e\x20\x70\x61\x72\x61\x6c\x6c\xc3\xa8\x6c\x65\x2e" ) );
    tabWidget_Beneficiaire->changeTab( Onglet_FSE, tr( "FSE" ) );
    textLabel1->setText( tr( "Carte CPS" ) );
    textLabel1_2->setText( tr( "Carte Vitale" ) );
    pushButton_FSE_ToListView->setText( tr( "..." ) );
    textLabel1_3->setText( tr( "FSE" ) );
    listView_FseGroup->header()->setLabel( 0, tr( "Type" ) );
    listView_FseGroup->header()->setLabel( 1, tr( "Valeur" ) );
    listView_FseGroup->header()->setLabel( 2, trUtf8( "\x4c\x69\x62\x65\x6c\x6c\xc3\xa9" ) );
    listView_FseGroup->header()->setLabel( 3, tr( "Occurence" ) );
    tabWidget_Beneficiaire->changeTab( Onglet_Monitoring, tr( "Monitoring" ) );
}

