/********************************* C_BDM_ApiVar.cpp ******************************
*  #include "C_BDM_ApiVar.h"                                                     *
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

// C++ Interface: C_BDM_ApiVar.cpp
//
#include "C_BDM_ApiVar.h"
#define   SKIP_BLANK_CAR(a)        while( *(a) && (*(a)==' ' || *(a)=='\t' || *(a)== 96) ) ++(a)
#define   NEXT_LINE(a)             while( *(a) && *(a)!='\r' && *(a)!='\n')++(a); while( *(a) && (*(a)=='\r'|| *(a)=='\n'))++(a)

//-----------------------------------------------------  C_DBVar -------------------------------------------
C_BDM_ApiVar::C_BDM_ApiVar(  const QString& confData /* = "" */)
{m_LastError   = "";
 if (confData.length()==0) initDefaultValues();
 else                      initValues(confData);
}
//-----------------------------------------------------  initDefaultValues -------------------------------------------
void C_BDM_ApiVar::initDefaultValues()
{ m_BDM_VERSION_TBL_NAME        =   "bdm_version";         // nom de la table des numeros de version
  m_BDM_VERSION_PK               =   "bdm_version_pk";      // clef primaire
  m_BDM_VERSION_OWNER            =   "bdm_version_owner";   // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
  m_BDM_VERSION_LANG             =   "bdm_version_lang";    // langue de la donnee (en fr etc...)
  m_BDM_VERSION_NUMBER           =   "bdm_version_number";  // numero de version
  //........................ MEDICA_TUX  Table de la liste des drogues .........................................
  m_BDM_DRUGLIST_TBL_NAME        =   "bdm_druglist";     // nom de la table de la liste des medicaments
  m_BDM_DRUGLIST_PK              =   "bdm_druglist_pk";           // clef primaire
  m_BDM_DRUGLIST_OWNER           =   "bdm_druglist_owner";        // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
  m_BDM_DRUGLIST_LANG            =   "bdm_druglist_lang";         // langue de la donnee (en fr etc...)
  m_BDM_DRUGLIST_ID              =   "bdm_id";           // identificateur unique du medicament
  m_BDM_DRUGLIST_TYPE_ID         =   "bdm_type_id";      // type d'identificateur (CIP CIP7 CIS)
  m_BDM_DRUGLIST_ATC             =   "bdm_atc";          // Code ATC
  m_BDM_DRUGLIST_LIBELLE         =   "bdm_libelle";      // libelle de la classe ATC
  m_BDM_DRUGLIST_DCI_1           =   "bdm_dci_1";        // premiere  substance DCI (libelle ATC)
  m_BDM_DRUGLIST_DCI_2           =   "bdm_dci_2";        // deuxieme  substance DCI (si produit compose)
  m_BDM_DRUGLIST_DCI_3           =   "bdm_dci_3";        // troisieme substance DCI (si produit compose)
  m_BDM_DRUGLIST_UCD             =   "bdm_ucd";          // code UCD
  m_BDM_DRUGLIST_UCD_PRICE       =   "bdm_ucd_price";    // Prix UCD
  m_BDM_DRUGLIST_GENERIQUE       =   "bdm_generique";    // generecite
  m_BDM_DRUGLIST_STATUT          =   "bdm_statut";       // statut (ville hopital)
  m_BDM_DRUGLIST_SPEC_FK         =   "bdm_spec_fk";      // pointeur sur les specifications produits
  //........................ MEDICA_TUX  Table du hit parade des posologies .........................................
  m_BDM_POSO_HIT_TBL_NAME        =   "bdm_poso_hit";                 // nom de la table de la table des favoris lies a un medicament
  m_BDM_POSO_HIT_PK              =   "bdm_poso_hit_pk";              // clef primaire
  m_BDM_POSO_HIT_OWNER           =   "bdm_poso_hit_owner";           // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
  m_BDM_POSO_HIT_LANG            =   "bdm_poso_hit_lang";            // langue de la donnee (en fr etc...)
  m_BDM_POSO_HIT_SCORE           =   "bdm_poso_hit_score";           // niveau du hit
  m_BDM_POSO_HIT_GRAMMAR         =   "bdm_poso_hit_grammar";         // posologie liee a ce favoris
  m_BDM_POSO_HIT_ORDER           =   "bdm_poso_hit_order";           // ordre rigidifie du hit donne la position du favori quelque soit son score
  m_BDM_POSO_HIT_CODE_INDIC      =   "bdm_poso_hit_code_indic";      // codage de l'indication liee a ce hit
  m_BDM_POSO_HIT_CODE_INDIC_TYPE =   "bdm_poso_hit_code_indic_type"; // type de codage CIM10 etc ....
  m_BDM_POSO_HIT_LIBELLE_INDIC   =   "bdm_poso_hit_libelle_indic";   // libelle de l'indication principale liee à ce favoris
  m_BDM_POSO_HIT_DRUG_ID         =   "bdm_poso_hit_drug_id";         // identificateur unique du medicament relie Ã  cet ASMR
  m_BDM_POSO_HIT_DRUG_TYPE_ID    =   "bdm_poso_hit_drug_id_type";    // type d'identificateur de l'id du medicament (CIP CIP7 CIS)
}
//-----------------------------------------------------  SetConfBase -------------------------------------------
int C_BDM_ApiVar::initValues( const QString& confData)
{
 QByteArray ba = confData.toLatin1();
 QString   err = "";
 char     *deb = (char*)(const char *)ba;
 char      *pt = deb;
 int      line = 0;

 pt = SetConfBase_SetProperties(pt,  m_BDM_VERSION_TBL_NAME,     "m_BDM_VERSION_TBL_NAME",    &line , err); if (err.length())     goto SetConfBase_Error;
 pt = SetConfBase_SetProperties(pt,  m_BDM_VERSION_PK,           "m_BDM_VERSION_PK",          &line , err); if (err.length())     goto SetConfBase_Error;
 pt = SetConfBase_SetProperties(pt,  m_BDM_VERSION_OWNER,        "m_BDM_VERSION_OWNER",       &line , err); if (err.length())     goto SetConfBase_Error;
 pt = SetConfBase_SetProperties(pt,  m_BDM_VERSION_LANG,         "m_BDM_VERSION_LANG",        &line , err); if (err.length())     goto SetConfBase_Error;
 pt = SetConfBase_SetProperties(pt,  m_BDM_VERSION_NUMBER,       "m_BDM_VERSION_NUMBER",      &line , err); if (err.length())     goto SetConfBase_Error;

 //........................ MEDICA_TUX  Table de la liste des drogues .........................................
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_TBL_NAME,  "m_BDM_DRUGLIST_TBL_NAME",  &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_druglist'  nom de la table de la liste des medicaments
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_PK,        "m_BDM_DRUGLIST_PK",        &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_pk'        clef primaire
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_OWNER,     "m_BDM_DRUGLIST_OWNER",     &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_owner'     Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_LANG,      "m_BDM_DRUGLIST_LANG",      &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_lang'      langue de la donnee (en fr etc...)
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_ID,        "m_BDM_DRUGLIST_ID",        &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_id'        identificateur unique du medicament
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_TYPE_ID,   "m_BDM_DRUGLIST_TYPE_ID",   &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_type_id'   type d'identificateur (CIP CIP7 CIS)
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_ATC,       "m_BDM_DRUGLIST_ATC",       &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_atc'       Code ATC
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_LIBELLE,   "m_BDM_DRUGLIST_LIBELLE",   &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_libelle'   libelle de la classe ATC
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_DCI_1,     "m_BDM_DRUGLIST_DCI_1",     &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_dci_1'     premiere  substance DCI (libelle ATC)
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_DCI_2,     "m_BDM_DRUGLIST_DCI_2",     &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_dci_2'     deuxieme  substance DCI (si produit compose)
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_DCI_3,     "m_BDM_DRUGLIST_DCI_3",     &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_dci_3'     troisieme substance DCI (si produit compose)
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_UCD,       "m_BDM_DRUGLIST_UCD",       &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_ucd'       code UCD
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_UCD_PRICE, "m_BDM_DRUGLIST_UCD_PRICE", &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_ucd_price' Prix UCD
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_GENERIQUE, "m_BDM_DRUGLIST_GENERIQUE", &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_generique' generecite
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_STATUT,    "m_BDM_DRUGLIST_STATUT",    &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_statut'    statut (ville hopital)
 pt =  SetConfBase_SetProperties(pt , m_BDM_DRUGLIST_SPEC_FK,   "m_BDM_DRUGLIST_SPEC_FK",   &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_pk_spec'   pointeur sur les specifications produits

 //........................ m_BDM_USER_MONOGRAPHIE_TBL_NAME  table des monographies utilisateur .........................................
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_TBL_NAME    ,  "m_BDM_USER_MONOGRAPHIE_TBL_NAME",  &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie'               // nom de la table des monographies utilisateur
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_PK          ,  "m_BDM_USER_MONOGRAPHIE_PK",        &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie_pk'            // clef primaire
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_OWNER       ,  "m_BDM_USER_MONOGRAPHIE_OWNER",     &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie_owner'         // Origine des donnees (AFSSAPS, Vidal, Thesorimed, Theriaque Claude Bernard, perso...)
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_LANG        ,  "m_BDM_USER_MONOGRAPHIE_LANG",      &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie_lang'          // langue de la donnee (en fr etc...)
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_ID          ,  "m_BDM_USER_MONOGRAPHIE_ID",        &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie_id'            // identificateur unique du produit afferent a cette monographie
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_TYPE_ID     ,  "m_BDM_USER_MONOGRAPHIE_TYPE_ID",   &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie_type_id'       // type d'identificateur (CIP CIP7 CIS)
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_LIBELLE     ,  "m_BDM_USER_MONOGRAPHIE_LIBELLE",   &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie_libelle'       // titre de la monographie
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_AUTEUR      ,  "m_BDM_USER_MONOGRAPHIE_AUTEUR",    &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie_auteur'        // auteur de la monographie
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_DATE        ,  "m_BDM_USER_MONOGRAPHIE_DATE",      &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie_date'          // date de la monographie
  pt =  SetConfBase_SetProperties(pt , m_BDM_USER_MONOGRAPHIE_HTML_BLOB   ,  "m_BDM_USER_MONOGRAPHIE_HTML_BLOB", &line , err); if (err.length())     goto SetConfBase_Error;  // 'bdm_user_monographie_blob'          // donnees de la monographie


 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_TBL_NAME,        "m_BDM_POSO_HIT_TBL_NAME",        &line , err); if (err.length())     goto SetConfBase_Error; // nom de la table de la table des favoris lies a un medicament
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_PK,              "m_BDM_POSO_HIT_PK",              &line , err); if (err.length())     goto SetConfBase_Error; // clef primaire
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_OWNER,           "m_BDM_POSO_HIT_OWNER",           &line , err); if (err.length())     goto SetConfBase_Error; // Origine des donnees (AFSSAPS, Vidal, Theriaque Claude Bernard, perso...)
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_LANG,            "m_BDM_POSO_HIT_LANG",            &line , err); if (err.length())     goto SetConfBase_Error; // langue de la donnee (en fr etc...)
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_SCORE,           "m_BDM_POSO_HIT_SCORE",           &line , err); if (err.length())     goto SetConfBase_Error; // niveau du hit
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_GRAMMAR,         "m_BDM_POSO_HIT_GRAMMAR",         &line , err); if (err.length())     goto SetConfBase_Error; // posologie liee a ce favoris
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_ORDER,           "m_BDM_POSO_HIT_ORDER",           &line , err); if (err.length())     goto SetConfBase_Error; // niveau du hit (nombre d'utilisations sera utilise pour son classement)
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_CODE_INDIC,      "m_BDM_POSO_HIT_CODE_INDIC",      &line , err); if (err.length())     goto SetConfBase_Error; // codage de l'indication liee a ce hit
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_CODE_INDIC_TYPE, "m_BDM_POSO_HIT_CODE_INDIC_TYPE", &line , err); if (err.length())     goto SetConfBase_Error; // type de codage CIM10 etc ....
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_LIBELLE_INDIC  , "m_BDM_POSO_HIT_LIBELLE_INDIC",   &line , err); if (err.length())     goto SetConfBase_Error; // libelle de l'indication principale
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_DRUG_ID,         "m_BDM_POSO_HIT_DRUG_ID",         &line , err); if (err.length())     goto SetConfBase_Error; // identificateur unique du medicament relie à ce hit
 pt =  SetConfBase_SetProperties(pt , m_BDM_POSO_HIT_DRUG_TYPE_ID,    "m_BDM_POSO_HIT_DRUG_TYPE_ID",    &line , err); if (err.length())     goto SetConfBase_Error; // type d'identificateur de l'id du medicament (CIP CIP7 CIS)
 return 1;

SetConfBase_Error:
 m_LastError += err;
 return 0;
}

//--------------------------------------------- SetConfBase_SetProperties ------------------------------------------------
char  *C_BDM_ApiVar::SetConfBase_SetProperties(char *pt, QString &propertie, const char *token, int *line , QString &err)
{int nLine      = *line;
err            = "";
long len_token = strlen (token);
while(*pt)
   {SKIP_BLANK_CAR(pt);
    if (strncmp(pt, token, len_token)==0)
       {//return pt+len_token;
        pt         += len_token;
        err         = "";
        propertie   = "";
        //............. chercher premier delimiteur ...............................
         while(*pt && *pt!='\r' && *pt !='\n' && *pt !='\'' ) pt++;
         if (*pt=='\'')         // si delimiteur pas trouve erreur
            {pt++;               // passer le delimiteur
             char *end = pt;
             //............. chercher deuxieme delimiteur ...............................
             while(*end && *end!='\r' && *end !='\n' && *end !='\'')end++;
             if (*end=='\'')
               { propertie= QString::fromLatin1 ( pt, end-pt );
                 if (line) *line = nLine;
                 return end +1;
                }
             else
               {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n End delimiter \"'\"  not find at line : ");
                err += QString::number(nLine);
                if (line) *line = nLine;
                return 0;
               }
            }
          else
            {err  = TR("Syntax Error: SetConfBaseGetValue()\r\n Start delimiter \"'\"  not find at line : ");
             err += QString::number(nLine);
             if (line) *line = nLine;
             return 0;
            }
       } //endif (strncmp(pt, token, len_token)==0)
    NEXT_LINE(pt);
    ++nLine;
   }
err  = TR("\nErreur de syntaxe : C_DBVar::SetConfBase_SetProperties()"
          "\nle fichier de configuration des bases 'DataBase.cfg'"
          "\ncomporte les erreurs suivantes : \n"
          "      ==> la mention %1 est non trouv\303\251e \303\240 la ligne : %2").arg(token, QString::number(nLine));
if (line) *line = nLine;
return pt;
}
