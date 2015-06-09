
/*********************************** C_Macro_Core.h *******************************
 *                                                                                *
 *   #include "C_Macro_Core.h"                                                    *
 *   Project: MedinTux                                                            *
 *   Copyright (C) for the eternity by Sevin Roland  and Data Medical Design      *
 *   E-Mail: data.medical.design@club-internet.fr                                 *
 *                                                                                *
 *   This program is free software; you can redistribute it and/or modify         *
 *   it under the terms of the CeCILL Version 1 du 21/06/2004                     *
 *   License (GPL compatible)                                                     *
 *                              http://www.cecill.info/                           *
 *   as published by :                                                            *
 *                                                                                *
 *   Commissariat a l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Federation, 75752 PARIS cedex 15.   *
 *                            FRANCE                                              *
 *   Centre National de la Recherche Scientifique                                 *
 *   - CNRS,                                                                      *
 *                            3 rue Michel-Ange, 75794 Paris cedex 16.            *
 *                            FRANCE                                              *
 *   Institut National de Recherche en Informatique et en Automatique             *
 *   - INRIA,                                                                     *
 *                            Domaine de Voluceau, Rocquencourt, BP 105, 78153    *
 *                            FRANCE                                              *
 *                                                                                *
 *         This program is distributed in the hope that it will be useful,        *
 *         but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                   *
 *         CeCILL  License (GPL compatible) for more details.                     *
 *                                                                                *
 **********************************************************************************/

#ifndef C_MACRO_CORE_H
#define C_MACRO_CORE_H

#include <QStringList>
#include <QString>
#include <QChar>
#include <QTextEdit>

#define   MACRO_IMPLEMENTATION(a)  QString C_Macro::a(QStringList &arg_list)
#define   MACRO_PROTO(a)  static QString a(QStringList &arg_list)
#define   MACRO_FUNC(a)    {#a,C_Macro::a}
#define   ALIAS_FUNC(a,b)  {#a,C_Macro::b}

typedef QMap<QString, QString> VAR_MAP;

//=============================================== KEY_MACRO ==============================================================
/*! \class   KEY_MACRO
    \brief   structure associant : un pointeur sur un mot clef, et un pointeur sur la fonction a executer pour ce mot clef,
  *   ces structures sont rassemblees au sein de 26 tableaux (un par lettre de l'alphabet),
  *   chacun de ces tableaux ( G_MacroTab_A[] ... a ... G_MacroTab_X[] )  contient triees alphabetiquement toutes les structures KEY_MACRO
  *   dont le membre : m_Token commence par la lettre pour lequel le tableau est dedie.
  */
typedef struct
       {const char       *m_Token;                   /*!< pointeur sur le mot clef de la fonction ou macro */
        QString         (*m_Function)(QStringList&); /*!< pointeur la fonction a executer pour ce mot clef */
       } KEY_MACRO;

//====================================== KEY_MACRO_TAB_DEF ==============================================================
/*! \class   KEY_MACRO_TAB_DEF
     \brief  structure stockant : un pointeur sur un tableau de KEY_MACRO et la taille en octet de ce tableau
  *   il y a 26 structures KEY_MACRO_TAB_DEF dont les 26 pointeurs sur ( G_MacroTab_A[] ... a ... G_MacroTab_X[] ) sont contenus dans le tableau G_MacroTab_List[],
  *   il y a  1 structure  KEY_MACRO_TAB_DEF par lettre de l'alphabet.
  */
typedef struct
       {KEY_MACRO        *m_MacroTab;  /*!< pointeur sur un des tableaux ( G_MacroTab_A[] ... a ... G_MacroTab_X[] ) de fonctions/clefs  KEY_MACRO[] */
        int               m_size;      /*!< taille du tableau en octets (initialise lors de la compilation) */
       } KEY_MACRO_TAB_DEF;


//====================================== C_Macro_Core ==============================================================
/*! \class   C_Macro_Core
     \brief  classe de base contenant le moteur permettant d'interpreter un script compose des instructions contenues
  *  dans les tableaux ( G_MacroTab_A[] ... a ... G_MacroTab_X[] ) eux memes rassembles dans le tableau G_MacroTab_List[]
  *  le constructeur de cette classe recoit en parametre l'adresse de ce tableau afin de disposer des instructions possibles
  */

class C_Macro_Core
{
public:
     C_Macro_Core(KEY_MACRO_TAB_DEF *pMacroTab_List, VAR_MAP *pVAR_MAP=0);
    ~C_Macro_Core();
    //----------------------------------------  run -------------------------------------------------
    /*! \brief Fusionne les champs de fusion d'un document donnee en entree par: QString *document
     *  \param  document : reference sur le document
     *  \return  le document modifie par les macros resolues
     */
    virtual QString   run ( QString &document );
    //----------------------------------------  run -------------------------------------------------
    /*! \brief Fusionne les champs de fusion d'un document donnee en entree par: QString *document
     *  \param  document : pointeur sur le document
     *  \return  le document modifie par les macros resolues
     */
    virtual QString   run ( QString *document );
    static void       setLogWidget(QTextEdit *pQTextEditLog);
    static QString   *document()         {return  m_pDocument;}
    static VAR_MAP   *getVarMap()        {return  m_pVariablesMap;}
    static VAR_MAP    varMap()           {return *m_pVariablesMap;}
    //----------------- getVarValue [static]--------------------------------------------------
    /*! \brief return value of a variable.
     *  \param varName : const QString & var name
     *  \return : contents of this varName
     */
    static QString getVarValue(const QString &var);
    //----------------- setVarValue [static]--------------------------------------------------
    /*! \brief return value of a variable.
     *  \param varName : const QString & var name
     *  \param value :  const QString & value to set for var name
     *  \return : contents of this varName
     */
    static void    setVarValue(const QString &varName, const QString &value);
    //------------------------------------ outMessage -------------------------------
    /*! \brief output a message on std error out put and logwidget if defined
     *  \param mess  const QString  message to display
     *  \param file  const QString source file from where the message is display
     *  \param line  const QString source line in file from where the message is display
     *  \return message diplayed
     */
    static     QString outMessage( const QString &mess, const QString &file  = "" , const int line  = -1);
    static     QString strToIsoStrDateTime(const QString &date, QString *pDate   =0 , QString *pTime   =0 );
    static     QString daysTo(const QString &date1, const QString &date2);
    static     QString normaliseDate(const QString &date);

public:
     static char Utf8_Insecable[3];

private:
    int        run(QString *pDocument, int pos_deb, int pos_max);
    int        replaceTokenByValue(QString *document, int pos_deb, int len , int *pDeltaLen /*=0*/);
    QString    getResolvTokenValue(QString *document, int pos_deb, int len );
    int        resolvFunctionToken(QString &resolvToken,  QString token);
    long       extractArgList(      QStringList &arg_list, const QString &arg_str);
    long       findLastDelimiter(   const QString &str,    const QChar delimiter);
    static int comparatorMacro (const void *a, const void *b);
    int        exeFunction(QString token, const QString& args, QString &resolvToken);
    //................... datas ...................................
    KEY_MACRO_TAB_DEF                        *m_MacroTab_List;
    static QString                           *m_pDocument;
    static QTextEdit                         *m_logWidget;
    static QMap <QString, QString>            m_VariablesMap;
    static VAR_MAP                           *m_pVariablesMap;
};

#endif // C_MACRO_CORE_H

