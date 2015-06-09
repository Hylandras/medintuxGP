/*
 * -------------------------------------------------------------------
 * (c) 2001-2003 GIE SESAM-VITALE
 *
 * PROJET   : Services SESAM Vitale
 *
 * FICHIER  : ssv.h (v5)
 *
 * Declaration des prototypes des fonctions SSV
 * -------------------------------------------------------------------
 */

#ifndef __SSV_H__
#define __SSV_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Inclusions
 */
#include <stddef.h>
/* Inclure le fichier de déclarations spécifiques système :
   win32def.h, macosdef.h, aixdef.h, etc. */


/****** FONCTIONS STANDARDS *********/
						
typedef unsigned short (API_ENTRY TFCTSSVTRADUIREARL)
						 (short                   NbZDataIn,
                          void   FARPTR FARPTR    TZDataIn,
                          size_t FARPTR           TTailleZoneIn,
                          void FARPTR FARPTR		pZDataOut,
						  size_t FARPTR			pTailleZoneOut);

typedef unsigned short (API_ENTRY TFCTSSVLIRECARTEPS)
                (const char FARPTR NomRessourcePS, 
                 const char FARPTR NomRessourceLecteur, 
                 const char FARPTR CodePorteurPS, 
                 void FARPTR FARPTR pZDataOut, 
                 size_t FARPTR pTailleZone);


typedef unsigned short (API_ENTRY TFCTSSVLIRECONFIG)
				(void FARPTR FARPTR pZDataOut, 
				 size_t FARPTR psTailleDataOut);

typedef unsigned short (API_ENTRY TFCTSSVFORMATERFAC)
						(const char FARPTR NomRessourcePS,
						const char FARPTR NomRessourceLecteur,
						const char FARPTR CodePorteurPS,
						char cFactureACreer,
						char cModeSecur,
						void FARPTR pZDataIn,
						size_t TailleDataIn,
						void FARPTR FARPTR pZDataOut,
						size_t FARPTR pTailleZone);

typedef unsigned short (API_ENTRY TFCTSSVFORMATERLOT)
						(	const char FARPTR	NomRessourcePS,
              			 	const char FARPTR	NomRessourceLecteur,
               				const char FARPTR	CodePorteurPS,
               				short				NBZDataIn,
               				void FARPTR FARPTR	TZDataIn,
               				size_t	FARPTR		TTailleZoneIn,
			   				short FARPTR 		pNbZDataOut,
                			void FARPTR FARPTR	TZDataOut,
                			size_t	FARPTR		TTailleZoneOut);

typedef unsigned short (API_ENTRY TFCTSSVFORMATERFIC)
						 (void FARPTR pZDataIn,size_t TailleDataIn,
						 void FARPTR FARPTR pZDataOut,size_t FARPTR pTailleZone);


typedef unsigned short  (API_ENTRY TFCTSSVINITTRACE)
						(char FARPTR NomFichier, char FARPTR ModeOuverture,
						 unsigned short Module, unsigned char Niveau);
						  

typedef unsigned short  (API_ENTRY TFCTSSVALLOUER)
						(void FARPTR FARPTR pZDataIn,
						 size_t taille);
						     
typedef void (API_ENTRY TFCTSSVLIBERER)
						(void FARPTR pZone);

typedef unsigned short  (API_ENTRY TFCTSSVMAJDATE)
						(const char FARPTR pcNomRessourceLecteur,
						 const char FARPTR pcDateHeure);

typedef unsigned short  (API_ENTRY TFCTSSVLIREDATE)
						(const char FARPTR pcNomRessourceLecteur,
						 char FARPTR pcDateHeure);


typedef unsigned short (API_ENTRY TFCTSSVLIREDROITS)
                (const char FARPTR NomRessourcePS, 
                 const char FARPTR NomRessourceLecteur, 
                 const char FARPTR CodePorteurPS, 
                 const char FARPTR DateConsultation, 
                 void FARPTR FARPTR pZDataOut, 
                 size_t FARPTR pTailleZone);


typedef unsigned short  (API_ENTRY TFCTSSVIDENTIFIERTLA)
						(const char FARPTR pcNomRessourceLecteur, 
						 const char FARPTR NumVersionCDC,
	    				 void FARPTR FARPTR pZDataOut, 
	    				 size_t FARPTR tailleDataOut);

typedef unsigned short (API_ENTRY TFCTSSVDECHGFACTPDT)
						(	const char FARPTR	NomRessourcePS,
              			 	const char FARPTR	NomRessourceLecteur,
               				const char FARPTR	CodePorteurPS,
               				const char FARPTR	pcNumFact,
							short sNbZDataIn,
						    void FARPTR FARPTR pvTZDataIn, 
						    size_t FARPTR psTTailleDataIn,
			   				short FARPTR 		pNbZDataOut,
                			void FARPTR FARPTR	TZDataOut,
                			size_t	FARPTR		TTailleZoneOut);

typedef unsigned short (API_ENTRY TFCTSSVDECHGFSETLA)
						(	const char FARPTR	NomRessourcePS,
              			 	const char FARPTR	NomRessourceLecteur,
               				const char FARPTR	CodePorteurPS,
               				const char FARPTR	pcNumFact,
			   				short FARPTR 		pNbZDataOut,
                			void FARPTR FARPTR	TZDataOut,
                			size_t	FARPTR		TTailleZoneOut);

typedef unsigned short (API_ENTRY TFCTSSVEFFACERTLA)
					(const char FARPTR NomRessourcePS,
					 const char FARPTR NomRessourceLecteur,
					 const char FARPTR CodePorteurPS, 	
					 const char FARPTR cNumFacturation,
					 const char FARPTR cTypeDonnee);

typedef unsigned short (API_ENTRY TFCTSSVCHARGERFACPDT)
						(const char FARPTR pcNomRessourceLecteur,
						const char FARPTR pcNumFacturation, 
						short sNbZDataIn,
						void FARPTR FARPTR pvTZDataIn, 
						size_t FARPTR psTTailleDataIn,
						short FARPTR 		pNbZDataOut,
                		void FARPTR FARPTR	TZDataOut,
                		size_t	FARPTR		TTailleZoneOut); 

typedef unsigned short (API_ENTRY TFCTSSVCHARGERAPPLI)
						(char FARPTR pcNomRessourceLecteur, 
						short sNbZDataIn,
						void FARPTR FARPTR pvTZDataIn, 
						size_t FARPTR psTTailleDataIn);

typedef unsigned short (API_ENTRY TFCTSSVCHARGERDONNEESTLA)
						(char FARPTR pcNomRessourceLecteur, 
						short sNbZDataIn,
						void FARPTR FARPTR pvTZDataIn, 
						size_t FARPTR psTTailleDataIn);

typedef unsigned short (API_ENTRY TFCTSSVDECHARGERBENEFICIAIRES)                 
                 (const char FARPTR   NomRessourcePS,
		          const char FARPTR	  NomRessourceLecteur,
		          const char FARPTR	  CodePorteurPS,
		          const char FARPTR	  cNumFacturation,
				  short FARPTR		  sNbZDataOut, 
				  void FARPTR FARPTR  pTZDataOut, 
				  size_t FARPTR		  sTTailleDataOut);

typedef unsigned short (API_ENTRY TFCTSSVTRADUIREFSE)
				 (void FARPTR pZDataIn,
				  size_t TailleDataIn,
				  void FARPTR FARPTR pZDataOut,
				  size_t FARPTR pTailleZone);

typedef unsigned short (API_ENTRY TFCTSSVSECURFACTURE)
                 (const char FARPTR pcNomRessourcePS,
				  const char FARPTR pcNomRessourceLecteur,
				  const char FARPTR pcCodePorteurPS, 
				  char cNologSituation,
				  const char FARPTR  pcNumFact,
				  void FARPTR pvDataIn,
				  size_t       szTailleDataIn ,
				  void FARPTR FARPTR pvDataOut, 
				  size_t FARPTR pszTailleDataOut);

typedef unsigned short (API_ENTRY TFCTSSVINITLIB) ();

typedef unsigned short (API_ENTRY TFCTSSVTERMLIB) ();


/* STANDARDS */
extern TFCTSSVALLOUER					SSV_AllouerZoneMem;
extern TFCTSSVLIBERER					SSV_LibererZoneMem;
extern TFCTSSVTRADUIREARL				SSV_TraduireARL; 					  
extern TFCTSSVLIRECARTEPS				SSV_LireCartePS;
extern TFCTSSVLIREDROITS				SSV_LireDroitsVitale;
extern TFCTSSVLIRECONFIG				SSV_LireConfig;
extern TFCTSSVFORMATERFIC				SSV_FormaterFichier;
extern TFCTSSVFORMATERFAC				SSV_FormaterFactures;                   
extern TFCTSSVFORMATERLOT				SSV_FormaterLot;
extern TFCTSSVINITTRACE					SSV_InitTrace;
extern TFCTSSVMAJDATE 					SSV_MajDateLecteur;
extern TFCTSSVLIREDATE					SSV_LireDateLecteur;
extern TFCTSSVCHARGERFACPDT				SSV_ChargerFacturesPdT;   
extern TFCTSSVDECHGFACTPDT				SSV_DechargerFacturesPdT;
extern TFCTSSVDECHGFSETLA				SSV_DechargerFSETLA;
extern TFCTSSVTRADUIREFSE				SSV_TraduireFSE;
extern TFCTSSVDECHARGERBENEFICIAIRES	SSV_DechargerBeneficiaires;
extern TFCTSSVIDENTIFIERTLA				SSV_IdentifierTLA;
extern TFCTSSVEFFACERTLA				SSV_EffacerTLA;
extern TFCTSSVCHARGERAPPLI				SSV_ChargerAppli;
extern TFCTSSVCHARGERDONNEESTLA         SSV_ChargerDonneesTLA;
extern TFCTSSVSECURFACTURE			    SSV_SecuriserFacture;

extern TFCTSSVINITLIB					SSV_InitLIB;
extern TFCTSSVTERMLIB					SSV_TermLIB;


#ifdef SGD
/****** FONCTIONS SIMPLIFIEES *********/

typedef unsigned short (API_ENTRY TFCTSISTRADUIREARL)
						 (unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISLIRECARTEPS)
                (const char FARPTR NomRessourcePS, 
                 const char FARPTR NomRessourceLecteur, 
                 const char FARPTR CodePorteurPS,
                 unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISLIRECONFIG)
				(unsigned short numeroSession);


typedef unsigned short (API_ENTRY TFCTSISFORMATERFAC)
						(const char FARPTR NomRessourcePS,
						const char FARPTR NomRessourceLecteur,
						const char FARPTR CodePorteurPS,
						char cFacturesACreer,
						char ModeSecur,
                        unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISFORMATERLOT)
						( const char FARPTR	NomRessourcePS,
              			 const char FARPTR	NomRessourceLecteur,
               			 const char FARPTR	CodePorteurPS,
				         unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISFORMATERFIC)
						 (unsigned short numeroSession);


typedef unsigned short  (API_ENTRY TFCTSISINITTRACE)
						(char FARPTR NomFichier, char FARPTR ModeOuverture,
						 unsigned short Module, unsigned char Niveau);
						  

typedef unsigned short  (API_ENTRY TFCTSISMAJDATE)
						(const char FARPTR pcNomRessourceLecteur,
						 const char FARPTR pcDateHeure);

typedef unsigned short  (API_ENTRY TFCTSISLIREDATE)
						(const char FARPTR pcNomRessourceLecteur,
						 char FARPTR pcDateHeure);


typedef unsigned short (API_ENTRY TFCTSISLIREDROITS)
                (const char FARPTR NomRessourcePS, 
                 const char FARPTR NomRessourceLecteur, 
                 const char FARPTR CodePorteurPS, 
                 const char FARPTR DateConsultation,
                 unsigned short numeroSession); 

typedef unsigned short  (API_ENTRY TFCTSISIDENTIFIERTLA)
						(const char FARPTR pcNomRessourceLecteur, 
						 const char FARPTR NumVersionCDC,
                         unsigned short numeroSession );

typedef unsigned short (API_ENTRY TFCTSISDECHGFACTPDT)
						( const char FARPTR	NomRessourcePS,
              			 	const char FARPTR	NomRessourceLecteur,
               				const char FARPTR	CodePorteurPS,
               				const char FARPTR	pcNumFact,
                                         unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISDECHGFSETLA)
						(const char FARPTR	NomRessourcePS,
              			 	const char FARPTR	NomRessourceLecteur,
               				const char FARPTR	CodePorteurPS,
               				const char FARPTR	pcNumFact,
                                        unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISEFFACERTLA)
					(const char FARPTR NomRessourcePS,
					 const char FARPTR NomRessourceLecteur,
					 const char FARPTR CodePorteurPS, 	
					 const char FARPTR cNumFacturation,
					 const char FARPTR cTypeDonnee);

typedef unsigned short (API_ENTRY TFCTSISCHARGERFACPDT)
						(char FARPTR pcNomRessourceLecteur,
						 char FARPTR pcNumFacturation,
                                                 unsigned short numeroSession); 

typedef unsigned short (API_ENTRY TFCTSISCHARGERAPPLI)
						( char FARPTR pcNomRessourceLecteur,
                                                  unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISCHARGERDONNEESTLA)
						(char FARPTR pcNomRessourceLecteur,
                                                unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISDECHARGERBENEFICIAIRES)                 
                 (	  const char FARPTR   NomRessourcePS,
		          const char FARPTR	  NomRessourceLecteur,
		          const char FARPTR	  CodePorteurPS,
		          const char FARPTR	  cNumFacturation,
                          unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISTRADUIREFSE)
				 (unsigned short numeroSession);

typedef unsigned short (API_ENTRY TFCTSISSECURFACTURE)
                 ( const char FARPTR pcNomRessourcePS,
				  const char FARPTR pcNomRessourceLecteur,
				  const char FARPTR pcCodePorteurPS, 
				  char cNologSituation,
				  const char FARPTR  pcNumFact,
                                  unsigned short numeroSession);
						  

/* SIMPLIFIEES */

extern TFCTSISTRADUIREARL				SIS_TraduireARL; 					  
extern TFCTSISLIRECARTEPS				SIS_LireCartePS;
extern TFCTSISLIREDROITS				SIS_LireDroitsVitale;
extern TFCTSISLIRECONFIG				SIS_LireConfig;
extern TFCTSISFORMATERFIC				SIS_FormaterFichier;
extern TFCTSISFORMATERFAC				SIS_FormaterFactures;                   
extern TFCTSISFORMATERLOT				SIS_FormaterLot;
extern TFCTSISINITTRACE					SIS_InitTrace;
extern TFCTSISMAJDATE 					SIS_MajDateLecteur;
extern TFCTSISLIREDATE					SIS_LireDateLecteur;
extern TFCTSISCHARGERFACPDT				SIS_ChargerFacturesPdT;   
extern TFCTSISDECHGFACTPDT				SIS_DechargerFacturesPdT;
extern TFCTSISDECHGFSETLA				SIS_DechargerFSETLA;
extern TFCTSISTRADUIREFSE				SIS_TraduireFSE;
extern TFCTSISDECHARGERBENEFICIAIRES	SIS_DechargerBeneficiaires;
extern TFCTSISIDENTIFIERTLA				SIS_IdentifierTLA;
extern TFCTSISEFFACERTLA				SIS_EffacerTLA;
extern TFCTSISCHARGERAPPLI				SIS_ChargerAppli;
extern TFCTSISSECURFACTURE			    SIS_SecuriserFacture;
extern TFCTSISCHARGERDONNEESTLA			SIS_ChargerDonneesTLA;

#endif

#ifdef __cplusplus
}
#endif

#endif
