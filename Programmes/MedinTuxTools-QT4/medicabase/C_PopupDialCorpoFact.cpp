#include "C_PopupDialCorpoFact.h"
#include "ui_C_PopupDialCorpoFact.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"

//----------------------------------- C_PopupDialCorpoFact -------------------------------------------
C_PopupDialCorpoFact::C_PopupDialCorpoFact( C_Frm_PosologieHitList   *pC_Frm_PosologieHitList,
                                            const QColor              &backGround,
                                            const QRect               &initialRect,
                                            const QRect               &deployRect
                                           ) :
    C_PopupDial(pC_Frm_PosologieHitList, backGround, initialRect, deployRect)
{   m_pC_Frm_PosologieHitList     =  pC_Frm_PosologieHitList;
    m_cur_grammar                 =  m_pC_Frm_PosologieHitList->grammar();
    m_IsPopupRun                  = false;
    if (pC_Frm_PosologieHitList)
         m_C_PatientCtx           = m_pC_Frm_PosologieHitList->c_bdm_api()->patientContexte();
    else
         m_C_PatientCtx           = 0;

    setupUi(this);
    //................ le menu des unite de facteur corporel ...............
    pushButton_mode->setIcon(Theme::getIcon("medicatux/menu_left.png"));
    pushButton_UFC->setIcon(Theme::getIcon("medicatux/menu_left.png"));
    m_poids_Menu = new ThemePopup(this);
    m_poids_Menu->setObjectName("HitList : pushButton_UFC menu ");
    ThemePopup::addOptionMenu(m_poids_Menu,tr("1 - par Kg de poids corporel"));
    ThemePopup::addOptionMenu(m_poids_Menu,tr("2 - par cm2 de surface corporelle "));
    ThemePopup::addOptionMenu(m_poids_Menu,tr("3 - par m2 de surface corporelle "));

    m_add_Menu   = new ThemePopup(this);
    m_add_Menu->setObjectName("HitList : pushButton_add menu");
    ThemePopup::addOptionMenu(m_add_Menu,tr("1 - mode fractionnaire simple (sur l'unit\303\251 de prise)"));
    ThemePopup::addOptionMenu(m_add_Menu,tr("2 - mode fractionnaire avanc\303\251 (sur le principe actif)"));
    ThemePopup::addOptionMenu(m_add_Menu,tr("3 - mode posologique non reli\303\251 au poids"));

    setFont(m_pC_Frm_PosologieHitList->font());

    //............... deploiement/repliement ..........................
    QRect dRect = QRect (m_pC_Frm_PosologieHitList->width()-width()-2,
                         deployRect.y(),
                         width(),
                         height());

    setRectanngles(initialRect, dRect);
    setToInitialRect(C_PopupDial::FAST);
    connect( pushButton_UFC,          SIGNAL(clicked ( bool )),                        this,  SLOT( Slot_pushButton_UFC_clicked ( bool )) );
    connect( pushButton_mode,         SIGNAL(clicked ( bool )),                        this,  SLOT( Slot_pushButton_mode_clicked ( bool )) );
    connect( this,                    SIGNAL( Sign_deployed (int&)),                   this , SLOT( Slot_DialCorpoFactDeployed(int&) ));
    connect( this,                    SIGNAL( Sign_minimized (int&)),                  this , SLOT( Slot_Dial_CorpoFactMinimized(int&) ));
    connectControls();
}
//------------------------------------ Slot_checkBox_secable_stateChanged ----------------------------------------------------------------------
void C_PopupDialCorpoFact::Slot_checkBox_secable_stateChanged ( int )
{ emit Sign_contentChanged(this);
}

//------------------------------------ Slot_PatientCtxChanged ----------------------------------------------------------------------
void C_PopupDialCorpoFact::Slot_PatientCtxChanged(C_PatientCtx *pC_PatientCtx)
{ disconnectControls();
  if (m_unite_facteurCorpo==tr("Kg"))
     { if (lineEdit_FactCorpo->text() != pC_PatientCtx->poids())
          { lineEdit_FactCorpo->setText(pC_PatientCtx->poids());
            emit Sign_contentChanged(this);
          }
     }
  else
     { if (lineEdit_FactCorpo->text() != pC_PatientCtx->surface())
          { lineEdit_FactCorpo->setText(pC_PatientCtx->surface());
            emit Sign_contentChanged(this);
          }
     }
  connectControls();
}

//------------------------------------ Slot_contentFactCorpoChanged ----------------------------------------------------------------------
void C_PopupDialCorpoFact::Slot_contentFactCorpoChanged(const QString &txt)
{   if (m_C_PatientCtx==0) return;
    if (m_unite_facteurCorpo==tr("Kg"))
       { m_C_PatientCtx->set_poids(txt);
       }
    else
       { double taille  = 0;
         double surface = txt.toDouble();
         m_C_PatientCtx->set_surface(txt);
         if (m_C_PatientCtx->poidsToDouble() != 0) taille = (surface*surface*3600)/m_C_PatientCtx->poidsToDouble();  // calcul de la  taille en fonction de la surface et poids
         m_C_PatientCtx->set_taille(QString::number(taille,'f',0));
       }
    emit Sign_FactCorpoChanged( m_C_PatientCtx );
    emit Sign_contentChanged(this);
}
//------------------------------------ Slot_contentChanged ----------------------------------------------------------------------
void C_PopupDialCorpoFact::Slot_contentChanged(const QString &txt)
{
    Q_UNUSED(txt);
    m_facteurCorpo = get_QuFC();
    setLabelFactCorpo();
    emit Sign_contentChanged(this);
}

//------------------------------------ Slot_pushButton_mode_clicked ----------------------------------------------------------------------
void C_PopupDialCorpoFact::Slot_pushButton_UFC_clicked ( bool )
{
    pushButton_UFC->setIcon(Theme::getIcon("medicatux/menu_down.png"));
    m_IsPopupRun        = true;
    int optionTexte     = m_poids_Menu->DoPopupList().toInt();
    switch( optionTexte )
      { default:
        case 1: m_unite_facteurCorpo = tr("Kg");  break;
        case 2: m_unite_facteurCorpo = tr("cm2"); break;
        case 3: m_unite_facteurCorpo = tr("m2");  break;
      }
    pushButton_UFC->setText(m_unite_facteurCorpo);
    setLabelFactCorpo();
    pushButton_UFC->setIcon(Theme::getIcon("medicatux/menu_left.png"));
    m_IsPopupRun        =   false;
}

//------------------------------------ Slot_pushButton_mode_clicked ----------------------------------------------------------------------
void C_PopupDialCorpoFact::Slot_pushButton_mode_clicked( bool  )
{   m_IsPopupRun        = true;
    int optionTexte     = m_add_Menu->DoPopupList().toInt();
    disconnectControls();
    setMode(optionTexte);
    connectControls();
    pushButton_mode->setIcon(Theme::getIcon("medicatux/menu_left.png"));
    m_IsPopupRun        =   false;
    emit Sign_contentChanged(this);
}

//------------------------------------ Slot_Dial_CorpoFactMinimized --------------------------------------------------------
void C_PopupDialCorpoFact::Slot_Dial_CorpoFactMinimized(int &ok)
{if (m_IsPopupRun) ok = 0;
 else              ok = 1;
}

//------------------------------------ Slot_DialDateDeployed ----------------------------------------------------------------------
void C_PopupDialCorpoFact::Slot_DialCorpoFactDeployed(int &ok)
{   ok=1;

 QPoint pos = mapFromGlobal (QCursor::pos());
 if (pos.x() < m_initialRect.width() - 30)      // si sur la zone edition ne pas deployer
    { ok=0;
     return;
    }
 ok = 1;
}

//------------------------------------ interfaceToGrammar --------------------------------------------------------
QString C_PopupDialCorpoFact::interfaceToGrammar( QString &grammar )
{    CGestIni::setXmlData("gph_pmin",      CGestIni::CutStrLeft  (get_QuPA_by_FC(), "-"),     grammar, 1);
     CGestIni::setXmlData("gph_pmax",      CGestIni::CutStrRight (get_QuPA_by_FC(), "-"),     grammar, 1);
     CGestIni::setXmlData("gph_punit",     get_QuPA_by_FC_Unit(),                             grammar, 1);
     CGestIni::setXmlData("gph_pfc",       get_QuFC(),                                        grammar, 1); // si zero alors 'mode non relie au poids'  c'est lui qui sera evalue lors de interfaceToGrammar()
     CGestIni::setXmlData("gph_pfcunit",   get_QuFC_Unit(),                                   grammar, 1);
     CGestIni::setXmlData("gph_pqbyuf",    lineEdit_QPA_by_UP->text(),                        grammar, 1);
     CGestIni::setXmlData("gph_psecable",  QString((checkBox_secable->isChecked()?"4":"-1")), grammar, 1);
   return grammar;
}
//------------------------------------ grammarToInterface ----------------------------------------------------------------------
void C_PopupDialCorpoFact::grammarToInterface(const QString &grammar, int mode /*=-1*/)
{
    int  nextPos                      = 0;
    m_unitForme                       = CGestIni::getXmlData("gph_uf",        grammar, &nextPos);  // comprime injection
    m_qPA_min_By_FactCorp             = CGestIni::getXmlData("gph_pmin",      grammar, &nextPos);
    m_qPA_max_By_FactCorp             = CGestIni::getXmlData("gph_pmax",      grammar, &nextPos);
    m_unitPoso                        = CGestIni::getXmlData("gph_punit",     grammar, &nextPos);  // mg
    m_facteurCorpo                    = CGestIni::getXmlData("gph_pfc",       grammar, &nextPos);  // 10              // si zero alors 'mode non relie au poids'  c'est lui qui sera evalue lors de interfaceToGrammar()
    m_unite_facteurCorpo              = CGestIni::getXmlData("gph_pfcunit",   grammar, &nextPos);  // kg m2 cm2
    m_qPA_ByUnitPrise                 = CGestIni::getXmlData("gph_pqbyuf",    grammar, &nextPos);  // 500 mg (par comprime)
    int gph_psecable                  = CGestIni::getXmlData("gph_psecable",  grammar, &nextPos).toInt();


    if (m_unite_facteurCorpo.length()==0)            m_unite_facteurCorpo = tr("Kg");
    else if (m_unite_facteurCorpo.startsWith("Kg"))  m_unite_facteurCorpo = tr("Kg");
    disconnectControls();
    //.............. le poids/surface toujours renseignes ..........................
    if (m_C_PatientCtx)
       { if ( m_unite_facteurCorpo==tr("Kg") )
             lineEdit_FactCorpo->setText(m_C_PatientCtx->poids());
         else
             lineEdit_FactCorpo->setText(m_C_PatientCtx->surface());
       }
    checkBox_secable->setChecked(gph_psecable!=-1);
    //....................... mode du dialogue selon indication grammaire .................
    if ( m_facteurCorpo.length() == 0 )
       {
         setMode( 3 );
         connectControls();
         return;
       }

    if ( m_qPA_ByUnitPrise == "-1" )
       { if (mode==-1) setMode(1);
         setLabelFactCorpo();
                                                        // Quantite ___ ___de principe actif par ___ ___unite (Kg M2) de facteur corporel
                                                        //           ^   ^                        ^   ^
    //   lineEdit_QPA->setText(qPA_ByFactCorpo);        //___________|   |                        |   |
    //   pushButton_UPA->setText(m_unitForme);          //_______________|                        |   |
         lineEdit_QFC->setText(m_facteurCorpo);         //________________________________________|   |
         pushButton_UFC->setText(m_unite_facteurCorpo); //____________________________________________|
         lineEdit_QPA_by_UP->setText(m_qPA_ByUnitPrise);
         label_QPA_by_UP_1->setText( tr("par %1").arg(m_unitForme) );
       }
    else
       { if (mode==-1) setMode(2);
         setLabelFactCorpo();
                                                        // Quantite ___ ___de principe actif par ___ ___unite (Kg M2) de facteur corporel
                                                        //           ^   ^                        ^   ^
    //   lineEdit_QPA->setText(qPA_ByFactCorpo);        //___________|   |                        |   |
    //   pushButton_UPA->setText(m_unitPoso);           //_______________|                        |   |
         lineEdit_QFC->setText(m_facteurCorpo);         //________________________________________|   |
         pushButton_UFC->setText(m_unite_facteurCorpo); //____________________________________________|
         lineEdit_QPA_by_UP->setText(m_qPA_ByUnitPrise);
         label_QPA_by_UP_1->setText( tr("par %1").arg(m_unitForme) );
    }
  connectControls();
}
//------------------------------------ disconnectControls ----------------------------------------------------------------------
void C_PopupDialCorpoFact::disconnectControls()
{
 disconnect( checkBox_secable,        SIGNAL( stateChanged ( int  )),                  this , SLOT( Slot_checkBox_secable_stateChanged ( int ) ));
 disconnect( lineEdit_FactCorpo,      SIGNAL( textChanged ( const QString &  )),       this , SLOT( Slot_contentFactCorpoChanged( const QString & ) ));
 disconnect( lineEdit_QPA,            SIGNAL( textChanged ( const QString &  )),       this , SLOT( Slot_contentChanged( const QString & ) ));
 disconnect( lineEdit_QFC,            SIGNAL( textChanged ( const QString &  )),       this , SLOT( Slot_contentChanged( const QString & ) ));
 disconnect( lineEdit_QPA_by_UP,      SIGNAL( textChanged ( const QString &  )),       this , SLOT( Slot_contentChanged( const QString & ) ));
}
//------------------------------------ connectControls ----------------------------------------------------------------------
void C_PopupDialCorpoFact::connectControls()
{
 connect( checkBox_secable,        SIGNAL( stateChanged ( int  )),                  this , SLOT( Slot_checkBox_secable_stateChanged ( int ) ));
 connect( lineEdit_FactCorpo,      SIGNAL( textChanged ( const QString &  )),       this , SLOT( Slot_contentFactCorpoChanged( const QString & ) ));
 connect( lineEdit_QPA,            SIGNAL( textChanged ( const QString &  )),       this , SLOT( Slot_contentChanged( const QString & ) ));
 connect( lineEdit_QFC,            SIGNAL( textChanged ( const QString &  )),       this , SLOT( Slot_contentChanged( const QString & ) ));
 connect( lineEdit_QPA_by_UP,      SIGNAL( textChanged ( const QString &  )),       this , SLOT( Slot_contentChanged( const QString & ) ));
}
//------------------------------------ setLabelFactCorpo ----------------------------------------------------------------------
void C_PopupDialCorpoFact::setLabelFactCorpo()
{ if (lineEdit_QPA_by_UP->text()=="-1")
     { label_unit_FactCorpo->setText("/ "+ m_facteurCorpo+" "+m_unite_facteurCorpo);
       label_kg_x->setText(m_unitForme + " x "+ lineEdit_QPA->text() + " x ");
     }
  else
     {label_unit_FactCorpo->setText(m_unite_facteurCorpo);
      label_kg_x->setText(lineEdit_QPA->text().replace("-",tr(" \303\240 "))+" "+m_unitPoso+" x ");
     }
}

//------------------------------------ setMode ----------------------------------------------------------------------
void C_PopupDialCorpoFact::setMode(int mode)
{   QString qPA_ByFactCorpo           = m_qPA_min_By_FactCorp;
    if (m_qPA_max_By_FactCorp.length() && m_qPA_min_By_FactCorp != m_qPA_max_By_FactCorp) qPA_ByFactCorpo += "-" + m_qPA_max_By_FactCorp;

    switch(mode)
    { case 1:
        pushButton_mode->setText(tr("mode fractionnaire simple (sur l'unit\303\251 de prise)"));
        //label_QFC->hide();
        //lineEdit_QFC->hide();
        //pushButton_UFC->hide();
        label_QPA->setText(tr("Quantit\303\251 par jour"));   // lineEdit_QPA
        label_kg_x->show();
        lineEdit_FactCorpo->show();
        label_unit_FactCorpo->show();

        label_QPA->show();
        lineEdit_QPA->show();
        pushButton_UPA->show();
        label_QFC->show();
        lineEdit_QFC->show();
        pushButton_UFC->show();

        label_QPA_by_UP_0->hide();
        label_QPA_by_UP_1->hide();
        lineEdit_QPA_by_UP->hide();
        lineEdit_QPA_by_UP->setText("-1");
        lineEdit_QPA->setText(qPA_ByFactCorpo);
        pushButton_UPA->setText(m_unitForme);
        lineEdit_QFC->setText(m_facteurCorpo);     // si zero alors 'mode non relie au poids' c'est lui qui sera evalue lors de interfaceToGrammar()
        label_kg_x->setText(m_unitForme + " x "+ qPA_ByFactCorpo + " x ");
        break;
      case 2:
        pushButton_mode->setText(tr("mode fractionnaire avanc\303\251 (sur le principe actif)"));
        //label_QFC->show();
        //lineEdit_QFC->show();
        //pushButton_UFC->show();
        label_QPA->setText(tr("Quantité de principe actif"));
        label_kg_x->show();
        lineEdit_FactCorpo->show();
        label_unit_FactCorpo->show();

        label_QPA->show();
        lineEdit_QPA->show();
        pushButton_UPA->show();
        label_QFC->show();
        lineEdit_QFC->show();
        pushButton_UFC->show();

        label_QPA_by_UP_0->show();
        label_QPA_by_UP_1->show();
        lineEdit_QPA_by_UP->show();
        lineEdit_QFC->setText(m_facteurCorpo);  // si zero alors 'mode non relie au poids'  c'est lui qui sera evalue lors de interfaceToGrammar()
        lineEdit_QPA_by_UP->setText(m_qPA_ByUnitPrise);
        lineEdit_QPA->setText(qPA_ByFactCorpo);
        pushButton_UPA->setText(m_unitPoso);

        label_kg_x->setText(qPA_ByFactCorpo.replace("-",tr(" \303\240 "))+" "+m_unitPoso+" x ");            // __(10)  __(mg) par
        break;
    case 3:
        pushButton_mode->setText(tr("posologie non reli\303\251e au poids (cliquez pour changer)"));

        label_QPA->setText(tr("Quantit\303\251 de principe actif"));
        label_kg_x->show();
        lineEdit_FactCorpo->hide();
        label_unit_FactCorpo->hide();

        label_QPA->hide();
        lineEdit_QPA->hide();
        pushButton_UPA->hide();
        label_QFC->hide();
        lineEdit_QFC->hide();
        pushButton_UFC->hide();

        label_QPA_by_UP_0->hide();
        label_QPA_by_UP_1->hide();
        lineEdit_QPA_by_UP->hide();
        lineEdit_QFC->setText("");     // si zero alors 'mode non relie au poids'  c'est lui qui sera evalue lors de interfaceToGrammar()
        label_kg_x->setText(tr("posologie non calcul\303\251e"));
        break;
    }
}
//------------------------------------ get_QuPA_by_FC ----------------------------------------------------------------------
QString C_PopupDialCorpoFact::get_QuPA_by_FC()        // 10
{return lineEdit_QPA->text();
}
//------------------------------------ get_QuPA_by_FC_Unit ----------------------------------------------------------------------
QString C_PopupDialCorpoFact::get_QuPA_by_FC_Unit()   // mg
{return pushButton_UPA->text();
}
//------------------------------------ get_QuFC ----------------------------------------------------------------------
QString C_PopupDialCorpoFact::get_QuFC()              // 3
{   if ( pushButton_mode->text() == tr("mode non relié au poids")) return "";
    return lineEdit_QFC->text();
}
//------------------------------------ get_QuFC_Unit ----------------------------------------------------------------------
QString C_PopupDialCorpoFact::get_QuFC_Unit()         // Kg           --> 10 mg par 3 kg de poids
{return pushButton_UFC->text();
}
//------------------------------------ get_QuPA_by_UP ----------------------------------------------------------------------
QString C_PopupDialCorpoFact::get_QuPA_by_UP()        // 500 mg par comprime
{return lineEdit_QPA_by_UP->text();
}

//------------------------------------ setFont --------------------------------------------------------
void C_PopupDialCorpoFact::setFont( QFont fnt)
{
 C_PopupDial::setFont(fnt);
 m_fGrd  =  fnt;
 label_unit_FactCorpo->setFont(m_fGrd);
 lineEdit_FactCorpo->setFont(m_fGrd);
 label_kg_x->setFont(m_fGrd);
 pushButton_UPA->setFont(m_fGrd);
 label_QPA->setFont(m_fGrd);
 label_QFC->setFont(m_fGrd);
 label_QPA_by_UP_0->setFont(m_fGrd);
 label_QPA_by_UP_1->setFont(m_fGrd);
 lineEdit_QPA_by_UP->setFont(m_fGrd);
 m_add_Menu->setFont(m_fGrd);
 m_poids_Menu->setFont(m_fGrd);
 checkBox_secable->setFont(m_fGrd);
 fnt.setBold(true);
 pushButton_UFC->setFont(fnt);
 fnt.setUnderline(true);
 pushButton_mode->setFont(fnt);
}
//----------------------------------- ~C_PopupDialCorpoFact -------------------------------------------
C_PopupDialCorpoFact::~C_PopupDialCorpoFact()
{
}
