/************************** QLightPad.h ***********************************
 *   Copyright (C) 2004 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef QLightPad_H
#define QLightPad_H

#include <qmainwindow.h>
#include <qmap.h>
#include <qobject.h>
#include <qwidget.h>
#include <qprocess.h>
#include "SaveRequest.h"
#include "DLG_A_propos.h"
//===============================================================  QLightPad =========================================================================
class QAction;
class QComboBox;
class QTabWidget;
class QTextEdit;
class MyEditText;

typedef QMap<MyEditText*, QString> QMAP_EDIT_FNAME;

class QLightPad : public QMainWindow
{
    Q_OBJECT

public:
    QLightPad( QWidget *parent = 0, const char *name = 0,  int argc = 0, char ** argv = 0);
    ~QLightPad();
    void  setModifiedAll(bool mode = FALSE);
private:
    void        setupFileActions();
    void        setupToolsActions();
    void        setupEditActions();
    void        setupTextActions();
    void        setupHelpActions();
    void        setupDrTuxPopupActions();

    void        load( const QString &f );
    void        createTabPopup(MyEditText *edit);
    void        closeEvent( QCloseEvent* ce );
    int         closeAll(int mode=CSaveRequest::SaveCurrentRequest, MyEditText* editToNotClose = 0);
    MyEditText *currentEditor() const;
    int         closeEditor(MyEditText *edit, int saveRequest = CSaveRequest::SaveCurrentRequest);
    void        doConnections( QTextEdit *e );
    void        fileSave(MyEditText *edit, const QString &metaTag="");
    void        tryToStopAPropos();
private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveMeta();
    void fileSaveAs();
    void filePrint();
    void fileClose();
    void fileExit();

    void editUndo();
    void editRedo();
    void editCut();
    void editCopy();
    void editPaste();

    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily( const QString &f );
    void textSize( const QString &p );
    void textStyle( int s );
    void textColor();
    void textAlign( QAction *a );

    void fontChanged( const QFont &f );
    void colorChanged( const QColor &c );
    void alignmentChanged( int a );
    void editorChanged( QWidget * );
    void tabRightClicked(int id);
    void ParametresParDefaut();
    void InsererImage();
    void InsererAncre();
    void InsererLien();
    void InsererEmail();
    void InsererLigne();
    void InsererListe();
    void InsererPuce();
    void setAutoRet();
    void aPropos();
    void aProposDelete();
    void aboutQt();
    void MenuActionFermer();
    void MenuActionFermerTousLesAutres();
    void creerZoneMenu();
    void AjouterItemZoneMenu();
    void InsererZoneMenu();
    void testerZoneMenu();
    void gererListeMenu();
    void InsererChampInsertion();
    void MakeInsertFieldList(const char *path, QStringList &fieldList, int mustBeClear );
    void InsererFonction();
    void CodePre();
    void RetirerInsecables();
    void InsererInsecables();
    void Slot_actionApropos();
    void Slot_actionAproposDisplay();
    void Slot_SauverLesMeubles();
public:
    int         m_AnchorSpaceToUnderscore;
private:
    QAction    *m_actionTextBold,
               *m_actionTextUnderline,
               *m_actionTextItalic,
               *m_actionTextColor,
               *m_actionAlignLeft,
               *m_actionAlignCenter,
               *m_actionAlignRight,
               *m_actionAlignJustify;
               //*m_actionAutoRet;
    QComboBox  *m_comboStyle,
               *m_comboFont,
               *m_comboSize;
    QTabWidget *m_tabWidget;
    QString     m_NUM_VERSION;
    QProcess   *m_Apropos_Proc;

    QMAP_EDIT_FNAME     m_FileNames;
    MyEditText         *m_EditTabMenuSelected;
    Dlg_A_Propos       *m_Dlg_A_Propos;
    bool                m_AppMustQuit;
    QString             m_PathGlossaire;
    QString             m_PathDrTux;
    QString             m_PathTheme;
    QStringList         m_InsertFieldList;
    QStringList         m_docToLoadList;
    int                 m_debug;
};


#endif
