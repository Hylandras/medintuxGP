/*
 * -------------------------------------------------------------------
 * (c) 2001-2003 GIE SESAM-VITALE
 *
 * PROJET   : Services Reglementation et Tarification
 *
 * FICHIER  : srt.h (v5)
 *
 * Declaration des prototypes des fonctions SRT
 * -------------------------------------------------------------------
 */

#ifndef __SRT_H__
#define __SRT_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Inclusions
 */
#include <stddef.h>
/* Inclure le fichier de déclarations spécifiques système :
   win32def.h, macosdef.h, unixdef.h, etc. */


/*
 * Declarations des fonctions publiques "standards"
 */

/* Recherche de codes CCAM par mot cle. */
typedef unsigned short (API_ENTRY TFCTSRTRECHMOTCLE)
                       (const char*    MotCle,
                        unsigned long  CadreRecherche,
                        void**         ZDonneesSortie,
                        size_t*        TailleDonneesSortie);
extern TFCTSRTRECHMOTCLE SRT_RechercherParMotCle;

/* Recherche de codes CCAM par filtre. */
typedef unsigned short (API_ENTRY TFCTSRTRECHFILTRE)
                       (const char*    Filtre,
                        void**         ZDonneesSortie,
                        size_t*        TailleDonneesSortie);
extern TFCTSRTRECHFILTRE SRT_RechercherParFiltre;

/* Consultation de donnees CCAM. */
typedef unsigned short (API_ENTRY TFCTSRTCONSULTERDONNEE)
                       (const char*    Identifiant,
                        const char*    DateReference,
                        void*          ZDonneesEntree,
                        size_t         TailleDonneesEntree,
                        void**         ZDonneesSortie,
                        size_t*        TailleDonneesSortie);
extern TFCTSRTCONSULTERDONNEE SRT_ConsulterDonnee;

/* Modification d'un champ mono-occurrent. */
typedef unsigned short (API_ENTRY TFCTSRTMODIFIERDONNEE)
                       (const char*    Identifiant,
                        const char*    DateEffet,
                        void*          ZDonneesEntree,
                        size_t         TailleDonneesEntree);
extern TFCTSRTMODIFIERDONNEE SRT_ModifierDonnee;

/* Annulation d'une modification */
typedef unsigned short (API_ENTRY TFCTSRTANNULERMODIF)
                       (const char*    Identifiant,
                        void*          ZDonneesEntree,
                        size_t         TailleDonneesEntree);
extern TFCTSRTANNULERMODIF SRT_AnnulerModification;

/* Controle de la validite d'une ou plusieurs donnees. */
typedef unsigned short (API_ENTRY TFCTSRTCONTROLERDONNEES)
                       (const char*    Identifiant,
                        const char*    DateReference,
                        void*          ZDonneesEntree,
                        size_t         TailleDonneesEntree,
                        unsigned char* Resultat);
extern TFCTSRTCONTROLERDONNEES SRT_ControlerDonnees;

/* Regle de gestion ou de tarification. */
typedef unsigned short (API_ENTRY TFCTSRTAPPLIQUERREGLE)
                       (const char*    Identifiant,
                        void*          ZDonneesEntree,
                        size_t         TailleDonneesEntree,
                        void**         ZDonneesSortie,
                        size_t*        TailleDonneesSortie);
extern TFCTSRTAPPLIQUERREGLE SRT_AppliquerRegle;

/* Controle complet. */
typedef unsigned short (API_ENTRY TFCTSRTCONTROLECOMPLET)
                       (void*          ZDonneesEntree,
                        size_t         TailleDonneesEntree,
                        void**         ZDonneesSortie,
                        size_t*        TailleDonneesSortie);
extern TFCTSRTCONTROLECOMPLET SRT_ControleComplet;

/* Chargement du referentiel. */
typedef unsigned short (API_ENTRY TFCTSRTINITLIB)();
extern TFCTSRTINITLIB SRT_InitLIB;

/* Dechargement du referentiel. */
typedef unsigned short (API_ENTRY TFCTSRTTERMLIB)();
extern TFCTSRTTERMLIB SRT_TermLIB;

/* Sauvegarde des modifications. */
typedef unsigned short (API_ENTRY TFCTSRTSAUVERREF)(const char* Commentaire);
extern TFCTSRTSAUVERREF SRT_SauvegarderReferentiel;

/* Version du referentiel. */
typedef unsigned short (API_ENTRY TFCTSRTLIREVERSION)
                       (void**         ZDonneesSortie,
                        size_t*        TailleDonneesSortie);
extern TFCTSRTLIREVERSION SRT_LireVersion;

/* Activation de la trace. */
typedef unsigned short (API_ENTRY TFCTSRTINITTRACE)
                       (const char*    NomFichier,
                        const char*    ModeOuverture,
                        unsigned short Module,
                        unsigned char  Niveau);
extern TFCTSRTINITTRACE SRT_InitTrace;

/* Allocation d'une zone memoire. */
typedef unsigned short (API_ENTRY TFCTSRTALLOUERZMEM)
                       (void**         Zone,
                        size_t         Taille);
extern TFCTSRTALLOUERZMEM SRT_AllouerZoneMem;

/* Allocation d'une zone memoire. */
typedef void (API_ENTRY TFCTSRTLIBERERZMEM) (void*  Zone);
extern TFCTSRTLIBERERZMEM SRT_LibererZoneMem;



#ifdef SGD

/*
 * Declarations des fonctions publiques "simplifiees"
 */

/* Recherche de codes CCAM par mot cle. */
typedef unsigned short (API_ENTRY TFCTSRTRECHMOTCLE_S)
                       (const char*    MotCle,
                        unsigned long  CadreRecherche,
                        unsigned short NumeroSession);
extern TFCTSRTRECHMOTCLE_S SRT_RechercherParMotCle_S;

/* Recherche de codes CCAM par filtre. */
typedef unsigned short (API_ENTRY TFCTSRTRECHFILTRE_S)
                       (const char*    Filtre,
                        unsigned short NumeroSession);
extern TFCTSRTRECHFILTRE_S SRT_RechercherParFiltre_S;

/* Consultation de donnees CCAM. */
typedef unsigned short (API_ENTRY TFCTSRTCONSULTERDONNEE_S)
                       (const char*    Identifiant,
                        const char*    DateReference,
                        unsigned short NumeroSession);
extern TFCTSRTCONSULTERDONNEE_S SRT_ConsulterDonnee_S;

/* Modification d'un champ mono-occurrent. */
typedef unsigned short (API_ENTRY TFCTSRTMODIFIERDONNEE_S)
                       (const char*    Identifiant,
                        const char*    DateEffet,
                        unsigned short NumeroSession);
extern TFCTSRTMODIFIERDONNEE_S SRT_ModifierDonnee_S;

/* Annulation des modifications d'une donnee. */
typedef unsigned short (API_ENTRY TFCTSRTANNULERMODIF_S)
                       (const char*    Identifiant,
                        unsigned short NumeroSession);
extern TFCTSRTANNULERMODIF_S SRT_AnnulerModification_S;

/* Controle de la validite d'une ou plusieurs donnees. */
typedef unsigned short (API_ENTRY TFCTSRTCONTROLERDONNEES_S)
                       (const char*    Identifiant,
                        const char*    DateReference,
                        unsigned short NumeroSession,
                        unsigned char* Resultat);
extern TFCTSRTCONTROLERDONNEES_S SRT_ControlerDonnees_S;

/* Regle de gestion ou de tarification. */
typedef unsigned short (API_ENTRY TFCTSRTAPPLIQUERREGLE_S)
                       (const char*    Identifiant,
                        unsigned short NumeroSession);
extern TFCTSRTAPPLIQUERREGLE_S SRT_AppliquerRegle_S;

/* Controle complet. */
typedef unsigned short (API_ENTRY TFCTSRTCONTROLECOMPLET_S)
                       (unsigned short NumeroSession);
extern TFCTSRTCONTROLECOMPLET_S SRT_ControleComplet_S;

/* Version du referentiel. */
typedef unsigned short (API_ENTRY TFCTSRTLIREVERSION_S)
                       (unsigned short NumeroSession);
extern TFCTSRTLIREVERSION_S SRT_LireVersion_S;

#endif


#ifdef __cplusplus
}
#endif

#endif
