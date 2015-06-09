/*********************************************************************************
 *                                                                                *
 * ...............................................................................*
 *   Project   :  MedinTux  (typographie made in france)                          *
 *   Copyright : (C) 2004-2005-2006-2007-2008-2009-2010 and for the eternity      *
 *   by        :  Sevin Roland     from   MedinTux project                        *
 *   E-Mail    : roland-sevin@medintux.org                                        *
 *   Web site  : www.medintux.org                                                 *
 * ...............................................................................*
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

#ifndef C_SPELLCHECKER_H
#define C_SPELLCHECKER_H

#include <QString>

class Hunspell;

//================================== C_SpellChecker ==========================================
class C_SpellChecker
{
public:
    //---------------------------------- C_SpellChecker ------------------------------------------------------
    /*! \brief  the constructor initialize dictionnaries and create Hunspell instance
     *  \param  dictionaryPath :  path of the main dictionnary without '.dic' or '.aff' extensions. \
     *   there will be added by constructor
     *  \param  userDictionary :  path of user dictionary where users words will be added
     */
     C_SpellChecker(const QString &dictionaryPath, const QString &userDictionary);
     //------------------------------- ~SpellChecker ----------------------------------------
     /*! \brief  the destructor release Hunspell instance
      */
    ~C_SpellChecker();
     //------------------------------- spell ----------------------------------------
     /*! \brief  check and verify if word exists in dictionnaries
      * \param  word : word ( in utf8 format ) to verify
      * \return bool if exits false otherwise and if so you can retrieve suggest \
      * list words by suggest(const QString &word) method
      */
    bool         spell(const QString &word);
    //------------------------------- suggest ----------------------------------------
    /*! \brief  retrieve suggest list words from input word
     * \param  word : word ( in utf8 format ) from wich retrieve list
     * \return QStringList suggest list words
     */
    QStringList  suggest(const QString &word);
    //------------------------------- ignoreWord ----------------------------------------
    /*! \brief  set a specific word as a ignored word
     * \param  word : word ( in utf8 format ) to set as ignored word
     */
    void         ignoreWord(const QString &word);
    //------------------------------- addToUserWordlist ----------------------------------------
    /*! \brief  add a specific word as a user dictionary word
     * \param  word : word ( in utf8 format ) to set as a user dictionary word
     */
    void         addToUserWordlist(const QString &word);

private:
    void put_word(const QString &word);
    Hunspell    *m_hunspell;
    QString      m_userDictionary;
    QString      m_encoding;
    QTextCodec * m_codec;
};


#endif // C_SPELLCHECKER_H
