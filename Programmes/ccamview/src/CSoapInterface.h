/***************************************************************************
 *   Copyright (C) 2004 by Sébastien SIVERA & Sevin Roland                 *
 *   upsis@club-internet.fr                                                *
 *   #include "CSoapInterface.h"                                           *
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

#ifndef  CSOAP_INTERFACE_H
#define  CSOAP_INTERFACE_H

//=============================================== INCLUDES ======================================================================
#include <q3http.h> 
#include <qxml.h>
#include <qobject.h>
#define TR  QObject::tr
//=============================================== DEFINE ========================================================================
#define  PAT_IDENTIFICATION              "WsPatientIdentification"
#define  PAT_IDENTIFICATION_LIST         "searchPatientReturn"

#define  WS_DOSSIER                      "selectDossierReturn"    //selectDossierReturn  WsDossier
#define  WS_DOSSIER_LIST                 "searchDossierReturn"

#define  WS_ACTECCAM                     "WsActeCcam"             
#define  WS_ACTECCAM_LIST                "searchActeTempReturn"  // ou "searchActeReturn"

#define WS_ACTEDEL                       "deleteActeTempReturn"

#define  BASE_IDENTIFICATION             "DatabaseIdentification"
#define  BASE_IDENTIFICATION_LIST        "getDatabasesReturn"
#define  BASE_SELECT_PATIENT             "selectPatientReturn"

#define  PARSE_STATE_NOTPARSED                0
#define  PARSE_STATE_BEGIN                    1
#define  PARSE_STATE_FINISHED                 2


//===================================== CSigemsReturn ==================================================================
class CSigemsReturn 
{ 
 public:
                CSigemsReturn(){}
  virtual      ~CSigemsReturn(){}
  void          Init (const char* type)   {m_Type       = type;  }  
  virtual int   AddData(const QString &, const char *, QString &,  QString &){  return 0; }  
  QString       m_Type;
};

//===================================== WsActeDel ==================================================================
class WsActeDel : public CSigemsReturn
{ 
 public:    
   WsActeDel()                          {Init(WS_ACTEDEL);
                                         _Clear();
                                        }
   WsActeDel(const char* type)          {Init(type);
                                         _Clear();
                                        }
  ~WsActeDel(){}
  void  _Clear()                        {m_ParseState                = PARSE_STATE_NOTPARSED;
                                        }
  int   _AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  int    AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  QString                    m_RetCode;
  int                        m_ParseState;
  
  //.................... variables Sigems ...........................
};

//===================================== WsActeCcam ==================================================================
class WsActeCcam : public CSigemsReturn
{ 
 public:    
   WsActeCcam()                          {Init(WS_ACTECCAM);
                                         _Clear();
                                        }
   WsActeCcam(const char* type)          {Init(type);
                                         _Clear();
                                        }
  ~WsActeCcam(){}
  void  _Clear()                        {m_numDossier                    = "";
                                         m_ippSigems                     = "";
                                         m_uniteMedicale                 = "";
					 m_enfant                        = "";
                                         m_codeActe                      = "";
                                         m_activite                      = ""; 
                                         m_extensionDocumentaire         = "";
                                         m_phase                         = "";
                                         m_modificateur1                 = "";
                                         m_modificateur2                 = "";
                                         m_modificateur3                 = "";
                                         m_modificateur4                 = "";
                                         m_associationNonPrevue          = "";
                                         m_remboursementExceptionnel     = "";
                                         m_chargeSupplementaire          = "";
                                         m_nbOccurence                   = "";
                                         m_date                          = "";  
                                         m_heureDebut                    = "";
                                         m_idPraticien                   = "";
					 m_lienAvecActeExonereAnterieur  = "";
					 m_prescripteur                  = "";
                                         m_depassementHonoraire          = "";
                                         m_codeRejet                     = "";
                                         m_ParseState                    = PARSE_STATE_NOTPARSED;
                                        }
  int   _AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  int    AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  QString                    m_RetCode;
  int                        m_ParseState;
  
  //.................... variables Sigems ...........................
  QString                    m_activite;
  QString                    m_associationNonPrevue;
  QString                    m_chargeSupplementaire;
  QString                    m_codeActe;
  QString                    m_date;  
  QString                    m_enfant;
  QString                    m_extensionDocumentaire;
  QString                    m_heureDebut;
  QString                    m_idPraticien;
  QString                    m_ippSigems;
  QString                    m_modificateur1;
  QString                    m_modificateur2;
  QString                    m_modificateur3;
  QString                    m_modificateur4;
  QString                    m_nbOccurence;
  QString                    m_numDossier;
  QString                    m_phase;
  QString                    m_remboursementExceptionnel;
  QString                    m_uniteMedicale;
  QString                    m_lienAvecActeExonereAnterieur;
  QString                    m_prescripteur;
  QString                    m_depassementHonoraire;
  QString                    m_codeRejet;
  };

//===================================== WsActeCcamListe ==================================================================
class WsActeCcamListe : public CSigemsReturn
{ 
 public:
  WsActeCcamListe()              {Init(WS_ACTECCAM_LIST);
                                  m_Nb                       = 0;
				  m_pWsActeCcam              = 0;
				  m_Current                  = -1;
				  m_RetCode                  = "";
                                 }
  WsActeCcamListe(QString typ)   {Init(typ);
                                  m_Nb                       = 0;
				  m_pWsActeCcam              = 0;
				  m_Current                  = -1;
				  m_RetCode                  = "";
                                 }
  ~WsActeCcamListe()             {if (m_pWsActeCcam) delete[]m_pWsActeCcam; 
                                      m_pWsActeCcam = 0;
                                 }
  int   AddData(const QString &dataName, const char* Data,  QString &attName,  QString &attValue);
  int  _AddData(const QString &dataName, const char *Data,  QString &attName,  QString &attValue);
  QString                    m_RetCode;
  long                       m_Nb;
  long                       m_Current;
  
  //.................... variables Sigems ...........................
  WsActeCcam                *m_pWsActeCcam; 
}; 

//===================================== WsDossier ==================================================================
class WsDossier : public CSigemsReturn
{ 
 public:    
   WsDossier()                          {Init(WS_DOSSIER);
                                         _Clear();
                                        }
   WsDossier(const char* type)          {Init(type);
                                         _Clear();
                                        }
  ~WsDossier(){}
  void  _Clear()                        {m_numero               = ""; 
                                         m_ippSigemsPatient     = "";
                                         m_ippSigemsAssure      = "";
                                         m_dateEntree           = "";
                                         m_heureEntree          = "";  
                                         m_dateSortiePrevue     = "";
                                         m_dateSortieReelle     = "";
                                         m_heureSortie          = "";
                                         m_modeTraitement       = "";
                                         m_service              = "";
                                         m_motif                = "";
                                         m_typeChambre          = "";
                                         m_dateIntervention     = "";
                                         m_heureIntervention    = "";
                                         m_idMedecinTraitant    = "";
                                         m_idChirurgien         = "";
                                         m_idAnesthesiste       = "";
					 m_ParseState           = PARSE_STATE_NOTPARSED;
                                        }
  int   _AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  int    AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  QString                    m_RetCode;
  int                        m_ParseState;
  
  //.................... variables Sigems ...........................
  QString                    m_numero;
  QString                    m_ippSigemsPatient;
  QString                    m_ippSigemsAssure;
  QString                    m_dateEntree;
  QString                    m_heureEntree;  
  QString                    m_dateSortiePrevue;
  QString                    m_dateSortieReelle;
  QString                    m_heureSortie;
  QString                    m_modeTraitement;
  QString                    m_service;
  QString                    m_motif;
  QString                    m_typeChambre;
  QString                    m_dateIntervention;
  QString                    m_heureIntervention;
  QString                    m_idMedecinTraitant;
  QString                    m_idChirurgien;
  QString                    m_idAnesthesiste;
 };

//===================================== WsDossierListe ==================================================================
class WsDossierListe : public CSigemsReturn
{ 
 public:
  WsDossierListe()               {Init(WS_DOSSIER_LIST);
                                  m_Nb                       = 0;
				  m_pWsDossier               = 0;
				  m_Current                  = -1;
				  m_RetCode                  = "";
                                 }
  ~WsDossierListe()              {if (m_pWsDossier) delete[]m_pWsDossier; 
                                      m_pWsDossier = 0;
                                 }
  int   AddData(const QString &dataName, const char* Data,  QString &attName,  QString &attValue);
  int  _AddData(const QString &dataName, const char *Data,  QString &attName,  QString &attValue);
  QString                    m_RetCode;
  long                       m_Nb;
  long                       m_Current;
  
  //.................... variables Sigems ...........................
  WsDossier                 *m_pWsDossier; 
}; 

//===================================== DatabaseIdentificationListe ==================================================================
class WsPatient : public CSigemsReturn
{ 
 public:
  WsPatient()                    {Init(BASE_SELECT_PATIENT);
				  m_RetCode                  = "";
                                 }
  ~WsPatient()                   { }
  int   AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);

  QString                    m_RetCode;
  
  //.................... variables Sigems ...........................
  QString                    m_adresse1;
  QString                    m_adresse2;
  QString                    m_cleSecu;
  QString                    m_codePostal;
  QString                    m_dateNaissance;    
  QString                    m_ippExtern;
  QString                    m_ippSigems;
  QString                    m_nomNaissance;
  QString                    m_nomUsuel;
  QString                    m_numSecu;
  QString                    m_prenom;
  QString                    m_sexe;
  QString                    m_telephone;
  QString                    m_ville;
  QString                    m_villeNaissance;
};

//===================================== DatabaseIdentification ==================================================================
class DatabaseIdentification : public CSigemsReturn
{ 
 public:    
  DatabaseIdentification()              {Init(BASE_IDENTIFICATION);
                                         m_description      = "";
					 m_identifiant      = "";
				         m_ParseState       = PARSE_STATE_NOTPARSED;
                                        }
  ~DatabaseIdentification(){}
  int   AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  int                        m_ParseState;
  
  //.................... variables Sigems ...........................
  QString                    m_description;
  QString                    m_identifiant;  
 };

//===================================== DatabaseIdentificationListe ==================================================================
class DatabaseIdentificationListe : public CSigemsReturn
{ 
 public:
  DatabaseIdentificationListe()  {Init(BASE_IDENTIFICATION_LIST);
                                  m_Nb                       = 0;
				  m_pDatabaseIdentification  = 0;
				  m_Current                  = -1;
				  m_RetCode                  = "";
                                 }
  ~DatabaseIdentificationListe() {if (m_pDatabaseIdentification) delete[]m_pDatabaseIdentification; 
                                      m_pDatabaseIdentification = 0;
                                 }
  int   AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  QString                    m_RetCode;
  long                       m_Nb;
  long                       m_Current;
  
  //.................... variables Sigems ...........................
  DatabaseIdentification    *m_pDatabaseIdentification; 
}; 

//===================================== WsPatientIdentification ==================================================================
class WsPatientIdentification : public CSigemsReturn
{ 
 public:    
  WsPatientIdentification()             {Init(PAT_IDENTIFICATION);
                                         m_ippSigems      = "";
					 m_nomNaissance   = "";
					 m_nomUsuel       = "";
					 m_prenom         = "";
					 m_dateNaissance  = "";
					 m_ParseState     = PARSE_STATE_NOTPARSED;
                                        }
  ~WsPatientIdentification(){}
  int   AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  int                        m_ParseState;
  
  //.................... variables Sigems ...........................
  QString                    m_ippSigems;
  QString                    m_nomNaissance;
  QString                    m_nomUsuel;
  QString                    m_prenom;
  QString                    m_dateNaissance;  
 };

//===================================== WsPatientIdentificationListe ==================================================================
class WsPatientIdentificationListe : public CSigemsReturn
{ 
 public:
  WsPatientIdentificationListe() {Init(PAT_IDENTIFICATION_LIST);
                                  m_Nb                       = 0;
				  m_pWsPatientIdentification = 0;
				  m_Current                  = -1;
				  m_RetCode                  = "";
                                 }
  ~WsPatientIdentificationListe(){if (m_pWsPatientIdentification) delete[]m_pWsPatientIdentification; 
                                      m_pWsPatientIdentification = 0;
                                 }
  int   AddData(const QString &dataName, const char *Data, QString &attName,  QString &attValue);
  QString                    m_RetCode;
  long                       m_Nb;
  long                       m_Current;
  
  //.................... variables Sigems ...........................
  WsPatientIdentification   *m_pWsPatientIdentification; 
}; 



//===================================== CSoapGenericXMLHandler ==================================================================
class CSoapGenericXMLHandler : public QXmlDefaultHandler
{
 //............................... METHODES ............................................................................     
 public:
        //................................... contructeur destructeur ..................................................
        CSoapGenericXMLHandler();

	//................................... autres methodes ..........................................................
        bool startDocument();
	bool startElement( const QString&, const QString&, const QString& , const QXmlAttributes& );
	bool endElement( const QString&, const QString&, const QString& );
	bool processingInstruction ( const QString & target, const QString & data );
	bool characters ( const QString & ch );
	bool comment( const QString & ch );
	//bool startEntity ( const QString & name );
	bool startCDATA ();
	bool endCDATA();		
	void SetResult(CSigemsReturn*   pResult){m_pResult=pResult;}
  //............................... DATA ................................................................................
 private:
	CSigemsReturn*   m_pResult;
	QString          m_DataName;
	int              m_WantCar;
};

//===================================== CSoapInterface ========================================================================
//  Pour que soit supporté Q_OBJECT ici
//  il faut désactivé la case à cocher Thread dans la boite de config de QMake
class CSoapInterface : public Q3Http
{
 //Q_OBJECT    
 //............................... METHODES ............................................................................     
 public:
    
 //................................... contructeur destructeur ........................................................
    CSoapInterface(); 
    // CSoapInterface ( QObject * parent =0 , const char * name = 0 );                   
    ~CSoapInterface(){disconnect( this, 0, 0, 0 );};
    
 //................................... autres methodes ..........................................................
  QString            SendMessage(QString             url_string,  
                                 QString             soapaction_string, 
				 QString             message,
				 QString             multiRef           ="",
				 QString             contenttype_string ="text/xml; charset=\"utf-8\"", 
				 QString             proxy_string       = "" ,
				 QString             proxy_port         = "80" 
				);
  Q3HttpRequestHeader PrepareHttpRequestHeader(QString             url_string,  
					      QString             soapaction_string, 
					      QString             contenttype_string ="text/xml; charset=\"utf-8\"", 
					      QString             proxy_string       = "" ,
					      QString             proxy_port         = "80" 
					     );
  QString            CreateSOAPMessage(QString &action,
                                       QString &message,
				       QString &multiRef,
				       QString ns_string   = "\n                    xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
                                                             "                    xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" ,
				       bool sendRawMess    = FALSE 
				      );
  int              GetXML_Part( QString &txt,  QString& xml, QString* http_header = 0);
  bool             XML_To_Colored( const QString& xml, QString& result );
  bool             XML_To_SigemsResult( const QString& xml, CSigemsReturn* pPesult );
  void             DisplaySigemsError(const QString &error_str);
  QString          ResponseHeaderToStr(const Q3HttpResponseHeader& response );
  QString          GetResponseHeader();
  QString          GetResponseBody(bool error);
  void             SetDebugMode(const QString &file, int debug_mode=FALSE);
  void             GotoDebug();
  
  int Base64_encode_len(int len);
  int Base64_encode(char *encoded, const char *str, int len);  
  
   //............................... DATA .............................................................................
 public: 
    int     m_TestFlag;
    QString m_ResponseTestFile;
    QString m_ResponseHeader;
    QString m_ResponseBody;
};


//=============================================== SOAPSyntaxHighlighter =========================================================
class SOAPSyntaxHighlighter : public QXmlDefaultHandler
{
public:
	SOAPSyntaxHighlighter();
	bool startDocument();
	bool startElement( const QString&, const QString&, const QString& , const QXmlAttributes& );
	bool endElement( const QString&, const QString&, const QString& );
	bool processingInstruction ( const QString & target, const QString & data );
	bool characters ( const QString & ch );
	bool comment( const QString & ch );
	//bool startEntity ( const QString & name );
	bool startCDATA ();
	bool endCDATA();
	
	QString getResult();

private:
	QString encodeXMLEntities( const QString& ) const;
	
	QString m_indent;
	QString m_Result;
	bool    m_StartedElement;
	bool    m_inCDATA;
        //bool    m_IsDataValue;
	QString m_XMLProcessingMarkupColor;
	QString m_XMLElementColor;
	QString m_XMLDataColor;
	QString m_XMLDTDColor;
	QString m_XMLAttributeNameColor;
	QString m_XMLAttributeValueColor;
	QString m_XMLCommentColor;
	QString m_XMLErrorColor;
	QString m_XMLCDATATagColor;
	QString m_XMLCDATAColor;
};

      

#endif



