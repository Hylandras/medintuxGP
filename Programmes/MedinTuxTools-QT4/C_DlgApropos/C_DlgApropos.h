/********************************* C_DlgApropos.h ********************************
*  #include "C_DlgApropos.h"                                                     *
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

#ifndef C_DLGAPROPOS_H
#define C_DLGAPROPOS_H

#include <QDialog>
#include <QLineEdit>
#include <QUrl>

namespace Ui {
    class C_DlgApropos;
}

class C_DlgApropos : public QDialog {
    Q_OBJECT
public:
    C_DlgApropos(QWidget *parent,
                 const QString &pathAppli,
                 const QString& module,
                 const QString& description,
                 const QString& version,
                 const QString &pathChangements,
                 const QString &pathIcone,
                 const QString &pathHelp,
                 const QString &pathApropos,
                 const QString &baseVersion);
    ~C_DlgApropos();
     void setPageHelp(        const QString &path );
     void setPageAPropos(     const QString &text );
     void setPageLicence(     const QString &text );
     void setPageChangements( const QString &text );
     static int  URL_ExecSpecialSyntax ( QString urlStr,  const QString &pathAppli );
     static void toPDF(QString path, QString destPdfFile, const QString &printerName="");
protected:
     void changeEvent(QEvent *e);
     QWidget   *m_Wdg_Changements;
     QWidget   *m_Wdg_Help;
     QWidget   *m_Wdg_Apropos;
     QWidget   *m_Wdg_Licence;

private:
    Ui::C_DlgApropos *ui;
    QString m_PathAppli;
    QString m_PathPageHelp;
    QString m_destPdf;

private slots:
    void Slot_ActivateMainWindow();            // pour avoir la fenetre en avant plan sur ubuntu unity
    void Slot_On_webView_Help_statusBarMessage (const QString &text);
    void Slot_On_webView_Help_loadStarted();
    void Slot_On_webView_Help_loadProgress(int progress);
    void Slot_On_webView_Help_loadFinished(bool ok);
    void Slot_GetNumVersionFromBin();
    void Slot_ShowChangements();
    void Slot_pushButtonrecordNumVers_clicked (bool);
    void Slot_LocationEdit_returnPressed();
    void Slot_WebFindEdit_returnPressed();
    void Slot_WebFindEdit_textChanged (const QString &text );
    void Slot_actionWebFind (bool);
    void Slot_actionWebPrint (bool);
    void Slot_actionWebHome (bool);
    void Slot_webView_urlChanged ( const QUrl &url  );
    void Slot_webView_linkClicked ( const QUrl & url );
    void Slot_webView_Apropos_linkClicked ( const QUrl & url );
    void Slot_CopyPDF();
};

#endif // C_DLGAPROPOS_H
