/*
%-----------------------------------------------------------------
% PROJET: SVADMLEC
%
% MODULE: Module principal qui contient les fonctions d'administration
% de l'API.
%
% VERSION: 1.00
%
% FICHIER: svadmlec.h
%
% Définit les pointeurs sur les fonctions d'administration et auxiliaires
% de l'API pour le chargement de la DLL en cours d'exécution de l'application
% (Pas d'utilisation du .lib)
%-----------------------------------------------------------------
% Version 1.00
% Jean-Marc Dagorne (JDA) - 14/10/2001 - Création
% 
%-----------------------------------------------------------------
*/
#ifndef __SVADMLEC_H_VU__
#define __SVADMLEC_H_VU__

/********************************************************/
/* Pointeurs sur les fonctions principales d'administration */
/********************************************************/


/* Pointeur sur la fonction de mise à jour de la date et de l'heure du lecteur:
unsigned short usMettreAJourDateHeure( IN char * pcIdRessourceLecteur,
                                       IN char * pcIdAppUtilisatrice,
                                       IN char * pcDateHeure,
                                       OUT void * pvRefus);
*/
typedef unsigned short (* LPFNDLL_MAJ_DATE_HEURE) (char *, char *, char *, void *);


/* Pointeur sur la fonction de consultation d'informations concernant le lecteur:
unsigned short usConsulterEtatLecteur( IN char* pcIdRessourceLecteur, 
                                       IN char* pcIdAppUtilisatrice,
                                       OUT void* pvEtatLecteur);
*/
typedef unsigned short (* LPFNDLL_CONSULTER_ETAT_LECTEUR) (char *, char *, void *);


/* Pointeur sur la fonction de mise à jour du logiciel noyau du lecteur:
unsigned short  usMettreAJourNoyau( IN char* pcIdRessourceLecteur,
                                    IN char* pcIdAppUtilisatrice, 
                                    IN char* pcNomFichierNoyau,
                                    IN char* pcNumeroVersion,
                                    OUT void* pvRefus);
*/
typedef unsigned short (* LPFNDLL_MAJ_NOYAU) (char *, char *, char *, char *, void *);


/* Pointeur sur la fonction de mise à jour du logiciel superviseur du lecteur:
unsigned short usMettreAJourSuperviseurCarte(IN char* pcIdRessourceLecteur,
                                             IN char* pcIdAppUtilisatrice,
                                             IN char* pcNomFichierSuperviseur,
                                             IN char* pcNumeroVersion,
                                             OUT void* pvRefus);
*/
typedef unsigned short (* LPFNDLL_MAJ_SUPERVISEUR) (char *, char *, char *, char *, void *);


/* Pointeur sur la fonction de mise à jour/installation d'une application sur le lecteur:
unsigned short usMettreAJourApplication (IN char* pcIdRessourceLecteur,
                                         IN char* pcIdAppUtilisatrice,
                                         IN char* pcIdApplication,
                                         IN char* pcNomFichierApplication,
                                         IN char* pcNumeroVersion,
                                         OUT void* pvRefus);
*/
typedef unsigned short (* LPFNDLL_MAJ_APPLICATION) (char *, char *, char *, char *, char *, void *);


/* Pointeur sur la fonction de suppression d'une application sur le lecteur:
unsigned short usSupprimerApplication( IN char* pcIdRessourceLecteur,
                                       IN char* pcIdAppUtilisatrice,
                                       IN char* pcIdApplication,
                                       OUT void* pvRefus);
*/
typedef unsigned short (* LPFNDLL_SUP_APPLICATION) (char *, char *, char *, void *);


/* Pointeur sur la fonction de lecture de la version logicielle de l'API:
_DLL unsigned short usLireVersionAPI( OUT char* pcVersionAPI);
*/

typedef unsigned short  (*LPFNDLL_LIRE_VERSION_API) (char * );


/***************************************/
/* Pointeurs des fonctions auxiliaires */
/***************************************/

/***** Paramètres de sortie Etat Lecteur *****/

/* Pointeur sur la fonction d'allocation de structure:
void* pvCreerEtatLecteur(void);
*/
typedef void* (* LPFNDLL_CREER_STRUCT_ETAT_LECTEUR)();


/* Pointeur sur la fonction de libération de structure:
unsigned short usLibererEtatLecteur(IN void* pvEtatLecteur);
*/
typedef unsigned short (* LPFNDLL_LIBERER_STRUCT_ETAT_LECTEUR)(void*);


/** Pointeur sur les fonctions de lecture des paramètres **/

/* Pointeur sur fonction:
unsigned short usLecteurGetConstructeur( IN void* pvEtatLecteur,
                                         OUT char* pcConstructeur);
*/
typedef unsigned short (* LPFNDLL_GET_CONSTRUCTEUR)(void*, char*);


/* Pointeur sur fonction:
unsigned short usLecteurGetDesignationCommerciale( IN void* pvEtatLecteur,
                                                   OUT char* pcDesignComm);*/
typedef unsigned short (* LPFNDLL_GET_DESIGN_COMM)(void*, char*);


/* Pointeur sur fonction:
unsigned short usLecteurGetType( IN void* pvEtatLecteur,
                                 OUT char* pcTypeLecteur);
*/
typedef unsigned short (* LPFNDLL_GET_TYPE)(void*, char*);


/* Pointeur sur fonction:
unsigned short usLecteurGetNumeroSerie(  IN void* pvEtatLecteur,
                                         OUT char* pcNumeroSerie);
*/
typedef unsigned short (* LPFNDLL_GET_NUMERO_DE_SERIE)(void*, char*);


/* Pointeur sur fonction:
unsigned short usLecteurGetTailleMemoire(  IN void* pvEtatLecteur,
                                           OUT char* pcTailleMemoire);
*/
typedef unsigned short (* LPFNDLL_GET_TAILLE_MEMOIRE)(void*, char*);


/* Pointeur sur fonction:
unsigned short usLecteurGetDate( IN void* pvEtatLecteur,
                                 OUT char* pcDateLecteur);
*/
typedef unsigned short (* LPFNDLL_GET_DATE)(void*, char*);


/* Pointeur sur fonction:
unsigned short usLecteurGetHeure(  IN void* pvEtatLecteur,
                                   OUT char* pcHeureLecteur);
*/
typedef unsigned short (* LPFNDLL_GET_HEURE)(void*, char*);


/* Pointeur sur fonction:
unsigned short usLecteurGetVersionNoyau( IN void* pvEtatLecteur,
                                         OUT char* pcVersionNoyau);
*/
typedef unsigned short (* LPFNDLL_GET_VERSION_NOYAU)(void*, char*);


/* Pointeur sur fonction:
unsigned short usLecteurGetVersionSuperviseur( IN void* pvEtatLecteur,
                                               OUT char* pcVersionSuperviseur);
*/
typedef unsigned short (* LPFNDLL_GET_VERSION_SUPERVISEUR)(void*, char*);



/**** Paramètres de sortie Refus ****/

/* Pointeur sur la fonction d'allocation de structure:
void* pvCreerRefus(void);
*/
typedef void* (* LPFNDLL_CREER_STRUCT_REFUS)();


/* Pointeur sur la fonction de libération de structure:
unsigned short usLibererRefus(IN void* pvEtatRefus);
*/
typedef unsigned short (* LPFNDLL_LIBERER_STRUCT_REFUS)(void*);


/** Pointeur sur les fonctions de lecture des paramètres **/

/* Pointeur sur fonction:
unsigned short usRefusGetIdApplication(IN void* pvRefus,OUT char* pcIdApplication);
*/
typedef unsigned short (* LPFNDLL_GET_ID_APPLICATION)(void*, char*);


/* Pointeur sur fonction:
unsigned short usRefusGetCommentaire(IN void* pvRefus,OUT char* pcCommentaire);
*/
typedef unsigned short (* LPFNDLL_GET_COMMENTAIRE)(void*, char*);




#endif
/*
% -------------------------------------------------------------------
% Fin du fichier svadmlec.h
% -------------------------------------------------------------------
*//*
%-----------------------------------------------------------------
% PROJET: SVADMLEC
%
% MODULE: Module principal qui contient les fonctions d'administration
% de l'API.
%
% VERSION: 1.00
%
% FICHIER: svadmlec.h
%
% Définit les pointeurs sur les fonctions d'administration et auxiliaires
% de l'API pour le chargement de la DLL en cours d'exécution de l'application
% (Pas d'utilisation du .lib)
%-----------------------------------------------------------------
% Version 1.00
% Jean-Marc Dagorne (JDA) - 14/10/2001 - Création
% 
%-----------------------------------------------------------------
*/
#ifndef __SVADMLEC_H_VU__
#define __SVADMLEC_H_VU__

/********************************************************/
/* Pointeurs sur les fonctions principales d'administration */
/********************************************************/

#define IN
#define OUT

#ifdef __CHGT_STATIC__

/*  Pointeur sur la fonction de mise à jour de la date et de l'heure du lecteur:*/
extern "C" unsigned short usMettreAJourDateHeure( IN char * pcIdRessourceLecteur,
                                       IN char * pcIdAppUtilisatrice,
                                       IN char * pcDateHeure,
                                       OUT void * pvRefus);
/* */
//typedef unsigned short (* LPFNDLL_MAJ_DATE_HEURE) (char *, char *, char *, void *);


/*  Pointeur sur la fonction de consultation d'informations concernant le lecteur:*/
extern "C" unsigned short usConsulterEtatLecteur( IN char* pcIdRessourceLecteur, 
                                       IN char* pcIdAppUtilisatrice,
                                       OUT void* pvEtatLecteur);
/* */
//typedef unsigned short (* LPFNDLL_CONSULTER_ETAT_LECTEUR) (char *, char *, void *);


/*  Pointeur sur la fonction de mise à jour du logiciel noyau du lecteur:*/
extern "C" unsigned short  usMettreAJourNoyau(IN char* pcIdRessourceLecteur,
                                    IN char* pcIdAppUtilisatrice, 
                                    IN char* pcNomFichierNoyau,
                                    IN char* pcNumeroVersion,
                                    OUT void* pvRefus);
/* */
//typedef unsigned short (* LPFNDLL_MAJ_NOYAU) (char *, char *, char *, char *, void *);


/*  Pointeur sur la fonction de mise à jour du logiciel superviseur du lecteur:*/
extern "C" unsigned short usMettreAJourSuperviseurCarte(IN char* pcIdRessourceLecteur,
                                             IN char* pcIdAppUtilisatrice,
                                             IN char* pcNomFichierSuperviseur,
                                             IN char* pcNumeroVersion,
                                             OUT void* pvRefus);
/* */
//typedef unsigned short (* LPFNDLL_MAJ_SUPERVISEUR) (char *, char *, char *, char *, void *);


/*  Pointeur sur la fonction de mise à jour/installation d'une application sur le lecteur:*/
extern "C" unsigned short usMettreAJourApplication (IN char* pcIdRessourceLecteur,
                                         IN char* pcIdAppUtilisatrice,
                                         IN char* pcIdApplication,
                                         IN char* pcNomFichierApplication,
                                         IN char* pcNumeroVersion,
                                         OUT void* pvRefus);
/* */
 //typedef unsigned short (* LPFNDLL_MAJ_APPLICATION) (char *, char *, char *, char *, char *, void *);


/* Pointeur sur la fonction de suppression d'une application sur le lecteur:*/
extern "C" unsigned short usSupprimerApplication( IN char* pcIdRessourceLecteur,
                                       IN char* pcIdAppUtilisatrice,
                                       IN char* pcIdApplication,
                                       OUT void* pvRefus);
/* */
//typedef unsigned short (* LPFNDLL_SUP_APPLICATION) (char *, char *, char *, void *);


/* Pointeur sur la fonction de lecture de la version logicielle de l'API:*/
/*_DLL*/ extern "C" unsigned short usLireVersionAPI( OUT char* pcVersionAPI);
/**/

//typedef unsigned short  (*LPFNDLL_LIRE_VERSION_API) (char * );


/***************************************/
/* Pointeurs des fonctions auxiliaires */
/***************************************/

/***** Paramètres de sortie Etat Lecteur *****/

/*  Pointeur sur la fonction d'allocation de structure:*/
extern "C" void* pvCreerEtatLecteur(void);
/* */
//typedef void* (* LPFNDLL_CREER_STRUCT_ETAT_LECTEUR)();


/*   Pointeur sur la fonction de libération de structure:*/
extern "C" unsigned short usLibererEtatLecteur(IN void* pvEtatLecteur);
/* */
//typedef unsigned short (* LPFNDLL_LIBERER_STRUCT_ETAT_LECTEUR)(void*);


/** Pointeur sur les fonctions de lecture des paramètres **/

/*  Pointeur sur fonction:*/
extern "C" unsigned short usLecteurGetConstructeur( IN void* pvEtatLecteur,
                                         OUT char* pcConstructeur);
/* */
//typedef unsigned short (* LPFNDLL_GET_CONSTRUCTEUR)(void*, char*);


/* Pointeur sur fonction:*/
extern "C" unsigned short usLecteurGetDesignationCommerciale( IN void* pvEtatLecteur,
                                                   OUT char* pcDesignComm);
//typedef unsigned short (* LPFNDLL_GET_DESIGN_COMM)(void*, char*);


/* Pointeur sur fonction:*/
extern "C" unsigned short usLecteurGetType( IN void* pvEtatLecteur,
                                 OUT char* pcTypeLecteur);
/**/
//typedef unsigned short (* LPFNDLL_GET_TYPE)(void*, char*);


/* Pointeur sur fonction:*/
extern "C" unsigned short usLecteurGetNumeroSerie(  IN void* pvEtatLecteur,
                                         OUT char* pcNumeroSerie);
/**/
//typedef unsigned short (* LPFNDLL_GET_NUMERO_DE_SERIE)(void*, char*);


/* Pointeur sur fonction:*/
extern "C" unsigned short usLecteurGetTailleMemoire(  IN void* pvEtatLecteur,
                                           OUT char* pcTailleMemoire);
/**/
//typedef unsigned short (* LPFNDLL_GET_TAILLE_MEMOIRE)(void*, char*);


/* Pointeur sur fonction:*/
extern "C" unsigned short usLecteurGetDate( IN void* pvEtatLecteur,
                                 OUT char* pcDateLecteur);
/**/
//typedef unsigned short (* LPFNDLL_GET_DATE)(void*, char*);


/* Pointeur sur fonction:*/
extern "C" unsigned short usLecteurGetHeure(  IN void* pvEtatLecteur,
                                   OUT char* pcHeureLecteur);
/**/
//typedef unsigned short (* LPFNDLL_GET_HEURE)(void*, char*);


/* Pointeur sur fonction:*/
extern "C" unsigned short usLecteurGetVersionNoyau( IN void* pvEtatLecteur,
                                         OUT char* pcVersionNoyau);
/**/
//typedef unsigned short (* LPFNDLL_GET_VERSION_NOYAU)(void*, char*);


/* Pointeur sur fonction:*/
extern "C" unsigned short usLecteurGetVersionSuperviseur( IN void* pvEtatLecteur,
                                               OUT char* pcVersionSuperviseur);
/**/
//typedef unsigned short (* LPFNDLL_GET_VERSION_SUPERVISEUR)(void*, char*);



/**** Paramètres de sortie Refus ****/

/* Pointeur sur la fonction d'allocation de structure:*/
extern "C" void* pvCreerRefus(void);
/**/
//typedef void* (* LPFNDLL_CREER_STRUCT_REFUS)();


/* Pointeur sur la fonction de libération de structure:*/
extern "C" unsigned short usLibererRefus(IN void* pvEtatRefus);
/**/
//typedef unsigned short (* LPFNDLL_LIBERER_STRUCT_REFUS)(void*);


/** Pointeur sur les fonctions de lecture des paramètres **/

/* Pointeur sur fonction:*/
extern "C" unsigned short usRefusGetIdApplication(IN void* pvRefus,OUT char* pcIdApplication);
/**/
//typedef unsigned short (* LPFNDLL_GET_ID_APPLICATION)(void*, char*);


/* Pointeur sur fonction:*/
extern "C" unsigned short usRefusGetCommentaire(IN void* pvRefus,OUT char* pcCommentaire);
/* */
//typedef unsigned short (* LPFNDLL_GET_COMMENTAIRE)(void*, char*);


#else
//#endif
//#ifndef __CHGT_STATIC__
/*  Pointeur sur la fonction de consultation d'informations concernant le lecteur:*/
typedef unsigned short (* LPFNDLL_CONSULTER_ETAT_LECTEUR) (char *, char *, void *);


/*  Pointeur sur la fonction de mise à jour du logiciel noyau du lecteur:*/
typedef unsigned short (* LPFNDLL_MAJ_NOYAU) (char *, char *, char *, char *, void *);


/*  Pointeur sur la fonction de mise à jour du logiciel superviseur du lecteur:*/
typedef unsigned short (* LPFNDLL_MAJ_SUPERVISEUR) (char *, char *, char *, char *, void *);


/*  Pointeur sur la fonction de mise à jour/installation d'une application sur le lecteur:*/
typedef unsigned short (* LPFNDLL_MAJ_APPLICATION) (char *, char *, char *, char *, char *, void *);


/* Pointeur sur la fonction de suppression d'une application sur le lecteur:*/
typedef unsigned short (* LPFNDLL_SUP_APPLICATION) (char *, char *, char *, void *);


/* Pointeur sur la fonction de lecture de la version logicielle de l'API:*/
typedef unsigned short  (*LPFNDLL_LIRE_VERSION_API) (char * );


/***************************************/
/* Pointeurs des fonctions auxiliaires */
/***************************************/

/***** Paramètres de sortie Etat Lecteur *****/

/*  Pointeur sur la fonction d'allocation de structure:*/
typedef void* (* LPFNDLL_CREER_STRUCT_ETAT_LECTEUR)();


/*   Pointeur sur la fonction de libération de structure:*/
typedef unsigned short (* LPFNDLL_LIBERER_STRUCT_ETAT_LECTEUR)(void*);


/** Pointeur sur les fonctions de lecture des paramètres **/

/*  Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_CONSTRUCTEUR)(void*, char*);


/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_DESIGN_COMM)(void*, char*);


/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_TYPE)(void*, char*);


/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_NUMERO_DE_SERIE)(void*, char*);


/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_TAILLE_MEMOIRE)(void*, char*);


/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_DATE)(void*, char*);


/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_HEURE)(void*, char*);


/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_VERSION_NOYAU)(void*, char*);


/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_VERSION_SUPERVISEUR)(void*, char*);

typedef unsigned short (* LPFNDLL_MAJ_DATE_HEURE) (char *, char *, char *, void *);

/**** Paramètres de sortie Refus ****/

/* Pointeur sur la fonction d'allocation de structure:*/
typedef void* (* LPFNDLL_CREER_STRUCT_REFUS)();


/* Pointeur sur la fonction de libération de structure:*/
typedef unsigned short (* LPFNDLL_LIBERER_STRUCT_REFUS)(void*);


/** Pointeur sur les fonctions de lecture des paramètres **/

/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_ID_APPLICATION)(void*, char*);


/* Pointeur sur fonction:*/
typedef unsigned short (* LPFNDLL_GET_COMMENTAIRE)(void*, char*);


//extern HINSTANCE hinstLib=NULL;  /* instance de la librairie */

extern LPFNDLL_MAJ_DATE_HEURE				usMettreAJourDateHeure = NULL;
extern LPFNDLL_CONSULTER_ETAT_LECTEUR		usConsulterEtatLecteur = NULL;
extern LPFNDLL_MAJ_NOYAU				usMettreAJourNoyau = NULL;
extern LPFNDLL_MAJ_SUPERVISEUR			usMettreAJourSuperviseurCarte = NULL;
extern LPFNDLL_MAJ_APPLICATION			usMettreAJourApplication = NULL;
extern LPFNDLL_SUP_APPLICATION			usSupprimerApplication = NULL;
extern LPFNDLL_LIRE_VERSION_API			usLireVersionAPI = NULL;
extern LPFNDLL_CREER_STRUCT_ETAT_LECTEUR		pvCreerEtatLecteur = NULL;
extern LPFNDLL_LIBERER_STRUCT_ETAT_LECTEUR	usLibererEtatLecteur = NULL;
extern LPFNDLL_GET_CONSTRUCTEUR			usLecteurGetConstructeur = NULL;
extern LPFNDLL_GET_DESIGN_COMM			usLecteurGetDesignationCommerciale = NULL;
extern LPFNDLL_GET_TYPE					usLecteurGetType = NULL;
extern LPFNDLL_GET_NUMERO_DE_SERIE			usLecteurGetNumeroSerie = NULL;
extern LPFNDLL_GET_TAILLE_MEMOIRE			usLecteurGetTailleMemoire = NULL;
extern LPFNDLL_GET_DATE					usLecteurGetDate = NULL;
extern LPFNDLL_GET_HEURE				usLecteurGetHeure = NULL;
extern LPFNDLL_GET_VERSION_NOYAU			usLecteurGetVersionNoyau = NULL;
extern LPFNDLL_GET_VERSION_SUPERVISEUR		usLecteurGetVersionSuperviseur = NULL;
extern LPFNDLL_CREER_STRUCT_REFUS			pvCreerRefus = NULL;
extern LPFNDLL_LIBERER_STRUCT_REFUS			usLibererRefus = NULL;
extern LPFNDLL_GET_ID_APPLICATION			usRefusGetIdApplication = NULL;
extern LPFNDLL_GET_COMMENTAIRE			usRefusGetCommentaire = NULL;

#endif

#endif
/*
% -------------------------------------------------------------------
% Fin du fichier svadmlec.h
% -------------------------------------------------------------------
*/
