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


#include "C_Dlg_Print.h"
#ifdef Q_WS_X11
  #include <QX11Info>
#endif


//-------------------------------------- C_Dlg_Print -------------------------------------------
C_Dlg_Print::C_Dlg_Print( Q3TextEdit*       pQTextEdit,
                          QString*          pHeadText,
                          QString*          pBodyText,
                          QString*          pFootText,
                          const QString   &printSectionName,
                          QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
: DlgPrint(parent,name, modal,fl)
{
    m_PrintSectionName   = printSectionName;
    m_pCViewPageWidget   = new CViewPageWidget(this);

    //.................. couleur des sliders ..............
    slider_Head_H->setStyleSheet("background-color: red");
    slider_Foot_H->setStyleSheet("background-color: red");
    if (m_pCViewPageWidget )
       {m_pCViewPageWidget->setGeometry (QRect (10,10, m_pCViewPageWidget->GetWidgetWidth(), m_pCViewPageWidget->GetWidgetHeight()));
       }
    m_pHeadRichText       = 0;
    m_pBodyRichText       = 0;
    m_pFootRichText       = 0;

    //....................... ajuster la geometrie du dialogue et des widgets .............................................
    QRect rectDlg    = this->geometry();
    QRect rectTool   = frameToolBar->geometry();
    QRect rectPage   = m_pCViewPageWidget->geometry();
    QRect rectscreen = QApplication::desktop()->rect();

    rectDlg.setHeight(Max (rectPage.height()+20, rectTool.height() +20));
    rectDlg.setWidth(rectPage.width()+10+rectTool.width()+10);
    rectTool.moveTop (10);
    rectTool.moveLeft (rectPage.width()+15);
    frameToolBar->setGeometry(rectTool);
    //.......................centrer le dialogue sur l'écran ..............................................................
    rectDlg.moveLeft( (rectscreen.width() -rectDlg.width()  ) / 2);
    rectDlg.moveTop ( (rectscreen.height()-rectDlg.height() ) / 2 );
    this->setGeometry(rectDlg);

    m_PageW = m_pCViewPageWidget->GetPageW();
    m_PageH = m_pCViewPageWidget->GetPageH();

    lineEdit_PageDeb->setText("1");
    //.................... declarer et instancier l'imprimante.................................
    m_pQPrinter = new QPrinter(QPrinter::ScreenResolution );  // PrinterResolution ScreenResolution HighResolution
    if (m_pQPrinter)
    {   m_pQPrinter->setFullPage(TRUE);
        // QPaintDeviceMetrics screen( this );
        // m_pQPrinter->setResolution( Max(screen.logicalDpiX(),screen.logicalDpiY()) );
        // m_pQPrinter->setResolution( screen.logicalDpiY() );
        m_pQPrinter->setResolution(96); // par defaut
        lineEditResoX->setText("96");
        lineEditResoY->setText("96");
    }

    //............................. variables permettant la reinitialisation du dialogue ................
    //                              sur de nouvelles en-têtes et pied de page
    QString val1         = "";
    QString val2         = "";
    m_pBodyTextEdit      = pQTextEdit;
    m_pBodyText          = pBodyText; //CreateBody();
    m_pHeadText          = pHeadText; //Get_Head_Foot (&m_FootText);
    m_pFootText          = pFootText;

#ifdef Q_WS_X11
    //.................... ajuster resolution d'impression .........................................
    if (CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName, "Resolution", &val1, &val2)==QString::null)  // zero = pas d'erreur
       {if (val2=="") val2=val1;
        lineEditResoX->setText(val1);
        lineEditResoY->setText(val2);
        m_pQPrinter->setResolution(val2.toInt());
        int screen = QX11Info().screen();
        QPaintDevice::x11SetAppDpiX(val1.toInt()-1,screen);
        QPaintDevice::x11SetAppDpiY(val2.toInt()-1,screen);
       }
#endif

    SetDialogOnHeadAndFoot(1);

    //.................... ajuster resolution d'impression .........................................
    int v;
    if (CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName, "MargTop",     &val1)==QString::null)      m_pCViewPageWidget->SetTopMarg(val1.toFloat()    / 10);
    if (CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName, "Marg_Left",   &val1)==QString::null)      m_pCViewPageWidget->SetLeftMarg(val1.toFloat()   / 10);
    if (CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName, "Marg_Right",  &val1)==QString::null)      m_pCViewPageWidget->SetRightMarg(val1.toFloat()  / 10);
    if (CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName, "Marg_Bottom", &val1)==QString::null)      m_pCViewPageWidget->SetBottomMarg(val1.toFloat() / 10);
    if (CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName, "Zoom",        &val1)==QString::null)      m_pCViewPageWidget->SetPrintScale(val1.toFloat() / 100);
    v = (int)(m_pCViewPageWidget->GetTopMarg()     *100/m_PageH ); slider_Marg_Top->setValue    (v);
    v = (int)(m_pCViewPageWidget->GetLeftMarg()    *100/m_PageW ); slider_Marg_Left->setValue   (v);
    v = (int)(m_pCViewPageWidget->GetRightMarg()   *100/m_PageW ); slider_Marg_Right->setValue  (v);
    v = (int)(m_pCViewPageWidget->GetBottomMarg()  *100/m_PageH ); slider_Marg_Bottom->setValue (v);

    lineEditZoom->setText(QString::number(m_pCViewPageWidget->GetPrintScale() *100));

    lineEdit_Marg_Top->setText   (QString::number(m_pCViewPageWidget->GetTopMarg()    *10));
    lineEdit_Head_H->setText     (QString::number(m_pCViewPageWidget->GetHead_H()     *10));
    lineEdit_Marg_Left->setText  (QString::number(m_pCViewPageWidget->GetLeftMarg()   *10));
    lineEdit_Marg_Right->setText (QString::number(m_pCViewPageWidget->GetRightMarg()  *10));
    lineEdit_Marg_Bottom->setText(QString::number(m_pCViewPageWidget->GetBottomMarg() *10));
    lineEdit_Foot_H->setText     (QString::number(m_pCViewPageWidget->GetFoot_H()     *10));

    slider_Marg_Top->setValue    ( (int)(m_pCViewPageWidget->GetTopMarg()     *100 / m_PageH ) );
    slider_Head_H->setValue      ( (int)(m_pCViewPageWidget->GetHead_H()      *100 / m_PageH ) );
    slider_Marg_Left->setValue   ( (int)(m_pCViewPageWidget->GetLeftMarg()    *100 / m_PageW ) );
    slider_Marg_Right->setValue  ( (int)(m_pCViewPageWidget->GetRightMarg()   *100 / m_PageW ) );
    slider_Foot_H->setValue      ( (int)(m_pCViewPageWidget->GetFoot_H()      *100 / m_PageH ) );
    slider_Marg_Bottom->setValue ( (int)(m_pCViewPageWidget->GetBottomMarg()  *100 / m_PageH ) );
    lineEditZoom->setText(QString::number(m_pCViewPageWidget->GetPrintScale() *100));

    //............................ positionner le nombre de copies ...........................
    if (CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName, "NumCopies",  &val1)==QString::null)
       {m_pQPrinter->setNumCopies(val1.toInt());
       }

    //.................... ajuster fichier de sortie .........................................
    if (CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName, "OutpuFileName", &val1)==QString::null)  // zero = pas d'erreur
       {m_pQPrinter->setOutputFileName(val1);
       }
    //.................... ajuster sortie fichier ou papier.........................................
    //                     à executer toujours apres: setOutputFileName();  car setOutputFileName()
    //                     replace sur TRUE !!!
    if (CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName, "IsOutToFile", &val1)==QString::null)  // zero = pas d'erreur
       { m_pQPrinter->setOutputToFile(val1.toInt());
       }

    //...................... titre de la boite de dialogue ..........................................
    setCaption(QString(tr("Imprimer  Rx :")) + QString::number(m_pCViewPageWidget->GetResolutionX()) +
               QString(tr("  Ry :")) + QString::number(m_pCViewPageWidget->GetResolutionY())
              );
    connect( slider_Marg_Top,      SIGNAL( sliderMoved(int) ), this, SLOT( slider_Marg_Top_sliderMoved(int) ) );
    connect( slider_Head_H,        SIGNAL( sliderMoved(int) ), this, SLOT( slider_Head_H_sliderMoved(int) ) );
    connect( slider_Marg_Left,     SIGNAL( sliderMoved(int) ), this, SLOT( slider_Marg_Left_sliderMoved(int) ) );
    connect( slider_Marg_Right,    SIGNAL( sliderMoved(int) ), this, SLOT( slider_Marg_Right_sliderMoved(int) ) );
    connect( slider_Foot_H,        SIGNAL( sliderMoved(int) ), this, SLOT( slider_Foot_H_sliderMoved(int) ) );
    connect( slider_Marg_Bottom,   SIGNAL( sliderMoved(int) ), this, SLOT( slider_Marg_Bottom_sliderMoved(int) ) );
    connect( pushButtonPrint,      SIGNAL( clicked() ),        this, SLOT( pushButtonPrint_clicked() ) );
    connect( pushButtonQuitter,    SIGNAL( clicked() ),        this, SLOT( pushButtonQuitter_clicked() ) );
    connect( pushButtonConfigurer, SIGNAL( clicked() ),        this, SLOT( pushButtonConfigurer_clicked() ) );
    connect( pushButtonPrevious,   SIGNAL( clicked() ),        this, SLOT( pushButtonPrevious_clicked() ) );
    connect( pushButtonNext,       SIGNAL( clicked() ),        this, SLOT( pushButtonNext_clicked() ) );
    connect( pushButtonSave,       SIGNAL( clicked() ),        this, SLOT( pushButtonSave_clicked() ) );
}

//-------------------------------------- C_Dlg_Print -------------------------------------------
C_Dlg_Print::~C_Dlg_Print()
{   if (m_pQPrinter)     delete  m_pQPrinter ;     m_pQPrinter        = 0;
    if (m_pHeadRichText) delete  m_pHeadRichText;  m_pHeadRichText    = 0;
    if (m_pBodyRichText) delete  m_pBodyRichText;  m_pBodyRichText    = 0;
    if (m_pFootRichText) delete  m_pFootRichText;  m_pFootRichText    = 0;
}

/*$SPECIALIZATION$*/
//-------------------------------------- pushButtonNext_clicked -------------------------------------------
void C_Dlg_Print::pushButtonNext_clicked()
{   m_pCViewPageWidget->SetCurrentPage(m_pCViewPageWidget->GetCurrentPage()+1);
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- pushButtonPrevious_clicked -------------------------------------------
void C_Dlg_Print::pushButtonPrevious_clicked()
{   m_pCViewPageWidget->SetCurrentPage(m_pCViewPageWidget->GetCurrentPage()-1);
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- pushButtonConfigurer_clicked -------------------------------------------
void C_Dlg_Print::pushButtonConfigurer_clicked()
{if ( m_pQPrinter->setup( this ) )
    {
    }
}

//-------------------------------------- pushButtonQuitter_clicked -------------------------------------------
void C_Dlg_Print::pushButtonQuitter_clicked()
{reject();
}

//-------------------------------------- pushButtonPrint_clicked -------------------------------------------
void C_Dlg_Print::pushButtonPrint_clicked()
{   float      zoom = lineEditZoom->text().toFloat()/100;
    int    page_deb = lineEdit_PageDeb->text().toInt();
    int    page_fin = lineEdit_PageFin->text().toInt();
    //................ recuperer resolution affichée ............................
    int        resoX = lineEditResoX->text().toInt();
    int        resoY = lineEditResoY->text().toInt();

#ifdef Q_WS_X11
        int screen = QX11Info().screen();
        QPaintDevice::x11SetAppDpiX (resoX-1,screen);
        QPaintDevice::x11SetAppDpiY (resoY-1,screen);
#endif
    m_pQPrinter->setResolution(resoY);
    //.............. borner page_fin au max possible ........................
    int nb_page = m_pCViewPageWidget->GetNbPage();
    if (page_fin>nb_page)
       {page_fin = nb_page;
        lineEdit_PageFin->setText(QString::number(page_fin));
       }

    //.............. borner page_deb au max et min possible ..................
    //
    QPainter* p = 0;
    if (page_deb<1)         page_deb = 1;
    if (page_deb>page_fin)  page_deb = page_fin;           //
    int page = page_deb;
    while(page <= page_fin )
    {   p = new QPainter( m_pQPrinter ); // logicalDpiX() and logicalDpiY()).
        m_pCViewPageWidget->PrintDoc(p, page, zoom ,zoom , 0, resoX, resoY);
        delete p;
        ++page;
        if (page > page_fin) break;
        m_pQPrinter->newPage();
    }
    //delete pQPrinter;
}

//-------------------------------------- pushButtonOk_clicked -------------------------------------------
void C_Dlg_Print::pushButtonOk_clicked()
{accept();
}

//-------------------------------------- initRichFootText -------------------------------------------
void C_Dlg_Print::initRichFootText( Q3SimpleRichText * pRichText )
{   m_pFootRichText        = pRichText;
    m_pCViewPageWidget->SetFoot(pRichText);
    if (pRichText) //reajuster hauteur pied de page à la taille de son contenu
    {   m_pCViewPageWidget->SetFoot_H(m_pCViewPageWidget->GetRichFoot_H());
        lineEdit_Foot_H->setText(QString::number(m_pCViewPageWidget->GetFoot_H()*10));
        slider_Foot_H->setValue ( (int)(m_pCViewPageWidget->GetFoot_H()*100/m_PageH ) );
        UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
    }
}

//-------------------------------------- initRichHeadText -------------------------------------------
void C_Dlg_Print::initRichHeadText( Q3SimpleRichText * pRichText )
{   m_pHeadRichText        = pRichText;
    m_pCViewPageWidget->SetHead(pRichText);
    if (pRichText) //reajuster hauteur header à la taille de son contenu
    {m_pCViewPageWidget->SetHead_H(m_pCViewPageWidget->GetRichHead_H());
        lineEdit_Head_H->setText(QString::number(m_pCViewPageWidget->GetHead_H()*10));
        slider_Head_H->setValue ( (int)(m_pCViewPageWidget->GetHead_H()*100/m_PageH ) );
    }
}

//-------------------------------------- initRichBodyText -------------------------------------------
void C_Dlg_Print::initRichBodyText( Q3SimpleRichText * pRichText )
{   m_pBodyRichText        = pRichText;
    m_pCViewPageWidget->SetBody(pRichText);
}

//-------------------------------------- AdjustHeightFootAndHead -------------------------------------------
// soit il existe un parametre fixé et sauvegardé dans la base de données et alors on utilise ce parametre
// soit il n'existe pas et alors on prend la hauteur des documents entete et pieds de page

void C_Dlg_Print::AdjustHeightFootAndHead()
{QString head_H  = "";
 QString foot_H  = "";
 CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName , "Head_H",    &head_H);
 CGestIni::Param_ReadParam(Global_ParamData, m_PrintSectionName , "Foot_H",    &foot_H);

    if (m_pHeadRichText) //reajuster hauteur header à la taille de son contenu
    {   if (head_H != "")  m_pCViewPageWidget->SetHead_H(head_H.toFloat() / 10);
        else               m_pCViewPageWidget->SetHead_H(m_pCViewPageWidget->GetRichHead_H());
        lineEdit_Head_H->setText(QString::number(m_pCViewPageWidget->GetHead_H()*10));
        slider_Head_H->setValue ( (int)(m_pCViewPageWidget->GetHead_H()*100/m_PageH ) );
    }
    if (m_pFootRichText) //reajuster hauteur pied de page à la taille de son contenu
    {   if (foot_H != "")  m_pCViewPageWidget->SetFoot_H(foot_H.toFloat() / 10);
        else               m_pCViewPageWidget->SetFoot_H(m_pCViewPageWidget->GetRichFoot_H());
        lineEdit_Foot_H->setText(QString::number(m_pCViewPageWidget->GetFoot_H()*10));
        slider_Foot_H->setValue ( (int)(m_pCViewPageWidget->GetFoot_H()*100/m_PageH ) );
    }
}

//-------------------------------------- SetDialogOnHeadAndFoot -------------------------------------------
// mode = 1  on reajuste les hauteurs des header et Foot sur leur contenu
//      = 0  on garde les valeurs deja presentes
void C_Dlg_Print::SetDialogOnHeadAndFoot( int mode )
{   QFont font( "Arial Narrow", 10 );
    if (m_pHeadRichText) delete m_pHeadRichText; m_pHeadRichText = 0;
    if (m_pBodyRichText) delete m_pBodyRichText; m_pBodyRichText = 0;
    if (m_pFootRichText) delete m_pFootRichText; m_pFootRichText = 0;

    //......................................... creer les QSimpleRichText ......................................
    //                                          d'affichage et d'impression

    m_pBodyRichText = new Q3SimpleRichText( *m_pBodyText, font, m_pBodyTextEdit->context(), m_pBodyTextEdit->styleSheet(),
                                            m_pBodyTextEdit->mimeSourceFactory(), -1 );
    m_pHeadRichText = new Q3SimpleRichText( *m_pHeadText, font, m_pBodyTextEdit->context(), m_pBodyTextEdit->styleSheet(),
                                            m_pBodyTextEdit->mimeSourceFactory(), -1 );
    m_pFootRichText = new Q3SimpleRichText( *m_pFootText, font, m_pBodyTextEdit->context(), m_pBodyTextEdit->styleSheet(),
                                            m_pBodyTextEdit->mimeSourceFactory(), -1 );

    m_pCViewPageWidget->SetHead(m_pHeadRichText);
    m_pCViewPageWidget->SetBody(m_pBodyRichText);
    m_pCViewPageWidget->SetFoot(m_pFootRichText);
    AdjustHeightFootAndHead();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- pushButtonSave_clicked -------------------------------------------
void C_Dlg_Print::pushButtonSave_clicked()
{
 //................... modifier et updater ces parametres selon les reglages ...........................................
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "Resolution",       lineEditResoX->text(), lineEditResoY->text());
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "MargTop",          lineEdit_Marg_Top->text());
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "Marg_Left",        lineEdit_Marg_Left->text());
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "Marg_Right",       lineEdit_Marg_Right->text());
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "Marg_Bottom",      lineEdit_Marg_Bottom->text());
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "Zoom",             lineEditZoom->text());
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "IsOutToFile",      QString::number((unsigned int) m_pQPrinter->outputToFile ()));
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "OutpuFileName",    m_pQPrinter->outputFileName ());

 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "Head_H",           lineEdit_Head_H->text());
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "Foot_H",           lineEdit_Foot_H->text());
 CGestIni::Param_WriteParam(&Global_ParamData, m_PrintSectionName, "NumCopies",        QString::number(m_pQPrinter->numCopies ()));
 CGestIni::Param_UpdateToDisk(Global_PathIni, Global_ParamData);

}

//-------------------------------------- UpdateTextePageInfo -------------------------------------------
void C_Dlg_Print::UpdateTextePageInfo( int nb_page )
{   QString qtext;
    qtext.sprintf(tr("Page %i sur %i"),m_pCViewPageWidget->GetCurrentPage(), nb_page);
    textLabel_NbPage->setText(qtext);
    lineEdit_PageFin->setText(QString::number(nb_page));
}

//-------------------------------------- slider_Foot_H_sliderMoved -------------------------------------------
void C_Dlg_Print::slider_Foot_H_sliderMoved( int value )
{   float fval = (m_PageH * value )/ 100.0;
    m_pCViewPageWidget->SetFoot_H(fval);
    lineEdit_Foot_H->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Marg_Bottom_sliderMoved -------------------------------------------
void C_Dlg_Print::slider_Marg_Bottom_sliderMoved( int value )
{   float fval = (m_PageH * value )/ 100.0;
    m_pCViewPageWidget->SetBottomMarg(fval);
    lineEdit_Marg_Bottom->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Marg_Right_sliderMoved -------------------------------------------
void C_Dlg_Print::slider_Marg_Right_sliderMoved( int value )
{   float fval = (m_PageW * value )/ 100.0;
    m_pCViewPageWidget->SetRightMarg(fval);
    lineEdit_Marg_Right->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Marg_Left_sliderMoved -------------------------------------------
void C_Dlg_Print::slider_Marg_Left_sliderMoved( int value )
{   float fval = (m_PageW * value )/ 100.0;
    m_pCViewPageWidget->SetLeftMarg(fval);
    lineEdit_Marg_Left->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Head_H_sliderMoved -------------------------------------------
void C_Dlg_Print::slider_Head_H_sliderMoved( int value )
{  float fval = (m_PageH * value )/ 100.0;
    m_pCViewPageWidget->SetHead_H(fval);
    lineEdit_Head_H->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Marg_Top_sliderMoved -------------------------------------------
void C_Dlg_Print::slider_Marg_Top_sliderMoved( int value )
{   float fval = (m_PageH * value )/ 100.0;
    m_pCViewPageWidget->SetTopMarg(fval);
    lineEdit_Marg_Top->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}




