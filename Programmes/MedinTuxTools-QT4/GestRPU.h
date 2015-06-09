// GestRPU.h: interface for the CGestRPU class.
/***************************************************************************
 *   Copyright (C) 2003 by Sevin Roland                                    *
 *   upsis@club-internet.fr                                                *
 *                                                                         *
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
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GESTRPU_H__C18A039B_7AF3_4D5B_8E1C_61CA6A972103__INCLUDED_)
#define AFX_GESTRPU_H__C18A039B_7AF3_4D5B_8E1C_61CA6A972103__INCLUDED_

#include <qdatetime.h>
#include <q3ptrlist.h>
#include <qdir.h>
#include <qobject.h>
#include "CGestIni.h"
#include "../drtux/src/CMoteur_Base.h"

#define RPU_TEST       1
#define CCMU_TEST      2
#define GEMSA_TEST     4
#define DEST_TEST      8
#define DIVERS_TEST    16
#define NO_TEST_DATE   32
#define ALL_RPU_TEST    (16+8+4+2+1)

#define SAU     '1'
#define POSU    '2'
#define UPATOU  '3'

#define BOOL bool
#define TR QObject::tr
#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))

#define WARNING_HEAD ":: WARNING "

typedef struct
 {char CodeAct[5];
  char NbAct[3];
 }ACTE_RPU;

typedef struct
 {char Num_Finess[10];
  char TypeAutorisation[2];
  char NumEnr[13];
  char sexe[2];
  char DtNaiss[9];
  char CodePostal[6];
  //....................
  char DtArrivee[9];
  char HeureArrivee[5];
  char TranspMedic[2];
  char TranspTyp[2];
  char Decede[2];
  char PEC_Typ[2];
  char AvisSpe[2];
  char CIM10[7];
  char Agressif[2];
  char Communication[2];
  char SocialEtAdmin[2];
  ACTE_RPU ActeRpu[10];
  char DtIssue[9];
  char HeureIssue[5];
  char Devenir[2];
  char TranspHosp[2];
  char CIM10_Str[256];
 }DATA_RPU;

typedef struct
 {long ccmu0;
  long ccmu1;
  long ccmu2;
  long ccmu3;
  long ccmu4;
  long ccmu5;
 }STRUCT_CCMU;

typedef struct
 {long indeter;
  long hospi;
  long trans;
  long exter;
 }STRUCT_DEST;

typedef struct
 {long gemsa_1;
  long gemsa_2;
  long gemsa_3;
  long gemsa_4;
  long gemsa_5;
  long gemsa_6;
  long gemsa_7;
  long gemsa_I;
 }STRUCT_GEMSA;

typedef struct
 {long Deces;
  long RetDom;
  long Hosp;
  long Ind;
 }STRUCT_DIVERS;

//============================================================  SSignal ===================================================================================
class SSignal
{
public:
 int setFromGUID(const QString& guid, CMoteurBase* pCMoteurBase);
 QString m_Nom;
 QString m_Prenom;
 QString m_codePostal;
 char    m_sexe[20];
 QDate   m_Naissance; //1970-01-01 00:00:00
 QString m_GUID;
};

//============================================================  ERR_RPU ===================================================================================

#define NB_NOM     63
#define NB_PRENOM  63
#define NB_DATE    10
#define NB_MEDECIN 63
#define NB_MESSAGE 255
#define NB_GUID    36

class ERR_RPU
{
public:
  ERR_RPU(const char* nom, const char* prenom, const char* date, const char* medecin, const char* message, const char* id)
         {strncpy(Nom,     nom,      NB_NOM);           Nom[NB_NOM]=0;
          strncpy(Prenom,  prenom,   NB_PRENOM);        Prenom[NB_PRENOM]=0;
          strncpy(Date,    date,     NB_DATE);          Date[NB_DATE]=0;
	  strncpy(Medecin, medecin,  NB_MEDECIN);       Medecin[NB_MEDECIN]=0;
          strncpy(Message, message,  NB_MESSAGE);       Message[NB_MESSAGE]=0;
	  strncpy(ID,      id,       NB_GUID);          ID[NB_GUID]=0;
         };

public:
  char Nom[NB_NOM+1];
  char Prenom[NB_PRENOM+1];
  char Date[NB_DATE+1];   //    ../../....
  char Medecin[NB_MEDECIN+1];
  char Message[NB_MESSAGE+1];
  char ID[NB_GUID+1];          //30DC82DD-7632-2C46-9DC3-BC1AC0ECD20A
};
//..................... Liste de la classe definie au dessus .........................................................
typedef Q3PtrList<ERR_RPU> ERRPtrList;

//============================================================  CGestRPU ===================================================================================
class CGestRPU
{
public:
 QString        m_ED_Finess;
 char           m_Qualif;
 STRUCT_CCMU    m_CCMU;
 STRUCT_DEST    m_DEST;
 STRUCT_GEMSA   m_GEMSA;
 STRUCT_DIVERS  m_DIVERS;
 ERRPtrList     m_ErrRpuList;
 long         m_NbErr;
 long         m_RPU_Record;
 long         m_TestFlag;
 long         m_NbFicWrited;
 QString      m_csErrPath;    // path des erreurs de rejet de dossier
 QString      m_csLogPath;    // path du fichier RPU à creer
 QString      m_csRpuPath;    // path du fichier RPU
 QDate        m_date_Debut;
 QDate        m_date_Fin;
 QDate        m_date_AgeDeb;
 QDate        m_date_AgeFin;
 int          m_Hdeb;
 int          m_Mdeb;
 int          m_Hfin;
 int          m_Mfin;

public:
    CGestRPU( long flag=ALL_RPU_TEST);
    CGestRPU( QDate &date_Debut, int hdeb, int mdeb, QDate &date_Fin,int hfin, int mfin,
              long flag);
    CGestRPU( QDate &date_Debut,int hdeb, int mdeb, QDate &date_Fin,int hfin, int mfin,
              const char *errPath =0, const char *logPath =0,
              const char *rpuPath=0,  long flag = ALL_RPU_TEST,
              char qualif='3',        const char *finess="123456789");
    CGestRPU( QDate &date_Debut,int hdeb, int mdeb, QDate &date_Fin,int hfin, int mfin,
              const char *errPath ,   const char *errName =0,
              const char *logPath =0, const char *logName =0,
              const char *rpuPath =0, const char *rpuName =0,
              long flag=ALL_RPU_TEST, char qualif  = '3' , const char *finess  = "123456789"
              );
    virtual ~CGestRPU();

    void InitRpuTest(long flag = 0, const char *errPath=0, const char *logPath=0, const char *rpuPath=0, char qualif=0, const char *finess=0);
    void SetRpuQualif(char qualif , const char *finess );

    void ResetErrorList(void);

    int  GetObservDateDeb(const char *observ, long len, QDateTime &coDt_deb, long &D,long &M,long &Y,long &h,long &m,long &s);
    int  GetObservDateEnd(const char *observ, long len, QDateTime &coDt_end, long &D,long &M,long &Y,long &h,long &m,long &s);
    bool IsDateInfDebLimit(long D,long M,long Y,long h,long m,long s);
    bool IsDateSupEndLimit(long D,long M,long Y,long h,long m,long s);
    int  DebugDATA_RPU(DATA_RPU &dataRpu, SSignal *pSignalPatient, QDateTime &coDt_deb, const char* medecin);
    long RPU_TesteRecord(SSignal *pSignal, const char* observ, long len_observ, const char* medecin="");
    void RPU_ParseObservation(const char *observ, long len, DATA_RPU &dataRpu);
    void RPU_Make(SSignal *pSignal, const char *observ, long len_observ, QDateTime &coDt_deb, QDateTime &coDt_end, int dst,  const char* medecin);
    int  CompteCCMU(SSignal *pSignalPatient, const char *pt, long len, STRUCT_CCMU &CCMU, QDateTime &coDt_deb, const char *medecin);
    int  CompteGEMSA(SSignal *pSignalPatient, const char *pt, long len, STRUCT_GEMSA &GEMSA , QDateTime &coDt_deb, const char *medecin);
    int  CompteDEST(SSignal *pSignalPatient, const char *pt, long len, STRUCT_DEST &DEST, QDateTime &coDt_deb, const char *medecin);
    int  CompteDIVERS(SSignal *pSignalPatient, const char *pt, long len, STRUCT_DIVERS &DIVERS , QDateTime &coDt_deb, const char *medecin);
    int  RPU_Verifie(DATA_RPU &dataRpu, SSignal *pSignalPatient)  ;
    int  RPU_Write(DATA_RPU &dataRpu, SSignal *pSignalPatient, QDateTime &coDt_deb, int dst, const char* medecin);
    long CalculeDureeSejour(DATA_RPU &dataRpu, long *jour=0, long *heure=0, long *minutes=0, long *secondes=0);

    int  InsertError(SSignal *pSignalPatient, const char* date, const char* mess, const char* medecin="");
    int  WriteErreur(const char *mess);
    int  WriteLogFile(const char *mess);
    int  WriteRpuFile(const char *mess);
    int  _strnicmp(const QString &t1, const QString &t2, int n);
    QString * PathConcat(QString &path, const char*  name);
    QString * PathConcat(const char* path, const char*  name, QString* dst);
    char  *PathConcat(const char* path, const char*  name, char* dst);
    char  *PathConcat(char* path, const char*  name);
    QString ValJJMMAAAA_To_Str(long d, long m, long y, char *sep=0);
    long    JJMMAAAA_To_Val(const char* date_in, long *d=0, long *m=0, long* y=0);
    long    HeureMinuteSeconde_To_Val(const char* heure_in, long *h=0, long *m=0, long* s=0);
    void NbSec_To_NbJHMS(long secondes_in, long*jr=0, long *hr=0, long *mr=0, long* sr=0);
    long DifferenceSeconde(QDateTime date1, QDateTime date2);
    bool    ExtraitChiffre(char **pt_t, long *result, char* end);
    long LogCreate(const char *txt, const char *path, const char *name =0);
    long LogWrite(const char *txt, const char *path, const char *name =0);
};

#endif // !defined(AFX_GESTRPU_H__C18A039B_7AF3_4D5B_8E1C_61CA6A972103__INCLUDED_)
