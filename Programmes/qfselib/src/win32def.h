/*
 * -------------------------------------------------------------------
 * (c) 2002 GIE SESAM-VITALE
 *
 * FICHIER : win32def.h (v1)
 *
 * PLATE-FORME : Windows 32 bits
 *
 * Définitions dépendantes du système.
 * -------------------------------------------------------------------
 */

#ifndef __WIN32DEF_H__
#define __WIN32DEF_H__

#ifndef API_ENTRY
#define API_ENTRY __stdcall
#endif

#ifndef FARPTR
#define FARPTR *
#endif

/* Activation des fonctions simplifiées. */
#ifndef SGD
#define SGD
#endif

#endif
