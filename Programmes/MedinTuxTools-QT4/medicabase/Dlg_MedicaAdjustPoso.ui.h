/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))
#include <math.h>

//-------------------------------------- init -------------------------------------------------------------
void Dlg_MedicaAdjustPoso::init()
{
 lineEdit_QMax_UP2->hide();
 lineEdit_QMin_UP2->hide();
 lineEdit_a_UP2->hide();
 textLabelUnite_UP2->hide();
 slider_VarPoso->hide();
 pushButton_NextPoso->hide();
 lineEdit_Poids->hide();
 textLabelPoids->hide();
 textLabelPdsUnit->hide();
 lineEdit_Taille->hide();
 textLabelTaille->hide();
 textLabelTailleUnit->hide();
 textLabelMaxPosoAlert->hide();
 frameIco1->hide();
 frameIco2->hide();
 frameIco3->hide();
 frameIco4->hide();
}

//-------------------------------------- initDialog -------------------------------------------------------
/*
    //......................... données structurelles .............................................
    //                          provenant de la base médicamenteuse
    QString       m_MEDICA_POSO_CIP;
    QString       m_MEDICA_POSO_PK;
    QString       m_MEDICA_POSO_LIBELLE;
    QString       m_MEDICA_POSO_AGE_MIN;
    QString       m_MEDICA_POSO_AGE_MAX;
    QString       m_MEDICA_POSO_SEXE;
    QString       m_MEDICA_POSO_TERRAIN;
    QString       m_MEDICA_POSO_POIDS_MIN;
    QString       m_MEDICA_POSO_POIDS_MAX;
    QString       m_MEDICA_POSO_DOSE_MIN;
    QString       m_MEDICA_POSO_DOSE_MAX;
    QString       m_MEDICA_POSO_DOSE_UNIT;
    QString       m_MEDICA_POSO_FACT_CORP_QU;
    QString       m_MEDICA_POSO_FACT_CORP_UNIT;
    QString       m_MEDICA_POSO_PERIODE;
    QString       m_MEDICA_POSO_EQUI_COEFF;
    QString       m_MEDICA_POSO_UNIT_COEFF;
    QString       m_MEDICA_POSO_NB_PRISES;           // nombre de prises conseillées
    QString       m_MEDICA_POSO_REPART_PRISES;       // description structurée de la répartition des prises
    QString       m_MEDICA_POSO_DIVERS;
    QString       m_Note;                            // commentaire concernant cette posologie
    //......................... données fonctionnelles .............................................
    //                          non dépendantes de la base médicamenteuse mais
    //                          de la prescription
    int           m_PosoAdjust                      // ajustement par rapport à fourchette de doses initiales conseillées de 0 à 99 %
    int           m_NumOrdre;                       // numero d'ordre du produit dans l'ordo ou la liste (utile pour affichage)
    QString       m_PosoTextuelle;                  // posologie manuelle ajoutée par l'utilisateur (complément textuel)
    int           m_ALD;                            // à 0 si pas en rapport avec longue maladie
    POSO_TYPE     m_PosoType;                       // CPosologie::Nothing  CPosologie::Textuelle  CPosologie::Structuree  CPosologie::AllMode
    float         m_Duree;                          // durée de la prescription en secondes
    int           m_IdTable;                        // indice de la table d'index
*/

void Dlg_MedicaAdjustPoso::initDialog( CPosologie &cPosologieIn,
                                      // CMDI_Terrain *pCMDI_Terrain,
                                       QString *pPoids,
                                       QString *pTaille)
{if (G_pCApp->m_pCMedicaBase==0) G_pCApp->m_pCMedicaBase = new CMedicaBase; if (G_pCApp->m_pCMedicaBase==0) return;
 m_pPoids         = pPoids;
 m_pTaille        = pTaille;
 //m_pCMDI_Terrain  = pCMDI_Terrain;
 m_PosoIn         = cPosologieIn;
 m_FactCorpUnit   = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_AD_TBL_NAME, m_PosoIn.m_MEDICA_POSO_FACT_CORP_UNIT);
 m_ProdName       = G_pCApp->m_pCMedicaBase->Medica_GetMedicamentNameByCIP(m_PosoIn.m_MEDICA_POSO_CIP);
 //................................ regler secabilite avant poids taille .......................................................
 if (m_PosoIn.m_MEDICA_POSO_SECABILITE.length()==0)
    {m_PosoIn.m_MEDICA_POSO_SECABILITE = G_pCApp->m_pCMedicaBase->Medica_GetSecabilite( m_PosoIn.m_MEDICA_POSO_CIP);
    }
 m_Secabilite =  m_PosoIn.m_MEDICA_POSO_SECABILITE.toInt();
 radioButtonSecable1->setChecked(FALSE);
 radioButtonSecable2->setChecked(FALSE);
 radioButtonSecable4->setChecked(FALSE);
 switch(m_Secabilite)
   {default:
    case 1: radioButtonSecable1->setChecked(TRUE); break;
    case 2: radioButtonSecable2->setChecked(TRUE); break;
    case 4: radioButtonSecable4->setChecked(TRUE); break;
   }
 //................................... poids taille .............................................................................
 //...................... connecter après pour eviter appels à lineEdit_Poids_textChanged() ......................................
 connect( lineEdit_Poids,  SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Poids_textChanged(const QString&) ) );
 connect( lineEdit_Taille, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Taille_textChanged(const QString&) ) );
 /*
 if (m_pCMDI_Terrain)
    {if (m_pPoids->length()<=0)  *m_pPoids  =   m_pCMDI_Terrain->GetPoids()  ;                  // cela permet de sauver le poids
     if (m_pTaille->length()<=0) *m_pTaille =   m_pCMDI_Terrain->GetTaille() ;
    }
*/
 if (m_pPoids->toFloat()>0)    lineEdit_Poids->setText(   *m_pPoids   );
 else                          lineEdit_Poids->setText(   ""   );
 if (m_pTaille->toFloat()>0)   lineEdit_Taille->setText(  *m_pTaille  );
 else                          lineEdit_Taille->setText(  ""  );
 //.......................... forme de l'unite de prise 1 ................................................
 //                           (affichée au dessus du slider)
 lineEdit_QMin_UP1->setText(m_PosoIn.m_MEDICA_POSO_DOSE_MIN);

 if (m_PosoIn.m_MEDICA_POSO_DOSE_MAX != "0")
    {textLabelUnite_UP1->setText(G_pCApp->m_pCMedicaBase->Medica_PosologieUP_UnitToHtml(m_PosoIn.m_MEDICA_POSO_DOSE_UNIT , m_PosoIn.m_MEDICA_POSO_DOSE_MAX)+
                                 G_pCApp->m_pCMedicaBase->Medica_PosologieFactCorpToHtml(m_PosoIn));
     lineEdit_QMax_UP1->setText(m_PosoIn.m_MEDICA_POSO_DOSE_MAX);
     lineEdit_QMax_UP1->show();
     pushButton_A->show();
     slider_VarPoso->show();
    }
 else
    {textLabelUnite_UP1->setText(G_pCApp->m_pCMedicaBase->Medica_PosologieUP_UnitToHtml(m_PosoIn.m_MEDICA_POSO_DOSE_UNIT , m_PosoIn.m_MEDICA_POSO_DOSE_MIN)+
                                 G_pCApp->m_pCMedicaBase->Medica_PosologieFactCorpToHtml(m_PosoIn));
     lineEdit_QMax_UP1->hide();
     pushButton_A->hide();
    }
 groupBox_UP->setTitle (tr("Nombre d'unités de prise ") + G_pCApp->m_pCMedicaBase->Medica_PosologiePeriodeToHtml(m_PosoIn.m_MEDICA_POSO_PERIODE));

 //.......................... forme de l'unite de prise 2 ................................................
 //                           (affichée au dessous du slider)
 if (m_PosoIn.m_MEDICA_POSO_EQUI_COEFF != "0")
    {SetSecondForm( );
    }
 //.................................. affichage du poids et taille si necessaire ..........................
 if (m_PosoIn.m_MEDICA_POSO_FACT_CORP_QU != "0")
    {lineEdit_Poids->show();
     textLabelPoids->show();
     textLabelPdsUnit->show();
     if (m_FactCorpUnit=="m2")
        {lineEdit_Taille->show();
         textLabelTaille->show();
         textLabelTailleUnit->show();
        }
    }


 //..................... positionner le comboBox sur la bonne repartition de  prise ........................................
 SetComboRepartOnValue(m_PosoIn.m_MEDICA_POSO_NB_PRISES.toInt());

 //...................... afficher si dispo du maximum possible ............................................................
 if (m_PosoIn.m_MEDICA_POSO_DOSE_LIMIT_MAX.length() && m_PosoIn.m_MEDICA_POSO_DOSE_LIMIT_MAX != "0" )
    {textLabelMaxPosoAlert->setText(tr("Dose Maximum possible: ")         +
                                    m_PosoIn.m_MEDICA_POSO_DOSE_LIMIT_MAX + " " +
                                    textLabelUnite_UP1->text()            + " " +
                                    G_pCApp->m_pCMedicaBase->Medica_PosologiePeriodeToHtml(m_PosoIn.m_MEDICA_POSO_PERIODE));
     textLabelMaxPosoAlert->show();
    }
 if (m_PosoIn.m_PosoTextuelle.length())
    {
     textEditNote->setText(m_PosoIn.m_PosoTextuelle);
    }
 else
    {if (m_PosoIn.m_MEDICA_POSO_DIVERS.left(2)=="Ok")            m_PosoIn.m_MEDICA_POSO_DIVERS="";
     else if (m_PosoIn.m_MEDICA_POSO_DIVERS.left(2)=="No" &&
              m_PosoIn.m_MEDICA_POSO_DIVERS.find("E:")!=-1)      m_PosoIn.m_MEDICA_POSO_DIVERS="";
     textEditNote->setText(m_PosoIn.m_MEDICA_POSO_DIVERS);
    }
 //..................... positionner ajustement de la dose .................................................................
 slider_VarPoso->setValue ( abs (m_PosoIn.m_PosoAdjust)-1 ); // m_PosoAdjust 1 à 100  slider_VarPoso 0 à 99
 //..................... Poso ajustée au slider ou DE à DE affiché .........................................................
 if (cPosologieIn.m_PosoAdjust < 0)   // il faut tester cPosologieIn car m_PosoIn a été mal positionné lors d'une onnexion
    {slider_VarPoso->setEnabled(FALSE);
     pushButton_A->toggle ();
    }
 //..................... afficher le resumé dans la vue html ...............................................................
 InterfaceToPoso(m_PosoIn);
 textBrowser_Result->setText(G_pCApp->m_pCMedicaBase->Medica_PosologieToPrescriptionHtml(  m_PosoIn                ,
                                                                                       lineEdit_Poids->text()  ,
                                                                                       lineEdit_Taille->text() ,
                                                                                       -1                      ,
                                                                                       m_ProdName              ,
                                                                                       m_FactCorpUnit
                                                                                    )
                           );
  //..................... positionner durée de traitement ...................................................................
 QString nb , unit;
 if (CGestIni::Param_ReadParam(G_pCApp->m_pCMedicaBase->m_IniParam, "Duree", "Duree par defaut", &nb)   !=0 ) nb   = "8";
 if (CGestIni::Param_ReadParam(G_pCApp->m_pCMedicaBase->m_IniParam, "Duree", "DureeUnite",       &unit) !=0 ) unit = "Jours";
 nb += " " + unit;
 G_pCApp->m_pCMedicaBase->Medica_initComboDuree(comboBoxPendant);
 G_pCApp->m_pCMedicaBase->Medica_SetComboOnDuree(comboBoxPendant, cPosologieIn.m_MEDICA_POSO_PENDANT, nb);
}

//------------------------------------------- initComboDuree --------------------------------------------------------------
void Dlg_MedicaAdjustPoso::initComboDuree()
{
}

//-------------------------------------- InterfaceToPoso --------------------------------------------------------------
void Dlg_MedicaAdjustPoso::InterfaceToPoso(CPosologie &poso)
{QString popRet                               = comboBoxPendant->currentText();
 popRet                                       = popRet.replace(tr("un"),"1");
 poso.m_MEDICA_POSO_PENDANT                   = QString::number(G_pCApp->m_pCMedicaBase->Medica_dureeMenuTextToSecond(popRet));
 poso.m_MEDICA_POSO_SECABILITE                = QString::number(m_Secabilite);
 if (pushButton_A->state () == QButton::On )  poso.m_PosoAdjust = -slider_VarPoso->value()-1; // m_PosoAdjust 1 à 100  slider_VarPoso 0 à 99
 else                                         poso.m_PosoAdjust =  slider_VarPoso->value()+1; // m_PosoAdjust 1 à 100  slider_VarPoso 0 à 99
}


//-------------------------------------- SetComboRepartOnValue -------------------------------------------------------
void Dlg_MedicaAdjustPoso::SetComboRepartOnValue(int nb_prises)
{int nb = comboBoxMoment->count();
 for (int i=0;  i<nb; ++i)
     {if (comboBoxMoment->text(i).left(1).toInt() == nb_prises)
         {comboBoxMoment->setCurrentItem(i);
          return;
         }
     }
}

//-------------------------------------- ComputeSecondForm -------------------------------------------------------
// RETOUR: float: valeur arrondie à la secabilité prés, de la dose à prendre à chaque prise (déjà divisée par la répartition)
//
void Dlg_MedicaAdjustPoso::ComputeSecondForm(const CPosologie &poso, QString &upMin, QString &upMax, QString &upUnit)
{upMin = upMax = upUnit = "0";
 if (poso.m_MEDICA_POSO_EQUI_COEFF == "0") return;
 float poids(  lineEdit_Poids->text().toFloat() );
 float taille( lineEdit_Taille->text().toFloat() );
 float fMin(0), fMax(0), coeff(1/poso.m_MEDICA_POSO_EQUI_COEFF.toFloat());
 //..................... multiplier par le coefficient par le poids ou la surface ............................
 if (poso.m_MEDICA_POSO_FACT_CORP_QU != "0")
    {if      (m_FactCorpUnit=="Kg" && poids>0)
             {coeff *= poids / poso.m_MEDICA_POSO_FACT_CORP_QU.toFloat();
             }
     else if (m_FactCorpUnit=="m2" && taille>0 && poids >0)
             {coeff *= sqrt(taille * poids / 3600) / poso.m_MEDICA_POSO_FACT_CORP_QU.toFloat();  //(Mosteller)
             }
    }
 //..................... calculer les valeurs finales Min et Max ............................
 fMin = poso.m_MEDICA_POSO_DOSE_MIN.toFloat()     * coeff;
 if (poso.m_MEDICA_POSO_DOSE_MAX != 0)
    {fMax = poso.m_MEDICA_POSO_DOSE_MAX.toFloat() * coeff;
    }
 upUnit = G_pCApp->m_pCMedicaBase->Medica_GetUnite(G_pCApp->m_pCMedicaBase->m_MEDICA_FORME_UP_TBL_NAME, poso.m_MEDICA_POSO_UNIT_COEFF)+"(s)";
 upMin  = QString::number( G_pCApp->m_pCMedicaBase->Medica_ArondirUniteDePrise(fMin, m_Secabilite ));
 upMax  = QString::number( G_pCApp->m_pCMedicaBase->Medica_ArondirUniteDePrise(fMax, m_Secabilite ));
}

//-------------------------------------- lineEdit_Poids_textChanged -------------------------------------------------------
void Dlg_MedicaAdjustPoso::lineEdit_Poids_textChanged( const QString &str )
{*m_pPoids = str;
  DisplayResult();
}

//-------------------------------------- lineEdit_Taille_textChanged -------------------------------------------------------
void Dlg_MedicaAdjustPoso::lineEdit_Taille_textChanged( const QString &str )
{*m_pTaille = str;
  DisplayResult();
}

//-------------------------------------- SetSecondForm ---------------------------------------------------------------------
void Dlg_MedicaAdjustPoso::SetSecondForm( )
{QString upMin, upMax, upUnit;
 ComputeSecondForm(m_PosoIn, upMin, upMax, upUnit);
 if (upMin != "0") {lineEdit_QMin_UP2->setText(upMin); lineEdit_QMin_UP2->show(); textLabelUnite_UP2->show();}
 else              {lineEdit_QMin_UP2->hide(); textLabelUnite_UP2->hide();}
 if (upMax != "0") {lineEdit_QMax_UP2->setText(upMax); lineEdit_QMax_UP2->show(); lineEdit_a_UP2->show(); textLabelUnite_UP2->show();}
 else              {lineEdit_QMax_UP2->hide(); lineEdit_a_UP2->hide(); textLabelUnite_UP2->hide();}
 textLabelUnite_UP2->setText(upUnit);
}

//-------------------------------------- radioButtonSecable1_clicked -------------------------------------------------------
void Dlg_MedicaAdjustPoso::radioButtonSecable1_clicked()
{radioButtonSecable1->setChecked(FALSE);
 radioButtonSecable2->setChecked(FALSE);
 radioButtonSecable4->setChecked(FALSE);
 radioButtonSecable1->setChecked(TRUE);
 m_Secabilite = 1;
 DisplayResult();
}

//-------------------------------------- radioButtonSecable2_clicked -------------------------------------------------------
void Dlg_MedicaAdjustPoso::radioButtonSecable2_clicked()
{radioButtonSecable1->setChecked(FALSE);
 radioButtonSecable2->setChecked(FALSE);
 radioButtonSecable4->setChecked(FALSE);
 radioButtonSecable2->setChecked(TRUE);
 m_Secabilite = 2;
 DisplayResult();
}

//-------------------------------------- radioButtonSecable4_clicked -------------------------------------------------------
void Dlg_MedicaAdjustPoso::radioButtonSecable4_clicked()
{radioButtonSecable1->setChecked(FALSE);
 radioButtonSecable2->setChecked(FALSE);
 radioButtonSecable4->setChecked(FALSE);
 radioButtonSecable4->setChecked(TRUE);
 m_Secabilite = 4;
 DisplayResult();
}

//-------------------------------------- DisplayResult -------------------------------------------------------
void Dlg_MedicaAdjustPoso::DisplayResult()
{SetSecondForm();
 InterfaceToPoso(m_PosoIn);
 QString result("<html><head><meta name=\"qrichtext\" content=\"1\" />"
                "<title>Prescription</title>"
                "</head>"
                "<body style=\"font-size:10pt;font-family:Arial Narrow\">");
 result += G_pCApp->m_pCMedicaBase->Medica_PosologieToPrescriptionHtml(  m_PosoIn                ,
                                                                     lineEdit_Poids->text()  ,
                                                                     lineEdit_Taille->text() ,
                                                                     -1                      ,
                                                                     m_ProdName              ,
                                                                     m_FactCorpUnit
                                                                  );
 textBrowser_Result->setText(result + "</body></html>");
}

//-------------------------------------- slider_VarPoso_valueChanged -------------------------------------------------------
void Dlg_MedicaAdjustPoso::slider_VarPoso_valueChanged( int )
{DisplayResult();
}

//-------------------------------------- comboBoxMoment_activated -------------------------------------------------------
void Dlg_MedicaAdjustPoso::comboBoxMoment_activated( const QString & )
{QString nbPrises(""), schema("");
 int action = 0;
 if      (comboBoxMoment->currentText()==tr("(vide)"))
         {m_PosoIn.m_MEDICA_POSO_NB_PRISES     = "1";
          action                               = 1;   // schema vide --> " par jour par semaine etc .."
         }
 else if (comboBoxMoment->currentText()==tr("Schéma de prise conseillé") &&
          G_pCApp->m_pCMedicaBase->Medica_GetSchemaDePrise(m_PosoIn.m_MEDICA_POSO_PK, nbPrises, schema))
         {m_PosoIn.m_MEDICA_POSO_NB_PRISES     =  nbPrises;
          action = comboBoxMoment->currentItem();
         }
 else    {m_PosoIn.m_MEDICA_POSO_NB_PRISES = comboBoxMoment->currentText().left(1);
          action = comboBoxMoment->currentItem();
         }

 switch(action)
    {case 1:  m_PosoIn.m_MEDICA_POSO_REPART_PRISES =  schema;          break;
     case 2:  m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "MA";             break;
     case 3:  m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "MI";             break;
     case 4:  m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "SO";             break;
     case 5:  m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "CO";             break;
     case 6:  m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "#rDJ";           break;
     case 7:  m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "#rMI";           break;
     case 8:  m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "#rSO";           break;
     case 9:  m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "MA;SO";          break;
     case 10: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "MA;MI";          break;
     case 11: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "MI;SO";          break;
     case 12: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "MA;CO";          break;
     case 13: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "#rDJ;#rSO";      break;
     case 14: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "#rDJ;#rMI";      break;
     case 15: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "#rMI;#rSO";      break;
     case 16: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "MA;MI;SO";       break;
     case 17: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "#rDJ;#rMI;#rSO"; break;
     case 18: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "MA;MI;AM;SO";    break;
     case 19: m_PosoIn.m_MEDICA_POSO_REPART_PRISES = "MA;MI;AM;SO;CO"; break;
     default:
              m_PosoIn.m_MEDICA_POSO_REPART_PRISES = m_PosoIn.m_MEDICA_POSO_NB_PRISES + "XJO";
    }
 DisplayResult();
}
//-------------------------------------- textEditNote_textChanged -------------------------------------------------------
void Dlg_MedicaAdjustPoso::textEditNote_textChanged()
{m_PosoIn.m_PosoTextuelle= textEditNote->text();
}

//-------------------------------------- comboBoxPendant_textChanged -------------------------------------------------------
void Dlg_MedicaAdjustPoso::comboBoxPendant_textChanged( const QString & )
{DisplayResult();
}


//-------------------------------------- pushButtonOK_clicked -------------------------------------------------------
void Dlg_MedicaAdjustPoso::pushButtonOK_clicked()
{if (m_PosoIn.m_PosoTextuelle.length())
    {m_PosoIn.m_PosoType |= CPosologie::Textuelle;
    }
else
    {m_PosoIn.m_PosoType &= ~CPosologie::Textuelle;
    }
*m_pPoids  = lineEdit_Poids->text();
*m_pTaille = lineEdit_Taille->text();
 accept();
}

//-------------------------------------- pushButton_A_clicked -------------------------------------------------------
void Dlg_MedicaAdjustPoso::pushButton_A_clicked()
{DisplayResult();     // positionne aussi : m_PosoIn.m_PosoAdjust sur bon état (positif ou négatif)
 if (m_PosoIn.m_PosoAdjust < 0)  slider_VarPoso->setEnabled(FALSE);
 else                            slider_VarPoso->setEnabled(TRUE);
}


