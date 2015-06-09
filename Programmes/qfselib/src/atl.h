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

#define IN
#define OUT


/*  Pointeur sur la fonction de mise à jour de la date et de l'heure du lecteur:*/
extern unsigned short usMettreAJourDateHeure( IN char * pcIdRessourceLecteur,
                                       IN char * pcIdAppUtilisatrice,
                                       IN char * pcDateHeure,
                                       OUT void * pvRefus);
/* */
//typedef unsigned short (* LPFNDLL_MAJ_DATE_HEURE) (char *, char *, char *, void *);


/*  Pointeur sur la fonction de consultation d'informations concernant le lecteur:*/
extern unsigned short usConsulterEtatLecteur( IN char* pcIdRessourceLecteur, 
                                       IN char* pcIdAppUtilisatrice,
                                       OUT void* pvEtatLecteur);
/* */
//typedef unsigned short (* LPFNDLL_CONSULTER_ETAT_LECTEUR) (char *, char *, void *);


/*  Pointeur sur la fonction de mise à jour du logiciel noyau du lecteur:*/
extern unsigned short  usMettreAJourNoyau(IN char* pcIdRessourceLecteur,
                                    IN char* pcIdAppUtilisatrice, 
                                    IN char* pcNomFichierNoyau,
                                    IN char* pcNumeroVersion,
                                    OUT void* pvRefus);
/* */
//typedef unsigned short (* LPFNDLL_MAJ_NOYAU) (char *, char *, char *, char *, void *);


/*  Pointeur sur la fonction de mise à jour du logiciel superviseur du lecteur:*/
extern unsigned short usMettreAJourSuperviseurCarte(IN char* pcIdRessourceLecteur,
                                             IN char* pcIdAppUtilisatrice,
                                             IN char* pcNomFichierSuperviseur,
                                             IN char* pcNumeroVersion,
                                             OUT void* pvRefus);
/* */
//typedef unsigned short (* LPFNDLL_MAJ_SUPERVISEUR) (char *, char *, char *, char *, void *);


/*  Pointeur sur la fonction de mise à jour/installation d'une application sur le lecteur:*/
extern unsigned short usMettreAJourApplication (IN char* pcIdRessourceLecteur,
                                         IN char* pcIdAppUtilisatrice,
                                         IN char* pcIdApplication,
                                         IN char* pcNomFichierApplication,
                                         IN char* pcNumeroVersion,
                                         OUT void* pvRefus);
/* */
 //typedef unsigned short (* LPFNDLL_MAJ_APPLICATION) (char *, char *, char *, char *, char *, void *);


/* Pointeur sur la fonction de suppression d'une application sur le lecteur:*/
extern unsigned short usSupprimerApplication( IN char* pcIdRessourceLecteur,
                                       IN char* pcIdAppUtilisatrice,
                                       IN char* pcIdApplication,
                                       OUT void* pvRefus);
/* */
//typedef unsigned short (* LPFNDLL_SUP_APPLICATION) (char *, char *, char *, void *);


/* Pointeur sur la fonction de lecture de la version logicielle de l'API:*/
/*_DLL*/ unsigned short usLireVersionAPI( OUT char* pcVersionAPI);
/**/

//typedef unsigned short  (*LPFNDLL_LIRE_VERSION_API) (char * );


/***************************************/
/* Pointeurs des fonctions auxiliaires */
/***************************************/

/***** Paramètres de sortie Etat Lecteur *****/

/*  Pointeur sur la fonction d'allocation de structure:*/
extern void* pvCreerEtatLecteur(void);
/* */
//typedef void* (* LPFNDLL_CREER_STRUCT_ETAT_LECTEUR)();


/*   Pointeur sur la fonction de libération de structure:*/
extern unsigned short usLibererEtatLecteur(IN void* pvEtatLecteur);
/* */
//typedef unsigned short (* LPFNDLL_LIBERER_STRUCT_ETAT_LECTEUR)(void*);


/** Pointeur sur les fonctions de lecture des paramètres **/

/*  Pointeur sur fonction:*/
extern unsigned short usLecteurGetConstructeur( IN void* pvEtatLecteur,
                                         OUT char* pcConstructeur);
/* */
//typedef unsigned short (* LPFNDLL_GET_CONSTRUCTEUR)(void*, char*);


/* Pointeur sur fonction:*/
extern unsigned short usLecteurGetDesignationCommerciale( IN void* pvEtatLecteur,
                                                   OUT char* pcDesignComm);
//typedef unsigned short (* LPFNDLL_GET_DESIGN_COMM)(void*, char*);


/* Pointeur sur fonction:*/
extern unsigned short usLecteurGetType( IN void* pvEtatLecteur,
                                 OUT char* pcTypeLecteur);
/**/
//typedef unsigned short (* LPFNDLL_GET_TYPE)(void*, char*);


/* Pointeur sur fonction:*/
extern unsigned short usLecteurGetNumeroSerie(  IN void* pvEtatLecteur,
                                         OUT char* pcNumeroSerie);
/**/
//typedef unsigned short (* LPFNDLL_GET_NUMERO_DE_SERIE)(void*, char*);


/* Pointeur sur fonction:*/
extern unsigned short usLecteurGetTailleMemoire(  IN void* pvEtatLecteur,
                                           OUT char* pcTailleMemoire);
/**/
//typedef unsigned short (* LPFNDLL_GET_TAILLE_MEMOIRE)(void*, char*);


/* Pointeur sur fonction:*/
extern unsigned short usLecteurGetDate( IN void* pvEtatLecteur,
                                 OUT char* pcDateLecteur);
/**/
//typedef unsigned short (* LPFNDLL_GET_DATE)(void*, char*);


/* Pointeur sur fonction:*/
extern unsigned short usLecteurGetHeure(  IN void* pvEtatLecteur,
                                   OUT char* pcHeureLecteur);
/**/
//typedef unsigned short (* LPFNDLL_GET_HEURE)(void*, char*);


/* Pointeur sur fonction:*/
extern unsigned short usLecteurGetVersionNoyau( IN void* pvEtatLecteur,
                                         OUT char* pcVersionNoyau);
/**/
//typedef unsigned short (* LPFNDLL_GET_VERSION_NOYAU)(void*, char*);


/* Pointeur sur fonction:*/
extern unsigned short usLecteurGetVersionSuperviseur( IN void* pvEtatLecteur,
                                               OUT char* pcVersionSuperviseur);
/**/
//typedef unsigned short (* LPFNDLL_GET_VERSION_SUPERVISEUR)(void*, char*);



/**** Paramètres de sortie Refus ****/

/* Pointeur sur la fonction d'allocation de structure:*/
extern void* pvCreerRefus(void);
/**/
//typedef void* (* LPFNDLL_CREER_STRUCT_REFUS)();


/* Pointeur sur la fonction de libération de structure:*/
extern unsigned short usLibererRefus(IN void* pvEtatRefus);
/**/
//typedef unsigned short (* LPFNDLL_LIBERER_STRUCT_REFUS)(void*);


/** Pointeur sur les fonctions de lecture des paramètres **/

/* Pointeur sur fonction:*/
extern unsigned short usRefusGetIdApplication(IN void* pvRefus,OUT char* pcIdApplication);
/**/
//typedef unsigned short (* LPFNDLL_GET_ID_APPLICATION)(void*, char*);


/* Pointeur sur fonction:*/
extern unsigned short usRefusGetCommentaire(IN void* pvRefus,OUT char* pcCommentaire);
/* */
//typedef unsigned short (* LPFNDLL_GET_COMMENTAIRE)(void*, char*);




#endif
/*
% -------------------------------------------------------------------
% Fin du fichier svadmlec.h
% -------------------------------------------------------------------
*/
