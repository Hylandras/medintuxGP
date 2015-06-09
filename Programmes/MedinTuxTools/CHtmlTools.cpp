/*************************** CHtmlTools.cpp ********************************
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
#include <stdlib.h>

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qfont.h>
#include <qpalette.h>
#include <qpixmap.h>
#include <qcolor.h>
#include <qobject.h>
#include <qapplication.h>
#include <qclipboard.h>
#include "CHtmlTools.h"

#define TR QObject::tr
#define DEB_PAR              "<br />"

//---------------------------------- tagPreToHtml ---------------------------------------------------------------------------
void CHtmlTools::tagPreToHtml( QString &txt)
{long deb_tag = 0;
 long end_tag = 0;
 long pos     = 0;
 while (pos<(int)txt.length())
    {deb_tag = txt.find("<pre>", pos, FALSE);
     if (deb_tag != -1)
	    {pos      = deb_tag + 5;
	     end_tag  = txt.find("</pre>", pos, FALSE);  if (end_tag==-1)  return;   //erreur de syntaxe
         while(pos<end_tag)
	        {char cc = txt.at(pos).latin1();

             if (cc=='>')
	            {txt.replace(pos,1,"&gt;");
	             pos     += 4;
		         end_tag += 3; // y avait déjà le caractère > donc que +3
	            }
	         else if (cc=='<')
	            {txt.replace(pos,1,"&lt;");
	             pos     += 4;
		         end_tag += 3; // y avait déjà le caractère < donc que +3
	            }
	         else ++pos;
	        }
       }
     else
       {pos = txt.length();
       }
    }
}

//---------------------------------- tagPreToHtml ---------------------------------------------------------------------------
QString CHtmlTools::removeHtmlTag( const QString &txt)
{int posDeb  = -1;
 int posEnd  =  0;
 int debSign =  0;
 int endSign =  0;
 QString strRet;
 while ( (posDeb  =  txt.find("<", posDeb + 1)) != -1  )
       {  posEnd  =  txt.find(">", posDeb + 1);
          endSign = posDeb;                          //       fin des caractères significatifs avant '<'
          if (posEnd != -1)
            {QString tag = txt.mid(posDeb, posEnd-posDeb);
             if (tag == "<br /" || tag=="<br")
                 {strRet +=  "\n";
                 }
             else if (tag.left(8)=="<img src")
                 {strRet = txt;
                  return strRet;
                 }
             else if (endSign>debSign)
                 {while( (txt[debSign]=='\r' || txt[debSign]=='\n') && debSign<endSign) ++debSign;
                  strRet +=  txt.mid(debSign, endSign-debSign);
                 }
             debSign = posEnd + 1;                    //       debut des caractères significatifs après '>'
             posDeb  = posEnd;
            }
       }
 return strRet;
}

//---------------------------------- findReplace ---------------------------------------------------------------------------
void CHtmlTools::findReplace( QString &txt, const QString &find, const QString &replaceBy, bool cs /* = TRUE */)
{int posDeb        = -1;
 int posEnd        =  0;
 int debSign       =  0;
 int endSign       =  0;
 QString interTag  = "";
 int     oldLen    = -1;
 int     newLen    = -1;
 int deltaLen      = 0;
 txt += "<toRmv>"; // etre certain qu'au moins un tag  html y soit (sinon pas de boucle)
 while ( (posDeb  =  txt.find("<", posDeb + 1)) != -1  )
       {  posEnd  =  txt.find(">", posDeb + 1);
          endSign = posDeb;                          //       fin des caractères significatifs avant '<'
          if (posEnd != -1)
             {deltaLen = 0;
              if (endSign>debSign)
                 {interTag = txt.mid(debSign,endSign-debSign);
                  oldLen   = interTag.length();
                  interTag = interTag.replace ( find, replaceBy, cs );
                  newLen   = interTag.length();
                  deltaLen = newLen - oldLen;
                  txt.replace ( debSign, endSign-debSign, interTag );
                 }
              debSign = posEnd + deltaLen + 1 ;                    //       debut des caractères significatifs après '>'
              posDeb  = posEnd + deltaLen;

             }
       }
 txt.truncate (txt.length()-7); // virer le "<toRmv>"
}

//----------------------------------------- checkIfImageAfter ----------------------------------------------------------------------
/*! \brief Fonction qui vérifie si collée devant le curseur il y a un tag image et auquel cas en retourne le nom
 *  \param qs_text : QString qui contient le bloc allant du curseur texte jusqu'à la fin du texte
 *  \return Renvoie un QString contenant le nom de l'image collée au curseur sinon vide
*/

QString CHtmlTools::checkIfImageAfter(const QString &qs_text)
{int deb_tag            = qs_text.find("<img");          // chercher le premier tag image en partant du debut du bloc
 int end_tag            = -1;
 int deb                = -1;
 int pos                = -1;
 long end               = -1;
 QString           ret  = "";
 QString           tag  = "";
 if (deb_tag != -1)
    {end_tag            =  qs_text.find(">",deb_tag+4);              // chercher fin du tag image
     if (end_tag !=-1)
        {deb = 0;                  // on part après le curseur (debut du bloc)
         end = deb_tag;            // pour parcourir jusqu'au debut du tag image
         while (deb<end)           // et chercher si il y a un caractère non tag entre (auquel cas il ne faudra pas compter l'image)
               {if (qs_text[deb]=="<") {pos = qs_text.find('>',deb+1);
                                        if (pos==-1)                   return ret;       // la il y a erreur car '>' non trouve et ça deconne
                                        ++pos;
                                        tag = qs_text.mid(deb,pos-deb).remove('/').remove(' ').lower();
                                        if (tag=="<br>"||tag=="<p>")   return ret;
                                        deb = pos;
                                       }
                else                                                   return ret;       // la on est ailleurs que dans un tag donc sur un truc entre le curseur et l'image
               }
         //............. si on est la c'est que le tag image trouve est 'contre' le curseur et doit être effacé .............
         //              donc retourner le nom de l'image
         tag  = qs_text.mid(deb_tag,end_tag-deb_tag+1);
         end  = 0;
         deb  = IsThisTagImage(tag, end , &ret ); // en entrée nameFind = "" car on recherche toutes les ancres
         if (deb>=0) return ret;           // si tout est OK
         ret  = "";
        }
    }
 return ret;
}

//----------------------------------------- checkIfImageBefore ----------------------------------------------------------------------
/*! \brief Fonction qui vérifie si collée en arriere du curseur il y a un tag image et auquel cas en retourne le nom
 *  \param qs_text : QString qui contient le bloc allant du curseur texte jusqu'au début du texte
 *  \return Renvoie un QString contenant le nom de l'image collée au curseur sinon vide
*/
QString CHtmlTools::checkIfImageBefore(const QString &qs_text)
{int deb_tag            = qs_text.findRev("<img");          // chercher le premier tag image en partant de la fin du bloc
 int end_tag            = -1;
 int deb                = -1;
 int pos                = -1;
 long end               = -1;
 QString           ret  = "";
 QString           tag  = "";
 if (deb_tag != -1)
    {end_tag            =  qs_text.find(">",deb_tag+4);              // chercher fin du tag image
     if (end_tag !=-1)
        {deb = end_tag+1;              // on repart après la fin du tag d'image
         end = qs_text.length();       // pour parcourir jusqu'au curseur texte (fin du bloc)
         while (deb<end)               // et chercher si il y a un caractère non tag entre (auquel cas il ne faudra pas compter l'image)
               {if (qs_text[deb]=="<") {pos = qs_text.find('>',deb+1);
                                        if (pos==-1)                   return ret;       // la il y a erreur car '>' non trouve et ça deconne
                                        ++pos;
                                        tag = qs_text.mid(deb,pos-deb).remove('/').remove(' ').lower();
                                        if (tag=="<br>"||tag=="<p>")   return ret;
                                        deb = pos;
                                       }
                else                                                   return ret;       // la on est ailleurs que dans un tag donc sur un truc entre le curseur et l'image
               }
         //............. si on est la c'est que le tag image trouve est 'contre' le curseur et doit être effacé .............
         //              donc retourner le nom de l'image
         tag  = qs_text.mid(deb_tag,end_tag-deb_tag+1);
         end  = 0;
         deb  = IsThisTagImage(tag, end , &ret ); // en entrée nameFind = "" car on recherche toutes les ancres
         if (deb>=0) return ret;           // si tout est OK
         ret  = "";
        }
    }
 return ret;
}

//---------------------------------- tagPreToHtml ---------------------------------------------------------------------------
void CHtmlTools::ReplaceImageTag( QString &text, const QString &imgNameBefore, QString imgNameAfter /*QString::null */, int w /* = -1 */, int h /* = -1 */)
{
 QString     imgName;
 //................. cette version ne place que les images de la liste ....................
 //                  auquelles fait référence le texte
 long       from   = 0;
 long    deb_tag   = 0;
 QString       tag = "";
 int ori_W, ori_H;
 int  oriTagLen;
 if (imgNameAfter.length()==0) imgNameAfter = imgNameBefore;
 while (from>=0 && from < (long)text.length() )
    {
     if ( (deb_tag     = IsThisTagImage(text, from , &imgName, &ori_W, &ori_H)) >=0  && imgNameBefore == imgName)    // rechercher le prochain tag image du texte
        {oriTagLen     =  from - deb_tag;
         tag  = "<img src=\"";
         tag += imgNameAfter + "\"";
         if (w != -1) ori_W = w;
         if (h != -1) ori_H = h;
         if (ori_W>0) tag  += " width=\""  + QString::number(ori_W) + "\"";
         if (ori_H>0) tag  += " height=\"" + QString::number(ori_H) + "\"";
         tag  += ">";
         from += tag.length() - oriTagLen;                 // reajuster la fin du tag
         text.replace (deb_tag, oriTagLen, tag);
        }
     //<img height=400 src=Image-0001h400w500 width=500 >
    } //end while (from>=0 && from<txt.length())     essai2.html
}

//---------------------------------- setDefaultTextAttributs ---------------------------------------------------------------------------
void CHtmlTools::setDefaultTextAttributs(QTextEdit*     edit,
					 const QString &backColor ,
					 const QString &backPixmapPath,
					 const QString &textColor,
					 const QFont   &font)
{if (backColor.length())
    {edit->setPaper (QBrush( QColor(backColor)));
    }
 if (backPixmapPath.length())
    {edit->setPaper (QBrush( QColor(), QPixmap (backPixmapPath ) ));
    }
 if (textColor.length())
    {edit->setPaletteForegroundColor (QColor (textColor) );
     QPalette palette(edit->palette());
     palette.setColor ( QColorGroup::Text, QColor (textColor) );
     edit->setPalette (palette);
    }
 edit->setFont(font);
}

//---------------------------------- QT_style_To_QFont ---------------------------------------------------------------------------
int CHtmlTools::QT_style_To_QFont( const QString &style, QFont &font,
				   const char* defaultFamily /*=0*/, const char *defaultSize /*=0*/)
{// "font-size:8pt; font-family:Sans"
 if (style.length()==0) return 0;
 QString family("");
 QString size("");
 int pos = style.find("font-size:");
 if (pos !=-1)
    {pos += 10;
     while (pos<(int)style.length() && style[pos]>='0' && style[pos]<='9')
           {size += style[pos];
	    ++pos;
	   }
    }
 pos = style.find("font-family:");
 if (pos !=-1)
    {pos += 12;
     while (pos<(int)style.length() && style[pos]!=';' && style[pos]!='\"' && style[pos]!='>')
           {family += style[pos];
	    ++pos;
	   }
     if (family.lower()=="sans" && defaultFamily)   family = defaultFamily;
    }
 if (family.length()==0 && size.length()==0) return 0;
 if (family != "")          font.setFamily ( family );
 else if (defaultFamily)    font.setFamily ( defaultFamily );
 if (size   != "")          font.setPointSize ( atoi(size) );
 else if (defaultSize)      font.setPointSize ( atoi(defaultSize) );
 return 1;
}

//------------------------------------- insertHtml --------------------------------------------------------------------------
void CHtmlTools::insertHtml(QTextEdit* dstEdit, const QString &htmlStr, int  noInsertIfJustTag /* = 1 */)
{if (htmlStr.length()==0)                            return;
 if (noInsertIfJustTag && removeHtmlTag( htmlStr).length()==0) return;
 QTextEdit *pQTextEdit = new QTextEdit(); //juste pour avoir fonctions copier/coller
 if (pQTextEdit)
     {//QFont      font;
      pQTextEdit->setTextFormat(QTextEdit::RichText);
      //QString style = CHtmlTools::getTextDefaultAttribut(htmlStr, "style");
      //QString old_clip = (QApplication::clipboard())->text ( QClipboard::Clipboard );
      //(QApplication::clipboard())->clear ( QClipboard::Clipboard );
      //qDebug(htmlStr);

      pQTextEdit->setText(htmlStr);
      pQTextEdit->selectAll();
      //if (pQFont)
      //   {pQTextEdit->setFont(*pQFont);
      //	  qDebug(QString::number(pQTextEdit->font().pointSize () ));
      //	 }
      //if (zoom) pQTextEdit->zoomTo(zoom);
      //if (QT_style_To_QFont(style , font, "Arial","10")) pQTextEdit->setFont(font);

      pQTextEdit->copy();
      dstEdit->paste();
      //qDebug(pQTextEdit->text());

      //(QApplication::clipboard())->setText (old_clip, QClipboard::Clipboard );
      delete pQTextEdit;
     }
}

//---------------------------------- getTextImageList ---------------------------------------------------------------------------
long CHtmlTools::getTextImageList( const QString &txt, QStringList &imgList, long from /* = 0 */)
{//<img align=BOTTOM border=0 height=48 name=Image1 src=ccam_view_icn.png width=48 >

 QString fileName;
 long      i   = 0;
 long deb_tag;
 while (from>=0 && from<(long)txt.length())
    {deb_tag     = IsThisTagImage(txt, from , &fileName ); // en entrée nameFind = "" car on recherche toutes les ancres
     if (deb_tag>=0)
        {fileName.replace("&gt;",">");
	 fileName.replace("&lt;","<");
	 imgList.append(fileName);
	 ++i;
        }
    } //end while (from>=0 && from<txt.length())     essai2.html
 return i;
}
//------------------------------------------ IsThisTagImage ------------------------------------------------------
// ENTREE: QString &txt:   reference du texte où doit se faire la recherche
//         long from:      position où doit commencer la recherche
//          QString *name  SI DIFFERENT de zero:
//                            SI contient un valeur (nom d'une ancre) il sera recherché une ancre de cette valeur
//                            SINON il sera retourné le nom de la première ancre trouvée
//          const QString &tagName: attribut du tag "name" "href"
//          QString *text  SI DIFFERENT de zero: il sera retourné le texte de l'ancre
// SORTIE: -1 : pas de tag de type ancre correspondant aux caractéristiques d'entrée (eventuel nom de l'ancre) trouvé
//         -2 : erreur de syntaxe separateur '=' non trouvé dans cette ancre
//         -3 : erreur de syntaxe fin de tag '>' du debut d'ancre  non trouvée
//         -4 : erreur de syntaxe debut '<' de tag fermant </a> non trouvé
//         -5 : erreur de syntaxe fin 'a>' de tag fermant </a> non trouvée
//         SINON pointeur juste aprés le tag fermant de cette ancre.
//
// <img align=BOTTOM border=0 height=48 name=Image1 src=ccam_view_icn.png width=48 >
long CHtmlTools::IsThisTagImage(const QString &txt, long &from , QString *fileName /* = 0 */ , int *w /* = 0 */, int *h /* = 0 */)
{long deb_file;
 long deb_tag;
 long end_tag;
 //char *pt_deb;
 //char *pt_end;
 //........... rechercher debut du tag .........................
 from     = txt.find("<img " , from, FALSE);
 if (from==-1)        {from = txt.length();        return -1;}
 deb_tag  = from;
 //........... rechercher fin du tag ...........................
 from    += 5;
 end_tag  = txt.find('>', from);
 if (end_tag==-1)                                  return -3;                    // erreur  -3 de syntaxe fin de tag '>' non trouvée

 int pos;
 //........... rechercher src ..................................
 if (fileName && (pos = txt.find("src", from, FALSE)) != -1 && pos<end_tag)
    { //........... extraire nom du fichier image ...................
      pos += 3;
      while (pos<end_tag && (txt.at(from)==' ' || txt.at(from)=='\t' || txt.at(pos)=='='))  ++pos;
      if (txt.at(pos)=='\"')
         {++pos;
          deb_file = pos;
          while (pos<end_tag &&  txt.at(pos)!='\"' && txt.at(pos)!=';')     ++pos;
         }
      else
         { deb_file = pos;
           while (pos<end_tag && txt.at(pos)!=' ' && txt.at(pos)!='\t' && txt.at(pos)!='\"' && txt.at(pos)!=';')     ++pos;
         }
      *fileName = txt.mid(deb_file, pos - deb_file);
    }
 //........... rechercher width ..................................
 if (w && (pos = txt.find("width", from, FALSE)) != -1 && pos<end_tag)
    { //........... extraire nom du fichier image ...................
      pos += 5;
      while (pos<end_tag && (txt.at(from)==' ' || txt.at(from)=='\t'|| txt.at(pos)=='\"' || txt.at(pos)=='='))  ++pos;
      deb_file = pos;
      while (pos<end_tag && txt.at(pos)!=' ' && txt.at(pos)!='\t' && txt.at(pos)!='\"' && txt.at(pos)!=';')     ++pos;
      *w = txt.mid(deb_file, pos - deb_file).toInt();
    }
 //........... rechercher height ..................................
 if (h && (pos = txt.find("height", from, FALSE)) != -1 && pos<end_tag)
    { //........... extraire nom du fichier image ...................
      pos += 6;
      while (pos<end_tag && (txt.at(from)==' ' || txt.at(from)=='\t'|| txt.at(pos)=='\"' || txt.at(pos)=='='))  ++pos;
      deb_file = pos;
      while (pos<end_tag && txt.at(pos)!=' ' && txt.at(pos)!='\t' && txt.at(pos)!='\"' && txt.at(pos)!=';')     ++pos;
      *h = txt.mid(deb_file, pos - deb_file).toInt();
    }
 from = end_tag+1;
 return deb_tag;
}

//---------------------------------- getTextDefaultAttribut ---------------------------------------------------------------------------
QString CHtmlTools::getTextDefaultAttribut( const QString &txt, const QString &attribut)
{// <body  background="ODBC_Converter_Back.jpg"  TEXT="yellow" BGCOLOR="#000000" style="font-size:8pt;font-family:Sans" LINK="#FF0000" VLINK="#FF9900" ALINK="#FFFF00" >
 //
 if (txt.length()==0)    return 0;
 int pos = txt.find("<body", 0, FALSE);
 if (pos==-1)            return 0;
 int end = txt.find(">", pos+5);
 if (end==-1)            return 0;
 pos = txt.find(attribut, pos+5, FALSE);
 if (pos==-1 || pos>end) return 0;
 pos = txt.find("=", pos+attribut.length());
 if (pos==-1 || pos>end) return 0;
 ++pos;
 while (pos<end && (txt[pos]==' ' || txt[pos]=='\t'|| txt[pos]=='\"')) ++pos;
 if (txt[pos]=='>')      return 0;
 int deb = pos;
 while (pos<end && /*txt[pos]!=' ' &&*/ txt[pos]!='\t' && txt[pos]!='\"' && txt[pos]!='\r' && txt[pos]!='\n') ++pos;
 return txt.mid(deb,pos-deb);
}

//-------------------------------------- setTextDefaultAtribut --------------------------------------------------------------------------
int CHtmlTools::setTextDefaultAtribut(QString &txt, const QString &attribut, const QString &value)
{// <body  background="ODBC_Converter_Back.jpg"  TEXT="yellow" BGCOLOR="#000000" LINK="#FF0000" VLINK="#FF9900" ALINK="#FFFF00" >
 if (value.length()==0)       return 0;
 int deb = txt.find("<body", 0, FALSE);
 if (deb ==-1)                return 0;
 int end = txt.find(">", deb+5);
 if (end ==-1)                return 0;

 int pos_img = txt.find(attribut, deb+5, FALSE);
 if (pos_img != -1 && pos_img<=end)
    {int pos = txt.find("=", pos_img + attribut.length());
     if (pos==-1 || pos>end)  return 0;
     ++pos;
     while (pos<end && (txt[pos]==' ' || txt[pos]=='\t'|| txt[pos]=='\"')) ++pos;
     while (pos<end && txt[pos]!=' ' && txt[pos]!='\t' && txt[pos]!='\"' && txt[pos]!='\r' && txt[pos]!='\n') ++pos;
     if (txt[pos]=='\"') ++pos;
     txt.remove(pos_img, pos-pos_img);
    }
 else
    {pos_img = deb + 5;
    }
 QString toInsert (attribut + "=\"" + value + "\"");
 if (txt[pos_img] != ' ') toInsert.prepend(' ');
 else                     ++pos_img;
 if (txt[pos_img] != ' ') toInsert.append(' ');
 txt.insert(pos_img, toInsert);
 return pos_img;
}

//-------------------------------------------- ListerLiens -----------------------------------------------------------
// ENTREE: QString &txt:      reference du texte où doit se faire la recherche
//         QListView *pQListView: pointe sur la QListView à initialiser
//         QString name ="":  SI VIDE (par defaut) alors il ne sera listé qu'un seul exemplaire de chaque ancre
//                            SINON  alors il ne sera listé qu'un seul exemplaire de chaque ancre, sauf de l'ancre spécifiée par ce nom,
//         long from:         position où doit commencer la recherche
//
void CHtmlTools::ListerLiens(const QString &txt, QListView *pQListView, long from /*=0*/)
{QString nameFind;
 QString text;
 //char *pt_deb;
 int nb_col   = pQListView->columns();
 int      i   = 0;
 long deb_tag;
 while (from>=0 && from<(long)txt.length())
    {nameFind = "";
     deb_tag     = IsThisTagLien(txt, from , &nameFind , &text); // en entrée nameFind = "" car on recherche toutes les ancres
     //pt_deb   = from + (char*)(const char *)txt;
     if (deb_tag>=0)
        {nameFind.replace("&gt;",">");
	 nameFind.replace("&lt;","<");
	 if (nb_col==1) new QListViewItem(pQListView, nameFind);
	 if (nb_col==2) new QListViewItem(pQListView, nameFind, text);
	 if (nb_col==3) new QListViewItem(pQListView, nameFind, text, QString::number(i));
	 i++;
        }
    } //end while (from>=0 && from<txt.length())     essai2.html
}

//-------------------------------------------- EffacerLien -----------------------------------------------------------
// ENTREE: QString &txt:      reference du texte où doit se faire l'épuration
//         int posLink:       numero du lien dans la page
//         long from:         position où doit commencer l'épuration
//
void CHtmlTools::EffacerLien(QString &txt, int posLink,  long from /*=0*/)
{long deb_tag;  //
 int i = 0;
 QString libelle;
 while (from>=0 && from<(long)txt.length())
    {deb_tag  = IsThisTagLien(txt, from ,0, &libelle);
     if (deb_tag>=0)
        {if (i==posLink)
            {txt.remove(deb_tag, from-deb_tag);	 // effacer tout le tag
	     txt.insert(deb_tag, libelle);       // conserver le libelle
	     from = deb_tag;
	     return;
            }
	 i++;
        }
    } //end while (from>=0 && from<txt.length())
}
//-------------------------------------------- ListerAncres -----------------------------------------------------------
// ENTREE: QString &txt:      reference du texte où doit se faire la recherche
//         QStringList &qstringList: pointe sur la QStringList à initialiser
//         QString name ="":  SI VIDE (par defaut) alors il ne sera listé qu'un seul exemplaire de chaque ancre
//                            SINON  alors il ne sera listé qu'un seul exemplaire de chaque ancre, sauf de l'ancre spécifiée par ce nom,
//         long from:         position où doit commencer la recherche
//
void CHtmlTools::ListerAncres(const QString &txt, QStringList &qstringList, QString name /*=""*/, long from /*=0*/)
{QString nameFind;
 QString text;
 long deb_tag;
 while (from>=0 && from<(long)txt.length())
    {nameFind = "";
     deb_tag     = IsThisTagAnchor(txt, from , &nameFind , &text); // en entrée nameFind = "" car on recherche toutes les ancres
     if (deb_tag>=0 && nameFind!=name)
        {name = nameFind;
	 nameFind.replace("&gt;",">");
	 nameFind.replace("&lt;","<");
	 qstringList.append(nameFind);
        }
    } //end while (from>=0 && from<txt.length())     essai2.html
}
//-------------------------------------------- ListerAncres -----------------------------------------------------------
// ENTREE: QString &txt:      reference du texte où doit se faire la recherche
//         QListView *pQListView: pointe sur la QListView à initialiser
//         QString name ="":  SI VIDE (par defaut) alors il ne sera listé qu'un seul exemplaire de chaque ancre
//                            SINON  alors il ne sera listé qu'un seul exemplaire de chaque ancre, sauf de l'ancre spécifiée par ce nom,
//         long from:         position où doit commencer la recherche
//
void CHtmlTools::ListerAncres(const QString &txt, QListViewItem *pQListViewItem, QString name /*=""*/, long from /*=0*/, QStringList *pQStringList /*=0*/)
{QString nameFind;
 QString text;
 int nb_col   = pQListViewItem->listView()->columns();
 long deb_tag;
 while (from>=0 && from<(long)txt.length())
    {nameFind = "";
     deb_tag     = IsThisTagAnchor(txt, from , &nameFind , &text); // en entrée nameFind = "" car on recherche toutes les ancres
     if (deb_tag>=0 && nameFind!=name)
        {name = nameFind;
	 nameFind.replace("&gt;",">");
	 nameFind.replace("&lt;","<");
	 if (nb_col==1) new QListViewItem(pQListViewItem, nameFind);
	 if (nb_col==2) new QListViewItem(pQListViewItem, nameFind, text);
         if (pQStringList) pQStringList->append(nameFind);
        }
    } //end while (from>=0 && from<txt.length())     essai2.html
}
//-------------------------------------------- ListerAncres -----------------------------------------------------------
// ENTREE: QString &txt:      reference du texte où doit se faire la recherche
//         QListView *pQListView: pointe sur la QListView à initialiser
//         QString name ="":  SI VIDE (par defaut) alors il ne sera listé qu'un seul exemplaire de chaque ancre
//                            SINON  alors il ne sera listé qu'un seul exemplaire de chaque ancre, sauf de l'ancre spécifiée par ce nom,
//         long from:         position où doit commencer la recherche
//
void CHtmlTools::ListerAncres(const QString &txt, QListView *pQListView, QString name /*=""*/, long from /*=0*/, QStringList *pQStringList/*=0*/)
{QString nameFind;
 QString text;
 int nb_col   = pQListView->columns();
 long deb_tag;
 while (from>=0 && from<(long)txt.length())
    {nameFind = "";
     deb_tag     = IsThisTagAnchor(txt, from , &nameFind , &text); // en entrée nameFind = "" car on recherche toutes les ancres
     if (deb_tag>=0 && nameFind!=name)
        {name = nameFind;
	 nameFind.replace("&gt;",">");
	 nameFind.replace("&lt;","<");
	 if (nb_col==1) new QListViewItem(pQListView, nameFind);
	 if (nb_col==2) new QListViewItem(pQListView, nameFind, text);
         if (pQStringList) pQStringList->append(nameFind);
        }
    } //end while (from>=0 && from<txt.length())     essai2.html
}

//-------------------------------------------- ElimineAncresRedondantes -----------------------------------------------------------
// ENTREE: QString &txt:      reference du texte où doit se faire l'épuration
//         QString name ="":  SI VIDE (par defaut) alors il sera conservé un seul exemplaire de chaque ancre
//                            SINON  alors il sera conservé AUCUN exemplaire de l'ancre spécifiée par ce nom
//         long from:         position où doit commencer l'épuration
//
void CHtmlTools::ElimineAncresRedondantes(QString &txt, QString name /*=""*/, long from /*=0*/)
{long deb_tag;
 QString nameFind;
 while (from>=0 && from<(long)txt.length())
    {nameFind = "";
     deb_tag  = IsThisTagAnchor(txt, from , &nameFind ); // en entrée nameFind = "" car on recherche toutes les ancres
     if (deb_tag>=0)
        {if (nameFind==name)
            {txt.remove(deb_tag, from-deb_tag);
	     from = deb_tag;
            }
	 else
	    {name = nameFind;
	    }
        }
    } //end while (from>=0 && from<txt.length())
}

//-------------------------------------------- EffacerAncre -----------------------------------------------------------
// ENTREE: QString &txt:      reference du texte où doit se faire l'épuration
//         QString name ="":  SI VIDE (par defaut) alors il sera conservé un seul exemplaire de chaque ancre
//                            SINON  alors il sera conservé AUCUN exemplaire de l'ancre spécifiée par ce nom
//         long from:         position où doit commencer l'épuration
//
void CHtmlTools::EffacerAncre(QString &txt, QString name , long from /*=0*/)
{long deb_tag;  //
 QString nameFind;
 while (from>=0 && from<(long)txt.length())
    {nameFind = "";
     deb_tag  = IsThisTagAnchor(txt, from , &nameFind ); // en entrée nameFind = "" car on recherche toutes les ancres
     if (deb_tag>=0)
        {if (nameFind==name)
            {txt.remove(deb_tag, from-deb_tag);
	     from = deb_tag;
            }
        }
    } //end while (from>=0 && from<txt.length())
}

//------------------------------------------ IsThisTagAnchor ------------------------------------------------------
// ENTREE: QString &txt:   reference du texte où doit se faire la recherche
//         long from:      position où doit commencer la recherche
//          QString *name  SI DIFFERENT de zero:
//                            SI contient un valeur (nom d'une ancre) il sera recherché une ancre de cette valeur
//                            SINON il sera retourné le nom de la première ancre trouvée
//          QString *text  SI DIFFERENT de zero: il sera retourné le texte de l'ancre
// SORTIE: -1 : pas de tag de type ancre correspondant aux caractéristiques d'entrée (eventuel nom de l'ancre) trouvé
//         -2 : erreur de syntaxe separateur '=' non trouvé dans cette ancre
//         -3 : erreur de syntaxe fin de tag '>' du debut d'ancre  non trouvée
//         -4 : erreur de syntaxe debut '<' de tag fermant </a> non trouvé
//         -5 : erreur de syntaxe fin 'a>' de tag fermant </a> non trouvée
//         SINON pointeur juste aprés le tag fermant de cette ancre.
//
// <a name="Ancre">trucmachin</a>
long CHtmlTools::IsThisTagAnchor(const QString &txt, long &from , QString *name /* = 0 */ , QString *text  /* = 0 */)
{return IsThisTagExist(txt, "name", from, name ,text );
}

//------------------------------------------ IsThisTagLien ------------------------------------------------------
// ENTREE: QString &txt:   reference du texte où doit se faire la recherche
//         long from:      position où doit commencer la recherche
//          QString *name  SI DIFFERENT de zero:
//                            SI contient un valeur (nom d'une ancre) il sera recherché une ancre de cette valeur
//                            SINON il sera retourné le nom de la première ancre trouvée
//          QString *text  SI DIFFERENT de zero: il sera retourné le texte de l'ancre
// SORTIE: -1 : pas de tag de type ancre correspondant aux caractéristiques d'entrée (eventuel nom de l'ancre) trouvé
//         -2 : erreur de syntaxe separateur '=' non trouvé dans cette ancre
//         -3 : erreur de syntaxe fin de tag '>' du debut d'ancre  non trouvée
//         -4 : erreur de syntaxe debut '<' de tag fermant </a> non trouvé
//         -5 : erreur de syntaxe fin 'a>' de tag fermant </a> non trouvée
//         SINON pointeur juste aprés le tag fermant de cette ancre.
//
// <a name="Ancre">trucmachin</a>
long CHtmlTools::IsThisTagLien(const QString &txt, long &from , QString *name /* = 0 */ , QString *text  /* = 0 */)
{ return IsThisTagExist(txt, "href", from, name ,text );
}

//------------------------------------------ IsThisTagExist ------------------------------------------------------
// ENTREE: QString &txt:   reference du texte où doit se faire la recherche
//         long from:      position où doit commencer la recherche
//          QString *name  SI DIFFERENT de zero:
//                            SI contient un valeur (nom d'une ancre) il sera recherché une ancre de cette valeur
//                            SINON il sera retourné le nom de la première ancre trouvée
//          const QString &tagName: attribut du tag "name" "href"
//          QString *text  SI DIFFERENT de zero: il sera retourné le texte de l'ancre
// SORTIE: -1 : pas de tag de type ancre correspondant aux caractéristiques d'entrée (eventuel nom de l'ancre) trouvé
//         -2 : erreur de syntaxe separateur '=' non trouvé dans cette ancre
//         -3 : erreur de syntaxe fin de tag '>' du debut d'ancre  non trouvée
//         -4 : erreur de syntaxe debut '<' de tag fermant </a> non trouvé
//         -5 : erreur de syntaxe fin 'a>' de tag fermant </a> non trouvée
//         SINON pointeur juste aprés le tag fermant de cette ancre.
//
// <a name="Ancre">trucmachin</a>
long CHtmlTools::IsThisTagExist(const QString &txt, const QString &tagName, long &from , QString *name /* = 0 */ , QString *text  /* = 0 */)
{long deb_name;
 long deb_tag;
 long end_tag;
 //char *pt_deb;
 //char *pt_end;
 //........... rechercher debut du tag ...................
 QString rech("<a ");
 rech   += tagName + "=";
 from    = txt.find(rech , from, FALSE);
 if (from==-1)                                     return -1;

 //........... rechercher fin du tag ...................
 deb_tag  = from;
 from    += rech.length()+1;
 deb_name = from ;
 end_tag  = txt.find('>', from);
 if (end_tag==-1)                                  return -3;                    // erreur  -3 de syntaxe fin de tag '>' non trouvée

 //........... rechercher > ...................
 if (name !=0)
    {QString findName ( txt.mid(deb_name, end_tag - deb_name).remove('\"').stripWhiteSpace() );
     if (name->length()>0 && *name != findName)    return -1;            // c'est pas l'ancre du nom recherché
     *name = findName;
    }
 //........... rechercher < de </a> ...................
 deb_name = end_tag+1;
 //pt_deb   = deb_name + (char*)(const char *)txt;
 from     = deb_name;

 //........... rechercher a> de </a> ...................
 from = txt.find("a>", from+1, FALSE);
 if (from==-1)                                     return -5;                      // erreur  -5 de syntaxe fin 'a>' de tag fermant </a> non trouvée

 long end_txt  = txt.findRev("</", from);
 //pt_end   = from + (char*)(const char *)txt;
 if (end_txt==-1)                                  return -4;                      // erreur  -4 de syntaxe debut '<' de tag fermant </a> non trouvé
 if (text !=0)
    {*text = txt.mid(deb_name, end_txt - deb_name);
     HtmlToAscii(*text);
    }

 from += 2;
 return deb_tag;
}

/*
{long deb_name;
 long deb_tag;
 long end_tag;
 //char *pt_deb;
 //char *pt_end;
 //........... rechercher debut du tag ...................
 from    = txt.find("<a", from, FALSE);
 if (from==-1)                                     return -1;
 deb_tag  = from;
 //........... rechercher fin du tag ...................
 from += 2;
 end_tag  = txt.find('>', from);
 if (end_tag==-1)                                  return -3;                    // erreur  -3 de syntaxe fin de tag '>' non trouvée
 //........... rechercher debut name ...................
 from = txt.find(tagName, from, FALSE);          // cas du href
 if (from==-1 || from>end_tag)
    {from = end_tag+1;                             return -1;
    }
 //........... rechercher = ...................
 from  = txt.find('=', from+4);
 if (from==-1 || from>end_tag)
     {from = end_tag+1;                            return -2;                       // erreur  -2  de syntaxe separateur '=' non trouvé
     }
 deb_name =  from + 1;
 //........... rechercher > ...................
 if (name !=0)
    {QString findName ( txt.mid(deb_name, end_tag - deb_name).remove('\"').stripWhiteSpace() );
     if (name->length()>0 && *name != findName)    return -1;            // c'est pas l'ancre du nom recherché
     *name = findName;
    }
 //........... rechercher < de </a> ...................
 deb_name = end_tag+1;
 //pt_deb   = deb_name + (char*)(const char *)txt;
 from     = deb_name;

 //........... rechercher a> de </a> ...................
 from = txt.find("a>", from+1, FALSE);
 if (from==-1)                                     return -5;                      // erreur  -5 de syntaxe fin 'a>' de tag fermant </a> non trouvée

 long end_txt  = txt.findRev("</", from);
 //pt_end   = from + (char*)(const char *)txt;
 if (end_txt==-1)                                  return -4;                      // erreur  -4 de syntaxe debut '<' de tag fermant </a> non trouvé
 if (text !=0)
    {*text = txt.mid(deb_name, end_txt - deb_name);
     HtmlToAscii(*text);
    }

 from += 2;
 return deb_tag;
}
*/

//------------------------------------------------------------- IsRtf --------------------------------------------------------
int CHtmlTools::IsRtf (const QString& str)
{QString findIn = str.left(10);
 if ( findIn.find("{\\rtf1") != -1) return 1;
 return 0;
}
//------------------------------------------------------------- IsRtf --------------------------------------------------------
int CHtmlTools::IsRtf (const char* txt, int limitFindTo /* = 10 */)
{
 if (txt==0)     return 0;
 char *pt     = (char*) txt;
 char *end    = pt + limitFindTo;
 for (; pt<end; ++pt)
      {if (pt[0]=='{' && pt[1]=='\\' && pt[2]=='r' && pt[3]=='t'  && pt[3]=='f'  && pt[4]=='1') return 1;
      }
 return 0;
}
//-------------------------------------------------- HtmlToAscii -----------------------------------------------------------------
QString CHtmlTools::HtmlToAscii(QString &str, int mode /* = 0 */)
{char bef  = 0;
 char car  = 0;
//str = str.ascii();
if (mode & CHtmlTools::KeepP)
   { str.replace("</p>","\r\n");
     str.replace("<p>","\r\n");
   }
else
   { str.replace("</p>","\n\n");
     str.replace("<p>","\n\n");
   }
 str.replace("<br>","\n");
 str.replace("<br />","\n");
 str.replace("&nbsp;"," ");
 str.replace("<br />","\n");
 str.replace("<li>","\t");
 str.replace("</li>","\n");
 str.replace("</ul>","\n");
 str.replace("</div>","\n");
 str.replace("</div>","\n");
 str.replace("&quot;","\"");
 str.replace("&amp;","&");

 if (mode&CHtmlTools::StrictASCII || mode&CHtmlTools::commaProtect)
    {
     QString result("");
     int pos = 0;
     while (pos < (int)str.length())
      {car  = str[pos].latin1();
       if (pos>1) bef = str[pos-1].latin1();
       if (str[pos]=='<')
          {pos =  str.find('>', pos+1);
           if (pos==-1) return QString (TR ("CHtmlTools::HtmlToAscii() Erreur de syntaxe tag html non fermé") );
           ++pos;
          }
       else
          {if ( (mode&CHtmlTools::commaProtect) &&  car==',')
	          {if ( pos>1  && str[pos-1] != '\\')
                  { result += "\\,";
                    ++pos;
	              }
               else
                  { result += ',';
                  }
               ++pos;
	          }
	       else if (car ==-96)
	          {result += ' ';
               ++pos;
	          }
          else
              {result += car;
               ++pos;
              }
          }
      }
     result.replace("&lt;","<");
     result.replace("&gt;",">");
     if (mode & CHtmlTools::KeepP)  result.replace("\r\n","<p>");
     if (mode & CHtmlTools::KeepBR) result.replace("\n","<br>");
     str = result;
     return str;
    }
 int posDeb=0;
 int posEnd=0;
 do {posDeb = str.find("<",posDeb);
     if (posDeb !=-1)
        {posEnd =  str.find(">", posDeb+1);
         if (posEnd != -1) str.remove(posDeb, posEnd-posDeb+1);
         else return QString (TR ("CHtmlTools::HtmlToAscii() Erreur de syntaxe tag html non fermé") );
        }
     else break;
    } while (1);
 str.replace("&lt;","<");
 str.replace("&gt;",">");
 if (mode & CHtmlTools::KeepP)  str.replace("\r\n","<p>");
 if (mode & CHtmlTools::KeepBR) str.replace("\n","<br>");
 return    str;
}

//-------------------------------------------------- QRichTextToHtml -----------------------------------------------------------------
void CHtmlTools::QRichTextToHtml(QString &str)
{int pos = 0;
 while (pos < (int)str.length())
 {if (str[pos]=='<')
     {pos =  str.find('>', pos+1);
      if (pos==-1) return;
      ++pos;
     }
  else
     {//if (str[pos]==' ')
      //{str.replace(pos, 1, "&nbsp;" );  //0x3f = ?
      //pos += 6;
      //}
      //if (str[pos]=='\t')
      //{str.replace(pos+1, 1, "&nbsp;&nbsp;&nbsp;&nbsp;" );
      //pos += 6;
      //}
      //else
      //{++pos;
      //}
      ++pos;
     }
 }
}
//----------------------------------------------------------------- ifNeed_Convert_RtftoHtml ---------------------------------------
QString  CHtmlTools::ifNeed_Convert_RtftoHtml( QString &strData )
{if (! CHtmlTools::IsRtf(strData)) return strData;
 QString   txt;
 CHtmlTools::RtfToQtextEdit_HTML(strData, strData.length(), txt, 1);
 strData = txt;
 return strData;
}

//----------------------------------------------------------------- RtfToQtextEdit_HTML ---------------------------------------
void CHtmlTools::RtfToQtextEdit_HTML(const char *txt, long len_rtf, QString &stringDST, int with_header /*= 1 */)
{QString qsVal        = "";
 char *pt             = (char*) txt;
 char *end_rtf        = 0;

 if (len_rtf<=0) len_rtf = strlen(txt);
 end_rtf = pt+len_rtf-1;


 //............... variables pour gerer effets texte ..............
 int souligne = 0;
 int gras     = 0;
 int ital     = 0;
 int justif   = 0;
 //........ Header qui va bien ....................................
 // pour retours chariots normaux il faut inserer le tag <qt> apres <html>
 // pour que \t soit active comme tabulations vraies il faut que le header
 //             suivant soit comme celui-ci
 /*
 stringDST  = "<html><qt><head><meta name=\"qrichtext\" content=\"1\" /></head>"
              "<body style=\"font-size:10pt;font-family:Lucida\">"
              "<font color=#000000>";
*/
 //#include "Ez_edit.xpm"


    // To be able to use this image within some rich text, for example inside a QLabel, you must create a QImage from the raw data and insert it into the factory with a unique name:
    // QMimeSourceFactory::defaultFactory()->setImage( "myimage", QImage(Ez_edit_xpm) );
    // 1 cm   = 567 twips
    // 1 twip = 20 points imprimante
    //QPixmap( green_icon )
    // Now you can create a rich text QLabel with
    // QLabel* label = new QLabel(
    //    "Rich text with embedded image:<img source=\"myimage\">"
    //    "Isn't that <em>cute</em>?" );
    // When no longer needed, you can clear the data from the factory:
    // delete label;

 if (   with_header ==1 )
    {stringDST  = "<html><qt><head><meta name=\"qrichtext\" content=\"1\" /></head>\n<body style=\"font-size:10pt;font-family:Sans\">"
		  //"<p><font color=#000000 face=\"Arial Narrow\"><img source=\"myimage\">";
                  "\n<p>"; //"<font color=#000000 face=\"Arial Narrow\"><img source=\"myimage\">";  // body style="font-size:10pt;font-family:Sans"
    }

 stringDST += DEB_PAR;

 /*........................................ gestion des couleurs .......................................*/
 long nb_color          = 0;
  COLOR_DEF * rgbTab    = 0;
 /*........................................ aller à la premiere acolade ................................*/
 while(pt<end_rtf && strncmp(pt, "{\\rtf1", 6) !=0) ++pt;
 ++pt;
/*......................................... scan et analyse du texte ..................................*/
 while(pt<end_rtf)
   {int car = *pt;
    switch(car)
       {case '\r':
        case '\n':
        case '}':
        case '{':
             ++pt;
             break;
        case '\f':                 // gloups avaler le bug rtf des ordos de promethée
             while(pt<end_rtf && *pt!=';') ++pt;
             ++pt;
             break;
        case  '\\':                // anti Slash
             if (pt[1]=='o' && (strncmp(pt+1,"object",6)==0)) //.......... object ......................
                {pt = JumpObject(pt, end_rtf);
                }
	     else if (pt[1]=='{' || pt[1]=='}')
	        {stringDST += pt[1];
		 pt += 2;
		}
             else if (pt[1]=='\\')
                {stringDST += pt[0];
                 ++pt;
                }
             else if (strncmp(pt+1,"colortbl",8)==0)  //................... table couleurs .............
                {nb_color       = RTFGetNbColor(pt+9, end_rtf);
                 rgbTab         = new  COLOR_DEF[nb_color];
                 if (rgbTab) pt = RTF_to_RGBTab(pt+9, end_rtf, rgbTab, nb_color);
                }
             else if (rgbTab && pt[1] =='c' && pt[2] =='f')  //........... '\cf1' couleur police .......
                {QString string_index;
                 int col_index = 0;
                 pt           += 3;
                 while(pt<end_rtf && *pt >= '0'&& *pt <= '9')
                   {string_index += *pt;
                    ++pt;
                   }
		 col_index    = atoi(string_index);
		 col_index    = Min(nb_color -1, Max(0, atoi(string_index)));
                 stringDST    = stringDST + "\n</font>";
                 stringDST    = stringDST + "\n<font color=#" + rgbTab[col_index].html_color +" face=\"Arial Narrow\">";
		 if (*pt==' ') ++pt;
                }
             else if (strncmp(pt+1,"plain",5)==0)  //.............. attributs normaux ..................
                {while(souligne>0)
                      {stringDST    +=   "</u>";
                       --souligne;
                      }
		 while(ital>0)
                      {stringDST    +=   "</i>";
                       --ital;
                      }
                 while(gras>0)
                      {stringDST    +=   "</b>";
                       --gras;
                      }
                 pt += 6;
		 if (*pt==' ') ++pt;
                }
             else if (strncmp(pt+1,"pard",4)==0)   // ............. paragraphe normal .....................
	        {if (justif)
		    {stringDST      +=   "\n</p>\n<p>";
		      --justif;
		    }
		 while(souligne>0)
                      {stringDST    +=   "</u>";
                       --souligne;
                      }
		 while(ital>0)
                      {stringDST    +=   "</i>";
                       --ital;
                      }
                 while(gras>0)
                      {stringDST    +=   "</b>";
                       --gras;
                      }
                 pt += 5;
		 if (*pt==' ') ++pt;
	        }
             else if (strncmp(pt+1,"qc",2)==0 &&   // ............. centré .....................
		     (pt[3]==' '||pt[3]=='\\')
		     )
                {stringDST  +=   "\n</p>\n<p align=\"center\">";
                 pt += 3;
                 ++justif;
		 if (*pt==' ') ++pt;
                }
	     /*
	     else if (strncmp(pt+1,"ql",2)==0 &&   // ............. left .......................
		     (pt[3]==' '||pt[3]=='\\')
		     )
                {stringDST  +=   "\n</p>\n<p align=\"left\">";
                 pt += 3;
                 ++justif;
		 if (*pt==' ') ++pt;
                }
	     */
	     else if (strncmp(pt+1,"qr",2)==0 &&   // ............. right .....................
		     (pt[3]==' '||pt[3]=='\\')
		     )
                {stringDST  +=   "\n</p>\n<p align=\"right\">";
                 pt += 3;
                 ++justif;
		 if (*pt==' ') ++pt;
                }
             else if (strncmp(pt+1,"i0",2)==0)     // ............. Italique .....................
                {if (ital>0)
		     {if (gras)     stringDST  += "</b>";
		      if (souligne) stringDST  += "</u>";
		      stringDST  +=   "</i>";
		      if (gras)     stringDST  += "<b>";
		      if (souligne) stringDST  += "<u>";
		      --ital;
		     }
		 pt += 3;
		 if (*pt==' ') ++pt;
                }
             else if (pt[1]=='i'&& (pt[2]==' '||pt[2]=='\\'))
                {stringDST  +=   "<i>";
		 pt += 2;
                 ++ital;
		 if (*pt==' ') ++pt;
                }
             else if (strncmp(pt+1,"b0",2)==0)           // ............. Bold .....................
                {if (gras>0)
		    {stringDST  +=   "</b>";
		     --gras;
		    }
		 pt += 3;
		 if (*pt==' ') ++pt;
                }
             else if (pt[1]=='b'&& (pt[2]==' '||pt[2]=='\\'))
                {stringDST    +=    "<b>";
                 pt += 2;
                 ++gras;
		 if (*pt==' ') ++pt;
                }
             else if (strncmp(pt+1,"ulnone",6)==0||    // ............. souligné .....................
		      strncmp(pt+1,"ul0",3)==0
	             )
                {if (souligne>0)
		    {stringDST    +=   "</u>";
		     --souligne;
		    }
                 pt += 7;
                 if (*pt==' ') ++pt;
                }
             else if (strncmp(pt+1,"ul",2)==0)
                {stringDST    +=   "<u>";
                 pt += 3;
                 ++souligne;
                 if (*pt==' ') ++pt;
                }
	     else if (pt[1]== '*')
	        {pt = JumpAcolades(pt + 1, end_rtf);
	        }
	     else if (strncmp(pt+1,"info", 4)==0)
	        {pt = JumpAcolades(pt + 4, end_rtf);
	        }
             else if (strncmp(pt+1,"stylesheet",10)==0)
	        {pt = JumpAcolades(pt + 10, end_rtf);
	        }
             else if (strncmp(pt+1,"fnil",4)==0          ||   //........... commandes terminées par ';' ......
                      strncmp(pt+1,"froman",6)==0        ||
                      strncmp(pt+1,"fswiss",6)==0        ||
                      strncmp(pt+1,"fmodern",7)==0       ||
                      strncmp(pt+1,"fscript",7)==0       ||
                      strncmp(pt+1,"fdecor",6)==0        ||
                      strncmp(pt+1,"ftech",5)==0         ||
                      strncmp(pt+1,"snext",5)==0
                     )
                {while(pt<end_rtf && *pt != ';') ++pt;
                 ++pt;
                 if (*pt==' ') ++pt;
                }
             else if (strncmp(pt+1,"tab",3)==0           //............ tab:  tabulation.................
                     )
                {stringDST += "\t";
                 pt        += 4;
                 if (*pt==' ') ++pt;
                }
             else if (pt[4] !='d'&&                      //............. par:  fin de paragraphe ........
                      strncmp(pt+1,"par",3)==0           // faut pas que ce soit 'pard' mais 'par' isolé
                     )
                {if (souligne) stringDST  += "</u>";
		 stringDST += "\n"END_PAR ;
                 stringDST += "\n"DEB_PAR ;
		 if (souligne) stringDST  += "<u>";
                 pt        += 4;
                 if (*pt==' ') ++pt;
                }
	     else if (pt[1]=='u'&&                       //......... code decimal d'un caractère: ..................
		      pt[2]>='0'&&
		      pt[2]<='9'
		      )
	        {qsVal = "";
		 pt   += 2;
		 while (*pt && *pt>='0' && *pt<='9'){qsVal += pt[0]; ++pt;}
		 if (*pt==' ') ++pt;
		 stringDST += atoi(qsVal);
	        }
             else if ( pt[1]== '\'')                     //......... code hexadecimal d'un caractère: ..................
                {char val;                               //          précédé d'une apostrophe
                 if (pt[2]>='a'&&pt[2]<='f')        val  = (pt[2]-'W')<<4;  //a (97) - W (87) --> 10
                 else if (pt[2]>'A'&&pt[2]<='F')    val  = (pt[2]-'7')<<4;  //A (65) - 7 (55) --> 10
                 else                               val  = (pt[2]-'0')<<4;  //0 (48) - 0 (48) --> 0
                 if (pt[3]>='a'&&pt[3]<='f')        val += (pt[3]-'W');
                 else if (pt[2]>='A'&&pt[2]<='F')   val += (pt[3]-'7');
                 else                               val += (pt[3]-'0');
                 stringDST += val;
                 pt        += 4;
                }
             else                                        //......... commande rtf non supportée ....................
                {++pt;
                 while  (pt<end_rtf && IsRtfControlCar(pt)) pt++;
                 if (*pt==' ') ++pt;
                }
             break;
         default:
	     if      (pt[0]=='<')
	             {if (pt[1]=='<') {stringDST += "{{";  ++pt;}   // debut champ de fusion
		      else            {stringDST +="&lt;";}
		     }
	     else if (pt[0]=='>')
	             {if (pt[1]=='>') {stringDST += "}}";  ++pt;}   // fin   champ de fusion
		      else            {stringDST +="&gt;";}
		     }
             else stringDST += pt[0]; //......... caractère normal affichable ........
             ++pt;
       }   /* endswitch (car) */
   } /*endwhile(pt<end_rtf) */

 //........ on a plus besoin du tableau de couleurs .................
 if (rgbTab) delete[]rgbTab;
 if (   with_header ==1 )
    {stringDST     += END_PAR ;
     stringDST     += "</p></font></body></qt></html>";
    }
 // SaveString("/home/ro/Developpement/QLightPad/bin/result", stringDST);
 return;
}

/*------------------------------ JumpObject ----------------------------------------*/
char* CHtmlTools::JumpObject(char *pt, char *end_rtf)
{long acco=1;
 while(pt < end_rtf && acco)
   {int car = *pt;
    switch(car)
       {case '}':
             acco--;
             pt++;
             break;
        case '{':
             acco++;
             pt++;
             break;
        default:
             pt++;
       } /* endswitch(car) */
   } /*endwhile(pt < end_rtf && acco) */
 return pt;
}

/*------------------------------ IsRtfControlCar --------------------------------------*/
int CHtmlTools::IsRtfControlCar(char *pt)
{if       (*pt >='a' && *pt<= 'z') return 1;
 else if  (*pt >='A' && *pt<= 'Z') return 1;
 else if  (*pt >='0' && *pt<= '9') return 1;
 else if  (*pt =='-')              return 1;
 return 0;
}

/*--------------------------------- JumpAcolades -------------------------------------*/
char *CHtmlTools::JumpAcolades(char *pt, char *end_rtf)
{int nb_acoll = 1;
 while(pt<end_rtf && nb_acoll>0)
    {if (*pt=='}') --nb_acoll;
     if (*pt=='{') ++nb_acoll;
     ++pt;
    }
 return pt;
}

/*------------------------------ RTFGetNbColor ---------------------------------------*/
long CHtmlTools::RTFGetNbColor(char *pt, char *end_rtf)
{long  n = 0;
 while(pt<end_rtf&& *pt != '}')
   {if (*pt==';') ++n;
    ++pt;
   }
 return n;
}

/*------------------------------ RTF_to_RGBTab ---------------------------------------*/
char* CHtmlTools::RTF_to_RGBTab(char *pt, char *end_rtf,  COLOR_DEF* rgbTab, long nb_color)
{long    i = 0;
 unsigned char r=0;
 unsigned char g=0;
 unsigned char b=0;
 char coul[32];
 char buft[32];
 while (pt<end_rtf && i<nb_color)
  {if      (*pt==';')
           {//rgbTab[i] = RGB(r,g,b);
            rgbTab[i].r=r;
            rgbTab[i].g=g;
            rgbTab[i].b=b;

            memcpy(&rgbTab[i].html_color[0], RGB_to_HTML(r,buft),2);
            memcpy(&rgbTab[i].html_color[2], RGB_to_HTML(g,buft),2);
            memcpy(&rgbTab[i].html_color[4], RGB_to_HTML(b,buft),2);
            rgbTab[i].html_color[6]=0;
            ++i;
            ++pt;
           }
   else if (*pt=='\\')
      {++pt;
       if (strncmp(pt,"red",3)==0)
               {pt   += 3;
                int n = 0;
                while(pt<end_rtf && *pt != ';'&& *pt != '\\')
                     {coul[n] = *pt;
                      ++pt;
                      ++n;
                     }
                coul[n] = 0;
                r       = (unsigned char)(atoi(coul));

               }
       else if (strncmp(pt,"green",5)==0)
               {pt   += 5;
                int n = 0;
                while(pt<end_rtf && *pt != ';'&& *pt != '\\')
                     {coul[n] = *pt;
                      ++pt;
                      ++n;
                     }
                coul[n] = 0;
                g       = (unsigned char)(atoi(coul));

               }
       else if (strncmp(pt,"blue",4)==0)
               {pt   += 4;
                int n = 0;
                while(pt<end_rtf && *pt != ';'&& *pt != '\\')
                     {coul[n] = *pt;
                      ++pt;
                      ++n;
                     }
                coul[n] = 0;
                b       = (unsigned char)(atoi(coul));

               }
      else     {++pt;
               }
      }
   else
     {++pt;
     }
  }
 return pt;
}

/*------------------------------------------------------- RGB_to_HTML -------------------------------------*/

char *CHtmlTools::RGB_to_HTML(unsigned char rgb, char *buf)
{const char     *hexStr  = "0123456789ABCDEF";
 buf[0] = hexStr[(rgb>>4) & 0x0F];
 buf[1] = hexStr[ rgb     & 0x0F];
 return buf;
}

//---------------------------------- Copy_File --------------------------------------------------------------
// ENTREE: const QString &fileSrc: fichier source chemin+file
//         const QString &pathDst: chemin de destination sans separateur de fin.
//         QString *file_name : nom du fichier de destination (si zero il sera extrait de celui source)
QString CHtmlTools::Copy_File(const QString &fileSrc, const QString &pathDst, QString *file_name /*=0*/)
{
//................. choisir le fichier image src à insérer .................................................

  QString fileName = "";
  QFile* pfileSrc = new QFile( fileSrc );
  if (pfileSrc==0)                                                              return fileName;
  if (file_name) fileName  = *file_name;
  else           fileName  = QFileInfo( *pfileSrc ).fileName();
  QFile* pfileDst = new QFile( pathDst + "/" +  fileName);
  if (pfileDst==0)                       {delete pfileSrc;                      return fileName;}

  //.................... si le dossier src n'est pas egal au dossier dst .......................
  //                     alors copier l'image src dans le dossier destination
  //                     sinon l'image s'y trouve forcémént
  if ( ! (pathDst == QFileInfo( *pfileSrc ).dirPath ( TRUE ) && QFileInfo( *pfileDst ).fileName() == QFileInfo( *pfileSrc ).fileName()) )
     {char *buffer = new char[32000];
      Q_LONG        len_read;
      if (buffer==0)                         {delete pfileSrc; delete pfileDst; return fileName;}
      QString srcF = fileSrc;                     srcF.replace('\\','/'); srcF.replace("//","/");
      QString dstF = pathDst + "/" + fileName;    dstF.replace('\\','/'); dstF.replace("//","/");

      if (srcF == dstF )                     {delete pfileSrc; delete pfileDst; return fileName;}
      QFile::remove ( dstF );                               // eventuellement effacer un  du même nom

      if ( !pfileSrc->open( IO_ReadOnly ) )  
         {QString mess =   TR("Could not open file for reading: %1").arg( qApp->translate("QFile",pfileSrc->errorString()) );
          qDebug(mess);
          delete pfileSrc; 
          delete pfileDst; 
          return fileName;
         }
      if ( !pfileDst->open( IO_WriteOnly ) ) {delete pfileDst; pfileSrc->close(); delete pfileSrc; return fileName;}

      do{
         len_read = pfileSrc->readBlock (buffer, 32000 );
         if (len_read!=-1) pfileDst->writeBlock (buffer , len_read);
        } while (len_read == 32000);

      pfileSrc->close ();
      pfileDst->close ();
      delete[] buffer;
     }
 return fileName;
}

//----------------------------------------- Copy_Dir ---------------------------------------------
void CHtmlTools::Copy_Dir(QString src_Dir,  QString dest_Dir, int mode /*= CHtmlTools::allObjects*/, const QString &incFilter /*=""*/, const QString &excFilter /*=""*/)
{   QStringList incFiltersList  =  QStringList::split( ";", incFilter , false);
    QStringList excFiltersList  =  QStringList::split( ";", excFilter , false);
    int isOk = 0;
    //........................ explorer la source .......................................
    QDir cur_dir( src_Dir );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );

    //........................ creer le repertoire de destination si n'existe pas  .......................................
    if ( ! QFile::exists( dest_Dir  ) )
       {cur_dir.mkdir(dest_Dir);
       }
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {
        QString fname = fi->fileName();
        QString   ext = fi->extension();
        if      (fname == "..")
                {
                }
        else if ((mode&CHtmlTools::copyDir) && fi->isDir() && fname != "." )
                {// Copy_Dir( src_Dir + "/" + fname, dest_Dir + "/" + fname, mode, filters);
                  if (incFilter.length()||excFilter.length())
                     {isOk = 1;
                      if (incFilter.length() && incFiltersList.findIndex (ext) != -1) isOk=1;
                      if (excFilter.length() && excFiltersList.findIndex (ext) != -1) isOk=0;
                      if ( isOk )
                         {CHtmlTools::Copy_Dir( src_Dir + "/" + fname, dest_Dir + "/" + fname);
                         }
                     }
                  else
                     {   CHtmlTools::Copy_Dir( src_Dir + "/" + fname, dest_Dir + "/" + fname);
                     }
                }
        else if ( (mode&CHtmlTools::copyFiles) && fi->isFile())
                {
                  if (incFilter.length()||excFilter.length())
                     {isOk = 1;
                      if (incFilter.length() && incFiltersList.findIndex (ext) != -1) isOk=1;
                      if (excFilter.length() && excFiltersList.findIndex (ext) != -1) isOk=0;
                      if ( isOk )
                         {CHtmlTools::Copy_File(src_Dir + "/" + fname, dest_Dir);
                         }
                     }
                  else
                     {    CHtmlTools::Copy_File(src_Dir + "/" + fname, dest_Dir);
                     }
                }
        ++it;
    }
}

//------------------------ HtmlFind ------------------------------------------
/*! \brief RETOUR : pointeur dans le texte après le tag de fin du motif trouvé.  ENTREE : char **debMotif pointeur de pointeur qui si est donné est positionné sur.
 *  {{[GET_TEXT]?//             Rubrique=Observation?//             Debut apres = Certificat                     :  u.?//             Fin Avant = .t?//          }}
*/
char* CHtmlTools::HtmlFind(const char* pt_in, QString motif, char **debMotif /*=0*/)
{if (pt_in==0) return 0;
 if (debMotif) *debMotif = 0;
 //.............. si il faut juste chercher la fin de la ligne ..................................
 char* pt = (char*) pt_in;
 if (motif=="FinDeLigne"||motif=="EOL")
    {while(*pt)
           {if (pt[0]=='<' )
               {if (pt[1]=='b'&& pt[2]=='r')
                   {int len = 0;
                    if (pt[3]=='>')                                 len = 4;  // variante
                    if (pt[3]==' '&& pt[4]=='/' && pt[5]=='>')      len = 6;  // variante
                    if (len)
                       {if (debMotif) *debMotif = pt;
                        return pt+len;
                       }
                   }
                else if (pt[1]=='/' && pt[2]=='p')
                   {int len = 0;
                    if (pt[3]=='>')                                 len = 3;  // variante
                    if (len)
                       {if (debMotif) *debMotif = pt;
                        return pt+len;
                       }
                   }
               }
            ++pt;
           }
     return 0;
    }

 //.............. nettoyer le motif à comparer de tous les trucs non signifiant .................
 CHtmlTools::HtmlToAscii(motif);
 motif.remove((char)0xA0);
 motif.remove(' ');
 motif.remove('\t');
 motif.remove('\r');
 motif.remove('\n');
 int   carToCompareInMotif = 0;
 int   len_motif           =  motif.length();
 while (*pt)
{if (*pt=='<')
        {int len = 0;
     if (pt[0]=='<' && pt[1]=='b'&& pt[2]=='r')
            {if (pt[3]=='>')                                 len = 4;  // variante  <br>
             if (pt[3]==' '&& pt[4]=='/' && pt[5]=='>')      len = 6;  // variante  <br />
        }
         //.......... si tag  ou  la comparaison s'arrete car plus valide ....................
     if (len)
        {pt += len;
                 carToCompareInMotif=0;
                 if (debMotif) *debMotif = 0;
            }
     else
         //.............. le tag est à sauter ...........................................................
        {pt = IfHtmlTagJumpIt(pt);          // sauter tag html
        }
        }
  else
        {    char car = pt[0];
         if (car==' '||car=='\r'||car=='\n'||car=='\t'||car==(char)0xA0)                                      // sauter caractères non significatifs ||car=='\t'
            {++pt;
            }
         else //................... là on peut comparer ...............................................
            {if (car==(char)0xA0) {car = ' '; }
                 if (debMotif && carToCompareInMotif==0) *debMotif = pt;
                 if (pt[0]=='&')   //............. convertir les caractères codés .........................
                    {if      (pt[1]=='g'&&pt[2]=='t'&&pt[3]==';')                             {car = '>';  pt += 4;}
                     else if (pt[1]=='l'&&pt[2]=='t'&&pt[3]==';')                             {car = '<';  pt += 4;}
                     else if (pt[1]=='a'&&pt[2]=='m'&&pt[3]=='p'&&pt[4]==';')                 {car = '&';  pt += 5;}
                     else if (pt[1]=='n'&&pt[2]=='b'&&pt[3]=='s'&&pt[4]=='p'&& pt[5]==';')    {car = ' ';  pt += 6;}
                     else if (pt[1]=='q'&&pt[2]=='u'&&pt[3]=='o'&&pt[4]=='t'&& pt[5]==';')    {car = '\"'; pt += 6;}
                     else                                                                     {car = *pt;  pt += 1;}
                    }
                 else
                    {++pt;
                    }
             //car_t = motif[carToCompareInMotif].latin1();
                 if (car == motif[carToCompareInMotif])
                    {++carToCompareInMotif;
                     if (carToCompareInMotif >= len_motif) return pt;
                    }
                 else
                    {carToCompareInMotif=0;
                     if (debMotif) *debMotif = 0;
                    }
                }
        }
    }// end while (*pt && carToCompareInMotif < len_motif)
 return 0;
}

//--------------------------------- IfHtmlTagJumpIt ------------------------------------------
/*! \brief Saute le tag HTML. */
char* CHtmlTools::IfHtmlTagJumpIt(char* pt)
{if (pt[0]=='<')
    {++pt;
     while (*pt)
        {if (pt[0]=='>') return pt +1;
         ++pt;
        }
    }
 return pt;
}

//----------------------------------------- eraseDirectory ---------------------------------------------
void CHtmlTools::eraseDirectory( QString dir_to_erase)
{   //........................ petit message .......................................
    //G_pCApp->Datasemp_OutMessage(textEdit_Message, tr ("----------- Effacement de ce répertoire en cours : ") + dir_to_erase + "-----------");
    QDir cur_dir( dir_to_erase );
    cur_dir.setFilter( QDir::All | QDir::NoSymLinks );
    //............ exploration de la liste ......................
    const QFileInfoList * pQFileInfoList = cur_dir.entryInfoList();
    if (pQFileInfoList==0) return;
    QFileInfoListIterator it( *pQFileInfoList );
    QFileInfo            *fi;
    while ( (fi = it.current()) != 0 )
    {
        QString fname = fi->fileName().latin1();
        if      (fname == "..")
        {
        }
        else if (fi->isDir() && fname != "." )
        {eraseDirectory(dir_to_erase + "/" + fname);
        }
        else if (fi->isFile())
        {   //G_pCApp->Datasemp_OutMessage(textEdit_Message, QString (tr("Fichier en cours d'effacement : ")) + dir_to_erase + "/" + fname);
            cur_dir.remove(dir_to_erase + "/" + fname);
        }
        ++it;
    }
    cur_dir.rmdir(dir_to_erase );
}

//---------------------------------------- ddMMyyyy_to_yyyyMMdd ---------------------------------------
QString CHtmlTools::ddMMyyyy_to_yyyyMMdd(const QString & dateTime ,const QString& sep /* = "" */)
{ int i= 0, len = dateTime.length();
  QString str = "";
  for (i= 0; i<len; ++i)
      {if (dateTime[i]>='0' && dateTime[i]<='9') str += dateTime[i];
      }
  len = str.length();
  if (len==0) return str;
  if (len<8) str.insert(4,"20");    // 050610  -->05062010
  str = str.left(8);
  return str.mid(4,4)+sep+str.mid(2,2)+sep+str.mid(0,2);
}

//---------------------------------------- yyyyMMdd_to_ddMMyyyy ---------------------------------------
QString CHtmlTools::yyyyMMdd_to_ddMMyyyy(const QString & dateTime ,const QString& sep /* = "" */)
{ int i= 0, len = dateTime.length();
  QString str = "";
  for (i= 0; i<len; ++i)
      {if (dateTime[i]>='0' && dateTime[i]<='9') str += dateTime[i];
      }
  len = str.length();
  if (len==0) return str;
  if (len<8) str.prepend("20");    // 100506 -->20100506
  str  = str.left(8);              // "19921928"
  return str.mid(6,2) +sep + str.mid(4,2) + sep + str.mid(0,4);
}
