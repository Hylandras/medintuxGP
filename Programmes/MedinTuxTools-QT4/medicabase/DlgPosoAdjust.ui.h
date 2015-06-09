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
void DlgPosoAdjust::init()
{
 lineEdit_QMax_UP2 ->hide();
 lineEdit_QMin_UP2 ->hide();
 lineEdit_a_UP2 ->hide();
 textLabelUnite_UP2 ->hide();
 slider_VarPoso ->hide();
 Infos_Techniques->hide();
 pushButton_NextPoso->hide();
 
 m_RatioMin = 1;   // ratio pour adapter poso au choix du nb de fois du combobox
 m_RatioMax = 1;   // ratio pour adapter poso au choix du nb de fois du combobox
}

//-------------------------------------- initDialog -------------------------------------------------------
void DlgPosoAdjust::initDialog(CMoteurBase* pCmoteurBase, Type_Posologie type_Posologie,  CMDI_Terrain *pCMDI_Terrain)
{m_pCMoteurBase   = pCmoteurBase;
 m_pCMDI_Terrain  = pCMDI_Terrain; 
 if (m_pCMoteurBase==0) return;
 m_pCMoteurBase->GotoDebug();
 //........................... on recharge la fiche posologie d'origine ............................
 //                            car la structure type_Posologie peut etre celle d'une
 //                            ordo déjà ajustée donc qui n'est plus valide pour les modifs
 //                            on conserve le numero d'ordo et l'ALD
 QString str_poso = m_pCMoteurBase->VidalGetFichePosologieEx(type_Posologie.Code_Posologie,
					                     QString::number(type_Posologie.Number),
							     QString::number(type_Posologie.Ald),
							     type_Posologie.PosoMix,
                                                             type_Posologie.CustomPoso
					                    );
 if (str_poso =="")    return;
 m_pCMoteurBase->VidalStringPosoToStructPoso(str_poso, type_Posologie ); 
 
 m_Poso            = type_Posologie;
 //m_pCMoteurBase->VidalCopyPosologie(type_Posologie, m_Poso);
 lineEdit_Poids->setText(QString::number(m_Poso.PoidsPatient)); // cela permet de sauver le poids 
 lineEdit_Taille->setText(QString::number(m_Poso.TaillePatient));
 //...................... connecter après pour eviter appels à lineEdit_Poids_textChanged() ......................................
 connect( lineEdit_Poids, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Poids_textChanged(const QString&) ) );
 connect( lineEdit_Taille, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit_Taille_textChanged(const QString&) ) );
    
 QString forme;
 
 //........................ essai de determination de la dose par unite de temps ...................................................
 //                          (cela permet de pouvoir changer le nbr de prise par jour)
 m_pCMoteurBase->VidalCalculeDoseParUniteDeTemps (m_Poso, m_DoseByUT_Min, m_DoseByUT_Max, m_UT, m_ParPoidsUT);
 forme = QString::number(m_DoseByUT_Min);         
 if (m_DoseByUT_Max) 
    {forme += " à " +  QString::number(m_DoseByUT_Max);
    }
 forme +=" ";
 forme += m_pCMoteurBase->VidalGetField("ex_unite_de_prise", "CodeUP", m_Poso.Code_Unite_De_Prise, "LibelleUP")+"(s)";
 if (m_ParPoidsUT)
    {forme += " par " ;
     if (m_ParPoidsUT>1)forme += QString::number(m_ParPoidsUT) ;
     if (m_Poso.Code_Par == "P") forme += " Kg ";
     if (m_Poso.Code_Par == "S") forme += " m² ";
    }
 if (m_UT<=86400)
    {forme += " toutes les ";
     forme += QString::number(m_UT/3600);
     forme += " heure(s) ";
    }
 else 
    {forme += " tous les ";
     forme += QString::number(m_UT/86400);
     forme += " jours(s) ";
    }
 if (m_DoseByUT_Min==0)
    {textLabelUP_Jour->setText("");
     comboBoxMoment->hide();
    }
 else
    {textLabelUP_Jour->setText(forme);
    }

 //............... a priori rien de visible ................................................................
 //                pour l'adéquation
 lineEdit_QMax_UP2 ->hide();
 lineEdit_QMin_UP2 ->hide();
 lineEdit_a_UP2 ->hide();
 textLabelUnite_UP2 ->hide();
 slider_VarPoso ->hide();

 //.......................... forme de l'unite de prise 1 ................................................
 //                           (affichée au dessus du slider)
 forme  = m_pCMoteurBase->VidalGetField("ex_unite_de_prise", "CodeUP", m_Poso.Code_Unite_De_Prise, "LibelleUP");
 if (m_Poso.P_Kg)   
    {forme += " par";
     if (m_Poso.P_Kg>1)          forme += " " +  QString::number(m_Poso.P_Kg);
     if (m_Poso.Code_Par == "P") forme += " Kg de poids";
     if (m_Poso.Code_Par == "S") forme += " m² de surface corporelle";     
    }
 else
    {lineEdit_Poids->hide();
     textLabelPoids->hide();
     textLabelPdsUnit->hide();
     lineEdit_Taille->hide();
     textLabelTaille->hide();
     textLabelTailleUnit->hide();
    }
 textLabelUnite_UP1 ->setText(forme);
 //............................. Affichage min et max selon valeurs unites de prise ...............
 if (m_Poso.Quantite2)
    {slider_VarPoso ->show();
     lineEdit_QMin_UP1->setText(QString::number(m_Poso.Quantite1));     
     lineEdit_QMax_UP1->setText(QString::number(m_Poso.Quantite2));
    }
 else
    {lineEdit_QMax_UP1->setText(QString::number(m_Poso.Quantite1));
     lineEdit_a_UP1->hide();
     lineEdit_QMin_UP1->hide();     
    }
 //.............................. si adequation etablir affichage ...................................
 //                               des valeurs sous le slider
 if (m_Poso.Adequation_UP1_UP2) 
    {m_pCMoteurBase->VidalGetAdequation(listView_Coef, type_Posologie);  // charger les infos adequations
     CalculeAdequationPoso(m_Poso, lineEdit_Poids->text(), lineEdit_Taille->text()); 
     if (m_Poso.Quantite2)   
        {lineEdit_QMin_UP2->show();
         lineEdit_QMax_UP2->show();
         lineEdit_a_UP2->show();
        }
     else
        {lineEdit_QMax_UP2->show();      
        }
     textLabelUnite_UP2->show();
     textLabelUnite_UP2 ->setText(m_pCMoteurBase->VidalGetField("ex_unite_de_prise", "CodeUP", m_Poso.Code_Unite_De_Prise2, "LibelleUP")+"(s)");
     m_Secabilite      = atoi(m_pCMoteurBase->VidalGetField("ex_unite_de_prise", "CodeUP", m_Poso.Code_Unite_De_Prise2, "Secabilite"));
    }
 else
    {CalculeAdequationPoso(m_Poso, lineEdit_Poids->text(), lineEdit_Taille->text());
     m_Secabilite      = atoi(m_pCMoteurBase->VidalGetField("ex_unite_de_prise", "CodeUP", m_Poso.Code_Unite_De_Prise, "Secabilite"));
    }
 //....................... initialiser secabilite ...........................................
 radioButtonSecable1->setChecked(FALSE); 
 radioButtonSecable2->setChecked(FALSE); 
 radioButtonSecable4->setChecked(FALSE); 
 switch(m_Secabilite)
   {default:
    case 1: radioButtonSecable1->setChecked(TRUE); break;
    case 2: radioButtonSecable2->setChecked(TRUE); break;
    case 4: radioButtonSecable4->setChecked(TRUE); break;
   }       
}

//-------------------------------------- CalculeAdequationPoso -----------------------------------------------------
Type_Posologie DlgPosoAdjust::CalculeAdequationPoso(Type_Posologie &posoOri, const QString &qsPoids, const QString &qsTaille)
{    Type_PosoDec  posoDec;
     QString       result;
     float         value  = 0;
     float nb_fois        = 0;
     QString code_Moment  = ComboMomentGetValue(nb_fois);
     float coefCorp       = ((posoOri.Code_Par == "S") ? sqrt(qsTaille.toFloat() * qsPoids.toFloat() / 3600) : qsPoids.toFloat()); 
     lineEdit_Value->setText(QString::number(coefCorp));   
     m_pCMoteurBase->GotoDebug();
    //.................. faire une copie de l'original sur un de travail .....................  
     Type_Posologie posologie = posoOri;    // = m_Poso;
     //m_pCMoteurBase->VidalCopyPosologie(posoOri, posologie);
     if (lineEdit_QMin_UP1->isVisible ()) posologie.Quantite1 = atof(lineEdit_QMin_UP1->text());
     if (lineEdit_QMax_UP1->isVisible ()) posologie.Quantite2 = atof(lineEdit_QMax_UP1->text());
     //..................... modifier valeurs si combo moment avec ............................
     //                      valeur != "(vide)"
     if (nb_fois && code_Moment != "0")
        {float ratioAdeqMin   = 1;
         float ratioAdeqMax   = 1;
         float t_mul          = 86400/m_UT;             // facteur multiplicatif pour avoir valeurs pour un jour
         float doseMin        = m_DoseByUT_Min * t_mul / nb_fois;
         float doseMax        = m_DoseByUT_Max * t_mul / nb_fois;
         if (posologie.Adequation_UP1_UP2)
            {Type_PosoDec poso_Dec;
             GetAdequationPosoDec(listView_Coef, poso_Dec/*, 100*/);
             if (poso_Dec.Qte1UP1 && poso_Dec.Qte1UP2) ratioAdeqMin = poso_Dec.Qte1UP2/poso_Dec.Qte1UP1;
             if (poso_Dec.Qte2UP1 && poso_Dec.Qte2UP2) ratioAdeqMax = poso_Dec.Qte2UP2/poso_Dec.Qte2UP1;
             posologie.Quantite1            = doseMin * ratioAdeqMin;
             posologie.Quantite2            = doseMax * ratioAdeqMax; 
	     //...... petite particularité speciale ..............
	     //       pour preserver l'unité de mesure du maximum
	     posologie.Nombre_Unites        = 1;
	     result                         = posologie.Code_Unite_De_Prise;
	     posologie.Code_Unite_De_Prise  = posologie.Code_Unite_De_Prise2;
             posologie.Code_Unite_De_Prise2 = result;	     
            }
         else
            {posologie.Quantite1   = doseMin;
             posologie.Quantite2   = doseMax;
            }
         posologie.Code_Moment          = code_Moment;
         posologie.Combien1             = (long)nb_fois;
         posologie.Combien2             = 0;
         posologie.Pendant1             = 0;
         posologie.Pendant2             = 0;
         posologie.Code_Duree2          = "0";
         posologie.Code_Duree1          = "40";
	 posologie.Adequation_UP1_UP2   = 0; 
         posologie.Tous_Les             = 0;

         if (posologie.Quantite2)
            {slider_VarPoso ->show();
             lineEdit_QMin_UP1->show();
             lineEdit_a_UP1->show();
             lineEdit_QMin_UP1->setText(QString::number(posologie.Quantite1,'f',2));     
             lineEdit_QMax_UP1->setText(QString::number(posologie.Quantite2,'f',2));
            }
         else
            {lineEdit_QMax_UP1->setText(QString::number(posologie.Quantite1,'f',2));
             lineEdit_QMin_UP1->hide();  
             slider_VarPoso ->hide();
             lineEdit_a_UP1->hide();                
            }
        }

     //................. calcul sans adequation ..............................................
     if (posologie.Adequation_UP1_UP2==0)  
        {if (posologie.P_Kg)
            {posologie.Quantite1            = (posologie.Quantite1*coefCorp/posologie.P_Kg) ;
             posologie.Quantite2            = (posologie.Quantite2*coefCorp/posologie.P_Kg) ;
             posologie.Code_Unite_De_Prise  = posologie.Code_Unite_De_Prise;
            }
         if (posologie.Quantite2) 
            {value = (posologie.Quantite1 + (posologie.Quantite2 - posologie.Quantite1)*slider_VarPoso->value ()/99);
             posologie.Quantite1 = value;
             posologie.Quantite2 = 0;
            }   
         if ((value=comboBoxPendantGetValue())>0)
            {posologie.Pendant1    = (long)value;
             posologie.Pendant2    = 0;
             posologie.Code_Duree2 = "40";
            }
         posologie.Quantite1 = ArondirUniteDePrise(posologie.Quantite1, m_Secabilite); 
         posologie.P_Kg      = 0;     // tout etant converti on ne s'exprime plus en pds/kg
            
         result        = "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                         "<title>Prescription</title>"
                         "</head>"
                         "<body style=\"font-size:10pt;font-family:Arial Narrow\">";
         m_pCMoteurBase->VidalDecodePosologieEx(posologie, result, m_NumInOrdo, m_NumInOrdo, TYP_DECOD_ORDO);
         textBrowser_Result1->setText(result+"</p></body></html>");           
         return posologie;
        }

     //.............. recuperer data de l'adequation .....................
     GetAdequationPosoDec(listView_Coef, posoDec/*, 100 */);                 // recuperer les infos pour 100 kgs
     if (posologie.Code_Par=="S") coefCorp *= 100; 
     //...................... première forme ..............................
     //                       celle d'origine non adéquée non arondie
     posologie .Quantite1            = (posoDec.Qte1UP1*coefCorp/100.0) ;
     posologie .Quantite2            = (posoDec.Qte2UP1*coefCorp/100.0) ;
     posologie.Maximum               = posoDec.MaxUP1;
     posologie .Code_Unite_De_Prise  = QString::number(posoDec.Code_UP1);
     posologie.P_Kg                  = 0;
     result       = "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                    "<title>Prescription</title>"
                    "</head>"
                    "<body style=\"font-size:10pt;font-family:Arial Narrow\">";
     m_pCMoteurBase->VidalDecodePosologieEx(posologie, result, m_NumInOrdo, m_NumInOrdo, TYP_DECOD_ORDO);
     textBrowser_Result2->setText(result+"</p></body></html>");
     textLabelUnite_UP2 ->show();
     if (posologie.Quantite2)   // posoDec.Qte2UP2
        {lineEdit_QMin_UP2->setText(QString::number(posoDec.Qte1UP2*coefCorp/100.0,'f',2));     
         lineEdit_QMax_UP2->setText(QString::number(posoDec.Qte2UP2*coefCorp/100.0));
        }
     else
        {lineEdit_QMax_UP2->setText(QString::number(posoDec.Qte1UP2*coefCorp/100.0,'f',2));
        }

     //...................... Deuxième forme ..............................
     //                       celle adéquée
     posologie.Quantite1            = (posoDec.Qte1UP2*coefCorp/100.0)  ;
     posologie.Quantite2            = (posoDec.Qte2UP2*coefCorp/100.0)  ;
     posologie.Maximum              = posoDec.MaxUP2;
     posologie.Code_Unite_De_Prise  = QString::number(posoDec.Code_UP2);
     if (posologie.Quantite2)   // posoDec.Qte2UP2
        {float   value = (posologie.Quantite1 + (posologie.Quantite2 - posologie.Quantite1)*slider_VarPoso->value ()/99);
         posologie.Quantite1 = value;
         posologie.Quantite2 = 0;
        } 
     if ((value=comboBoxPendantGetValue())>0)
        {posologie.Pendant1    = (long)value;
	 posologie.Pendant2    = 0;
         posologie.Code_Duree2 = "40";
        } 
     posologie.Quantite1 = ArondirUniteDePrise(posologie.Quantite1, m_Secabilite);
     posologie.P_Kg      = 0; // tout etant converti on n'exprime plus en pds/kg
     
     result       = "<html><head><meta name=\"qrichtext\" content=\"1\" />"
                    "<title>Prescription</title>"
                    "</head>"
                    "<body style=\"font-size:10pt;font-family:Arial Narrow\">";
     m_pCMoteurBase->VidalDecodePosologieEx(posologie, result, m_NumInOrdo, m_NumInOrdo, TYP_DECOD_ORDO);
     textBrowser_Result1->setText(result+"</p></body></html>");   
     
 return posologie;
}

//-------------------------------------- ArondirUniteDePrise -------------------------------------------------------
float DlgPosoAdjust::ArondirUniteDePrise(float value, 
                                         int secabilite  // nbr de divisions possibles de l'unite de prise
 				        )
{return ArondirUniteDePrise(QString::number(value,'f',2), secabilite);
}

//-------------------------------------- ArondirUniteDePrise -------------------------------------------------------
float DlgPosoAdjust::ArondirUniteDePrise(QString value, int secabilite)
{int f = value.find('.');
 if (f == -1)             return atof(value);
 switch(secabilite)
 {default:
  case 1:
       return atof(value.left(f));
  case 2:
      {int   val  = value.at(f+1).latin1()-48; // recuperer premier car apres la virgule
       if (val<=3)              return atof(value.left(f));
       if (val>=4 && val <=7)   return atof(value.left(f)+".5");
       else                     return atoi(value.left(f))+1;
      }
  case 4:
      {int   val  = value.at(f+1).latin1()-48; // recuperer premier car apres la virgule
       if (val<=2)              return atof(value.left(f));
       if (val>=3 && val <=5)   return atof(value.left(f)+".25");
       if (val>=5 && val <=6)   return atof(value.left(f)+".5");
       if (val>=7 && val <=9)   return atof(value.left(f)+".75");
       else                     return atoi(value.left(f))+1;
      }
 }//endswitch(secabilite) 
}

//-------------------------------------- GetAdequationPosoDec -------------------------------------------------------
void DlgPosoAdjust::GetAdequationPosoDec(QListView *pQlistView, Type_PosoDec &posoDec /*, int poids */)
{QListViewItem *pQListViewItem = pQlistView->findItem (/*QString::number(poids)*/"100", 0);
 if (pQListViewItem<=0) return;
 posoDec.Qte1UP1          = pQListViewItem->text(1).toFloat(); // Qte1UP1
 posoDec.Qte2UP1          = pQListViewItem->text(2).toFloat(); // Qte2UP1
 posoDec.MaxUP1           = pQListViewItem->text(3).toFloat(); // MaxUP1
 posoDec.Qte1UP2          = pQListViewItem->text(6).toFloat(); // Qte1UP2
 posoDec.Qte2UP2          = pQListViewItem->text(7).toFloat(); // Qte2UP2
 posoDec.MaxUP2           = pQListViewItem->text(8).toFloat(); // MaxUP2
 posoDec.UtilUP1          = 0;  // non utilisé
 posoDec.UtilUP2          = 0;  // non utilisé
 posoDec.NbUP1            = pQListViewItem->text(4).toFloat(); // NbUP1
 posoDec.NbUP2            = pQListViewItem->text(9).toFloat(); // NbUP2
 posoDec.Code_UP1         = pQListViewItem->text(11).toLong(); // Code_UP1
 posoDec.Code_UP2         = pQListViewItem->text(12).toLong(); // Code_UP2
 posoDec.PosologieUP1     =""; // non utilisé
 posoDec.PosologieUP2     =""; // non utilisé
 posoDec.Commentaire      =""; // non utilisé
 posoDec.Nombre_UnitesUP1 =""; // non utilisé
 posoDec.Nombre_UnitesUP2 =""; // non utilisé	 					       
}


//-------------------------------------- lineEdit_Poids_textChanged -------------------------------------------------------
void DlgPosoAdjust::lineEdit_Poids_textChanged( const QString &qsPoids )
{CalculeAdequationPoso(m_Poso, qsPoids, lineEdit_Taille->text());
}

//-------------------------------------- lineEdit_Taille_textChanged() ---------------------------------------------------
void DlgPosoAdjust::lineEdit_Taille_textChanged( const QString &qsTaille )
{CalculeAdequationPoso(m_Poso, lineEdit_Poids->text(), qsTaille);
}

//-------------------------------------- lineEdit_Poids_textChanged -------------------------------------------------------
void DlgPosoAdjust::comboBoxMoment_activated( const QString& )
{
 CalculeAdequationPoso(m_Poso, lineEdit_Poids->text(), lineEdit_Taille->text());

}

//-------------------------------------- ComboMomentGetValue ---------------------------------------------------
QString DlgPosoAdjust::ComboMomentGetValue(float &nb_fois)
{        nb_fois     = 0;
 QString code_Moment = "0";
 QString       qtext = comboBoxMoment->text( comboBoxMoment->currentItem() ); 
 if (qtext=="(vide)")   return code_Moment;
 int f = qtext.find(' ');
 if (f==-1)             return code_Moment;
 nb_fois = atof(qtext.left(f));
 code_Moment =  qtext.mid(qtext.findRev(' ')+1);     
 return code_Moment;
}

//-------------------------------------- slider_VarPoso_valueChanged -------------------------------------------------------
void DlgPosoAdjust::slider_VarPoso_valueChanged( int )
{CalculeAdequationPoso(m_Poso, lineEdit_Poids->text(), lineEdit_Taille->text());
}



//-------------------------------------- pushButtonOK_clicked -------------------------------------------------------
void DlgPosoAdjust::pushButtonOK_clicked()
{m_Poso = CalculeAdequationPoso(m_Poso,  lineEdit_Poids->text(), lineEdit_Taille->text());
 m_Poso.PoidsPatient = atof(lineEdit_Poids->text());
 accept();
}



//-------------------------------------- radioButtonSecable1_clicked -------------------------------------------------------
void DlgPosoAdjust::radioButtonSecable1_clicked()
{radioButtonSecable1->setChecked(FALSE); 
 radioButtonSecable2->setChecked(FALSE); 
 radioButtonSecable4->setChecked(FALSE); 
 radioButtonSecable1->setChecked(TRUE); 
 m_Secabilite = 1;
 CalculeAdequationPoso(m_Poso,  lineEdit_Poids->text(), lineEdit_Taille->text());
}


//-------------------------------------- radioButtonSecable2_clicked -------------------------------------------------------
void DlgPosoAdjust::radioButtonSecable2_clicked()
{radioButtonSecable1->setChecked(FALSE); 
 radioButtonSecable2->setChecked(FALSE); 
 radioButtonSecable4->setChecked(FALSE); 
 radioButtonSecable2->setChecked(TRUE);
 m_Secabilite = 2;
 CalculeAdequationPoso(m_Poso,  lineEdit_Poids->text(), lineEdit_Taille->text());
}


//-------------------------------------- radioButtonSecable4_clicked -------------------------------------------------------
void DlgPosoAdjust::radioButtonSecable4_clicked()
{radioButtonSecable1->setChecked(FALSE); 
 radioButtonSecable2->setChecked(FALSE); 
 radioButtonSecable4->setChecked(FALSE); 
 radioButtonSecable4->setChecked(TRUE);
 m_Secabilite = 4;
 CalculeAdequationPoso(m_Poso,  lineEdit_Poids->text(), lineEdit_Taille->text());
}


//-------------------------------------- comboBoxPendant_activated -------------------------------------------------------
void DlgPosoAdjust::comboBoxPendant_activated( const QString & )
{
 CalculeAdequationPoso(m_Poso,  lineEdit_Poids->text(), lineEdit_Taille->text());
}


//-------------------------------------- comboBoxPendant_textChanged -------------------------------------------------------
void DlgPosoAdjust::comboBoxPendant_textChanged( const QString & )
{
 CalculeAdequationPoso(m_Poso,  lineEdit_Poids->text(), lineEdit_Taille->text());
}

//-------------------------------------- comboBoxPendantGetValue ---------------------------------------------------
float DlgPosoAdjust::comboBoxPendantGetValue()
{QString       qtext = comboBoxPendant->lineEdit()->text(); 
 if (qtext=="(vide)")   return -1;
 return atof(qtext);  
}

//-------------------------------------- pushButton_NextPoso_clicked() ---------------------------------------------------
void DlgPosoAdjust::pushButton_NextPoso_clicked()
{

}


