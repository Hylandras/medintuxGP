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

#ifndef C_POPUPDIAL_TERRAINCHOICE_H
#define C_POPUPDIAL_TERRAINCHOICE_H

namespace Ui {
               class C_PopupDial_TerrainChoice;
             }
#include <QWidget>
#include <QObject>
#include "../../MedinTuxTools-QT4/C_PopupDial/C_PopupDial.h"
#include "../../MedinTuxTools-QT4/medicabase/C_Frm_Prescription.h"
#include "ui_C_PopupDial_TerrainChoice.h"
#include "../../MedinTuxTools-QT4/Theme/ThemePopup.h"

//========================== C_PopupDial_TerrainChoice ====================================
class C_PopupDial_TerrainChoice : public C_PopupDial, public Ui::C_PopupDial_TerrainChoice
{

     Q_OBJECT
 public:
     explicit C_PopupDial_TerrainChoice   (C_Frm_PosologieHitList *pC_Frm_PosologieHitList,
                                           CMyQLineEdit           *pCMyQLineEdit,
                                           const QColor            &backGround,
                                           const QRect             &initialRect,
                                           const QRect             &deployRect);
     ~C_PopupDial_TerrainChoice();
 protected:
     void    setFont(const QFont &fnt);
     QString interfaceToGrammar();
     void    grammarToInterface();
     bool    isNumValid(const QString &borne);
     void    menuAddVariableTriggered( const QString &optionTexte  );

 private:
     QFont                   m_fGrd;
     QString                 m_cur_grammar;
     C_Frm_PosologieHitList *m_pC_Frm_PosologieHitList;
     CMyQLineEdit           *m_pCMyQLineEdit;
     ThemePopup             *m_add_Menu;
     bool                    m_IsPopupRun;
 signals:

 public slots:
     void Slot_Dial_TerrainChoiceDeployed(int &ok);
     void Slot_Dial_TerrainChoiceMinimized(int &ok);
     void Slot_pushButton_add_clicked ( bool );
     void Slot_pushButton_del_clicked ( bool );
     void Slot_MenuAddVariableTriggered( QAction *pQAction  );
     void Slot_treeWidget_Variables_itemChanged ( QTreeWidgetItem * item, int column );

     //void Slot_treeWidget_Indications_itemClicked ( QTreeWidgetItem * , int   );
     //void Slot_treeWidget_Indications_itemDoubleClicked ( QTreeWidgetItem * , int   );

 };

#endif // C_POPUPDIAL_TERRAINCHOICE_H
