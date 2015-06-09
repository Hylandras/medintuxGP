/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "../Theme.h"
#include "C_Horloge.h"
//---------------------------------------- init() ---------------------------------------
void Dlg_Calendar::init()
{//................... Theme ............................................
 if (Theme::getPath() != "")
 {pushButtonYearMoins->setIconSet  ( Theme::getIcon("16x16/2leftarrow.png"));
  pushButtonMonthMoins->setIconSet ( Theme::getIcon("16x16/1leftarrow.png"));
  pushButtonMonthPlus->setIconSet  ( Theme::getIcon("16x16/1rightarrow.png"));
  pushButtonYearPlus->setIconSet   ( Theme::getIcon("16x16/2rightarrow.png"));
 }
 m_Mode = "days|debDate|endDate|hours";
 //................... bouton du menu des dates .........................
 m_pQPopupMenu    = 0;
 m_PopupListItem.append(tr("Ce jour à 8:00 h - 2 jours"));
 m_PopupListItem.append(tr("Ce jour à 8:00 h - 1 jour"));
 m_PopupListItem.append(tr("Ce jour à 8:00 h - 24 h"));
 m_PopupListItem.append(tr("Ce jour à 8:00 h"));
 m_PopupListItem.append(tr("Ce jour à 8:00 h + 24 h"));
 m_PopupListItem.append(tr("Ce jour à 8:00 h + 1 jour"));
 m_PopupListItem.append(tr("Ce jour à 8:00 h + 2 jours"));
 m_PopupListItem.append(tr("Ce mois à 0:00 h"));
 m_PopupListItem.append(tr("Ce mois à 0:00 h - 1 mois"));
 m_PopupListItem.append(tr("Cette année à 0:00 h"));
 m_PopupListItem.append(tr("Cette année à 0:00 h - 1 an"));
 pushButtonMenu->hide();

 //........................ reste de l'interface .........................
 QDate date       = QDate::currentDate();
 m_pMyDateTable   = new MyDateTable(this, date );
 connect( pushButtonMenu,  SIGNAL( clicked()),
          this,            SLOT(   Slot_pushButtonMenu_clicked())
        );

 Dlg_CalendarLayout->addWidget(m_pMyDateTable, 1, 0 );
 //.................... l'horloge ..................
 m_pC_Horloge = new C_Horloge(Theme::getPath(Theme::WithSeparator)+"Horloge/HorlogeCal.png", this, "horlogeCal" );
 if (m_pC_Horloge)
    {connect( timeEdit_Heure ,    SIGNAL(valueChanged            ( const QTime &  )),   m_pC_Horloge,   SLOT( setTime(const QTime & ) ) );
     connect( m_pC_Horloge ,      SIGNAL(Sign_HorlogeTimeChanged ( const QTime &  )),   timeEdit_Heure, SLOT( setTime(const QTime & ) ) );
     Dlg_CalendarLayout->addWidget(m_pC_Horloge , 1, 1 );
    }


 resize( (200/7)*7+6+250, (184/7)*7 );
 connect( m_pMyDateTable, SIGNAL( tableDoubleClicked()),
          this,           SLOT(   Slot_OnTableDoubleClicked())
        );
 connect( m_pMyDateTable, SIGNAL( tableClicked()),
          this,           SLOT(   Slot_OnTableClicked())
        );
 connect( m_pMyDateTable, SIGNAL( dateChanged(QDate)),
          this,           SLOT(   Slot_OndateChanged(QDate))
        );

 connect( cMaskedLineEditDate, SIGNAL( textChanged( const QString&)),
          this,                SLOT(   Slot_OnTextDateChanged( const QString&))
        );
 connect( cMaskedLineEditDate, SIGNAL( returnPressed()),
          this,                SLOT(   Slot_OnReturnPressed())
        );

 AdjustInterfaceToDate(date);
 cMaskedLineEditDate->setFocus();
 cMaskedLineEditDate->selectAll();
 hideHeure();
 //setMenuPreSelectDate(m_PopupListItem);
}
//---------------------------------------- DateMenu_setIcon ---------------------------------------
void Dlg_Calendar::DateMenu_setIcon(const QPixmap &rQPixmap)
{pushButtonMenu->setIconSet ( QIconSet (rQPixmap)  );
}
//---------------------------------------- Month_setIcon ---------------------------------------
void Dlg_Calendar::Month_setIcon(const QPixmap &moinsQPixmap, const QPixmap &plusQPixmap)
{pushButtonMonthMoins->setIconSet ( QIconSet (moinsQPixmap) );
 pushButtonMonthPlus->setIconSet  ( QIconSet (plusQPixmap)  );
}
//---------------------------------------- Year_setIcon ---------------------------------------
void Dlg_Calendar::Year_setIcon(const QPixmap &moinsQPixmap, const QPixmap &plusQPixmap)
{pushButtonYearMoins->setIconSet ( QIconSet (moinsQPixmap) );
 pushButtonYearPlus->setIconSet  ( QIconSet (plusQPixmap)  );
}

//---------------------------------------- setDateDeb ---------------------------------------
void Dlg_Calendar::setDateDeb(QDate* pdate)
{m_pMyDateTable->setDateDeb( pdate );
}

//---------------------------------------- setMode ---------------------------------------
void Dlg_Calendar::setMode(const QString &mode)
{m_Mode = mode;   // days|debDate|endDate|hours
 if (mode.find("hours")==-1) hideHeure();
 //if (mode.find("debDate")==-1 || mode.find("endDate")==-1) hideHeure();
}

//---------------------------------------- hideHeure ---------------------------------------
void Dlg_Calendar::hideHeure()
{textLabel_Heure->hide();
 timeEdit_Heure->hide();
 if (m_pC_Horloge) m_pC_Horloge->hide();

}

//---------------------------------------- setDateTimeNatural_jjMMyyyy_Order ---------------------------------------
int Dlg_Calendar::setDateTimeNatural_jjMMyyyy_Order(const QString & dateTime )
{ int i= 0, len = dateTime.length();
  QString str = "";
  for (i= 0; i<len; ++i)
      {if (dateTime.at(i)>='0' && dateTime.at(i)<='9') str += dateTime.at(i);
      }
  QString date  = str.left(8);
  int jour  = date.mid(0,2).toInt();
  int mois  = date.mid(2,2).toInt();
  int annee = date.mid(4,4).toInt();
  if (QDate::isValid ( annee, mois, jour ) )
    {setDate(QDate(annee, mois, jour));
     setHeure( str.mid(8) );
     return 1;
    }
  setHeure( str.mid(8) );
  return 0;
}

//---------------------------------------- setDateTimeNumeric_yyyyMMjj_Order ---------------------------------------
int Dlg_Calendar::setDateTimeNumeric_yyyyMMjj_Order(const QString & dateTime )
{ int i= 0, len = dateTime.length();
  QString str = "";
  for (i= 0; i<len; ++i)
      {if (dateTime.at(i)>='0' && dateTime.at(i)<='9') str += dateTime.at(i);
      }
  QString date  = str.left(8);
  int jour  = date.mid(6,2).toInt();
  int mois  = date.mid(4,2).toInt();
  int annee = date.mid(0,4).toInt();
  if (QDate::isValid ( annee, mois, jour ) )
    {setDate(QDate(annee, mois, jour));
     setHeure( str.mid(8) );
     return 1;
    }
  setHeure( str.mid(8) );
  return 0;
}

//---------------------------------------- setHeure ---------------------------------------
void Dlg_Calendar::setHeure(const QString & heure )
{ int h=0; int m=0; int s=0; int ms=0; int i= 0, len = heure.length();
  QString str = "";
  for (i= 0; i<len; ++i)
      {if (heure.at(i)>='0' && heure.at(i)<='9') str += heure.at(i);
      }
  len = str.length();
  if (len>=2) h  = str.mid(0,2).toInt();  // hh
  if (len>=4) m  = str.mid(2,2).toInt();  // hhmm
  if (len>=6) s  = str.mid(4,2).toInt();  // hhmmss
  if (len>=9) ms = str.mid(6).toInt();    // hhmmss
  setHeure ( h,  m,  s,  ms );
}

//---------------------------------------- setHeure ---------------------------------------
void Dlg_Calendar::setHeure(int h, int m, int s, int ms )
{ QTime qt = QTime( h,  m,  s,  ms);
  setHeure (qt );
  if (m_Mode.find("hours") == -1) 
     {hideHeure();
     }
  else
     {textLabel_Heure->show();
      timeEdit_Heure->show();
      if (m_pC_Horloge) m_pC_Horloge->show();
     }
}
//---------------------------------------- setHeure ---------------------------------------
void Dlg_Calendar::setHeure(const QTime & time )
{ timeEdit_Heure->setTime ( time );
}
//---------------------------------------- SelectDateAll ---------------------------------------
void Dlg_Calendar::SelectDateAll()
{cMaskedLineEditDate->selectAll();
}

//---------------------------------------- Slot_OnReturnPressed ---------------------------------------
void Dlg_Calendar::Slot_OnReturnPressed()
{accept();
}

//---------------------------------------- Slot_OnTableDoubleClicked ---------------------------------------
void Dlg_Calendar::Slot_OnTableDoubleClicked()
{accept();
}
//---------------------------------------- Slot_OnTableDoubleClicked ---------------------------------------
void Dlg_Calendar::Slot_OnTableClicked()
{accept();
}
//---------------------------------------- Slot_OnTextDateChanged ---------------------------------------
void Dlg_Calendar::Slot_OnTextDateChanged( const QString &)
{if (cMaskedLineEditDate->text().length()>=8)
    {disconnect( cMaskedLineEditDate, SIGNAL(textChanged(const QString &)), 0, 0 );
     QString text = cMaskedLineEditDate->text();
     int jour, mois, annee;
     jour  = text.mid(0,2).toInt();
     mois  = text.mid(2,2).toInt();
     annee = text.mid(4,4).toInt();
     if (QDate::isValid ( annee, mois, jour ) )
        {setDate(QDate(annee, mois, jour));
        }
     else
        {QMessageBox::warning ( this, tr("Erreur de saisie dans la date"),
                                      tr("Erreur de saisie de la date\n"
                                         "données invalides"),
                                         1, 0, 0 );
	 cMaskedLineEditDate->setText(getDate().toString("ddMMyyyy"));    // retablir ancienne valeur
        }
     connect(    cMaskedLineEditDate, SIGNAL(textChanged( const QString&)),  this, SLOT(Slot_OnTextDateChanged( const QString&)) );
    }
}

//---------------------------------------- setDate ------------------------------------------------------------
void Dlg_Calendar::setDate(const QDate& date)
{m_pMyDateTable->setDate( date);
}

//---------------------------------------- Slot_OndateChanged ---------------------------------------
void Dlg_Calendar::Slot_OndateChanged(QDate date)
{AdjustInterfaceToDate(date);
}

//---------------------------------------- AdjustInterfaceToDate ------------------------------------------
void Dlg_Calendar::AdjustInterfaceToDate(const QDate &date)
{cMaskedLineEditDate->setText(date.toString("ddMMyyyy"));
 comboBoxMois->setCurrentItem ( date.month () -1 );
 setComboAnOnDate(date);
}

//---------------------------------------- getDate --------------------------------------------------------
QDate Dlg_Calendar::getDate()
{return m_pMyDateTable->getDate();
}
//---------------------------------------- getDate --------------------------------------------------------
QDateTime Dlg_Calendar::getDateTime()
{return QDateTime (m_pMyDateTable->getDate(),timeEdit_Heure->time ());
}
//---------------------------------------- get_SQL_DateTime --------------------------------------------------------
QString Dlg_Calendar::get_SQL_DateTime()
{return m_pMyDateTable->getDate().toString("yyyy-MM-dd")+"T"+timeEdit_Heure->time().toString("hh:mm:ss");
}
//---------------------------------------- pushButtonYearPlus_clicked ---------------------------------------
void Dlg_Calendar::pushButtonYearPlus_clicked()
{setDate(getDate().addYears(1));
}

//---------------------------------------- pushButtonMonthPlus_clicked ---------------------------------------
void Dlg_Calendar::pushButtonMonthPlus_clicked()
{setDate(getDate().addMonths(1));
}

//---------------------------------------- pushButtonYearMoins_clicked ---------------------------------------
void Dlg_Calendar::pushButtonYearMoins_clicked()
{setDate(getDate().addYears(-1));
}

//---------------------------------------- pushButtonMonthMoins_clicked ---------------------------------------
void Dlg_Calendar::pushButtonMonthMoins_clicked()
{setDate(getDate().addMonths(-1));
}


//---------------------------------------- comboBoxMois_activated ---------------------------------------
void Dlg_Calendar::comboBoxMois_activated( const QString & )
{int m      = comboBoxMois->currentItem() + 1;
 QDate date = getDate();
 if (date.month() != m)
    {disconnect( comboBoxMois, SIGNAL(activated(const QString &)), 0, 0 );
     setDate( QDate(date.year(), m, date.day()));
     connect( comboBoxMois, SIGNAL(activated(const QString &)),   this, SLOT(comboBoxMois_activated(const QString &)) );
    }
}

//---------------------------------------- comboBoxAn_activated ---------------------------------------
void Dlg_Calendar::comboBoxAn_activated( const QString &an )
{comboBoxAn_textChanged( an );
}

//---------------------------------------- comboBoxAn_textChanged ---------------------------------------
void Dlg_Calendar::comboBoxAn_textChanged( const QString &an )
{if (an.length()>=4)
    {disconnect( comboBoxAn, SIGNAL(textChanged(const QString &)), 0, 0 );
     disconnect( comboBoxAn, SIGNAL(activated(const QString &)), 0, 0 );
     QDate date = getDate();
     int y = an.toInt();
     if (y<=1753||y>=8000)
        {QMessageBox::warning ( this, tr("Erreur de saisie dans la date"),
                                      tr("Erreur de saisie de l'année\n"
                                         "Minimum possible: 1753 Maximum: 8000"),
                                         1, 0, 0 );
	 setDate(QDate::currentDate());
        }
    else
       {setDate( QDate(y,date.month(),date.day()));
       }
    connect( comboBoxAn, SIGNAL(textChanged(const QString &)), this, SLOT(comboBoxAn_textChanged(const QString &)) );
    connect( comboBoxAn, SIGNAL(activated(const QString &)),   this, SLOT(comboBoxAn_activated(const QString &)) );
   }
}

//---------------------------------------- setComboAnOnDate ---------------------------------------
void Dlg_Calendar::setComboAnOnDate(const QDate &date)
{setComboAnOnDate_Offset(date, 10, 20);
}

//---------------------------------------- setComboAnOnDate_Offset ---------------------------------------
void Dlg_Calendar::setComboAnOnDate_Offset(const QDate &date, int neg_Offset, int pos_Offset)
{disconnect( comboBoxMois, SIGNAL(activated(const QString &)), 0, 0 );
 disconnect( comboBoxAn,   SIGNAL(textChanged(const QString &)), 0, 0 );
 disconnect( comboBoxAn,   SIGNAL(activated(const QString &)), 0, 0 );
 comboBoxAn->clear();
 int an  = date.year ();
 int deb = an - neg_Offset;
 int end = an + pos_Offset+1;
 for (int i=deb; i<end;++i)
   {QString txt = QString("0000") + QString::number(i);
    txt = txt.mid(txt.length()-4,4);
    comboBoxAn->insertItem ( txt );
   }
 comboBoxAn->setCurrentItem(neg_Offset);
 connect( comboBoxMois, SIGNAL(activated(const QString &)),   this, SLOT(comboBoxMois_activated(const QString &)) );
 connect( comboBoxAn,   SIGNAL(textChanged(const QString &)), this, SLOT(comboBoxAn_textChanged(const QString &)) );
 connect( comboBoxAn,   SIGNAL(activated(const QString &)),   this, SLOT(comboBoxAn_activated(const QString &)) );
}

//---------------------------------------- setPreselectDateOn ---------------------------------------
void Dlg_Calendar::setMenuPreselectDateOn()
{pushButtonMenu->show();
 textLabel_Heure->show();
 timeEdit_Heure->show();
 if (m_pC_Horloge) m_pC_Horloge->show();
}

//---------------------------------------- setMenuPreselectDateOff ---------------------------------------
void Dlg_Calendar::setMenuPreselectDateOff()
{pushButtonMenu->hide();
textLabel_Heure->hide();
timeEdit_Heure->hide();
if (m_pC_Horloge) m_pC_Horloge->hide();
}
//---------------------------------------- setMenuPreSelectDate ---------------------------------------
void Dlg_Calendar::setMenuPreSelectDate(const QStringList &itemList)
{m_PopupListItem = itemList;
 setMenuPreselectDateOn();
}

//--------------------------------- Slot_pushButtonMenu_clicked ----------------------------------------------------------------------
// m_PopupListItem.append(tr("Ce jour à 8:00 h - 2 jours"));
// m_PopupListItem.append(tr("Ce jour à 8:00 h - 1 jour"));
// m_PopupListItem.append(tr("Ce jour à 8:00 h - 24 h"));
// m_PopupListItem.append(tr("Ce jour à 8:00 h"));
// m_PopupListItem.append(tr("Ce jour à 8:00 h + 24 h"));
// m_PopupListItem.append(tr("Ce jour à 8:00 h + 1 jour"));
// m_PopupListItem.append(tr("Ce jour à 8:00 h + 2 jours"));
// m_PopupListItem.append(tr("Ce mois à 0:00 h"));
// m_PopupListItem.append(tr("Ce mois à 0:00 h - 1 mois"));
// m_PopupListItem.append(tr("Cette année à 0:00 h"));
// m_PopupListItem.append(tr("Cette année à 0:00 h - 1 an"));

void Dlg_Calendar::Slot_pushButtonMenu_clicked()
{QString itemSelect = DoPopupList(m_PopupListItem); // "Ce jour à 8:00 h - 2 jours"
 if (itemSelect.length()==0)       return;
 int pos_deb = itemSelect.find(tr("à"));
 if (pos_deb==-1)                  return;
 QString deb_day  = itemSelect.left(pos_deb);
 itemSelect = itemSelect.mid(pos_deb+1);            // " 8:00 h - 2 jours"
 pos_deb = itemSelect.find(tr("h"));
 if (pos_deb==-1)                  return;
 QString deb_heure = itemSelect.left(pos_deb).stripWhiteSpace();

 itemSelect        = itemSelect.mid(pos_deb+1);     // " - 2 jours"
 QString signToAdd = "";
 if      ( (pos_deb=itemSelect.find(tr("+"))) !=-1 )        { signToAdd = "+"; itemSelect = itemSelect.mid(pos_deb+1);}        // " 2 jours"
 else if ( (pos_deb=itemSelect.find(tr("-"))) !=-1 )        { signToAdd = "-"; itemSelect = itemSelect.mid(pos_deb+1);}        // " 2 jours"

 char            *pt = (char*)(const char*) itemSelect;
 QString  quantToAdd = "0";
 if (pt) while ( *pt ) {if (*pt>='0'&&*pt<='9') {quantToAdd += *pt;} ++pt;}  // eliminer tout ce qui n'est pas numérique '-' et autre '/'
 QDate date = QDate::currentDate();
 int      y = date.year ();
 int      m = date.month ();
 int      d = date.day ();

 if      ( deb_day.find(tr("mois")) != -1) {d=1;     }
 else if ( deb_day.find(tr("anné")) != -1) {d=1; m=1;}
 date.setYMD (  y,  m,  d );

 int     qToAdd = quantToAdd.toInt();
 if ( signToAdd == "-" ) qToAdd = -qToAdd;
 if      ( itemSelect.find(tr("jour")) != -1) {date = date.addDays(qToAdd);   }
 else if ( itemSelect.find(tr("mois")) != -1) {date = date.addMonths(qToAdd); }
 else if ( itemSelect.find(tr("an"))   != -1) {date = date.addYears(qToAdd);  }

 int   hr = 0;
 int   mn = 0;
 pos_deb    = deb_heure.find(tr(":"));
 if (pos_deb == -1) {hr = deb_heure.toInt();}
 else               {hr = deb_heure.left(pos_deb).toInt(); mn = deb_heure.mid(pos_deb+1).toInt();}

 QTime time (hr, mn);
 QDateTime dateTime ( date ,time );
 if      ( itemSelect.find(tr("h")) != -1) {dateTime = dateTime.addSecs(qToAdd*3600); }
 setDateTimeNatural_jjMMyyyy_Order( dateTime.toString("ddMMyyyyhhmm") );
}

//--------------------------------- DoPopupList ----------------------------------------------------------------------
QString Dlg_Calendar::DoPopupList(QStringList &list)
{   m_CurrentPopupItem = "";
    m_RetPopupItem     = "";
    if (m_pQPopupMenu) return m_RetPopupItem;
    m_pQPopupMenu      = new QPopupMenu(0, "MyPopupMenu" );
    if (m_pQPopupMenu==0)       return m_RetPopupItem;
    //............................... chercher une zone de liste deroulante (bleue) .....................
    for ( QStringList::Iterator it =list.begin(); it != list.end(); ++it )
    {if ( (*(it)).left(4)=="----") m_pQPopupMenu->insertSeparator ();
     else                          m_pQPopupMenu->insertItem( *(it),    this, SLOT( GetPopupItem()),  CTRL+Key_L );
    }
    //................. connecter le menu .................................................................
    connect( m_pQPopupMenu, SIGNAL( activated( int)),
             this,          SLOT(OnMenuActivated( int))
             );

    m_pQPopupMenu->exec(QCursor::pos());
    delete  m_pQPopupMenu;
    m_pQPopupMenu = 0;
    return  m_RetPopupItem;
}

//--------------------------------- OnMenuActivated  -------------------------------------------------------------
void Dlg_Calendar::OnMenuActivated (int id)
{if (id != -1 && m_pQPopupMenu)
    {m_CurrentPopupItem = m_pQPopupMenu->text(id);
    }
}

//--------------------------------- GetPopupItem  -------------------------------------------------------------
void Dlg_Calendar::GetPopupItem ()
{m_RetPopupItem  = m_CurrentPopupItem;
}


