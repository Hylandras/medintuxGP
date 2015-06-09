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

#include "C_SpellChecker.h"

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>
#include <QDebug>
#include <hunspell.hxx>

//================================== C_SpellChecker ======================================================
//---------------------------------- C_SpellChecker ------------------------------------------------------
/*! \brief  the constructor initialize dictionnaries and create Hunspell instance
 *  \param  dictionaryPath     path of the main dictionnary without '.dic' or '.aff' extensions. \
 *   there will be added by constructor
 *  \param  userDictionary   path of user dictionary where users words will be added
 */
C_SpellChecker::C_SpellChecker(const QString &dictionaryPath, const QString &userDictionary)
{
    m_userDictionary           = userDictionary;
    QString dictFile           = dictionaryPath + ".dic";
    QString affixFile          = dictionaryPath + ".aff";
    QByteArray dictFilePathBA  = dictFile.toLocal8Bit();
    QByteArray affixFilePathBA = affixFile.toLocal8Bit();
    m_hunspell                 = new Hunspell( affixFilePathBA.constData(), dictFilePathBA.constData() );

    // detect encoding analyzing the SET option in the affix file
    m_encoding = "ISO8859-1";
    QFile _affixFile(affixFile);
    if (_affixFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&_affixFile);
        QRegExp enc_detector("^\\s*SET\\s+([A-Z0-9\\-]+)\\s*", Qt::CaseInsensitive);
        int i = 0;
        for (QString line = stream.readLine(); i<100; line = stream.readLine())
            {++i;
             if (enc_detector.indexIn(line) > -1) {
                 m_encoding = enc_detector.cap(1);
                qDebug() << QString("Encoding set to ") + m_encoding;
                break;
            }
        }
        _affixFile.close();
    }
    m_codec = QTextCodec::codecForName(this->m_encoding.toLatin1().constData());

    if(!m_userDictionary.isEmpty()) {
        QFile userDictonaryFile(m_userDictionary);
        if(userDictonaryFile.open(QIODevice::ReadOnly)) {
            QTextStream stream(&userDictonaryFile);
            for(QString word = stream.readLine(); !word.isEmpty(); word = stream.readLine())
                put_word(word);
            userDictonaryFile.close();
        } else {
            qWarning() << "User dictionary in " << m_userDictionary << "could not be opened";
        }
    } else {
        qDebug() << "User dictionary not set.";
    }
}

//------------------------------- ~SpellChecker ----------------------------------------
/*! \brief  the destructor release Hunspell instance
 */
C_SpellChecker::~C_SpellChecker()
{ delete m_hunspell;
}

//------------------------------- spell ----------------------------------------
/*! \brief  check and verify if word exists in dictionnaries
 * \param  word : word ( in utf8 format ) to verify
 * \return bool if exits false otherwise and if so you can retrieve suggest \
 * list words by suggest(const QString &word) method
 */
bool C_SpellChecker::spell(const QString &word)
{
    // Encode from Unicode to the encoding used by current dictionary
    return m_hunspell->spell(m_codec->fromUnicode(word).constData()) != 0;
}

//------------------------------- suggest ----------------------------------------
/*! \brief  retrieve suggest list words from input word
 * \param  word : word ( in utf8 format ) from wich retrieve list
 * \return QStringList suggest list words
 */
QStringList C_SpellChecker::suggest(const QString &word)
{
    char **suggestWordList;

    // Encode from Unicode to the encoding used by current dictionary
    int numSuggestions = m_hunspell->suggest(&suggestWordList, m_codec->fromUnicode(word).constData());
    QStringList suggestions;
    for(int i=0; i < numSuggestions; ++i) {
        suggestions << m_codec->toUnicode(suggestWordList[i]);
        free(suggestWordList[i]);
    }
    return suggestions;
}

//------------------------------- ignoreWord ----------------------------------------
/*! \brief  set a specific word as a ignored word
 * \param  word : word ( in utf8 format ) to set as ignored word
 */
void C_SpellChecker::ignoreWord(const QString &word)
{ put_word(word);
}

//------------------------------- addToUserWordlist ----------------------------------------
/*! \brief  add a specific word as a user dictionary word
 * \param  word : word ( in utf8 format ) to set as a user dictionary word
 */
void C_SpellChecker::addToUserWordlist(const QString &word)
{
    put_word(word);
    if(!m_userDictionary.isEmpty()) {
        QFile userDictonaryFile(m_userDictionary);
        if(userDictonaryFile.open(QIODevice::Append)) {
            QTextStream stream(&userDictonaryFile);
            stream << word << "\n";
            userDictonaryFile.close();
        } else {
            qWarning() << "User dictionary in " << m_userDictionary << "could not be opened for appending a new word";
        }
    } else {
        qDebug() << "User dictionary not set.";
    }
}

//------------------------------- put_word ----------------------------------------
void C_SpellChecker::put_word(const QString &word)
{
    m_hunspell->add(m_codec->fromUnicode(word).constData());
}
