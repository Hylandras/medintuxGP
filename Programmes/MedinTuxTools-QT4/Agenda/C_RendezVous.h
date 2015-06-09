 /********************************* C_RendezVous.h ********************************
 *  #include "C_RendezVous.h"                                                     *
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
 *   Commissariat √  l'Energie Atomique                                            *
 *   - CEA,                                                                       *
 *                            31-33 rue de la F√©d√©ration, 75752 PARIS cedex 15.   *
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

#ifndef  CRENDEZ_VOUS_H
#define  CRENDEZ_VOUS_H

#include <QObject>
#include <QList>
#include <QSqlQuery>
#include <QDateTime>
#include <QTimer>
#include <QMap>
#include <QApplication>

#define SKIP_BLANK_CAR(a)       while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) (a)++
#define NEXT_LINE(a)            while( *(a) && *(a)!='\r' && *(a)!='\n')(a)++; while( *(a) && (*(a)=='\r'|| *(a)=='\n'))(a)++
#define TR QObject::tr

class C_RendezVous;

//===================================================== C_ColorType =====================================================
class C_ColorType
{
    //................... constructeur destructeur ..................................
public:
    C_ColorType(const QString &type  = "",
                const QString &color = "",
                const QString &duree = ""
               )
              {m_Type            = type;   // peut etre √  zero lors de nouveaux documents dans liste et non enregistr√©s
               m_Color           = color;  // peut etre √  zero lors de nouveaux documents dans liste et non enregistr√©s
               m_Duree           = duree;
              }
    int     getDureeInt(){return m_Duree.toInt();}
    QString getDuree()   {return m_Duree;}
    QString getType()    {return m_Type;}
    QString getColor()   {return m_Color;}
    ~C_ColorType()
              {
              }
    //................... data ......................................................
public:
    QString       m_Type;
    QString       m_Color;
    QString       m_Duree;
};

typedef QMap<QString, C_ColorType> MAP_COLOR;
typedef QList <C_RendezVous*>  RDV_LIST;
 //===================================== C_RendezVous ========================================================================
 class  C_RendezVous
 {
  Q_DECLARE_TR_FUNCTIONS(C_RendezVous)
  public:
     //RendezVous(){};
     C_RendezVous(){}
     C_RendezVous(const  QDateTime  &date ,
                  const  long       &duree    = 0,
                  const  QString    &nom      = 0,
                  const  QString    &prenom   = 0,
                  const  QString    &tel      = 0,
                  const  QString    &note     = 0,
                  const  QString    &guid     = 0,
                  const  QString    &prisAvec = 0,
                  const  QString    &prisPar  = 0,
                  const  QString    &type     = 0,
                  const  QString    &primKey  = 0,
                  const  QString    &state    = "",
                  const  QString    &where    = ""
                )
         :m_date (date),
          m_Duree(duree),
          m_Nom(nom),
          m_Prenom(prenom),
          m_Tel(tel),
          m_Note(note),
          m_GUID(guid),
          m_PrisPar(prisPar),
          m_PrisAvec(prisAvec),
          m_Type(type),
          m_PrimKey(primKey),
          m_State(state),
          m_Where(where)
     {
     }
  const QDateTime &getDateTime()                       {return m_date;}
  void             setDateTime(const QDateTime  &date) {m_date=date;}
  long             getDuree()                          {return m_Duree;}
  QString          getWhere()                          {return m_Where;}
  C_RendezVous    *rdv_instance()                      {return this;}

  //---------------------------- unSerialize ------------------------------------------------
  static C_RendezVous unSerialize(const QString & data);
  //---------------------------- serialize ------------------------------------------------
  QString serialize() const;
  static QString serialize(const C_RendezVous *pC_RendezVous);
  //---------------------------- addXmlData ------------------------------------------------
  static void addXmlData(const QString& dName, QString    data, QString &modeleXML, bool mustBeB64Protected)
  {addXmlData(dName, data.toAscii(), modeleXML, mustBeB64Protected);
  }
  //------------------------ getRdvColor ---------------------------------------
  static QString getRdvColor(const C_RendezVous & rdv, MAP_COLOR *colorProfils);
  //---------------------------- addXmlData ------------------------------------------------
  static void addXmlData(const QString& dName, QByteArray data, QString &modeleXML, bool mustBeB64Protected=FALSE)
  {QString dataName    = dName;
   QString dataToPlace = "";
   data.replace('>',"&gt;");
   data.replace('<',"&lt;");
   if (mustBeB64Protected)
      {dataName    = dataName.prepend('_');
       data        = data.toBase64();
       dataToPlace = data;
      }
   else
      {dataToPlace = data;
      }
   modeleXML += QString("   <%1>%2</%3>\n").arg(dataName, dataToPlace, dataName);
  }

  //---------------------------- getXmlData ------------------------------------------------
  static QString getXmlData(const QString& dataName, const QString& data, int *nextPos);

  //---------------------------- getQWhatsThisString ------------------------------------------------
  QString getQWhatsThisString(const QString &pathTheme, QString color);

  QDateTime  m_date;
  long       m_Duree;
  QString    m_Nom;
  QString    m_Prenom;
  QString    m_Tel;
  QString    m_Note;
  QString    m_GUID;
  QString    m_PrisPar;
  QString    m_PrisAvec;
  QString    m_Type;
  QString    m_PrimKey;
  QString    m_State;
  QString    m_Where;
 };

#endif
