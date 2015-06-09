#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QDomElement>
#include <QDomNode>
#include <QSqlQuery>
#include <QVariant>
#include <QMultiMap>
#include <float.h>
#include <QApplication>
#include "C_BDM_DatasempPlugin.h"

#include "C_PosologieGrammar.h"
#include "C_PatientCtx.h"
#include "../../MedinTuxTools-QT4/CGestIni.h"
#include "../../MedinTuxTools-QT4/C_Utils_Html.h"

typedef struct  { int       m_code;
                  QString   m_codeType;
                  QString   m_drug_name;
                  QString   m_libelle;
                  QString   m_code_produit;
                  QString   m_code_classe;
                  QString   m_code_1;
                  QString   m_code_2;
                } ALLERG_STRUCT;


//-------------------------------- C_BDM_DatasempPlugin ------------------------------------------------------
C_BDM_DatasempPlugin::C_BDM_DatasempPlugin(const QString &confDatas, const QString &prefixLabel, QObject *parent /* =0 */, QTextEdit *logWidget /* =0 */, QProgressBar *pQProgressBar /*=0*/ )
    : C_BaseCommon(parent) , C_BDM_PluginI()
{ m_dataSourceVersion             = "09052012";  // TODO  en attendant le numero provenant de la BD datasemp
  m_firstTimeGetNextSmrInfoForCIP = true;
  m_pQSqlQuery                    = 0;
  if (logWidget)        { C_Log::setLogWidget(logWidget);
                        }
  if (pQProgressBar)    { C_BaseCommon::setProgressWidget(pQProgressBar);
                        }
  if (init(confDatas, prefixLabel)==0)
     {outMessage(  C_BaseCommon::getLastError());
      return;
     }
  this->setObjectName(prefixLabel + "C_BDM_DatasempPlugin");
}

//-------------------------------- ~C_BDM_DatasempPlugin ---------------------------------------
C_BDM_DatasempPlugin::~C_BDM_DatasempPlugin()
{ if (m_pQSqlQuery) delete m_pQSqlQuery;
  qDebug() << this->objectName() + tr(" is deleted");
}

//-------------------------------- init ------------------------------------------------------
int C_BDM_DatasempPlugin::init              ( const QString &confData   , const QString &prefixLabel  )
{   QString errMess           =   "";
    m_pathAppli               =   CGestIni::Param_ReadUniqueParam(confData.toLatin1(),"BDM_Configuration", "PathAppli");
    m_pathIni                 =   CGestIni::Param_ReadUniqueParam(confData.toLatin1(),"BDM_Configuration", "PathIni");
    setInitialisedSate(0);
    if (m_pathAppli.length()==0)
       {setLastError (tr("C_BDM_DatasempPlugin::init() BDM_Configuration/PathAppli is empty"));
        return 0;
       }
    if (!QFile::exists ( m_pathAppli ))
       {setLastError (tr("C_BDM_DatasempPlugin::init() BDM_Configuration/PathAppli don't exists"));
        return 0;
       }
    if (m_pathIni.length()==0)
       {setLastError (tr("C_BDM_DatasempPlugin::init() BDM_Configuration/PathIni is empty"));
        return 0;
       }
    if (QDir::isRelativePath ( m_pathIni )) m_pathIni = m_pathIni.prepend(m_pathAppli);
    m_pathIni = QDir::cleanPath ( m_pathIni );
    if (!QFile::exists ( m_pathIni ))
       {setLastError (tr("C_BDM_DatasempPlugin::init() BDM_Configuration/PathIni don't exists"));
        return 0;
       }
    m_datasIni       =  CGestIni::Param_UpdateFromDisk(m_pathIni);
    if (m_datasIni.length()==0)
       {setLastError (tr("C_BDM_DatasempPlugin::init() m_datasIni is empty"));
        return 0;
       }
    if (BaseConnect(m_datasIni, prefixLabel+"BDM_DATASEMP", "DatasempTest", &errMess, "Connexion_Datasemp" )==0)
       {setLastError (tr("C_BDM_DatasempPlugin::init() error in BDM connexion BaseConnect()\n%1").arg(errMess));
        return 0;
       }
    m_pQSqlQuery  = new QSqlQuery (QString::null , database() );

    if (dbi_init_database("dbi_datasemp",prefixLabel, "DS", this)==0)
       {setLastError (tr("C_BDM_DatasempPlugin::init() error in dbi_init_database()\n"));
        return 0;
       }
    setInitialisedSate(1);
    return 1;

}
//-------------------------------- setLogWidget ------------------------------------------------------
void  C_BDM_DatasempPlugin::setLogWidget      ( QTextEdit *logWidget        )
{ C_Log::setLogWidget(logWidget);
}
//-------------------------------- setProgressWidget ------------------------------------------------------
void  C_BDM_DatasempPlugin::setProgressWidget ( QProgressBar *pQProgressBar )
{ //setProgressWidget(pQProgressBar);
  C_BaseCommon::setProgressWidget(pQProgressBar);
}

//-------------------------------- dataSourceVersion ------------------------------------------------------
QString C_BDM_DatasempPlugin::dataSourceVersion()
{   QString numEdition   = "";
    QString packBCArrete = "";
    /*QString ods          =  */ Datasemp_GetBaseVersion(&numEdition,&packBCArrete);
    m_dataSourceVersion  =  tr("Datasemp Version : %1  date d'extraction : %2").arg( numEdition, packBCArrete);
    return  m_dataSourceVersion;
}
//-------------------------------- is_plugin_HAS_certified ------------------------------------------------------
/*! \brief return if  plugin is HAS certified
 *  \return return HAS certified status
 */
bool C_BDM_DatasempPlugin::is_plugin_HAS_certified()
{return false;
}
//-------------------------------- plugin_name ------------------------------------------------------
/*! \brief return active plugin name
 *  \return return active plugin name
 */
QString C_BDM_DatasempPlugin::plugin_name()
{return "Datasemp";
}
//-------------------------------- plugin_icon ------------------------------------------------------
/*! \brief return active plugin_icon
 *  \return return active plugin_icon
 */
QPixmap C_BDM_DatasempPlugin::plugin_icon()
{// QPixmap icon(vidal_22x20_xpm);
 // return icon;
 // QString path = m_pathAppli+"Ressources/datasemp.png";
 // return QPixmap(path);
 static char const * dossier_rouge_xpm[] =  {
 "22 20 202 2",
 "  	c None",
 ". 	c #9C0000",
 "+ 	c #960707",
 "@ 	c #970C0C",
 "# 	c #970B0B",
 "$ 	c #980B0B",
 "% 	c #990B0B",
 "& 	c #990C0C",
 "* 	c #9B0C0C",
 "= 	c #9C0C0C",
 "- 	c #9D0C0C",
 "; 	c #9D0D0D",
 "> 	c #9E0D0D",
 ", 	c #9F0D0D",
 "' 	c #A00D0D",
 ") 	c #A10E0E",
 "! 	c #A20D0D",
 "~ 	c #A10808",
 "{ 	c #A50000",
 "] 	c #970303",
 "^ 	c #ECC8C8",
 "/ 	c #EBBBBB",
 "( 	c #ECBCBC",
 "_ 	c #ECBDBD",
 ": 	c #EDBDBD",
 "< 	c #EDBEBE",
 "[ 	c #EEBFBF",
 "} 	c #EFC0C0",
 "| 	c #F0C1C1",
 "1 	c #F0C2C2",
 "2 	c #F1C3C3",
 "3 	c #F2C4C4",
 "4 	c #F3C5C5",
 "5 	c #F3C6C6",
 "6 	c #F6D3D3",
 "7 	c #A00303",
 "8 	c #980606",
 "9 	c #EAC0C0",
 "0 	c #B40000",
 "a 	c #B50000",
 "b 	c #B70000",
 "c 	c #B80000",
 "d 	c #B90000",
 "e 	c #BC0000",
 "f 	c #BD0000",
 "g 	c #C00000",
 "h 	c #C10000",
 "i 	c #C20000",
 "j 	c #C40000",
 "k 	c #C60000",
 "l 	c #C70000",
 "m 	c #C80000",
 "n 	c #C90000",
 "o 	c #CB0000",
 "p 	c #F5CDCD",
 "q 	c #A00707",
 "r 	c #B60000",
 "s 	c #BA0000",
 "t 	c #BE0000",
 "u 	c #C50000",
 "v 	c #DC6464",
 "w 	c #C90202",
 "x 	c #CA0000",
 "y 	c #F5CECE",
 "z 	c #A10707",
 "A 	c #EAC1C1",
 "B 	c #BB0000",
 "C 	c #BF0000",
 "D 	c #C30000",
 "E 	c #D02B2B",
 "F 	c #FDF7F7",
 "G 	c #EB9D9D",
 "H 	c #CC0B0B",
 "I 	c #DB4444",
 "J 	c #F6CECE",
 "K 	c #EBC1C1",
 "L 	c #CA1A1A",
 "M 	c #D33A3A",
 "N 	c #DD5C5C",
 "O 	c #FFFDFD",
 "P 	c #E89292",
 "Q 	c #F7D7D7",
 "R 	c #F1B8B8",
 "S 	c #F6CFCF",
 "T 	c #A20707",
 "U 	c #F7DDDD",
 "V 	c #FEFDFD",
 "W 	c #EFB2B2",
 "X 	c #D22727",
 "Y 	c #E58080",
 "Z 	c #FFFFFF",
 "` 	c #EC9D9D",
 " .	c #D00303",
 "..	c #A20808",
 "+.	c #9A0606",
 "@.	c #ECC2C2",
 "#.	c #EFB7B7",
 "$.	c #EBA3A3",
 "%.	c #FEFAFA",
 "&.	c #EB9797",
 "*.	c #DB4747",
 "=.	c #CF0000",
 "-.	c #D00000",
 ";.	c #F7D0D0",
 ">.	c #C30202",
 ",.	c #E17B7B",
 "'.	c #F4CDCD",
 ").	c #E17979",
 "!.	c #DC5C5C",
 "~.	c #FCF1F1",
 "{.	c #F9E4E4",
 "].	c #DC4D4D",
 "^.	c #CE0000",
 "/.	c #9B0606",
 "(.	c #ECC3C3",
 "_.	c #C50D0D",
 ":.	c #EEB9B9",
 "<.	c #F4CFCF",
 "[.	c #E99B9B",
 "}.	c #E27272",
 "|.	c #E27373",
 "1.	c #D20000",
 "2.	c #EDC3C3",
 "3.	c #CA2020",
 "4.	c #C91A1A",
 "5.	c #F5D4D4",
 "6.	c #F7DADA",
 "7.	c #CB1212",
 "8.	c #ECA4A4",
 "9.	c #F2C1C1",
 "0.	c #E16E6E",
 "a.	c #D01313",
 "b.	c #D10000",
 "c.	c #9C0707",
 "d.	c #EDC4C4",
 "e.	c #C40A0A",
 "f.	c #F4D2D2",
 "g.	c #F0BEBE",
 "h.	c #CD2222",
 "i.	c #FAE8E8",
 "j.	c #F4CACA",
 "k.	c #FAE5E5",
 "l.	c #E06666",
 "m.	c #CD0000",
 "n.	c #D03C3C",
 "o.	c #F4D3D3",
 "p.	c #D13A3A",
 "q.	c #FDF6F6",
 "r.	c #EAA1A1",
 "s.	c #D43838",
 "t.	c #FCEFEF",
 "u.	c #E06767",
 "v.	c #9D0707",
 "w.	c #EEC5C5",
 "x.	c #C20606",
 "y.	c #C91D1D",
 "z.	c #FEFBFB",
 "A.	c #D33F3F",
 "B.	c #DA5858",
 "C.	c #E27878",
 "D.	c #CD0F0F",
 "E.	c #9E0707",
 "F.	c #E79C9C",
 "G.	c #F7DCDC",
 "H.	c #D34646",
 "I.	c #EDAEAE",
 "J.	c #E06868",
 "K.	c #EEC6C6",
 "L.	c #E79A9A",
 "M.	c #FEF9F9",
 "N.	c #F5D3D3",
 "O.	c #FCF0F0",
 "P.	c #CB0707",
 "Q.	c #CC0000",
 "R.	c #9F0707",
 "S.	c #EFC6C6",
 "T.	c #CE2929",
 "U.	c #EFB5B5",
 "V.	c #EBA2A2",
 "W.	c #CB0404",
 "X.	c #EFC7C7",
 "Y.	c #9F0202",
 "Z.	c #EECACA",
 "`.	c #EEC1C1",
 " +	c #EFC2C2",
 ".+	c #EFC3C3",
 "++	c #F0C4C4",
 "@+	c #F1C5C5",
 "#+	c #F2C6C6",
 "$+	c #F2C7C7",
 "%+	c #F3C7C7",
 "&+	c #F5D2D2",
 "*+	c #A10303",
 "=+	c #A60000",
 "-+	c #9F0303",
 ";+	c #A00606",
 ">+	c #A40707",
 ",+	c #A30707",
 "'+	c #A40808",
 ")+	c #A50808",
 "!+	c #A40303",
 "~+	c #AB0000",
 ". + @ @ # $ % & * * = - ; > , , ' ) ) ! ~ { ",
 "] ^ / / ( _ : < [ [ } } | 1 1 2 3 3 4 5 6 7 ",
 "8 9 0 a b c d e e f g g h i j k l m n o p q ",
 "8 9 a r c d s e f t g h i j u v w n x o y z ",
 "8 A r b d s B f t C i i D u E F G x H I J q ",
 "8 K b c s B f t C g i D L M l N O P Q R S T ",
 "8 K c d B f f C g i i j U V W X Y Z `  .S ..",
 "+.@.d e e f g g h i j k #.$.6 %.&.*.=.-.;...",
 "+.@.s e f t g h >.,.'.).!.~.{.Y ].^.-.-.;...",
 "/.(.B f t C i _.:.<.[.V }.~.|.^.^.^.-.1.;...",
 "/.2.f t C g 3.4.5.6.7.8.9.0.a.^.=.b.1.1.;...",
 "c.d.f C g e.f.g.h.i.j.k.l.m.^.=.b.1.1.1.;...",
 "c.d.g g n.o.p.q.r.s.t.u.o m.=.-.1.1.1.1.;...",
 "v.w.g x.y.z.A.B.O C.D.o m.^.-.-.1.1.1.1.;...",
 "E.w.F.G.H.I.r.l J.X x ^.^.^.-.1.1.1.1.1.;...",
 "E.K.i L.M.N.O.P.x x Q.^.=.b.1.1.1.1.1.1.;...",
 "R.S.i j T.U.V.W.o m.^.=.-.1.1.1.1.1.1.1.;...",
 "R.X.j k l m n o o m.=.-.1.1.1.1.1.1.1.1.;...",
 "Y.Z.`. +.+++++@+@+@+#+$+%+%+%+%+%+%+%+%+&+*+",
 "=+-+;+z T T T >+>+,+'+)+)+)+)+)+)+)+)+)+!+~+"};
 return QPixmap(dossier_rouge_xpm);
}
//---------------------------------------- get_database -------------------------------------------------------
/*! \brief  return active database. This facility is to do extended SQL querys which are not in API
 *  \return  database
 */
QSqlDatabase C_BDM_DatasempPlugin::get_database()
{ return database();
}
//-------------------------------- plugin_image_source ------------------------------------------------------
/*! \brief  return image source url for monographies
 *  \return return image source url for monographies
 */
QString C_BDM_DatasempPlugin::plugin_image_source()
{   QString pathFile = CGestIni::Param_ReadUniqueParam(m_datasIni.toLatin1(),QString("Connexion_"+plugin_name()).toLatin1(), "PathImageSource");
    if ( QDir::isRelativePath ( pathFile ) ) pathFile  = QDir::cleanPath ( pathFile.prepend(m_pathAppli) );
    if (!pathFile.endsWith("/")) pathFile += "/";
    return pathFile;
}
//-------------------------------- lang ------------------------------------------------------
QString  C_BDM_DatasempPlugin::lang()
{return "fr";
}
//-------------------------------- owner ------------------------------------------------------
QString  C_BDM_DatasempPlugin::owner()
{return "DS";
}
//-------------------------------- getLastError ------------------------------------------------------
QString  C_BDM_DatasempPlugin::getLastError()
{return C_BaseCommon::getLastError();
}

//-------------------------------- getCompositionFromCIP ------------------------------------------------------
/*! \brief return drug composition.
 *  \param const QString& cip drug cip identification .
 *  \return QList<C_InterResultPair> result list where
 *              C_CompositionItem.name()              is the composant name (DCI)
 *              C_CompositionItem.amount()            is the quantity
 *              C_CompositionItem.unit()              is the quantity unit
 *              C_CompositionItem.unit_plural()       is the accorded quantity unit if more than one
 */
/*
SELECT t4B.f8 as drugName,
       t08.f2 as codeSubType,
       t1C.f1 as subType

FROM  t08,
      t1C,
      t4B
WHERE t08.f0 = t4B.f0                     -- t08.f0 = '053133'
AND   t08.f2 = t1C.f0
and   t4B.f8 like "%LAM%"
order by t4B.f8
-- AND   t07.f1 = '0'
LIMIT 0,30000
 */
/*
SELECT t07.f4 as Quantite,
       t24.f1 as QuUnitName,
       t27.f2 as SubName,
       t00.f3 as DrugName,
       t07.f2 as CodeSbc,
       t07.f1 as CodeSection,
       t07.f3 as NumOrdre,
       t07.f5 as QuUnitCode
FROM  t07,
      t00,
      t27,
      t24
WHERE t07.f0 = t00.f0
AND   t00.f2 = '3517301'
AND   t27.f0 = t07.f2
AND   t07.f1 = '0'
AND   t24.f0 = t07.f5

SELECT t00.f3        as drugName,
       t07.f4        as Qu,
       t24.f1        as QuUnitName,
       UPPER(t27.f2) as SubName

FROM  t07,
      t27,
      t08,
      t24,
      t00
WHERE t00.f3 like '%LAMALINE%' -- t00.f2 = '3024490'
AND   t07.f0 = t00.f0
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
and   t08.f1 = '0'
and   t08.f0 = t00.f0
AND   t24.f0 = t07.f5
ORDER BY t00.f2

SELECT UPPER(t27.f2) as SubName,
       t24.f1        as QuUnitName,
       t07.f4        as Qu
FROM  t07,
      t27,
      t08,
      t24,
      t00
WHERE t00.f2 = '3024490'
AND   t07.f0 = t00.f0
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
and   t08.f1 = '0'
and   t08.f0 = t00.f0
AND   t24.f0 = t07.f5

SELECT UPPER(t27.f2) as SubName,
       t00.f3        as drugName,
       t07.f4        as qu,
       t24.f5        as unit
FROM  t07,
      t27,
      t08,
      t00,
      t24
WHERE t07.f0 = t00.f0 -- t07.f0 = '202489'
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t08.f0 = t00.f0 -- '202489'       -- COLCHIMAX
AND   t24.f0 = t07.f5
AND   t00.f3 like '%COLCHIMAX%'
ORDER BY t00.f3

 SELECT t54.f0 AS num_subst,
        t54.f2 AS nom_subst,
        t00.f3 AS nom_medoc,
        t00.f0 AS num_medoc ,
        t00.f1 AS cuv_medoc
 FROM  t54
       INNER JOIN t07 ON t54.f0=t07.f2
       INNER JOIN t00 ON t07.f0=t00.f0

 WHERE t54.f1='FRA'
 -- AND CAST(t54.f2 AS CHAR) LIKE '%AMOXI%'
  AND  t54.f3 = TRUE
 -- AND  t00.f3 like '%AUGMEN%'
 ORDER BY t00.f3
 limit 0,10000
*/
 QList<C_CompositionItem> C_BDM_DatasempPlugin::getCompositionFromCIP(const QString &cip)

 { QList<C_CompositionItem> retList;
   QString requete = QString (
                               " SELECT   t07.f4,          \n"         // 0   quantite
                               "          t24.f1,          \n"         // 1   unite quantite au singulier
                               "          UPPER(t27.f2),   \n"         // 2   libelle de la substance
                               "          t24.f5,          \n"         // 3   unite quantite au pluriel
                               "          t27.f0           \n"         // 4   code de la substance
                               " FROM  t07,                \n"
                               "       t27,                \n"
                               "       t08,                \n"
                               "       t24,                \n"
                               "       t00                 \n"
                               " WHERE t00.f2 = '%1'       \n"
                               " AND   t07.f0 = t00.f0     \n"
                               " AND   t27.f0 = t07.f2     \n"
                               " AND   t07.f1 = t08.f1     \n"
                               " AND   t08.f1 = '0'        \n"
                               " AND   t08.f0 = t00.f0     \n"
                               " AND   t24.f0 = t07.f5     \n"
                               " ORDER BY t07.f3           \n"
                             ).arg(cip);
   QSqlQuery query(requete , database() );
   if (! query.isActive())                             return retList;
   while (query.next())
      { QString          dose = query.value(0).toString();
        QString unitSingulier = Utf8_Query(query, 1);
        QString   unitPluriel = Utf8_Query(query, 3);
        QString   substance   = get_SubstancePere( query.value(4).toString() );  // on tente la substance pere (vaut mieux exploiter le nommage le plus simple du pere sans les acides chlorydrates ..)
        if (substance.length()==0) substance = CGestIni::Utf8_Query(query, 2);
        if (unitPluriel.length()==0) unitPluriel = unitSingulier;  // si pas de pluiel c'est que c'est le meme que celui singulier
        retList.append ( C_CompositionItem (substance ,
                                            dose                           ,
                                            unitSingulier                  ,
                                            unitPluriel
                                           )
                       );
      }
   return  retList;
 }
 //-------------------------------- getExcipientsFromCIP ------------------------------------------------------
 /*! \brief return drug excipients list .
  *  \param const QString& cip drug cip identification .
  *  \return QList<C_ExcipientItem> result list where
  *              C_ExcipientItem.name()              is the composant name (excipient)
  *              C_ExcipientItem.amount()            is the quantity if present (not regular)
  *              C_ExcipientItem.unit()              is the quantity unit if present (not regular)
  *              C_ExcipientItem.code()              is the substance code  (specific at BDM implementation)
  *              C_ExcipientItem.code_typ            is the code type (specific at BDM implementation)
  */
 QList<C_ExcipientItem> C_BDM_DatasempPlugin::getExcipientsFromCIP(const QString &cip)
 {   QList<C_ExcipientItem> retList;
        QString requete = QString ( " SELECT   t07.f4,          \n"         // 0   quantite
                                    "          t27.f2,          \n"         // 1   libelle de la substance
                                    "          t07.f2           \n"         // 2   code de la substance
                                    " FROM  t07,                \n"
                                    "       t27,                \n"
                                    "       t00                 \n"
                                    " WHERE t07.f2=t27.f0       \n"
                                    " AND   t07.f0=t00.f0       \n"
                                    " AND   t00.f2 = '%1'       \n"
                                    " AND   t07.f1 > 0          \n"
                                    " ORDER BY t07.f3           \n"
                                  ).arg(cip);
        QSqlQuery query(requete , database() );
        if (! query.isActive())
           {  outSQL_error( query, tr("ERROR in get T10 getExcipientsFromCIP(%1)").arg(cip) , requete , __FILE__, __LINE__ );
              return retList;
           }
        while (query.next())
           { retList.append ( C_ExcipientItem (  Utf8_Query(query, 1)           ,    // libelle substance
                                                 query.value(0).toString()      ,    // quantite le plus souvent NULL pour les excipients
                                                 ""                             ,    // unite non evaluee car jamais renseignee pour les excipients
                                                 query.value(2).toString()      ,    // code de la substance
                                                 "DS_E"
                                              )
                            );
           }
     return retList;
 }
//--------------------------- get_CIS_and_RCP_From_CIP -----------------------------------------------------------
/*! \brief return ANSM (ex afssaps) CIS ans RCP code from CIP code, to use in html request with ANSM (ex afssaps) web site.
 *  \param const QString cip is drug cip code.
 *  \param QString *rcp is pointer on QString to return RCP result if not zero .
 *  \return CIS code string
*/
QString C_BDM_DatasempPlugin::get_CIS_and_RCP_From_CIP(const QString &cip, QString *rcp  /* =0 */ )
{
    Q_UNUSED(rcp);
    QString ret = "";
    QString requete = QString (
                                " SELECT t4B.f3          \n"
                                " FROM   t4B,            \n"
                                "        t00             \n"
                                " WHERE  t00.f2 = '%1'   \n"  // -- 3690548 morphine   -- 3575327 actifed
                                " AND    t00.f0 = t4B.f0 \n"
                              ).arg(cip);
    QSqlQuery query(requete , database() );
    if (query.isActive() && query.next())
       {ret = query.value(0).toString();
       }
    return ret;
}
//-------------------------------- isStupefiant ------------------------------------------------------
/*! \brief return if the product identified par Id is a stupefiant
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return 1 is stupefiant
 */
/*
SELECT t60.f1 as libelle,
       t63.f1 as code,
       t00.f3 as drug_name
FROM   t63,
       t60,
       t00
-- where  t00.f2='3690548'   -- morphine
-- where  t00.f2='3575327'   -- actifed
where  t63.f0=t00.f1
-- AND    t63.f0=t00.f1
AND    t60.f0=t63.f1
AND    t63.f1>1
AND    t63.f1<4
-- AND    t63.f1='2'
ORDER by t63.f1
LIMIT 0,30000
 */
int C_BDM_DatasempPlugin::isStupefiant ( const QString &cip )
{
    QString requete = QString (
                                " SELECT t63.f1          \n"
                                " FROM   t63,            \n"
                                "        t00             \n"
                                " WHERE  t00.f2 = '%1'   \n"  // -- 3690548 morphine   -- 3575327 actifed
                                " AND    t63.f0 = t00.f1 \n"
                                " AND    t63.f1>1        \n"
                                " AND    t63.f1<4        \n"
                              ).arg(cip);
    QSqlQuery query(requete , database() );
    return (query.isActive() && query.next());
}
//-------------------------------- code_ATC_To_Name ------------------------------------------------------
/*! \brief return if ATC libelle from code ATC
 *  \param const QString &code      code ATC .
 *  \return QString return libelle
 */
QString C_BDM_DatasempPlugin::code_ATC_To_Name(const QString &code)
{   QSqlQuery query (QString("SELECT f2 FROM t42 WHERE f0 ='%1'").arg(code) , database() );
    if (query.isActive() && query.next())
       {return Utf8_Query(query, 0);
       }
    return "";
}
//-------------------------------- getDrugRoutePosoOnT2M_T15 ------------------------------------------------------
/*! \brief return drug route
 *  \param const QString &t2Pf0 key from T2P.f0 (posologie table)
 *  \return  drug route
 */
QString  C_BDM_DatasempPlugin::getDrugRoutePosoOnT2M_T15 (const QString &t2Pf0)
{ QString requete = QString ( " SELECT                   \n"
                              "  t15.f1                  \n"   // 0  voie
                              " FROM                     \n"
                              "  t2M,                    \n"
                              "  t15                     \n"
                              " WHERE    t2M.f1= t15.f0  \n"
                              " AND      t2M.f0='%1'     ").arg(t2Pf0);
    QSqlQuery query(requete , database() );
    if (! query.isActive())  return "";
    while (query.next())
       {return CGestIni::Utf8_Query(query, 0);
       }
    return "";
}
//-------------------------------- get_LibelleT24 ------------------------------------------------------
/*! \brief   return libelle from table t24
 *  \param   const QString &t24f0 key from t24.f0
 *  \param   int quForPlurial if > 1 and plurial form exists, it's will be returned
 *  \return  libelle
 */
QString  C_BDM_DatasempPlugin::get_LibelleT24 (const QString &t24f0, int quForPlurial /* = 0 */)
{ QString requete = QString ( " SELECT"
                              " t24.f1,"   // 0  libelle
                              " t24.f5"    // 1  plurial
                              " FROM"
                              " t24"
                              " WHERE t24.f0='%1'").arg(t24f0);
    QSqlQuery query(requete , database() );
    if (! query.isActive())  return "";
    while (query.next())
       { if (quForPlurial>1)
            { QString value = CGestIni::Utf8_Query(query, 1);
              if (value.length()) return value;
            }
         return CGestIni::Utf8_Query(query, 0);
       }
    return "";
}
//-------------------------------- getPhysioStateForThisPosoOnT2L_T2A ------------------------------------------------------
/*! \brief return posology's sex
 *  \param const QString &t2Pf0 key from T2P.f0 (posologie table)
 *  \param QString &infValue  inferior value will be returned in this
 *  \param QString &infValue  superior value will be returned in this
 *  \param QString &codeUnit  unit code  will be returned in this
 *  \param QString &unit      unit will be returned in this
 *  \return  property type (age, poids,sexe ...)
 */
QString  C_BDM_DatasempPlugin::getPhysioStateForThisPosoOnT2L_T2A (const QString &t2Pf0, QString &infValue, QString &supValue, QString &codeUnit, QString &unitLibelle)
{ QString requete = QString ( " SELECT   \n"
                              "  t2L.f0  \n"   // 0  property (age, poids taille surface etc...)
                              " ,t2L.f1  \n"   // 1  inferior value
                              " ,t2L.f2  \n"   // 2  superior value
                              " ,t2L.f3  \n"   // 3  unit code
                              " ,t2A.f1  \n"   // 4  unit libelle
                              " FROM     \n"
                              "  t2L,    \n"
                              "  t2A     \n"
                              " WHERE    t2L.f4='%1'   "
                              " AND      t2L.f0=t2A.f0 ").arg(t2Pf0);
    QSqlQuery query(requete , database() );
    if (! query.isActive() )  return "";
    while (query.next())
       { infValue    = query.value(1).toString();
         supValue    = query.value(2).toString();
         codeUnit    = query.value(3).toString();
         unitLibelle = CGestIni::Utf8_Query(query, 4);
         return query.value(0).toString();
       }
    return "";
}
//-------------------------------- Slot_On_CursorError ------------------------------------------------------
void C_BDM_DatasempPlugin::Slot_On_CursorError (const QSqlQuery &qsqlquery, const QString &from, const QString &sql_query, const QString &file, int  line)
{ outSQL_error( qsqlquery, from , sql_query , file, line );
}

//-------------------------------- getPosologiesFromCIP ------------------------------------------------------
/*! \brief return posologies list as posologie grammar QString
 *   NOTE : status is posionned on P (as Posologie) as thereafter <status>P</status>
 *  \param C_PatientCtx *pC_PatientCtx physiopathological patient state for best selection
 *  \param const QString &cip is cip code or if len > 20 it can be a string with grammar allready fill with name, dci, cip basics informations from product etc...
 *  \return QStringList with grammar QString list
 */
QStringList   C_BDM_DatasempPlugin::getPosologiesFromCIP (C_PatientCtx *pC_PatientCtx, const QString &cipOrGrammar)
{   Q_UNUSED(pC_PatientCtx);
    static const char toCodeDureeDs[8]   = "JJJJSMA";
    QString grammar                      = "";
    QStringList grammarList;
    //............. determiner si le parametre d'entree _cip ............................
    //              est le CIP ou une structure  grammar deja preremplie
    //              seul cas traite ici
    int nextPos       = 0;
    QString cip       = CGestIni::getXmlData("gph_id", cipOrGrammar, &nextPos);
    if (cip.length()==0) return grammarList;

    QString requete = QString ( " SELECT                   \n"
                                "  t2P.f0                  \n"   // 0  lien vers t2L.f4 (etat physio path) et t2M.f0->f1->t15.f0->f1 voie
                                " ,t2P.f2                  \n"   // 1  Qu  min
                                " ,t2P.f3                  \n"   // 2  Qu  min UnitCode ->t24.f0->f1->unite
                                " ,t2P.f4                  \n"   // 3  Qu  min unite de temps (mode textuel type 'par 24 heures')
                                " ,t2P.f5                  \n"   // 4  Fce min
                                " ,t2P.f6                  \n"   // 5  Fce min UnitCode ->t24.f0->f1->unite

                                " ,t2P.f7                  \n"   // 6  Qu  max
                                " ,t2P.f8                  \n"   // 7  Qu  max UnitCode ->t24.f0->f1->unite
                                " ,t2P.f9                  \n"   // 8  Qu  max unite de temps (mode textuel type 'par 24 heures')
                                " ,t2P.fA                  \n"   // 9  Fce max
                                " ,t2P.fB                  \n"   // 10 Fce max UnitCode ->t24.f0->f1->unite

                                " ,t2P.fC                  \n"   // 11 Dose max absolue
                                " ,t2P.fD                  \n"   // 12 Dose max absolue UnitCode ->t24.f0->f1->unite
                                " ,t2P.fE                  \n"   // 13 Duree de la dose max absolue (texte a traiter)

                                " ,t2P.fL                  \n"   // 14 duree minimum de la prescription
                                " ,t2P.fM                  \n"   // 15 duree minimum unite de temps ->t24.f0->f1->unite
                                " ,t2P.fN                  \n"   // 16 duree maximum de la prescription
                                " ,t2P.fO                  \n"   // 17 duree maximum unite de temps ->t24.f0->f1->unite

                                " ,t2P.fP                  \n"   // 18 si -1 pas de donnee exploitable
                                "  FROM                    \n"
                                " t2P,                     \n"
                                " t4B,                     \n"
                                " t00                      \n"
                                " WHERE    t00.f0= t4B.f0  \n"
                                " AND      t2P.f1=t4B.f0   \n"
                                " AND      t00.f2 = '%1'  "  ).arg(cip);

    // CGestIni::Param_UpdateToDisk("/home/ro/requete.sql", requete);
    QSqlQuery query(requete , database() );
    if (! query.isActive())  return grammarList;
    while (query.next() && query.value(18).toInt()!=-1) // t2P.fP 18 si -1 pas de donnee exploitable
       {
         grammar               = cipOrGrammar;

         QString key           = query.value(0).toString();

         QString qu_min        = query.value(1).toString();
         QString qu_minUnit    = get_LibelleT24 (query.value(2).toString());
         QString fc_minTpsTxt  = query.value(3).toString();                       // CGestIni::Utf8_Query(query, 3);
         QString fc_min        = query.value(4).toString();
         QString fc_minUnit    = get_LibelleT24 (query.value(5).toString());

         QString qu_max        = query.value(6).toString();
         QString qu_maxUnit    = get_LibelleT24 (query.value(7).toString());
         QString fc_maxTpsTxt  = query.value(8).toString();                       // CGestIni::Utf8_Query(query, 8);
         QString fc_max        = query.value(9).toString();
         QString fc_maxUnit    = get_LibelleT24 (query.value(10).toString());

         QString qu_maxAbs     = query.value(11).toString();
         QString qu_maxAbsUnit = get_LibelleT24 (query.value(12).toString());
         QString qu_maxAbsTps  = query.value(13).toString();                     // CGestIni::Utf8_Query(query, 13);
         QString valueTps;
         convertDureeQmaxToMinutes(qu_maxAbsTps, &valueTps);
         //............. le statut (vient des poso BDM) .......................
         QString status        = "P";
         CGestIni::setXmlData("status", status,  grammar);

         //............. la voie ..............................................
         QString voie          = getDrugRoutePosoOnT2M_T15 (key);
         CGestIni::setXmlData("gph_voie", voie,  grammar);

         //............. duree de prescription ................................
         QString duree_min     = query.value(14).toString();
         int     duree_minUnit = query.value(15).toInt();

         QString duree_max     = query.value(16).toString();
         int     duree_maxUnit = query.value(17).toInt();
         //........... terrain, indication de la posologie, etat physio pathologique (EPP)..................
         /*     " <gph_tr>\n"                      //        - debut de la liste des terrains por lesquels s'applique cette psologie
                "  <gph_trl>\n"                    //        -- debut du premier element de la liste des terrains
                "    <gph_trn></gph_trn>"          //        --- nom du premier element du teraain (correspond a une variable du contexte : age, poids, taille, valeur biologique ...)
                    "<gph_tri></gph_tri>"          //        --- borne inferieure
                    "<gph_trs></gph_trs>"          //        --- borne superieure
                    "<gph_tru></gph_tru>\n"        //        --- unite des bornes (an, kg, m2 cm2, mg ml UI ...)
                "  </gph_trl>\n"                   //        --fin du premier element de la liste des terrains
                " </gph_tr>\n"                     //        - fin de la liste des terrains por lesquels s'applique cette psologie
         */
         QString terrain       = "";
         C_GetPosologieEPP_CUR  C_EPP ( database(), this );
         while ( C_EPP.next( key ) )
               { switch ( C_EPP.DESCRIPTEUR() )
                        {case 'A':
                              terrain += "  <gph_trl>\n"
                                         "    <gph_trn>"+tr("Allaitement")+"</gph_tr><gph_tri></gph_tri><gph_trs></gph_trs><gph_tru>A</gph_tru>\n"
                                         "  </gph_trl>\n";
                              break;
                         case 'F':
                              terrain += "  <gph_trl>\n"
                                         "    <gph_trn>"+tr("Sexe")+"</gph_trn><gph_tri>F</gph_tri><gph_trs>F</gph_trs><gph_tru>S</gph_tru>\n"
                                         "  </gph_trl>\n";
                              break;
                         case 'M':
                              terrain += "  <gph_trl>\n"
                                         "    <gph_trn>"+tr("Sexe")+"</gph_trn><gph_tri>M</gph_tri><gph_trs>M</gph_trs><gph_tru>S</gph_tru>\n"
                                         "  </gph_trl>\n";
                              break;
                         default:
                              char sexe;
                              terrain +=  "  <gph_trl>\n"
                                          "    <gph_trn>"+ C_EPP.TYPE_LIBELLE() +"</gph_trn>"               // nom de la variable terrain
                                              "<gph_tri>"+ C_EPP.INF_VALUE()    +"</gph_tri>"               // borne inferieure
                                              "<gph_trs>"+ C_EPP.SUP_VALUE()    +"</gph_trs>"               // borne superieure
                                              "<gph_tru>"+ C_PosologieGrammar::get_AccordedWord( C_EPP.SUP_VALUE().toInt(), C_EPP.UNIT_LIBELLE(), sexe )+"</gph_tru>\n"   // unite des bornes
                                          "  </gph_trl>\n";
                       }
               }
         CGestIni::setXmlData("gph_tr", terrain,               grammar,1);

         //............. exploitation des donnees recueillies .................
         QString gph_cy       = "";
         if      (duree_minUnit >= 38 && duree_minUnit <= 44) gph_cy = toCodeDureeDs[duree_minUnit-38];
         else if (duree_maxUnit >= 38 && duree_maxUnit <= 44) gph_cy = toCodeDureeDs[duree_maxUnit-38];
         if (gph_cy.length())
            { CGestIni::setXmlData("gph_dunit", gph_cy,  grammar);
              int duree_minInt = duree_min.toInt();  // toInt() car on peut avoir duree_min "0" et duree_max "10"
              int duree_maxInt = duree_max.toInt();
              if       (duree_minInt && duree_maxInt)
                       { gph_cy += QString::number( (duree_max.toInt()+duree_min.toInt())/2);
                         CGestIni::setXmlData("gph_dmin", duree_min,  grammar);
                         CGestIni::setXmlData("gph_dmax", duree_max,  grammar);
                       }
              else  if (duree_minInt)
                      { gph_cy += duree_min;
                        CGestIni::setXmlData("gph_dmin", duree_min,  grammar);
                        CGestIni::setXmlData("gph_dmax", duree_min,  grammar);
                      }
              else  if (duree_maxInt)
                      { gph_cy += duree_max;
                        CGestIni::setXmlData("gph_dmin", duree_max,  grammar);
                        CGestIni::setXmlData("gph_dmax", duree_max,  grammar);
                      }
              else   gph_cy.clear();
            }
         CGestIni::setXmlData("gph_cy", gph_cy,  grammar,1);
         //............. sequence posologique ....................
         // <gpp>[1;m|2;d|4;s]J8!\303\240 la phase initiale en bolus</gpp>
         // <gpp>[100-200;]U15S:!\303\240 la phase initiale en bolus</gpp>   // 100 a 200 mg a ne faire q'une fois a la phase initiale en bolus
         QString gpp             = "";

         if (qu_minUnit.length() && qu_maxUnit.length()) gpp += qu_maxUnit;
         else if (qu_minUnit.length())                   gpp += qu_minUnit;
         else                                            gpp += qu_maxUnit;

         QString gph_pfcunit     = "";
         int               p     = gpp.lastIndexOf('/');  // comprime/kg      dose-graduation/m2
         if ( p!= -1 )
            { ++p; QChar c1 = gpp[p].toLower();
              ++p; QChar c2 = gpp[p].toLower();
              if (c1 =='k' && c2=='g')
                 { gph_pfcunit = "Kg";           gpp.chop(3); CGestIni::setXmlData("gph_uf", gpp,  grammar);
                 }   // a ce stade gpp contient la forme galenique
              else if (c1=='m' && (c2==QChar(178)||c2=='2'))
                 { gph_pfcunit = "m2";           gpp.chop(3); CGestIni::setXmlData("gph_uf", gpp,  grammar);
                 }
              else if (c1=='c' &&  c2=='m')
                 { gph_pfcunit = "cm2";          gpp.chop(4); CGestIni::setXmlData("gph_uf", gpp,  grammar);
                 }
              /*
              if      (gpp.endsWith("/kg"))  {gph_pfcunit = "Kg";          gpp.chop(3); CGestIni::setXmlData("gph_uf", gpp,  grammar);}   // a ce stade gpp contient la forme galenique
              else if (gpp.endsWith("/m2"))  {gph_pfcunit = "m2";          gpp.chop(2); CGestIni::setXmlData("gph_uf", gpp,  grammar);}   // a ce stade gpp contient la forme galenique
              else if (gpp.endsWith("/m2"))  {gph_pfcunit = "m0302 0262";  gpp.chop(2); CGestIni::setXmlData("gph_uf", gpp,  grammar);}   // a ce stade gpp contient la forme galenique
              else if (gpp.endsWith("/cm2")) {gph_pfcunit = "cm2";         gpp.chop(3); CGestIni::setXmlData("gph_uf", gpp,  grammar);}   // a ce stade gpp contient la forme galenique
              */
           }
         if (gph_pfcunit.length()) // si un facteur corporel est lie a cette posologie
            { CGestIni::setXmlData("gph_pfc",        QString("1"),      grammar, 1);   // 7
              CGestIni::setXmlData("gph_pfcunit",    gph_pfcunit,       grammar, 1);   // kg  ( par 7 kg de poids )
              CGestIni::setXmlData("gph_pqbyuf",     QString("-1"),     grammar, 1);   // 500 ( 500 mg / comprime ) quantite d'unite posologique par unite de forme si -1 c'est forme galenique x facteur poids x poids
              CGestIni::setXmlData("gph_pmin",       qu_min,            grammar, 1);   // n       QPA_min  Quantite de Principe Actif minimum
              CGestIni::setXmlData("gph_pmax",       qu_max,            grammar, 1);   // N       QPA_max  Quantite de Principe Actif maximum
              CGestIni::setXmlData("gph_punit",      gpp,               grammar, 1);   // mg      est utilise que si gph_pqbyuf != -1 CAD lie a la quantite de principe actif
            }
         if (qu_min.length() && qu_max.length())
            { if (qu_min != qu_max)                      gpp +="["+qu_min+"-"+qu_max+";]";
              else                                       gpp +="["+qu_min+";]";
            }
         else if (qu_min.length())                       gpp +="["+qu_min+";]";
         else if (qu_max.length())                       gpp +="["+qu_max+";]";

         QString valueTps_gpp = "";
         if      (fc_minTpsTxt.length() && fc_maxTpsTxt.length())  convertDureeQmaxToMinutes(fc_maxTpsTxt, &valueTps_gpp);
         else if (fc_minTpsTxt.length())                           convertDureeQmaxToMinutes(fc_minTpsTxt, &valueTps_gpp);
         else if (fc_maxTpsTxt.length())                           convertDureeQmaxToMinutes(fc_maxTpsTxt, &valueTps_gpp);
         if (valueTps_gpp.length())                      gpp += "U" + valueTps_gpp.mid(1) + valueTps_gpp[0] + ":";          // 'J1' --> U1J:
         gpp = "\n <gpp>"+gpp+"</gpp>\n";
         CGestIni::setXmlData("gps",   gpp, grammar,1);

         //...................... origine des donnees ....................
         QString gpi_engine = "POSO : " + dataSourceVersion();
         CGestIni::setXmlData("gpi_engine", gpi_engine,               grammar);

         grammarList.append(grammar);

         /*
         " <gph_fmin></gph_fmin>\n"         //        frequence minimum
         " <gph_fmax></gph_fmax>\n"         //        frequence maximum
         " <gph_funit></gph_funit>\n"       //        frequence unite
         " <gph_pmin></gph_pmin>\n"         //        posologie minimum
         " <gph_pmax></gph_pmax>\n"         //        posologie maximum
         " <gph_punit></gph_punit>\n"       //        posologie unite
         " <gph_nm></gph_nm>\n"             //        conversion mode en mode numerique UP_QUANTITE MOMENT_MEAL ALL_LITERAL (on se sert de celui-ci car plus rapide a interpreter)
         " <gph_cm></gph_cm>\n"             //        conversion mode en mode literal pour information lisible de celui du mode numerique  UP_QUANTITE MOMENT_MEAL ALL_LITERAL on a pas peur des nndances ! (vive la memoire vive et les HD)
         " <gph_in>\n"                      //        liste des indications
         "  <gph_il></gph_il>\n"            //        premier element de la liste des indications
         " </gph_in>\n"                     //        fin de la liste des indications <gph_il> </gph_il>
         " <gph_co></gph_co>\n"             //        commentaire global en fin de prescription
         "</gph>\n"                         // -- FIN du header d'une ligne prescriptive ---
         "<gpi_engine></gpi_engine>\n"      //        systeme de donnees a l'origine de cette ligne prescriptive
         "<gpi_user></gpi_user>\n"          //        utilisateur a l'origine de cette ligne prescriptive
         */

    }
 return grammarList;
}
//-------------------------------- get_DCI_list_FromCIP ------------------------------------------------------
/*! \brief return drug composition.
 *  \param const QString& cip drug cip identification .
 *  \return QStringList result list with dci composants
 */
QStringList C_BDM_DatasempPlugin::get_DCI_list_FromCIP(const QString &cip)
{
    QStringList retList;
    QString requete = QString (
                                " SELECT   UPPER(t27.f2)    \n"         // 0
                                " FROM  t11,                \n"
                                "       t27,                \n"
                                "       t00                 \n"
                                " WHERE t11.f0 = t00.f0     \n"
                                " AND   t00.f2 =  '%1'      \n"
                                " AND   t11.f1 = t27.f0     \n"
                              ).arg(cip);

    QSqlQuery query(requete , database() );
    if (! query.isActive())                             return retList;
    while (query.next())
       { retList.append ( CGestIni::Utf8_Query(query, 0) );
       }
    return  retList;
}

//-------------------------------- codePeriodToDays ------------------------------------------------------
double C_BDM_DatasempPlugin::codePeriodToDays (int code_period, double qu)
{switch (code_period)
   {case 44:   return  qu * 365.25;                     // ans --> jours
    case 43:   return  qu * 30.5;                       // mois --> jours
    case 42:   return  qu * 7;                          // semaine --> jours
    case 38:   return  qu * 1;                          // jours --> jours
   }
 return 0;
}

//-------------------------------- getInteractionDoseMinMax_fromCIP ------------------------------------------------------
/*! \brief test compatibility with maximum and minimun authorized dose and duration from a drug and patient context.
 *  \param C_PatientCtx *pC_PatientCtx
 *  \param const QString &grammar prescription for this drug
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair>  C_BDM_DatasempPlugin::getInteractionDoseMinMax_fromCIP(  C_PatientCtx *pC_PatientCtx, const QString &grammar)
{
/*
SELECT
t00.f2  as e,
t4B.f8  as name,
t2P.fp  as isPosoFixe,
t2P.f2  as QuMini,
t2P.f7  as QuMaxi,
t2p.f8  as QuCode,
tq.f1   as QuUnit,
t2P.fC  as DoseMaxi,
tmax.f1 as unitDoseMaxi,
t2P.f9  as DureQuMaxi,
t2P.fE  as DureDoseMaxi,

t2L.f1  as BorneInf,
t2L.f2  as BorneSup,
t2L.f3  as codeUnit,   -- liens poso EPP si 1 -> poids kg  44-> age an  43->age mois 42->age semaine  0->pas d'unite
tl.f1   as unitBorne
FROM
t24 tl,
t24 tmax,
t24 tq,
t2l,                 -- liens poso EPP
t2P,
t4B,
t00
where    t00.f0= t4B.f0
and      t2L.f4=t2P.f0
and      t2P.f1=t4B.f0
and      tl.f0=t2L.f3
and      tmax.f0=t2P.fD
and      tq.f0=t2P.f8
-- and      t2L.f3=0
-- and      t00.f2='3349826'  -- 3364062 / advil    3176159 / HICONCIL  3349826 / NOVARTEX
limit      0,500


SELECT distinct t2P.f9
FROM
t2P
group by t2P.f9
limit      0,50000

1 prise
par 24 heures
par 48 heures
par an
par heure
par jour
par minute
par mois
par semaine
toutes les 12 heures
type de  frequence non-specifie

*/
    QList<C_InterResultPair>  interResultPairList;
    if (pC_PatientCtx == 0) return interResultPairList;
    double      poids                 = 0;
    QString   s_poids                 = "";
    double      surface               = 0;
    int         ageInDays             = 0;
    QString     s_age                 = "";
    char        sexe                  = 'I';
    QString     s_sexe                = "";
    int            pos                = -1;
    QDate       dateOfPrise;
    /* toutes les 4 a 6 heures (46)
       par jour   par 24 heures
       par 24 heures (espacer les prises de 6 heures)
       1 prise
       par semaine
       par an
       par mois
       toutes les n heures (12 NH)
       par heure
       par minute
       par 48 heures
       type de frequence non-specifie
    */
    double q_min                      = 0;
    double q_max                      = 0;
    int nextPos                       = 0;
    QString    id                     = CGestIni::getXmlData( "gph_id",      grammar, &nextPos);
    QString    id_type                = CGestIni::getXmlData( "gph_it",      grammar, &nextPos);
    if ( id_type   != "CIP" )   return interResultPairList;
    QString    datePrise              = CGestIni::getXmlData( "gph_dt",      grammar, &nextPos).left(10); //        date et heure du debut de prise de la prescription format dd-MM-yyyy hh:mm:ss (la date de fin est calculee avec <gph_cy>)
    QString drugName                  = CGestIni::getXmlData( "gph_na",      grammar, &nextPos);
    QString duration                  = CGestIni::getXmlData( "gph_cy",      grammar, &nextPos);
    QStringList seqList               = CGestIni::getXmlDataList( "gpp",     grammar, &nextPos);

    double durationInDays             = C_PosologieGrammar::totalGlobalDurationInDays( duration , 1);

    C_PosologieGrammar::toMinMax_UP(seqList,  q_min,  q_max);
    QString s_q_min                   = QString::number(q_min,'f',2);
    QString s_q_max                   = QString::number(q_max,'f',2);
    //............ si doses non renseign\303\251es alors cassos car controles non possibles ..........................
    if (q_min==0 && q_max==0)    // Alerte : dose non precisees
       { interResultPairList.append( C_InterResultPair(  s_q_min+";"+s_q_max    , "QUANT", tr("DOSES NON PRECIS\303\211ES") ,
                                                         id                     , "CIP"  , drugName,
                                                         tr("Les doses indiqu\303\251es pour ce produit ne permettent pas de contr\303\264ler correctement la prescription, "
                                                            "car elles sont non renseign\303\251es"
                                                           ),"IT_QUANT", "1"
                                                      )
                                   );
         return interResultPairList;
       }
    if (pC_PatientCtx)
       {s_poids     = pC_PatientCtx->poids();
        poids       = pC_PatientCtx->poidsToDouble();
        surface     = pC_PatientCtx->surfaceToDouble();
        dateOfPrise = QDate::fromString(datePrise, "dd-MM-yyyy");
        ageInDays   = pC_PatientCtx->get_dateOfBirth().daysTo ( dateOfPrise );    // age du patient en jours au moment de la redaction de l'ordo
        s_sexe      = pC_PatientCtx->get_sexe();
        sexe        = s_sexe.toLatin1()[0];
        s_age       = pC_PatientCtx->get_age();
       }

    //........ recuperera les donnees de la base .....................
    double  drug_min                   = 0;
    double  drug_max                   = 0;
    QString s_drug_min                 = "";
    QString s_drug_max                 = "";
    QString drug_qu_unit               = "";
    QString drug_fact_corp             = "";
    double  drug_dose_max              = -1;
    QString drug_DoseMaxDuree          = "";
    double  drug_epp_inf               = 0;
    double  drug_epp_sup               = 0;
    int     drug_epp_unit              = 0;
    int     periodForMaxValue          = 0;
    char    drug_epp_flag              = 0;
    bool    isNoResponse               = true;
    int  ageIsNotInLimits              = 1;
    double  drug_max_age               = 0;
    double  drug_min_age               = DBL_MAX;
    int  weightIsNotInLimits           = 1;
    double  drug_max_weight            = 0;
    double  drug_min_weight            = DBL_MAX;
    double  drug_max_period            = 0;
    double  drug_min_period            = DBL_MAX;
    int isNotSexResponse               = 1;

    QString requete =           " SELECT                   \n"
                             // "  t00.f2                  \n"   //    as CIP
                             // " ,t4B.f8                  \n"   //    as name
                                "  t2P.f2                  \n"   // 0  as QuMini
                                " ,t2P.f7                  \n"   // 1  as QuMaxi
                             // " ,t2p.f8                  \n"   //    as QuCode
                                " ,tq.f1                   \n"   // 2  as QuUnit        -- cuillere-mesure de 5 millilitres/kg    /m2
                                " ,t2P.fC                  \n"   // 3  as DoseMaxi
                             // " ,tmax.f1                 \n"   //    as unitDoseMaxi  -- dose-graduation/kg
                                " ,t2P.fE                  \n"   // 4  as DureDoseMaxi  -- toutes les 4 a 6 heures (46)
                                " ,t2L.f1                  \n"   // 5  as BorneInf
                                " ,t2L.f2                  \n"   // 6  as BorneSup
                                " ,t2L.f3                  \n"   // 7  as codeUnit        -- 1 kg  44 an  43 mois 42 semaine 38 jours  0 pas d'unite
                                " ,t2L.f6                  \n"   // 8  as Flag         A/Allaitement F/Feminin M/Masculain
                                " ,t2P.fL                  \n"   // 9  as QuDureMini
                                " ,t2P.fM                  \n"   // 10 as CodeUnitQuDureMini
                             // " ,td_min.f1               \n"   //    as UnitDureMini
                                " ,t2P.fN                  \n"   // 11 as QuDureMaxi
                                " ,t2P.fO                  \n"   // 12 as CodeUnitQuDureMaxi
                                " ,t2P.f9                  \n"   // 13 as DureQuMaxi    -- idem  DureDoseMaxi  t2P.fE (pas tout a fait)

                             // " ,td_max.f1               \n"   //    as UnitDureMaxi
                             // " ,tl.f1                   \n"   //    as unitBorne

                                " FROM                     \n"
                             // " t24 tL,                  \n"
                             // " t24 tmax,                \n"
                                " t24 tq,                  \n"
                                " t2L,                     \n"
                                " t2P,                     \n"
                                " t4B,                     \n"
                                " t00                      \n"
                                " WHERE    t00.f0= t4B.f0  \n"
                                " AND      t2L.f4=t2P.f0   \n"
                                " AND      t2P.f1=t4B.f0   \n"
                             // " AND      tl.f0=t2L.f3    \n"
                             // " AND      tmax.f0=t2P.fD  \n"
                                " AND      tq.f0=t2P.f8    \n";
                             // " AND      t00.f2='%1'     \n" // -- 3364062 / advil    3176159 / HICONCIL"

    if (id.startsWith("'"))
        requete +=  QString (  " AND    t00.f2 IN (%1)  " ).arg(id);    // e datasemp code sous forme de liste
    else
        requete +=  QString (  " AND    t00.f2 = '%1'  "  ).arg(id);    // e datasemp code
    // CGestIni::Param_UpdateToDisk("/home/ro/requete.sql", requete);
    QSqlQuery query(requete , database() );
    if (! query.isActive())  return interResultPairList;
    C_InterResultPair last_IncorrectTaille;
    C_InterResultPair last_IncorrectQuForPoids;
    C_InterResultPair last_IncorrectQuForAge;
    C_InterResultPair last_IncorrectQuForSex;

    while (query.next())
       { isNoResponse        = false;
         drug_min            = query.value(0).toDouble();
         drug_max            = query.value(1).toDouble();
         if (drug_min>drug_max){double tmp = drug_max; drug_max=drug_min; drug_min=tmp;}
         drug_qu_unit        = CGestIni::Utf8_Query(query, 2);
         periodForMaxValue   = convertDureeQmaxToMinutes(query.value(13).toString());
         pos                 = drug_qu_unit.indexOf("/");
         if ( pos==-1 )
            { drug_fact_corp = "";
            }
         else
            { drug_fact_corp = drug_qu_unit.mid(pos+1,1);
              drug_qu_unit   = drug_qu_unit.left(pos);
            }
         drug_dose_max       = query.value(3).toDouble();  // exprim\303\251e pour la periode 'periodForMaxValue'
         drug_DoseMaxDuree   = CGestIni::Utf8_Query(query, 4);
         drug_epp_inf        = query.value(5).toDouble();
         drug_epp_sup        = query.value(6).toDouble();
         drug_epp_unit       = query.value(7).toInt();
         drug_epp_flag       = query.value(8).toChar().toLatin1();
         drug_min_period     = qMin(codePeriodToDays ( query.value(10).toInt(), query.value(9).toDouble()  ) , drug_min_period)  ;
         drug_max_period     = qMax(codePeriodToDays ( query.value(12).toInt(), query.value(11).toDouble() ) , drug_max_period)  ;

         if (drug_fact_corp=="k")
            { drug_min *= poids;
              drug_max *= poids;
            }
         else if (drug_fact_corp=="m")
            { if ( surface <= 0 )
                 {  last_IncorrectTaille = C_InterResultPair(  "NO DATA"    , "QUANT", tr("TAILLE DU PATIENT NON PRECIS\303\211E") ,
                                                                id                    , "CIP"  , drugName,
                                                                tr("La taille du patient \303\251tant indisponible les contr\303\264les"
                                                                    " sur les quantit\303\251s ne pourront \303\252tre correctement \303\251tablis, "
                                                                  ),"IT_QUANT", "1"
                                                            );
                 }
              drug_min *= surface;
              drug_max *= surface;
            }
         s_drug_min = QString::number(drug_min,'f',2);
         s_drug_max = QString::number(drug_max,'f',2);

         switch (drug_epp_unit)
            { //............ drug_epp_inf et drug_epp_sup expriment un poids et poso pour un poids entre __ et __ .................
              case 1:               // kg
                  { drug_max_weight  = qMax( drug_epp_sup , drug_max_weight );
                    drug_min_weight  = qMin( drug_epp_inf , drug_min_weight );
                    if ( weightIsNotInLimits && poids>drug_epp_inf && poids<drug_epp_sup )   // Poso conseillee pour cette fourchette de poids
                       {  weightIsNotInLimits = 0;           //-->  poso pour le poids est bonne concernant le poids
                             { if ( q_min  && q_min<drug_min )           // alert on est en dessous du minimum conseille pour ce poids
                                  { last_IncorrectQuForPoids =  C_InterResultPair(  s_q_min  , "QUANT", tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_min, drug_qu_unit) ,
                                                                                    id       , "CIP"  , drugName,
                                                                                    tr ( "Dose prescrite minimum %1 trop faible.<br/>"
                                                                                         "La dose minimum permise est de %2 pour un poids de %3 Kg"
                                                                                       ).arg(s_q_min+" "+drug_qu_unit, s_drug_min+" "+drug_qu_unit, s_poids),"IT_QUANT", "2"
                                                                                 );
                                    weightIsNotInLimits = 1; //-->  poso pour le poids n'est pas bonne concernant les quantites pour ce poids
                                  }
                               if (q_max  && q_max>drug_max)         // alert on est au dessus du maximum conseille pour ce poids
                                  { last_IncorrectQuForPoids =  C_InterResultPair(  s_q_max  , "QUANT", tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_max, drug_qu_unit) ,
                                                                                    id       , "CIP"  , drugName,
                                                                                    tr ( "Dose prescrite maximum %1 trop elev\303\251e.<br/>"
                                                                                         "Dose maximum permise est de %2 pour un poids de %3 Kg"
                                                                                       ).arg(s_q_max+" "+drug_qu_unit, s_drug_max+" "+drug_qu_unit, s_poids),"IT_QUANT", "4"
                                                                                 );
                                    weightIsNotInLimits = 1; //-->  poso pour le poids n'est pas bonne concernant les quantites pour ce poids
                                  }
                             }
                       }
                  } break;
              //............ drug_epp_inf et drug_epp_sup expriment un age et une poso pour un age entre __ et __ .................
              //             on transforme tous les ages en jours (permet de comparer toutes les sorties SQL)
              case 44:              // an
              case 43:              // mois
              case 42:              // semaine
              case 38:              // jour
                  { drug_epp_inf = codePeriodToDays ( drug_epp_unit, drug_epp_inf );   // normaliser les valeurs drug_epp_inf en jours
                    drug_epp_sup = codePeriodToDays ( drug_epp_unit, drug_epp_sup );   // normaliser les valeurs drug_epp_sup en jours
                    drug_max_age = qMax( drug_epp_sup , drug_max_age );
                    drug_min_age = qMin( drug_epp_inf , drug_min_age );
                    if (ageIsNotInLimits && ageInDays>drug_epp_inf && ageInDays<drug_epp_sup)   // Ok la Poso  est dans celle conseillee pour cette fourchette d'age
                       {  ageIsNotInLimits = 0;             //-->  poso pour l'age est bonne concernant l'age
                             { if ( q_min  && q_min<drug_min )         // alert quantite en dessous du minimum conseille pour cet age
                                  { last_IncorrectQuForAge = C_InterResultPair(   s_q_min  , "QUANT", tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_min, drug_qu_unit) ,
                                                                                  id       , "CIP"  , drugName,
                                                                                  tr( "Dose prescrite minimum %1 trop faible.<br/>"
                                                                                      "La dose minimum permise est de %2 pour un \303\242ge de %3 "
                                                                                    ).arg(s_q_min+" "+drug_qu_unit, s_drug_min+" "+drug_qu_unit, s_age),"IT_QUANT", "2"
                                                                              );
                                    // ageIsNotInLimits = 1;   //-->  poso pour l'age est pas bonne concernant les quantites pour l'age
                                  }
                               if ( q_max && drug_max && q_max>drug_max )         // alert quantite  au dessus du maximum conseille pour cet age
                                  { last_IncorrectQuForAge = C_InterResultPair(  s_q_max  , "QUANT", tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_max, drug_qu_unit) ,
                                                                                 id       , "CIP"  , drugName,
                                                                                 tr( "Dose prescrite %1 trop elev\303\251e pour ce terrain.<br/>"
                                                                                     "La dose maximum permise est de %2 pour : un \303\242ge de %3 et %4"
                                                                                   )
                                                                                 .arg( s_q_max+" "+drug_qu_unit,
                                                                                       s_drug_max+" "+drug_qu_unit,
                                                                                       s_age,
                                                                                       drug_fact_corp=="m"?tr(" surface : %1 m2").arg(QString::number(surface,'f',2)):tr(" poids : %1 kg").arg(s_poids)
                                                                                      ),"IT_QUANT", "4"
                                                                              );
                                   // ageIsNotInLimits = 1;   //-->  poso pour l'age est pas bonne concernant les quantites pour l'age
                                  }
                             } // if ( quForAgeIsNotInLimits )
                       } // if (ageInDays>drug_epp_inf && ageInDays<drug_epp_sup)
                  } break;
              //............ pas de systeme de poso selon facteur corporel .................
              //             mais le sexe peut etre une donnee de choix de la posologie
              default:
                  { if ( isNotSexResponse && drug_epp_flag==sexe )          // Poso conseillee pour le sexe en cours
                       {  isNotSexResponse = 0;
                          //if ( quForSexIsNotInLimits )
                             { if ( q_min  && q_min<drug_min )         // alert quantite en dessous du minimum conseille pour ce sexe
                                  { last_IncorrectQuForSex = C_InterResultPair(  s_q_min  , "QUANT", tr("DOSE %1 %2 INCOMPATIBLE POUR LE SEXE %3").arg(s_q_min, drug_qu_unit, s_sexe) ,
                                                                                 id       , "CIP" , drugName,
                                                                                 tr( "Dose prescrite minimum %1 trop faible.<br/>"
                                                                                     "La dose minimum permise pour le sexe %3 est de %2"
                                                                                   ).arg( s_q_min+" "+drug_qu_unit , s_drug_min+" "+drug_qu_unit, s_sexe ),"IT_QUANT", "2"
                                                                              );
                                    isNotSexResponse = 1;
                                  }
                               if ( q_max  && q_max>drug_max )         // alert quantite  au dessus du maximum conseille pour ce sexe
                                  { last_IncorrectQuForSex = C_InterResultPair(  s_q_max  , "QUANT", tr("DOSE %1 %2 INCOMPATIBLE POUR LE SEXE %3").arg(s_q_min, drug_qu_unit, s_sexe) ,
                                                                                 id       , "CIP"  , drugName,
                                                                                 tr( "Dose prescrite maximum %1 trop elev\303\251e.<br/>"
                                                                                     "La dose maximum permise pour le sexe %3 est de %2 "
                                                                                   ).arg( s_q_max+" "+drug_qu_unit , s_drug_max+" "+drug_qu_unit, s_sexe ),"IT_QUANT", "4"
                                                                              );
                                    isNotSexResponse = 1;
                                  }
                             }
                       }
                    else    // Alerte :aucun critere physio pathologique mais quantites en dehors des limites
                       {
                          if (q_min  && q_min<drug_min)         // alert quantite en dessous du minimum conseille
                             { interResultPairList.append( C_InterResultPair(  s_q_min  , "QUANT", tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_min, drug_qu_unit) ,
                                                                               id       , "CIP"  , drugName,
                                                                               tr("Dose minimum %1 trop faible (pas de facteur physiopathologique)<br/>"
                                                                                  "La dose minimum permise est de %2"
                                                                                 ).arg( s_q_max+" "+drug_qu_unit , s_drug_min+" "+drug_qu_unit ),"IT_QUANT", "2"
                                                                            )
                                                         );
                             }
                          if (q_max  && q_max>drug_max)         // alert quantite  au dessus du maximum conseille
                             { interResultPairList.append( C_InterResultPair(  s_q_max  , "QUANT", tr("DOSE %1 %2 INCOMPATIBLE").arg(s_q_max, drug_qu_unit) ,
                                                                               id       , "CIP"  , drugName,
                                                                               tr("Dose maximum %1 trop elev\303\251e (pas de facteur physiopathologique)<br/>"
                                                                                  "La dose maximum permise est de %2"
                                                                                 ).arg( s_q_max+" "+drug_qu_unit ,  s_drug_max+" "+drug_qu_unit ),"IT_QUANT", "4"
                                                                            )
                                                         );
                             }
                       }
                  }
            } // switch (drug_epp_unit)
       } // end while (query.next())
    if (isNoResponse)
       { interResultPairList.append( C_InterResultPair(  s_q_max  , "QUANT", tr("CONTROLE DES DOSES ET DUREES NON POSSIBLE") ,
                                                         id       , "CIP"  , drugName,
                                                         tr("Donn\303\251es non trouv\303\251es pour le contr\303\264le des doses <br/>"),"IT_QUANT", "1"
                                                      )
                                   );
         return interResultPairList;
       }

    if ( drug_min_period && durationInDays < drug_min_period)    // Alerte : duree prescription trop faible
       { QString s_durationInDays = QString::number(durationInDays,'f',0);
         interResultPairList.append( C_InterResultPair(  s_durationInDays       , "TEMPS"        , tr("DUREE DE PRESCRIPTION DE %1 JOUR(S) TROP COURTE").arg(s_durationInDays) ,
                                                         id                     , "CIP"          , drugName,
                                                         tr("La dur\303\251e de prescription %1 jours(s) est trop faible. "
                                                            "La fourchette de dur\303\251es pour une prescription correcte de ce produit est de %2 \303\240 %3 jour(s)"
                                                           ).arg(s_durationInDays, QString::number(drug_min_period,'f',2), QString::number(drug_max_period,'f',2)),"IT_TEMPS", "2"
                                                      )
                                   );
       }
    if ( drug_max_period && durationInDays > drug_max_period)    // Alerte : duree prescription trop elevee
       { QString s_durationInDays = QString::number(durationInDays,'f',0);
         interResultPairList.append( C_InterResultPair(  s_durationInDays       , "TEMPS"        , tr("DUREE DE PRESCRIPTION DE %1 JOUR(S) TROP GRANDE").arg(s_durationInDays) ,
                                                         id                     , "CIP"          , drugName,
                                                         tr("La dur\303\251e de prescription %1 jours(s) est trop grande. "
                                                            "La fourchette de dur\303\251es pour une prescription correcte de ce produit est de %2 \303\240 %3 jour(s)"
                                                           ).arg(s_durationInDays, QString::number(drug_min_period,'f',2), QString::number(drug_max_period,'f',2)),"IT_TEMPS", "2"
                                                      )
                                   );
       }
    if ( weightIsNotInLimits && drug_max_weight>0 )    // Alerte : poids evalue (drug_max_weight>0) mais en dehors des limites (il y a eu des reponse mais aucune correspond)
       { interResultPairList.append( C_InterResultPair(  s_q_min+";"+s_q_max    , "QUANT", tr("POIDS PATIENT %1 Kg INCOMPATIBLE").arg(s_poids) ,
                                                         id                     , "CIP"  , drugName,
                                                         tr("Ce produit ne permet pas d'\303\251tablir une prescription correcte pour le poids de %1 kg de ce patient. "
                                                            "La fourchette de poids pour une prescription correcte de ce produit est de %2 \303\240 %3 kg"
                                                           ).arg(s_poids, QString::number(drug_min_weight,'f',2), QString::number(drug_max_weight,'f',2)),"IT_POIDS", "2"
                                                      )
                                   );
       }
    if ( ageIsNotInLimits && drug_max_age>0 )        // Alerte : age evalue (drug_max_age>0 ) mais en dehors des limites (il y a eu des reponse mais aucune correspond)
       {
         interResultPairList.append( C_InterResultPair(  s_q_min+";"+s_q_max    , "QUANT", tr("AGE %1 INCOMPATIBLE").arg(s_age) ,
                                                          id                     , "CIP" , drugName,
                                                          tr("Ce produit ne permet pas d'\303\251tablir une prescription correcte pour l'\303\242ge de %1 de ce patient. "
                                                             "La fourchette d'\303\242ge pour une prescription correcte est de %2 \303\240 %3  an(s)"
                                                            ).arg(s_age, QString::number(drug_min_age/365.25,'f',2),QString::number(drug_max_age/365.25,'f',2)),"IT_YEARS", "2"
                                                       )
                                    );
       }
    if (last_IncorrectTaille.CodeTyp1()=="QUANT")                              interResultPairList.append(last_IncorrectTaille);
    if (isNotSexResponse    && last_IncorrectQuForSex.CodeTyp1()=="QUANT")     interResultPairList.append(last_IncorrectQuForSex);
    if (weightIsNotInLimits && last_IncorrectQuForPoids.CodeTyp1()=="QUANT")   interResultPairList.append(last_IncorrectQuForPoids);
    if (ageIsNotInLimits==0 && last_IncorrectQuForAge.CodeTyp1()=="QUANT")     interResultPairList.append(last_IncorrectQuForAge);
    return interResultPairList;
}
//-------------------------------- convertDureeQmaxToMinutes -------------------------------------------------
/*! \brief times periods values for posology max are in textual format. To numericals controls there must be converted in numerical values
 *  \param const QString& perioForQuMax textual period .
 *  \return int numerical value
 */
// les donnees concernant la periode pour laquelle la qu max est definie, n'est que sous FromCIP textuelle donc pour
// faire du controle a posteriori il faut convertir les donnees textuelles en numeriques
/*

 */
int  C_BDM_DatasempPlugin::convertDureeQmaxToMinutes(const QString &perioForQuMax,  QString *valueTps /*=0*/)
{
    /*
    SELECT distinct t2P.f9
    FROM
    t2P
    group by t2P.f9
    limit 0, 50000

            1 prise
            par 24 heures
            par 24 heures (espacer les prises de 12 heures)
            par 24 heures (espacer les prises de 2 heures)
            par 24 heures (espacer les prises de 4 heures)
            par 24 heures (espacer les prises de 4 a 6 heures)
            par 24 heures (espacer les prises de 6 heures)
            par 24 heures (espacer les prises de 8 heures)
            par 48 heures
            par an
            par heure
            par jour
            par minute
            par mois
            par semaine
            toutes les 12 heures
            toutes les 2 heures
            toutes les 3 heures
            toutes les 4 heures
            toutes les 48 heures
            toutes les 5 heures
            toutes les 6 heures
            toutes les 72 heures
            toutes les 8 heures
            type de frequence non-specifie
    */
 int       qu = 1;
 QString s_qu = "1";
 if (valueTps) *valueTps="";

 QString word;
 if (perioForQuMax.startsWith("par "))
    {
      int pos = CGestIni::getNextWord(perioForQuMax, 4, word);
      if (word[0]>=0 && word[0]<=9)
         { s_qu = word;
           qu   = s_qu.toInt();
           pos  = CGestIni::getNextWord(perioForQuMax, pos, word);
         }
      if      (word[0]=='h')                 { if (valueTps) *valueTps="H"+s_qu;  return qu*60;}
      else if (word[0]=='m' && word[1]=='i') { if (valueTps) *valueTps="I"+s_qu;  return qu;}
      else if (word[0]=='j')                 { if (valueTps) *valueTps="J"+s_qu;  return qu*1440;}
      else if (word[0]=='s')                 { if (valueTps) *valueTps="S"+s_qu;  return qu*1440*7;}
      else if (word[0]=='m' && word[1]=='o') { if (valueTps) *valueTps="M"+s_qu;  return qu*1440*30;}
      else if (word[0]=='a')                 { if (valueTps) *valueTps="A"+s_qu;  return qu*1440*365;}
    }
 else if (perioForQuMax.startsWith("toutes les "))
    { int pos = CGestIni::getNextWord(perioForQuMax, 11, word);
      if (word[0]>=0 && word[0]<=9)
         { s_qu = word;
           qu   = s_qu.toInt();
           pos  = CGestIni::getNextWord(perioForQuMax, pos, word);
         }
      { if (valueTps) *valueTps="H"+s_qu; return qu*60;};
    }
 return 0;
}

//-------------------------------- getInteractionCim10 ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param const QStringList &cim10List cim10 pathologies list .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
/*
SELECT t55.f0 AS code_vidal_patho,
       t4N.f2 as nom,
       t4N.f3 as codeCim10,
       t50.f1 as nameDatasemp
FROM t4N ,
     t55 ,
     t50
WHERE t4N.f0=t55.f1
AND   t50.f0=t55.f0
AND   t4N.f3='Z32.1'

SELECT
       t52.f2 as niveau,
       t1C.f1 as libelleNiveau,
       t00.f3 as drug_name,
       t50.f1 AS CI_Vidal,
       t4N.f2 AS CIM_X,
       t4N.f3 AS codeCIM_X
FROM t52
INNER JOIN t00 ON t52.f1 = t00.f0
INNER JOIN t1C ON t52.f2 = t1C.f0  -- libelle
INNER JOIN t50 ON t52.f0 = t50.f0,
t55,
t4N
WHERE t00.f2='3514685'
-- AND t00.f3 LIKE 'ASPEGIC 1000%'
AND t55.f1 = t4N.f0
AND t55.f0 = t50.f0
AND t4N.f3 like 'K27%' -- 'K27'
limit 0,100

 SELECT
 t52.f2 ,                                             --  0 as niveau
 t1C.f1 ,                                             --  1 as libelleNiveau
 t50.f1 ,                                             --  2 as CI_Vidal
 t4N.f2 ,                                             --  3 as CIM_X
 t4N.f3 ,                                             --  4 as codeCIM_X
 t00.f3                                               --  5 as drug_name
 FROM t52
 INNER JOIN t00 ON t52.f1 = t00.f0
 INNER JOIN t1C ON t52.f2 = t1C.f0                    --  libelle Niveau
 INNER JOIN t50 ON t52.f0 = t50.f0,
 t55,                                                 --  lien patho vidal et cim10
 t4N                                                  --  table cim10
 WHERE t00.f2 ='3777769'                              --  cip '3844187'
 AND   t55.f1 = t4N.f0
 AND   t55.f0 = t50.f0
 AND   t4N.f3 IN ('K35','J45.0','J45','Z32.1','Z32','N19','Q89.3','Q89','Z39.1','Z39')                                  --  liste des pathos patient
 -- AND   t52.f2 IN ('2999','2991','2990','2996')
 */
QList<C_InterResultPair> C_BDM_DatasempPlugin::getInteractionCim10(const QString &cip, const QStringList &cim10List , QString drugName  /* = "" */ )
{   QList<C_InterResultPair>            interResultPairList;
    if (cim10List.length()==0)   return interResultPairList;
    QMap         <QString, C_InterResultPair>  mapResult;
    mapResult = Datasemp_get_Cim10_InterResultPair_Map( cip, cim10List , drugName);        // la map evite les doublons
    QMapIterator <QString, C_InterResultPair> it1(mapResult);
    while ( it1.hasNext() )
          {  it1.next();
             interResultPairList.append( it1.value() );
          }
    return interResultPairList;
}

     //-------------------------------- Datasemp_get_Cim10_InterResultPair_Map ------------------------------------------------------
     /*! \brief return physiopath interactions from patient and drug.
      *  \param const QString& cip drug cip identification .
      *  \param const QStringList &cim10List cim10 pathologies list .
      *  \param QString drugName (default empty) the drug name.
      *  \return QList<C_InterResultPair> result interaction pair list
      */
     QMap<QString, C_InterResultPair> C_BDM_DatasempPlugin::Datasemp_get_Cim10_InterResultPair_Map(const QString &cip, const QStringList &cim10List , QString drugName)
     {   QMap<QString,C_InterResultPair> cimMap;
         int     niveau              = -1;
         QString  cim10              = "";
         QString  str                = "";
         QString codesCim10Patients  = "(";
         for (int i=0; i<cim10List.size(); ++i)
             { str = cim10List[i];
               if (str.length()>3)
                  { codesCim10Patients += "'" + str.insert(3,'.') + "',";
                    codesCim10Patients += "'" + str.left(3)       + "',";
                  }
               else
                  {codesCim10Patients += "'" + str      + "',";
                  }
             }
         codesCim10Patients.chop(1);   // virer la derniere virgule
         codesCim10Patients += ")";

         QString requete = QString (
                                    " SELECT                                        \n"
                                    " t52.f2 ,                                      \n"   // 0 as niveau
                                    " t1C.f1 ,                                      \n"   // 1 as libelleNiveau
                                    " t50.f1 ,                                      \n"   // 2 as CI_Vidal
                                    " t4N.f2 ,                                      \n"   // 3 as CIM_X
                                    " t4N.f3 ,                                      \n"   // 4 as codeCIM_X
                                    " t00.f3 ,                                      \n"   // 5 as drug_name
                                    " t52.f5 ,                                      \n"   // 6 as memo
                                    " t45.f1 ,                                      \n"   // 7 as libelle_biblio,
                                    " t45.f5 ,                                      \n"   // 8 as lien_biblio
                                    " t45.fC                                        \n"   // 9 as date_biblio  ne pas s'exiter : Le champ t45.f3 (memo) est toujours vide et ne sert a rien
                                    " FROM t52                                      \n"
                                    " INNER JOIN t00 ON t52.f1 = t00.f0             \n"
                                    " INNER JOIN t1C ON t52.f2 = t1C.f0             \n"   // libelle Niveau
                                    " INNER JOIN t50 ON t52.f0 = t50.f0,            \n"
                                    " t55,                                          \n"   // lien patho vidal et cim10
                                    " t4N,                                          \n"   // table cim10
                                    " t45                                           \n"   // table references
                                    " WHERE t00.f2 ='%1'                            \n"   // cip
                                    " AND   t55.f1 = t4N.f0                         \n"
                                    " AND   t55.f0 = t50.f0                         \n"
                                    " AND   t52.f6 = t45.f0                         \n"   // table references
                            //      " AND   t4N.f3 IN %2                            \n"   // liste des pathos patient
                                    " AND   t52.f2 IN ('2999','2991','2990','2996') \n"   // ultra lent --> il vaut mieux filtrer les niveaux par programmation
                                    ).arg(cip);

          QSqlQuery query(requete , database() );
          if    (  !query.isActive() ) return cimMap;
          while (   query.next()     )
                {   niveau = query.value(0).toInt();
                    switch(niveau)   // fitrage des niveaux par programmation car en SQL IN ('2999','2991','2990','2996') c'est archi lent ??!!
                          {case 2999:      // CI absolue
                           case 2991:      // effet ind\303\251sirable information
                           case 2988:      // Mise en garde
                           case 2990:      // precaution d'emploi
                           case 2996:      // CI relative
                               { cim10  = query.value(4).toString().remove('.');
                                 if ( Datasemp_isThisCim10Code_in_Cim10List(cim10, cim10List))
                                    { QString info            = CGestIni::Utf8_Query(query, 6);
                                      QString biblio_libelle  = CGestIni::Utf8_Query(query, 7);
                                      QString biblio_ref      = query.value(8).toString();
                                      QString biblio_date     = query.value(9).toString();
                                      if ( biblio_ref.startsWith("http://") )  biblio_ref = QString("<a href=\"%1\">%1</a>")         .arg(biblio_ref);
                                      else                                     biblio_ref = QString("<a href=\"ref_mono:%1\">%1</a>").arg(biblio_ref);

                                      if ( biblio_date.length() )
                                         { biblio_date = QDateTime::fromString(biblio_date,Qt::ISODate).toString("dd-MM-yyyy");
                                         }
                                      else
                                         { biblio_date = "date indisponible";
                                         }
                                      QString memo  = tr("<b>\303\211tat physiopathologique du patient</b> : %1<br/>").arg( CGestIni::Utf8_Query(query, 2) ) +
                                                      tr("<b>Niveau</b> : %1<br/>" ).arg( CGestIni::Utf8_Query(query, 1))+
                                                      tr("<b>R\303\251f\303\251rence dat\303\251e du</b> : %1<br/>"   // biblio date
                                                         "<b>R\303\251f\303\251rence</b> : %2<br/>"                   // biblio libelle
                                                         "%3<br/>"                                                    // biblio reference
                                                        ).arg(biblio_date,biblio_libelle,biblio_ref)
                                                      +
                                                      tr("<b>Informations</b> : %1").arg( (info.length()?info:tr("indisponible.")) );
                                      cimMap[cim10] =  C_InterResultPair(  cim10    , "CIM", CGestIni::Utf8_Query(query, 3)+ " ("+query.value(4).toString()+")",
                                                                           cip      , "CIP", drugName,
                                                                           memo     ,
                                                                           "IT_CIM", Datasemp_LevelToLevelStr(niveau)
                                                                        );

                                    } // if ( Datasemp_isThisCim10Code_in_Cim10List(cim10, cim10List))
                               } // case
                          } // switch(niveau)
                }
         return cimMap;
     }

     //--------------------------- Datasemp_isThisCim10Code_in_Cim10List -----------------------------------------------------------
     /*! \brief return if a cim10 code is in cim10 patient list.
      *  \param const QString& cim10 cim10 code to test presence in Cim10List patient codes .
      *  \param const QStringList &cim10List cim10 patient pathologies list .
      *  \return bool true if code already present false otherwise
      */
     bool C_BDM_DatasempPlugin::Datasemp_isThisCim10Code_in_Cim10List(const QString &cim10, const QStringList &Cim10List)
     {
      if (cim10.length()==0) return false;
      int pos          = cim10.indexOf('-');
      QString cim      = "";
      QString code     = "";
      int      lenCmp  = 0;
      int           i  = 0;
      //............. ce n'est pas un code compose on cherche en direct dans la liste ......................
      if (pos == -1)
         { for ( i=0; i<Cim10List.count();++i)
               {code   = Cim10List[i];
                cim    = cim10;
                lenCmp = qMin( cim.length(), code.length() );
                cim.truncate(lenCmp);
                code.truncate(lenCmp);
                if (cim==code)
                   {return true;
                   }
               }
           return false;
         }
      //............. c'est un code compose N84-N89 on cherche en parcourant dans la liste ..................
      int n            = 0;
      QString cim10Deb = cim10.left(pos);
      QString cim10End = cim10.mid(pos+1);
      cim10Deb.truncate(3);
      cim10End.truncate(3);
      int nMin         = cim10Deb.mid(1).toInt();
      int nMax         = cim10End.mid(1).toInt();;

      for ( i=0; i<Cim10List.size();++i)
          { code   = Cim10List[i].left(3);
            if (cim10[0] != code[0]) continue;
            n      = code.mid(1).toInt();
            if (n>=nMin && n<=nMax) return true;
          }
      return false;
     }

//-------------------------------- getInteractionPairFromCipList ------------------------------------------------------
/*! \brief return interactions xml results from a cip list.
 *  \param const QStringList &cipList      drugs CIP codes list to test.
 *  \param QString drugName (default empty) the drug name.
 *  \return result interaction pair list
 */

QList<C_InterResultPair> C_BDM_DatasempPlugin::getInteractionPairFromCipList ( const QStringList &cipList)
{ QList<C_InterResultPair>        interResultPairList;

#ifdef QT_DEBUG
     QTime deb_t = QTime::currentTime();
#endif
  int cipListSize = cipList.size();
  if (cipListSize <= 1 )  return  interResultPairList;
  INTER_STRUCT inter;
  INTER_STRUCT inter_1;
  INTER_STRUCT inter_2;
  //..................... la requete sql .................
  QString requete = QString (
                             " SELECT     \n"
                             " t00.f2 ,   \n" // 0 as cip,
                             " t00.f3 ,   \n" // 1 as specialite drugname,
                             " t37.f1 ,   \n" // 2 as Code_Classe_Inter,
                             " t26.f4     \n" // 3 as Libelle_Classe_Inter
                             " FROM t4B,  \n"
                             " t37,       \n"
                             " t00,       \n"
                             " t26        \n"
                             " WHERE t4B.f0 = t00.f0  \n"
                             " AND   t4B.f4 = t37.f0  \n"
                             " AND   t26.f0 = t37.f0  \n"
                             " AND   t00.f2 IN %1     \n"
                             " ORDER BY t00.f2        \n"
                             ).arg( "('" + cipList.join("','") + "')" );
   // QString str_cip_list = "('" + cipList.join("','") + "')" ;
   //qDebug()<< "CIP LIST : "<<str_cip_list;
   QSqlQuery query(requete , database() );
   if    ( !query.isActive() ) return interResultPairList;
   //......... relever toutes les lignes d'interactions de chaque produit ...........
   //          pour chaque cip de la liste. Elles sont en vrac.
   //          et partiellement renseignees (pas de level ni infos etendues
   //          ni reference biblio)
   QList<INTER_STRUCT> sql_result_inter_code_list;
   while (  query.next() )
         { //........ relever les infos .............
           inter_1.m_cip      = query.value(0).toInt();          // 0 as cip,
           inter_1.m_drugName = CGestIni::Utf8_Query(query, 1);  // 1 as specialite drugname,
           inter_1.m_id_1     = query.value(2).toInt();          // 2 as Code_Classe_Inter,
           inter_1.m_libelle  = CGestIni::Utf8_Query(query, 3);  // 3 as Libelle_Classe_Inter
           //qDebug() <<"First list CIP : " <<  inter_1.m_cip << " name : " << inter_1.m_drugName << " code : " << inter_1.m_id_1;
           sql_result_inter_code_list.append( inter_1 ) ;
        }
   //......... croiser les lignes d'interactions pour reperer les produits .............
   //          en eventuelle interaction entre eux, faire des paires
   //          qui seront testees ensuite
   QMap< QString, INTER_STRUCT> inter_pair_Map;
   for (int i=0; i<sql_result_inter_code_list.size(); ++i)
       { inter            = sql_result_inter_code_list[i];
         for (int u=0; u<sql_result_inter_code_list.size(); ++u)
             { inter_2            =  sql_result_inter_code_list[u];
               if ( inter.m_cip   == inter_2.m_cip  ) continue;  // ne pas se coupler a nous meme
               if ( inter.m_id_1  == inter_2.m_id_1 ) continue;  // ne pas se coupler a nous meme
               inter_1            = inter; // faut pas que lors d'un eventuel swap celui de reference soit change la suite du traitement se fait donc sur inter_1
               //.......... le swap afin que le plus petit code soit en premier .............
               QString key = "";  // 'petit cip' 'grand cip' 'petit code' 'grand code'  // eviter le redondances
               if ( inter_1.m_id_1 > inter_2.m_id_1 )
                  { INTER_STRUCT inter_tmp = inter_1;           inter_1 = inter_2;           inter_2 = inter_tmp;
                  }
               if ( inter_1.m_cip < inter_2.m_cip)   key = QString::number(inter_1.m_cip)+QString::number(inter_2.m_cip);
               else                                  key = QString::number(inter_2.m_cip)+QString::number(inter_1.m_cip);
               key +=   QString::number(inter_1.m_id_1)+ QString::number(inter_2.m_id_1);
               //qDebug() <<"Key_: " <<  key;
               if ( ! inter_pair_Map.contains(key) )
                  { //... renseigner les info du deuxieme ...........
                    //qDebug() <<"Key : " <<  key;
                    inter_1.m_drugName_2 = inter_2.m_drugName;
                    inter_1.m_id_2       = inter_2.m_id_1;      // peuvent avoir ete permutes donc le renoter ici
                    inter_1.m_cip2       = inter_2.m_cip;
                    inter_1.m_libelle_2  = inter_2.m_libelle;
                    inter_pair_Map.insert( key, inter_1 );
                  }
             }
       }  // for (int i=0; i<sql_result_inter_code_list; ++i)

   //................ methode IN (ne le fait pas et ben si et si eh ben non presque et c'est dommage)................................
   /*
   QStringList list_1,list_2;
   QMapIterator< QString, INTER_STRUCT> lt (inter_pair_Map);
   while ( lt.hasNext() )
         { lt.next();
           INTER_STRUCT inter = lt.value();
           if (inter.m_id_1==inter.m_id_2) continue;  // ne pas se coupler a nous meme
           list_1 << QString::number(inter.m_id_1);
           list_2 << QString::number(inter.m_id_2);
         }
   QString l1 = "('"+list_1.join("','")+"')";
   QString l2 = "('"+list_2.join("','")+"')";
   requete = QString (
                        " SELECT     \n"
                        " t10.f2 ,   \n" // 0 as code_alerte,
                        " t10.f0 ,   \n" // 1 as code_1,
                        " t10.f1 ,   \n" // 2 as code_2,
                        " t10.f4 ,   \n" // 3 as ref_biblio
                        " t10.f5 ,   \n" // 4 as fiche_SGML
                        " t10.f6 ,   \n" // 5 as code_doc
                        " t10.fC     \n" // 6 as date_doc
                  //      ",t1G.f1     \n" // 7 as libele_alerte
                  //      ",t29.f1     \n" // 8 as reference_alerte
                        " FROM t10  \n"
                  //    "    , t1G  \n"
                  //    "    ,t29   \n"
                        " WHERE t10.f0 IN %1      \n"
                        " AND   t10.f1 IN %2      \n"
                  //      " AND   t1G.f0 = t10.f2   \n"
                  //      " AND   t29.f0 = t10.f4   \n"
                     ).arg(l1,l2);
   //....... voir si interaction pour une de ces paires .................
   // qDebug() << requete;
   if ( query.exec( requete ) )
      { while ( query.isActive() && query.next() )
              {   //...... on filtre sur les couple de code d'interaction connus .............

                  int code_inter_1 = query.value(1).toInt();
                  int code_inter_2 = query.value(2).toInt();
                  if (code_inter_1>code_inter_2){int tmp=code_inter_1; code_inter_1=code_inter_2; code_inter_2=tmp;}
                  QString key = QString::number(code_inter_1) + QString::number(code_inter_2);

                  QMapIterator<QString, INTER_STRUCT> mi(inter_pair_Map);
                  inter.m_cip = 0;
                  while ( mi.hasNext() ) {  mi.next();
                                            if ( mi.key().mid(14)==key )
                                               { inter = mi.value();
                                                 break;
                                               }
                                         }
                  if ( inter.m_cip == 0 ) continue;

                  #ifdef QT_DEBUG
                     qDebug() << "Key :" << key << "Drug1 " << inter.m_drugName << "Drug2 " << inter.m_drugName_2;
                  #endif
                  inter.m_level        = query.value(0).toString();
                  inter.m_document     = CGestIni::Utf8_Query(query, 4);

                  QString classe_info  = tr("<b>Date de la ref\303\251rence </b> : %1<br/>"
                                       //   "<b>Ref\303\251rence</b> :"+CGestIni::Utf8_Query(query, 8)+
                                            "<b>Classe 1</b> : %2<br/>"
                                            "<b>Classe 2</b> : %3<br/>"
                                            "<br/><b><u>Informations et pr\303\251cisions</u></b> :"
                                           )
                                           .arg( query.value(6).toDateTime().toString("dd-MM-yyyy"), inter.m_libelle, inter.m_libelle_2);
                  interResultPairList.append (  C_InterResultPair (  QString::number(inter.m_cip)     ,                 "CIP",       inter.m_drugName,
                                                                     QString::number(inter.m_cip2)    ,                 "CIP",       inter.m_drugName_2,
                                                                     tr ( "%1<br/>%2" ).arg(classe_info, Datasemp_SGML_to_html(inter.m_document))     , "AFSSAPS", inter.m_level
                                                                        )
                                             );
              }
      }
   else
      { outSQL_error( query, "ERROR in get T10 infos" , requete , __FILE__, __LINE__ );
      }
#ifdef QT_DEBUG
     QTime end_t = QTime::currentTime();
     // outMessage(tr("TIME method IN : getInteractionPairFromCipList %1").arg(QString::number(deb_t.msecsTo(end_t))));
     qDebug()<< tr("TIME method IN : getInteractionPairFromCipList %1").arg(QString::number(deb_t.msecsTo(end_t)));
#endif
 */

   //................ methode WHILE ................................
   requete = QString (
                        " SELECT     \n"
                        " t10.f2 ,   \n" // 0 as code_alerte,
                        " t10.f0 ,   \n" // 1 as code_1,
                        " t10.f1 ,   \n" // 2 as code_1,
                        " t10.f4 ,   \n" // 3 as ref_biblio
                        " t10.f5 ,   \n" // 4 as fiche_SGML
                        " t10.f6 ,   \n" // 5 as code_doc
                        " t10.fC ,   \n" // 6 as date_doc
                        " t45.f1 ,   \n" // 7 as libelle_biblio,
                        " t45.f5 ,   \n" // 8 as lien_biblio
                        " t45.fC     \n" // 9 as date_biblio
                      //  " ,t1G.f1     \n" // 7 as libele_alerte
                        " FROM t10               \n"
                        "     ,t45               \n"
                        " WHERE t10.f0 = %1      \n"
                        " AND   t10.f1 = %2      \n"
                        " AND   t45.f0 = t10.f6  \n"
                     );
   //qDebug() << requete;
   //....... voir si interaction pour une de ces paires .................
   QMapIterator< QString, INTER_STRUCT> lt (inter_pair_Map);
   while ( lt.hasNext() )
         { lt.next();
           QString key = lt.key() ;

           if ( m_interCIP_cache.contains( key ) )
              { interResultPairList.append ( m_interCIP_cache[key] );
                qDebug()<< tr("getInteractionPairFromCipList() find in cache key : %1").arg(key);
                continue;
              }

           inter              = lt.value();
           QString    cur_req = requete;    //ne pas detruire le modele
                      cur_req = cur_req.arg( QString::number( inter.m_id_1 ) , QString::number( inter.m_id_2 ) );
          //            qDebug()<<"Key 2 : " << QString::number(inter.m_id_1) <<" " <<QString::number(inter.m_id_2);
           if ( ! query.exec(cur_req) )
              { outSQL_error( query, "ERROR in get T10 infos" , cur_req , __FILE__, __LINE__ );
                continue;
              }
           while ( query.isActive() && query.next() )
              {// qDebug()<<"Key 2 : " <<  lt.key();
               inter.m_level           = query.value(0).toString();
               inter.m_document        = CGestIni::Utf8_Query(query, 4);
               QString biblio_libelle  = CGestIni::Utf8_Query(query, 7);
               QString biblio_ref      = query.value(8).toString();
               QString biblio_date     = query.value(9).toString();
               if ( biblio_ref.startsWith("http://") )  biblio_ref = QString("<a href=\"%1\">%1</a>")         .arg(biblio_ref);
               else                                     biblio_ref = QString("<a href=\"ref_mono:%1\">%1</a>").arg(biblio_ref);

               if ( biblio_date.length() )
                  { biblio_date = QDateTime::fromString(biblio_date,Qt::ISODate).toString("dd-MM-yyyy");
                    biblio_date = ("<b>Date du document</b> : "+biblio_date+" ");
                  }
               else
                  { biblio_date = "<b>Date du document</b> : absente ";
                  }

               QString classe_info  = tr("<b>Date de la ref\303\251rence </b> : %1<br/>"
                                         "%4<br/>"   // biblio date
                                         "<b>Source du document</b> : %5<br/>"   // biblio libelle
                                         "%6<br/>"   // biblio reference
                                    //   "<b>Ref\303\251rence</b> :"+CGestIni::Utf8_Query(query, 8)+
                                         "<b>Classe 1</b> : %2<br/>"
                                         "<b>Classe 2</b> : %3<br/>"
                                         "<br/><b><u>Informations et pr\303\251cisions</u></b> :"
                                        )
                                        .arg( query.value(6).toDateTime().toString("dd-MM-yyyy"), inter.m_libelle, inter.m_libelle_2, biblio_date, biblio_libelle, biblio_ref);
               C_InterResultPair result (  QString::number(inter.m_cip)     ,                 "CIP",       inter.m_drugName,
                                           QString::number(inter.m_cip2)    ,                 "CIP",       inter.m_drugName_2,
                                           tr ( "%1<br/>%2" ).arg(classe_info, Datasemp_SGML_to_html(inter.m_document))     , "AFSSAPS", inter.m_level
                                        );
              // if ( !m_interCIP_cache.contains( key ) )
                     m_interCIP_cache[key]=result;
               interResultPairList.append ( result );
              }
         }
#ifdef QT_DEBUG
     QTime end_t = QTime::currentTime();
     // outMessage(tr("TIME : getInteractionPairFromCipList %1").arg(QString::number(deb_t.msecsTo(end_t))));
     qDebug()<< tr("TIME method While : getInteractionPairFromCipList %1").arg(QString::number(deb_t.msecsTo(end_t)));
#endif

  return interResultPairList;
}

//-------------------------------- Datasemp_isThisInteractionPresentInListe ------------------------------------------------------
int C_BDM_DatasempPlugin::Datasemp_isThisInteractionPresentInListe(int cipToExclude, int itCode, const QMap <int, INTER_STRUCT> &cip_interMultiMap, INTER_STRUCT &inter_act)
{   INTER_STRUCT inter;
    QMapIterator< int, INTER_STRUCT> it (cip_interMultiMap);
    while (it.hasNext())
    {   it.next();
        if (it.key() != cipToExclude)   // on ne ne teste pas ce cip car on verifie si il interfere avec les autres et non pas avec lui meme !
           { inter    = it.value();
             if ( inter.m_id_1==itCode )
                { inter_act = inter;
                  return inter.m_cip;
                }
           }
    }
    return 0;
}
//-------------------------------- getAllergiesFromCipList ------------------------------------------------------
/*! \brief return Allergies interactions in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */

QList<C_InterResultPair> C_BDM_DatasempPlugin::getAllergiesFromCipList( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{
 QList<C_InterResultPair>      interResultPairList;
 QList<C_LifeEvent>            lifeEventList = pC_PatientCtx->lifeEventList();
 QMap <QString,QString>        cip_allergMap;
 QMap <QString,C_AllergItem>   allergItem_Map;    // code d'appariement , C_AllergItem
 QStringList                   cip_toTest;
 C_AllergItem                  allergItem;
 //................... tester le CIP ..................................................
 for (int i = 0; i<lifeEventList.size();++i)
     {     QString codePatho     = lifeEventList[i].code();    // piecesList.at(0).trimmed();
           QString codePathoTyp  = lifeEventList[i].codeTyp(); // piecesList.at(1).trimmed();
           QString termPatho     = lifeEventList[i].libelle();
           if (codePathoTyp=="CIP")
              { cip_allergMap[codePatho]=termPatho;
                cip_toTest << codePatho;
              }
     }
 cip_toTest.append( cipList );
 QString s_cip_list = cip_toTest.join("','");

 QString requete = QString ( " SELECT t00.f3 , \n"    // 0  as drug_name
                             "        t00.f2 , \n"    // 1  as cip
                             "        t11.f1 , \n"    // 2  as code_m,
                             "        t58.f0 , \n"    // 3  as code_f,
                             "        t56.f1 , \n"    // 4  as allergie,
                             "        t57.f0 , \n"    // 5  as code_1,
                             "        t57.f1   \n"    // 6  as code_2
                             " FROM   t00      \n"
                             "       ,t11      \n"
                             "       ,t58      \n"
                             "       ,t57      \n"
                             "       ,t56      \n"
                             " WHERE  t00.f0 = t11.f0 \n"
                             " AND    t58.f1 = t11.f1 \n"
                             " AND    ( t57.f0 = t58.f0 or t57.f1 = t58.f0 ) \n"
                             " AND    t56.f0 = t58.f0 \n"
                             " AND    t00.f2 IN ('%1')\n").arg(s_cip_list);
 QSqlQuery query(requete , database() );
 // CGestIni::Param_UpdateToDisk(m_pathAppli+"requete.txt",requete.toLatin1());
 if ( ! query.isActive() )
    {  return interResultPairList;
    }
 //............ appareiller les resultats sur leurs codes ......................
 QString cip = "";
 while (  query.next() )
    { allergItem_Map.insertMulti( query.value(5).toString()+query.value(6).toString()  ,   // code d'appariement
                                  C_AllergItem    ( query.value(1).toString()          ,
                                                    "CIP"                              ,
                                                    CGestIni::Utf8_Query(query, 0)     ,
                                                    CGestIni::Utf8_Query(query, 4)     ,
                                                    query.value(2).toString()          ,
                                                    query.value(3).toString()          ,
                                                    query.value(5).toString()          ,
                                                    query.value(6).toString()
                                                  )
                                 );
    }
 //......... parser toutes la liste des substances declaree comme allergies .......................
 //          et voir si paire
 QList <C_AllergItem> values;
 QMapIterator<QString, C_AllergItem> it( allergItem_Map );
 while ( it.hasNext() )
       { it.next();
         QString code_inter = it.key();                  // code d'appariement
         allergItem         = it.value();
         values             = allergItem_Map.values( code_inter );     // recuperer la liste des items allergie correspondant au code d'appariement
         cip                = allergItem.code();
         if ( values.size()>1 && cip_allergMap.contains( cip ) )       // il faut que ce soit un item des allergie declarees
            { QString drug_name = cip_allergMap[cip];                  // nom de la drogue de l'allergie declaree
              for ( int i = 0; i < values.size(); ++i )                // pour toutes les valeurs correspondant a cette allergie
                  { if ( values[i].code() != cip )                     // ne pas retenir celle du cip allergene
                       { interResultPairList.append( C_InterResultPair( cip               , "CIP",  drug_name            ,
                                                                        values[i].code()  , "CIP",  values[i].drug_name() ,
                                                                        tr("<b>Classe allergique</b> : %1").arg(values[i].libelle()) ,"PATCTXALL", "1"
                                                                      )
                                                   );
                       }  //
                  }
            }
       }
 return interResultPairList;
}
//-------------------------------- getInteractionPhysicoChimiques ------------------------------------------------------
/*! \brief return chimico physiques interactions in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_DatasempPlugin::getInteractionPhysicoChimiques( const QStringList &cipList, C_PatientCtx *pC_PatientCtx)
{
  Q_UNUSED(pC_PatientCtx);
  QList<C_InterResultPair>        interResultPairList;
  QString s_cip_list = cipList.join("','");
  QString requete = QString ( " SELECT              \n"
                      "         t00.f2,     \n"    // 0 cip
                      "         t00.f3,     \n"    // 1 drug name
                      "         t50.f1,     \n"    // 2 title
                      "         t52.f5,     \n"    // 3 memo
                      "         t52.f6      \n"    // 4 code
                      " FROM t50 ,          \n"
                      "      t52 ,          \n"
                      "      t00            \n"
                      " WHERE t50.f0 = t52.f0    \n"
                      " AND   t52.f2 = 1992      \n"
                      " AND   t00.f2 IN ('%1')   \n"
                      " AND   t52.f1 = t00.f0;   \n"
                    ).arg(s_cip_list);

  QSqlQuery query(requete , database() );

  if ( ! query.isActive() )
     {  return interResultPairList;
     }
  while (  query.next() )
     { interResultPairList.append( C_InterResultPair( query.value(0).toString() , "CIP",    CGestIni::Utf8_Query(query, 1),
                                                      query.value(4).toString() , "HISICO", CGestIni::Utf8_Query(query, 2),
                                                      tr("<b>Information</b> : %1").arg(CGestIni::Utf8_Query(query, 3)) ,"HISICO", "1"
                                                    )
                                 );
     }
  return interResultPairList;
}


//-------------------------------- getInteractionRedundancy ------------------------------------------------------
/*! \brief return redundancy composition in a cip list of drugs.
 *  \param cipList QStringList &cipList drugs cip list .
 *  \param pC_PatientCtx  pointer on C_PatientCtx patient context .
 *  \return QList<C_InterResultPair> result interaction pair list
 */
QList<C_InterResultPair> C_BDM_DatasempPlugin::getInteractionRedundancy( const QStringList &edit_cipList, C_PatientCtx *pC_PatientCtx )
{   QList<C_InterResultPair>            interResultPairList;
    QStringList list_cip_to_evaluate;
    QStringList list_cip_fond    = pC_PatientCtx->list_cip_traitement_fond();
    QStringList list_cip_current = pC_PatientCtx->list_cip_traitement_inter();
    //.............. tous les cip idem au traitement de fond ne sont pas evalues ................
    //               regle : on considere que si l'on prescrit un medicament exactement semblable
    //                       a celui du traitement de fond c'est qu'on le renouvelle
    for (int i=0; i<edit_cipList.size(); ++i)    // on ajoute tous ceux de l'ordonnance en cours sans ceux communs avec le ttt de fond
        { QString cip = edit_cipList[i];
          if ( list_cip_fond.indexOf(cip) == -1 ) list_cip_to_evaluate << cip;
        }
    for (int i=0; i<list_cip_fond.size(); ++i)    // on ajoute tous ceux du TTT de fond sans ceux communs a l'ordonnance en cours
        { QString cip = list_cip_fond[i];
          if ( edit_cipList.indexOf(cip) == -1 ) list_cip_to_evaluate << cip;
        }
    for (int i=0; i<list_cip_current.size(); ++i) // on ajoute tous ceux des traitement intercurrents en cours sans ceux communs avec le ttt de fond
        { QString cip = list_cip_current[i];
          if ( list_cip_fond.indexOf(cip) == -1 ) list_cip_to_evaluate << cip;
        }
    if ( list_cip_to_evaluate.length() <= 1 ) return interResultPairList;         // il faut au moins deux produits pour une redondance
    INTER_STRUCT inter;
    QMap      <QString, INTER_STRUCT> subst_MultiMap;          // liste associant key = nom de la substance  data = INTER_STRUCT donnee du produit contenant cette substance
    QMap      <QString, QString>      subst_DistinctMap;       // liste des subtances QMap pour avoir unicite des noms
    QString   substName = "";
    //.......... creer la liste des substances de l'ordonnance ..........................................
    QString s_cip_list = list_cip_to_evaluate.join("','");
    QString requete    = QString (   " SELECT t00.f2 ,         \n"    // 0   as cip
                                     "        t00.f3 ,         \n"    // 1   as drug_name
                                     "        t27.f2           \n"    // 2   as subst_Name
                                     " FROM   t00,             \n"
                                     "        t11,             \n"
                                     "        t27              \n"
                                     " WHERE  t00.f2 IN ('%1') \n"
                                     " AND    t11.f0 = t00.f0  \n"
                                     " AND    t27.f0 = t11.f1  \n"
                                     " ORDER BY        t11.f1  \n"  ).arg(s_cip_list);
    //qDebug()<<requete;
    QSqlQuery query(requete , database() );
    if    ( !query.isActive() ) return interResultPairList;
    while (  query.next() )
       { inter.m_cip      = query.value(0).toInt();
         inter.m_drugName = CGestIni::Utf8_Query(query, 1);
         inter.m_product  = CGestIni::Utf8_Query(query, 2);
         int nb_cip       = list_cip_to_evaluate.count( QString::number(inter.m_cip));
         if ( nb_cip >= 2 )                    // PB si plusieurs fois fois le meme CIP seule une reponse est retournee par la requete donc detecter dans la liste des CIP si ce CIP est pluriel
            { for (int i=0; i<nb_cip; ++i)     //    et rajouter autant de fois le produit (on accepte les clefs multiples sur la substance pour mapper chaque specialite dessus)
                  { subst_MultiMap.insertMulti(inter.m_product, inter);
                  }
            }
         else
            { subst_MultiMap.insertMulti(inter.m_product, inter);          // on accepte les clefs multiples sur la substance pour mapper chaque specialite dessus
            }
         subst_DistinctMap.insert(inter.m_product,inter.m_product);        // on accepte pas les clefs multiples ==> liste de substance unique ordonnee
       }
    //......... parser toutes la liste des substances uniques ordonnees et voir si plusieurs produits repondent .......................
    QMapIterator<QString, QString> it(subst_DistinctMap);
    while (it.hasNext())
          { it.next();
            substName                      = it.value();
            QList<INTER_STRUCT> inter_list = subst_MultiMap.values( substName );
            int  nb  = inter_list.size();
            if ( nb >= 2 )   // une redondance est trouvee car deux CIP avec la meme substance
               { QString title = tr(" <b>%1</b> redondances de subtance possibles ").arg(QString::number(nb));
                 QString avec  = "";
                 for (int i=0; i<inter_list.size(); ++i)
                     { avec += "<br/><b>" + inter_list[i].m_drugName+"</b>";
                     }
                 interResultPairList.append( C_InterResultPair( ""       , "REDON", title,
                                                                ""       , "REDON", substName,
                                                                tr("%1 produits avec la m\303\252me substance <b>%2</b> ont \303\251t\303\251 trouv\303\251s :<br/>").arg(QString::number(nb), substName) + avec + "<br/>","REDONDANCES", QString::number(nb)
                                                              )
                                           );

               }  // if ( inter_list.size() >= 2 )
          } // while (it.hasNext())
    return interResultPairList;
}

//-------------------------------- getInteractionTerrain ------------------------------------------------------
/*! \brief return physiopath interactions from patient and drug.
 *  \param const QString& cip drug cip identification .
 *  \param const QString &age patient age .
 *  \param const QString &sexe patient sexe F-M-I.
 *  \param const QString &poids patient poids .
 *  \param const QString &taille patient size .
 *  \param const QString &amenorrhee in weks
 *  \param const QString &clairance in ml/min
 *  \param int   allaitement in number of days
 *  \param bool sport if true particular attention for doping drugs is required  .
 *  \param bool vigil if true particular attention for drugs which vigilance  alteration is required  .
 *  \param QString drugName (default empty) the drug name.
 *  \return QList<C_InterResultPair> result interaction pair list
 */
 /*
SELECT
       t52.f2 as niveau,
       t1C.f1 as libelleNiveau,
       t00.f3 as drug_name,
       t50.f1 AS CI_Vidal,
       t4N.f2 AS CIM_X,
       t4N.f3 AS codeCIM_X
FROM t52
INNER JOIN t4B ON t52.f1 = t4B.f0
INNER JOIN t1C ON t52.f2 = t1C.f0  -- libelle
INNER JOIN t50 ON t52.f0 = t50.f0,
t55,
t4N,
t00
WHERE t00.f0 = t4B.f0
-- AND t00.f3 LIKE 'ASPEGIC 1000%'
-- AND t00.f2='3514685'
AND t55.f1 = t4N.f0
AND t55.f0 = t50.f0
AND t52.f2 IN ('2999','2991','2990','2996')
-- AND t4N.f3 like 'K27%' -- 'K27'
limit 0,3000

 SELECT
 t00.f2          as cip,
 t2A.f0          as codeTerrain,
 t50.f1          as libelle_terrain,
 t2G.f2          as borneInf,
 t2G.f1          as borneSup,
 t2G.f6          as All_SexeFM,
 t52.f2          as niveau,
 t45.f5          as document,
 t1C.f1          as libelle_niveau,
 t2G.f3          as unitCode,
 t4B.fG          as dopant,
 t4B.fD          as vigilance
 FROM t00,t2G
 INNER JOIN t52 ON t2G.f4 = t52.f0
 INNER JOIN t4B ON t52.f1 = t4B.f0
 INNER JOIN t2A ON t2G.f0 = t2A.f0
 INNER JOIN t1C ON t52.f2 = t1C.f0
 INNER JOIN t50 ON t52.f0 = t50.f0,
 t45
 WHERE t00.f0 = t4B.f0
 AND   t45.f0 = t52.f6
 AND   t00.f3 like '%ABILIFY %'
 AND   t2A.f0 = 1
-- AND   t00.f2 = '%1' -- cip
 limit 0,100

select t00.f2 as cip,
       t00.f3 as drugName,
       t4B.fg as dopant,
       t4B.fd as codeVigil,
       t5A.f1 as libelleVigil,
       t5A.f3 as niveauVigil
from   t4B,
       t00,
       t5A
where  t4B.f0 = t00.f0
and    t4B.fD = t5A.f0
limit 0,200
 */
 QList<C_InterResultPair>    C_BDM_DatasempPlugin::getInteractionAgeSexePoidsSportVigil( const QString &cip,
                                                                                         const QString &_age ,
                                                                                         const QString &sexe,
                                                                                         const QString &_poids,
                                                                                         const QString &taille,
                                                                                         const QString &_amenorrhee,
                                                                                         const QString &_clairance,
                                                                                         int   allaitement,
                                                                                         bool  sport       /* = true */,
                                                                                         bool  vigil       /* = true */,
                                                                                         QString drugName  /*= "" */ )
 {
     Q_UNUSED (taille);
     QList<C_InterResultPair>        interResultPairList;
     char c_sexe     =  sexe.toLatin1()[0]|32;
     double d_age    =   _age.toDouble();
     int poids       = _poids.toInt();
     int amenorrhee  = _amenorrhee.toInt();
     int clairance   = _clairance.toInt(); if (clairance<0) clairance = 100;   // -1 si non definie
     QString requete = QString (
                                " SELECT t2A.f0 ,                    \n"   // 0   as codeTerrain
                                " t2G.f1,                            \n"   // 1   as borneInf
                                " t2G.f2,                            \n"   // 2   as borneSup
                                " t2G.f6,                            \n"   // 3   as All_SexeFM
                                " t52.f2,                            \n"   // 4   as niveau
                                " t52.f5,                            \n"   // 5   as document
                                " t50.f1,                            \n"   // 6   as libelle_terrain
                                " t1C.f1,                            \n"   // 7   as libelle_niveau
                                " t2G.f3,                            \n"   // 8   as unitCode
                                " t4B.fG,                            \n"   // 9   as dopant
                                " t4B.fD,                            \n"   // 10  as vigilance
                                " t4B.fE,                            \n"   // 11  as commentaire vigilance
                                " t52.f6                             \n"   // 12  as code ref biblio
                                " FROM t00,t2G                       \n"
                                " INNER JOIN t52 ON t2G.f4 = t52.f0  \n"
                                " INNER JOIN t4B ON t52.f1 = t4B.f0  \n"
                                " INNER JOIN t2A ON t2G.f0 = t2A.f0  \n"
                                " INNER JOIN t1C ON t52.f2 = t1C.f0  \n"
                                " INNER JOIN t50 ON t52.f0 = t50.f0  \n"
                                " WHERE t00.f0 = t4B.f0              \n"
                                " AND   t00.f2 = '%1'                \n"
                                ).arg(cip);
      // qDebug() << requete;
      QSqlQuery query(requete , database() );
      if    ( !query.isActive() ) return interResultPairList;
      C_InterResultPair insuffRenaleInterResultPair;
      C_InterResultPair ageInterResultPair;
      int lastIRLevel   = -1;
      int lastAgeLevel  = -1;   // 3600 mois = 300 ans
      int sexAlertNoSex =  0;
      QString s_bSup;
      QString s_bInf;
      bool dopeIsNotEvaluated = true;
      bool vigiIsNotEvaluated = true;
      while (  query.next() )
            {  int codeTerrain = query.value(0).toInt();
               s_bInf          = query.value(1).toString();
               s_bSup          = query.value(2).toString();
               int borneInf    = s_bInf.toInt();
               int borneSup    = s_bSup.toInt();
               QString SexeAll = query.value(3).toString();
               int level       = query.value(4).toInt();
               QString texte   = query.value(5).toString(); // CGestIni::Utf8_Query(query, 5);
               int unitCode    = query.value(8).toInt();
               QString title   = "";
               QByteArray ba   = query.value(9).toByteArray();   // query.value(9).toBool()  fonctionne pas sous mac ??!!
               //.........la ref biblio peut pas etre liee au dessus car limite les sorties de facon anormale)...........
               QString biblio_libelle  = "";
               QString biblio_ref      = "";
               QString biblio_date     = "";
               QString biblio_memo     = "";
               requete =    " SELECT          \n"
                            "       t45.f1,   \n"   // 0  as libelle_biblio,
                            "       t45.f5,   \n"   // 1  as lien_biblio
                            "       t45.fC    \n"   // 2  as date_biblio
                            " FROM  t45       \n"   // table references
                            " WHERE t45.f0 ='"+query.value(12).toString()+"' \n";   // 12  as code ref biblio
               QSqlQuery query_b(requete , database() );
               if    ( query_b.isActive() )
                     { while (  query_b.next() )
                             {  biblio_libelle  = CGestIni::Utf8_Query(query_b, 0);
                                biblio_ref      = query_b.value(1).toString();
                                biblio_date     = query_b.value(2).toString();
                             }

                       if ( biblio_ref.startsWith("http://") )  biblio_ref = QString("<a href=\"%1\">%1</a>")         .arg(biblio_ref);
                       else                                     biblio_ref = QString("<a href=\"ref_mono:%1\">%1</a>").arg(biblio_ref);
                       if ( biblio_date.length() )
                          { biblio_date = QDateTime::fromString(biblio_date,Qt::ISODate).toString("dd-MM-yyyy");
                          }
                       else
                          { biblio_date = "date indisponible";
                          }
                       biblio_memo = tr("<b>R\303\251f\303\251rence dat\303\251e du</b> : %1<br/>"   // biblio date
                                        "<b>R\303\251f\303\251rence</b> : %2<br/>"                   // biblio libelle
                                        "%3<br/>"                                                    // biblio reference
                                       ).arg(biblio_date,biblio_libelle,biblio_ref);
                     }
               //......... evaluations ..........
               if (dopeIsNotEvaluated && ba.length() && ba[0])
                  { title = tr(" Produit pouvant \303\252tre positif lors des contr\303\264les anti-dopage");
                    interResultPairList.append( C_InterResultPair( ""      , "DOPE", title,
                                                                   cip     , "CIP", drugName,
                                                                   tr("Sport : <br/>") +title + "<br/>"+ biblio_memo
                                                                   ,"IT_DOPE", sport==true?"4":"0"
                                                                 )
                                               );
                    dopeIsNotEvaluated = false;                        // c'est fait (ne pas le faire deux fois)

                  }
               int niveauVigi = query.value(10).toInt();
               if (vigiIsNotEvaluated && niveauVigi>1 && niveauVigi<= 5)
                  { if (niveauVigi==5)niveauVigi=1;
                    title = tr(" Produit alt\303\251rant la vigilance (niveau %1)").arg(QString::number(niveauVigi-1));   // le niveau Vidal est de +1 pararapport au niveau HAS (celui a afficher)
                    interResultPairList.append( C_InterResultPair( ""     , "VIGIL", tr("Effets sur l'aptitude \303\240 conduire des v\303\251hicules et \303\240 utiliser des machines"),
                                                                   cip    , "CIP"  , drugName,
                                                                   tr("Vigilance : <br/>") +title + "<br/>"+CGestIni::Utf8_Query(query, 11)+ "<br/>"+ biblio_memo
                                                                   ,vigil==true?"IT_VIG":"IT_W_VIG", QString::number(niveauVigi)
                                                                 )
                                               );
                    vigiIsNotEvaluated = false;    // c'est fait (ne pas le faire deux fois)
                  }
                       title   = CGestIni::Utf8_Query(query, 6);
               int api_level   = 0;
               switch (codeTerrain)
                      {
                       /* a table t2G contient les donnees correspondantes a celle du lexique (t50) :
                          Dans le cas ci-dessus nous avons :
                          Precaution d'emploi pour enfant de moins de 15 ans correspond aux valeurs 0 (t2G.f1) et 15 (t2G.f2), si l'age saisi est entre 0 et 15  il faut afficher une alerte.
                          Mise en garde Reserve a l'adulte correspond aux valeurs 15(t2G.f1) et 0(t2G.f2), si l'age saisi est entre 0 et 15  il faut afficher une alerte.
                          Pour les valeurs de t52.f2 (2990 [Precaution d'emploi], 2996 [Contre-indication relative] et 2999 [contre-indication absolue]), les valeurs dans t2G sont des valeurs exclusives
                          si une donnee  correspond a un EPP*, il faut afficher une alerte.
                          Pour la valeur de   t52.f2 (2988 [Mise en garde]) les valeurs dans t2G sont inclusives  si une donnee  ne correspond pas a un EPP*,  il faut afficher une alerte de 'reserve'.
                          Pour la valeur de   t52.f2 (2994 [Indication]), les valeurs dans t2G sont inclusives comme il peut exister plusieurs indications sans preconisation d'age, ceci est donne a titre 'indicatif'.
                          Il est peut-\303\252tre judicieux de n'afficher qu?une seule alerte pour l'age, et d'indiquer qu'il y a une mise en garde et une precaution d'emploi.
                       */
                       case 1:  // age
                             {
                                double d_borneSup = s_bSup.toDouble();
                                double d_borneInf = s_bInf.toDouble();
                                if (unitCode==44)   //  44-ans 43-mois 42 semaines
                                   { d_borneInf *= 12.0;
                                     d_borneSup *= 12.0;
                                   }
                                if (d_borneSup==0) d_borneSup = 3600;
                                //............. traiter le cas des inclusifs exclusifs ...........
                                int isAlert             = 0;
                                QString strAlert        = "";
                                QString ageToDisplaySup = "";
                                QString ageToDisplayInf = "";
                                QString ageToDisplay    = "";
                                if (d_borneSup>=24) ageToDisplaySup = QString::number(d_borneSup/12.0,'f',2)  + " ans";
                                else                ageToDisplaySup = QString::number(d_borneSup,'f',0)       + " mois";
                                if (d_borneInf>=24) ageToDisplayInf = QString::number(d_borneInf/12.0,'f',2)  + " ans";
                                else                ageToDisplayInf = QString::number(d_borneInf,'f',0)       + " mois";
                                if (d_age>=24)      ageToDisplay    = QString::number(d_age/12.0,'f',2)       + " ans";
                                else                ageToDisplay    = QString::number(d_age,'f',0)            + " mois";
                                //........................................................
                                // COMPRENDRE : deux types d'alertes peuvent etre affichees :
                                //     2899  si l'age est en dehors des clous on affiche l'alerte (detection d'une non compatibilite du medoc avec l'age)
                                //     2990  si l'age est dans les clous on affiche l'alerte      (detection d'une information a aficher +/- importante pour cet age)
                                if ( alert_If_IN_Limits( level ) )
                                   { isAlert  = (d_age >=  d_borneInf && d_age < d_borneSup);   // d_borneInf = 0    d_borneSup = 15 les valeurs dans t2G sont exclusives
                                     strAlert = tr("Information li\303\251e \303\240 l'\303\242ge de <b>%1</b> : <br/>").arg(ageToDisplay);
                                   }
                                else
                                   { isAlert  =  !(d_age >= d_borneInf && d_age < d_borneSup);   // d_borneInf = 15   d_borneSup = 0  les valeurs dans t2G sont inclusives
                                     strAlert = tr("Age du patient <b>%1</b> en dehors des zones de compatibilit\303\251 <b>%2</b> et <b>%3</b><br/>").arg(ageToDisplay, ageToDisplayInf, ageToDisplaySup);
                                   }
                                api_level = Datasemp_LevelToLevelInt(level);
                                if ( api_level > lastAgeLevel && isAlert) // on garde la raison la plus grave
                                   { ageInterResultPair.setDatas( _age   , "AGE", title,
                                                                  cip    , "CIP", drugName,
                                                                  strAlert + texte +"<br/>"+ biblio_memo
                                                                  ,"IT_YEARS", Datasemp_LevelToLevelStr(level)
                                                                );
                                    lastAgeLevel = api_level;
                                  }
                                break;
                             }  // end case
                       case 2:  // poids
                             {  if (borneSup==0) borneSup = 500;   // 500 kg
                                //............. traiter le cas des inclusifs exclusifs ...........
                                int isAlert      = 0;
                                QString strAlert = "";
                                if    (level==2988)
                                      { isAlert  = ! (poids >= borneInf && poids < borneSup);   // borneInf = 15   d_borneSup = 0  les valeurs dans t2G sont inclusives
                                        strAlert = tr("Poids du patient <b>%1 kg</b> en dehors des zones de compatibilit\303\251 <b>%2</b> et <b>%3</b><br/>").arg(_poids, QString::number(borneInf), QString::number(borneSup));
                                      }
                                else
                                      { isAlert  =  (poids >  borneInf && poids < borneSup);   // borneInf = 0    d_borneSup = 15 les valeurs dans t2G sont exclusives
                                        strAlert = tr("Information li\303\251e au poids du patient <b>%1 kg</b><br/>").arg(_poids);
                                      }

                                if (isAlert)
                                   { interResultPairList.append( C_InterResultPair( _poids  , "POIDS", title,
                                                                                    cip     , "CIP", drugName,
                                                                                    strAlert + texte +"<br/>"+ biblio_memo
                                                                                    ,"IT_POIDS",      Datasemp_LevelToLevelStr(level)
                                                                                  )
                                                               );
                                   }
                                break;
                             } // end case
                      // COMPRENDRE : deux types d'alertes peuvent etre affichees :
                      //     2899  si l'age est en dehors des clous on affiche l'alerte (detection d'une non compatibilite du medoc avec l'age)
                      //     2990  si l'age est dans les clous on affiche l'alerte      (detection d'une information a aficher +/- importante pour cet age)

                      case 3:  // Gossesse 2 types d'alertes: une pour grossesse confirmee et une pour patiente en age de procreer
                            { if (borneSup==0) borneSup = 500;   // 500 semaines d'amenorrhee
                              //............. traiter le cas des inclusifs exclusifs ...........
                              QString s_amenorrhee = QString::number(amenorrhee);
                              int isAlert      = 0;
                              QString strAlert = "";   // alertIfOutLimits(int level)

                              if ( ! alert_If_IN_Limits( level ) )
                                    { isAlert  = ! (amenorrhee >= borneInf && amenorrhee < borneSup);   // borneInf = 15   d_borneSup = 0  les valeurs dans t2G sont inclusives
                                      if (isAlert)
                                         { strAlert = tr("Age de grossesse de la patiente <b>%1 semaine(s) d'am\303\251norrh\303\251e</b>"
                                                         " en dehors des zones de compatibilit\303\251 <b>%2</b> et <b>%3</b><br/>").arg(s_amenorrhee, QString::number(borneInf), QString::number(borneSup));
                                         }
                                      else if ( c_sexe=='f' && d_age>120 && d_age<600)    // age de procreer
                                         { strAlert = tr("Information li\303\251e \303\240 l'\303\242ge <b>%1 ans compatible avec la procr\303\251ation</b><br/>"
                                                         "(voir monographie)<br/>").arg(QString::number(d_age/12,'f',0));
                                          level    = 2990;
                                           isAlert = 1;
                                         }
                                    }
                              else
                                    { isAlert  =   (amenorrhee >  borneInf && amenorrhee < borneSup);   // borneInf = 0    d_borneSup = 15 les valeurs dans t2G sont exclusives
                                      if (isAlert)
                                         { strAlert = tr("Information li\303\251e \303\240 l'\303\242ge <b>%1 semaine(s) d'am\303\251norrh\303\251e </b> de la grossesse <br/>"
                                                        "bornes : <b>%2</b> et <b>%3</b><br/>").arg(s_amenorrhee,QString::number(borneInf), QString::number(borneSup));
                                         }
                                      else if ( c_sexe=='f' && d_age>120 && d_age<600)    // age de procreer
                                         { strAlert = tr("Information li\303\251e \303\240 l'\303\242ge <b>%1 ans compatible avec la procr\303\251ation</b><br/>"
                                                         "(voir monographie)<br/>").arg(QString::number(d_age/12,'f',0));
                                           level   = 2990;
                                           isAlert = 1;
                                         }
                                    }

                              if (isAlert)
                                 { interResultPairList.append( C_InterResultPair( "Z32.1"   , "CIM", title,
                                                                                  cip       , "CIP", drugName,
                                                                                  strAlert + texte +"<br/>"+ biblio_memo
                                                                                  ,"IT_GRO",      Datasemp_LevelToLevelStr(level)
                                                                                )
                                                             );
                                 }
                               break;
                            }  // end case
                      case 4:  // allaitement (bornes sans signification)
                            { if (allaitement)
                                 { interResultPairList.append( C_InterResultPair( "Z39.1" , "CIM", title,
                                                                                   cip    , "CIP", drugName,
                                                                                   tr("Allaitement : <br/>")+ texte + "<br/>" + biblio_memo
                                                                                  ,"IT_ALL",      Datasemp_LevelToLevelStr(level)
                                                                                 )
                                                              );
                                 }
                               break;
                            }  // end case
                      case 5:  // insuffisance renale
                            { if (borneSup==0) borneSup = 500;   // 500 ml/mn
                              //............. traiter le cas des inclusifs exclusifs ...........
                              int isAlert      = 0;
                              QString strAlert = "";
                              if    (level==2988)
                                    { isAlert  = ! (clairance >= borneInf && clairance < borneSup);   // borneInf = 15   d_borneSup = 0  les valeurs dans t2G sont inclusives
                                      strAlert = tr("Clairance du patient <b>%1 ml/min</b> en dehors des zones de compatibilit\303\251 <b>%2</b> et <b>%3</b><br/>").arg(_clairance, QString::number(borneInf), QString::number(borneSup));
                                    }
                              else
                                    {
                                      isAlert  =   (clairance >  borneInf && clairance < borneSup);   // borneInf = 0    d_borneSup = 15 les valeurs dans t2G sont exclusives
                                      strAlert = tr("Information li\303\251e \303\240 la Clairance du patient <b>%1 ml/min</b><br/>").arg(_clairance);
                                    }
                              api_level = Datasemp_LevelToLevelInt(level);
                              if (api_level > lastIRLevel &&  isAlert)    // on garde la raison la plus grave
                                 { insuffRenaleInterResultPair.setDatas(  "N19"     , "CIM", title,
                                                                          cip       , "CIP", drugName,
                                                                          strAlert + texte +"<br/>"+ biblio_memo
                                                                          ,"IT_CIM",      Datasemp_LevelToLevelStr(level)
                                                                       );
                                   lastIRLevel = api_level;
                                 }
                               break;
                            }  // end case
                      case 6:  // sexe    ATTENION SexeAll (sexe concerne par l'information) peut tres bien etre a 'F' exemple: CIALIS pour signaler qu'il n'est pas adapte a la femme
                            { int isAlert          = 0;
                              QString strAlert     = "";
                              if    (sexe.length()==0)    // une information de type sexe est requise mais non disponible
                                    { if (sexAlertNoSex==0)  // information necessaire qu'une fois
                                         { isAlert = 1;
                                           title   =  tr("L'information sur le sexe du patient est requise, mais non renseign\303\251e. Veillez \303\240 bien saisir le sexe.");
                                           texte   =  tr("L'information sur le sexe du patient est requise, mais non renseign\303\251e. Veillez \303\240 bien rentrer le sexe.");
                                           sexAlertNoSex = 1;
                                         }
                                    }
                              else if    (level==2988)
                                    { isAlert  =  !(SexeAll == sexe);   // borneInf = 15   d_borneSup = 0  les valeurs dans t2G sont inclusives
                                      title    =  tr("Ce produit est r\303\251serv\303\251 aux patients de sexe %1<br/>%2").arg((SexeAll[0].toLatin1()=='F')?tr("F\303\251minin"):tr("Masculin"), title);
                                      texte    =  tr("Ce produit est r\303\251serv\303\251 aux patients de sexe <b>%1</b><br/>").arg((SexeAll[0].toLatin1()=='F')?tr("F\303\251minin"):tr("Masculin"));
                                    }
                              else
                                    { isAlert  =  (SexeAll == sexe);   // borneInf = 0    d_borneSup = 15 les valeurs dans t2G sont exclusives
                                      strAlert =  tr("Information li\303\251e  avec le sexe <b>%1</b> du patient <br/>").arg((SexeAll[0].toLatin1()=='F')?tr("F\303\251minin"):tr("Masculin"));
                                    }
                              api_level = Datasemp_LevelToLevelInt(level);
                              if ( isAlert )
                                 { interResultPairList.append( C_InterResultPair(  " "    , "", title,
                                                                                   cip     , "CIP", drugName,
                                                                                   tr("Sexe : <br/>")+title+"<br/>"+strAlert+ texte + "<br/>" + biblio_memo
                                                                                   ,"IT_SEX",      Datasemp_LevelToLevelStr(level)
                                                                                )
                                                             );
                                 }
                               break;
                            }  // end case
                      } // switch (codeTerrain)
            } // end while (  query.next() )
      if (insuffRenaleInterResultPair.Code1()=="N19")    interResultPairList.append(insuffRenaleInterResultPair);   // si contient la valeur "N19" c'est qu'il a ete initialise
      if (ageInterResultPair.CodeTyp1()=="AGE")          interResultPairList.append(ageInterResultPair);            // si contient la valeur "AGE" c'est qu'il a ete initialise
      return interResultPairList;
 }
 //-------------------------------- alert_If_IN_Limits ------------------------------------------------------
 /* a table t2G contient les donnees correspondantes a celle du lexique (t50) :
    Dans le cas ci-dessus nous avons :
    Precaution d'emploi pour enfant de moins de 15 ans correspond aux valeurs 0 (t2G.f1) et 15 (t2G.f2), si l'age saisi est entre 0 et 15  il faut afficher une alerte.
    Mise en garde Reserve a l'adulte correspond aux valeurs 15(t2G.f1) et 0(t2G.f2), si l'age saisi est entre 0 et 15  il faut afficher une alerte.
    Pour les valeurs de t52.f2 (2990 [Precaution d'emploi], 2996 [Contre-indication relative] et 2999 [contre-indication absolue]), les valeurs dans t2G sont des valeurs exclusives
    si une donnee  correspond a un EPP*, il faut afficher une alerte.
    Pour la valeur de   t52.f2 (2988 [Mise en garde]) les valeurs dans t2G sont inclusives  si une donnee  ne correspond pas a un EPP*,  il faut afficher une alerte de 'reserve'.
    Pour la valeur de   t52.f2 (2994 [Indication]), les valeurs dans t2G sont inclusives comme il peut exister plusieurs indications sans preconisation d'age, ceci est donne a titre 'indicatif'.
    Il est peut-\303\252tre judicieux de n'afficher qu?une seule alerte pour l'age, et d'indiquer qu'il y a une mise en garde et une precaution d'emploi.
 */

 bool  C_BDM_DatasempPlugin::alert_If_IN_Limits(int level)
 {   switch (level)
     {  case 2994:
        case 2988:    // sortir une alerte si pas dans les bornes
          return false;
        case 2999:    // sortir une alerte si dans les bornes
        case 2996:
        case 2990:
           return true;
     }
    return false;
 }

 //-------------------------------- nextATC_Code ------------------------------------------------------
 /*
 Table 42 : Dictionnaire des classes ATC
 Classification OMS des produits, par intentions th\303\251rapeutiques.
         Code	Description	Type	Taille	Relation
 1  	 0	Code classe ATC
 Code structur\303\251, traduisant une arborescence.	[9] Texte longueur variable	 7
         1	Intitul\303\251 anglais
 Intitul\303\251 de la classe en anglais (OMS)	[9] Texte longueur variable	 127
         2	Intitul\303\251 fran\303\247ais
 Intitul\303\251 de la classe en fran\303\247ais	[9] Texte longueur variable	 255
 */
int  C_BDM_DatasempPlugin::nextATC_Code(QString &codeAtc, QString &libelle)
{
    if (codeAtc.length()==0 && libelle.length()==0)    // premiere fois, apres il devrait contenir le retour SQL
       {
        m_position = 0;
        if (!database().isOpen() && database().open()== false)
           { outMessage( tr("ERREUR : C_BDM_DatasempPlugin::nextATC_Code() database can not be open"), __FILE__, __LINE__);  return 0;
           }
        QString     requete  =" SELECT f0, f2 FROM t42 ORDER BY f0";
        m_pQSqlQuery->exec(requete);
        if ( ! m_pQSqlQuery->isActive() )
           { outSQL_error(*m_pQSqlQuery, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::nextATC_Code()</b></font>"), requete, __FILE__, __LINE__  );
             return -1;
           }
       }
    if (m_pQSqlQuery->next())
       {  ++m_position;
          codeAtc     = m_pQSqlQuery->value(0).toString();
          libelle     = CGestIni::Utf8_Query(m_pQSqlQuery, 1);
          return m_position;
       }
    m_position = 0;
    return 0;
}
//-------------------------------- drugListMakeSQLFilter ------------------------------------------------------
/*
 SELECT t00.f2 as Cip,
        t00.f3 as DrugName,
        t25.f1 as natureName,
        t4B.fA as ATC,
        -- t34.f1 as disponibilite,
        t63.fE as hosto,          -- == HOSTO ->1   != HOSTO --> 0
        t01.f1 as Prix
 FROM  t00
      ,t4B
      ,t25
      ,t02
 --     ,t34
      ,t63
      ,t01
 WHERE  t02.f0  = t00.f1
 AND    t02.f3  = '0'      -- doit etre disponible
 AND    t4B.fA != 'NULL'   -- si c'est un medoc faut un code ATC
 AND    t00.f0  = t4B.f0
 AND    t25.f0  = t4B.f1
 -- AND    t34.f0  = t02.f3
 AND    t00.f1  = t63.f0
 AND    t01.f0  = t00.f1
 -- AND    t63.fE  = "0"      -- == HOSTO ->1   != HOSTO --> 0
 ORDER  by t01.f1
 LIMIT 0,30000

SELECT UPPER(t27.f2) as SubName,
       t00.f3        as drugName,
       t00.f2        as cip,
       t07.f4        as qu,
       t24.f5        as unit,
       t63.fE        as hosto,
       t01.f1        as Prix,
       t4B.fA        as atc
FROM  t07,
      t27,
      t08,
      t00,
      t24,
      t02,
      t4B,
      t01,
      t63
WHERE t07.f0 = t00.f0 -- t07.f0 = '202489'
AND   t00.f0 = t4B.f0
AND   t4B.fA IS NOT NULL
-- AND   t4B.fA IS NULL
AND   t00.f1 = t63.f0
AND   t01.f0 = t00.f1
AND   t02.f0 = t00.f1
AND   t02.f3 = '0'
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t07.f3 = '1'
AND   t08.f0 = t00.f0 -- '202489'       -- COLCHIMAX
AND   t24.f0 = t07.f5
-- AND   t00.f3 like '%AUGMENTIN%'
ORDER BY t00.f3
 */
QString  C_BDM_DatasempPlugin::drugListMakeSQLFilter()
{   //.......... cette requete elimine les produits sans composition ............
    //           non disponibles et sans code ATC
    /*
    return   " FROM  t07                  \n"
          // "      ,t27                  \n"
             "      ,t08                  \n"
             "      ,t00                  \n"
             "      ,t24                  \n"
             "      ,t02                  \n"
             "      ,t4B                  \n"
             "      ,t01                  \n"
             " WHERE t07.f0 = t00.f0      \n"
             " AND   t00.f0 = t4B.f0      \n"
             " AND   t4B.fA IS NOT NULL   \n" // code ATC non null   -- AND   t4B.fA IS NULL
             " AND   t01.f0 = t00.f1      \n"
             " AND   t02.f0 = t00.f1      \n"
          //   " AND   t02.f3 = '0'         \n" // disponible
          // " AND   t27.f0 = t07.f2      \n"
             " AND   t07.f1 = t08.f1      \n"
             " AND   t08.f1 = '0'         \n" // Numero de section, code section
             " AND   t07.f3 = '1'         \n" // ordre substance 1 (pour limiter a une reponse si plusieurs composants)
             " AND   t08.f0 = t00.f0      \n"
             " AND   t24.f0 = t07.f5      \n"
             " ORDER BY t00.f3            \n";
     */
    return   " FROM  t00                  \n"
             "      ,t02                  \n"
             "      ,t4B                  \n"
             "      ,t01                  \n"
             " WHERE  t00.f0 = t4B.f0     \n"
          //   " AND   t4B.fA IS NOT NULL   \n" // code ATC non null   -- AND   t4B.fA IS NULL
             " AND   t01.f0 = t00.f1      \n"
             " AND   t02.f0 = t00.f1      \n"
             " AND   t00.fK LIKE '1%'     \n"   // famille datasemp K = 1 = Sp\303\251cialit\303\251s ; 2 = Di\303\251t\303\251tiques ; 3 = V\303\251t\303\251rinaires ; 4 = Parapharmacie ; 5 = Accessoires ; 6 = Divers (herboristerie, etc.) ; 7 = Hom\303\251opathie
             " AND   t02.f3 = '0'         \n"   // disponible
          //   " AND   t4b.f7 like '1%'     \n"
             " ORDER BY t00.f3            \n";
}

//-------------------------------- drugsList_Count ------------------------------------------------------
long  C_BDM_DatasempPlugin::drugsList_Count()
{   m_position = 0;
    if (!database().isOpen() && database().open()== false)
       {outMessage( tr("ERREUR : C_BDM_DatasempPlugin::drugsList_Count() database can not be open"), __FILE__, __LINE__);  return 0;
       }
    QString requete  =
            " SELECT COUNT(*)            \n"
            + drugListMakeSQLFilter();

    QSqlQuery query(requete , database() );
    outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::drugsList_Count() </b></font>"), requete, __FILE__, __LINE__  );
    if (query.isActive() && query.next())
       {return (long) query.value(0).toDouble();
       }
    return 0;
}

//-------------------------------- getDrugList_Start ------------------------------------------------------
int C_BDM_DatasempPlugin::getDrugList_Start()
{   m_position = 0;
    if (!database().isOpen() && database().open()== false)
       {outMessage( tr("ERREUR : C_BDM_DatasempPlugin::drugsList_Count() database can not be open"), __FILE__, __LINE__);  return 0;
       }
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    m_pQSqlQuery->exec( "ALTER TABLE `DatasempTest`.`t00` ADD INDEX `a2prim` (`f2`)" );    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00` ADD INDEX `a2prim` (`f2`) ", "", __FILE__, __LINE__);  // index ultra-critique sur le cip
    m_pQSqlQuery->exec( "ALTER TABLE `DatasempTest`.`t00` ADD INDEX `a3prim` (`f0`)" );    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00` ADD INDEX `a3prim` (`f0`) ", "", __FILE__, __LINE__);// index ultra-critique pour la relation t4B.f0 <--> t00.f0
    m_pQSqlQuery->exec( "ALTER TABLE `DatasempTest`.`t3C` ADD INDEX `a1prim` (`f1`)" );    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t3C` ADD INDEX `a1prim` (`f1`) ", "", __FILE__, __LINE__);// index ultra-critique pour la relation t4B.f0 <--> t3C.f1

    m_pQSqlQuery->exec( "ALTER TABLE `t00` ADD INDEX f3 (f3)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00  ADD INDEX f3", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t00` ADD INDEX f4 (f4)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00  ADD INDEX f4", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t00` ADD INDEX fA (fA)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00` ADD INDEX fA", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t00` ADD INDEX fB (fB)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00` ADD INDEX fB", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t00` ADD INDEX fD (fD)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00` ADD INDEX fD", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t00` ADD INDEX fK (fK)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00` ADD INDEX fK", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t02` ADD INDEX f3 (f3)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00  ADD INDEX f3", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t2C` ADD UNIQUE f1 (f1)" );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t02` ADD INDEX f3", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t38` ADD INDEX f0 (f0)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t38` ADD INDEX f0", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t38` ADD INDEX f2 (f2)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t38` ADD INDEX f2", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t3F` ADD INDEX f0 (f0)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t3F` ADD INDEX f0", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t3F` ADD INDEX f1 (f1)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t00  ADD INDEX f3", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t4B` ADD INDEX f4 (f4)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t4B` ADD INDEX f4", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t4C` ADD INDEX f0 (f0)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t4C` ADD INDEX f0", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t4C` ADD INDEX f1 (f1)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t4C` ADD INDEX f1", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t4C` ADD INDEX f2 (f2)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t4C` ADD INDEX f2", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t4D` ADD INDEX f0 (f0)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t4D` ADD INDEX f0", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t4D` ADD INDEX f1 (f1)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t4D` ADD INDEX f1", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t5C` ADD INDEX f4 (f4)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t5C` ADD INDEX f4", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t5D` ADD INDEX f0 (f0)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t5D` ADD INDEX f0", "", __FILE__, __LINE__);
    m_pQSqlQuery->exec( "ALTER TABLE `t5D` ADD INDEX f1 (f1)"  );  outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start() t5D` ADD INDEX f1", "", __FILE__, __LINE__);

    //.......... cette requete elimine les produits sans composition ............
    //           non disponibles sans code ATC
    QString requete  =
            " SELECT                     \n"
            " t00.f0,                    \n"    // 0 as cpg,
            " t00.f2,                    \n"    // 1 as cip,
            " t00.f3,                    \n"    // 2 as drugName,
            " t4B.fA,                    \n"    // 3 as atc,
            " t01.f3,                    \n"    // 4 as Prix
            " t02.f2,                    \n"    // 5 as 4/ville    2/hosto
            " t00.f8                     \n"    // 6 nbr UP (unite de prise)
            + drugListMakeSQLFilter();
   // CGestIni::Param_UpdateToDisk("/home/ro/requete.sql",requete);
    m_pQSqlQuery->exec( requete );
    outSQL_error( *m_pQSqlQuery, "ERREUR  : C_BDM_DatasempPlugin::getDrugList_Start()", requete, __FILE__, __LINE__);
    QApplication::restoreOverrideCursor();
    if ( !m_pQSqlQuery->isActive() ) return 0;
    else                             return 1;
}

//-------------------------------- convertPrixDecimalToCts ------------------------------------------------------
QString  C_BDM_DatasempPlugin::convertPrixDecimalToCts(const QString &prix)
{QString  int_part = "";
 QString  dec_part = "";
 int             i = 0;
 int           len = prix.length();
 int           pos = prix.indexOf('.');
 if (pos==-1)  pos = prix.indexOf(',');
 if (pos==-1)                    // pas de partie d\303\251cimale exple 0012  12 faut rajouter deux zeros
    { int_part = prix;
      for ( i=0; i<len; ++i)   // enlever les zeros non significatifs de debut
          { if (int_part.toLatin1()[i]!='0')
               {break;
               }
          }
      int_part = int_part.mid(i);       // si i==len cas du "0000000" alors ret.mid(i) --> "" donc ok
      int_part += "00";
      return int_part;
    }
  else                       // il y a partie d\303\251cimale exple 0012.25  12.1 faut ajuster la partie decimale a deux chiffres
    { int_part = prix.left(pos);
      for ( i=0; i<len; ++i)   // enlever les zeros non significatifs de debut
          { if (int_part.toLatin1()[i]!='0')
               {break;
               }
          }
      int_part = int_part.mid(i);         // si i==len cas du "0000000" alors ret.mid(i) --> "" donc ok
      dec_part = prix.mid(pos+1) + "00";  // on y rajoute deux zeros quite a les couper apres
      dec_part = dec_part.left(2);        // on garde deux chiffres pour la partie decimale
      return int_part + dec_part;
    }
}

//-------------------------------- getDrugList_NextRecord ------------------------------------------------------
int  C_BDM_DatasempPlugin::getDrugList_NextRecord(C_BDM_DrugListRecord &rC_BDM_DrugListRecord)
{
        QStringList dciList;
        QString     code_cpg  = "";
        QString     libelle   = "";
        QString     cip_code  = "";
        QString     nb_UP     = "";
        QString     atc_code  = "";
        QString     prix      = "";
        QString     generique = "";
        QString     statut    = "";

      if (m_pQSqlQuery->next() && m_position < 200000)
         {code_cpg     = m_pQSqlQuery->value(0).toString();
          cip_code     = m_pQSqlQuery->value(1).toString();
          libelle      = CGestIni::Utf8_Query(*m_pQSqlQuery, 2);
          atc_code     = m_pQSqlQuery->value(3).toString();
          prix         = C_BDM_Api::deleteUnsignificativesZeros( convertPrixDecimalToCts(m_pQSqlQuery->value(4).toString()) );
          statut       = m_pQSqlQuery->value(5).toInt()==2?"H":"VH";
          nb_UP        = m_pQSqlQuery->value(6).toString();
          generique    = code_CPG_to_StatutGenerique(code_cpg);
          dciList      = code_CPG_to_DCI_1_2_3(code_cpg);

          rC_BDM_DrugListRecord.set_commercialName(libelle);
          rC_BDM_DrugListRecord.set_id(cip_code);
          rC_BDM_DrugListRecord.set_id_type("CIP");
          rC_BDM_DrugListRecord.set_owner(owner());
          rC_BDM_DrugListRecord.set_lang(lang());
          rC_BDM_DrugListRecord.set_atc(atc_code);
          rC_BDM_DrugListRecord.set_dci_1Name(dciList[0]);
          rC_BDM_DrugListRecord.set_dci_2Name(dciList[1]);
          rC_BDM_DrugListRecord.set_dci_3Name(dciList[2]);
          rC_BDM_DrugListRecord.set_nb_up(nb_UP);
          rC_BDM_DrugListRecord.set_ucd_price(prix);
          rC_BDM_DrugListRecord.set_generique(generique);
          rC_BDM_DrugListRecord.set_statut(statut);
          ++m_position;
          return 1;
        }  // endif (query.isActive() )
        return 0;
}
//------------------------------ getRefundedIndications ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code_indication  indication code  as giving by the drugs BDM.
 *  \param const QString &code_type  code typ as giving by the drugs BDM
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_DatasempPlugin::getRefundedIndications( const QString &code_indication,   const QString &code_type )
{
    Q_UNUSED(code_type);
    QList <C_BDM_IndicationRecord> retList;
    QString requete =      " SELECT t00.f3,         \r"  // 0  as drug_name
                           "        t00.f2,         \r"  // 1  as cip
                           "        t4E.f5,         \r"  // 2  as tarif
                           "        t50.f1          \r"  // 3  as indication
                           " FROM   t50             \r"
                           "       ,t52             \r"
                           "       ,t00             \r"
                           "       ,t4E             \r"
                           "       ,t5J             \r"
                           "       ,t4k             \r"
                           " WHERE  t52.f1  = t00.f0     \r"
                           " AND    t52.f2  = 2994       \r" //     -- selectionner indications pour ce CIP
                           " AND    t50.f0  = t52.f0     \r" //
                           " AND    t5J.f0  = T4E.f0     \r" //     -- lier table tarifs aux indications
                           " AND    t5J.f1  = t50.f0     \r" //
                           " AND    t4E.fE !='0'         \r" //     -- selectionner celles remboursables
                           " AND    t4E.f0  =t4K.f1      \r" //
                           " AND    t00.f1  =t4K.f0      \r" //     -- selectionner ce CIP dans entites remboursables
                           " AND    t50.f0  = '" + code_indication + "'  \r";
    QSqlQuery query(requete , database() );
    if    ( !query.isActive() )
          {  outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::getRefundedIndications()</b></font>"), requete, __FILE__, __LINE__  );
             return retList;
          }
    while (  query.next() )
          { retList.append(C_BDM_IndicationRecord( query.value(1).toString(),        // cip
                                                   "",                               // date
                                                   "",                               // level
                                                   "",                               // level type
                                                   "",                               // level libelle
                                                   CGestIni::Utf8_Query(query, 3),   // libelle (ici libelle de l'indication)
                                                   CGestIni::Utf8_Query(query, 0),   // descriptif (ici drug name)
                                                   code_indication,                  // code
                                                   "DS_I",                           // code type
                                                   query.value(2).toString()         // tarif si remboursable
                                                  )
                          );
          }
 return    retList;
}

/*
 * SELECT   t50.f1 as indication
        ,t00.f3 as drugName
        ,t00.f2 as cip
        ,t1C.f1 as nature
        ,t52.f2 as codeNature
 FROM   t52,
        t00,
        t50,
        t4B,
        t1C
 WHERE LOWER(t50.f1) like '%urinaire%'
 -- AND (t50.f4=1 OR t50.f4=3 )

 AND   t52.f0 = t50.f0
 AND   t52.f2 = 2994
 AND   t1C.f0 = t52.f2
 AND   t00.f0 = t52.f1
 AND   t4B.f0 = t00.f0
 * */
//------------------------------ getIndicationsFromText ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &text      text from wich we want indications datas.
 *  \param const QString &filter   filter wanted datas  a combination | of this flags
 *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
 *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
 *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
 *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
 *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
 QList <C_BDM_IndicationRecord> C_BDM_DatasempPlugin::getIndicationsFromText( const QString &text,   int filter )
 {   Q_UNUSED( filter );
     QList <C_BDM_IndicationRecord> retList;
     QString requete =            " SELECT DISTINCT\n"
                                  " t50.f1\n"     // as indication
                                  " ,t50.f0\n"    // as code indication
                                  " FROM t52\n"
                                  " ,t50\n"
                                  " WHERE LOWER(t50.f1) like '%"+text.toLower()+"%' \n"
                                  " AND t52.f0 = t50.f0\n"
                                  " AND t52.f2 = 2994\n"
                                  " GROUP BY t50.f0";
      QSqlQuery query(requete , database() );
      if    ( !query.isActive() ) return retList;
      while (  query.next() )
            { retList.append(C_BDM_IndicationRecord( "",
                                                     "",
                                                     "",
                                                     "",
                                                     "",
                                                     CGestIni::Utf8_Query(query, 0),   // libelle
                                                     "",
                                                     query.value(1).toString(),        // pk
                                                     "DS_I"
                                                    )

                            );
            }
   return    retList;
 }

//------------------------------ getIndicationsFromCIP ------------------------------------------
/*
// SMR ASMR selon cip
SELECT t00.f2 as cip,
       t3C.f0 as id_indic,
       t00.f3 as drugName,
       t3C.f3 as niveau,
       t3E.f1 as libelleNiveau,
       t3E.f2 as infoNiveau,
       t3C.f4 as indication,
       t3C.f2 as type
 FROM
       t3C,
       t3E,
       t00,
       t4B
where t4B.f0=t00.f0
and   t00.f2='3646903'       -- lovenox
and   t3C.f1=t4B.f0
and   t3C.f3=t3E.f0
-- and   t3C.f2='A'
limit 0,300

// indications DATASEMP from cip
 SELECT t52.f0 ,
        t00.f3 as drugName,
        t50.f1
 FROM   t52,
        t00,
        t50
 WHERE t52.f1 = t00.f0
 AND t52.f2 >= 2993
 AND t52.f2 <= 2995
 AND t50.f0 = t52.f0
 -- and t00.f3 like '%LOVE%'
 and t00.f2 = '3646926'
limit 0,100

// indications CIM10 from cip
SELECT t00.f2 as cip,
       t52.f2 as niveau,
       t1C.f1 as libelleNiveau,
       t00.f3 as drug_name,
       t50.f1 AS CI_Vidal,
       t4N.f2 AS CIM_X,
       t4N.f3 AS codeCIM_X
FROM t52
INNER JOIN t4B ON t52.f1 = t4B.f0
INNER JOIN t1C ON t52.f2 = t1C.f0  -- libelle
INNER JOIN t50 ON t52.f0 = t50.f0,
t55,
t4N,
t00
WHERE t00.f0 = t4B.f0
-- AND t00.f3 LIKE 'LOVE%'
AND t00.f2='3646926'       -- lovenox
AND t55.f1 = t4N.f0
AND t55.f0 = t50.f0
AND t52.f2 >= 2993        -- t52 selection les indications ou ci du produit en relation avec t00.f0
AND t52.f2 <= 2995
-- AND t4N.f3 like 'K27%' -- 'K27'
limit 0,3000
*/
//------------------------------ getIndicationsFromCIP ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \param const QString &filter   filter wanted datas  a combination | of this flags
 *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
 *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
 *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
 *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
 *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
 *                                  C_BDM_PluginI::REFUND      we check if indications are linked to refund ( m_refundLink will contains price)
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_DatasempPlugin::getIndicationsFromCIP( const QString &cip,   int filter )
 {
 QList <C_BDM_IndicationRecord> retList;
 retList                = C_BDM_DatasempPlugin::Datasemp_getIndications_FromCIP(cip, filter);
 if ( filter&C_BDM_PluginI::ASMR || filter&C_BDM_PluginI::SMR )
    {retList += Datasemp_getASMR_SMR_FromCIP(cip, filter);
    }
 return retList;
}

//------------------------------ getCIPFromIndication ------------------------------------------
/*! \brief select indications informations for a drug.
 *  \param const QString &code       indication code of the drugs from wich we want datas.
 *  \param const QString &codeType   type of the indication code. (TH_I CIM CISP DS_A DS_S DS_I)
 *  \return QStringList cip result list
 */
QStringList C_BDM_DatasempPlugin::getCIPFromIndication( const QString &code, const QString &codeType )
{QStringList retList;
 if (codeType=="CIM")                               retList +=  Datasemp_get_CIP_From_CIM10(code);
 else if (codeType == "DS_I")                       retList +=  Datasemp_get_CIP_From_DatasempIndicationCode(code);
 else if (codeType == "DS_A"||codeType == "DS_S")   retList +=  Datasemp_get_CIP_From_ASMR_SMR_Code(code, codeType );
 return retList;
}
//------------------------------------ addCollapsibleJSPanel -------------------------------
/*! \brief create  hmtl block which is a collapsible panel which works with JS SpryCollapsiblePanel.js.
 *  \param int   index of panel (for identification anchor).
 *  \param QString &date           it will contains date        of panel.
 *  \param QString &level          it will contains level       of SMR ASM if present (right information).
 *  \param QString &indication     it will contains indication  red text .
 *  \param QString &descriptif     it will contains description which is collapsible .
 *  \param QString &codeCimx       it will contains code CIM10  of SMR ASM if present.
 *  \param QString &path           path of the JS script and images.
 *  \param QString &javaS_CreatePanel_Body      JS instanciation panels Widget list on wich append panel instance.
 *  \return QString which is html block to append.
 */
QString  C_BDM_DatasempPlugin::addCollapsibleJSPanel       (  int   index,
                                                              const QString &date,
                                                              const QString &level,
                                                              const QString &libelle,
                                                              const QString &codeCimx,
                                                              const QString &indication,
                                                              const QString &path,
                                                              QString &descriptif,
                                                              QString &javaS_CreatePanel_Body ,
                                                              const QString &className /* = "CollapsiblePanelTab" */)
{       QString idStr = QString::number(index);
        //............... cree les collapse panell .................................................
        descriptif += "<br/>";
        descriptif  = C_Utils_Html::makeLinkList(descriptif,"<br/>","EOL", idStr).replace("- ","<br/>&nbsp;&nbsp;&nbsp;\302\272");
        javaS_CreatePanel_Body += QString("    var CollapsiblePanel%1 = new Spry.Widget.CollapsiblePanel(\"CollapsiblePanel%1\", {contentIsOpen:false});\n"
                                         ).arg(idStr);
        return                    QString("\n"
                                          "<a name=\"smr%1\"></a>\n"       // ancre
                                          "<div id=\"CollapsiblePanel%1\" class=\"CollapsiblePanel CollapsiblePanelClosed\">\n"
                                          "  <div class=\"%9\" tabindex=\"0\" onclick=\"openClosePanel(%1)\">\n"
                                          "     <table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"99%\">\n"
                                          "        <tbody>\n"
                                          "           <tr>\n"
                                          "               <td class=\"bdm_titre_chapitre_bleu\">%8<font color=#9f0000>  %4 %5</font> </td>\n"
                                          "               <td align=\"right\" class=\"bdm_info_statut\"> %2 <br/> %3</td>\n"
                                          "               <td align=\"right\" width=\"30\"><img src=\"%6/panel_ouvrir.png\" name=\"bouton%1\" title=\"Ouvrir\"></td>\n"
                                          "           </tr>\n"
                                          "        </tbody>\n"
                                          "     </table>\n"
                                          "  </div>\n"
                                          "  <div class=\"CollapsiblePanelContent\" style=\"visibility: hidden; display: block; height: 524px; \">\n"
                                          "      <div id=\"data%1\">\n"
                                          "        %7\n"                    // truc \303\240 afficher
                                          "      </div>\n"
                                          "  </div>\n"     // %1  %2     %3      %4          %5      %6      %7         %8    %9
                                          "</div>\n").arg(idStr, date, level, codeCimx, indication, path, descriptif, libelle, className);
}

//------------------------------------ getAuthorizedPrescriptors -------------------------------
/*! \brief get authorized prescriptors list.
 *  \param  QString      code_produit  datasemp prouct code
 *  \return QStringList  with all authorized prescriptors list
 */
QStringList  C_BDM_DatasempPlugin::getAuthorizedPrescriptors(const QString &code_produit)
{   QStringList ret_list;
    QString requete = QString (  " SELECT  t62.f1 ,       \n"  // 0 intitule
                                 "         t62.f0         \n"  // 1 code prescripteur
                                 " FROM    t61            \n"
                                 "        ,t62            \n"
                                 " WHERE  t62.f0 = t61.f2 \n"
                                 " AND    t61.f0 = '%1'   \n").arg(code_produit);
    QSqlQuery query(requete , database() );
    if ( query.isActive() )
       { while ( query.next() )
               { ret_list << CGestIni::Utf8_Query(query, 0);
               }
       }
    else
       {
         outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::getPrescripteursAutorises(%1)</b></font>").arg(code_produit), requete, __FILE__, __LINE__  );
       }
    return ret_list;
}
//------------------------------------ getAuthorizedPrescriptorsFromCIP -------------------------------
/*! \brief  get authorized prescriptors list from CIP.
 *  \param  QString      cip  can be : '1234','1232' or 1234 or ('1234','1235')
 *  \return QMap <int,C_PrescriptionRestreinte>  with all authorized prescriptors\
 *          QMap list  where key is cip code and value is C_PrescriptionRestreinte class where\
 *          cip code, drug name, and prescriptors list are filled
 */
QMap <int,C_PrescriptionRestreinte>  C_BDM_DatasempPlugin::getAuthorizedPrescriptorsFromCIP(const QString &cip)
{   QMap < int, C_PrescriptionRestreinte > ret_map;            // < cip, C_PrescriptionRestreinte >
    QString requete =            " SELECT  t62.f1 ,       \n"  // 0 prescripteur
                                 "         t00.f2 ,       \n"  // 1 cip
                                 "         t00.f3         \n"  // 2 drug name
                                 " FROM    t61            \n"
                                 "        ,t62            \n"
                                 "        ,t00            \n"
                                 " WHERE  t62.f0 = t61.f2 \n"
                                 " AND    t61.f0 = t00.f1 \n";
    if ( cip.startsWith("'") )
         requete  +=  " AND    t00.f2 IN ("+cip+  ") \n";     // liste de cip de la forme 'cip_0','cip_1','cip_2'
    else if ( cip.startsWith("(") )
         requete  +=  " AND    t00.f2 IN " +cip+   " \n";     // liste de cip de la forme ('cip_0','cip_1','cip_2')
    else
         requete  +=  " AND    t00.f2 = '" +cip+   "'\n";
    requete       +=  " ORDER BY    t00.f2           \n";
    // CGestIni::Param_UpdateToDisk(m_pathAppli+"requete.txt",requete.toLatin1());
    QSqlQuery query(requete , database() );

    if ( query.isActive() )
       { while ( query.next() )
               { int                                               i_cip = query.value(1).toInt();
                 QString                                    prescripteur = CGestIni::Utf8_Query(query, 0);
                 QMap <int, C_PrescriptionRestreinte>::const_iterator it = ret_map.find ( i_cip );
                 QStringList prescripteurs;
                 //............... un enregistrement dans la QMap ne peut etre modifie car constant..........................
                 //                on ne peut donc rajouter un prescripteur a sa liste de prescripteurs
                 //                si il existe deja il faut recuperer sa liste des prescripteurs, y rajouter le prescripteur
                 //                detruire l'enregistrement qui sera recree par la suite
                 if ( it !=  ret_map.end() )                         // si le produit est deja existant et enregistre
                    { prescripteurs = it.value().prescripteurs();    // recuperer sa liste des prescripteurs a laquelle sera ajoute le prescripeur
                      ret_map.remove(i_cip);                         // enlever le produit de la QMap (sera rajoute ensuite)
                    }
                 //............... rajouter un prescripteur a la liste et ajouter l'enregistrement a la QMap..........................
                 prescripteurs << prescripteur;                 // ajouter le prescripteur a la liste des prescripteurs
                 ret_map.insert ( i_cip,                        // rajouter le produit a la liste QMap (clef = cip)
                                  C_PrescriptionRestreinte( query.value(1).toString()        ,    // 6 cip
                                                            CGestIni::Utf8_Query(query, 2)   ,    // 7 drug name
                                                            ""                               ,    // 0 Registre prescriptif
                                                            ""                               ,    // 1 Conduite pour le suivi
                                                            ""                               ,    // 2 duree maximale
                                                            ""                               ,    // 3 fractionnement
                                                            ""                               ,    // 4 site de prescription
                                                            ""                               ,    // 5 site de dispensation
                                                            ""                               ,    // 8 liste I ou II
                                                            "0"                              ,    // 11 isStupefiant si 2 ou 3
                                                            ""                               ,    // 9 prescr_initiale
                                                            ""                               ,    // 10 dispen_initiale
                                                            ""                               ,    // 13 portée
                                                            ""                               ,    // 12 exception si 1
                                                            ""                               ,    // 14 gestion ordo
                                                            prescripteurs                    ,    // 15 prescripteurs
                                                            true
                                                          ) // C_PrescriptionRestreinte()
                                );  // ret_map.insert()
               } // while ( query.next() )
       }
    else
       { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::getAuthorizedPrescriptorsFromCIP(%1)</b></font>").arg(cip), requete, __FILE__, __LINE__  );
       }
    return ret_map;
}
//-------------------------------- getPrescriptionRestreinte ------------------------------------------------------
/*! \brief return a C_PrescriptionRestreinte class with restrictives prescriptions informations
 *         if there is not restrictive prescription for this CIP  isValid() is false;
 *  \param const QString& cip drug cip identification . this input can be a cip list for several tests
 *                        as ('cip_0','cip_1' .... 'cip_n') (distinction is on argument starting with '(' )
 *                        or  'cip_0','cip_1' .... 'cip_n'  (distinction is on argument starting with  ' )
 *  \return QList<C_PrescriptionRestreinte>
 */
QList<C_PrescriptionRestreinte> C_BDM_DatasempPlugin::getPrescriptionRestreinteFromCIP(const QString &cip)
{   QList<C_PrescriptionRestreinte>       retList;
    QMap <int, QString>            map_cip_tested;  // dans cette QMap seulle la clef (cip) est utile
    QString requete = " SELECT t60.f1 ,        \n"  // 0 registre comptable
                      "        t63.f2 ,        \n"  // 1 conduite a tenir (tres souvent vide)
                      "        t63.f4,         \n"  // 2 Duree maximale du traitement en jours, pour une prescription. Pour certains produits, ce peut etre la date de fin du traitement prescrit qui sert de base a la quantite délivree.
                      "        t63.f5,         \n"  // 3 Fractionnement Indique le cas echeant nombre maximum de jours de traitement pouvant faire l'objet d'une delivrance unique. NULL si  pas reglemente.
                      "        t63.f6,         \n"  // 4 code lieu de renouvellement :  -> t1A.f0 prescr_ordinaire : En cas de restriction de la prescription a un type d'etablissements (hopitaux, centres specialises, etc.), ce champ contient un pointeur sur le type de sites. NULL dans le cas contraire.
                      "        t63.f7,         \n"  // 5 code lieu de dispensation   :  -> t1A.f0 dispen_ordinaire : En cas de restriction de la dispensation a un type d'etablissements (hopitaux, centres specialises, etc.), ce champ contient un pointeur sur le type de sites. NULL dans le cas contraire.
                      "        t00.f2,         \n"  // 6 code cip
                      "        t00.f3,         \n"  // 7 drug name
                      "        t63.f3,         \n"  // 8 liste I ou II
                      "        t63.f8,         \n"  // 9  -> t1A.f0 prescr_initiale  : En cas de restriction de la prescription a un type d'etablissements (hopitaux, centres specialises, etc.), ce champ contient un pointeur sur le type de sites. NULL dans le cas contraire.
                      "        t63.f9,         \n"  // 10 -> t1A.f0 dispen_initiale  : En cas de restriction de la prescription a un type d'etablissements (hopitaux, centres specialises, etc.), ce champ contient un pointeur sur le type de sites. NULL dans le cas contraire.
                      "        t63.f1,         \n"  // 11 isStupefiant si 2 ou 3
                      "        t63.fB,         \n"  // 12 exception si 1
                      "        t63.fA,         \n"  // 13 portee : Exprimee en jours. Duree au-dela de laquelle le renouvellement n'est plus possible (necessite d'une nouvelle prescription "initiale").
                      "        t63.fD,         \n"  // 14 gestion de l'ordonnance --> 1C.f0
                      "        t63.f0          \n"  // 15 code produit --> sert pour rechercher les prescipteurs autorises
                      " FROM   t63             \n"
                      "       ,t60             \n"
                      "       ,t00             \n"
                      " WHERE  t63.f0 = t00.f1 \n"
                      " AND    t63.f1 = t60.f0 \n"
                     /* Philippe Duchateau
                        t63.f6 IN('2','4','8') -- is restrictive renouv prescription
                        OR t63.f7 IN('2','4','8') -- is restrictive renouv dispensation
                        OR t63.f8 IN('2','4','8') -- is restrictive initial prescription
                        OR t63.f9 IN('2','4','8') -- is restrictive initial dispensation
                      */
                     /*
                      " AND    (     t63.f6 IN('2','4','8') \n"     // is restrictive renouv prescription
                      "           OR t63.f7 IN('2','4','8') \n"     // is restrictive renouv dispensation
                      "           OR t63.f8 IN('2','4','8') \n"     // is restrictive initial prescription
                      "           OR t63.f9 IN('2','4','8') \n"     // is restrictive initial dispensation
                      "           OR t63.fB = '1'                   \n"     // is exception
                      "           OR t63.f1 IN('2','3')             \n"     // isStupefiant si 2 ou 3
                      "           OR t63.f4 > '0'                   \n"     // is time limited
                      "        )                                    \n" ;
                      */

                      " AND    (     t63.f6 IN('2','3','5','6','9') \n"     // is restrictive renouv prescription
                      "           OR t63.f7 IN('2','3','5','6','9') \n"     // is restrictive renouv dispensation
                      "           OR t63.f8 IN('2','3','5','6','9') \n"     // is restrictive initial prescription
                      "           OR t63.f9 IN('2','3','5','6','9') \n"     // is restrictive initial dispensation
                      "           OR t63.fB = '1'                   \n"     // is exception
                      "           OR t63.f1 IN('2','3')             \n"     // isStupefiant si 2 ou 3
                      "           OR t63.f4 > '0'                   \n"     // is time limited
                      "        )                                    \n" ;

    if ( cip.startsWith("'") )
         requete  +=  " AND    t00.f2 IN ("+cip+  ") \n";     // liste de cip de la forme 'cip_0','cip_1','cip_2'
    else if ( cip.startsWith("(") )
         requete  +=  " AND    t00.f2 IN " +cip+   " \n";     // liste de cip de la forme ('cip_0','cip_1','cip_2')
    else
         requete  +=  " AND    t00.f2 = '" +cip+   "'\n";

    // (1) ................. recuperer les prescriptions restreintes ........................
    QSqlQuery query(requete , database() );
    if ( query.isActive() )
       { while ( query.next() )
               {  //if (query.value(2).toInt()<=0) continue;     // duree critere de validite ?
                  QString site_presc        = query.value(4).toString();
                  QString site_disp         = query.value(5).toString();
                  QString duree_max         = query.value(2).toString();
                  QString fractnmt          = query.value(3).toString();
                  QString drug_name         = CGestIni::Utf8_Query(query, 7);
                  QStringList prescripteurs = getAuthorizedPrescriptors(query.value(15).toString());

                  if ( site_presc.toInt() > 0 ) site_presc =  isThisValueInTable("t1A", "t1A.f0", site_presc, "t1A.f1") ;
                  else                          site_presc = "";
                  if ( site_disp.toInt()  > 0 ) site_disp  =  isThisValueInTable("t1A", "t1A.f0", site_disp , "t1A.f1") ;
                  else                          site_disp  = "";
                  QString prescr_initiale     = query.value(9).toString();
                  QString dispen_initiale     = query.value(10).toString();
                  if ( prescr_initiale.toInt()  > 0 ) prescr_initiale  =  isThisValueInTable("t1A", "t1A.f0", prescr_initiale, "t1A.f1") ;
                  else                                prescr_initiale  = "";
                  if ( dispen_initiale.toInt()  > 0 ) dispen_initiale  =  isThisValueInTable("t1A", "t1A.f0", dispen_initiale , "t1A.f1") ;
                  else                                dispen_initiale  = "";
                  QString portee              = query.value(13).toString();              // periode au delas de laquelle la prescription initiale doit etre refaite (renouvellement plus permis)
                  QString gest_ordo           = query.value(14).toString();
                  if ( gest_ordo.toInt()  > 0 ) gest_ordo  =  isThisValueInTable("t1C", "t1C.f0", gest_ordo , "t1C.f1") ;
                  else                          gest_ordo  =  "";
                  int stup = query.value(11).toInt();
                  int exce = 0;
                  QByteArray b_exce =  query.value(12).toByteArray();
                  if (b_exce.size()>0 && b_exce[0]==(char)'\001') exce = 1;

                  map_cip_tested[ query.value(6).toInt() ] = ""; // dans cette QMap seulle la clef (cip) est utile noter ce cip comme deja evalue
                  retList.append(
                                  C_PrescriptionRestreinte( query.value(6).toString()       ,    // 6 cip
                                                            drug_name                       ,    // 7 drug name
                                                            CGestIni::Utf8_Query(query, 0)  ,    // 0 Registre prescriptif
                                                            CGestIni::Utf8_Query(query, 1)  ,    // 1 Conduite pour le suivi
                                                            duree_max.toInt()?duree_max:""  ,    // 2 duree maximale
                                                            fractnmt.toInt() ?fractnmt: ""  ,    // 3 fractionnement
                                                            site_presc                      ,    // 4 site de prescription
                                                            site_disp                       ,    // 5 site de dispensation
                                                            query.value(8).toString()       ,    // 8 liste I ou II
                                                            (stup>=2 && stup<=3)?"1":"0"    ,    // 11 isStupefiant si 2 ou 3
                                                            prescr_initiale                 ,    // 9 prescr_initiale
                                                            dispen_initiale                 ,    // 10 dispen_initiale
                                                            portee.toInt() ? portee : ""    ,    // 13 portée
                                                            exce==0?"":"1"                  ,    // 12 exception si 1
                                                            gest_ordo                       ,    // 14 gestion ordo
                                                            prescripteurs                   ,    // 15 prescripteurs
                                                            true
                                                          )  // C_PrescriptionRestreinte(
                                ); // retList.append()
               } // while ( query.next() )
       }
    else
       { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::getPrescriptionRestreinteFromCIP(%1)</b></font>").arg(cip), requete, __FILE__, __LINE__  );
       }
    // (2) ................. inclure a la liste des prescriptions restreintes ........................
    //                       celles limitees a des prescripteurs autorisees
    //                       ayant echappe aux tests ci-dessus faits en (1)
    QMap <int, C_PrescriptionRestreinte>  cip_prescripteurs = getAuthorizedPrescriptorsFromCIP(cip);  // recuperer la liste de tous les prescripteurs autorises
    QMap <int, C_PrescriptionRestreinte>::const_iterator it = cip_prescripteurs.constBegin();
    while ( it != cip_prescripteurs.constEnd() )                       // explorer cette liste des prescripteurs autorises
      {  int i_cip  = it.key();                                        // recuperer le cip
         if ( ! map_cip_tested.contains( i_cip ) )                     // si un des cip de cette liste n'est pas dans la liste de ceux crees en (1)
            {   retList.append( cip_prescripteurs.values ( i_cip) );   // le rajouter
            }
         ++it;
      }
    return retList;
}

//------------------------------ getMonographieFromCIP ------------------------------------------
/*! \brief select monographie for a drug.
 *  \param const QString &cip            code CIP of the drug from wich we want datas.
 *  \param  bool dataMustBeReadInCache   if TRUE datas are read in internal cache else there are computed and after placed in cache.
 *  \return QString contains monographie
 */
QString  C_BDM_DatasempPlugin::getMonographieFromCIP (    const QString &cip   , bool dataMustBeReadInCache /* = TRUE */ )
{
  Q_UNUSED(dataMustBeReadInCache);

    /////////////////////////////// MONOGRAPHIE NORMALE /////////////////////////////////////////////////////////
  QString requete;
  // 3  Code Produit Global ________________________________
  // 2  intitule du medoc ______________________________    |
  // 1  Donnees XML ____________________________        |   |
  // 0  Code unite de vente __________          |       |   |
  //                                  |         |       |   |
  requete +=       " SELECT     t00.f1,   t4B.f5, t4B.f2 , t4B.f0"
                   " FROM       t00,t4B                  "
                   " WHERE      t00.f0 = t4B.f0          "
                   " AND        t00.f2 = '" + cip + "'" ;
  QSqlQuery query(requete , database() );
  if (! (query.isActive() && query.next()))                             return QString::null;

  QString mono = Datasemp_GetMonographie( query.value(0).toString() );
  if ( mono.length()==0 )
     {
      //................. si pas de monographie vidal pour ce produit...................................................
      //                  creer une page html a  partir des infos XML
      mono = CGestIni::Utf8_Query(query, 1);  // recuperer les donnes XML
      if ( mono.length() )
         { Datasemp_Xml_to_html(mono, CGestIni::Utf8_Query(query, 2));
         }
     }
  //...................... references produit......................
  QString codeProduit = query.value(3).toString();
  QString codeUV      = query.value(0).toString();

  /////////////////////////////// COMPLEMENTS MONOGRAPHIQUES /////////////////////////////////////////////////////////
  //..................... l'entete pour le collapsible ...............................................
  QString path                   = m_pathAppli+"Ressources/Datasemp/";
  QString html                   = QString  ("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
                                                "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
                                                "<head>\n"
                                                "<title>BDM -Monographie : %2</title>\n"
                                                "<link href=\"%1/monographie_THE.css\" rel=\"stylesheet\" type=\"text/css\">\n"
                                                "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF8\">\n"
                                                "<script src=\"%1/SpryCollapsiblePanel.js\" type=\"text/javascript\"></script>\n"
                                                "<link href=\"%1/SpryCollapsiblePanel.css\" rel=\"stylesheet\" type=\"text/css\">\n"
                                                "<script id=\"clientEventHandlersJS\" language=\"javascript\">\n"
                                                "<!--\n"
                                                "function openClosePanel(numpanel)\n"
                                                "{    if (eval(\"CollapsiblePanel\" + numpanel).contentIsOpen == false) {\n"
                                                "        document.images['bouton' + numpanel].src = \"%1/panel_fermer.png\";\n"
                                                "        document.images['bouton' + numpanel].title = \"Fermer\";\n"
                                                "        } else {\n"
                                                "        document.images['bouton' + numpanel].src = \"%1/panel_ouvrir.png\";\n"
                                                "        document.images['bouton' + numpanel].title = \"Ouvrir\";\n"
                                                "        }\n"
                                                "}\n"
                                                "//-->\n"
                                                "</script>\n"
                                                "</head>\n"
                                                "<body>\n"
                                            ).arg(path, dataSourceVersion() ) ;
  QString javaS_CreatePanel_Body =  "<script type=\"text/javascript\">\n"             // ce bloc contient les creations des panneaux et est \303\240 la fin
                                    "<!--\n";
  int indexCollapsiblePanel = 0;
  //                       les infos complementaires generiques ......................................
  html   += tr("<p class=nom_chapitres> COMPL\303\211MENTS MONOGRAPHIQUES </p>");
  requete = QString ( " SELECT t3F.f1        \n"    //0 as code_groupe
                      " FROM   t3F           \n"
                      " WHERE  t3F.f0 = '%1' \n" ).arg(codeProduit);
  QStringList codes_lst;
  QString     str_codes_lst = "";
  query.exec(requete);
  if ( query.isActive())
     { while ( query.next() ) codes_lst << query.value(0).toString();
     }
  else
     { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::getMonographieFromCIP(get code groupe)</b></font>"), requete, __FILE__, __LINE__  );
     }
  if (codes_lst.size()) str_codes_lst = "('"+codes_lst.join("','")+"')";

  if (str_codes_lst.length())
     { requete = QString ( " SELECT t3F.f2,          \n"    // 0  as Type_groupe
                           "        t00.f3,          \n"    // 1 as drugName
                           "        t00.f2,          \n"    // 2 as cip
                           "        t01.f3,          \n"    // 3 as Prix
                           "        t00.f8           \n"    // 4 nbr UP (unite de prise)
                           " FROM   t3F,             \n"
                           "        t00,             \n"
                           "        t01,             \n"
                           "        t02              \n"
                           " WHERE  t3F.f0 = t00.f0  \n"
                           " AND    t01.f0 = t00.f1  \n"
                           " AND    t02.f0 = t00.f1  \n"
                           " AND    t02.f3 = '0'     \n"   // disponible
                           " AND    t00.fK LIKE '1%' \n"
                           " AND  t3F.f1 IN %1 group by (t00.f3)  order by t3F.f2    \n"
                         ).arg(str_codes_lst);
       str_codes_lst     = "";   // on s'en sert pour les resultats
       int last_type = -1;
       query.exec(requete);
       if ( query.isActive())
          { str_codes_lst += "     <table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"99%\">\n"
                             "        <tbody>\n"
                             "           <tr>\n";
            while ( query.next() )
                  { int type = query.value(0).toInt();
                    if (type != last_type)   // 0 champ de bit de la relation 1 : Référent  6 : Générique  7 : Substituable  8 : Spécialité pharmaceutique proche
                       { if       ( type==1 ) str_codes_lst += QString::fromUtf8("<tr><td width=\"50%\";><b><u>R\303\251f\303\251rent du groupe des g\303\251n\303\251riques</b></u> : </td></tr>");
                         else if  ( type==6 ) str_codes_lst += QString::fromUtf8("<tr><td width=\"50%\";><b><u>G\303\251n\303\251riques</b></u> : </td></tr>");
                         else if  ( type==7 ) str_codes_lst += QString::fromUtf8("<tr><td width=\"50%\";><b><u>M\303\251dicaments substituables</b></u> : </td></tr>");
                         else if  ( type==8 ) str_codes_lst += QString::fromUtf8("<tr><td width=\"50%\";><b><u>Sp\303\251cialit\303\251s pharmaceutiques proches</b></u> : </td></tr>");
                         last_type = type;
                       }
                    QString value  = CGestIni::Utf8_Query(query, 1);
                    QString code   = query.value(2).toString();
                    double  nb_up  = query.value(4).toDouble();
                    double   prix  = query.value(3).toDouble();
                    bool isOk      = nb_up>0&&prix>0;
                    str_codes_lst += QString ("<tr><td width=\"50%\";><a href=\"Code_CIP=%1\">%2</a></td>"  // code value
                                                  "<td width=\"15%\";>%3</td>"                              //  prix boite
                                                  "<td width=\"15%\";>%4</td>"                              // par UP
                                                  "<td width=\"20%\";>%5</td>"                              // prix UP
                                              "</tr>"
                                             )
                                            .arg( code,
                                                  value,
                                                  prix>0?query.value(3).toString()+" &euro;":"",
                                                  isOk>0?tr(" par UP "):"",
                                                  isOk>0?QString::number(prix/nb_up,'f',3):""
                                                );
                  }
            str_codes_lst += "        </tbody>\n"
                             "     </table>\n";
          }
       else
          { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::getMonographieFromCIP(get generiques)</b></font>"), requete, __FILE__, __LINE__  );
          }
     }
  if ( str_codes_lst.length() )
     { html  += addCollapsibleJSPanel  ( indexCollapsiblePanel++,
                                         "", //C_GET_THE_EXP_ID.SAU_DOC_DATE().toString("dd-MM-yyyy"),
                                         "",
                                         tr("G\303\211N\303\211RIQUES SUBSTITUABLES COMPARATEUR"),
                                         "",                                        // cim 10
                                         "",
                                         path,   //R\303\251f. : Journal Officiel du
                                         str_codes_lst ,
                                         javaS_CreatePanel_Body,
                                         "panel_ComplementMono_Css");
     }
  //                       les infos complementaires bibliographiques ................................
  requete     =    " SELECT     t45.f5,   \n"   //  0 chemin complet du document
                   "            t45.fC    \n"   //  1 date
                   " FROM       t49,      \n"
                   "            t45       \n"
                   " WHERE      t49.f0 = '" + codeProduit        + "'\n"
                   " AND        t49.f1 = t45.f0 ORDER BY t45.fC DESC \n" ;
  QStringList ref_biblios;
  QString     ref_biblios_bloc = "";
  QString           biblio_pgr = "";
  query.exec(requete);
  while ( query.isActive() && query.next() )
     {  QByteArray ba  = query.value(0).toByteArray();   // attention faut passer par un QByteArray car caracteres \0 qui font planter la conversion vers QString
        QString   ref  = "";
        for ( int i=0; i<ba.size(); ++i )
            { if (i<ba.size()-1 && ba[i]=='\0' ) ref +="/0";   // incroyable mais vrai !!!!
              else                               ref +=  ba[i];
            }
        ref  = ref.replace("DS_DOCSALD"    , "DS_DOCS/ALD/"     );
        ref  = ref.replace("DS_DOCSPGR"    , "DS_DOCS/PGR/"     );
        ref  = ref.replace("DS_DOCSBUM"    , "DS_DOCS/BUM/"     );
        ref  = ref.replace("DS_DOCSFIT"    , "DS_DOCS/FIT/"     );
        ref  = ref.replace("DS_DOCSRBUPTT" , "DS_DOCS/RBU/ptt/" );
        ref  = ref.replace("DS_DOCSRBUSNA" , "DS_DOCS/RBU/sna/" );
        ref  = ref.replace("DS_DOCSRBUIDD" , "DS_DOCS/RBU/idd/" );
        if ( ref.startsWith("http://") )  ref = QString("<a href=\"%1\">%1</a>")         .arg(ref);
        else                              ref = QString("<a href=\"ref_mono:%1\">%1</a>").arg(ref);
        QString date = query.value(1).toString();
        if ( date.length() )
           { date = QDateTime::fromString(date,Qt::ISODate).toString("dd-MM-yyyy");
             ref.prepend("Date du document : "+date+" ");
           }
        else
           { ref.prepend("Date du document : absente ");
           }
        ref_biblios <<  ref + "<br/>";
     }
  if ( ref_biblios.size() )
     { ref_biblios_bloc += QString::fromUtf8("<br/><b><u>R\303\251f\303\251rences bibliographiques</b></u> : <br/>");
       for ( int i= 0; i<ref_biblios.size(); ++i )
           {  if (ref_biblios[i].contains("DS_DOCS/PGR/"))
                 {   biblio_pgr       += ref_biblios[i];
                 }
              else
                 {   ref_biblios_bloc += ref_biblios[i];
                 }
           }
     }
  if ( ref_biblios_bloc.length() )
     { html  += addCollapsibleJSPanel  ( indexCollapsiblePanel++,
                                         "", //C_GET_THE_EXP_ID.SAU_DOC_DATE().toString("dd-MM-yyyy"),
                                         "",
                                         tr("BIBLIOGRAPHIES DIVERSES"),
                                         "",                                        // cim 10
                                         "",
                                         path,   //R\303\251f. : Journal Officiel du
                                         ref_biblios_bloc ,
                                         javaS_CreatePanel_Body,
                                         "panel_ComplementMono_Css");
     }
  if ( biblio_pgr.length() )
     { html  += addCollapsibleJSPanel  ( indexCollapsiblePanel++,
                                         "", //C_GET_THE_EXP_ID.SAU_DOC_DATE().toString("dd-MM-yyyy"),
                                         "",
                                         tr("PLAN DE GESTION DES RISQUES"),
                                         "",                                        // cim 10
                                         "",
                                         path,   //R\303\251f. : Journal Officiel du
                                         biblio_pgr ,
                                         javaS_CreatePanel_Body,
                                         "panel_ComplementMono_Css");
    }
  //....................... la prescription restreinte ...................................................................
  QString prescr_restr = "";
  QList<C_PrescriptionRestreinte> prescr_restr_list = getPrescriptionRestreinteFromCIP(cip);
  if ( prescr_restr_list.size() )
     {
       prescr_restr = prescr_restr_list[0].toString();
       if ( prescr_restr_list[0].liste().length() )
          { prescr_restr += tr("<br /><b><u>Information sur les Listes I et II </b></u><br />"
                               "Les m\303\251dicaments sur listes I et II sont des m\303\251dicaments soumis \303\240 prescription m\303\251dicale et qui "
                               "ne peuvent ainsi \303\252tre obtenus que sur pr\303\251sentation d'une ordonnance r\303\251dig\303\251e par un m\303\251decin, un chirurgien-dentiste ou une sage-femme.<br />"
                               "<b>Les m\303\251dicaments sur liste I</b> (cadre rouge sur la bo\303\256te) ne peuvent \303\252tre d\303\251livr\303\251s que pour la dur\303\251e de traitement mentionn\303\251e sur l'ordonnance.<br />"
                               "<b>Les m\303\251dicaments sur liste II</b> (cadre vert sur la bo\303\256te) peuvent \303\252tre d\303\251livr\303\251s plusieurs fois \303\240 partir de la m\303\252me ordonnance pendant 12 mois, sauf indication contraire du prescripteur.<br />"
                               "Une autre liste existe \303\251galement et regroupe les m\303\251dicaments stup\303\251fiants.<br />"
                               "<b>ref\303\251rence</b> : (CSP : Art. L.5132-6 et R.5121-36 et R5132-22) ");
          }
     }
  /*
  QString prescr_restr = "";
  requete     =    " SELECT t60.f1 ,        \n"  // 0 registre comptable
                   "        t63.f2 ,        \n"  // 1 conduite a tenir (tres souvent vide)
                   "        t63.f4,         \n"  // 2 Duree maximale du traitement en jours, pour une prescription. Pour certains produits, ce peut etre la date de fin du traitement prescrit qui sert de base a la quantite délivree.
                   "        t63.f5,         \n"  // 3 Fractionnement Indique le cas echeant nombre maximum de jours de traitement pouvant faire l'objet d'une delivrance unique. NULL si  pas reglemente.
                   "        t63.f6,         \n"  // 4 code lieu de renouvellement
                   "        t63.f7,         \n"  // 5 code lieu de dispensation
                   "        t63.f3          \n"  // 6 liste I ou II
                   " FROM   t63             \n"
                   "       ,t60                 \n"
                   " WHERE  t63.f0='"+codeUV+"' \n"
                   " AND    t63.f1 = t60.f0     \n";
  query.exec(requete);
  QString str   = "";
  int duree_max = 0;
  int liste     = 0;
  if ( query.isActive() && query.next() )
     { duree_max =  query.value(2).toInt();
       liste     =  query.value(6).toInt();
      //if ( query.value(2).toInt()>0 )    // duree critere de validite ?
           { str = CGestIni::Utf8_Query(query, 0);
             if (str.length())  prescr_restr += tr("<b><u>Registre prescriptif</b></u> : %1<br />").arg(str);
             str          = CGestIni::Utf8_Query(query, 1);
             if (str.length())  prescr_restr += tr("<b><u>Conduite pour le suivi</b></u> : <br />%1<br />").arg(str);
             str          = query.value(2).toString();
             if (duree_max)     prescr_restr += tr("<b><u>Dur\303\251e maximale</b></u> : %1<br />").arg(str);
             str          = query.value(3).toString();
             if (str.toInt())   prescr_restr += tr("<b><u>Fractionnement</b></u> : %1<br />").arg(str);
             str          = query.value(4).toString();
             if (str.toInt()>0) prescr_restr += tr("<b><u>Site de prescription</b></u> : %1<br />").arg( isThisValueInTable("t1A", "t1A.f0", str, "t1A.f1") );
             str          = query.value(5).toString();
             if (str.toInt()>0) prescr_restr += tr("<b><u>Site de dispensation</b></u> : %1").arg( isThisValueInTable("t1A", "t1A.f0", str, "t1A.f1") );
             prescr_restr += tr("<br /><b><u>Information sur les Listes I et II </b></u><br />"
                                "Les m\303\251dicaments sur listes I et II sont des m\303\251dicaments soumis \303\240 prescription m\303\251dicale et qui "
                                "ne peuvent ainsi \303\252tre obtenus que sur pr\303\251sentation d'une ordonnance r\303\251dig\303\251e par un m\303\251decin, un chirurgien-dentiste ou une sage-femme.<br />"
                                "<b>Les m\303\251dicaments sur liste I</b> (cadre rouge sur la bo\303\256te) ne peuvent \303\252tre d\303\251livr\303\251s que pour la dur\303\251e de traitement mentionn\303\251e sur l'ordonnance.<br />"
                                "<b>Les m\303\251dicaments sur liste II</b> (cadre vert sur la bo\303\256te) peuvent \303\252tre d\303\251livr\303\251s plusieurs fois \303\240 partir de la m\303\252me ordonnance pendant 12 mois, sauf indication contraire du prescripteur.<br />"
                                "Une autre liste existe \303\251galement et regroupe les m\303\251dicaments stup\303\251fiants.<br />"
                                "<b>ref\303\251rence</b> : (CSP : Art. L.5132-6 et R.5121-36 et R5132-22) ");
          }

     }
  else
     { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::getMonographieFromCIP(get dispensation)</b></font>"), requete, __FILE__, __LINE__  );
     }
*/
  if ( prescr_restr.length() )
     { html  += addCollapsibleJSPanel  ( indexCollapsiblePanel++,
                                         "", //C_GET_THE_EXP_ID.SAU_DOC_DATE().toString("dd-MM-yyyy"),
                                         "",
                                         tr("SP\303\211CIFICATIONS DE PRESCRIPTON ET D\303\211LIVRANCE"),
                                         tr("Liste %1").arg(prescr_restr_list[0].liste().length()==1?"I":"II"),                                        // cim 10
                                         prescr_restr_list[0].duree_max().toInt()>0?tr(" Prescription limit\303\251 \303\240 : %1 jours").arg(prescr_restr_list[0].duree_max()):"",
                                         path,   //R\303\251f. : Journal Officiel du
                                         prescr_restr ,
                                         javaS_CreatePanel_Body,
                                         "panel_ComplementMono_Css");
    }
  //....................... le tfr ...................................................................
  QString tfr = "";
  requete     =    " SELECT t4E.f0 ,        \n"  // 0 AS id_class
                   " t4E.f5,                \n"  // 1 AS tfr
                   " t4K.f1                 \n"  // 2 AS lien
                   " FROM   t4E INNER JOIN t4K ON t4K.f1=t4E.f0   \n"
                   " WHERE  t4K.f0='"+codeUV+"'                   \n";
  query.exec(requete);
  if ( query.isActive() && query.next() )
     {  tfr = query.value(1).toString();
     }
  else
     { outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::getMonographieFromCIP(get tfr)</b></font>"), requete, __FILE__, __LINE__  );
     }
  //                       les infos complementaires bibliographiques ................................
  //if (tfr.length())              mono.prepend(tfr.prepend (QString::fromUtf8("<br/><b><u>Tarif de responsabilit\303\251 forfaitaire</b></u> : ")) +"<hr/>");
  if ( tfr.toDouble()>0.0)
     { html  += addCollapsibleJSPanel  ( indexCollapsiblePanel++,
                                         "", //C_GET_THE_EXP_ID.SAU_DOC_DATE().toString("dd-MM-yyyy"),
                                         "",
                                         tr("Tarif Fortaitaire de Responsabilit\303\251 "),
                                         "",                                        // cim 10
                                         "  TFR =  "+ tfr + " euros",
                                         path,   //R\303\251f. : Journal Officiel du
                                         tfr ,
                                         javaS_CreatePanel_Body,
                                         "panel_ComplementMono_Css");
    }
  //................. la fin du collapsible .........................
  html += javaS_CreatePanel_Body +
          "//-->\n"
          "</script>\n"
          "    </body>\n"
          "<html>\n";
  //................. apres la monographie complementaire mettre la monographie standard .........................
  return html + tr("<p class=nom_chapitres> MONOGRAPHIE VIDAL DATASEMP\302\251 </p>")+
         mono;
}

//-------------------------------- getPriseUnitFormFromCIP ------------------------------------------------------
/*! \brief return the prise unit form from cip. (comprimes, suppositoires sachets,ampoules,doses kg, cuilleres a caf\303\251 etc...)
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return QString with unit form
 */
/*
SELECT  UPPER(t27.f2) as SubName,
       t00.f3        as drugName,
       t07.f4        as qu,
       t24.f1        as unit,
       t24.f5        as unitPlur
FROM  t07,
      t27,
      t08,
      t00,
      t24
WHERE t07.f0 = t00.f0 -- t07.f0 = '202489'
AND   t27.f0 = t07.f2
AND   t07.f3 = 1
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t08.f0 = t00.f0 -- '202489'       -- COLCHIMAX
AND   t24.f0 = t07.f5
AND   t00.f3 like '%LOVENOX%'
ORDER BY t00.f3
LIMIT 0,30000
 */
QString  C_BDM_DatasempPlugin::getPriseUnitFormFromCIP ( const QString &cip , const QString &commercialName /* = 0 */)
{Q_UNUSED(commercialName);
 QString ret ="";
 QString requete = QString (
        " SELECT t24.f1                 \n"   //  as natureName
        " FROM   t24,                   \n"
        "        t00                    \n"
        " WHERE  t00.f2 = '%1'          \n"  // -- 3690548 morphine   -- 3575327 actifed
        " AND    t00.f7 = t24.f0        \n"
      ).arg(cip);
QSqlQuery query(requete , database() );
if (query.isActive() && query.next())
   {ret = CGestIni::Utf8_Query(query, 0);
   }
if (ret.length()) return ret;
//............. plan B avec les poso ......................
requete = QString ( " SELECT                   \n"
                    " t24.f1                   \n"  // as QuUnit
                    " FROM                     \n"
                    " t24,                     \n"
                    " t2P,                     \n"
                    " t00                      \n"
                    " WHERE  t2P.f1 = t00.f0   \n"
                    " AND    t24.f0 = t2P.f8   \n"
                    " AND    t00.f2 = '%1'     \n"  //IN ('3364062', '3176159', '3902447') -- 3364062 / advil    3176159 / HICONCIL
                ).arg(cip);
query.exec(requete );
if (query.isActive() && query.next())
   {ret = CGestIni::Utf8_Query(query, 0);
   }
return ret;
}
//-------------------------------- getFormeFromCIP -----------------------------------------------------------------
/*
SELECT t25.f1  as natureName,
        t00.f2 as Cip
 FROM   t25,
        t00,
        t4B
 WHERE  t00.f2 = '3364062'           -- 3364062 advil susp buvable -- 3690548 morphine   -- 3575327 actifed
 AND    t25.f0 = t4B.f1
 AND    t00.f0 = t4B.f0

SELECT  *
 FROM   t25,
        t00,
        t4B,
        t08
 WHERE  t00.f2 = '3364062'           -- 3364062 advil susp buvable -- 3690548 morphine   -- 3575327 actifed
 AND    t25.f0 = t4B.f1
 AND    t00.f0 = t4B.f0
 AND    t08.f0 = t4B.f0
 limit 0,10000

SELECT  *
 FROM   -- t25,
        -- t24,
        t00,
        t4B,
        t08
 WHERE  t00.f2 = '3364062'           -- 3364062 advil susp buvable -- 3690548 morphine   -- 3575327 actifed
 -- AND    t25.f0 = t4B.f1
 AND    t00.f0 = t4B.f0
 AND    t08.f0 = t4B.f0
 -- AND    t24.f0 = t08.f8
 limit 0,10000

 */
/*! \brief return drug galenic presentation form from cip. (comprimes, suppositoires sachets,ampoules, suspension buvable etc...)
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return QString with presentation form
 */
QString  C_BDM_DatasempPlugin::getFormeFromCIP ( const QString &cip)
{    QString requete = QString (
                                " SELECT t25.f1                 \n"   //  as natureName
                                " FROM   t25,                   \n"
                                "        t00                    \n"
                                " WHERE  t00.f2 = '%1'          \n"  // -- 3690548 morphine   -- 3575327 actifed
                                " AND    t25.f0 = t4B.f1        \n"
                              ).arg(cip);
    QSqlQuery query(requete , database() );
    if (query.isActive() && query.next())
       {return CGestIni::Utf8_Query(query, 0);
       }
    return "";
}
//-------------------------------- getDrugRouteFromCIP ------------------------------------------------------
/*
SELECT t00.f2       as cip,
       t00.f3       as drugName,
       t15.f1       as voie
FROM   t00
      ,t0A
      ,t15
WHERE  t0A.f0=t00.f0
AND    t15.f0=t0A.f1
AND    t00.f2='5610708'
*/
/*! \brief return the drug route from cip.
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return QString with the drug route
 */
QString   C_BDM_DatasempPlugin::getDrugRouteFromCIP ( const QString &cip)
{QString requete = QString (
                             " SELECT               \n"
                             "        t15.f1        \n"    // as voie
                             " FROM   t00           \n"
                             "       ,t0A           \n"
                             "       ,t15           \n"
                             " WHERE  t0A.f0=t00.f0 \n"
                             " AND    t15.f0=t0A.f1 \n"
                             " AND    t00.f2='%1'   \n"
                           ).arg(cip);
 QString ret = "";
 QSqlQuery query(requete , database() );
 if    ( !query.isActive() ) return ret;
 while (  query.next() )
       { ret += CGestIni::Utf8_Query(query, 0) + ", ";
       }
 ret.chop(2);
 return ret;
}

///////////////////////////////////////////////////////// DATASEMP //////////////////////////////////////////////////////////////


//--------------------------- get_SubstancePere -----------------------------------------------------------
/*! \brief return father libelle or a substance
 *  \param const QString code from substance.
 *  \return libelle of father substance or empty if not.
*/
QString C_BDM_DatasempPlugin::get_SubstancePere(const QString &code)
{   return "";
    QString requete = QString (
                              " SELECT   UPPER(t27.f2)    \n"         // 0   libelle de la substance
                              " FROM  t27,                \n"
                              "       t59                 \n"
                              " WHERE t59.f0 = '%1'       \n"         // code du fils
                              " AND   t59.f1 = t27.f0     \n"         // code pere lie a la table des substances
                             ).arg(code);
  QSqlQuery query(requete , database() );
  if (! query.isActive())                             return "";
  QString ret = "";
  while (query.next())   // on suppose que la derniere occurence est la bonne substance exemple : cordarone -->chorydrate puis amiodarone
      { ret = Utf8_Query(query, 0);
      }
  return ret;
}
//-------------------------------- code_CPG_to_DCI_1_2_3 ------------------------------------------------------
/*
SELECT UPPER(t27.f2) as SubName
FROM  t07,
      t27,
      t08
WHERE t07.f0 = '202489'
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t08.f0 = '202489'       -- COLCHIMAX
ORDER BY t07.f3

SELECT UPPER(t27.f2) as SubName,
       t00.f3        as drugName,
       t07.f4        as qu,
       t24.f5        as unit
FROM  t07,
      t27,
      t08,
      t00,
      t24
WHERE t07.f0 = t00.f0 -- t07.f0 = '202489'
AND   t27.f0 = t07.f2
AND   t07.f1 = t08.f1
AND   t08.f1 = '0'
AND   t08.f0 = t00.f0 -- '202489'       -- COLCHIMAX
AND   t24.f0 = t07.f5
AND   t00.f3 like '%COLCHIMAX%'
ORDER BY t00.f3
*/
QStringList C_BDM_DatasempPlugin::code_CPG_to_DCI_1_2_3(const QString &codeCPG)    // code cpg t00.f0
{   QStringList retList;
    /*
    QString requete = QString (
                                " SELECT   UPPER(t27.f2)    \n"         // 0
                                " FROM  t07,                \n"
                                "       t27,                \n"
                                "       t08                 \n"
                                " WHERE t07.f0 = '%1'       \n"
                                " AND   t27.f0 = t07.f2     \n"
                                " AND   t07.f1 = t08.f1     \n"
                                " AND   t08.f1 = '0'        \n"
                                " AND   t08.f0 = '%1'       \n"
                                " ORDER BY t07.f3           \n"
                              ).arg(codeCPG);
    */
    QString requete = QString (
                             // " SELECT   t07.f4,          \n"         // 0   quantite
                             // "          t24.f1,          \n"         // 1   unite quantite au singulier
                                " SELECT   UPPER(t27.f2),   \n"         // 2   0 libelle de la substance
                             // "          t24.f5,          \n"         // 3   unite quantite au pluriel
                                "          t27.f0           \n"         // 4   1 code de la substance
                                " FROM  t07,                \n"
                                "       t27,                \n"
                                "       t08                 \n"
                         //     "       t24                 \n"
                         //     "       t00                 \n"
                         //     " WHERE t00.f2 = '%1'       \n"    // t00.f2 as cip,
                         //     " WHERE t00.f0 = '%1'       \n"    // t00.f0 as cpg,
                         //     " AND   t07.f0 = t00.f0     \n"
                                " WHERE t07.f0 = '%1'       \n"
                                " AND   t27.f0 = t07.f2     \n"
                                " AND   t07.f1 = t08.f1     \n"
                                " AND   t08.f1 = '0'        \n"
                                " AND   t08.f0 = t07.f0     \n"
                         //     " AND   t24.f0 = t07.f5     \n"
                                " ORDER BY t07.f3           \n"
                               ).arg(codeCPG);
    QSqlQuery query(requete , database() );
    if (! query.isActive())                             return retList;
    while (query.next())
       { QString   substance   = get_SubstancePere( query.value(1).toString() );  // on tente la substance pere (vaut mieux exploiter le nommage le plus simple du pere sans les acides chlorydrates ..)
         if (substance.length()==0) substance = CGestIni::Utf8_Query(query, 0);
         retList.append ( substance );
       }
    retList.append("");     // on rajoute 3 vides pour que la liste ait toujours au moins trois donnees
    retList.append("");
    retList.append("");
    return  retList;
    /*
    QString requete = QString (
                                " SELECT   UPPER(t27.f2)    \n"         // 0
                                " FROM  t11,                \n"
                                "       t27                 \n"
                                " WHERE t11.f0 = '%1'       \n"
                                " AND   t11.f1 = t27.f0     \n"
                              ).arg(codeCPG);

    QSqlQuery query(requete , database() );
    if (! query.isActive())                             return retList;
    while (query.next())
       { retList.append ( CGestIni::Utf8_Query(query, 0) );
       }
    retList.append("");     // on rajoute 3 vides pour que la liste ai toujours au moins trois donnees
    retList.append("");
    retList.append("");
    return  retList;
  */
}

//-------------------------------- code_CPG_to_StatutGenerique ------------------------------------------------------
/*
RAPPEL   DU CONTENU DE CETTE TABLE DE LIENS
Vous pouvez d\303\251terminer si une sp\303\251cialit\303\251 appartient au r\303\251pertoire des g\303\251n\303\251riques en \303\251tant soit
     -   un r\303\251f\303\251rent (t3F.f2=1)
     -   un g\303\251n\303\251rique (t3F.f2=6)
     -   un quasi-g\303\251n\303\251rique (t3F.f2=7).
RAPPEL   SUR LES QUASI-GENERIQUES
Sp\303\251cialit\303\251 substituable « S » conform\303\251ment au D\303\251cret 2011-149 du 3 f\303\251vrier 2011 relatif \303\240
l?inscription au r\303\251pertoire des groupes g\303\251n\303\251riques des sp\303\251cialit\303\251s qui se pr\303\251sentent sous une forme
pharmaceutique orale \303\240 lib\303\251ration modifi\303\251e diff\303\251rente de celle de la sp\303\251cialit\303\251 de r\303\251f\303\251rence.


Table 3F : Relation entre produit (r\303\251pertoire des g\303\251n\303\251riques et sp\303\251cialit\303\251
pharmaceutique proche)
Relation entre produit (r\303\251pertoire des g\303\251n\303\251riques et sp\303\251cialit\303\251 pharmaceutique proche). Une
sp\303\251cialit\303\251 pharmaceutique proche est une sp\303\251cialit\303\251 de m\303\252me composition en substance(s)
active(s) de m\303\252me dosage(s) de m\303\252me forme(s) et m\303\252me voie d'administration
     Code                          Description                             Type       Taille Relation
 1           Code du produit                                          [8] Texte
     0                                                                                6          4B.0
             Code du produit membre d'un groupe.                      longueur fixe
             Code du groupe ou sp\303\251cialit\303\251 proche
 2                                                                    [8] Texte
     1       Code du groupe g\303\251n\303\251rique ou de la sp\303\251cialit\303\251                             6          4B.0
                                                                      longueur fixe
             pharmaceutique proche
             Champ de bits qualifiant la relation.
             Champ de bits qualifiant la relation. 1 : R\303\251f\303\251rent 6 :   [2] Entier
     2                                                                                0          1F.0
             G\303\251n\303\251rique 7 : Substituable 8 : Sp\303\251cialit\303\251 pharmaceutique Octet
             proche
             Pointeur \303\251ventuel vers un commentaire
                                                                      [5] Entier long
             Certaines \303\251quivalences sont accompagn\303\251es d'un
     3                                                                (4 octets       0          1C.0
             commentaire, conserv\303\251 dans la table 1C. Dans ce cas, ce
                                                                      binaires)
             champ en contient le code.
             Commentaires sur la participation du produit au groupe   [L] Structure
     5                                                                                0
             Commentaires sur la participation du produit au groupe.  SGML
             Date de mise \303\240 jour de la fiche technique
     C                                                                [S] Date        0          3F.5
             Date de derniere modification du contenu du champ 5.

*/
QString C_BDM_DatasempPlugin::code_CPG_to_StatutGenerique(const QString &codeCPG)
{   int s;
    QSqlQuery query (QString("SELECT f2 FROM t3F WHERE f0 ='%1'").arg(codeCPG) , database() );
    if (query.isActive() && query.next())
       {s = query.value(0).toInt();
        switch(s)
              { case 1: return "R";
                case 6: return "G";
                case 7: return "S";
              }
       }
    return "";
}
//-------------------------------- code_CPG_to_code_UCD ------------------------------------------------------
QString C_BDM_DatasempPlugin::code_CPG_to_code_UCD(const QString &codeCPG)
{   QSqlQuery query (QString("SELECT f1 FROM t0K WHERE f0 ='%1'").arg(codeCPG) , database() );
    if (query.isActive() && query.next())
       {return query.value(0).toString();
       }
    return "";
}
//---------------------------- code_UV_to_dispVilleHopital ---------------------------------------
/*
SELECT t00.f2 as Cip,
       t00.f3 as DrugName,
       t25.f1 as natureName,
       t4B.fA as ATC,
       t02.f2 as codeLieux,
       t1A.f1 as lieux,
       -- t34.f1 as disponibilite,
--       t63.fE as hosto,          -- == HOSTO ->1   != HOSTO --> 0
       t01.f1 as Prix
FROM  t00
     ,t4B
     ,t25
     ,t02
     ,t1A
--     ,t34
--     ,t63
     ,t01
WHERE  t02.f0  = t00.f1
AND    t02.f3  = '0'      -- doit etre disponible
AND    t4B.fA != 'NULL'   -- si c'est un medoc faut un code ATC
AND    t00.f0  = t4B.f0
AND    t25.f0  = t4B.f1
AND     t1A.f0  = t02.f2
-- AND    t34.f0  = t02.f3
-- AND    t00.f1  = t63.f0
AND    t01.f0  = t00.f1
-- AND    t63.fE  = "0"      -- == HOSTO ->1   != HOSTO --> 0
ORDER  by t00.f3
LIMIT 0,30000
*/
QString C_BDM_DatasempPlugin::code_UV_to_dispVilleHopital(const QString &codeUV)
{
    QString requete = QString (
                               " SELECT                   \n"
                               "        t02.f2            \n" // 2 = hopitaux as codeLieux,
                               " FROM   t00               \n"
                               "       ,t02               \n"
                               " WHERE  t02.f0  = t00.f1  \n"
                               " AND    t00.f1  = '%1'    \n"
                              ).arg(codeUV);
    int statut = 4;
    QSqlQuery query (requete , database() );
    if (query.isActive() && query.next())
       { statut = query.value(0).toInt();
         if (statut==2) return "H";
       }
    return "VH";
}
//--------------------------------- Datasemp_LevelToLevelStr ------------------------------------------------------------
/*
case 2999:      // CI absolue
case 2991:      // effet ind\303\251sirable information
case 2988:      // Mise en garde
case 2990:      //  precaution d'emploi
case 2996:      // CI relative
*/
QString   C_BDM_DatasempPlugin::Datasemp_LevelToLevelStr(int niveau)
{ switch (niveau)
  { case  2991: return "0";         // effet indesirable
    case  2990: return "1";         // precaution d'emploi
    case  2988: return "2";         // mise en garde
    case  2996: return "3";         // Contre-indication relative (deconseillee)
    case  2999: return "4";         // Contre-indication absolue
    default:    return "4";
  }
}
//--------------------------------- Datasemp_LevelToLevelInt ------------------------------------------------------------
int   C_BDM_DatasempPlugin::Datasemp_LevelToLevelInt(int niveau)
{ switch (niveau)
  { case  2991: return 0;         // effet indesirable
    case  2990: return 1;         // precaution d'emploi
    case  2988: return 2;         // mise en garde
    case  2996: return 3;         // Contre-indication relative (deconseillee)
    case  2999: return 4;         // Contre-indication absolue
    default:    return 4;
  }
}
//--------------------------------- Utf8_Query ------------------------------------------------------------
QString C_BDM_DatasempPlugin::Utf8_Query(QSqlQuery &cur, int field)
{ return CGestIni::Utf8_Query(&cur, field);
}

//-----------------------------------------------------  Datasemp_SetHTMLFontProperty -------------------------------------------
void C_BDM_DatasempPlugin::Datasemp_SetHTMLFontProperty(const QString &family, const QString &titleSize, const QString &chapitreSize, const QString &paragraphSize, const QString &normalText)
{  m_FontFamily         = family;
   m_FontTitleSize      = titleSize;
   m_FontchapitreSize   = chapitreSize;
   m_FontparagraphSize  = paragraphSize;
   m_FontnormalTextSize = normalText;
}
//------------------------------ Datasemp_get_CIP_From_DatasempIndicationCode ------------------------------------------
/*! \brief select cip list from datasemp indication code
 *  \param const QString &code     datasemp indication code from wich we want datas.
 *  \return QStringList cip result list
 */
QStringList C_BDM_DatasempPlugin::Datasemp_get_CIP_From_DatasempIndicationCode(const QString &code , bool codesAreInList /* = false */)
{
  QStringList retList;
  //.............. on cherche les codes vidal ..................................
  QString requete =
                             " SELECT t00.f2" // 0 as cip
                             " FROM   t52   "
                             "       ,t00   "
                             " WHERE  t52.f1 = t00.f0  "
                             " AND    t52.f2 >= 2993   "
                             " AND    t52.f2 <= 2995   ";
         if (codesAreInList||code.startsWith("'"))
             requete +=  QString (  " AND    t52.f0 IN (%1)  " ).arg(code);    // indication datasemp code sous forme de liste
         else
             requete +=  QString (  " AND    t52.f0 = '%1'  "  ).arg(code);    // indication datasemp code

  QSqlQuery query(requete , database() );
  if    ( !query.isActive() ) return retList;
  while (  query.next() )
        { retList << query.value(0).toString();
        }
 return retList;
}
//------------------------------ Datasemp_get_CIP_From_CIM10 ------------------------------------------
/*! \brief select cip list from cim10 code
 *  \param const QString &code      cim10 code from wich we want datas.
 *  \return QStringList cip result list
 */
QStringList C_BDM_DatasempPlugin::Datasemp_get_CIP_From_CIM10(const QString &code)
{   QStringList  ds_codes;
    //............... on cherche les codes indications datasemp correspondants a ce code cim10 .........
    // bool         run = true;
    int pos          = -1;
    QString codeCimX = code;
    int       testNb = 0;
    do {  //.......... rechercher les codes indications Datasemp a partir des codes CimX .........................
          //           car seuls les codes indications Datasemp sont en relation avec les CIP

          QString requete =       " SELECT DISTINCT(t52.f0)   \n"     // 0 as codeDS,
                                  " FROM   t52,               \n"
                                  "        t55,               \n"
                                  "        t4N                \n"
                                  " WHERE  t55.f1  = t4N.f0   \n"
                                  " AND    t52.f0  = t55.f0   \n"
                                  " AND    t52.f2 >= 2993     \n"
                                  " AND    t52.f2 <= 2995     \n";
          pos = codeCimX.indexOf('-');      // K20-K31
          if (pos==-1)
             { requete += QString (" AND    t4N.f3  = '%1'     \n").arg(codeCimX);                                                   // code cim 10
             }
          else
             { requete += QString (" AND    t4N.f3 >='%1' AND t4N.f3 <= '%2'     \n").arg(codeCimX.left(pos),codeCimX.mid(pos+1));   // code cim 10
             }

          QSqlQuery query(requete , database() );
          if    ( !query.isActive() ) return QStringList();
          while (  query.next() )
                { ds_codes << query.value(0).toString();
                }
          if (ds_codes.size()==0)                                    //--> si pas de resultat on tente avec le pere du code cimX
             { codeCimX = Datasemp_get_Father_From_CIM10(codeCimX);
               if (codeCimX.length()==0) testNb = 2; //run = false;  // si pas de pere on arrete la recherche et on ne reboucle pas
               ++ testNb;                                            // on compte le nombre de fois que le pere est recheche (car une fois suffit)
             }
          else testNb = 2; // run = false;                           //--> Ok on a trouve on se casse
       } while ( /* run &&  */ testNb <= 1);                         // testNb <= 1 car on ne remonte que d'un pere
    if (ds_codes.size()==0)          return QStringList();
    //......... on cherche les cip correspondants aux indications datasemp ..............
    return  Datasemp_get_CIP_From_DatasempIndicationCode( "'"+ds_codes.join("','")+"'", true );   // true pour : on passe une liste de cip et non un cip isole
}

//------------------------------ Datasemp_get_Father_From_CIM10 ------------------------------------------
/*! \brief return ascendant cimX code from a cimX code
 *  \param const QString &code  cimX code from wich we want father
 *  \return QString father cimX code
 */
QString C_BDM_DatasempPlugin::Datasemp_get_Father_From_CIM10(const QString &code)
{
     QString requete = QString ( " SELECT    ts.f3                 \n" //as Cim_Pere
                           //      "        ,ts.f2 as Libelle_Pere \n"
                           //      "        ,td.f3 as Cim_Test     \n"
                           //      "        ,td.f2 as Libelle_Test \n"
                                 " FROM   t4S,                   \n"
                                 "        t4N ts,                \n"
                                 "        t4N td                 \n"
                                 " WHERE  t4S.f0 = ts.f0         \n"
                                 " AND    td.f3  = '%1'          \n"
                                 " AND    t4S.f1 = td.f0         \n"
                               ).arg(code);
     QSqlQuery query(requete , database() );
     if    ( !query.isActive() ) return "";
     while (  query.next() )
           { return query.value(0).toString();
           }
     return "";
}

//------------------------------ Datasemp_get_CIP_From_ASMR_SMR_Code ------------------------------------------
/*! \brief select cip list from  ASMR SMR code.
 *  \param const QString &code      Datasemp code ASMR or SMR from wich we want datas.
 *  \param const QString &typCode =="DS_A" for ASMR code or typCode =="DS_S" for SMR code
 *  \return QStringList cip result list
 */
QStringList C_BDM_DatasempPlugin::Datasemp_get_CIP_From_ASMR_SMR_Code(const QString &code, const QString &typCode )
{
  QStringList retList;
  QString requete = QString (
                             "SELECT t00.f2,"       // 0  as cip,
                             " FROM         "
                             "        t3C,  "
                             "        t00,  "
                             "        t4B   "
                             " where        t4B.f0=t00.f0               "
                             "        and   t3C.f0='%1'                 "  // -- code
                             "        and   t3C.f1=t4B.f0               "
                           ).arg(code);
      if ( typCode=="DS_A" )
         {    requete +=          "        and   (t3C.f2='A')  ";
         }
      else if ( typCode=="DS_S"  )
         {    requete +=          "        and   (t3C.f2='S')  ";
         }
  requete +=                      "        order by t3C.f2     ";
  QSqlQuery query(requete , database() );
  if    ( !query.isActive() ) return retList;
  while (  query.next() )
        { retList << query.value(0).toString();
        }
 return retList;
}
//------------------------------ Datasemp_getIndications_FromCIP ------------------------------------------
/*! \brief select indications ASMR SMR informations for a drug.
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \param const QString &filter   filter wanted datas  a combination | of this flags
 *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
 *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
 *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
 *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
 *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
 *                                  C_BDM_PluginI::REFUND      we check if indications are linked to refund ( m_refundLink will contains price)
 *  \param QString *codeIndicList = 0   if not null datasemp indications codes list will be returned as '1223','2365','987'
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_DatasempPlugin::Datasemp_getIndications_FromCIP(const QString &cip, int filter, QString *codeIndicList /* = 0*/ )
{
  QList <C_BDM_IndicationRecord> retList;
  QList <C_BDM_IndicationRecord> cimList;
  QMap <QString, QString> indic_tarif_Map;
  //.............. on cherche les indications remboursables pour ce cip ..................................
  if ( filter&C_BDM_PluginI::REFUND )  indic_tarif_Map = Datasemp_get_Remboursables_Indic_From_CIP( cip );
  //.............. on cherche les codes vidal ..................................
  QString requete = QString (
                             " SELECT t50.f0, " // 0 as code ,
                             "        t50.f1, " // 1 as indication
                             "        t00.f3  " // 2 as drug name (used in descriptif
                             " FROM   t52,   "
                             "        t00,   "
                             "        t50    "    // table indications
                             " WHERE  t52.f1 = t00.f0  "
                             " AND    t52.f2 = 2994    "
                             " AND    t50.f0 = t52.f0  "
                             " AND    t00.f2 = '%1'    "
                            ).arg(cip);

  QSqlQuery query(requete , database() );
  if    ( !query.isActive() ) return retList;
  while (  query.next() )
        {  QString code_indic = query.value(0).toString();
           QString tarif_remb = "";
           if ( filter&C_BDM_PluginI::REFUND && indic_tarif_Map.contains(code_indic)) tarif_remb = indic_tarif_Map[code_indic];
           C_BDM_IndicationRecord indicRecord ( cip,                             // const QString &cip,
                                                "",                              // const QString &date
                                                "",                              // QString &level,
                                                "",                              // QString &levelTyp,
                                                "",                              // QString &levelLibelle,
                                                CGestIni::Utf8_Query(query, 1),  // QString &indication,
                                                CGestIni::Utf8_Query(query, 2),  // QString &descriptif,  drug name here
                                                code_indic,                      // QString &code,
                                                "DS_I",                          // QString &codeTyp "DS_I"/indication datasemp
                                                tarif_remb                       // tarif remboursable si il existe
                                              );
           retList.append(indicRecord);
       }
 //.......... on cree la liste des codes indications ..................
 int lenListCode  =  retList.size();
 QString codeList = "'";
 if (lenListCode)
    { for (int i = 0; i < lenListCode; ++i)
          { codeList += retList.at(i).code() + "','";
          }
      codeList.chop(2);
    }
 if (codeIndicList) *codeIndicList = codeList;
 //............... on cherche les codes cim10 correspondants a ce code vidal .........
 if ( lenListCode && filter&C_BDM_PluginI::CIM )
    { cimList += Datasemp_get_CIM10_From_Datasemp_Indic(cip, codeList );
    }

 if      ( filter&C_BDM_PluginI::CIM  && filter&C_BDM_PluginI::BDM)  return retList+cimList;
 else if ( filter&C_BDM_PluginI::BDM)                                return retList;
 else if ( filter&C_BDM_PluginI::CIM)                                return cimList;
 else                                                                return retList;
}
//------------------------------ Datasemp_get_Remboursables_Indic_From_CIP ------------------------------------------
/*! \brief select refunds indications list for a drug.
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \return QMap <QString QString> key is indication code, value is price
 */
QMap <QString, QString> C_BDM_DatasempPlugin::Datasemp_get_Remboursables_Indic_From_CIP( const QString &cip )
{   QMap <QString, QString> indic_tarif_Map;
    QString requete = QString (
                                " SELECT t4E.f5, \n" // 0 as tarif,
                                "        t50.f0, \n" // 1 as code_ind,
                                "        t50.f1  \n" // 2 as indication
                                " FROM   t52,    \n"
                                " t00,           \n"
                                " t50,           \n"
                                " t4E,           \n"
                                " t5J,           \n"
                                " t4K            \n"
                                " WHERE  t52.f1  = t00.f0   \n"
                                " AND    t52.f2  = 2994     \n" // -- selectionner indications pour ce CIP
                                " AND    t50.f0  = t52.f0   \n"
                                " AND    t5J.f0  = t4E.f0   \n" // -- lier table tarifs aux indications
                                " AND    t5J.f1  = t50.f0   \n"
                                " AND    t4E.fE !='0'       \n" // -- selectionner celles remboursables
                                " AND    t4E.f0  =t4K.f1    \n"
                                " AND    t00.f1  =t4K.f0    \n" //  -- selectionner ce CIP dans entites remboursables
                                " AND    t00.f2  = '%1'     \n" // -- cip
                                ).arg(cip);
    // qDebug()<<requete;
    QSqlQuery query(requete , database() );
    if    ( !query.isActive() )
          {  outSQL_error(query, tr("<font color=#ff0000><b>ERREUR  : C_BDM_DatasempPlugin::Datasemp_get_Remboursables_Indic_From_CIP()</b></font>"), requete, __FILE__, __LINE__  );
             return indic_tarif_Map;
          }
    while (  query.next() )
          {  indic_tarif_Map.insertMulti(  query.value(1).toString(),       // code indication
                                           query.value(0).toString()        // tarif remboursable
                                        );
          }
    return indic_tarif_Map;
}
//------------------------------ Datasemp_get_CIM10_From_Datasemp_Indic ------------------------------------------
/*! \brief select CIMX indications from datasemp indication code
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \param const QString &code     datasemp indication code of the drug from wich we want datas.
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_DatasempPlugin::Datasemp_get_CIM10_From_Datasemp_Indic(const QString &cip, const QString &code )
{   QList <C_BDM_IndicationRecord> retList;
    //............... on cherche les codes cim10 correspondants a ce code vidal .........
    QString requete = " SELECT                  "
                      "  DISTINCT (t4N.f0)      "     // 0 as pk
                      "        ,t4N.f3          "     // 1 as code CIM10,
                      "        ,t4N.f2          "     // 2 AS libelle_CIM_X
                      " FROM   t55              "
                      "        ,t4N             "
                      " WHERE  t55.f1 = t4N.f0  ";
    if (code.startsWith("'"))
       {requete += QString (" AND    t55.f0  IN (%1) " ).arg(code);   // code indication vidal sous forme de liste '123','456','789'
       }
    else
       {requete += QString (" AND    t55.f0  = '%1'  " ).arg(code);   // code indication vidal
       }
    QSqlQuery query(requete , database() );
    if    ( !query.isActive() ) return retList;
    while (  query.next() )
          { retList.append( C_BDM_IndicationRecord( cip,                              // const QString &cip,
                                                    "",                               // const QString &date
                                                    "",                               // QString &level,
                                                    "",                               // QString &levelTyp,
                                                    "",                               // QString &levelLibelle,
                                                    CGestIni::Utf8_Query(query, 2),   // QString &indication,query.value(4).toString()
                                                    "",                               // QString &descriptif,
                                                    query.value(1).toString(),        // QString &code,
                                                    "CIM"                             // QString &codeTyp "DS_I"/indication datasemp
                                                  )
                          );
          }
    return retList;
}

//------------------------------ Datasemp_getASMR_SMR_FromCIP ------------------------------------------
/*! \brief select indications ASMR SMR informations for a drug.
 *  \param const QString &cip      code CIP of the drug from wich we want datas.
 *  \param const QString &filter   filter wanted datas  a combination | of this flags
 *                                  C_BDM_PluginI::ASMR        we want indications with ASMR level (if exists)
 *                                  C_BDM_PluginI::SMR         we want indications with SMR level (if exists)
 *                                  C_BDM_PluginI::CIM         we want coded CIMX indications
 *                                  C_BDM_PluginI::BDM         we want specifics database coded indications
 *                                  C_BDM_PluginI::ALL_TYPE    we want all coded indications
 *  \return QList <C_BDM_IndicationRecord> indications result list
 */
QList <C_BDM_IndicationRecord> C_BDM_DatasempPlugin::Datasemp_getASMR_SMR_FromCIP(const QString &cip, int filter )
{
  QList <C_BDM_IndicationRecord> retList;
  QString requete = QString (
                             "SELECT t00.f2,"       // 0  as cip,
                             "       t3C.f0,"       // 1  as id_indic,
                             "       t00.f3,"       // 2  as drugName,
                             "       t3C.f3,"       // 3  as niveau,
                             "       t3E.f1,"       // 4  as libelleNiveau,
                             "       t3E.f2,"       // 5  as infoNiveau,
                             "       t3C.f4,"       // 6  as indication,
                             "       t3C.f2,"       // 7  as type A ou S
                             "       t3C.f5,"       // 8  as date
                             "       t3C.f8 "       // 9  as memo
                             " FROM         "
                             "        t3C,  "
                             "        t3E,  "
                             "        t00,  "
                             "        t4B   "
                             " where  t4B.f0=t00.f0                     "
                             "        and   t00.f2='%1'                 "  // -- cip
                             "        and   t3C.f1=t4B.f0               "
                             "        and   t3C.f3=t3E.f0               "
                           ).arg(cip);
      if ( filter&C_BDM_PluginI::ASMR && filter&C_BDM_PluginI::SMR )
         {    requete +=          "        and   (t3C.f2='A' or t3C.f2='S')  ";
         }
      else if ( filter&C_BDM_PluginI::ASMR )
         {    requete +=          "        and   (t3C.f2='A')  ";
         }
      else if ( filter&C_BDM_PluginI::SMR )
         {    requete +=          "        and   (t3C.f2='S')  ";
         }
  requete +=                      "        order by t3C.f2     ";
  QSqlQuery query(requete , database() );
  if    ( !query.isActive() ) return retList;
  while (  query.next() )
        { QString levelTyp  = query.value(7).toString();
          retList.append( C_BDM_IndicationRecord( cip,                                                  // const QString &cip,
                                                  query.value(8).toDate().toString("dd-MM-yyyy"),       // const QString &date
                                                  (levelTyp=="A")? "A"+Datasemp_ASMR_Level_To_level (query.value(3).toString())
                                                                 : "S"+query.value(3).toString(),       // QString &level,
                                                  levelTyp,                                             // QString &levelTyp A ou S
                                                  CGestIni::Utf8_Query(query, 4),  // query.value(4).toString(),       // QString &levelLibelle,
                                                  query.value(6).toString(),       // QString &indication,CGestIni::Utf8_Query(query, 6)
                                                  CGestIni::Utf8_Query(query, 8),  // QString &descriptif,
                                                  query.value(1).toString(),       // QString &code,
                                                  "DS_"+levelTyp                   // QString &codeTyp "DS_A"/indication ASMR    "DS_S"/indication SMR
                                                )
                        );
       }
 return retList;
}


//------------------------------ Datasemp_ASMR_Level_To_level ------------------------------------------
/*! \brief return normalised ASMR number betwen  0 to 4 from datasem level
 *  \param const QString &level            datasem level
 *  \return QString contains normalised level
 */
QString  C_BDM_DatasempPlugin::Datasemp_ASMR_Level_To_level (    const QString &level)
{switch(level.toInt())
    { case 105 : return "0";   // pas d'amelioration
      case 104 : return "1";   // amelioration mineure
      case 103 : return "2";   // modere
      case 102 : return "3";   // importante
      case 101 : return "4";   // majeure
      default  : return  "";   // chais pas c'est quoi
    }

}
//-----------------------------------------------------  Datasemp_GetMonographie -------------------------------------------
//                  la norme vidal est que les noms des images des mononographies sont en majuscules : src="IMAGES/B00/C01/D35.GIF"
QString   C_BDM_DatasempPlugin::Datasemp_GetMonographie(const QString &code_UV)
{ QString  result = "";
  int       f     = -1;
  //................. chercher si existe une  monographie vidal pour ce produit...................................................
  //                              recuperer code document de la monographie
  QSqlQuery query (QString( " SELECT t5C.f3          "
                            " FROM   t5D, t5C        "
                            " WHERE  t5C.f0 = t5D.f1 "
                            " AND    t5D.f0 = '%1'").arg( code_UV ) , database() );

  if (query.isActive() &&query.next())                           result     =  query.value(0).toString();
  if ( (f=result.indexOf("<html",0, Qt::CaseInsensitive)) > 0)   result.remove (0, f );
  return result;
}
//-----------------------------------------------------  Datasemp_GetMonographieFromCodeProduit -------------------------------------------
//                  la norme vidal est que les noms des images des mononographies sont en majuscules : src="IMAGES/B00/C01/D35.GIF"
QString   C_BDM_DatasempPlugin::Datasemp_GetMonographieFromCodeProduit(const QString &code_produit)
{ QString  result = "";
  int       f     = -1;
  //................. chercher si existe une  monographie vidal pour ce produit...................................................
  //                              recuperer code document de la monographie
  QSqlQuery query (QString( " SELECT t5C.f3          "
                            " FROM   t5E, t5C        "
                            " WHERE  t5C.f0 = t5E.f1 "
                            " AND    t5E.f0 = '%1'").arg( code_produit ) , database() );

  if (query.isActive() &&query.next())                           result     =  query.value(0).toString();
  if ( (f=result.indexOf("<html",0, Qt::CaseInsensitive)) > 0)   result.remove (0, f );
  return result;
}
//-------------------------------------- Datasemp_SGML_to_html -------------------------------------------------------
QString C_BDM_DatasempPlugin::Datasemp_SGML_to_html( QString& input, QString  name  /* = QString::null */ )
{
 int xmlDepth      = -1;
 //................ convertir les retours chariot en trucs non XML (ne doivent pas etres interprÃ©tÃ©s) ................
 input.replace("<BR/>","{{@}}");
 QDomDocument doc( "mydocument" );
 if ( !doc.setContent( input ) ) return "";

 // input =  "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
 //         "<body style=\"font-size:16pt;font-family:Arial\">";

 if (name.length())
    { input += "<p align=\"center\"><span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontTitleSize+"pt;font-style:italic;font-weight:600;color:#AA0000\">";
      input += name  +  "</span></p> <hr size=\"2\" />";
    }
 Datasemp_Xml_ListChild(input, doc.documentElement(), xmlDepth );
 // input     += "</body></html>";
 input.replace("\303\242","\'");
 return input;
}
//-------------------------------------- Datasemp_Xml_to_html -------------------------------------------------------
void C_BDM_DatasempPlugin::Datasemp_Xml_to_html( QString& input, QString  name  /* = QString::null */ )
{
 int xmlDepth      = -1;
 //................ convertir les retours chariot en trucs non XML (ne doivent pas etres interprÃ©tÃ©s) ................
 input.replace("<BR/>","{{@}}");
 QDomDocument doc( "mydocument" );
 if ( !doc.setContent( input ) ) return;

 input =  "<html><head><meta name=\"qrichtext\" content=\"1\" /></head>"
          "<body style=\"font-size:16pt;font-family:Arial\">";

 if (name != QString::null)
    {input += "<p align=\"center\"><span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontTitleSize+"pt;font-style:italic;font-weight:600;color:#AA0000\">";
     input += name  +  "</span></p> <hr size=\"2\" />";
    }
 Datasemp_Xml_ListChild(input, doc.documentElement(), xmlDepth );
 input     += "</body></html>";
 input.replace("\303\242","\'");
}

//-------------------------------------- Datasemp_Xml_ListChild -------------------------------------------------------
void C_BDM_DatasempPlugin::Datasemp_Xml_ListChild(QString& input, const QDomElement &parentElement, int &xmlDepth )
{   ++xmlDepth;
    QString ofset = "";
    for (int i=0; i<xmlDepth; i++) ofset += "\t";
    QDomNode node = parentElement.firstChild();
    while ( !node.isNull() )
       {if ( node.isElement() /* && node.nodeName() == "outline" */)
           {QDomElement e   = node.toElement();
            QString text    = e.text();
            QString tagName = e.tagName();
            QString title   = e.attribute("TITRE");
            int         tag = 1;
            if (tagName=="RUBRIQUE")
               {input +=  ofset + "<br /><span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontTitleSize+"pt;font-weight:600;text-decoration:underline;color:#AA0000\">";
                input +=  title + "</span><br />";
               }
            else if (tagName=="SSRUBR")
               {title.replace(">", "&gt;");
                title.replace("<", "&lt;");
                input += ofset + "<span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontchapitreSize+"pt;font-weight:600;text-decoration:underline;color:#000000\">";
                input += title + "</span><br />";
               }
            else if (tagName=="CAS")
               {title.replace(">", "&gt;");
                title.replace("<", "&lt;");
                input += ofset + "<span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontparagraphSize+"pt;font-weight:600;color:#0000FF\">";
                input += title + "</span><br />";
               }
            else tag=0;
            if (tag && text!="")
               {text.replace(">", "&gt;");
                text.replace("<", "&lt;");
                text.replace("{{@}}", " <br />" + ofset); // replacer les retours chariots normaux
                input += ofset + "<span style=\"font-family:"+m_FontFamily+";font-size:"+m_FontnormalTextSize+"pt;color:#00007F\">";
                input += text  + "</span><br />";
               }
            //........... APPEL RECURSIF ...........
            Datasemp_Xml_ListChild( input, node.toElement(), xmlDepth );
           }
        node = node.nextSibling();
       }
    --xmlDepth;
}

//----------------------------------------------- Datasemp_GetBaseVersion ---------------------------------------------------------------------
QString C_BDM_DatasempPlugin::Datasemp_GetBaseVersion( QString *numEdition, QString *packBCArrete /* = 0 */, QString *packIRArrete /* = 0 */)
{ QString ods("");
  if      (packBCArrete ) *packBCArrete = "";
  if      (packIRArrete)  *packIRArrete = "";
  if      (numEdition)    *numEdition = "";

  //.................. Preparer la requete .....................................
  QSqlQuery query("SELECT f0,f1 FROM tFB", database() );
  //.................. si la requete a un resultat .............................
  if (query.isActive())
     {while (query.next())
            {if      (packBCArrete && query.value(0).toString()=="ARRETE PACK BC") *packBCArrete = query.value(1).toString();
             else if (packIRArrete && query.value(0).toString()=="ARRETE PACK IR") *packIRArrete = query.value(1).toString();
             else if (numEdition   && query.value(0).toString()=="EDITION NUMERO") *numEdition   = query.value(1).toString();
             else if (query.value(0).toString()=="ODS")                             ods          = query.value(1).toString();
            }
     }
  QString err;
  // OutSQL_error( query, "Datasemp_GetBaseVersion()", "SELECT f0,f1, FROM tFB", &err);
  return ods.remove("#00");
}
