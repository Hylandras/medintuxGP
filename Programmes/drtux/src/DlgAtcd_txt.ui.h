/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#define TR	QObject::tr
#include "CApp.h"
#include <qdatetime.h>
#include <qdatetimeedit.h>
#include <qlistbox.h>
#include "../../MedinTuxTools/CGestIni.h"
#include "../../MedinTuxTools/CMaskedLineEdit.h"
#include "ui/Dlg_Calendar.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli


#include "C_Dlg_FamilleGenre.h"
//--------------------------------- init -------------------------------------------------------------
void DlgAtcd_txt::init()
{ // initialise le combo des années
 m_ATCD_FamilyList.clear();
 m_ATCD_GenreList.clear();
 m_toViewOnly             = FALSE;
 m_isFamilleGenreModified = FALSE;
 pushButtonFamilleGenreEdit->setPixmap(Theme::getIcon( "16x16/configure.png"));
 line1->setFrameStyle ( QFrame::HLine );
 line1->setFrameShadow (QFrame::Sunken );
 m_LineEdit_Libelle->setFocus();
 CGestIni::Param_UpdateFromDisk(G_pCApp->m_PathAppli + "Ressources/MenuATCD.txt", m_structStr);
 connect( m_Combo_Family, SIGNAL( highlighted(const QString & ) ), this,    SLOT( comboBox_Family_highlighted(const QString &) ) );
 connect( pushButtonFamilleGenreEdit,  SIGNAL( clicked() ),        this,    SLOT( Slot_pushButtonFamilleGenreEdit() ) );
 setComboFamilyGenre();
 textLabel_Libelle_Long->hide();
 m_LineEdit_Libelle_Long->hide();
 pushButtonDateDebDlg->setIconSet( Theme::getIconSet("16x16/calendrierDeb.png"));
 pushButtonDateFinDlg->setIconSet( Theme::getIconSet("16x16/calendrierDeb.png"));
 pushButtonDateDeb_Del->setIconSet( Theme::getIconSet("16x16/date_annule.png"));
 pushButtonDateFin_Del->setIconSet( Theme::getIconSet("16x16/date_annule.png"));
 connect( pushButtonDateDebDlg, SIGNAL( clicked()),   this,  SLOT  ( Slot_pushButtonDateDebDlg())  );
 connect( pushButtonDateFinDlg, SIGNAL( clicked()),   this,  SLOT  ( Slot_pushButtonDateFinDlg())  );
 connect( pushButtonDateDeb_Del, SIGNAL( clicked()),   this,  SLOT  ( Slot_pushButtonDateDeb_Del())  );
 connect( pushButtonDateFin_Del, SIGNAL( clicked()),   this,  SLOT  ( Slot_pushButtonDateFin_Del())  );
}

//------------------------------- Slot_pushButtonDateDebDlg -------------------------------------
void DlgAtcd_txt::Slot_pushButtonDateDebDlg()
{
        Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", FALSE);
        if (dlg ==0)                                                   return;
        dlg->setCaption(tr("Date et heure"));
        if ( ! dlg->setDateTimeNatural_jjMMyyyy_Order(cMaskedLineDateDebAtcd->text() + "00:00:00" ))
           {dlg->setDate  (QDate::currentDate());
           }
        dlg->hideHeure();
        if (dlg->exec()== QDialog::Accepted )
           {cMaskedLineDateDebAtcd->setText(dlg->getDate().toString("ddMMyyyy"));
           }
        delete dlg;
}
//------------------------------- Slot_pushButtonDateFinDlg -------------------------------------
void DlgAtcd_txt::Slot_pushButtonDateFinDlg()
{
        Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", FALSE);
        if (dlg ==0)                                                   return;
        dlg->setCaption(tr("Date et heure"));
        if ( ! dlg->setDateTimeNatural_jjMMyyyy_Order(cMaskedLineDateFinAtcd->text() + "00:00:00" ))
           {dlg->setDate  (QDate::currentDate());
           }
        dlg->hideHeure();
        if (dlg->exec()== QDialog::Accepted )
           {cMaskedLineDateFinAtcd->setText(dlg->getDate().toString("ddMMyyyy"));
           }
        delete dlg;
}
//--------------------------------- Slot_pushButtonDateDeb_Del -------------------------------------------------------------
void DlgAtcd_txt::Slot_pushButtonDateDeb_Del()
{ cMaskedLineDateDebAtcd->setText("");
}
//--------------------------------- Slot_pushButtonDateFin_Del -------------------------------------------------------------
void DlgAtcd_txt::Slot_pushButtonDateFin_Del()
{ cMaskedLineDateFinAtcd->setText("");
}
//--------------------------------- isFamilleGenreModified -------------------------------------------------------------
bool DlgAtcd_txt::isFamilleGenreModified()
{return m_isFamilleGenreModified;
}

//--------------------------------- Slot_pushButtonFamilleGenreEdit -------------------------------------------------------------
void DlgAtcd_txt::Slot_pushButtonFamilleGenreEdit()
{ C_Dlg_FamilleGenre *pC_Dlg_FamilleGenre = new C_Dlg_FamilleGenre(&m_structStr, this); if (pC_Dlg_FamilleGenre==0) return;
  pC_Dlg_FamilleGenre->exec();
  if (pC_Dlg_FamilleGenre->isModified())
     {setComboFamilyGenre();
      m_isFamilleGenreModified = TRUE;
     }
  delete pC_Dlg_FamilleGenre;
}

//--------------------------------- setComboOnValue -------------------------------------------------------------
void DlgAtcd_txt::setComboOnValue(QComboBox *pQComboBox, const QString &value)
{if (value.length() == 0)                                                                              return;
 QListBox     *pQListBox      = pQComboBox->listBox ();                        if (pQListBox     == 0) return;
 QListBoxItem *pQListBoxItem  = pQListBox->findItem ( value, Qt::ExactMatch ); if (pQListBoxItem == 0) return;

 pQListBox->setCurrentItem ( pQListBoxItem );
 pQComboBox->setCurrentItem ( pQListBox->index ( pQListBoxItem) );
}
/*
//--------------------------------- isALD -------------------------------------------------------------
bool DlgAtcd_txt::isALD()
{return radioButton_ALD->isOn();
}
//--------------------------------- isSport-------------------------------------------------------------
bool DlgAtcd_txt::isSport()
{return radioButton_Sport->isOn();
}
*/
//--------------------------------- setSate_Ald_Sport -------------------------------------------------------------
void DlgAtcd_txt::setSate_Ald_Sport(const QString &sport_ald)
{
 if          (sport_ald[0]=='A') radioButton_ALD->setChecked(true);
 else  if    (sport_ald[0]=='S') radioButton_Sport->setChecked(true);
 else  if    (sport_ald[0]=='V') radioButton_Vigilance->setChecked(true);
 else                            radioButton_nonDefini->setChecked(true);
}

//--------------------------------- setDateFin -------------------------------------------------------------
QString DlgAtcd_txt::setDateFin(const QString &old_state)
{  QDate dateFin;
   QString etat = Atcd_Code::setNewState_FromOldState(old_state, &dateFin);
   if (dateFin.isValid())
     { setDateFin( dateFin );
     }
   return etat;
}
//--------------------------------- setDateFin -------------------------------------------------------------
/*! \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void DlgAtcd_txt::setDateFin(QDate dt)
{ cMaskedLineDateDebAtcd->setText(dt.toString("ddMMyyyy"));
}
//--------------------------------- setDateDeb -------------------------------------------------------------
/*! \brief A tout moment, met les combobox de la date aux bonnes valeurs. La date est passée en paramètre au format "dd-MM-yyyy".
 */
void DlgAtcd_txt::setDateDeb(QDate dt)
{ cMaskedLineDateDebAtcd->setText(dt.toString("ddMMyyyy"));
}

//--------------------------------- getSate_Ald_Sport_Sport_Sate -------------------------------------------------------------
QString DlgAtcd_txt::getSate_Ald_Sport()
{ if      (radioButton_ALD->isOn())        return QString("ALD");
  else if (radioButton_Sport->isOn())      return QString("Sport");
  else if (radioButton_Vigilance->isOn())  return QString("Vigilance");
  return QString("");
}
//--------------------------------- getDateFin -------------------------------------------------------------
QString DlgAtcd_txt::getDateFin()
{return Atcd_Code::valid_DateStringOrEmptyString(cMaskedLineDateFinAtcd->text());
}
//--------------------------------- getDateDeb -------------------------------------------------------------
QString DlgAtcd_txt::getDateDeb()
{return Atcd_Code::valid_DateStringOrEmptyString(cMaskedLineDateDebAtcd->text());
}

//--------------------------------- getInfos -------------------------------------------------------------
void DlgAtcd_txt::getInfos(QString& libelle, QString& family, QString& dateDebIso,QString& dateFinIso, QString& commentaire)
{ libelle     = m_LineEdit_Libelle->text();
  commentaire = m_LineEdit_Commentaire->text();
  family      = getFamilleGenre();
  dateDebIso  = Atcd_Code::valid_DateStringOrEmptyString(cMaskedLineDateDebAtcd->text(),"yyyy-MM-dd");
  dateFinIso  = Atcd_Code::valid_DateStringOrEmptyString(cMaskedLineDateFinAtcd->text(),"yyyy-MM-dd");
}
//--------------------------------- getFamilleGenre -------------------------------------------------------------
QString DlgAtcd_txt::getFamilleGenre()
{ QString family =  m_Combo_Family->currentText();
  QString genre  =  m_Combo_Genre->currentText();
  if (family  != genre) family += " ("+m_Combo_Genre->currentText()+")";
  return family;
}
//--------------------------------- getLibelle -------------------------------------------------------------
QString DlgAtcd_txt::getLibelle()
{ return m_LineEdit_Libelle->text();
}
//---------------------------------  -------------------------------------------------------------
QString DlgAtcd_txt::getComment()
{ return m_LineEdit_Commentaire->text();
}
//--------------------------------- setComboFamilyGenre -------------------------------------------------------------
void DlgAtcd_txt::setComboFamilyGenre()
{m_ATCD_FamilyList.clear();
 QString menuStr        = m_structStr;
 QString section        = "";
 int posDebSection      = -1;
 int posEndSection      = 0;
 while ( (posDebSection = menuStr.find("[",posEndSection)) != -1 )
    {++posDebSection;
     if ( (posEndSection = menuStr.find("]",posDebSection)) != -1 )
        {section = menuStr.mid(posDebSection,posEndSection-posDebSection).stripWhiteSpace();
         if (section.length())
            {m_ATCD_FamilyList<<section;
            }
        }
    }
 m_Combo_Family->clear();
 m_Combo_Family->setCurrentItem(0);
 m_Combo_Family->insertStringList(  m_ATCD_FamilyList );
 comboBox_Family_highlighted(       m_Combo_Family->text ( 0 )  );
}

//--------------------------------- setFamilyGenre -------------------------------------------------------------
void DlgAtcd_txt::setFamilyGenre(QString familyGenre)
{ bool isComboSet  = FALSE;
  familyGenre = familyGenre.stripWhiteSpace();
  if (familyGenre.length())
     {   QString genre  = "";
         int posDeb     = familyGenre.find("(");
         int posEnd     = posDeb;
         if (posDeb    != -1)
            { ++posDeb;
              posEnd    = familyGenre.find(")",posDeb);
              if (posEnd != -1)
                  {genre       = familyGenre.mid(posDeb,posEnd-posDeb).stripWhiteSpace();
                   familyGenre = familyGenre.left(posDeb-1).stripWhiteSpace();
                   setComboOnValue(m_Combo_Family , familyGenre);
                   //CGestIni::Param_GetList(m_structStr, familyGenre, "",  m_ATCD_GenreList , 1);
                   //m_ATCD_GenreList.prepend(familyGenre);
                   //m_Combo_Genre->clear();
                   //m_Combo_Genre->insertStringList( m_ATCD_GenreList );
                   setComboOnValue(m_Combo_Genre,   genre);
                   isComboSet = TRUE;
                  }
            }
         if (isComboSet  == FALSE)  setComboOnValue(m_Combo_Family , familyGenre);
     }
 else
     {setComboOnValue(m_Combo_Family,tr("Médicaux"));
     }
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////   differents modes du dialogue  ///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//--------------------------------- setDialogForAddToThesaurus -------------------------------------------------------------
void DlgAtcd_txt::setDialogForAddToThesaurus()
{lbl_Titre->setText(tr("<b>Définir le libellé usuel, la famille et le genre</b>"));
 textLabel_Libelle->setText(tr("Libellé usuel "));
 //................ on ne montre que le texte du libelle et la classification des genres .............
 groupBox_dates->hide();
 buttonGroup_Sport_ALD->hide();
 textLabel_Commentaire->hide();
 m_LineEdit_Commentaire->hide();

 textLabel_Libelle_Long->show();
 m_LineEdit_Libelle_Long->show();
 setCaption (tr("Ajouter un élément au thésaurus"));
}
//--------------------------------- setATCD_toView -------------------------------------------------------------
void DlgAtcd_txt::setATCD_toView(Atcd_Code* /*pAtcd_Code*/, Atcd_Element* atcd)
{ m_toViewOnly = TRUE;
  lbl_Titre->setText("<b>"+TR("Visualisation d'un antécédent (modifications non possibles)")+"</b>");
  //............. on rempli la viw avec les datas ..........................
  put_Atcd_Data_in_View(atcd);
  //............ mettre les champs etditable en read only ..................
  m_LineEdit_Libelle->setReadOnly(TRUE);
  m_LineEdit_Commentaire->setReadOnly(TRUE);
  m_LineEdit_Libelle_Long->setReadOnly(TRUE);
  cMaskedLineDateDebAtcd->setReadOnly(TRUE);
  cMaskedLineDateFinAtcd->setReadOnly(TRUE);
  //.............. on s'occupe des dates ........................
  groupBox_dates->show();
  cMaskedLineDateDebAtcd->setEnabled(FALSE);
  pushButtonDateDebDlg->setEnabled(FALSE);
  pushButtonDateDeb_Del->setEnabled(FALSE);

  cMaskedLineDateFinAtcd->setEnabled(FALSE);
  pushButtonDateFinDlg->setEnabled(FALSE);
  pushButtonDateFin_Del->setEnabled(FALSE);
  //.............. on s'occupe des combo ........................
  m_Combo_Family->setEnabled(FALSE);
  m_Combo_Genre->setEnabled(FALSE);

  //.............. on s'occupe de l'etat ALD SPORT ........................
  buttonGroup_Sport_ALD->show();
  radioButton_nonDefini->setEnabled(FALSE);
  radioButton_ALD->setEnabled(FALSE);
  radioButton_Sport->setEnabled(FALSE);
  radioButton_Vigilance->setEnabled(FALSE);
}

//--------------------------------- setToUpdate -------------------------------------------------------------
void DlgAtcd_txt::setToUpdate(bool b)
{ if (!b) {m_toViewOnly=TRUE; return;}
  m_toViewOnly=FALSE;
  lbl_Titre->setText("<b>"+TR("Modification d'un antécédent")+"</b>");
  //............ mettre les champs etditable en read only ..................
  m_LineEdit_Libelle->setReadOnly(FALSE);
  m_LineEdit_Commentaire->setReadOnly(FALSE);
  m_LineEdit_Libelle_Long->setReadOnly(FALSE);
  cMaskedLineDateDebAtcd->setReadOnly(FALSE);
  cMaskedLineDateFinAtcd->setReadOnly(FALSE);
  //.............. on s'ocupe des dates ........................
  groupBox_dates->show();
  cMaskedLineDateDebAtcd->setEnabled(TRUE);
  pushButtonDateDebDlg->setEnabled(TRUE);
  pushButtonDateDeb_Del->setEnabled(TRUE);

  cMaskedLineDateFinAtcd->setEnabled(TRUE);
  pushButtonDateFinDlg->setEnabled(TRUE);
  pushButtonDateFin_Del->setEnabled(TRUE);

  //.............. on s'ocupe des combo ........................
  m_Combo_Family->setEnabled(TRUE);
  m_Combo_Genre->setEnabled(TRUE);

  //.............. on s'ocupe de l'etat ALD SPORT ........................
  buttonGroup_Sport_ALD->show();
  radioButton_nonDefini->setEnabled(TRUE);
  radioButton_ALD->setEnabled(TRUE);
  radioButton_Sport->setEnabled(TRUE);
  radioButton_Vigilance->setEnabled(TRUE);
}

//--------------------------------- put_Atcd_Data_in_View -------------------------------------------------------------
void DlgAtcd_txt::put_Atcd_Data_in_View(Atcd_Element* atcd)
{m_LineEdit_Libelle->setText( atcd->m_Libelle );
 m_LineEdit_Commentaire->setText ( atcd->m_Commentaire );

 if ( atcd->m_DateDeb.isValid() )
     { setDateDeb( atcd->m_DateDeb );
     }
 if ( atcd->m_DateFin.isValid() )
     { setDateFin( atcd->m_DateFin );
     }

 setFamilyGenre(atcd->m_Rubrique.stripWhiteSpace());
 setSate_Ald_Sport(atcd->m_ald.stripWhiteSpace());
}

////////////////////////////////////////////////////////////////////////////
//////////////////////////   GESTION DATES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//--------------------------------- but_Valider_clicked -------------------------------------------------------------
void DlgAtcd_txt::but_Valider_clicked()
{ accept();
}

//--------------------------------- but_Annuler_clicked -------------------------------------------------------------
void DlgAtcd_txt::but_Annuler_clicked()
{ reject();
}

//--------------------------------- comboBox_Family_highlighted -------------------------------------------------------------
void DlgAtcd_txt::comboBox_Family_highlighted( const QString& section )
{m_ATCD_GenreList.clear();
 CGestIni::Param_GetList(m_structStr, section, "",  m_ATCD_GenreList , 1);
 m_ATCD_GenreList.prepend(section);
 m_Combo_Genre->clear();
 m_Combo_Genre->insertStringList( m_ATCD_GenreList );
}

//--------------------------------- combo_Family_activated -------------------------------------------------------------
void DlgAtcd_txt::combo_Family_activated( int )
{//m_Combo_Family->setCurrentItem(0);
}

//--------------------------------- combo_Genre_activated -------------------------------------------------------------
void DlgAtcd_txt::combo_Genre_activated( int )
{//m_Combo_Genre->setCurrentItem(0);
}



