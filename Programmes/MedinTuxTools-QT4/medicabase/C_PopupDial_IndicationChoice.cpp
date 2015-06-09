#include "C_PopupDial_IndicationChoice.h"
#include "ui_C_PopupDial_IndicationChoice.h"
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QTreeWidgetItem>
#include "../../MedinTuxTools-QT4/medicabase/C_LevelItemDelegate.h"
#include "../../MedinTuxTools-QT4/Theme/Theme.h"


//----------------------------------- C_PopupDial_IndicationChoice -------------------------------------------
C_PopupDial_IndicationChoice::C_PopupDial_IndicationChoice(C_Frm_PosologieHitList *pC_Frm_PosologieHitList,
                                                           CMyQLineEdit           *pCMyQLineEdit,
                                                           const QColor            &backGround,
                                                           const QRect             &initialRect,
                                                           const QRect             &deployRect) :
   C_PopupDial(pC_Frm_PosologieHitList, backGround, initialRect, deployRect)
{   //m_cur_grammar             = pC_Frm_PosologieHitList->grammar();
    m_pC_Frm_PosologieHitList = pC_Frm_PosologieHitList;
    m_pCMyQLineEdit           = pCMyQLineEdit;
    QString text_indic        = m_pCMyQLineEdit->text().trimmed();
    setupUi(this);
    checkBox_IndicationCIM10->hide();
    checkBox_IndicationBDM->hide();
    checkBox_SMR->hide();
    checkBox_ASMR->hide();
    setFont(pC_Frm_PosologieHitList->font());
    treeWidget_Indications->clear();
    QString text =  tr("Veuillez s\303\251lectionner une indication dans la liste ci-dessous.<br>"
                       "Double cliquez pour lier une indication \303\240 votre prescription.<br>"
                       "Simple clic pour apercevoir le contenu de l'indication ici-m\303\252me.<br>"
                       "La premi\303\250re indication de cette liste est modifiable ici-m\303\252me.");
    textEdit_memo->setText(text);
    textEdit_memo->setStyleSheet( QString("background-color: %1;font: 10px;").arg(backGround.lighter(120).name()));
    treeWidget_Indications->setStyleSheet( QString("background-color: %1;font: 10px;").arg(backGround.lighter(120).name()));
    QTreeWidgetItem *pQTreeWidgetItem = new QTreeWidgetItem(treeWidget_Indications);
    pQTreeWidgetItem->setText(INDIC_COL_CODE,       "");
    pQTreeWidgetItem->setText(INDIC_COL_SMR,        "");
    pQTreeWidgetItem->setText(INDIC_COL_DATE,       "");
    pQTreeWidgetItem->setText(INDIC_COL_COMMENT,    "");
    pQTreeWidgetItem->setText(INDIC_COL_LIBELLE,    (text_indic.length()?text_indic:tr("Texte libre \303\240 modifier....")));
    pQTreeWidgetItem->setText(INDIC_COL_TYPE,       "F");
    pQTreeWidgetItem->setIcon(INDIC_COL_ICONE,Theme::getIcon("medicatux/indic_free.png"));
    treeWidget_Indications->insertTopLevelItem( 0, pQTreeWidgetItem );

    fillTreeWidgetIndications(pC_Frm_PosologieHitList->cip());
    QRect dRect = QRect (deployRect.x(),
                         deployRect.y(),
                         width(),
                         deployRect.height());

    setRectanngles(initialRect, dRect);
    setToInitialRect(C_PopupDial::FAST);
    connect( this, SIGNAL( Sign_deployed (int&)),                                             this , SLOT( Slot_Dial_IndicationChoiceDeployed(int&) ));
    connect( treeWidget_Indications, SIGNAL(itemClicked ( QTreeWidgetItem * , int  )),        this , SLOT( Slot_treeWidget_Indications_itemClicked ( QTreeWidgetItem * , int   )) );
    connect( treeWidget_Indications, SIGNAL(itemDoubleClicked ( QTreeWidgetItem * , int  )),  this , SLOT( Slot_treeWidget_Indications_itemDoubleClicked ( QTreeWidgetItem * , int   )) );
    connect( textEdit_memo,          SIGNAL(textChanged()),                                   this , SLOT( Slot_textChanged()) );
}
//--------------------------------------- Slot_textChanged -------------------------------------------------------
void C_PopupDial_IndicationChoice::Slot_textChanged()
{   QTreeWidgetItem *pQTreeWidgetItem = treeWidget_Indications->currentItem();
    if ( pQTreeWidgetItem == 0 )                   return;
    if ( pQTreeWidgetItem->text(INDIC_COL_TYPE) != "F" ) return;   // seuls les textes libres son editables
    QString text = textEdit_memo->toPlainText();
    if (text==tr("Texte libre \303\240 modifier...."))  m_pCMyQLineEdit->setText ("");
    else                                         m_pCMyQLineEdit->setText (text);
    pQTreeWidgetItem->setText (INDIC_COL_LIBELLE,text);
}

//--------------------------------------- Slot_treeWidget_Indications_itemClicked -------------------------------------------------------

void C_PopupDial_IndicationChoice::Slot_treeWidget_Indications_itemClicked ( QTreeWidgetItem *pQTreeWidgetItem , int   )
{if (m_pCMyQLineEdit  == 0) return;
 if (pQTreeWidgetItem == 0) return;
 QString indic_str    = "";
 QString col_smr      = pQTreeWidgetItem->text(INDIC_COL_SMR);   // si SMR ou ASM est egale a S4 S3 S1 pour le niveau si rembourse est egal a la somme 2.94 3.15 etc...
 if (pQTreeWidgetItem->text(INDIC_COL_TYPE)=="SMR")
     indic_str = col_smr.mid(1)+"|SMR|"+pQTreeWidgetItem->text(INDIC_COL_LIBELLE);
 else
     indic_str = pQTreeWidgetItem->text(INDIC_COL_CODE)+"|"+pQTreeWidgetItem->text(INDIC_COL_TYPE)+"|"+pQTreeWidgetItem->text(INDIC_COL_LIBELLE);
 indic_str += "|";                                                                                           // rajouter la case indication remboursee
 if ( !(col_smr.startsWith("S")||col_smr.startsWith("A")) ) indic_str += col_smr;     // rajouter la valeur indication remboursee

 m_pC_Frm_PosologieHitList->indication_stringToInterface( indic_str );
 disconnect( textEdit_memo,          SIGNAL(textChanged()),                                   this , SLOT( Slot_textChanged()) );
 if ( pQTreeWidgetItem->text(INDIC_COL_TYPE) == "F" )
    { textEdit_memo->setStyleSheet( QString("background-color: %1;font: 10px;").arg("#FFFFFF"));
      textEdit_memo->setTextInteractionFlags ( Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard|Qt::TextEditable );
    }
 else
    { textEdit_memo->setStyleSheet( QString("background-color: %1;font: 10px;").arg(m_backGround.lighter(120).name()));
      textEdit_memo->setTextInteractionFlags ( Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard );
    }
 QString text = pQTreeWidgetItem->text(INDIC_COL_LIBELLE);
 textEdit_memo->setText(text + pQTreeWidgetItem->text(INDIC_COL_COMMENT));
 if (text==tr("Texte libre \303\240 modifier...."))
    { textEdit_memo->setFocus();
      textEdit_memo->selectAll();
    }
 connect(    textEdit_memo,          SIGNAL(textChanged()),                                   this , SLOT( Slot_textChanged()) );

}
//--------------------------------------- Slot_treeWidget_Indications_itemDoubleClicked -------------------------------------------------------
void C_PopupDial_IndicationChoice::Slot_treeWidget_Indications_itemDoubleClicked ( QTreeWidgetItem *pQTreeWidgetItem , int c   )
{Slot_treeWidget_Indications_itemClicked (pQTreeWidgetItem , c);
 setToInitialRect();
}

//--------------------------------------- fillTreeWidgetIndications -------------------------------------------------------
void C_PopupDial_IndicationChoice::fillTreeWidgetIndications(const QString &cip)
{
 QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
 m_pC_Frm_PosologieHitList->c_bdm_api()->fillTreeWidgetIndicationsFromPatientContext(treeWidget_Indications,Theme::getIcon("medicatux/terrain_indic.png"));
 QList <C_BDM_IndicationRecord>    retList = m_pC_Frm_PosologieHitList->c_bdm_api()->getIndicationsFromCIP( cip, C_BDM_PluginI::ALL_INDIC | C_BDM_PluginI::REFUND  );
 QTreeWidgetItem                    *item  = 0;
 C_LevelItemDelegate *pC_LevelItemDelegate = 0;

 if (retList.size())
    { for (int i = 0; i < retList.size(); ++i )
          { C_BDM_IndicationRecord indicationRecord   = retList.at(i);
            QString codeTyp                           = indicationRecord.codeTyp();
            item                                      = new QTreeWidgetItem (treeWidget_Indications);
            pC_LevelItemDelegate                      = new C_LevelItemDelegate(treeWidget_Indications);
            treeWidget_Indications->setItemDelegate(pC_LevelItemDelegate);
            if      (indicationRecord.levelTyp()=="S") {codeTyp = "SMR"; }
            else if (indicationRecord.levelTyp()=="A") {codeTyp = "ASMR"; }
            if (indicationRecord.refundLink().length())
                item->setText(INDIC_COL_SMR,indicationRecord.refundLink());   // un tarif commence par un chiffre et ne sera pas interprete comme un progressBar
            else
                item->setText(INDIC_COL_SMR,indicationRecord.level());        // un niveau sera dessine comme progressBar tel car commence par une lettre

            //item->setText(COL_SMR,niveau);                                   // niveau
            item->setText(INDIC_COL_LIBELLE,indicationRecord.indication());    // libelle
            item->setText(INDIC_COL_CODE,indicationRecord.code());             // code
            item->setText(INDIC_COL_DATE,indicationRecord.date());             // codeTyp
            item->setText(INDIC_COL_TYPE,codeTyp);                             // codeTyp
            item->setText(INDIC_COL_COMMENT,indicationRecord.descriptif());    // commentaire
            item->setText(INDIC_COL_REFUNDED,indicationRecord.refundLink());   // remboursement
            item->setText(INDIC_COL_CIP,indicationRecord.cip());               // cip
            if (codeTyp=="CIM")
               {item->setIcon(INDIC_COL_ICONE, QPixmap(m_pC_Frm_PosologieHitList->bitMapCollection()->imagePath("CimItem.png")));
               }
            else if (codeTyp=="DS_I")
               {item->setIcon(INDIC_COL_ICONE,QPixmap(m_pC_Frm_PosologieHitList->bitMapCollection()->imagePath("VidalItem.png")));
               }
            else if (codeTyp=="TH_I")
               {item->setIcon(INDIC_COL_ICONE,QPixmap(m_pC_Frm_PosologieHitList->bitMapCollection()->imagePath("TheriaqueItem.png")));
               }
            else
               {item->setIcon(INDIC_COL_ICONE,QPixmap(m_pC_Frm_PosologieHitList->bitMapCollection()->imagePath("HasItem.png")));
               }
          }
    }
 treeWidget_Indications->setColumnWidth( 0 , 220 );

 QApplication::restoreOverrideCursor();
}

//------------------------------------ setFont --------------------------------------------------------
void C_PopupDial_IndicationChoice::setFont(const QFont &fnt)
{
 C_PopupDial::setFont(fnt);
 m_fGrd  =  fnt;
}
//----------------------------------- ~C_PopupDial_IndicationChoice -------------------------------------------
C_PopupDial_IndicationChoice::~C_PopupDial_IndicationChoice()
{
}
//------------------------------------------------------ Slot_DialDateDeployed ----------------------------------------------------------------------
void C_PopupDial_IndicationChoice::Slot_Dial_IndicationChoiceDeployed(int &ok)
{   ok=1;
 /*
 QPoint pos = mapFromGlobal (QCursor::pos());
 if (pos.x() < m_initialRect.width() - 30)      // si sur la zone edition ne pas deployer
    { ok=0;
     return;
    }

 ok = 1;
 */
}
