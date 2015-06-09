/*********************************** C_Macro.h ************************************
 *                                                                                *
 *   #include "C_Macro.h"                                                         *
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
 *   Commissariat à l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la Fédération, 75752 PARIS cedex 15.   *
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

#ifndef C_MACRO_H
#define C_MACRO_H

#include "../../MedinTuxTools-QT4/C_Macro_Core.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"
#include "../../MedinTuxTools-QT4/medicabase/C_BDM_Api.h"

#include  <QObject>
#include  <QProgressBar>
#include  <QWebView>

//.......... forUse_G_MacroTab_List ............
static int forUse_G_MacroTab_List();

//======================================== C_Macro =========================================
class C_Macro : public C_Macro_Core
{

public:
    C_Macro(QObject *parentForC_Utils_Html);
    ~C_Macro();

    //...................... utils fonctions ........................
    //------------------------------------ setQNetworkAccessManager -------------------------------
    /*! \brief set the QNetworkAccessManager
     *  \param pQNetworkAccessManager pointer on QNetworkAccessManager to set
     */
    static void                   setQNetworkAccessManager(QNetworkAccessManager *pQNetworkAccessManager);
    //------------------------------------ setQNetworkAccessManager -------------------------------
    /*! \brief get the QNetworkAccessManager
     *  \return pQNetworkAccessManager pointer on QNetworkAccessManager
     */
    static QNetworkAccessManager *getQNetworkAccessManager();
    //------------------------------------ setQProgressBar -------------------------------
    /*! \brief set the QProgressBar
     *  \param pQProgressBar pointer on QProgressBar to set
     */
    static void                   setQProgressBar(QProgressBar *pQProgressBar);

    //------------------------------------ set_BDM_Plugin -------------------------------
    /*! \brief set the G_pC_BDM_PluginI global pointeur on DBM plugin
     *  \param C_BDM_PluginI *pC_BDM_PluginI pointer on C_BDM_PluginI to set
     */
    static void                   set_BDM_Plugin(C_BDM_PluginI *pC_BDM_PluginI);
    //------------------------------------ setQWebView -------------------------------
    /*! \brief set the QWebView
     *  \param QWebView pointer on QWebView to set
     */
    void                   setQWebView(QWebView *pQWebView){m_pQWebView = pQWebView;}
    QWebView              *getQWebView(){return m_pQWebView;}
    //----------------------------------------------- setLogWidget ---------------------------------------------------------------------
    /*! \brief  initialise class on a QTextEdit display logwidget
     *  \param  QTextEdit *logWidget                pointer on the logwidget
    */
    void        setLogWidget(QTextEdit *logWidget);

    //.......... eviter message Not Used ............
    int justForUsed() {return forUse_G_MacroTab_List();}
    //------------------------------------ outMessage -------------------------------
    /*! \brief output a message on std error out put and logwidget if defined
     *  \param mess  const QString  message to display
     *  \param file  const QString source file from where the message is display
     *  \param line  const QString source line in file from where the message is display
     *  \return message diplayed
     */
    static void                   outMessage( const QString &mess, const QString &file  = "" , const int line  = -1);

    static QString                func_Extract(const QString &text, QString &patern_deb, QString &patern_end, QString &find_to, QString &replace_by, const QString justNum_in);
    static QString                loadContentsIfBeginByTokenFile(QString &arg);
    static QString                resolvePath(QString pathFile);
    static QString                resolvePathKeyWord(QString pathFile);
    static C_Utils_Html          *u_Html();
    static QWebView              *pQWebView();

    //...................... macros fonctions ........................
    static QString ABS_PATH             (QStringList &arg_list);
    static QString NOP                  (QStringList &arg_list);
    static QString CALC                 (QStringList &arg_list);
    static QString CLEAR_SPACES         (QStringList &arg_list);
    static QString CUT                  (QStringList &arg_list);
    static QString DATECOURANTE         (QStringList &arg_list);
    static QString DATE_CALC            (QStringList &arg_list);
    static QString DATE_FROMNUM         (QStringList &arg_list);
    static QString DATE_TONUM           (QStringList &arg_list);
    static QString DAYSTO               (QStringList &arg_list);
    static QString ECHO                 (QStringList &arg_list);
    static QString EXE_JAVA_FUNC        (QStringList &arg_list);
    static QString EXE_PROCESS          (QStringList &arg_list);
    static QString EXTRACT              (QStringList &arg_list);
    static QString GET_DISPLAY_PAGE     (QStringList &arg_list);
    static QString INDEX_OF             (QStringList &arg_list);
    static QString LEFT                 (QStringList &arg_list);
    static QString LENGTH               (QStringList &arg_list);
    static QString LOAD_URL             (QStringList &arg_list);
    static QString LOAD_URL_IN_VAR      (QStringList &arg_list);
    static QString LOWER                (QStringList &arg_list);
    static QString MID                  (QStringList &arg_list);
    static QString OS                   (QStringList &arg_list);
    static QString POST_DATA            (QStringList &arg_list);
    static QString PUT_PAGE_IN_BASE     (QStringList &arg_list);
    static QString PUT_VAR              (QStringList &arg_list);
    static QString DISPLAY_PAGE         (QStringList &arg_list);
    //
    static QString REPLACE              (QStringList &arg_list);
    static QString RIGHT                (QStringList &arg_list);
    static QString SET_VAR              (QStringList &arg_list);
    static QString SIMPLIFY             (QStringList &arg_list);
    static QString TEST                 (QStringList &arg_list);
    static QString TRIMMED              (QStringList &arg_list);

    static QString TO_NUM               (QStringList &arg_list);
    static QString UPPER                (QStringList &arg_list);
    static QString VAR_LIST_CLEAR       (QStringList &arg_list);
    static QString VAR_SPLIT            (QStringList &arg_list);

    /*
    static QString DO_LISTE             (QStringList &arg_list);
    static QString EXIT                 (QStringList &arg_list);
    static QString FILE_EXISTS          (QStringList &arg_list);
    static QString FILL                 (QStringList &arg_list);
    static QString GET_SAVE_FILE_NAME   (QStringList &arg_list);
    static QString GET_SELECTION        (QStringList &arg_list);
    static QString HHHMM                (QStringList &arg_list);
    static QString IMPORT               (QStringList &arg_list);
    static QString INCLUDE              (QStringList &arg_list);
    static QString INSERT_IMAGE         (QStringList &arg_list);
    static QString LEFT                 (QStringList &arg_list);
    static QString LENGTH               (QStringList &arg_list);
    static QString LIST_DIR             (QStringList &arg_list);
    static QString LOWER                (QStringList &arg_list);
    static QString MID                  (QStringList &arg_list);
    static QString REPLACE_BY           (QStringList &arg_list);
    static QString REPLACE_IN_FILE      (QStringList &arg_list);
    static QString RIGHT                (QStringList &arg_list);
    static QString TEST                 (QStringList &arg_list);
    static QString SIMPLIFY             (QStringList &arg_list);
    static QString UPPER                (QStringList &arg_list);
    */


private:
    QWebView      *m_pQWebView;
};



//...................................... tableaux de fonctions .............................................................
static KEY_MACRO      G_MacroTab_A[ ] = {{"ABS_PATH",                       C_Macro::ABS_PATH},
                                         {"AP",                             C_Macro::ABS_PATH}
                                        };
static KEY_MACRO      G_MacroTab_B[ ] = {{"B_NOP",                          C_Macro::NOP}
                                        };
static KEY_MACRO      G_MacroTab_C[ ] = {{"CALC",                           C_Macro::CALC},
                                         {"CLEAR_SPACES",                   C_Macro::TRIMMED},
                                         {"CS",                             C_Macro::TRIMMED},
                                         {"CUT",                            C_Macro::CUT}
                                        };
static KEY_MACRO      G_MacroTab_D[ ] = {{"DATE",                           C_Macro::DATECOURANTE},
                                         {"DATECOURANTE",                   C_Macro::DATECOURANTE},
                                         {"DATE_CALC",                      C_Macro::DATE_CALC},
                                         {"DATE_FROMNUM",                   C_Macro::DATE_FROMNUM},
                                         {"DATE_TONUM",                     C_Macro::DATE_TONUM},
                                         {"DAYSTO",                         C_Macro::DAYSTO},
                                         {"DD-MM-YYYY",                     C_Macro::DATECOURANTE},
                                         {"DISPLAY_PAGE",                   C_Macro::DISPLAY_PAGE},
                                         //{"DO_LISTE",                       C_Macro::DO_LISTE},
                                         {"DT",                             C_Macro::DATECOURANTE},
                                         {"DTC",                            C_Macro::DATE_CALC},
                                         {"DTFN",                           C_Macro::DATE_FROMNUM},
                                         {"DTN",                            C_Macro::DATE_TONUM}
                                        };
static KEY_MACRO      G_MacroTab_E[ ] = {{"ECHO",                           C_Macro::ECHO},
                                         {"EXE_JAVA_FUNC",                  C_Macro::EXE_JAVA_FUNC},
                                         {"EXE_PROCESS",                    C_Macro::EXE_PROCESS},
                                         //{"EXIT",                           C_Macro::EXIT},
                                         {"EXTRACT",                        C_Macro::EXTRACT}
                                        };

static KEY_MACRO      G_MacroTab_F[ ] = {{"F_NOP",                          C_Macro::NOP}/*
                                         {"FE",                             C_Macro::FILE_EXISTS},
                                         {"FILE_EXISTS",                    C_Macro::FILE_EXISTS},
                                         {"FILL",                           C_Macro::FILL} */
                                        };
static KEY_MACRO      G_MacroTab_G[ ] = {{"GET_DISPLAY_PAGE",               C_Macro::GET_DISPLAY_PAGE} /*
                                         {"GET_SAVE_FILE_NAME",             C_Macro::GET_SAVE_FILE_NAME},
                                         {"GET_SELECTION",                  C_Macro::GET_SELECTION}*/
                                        };
static KEY_MACRO      G_MacroTab_H[ ] = {{"H_NOP",                          C_Macro::NOP}/*
                                         {"HEURECOURANTE",                  C_Macro::HHHMM},
                                         {"HHHMM",                          C_Macro::HHHMM}*/
                                        };
static KEY_MACRO      G_MacroTab_I[ ] = {// {"IMPORT",                         C_Macro::IMPORT},
                                         // {"INCLUDE",                        C_Macro::INCLUDE},
                                         {"INDEX_OF",                       C_Macro::INDEX_OF},    /*
                                         {"INSERT_IMAGE",                   C_Macro::INSERT_IMAGE},*/
                                         {"IO",                             C_Macro::INDEX_OF}
                                        };
static KEY_MACRO      G_MacroTab_J[ ] = {{"J_NOP",                          C_Macro::NOP}
                                        };
static KEY_MACRO      G_MacroTab_K[ ] = {{"K_NOP",                          C_Macro::NOP}
                                        };
static KEY_MACRO      G_MacroTab_L[ ] = {{"LEFT",                           C_Macro::LEFT},
                                         {"LEN",                            C_Macro::LENGTH},
                                         {"LENGTH",                         C_Macro::LENGTH},/*
                                         {"LIST_DIR",                       C_Macro::LIST_DIR},*/
                                         {"LOAD_URL",                       C_Macro::LOAD_URL},
                                         {"LOAD_URL_IN_VAR",                C_Macro::LOAD_URL_IN_VAR},
                                         {"LOWER",                          C_Macro::LOWER}
                                        };
static KEY_MACRO      G_MacroTab_M[ ] = {{"MID",                            C_Macro::MID}
                                         };
static KEY_MACRO      G_MacroTab_N[ ] = {{"NOP",                            C_Macro::NOP}
                                        };
static KEY_MACRO      G_MacroTab_O[ ] = {{"OS",                             C_Macro::OS}
                                        };
static KEY_MACRO      G_MacroTab_P[ ] = {{"POST_DATA",                      C_Macro::POST_DATA},
                                         {"PUT_PAGE_IN_BASE",               C_Macro::PUT_PAGE_IN_BASE},
                                         {"PUT_VAR",                        C_Macro::PUT_VAR}
                                        };
static KEY_MACRO      G_MacroTab_Q[ ] = {{"Q_NOP",                          C_Macro::NOP}};
static KEY_MACRO      G_MacroTab_R[ ] = {/*
                                         {"RB",                             C_Macro::REPLACE_BY},
                                         {"REM",                            C_Macro::NOP},
                                         {"REMARQUE",                       C_Macro::NOP},*/
                                         {"REMOVE",                         C_Macro::SIMPLIFY},
                                         {"REPLACE",                        C_Macro::REPLACE},/*
                                         {"REPLACE_IN_FILE",                C_Macro::REPLACE_IN_FILE},
                                         {"RF",                             C_Macro::REPLACE_IN_FILE},*/
                                         {"RIGHT",                          C_Macro::RIGHT} ,
                                         {"RM",                             C_Macro::SIMPLIFY}
                                         };
static KEY_MACRO      G_MacroTab_S[ ] = {{"SET_VAR",                        C_Macro::SET_VAR},
                                         {"SI",                             C_Macro::TEST},
                                         {"SIMPLIFY",                       C_Macro::SIMPLIFY}
                                        };
static KEY_MACRO      G_MacroTab_T[ ] = {{"TEST",                           C_Macro::TEST},
                                         {"TN",                             C_Macro::TO_NUM},
                                         {"TO_NUM",                         C_Macro::TO_NUM},
                                         {"TRIMMED",                        C_Macro::TRIMMED}
                                        };
static KEY_MACRO      G_MacroTab_U[ ] = {{"UPPER",                          C_Macro::UPPER}
                                       };
static KEY_MACRO      G_MacroTab_V[ ] = {{"VAR",                            C_Macro::PUT_VAR},
                                         {"VAR_LIST_CLEAR",                 C_Macro::VAR_LIST_CLEAR},
                                         {"VAR_SET",                        C_Macro::SET_VAR},
                                         {"VAR_SPLIT",                      C_Macro::VAR_SPLIT}
                                        };
static KEY_MACRO      G_MacroTab_W[ ] = {{"W_NOP",                          C_Macro::NOP}};
static KEY_MACRO      G_MacroTab_X[ ] = {{"X_NOP",                          C_Macro::NOP}};
static KEY_MACRO      G_MacroTab_Y[ ] = {{"Y_NOP",                          C_Macro::NOP}};
static KEY_MACRO      G_MacroTab_Z[ ] = {{"Z_NOP",                          C_Macro::NOP}};


//...................................... tableaux des tableaux de fonctions .............................................................
//                                       un tableau par lettre de l'alphabet
static KEY_MACRO_TAB_DEF G_MacroTab_List[]  ={ {G_MacroTab_A, sizeof(G_MacroTab_A)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_B, sizeof(G_MacroTab_B)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_C, sizeof(G_MacroTab_C)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_D, sizeof(G_MacroTab_D)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_E, sizeof(G_MacroTab_E)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_F, sizeof(G_MacroTab_F)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_G, sizeof(G_MacroTab_G)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_H, sizeof(G_MacroTab_H)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_I, sizeof(G_MacroTab_I)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_J, sizeof(G_MacroTab_J)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_K, sizeof(G_MacroTab_K)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_L, sizeof(G_MacroTab_L)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_M, sizeof(G_MacroTab_M)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_N, sizeof(G_MacroTab_N)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_O, sizeof(G_MacroTab_O)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_P, sizeof(G_MacroTab_P)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_Q, sizeof(G_MacroTab_Q)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_R, sizeof(G_MacroTab_R)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_S, sizeof(G_MacroTab_S)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_T, sizeof(G_MacroTab_T)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_U, sizeof(G_MacroTab_U)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_V, sizeof(G_MacroTab_V)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_W, sizeof(G_MacroTab_W)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_X, sizeof(G_MacroTab_X)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_Y, sizeof(G_MacroTab_Y)  / sizeof(KEY_MACRO)},
                                               {G_MacroTab_Z, sizeof(G_MacroTab_Z)  / sizeof(KEY_MACRO)}
                                             };
//.......... forUse_G_MacroTab_List ..........................
int forUse_G_MacroTab_List() {return G_MacroTab_List->m_size;}

#endif // C_MACRO_H
