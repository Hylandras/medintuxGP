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
 *   CPrinter.h                                                            *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CPRINTER_H
#define CPRINTER_H


#include <qwidget.h>
#include <qevent.h> 
#include <qpixmap.h> 
#include <qpainter.h> 
#include <qfont.h> 
#include <qlabel.h> 
#include <q3simplerichtext.h> 
#include <q3paintdevicemetrics.h>
#include <qprinter.h>
#include <qpalette.h> 
//Added by qt3to4:
#include <QPaintEvent>

//====================================== CPageDef ==============================================================
class CPageDef
{
 //........................................... METHODES ...........................................................   
 public:
     CPageDef(     float   cm_PageW       = 21.0,
                   float   cm_PageH       = 29.7,
                   float   cm_MrgLeft     = 1.0,
                   float   cm_MrgTop      = 0.2,
                   float   cm_MrgRight    = 1.0,
                   float   cm_MrgBottom   = 0.1,
                   float   cm_HeadH       = 3.0,
                   float   cm_FootH       = 2.0,
		   float   scale          = 0.5,
		   float   print_scale    = 1,
		   QPaintDevice * pd      = 0
	           
             );
    ~CPageDef();
    
 void     SetResolution(int resol_X, int resol_Y);   
 int      GetScreenTextWorkSpaceWidth();
 int      GetScreenTextWorkSpaceHeightBody(); 
 int      GetScreenTextWorkSpaceHeightHead();
 int      GetScreenTextWorkSpaceHeightFoot();
 int      GetScreenTextWorkSpaceHeight();
 int      GetResolutionX();
 int      GetResolutionY();

 //........................................... DATA ...............................................................
 public: 
    float   m_Cm_PageW;
    float   m_Cm_PageH;
    float   m_Scale;          // zoom pour visualiser le document dans le widget
    float   m_PrintScale;     // zoom pour imprimer le document
    float   m_Cm_MrgLeft;
    float   m_Cm_MrgTop;
    float   m_Cm_MrgRight;
    float   m_Cm_MrgBottom;
    float   m_Cm_HeadH;
    float   m_Cm_FootH; 
    int     m_ScreenResX;
    int     m_ScreenResY;
    
};

//====================================== CViewPageWidget =======================================================
class CViewPageWidget : public QWidget
{
 Q_OBJECT
 //........................................... METHODES ...........................................................   
 public:
      CViewPageWidget(
		     QWidget*         parent           = 0, 
		     const char*      name             = 0, 
		     Qt::WFlags           f                = 0,
		     Q3SimpleRichText* pRichTextHead    = 0, 
		     CPageDef*        pCPageDef        = 0, 
                     Q3SimpleRichText* pRichTextBody    = 0, 
		     Q3SimpleRichText* pRichTextFoot    = 0 
		    );
      ~CViewPageWidget(){};
   void             SetBody(Q3SimpleRichText* pRichText);    
   void             SetHead(Q3SimpleRichText* pRichText);    
   void             SetFoot(Q3SimpleRichText* pRichText);    
   int              GetResolutionX();
   int              GetResolutionY();
   void             SetResolution(int resol_X, int resol_Y);
   int              GetWidgetWidth();
   int              GetWidgetHeight();
   
   int              GetNbPage();
   int              GetCurrentPage();
   int              SetCurrentPage(int page);
   
   float            GetPageW();
   float            GetPageH();
   
   float            GetLeftMarg();
   float            GetTopMarg();
   float            GetRightMarg();
   float            GetBottomMarg();
   float            GetHead_H();
   float            GetFoot_H();
   float            GetPrintScale();
   
   float            GetRichHead_H();
   float            GetRichBody_H();
   float            GetRichFoot_H();
   
   float            SetLeftMarg(float value=0);
   float            SetTopMarg(float value=0);
   float            SetRightMarg(float value=0);
   float            SetBottomMarg(float value=0);
   float            SetHead_H(float value=0);
   float            SetFoot_H(float value=0);
   float            SetPrintScale(float value=0);
   
   //................... impression (aussi bien à l'ecran que sur imprimante) ..............
   int              PrintDoc(QPainter*  p, int page=1, float zoomX =1.0, float zoomY =1.0, int flag=1);
   int              PrintDoc(QPainter*  p, int page, float zoomX, float zoomY , int flag , int resX, int resY);

 protected:
   void             paintEvent( QPaintEvent *e );
    
    
 //........................................... DATA ...............................................................
 protected:    
    CPageDef            m_CPageDef;
    Q3SimpleRichText*   m_pRichTextHead;
    Q3SimpleRichText*   m_pRichTextBody;
    Q3SimpleRichText*   m_pRichTextFoot;

    int                m_ScreenResX;
    int                m_ScreenResY;
    int                m_CurrentPage;
};




#endif
