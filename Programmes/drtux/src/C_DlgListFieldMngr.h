 /************************* C_DlgListFieldMngr.h ***************************
 *  #include "C_DlgListFieldMngr.h"           QT3                          *
 * ........................................................................*
 *   Project   :  MedinTux  (typographie made in france)                   *
 *   Copyright :  (C) 2005-2006-2007-2008-2009-2010 and for the eternity   *
 *   by        :  Sevin Roland     from   MedinTux project                 *
 *   E-Mail    :  roland-sevin@medintux.org                                *
 *   Web site  :  www.medintux.org                                         *
 * ........................................................................*
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

#ifndef C_DLG_LISTFIELDMNGR_H
#define C_DLG_LISTFIELDMNGR_H

#ifndef QLIGHTPAD
  #include "../../drtux/src/ui/DlgListFieldMngr.h"
#else
  #include "../../QLightPad/src/ui/DlgListFieldMngr.h"
#endif

class QTextEdit;

//=================================== C_DlgListFieldMngr ==========================================================
class C_DlgListFieldMngr : public FormDlgListFieldMngr
{
  Q_OBJECT

public:
  C_DlgListFieldMngr(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~C_DlgListFieldMngr();
  /*$PUBLIC_FUNCTIONS$*/
  virtual void initDialog( const QString & drTuxParam, const QString & pathGlossaire, const QString & pathDrTux );
  void fillListView(const QString &path);
public:
QString        m_PathMenuLib;
QStringList    m_black_list;
public slots:
  /*$PUBLIC_SLOTS$*/
    virtual QString Slot_ImportMenuModulesLocalize( QString path_in );
    virtual QString GetPathItem( QListViewItem * qlistViewItem );
    virtual QListViewItem * FindDepthItem( QString name, int depth );
    virtual bool IsThisFileNameCompatible( const QString & qsName );
    virtual int DeleteListContent( QString path, QString name_ref );
    virtual bool IsThisFolderExist( const char * path, const char * folderName );
    virtual bool IsThisFileExist( const char * path );
    virtual QListViewItem * FindApproximativeList( const QString & name, int depth );
    virtual QString HtmlToAscii( QString str );

protected:
  /*$PROTECTED_FUNCTIONS$*/
    QListViewItem *m_pQListViewItem;
    QListViewItem *m_OldpQlistViewItem;
    QString        m_PathListes;
    QString        m_PathDrTux;
    QString        m_PathGetFile;
    QString        m_PathGlossaire;
    QString       *m_pRet;
    QString        m_DrTuxParam;
    QString        m_subOptMenu;
public slots:
    virtual void setFontEdit( QFont & font );
    virtual void listViewList_doubleClicked( QListViewItem * qlistViewItem );
    virtual void selectAndOpenItem( QString itemName );
    virtual void setReturnVal( QString * ret );
    virtual void setReturnValList( QString * ret );

protected slots:
  /*$PROTECTED_SLOTS$*/
    virtual void pushButtonImportMenuLocalize_clicked();
    virtual void pushButtonImportMenu_clicked();
    virtual void listView_RubMenus_clicked( QListViewItem * pQListViewItem );
    virtual void init_listView_RubMenus();
    virtual void accept();
    virtual void reject();
    virtual void doConnections( QTextEdit * e );
    virtual void fontChanged( const QFont & f );
    virtual void colorChanged( const QColor & c );
    virtual void alignmentChanged( int a );
    virtual void pushButtonUnderline_toggled( bool state );
    virtual void pushButtonItalique_toggled( bool state );
    virtual void pushButtonBold_toggled( bool state );
    virtual void textSize( const QString & p );
    virtual void pushButtonColor_clicked();
    virtual void buttonGroupJustif_pressed( int id );
    virtual void pushButton_InsertImage_clicked();
    virtual void AddSubFolder( QListViewItem * qlistViewItem, QString list_name );
    virtual void MenuActionModifier();
    virtual void textEditField_textChanged();
    virtual void On_lineEditFindItem_F2_Pressed();
    virtual void On_lineEditFindItem_F6_Pressed();
    virtual void listViewList_contextMenuRequested( QListViewItem * qlistViewItem, const QPoint & pos, int v );
    virtual void listViewList_rightButtonClicked( QListViewItem * qlistViewItem, const QPoint & pos, int );
    virtual void Slot_OnMenuActionSelected();
    virtual void MenuActionGotoListe();
    virtual void Slot_RestaureConnection();
    virtual void MenuActionListOffRef();
    virtual void MenuActionNewListe();
    virtual void MenuActionNewRef();
    virtual void ActionNewRef( QString & qsName );
    virtual void ActionCreateNewRef( QString & qsName );
    virtual void MenuActionNewSimple();
    virtual int  MenuActionCreateNewSimple( QString qsName, const QString & dirSrc );
    virtual void MenuActionDelete();
    virtual void MenuActionRename();
    virtual void MenuActionIsExist();
    virtual void listViewList_clicked( int button, QListViewItem * qlistViewItem, const QPoint & pos, int c );
    virtual void listViewList_returnPressed( QListViewItem * qlistViewItem );
    virtual void pushButtonSave_clicked();
    virtual bool saveTextIfModified();
    virtual void saveFile( QString path, QString text, QListViewItem * qlistViewItem );
    virtual void lineEditFindItem_textChanged( const QString & qtxt );
    virtual void pushButtonSetMasque_clicked();
    virtual void MenuActionListGet();
    virtual void MenuActionListGetFile();
    virtual void MenuActionInsertField();
    virtual void MakeInsertFieldList( const char * path, QStringList & fieldList, int mustBeClear );
    virtual void MenuActionListNewRef();
    virtual void listViewList_spacePressed( QListViewItem * pQListViewItem );
    virtual void pushButtonSetTextField_clicked();
    virtual void lineEditFindItem_returnPressed();
    virtual void pushButton_Quitter_clicked();
    virtual void pushButton_Ok_clicked();
};
#endif





