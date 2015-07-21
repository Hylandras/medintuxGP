/****************************************************************************DlgPrint
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "Global.h"
#include "CApp.h"
//-------------------------------------- init -------------------------------------------
void DlgPrint::init()
{   pushButtonSave->setPixmap( Theme::getIconSave().pixmap(QIconSet::Automatic,TRUE,QIconSet::On) );
    pushButtonNext->setPixmap(QPixmap( Theme::getIconNext().pixmap(QIconSet::Automatic,TRUE,QIconSet::On)));
    pushButtonPrevious->setPixmap(QPixmap( Theme::getIconPrevious().pixmap(QIconSet::Automatic,TRUE,QIconSet::On)));
    setIcon( Theme::getIconPrint().pixmap(QIconSet::Automatic,TRUE,QIconSet::On));
    m_pCViewPageWidget = new CViewPageWidget(this);
    //CMoteurBase::GotoDebug();
    if (m_pCViewPageWidget )
    {m_pCViewPageWidget->setGeometry (QRect (10,10, m_pCViewPageWidget->GetWidgetWidth(), m_pCViewPageWidget->GetWidgetHeight()));
    }
    m_pHeadRichText       = 0;
    m_pBodyRichText       = 0;
    m_pFootRichText       = 0;
    G_pCApp->m_VAR_MAP.insert("$SCRIPT_STATUS","$PRINT_SCRIPT_BEFORE");
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
    checkBox_Duplicata->setHidden(FALSE);
    checkBox_PrintDuplicata->setHidden(FALSE);
}

//-------------------------------------- destroy -------------------------------------------
void DlgPrint::destroy()
{   if (m_pQPrinter)     delete  m_pQPrinter ;     m_pQPrinter        = 0;
    if (m_pHeadRichText) delete  m_pHeadRichText;  m_pHeadRichText    = 0;
    if (m_pBodyRichText) delete  m_pBodyRichText;  m_pBodyRichText    = 0;
    if (m_pFootRichText) delete  m_pFootRichText;  m_pFootRichText    = 0;
}

//-------------------------------------- slider_Marg_Top_sliderMoved -------------------------------------------
void DlgPrint::slider_Marg_Top_sliderMoved( int value)
{
    float fval = (m_PageH * value )/ 100.0;
    m_pCViewPageWidget->SetTopMarg(fval);
    lineEdit_Marg_Top->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Head_H_sliderMoved -------------------------------------------
void DlgPrint::slider_Head_H_sliderMoved( int value)
{float fval = (m_PageH * value )/ 100.0;
    m_pCViewPageWidget->SetHead_H(fval);
    lineEdit_Head_H->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Marg_Left_sliderMoved -------------------------------------------
void DlgPrint::slider_Marg_Left_sliderMoved( int value)
{float fval = (m_PageW * value )/ 100.0;
    m_pCViewPageWidget->SetLeftMarg(fval);
    lineEdit_Marg_Left->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Marg_Right_sliderMoved -------------------------------------------
void DlgPrint::slider_Marg_Right_sliderMoved( int value)
{float fval = (m_PageW * value )/ 100.0;
    m_pCViewPageWidget->SetRightMarg(fval);
    lineEdit_Marg_Right->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Foot_H_sliderMoved -------------------------------------------
void DlgPrint::slider_Marg_Bottom_sliderMoved( int value)
{float fval = (m_PageH * value )/ 100.0;
    m_pCViewPageWidget->SetBottomMarg(fval);
    lineEdit_Marg_Bottom->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Foot_H_sliderMoved -------------------------------------------
void DlgPrint::slider_Foot_H_sliderMoved( int value)
{float fval = (m_PageH * value )/ 100.0;
    m_pCViewPageWidget->SetFoot_H(fval);
    lineEdit_Foot_H->setText(QString::number( (fval *10.0) ));
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- slider_Foot_H_sliderMoved -------------------------------------------
void DlgPrint::UpdateTextePageInfo( int nb_page)
{QString qtext;
    qtext.sprintf(tr("Page:%i sur %i"),m_pCViewPageWidget->GetCurrentPage(), nb_page);
    textLabel_NbPage->setText(qtext);
    lineEdit_PageFin->setText(QString::number(nb_page));
}

/*
   int              GetLeftMarg();
   int              GetTopMarg();
   int              GetRightMarg();
   int              GetBottomMarg();
   int              GetHead_H();
   int              GetFoot_H();
   int              SetLeftMarg(int value=0);
   int              SetTopMarg(int value=0);
   int              SetRightMarg(int value=0);
   int              SetBottomMarg(int value=0);
   int              SetHead_H(int value=0);
   int              SetFoot_H(int value=0);
*/
/*
CViewPageWidget::CViewPageWidget( QWidget*         parent ,         / * = 0 * /
                                  const char*      name ,           / * = 0 * /
                                  WFlags           f,               / * = 0 * /
                                  QSimpleRichText* pRichTextBody,   / * = 0 * /
                                  CPageDef*        pCPageDef,       / * = 0 * /
                                  QSimpleRichText* pRichTextHead,   / * = 0 * /
                                  QSimpleRichText* pRichTextFoot,   / * = 0 * /
                                 )
*/
//-------------------------------------- initDialog ----------------------------------------------------------
// ENTRE: QTextEdit*   pBodyTextEdit           : texte à imprimer
//        QString     &IdentPrimKey            : clef primaire de l'identité
//        CMoteurBase* pCMoteurBase            : l'eternel
//        RUBREC_LIST* pRubList                : liste des rubriques en cours
//        int          id_Obs                  : iterator sur l'observation en cours de cette liste
//        int          id_Prs                  : iterator sur la prescription à imprimer (si -1 alors il faut imprimer une observ)
//        const char*  observText              : pointeur sur le texte de l'observation

int DlgPrint::initDialog( QTextEdit*   pBodyTextEdit,
                          QString     &IdentPrimKey,                          // il faut les renseignements sur le patient
                          CMoteurBase* pCMoteurBase,
                          RUBREC_LIST* pRubList,      // liste des rubriques en cours
                          int          id_Obs,        // iterator sur l'observation en cours
                          int          id_Prs,        // iterator sur le document à imprimer (prescription ou observation)
                          const char*  observText,
                          const char*  userActuelPk,
                          const char*  rubName
                          )
{   //............................. variables permettant la reinitialisation du dialogue ................
    //                              sur de nouvelles entêtes et pied de page
    m_ID_Obs               = id_Obs;
    m_ID_Prs               = id_Prs;
    QString val1           = "";
    QString val2           = "";
    m_pRubList             = pRubList;
    m_IdentPrimKey         = IdentPrimKey;
    m_pBodyTextEdit        = pBodyTextEdit;
    m_pCMoteurBase         = pCMoteurBase;
    m_ObsText              = observText;
    m_UserActuelPk         = userActuelPk;
    m_HeadText             = "";
    m_FootText             = "";
    m_BodyText             = m_pBodyTextEdit->text();
    m_currentDocDisplayMap = G_pCApp->m_pDrTux->GetMapActiveID_Doc();

    //....................... determination de l'utilisateur .................................................
    RUBREC_LIST::iterator it = m_pRubList->at(m_ID_Prs);      // iterateur de la liste des documents, sur le document a imprimer
    m_SignUser               = (*it).m_SignUser;              // utilisateur

    QString typ_deb          = (*it).m_Type;
    QString typ_end          = QString::null;

    m_pCMoteurBase->GotoDebug();

    int typRub               = (*it).m_Type.left(4).toInt();
    //>>>>>>>................ ancienne numerotation des rubriques dans les documents types ..........................
    //                         ( c'est le type de la rubrique et document qui filtre )
    if (typRub  < 2008)
       { //................................... si prescription elargir la recherche .....................
         //                                    à tous les documents maitres de prescription
         if (typRub  == 2002)
            {   typ_deb = QString::number(TYP_PRESCRIPTION);
                typ_end = QString::number(TYP_PRESCRIPTION+999);
            }
      }
    else
    //>>>>>>>............ forme particuliere des nouvelles rubriques a venir  ..................................
    //                    le libelle du document d'entete et de pied de page doit etre
    //                    de la forme : [NOM DE LA RUBRIQUE] LIBELLE DE LA RUBRIQUE (avec le # ou * et eventuels prefixages)
      {
       typ_deb = "RUBNAME";   //  QString         type1 /* = QString::null */,
       typ_end = rubName;     //  QString         type2 /* = QString::null */
      }
    //............................... remplir le comboBox avec les documents types .......................................
    m_pCMoteurBase->InitComboWithUserDocList(    comboBox_TypeDoc,
                                                 m_SignUser,
                                                 rubName,
                                                 typ_deb,   //  QString         type1 /* = QString::null */,
                                                 typ_end,   //  QString         type2 /* = QString::null */
                                                 1);        // effacer la liste
    //............................. positionner le combo sur bon élément ..................................................
    //                              le plus probable
    int nb = comboBox_TypeDoc->count();
    int i=0;
    CPrtQListBoxItem *pCPrtQListBoxItem = 0;
    QString itemComboText("");
    QString printType = m_pCMoteurBase->TypeDoc_To_PrintTypeDoc((*it).m_Type);  // à plusieurs types de documents correspondent q'un type de modèle d'impression
    QString libelle   = (*it).m_Libelle.lower();

    //................ # PREFIX  on cherche le type de modèle de masque d'impression marqué par # dont le nom...........................
    //                           est contenu dans le titre du documment à imprimer
    for (i=0; i<nb; i++)
        {   itemComboText             = comboBox_TypeDoc->text(i);
            pCPrtQListBoxItem         = (CPrtQListBoxItem*) comboBox_TypeDoc->listBox()->item(i);
            if  (itemComboText.at(0) == '#' && libelle.contains(itemComboText.mid(2).lower())!= 0 )             {comboBox_TypeDoc_activated (i); break;}
        }
    //................[NOM RUBRIQUE @ SOUS TYPE] chercher le type de modele correspondant au sous type du document à imprimer .....................
    if (i==nb)
       {for (i=0; i<nb; i++)
            {   itemComboText                        = comboBox_TypeDoc->text(i);
                pCPrtQListBoxItem                    = (CPrtQListBoxItem*) comboBox_TypeDoc->listBox()->item(i);
                if  (pCPrtQListBoxItem->GetSubType()== (*it).m_SubType)                                         {comboBox_TypeDoc_activated (i); break;}
            }
       }
    //................ * modele correspondant au type du document ...........................
    if (i==nb)
       {//............................. Si echec on cherche le premier type de doccument ....................................
        //                              pouvant correspondre
        //............................. on cherche correspondance exacte entre type de doccument ...........................
        //                              et celui marqué par defaut par une '*' du meme user et meme type de document
        for (i=0; i<nb; i++)
            {   itemComboText             = comboBox_TypeDoc->text(i);
                pCPrtQListBoxItem         = (CPrtQListBoxItem*) comboBox_TypeDoc->listBox()->item(i);
                if  (itemComboText.at(0) == '#' && libelle.contains(itemComboText.lower())!= 0 )               {comboBox_TypeDoc_activated (i); break;}
                if  (printType           == pCPrtQListBoxItem->GetType() &&
                     itemComboText.at(0) == '*' )                                                              {comboBox_TypeDoc_activated (i); break;}

            }
       }
    //................ * modele le premier avec *  ...........................
    if (i==nb)
       {//............................. Si echec on cherche le premier type de doccument ....................................
        //                              pouvant correspondre
        //............................. on cherche correspondance exacte entre type de doccument ...........................
        //                              et celui marqué par defaut par une '*' du meme user et meme type de document
        for (i=0; i<nb; i++)
            {   itemComboText             = comboBox_TypeDoc->text(i);
                pCPrtQListBoxItem         = (CPrtQListBoxItem*) comboBox_TypeDoc->listBox()->item(i);
                if  (itemComboText.at(0) == '#' && libelle.contains(itemComboText.lower())!= 0 )               {comboBox_TypeDoc_activated (i); break;}
                if  (itemComboText.at(0) == '*' )                                                              {comboBox_TypeDoc_activated (i); break;}

            }
       }
    if (nb == 0)
       {
       }
    else if (i==nb)
       {comboBox_TypeDoc->setCurrentItem (0);
       }
    else
       {comboBox_TypeDoc->setCurrentItem (i);
       }
    //............................ on a trouve qq chose recuperer les documents .............................................
    //if (pCPrtQListBoxItem && i != nb ) m_HeadText = Get_User_Head_Foot(m_SignUser, &m_FootText, comboBox_TypeDoc->text(i), pCPrtQListBoxItem->GetType());
    //......................... recuperer parametres correspondant à l'utilisateur ..........................................
    SetDialogOnPrinterSpec();
    pushButtonPrint->setFocus();
    m_pCViewPageWidget->SetHead(m_pHeadRichText);
    m_pCViewPageWidget->SetBody(m_pBodyRichText);
    m_pCViewPageWidget->SetFoot(m_pFootRichText);
    if (m_HeadText.find             ("$PRINT_STOP") != -1 ||
        m_BodyText.find             ("$PRINT_STOP") != -1 ||
        m_FootText.find             ("$PRINT_STOP") != -1
       )
       {return 0;
       }
    return 1;
}

//-------------------------------------- executeTransfertMacro ----------------------------------------------------------
void DlgPrint::executeTransfertMacro()
{   m_BodyText      = m_pBodyTextEdit->text();
    //......................................... faire les transferts de blocs ........................................
    //                                          haeader vers body

    int posDebBloc = m_HeadText.find("{{TO_INSERT_IN_BODY");
    int posEndBloc = m_HeadText.find("{{END_TO_INSERT_IN_BODY}}");
    if (posDebBloc != -1 && posEndBloc != -1)
       {
        QString toInsertAfter = "";
        int               deb = posDebBloc+19;
        int               end = m_HeadText.find("}}", deb);

        if (end != -1)
           {toInsertAfter     = m_HeadText.mid(deb,end-deb);                               // recuperer la chaine du corps apres laquelle il faut inserer
            CHtmlTools::HtmlToAscii(toInsertAfter, CHtmlTools::StrictASCII);               // un peu de menage
            toInsertAfter     = toInsertAfter.remove("=").stripWhiteSpace();               // encore du menage
            int posToInsert   = m_BodyText.find(toInsertAfter);                            // on sait apres
            deb               = end + 2;                                                   // pointer en fin de }} soit debut de la chaine a inserer
            QString toInsert  = m_HeadText.mid(deb,posEndBloc-deb);                        // recuperer la chaine a inserer
            posEndBloc       += 25;                                                        // pointer en fin de tag (pour tout virer)
            m_HeadText        = m_HeadText.remove(posDebBloc, posEndBloc - posDebBloc);    // virer le bloc du header (plus besoin car recupere)
            if (posToInsert  == -1) {posToInsert = 0;toInsertAfter="";}
            //if (posToInsert  != -1)
               {posToInsert  += toInsertAfter.length(); // on insere apres
                FusionneDocument(&toInsert, m_UserActuelPk);
                m_BodyText    = m_BodyText.insert (posToInsert, toInsert );
                //m_BodyText    = m_BodyText.remove(posToInsert-toInsertAfter.length(),toInsertAfter.length());
                //CGestIni::Param_UpdateToDisk("C:\\body.html",m_BodyText);
               }
           }
       }
   /*
    int posDebBloc = m_HeadText.find("{{TO_INSERT_IN_BODY");
    int posEndBloc = m_HeadText.find("{{END_TO_INSERT_IN_BODY}}");
    if (posDebBloc != -1 && posEndBloc != -1)
       {
        QString toInsertAfter = "";
        int               deb = posDebBloc+19;
        int               end = m_HeadText.find("}}", deb);

        if (end != -1)
           {toInsertAfter     = m_HeadText.mid(deb,end-deb);                               // recuperer la chaine du corps apres laquelle il faut inserer
            CHtmlTools::HtmlToAscii(toInsertAfter, CHtmlTools::StrictASCII);               // un peu de menage
            toInsertAfter     = toInsertAfter.remove("=").stripWhiteSpace();               // encore du menage
            deb               = end + 2;                                                   // pointer en fin de }} soit debut de la chaine a inserer
            QString toInsert  = m_HeadText.mid(deb,posEndBloc-deb);                        // recuperer la chaine a inserer
            int endToInsert   = 0;
            int posToInsert   = 0;
            FusionneDocument(&toInsert, m_UserActuelPk);
            CHtmlTools::findReplace( m_BodyText, toInsertAfter, toInsert, TRUE, &posToInsert, &endToInsert,1);

            posEndBloc       += 25;                                                        // pointer en fin de tag (pour tout virer)
            m_HeadText        = m_HeadText.remove(posDebBloc, posEndBloc - posDebBloc);    // virer le bloc du header (plus besoin car recupere)
            CGestIni::Param_UpdateToDisk("C:\\body.html",m_BodyText);
           }
       }
*/
}

//-------------------------------------- SetCaption ----------------------------------------------------------
void DlgPrint::SetCaption()
{setCaption(G_pCApp->m_User +"::"+m_SignUser + tr(" imprime sur ") + m_pQPrinter->printerName() +
            tr(" en ")  + QString::number(m_pCViewPageWidget->GetResolutionY()) +
            tr(" par ") + QString::number(m_pCViewPageWidget->GetResolutionY()) + tr(" points"));
}

//-------------------------------------- pushButtonSave_clicked ----------------------------------------------------------
// Sauver les reglages, le PB est que l'utilisateur du document en train d'etre imprime n'est pas forcement le meme que
//                      celui de DrTux.
void DlgPrint::pushButtonSave_clicked()
{   UPDATE_USER_PARAM( &USER_PARAM, m_SignUser);    // sauvegarder les parametres de l'utilisateur DrTUX au cas ou l'user dont
                                                   // il est question ici avec m_User est le meme.(avant de modifier)
    //................... recuperer les parametres deja existant pour cet utilisateur ....................................
    QString nameDoc        = GetNameDocFromCombo();
    QString param          = m_pCMoteurBase->ReadDrTuxUserParametres(m_SignUser );   // lecture directe dans la base de donnee pour modifier ensuite
    if (nameDoc.length()==0) return;
    QString docSectionName = "Impression";                  // réglage par défaut de l'imprimante
    if (nameDoc.length() ) docSectionName = nameDoc;        // réglage spécifique au document
    //................... modifier et updater ces parametres selon les reglages ...........................................
    CGestIni::Param_WriteParam(&param, docSectionName, "Resolution",             lineEditResoX->text(), lineEditResoY->text());
    CGestIni::Param_WriteParam(&param, docSectionName, "MargTop",                lineEdit_Marg_Top->text());
    //CGestIni::Param_WriteParam( docSectionName, "Head_H",           lineEdit_Head_H->text());
    CGestIni::Param_WriteParam(&param, docSectionName, "Marg_Left",              lineEdit_Marg_Left->text());
    CGestIni::Param_WriteParam(&param, docSectionName, "Marg_Right",             lineEdit_Marg_Right->text());
    //CGestIni::Param_WriteParam( docSectionName, "Foot_H",           lineEdit_Foot_H->text());
    CGestIni::Param_WriteParam(&param, docSectionName, "Marg_Bottom",            lineEdit_Marg_Bottom->text());
    CGestIni::Param_WriteParam(&param, docSectionName, "Zoom",                   lineEditZoom->text());
    CGestIni::Param_WriteParam(&param, docSectionName, "IsOutToFile",            QString::number(m_pQPrinter->outputToFile ()));
    CGestIni::Param_WriteParam(&param, docSectionName, "OutpuFileName",          m_pQPrinter->outputFileName ());
    CGestIni::Param_WriteParam(&param, docSectionName, "printerName"  ,          m_pQPrinter->printerName ());
    CGestIni::Param_WriteParam(&param, docSectionName, "printerSelectionOption", m_pQPrinter->printerSelectionOption());
    CGestIni::Param_WriteParam(&param, docSectionName, "colorMode",              QString::number(m_pQPrinter->colorMode()));
    CGestIni::Param_WriteParam(&param, docSectionName, "IsDuplicataToPrint",     checkBox_PrintDuplicata->isChecked()?"o":"n");
    CGestIni::Param_WriteParam(&param, nameDoc, "Head_H",        lineEdit_Head_H->text());
    CGestIni::Param_WriteParam(&param, nameDoc, "Foot_H",        lineEdit_Foot_H->text());
    CGestIni::Param_WriteParam(&param, nameDoc, "NumCopies",     QString::number(m_pQPrinter->numCopies ()));

        //............... enregistrer les données parametres dans la base de données de cet user ............................
    m_pCMoteurBase->Param_SavParam( &param, m_SignUser);
}

//-------------------------------------- SetDialogOnPrinterSpec ----------------------------------------------------------
void DlgPrint::SetDialogOnPrinterSpec()
{   QString val1, val2;
    //......................... recuperer parametres correspondant à l'utilisateur ..........................................
    QString nameDoc        = GetNameDocFromCombo();
    QString param          = m_pCMoteurBase->ReadDrTuxUserParametres(m_SignUser);
    QString docSectionName = "Impression";                  // réglage par défaut de l'imprimante
    if (nameDoc.length() )                                  // réglage spécifique au document
       {if (CGestIni::Param_ReadParam(param, nameDoc, "Zoom", &val1)==0)  // si param specifiques existent (zero = pas d'erreur)
           {docSectionName = nameDoc;
           }
#ifdef Q_WS_X11
        //.................... ajuster resolution d'impression .........................................
        if (CGestIni::Param_ReadParam(param, docSectionName, "Resolution", &val1, &val2)==0)  // zero = pas d'erreur
           {if (val2=="") val2=val1;
            lineEditResoX->setText(val1);
            lineEditResoY->setText(val2);
            m_pQPrinter->setResolution(atoi(val2));
            QPaintDevice::x11SetAppDpiX(atoi(val1));
            QPaintDevice::x11SetAppDpiY(atoi(val2));
           }
#endif
       }

    SetDialogOnHeadAndFoot(1);

    if (nameDoc.length() )                                  // réglage spécifique au document
       {//.................... ajuster resolution d'impression .........................................
        if (CGestIni::Param_ReadParam(param, docSectionName, "MargTop", &val1)==0)      m_pCViewPageWidget->SetTopMarg(atof(val1) / 10);
        //if (CGestIni::Param_ReadParam(param, "Impression", "Head_H", &val1)==0)       lineEdit_Head_H->setText(val1);
        if (CGestIni::Param_ReadParam(param, docSectionName, "Marg_Left", &val1)==0)    m_pCViewPageWidget->SetLeftMarg(atof(val1) / 10);
        if (CGestIni::Param_ReadParam(param, docSectionName, "Marg_Right", &val1)==0)   m_pCViewPageWidget->SetRightMarg(atof(val1) / 10);
        //if (CGestIni::Param_ReadParam(param, "Impression", "Foot_H", &val1)==0)       lineEdit_Foot_H->setText(val1);
        if (CGestIni::Param_ReadParam(param, docSectionName,"Impression" "Marg_Bottom", &val1)==0)  m_pCViewPageWidget->SetBottomMarg(atof(val1) / 10);
        if (CGestIni::Param_ReadParam(param, docSectionName, "Zoom", &val1)==0)         m_pCViewPageWidget->SetPrintScale(atof(val1)/100 );
       }
    slider_Marg_Top->setValue ( (int)(m_pCViewPageWidget->GetTopMarg()*100/m_PageH ) );
    //slider_Head_H->setValue ( (int)(m_pCViewPageWidget->GetHead_H()*100/m_PageH ) );
    slider_Marg_Left->setValue ( (int)(m_pCViewPageWidget->GetLeftMarg()*100/m_PageW ) );
    slider_Marg_Right->setValue ( (int)(m_pCViewPageWidget->GetRightMarg()*100/m_PageW ) );
    //slider_Foot_H->setValue ( (int)(m_pCViewPageWidget->GetFoot_H()*100/m_PageH ) );
    slider_Marg_Bottom->setValue ( (int)(m_pCViewPageWidget->GetBottomMarg()*100/m_PageH ) );
    lineEditZoom->setText(QString::number(m_pCViewPageWidget->GetPrintScale()*100));


    lineEdit_Marg_Top->setText(QString::number(m_pCViewPageWidget->GetTopMarg()*10));
    lineEdit_Head_H->setText(QString::number(m_pCViewPageWidget->GetHead_H()*10));
    lineEdit_Marg_Left->setText(QString::number(m_pCViewPageWidget->GetLeftMarg()*10));
    lineEdit_Marg_Right->setText(QString::number(m_pCViewPageWidget->GetRightMarg()*10));
    lineEdit_Marg_Bottom->setText(QString::number(m_pCViewPageWidget->GetBottomMarg()*10));
    lineEdit_Foot_H->setText(QString::number(m_pCViewPageWidget->GetFoot_H()*10));


    slider_Marg_Top->setValue ( (int)(m_pCViewPageWidget->GetTopMarg()*100/m_PageH ) );
    slider_Head_H->setValue ( (int)(m_pCViewPageWidget->GetHead_H()*100/m_PageH ) );
    slider_Marg_Left->setValue ( (int)(m_pCViewPageWidget->GetLeftMarg()*100/m_PageW ) );
    slider_Marg_Right->setValue ( (int)(m_pCViewPageWidget->GetRightMarg()*100/m_PageW ) );
    slider_Foot_H->setValue ( (int)(m_pCViewPageWidget->GetFoot_H()*100/m_PageH ) );
    slider_Marg_Bottom->setValue ( (int)(m_pCViewPageWidget->GetBottomMarg()*100/m_PageH ) );
    lineEditZoom->setText(QString::number(m_pCViewPageWidget->GetPrintScale()*100));

    //........................ param lies au document .......................................
    if (nameDoc.length())
       {//............................ positionner le nombre de copies ...........................
        if ( CGestIni::Param_ReadParam(param, nameDoc, "NumCopies",  &val1)==0)
           {m_pQPrinter->setNumCopies(atoi(val1));
           }
        //.................... ajuster fichier de sortie .........................................
        if ( CGestIni::Param_ReadParam(param, docSectionName, "OutpuFileName", &val1)==0)  // zero = pas d'erreur
           {m_pQPrinter->setOutputFileName(val1);
           }
        //.................... ajuster sortie fichier ou papier.........................................
        //                     à executer toujours apres: setOutputFileName();  car setOutputFileName()
        //                     replace sur TRUE !!!
        if ( CGestIni::Param_ReadParam(param, docSectionName, "IsOutToFile", &val1)==0)  // zero = pas d'erreur
           { m_pQPrinter->setOutputToFile(atoi(val1));
           }
        //.................... ajuster imprimante par defaut .........................................
        if ( CGestIni::Param_ReadParam(param, docSectionName, "printerName", &val1)==0)  // zero = pas d'erreur
           {m_pQPrinter->setPrinterName(val1);
           }
        //.................... ajuster options .........................................
        if ( CGestIni::Param_ReadParam(param, docSectionName, "printerSelectionOption", &val1)==0)  // zero = pas d'erreur
           {m_pQPrinter->setPrinterSelectionOption(val1);
           }
        //.................... ajuster options couleur.........................................
        if ( CGestIni::Param_ReadParam(param, docSectionName, "colorMode", &val1)==0)  // zero = pas d'erreur
           {m_pQPrinter->setColorMode((QPrinter::ColorMode)val1.toInt());
           }
      }
    //........................ param surchargeant tous les autres .......................................
    // [Print]
    //      OverAll = HP2420UrgChir
    if (CGestIni::Param_ReadParam( G_pCApp->m_DrTuxParam, "Print", "OverAll", &val1) ==0 ) // zero = pas d'erreur
       { m_pQPrinter->setPrinterName(val1);
       }
    //.................... ajuster options couleur.........................................
    checkBox_PrintDuplicata->setChecked(TRUE);
    if (nameDoc.length() && CGestIni::Param_ReadParam(param, docSectionName, "IsDuplicataToPrint", &val1)==0)  // zero = pas d'erreur
    {if (val1.left(1).lower()=="n") checkBox_PrintDuplicata->setChecked(FALSE);
     else                           checkBox_PrintDuplicata->setChecked(TRUE);
    }

    if (IsExistDuplicata()!=QString::null) {checkBox_Duplicata->show(); checkBox_PrintDuplicata->show();}
    else                                   {checkBox_Duplicata->hide(); checkBox_PrintDuplicata->hide();}

    //...................... titre de la boite de dialogue ..........................................
    SetCaption();
}

//-------------------------------------- pushButtonNext_clicked ------------------------------------------------
void DlgPrint::comboBox_TypeDoc_activated( const QString &)
{  G_pCApp->m_VAR_MAP.insert("$SCRIPT_STATUS","$PRINT_HEADER_CHANGED");
   comboBox_TypeDoc_activated(comboBox_TypeDoc->currentItem());
}

//-------------------------------------- pushButtonNext_clicked ------------------------------------------------
void DlgPrint::comboBox_TypeDoc_activated(int item)
{   QString     foot  = "";
    QString     head  = "";
    if (item==-1) return;
    CPrtQListBoxItem *pCPrtQListBoxItem = (CPrtQListBoxItem*) comboBox_TypeDoc->listBox()->item(item);
    if (pCPrtQListBoxItem->GetType().toInt() == TYP_ORDONNANCE) {checkBox_Duplicata->show(); checkBox_PrintDuplicata->show();}
    else                                                        {checkBox_Duplicata->hide(); checkBox_PrintDuplicata->hide();}

    head = m_pCMoteurBase->GetUserDocument(pCPrtQListBoxItem->GetPk(),
                                           0,        // TYP_ORDONNANCE 2  TYP_CERTIFICAT 3 TYP_OBSERVATION 4
                                           &foot,    // adresse de la chaine où l'on veut recuperer le pied de page
                                           checkBox_Duplicata->isChecked ()         // 0 par defaut (RECTO) 1 si l'on veut le DUPLICATA (que pour TYP_ORDONNANCE)
                                           );
    //if (head !="" )
    {m_HeadText = head;
    }
    //if (foot != "")
    {m_FootText = foot;
    }
    executeTransfertMacro();
    //if (foot != "" || head !="")
    {SetDialogOnHeadAndFoot(1);
     m_pCViewPageWidget->update ();
    }
}

//-------------------------------------- SetDialogOnHeadAndFoot ----------------------------------------------------------
// mode = 1  on reajuste les hauteurs des header et Foot sur leur contenu
//      = 0  on garde les valeurs deja presentes
void DlgPrint::SetDialogOnHeadAndFoot(int /*mode*/)
{   QFont font( "Arial Narrow", 10 );
    if (m_pHeadRichText) delete m_pHeadRichText; m_pHeadRichText = 0;
    if (m_pBodyRichText) delete m_pBodyRichText; m_pBodyRichText = 0;
    if (m_pFootRichText) delete m_pFootRichText; m_pFootRichText = 0;
    //......................................... fusionner les documents ........................................
    //if (m_IsFusionOk==0)
       { FusionneDocument(&m_HeadText, m_UserActuelPk);
         FusionneDocument(&m_FootText, m_UserActuelPk);
       }
    G_pCApp->m_VAR_MAP.insert("$SCRIPT_STATUS","$PRINT_SCRIPT_END");
    //m_IsFusionOk = 1;
    //......................................... creer les QSimpleRichText ......................................
    //                                          d'affichage et d'impression
    m_pBodyTextEdit->mimeSourceFactory()->addFilePath(G_pCApp->m_PathAppli     + "Ressources/RubriquesTextes");
    m_pBodyTextEdit->mimeSourceFactory()->addFilePath(G_pCApp->m_PathGlossaire + "ImagesFolder");

    GetPossibleDefaultFont(m_BodyText, font);
    m_pBodyRichText = new QSimpleRichText( m_BodyText, font, m_pBodyTextEdit->context(), m_pBodyTextEdit->styleSheet(),
                                           m_pBodyTextEdit->mimeSourceFactory(), 0 );

    GetPossibleDefaultFont(m_HeadText, font);
    m_pHeadRichText = new QSimpleRichText( m_HeadText, font, m_pBodyTextEdit->context(), m_pBodyTextEdit->styleSheet(),
                                           m_pBodyTextEdit->mimeSourceFactory(), 0 );

    GetPossibleDefaultFont(m_FootText, font);
    m_pFootRichText = new QSimpleRichText( m_FootText, font, m_pBodyTextEdit->context(), m_pBodyTextEdit->styleSheet(),
                                           m_pBodyTextEdit->mimeSourceFactory(), 0 );

    m_pCViewPageWidget->SetHead(m_pHeadRichText);
    m_pCViewPageWidget->SetBody(m_pBodyRichText);
    m_pCViewPageWidget->SetFoot(m_pFootRichText);
    AdjustHeightFootAndHead();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- GetPossibleDefaultFont ----------------------------------------------------------
void DlgPrint::GetPossibleDefaultFont(const QString &txt, QFont &font)
{QString style                   = CHtmlTools::getTextDefaultAttribut(txt, "style");
 //........ il semblerait que positionner la fonte avant le texte ...............
 //         fait planter dans certaines conditions ???
 CHtmlTools::QT_style_To_QFont(style , font, "Arial","10");
}

//------------------------------ FusionneDocument ---------------------------------------------
void DlgPrint::FusionneDocument(QString  *pDocument, QString userActuelPk)
{
 //............. creer la liste des documents à l'affichage ...........................
 // DOCUMENT_DISPLAY_MAP  currentDocDisplayMap;
 // QString user   = G_pCApp->m_pDrTux->MapActiveID_Doc(currentDocDisplayMap);   // retour = utilisateur le plus probable
 CDevilCrucible *pfusion = new CDevilCrucible(pDocument                      ,   // 0 texte du document à fusionner
                                              m_pCMoteurBase                 ,   // 1 moteur de base de données (faut bien accéder aux fonctions)
                                              G_pCApp->m_pCMedicaBase        ,   // 2 moteur de base de données medicamenteuses(faut bien accéder aux fonctions)
                                              &G_pCApp->m_pDrTux->m_RubList  ,   // 3 liste des documents composant le dossier patient
                                              m_IdentPrimKey                 ,   // 4 il faut les renseignements sur le patient
                                              &m_currentDocDisplayMap        ,   // 5 liste des rubriques courantes (affichées)
                                              userActuelPk                   ,   // 6 utilisateur responsable du document
                                              0                              ,   // 7 pointeur sur la rubrique en cours de modif (si c'est une rubrique) zero sinon
                                               &G_pCApp->m_VAR_MAP               // 8 pointeur sur la liste des variables (zero par defaut)
                                             );
 if (  pfusion )
    {pfusion-> DoMixture();
     delete pfusion;
    }
}

//-------------------------------------- AdjustHeightFootAndHead ----------------------------------------------
// soit il existe un parametre fixé et sauvegardé dans la base de données et alors on utilise ce parametre
// soit il n'existe pas et alors on prend la hauteur des documents entete et pieds de page

void DlgPrint::AdjustHeightFootAndHead()
{   QString head_H  = "";
    QString foot_H  = "";
    QString param   = "";
    QString nameDoc = GetNameDocFromCombo();
    if (nameDoc.length()!=0)
       { param   = m_pCMoteurBase->ReadDrTuxUserParametres(m_SignUser);
         CGestIni::Param_ReadParam(param, nameDoc , "Head_H",    &head_H);
         CGestIni::Param_ReadParam(param, nameDoc , "Foot_H",    &foot_H);
       }
    if (m_pHeadRichText) //reajuster hauteur header à la taille de son contenu
    {   if (head_H != "")  m_pCViewPageWidget->SetHead_H(atof(head_H) / 10);
        else               m_pCViewPageWidget->SetHead_H(m_pCViewPageWidget->GetRichHead_H());
        lineEdit_Head_H->setText(QString::number(m_pCViewPageWidget->GetHead_H()*10));
        slider_Head_H->setValue ( (int)(m_pCViewPageWidget->GetHead_H()*100/m_PageH ) );
    }
    if (m_pFootRichText) //reajuster hauteur pied de page à la taille de son contenu
    {   if (foot_H != "")  m_pCViewPageWidget->SetFoot_H(atof(foot_H ) / 10);
        else               m_pCViewPageWidget->SetFoot_H(m_pCViewPageWidget->GetRichFoot_H());
        lineEdit_Foot_H->setText(QString::number(m_pCViewPageWidget->GetFoot_H()*10));
        slider_Foot_H->setValue ( (int)(m_pCViewPageWidget->GetFoot_H()*100/m_PageH ) );
    }
}

//-------------------------------------- Get_User_Head_Foot --------------------------------------------------
QString DlgPrint::Get_User_Head_Foot(QString user, QString* foot, const QString &nameDoc, const QString &str_type)
{return m_pCMoteurBase->GetUserDocument( user,
                                         nameDoc,
                                         m_pCMoteurBase->TypeDoc_To_PrintTypeDoc(str_type.toInt()),
                                         foot
                                         );  // sub type 0->RECTO   1->VERSO
}

//-------------------------------------- initRichBodyText ----------------------------------------------------
void DlgPrint::initRichBodyText(QSimpleRichText* pRichText)
{   m_pBodyRichText        = pRichText;
    m_pCViewPageWidget->SetBody(pRichText);
}

//-------------------------------------- initRichHeadText ----------------------------------------------------
void DlgPrint::initRichHeadText(QSimpleRichText* pRichText)
{
    m_pHeadRichText        = pRichText;
    m_pCViewPageWidget->SetHead(pRichText);
    if (pRichText) //reajuster hauteur header à la taille de son contenu
    {m_pCViewPageWidget->SetHead_H(m_pCViewPageWidget->GetRichHead_H());
        lineEdit_Head_H->setText(QString::number(m_pCViewPageWidget->GetHead_H()*10));
        slider_Head_H->setValue ( (int)(m_pCViewPageWidget->GetHead_H()*100/m_PageH ) );
    }
}

//-------------------------------------- initRichFootText -------------------------------------------
void DlgPrint::initRichFootText(QSimpleRichText* pRichText)
{   m_pFootRichText        = pRichText;
    m_pCViewPageWidget->SetFoot(pRichText);
    if (pRichText) //reajuster hauteur pied de page à la taille de son contenu
    {   m_pCViewPageWidget->SetFoot_H(m_pCViewPageWidget->GetRichFoot_H());
        lineEdit_Foot_H->setText(QString::number(m_pCViewPageWidget->GetFoot_H()*10));
        slider_Foot_H->setValue ( (int)(m_pCViewPageWidget->GetFoot_H()*100/m_PageH ) );
        UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
    }
}

//-------------------------------------- pushButtonOk_clicked --------------------------------------------------
void DlgPrint::pushButtonOk_clicked()
{accept();
}
//-------------------------------------- pushButtonPrint_2_clicked ------------------------------------------------
void DlgPrint::pushButtonPrint_2_clicked()
{printDoc(1);
}

//-------------------------------------- pushButtonPrint_2_2_clicked ------------------------------------------------
void DlgPrint::pushButtonPrint_2_2_clicked()
{printDoc(2);
}
//-------------------------------------- pushButtonPrint_clicked ----------------------------------------------
void DlgPrint::pushButtonPrint_clicked()
{printDoc(0);
}

//-------------------------------------- printDoc ------------------------------------------------------------
/*! \fn void DlgPrint::printDoc(int nbCopies)
 *  \brief Prépare l'impression des documents demandés. Gère les copies à la main...
*/
void DlgPrint::printDoc(int nbCopies)
{//if (comboBox_TypeDoc->count ()<=0) return;     // si liste des entetes et pieds de page vides --> cassos
    QString primKey = "";
    float      zoom = atof(lineEditZoom->text())/100;
    int    page_deb = atoi(lineEdit_PageDeb->text());
    int    page_fin = atoi(lineEdit_PageFin->text());
    //................ recuperer resolution affichée ............................
    int        resoX = atoi(lineEditResoX->text());
    int        resoY = atoi(lineEditResoY->text());
    int sav_nbCopies = m_pQPrinter->numCopies ();

    m_pQPrinter->setResolution(resoY);

#ifdef Q_WS_X11
    QPaintDevice::x11SetAppDpiX (resoX);
    QPaintDevice::x11SetAppDpiY (resoY);
#endif

    // .................. Intituter le document à imprimer  ..............................
    QString nom_patient;
    nom_patient = m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME,
				       m_pCMoteurBase->m_DOSS_INDEX_NOM,
				       m_pCMoteurBase->m_DOSS_INDEX_PRIM_KEY,
				       m_IdentPrimKey)+" ";
     nom_patient += m_pCMoteurBase->GetFieldValue(m_pCMoteurBase->m_DOSS_INDEX_TBL_NAME,
					 m_pCMoteurBase->m_DOSS_INDEX_PRENOM,
					 m_pCMoteurBase->m_DOSS_INDEX_PRIM_KEY ,
					 m_IdentPrimKey);
    m_pQPrinter->setCreator("Dr Tux");
//    if (nbCopies != 0) m_pQPrinter->setNumCopies(nbCopies);
    int j = 0;
    if (nbCopies == 0) j=1; else j=nbCopies;
    if (m_pQPrinter->numCopies() > 1)
    { // Remettre à 1 exemplaire par impression l'imprimante, récupérer le nombre de copies demandées
      j = m_pQPrinter->numCopies() * j;
      m_pQPrinter->setNumCopies(1);
    }

    // Récupérer dans le painter toutes les pages et toutes les copies
    //  (car bug avec setNumCopies pour certains drivers d'impression)
    // on recode tout à la main.... dur dur !!!
    int page = 0;
    int nb_page = 0;
    QString nomDoc1 = "";
    QString nomDoc2 = "";
    //.............. borner page_deb au max et min possible ..................
    if (page_deb<1)         page_deb = 1;
    if (page_deb>page_fin)  page_deb = page_fin;

 for (int i=0; i != j ; ++i)
  { // Préparer le nom de l'impression pour chaque exemplaire
    // Attention quand ça va très vite il faut un nom de document différent par impression
    // Pour ceux qui impriment sur driver cups-PDF par exemple.
    nomDoc2 = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-ms") + "-" + nom_patient;
    if (nomDoc2 == nomDoc1 ) nomDoc2 += QString::number(i+1);
    nomDoc1=nomDoc2;
    m_pQPrinter->setDocName(nomDoc2);

    // Débuter le painter pour l'impression
    QPainter* toPrint = new QPainter (m_pQPrinter);

    //.................. recharger le texte normal ..............................
    //              (peut avoir ete mis sur duplicata)
    primKey = GetCurrentComboPrimKey();
    GetHeadAndFoot(primKey, 0, 1); // 0 car on veut le document standard  1 car on veut redessiner le widget

    //.............. borner page_fin au max possible ........................
    nb_page = m_pCViewPageWidget->GetNbPage();
    if (page_fin>nb_page)
    {page_fin = nb_page;
        lineEdit_PageFin->setText(QString::number(page_fin));
    }

    page = page_deb;

    while(page <= page_fin )
    {   m_pCViewPageWidget->PrintDoc(toPrint, page, zoom ,zoom , 0, resoX, resoY, 0); // Ne termine pas le painter
        ++page;
        if (page > page_fin) break;
        m_pQPrinter->newPage();
    }

    //.................. intégrer le duplicata .....................
    primKey = IsExistDuplicata();
    //QMessageBox::information(this, " ", primKey, "ok","","",1,0);
    if (primKey != QString::null && (checkBox_PrintDuplicata->isVisible() && checkBox_PrintDuplicata->isChecked() ))
    {	m_pQPrinter->newPage();		// Sauter une page pour imprimer le duplicata
	GetHeadAndFoot(primKey, 1, 1);	// 1 car on veut le duplicata      1 car on veut redessiner le widget

	page = page_deb;
	while(page <= page_fin )
	  { m_pCViewPageWidget->PrintDoc(toPrint, page, zoom ,zoom , 0, resoX, resoY, 0);
	    ++page;
	    if (page > page_fin) break;
	    m_pQPrinter->newPage();
	  }
      } // Fin intégration du duplicata

    // Fin de l'impression pour ce document, détruire toPrint
    // Ceci à pour effet de terminer ce travail d'impression et d'en relancer un nouveau pour chaque exemplaire.
    toPrint->end();
    delete toPrint;
  } // Fin for nbcopies

    // retour à l'état antérieur
    m_pQPrinter->setNumCopies(sav_nbCopies);
    accept();
}

//-------------------------------- GetHeadAndFoot ------------------------------------------------
/*! \fn void DlgPrint::GetHeadAndFoot(QString prim_key, int type, int update)
 * \brief Regarde si le combo est sur une ordonnace (avec dupilcata donc) et l'extrait et reinitialise m_pCViewPageWidget dessus.
 *  type   = 0  --> document standard
 *  type   = 1  --> duplicata
 *  update = 0  --> pas de redessin de  m_pCViewPageWidget
 *  update = 0  --> redessin de  m_pCViewPageWidget
*/
void DlgPrint::GetHeadAndFoot(QString prim_key, int type, int update)
{   QString     foot  = "";
    QString     head  = "";
    if (prim_key=="") prim_key = GetCurrentComboPrimKey();
    if (prim_key=="") return;
    head = m_pCMoteurBase->GetUserDocument(prim_key,
                                           0,
                                           &foot,    // adresse de la chaine où l'on veut recuperer le pied de page
                                           type      // 0 par defaut (RECTO) 1 si l'on veut le DUPLICATA (que pour TYP_ORDONNANCE)
                                           );

    if (head !="" )
    {   m_HeadText = head;
    }
    if (foot != "")
    {   m_FootText = foot;
    }
   executeTransfertMacro();
   if (foot != "" || head !="")
       {   SetDialogOnHeadAndFoot(1);
           if (update) m_pCViewPageWidget->update ();
       }

    return ;
}

//-------------------------------- GetCurrentComboPrimKey ------------------------------------------------
QString DlgPrint::GetCurrentComboPrimKey()
{   int   item;
    if (comboBox_TypeDoc->count()==0 || (item  = comboBox_TypeDoc->currentItem()) ==-1 ) return QString::null;
    CPrtQListBoxItem *pCPrtQListBoxItem = (CPrtQListBoxItem*) comboBox_TypeDoc->listBox()->item(item);
    if (pCPrtQListBoxItem==0)                             return QString::null;   //ça manquait oh le mechant bug !!!
    return pCPrtQListBoxItem->GetPk();            // recuperer primkey
}

//--------------------------------------IsExistDuplicata -------------------------------------------------
/*! \fn QString DlgPrint::IsExistDuplicata()
 * \brief Regarde si le combo est sur une ordonnance (avec duplicata donc), l'extrait et retourne la clef primaire de l'enregistrement
*/
QString DlgPrint::IsExistDuplicata()
{   int   item;
    if (comboBox_TypeDoc->count()==0 ||  (item  = comboBox_TypeDoc->currentItem()) ==-1 )         return QString::null;
    CPrtQListBoxItem *pCPrtQListBoxItem = (CPrtQListBoxItem*) comboBox_TypeDoc->listBox()->item(item);
    if (pCPrtQListBoxItem==0)                                     return QString::null;   //ça manquait oh le mechant bug !!!
    if (pCPrtQListBoxItem->GetType().toInt() != TYP_ORDONNANCE)   return QString::null;
    return pCPrtQListBoxItem->GetPk();
}

//-------------------------------------- GetNameDocFromCombo() ----------------------------------------------------------------
QString DlgPrint::GetNameDocFromCombo()
{   int   item;
    if (comboBox_TypeDoc->count()==0 || (item  = comboBox_TypeDoc->currentItem()) ==-1 )         return QString::null;
    return comboBox_TypeDoc->text( item );
}

//-------------------------------------- pushButtonQuitter_clicked ------------------------------------------------------
void DlgPrint::pushButtonQuitter_clicked()
{reject();
}

//-------------------------------------- pushButtonConfigurer_clicked ------------------------------------------------------
void DlgPrint::pushButtonConfigurer_clicked()
{if ( m_pQPrinter->setup( this ) )
    {
    }
 SetCaption();
}

//-------------------------------------- pushButtonPrevious_clicked -----------------------------------------------------
void DlgPrint::pushButtonPrevious_clicked()
{  m_pCViewPageWidget->SetCurrentPage(m_pCViewPageWidget->GetCurrentPage()-1);
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- pushButtonNext_clicked ---------------------------------------------------------
void DlgPrint::pushButtonNext_clicked()
{   m_pCViewPageWidget->SetCurrentPage(m_pCViewPageWidget->GetCurrentPage()+1);
    m_pCViewPageWidget->update ();
    UpdateTextePageInfo(m_pCViewPageWidget->GetNbPage());
}

//-------------------------------------- checkBox_Duplicata_stateChanged ------------------------------------------------
void DlgPrint::checkBox_Duplicata_stateChanged(int )
{comboBox_TypeDoc_activated((comboBox_TypeDoc->currentItem()));
}




