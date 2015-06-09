/************************** CHtmlTools.h ***********************************
 *   Copyright (C) 2004 by Sevin Roland                                    *
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
#ifndef CHtmlTools_H
#define CHtmlTools_H

#include <qlistview.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtextedit.h>
#include <qfont.h>


#define END_PAR              ""

typedef struct Color_Ref
                 {unsigned char r;
                  unsigned char g;
                  unsigned char b;
                  char html_color[8];
                 } COLOR_DEF;

#define Min(a, b)             ((a) < (b) ? (a) : (b))
#define Max(a, b)             ((a) > (b) ? (a) : (b))

//===============================================================  CHtmlTools =========================================================================
class CHtmlTools
{
 public:
     CHtmlTools(){};
    ~CHtmlTools(){};
  enum flags
         {   StrictASCII         = 0x0002,
             KeepBR              = 0X0004,
             commaProtect        = 0X0008,
             KeepP               = 0X00010
         };
  enum flagsCopy
         {   allObjects          = 0x0003,
             copyDir             = 0x0001,
             copyFiles           = 0x0002
         };

  static void     tagPreToHtml( QString &txt);
  static QString  checkIfImageAfter(const QString &qs_text);
  static QString  checkIfImageBefore(const QString &qs_text);

  static void     insertHtml(QTextEdit* dstEdit, const QString &htmlStr,  int  noInsertIfJustTag  = 1 );
  static QString  removeHtmlTag( const QString &txt);
  static void     setDefaultTextAttributs(QTextEdit*     edit,
					  const QString &backColor ,
					  const QString &backPixmapPath,
					  const QString &textColor,
					  const QFont   &font);
  static int      QT_style_To_QFont( const QString &style, QFont &font,
				     const char* defaultFamily = 0, const char *defaultSize = 0);
  static long     getTextImageList( const QString &txt, QStringList &imgList, long from = 0 );
  static long     IsThisTagImage(const QString &txt, long &from , QString *fileName  = 0  , int *w  = 0 , int *h  = 0 );
  static void     ReplaceImageTag(QString &text, const QString &imgNameToReplace, QString imgNameReplaceBy = QString::null, int w = -1, int h = -1);
  static QString  getTextDefaultAttribut( const QString &txt, const QString &attribut);
  static int      setTextDefaultAtribut(QString &txt, const QString &attribut, const QString &value);
  static void  ListerAncres(const QString &txt, QStringList &qstringList, QString name="", long from=0);
  static void  ListerAncres(const QString &txt, QListView *pQListView, QString name ="", long from =0, QStringList *pQStringList =0);
  static void  ListerAncres(const QString &txt, QListViewItem *pQListViewItem, QString name ="", long from =0, QStringList *pQStringList =0);
  static void  ListerLiens(const QString &txt, QListView *pQListView, long from =0);
  static void  EffacerLien(QString &txt, int posLink,  long from =0);
  static void  ElimineAncresRedondantes(QString &txt, QString name ="", long pos=0);
  static void  EffacerAncre(QString &txt, const QString name , long from =0);
  static long  IsThisTagAnchor(const QString &txt, long &from,  QString *name=0  , QString *text=0 );
  static long  IsThisTagLien  (const QString &txt, long &from,  QString *name=0  , QString *text=0 );
  static long  IsThisTagExist (const QString &txt, const QString &tagName, long &from,  QString *name=0  , QString *text=0);
  static void     findReplace( QString &txt, const QString &find, const QString &replaceBy, bool cs = TRUE );
  static QString  ifNeed_Convert_RtftoHtml(QString &strData);
  static void     RtfToQtextEdit_HTML(const char *txt, long len_rtf, QString &stringDST, int with_header = 1 );
  static QString  HtmlToAscii(QString &str, int mode = 0);
  static void     QRichTextToHtml(QString &str);
  static int      IsRtf (const QString& str);
  static int      IsRtf (const char* txt, int limitFindTo = 10);
  static char    *JumpObject(char *pt, char *end_rtf);
  static int      IsRtfControlCar(char *pt);
  static char    *JumpAcolades(char *pt, char *end_rtf);
  static long     RTFGetNbColor(char *pt, char *end_rtf);
  static char    *RTF_to_RGBTab(char *pt, char *end_rtf,  COLOR_DEF* rgbTab, long nb_color);
  static char    *RGB_to_HTML(unsigned char rgb, char *buf);
  static QString  Copy_File(const QString &fileSrc, const QString &pathDst, QString *file_name =0);
  static void     Copy_Dir(QString src_Dir,  QString dest_Dir, int mode=7, const QString &incFilter="", const QString &excFilter="");
  static void     eraseDirectory( QString dir_to_erase);
  static char*    HtmlFind(const char* pt_in, QString motif, char **debMotif =0);
  static char*    IfHtmlTagJumpIt(char* pt);
  static QString  ddMMyyyy_to_yyyyMMdd(const QString & dateTime ,const QString& sep  = "");
  static QString  yyyyMMdd_to_ddMMyyyy(const QString & dateTime ,const QString& sep  = "");

};

#endif
