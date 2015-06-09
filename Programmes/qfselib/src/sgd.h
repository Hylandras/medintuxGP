/*
 * -------------------------------------------------------------------
 * (c) 2001 GIE SESAM-VITALE
 *
 * PROJET   : Services de Gestion de Donnees
 *
 * FICHIER  : sgd.h (v5)
 *
 * Declaration des prototypes des fonctions SGD pour les progiciels.
 * -------------------------------------------------------------------
 */

#ifndef __SGD_H__
#define __SGD_H__

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Inclusions
 */
#include <stddef.h>
/* Inclure le fichier de déclarations spécifiques système :
   win32def.h, macosdef.h, unixdef.h, etc. */
//......... gérer les spécificités selon OS (## add by and for MedinTux) ...............
 #if   defined(Q_WS_WIN)
   #include "win32def.h"
#elif defined(Q_WS_X11)
   #include "unixdef.h"
#elif defined(Q_WS_MAC)
   #include "macosdef.h"
#endif



/* Reservation d'un session d'echange. */
typedef unsigned short (API_ENTRY TFCTSGDRESERVER)
                       (size_t          Taille,
                        unsigned short* NumeroSession);
extern TFCTSGDRESERVER SGD_ReserverSession;

/* Redimensionnement d'une zone d'entree interne. */
typedef unsigned short (API_ENTRY TFCTSGDREDIMENSIONNER)
                       (unsigned short NumeroSession,
                        size_t         NouvelleTaille);
extern TFCTSGDREDIMENSIONNER SGD_RedimensionnerBuffer;

/* Liberation d'une session d'echange. */
typedef unsigned short (API_ENTRY TFCTSGDLIBERER)
                       (unsigned short NumeroSession);
extern TFCTSGDLIBERER SGD_LibererSession;

/* Ajout d'un groupe. */
typedef unsigned short (API_ENTRY TFCTSGDAJOUTERGROUPE)
                       (unsigned short NumeroSession,
                        unsigned short IdGroupe);
extern TFCTSGDAJOUTERGROUPE SGD_AjouterGroupe;

/* Ajout d'un champ. */
typedef unsigned short (API_ENTRY TFCTSGDAJOUTERCHAMP)
                       (unsigned short NumeroSession,
                        const char*    ValeurChamp);
extern TFCTSGDAJOUTERCHAMP SGD_AjouterChamp;

/* Ajout d'un champ d'octets. */
typedef unsigned short (API_ENTRY TFCTSGDAJOUTERBIN)
                       (unsigned short NumeroSession,
                        const char*    ValeurChamp,
                        size_t         TailleChamp);
extern TFCTSGDAJOUTERBIN SGD_AjouterChampBin;

/* Ajout d'un champ d'octets d'un Fichier. */
typedef unsigned short (API_ENTRY TFCTSGDAJOUTERFICHIER)
                       (unsigned short NumeroSession,
                        const char*    NomFichier);
extern TFCTSGDAJOUTERFICHIER SGD_AjouterChampFichier;

/* Lecture du numero du groupe suivant. */
typedef unsigned short (API_ENTRY TFCTSGDLIREGROUPE)
                       (unsigned short  NumeroSession,
                        unsigned short  IdGroupeCourant,
                        unsigned short  OccurrenceGroupeCourant,
                        unsigned short* IdGroupeSuivant,
                        unsigned short* OccurrenceGroupeSuivant);
extern TFCTSGDLIREGROUPE SGD_LireGroupeSuivant;

/* Lecture d'un champ. */
typedef unsigned short (API_ENTRY TFCTSGDLIRECHAMP)
                       (unsigned short NumeroSession,
                        unsigned short IdGroupe,
                        unsigned short OccurrenceGroupe,
                        unsigned short NumeroChamp,
                        char*          ValeurChamp,
                        size_t*        TailleMax);
extern TFCTSGDLIRECHAMP SGD_LireChamp;

/* Lecture d'un champ d'octets. */
typedef unsigned short (API_ENTRY TFCTSGDLIREBIN)
                       (unsigned short NumeroSession,
                        unsigned short IdGroupe,
                        unsigned short OccurrenceGroupe,
                        unsigned short NumeroChamp,
                        char*          ValeurChamp,
                        size_t*        TailleMax);
extern TFCTSGDLIREBIN SGD_LireChampBin;

/* Lecture d'un champ vers un fichier. */
typedef unsigned short (API_ENTRY TFCTSGDLIREFICHIER)
                       (unsigned short NumeroSession,
                        unsigned short IdGroupe,
                        unsigned short OccurrenceGroupe,
                        unsigned short NumeroChamp,
                        const char*    NomFichier);
extern TFCTSGDLIREFICHIER SGD_LireChampFichier;

/* Activation de la trace. */
typedef unsigned short (API_ENTRY TFCTSGDINITTRACE)
                       (const char*    NomFichier,
                        const char*    ModeOuverture,
                        unsigned short Module,
                        unsigned char  Niveau);
extern TFCTSGDINITTRACE SGD_InitTrace;

#ifdef __cplusplus
}
#endif

#endif
