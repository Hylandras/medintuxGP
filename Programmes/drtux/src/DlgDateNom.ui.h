/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "CApp.h"
#include "../../MedinTuxTools/CMaskedLineEdit.h"
#include "ui/Dlg_Calendar.h"
#include "../../MedinTuxTools/Theme.h"  // Gestion du thème de l'appli

#include <qdatetime.h>
#include <qdatetimeedit.h> 
//------------------------------- initDialog -------------------------------------
void DlgDateNom::initDialog(QString *date_time, QString *nom, QString *subType)  //1999-10-08 00:00:00
{*date_time  += "                   "; 
 QString date =   date_time->mid(8,2)  + date_time->mid(5,2)  + date_time->mid(0,4);
 QString time =   date_time->mid(11,2) + date_time->mid(14,2);
 cMaskedLineEditDate->setText(date);
 cMaskedLineEditTime->setText(time);
 lineEditNom->setText(*nom);
 lineEdit_SubType->setText(*subType);
 m_pDateTime = date_time;
 m_pNom      = nom;
 m_pSubType  = subType;
 pushButtonDateDlg->setIconSet( Theme::getIconSet("16x16/calendrierDeb.png"));
 connect( pushButtonDateDlg, SIGNAL( clicked()),  this,  SLOT  ( Slot_pushButtonDateDlg())  );
 lineEditNom->setFocus();
}

//------------------------------- Slot_pushButtonDateDlg -------------------------------------
void DlgDateNom::Slot_pushButtonDateDlg()
{       
        Dlg_Calendar          *dlg  = new Dlg_Calendar(this, "Calendar_Dial", FALSE);
        if (dlg ==0)                                                   return;
        dlg->setCaption(tr("Date et heure de la rubrique")); //   dlg->setIcon ( Theme::getIcon("16x16/calendrierFin.png"));}
        if ( ! dlg->setDateTimeNatural_jjMMyyyy_Order(cMaskedLineEditDate->text() + cMaskedLineEditTime->text() + ":00" ))
           {dlg->setDate  (QDate::currentDate());
            dlg->setHeure (QTime::currentTime());
           }
        if (dlg->exec()== QDialog::Accepted )
           {cMaskedLineEditDate->setText(dlg->getDate().toString("ddMMyyyy"));
            cMaskedLineEditTime->setText(dlg->timeEdit_Heure->time().toString("hh:mm"));
           }
        delete dlg;
}

//------------------------------- pushButtonOK_clicked -------------------------------------
void DlgDateNom::pushButtonOK_clicked()
{ QString date = cMaskedLineEditDate->text();         // 04122008
          date = date.mid(4,4)+"-"+ date.mid(2,2)+"-"+date.mid(0,2) + " " + cMaskedLineEditTime->getMixText() + ":00";
  QDateTime dt = QDateTime::fromString ( date, Qt::ISODate );

  if ( dt.isValid () )  *m_pDateTime = date;
  else                   G_pCApp->CouCou(tr("Date et heure :\n%1\nnon positionnées car la saisie est invalide").arg(cMaskedLineEditDate->getMixText()+
                                                                                                                     " "                              +
                                                                                                                     cMaskedLineEditTime->getMixText()));
  *m_pNom      = lineEditNom->text();
  *m_pSubType  = lineEdit_SubType->text();
 accept();
}
