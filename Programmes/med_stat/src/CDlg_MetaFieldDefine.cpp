 /********************************* CDlg_MetaFieldDefine.cpp **********************************
 \file CDlg_MetaFieldDefine.cpp
 \brief (put some short descripion here).

     #include "CDlg_MetaFieldDefine.h"
     Project: MedinTux
     Copyright (C) 2003 2004 2005 2006 2007
     by Sevin Roland  and Data Medical Design
     E-Mail: medintux@medintux.org

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



#include "CDlg_MetaFieldDefine.h"
#include "CMetaField.h"
#include <qregexp.h>

//----------------------------------------- CDlg_MetaFieldDefine ----------------------------------------------------------------------
CDlg_MetaFieldDefine::CDlg_MetaFieldDefine(QListView * pQListView, QListViewItem* pQListViewItem, QWidget* parent, const char* name, bool modal, WFlags fl)
: Dlg_MetaFieldDefine(parent,name, modal,fl)
{CDlg_MetaFieldDefineInitDialog(pQListView,  pQListViewItem);
}

//----------------------------------------- CDlg_MetaFieldDefineInitDialog ----------------------------------------------------------------------
void CDlg_MetaFieldDefine::CDlg_MetaFieldDefineInitDialog(QListView * pQListView, QListViewItem* pQListViewItem)
{ m_pQListView       = pQListView;
  m_pQListViewItem   = pQListViewItem;
  int isNewCreated   = FALSE;
  lineEdit_Name->setText(m_pQListViewItem->text(0));

  //.............................. notons que si il y a création d'un meta-champ .............................
  //                               avec  pQListViewItem(1) et pQListViewItem(2) vide
  //                               c'est qu'il est nouvellement cree
  QStringList   list = CMetaField::GetPossiblesRubTypesList();
  if (m_pQListViewItem->text(1).length()==0) {m_pQListViewItem->setText(1,list[0]); isNewCreated=TRUE;};        // si vide (nouvelle définition) on renseigne par defaut

  //.....................liste des rubriques disponible  ............................
  comboBox_TypeRubrique->insertStringList ( list );
  QListBoxItem* pQListBoxItem =  comboBox_TypeRubrique->listBox()->findItem ( m_pQListViewItem->text(1), Qt::ExactMatch );
  if (pQListBoxItem)
    { comboBox_TypeRubrique->listBox()->setCurrentItem (pQListBoxItem );
      comboBox_TypeRubrique->setCurrentText (m_pQListViewItem->text(1) );
      textLabel_SyntaxDispo->setText(tr("Syntaxes d'extraction des fonction disponibles pour : <font color=\"#ff0000\">") + m_pQListViewItem->text(1) + "</font>");
    }
  //.....................initialiser le combobox avec liste des syntaxes disponible pour la rubrique ............................
  //
  list.clear();
  CMetaField::GetSyntaxListFromRubriqueName(m_pQListViewItem->text(1), list);
  comboBox_SyntaxModels->insertStringList (list );
  if (m_pQListViewItem->text(2).length()==0) m_pQListViewItem->setText(2,list[0]);        // si vide (nouvelle définition) on renseigne par defaut

  //....................... isoler la fonction des parametres ......................................................................
  QStringList argList;
  QString     texte = m_pQListViewItem->text(2);
  QString function  = "";
  int len           = 0;
  QRegExp rx( "[(].*[)]" );
  int pos           = rx.match ( texte, 0, &len );
  if (pos!=-1)
     {QString param = texte.mid(pos+1,len-2).stripWhiteSpace();
      if (param.length()) argList       = QStringList::split(',' , param);
      function      = texte.left(pos).stripWhiteSpace();
     }

  //................. recuperer modele de la fonction ......................
  //                  pour créer la table relative à cette fonction
  QString func_modele =  CMetaField::TokenSyntaxToFunctionSyntax(function);
  pQListBoxItem       =  comboBox_SyntaxModels->listBox()->findItem ( func_modele, Qt::ExactMatch );
  if (pQListBoxItem)
    { comboBox_SyntaxModels->listBox()->setCurrentItem (pQListBoxItem );
      comboBox_SyntaxModels->setCurrentText ( func_modele );
    }
  comboBox_SyntaxModels_activated( func_modele );
  //................. placer les parametres dans la table ...................
  int nb = argList.count();
  for (int i=0; i<nb; ++i)
      {if (isNewCreated)  table_Param->setText ( i, 0, "" );
       else               table_Param->setText ( i, 0, argList[i].stripWhiteSpace() );
      }
  table_Param->adjustColumn ( 0 );
  textEdit_SyntaxResult->hide();
}

//----------------------------------------- ~CDlg_MetaFieldDefine ----------------------------------------------------------------------
CDlg_MetaFieldDefine::~CDlg_MetaFieldDefine()
{
}

//----------------------------------------- IfThisNameExist ---------------------------------------------------------
QListViewItem * CDlg_MetaFieldDefine::IfThisNameExist( QListView * pQListView, const QString & name )
{       QListViewItem *pQListViewCurrentItem = pQListView->currentItem();
        QListViewItemIterator it( pQListView );
        while ( it.current() )
        {
            QListViewItem *pQListViewItem = it.current();
            if (pQListViewCurrentItem!=pQListViewItem  &&  pQListViewItem->text(0)==name) return pQListViewItem;
            ++it;
        }
 return 0;
}

//----------------------------------------- table_ParamClear ----------------------------------------------------------------------
void CDlg_MetaFieldDefine::table_ParamClear()
{
     int i, nb;
     nb = table_Param->numRows();
     for (i=0; i<nb; i++)
         {table_Param->removeRow(0);
         }
}

/*$SPECIALIZATION$*/
//----------------------------------------- comboBox_SyntaxModels_activated ----------------------------------------------------------------------
void CDlg_MetaFieldDefine::comboBox_SyntaxModels_activated( const QString &expression )
{QString     function;
 QStringList argList;
 CMetaField::decomposeToken(expression, function, argList);
 int nb = argList.count();
 table_ParamClear();
 if (nb)
    {table_Param->insertRows ( 0, nb);
     table_Param->setRowLabels ( argList );
     table_Param->show();
     textLabel_FoncParam->show();
     textLabel_FoncParam->setText(tr("Paramètres d'entrée de la fonction : <font color=\"#ff0000\">") + function + "</font>");
    }
 else
    {table_Param->hide();
     textLabel_FoncParam->hide();
    }

}

//----------------------------------------- comboBox_TypeRubrique_activated ----------------------------------------------------------------------
void CDlg_MetaFieldDefine::comboBox_TypeRubrique_activated( const QString &rubType )
{ QString      actualModel  = comboBox_SyntaxModels->currentText();
  int                  pos  = actualModel.find('(');
  if (pos!=-1) actualModel  = actualModel.left(pos-1);
  actualModel               = actualModel.stripWhiteSpace();


  QStringList list;
  CMetaField::GetSyntaxListFromRubriqueName(rubType, list);
  comboBox_SyntaxModels->clear();
  comboBox_SyntaxModels->insertStringList (list );
  textLabel_SyntaxDispo->setText(tr("Fonction d'extractions disponibles pour : <font color=\"#ff0000\">") + rubType + "</font>");
  //.................. chercher dans la liste de cette rubrique .......................
  //                   si la syntaxe deja en place exite
  for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
      { QString newStr = *(it);
        pos  = newStr.find('(');
        if (pos!=-1) newStr  = newStr.left(pos-1);
        newStr               = newStr.stripWhiteSpace();
        if (newStr==actualModel)
           {comboBox_SyntaxModels_activated( *(it) );
            return;       // cette syntaxes besoin d'effacer la liste
           }
      }
 comboBox_SyntaxModels_activated( list[0] );
}

//----------------------------------------- reject ----------------------------------------------------------------------
void CDlg_MetaFieldDefine::reject()
{
  QDialog::reject();
}

//----------------------------------------- accept ----------------------------------------------------------------------
void CDlg_MetaFieldDefine::accept()
{ if (IfThisNameExist( m_pQListView, lineEdit_Name->text() ))
     {QMessageBox::critical (0, QObject::tr("Définir un méta-champ"),
                                QObject::tr("Le nom de ce  méta-champ existe déjà\r\nVeuillez recommencer avec un autre nom"),
                                QMessageBox::Abort, QMessageBox::NoButton, QMessageBox::NoButton );
     }
  else
     { if (m_pQListViewItem)
          { m_pQListViewItem->setText(0,lineEdit_Name->text());
            m_pQListViewItem->setText(1,comboBox_TypeRubrique->currentText());
            QString model  = comboBox_SyntaxModels->currentText();
            int       pos  = model.find('(');
            if (pos!=-1) model  = model.left(pos-1);
            model  = model.stripWhiteSpace();
            model += " ( ";
            int i, nb;
            nb = table_Param->numRows();
            //........................... replacer le contenu de l'éditeur d cellue courant dans la cellule .............................
            //                            (comme après appui sur la touche return )
            QTableItem *pQTableitem       = table_Param->item(  table_Param->currentRow () ,  table_Param->currentColumn ()  );
            QWidget    *pQTableCellWidget = table_Param->cellWidget(  table_Param->currentRow () ,  table_Param->currentColumn ()  );
            //............................. recuperer le contenu de toutes les celllules ..................................................
            if (pQTableitem && pQTableCellWidget)  pQTableitem->setContentFromEditor ( pQTableCellWidget  );
            for (i=0; i<nb; i++)
                {if (i>0)  model += " , ";
                 model += table_Param->text ( i, 0 );
                }
            model += " )";
            m_pQListViewItem->setText(2, model);
          }
       QDialog::accept();
     }
}




