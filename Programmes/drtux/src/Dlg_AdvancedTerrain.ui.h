/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

//------------------------------------------------ init ----------------------------------------------------------------------
void Dlg_AdvancedTerrain::init()
{if (G_pCApp->m_pCMedicaBase==0) G_pCApp->m_pCMedicaBase = new CMedicaBase;  if (G_pCApp->m_pCMedicaBase==0) return;
 comboBox_Allaitement->setCurrentItem (1);
 comboBox_UnitGrossesse->setCurrentItem(1);
 comboBox_Hemodialyse->setCurrentItem(1);   // insuffisant rénal
}

//------------------------------------------------ initDialog ----------------------------------------------------------------
//QString terrain = lineEdit_Terrain->text()+"$AG:"+lineEdit_AgeMin->text()   + ":" + lineEdit_AgeMax->text()   + ":" + "mois"+
//                                           +"$PS:"+lineEdit_PoidsMin->text() + ":" + lineEdit_PoidsMax->text() + ":" + "kg"+;

int Dlg_AdvancedTerrain::initDialog( QString &terrain )
{ QStringList varList = QStringList::split("$", terrain);
  for ( QStringList::Iterator it = varList.begin(); it != varList.end(); ++it )
      {QStringList sublist = QStringList::split (":", *it , TRUE);
       int n = sublist.count();
       if (n==4)
          {// " $AG::"          // age
           // " $PS::"          // poids
           // " $AL::"          // allaitement
           // " $CR::"          // creatininémie
           // " $GR::"          // grossesse
           // " $CL::"          // clearence de la créatine
           // " $BL::"          // bilirubinémie
           // " $FJ::"          // fraction d'ejection systolique
           //........................... insuffisance renale ..........................
           if ( (*it )[0]=='C')
              {if (*it=="CR::0:ml/mn"||*it=="CR:::" || *it=="CL:::")
                  {groupBox_Creat->hide();
                   textLabel_EntreClearence->hide();
                   textLabel_EtClearence->hide();
                   textLabel_UnitClearence->hide();
                   LineEdit_ClearenceMin->hide();
                   LineEdit_ClearenceMax->hide();
                   comboBox_Hemodialyse->setCurrentItem(0);   // hemodialysé
                  }
               if (sublist[0]=="CL")
	          {LineEdit_ClearenceMin->setText(sublist[1]);
                   LineEdit_ClearenceMax->setText(sublist[2]);
	          }
               else if (sublist[0]=="CR")
	          {LineEdit_CreatMin->setText(sublist[1]);
                   LineEdit_CreatMax->setText(sublist[2]);
	          }
              }
           //........................... reste des éléments du terrain ..........................
	   else if (sublist[0]=="AL")
	      {comboBox_Allaitement->setCurrentItem (0);
	      }
           else if (sublist[0]=="AG")
	      {int m = sublist[1].toInt();
               int a = m/12;
               LineEdit_AnMin->setText(QString::number(a));
               LineEdit_MoisMin->setText(QString::number( m - a*12));
               m = sublist[2].toInt();
               a = m/12;
               LineEdit_AnMax->setText(QString::number(a));
               LineEdit_MoisMax->setText(QString::number(m - a*12));
	      }
	   else if (sublist[0]=="PS")
	      {LineEdit_PoidsMin->setText(sublist[1]);
               LineEdit_PoidsMax->setText(sublist[2]);
	      }
	   else if (sublist[0]=="GR")
	      {if (*it == "GR:::")
                  {comboBox_UnitGrossesse->setCurrentItem(2);
                   textLabelGrossesseEntre->hide();
                   LineEdit_GrossesseMin->hide();
                   LineEdit_GrossesseMax->hide();
                   textLabel_GrossesseEt->hide();
                  }
               else
                  {textLabelGrossesseEntre->show();
                   LineEdit_GrossesseMin->show();
                   LineEdit_GrossesseMax->show();
                   textLabel_GrossesseEt->show();
                  }
               LineEdit_GrossesseMin->setText(sublist[1]);
               LineEdit_GrossesseMax->setText(sublist[2]);
               if (sublist[3].find("imestre")!=-1) comboBox_UnitGrossesse->setCurrentItem(0);
	      }

	   else if (sublist[0]=="BL")
	      {LineEdit_BiliMin->setText(sublist[1]);
               LineEdit_BiliMax->setText(sublist[2]);
	      }
           else if (sublist[0]=="FJ")
	      {LineEdit_EjectMin->setText(sublist[1]);
               LineEdit_EjectMax->setText(sublist[2]);
	      }
          } //endif (n==4)
      }
 connect( LineEdit_AnMin,         SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_MoisMin,       SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_AnMax,         SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_MoisMax,       SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_PoidsMin,      SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_PoidsMax,      SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_CreatMin,      SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_CreatMax,      SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_GrossesseMin,  SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_GrossesseMax,  SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_ClearenceMin,  SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_ClearenceMax,  SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_BiliMin,       SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_BiliMax,       SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_EjectMin,      SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 connect( LineEdit_EjectMax,      SIGNAL( textChanged(const QString&)) ,  this, SLOT(   UpdateHtmlView(const QString&)) );
 UpdateHtmlView( "" ) ;

 return 1;
}

//------------------------------------------------ UpdateHtmlView ----------------------------------------------------------------
void Dlg_AdvancedTerrain::UpdateHtmlView( const QString & )
{textBrowser_ResultView->setText(G_pCApp->m_pCMedicaBase->Medica_PosologieTerrainToHtml(dialogToTerrain()));
}

//------------------------------------------------ dialogToTerrain ----------------------------------------------------------------
QString Dlg_AdvancedTerrain::dialogToTerrain()
{QString result("");
 QString ageMin("");
 QString ageMax("");
 //......................... age ...........................................................................
 float fmin = LineEdit_AnMin->text().stripWhiteSpace().toFloat()*12 + LineEdit_MoisMin->text().stripWhiteSpace().toFloat();
 float fmax = LineEdit_AnMax->text().stripWhiteSpace().toFloat()*12 + LineEdit_MoisMax->text().stripWhiteSpace().toFloat();
 if (fmin != 0) ageMin = QString::number(fmin);
 if (fmax != 0) ageMax = QString::number(fmax);
 result += EDToVal("$AG:",ageMin,  ageMax, "mois");
 //......................... poids .........................................................................
 result += EDToVal("$PS:", LineEdit_PoidsMin->text().stripWhiteSpace(), LineEdit_PoidsMax->text().stripWhiteSpace(), "Kg");
 //......................... Creatininémie .................................................................
 result += EDToVal("$CR:", LineEdit_CreatMin->text().stripWhiteSpace(), LineEdit_CreatMax->text().stripWhiteSpace(), "mg/L");
 //......................... Grossesse .....................................................................
 if (comboBox_UnitGrossesse->currentItem()==2)           result += "$GR:::";
 else result += EDToVal("$GR:", LineEdit_GrossesseMin->text().stripWhiteSpace(), LineEdit_GrossesseMax->text().stripWhiteSpace(), comboBox_UnitGrossesse->currentText());
 //......................... Clearence de la Creatinine ....................................................
 if (comboBox_Hemodialyse->currentItem()==0)             result += "$CL:::";  // hémodialysé
 else result += EDToVal("$CL:", LineEdit_ClearenceMin->text().stripWhiteSpace(), LineEdit_ClearenceMax->text().stripWhiteSpace(), "ml/mn");
 //......................... Bilibirubinémie ...............................................................
 result += EDToVal("$BL:", LineEdit_BiliMin->text().stripWhiteSpace(),      LineEdit_BiliMax->text().stripWhiteSpace(),      "mg/L");
 //......................... Fraction d'ejection ...........................................................
 result += EDToVal("$FJ:", LineEdit_EjectMin->text().stripWhiteSpace(),     LineEdit_EjectMax->text().stripWhiteSpace(),     "%");
 //......................... allaitement ...........................................................
 if (comboBox_Allaitement->currentItem () == 0) result += "$AL:::";
 return result;
}

//------------------------------------------------ EDToVal ----------------------------------------------------------------
QString Dlg_AdvancedTerrain::EDToVal(const QString &var, QString textMin,  QString textMax, const QString &unit)
{ if (textMin.stripWhiteSpace().length()==0 && textMax.stripWhiteSpace().length()==0) return QString("");
  QString ret = var + textMin + ":"+ textMax + ":"+ unit;
  return  ret;
}

//------------------------------------------------ comboBox_Allaitement_activated ----------------------------------------------------------------
void Dlg_AdvancedTerrain::comboBox_Allaitement_activated( const QString &)
{UpdateHtmlView("");
}

//------------------------------------------------ comboBox_UnitGrossesse_activated ----------------------------------------------------------------
void Dlg_AdvancedTerrain::comboBox_UnitGrossesse_activated( const QString & )
{if (comboBox_UnitGrossesse->currentItem()==2)   // en cours
    {textLabelGrossesseEntre->hide();
     LineEdit_GrossesseMin->hide();
     LineEdit_GrossesseMax->hide();
     textLabel_GrossesseEt->hide();
    }
 else
    {textLabelGrossesseEntre->show();
      LineEdit_GrossesseMin->show();
      LineEdit_GrossesseMax->show();
      textLabel_GrossesseEt->show();
    }
 UpdateHtmlView("");
}

//------------------------------------------------ comboBox_Hemodialyse_activated ----------------------------------------------------------------
void Dlg_AdvancedTerrain::comboBox_Hemodialyse_activated( const QString & )
{if (comboBox_Hemodialyse->currentItem()==0)   // en cours)
    {groupBox_Creat->hide();
     textLabel_EntreClearence->hide();
     textLabel_EtClearence->hide();
     textLabel_UnitClearence->hide();
     LineEdit_ClearenceMin->hide();
     LineEdit_ClearenceMax->hide();
     comboBox_Hemodialyse->setCurrentItem(0);   // hemodialysé
    }
 else
    {groupBox_Creat->show();
     textLabel_EntreClearence->show();
     textLabel_EtClearence->show();
     textLabel_UnitClearence->show();
     LineEdit_ClearenceMin->show();
     LineEdit_ClearenceMax->show();
     comboBox_Hemodialyse->setCurrentItem(1);   // insuffisant rénal
    }
 UpdateHtmlView("");
}

//------------------------------------------------ pushButton_Ok_clicked ----------------------------------------------------------------
void Dlg_AdvancedTerrain::pushButton_Ok_clicked()
{accept();
}



