/********************************* CRubList.h ************************************
 *                                                                                *
 *   #include "CRubList.h"                                                        *
 *   Project: MedinTux                                                            *
 *   Copyright (C) 2003 2004 2005 by Sevin Roland  and Data Medical Design        *
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
#ifndef  CRUBLIST_H
#define  CRUBLIST_H
#include <qvaluelist.h>
#include <qstring.h>
#include <qcstring.h>
#include <qdatetime.h>
#include <qdragobject.h>
#include <qcstring.h>
#include <qobject.h>
#include <qwidget.h>

#include "../../MedinTuxTools/CGenTools.h"

#define TR QObject::tr

#define TO_DELETE   1
#define TO_REMOVE   64     // juste pour affichage de l'icone qui va bien
#define TO_UPDATE   2
#define TO_MASK     4
#define TO_DEBUG    8
#define TO_CREATE   16
#define TO_DESTROY  32
//====================================== CRubEvntPair =========================================================================
/*! \class CRubEvntPair
 *  \brief Classe de données pour stocker les relations hierarchiques entre les documents du dossier médical.
 * la structure est la representation de la table : Evnmt_Liens avec le pk de l'enregistrement
 * si le m_Pk commence par # alors les caracteres qui suivent sont celui  d'un pk provisoire unique qui sert a
 * reperer un enregistrement nouveau lors la mise à jour definitive du pk lors d'un enregistrement des donnees
*/

class CRubEvntPair
{public:
 CRubEvntPair(){}
 CRubEvntPair(const char* pere, const char* pkDoc, const char* pk, const char* dossPk)
           :m_Pere(pere), m_PkDoc(pkDoc), m_Pk(pk), m_DossPk(dossPk), m_State(0)
           {
           };
~CRubEvntPair()
           {
           };

 int  setState(int state)      {m_State |= state;   return m_State;}
 int  removeState(int state)   {m_State &= ~state;  return m_State;}
 bool IsStatePlaced(int state) { return m_State & state;}
 int  getState()               { return m_State;}
 QCString serialize() {return serialize(1);}
 QCString serialize(int withXmlHead)
    {QString           xmlHead = "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\" standalone=\"yes\" ?>";
     if (!withXmlHead) xmlHead = "";
     return QCString(  xmlHead)               +
                     "\n<CRubEvntPair>"       +
                     "\n   <m_Pere>"   +QCString( m_Pere                    )     +  "</m_Pere>"
                     "\n   <m_PkDoc>"  +QCString( m_PkDoc                   )     +  "</m_PkDoc>"
                     "\n   <m_Pk>"     +QCString( m_Pk                      )     +  "</m_Pk>"
                     "\n   <m_DossPk>" +QCString( m_DossPk                  )     +  "</m_DossPk>"
                     "\n   <m_State>"  +QCString( QString::number( m_State) )     +  "</m_State>"
                     "\n</CRubEvntPair>";
    }
 QString m_Pere;
 QString m_PkDoc;
 QString m_Pk;
 QString m_DossPk;
 int     m_State;
};

//====================================== CRubEvntList =========================================================================
/*! \class CRubEvntList
 *  \brief Classe de liste de données pour stocker les relations hierarchiques entre les documents du dossier médical.
 * la structure est la representation de la table : Evnmt_Liens avec le pk de l'enregistrement
 * si le m_Pk commence par # alors les caracteres qui suivent sont celui  d'un pk provisoire unique qui sert a
 * reperer un enregistrement nouveau lors la mise à jour definitive du pk lors d'un enregistrement des donnees
*/
class CRubEvntList : public QValueList <CRubEvntPair>
{

 public:
    CRubEvntList() : m_IsThisItemInBranch(FALSE) { }
    CRubEvntList( const CRubEvntList& l ) : QValueList<CRubEvntPair>(l), m_IsThisItemInBranch(FALSE) { }
    CRubEvntList( const QValueList<CRubEvntPair>& l ) : QValueList<CRubEvntPair>(l) , m_IsThisItemInBranch(FALSE){ }

//-------------------------------------------- Evnmt_GetPkProvisoire ---------------------------------------------------------
/*! \brief Fonction qui retourne un numero d'nregistrement provisoire unique (commence par #) permet de reperer
 *  \return QString : qui est le numero d'enregistrement provisoire
*/
QString  Evnmt_GetPkProvisoire()
{QString str_pk  = QString::number(GetLastPkProvisoire()+1);
 return  str_pk.prepend("#");
}
//-------------------------------------------- serialize ---------------------------------------------------------
/*! \brief Fonction qui retourne une chaine contenant la serialisation de la liste
 *  \param withXmlHead    int : si zero le header XML ne sera pa ajoute en tete
 *  \return QString :     qui est la serialisation de la liste
*/
QCString serialize() {return serialize(1);}
QCString serialize(int withXmlHead)
{QCString           xml = "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\" standalone=\"yes\" ?>";
 if (!withXmlHead)  xml = "";
 CRubEvntList::iterator it;
 int i=0;
 for (it = begin(); it != end(); ++it )
     {xml += QCString("\n<CRubEvntPairNumber>") +QCString(QString::number(i))+"</CRubEvntPairNumber>" + (*it).serialize(0);
      ++i;
     } //end while (pSqlQuery->next())
 return xml;
}

//-------------------------------------------- Evnmt_GetLastPkProvisoire ---------------------------------------------------------
/*! \brief Fonction qui retourne un numero d'nregistrement provisoire unique (commence par #) permet de reperer
 *  \return QString : qui est le numero d'enregistrement provisoire
*/
int  GetLastPkProvisoire()
{QString str_pk = "";
 int     lastPk = 0;
 int     pk     = 0;
 CRubEvntList::iterator it;
 for (it = begin(); it != end(); ++it )
     {str_pk = (*it).m_Pk;
      if (str_pk[0]=='#')
         {pk = str_pk.mid(1).toInt();
          if ( pk > lastPk) lastPk = pk;
         }
     } //end while (pSqlQuery->next())
 return lastPk;
}

//----------------------------------------------- Evnmt_SetDefinitivePkDoc ---------------------------------------------------
/*! \brief Fonction qui remplace le numero d'enregistrement provisoire d'un document par un definitif
 *  \param provPk           const QString : numero provisoire à changer
 *  \param defPk            const QString : numero definitif à placer
 *  \return neant
*/
void Evnmt_SetDefinitivePkDoc(const QString &provPk, const QString &defPk)
 {CRubEvntList::iterator it;
  for (it = begin(); it !=  end(); ++it )
      {if ((*it).m_PkDoc == provPk)
          {(*it).m_PkDoc = defPk;
           (*it).setState(TO_UPDATE);
          }
      }
 }
//----------------------------------------------- Evnmt_SetDefinitivePkEvnt ---------------------------------------------------
/*! \brief Fonction qui remplace le numero d'enregistrement provisoire d'un element de la vigie par un definitif
 *  \param provPk           const QString : numero provisoire à changer
 *  \param defPk            const QString : numero definitif à placer
 *  \return neant
*/
void Evnmt_SetDefinitivePkEvnt(const QString &provPk, const QString &defPk)
 {CRubEvntList::iterator it;
  for (it = begin(); it !=  end(); ++it )
      {if ((*it).m_Pere == provPk)
          {(*it).m_Pere = defPk;
           (*it).setState(TO_UPDATE);
          }
      }
 }

//----------------------------------------------- Evnmt_SetToDelete_Pk ---------------------------------------------------
/*! \brief Fonction qui met un des elements de la vigie en etat d'effacement futur
 *  \param pk_Head           const QString : pk de l'enregistrement à modifier
 *  \return neant
*/
int Evnmt_SetToDelete_Pk(const QString &pk_Head)
{ CRubEvntList::iterator it;
  for (it = begin(); it !=  end(); ++it )
      {if ((*it).m_PkDoc == pk_Head)
          {(*it).setState(TO_DELETE);
          }
      }
 return 0;
}

//----------------------------------------------- Evnmt_Set_StateOn_AllFils ---------------------------------------------------
/*! \brief Fonction qui met un des elements et toute sa descendance de la vigie sur un etat
 *  \param pk_Pere  const QString : pk de l'enregistrement parent à modifier
 *  \param state    int : etat à positionner
 *  \return neant
*/
int Evnmt_Set_StateOn_AllFils(const QString &pk_Pere, int state)
{ CRubEvntList::iterator it;
  for (it = begin(); it !=  end(); ++it )
      {if (pk_Pere == (*it).m_Pere)
          {(*it).setState(state);
            while (Evnmt_Set_StateOn_AllFils((*it).m_Pk, state)); // voir si la filliation n'est pas a effacer
          }
      }
 return 0;
}
//----------------------------------------------- Evnmt_Remove_StateOn_AllFils ---------------------------------------------------
/*! \brief Fonction qui enleve un des elements et toute sa descendance de la vigie sur un etat
 *  \param pk_Pere  const QString : pk de l'enregistrement parent à modifier
 *  \param state    int : etat à positionner
 *  \return neant
*/
int Evnmt_Remove_StateOn_AllFils(QString pk_Pere, int state)
{ CRubEvntList::iterator it;
  for (it = begin(); it !=  end(); ++it )
      {if (pk_Pere == (*it).m_Pere)
          {(*it).removeState(state);
            while (Evnmt_Remove_StateOn_AllFils((*it).m_Pk, state)); // voir si la filliation n'est pas a effacer
          }
      }
 return 0;
}

//----------------------------------------------- Evnmt_RemoveAllItemsToDestroy ---------------------------------------------------
/*! \brief Fonction qui retire de la liste tous les elements marques et positionnes sur effacement
 *  \return neant
*/
void Evnmt_RemoveAllItemsToDestroy()
{ CRubEvntList::iterator it = begin();
  while (it != end() )
      {if ((*it).IsStatePlaced(TO_DESTROY))
          {it = remove ( it );
          }
       else
          {++it;
          }
      }
}
//----------------------------------------------- GetEvenementRecordInListFromPk ---------------------------------------------------
/*! \brief Fonction qui retourne un pointeur sur un enregistrement evennement de la liste d'evennement à partir de son Pk
 *  \param pk           const QString : le pk de l'evennement
 *  \return CRubEvntPair* : pointeur sur l'enregistrement ou zero si pas trouve
*/

CRubEvntPair* GetEvenementRecordInListFromPk(const QString &pk)
{ CRubEvntList::iterator it;
  for (it = begin(); it !=  end(); ++it )
      {if ((*it).m_Pk == pk) return &(*it);
      }
 return 0;
}

//----------------------------------------------- FirstStateFindInParents ---------------------------------------------------
/*! \brief Fonction qui recherche dans les parents le premier etat trouvé
 *  \param pCRubEvntPairSrc*   CRubEvntPair * : pointeur sur l'enregistrement a partir duquel explorer les parents
 *  \param state           int :      etat à tester
 *  \return CRubEvntPair* qui est le premier parent avec l'état place sinon zéro
*/

CRubEvntPair* FirstStateFindInParents( CRubEvntPair* pCRubEvntPairSrc, int state)
{while ( (pCRubEvntPairSrc = GetParent(pCRubEvntPairSrc)) != 0)
       {if (pCRubEvntPairSrc->IsStatePlaced(state)) return pCRubEvntPairSrc;
       }
 return 0;

}

//----------------------------------------------- GetParent ---------------------------------------------------
/*! \brief Fonction qui recherche l'enregistrement parent d'un autre
 *  \param pCRubEvntPairSrc*   CRubEvntPair * : pointeur sur l'enregistrement dont on cherche le parent
 *  \return CRubEvntPair* qui est le parent sinon zéro
*/

CRubEvntPair* GetParent( CRubEvntPair* pCRubEvntPairSrc)
{
   QString pk = pCRubEvntPairSrc->m_Pere;
   if (pk=="0")              return 0;
   CRubEvntList::iterator it;
   for (it = begin(); it !=  end(); ++it )
      {if ((*it).m_Pk == pk) return &(*it);
      }
   return 0;
}

//----------------------------------------------- IsThisItemInBranch ---------------------------------------------------
/*! \brief Fonction qui recherche si un enregistrement est fils de l'autre
 *  \param pEvnmtList          EVNT_LIST *  :   pointeur sur la liste d'evennement patient
 *  \param pCRubEvntPairSrc*   CRubEvntPair * : pointeur sur l'enregistrement suppose parent de l'autre
 *  \param pk_toTest           QString :        sur le pk de l'enregistrement suppose fils de l'autre
 *  \return bool TRUE si pk_toTest est fils de pCRubEvntPairSrc
*/
bool IsThisItemInBranch( CRubEvntPair* pCRubEvntPairSrc, const QString &pk_toTest)
{  if (pCRubEvntPairSrc==0) return FALSE;
   m_IsThisItemInBranch = FALSE;
   CRubEvntList::iterator it;
   for (it = begin(); it != end(); ++it )
      {if ((*it).m_Pk == pCRubEvntPairSrc->m_Pk)
          { ExploreItemInBranch(pCRubEvntPairSrc->m_Pk, pk_toTest);
          }
      }
 return m_IsThisItemInBranch;
}

//----------------------------------------------- ExploreItemInBranch ---------------------------------------------------
/*! \brief Fonction recursive auto appelee qui explore tous les fils à la recherche d'un autre fils si un enregistrement est fils de l'autre cette fonction met la variable  m_IsThisItemInBranch a TRUE si trouve
 *  \param pEvnmtList  EVNT_LIST *  :  pointeur sur la liste d'evennement patient
 *  \param pk_pere*    QString     :  primary key de l'enregistrement suppose etre parent de l'autre
 *  \param pk_toTest   QString : sur le pk de l'enregistrement suppose fils de l'autre
 *  \return bool TRUE si l'exploration est finie
*/
 bool ExploreItemInBranch( const QString &pk_pere , const QString &pk_toTest)
{CRubEvntList::iterator it;
 for (it = begin(); it !=  end(); ++it )
      {if ((*it).m_Pere == pk_pere)
          { if ( (*it).m_Pk==pk_toTest){ m_IsThisItemInBranch = TRUE; return FALSE;}
            while (ExploreItemInBranch( (*it).m_Pk, pk_toTest ));    // voir si le fils n'a pas lui aussi des fils
          }
      }
  return FALSE;
}
protected:
  bool             m_IsThisItemInBranch;

};


//typedef QValueList<CRubEvntPair> EVNT_LIST; /*!< Liste des des rapports hierarchiques entre documents */
typedef CRubEvntList EVNT_LIST; /*!< Liste des des rapports hierarchiques entre documents */

/*! \class CRubRecord
 *  \brief Classe de données pour les documents du dossier médical. Hérite de QByteArray.
 *  Pointeur this = contenu du blob (données compilées ou brutes).
 *
 *  Exemple les Documents de la rubrique Prescription de type ordonnance contiennent :
 *  - un code XML avec le texte de l'ordonnance
 *  - un code XML avec les paramètres de l'ordonnance (médicaments utilisés, posologie...) qui seront récupérés dans les classes appropriées.
 *
 *  Exemple les Documents de la rubrique Documents contiennent :
 *  - un code XML avec le texte de permettant la gestion de l'affichage des documents multimédia
 *  - un code XML avec les documents multimédias encodés en base64.
 *

*/

//====================================== CRubRecord =========================================================================
class CRubRecord : public QByteArray
{
    //------------------------------------------------ constructeur destructeur ------------------------------------------------
public:
    CRubRecord(){}
    CRubRecord(    const char* primKey,
                   const char* libelle,
                   const char* type,
                   const char* subType,
                   const char* date,
                   const char* user,
                   const char* sign_user,
                   const char* data           = 0,
                   long        len_data       = 0,
                   const char* refBlobsPk     = "",
                   const char* fin            = "",
                         int   duree          = -1,
                   const char* prop_0         = "",
                   const char* prop_1         = "",
                   const char* prop_2         = "",
                   const char* prop_3         = "",
                   const char* prop_4         = ""
                  )
              {m_PrimKey        = primKey;               // peut etre à zero lors de nouveaux documents dans liste et non enregistrés
               m_Libelle        = libelle;
               m_Type           = type;
               m_SubType        = subType;
               m_Date           = date;
               m_User           = user;
               m_SignUser       = sign_user;
               m_Ref_Blobs_PK   = refBlobsPk;
               m_State          = 0;
               m_Fin            = fin;
               m_DureeMod       = duree;
               m_Prop_0         = prop_0;
               m_Prop_1         = prop_1;
               m_Prop_2         = prop_2;
               m_Prop_3         = prop_3;
               m_Prop_4         = prop_4;
               m_LastErr        = "";
               //  plantage à cause du delete[] dans la classe !!!!! normal car
               //  QMemArray<type> & QMemArray::assign ( const type * data, uint size )
               //  This is an overloaded member function, provided for convenience. It behaves essentially like the above function.
               //  Shallow copy. Dereferences the current array and references the array data data, which contains size elements. Returns a reference to this array.
               //  Do not delete data later; QMemArray will call free() on it at the right time.
               SetData(data, len_data);
              }
     CRubRecord( const QString &dataToUnserialize)
              {m_LastErr = unSerialise(dataToUnserialize);
              }
    ~CRubRecord()
              {
              }
    //------------------------------------------------ getLastError ------------------------------------------------
    const QString &getLastError(){return m_LastErr;}
    //------------------------------------------------ SetData ------------------------------------------------
    /*! \brief Définit le contenu des données brutes du document. Données qui seront sauvegardées dans le blob correspondant. Gestion de la mémoire au passage. */
    void SetData (const char* data, long len_data)
            { if (len_data)
                 {resize ( len_data +1);
                  memcpy( this->data(), data, len_data);
                          this->data()[len_data]=0;
                 }
              else resize (0);
            }

    //------------------------------------------------  getDateTime ------------------------------------------------
    /*! \brief retourne la date de la rubrique au format QDate. */
    QDateTime getDateTime() { return QDateTime::fromString(m_Date, Qt::ISODate); }

    //------------------------------------------------ dragRubrique .------------------------------------------------
    /*! \brief entame une action de drag and drop avec la rubrique. */
   void  dragRubrique( QWidget* pQWidget)
    {
     QStoredDrag *d = new QStoredDrag( "medintux/rubdrag", pQWidget );
                  d->setEncodedData ( getDragAndDropData() );
                  d->dragCopy();
    }

    //------------------------------------------------ serialize ------------------------------------------------
    /*! \brief retourne les donnees d'identification de la rubrique pour un drag and drop */
   QCString getDragAndDropData()
   {return serialize();
   }
   QCString serialize() {return serialize(1);}
   QCString serialize(int withXmlHead)
    {QCString          xmlHead = "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\" standalone=\"yes\" ?>";
     if (!withXmlHead) xmlHead = "";
     return QCString( xmlHead)                     +
                     "\n<Drag_MedinTux_Rub>"       +
                     "\n   <PrimKey>"     +QCString( m_PrimKey                    )     +  "</PrimKey>"
                     "\n   <libelle>"     +QCString( m_Libelle                    )     +  "</libelle>"
                     "\n   <Type>"        +QCString( m_Type                       )     +  "</Type>"
                     "\n   <subType>"     +QCString( m_SubType                    )     +  "</subType>"
                     "\n   <DateDebut>"   +QCString( m_Date                       )     +  "</DateDebut>"
                     "\n   <user>"        +QCString( m_User                       )     +  "</user>"
                     "\n   <signUser>"    +QCString( m_SignUser                   )     +  "</signUser>"
                     "\n   <refBlobPk>"   +QCString( m_Ref_Blobs_PK               )     +  "</refBlobPk>"
                     "\n   <state>"       +QCString( QString::number( m_State)    )     +  "</state>"
                     "\n   <DateFin>"     +QCString( m_Fin                        )     +  "</DateFin>"
                     "\n   <dureeLimite>" +QCString( QString::number( m_DureeMod) )     +  "</dureeLimite>"
                     "\n   <Prop_0>"      +QCString( m_Prop_0                     )     +  "</Prop_0>"
                     "\n   <Prop_1>"      +QCString( m_Prop_1                     )     +  "</Prop_1>"
                     "\n   <Prop_2>"      +QCString( m_Prop_2                     )     +  "</Prop_2>"
                     "\n   <Prop_3>"      +QCString( m_Prop_3                     )     +  "</Prop_3>"
                     "\n   <Prop_4>"      +QCString( m_Prop_4                     )     +  "</Prop_4>"
                     "\n</Drag_MedinTux_Rub>";
    }

    //------------------------------------------------unSerialise ------------------------------------------------
    /*! \brief deserialize les donnees xml vers l'objet */
   QString unSerialise(const QString &dataToExamine)
    {
      QString        xmlTag = "";
      QString   strToGet    = "";
      int       pos         = 0;

     xmlTag = "<PrimKey>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_PrimKey, pos, &m_LastErr))==-1 )        goto erreur_Out_unSerialise;       // allez une petite faiblesse
     xmlTag = "<libelle>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Libelle, pos, &m_LastErr))==-1 )        goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     xmlTag = "<Type>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Type, pos, &m_LastErr))==-1 )           goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     xmlTag = "<subType>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_SubType, pos, &m_LastErr))==-1 )        goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     xmlTag = "<DateDebut>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Date, pos, &m_LastErr))==-1 )           goto erreur_Out_unSerialise;    // si echec tenter du d�ut
     xmlTag = "<user>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_User, pos, &m_LastErr))==-1 )           goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     xmlTag = "<signUser>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_SignUser, pos, &m_LastErr))==-1 )       goto erreur_Out_unSerialise;    // si echec tenter du d�ut
     xmlTag = "<refBlobPk>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Ref_Blobs_PK, pos, &m_LastErr))==-1 )   goto erreur_Out_unSerialise;    // si echec tenter du d�ut
     xmlTag = "<state>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, strToGet, pos, &m_LastErr))==-1 )         goto erreur_Out_unSerialise;     // si echec tenter du d�ut
                                                    m_State    = strToGet.toInt();
     xmlTag = "<DateFin>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Fin, pos, &m_LastErr))==-1 )            goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     xmlTag = "<dureeLimite>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, strToGet, pos, &m_LastErr))==-1 )         goto erreur_Out_unSerialise;   // si echec tenter du d�ut
                                                    m_DureeMod = strToGet.toInt();
     xmlTag = "<Prop_0>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Prop_0, pos, &m_LastErr))==-1 )         goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     xmlTag = "<Prop_1>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Prop_1, pos, &m_LastErr))==-1 )         goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     xmlTag = "<Prop_2>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Prop_2, pos, &m_LastErr))==-1 )         goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     xmlTag = "<Prop_3>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Prop_3, pos, &m_LastErr))==-1 )         goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     xmlTag = "<Prop_4>";
     if ( (pos = CGenTools::GetXMLValue(dataToExamine, xmlTag, m_Prop_4, pos, &m_LastErr))==-1 )         goto erreur_Out_unSerialise;     // si echec tenter du d�ut
     return QString::null;

erreur_Out_unSerialise:
     return TR( " ERREUR unSerialise() : donnée \"" +xmlTag + "\" non retrouvée.");
    }
    //................... data ......................................................
public:
    QString       m_PrimKey;		/*!< clé primaire du document ? */
    QString       m_Ref_Blobs_PK;	/*!< clé primaire du blob */
    QString       m_Date;		/*!< date du document */
    QString       m_Libelle;		/*!< libelle du document */
    QString       m_Type;		/*!< Type du document */
    QString       m_SubType;		/*!< Sous-type du document */
    QString       m_User;		/*!< Utilisateur */
    QString       m_SignUser;		/*!< Signataire */
    int           m_State;		/*!< Etat du document */
    QString       m_Fin;                /*!< date time de fin */
    int           m_DureeMod;           /*!< duree d'acces en modification */
    QString       m_Prop_0;             /*!< Propriete N°1 */
    QString       m_Prop_1;             /*!< Propriete N°2 */
    QString       m_Prop_2;             /*!< Propriete N°3 */
    QString       m_Prop_3;             /*!< Propriete N°4 */
    QString       m_Prop_4;             /*!< Propriete N°5 */
    QString       m_LastErr;            /*!< derniere erreur rencontree  */
};

//..................... Liste de la classe definie au dessus .........................................................
//typedef QValueList<CRubRecord> RUBREC_LIST; /*!< Liste des documents constituant le dossier médical */
//====================================== CRubEvntList =========================================================================
/*! \class CRubEvntList
 *  \brief Classe de liste de données pour stocker les relations hierarchiques entre les documents du dossier médical.
 * la structure est la representation de la table : Evnmt_Liens avec le pk de l'enregistrement
 * si le m_Pk commence par # alors les caracteres qui suivent sont celui  d'un pk provisoire unique qui sert a
 * reperer un enregistrement nouveau lors la mise à jour definitive du pk lors d'un enregistrement des donnees
*/
class RUBREC_LIST : public QValueList <CRubRecord>
{

 public:
    RUBREC_LIST(){ }
    RUBREC_LIST( const RUBREC_LIST& l )            : QValueList<CRubRecord>(l) { }
    RUBREC_LIST( const QValueList<CRubRecord>& l ) : QValueList<CRubRecord>(l){ }

//-------------------------------------------- serialize ---------------------------------------------------------
/*! \brief Fonction qui retourne une chaine contenant la serialisation de la liste
 *  \param withXmlHead    int : si zero le header XML ne sera pa ajoute en tete
 *  \return QString :     qui est la serialisation de la liste
*/
QCString serialize() {return serialize(1);}
QCString serialize(int withXmlHead)
{QCString          xml = "<?xml version=\"1.0\"  encoding=\"ISO-8859-1\" standalone=\"yes\" ?>";
 if (!withXmlHead) xml = "";
 RUBREC_LIST::iterator it;
 int i=0;
 for (it = begin(); it != end(); ++it )
     {xml += QCString("\n<CRubRecordNumber>")+QCString(QString::number(i))+"</CRubRecordNumber>" + (*it).serialize(0);
      ++i;
     } //end while (pSqlQuery->next())
 return xml;
}

};

//..................... Liste de paires type de document-Index dans RUBREC_LIST.........................................................
//                      sert par exemple à établir un liste de documents en cours
//                      d'affichage avec leur index dans liste des rubrique
//====================================== DOCUMENT_DISPLAY_MAP =======================================================
// cette QMap permet d'associer un pointeur sur un contenu de rubrique : *CRubRecord   �
// une clef : QString qui contient le nom : 'Observation' , 'Prescription',  'Terrain'  etc ...
// de la rubrique en cours dans laquelle est affich� : CRubRecord
//====================================== CRubEvntList =========================================================================
/*! \class CRubEvntList
 *  \brief Classe de liste de données pour stocker les relations hierarchiques entre les documents du dossier médical.
 * la structure est la representation de la table : Evnmt_Liens avec le pk de l'enregistrement
 * si le m_Pk commence par # alors les caracteres qui suivent sont celui  d'un pk provisoire unique qui sert a
 * reperer un enregistrement nouveau lors la mise à jour definitive du pk lors d'un enregistrement des donnees
*/
class DOCUMENT_DISPLAY_MAP : public QMap <QString, CRubRecord*>
{

 public:
    DOCUMENT_DISPLAY_MAP()
           : QMap <QString, CRubRecord*>()
    {
    }

//-------------------------------------------- serialize ---------------------------------------------------------
/*! \brief Fonction qui retourne une chaine contenant la serialisation de la liste
 *  \param withXmlHead    int : si zero le header XML ne sera pa ajoute en tete
 *  \return QString :     qui est la serialisation de la liste
*/
void copy( const QMap  <QString, CRubRecord*> &map)
{ clear();
  QMapConstIterator <QString, CRubRecord*> it; 
  for  ( it = map.begin(); it != map.end(); ++it ) 
       {  insert ( it.key(), it.data() );
       }
}
//-------------------------------------------- serialize ---------------------------------------------------------
/*! \brief Fonction qui retourne une chaine contenant la serialisation de la liste
 *  \param withXmlHead    int : si zero le header XML ne sera pa ajoute en tete
 *  \return QString :     qui est la serialisation de la liste
*/
void copy( const QMap  <QString, CRubRecord*> *map)
{ clear();
  QMapConstIterator <QString, CRubRecord*> it;
  for  ( it = map->begin(); it != map->end(); ++it ) 
       {  insert ( it.key(), it.data() );
       }
}
};
// typedef QMap<QString, CRubRecord*> DOCUMENT_DISPLAY_MAP;
#endif

