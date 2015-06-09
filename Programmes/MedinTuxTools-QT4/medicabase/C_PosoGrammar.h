#ifndef GRAMMATTOOLS_H
#define GRAMMATTOOLS_H
#ifndef C_POSOLOGIEGRAMMAR_H
#define C_POSOLOGIEGRAMMAR_H

#include<QStringList>
#include<QMap>

#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/medicabase/C_PosologieGrammar.h"

//=================== C_PosoGrammar ===================================
/*! \class C_PosologieGrammar
 *  \brief Class to set and interpret posologie grammar.
 */

class C_PosoGrammar : public C_PosologieGrammar
{

/*
public:    enum flags  {NOT_LITERAL        = 0,
                        UP_QUANTITE        = 1,
                        UP_JUST_FRACTION   = 2,
                        DURATION           = 4,
                        SEQUENCE_HOUR      = 8,
                        MOMENT_MEAL        = 16,
                        INDICATION_LIST    = 32,
                        INDICATION_CODE    = 64,
                        INDICATION_TYPE    = 128,
                        ALL_LITERAL        = 31
                       };
           enum indicFlag  { CODE       = 0,
                             CODE_TYPE  = 1,
                             LIBELLE    = 2,
                             ALL_INDIC  = 3
                           };
*/
          //---------------------------- C_PosologieGrammar ----------------------------------------------
          explicit  C_PosoGrammar(const QString &grammar_expression="")
          {setGrammar(grammar_expression);
          }

          //---------------------------- grammar ------------------------------------------------
          /*! \brief return the grammar string
          */
          QString   grammar(){return m_grammar_expression;}
          //---------------------------- setGrammar ----------------------------------------------
          /*! \brief set the literal grammar string. if empty a empty skeleton grammar  is set
           *  the grammar string is updated whith flags contents indications
           *  \param const QString &grammar_expression :   grammar string to set
           *  \return the grammar string.
          */
          QString setGrammar(const QString &grammar_expression)
          {m_grammar_expression = grammar_expression;
           if (m_grammar_expression.length()==0)
              {m_grammar_expression = empty_skeleton_grammar();
              }
           return m_grammar_expression;
          }
          //---------------------------- isLiteral ----------------------------------------------
          /*! \brief return the literal int flag. if zero nothing must be in literal form
           *  \return the literal int flag.
          */
          int isLiteral()
          {return m_must_be_literal;
          }
          //---------------------------- setLiteralFlag ----------------------------------------------
          /*! \brief set how the grammar must be converted
           *  \param int must_be_literal (default = C_PosologieGrammar::ALL_LITERAL).
           *   if  must_be_literal is set to  undefined if -1 (the grammar string try to be used for this and if not possible it's defined to C_PosologieGrammar::NOT_LITERAL)
           *         otherwise it's must be a combination of : C_PosologieGrammar::UP_QUANTITE
           *                                                   C_PosologieGrammar::UP_JUST_FRACTION   //  conversion uniquement de 0.25 en un quart 0.5 en un demi 0.75 en trois quart
           *                                                   C_PosologieGrammar::DURATION
           *                                                   C_PosologieGrammar::SEQUENCE_HOUR
           *                                                   C_PosologieGrammar::MOMENT_MEAL
           *                                                   C_PosologieGrammar::INDICATION_LIST   indications list must be showed
           *                                                   C_PosologieGrammar::INDICATION_CODE   indications code must be showed
           *                                                   C_PosologieGrammar::INDICATION_TYPE   indications code typ (CIP CISP etc.) must be showed
           *         this parameter define how numerics values must be converted in literal string.
           */
          void setLiteralFlag(int must_be_literal    = C_PosologieGrammar::ALL_LITERAL)
          {m_must_be_literal = must_be_literal;
           CGestIni::setXmlData("gph_nm", QString::number( must_be_literal),               m_grammar_expression);
           CGestIni::setXmlData("gph_cm", literalFlagToString(must_be_literal).toAscii(),  m_grammar_expression);
          }

          //---------------------------- toString ------------------------------------------------
          /*! \brief return human readable string from posologie grammar.
           *      <gph>
           *         <gph_id>301546</gph_id>
           *         <gph_it>CIP</gph_it>
           *         <gph_pk>123</gph_pk>
           *         <gph_uf>comprimé</gph_uf>
           *         <gph_voie>orale</gph_voie>
           *         <gph_nm>5</gph_nm>                        // conversion mode en mode numerique UP_QUANTITE MOMENT_MEAL ALL_LITERAL (on se sert de celui-ci car plus rapide a interpreter)
           *         <gph_cm>UP_QUANTITE DURATION</gph_cm>     // traduction textuelle de <gph_nm>5</gph_nm> information lisible de celle numerique  UP_QUANTITE DURATION on a pas peur des redondances ! (vive la memoire vive et les HD)
           *         <gph_in>
           *             <gph_il>I13.1|CIM|insuffisance rénale et cardiopathie hypertensive</gph_il>
           *             <gph_il>P96.0|CIM|insuffisance rénale congénitale</gph_il>
           *         </gph_in>
           *         <gph_co>�  prendre avec un grand verre d'eau sans respirer pendant 30 minutes</gph_co>
           *      </gph>
           *      <gps>
           *         <gpp>[1;m|2;M|4;s]/8</gpp>
           *         <gpp>[0.5;m|1;M15r|2;sr30|3;h10-13]/4\</gpp>
           *         <gpp>[0.5;m|1;s]/3</gpp>
           *      </gps>
           *   if  m_must_be_literal is set to  undefined if -1 (the grammar string try to be used for this and if not possible it's defined to C_PosologieGrammar::NOT_LITERAL)
           *         otherwise it's must be a combination of : C_PosologieGrammar::UP_QUANTITE
           *                                                   C_PosologieGrammar::UP_JUST_FRACTION   //  conversion uniquement de 0.25 en un quart 0.5 en un demi 0.75 en trois quart
           *                                                   C_PosologieGrammar::DURATION
           *                                                   C_PosologieGrammar::SEQUENCE_HOUR
           *                                                   C_PosologieGrammar::MOMENT_MEAL
           *                                                   C_PosologieGrammar::INDICATION_LIST   indications list must be showed
           *                                                   C_PosologieGrammar::INDICATION_CODE   indications code must be showed
           *                                                   C_PosologieGrammar::INDICATION_TYPE   indications code typ (CIP CISP etc.) must be showed
           *         this parameter define how numerics values must be converted in literal string.
           *  \param QString &_up_forme      if == "" the grammar string try to be used to define, this parameter is the drug unit form.
           *  \return a french human readable string (for other langs the source code is there after ...)
           */
          //---------------------------- toString ------------------------------------------------
          QString toString(const QString &_up_forme  = "" )
          {return C_PosologieGrammar::toString(m_grammar_expression,  _up_forme, m_must_be_literal);
          }

          //---------------------------- get_indicationsList  ------------------------------------------------
          /*! \brief return indication list from grammar string
           *  \param const QString &grammar_expression :     grammar input string as
           *  \return list code if present as 'code'|'typCode'|'libelle'
          */
          QStringList get_indicationsList(){return C_PosologieGrammar::get_indicationsList(m_grammar_expression);}
          //---------------------------- indication  ------------------------------------------------
          /*! \brief return indication from posologie grammar.
           *  \param const QString &grammar_expression :     grammar input string which must be modified
           *  \param , int index :     index of indication to retrieve
           *  \return indication expression 'code'|'typCode'|'libelle'
          */
          QString get_indication(int index, int flag = C_PosologieGrammar::ALL_INDIC){return C_PosologieGrammar::get_indication(m_grammar_expression, index, flag);}
          //---------------------------- setCip  ------------------------------------------------
          /*! \brief set the cip code in posologie grammar.
           *  \param const QString &cip :     cip code to set
           *  \return modified grammar expression.
          */
          QString setCip(const QString &cip) {return C_PosologieGrammar::setCip(m_grammar_expression, cip);}
          //---------------------------- cip  ------------------------------------------------
          /*! \brief return the cip code string from posologie grammar. If not present return ""
           *  \param const QString &grammar_expression :     grammar input string
           *  \return cip code if present.
          */
          QString cip (){return C_PosologieGrammar::cip(m_grammar_expression);}
          //---------------------------- setIdType [static] ------------------------------------------------
          /*! \brief set setIdType code in posologie grammar.  [static]
           *  \param const QString &grammar_expression :     grammar input string which must be modified
           *  \param const QString &setIdType :     setIdType code to set
           *  \return modified grammar expression.
          */
          QString setIdType(const QString &id_type){return C_PosologieGrammar::setIdType(m_grammar_expression, id_type);}
          //---------------------------- idType  ------------------------------------------------
          /*! \brief return the idType code string from posologie grammar. If not present return ""
           *  \param const QString &grammar_expression :     grammar input string
           *  \return idType code if present.
          */
          QString idType (){return C_PosologieGrammar::idType(m_grammar_expression);}
          //---------------------------- setDrugRoute  ------------------------------------------------
          /*! \brief set the drug route string in posologie grammar.  [static]
          *  \param const QString &grammar_expression :     grammar input string which must be modified
          *  \param const QString &voie :     drug route to set
          *  \return modified grammar expression.
         */
          QString setDrugRoute(const QString &drugRoute){return C_PosologieGrammar::setDrugRoute(m_grammar_expression, drugRoute);}
         //---------------------------- drugRoute  ------------------------------------------------
         /*! \brief return drug route string from posologie grammar. If not present return "" [static]
          *  \param const QString &grammar_expression :     grammar input string
          *  \return the drug route if present.
          */
          QString drugRoute(){return C_PosologieGrammar::drugRoute(m_grammar_expression);}
          //---------------------------- setUnitPriseForm  ------------------------------------------------
          /*! \brief set the unit prise form string in posologie grammar.
           *  \param const QString &unitUPForm :     unit prise form to set
           *  \return modified grammar expression.
          */
          QString setUnitPriseForm(const QString &unitUPForm) {return C_PosologieGrammar::setUnitPriseForm(m_grammar_expression, unitUPForm);}
      private:
          QString   m_grammar_expression;
          int       m_must_be_literal;

      };


#endif // GRAMMATTOOLS_H
