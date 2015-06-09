#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/medicabase/C_PosologieGrammar.h"
#include <QDateTime>
#include <QPixmap>
#include <float.h>
#include <QObject>
#include <QDebug>
//=================== C_PosologieGrammar ===================================
/*! \class C_PosologieGrammar
 *  \brief Class to set and interpret posologie grammar.
 */
 static QMap<QString, C_WordGenrePair> m_WordGenreMap;	/*!< Contient le dictionnaire de mots */
 //---------------------------- get_AccordedWord [static]----------------------------------------------
 /*! \brief intialise  QMap<QString, C_WordGenrePair>  values to allow corect according genre and plural of a word
  *  \param isPlural = number to specifie if word must bee accorded to plural (if >1)
  *  \param key_word :   to be accorded
  *  \return correct accorded plural singular word and sexe genre in
 */
 QString C_PosologieGrammar::get_AccordedWord( int isPlural, const QString &key_word )
 {   char sexe;
     return get_AccordedWord( isPlural, key_word, sexe );
 }

//---------------------------- get_AccordedWord [static]----------------------------------------------
/*! \brief intialise  QMap<QString, C_WordGenrePair>  values to allow corect according genre and plural of a word
 *  \param isPlural = number to specifie if word must bee accorded to plural (if >1)
 *  \param key_word :   to be accorded
 *  \param sexe: reference to a char in wich will be returned genre 'f' or 'm'
 *  \return correct accorded plural singular word and sexe genre in
*/
QString C_PosologieGrammar::get_AccordedWord( int isPlural, const QString &key_word, char &sexe )
{   QMap<QString, C_WordGenrePair>::const_iterator it = m_WordGenreMap.find( key_word );
    if ( it == m_WordGenreMap.end() )
       { sexe = 'm';
         return key_word;
       }
    C_WordGenrePair wordGenrePair = it.value();
    sexe                          = wordGenrePair.sexe();
    if (isPlural>1)  return wordGenrePair.plural_word();
    else             return wordGenrePair.singular_word();
}

//---------------------------- set_QMapGenre [static]----------------------------------------------
/*! \brief intialise  QMap<QString, C_WordGenrePair> m_WordGenreMap  values to allow correct according genre and plural of a word
 *  \param const QString &textData :   data as follows : key_word 1 | singular word 1 | plural word 1 | f \n
 *                                                       key_word 2 | singular word 2 | plural word 2 | m \n
 *                                     and so...
 *  \return the size of static  QMap m_WordGenreMap.
*/
int C_PosologieGrammar::set_QMapGenre( const QString & path)
{
    QString      textData = CGestIni::Param_UpdateFromDisk(path);
    if ( textData.length() ==0 )  return 0;
    textData.remove('\r');
    QStringList linesList = textData.split('\n');
    for (int i=0; i<linesList.size(); ++i)
        { QStringList    partsList = linesList[i].split('|');
          if (partsList.size()==4)
             { QString              key = partsList[0];
               m_WordGenreMap.insert (  key, C_WordGenrePair (   partsList[0].trimmed(),                 // orthographe approximative ( mot clef de recherche )
                                                                 partsList[1].trimmed(),                 // orthographe correcte singulier Masculin/Feminin
                                                                 partsList[2].trimmed(),                 // orthographe correcte pluriel   Masculin/Feminin
                                                                 partsList[3].trimmed()[0].toLatin1()    // sexe
                                                             )
                                     );
             }  // if (partsList.size()==4)
         }  // for (int i=0; i<linesList.size(); ++i)
    return m_WordGenreMap.size();
}

//---------------------------- literalFlagToString [static] ------------------------------------------------
/*! \brief return a literal string from int flag.  [static]
 *  \param int must_be_literal :    int flag
 *  \return literal string.
*/
QString C_PosologieGrammar::literalFlagToString(int must_be_literal)
{QString ret = "";
if (must_be_literal & C_PosologieGrammar::UP_QUANTITE)        ret += "UP_QUANTITE ";
if (must_be_literal & C_PosologieGrammar::DURATION)           ret += "DURATION ";
if (must_be_literal & C_PosologieGrammar::SEQUENCE_HOUR)      ret += "SEQUENCE_HOUR ";
if (must_be_literal & C_PosologieGrammar::MOMENT_MEAL)        ret += "MOMENT_MEAL ";
if (must_be_literal & C_PosologieGrammar::UP_JUST_FRACTION)   ret += "UP_JUST_FRACTION ";
return ret;
}
//---------------------------- literalStringToFlag [static] ------------------------------------------------
/*! \brief return the literal flag from literal string.  [static]
 *  \param const QString &conv_mod :  literal string to convert in flag
 *  \return literal flag.
*/
int C_PosologieGrammar::literalStringToFlag(const QString &conv_mod)
{int must_be_literal = 0;
 if (conv_mod.contains("UP_QUANTITE"))        must_be_literal |= C_PosologieGrammar::UP_QUANTITE;
 if (conv_mod.contains("DURATION"))           must_be_literal |= C_PosologieGrammar::DURATION;
 if (conv_mod.contains("SEQUENCE_HOUR"))      must_be_literal |= C_PosologieGrammar::SEQUENCE_HOUR;
 if (conv_mod.contains("MOMENT_MEAL"))        must_be_literal |= C_PosologieGrammar::MOMENT_MEAL;
 if (conv_mod.contains("ALL_LITERAL"))        must_be_literal  = C_PosologieGrammar::ALL_LITERAL;
 if (conv_mod.contains("NOT_LITERAL"))        must_be_literal  = C_PosologieGrammar::UP_JUST_FRACTION;
 return must_be_literal;
}
//---------------------------- empty_skeleton_grammar [static] ------------------------------------------------
/*! \brief return a empty grammar string.  [static]
 *  \return empty grammar string.
*/
QString C_PosologieGrammar::empty_skeleton_grammar()
{return QString(
                "<gph>\n"                          // -- DEBUT du header d'une ligne prescriptive ---
                " <atc></atc>\n"                   //        code ATC du produit
                " <stup></stup>\n"                 //        1/si supefiant  0/sinon
                " <rfid></rfid>\n"                 //        rembourse pour cette indication, vide sinon
                " <price></price>\n"               //        cout d'une boite
                " <total_price></total_price>\n"   //        cout total de la prescription
                " <nb_udv></nb_udv>\n"             //        nombre total d'unite de prise de la prescription (ne correspond par forcement a nb_boites*nb_up/boite)
                " <nb_boites></nb_boites>\n"       //        nombre de boites
                " <status></status>\n"             //        proprietes type F/traitement de fond et autres a venir
                " <smr></smr>\n"                   //        niveau du service medical rendu
                " <smr_l></smr_l>\n"               //        libelle du service medical rendu
                " <gph_html></gph_html>\n"         //        facultatif cache du contenu html d'une ligne (accelere l'affichage lors reprise d'une ordo car ne passe pas par l'interpreteur)
                " <gph_ald></gph_ald>\n"           //        "ALD"/prescrit en ALD   ""/sans ALD
                " <gph_id></gph_id>\n"             //        id un code d'identification unique du produit (exemple : CIP du medicament )
                " <gph_it></gph_it>\n"             //        id_type "CIP"  pour les medicaments CIP compatibles controle interaction, "cip" pour les autres disposant d'un CIP (accessoire veto etc..)
                " <gph_dr></gph_dr>\n"             //        date et heure de la redaction de la prescription    format dd-MM-yyyy hh:mm:ss
                " <gph_dt></gph_dt>\n"             //        date et heure du debut de prise de la prescription  format dd-MM-yyyy hh:mm:ss
                " <gph_df></gph_df>\n"             //        date et heure de fin de prise de la prescription    format dd-MM-yyyy hh:mm:ss (la date de fin peut etre calculee avec <gph_cy>)
                " <gph_na></gph_na>\n"             //        nom commercial et usuel du produit
                " <gph_dcl>\n"                     //        - debut de la liste des dci composant le produit
                "  <gph_dc>\n"                     //        -- debut du premier element de la liste des dci
                "    <gph_dcn></gph_dcn>\n"        //        --- nom   du premier element de la liste des dci
                "    <gph_dcp></gph_dcp>\n"        //        --- dose  du premier element de la liste des dci
                "    <gph_dcu></gph_dcu>\n"        //        --- unite du premier element de la liste des dci
                "  </gph_dc>\n"                    //        --fin du premier element de la liste des dci
                " </gph_dcl>\n"                    //        - fin de la liste des dci composant le produit
                " <gph_cy></gph_cy>\n"             //        duree totale de toutes les sequences (J15  S10 M10   U45M:M3R2) 'J' pour jours  ou 'S' pour semaines  ou 'M' pour mois  suivi du nbr et falcutatif 'R' pour renouvelable suivi du nbr
                " <gph_sb></gph_sb>\n"             //        substituable 0 = non substituable  1 = substituable
                " <gph_dci></gph_dci>\n"           //        affichage libelle produit : 3=que dci  2=dci+commercial 1=commercial+dci 0=que commercial (par defaut si vide c'est 3)
                " <gph_uf></gph_uf>\n"             //        forme galenique de l'unite de prise
                " <gph_voie></gph_voie>\n"         //        voie d'administration
                " <gph_fmin></gph_fmin>\n"         //        frequence minimum
                " <gph_fmax></gph_fmax>\n"         //        frequence maximum
                " <gph_funit></gph_funit>\n"       //        frequence unite
                " <gph_pmin></gph_pmin>\n"         //        posologie minimum
                " <gph_pmax></gph_pmax>\n"         //        posologie maximum
                " <gph_punit></gph_punit>\n"       //        posologie unite
                " <gph_pfc></gph_pfc>\n"           //        posologie facteur corporel qu de gph_punit par facteur corporel
                " <gph_pfcunit></gph_pfcunit>\n"   //        posologie facteur corporel unite Kg cm
                " <gph_pqbyuf></gph_pqbyuf>\n"     //        quantite d'unite  posologique par unite de forme de gph_punit par gph_uf quantite d'unite posologique par unite de forme si -1 c'est forme galenique x facteur poids x poids
                " <gph_psecable></gph_psecable>\n" //        secabilite : -1/non secable 4/secable en 4 etc...
                " <gph_dmin></gph_dmin>\n"         //        duree minimum
                " <gph_dmax></gph_dmax>\n"         //        duree maximum
                " <gph_dunit></gph_dunit>\n"       //        duree unite
                " <gph_nm></gph_nm>\n"             //        conversion mode en mode numerique UP_QUANTITE MOMENT_MEAL ALL_LITERAL (on se sert de celui-ci car plus rapide a interpreter)
                " <gph_cm></gph_cm>\n"             //        conversion mode en mode literal pour information lisible de celui du mode numerique  UP_QUANTITE MOMENT_MEAL ALL_LITERAL on a pas peur des redondances ! (vive la memoire vive et les HD)
                " <gph_tr>\n"                      //        - debut de la liste des terrains por lesquels s'applique cette psologie
                "  <gph_trl>\n"                    //        -- debut du premier element de la liste des terrains
                    "<gph_trn></gph_trn>"          //        --- nom du premier element du terrain (correspond a une variable du contexte : age, poids, taille, valeur biologique ...)
                    "<gph_trs></gph_trs>"          //        --- borne inferieure
                    "<gph_tri></gph_tri>"          //        --- borne superieure
                    "<gph_tru></gph_tru>\n"        //        --- unite des bornes (an, kg, m2 cm2, mg ml UI ...)
                "  </gph_trl>\n"                   //        --fin du premier element de la liste des terrains
                " </gph_tr>\n"                     //        - fin de la liste des terrains por lesquels s'applique cette psologie
                " <gph_in>\n"                      //        - debut de la liste liste des indications
                "  <gph_il></gph_il>\n"            //        -- premier element de la liste des indications
                " </gph_in>\n"                     //        - fin de la liste des indications <gph_il> </gph_il>
                " <gph_co></gph_co>\n"             //        commentaire global en fin de prescription
                "</gph>\n"                         // -- FIN du header d'une ligne prescriptive ---
                "<gps>\n"                          // -- DEBUT de la liste des sequences prescriptives
                " <gpp></gpp>\n"                   //        premiere sequence prescriptive si plusieurs, elle sont seront separees a l'affichage par le terme 'puis, '
                "</gps>\n"                         // -- FIN de la liste des sequences prescriptives
                "<gpi_engine></gpi_engine>\n"      //        systeme de donnees a l'origine de cette ligne prescriptive
                "<gpi_user></gpi_user>\n"          //        utilisateur a l'origine de cette ligne prescriptive
                );
    //....................... la liste des indications ..............................
    //  la liste des indications se situe entre les tags  <gph_in> et </gph_in>.
    //  cette liste facultative peut etre vide
    // " <gph_in>\n"                      //        - debut de la liste liste des indications
    // "  <gph_il></gph_il>\n"            //        -- premier element de la liste des indications
    // " </gph_in>\n"                     //        - fin de la liste des indications <gph_il> </gph_il>
    // le premier element  <gph_il></gph_il> doit etre l'indication principale pour lequel le produit est prescrit
    // les autres elements <gph_il></gph_il> sont les indications secondaires pour lequel le produit est prescrit
    // le contenu de ce qu'il doit y avoir entre les tags <gph_il></gph_il> obeit a la syntaxe suivante.
    // code de l'indication | type de code | libelle de l'indication | tarif specidiquement rembourse pour cette indication (vide sinon)
    // exemple :
    //            -1971044 | DS_I | Episode dépressif majeur | 2.91
    // ou :
    //        -1971044                     est le code Datasemp de cette indication
    //         DS_I                        indique que ce code indication provient du Datasemp
    //         Episode depressif majeur    est le libelle de cette indication
    //         2.91                        est le tarif rembourse specifiquement pour cette indication
    //
    // les types de codes actuellement supportes par MedinTux :
    //    DS_I    indication datasemp
    //    DS_C    cim10 datasemp
    //    DS_A    asmr datasemp
    //    DS_S    smr datasemp
    //    TH_I    indication theriaque
    //    TH_C    cim10 theriaque
    //    TH_A    asmr theriaque
    //    TH_S    smr theriaque
    //    F       texte libre
    //....................... les sequences prescriptives ..............................
    //  Une sequence prescriptive est le terme situe entre la balise <gpp> et </gpp>.
    //  Elle decrit la quantite et repartition d'une prescription sur une periode de temps.
    //  Il est possible d'en placer plusieurs (elles seront separees par le terme ', puis'
    //  lors de l'affichage de l'ordonnance).
    //  Cela permet d'etablir des prescriptions du type :
    //     2 comprimes pendant deux jours, puis 1 comprime les jours suivants pendant un mois
    //
    // une sequence prescriptive comporte quatre parties :
    //
    // 1 -- La forme galenique : avant le crochet ouvrant '[' peut etre indiquee
    //                      la forme galenique du produit, si indiquee elle surcharge
    //                      celle indiquee en global.
    //
    // 2 -- La grammaire prescriptive : entre crochets la grammaire prescriptive est composee de un ou plusieurs
    //           points de prise : si plusieurs points de prise ils sont separes par le caractere '|'
    //           chaque point de prise est en deux parties separees par le caractere ';'
    //           - ce qui est avant le point virgule est la quantite de la forme galenique a prendre.
    //                Si valeurs separees par un tiret alors il est indique une fourchette de deux valeurs
    //                exemple (la forme galenique etant 'comprime') : 1 --> 1 comprime    2-4  --> 2 a 4 comprimes
    //           - ce qui est apres le point virgule est le moment de la prise : compose,
    //                d'un caractere unique : 'm' pour matin      'd' pour midi
    //                                        'a' pour apres-midi 's' pour soir
    //                                        'c' pour coucher    'n' pour nuit
    //                du caractere 'h' suivi d'un nombre precisant l'horaire de prise
    //                                 exemple : h10 --> a 10 heures h10-12 --> entre 10 et 12 heures
    //
    // 3 -- La duree de la sequence : apres le crochet fermant ']' est indiquee la duree de la sequence
    //           - le premier caractere peut etre :
    //             U pour mentionner une periode du type U15S: pour toutes les 15 semaines
    //             J pour jours suivi du nombre de jours
    //             S pour semaine suivi du nombre de semaine
    //             M pour mois suivi du nombre de mois
    //             A pour an suivie du nombre d'annees
    //             I suivi des jours de la semaine (lmMjvsd) ou doit etre pris le traitement
    //               exemple : IlmMjv  --> a prendre le lundi mardi mercredi jeudi vendredi
    //             E suivi des jours de la semaine (lmMjvsd) ou ne doit pas etre pris le traitement
    //               exemple : Esd     --> sauf samedi et dimanche
    //
    // 4 -- Le commentaire est apres le crochet fermant ']' et apres la duree de sequence, et il est introduit
    //      par le caractere !
    //
    // EXEMPLES :
    //
    //   <gpp>comprim\303\251[1;m|2;d|4;s]J8!a suspendre si malaise</gpp>  // 1 comprime le matin 2 comprimes a midi 4comprimes le soir pendant 8 jour a suspendre si malaise
    //   <gpp>mg[100-200;]U1!a la phase initiale en bolus</gpp>            // 100 a 200 mg a ne faire q'une fois a la phase initiale en bolus
    //   <gpp>comprime[1;m]Esd</gpp>                                       // 1 comprime le matin sauf samedi et dimanche
    //
    // 5 -- le flag de literalite conditionne le mode d'affichage des nombres et chiffres.
    //      il se trouve entre le tag xml : <gph_nm></gph_nm> est facultatif mais permet de rendre lisible le xml exple                      'UP_QUANTITE DURATION MOMENT_MEAL'
    //                      et le tag xml : <gph_cm></gph_cm> obligatoire valeur numerique qui est l'addition des modes ci-dessous exple 21  (      1    +   4    +    16     )
    //
    //       NOT_LITERAL        = 0,
    //       UP_QUANTITE        = 1,
    //       UP_JUST_FRACTION   = 2,
    //       DURATION           = 4,
    //       SEQUENCE_HOUR      = 8,
    //       MOMENT_MEAL        = 16,
    //       ALL_LITERAL        = 31

}
//---------------------------- isExistsThis_HitGrammar_InList [static] ------------------------------------------------
/*! \brief test if a grammar used for Hit list is pesent in a list of grammars
 *  \param const QString &grammar :  grammar to test
 *  \param const QStringList  &grammarList where to find imput grammar
 *  \return 1 if present / 0 if not found
*/
int C_PosologieGrammar::isExistsThis_HitGrammar_InList(const QString &grammar,  const QStringList  &grammarList)
{
  int                     nbItem = grammarList.count();
  QString toCompare_Header               = "";
  QString toCompare_Sequences            = "";
  QString toCompare_indicStr             = "";
  QString toCompare_qsp                  = "";
  QString toCompare_display              = "";
  QString toCompare_qPA_min_By_FactCorp  = "";
  QString toCompare_qPA_max_By_FactCorp  = "";
  QString toCompare_facteurCorpo         = "";
  QString toCompare_indicComment         = "";

  int  nextPos                      = 0;
  QString header                    = CGestIni::getXmlData("gph", grammar, &nextPos);
  QString sequences                 = CGestIni::getXmlData("gps", grammar, &nextPos);
  nextPos                           = 0;
  QString qsp                       = CGestIni::getXmlData("gph_cy",      header, &nextPos);
  QString display                   = CGestIni::getXmlData("gph_dci",     header, &nextPos);
  QString qPA_min_By_FactCorp       = CGestIni::getXmlData("gph_pmin",    header, &nextPos);
  QString qPA_max_By_FactCorp       = CGestIni::getXmlData("gph_pmax",    header, &nextPos);
  //QString unitePoso                 = CGestIni::getXmlData("gph_punit",   header, &nextPos);  // mg
  QString facteurCorpo              = CGestIni::getXmlData("gph_pfc",     header, &nextPos);  // 10
  //QString unite_facteurCorpo        = CGestIni::getXmlData("gph_pfcunit", header, &nextPos);  // kg m2
  //QString qPA_ByUnitPrise           = CGestIni::getXmlData("gph_pqbyuf",  header, &nextPos);  // 500 mg (par comprime)

  QString indicStr                  = CGestIni::getXmlData("gph_in",      header, &nextPos);
  QString comment                   = CGestIni::getXmlData("gph_co",      header, &nextPos);

 for (int i=0; i<nbItem; ++i)
    { nextPos                        = 0;
      toCompare_Header               = CGestIni::getXmlData("gph", grammarList[i], &nextPos);
      toCompare_Sequences            = CGestIni::getXmlData("gps", grammarList[i], &nextPos);
      nextPos                        = 0;
      toCompare_qsp                  = CGestIni::getXmlData("gph_cy",   toCompare_Header, &nextPos);
      toCompare_display              = CGestIni::getXmlData("gph_dci",  toCompare_Header, &nextPos);
      toCompare_qPA_min_By_FactCorp  = CGestIni::getXmlData("gph_pmin", toCompare_Header, &nextPos);
      toCompare_qPA_max_By_FactCorp  = CGestIni::getXmlData("gph_pmax", toCompare_Header, &nextPos);
      toCompare_facteurCorpo         = CGestIni::getXmlData("gph_pfc",  toCompare_Header, &nextPos);
      toCompare_indicStr             = CGestIni::getXmlData("gph_in",   toCompare_Header, &nextPos);
      toCompare_indicComment         = CGestIni::getXmlData("gph_co",   toCompare_Header, &nextPos);

     if (toCompare_qsp                      == qsp                   &&
         toCompare_Sequences                == sequences             &&
         toCompare_indicStr                 == indicStr              &&
         toCompare_display                  == display               &&
         toCompare_qPA_min_By_FactCorp      == qPA_min_By_FactCorp   &&
         toCompare_qPA_max_By_FactCorp      == qPA_max_By_FactCorp   &&
         toCompare_facteurCorpo             == facteurCorpo          &&
         toCompare_indicComment             == comment)  return i+1;
    }
 return 0;
}

//---------------------------- setDrugRoute [static] ------------------------------------------------
/*! \brief set the voie form string in posologie grammar.  [static]
 *  \param const QString &grammar_expression :     grammar input string which must be modified
 *  \param const QString &drugRoute :     drug route to set
 *  \return modified grammar expression.
*/
QString C_PosologieGrammar::setDrugRoute(QString &grammar_expression, const QString &drugRoute)
{ CGestIni::setXmlData("gph_voie", drugRoute.toLatin1(),  grammar_expression);
  return   grammar_expression;
}

//---------------------------- drugRoute [static] ------------------------------------------------
/*! \brief return drugRoute form string from posologie grammar. If not present return "" [static]
 *  \param const QString &grammar_expression :     grammar input string
 *  \return the drug route if present.
 */
QString C_PosologieGrammar::drugRoute(const QString &grammar_expression)
{return   CGestIni::getXmlData("gph_voie", grammar_expression);
}

//---------------------------- setUnitPriseForm [static] ------------------------------------------------
/*! \brief set the unit prise form string in posologie grammar.  [static]
 *  \param const QString &grammar_expression :     grammar input string which must be modified
 *  \param const QString &unitUPForm :     unit prise form to set
 *  \return modified grammar expression.
*/
QString C_PosologieGrammar::setUnitPriseForm(QString &grammar_expression, const QString &unitUPForm)
{ CGestIni::setXmlData("gph_uf", unitUPForm.toLatin1(),  grammar_expression);
  return   grammar_expression;
}

//---------------------------- unitPriseForm [static] ------------------------------------------------
/*! \brief return the unit prise form string from posologie grammar. If not present return "" [static]
 *  \param const QString &grammar_expression :     grammar input string as
 *  \return the unit prise form if present.
 */
QString C_PosologieGrammar::unitPriseForm(const QString &grammar_expression)
{return   CGestIni::getXmlData("gph_uf", grammar_expression);
}
//---------------------------- setId [static] ------------------------------------------------
/*! \brief set e code in posologie grammar.  [static]
 *  \param const QString &grammar_expression :     grammar input string which must be modified
 *  \param const QString &id :     id code to set (usualy e code)
 *  \return modified grammar expression.
*/
QString C_PosologieGrammar::setId(QString &grammar_expression, const QString &id)
{ CGestIni::setXmlData("gph_id", id.toLatin1(),  grammar_expression);
  return   grammar_expression;
}
//---------------------------- id [static] ------------------------------------------------
/*! \brief return the id code string from posologie grammar. If not present return "" [static]
 *  \param const QString &grammar_expression :     grammar input string as
 *  \return id code if present  (usualy e code).
*/
QString C_PosologieGrammar::id (const QString &grammar_expression)
{return   CGestIni::getXmlData("gph_id", grammar_expression);
}
//---------------------------- setAld [static] ------------------------------------------------
/*! \brief set ALD code in posologie grammar.  [static]
 *  \param const QString &grammar_expression :     grammar input string which must be modified
 *  \param const QString &ald :    ald code if present  (empty is not ALD   "ALD" is ALD).
 *  \return modified grammar expression.
*/
QString C_PosologieGrammar::setAld(QString &grammar_expression, const QString &ald)
{ CGestIni::setXmlData("gph_ald", ald.toLatin1(),  grammar_expression);
  return   grammar_expression;
}
//---------------------------- ald [static] ------------------------------------------------
/*! \brief return the idTyp code string from posologie grammar. If not present return "" [static]
 *  \param const QString &grammar_expression :     grammar input string as
 *  \return ald code if present  (empty is not ALD   "ALD" is ALD).
 */
QString C_PosologieGrammar::ald (const QString &grammar_expression)
{return   CGestIni::getXmlData("gph_ald", grammar_expression);
}

//---------------------------- pk [static] ------------------------------------------------
/*! \brief return the pk code string from posologie grammar. If not present return "" [static]
 *  \param const QString &grammar_expression :     grammar input string as
 *  \return pk code if present.
 */
QString C_PosologieGrammar::pk (const QString &grammar_expression)
{return   CGestIni::getXmlData("gph_pk", grammar_expression);
}

//---------------------------- setIdType [static] ------------------------------------------------
/*! \brief set setIdType code in posologie grammar.  [static]
 *  \param const QString &grammar_expression :     grammar input string which must be modified
 *  \param const QString &setIdType :     setIdType code to set
 *  \return modified grammar expression.
*/
QString C_PosologieGrammar::setIdType(QString &grammar_expression, const QString &id_type)
{ CGestIni::setXmlData("gph_it", id_type.toLatin1(),  grammar_expression);
  return   grammar_expression;
}

//---------------------------- idType [static] ------------------------------------------------
/*! \brief return the idTyp code string from posologie grammar. If not present return "" [static]
 *  \param const QString &grammar_expression :     grammar input string as
 *  \return idTyp code if present.
 */
QString C_PosologieGrammar::idType (const QString &grammar_expression)
{return   CGestIni::getXmlData("gph_it", grammar_expression);
}

//---------------------------- get_indicationsList [static] ------------------------------------------------
/*! \brief return indication list from grammar string [static]
 *  \param const QString &grammar_expression :     grammar input string as
 *  \return list code if present.
*/
QStringList C_PosologieGrammar::get_indicationsList(const QString &grammar_expression)
{QStringList list  = CGestIni::getXmlDataList("gph_il", grammar_expression);
 return list;
}
//---------------------------- get_TerrainList [static] ------------------------------------------------
/*! \brief return terrain list from grammar string [static]
 *  \param const QString &grammar_expression :     grammar input string as
 *  \return list code if present.
*/
QStringList C_PosologieGrammar::get_TerrainList(const QString &grammar_expression)
{QStringList list  = CGestIni::getXmlDataList("gph_trl", grammar_expression);
 return list;
}
//---------------------------- get_terrainReadyDisplayString [static] ------------------------------------------------
/*! \brief compute terrain string from grammar string [static]
 *  \param const QString &grammar_expression :     grammar input string as
 *  \return computed readable humman string.
*/
QString C_PosologieGrammar::get_terrainReadyDisplayString(const QString &grammar_expression)
{   //..................... fabriquer la chaine du terrain avec la structure du terrain................................................
    QStringList terrainList = C_PosologieGrammar::get_TerrainList(grammar_expression);
    QString     terrain     = "";
    for (int i=0; i<terrainList.size(); ++i)
        { int nextPos_t = 0;
          QString terrainLine = terrainList[i];
          QString name    = CGestIni::getXmlData("gph_trn", terrainLine, &nextPos_t);
          QString binf    = CGestIni::getXmlData("gph_tri", terrainLine, &nextPos_t);
          QString bsup    = CGestIni::getXmlData("gph_trs", terrainLine, &nextPos_t);
          QString unit    = CGestIni::getXmlData("gph_tru", terrainLine, &nextPos_t);
          if (unit=="S")          terrain += QObject::tr(" %1 : %2;").arg( name, (bsup=="F")?QObject::tr("F\303\251minin"):QObject::tr("Masculin") );
          else if (unit=="A")     terrain += " "+name +"; ";
          else if (unit.length()) terrain += QObject::tr(" %1 : de %2 \303\240 %3 %4;").arg( name, binf, bsup, unit );
        }
  return terrain;
}

//---------------------------- get_indication [static] ------------------------------------------------
/*! \brief return indication from posologie grammar.  [static]
 *  \param const QString &grammar_expression :     grammar input string which must be modified
 *  \param , int index :     index of indication to retrieve
 *  \param , QString *type=0 :     if not null type will be returned in *type
 *  \param , QString *code=0 :     if not null type will be returned in *code
 *  \return indication expression 'code'|'typCode'|'libelle'
*/
QString C_PosologieGrammar::get_indication(const QString &grammar_expression, int index, int flag /*= C_PosologieGrammar::ALL_INDIC */, QString* type /* = 0 */, QString*code/* = 0 */)
{ QStringList list  = CGestIni::getXmlDataList("gph_il", grammar_expression);
  int nb = list.count();
  if (nb==0) return "";
  QString indication = list[qMin( nb-1, index)];
  list        = indication.split('|');
  indication  = "";
  nb          = list.count();
  if (type)
     { if (nb==3)  *type= list[1];
       else        *type= "";
     }
  if (code)
     { if (nb==3)  *code= list[0];
       else        *code= "";
     }

  if ( nb>=1 && flag & C_PosologieGrammar::CODE)      indication += list[0]+"|";
  if ( nb>=2 && flag & C_PosologieGrammar::CODE_TYPE) indication += list[1]+"|";
  if ( nb>=3 && flag & C_PosologieGrammar::LIBELLE)   indication += list[2]+"|";
  if (indication.length()==0 &&  nb>=1 && flag & C_PosologieGrammar::LIBELLE) indication = indication += list[0];
  if (indication.endsWith('|')) indication = indication.left(indication.length()-1);
  return indication;
}

//---------------------------- adjustFractionalPart ------------------------------------------------
/*! \brief return adjust decimal part to near 00,25,50,75 number value
 *  \param const QString &value to adjusted string
 *  \return the adjust value.
*/
QString C_PosologieGrammar::adjustFractionalPart(const QString& value, int mode /* = C_PosologieGrammar::NO_DECIMAL */)
{
    int pos = value.indexOf(".");
    if (pos==-1) return value;

    QString decimal_part = (value+"00").mid(pos+1,2);
    QString  entire_part = value.left(pos);
    int               vd = decimal_part.toInt();
    int               vi = entire_part.toInt();

    switch(mode)
    {default :
     case  C_PosologieGrammar::NO_ADJUST:
          { return entire_part + "." + decimal_part;
          }
     case  C_PosologieGrammar::NO_DECIMAL:
          { if      (vd<50)             return entire_part;
            else                        return (QString::number(vi+1));
          }
     case C_PosologieGrammar::FRACT_DEFAULT:           // 0 = valeur si non definie dans le tag on decide d'une secabilite en 4
     case C_PosologieGrammar::FRACT_25:
           { if (vd<25)                 return entire_part;
             else if (vd>=25 && vd<50)  return entire_part + ".25";
             else if (vd>=50 && vd<75)  return entire_part + ".50";
             else if (vd>=75 )          return entire_part + ".75";
           }
      case C_PosologieGrammar::FRACT_50:
          { if      (vd<50)            return entire_part;
            else if (vd>=50 && vd<75)  return entire_part + ".50";
            else                       return (QString::number(vi+1));
          }
    } // end switch
}

//---------------------------- totalDurationInDaysFromGrammar ------------------------------------------------
/*! \brief compute the total duration of a prescription in days
 *  \param  const QString &grammar grammar to analyze
 *  \param  int withRepeat compute with repeat mention.
 *  \param  int withSequences compute with sequences durations (default=0)
 *  \return number of days
*/
double C_PosologieGrammar::totalDurationInDaysFromGrammar( const QString &grammar , int withRepeat, int withSequences /*=0*/)
{    int nbCycle;
     int nextPos = 0;
     double global_duration  =  totalGlobalDurationInDays   (  CGestIni::getXmlData("gph_cy",   grammar, &nextPos) , withRepeat, &nbCycle);
     if (withSequences==0) return global_duration;
     double seqence_duration =  totalSequencesDurationInDays(  CGestIni::getXmlDataList( "gpp", grammar, &nextPos) , nbCycle);
     return qMax(global_duration, seqence_duration);
}
//---------------------------- totalSequencesDurationInDays ------------------------------------------------
/*! \brief  compute the total duration of all sequences in days
 *  \param  QString duration sequence to analyze
 *  \param  int nbCycle compute with repeat mention.
 *  \return double number of days
*/
double C_PosologieGrammar::totalSequencesDurationInDays( const QStringList &seqList, int nbCycle)
{   // <gpp>[1;m|2;d|4;s]J8!\303\240 la phase initiale en bolus</gpp>
    // <gpp>[100-200;]U15S:!\303\240 la phase initiale en bolus</gpp>   // 100 a 200 mg \303\240 ne faire q'une fois \303\240 la phase initiale en bolus
    QString sequence  = "";
    QString seq_days  = "";
    double    nb_j    = 0.0;
    for ( int s=0; s < seqList.count();  ++s)       // < 1  car l'ancien systeme ne connait pas la crise des sequences multiples
        { sequence                     = seqList[s];
          int     pos_deb              = sequence.indexOf(']');
          if (pos_deb == -1 )          return 0;
          seq_days                     = sequence.mid ( ++pos_deb );
          int  pos_end                 = seq_days.indexOf('!');
          if (pos_end != -1 ) seq_days.truncate(pos_end);
          if (seq_days.length() )
             {
               if ( (seq_days[0].toLatin1()|32)=='u')
                  { pos_deb = seq_days.indexOf(':');
                    if (pos_deb==-1)  seq_days.clear();
                    else              seq_days = seq_days.mid( ++pos_deb );
                  }
               if ( seq_days.length() )
                  { nb_j += totalGlobalDurationInDays(  seq_days, 0);
                  }
             }
        }
    if (nbCycle) return nb_j * nbCycle;
    else         return nb_j;
}

//---------------------------- adjustGrammarSequencesToCorporalFactor ------------------------------------------------
/*! \brief  adjust number of UP (dosage-unit in french Prise Unit) in sequences, from corporal factor (Kg M2)
 *  \param  const QString &grammar grammar to analyze
 *  \param  double poids   Corporal Factor weight
 *  \param  double surface Corporal Factor surface
 *  \param  QString &sequencesXml XML description of sequences list
 *  \param  int doNotAdjustXMLSequences if 1/return only the minimum and maximum total quantity dosage-uni otherwise sequencesXml is adjusted
 *  \return QString the minimum and maximum total quantity dosage-unit (in french Unit Prise UP) minimum-maximum as 2-6
*/
QString C_PosologieGrammar::adjustGrammarSequencesToCorporalFactor( const QString &grammar, double poids, double surface,  QString &sequencesXml , int doNotAdjustXMLSequences )
{   //............ ajuste les nombre d'unite de prise dans les sequences selon le poids ou surface .................................
    int           nextPos             = 0;
    QString forme                     = CGestIni::getXmlData("gph_uf",      grammar, &nextPos);     // n      forme
    QString qPA_min_By_FactCorp       = CGestIni::getXmlData("gph_pmin",    grammar, &nextPos);     // n       QPA_min  Quantite de Principe Actif minimum
    QString qPA_max_By_FactCorp       = CGestIni::getXmlData("gph_pmax",    grammar, &nextPos);     // N       QPA_max  Quantite de Principe Actif maximum
    QString unitePoso                 = CGestIni::getXmlData("gph_punit",   grammar, &nextPos);     // mg      UPA      unite du Principe actif
    QString facteurCorpo              = CGestIni::getXmlData("gph_pfc",     grammar, &nextPos);     // 10      QFC      quantite de facteur corporel
    //..................... si facteur corporel :ajuster la poso totale (en nombre UP)  selon le poids ou surface..........................
    //                      pour cela : on prend le poids ou surface X par la Qu par facteur corporel
    //                      divise par la QPA_by_UP (Quantite de Principe Actif) par UP (Unite de Prise)  pour obtenir la dose totale d'UP
    if (facteurCorpo.length()==0 || (qPA_min_By_FactCorp.length()==0 && qPA_max_By_FactCorp.length()==0 )) return "";     // >>>>  ON SE CASSE CAR NON EXPRIME EN FACTEUR CORPOREL
    QString unite_facteurCorpo        = CGestIni::getXmlData("gph_pfcunit",   grammar, &nextPos);             // kg m2   UFC      unite du facteur corporel (kg, m2)  --> dose totale =  n a N mg par 10 kg ou M2
    QString qPA_ByUnitPrise           = CGestIni::getXmlData("gph_pqbyuf",    grammar, &nextPos);             // 500 mg  QPA_by_UP 500 mg par (par comprime)  (Quantite de Principe Actif) par UP (Unite de Prise)
    int     gph_psecable              = CGestIni::getXmlData("gph_psecable",  grammar, &nextPos).toInt();     // secabilite : -1/non secable 4/secable en 4 etc...
    QString posoTotale                = "";
    double quFactCorp                 = 0;
    QStringList seqList               = CGestIni::getXmlDataList( "gpp",    sequencesXml);

    if ( unite_facteurCorpo.length() )
       { if ( unite_facteurCorpo.at(0).toUpper()=='K') quFactCorp = poids;
         else                                          quFactCorp = surface;
       }
    double nb_total_max = 0;
    double nb_total_min = 0;
    if (qPA_ByUnitPrise=="-1")
       { nb_total_max = (  qPA_max_By_FactCorp.toDouble() * quFactCorp )/facteurCorpo.toDouble();
         nb_total_min = (  qPA_min_By_FactCorp.toDouble() * quFactCorp )/facteurCorpo.toDouble();
       }
    else
       { nb_total_max = (  qPA_max_By_FactCorp.toDouble() * quFactCorp )/ (qPA_ByUnitPrise.toDouble()*facteurCorpo.toDouble());
         nb_total_min = (  qPA_min_By_FactCorp.toDouble() * quFactCorp )/ (qPA_ByUnitPrise.toDouble()*facteurCorpo.toDouble());
       }
    QString nbMax = QString::number(nb_total_max,'f',2);
    nbMax         = C_PosologieGrammar::adjustFractionalPart(nbMax, gph_psecable);
    QString nbMin = QString::number(nb_total_min,'f',2);
    nbMin         = C_PosologieGrammar::adjustFractionalPart(nbMin, gph_psecable);
    //............. ajuster la chaine de la dose totale a placer dans le widget .......................
    if      (qPA_min_By_FactCorp==qPA_max_By_FactCorp)                         posoTotale = nbMax;
    else if (qPA_min_By_FactCorp.length()==0 && qPA_max_By_FactCorp.length())  posoTotale = nbMax;
    else if (qPA_max_By_FactCorp.length()==0 && qPA_min_By_FactCorp.length())  posoTotale = nbMin;
    else                                                                       posoTotale = nbMin + "-" + nbMax;
    if (doNotAdjustXMLSequences == C_PosologieGrammar::JUST_ADJUST_TOTAL_AMOUNT)   return posoTotale;
    //................. on a le total Maximum et Minimum possible du nombre d'UP selon le poids ou surface....................................
    //                  il faut maintenant exploser les sequences, pour le repartir dans chacune des sequences
    //                  ne mollissons pas,  moi a pas peur et tout le fourbi, on va y arriver
    // <gpp>[1;m|2;d|4;s]J8!\303\240 la phase initiale en bolus</gpp>
    // <gpp>[100-200;]U1!\303\240 la phase initiale en bolus</gpp>   // 100 \303\240 200 mg \303\240 ne faire q'une fois \303\240 la phase initiale en bolus

    QString           sequence    = "";
    QString str_before_bracket    = "";
    QString str_after_bracket     = "";
    double  nb_prise_point        = 0;
    double  qu_by_point_min       = 0;
    double  qu_by_point_max       = 0;
    QString s_qu_by_point_min     = "";
    QString s_qu_by_point_max     = "";
    QString poso                  = "";
    QString  s_when               = "";
    QString  s_point              = "";
    double   mult_by              = 0;
    QString tmp_str               = "";
    int     s                     = 0;
    QStringList point_list;
    for ( s=0; s < seqList.size();  ++s)
        { s_point                     = "";
          mult_by                     = 0;
          sequence                    = seqList[s];
          int     pos_deb             = sequence.indexOf('[');
          if (pos_deb == -1 )        continue;
          str_before_bracket          = sequence.left(pos_deb);
          ++ pos_deb;
          sequence                    = sequence.mid ( pos_deb );
          int     pos_end             = sequence.indexOf(']');
          if (pos_end == -1 )        continue;
          str_after_bracket           = sequence.mid ( pos_end+1 );
          sequence                    = sequence.left( pos_end );
          //.......... la on a isole : 1;m|2;d|4;s .......................................
          //           on pinaille pas et soit on repartit egalement entre chaque point de prise deja presents.
          //           soit on utilise une methode de calcul indiquee sur chaque point de prise (pour l'instant un facteur multiplicatif)
          point_list                  = sequence.split("|", QString::SkipEmptyParts);
          nb_prise_point              = point_list.size();
          if ( point_list.at(0).length() )
             { //............. methode proportionnelle (un facteur multiplicatif de la QU totale est place sur chaque point de prise) ...................
               //              x0.5;m|x0.25;d|x0.25;s
               if (sequence.at(0).toLower()=='x')
                  { sequence    = "";
                    for ( int i = 0; i < point_list.size();  ++i)
                        { s_when   = "";
                          mult_by  = 1 ;
                          s_point  = point_list.at(0).mid(1);  // .mid(1) pour sauter le X
                          int   p  = s_point.indexOf(';');
                          if  ( p != -1)
                              { s_when  = s_point.mid  (p);
                                mult_by = s_point.left (p).toDouble();
                              }
                          qu_by_point_min    = nb_total_min * mult_by;
                          qu_by_point_max    = nb_total_max * mult_by;
                          s_qu_by_point_min  = C_PosologieGrammar::adjustFractionalPart(QString::number(qu_by_point_min, 'f', 2), gph_psecable) ;
                          s_qu_by_point_max  = C_PosologieGrammar::adjustFractionalPart(QString::number(qu_by_point_max, 'f', 2), gph_psecable) ;
                          //............. ajuster la chaine a placer comme quantite ...............................
                          //              soit valeur unique soit fourchette de valeurs
                          if      (qu_by_point_min    == qu_by_point_max)       poso = s_qu_by_point_max;
                          else if (qu_by_point_min==0 && qu_by_point_max)       poso = s_qu_by_point_max;
                          else if (qu_by_point_min    && qu_by_point_max==0)    poso = s_qu_by_point_min;
                          else                                                  poso = s_qu_by_point_min + "-" + s_qu_by_point_max;
                          sequence += poso + s_when +"|";
                        } // end for ( int i=0; i < point_list.size();  ++i)
                    if (sequence.endsWith('|')) sequence.chop(1);     // virer le dernier |
                  }  // end if (point_list.at(0).toLower()=='x')
               //............. methode equi valeur  (la QU totale est repartie uniformement entre chaque point de prise) ................................
               // 1;m|1;d|1;s    // quelque soit la valeur avant le ; elle sera remplacee par la QU totale / nb de prises
               else
                  { sequence           = "";
                    qu_by_point_min    = nb_total_min / nb_prise_point;
                    qu_by_point_max    = nb_total_max / nb_prise_point;
                    s_qu_by_point_min  = C_PosologieGrammar::adjustFractionalPart(QString::number(qu_by_point_min, 'f', 2), gph_psecable) ;
                    s_qu_by_point_max  = C_PosologieGrammar::adjustFractionalPart(QString::number(qu_by_point_max, 'f', 2), gph_psecable) ;
                    //............. ajuster la chaine a placer comme quantite ...............................
                    //              soit valeur unique soit fourchette de valeurs
                    if      (qu_by_point_min    == qu_by_point_max)             poso = s_qu_by_point_max;
                    else if (qu_by_point_min==0 && qu_by_point_max)             poso = s_qu_by_point_max;
                    else if (qu_by_point_min    && qu_by_point_max==0)          poso = s_qu_by_point_min;
                    else                                                        poso = s_qu_by_point_min + "-" + s_qu_by_point_max;
                    //.......... recontruire la sequence ...................................
                    for ( int i=0; i < point_list.size();  ++i)
                        { tmp_str  = point_list.at(i);
                          int   p  = tmp_str.indexOf(';');
                          if  ( p != -1)
                              { sequence += poso +tmp_str.mid(p)+"|"; // on garde l'ancienne specification du moment de prise  m/le matin d/a midi a/l'aprs midi s/le soir c/coucher n/nuit n/horaire
                              }
                        }  //end for ( int i=0; i < point_list.size();  ++i)
                  } //end else if (point_list.at(0).toLower()=='x')
               if (sequence.endsWith('|')) sequence.chop(1);     // virer le dernier |
             }
          seqList[s] = str_before_bracket + "[" + sequence + "]" + str_after_bracket;
        } // end for ( s=0; s < seqList.size();  ++s)
    //............. refaire le XML des sequences ......................................................
    //  "<gps>\n"                          // -- DEBUT de la liste des sequences pescriptives
    //  " <gpp></gpp>\n"                   //        premiere sequence prescriptive
    //  "</gps>\n"                         // -- FIN de la liste des sequences pescriptives
    /*
          *      <gps>
          *         <gpp>[1;m|2;d|4;s]J8</gpp>
          *         <gpp>[0.5;m|1;d15r|2;sr30|3;h10-13]J4\</gpp>
          *         <gpp>[0.5;m|1;s]J3</gpp>
          *      </gps>
    */
    sequencesXml = "\n";
    if (seqList.size()==0)
       { if (qPA_ByUnitPrise=="-1")
            sequencesXml += " <gpp>"+forme+"[" + posoTotale + ";]</gpp>\n";
         else
            sequencesXml += " <gpp>"+unitePoso+"[" + posoTotale + ";]</gpp>\n";
       }
    else
       { for ( s=0; s < seqList.size();  ++s)
             {sequencesXml += " <gpp>" + seqList.at(s) + "</gpp>\n";
             }
       }
    return posoTotale;
}

//---------------------------- qu_total_max_UP [static] ------------------------------------------------
/*! \brief  retrieve maximum of UP of a prescription
 *  \param  const QString &grammar_expression
 *  \param  renew  number of prescription renew zero by default
 *  \param  p_q_min pointer on double to return minimum of UP of a prescription
 *  \return double as maximum of UP of a prescription
 */
double C_PosologieGrammar::qu_total_max_UP(const QString &grammar, int renew /* = 0 */, double *p_q_min /* = 0 */)
{   int          nextPos              = 0 ;
    QString      duration             = CGestIni::getXmlData( "gph_cy",      grammar, &nextPos);
    QStringList  seqList              = CGestIni::getXmlDataList( "gpp",     grammar, &nextPos);
    double global_durationInMinutes   = (C_PosologieGrammar::totalGlobalDurationInDays( duration , renew)*1440);
    return qu_total_max_UP( global_durationInMinutes, seqList, renew , p_q_min );
}
//---------------------------- qu_total_max_UP [static] ------------------------------------------------
/*! \brief  retrieve maximum of UP of a prescription
 *  \param  global_durationInMinutes global duration
 *  \param  seqList_str sequences list
 *  \param  renew  number of prescription renew zero by default
 *  \param  p_q_min pointer on double to return minimum of UP of a prescription
 *  \return double as maximum of UP of a prescription
 */
double C_PosologieGrammar::qu_total_max_UP(double global_durationInMinutes, const QString &seqList_str, int renew /* = 0 */, double *p_q_min /* = 0 */)
{ QStringList seqList   = CGestIni::getXmlDataList( "gpp",    seqList_str);
  return qu_total_max_UP( global_durationInMinutes, seqList, renew , p_q_min );
}

//---------------------------- qu_total_max_UP [static] ------------------------------------------------
/*! \brief  retrieve maximum of UP of a prescription
 *  \param  global_durationInMinutes global duration
 *  \param  seqList sequences list
 *  \param  renew  number of prescription renew zero by default
 *  \param  p_q_min pointer on double to return minimum of UP of a prescription
 *  \return double as maximum of UP of a prescription
 */
double C_PosologieGrammar::qu_total_max_UP(double global_durationInMinutes, const QStringList  &seqList, int renew /* = 0 */, double *p_q_min /* = 0 */)
{
    Q_UNUSED (renew);
    double    q_min             = 0;
    double    q_max             = 0;
    int up_frequency            = -1;
    double              d_min   = 0;
    double              d_max   = 0;
    int    d_by_UT_inMinutes    = 0;
    int    last_nb_repetitions  = 0;
    QString         sequence    = "";
    QString seq_days            = "";
    int  last_sequence_duration = 0;
    int nb_sequences            = seqList.count();
    for ( int s=0; s < nb_sequences;  ++s)       // < 1  car l'ancien systeme ne connait pas la crise des sequences multiples
        { //....... reperer si la sequence comporte une forme locale ..............
          //        et auquel cas l'utiliser
          sequence                    = seqList[s];
          int     pos_deb             = sequence.indexOf('[');
          if (pos_deb == -1 )         return -1;
          sequence                    = sequence.mid ( pos_deb+1 );
          int     pos_end             = sequence.indexOf(']');
          if (pos_end == -1 )         return -1;
          seq_days                    = sequence.mid ( pos_end+1 );
          sequence                    = sequence.left( pos_end );
                  pos_end             = seq_days.indexOf('!');
          if (pos_end != -1 )seq_days = seq_days.left( pos_end );

          last_sequence_duration      = 0;
          last_nb_repetitions         = nombreDeRepetitionDeLaSequence( seq_days, last_sequence_duration );           // nombre de fois que [0.5;m|1;d15r|2;sr30|3;h10-13]           se reproduit sur la periode de temps de la sequence
          up_frequency                = qMax( up_frequency , sequence_timeToMinMax_UP( sequence, d_min,  d_max ) );   // nombre min et max d'UP dans [0.5;m|1;d15r|2;sr30|3;h10-13]
          if ( s<nb_sequences-1 )    // la derniere sequence est soumise a la duree globale de la prescription
             { d_by_UT_inMinutes     += last_sequence_duration;
               q_min                 += qMin(d_min, d_max)*last_nb_repetitions;
               q_max                 += qMax(d_min, d_max)*last_nb_repetitions;
             }
        }
    //...... la derniere sequence doit s'ajuster a la duree globale............................
    if ( global_durationInMinutes > d_by_UT_inMinutes ) global_durationInMinutes -= d_by_UT_inMinutes;
    if ( last_sequence_duration == 0)  last_sequence_duration = 1440;      // un jour par defaut si non precise
    if ( last_sequence_duration > global_durationInMinutes ) global_durationInMinutes = last_sequence_duration;
    last_nb_repetitions    = (global_durationInMinutes/last_sequence_duration) * last_nb_repetitions;

    q_min                 += qMin(d_min, d_max)*last_nb_repetitions;
    q_max                 += qMax(d_min, d_max)*last_nb_repetitions;
    if (p_q_min)  *p_q_min = q_min;
    return q_max;
}
//---------------------------- nombreDeRepetitionDeLaSequence [static] ------------------------------------------------
/*! \brief  nombre de fois que la sequence se repete
 *  \param  const QString &seq_days to analyze as   J8 or M12
 *  \return int nombre de fois que la sequence se repete
*/
int C_PosologieGrammar::nombreDeRepetitionDeLaSequence(const QString &seq_days, int &period_in_minutes)
{
 // [1;m]U1J:J2 @application[2;m]    cutanée@application[1;m]J2 @application[2;m]    [1;m] @application[2;m]
 if ( seq_days.length()==0 )         // par defaut shema une fois par sequence d'une duree de 1 jour [1;m]
    { period_in_minutes +=  1440;
      return  1;
    }
 int nb = 0;
 char              t = seq_days[0].toLatin1()|32;
 switch(t)
 { case 'i': // IlmMjvsd I comme Include par JOUR le lundi mardi ....
     period_in_minutes +=  1440*7;
     return seq_days.mid(1).length();
   case 'e':
     period_in_minutes +=  1440*7;
     return 7 - seq_days.mid(1).length();
   case 'u': // je suis non fumeur pourtant .....j'y comprends rien .......
       { int pos_deb = seq_days.indexOf(':');
         if (pos_deb==-1)  return 0;
         int _period_in_minutes = 0;
         int ret = nombreDeRepetitionDeLaSequence( seq_days.mid( ++pos_deb ) , _period_in_minutes); // un peu de recursivite
         period_in_minutes     += _period_in_minutes;
         return ret;
       }
   case 'j':
     nb =  seq_days.mid(1).toInt();
     period_in_minutes +=  1440*nb;
     return  nb;
   case 's':
     nb =  seq_days.mid(1).toInt();
     period_in_minutes +=  1440*7*nb;
     return  nb * 7;
   case 'm':
     nb =  seq_days.mid(1).toInt();
     period_in_minutes +=  1440*30*nb;
     return  nb * 30;
   case 'a':
     nb =  seq_days.mid(1).toInt();
     period_in_minutes +=  1440*365*nb;
     return  nb * 365;
 }
 return 0;
}

//---------------------------- totalGlobalDurationInDays ------------------------------------------------
/*! \brief  compute the total duration of a prescription in days
 *  \param  QString duration sequence to analyze
 *  \param  int withRepeat compute with repeat mention.
 *  \param   int *retCycle if not null, cycle number will be returned.
 *  \return number of days
*/
double C_PosologieGrammar::totalGlobalDurationInDays(  QString duration , int withRepeat, int *retCycle /*=0*/)
{      if (retCycle) *retCycle = 0;
       if (duration.length()==0)   return 0;
       double nb_j      = 0;
       int    cycle     = 0;
       int    pos       = duration.indexOf('R');
       if ( pos != -1 )
          { cycle       = duration.mid(  pos + 1 ).toInt();
            duration    = duration.left( pos );
          }
       if (retCycle) *retCycle = cycle;
       if ( duration.length() )
          { switch (duration[0].toLatin1()|32)
                  {case 'j': nb_j  =    duration.mid(1).toDouble() * 1;      break;
                   case 's': nb_j  =    duration.mid(1).toDouble() * 7;      break;
                   case 'm': nb_j  =    duration.mid(1).toDouble() * 30.5;   break;
                   case 'a': nb_j  =    duration.mid(1).toDouble() * 365.25; break;
                  }
         }
       if (withRepeat && cycle) return nb_j * cycle;
       else                     return nb_j;
}
//---------------------------- toMinMax_UP ------------------------------------------------
/*! \brief  retrieve frequence of UP, minimum and maximum quantity of UP  by grammar
 *  \param  const QStringList &seqList list of sequences to analyze
 *  \param  QString &q_min minimum quantity of UP from this sequence
 *  \param  QString &q_max maximum quantity of UP from this sequence
 *  \param  QString &q_max maximum quantity of UP from this sequence
 *  \return QString the maximum frequence of UP by grammar: qmin|
*/
int C_PosologieGrammar::toMinMax_UP(const QStringList &seqList, double &q_min,  double &q_max) //, QString &up_formes_min_max_period)
{
    // <gpp>[1;m|2;d|4;s]J8!lors de la phase initiale en bolus</gpp>
    // <gpp>[100-200;]U1!lors de la phase initiale en bolus</gpp>   // 100 a 200 mg \303\240 ne faire q'une fois \303\240 la phase initiale en bolus
    int up_frequency          = -1;
    double              d_min = 0;
    double              d_max = 0;
    int    d_by_UT_inMinutes  = -1;
    QString         sequence  = "";
    QString seq_days          = "";
    for ( int s=0; s < seqList.count();  ++s)       // < 1  car l'ancien systeme ne connait pas la crise des sequences multiples
        { //....... reperer si la sequence comporte une forme locale ..............
          //        et auquel cas l'utiliser
          sequence                    = seqList[s];
          int     pos_deb             = sequence.indexOf('[');
          if (pos_deb == -1 )         return -1;
          sequence                    = sequence.mid ( pos_deb+1 );
          int     pos_end             = sequence.indexOf(']');
          if (pos_end == -1 )         return -1;
          seq_days                    = sequence.mid ( pos_end+1 );
          sequence                    = sequence.left( pos_end );
                  pos_end             = seq_days.indexOf('!');
          if (pos_end != -1 )seq_days = seq_days.left( pos_end );
          d_by_UT_inMinutes           = sequence_daysToUnitPeriodInMinutes( seq_days );
          up_frequency                = qMax( up_frequency , sequence_timeToMinMax_UP( sequence, d_min,  d_max ) );
          q_min                       = qMin( qMin(d_min, d_max) , q_min);
          q_max                       = qMax( qMax(d_min, d_max) , q_max);
        }
    return up_frequency;
}
//---------------------------- sequence_daysToUnitPeriodInMinutes ------------------------------------------------
/*! \brief  compute a rough unit period of this prescription sequence (if one day by week --> 1440 (day) is returned)
 *  \param  const QString &seq_days to analyze as   J8 or M12
 *  \return int the period of this  UP's sequence in minute
*/
int C_PosologieGrammar::sequence_daysToUnitPeriodInMinutes(const QString &seq_days)
{if (seq_days.length()==0) return 0;
 char              t = seq_days[0].toLatin1()|32;
 switch(t)
 { case 'i':  return  1440;    // IlmMjvsd I comme Include
   case 'e':  return  1440;    // ElmMjvsd E comme Exclude
   case 'u': // je suis non fumeur pourtant .....j'y comprends rien .......
       { int pos_deb = seq_days.indexOf(':');
         if (pos_deb==-1)  return 0;
         int ret = sequence_daysToUnitPeriodInMinutes( seq_days.mid( ++pos_deb ) ); // un peu de recursivite
         return ret;
       }
   case 'j':  return  1440;
   case 's':  return  1440 * 7;
   case 'm':  return  1440 * 30;
   case 'a':  return  1440 * 365;
   default :  return  0;
 }
}

//---------------------------- sequence_timeToMinMax_UP ------------------------------------------------
/*! \brief  retrieve frequence of UP, minimum and maximum quantity of UP  by sequence
 *  \param  const QString &seq_time to analyze as  <b>0.5;m|1;d15r|2;sr30|3;h10-13</b>
 *  \param  QString &q_min minimum quantity of UP from this sequence
 *  \param  QString &q_max maximum quantity of UP from this sequence
 *  \return QString the frequence of UP by sequence
*/
QString C_PosologieGrammar::sequence_timeToMinMax_UP(const QString &seq_time,  QString &q_min,  QString &q_max )
{double d_min;
 double d_max;
 double up_frequency = sequence_timeToMinMax_UP(seq_time,  d_min,  d_max);
 //......... preparer les sorties ...............
 if (d_min>0)          q_min  = QString::number(d_min,'f',2);
 else                  q_min  = "";
 if (d_max>0)          q_max  = QString::number(d_max,'f',2);
 else                  q_max  = "";
 return QString::number(up_frequency);
}

//---------------------------- sequence_timeToMinMax_UP [static]------------------------------------------------
/*! \brief  retrieve frequence of UP, minimum and maximum quantity of UP  by sequence
 *  \param  const QString &seq_time to analyze as  <b>0.5;m|1;d15r|2;sr30|3;h10-13</b>
 *  \param  double &q_min minimum quantity of UP from this sequence
 *  \param  double &q_max maximum quantity of UP from this sequence
 *  \return double the frequence of UP by sequence
*/
int C_PosologieGrammar::sequence_timeToMinMax_UP(const QString &seq_time,  double &q_min,  double &q_max )
{QStringList seqTList = seq_time.split("|");    //  bool allowEmptyEntries
 QString          str = "";
 double         t_min = 0;
 double         t_max = 0;
 double         d_min = 0;
 double         d_max = 0;
 double         d_tmp = 0;
 int              pos = 0;
 bool    isFirstTime  = true;
 for (int i=0; i< (int)seqTList.count(); ++i)
    {str        = seqTList[i];
     pos        = str.indexOf(';');
     if (pos   != -1) str = str.left(pos);
     if (str.length()==0) continue;
     //.......... CMedicaBase::UP_QUANTITE la quantite d'unite de prise .....................
     pos       = str.indexOf ('-');
     if ( pos != -1 )                // cas ou 1-2   (un a deux)
        { t_min  = str.left ( pos ).toDouble();
          t_max  = str.mid  ( pos + 1).toDouble();
          if (t_min>t_max) {d_tmp = t_max; t_max = t_min; t_min = d_tmp;}
        }
     else
        { t_max  = str.toDouble();
        }
     if (isFirstTime)        // au premier passage on prend les valeurs
        { d_min = t_min;
          d_max = t_max;
        }
     else                    // aux passages suivant on additione les valeurs
        { d_min =  t_min + d_min ;
          d_max =  t_max + d_max ;
        }
     isFirstTime = false;
     //.......... on releve le max et min trouve ..........................
    }  // end for (int i=0; i<seqTList.count(); ++i)
 //......... preparer les sorties ...............
 q_min  = d_min;
 q_max  = d_max;
 return seqTList.size();
}
//------------------------ pair_divBy [static]---------------------------------------
/*! \brief return a divided by valueDOUBLE_PAIR
 *  \param const DOUBLE_PAIR &val_pair pair to divide
 *  \param double div value by wich
 *  \return DOUBLE_PAIR divided by div.
*/
DOUBLE_PAIR C_PosologieGrammar::pair_divBy(const DOUBLE_PAIR &val_pair, double div)
{DOUBLE_PAIR ret;
 if (div==0) { qDebug() << "C_PosologieGrammar::pair_divBy(ZERO)"; return ret; }
 ret.v1 = val_pair.v1/div;
 ret.v2 = val_pair.v2/div;
 return ret;
}
//------------------------ p_Str [static]---------------------------------------
/*! \brief return formated string : if no decimal part just entire part is returned,
 *  \param double val to convert
 *  \param int nbDec number of zero after point
 *  \return QString converted string
*/
QString C_PosologieGrammar::p_Str(double val, int nbDec)
{ QString zer = "000000000000000000000000000000000000000";
  QString str = QString::number(val,'f',nbDec);
  int     pos = str.indexOf('.');
  zer.truncate(nbDec);
  if (pos !=-1 && zer==str.mid(pos+1)) return str.left(pos);
  return str;
}
//------------------------ strTo_DOUBLE_PAIR [static] ---------------------------------------
/*! \brief return DOUBLE_PAIR  structure initialised with input QString  as folows 0.123-5.236
 *  \param QString to convert
 *  \return DOUBLE_PAIR structure
*/
DOUBLE_PAIR C_PosologieGrammar::strTo_DOUBLE_PAIR(const QString &qu)
{   DOUBLE_PAIR ret;
    int pos  = qu.indexOf('-');
    if (pos != -1)
      {  ret.v1     = qu.left(pos).toDouble();
         ret.v2     = qu.mid(pos+1).toDouble();
         ret.isPair = 1;
      }
    else
      {  ret.v1     = 0;
         ret.v2     = qu.toDouble();
         ret.isPair = 0;
      }
    return ret;
}

//---------------------------- fuse ------------------------------------------------
QString C_PosologieGrammar::fuse  (const QString &grammar_expression               ,
                                   QString mask                                   ,
                                   const QString &substituableImgPath    /* = "" */,
                                   const QString num_Ordre               /* = "" */
                                  )
{
        int nextPos              = 0;
        QString    total_price   = CGestIni::getXmlData(     "total_price",    grammar_expression, &nextPos);
        QString    nb_boites     = CGestIni::getXmlData(     "nb_boites",      grammar_expression, &nextPos);
        QString    s_dateRed     = CGestIni::getXmlData(     "gph_dr",         grammar_expression, &nextPos).left(10);
        QString    s_dateDeb     = CGestIni::getXmlData(     "gph_dt",         grammar_expression, &nextPos).left(10);
        QString commerialName    = CGestIni::getXmlData(     "gph_na",         grammar_expression, &nextPos);
        QString compoXmlParts    = CGestIni::getXmlData(     "gph_dcl",        grammar_expression, &nextPos);
        QString cycle            = CGestIni::getXmlData(     "gph_cy",         grammar_expression, &nextPos);
        QString substituable     = CGestIni::getXmlData(     "gph_sb",         grammar_expression, &nextPos);
        int     dciMode          = CGestIni::getXmlData(     "gph_dci",        grammar_expression, &nextPos).toInt();
        QString up_forme         = CGestIni::getXmlData(     "gph_uf",         grammar_expression, &nextPos);
        QString voie             = CGestIni::getXmlData(     "gph_voie",       grammar_expression, &nextPos);
        QString conv_mod_T       = CGestIni::getXmlData(     "gph_cm",         grammar_expression, &nextPos);
        QString conv_mod_N       = CGestIni::getXmlData(     "gph_nm",         grammar_expression, &nextPos);
        QString comment          = CGestIni::getXmlData(     "gph_co",         grammar_expression, &nextPos);
        QStringList seqList      = CGestIni::getXmlDataList( "gpp",            grammar_expression, &nextPos);
        int must_be_literal      = -1;
     //........ traiter le cas ou la definition de literalite est donnee dans la grammaire avant le signe = .........
     if (conv_mod_T.length())          // .......... si literalite definie dans grammaire l'utiliser que si parametre d'entree non defini ..............
        { if (conv_mod_N.length()) must_be_literal = conv_mod_N.toInt();                                    // court
          else                     must_be_literal = C_PosologieGrammar::literalStringToFlag(conv_mod_T);   // long
        }
     if (must_be_literal==-1)
        {  must_be_literal = C_PosologieGrammar::NOT_LITERAL;
        }
     //............. si forme definie en entree on l'utilise et elle surcharge celle de la grammaire .....................................
     //              sinon on tente d'utiliser celle d\303\251finie dans la grammaire
     nextPos              = 0;
     QString joinStr      = QObject::tr(", \npuis ");
     QString text         = "";
     for (int i=0; i<seqList.count(); ++i)
         { //....... reperer si la sequence comporte une forme locale ..............
           //        et auquel cas l'utiliser
           QString sequence        = seqList[i];
           int     pos_deb         = sequence.indexOf('@');
           if (pos_deb != -1 )
              { voie               = sequence.left(pos_deb);
                sequence           = sequence.mid(pos_deb+1);
              }
                   pos_deb         = sequence.indexOf('[');
           if (pos_deb != -1 )
              { up_forme           = sequence.left(pos_deb);
                sequence           = sequence.mid(pos_deb+1);
              }
           text  += sequenceToString(i, sequence, must_be_literal, up_forme); text += joinStr;
         }
     text.chop(joinStr.length());    // on enleve le dernier "\npuis "
     mask.replace("{{posologie}}",     text);
     //...........  commencer le ..................................
     text           = "";
     if (s_dateRed != s_dateDeb && QDate::fromString(s_dateDeb.left(10),"dd-MM-yyyy") > QDate::fromString(s_dateRed.left(10),"dd-MM-yyyy"))
        {                              text += QObject::tr(", \303\240 commencer le : ") + s_dateDeb.left(10);
        }
     mask.replace("{{commencer_le}}",  text);
     //...........  duree totale ..................................
     text           = "";
     if (cycle.length())               text += dureeTotaleRenouvToString(cycle, must_be_literal );
     mask.replace("{{duree_totale}}",         text);
     //...........  commentaire ..................................
     mask.replace("{{commentaire}}",  comment);
     //...........  prix ..................................
     int  deb    = mask.indexOf("{{prix=");
     int  end    = -1;
     int deb_sub = -1;
                    if ( deb != -1 ) { deb_sub = deb+7; end = mask.indexOf("}}", deb_sub);}
     if ( end != -1 )
        { if ( total_price.toDouble() > 0 )
             { QString subMask = mask.mid(deb_sub,end-deb_sub).replace("[PRIX]", total_price);
               end += 2;   // ne pas oublier les }} fermants
               mask = mask.replace(deb,end-deb,subMask);
             }
         else
             { end += 2;   // ne pas oublier les }} fermants
               mask = mask.remove(deb,end-deb);
             }
        }
     //...........  nb boites ..................................
     int box_nb = nb_boites.toInt();
     deb        = mask.indexOf("{{nb_boites=");
     end        = -1;
     deb_sub    = -1;
     if ( deb  != -1 ) { deb_sub = deb+12; end = mask.indexOf("}}", deb_sub);}
     if ( end  != -1 )
        { if ( box_nb > 0 )
             { QString subMask = mask.mid(deb_sub,end-deb_sub).replace("[NB_BOITES]", nb_boites + " " + get_AccordedWord(box_nb,"boite"));
               end += 2;   // ne pas oublier les }} fermants
               mask = mask.replace(deb,end-deb,subMask);
             }
         else
             { end += 2;   // ne pas oublier les }} fermants
               mask = mask.remove(deb,end-deb);
             }
        }
     if (substituableImgPath.length()==0)     return mask;

     //................................... PARTIE AVEC HTML COMPLET ..................................................
    //.................. nom et composition DCI .....................................................................
    QString compositionDCI   = "";
    QStringList compoXmlList = CGestIni::getXmlDataList( "gph_dc",     compoXmlParts);
    for (int i=0; i<compoXmlList.size();++i)
        {nextPos             = 0;
         QString compoXML    = compoXmlList.at(i);
         QString name        = CGestIni::getXmlData(     "gph_dcn", compoXML, &nextPos).trimmed();
         QString dose        = CGestIni::getXmlData(     "gph_dcp", compoXML, &nextPos);
         QString unit        = CGestIni::getXmlData(     "gph_dcu", compoXML, &nextPos);
         if (name.length()) compositionDCI     +=  name + "  "+ dose +" " +  get_AccordedWord( dose.toInt(), unit) + " + ";
        }

    //................. gerer l'affichage nom / DCI ................................................................
    QString mainName       = "";
    QString optionnalName  = "";
    if (compositionDCI.length()==0)  dciMode = 0;              // si pas de compostion DCI on force le mode commecial
    else                             compositionDCI.chop(3);   // enlever le "+" de fin.
    switch (dciMode)
    {case 0: mainName     += commerialName; break;
     case 1: mainName     += commerialName;
             optionnalName = compositionDCI;
             break;
     case 2: mainName     += compositionDCI;
             optionnalName = commerialName;
             break;
     case 3: mainName     += compositionDCI;
             break;
    }
    mask.replace("{{main_name}}",   mainName);
    mask.replace("{{second_name}}", optionnalName);
    if (optionnalName.length()==0)  mask.remove("()");
    //................ substituable ................................................................................
    if (substituable=="0") substituable = QString("<img src=\"%1\" alt=\"jours\" height=\"16\" width=\"130\">").arg(substituableImgPath);
    else                   substituable = "";
    mask.replace("{{substituable}}", substituable);
    //................ forme voie ..................................................................................
    mask.replace("{{forme}}", up_forme);
    mask.replace("{{voie}}",  (voie.length() ? QObject::tr("par voie ") + voie : ""));
    //................ numero d'ordre ..................................................................................
    mask.replace("{{num_order}}", num_Ordre);
    return mask;
}

/*! \brief return human readable string from posologie grammar.
 *  \param const QString &grammar_expression     xml grammar input string as
 *      <gph>
 *         <gph_id>301546</gph_id>
 *         <gph_it>e</gph_it>
 *         <gph_pk>123</gph_pk>
 *         <gph_uf>comprim\303\251</gph_uf>
 *         <gph_nm>5</gph_nm>                        // conversion mode en mode numerique UP_QUANTITE MOMENT_MEAL ALL_LITERAL (on se sert de celui-ci car plus rapide a interpreter)
 *         <gph_cm>UP_QUANTITE DURATION</gph_cm>     // traduction textuelle de <gph_nm>5</gph_nm> information lisible de celle numerique  UP_QUANTITE DURATION on a pas peur des redondances ! (vive la memoire vive et les HD)
 *         <gph_in>
 *             <gph_il>I13.1|CIM|insuffisance r\303\251nale et cardiopathie hypertensive</gph_il>
 *             <gph_il>P96.0|CIM|insuffisance r\303\251nale cong\303\251nitale</gph_il>
 *         </gph_in>
 *         <gph_co>\303\240 prendre avec un grand verre d'eau sans respirer pendant 30 minutes</gph_co>
 *      </gph>
 *      <gps>
 *         <gpp>[1;m|2;d|4;s]J8</gpp>
 *         <gpp>[0.5;m|1;d15r|2;sr30|3;h10-13]J4\</gpp>
 *         <gpp>[0.5;m|1;s]J3</gpp>
 *      </gps>
 *  \param int must_be_literal  undefined if -1 (the grammar string try to be used for this and if not possible it's defined to C_PosologieGrammar::NOT_LITERAL)
 *         otherwise it's must be a combination of : C_PosologieGrammar::UP_QUANTITE
 *                                                   C_PosologieGrammar::UP_JUST_FRACTION   //  conversion uniquement de 0.25 en un quart 0.5 en un demi 0.75 en trois quart
 *                                                   C_PosologieGrammar::DURATION
 *                                                   C_PosologieGrammar::SEQUENCE_HOUR
 *                                                   C_PosologieGrammar::MOMENT_MEAL
 *                                                   C_PosologieGrammar::INDICATION_LIST   indications list must be showed
 *                                                   C_PosologieGrammar::INDICATION_CODE   indications code must be showed
 *                                                   C_PosologieGrammar::INDICATION_TYPE   indications code typ (e CISP etc.) must be showed
 *         this parameter define how numerics values must be converted in literal string.
 *  \param QString &_up_forme      if == "" the grammar string try to be used to define, this parameter is the drug unit form.
 *  \return a french human readable string (for other langs the source code is there after ...)
 */

//---------------------------- toHtml ------------------------------------------------
QString C_PosologieGrammar::toHtml(const QString &grammar_expression,
                                   const QString &substituableImgPath   /* = "" */,
                                   const QString &css                   /* = "" */,
                                   const QColor  &background            /* = QColor(Qt::black) */,
                                   const QString num_Ordre              /* = "" */
                                   )
{
        int nextPos              = 0;
        int is_stupefiant        = CGestIni::getXmlData(     "stup",         grammar_expression, &nextPos).toInt();    // place en debut donc pas penalisant si recherche sans le curseur nextPos
        QString rfid             = CGestIni::getXmlData(     "rfid",         grammar_expression, &nextPos);
        QString totalPrice       = CGestIni::getXmlData(     "total_price",  grammar_expression, &nextPos);
        QString nb_boites        = CGestIni::getXmlData(     "nb_boites",    grammar_expression, &nextPos);

        //QString id               = CGestIni::getXmlData(     "gph_id",    grammar_expression, &nextPos);
        //QString id_type          = CGestIni::getXmlData(     "gph_it",  grammar_expression, &nextPos);
        //QString pk               = CGestIni::getXmlData(     "gph_pk",  grammar_expression, &nextPos);
        QString    s_dateRed     = CGestIni::getXmlData(     "gph_dr",    grammar_expression, &nextPos).left(10);
        QString    s_dateDeb     = CGestIni::getXmlData(     "gph_dt",    grammar_expression, &nextPos).left(10);
        QString commercialName   = CGestIni::getXmlData(     "gph_na",    grammar_expression, &nextPos);
        QString compoXmlParts    = CGestIni::getXmlData(     "gph_dcl",   grammar_expression, &nextPos);

        QString cycle            = CGestIni::getXmlData(     "gph_cy",    grammar_expression, &nextPos);
        QString substituable     = CGestIni::getXmlData(     "gph_sb",    grammar_expression, &nextPos);
        int     dciMode          = CGestIni::getXmlData(     "gph_dci",   grammar_expression, &nextPos).toInt();
        QString up_forme         = CGestIni::getXmlData(     "gph_uf",    grammar_expression, &nextPos);
        QString voie             = CGestIni::getXmlData(     "gph_voie",  grammar_expression, &nextPos);
        QString conv_mod_T       = CGestIni::getXmlData(     "gph_cm",    grammar_expression, &nextPos);
        QString conv_mod_N       = CGestIni::getXmlData(     "gph_nm",    grammar_expression, &nextPos);
        //QString indicStr         = CGestIni::getXmlData(     "gph_in",  grammar_expression, &nextPos);
        QString comment          = CGestIni::getXmlData(     "gph_co",    grammar_expression, &nextPos);
        QStringList seqList      = CGestIni::getXmlDataList( "gpp",       grammar_expression, &nextPos);
        int must_be_literal      = -1;
     //........ traiter le cas ou la definition de literalite est donnee dans la grammaire avant le signe = .........
     if (conv_mod_T.length())          // .......... si literalite definie dans grammaire l'utiliser que si parametre d'entree non defini ..............
        { if (conv_mod_N.length()) must_be_literal = conv_mod_N.toInt();                                    // court
          else                     must_be_literal = C_PosologieGrammar::literalStringToFlag(conv_mod_T);   // long
        }
     if (must_be_literal==-1)
        {  must_be_literal = C_PosologieGrammar::NOT_LITERAL;
        }
     if (is_stupefiant) must_be_literal = C_PosologieGrammar::ALL_LITERAL;   // c'est stupefiant donc on surcharge en tout literal
     //............. si forme definie en entree on l'utilise et elle surcharge celle de la grammaire .....................................
     //              sinon on tente d'utiliser celle definie dans la grammaire
     nextPos              = 0;
     QString joinStr      = QObject::tr(", \npuis ");
     QString body         = "";

     for (int i=0; i<seqList.count(); ++i)
         { //....... reperer si la sequence comporte une forme locale ..............
           //        et auquel cas l'utiliser
           QString sequence        = seqList[i];
           int     pos_deb         = sequence.indexOf('@');
           if (pos_deb != -1 )
              { voie               = sequence.left(pos_deb);
                sequence           = sequence.mid(pos_deb+1);
              }
                   pos_deb         = sequence.indexOf('[');
           if (pos_deb != -1 )
              { up_forme           = sequence.left (pos_deb);
                sequence           = sequence.mid  (pos_deb+1);
              }
           body  += sequenceToString(i, sequence, must_be_literal, up_forme); body += joinStr;
         }
     body.chop(joinStr.length());    // on enleve le dernier "\npuis "
     if (comment.length())                 body += " " + comment;
     if (s_dateRed != s_dateDeb && QDate::fromString(s_dateDeb.left(10),"dd-MM-yyyy") > QDate::fromString(s_dateRed.left(10),"dd-MM-yyyy"))
        {                                  body += QObject::tr(", \303\240 commencer le : ") + s_dateDeb.left(10);
        }
     if (cycle.length())                   body += dureeTotaleRenouvToString(cycle, must_be_literal );
     if (substituableImgPath.length()==0)     return body;

    //................................... PARTIE AVEC HTML COMPLET ..................................................
    //.................................. le modele css par defaut .....................................................
    QString span    =
                     "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
                     "<html>"
                     "<head>"
                     "    <meta name=\"qrichtext\" content=\"1\" />"
                     "    <style type=\"text/css\">"
                     "        span.mainName {"
                     "         color:%1;"                                    // 1 color dci
                     "         font-family: arial, verdana, sans-serif;"
                     "         text-decoration: underline;"
                     "         font-size: 10px;"
                     "         font-weight: bold;"
                     "        }"
                     "        span.optionalName {"
                     "          color:%2;"                                   // 2 color commercial name
                     "          font-family: arial, verdana, sans-serif;"
                     "          font-style: italic;"
                     "          font-size: 10px;"
                     "        }"
                     "        span.texteNormal {"
                     "          color:%3;"                                   // 3 color text normal
                     "          font-family: arial, verdana, sans-serif;"
                     "          font-size: 10px;"
                     "        }"
                     "        span.numOrdre {"
                     "          color:%4;"                                   // 4 color num Ordre
                     "          font-family: arial, verdana, sans-serif;"
                     "          font-size: 10px;"
                     "          font-weight: bold;"
                     "        }"
                     "        span.drugForme {"
                     "          color:%5;"                                   // 5 color forme
                     "          font-family: arial, verdana, sans-serif;"
                     "          font-size: 10px;"
                     "          font-weight: bold;"
                     "        }"
                     "        span.texteRfid {"
                     "          color:%6;"                                   // 6 color texteRfid
                     "          font-family: arial, verdana, sans-serif;"
                     "          font-size: 10px;"
                     "          font-weight: bold;"
                     "        }"
                     "        span.textePrice {"
                     "          color:%7;"                                   // 7 color textePrice
                     "          font-family: arial, verdana, sans-serif;"
                     "          font-size: 10px;"
                     "          font-weight: bold;"
                     "        }"
                     "        span.texteNbBox {"
                     "          color:%8;"                                   // 8 color texteNbBox
                     "          font-family: arial, verdana, sans-serif;"
                     "          font-size: 10px;"
                     "          font-weight: bold;"
                     "        }"

                     "    </style>"
                     "</head>";
    QString text =
                     "<body>"
                     "<span class=\"numOrdre\">%1</span> "
                     "%6"                                   // mention substituable
                     "<span class=\"mainName\">%2</span> "
                     "<span class=\"optionalName\">%3</span> "
                     "<span class=\"drugForme\">%4</span> "
                     "<span class=\"texteNormal\">%5</span>"
                     "<span class=\"texteRfid\">%7</span>"
                     "<span class=\"textePrice\">%8</span>"
                     "<span class=\"textePrice\">%9</span>"
                     "<br />"
                     "</body>"
                     "</html>";
    //.................. composition DCI .....................................................................
    QString compositionDCI   = "";
    QStringList compoXmlList = CGestIni::getXmlDataList( "gph_dc",     compoXmlParts);
    for (int i=0; i<compoXmlList.size();++i)
        {nextPos             = 0;
         QString compoXML    = compoXmlList.at(i);
         QString name        = CGestIni::getXmlData(     "gph_dcn", compoXML, &nextPos).trimmed();
         QString dose        = CGestIni::getXmlData(     "gph_dcp", compoXML, &nextPos);
         QString unit        = CGestIni::getXmlData(     "gph_dcu", compoXML, &nextPos);
         if (name.length()) compositionDCI     +=  name + "  "+ dose +" " +  get_AccordedWord( dose.toInt(), unit) + " + ";
        }

    //................. gerer l'affichage DCI ................................................................
    if (compositionDCI.length()==0) dciMode = 0;              // pas de DCI alors on force a rester en commercial
    else                            compositionDCI.chop(3);   // enlever le "+" de fin.
    QString mainName       = "";
    QString optionnalName  = "";
    switch (dciMode)
    {case 0: mainName     += commercialName; break;
     case 1: mainName     += commercialName;
             optionnalName = "("+ compositionDCI + ")";
             break;
     case 2: mainName     += compositionDCI;
             optionnalName = "("+ commercialName + ")";
             break;
     case 3: mainName     += compositionDCI;
             break;
    }

    //................ substituable ................................................................................
    if (substituable=="0") substituable = QString("<img src=\"%1\" alt=\"jours\" height=\"16\" width=\"130\">").arg(substituableImgPath);
    else                   substituable = "";
    //................ forme voie ..................................................................................
    QString forme =  up_forme + (voie.length() ? QObject::tr(" par voie ") + voie : "");
    //................ le reste ..................................................................................
    QString html  = "";
    if (css.length())
       {    html += css;               // on prend le css donne en entree
       }
    else                               // on prend le css par defaut
       {    html += span.arg ( background.darker(200).name(),                                               // 1 color dci
                               background.darker(120).name(),                                               // 2 color commercial name
                               background.darker(200).name(),                                               // 3 color text normal
                               "#FF0000",                                                                   // 4 color num Ordre  background.darker(200).name()
                               background.darker(200).name(),                                               // 5 color drug forme
                               background.darker(180).name(),                                               // 6 color refund for indication
                               "#FF0000",                                                                   // 7 color price
                               background.darker(180).name()                                                // 8 color nb box
                             );
       }

    int box_nb  = nb_boites.toInt();
    html += text.arg (  num_Ordre + ")" ,                                                                   // 1 numero d'ordre
                        mainName,                                                                           // 2 nom principal
                        optionnalName,                                                                      // 3 nom secondaire
                        forme+"<br />",                                                                     // 4 forme
                        body,                                                                               // 5 posologie
                        substituable,                                                                       // 6 mention substituable
                        rfid,                                                                               // 7 mention rembourse pour cette indication
                        totalPrice.toDouble()>0?QObject::tr ("<br/> Prix %1").arg(totalPrice):"",           // 8 prix
                        box_nb?"  "+nb_boites+" "+get_AccordedWord(box_nb,"boite"):""                       // 9 nb de boites
                     );
    return html;
}

//---------------------------- dureeTotaleRenouvToString ------------------------------------------------
// <gph_cy>S3R6</gph_cy>     // JSMA     exples J6 --> pour 6 jours S7 --> pour 7 semaines M3R6  pour 3 mois renouvelable 6 fois
QString C_PosologieGrammar::dureeTotaleRenouvToString(QString cycle, int must_be_literal /* = C_PosologieGrammar::NOT_LITERAL */)
{   if (cycle.length()==0) return "";
    QString duree        = "";
    QString        ret   = "";
    int            pos   = cycle.indexOf('R');
    if (pos != -1)
       { duree  = cycle.left(pos);
         cycle  = cycle.mid(pos+1);
       }
    else
      { duree = cycle;
        cycle = "";
      }
    if (duree.length() && duree.mid(1).toInt())   // si duree nulle ne pas mettre la mention
      { ret  = " " + dureeTotaleToString(duree, must_be_literal );
      }
    if (pos != -1)
       { int n = cycle.toInt();
         if (n) ret  += QObject::tr(" (renouvelable %1 fois)").arg(numberToLiteral(n,'f'));
       }
    if (ret.length()) ret = ", " + ret;
    return ret;
}
//---------------------------- dureeTotaleToString ------------------------------------------------
//  J2
QString C_PosologieGrammar::dureeTotaleToString(const QString &seq_days, int must_be_literal /* = C_PosologieGrammar::NOT_LITERAL */)
{if (seq_days.length()==0) return "";
 QString    introStr = QObject::tr("traitement \303\240 poursuivre pendant ");   //QObject::tr(". La dur\303\251e totale de cette prescription est de ")
 char              t = seq_days[0].toLatin1()|32;
 QString         ret = seq_days;
 QString str_nb      = ret.mid(1);
 int         nb      = str_nb.toInt();

 switch(t)
 {case 'j': return    introStr + ( (must_be_literal&C_PosologieGrammar::DURATION)?numberToLiteral(nb)     : str_nb) + QObject::tr(" jour")    + ((nb>1)?"s":"");
  case 's': return    introStr + ( (must_be_literal&C_PosologieGrammar::DURATION)?numberToLiteral(nb,'f') : str_nb) + QObject::tr(" semaine") + ((nb>1)?"s":"");
  case 'm': return    introStr + ( (must_be_literal&C_PosologieGrammar::DURATION)?numberToLiteral(nb)     : str_nb) + QObject::tr(" mois");
  case 'a': return    introStr + ( (must_be_literal&C_PosologieGrammar::DURATION)?numberToLiteral(nb)     : str_nb) + QObject::tr(" an")      + ((nb>1)?"s":"");
 }
 return ret;
}
//---------------------------- sequenceToString ------------------------------------------------
// <gpp>[1;m|2;d|4;s]J8!a la phase initiale en bolus</gpp>
// <gpp>[100-200;]U1!a la phase initiale en bolus</gpp>   // 100 a 200 mg a ne faire q'une fois a la phase initiale en bolus
QString C_PosologieGrammar::sequenceToString(int numSeq, const QString &sequence, int must_be_literal /* = C_PosologieGrammar::NOT_LITERAL */, const QString &up_forme)
{
 int pos             = sequence.indexOf(']');
 if (pos==-1) return QString("");
 int pcom            = sequence.indexOf('!');
 int len             = -1;    // si pas de commentaire le mid prend tout
 QString commentaire = "";
 if (pcom != -1)
    { commentaire = sequence.mid(pcom+1);
      len = pcom-pos-1;
    }
 QString ret                       = "";
 QString seq_days                  = sequence.mid(pos + 1,len);
 QString seq_time                  = sequence.left(pos).remove("[").trimmed();
 QString result                    = sequence_timeToString(seq_time, seq_days, must_be_literal, up_forme);
 if (result.endsWith(", ")) result = result.left(result.length()-2);
 ret                              += result;
 result                            = sequence_daysToString(numSeq, seq_days, must_be_literal );
 if (result.length())         ret += result;
 if (commentaire.length())    ret += " " + commentaire;
 return ret;
}

//---------------------------- sequence_daysToString ------------------------------------------------
//  J2
QString C_PosologieGrammar::sequence_daysToString(int numSeq, const QString &_seq_days, int must_be_literal /* = C_PosologieGrammar::NOT_LITERAL */)
{ if (_seq_days.length()==0) return "";
 QString       ret   = "";
 QString  s_qu       = "";
 int       qu        = 0;
 char unit_tousles   = 0;
 char t              = _seq_days[0].toLatin1()|32;

 //.................... cas ou une periode type 'tous les'  ... est precisee .....................
 if ( t == 'u' )      // U1S:6M  (a ne faire qu'une fois par semaine pendant 6 mois)  U3J: (a ne faire qu'une fois tous les trois jours)
    { int pos = _seq_days.indexOf(':');
      s_qu    = "";
      qu      = 0;
      if (pos != -1)
         {  s_qu         = _seq_days.mid(1,pos-2);
            qu           = s_qu.toInt();
            unit_tousles = _seq_days[pos-1].toLatin1()|32;
            s_qu         = ( (must_be_literal&C_PosologieGrammar::DURATION) ? numberToLiteral(qu) : s_qu );  // on ajuste selon literalite
         }
      switch (unit_tousles)
         { case 'h':  ret =  QObject::tr(", toutes les %1").arg ( (qu>1) ? s_qu + " heures"    : "heures");   break;
           case 'j':  ret =  QObject::tr(", tous les %1").  arg ( (qu>1) ? s_qu + " jours"     : "jours");    break;
           case 's':  ret =  QObject::tr(", toutes les %1").arg ( (qu>1) ? s_qu + " semaines"  : "semaines"); break;
           case 'm':  ret =  QObject::tr(", tous les %1").  arg ( (qu>1) ? s_qu + " mois"      : "mois");     break;
           case 'a':  ret =  QObject::tr(", tous les %1").  arg ( (qu>1) ? s_qu + " ans"       : "ans");      break;
         }
      //...... se repositionner apres la mention U1S: et recuperer valeurs ...........
      ++pos;
      if ( pos == _seq_days.length() ) return ret; // ==> CASSOS si pas de mention de duree apres celle de 'tous les' sinon bug fatal
      t    = _seq_days[pos].toLatin1()|32;         // recuperer lettre clef apres les deux points
      s_qu = _seq_days.mid(++pos);                 // recuperer quantite apres les deux points et la lettre clef
    } // endif ( t=='u' )
 else
    {s_qu       = _seq_days.mid(1); // recuperer quantite apres la lettre clef
    }

 //.......... cas du semainier d'inclusion ou d'exclusion .................
 switch(t)
 {case 'i':    // IlmMjvsd I comme Include
  case 'e':    // ElmMjvsd E comme Exclude
    {
     // const char *pt = str_nb.toLatin1();    // le pt n'est pas fiable du tout et ne permet pas le parsing des que ++pt le contenu part dans les choux pourquoi ???
     int        len = s_qu.length();
     if (len ==0)       return "";
     if (t=='e') ret = QObject::tr(" sauf le");
     else        ret = QObject::tr(" \303\240 prendre le");
     for (int i=0; i<len;++i)
       { switch(s_qu[i].toLatin1())
          { case 'l': ret += QObject::tr(" lundi,");    break;
            case 'm': ret += QObject::tr(" mardi,");    break;
            case 'M': ret += QObject::tr(" mercredi,"); break;
            case 'j': ret += QObject::tr(" jeudi,");    break;
            case 'v': ret += QObject::tr(" vendredi,"); break;
            case 's': ret += QObject::tr(" samedi,");   break;
            case 'd': ret += QObject::tr(" dimanche,"); break;
          }
       }
      if (ret .endsWith(",")) ret = ret.left(ret.length()-1);
      return ret;  // ====> CASSOS
    }
  }
 //.......... cas du detail des durees ................................
 qu   = s_qu.toInt();
 s_qu = ( (must_be_literal&C_PosologieGrammar::DURATION)?numberToLiteral(qu):s_qu );  // on ajuste selon literalite
 switch(t)
 {
  case 'j':    // JSMA
    {if (numSeq==0) return ret + ((qu==1) ? QObject::tr(", le premier jour") :
                                            QObject::tr(", les ") + s_qu + QObject::tr(" premiers jours"));
     else           return ret + ((qu==1) ? QObject::tr(", le jour suivant") :
                                            QObject::tr(", les ") + s_qu + QObject::tr(" jours suivants"));
    } break;
  case 's':
    {if (numSeq==0) return ret + ((qu==1) ? QObject::tr(", la premi\303\250re semaine") :
                                            QObject::tr(", les ") + s_qu + QObject::tr(" premi\303\250res semaines"));
     else           return ret + ((qu==1) ? QObject::tr(", la semaine suivante") :
                                            QObject::tr(", les ") + s_qu + QObject::tr(" semaines suivantes"));
    } break;
  case 'm':
    {if (numSeq==0) return ret + ((qu==1) ? QObject::tr(", le premier mois") :
                                            QObject::tr(", les ") + s_qu + QObject::tr(" premiers mois"));
     else           return ret + ((qu==1) ? QObject::tr(", le mois suivant") :
                                            QObject::tr(", les ") + s_qu + QObject::tr(" mois suivants"));
    } break;
  case 'a':
    {if (numSeq==0) return ret + ((qu==1) ? QObject::tr(", la premi\303\250re ann\303\251e") :
                                            QObject::tr(", les ") + s_qu + QObject::tr(" premi\303\250res ann\303\251es"));
     else           return ret + ((qu==1) ? QObject::tr(", l'ann\303\251e suivante") :
                                            QObject::tr(", les ") + s_qu + QObject::tr(" ann\303\251es suivantes"));
    } break;
 }
 return ret;
}
//---------------------------- sequence_timeToString ------------------------------------------------
// 0.5;m|1;d15r|2;sr30|3;h10-13
QString C_PosologieGrammar::sequence_timeToString(const QString &seq_time, const QString &_seq_days, int must_be_literal /* = C_PosologieGrammar::NOT_LITERAL */, const QString &_up_forme /* = "" */)
{QString     up_forme = "";
 QStringList  seqList = seq_time.split('|');
 QString          ret = "";
 QString          str = "";
 QString           qu = "";
 QString          qu2 = "";
 QString       qu_lit = "";
 QString      qu_lit2 = "";
 QString before       = "";
 QString after        = "";
 int     before_n     = 0;
 int     after_n      = 0;
 int              pos = 0;
 int               p  = 0;
 int               nb = 0;
 int                l = 0;
 //....... tenter le regroupement des doses ...
 /*
 bool     isCommonQu  = false;
 QString  toCmp     = "";
 QString momments   = "";
 if (seqList.count()>1)
    { str        = seqList[i];
      pos        = str.indexOf(';');
      if (pos!=-1)
         { momments  += str[++pos];
           toCmp      = str.remove(pos,1);  // on fait peter le 'd' ou 'm' etc.. (l'indication du moment de prise)
           isCommonQu = true;
           for (int i=1; i<seqList.count() && isCommonQu; ++i)
               { str        = seqList[i];
                 pos        = str.indexOf(';');
                 if (pos==-1) {isCommonQu = false; break;}
                 momments  += str[++pos];
                 isCommonQu = (curCmp==str.remove(pos,1));
               }
         }
      if (isCommonQu)
         {

         }
    }
 */
 for (int i=0; i<seqList.count(); ++i)
    {up_forme   = _up_forme;   // ne pas accorder sexe nombre plusieurs fois donc reinitialiser
     str        = seqList[i];
     pos        = str.indexOf(';');
     if (pos   != -1)
        {//.......... C_PosologieGrammar::UP_QUANTITE la quantite d'unite de prise .....................
         //           ici est traite le mode literal ou numerique
         qu     = str.left(pos);
         p      = qu.indexOf('-');
         if (p != -1)                // cas ou 1-2   (un \303\240 deux)
            { qu2 = qu.mid(p+1);
              qu  = qu.left(p);
            }

         if      (must_be_literal & C_PosologieGrammar::UP_QUANTITE)       // tout doit etre converti en lettre quelque soit le flag C_PosologieGrammar::UP_JUST_FRACTION
                 {qu_lit = numberToLiteral(qu, up_forme, 0);               // cet appel est toujours necessaire meme si pas de literal car met a jour orthographe et pluriel de up_forme
                  ret   += qu_lit;
                  if (qu2.length())
                     {qu_lit2 = numberToLiteral(qu2, up_forme, 0);
                      ret    += QObject::tr(" \303\240 ") + qu_lit2;
                     }
                 }
         else if (must_be_literal & C_PosologieGrammar::UP_JUST_FRACTION)  // seules les parties fractionnaires 0.25 0.30 0.75 --> un quart un demi  trois quart
                 {qu_lit = numberToLiteral(qu, up_forme, 1);               // cet appel est toujours necessaire meme si pas de literal car met a jour orthographe et pluriel de up_forme
                  ret   += qu_lit;
                  if (qu2.length())
                     {qu_lit2 = numberToLiteral(qu2, up_forme, 1);
                      ret    += QObject::tr(" \303\240 ") + qu_lit2;
                     }
                 }
         else                                                              // tout doit rester en numerique
                 {qu_lit = numberToLiteral(qu, up_forme, 0);               // cet appel est toujours necessaire meme si pas de literal car met a jour orthographe et pluriel de up_forme
                  ret   += qu;
                  if (qu2.length())
                     {qu_lit2 = numberToLiteral(qu2, up_forme, 1);
                      ret    += QObject::tr(" \303\240 ") + qu2;
                     }
                 }
         //.......... la forme (est precedemment ajustee grammaticalement par l'appel numberToLiteral(qu, up_forme);) ..............
         ret   += " " + up_forme+" "; // + ((qu.toFloat()>1.0)?"s ":" ");
         //........... C_PosologieGrammar::SEQUENCE_HOUR  le moment ou prendre les medocs dans la journee .......................
         //            si specifie en heures, le mode literal
         //            ou numerique sera traite ici
         str    = str.mid(pos+1);   // prendre ce qu'il y a apres le point virgule
         l      = str.length();
         if (l==0 && _seq_days !="U1J:")          // mention l apres le point virgule  "U1J:" est 'une fois par 'tous les jour'"
            {ret   += QObject::tr(" par jour");   // sera remplace par l'unite de temps de la periode (jour semaine mois etc...)
            }
         else
            { char t  = str[0].toLatin1()|32;    // relever le m d a s c n h
              str     = str.mid(1);
              switch (t)
              {    case 'm': ret += QObject::tr("le matin");     break;
                   case 'd': ret += QObject::tr("\303\240 midi");       break;
                   case 'a': ret += QObject::tr("l'apr\303\250s-midi"); break;
                   case 's': ret += QObject::tr("le soir");      break;
                   case 'c': ret += QObject::tr("au coucher");   break;
                   case 'n': ret += QObject::tr("la nuit");      break;
                   case 'h':  // mode horaire
                      {pos  = str.indexOf('-');
                       nb   = 0;                   // sera utilise pour le pluriel des heures
                       if (pos != -1)
                          {      before   = str.left(pos);
                                 after    = str.mid(pos+1);
                                 before_n = before.toInt();
                                 after_n  = after.toInt();
                                 nb       = qMax(before_n,after_n);  // reperer si le pluriel doit etre applique
                                 if (must_be_literal & C_PosologieGrammar::SEQUENCE_HOUR)
                                    {before  =  numberToLiteral(before_n);
                                     after   =  numberToLiteral(after_n);
                                    }
                                 ret   += QObject::tr("de ")+ before + QObject::tr(" \303\240 ") + after;
                          }
                       else
                          {      nb = str.toInt();
                                 if (must_be_literal & C_PosologieGrammar::SEQUENCE_HOUR)
                                    { str = numberToLiteral(nb);
                                    }
                                 ret   += QObject::tr("\303\240 ") + str;
                          }
                       ret += QObject::tr(" heure") + ((nb>1)?"s":"");
                      } break;
                  }
              //............ C_PosologieGrammar::MOMENT_MEAL le moment par rapport au repas .........................
              //             en minutes, le mode literal numerique sera traite ici
              pos  = str.indexOf('r');
              if  (pos != -1)
                  { before   = str.left(pos);
                    after    = str.mid(pos+1);
                    before_n = before.toInt();
                    after_n  = after.toInt();
                    if ( after=="0" && before=="0")
                       {ret +=  QObject::tr(" au cours du repas");
                       }
                    else if ( before=="0" )
                       {ret +=  QObject::tr(" avant le repas");
                       }
                    else if (after=="0")
                       {ret +=  QObject::tr(" apr\303\250s le repas");
                       }
                    else if (before_n==0 && after_n==0)
                       {ret +=  QObject::tr(" au cours du repas");
                       }
                    else //if (before_n||after_n)
                       {
                        if (must_be_literal & C_PosologieGrammar::MOMENT_MEAL)
                           { before = numberToLiteral(before.toInt());
                             after  = numberToLiteral(after.toInt());
                           }
                        if      (before.length()) ret += " " + before + QObject::tr(" minutes avant le repas");
                        else if (after.length() ) ret += " " + after  + QObject::tr(" minutes apr\303\250s le repas");
                        else                      ret += " " + QObject::tr("au cours du repas");
                       }
                  }
          } // if (str.length())
         ret += ", ";
        }
    }  // end for (int i=0; i<seqList.count(); ++i)
 return ret;
}
//---------------------------- numberToLiteral ------------------------------------------------
QString C_PosologieGrammar::numberToLiteral(QString number, QString &up_forme , int justOneFraction /* = false */)
{number.replace(',','.');
 QString       str  = "";
 int left_part_int  = 0;
 QString left_part  = "";
 QString right_part = "";
 char          sexe = 'm';
 int pos            = number.indexOf(".");
 if (pos != -1)               // UP_QUANTITE et UP_JUST_FRACTION ==>
    {//  0.75 1.75 789.75
     left_part         = number.left(pos);
     right_part        = number.mid (pos+1);
     left_part_int     = left_part.toInt();
     up_forme          = get_AccordedWord(left_part_int, up_forme, sexe);

     if (left_part_int==0)  left_part  = "";
     else                   left_part  = numberToLiteral(left_part.toInt(), sexe);
     if      (right_part=="25")
             { if (left_part_int)
                  {if (justOneFraction ) return number;
                   else                  return left_part + " et un quart de";
                  }
               else                      return QString("un quart de");
             }
     else if (right_part=="50" || right_part=="5")
             { str = (sexe=='f')?"une demie":"un demi";
               if (left_part_int)
                  {if (justOneFraction) return number;
                   else                 return left_part + " et "+ str;
                  }
               else                     return str;
             }
     else if (right_part=="75")
             { if (left_part_int)
                  {if (justOneFraction) return number;
                   else                 return left_part + " et trois quarts de";
                  }
               else                     return QString("trois quarts de");
             }
     else    { if (justOneFraction)     return number;
               else  right_part = numberToLiteral(right_part.toInt(),sexe);
             }
     if (left_part_int==0) return QString::fromUtf8("z\303\251ro virgule ") + right_part;
     else                  return left_part + " virgule "  + right_part;
    }
 left_part_int     = number.toInt();
 if (up_forme.endsWith("(s)")) up_forme.chop(3);                       // si mauvais traitement du pluriel deja place le virer
 up_forme          = get_AccordedWord(left_part_int, up_forme, sexe);  // toujours executer get_AccordedWord() car ajuste la forme au pluriel
 if (justOneFraction) return number;
 else                 return numberToLiteral(left_part_int,sexe);
}

//---------------------------- numberToLiteral ------------------------------------------------
QString C_PosologieGrammar::numberToLiteral(int nb, char sexe /* = 'm' */)
{
 QString ret                  = "";
 static QString nompart[10]   = {"dix"  ,"onze"  ,"douze"   ,"treize"   ,"quatorze","quinze"   ,"seize"       ,"dix-sept"    ,"dix-huit","dix-neuf"};
 static QString nomunite[10]  = {""     ,"un"    ,"deux"    ,"trois"    ,"quatre"  ,"cinq"     ,"six"         ,"sept"        ,"huit"    ,"neuf"};
 static QString nomdizaine[8] = {"vingt","trente","quarante","cinquante","soixante","soixante-","quatre-vingt","quatre-vingt-"};

 int milliers  = nb/1000;
     nb        = nb%1000;
 int centaines = nb/100;
 int unites    = nb%100;
 int dizaines  = unites/10;
 unites        = unites%10;
 //......... le sexe doit s'accorder pour ce qui se termine par 'un' (nomunite[1] et nomspe[1] ) ....
 if (milliers>1 && milliers<10)                   ret += nomunite[milliers] + " mille ";               // milliers>1  => nomunite[milliers] jamais "un"
 if (milliers==1)                                 ret +=                      "mille ";
 if (centaines>1)
    {/* -->  something at end of line          */ ret += nomunite[centaines] +" cent ";
     if ( unites==0 && dizaines==0 )              ret.chop(1);
    }
 if ( centaines==1 )                              ret += "cent ";
 if ( dizaines )
    { if ( dizaines>1 && dizaines!=7 && dizaines!=9 )  // 20 30 40 50 60 80
         { /* -->   something at end of line   */ ret += nomdizaine[dizaines-2];
           if ( unites>1 )                        // unites>1  => nomunite[unites] jamais "un"
              {                                   ret += "-" + nomunite[unites];
              }
           else if ( unites==1 )
             { if (dizaines==8)                   ret += (sexe=='f')?"-une"   :"-un";   // 81
               else                               ret += (sexe=='f')?"-et-une":"-et-un";
             }
        }
      else                                            // 10 70 90
        {
          if (dizaines==7 || dizaines==9)         ret += nomdizaine[dizaines-2]; // "soixante-" "quatre-vingt-"
          if (dizaines==7 && unites==1)           ret += "et-";                  // "soixante-" "et-"
          /* -->   something at end of line    */ ret += nompart[unites];        // "dix"  ,"onze"  ,"douze"   ,"treize"   ,"quatorze","quinze"   ,"seize"       ,"dix-sept"    ,"dix-huit","dix-neuf"
        }
      if (centaines!=0)                           ret += " ";
      unites = 0;
    }   // endif (dizaines)
 if (unites==1)                                   ret += (sexe=='f')?"une":"un";
 else                                             ret += nomunite[unites];
 return ret.trimmed();
}


