/************************* CMaskedLineEdit.cpp *****************************
 *   Copyright (C) 2004 by Sébastien SIVERA & Sevin Roland                 *
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

#include "CMaskedLineEdit.h"
#include <qapplication.h>
#include <qclipboard.h>
#include <qcursor.h>


// ---------------------------------------------- CMaskedLineEdit ---------------------------------------------------------------

CMaskedLineEdit::CMaskedLineEdit( QWidget *parent, const char *name )
    : QLineEdit( parent, name )
{
  m_ControlIsDown  = CMaskedLineEdit::NoCtrlKey;
  m_CarMask        = ' ';
  m_CursPos        = 0;
  m_Template       = "_";
  m_Text           = "";
  m_Valid          = "X";
  m_CursPos        = 0;
  m_NumKeyboardMap = tr("&é\"'(-è_çà");
  m_debMix         = 0;
  m_endMix         = 0;
  m_IsSel          = 0;
  m_MixMode        = 0;
}

// ---------------------------------------------- ~CMaskedLineEdit ---------------------------------------------------------------
CMaskedLineEdit::~CMaskedLineEdit()
{
}

///////////////////////////////////////////////////// PROPERTY ///////////////////////////////////////////////////////////////
//--------------------------------------------- setTemplate -------------------------------------------------------------------
void CMaskedLineEdit::setTemplate ( const QString &templ )
{m_Template = templ;
 //qDebug(QString("m_Template.length() : ")+QString::number(m_Template.length()));
}

//--------------------------------------------- setValid -------------------------------------------------------------------
void CMaskedLineEdit::setValid ( const QString &valid )
{m_Valid = valid;
 //qDebug(QString("m_Valid.length() : ")+QString::number(m_Valid.length()));
}

//--------------------------------------------- getTemplate -------------------------------------------------------------------
QString CMaskedLineEdit::getTemplate ( ) const
{return m_Template ;
}

//--------------------------------------------- getValid -------------------------------------------------------------------
QString  CMaskedLineEdit::getValid (  ) const
{return m_Valid;
}

//--------------------------------------------- text -------------------------------------------------------------------
QString CMaskedLineEdit::text () const
{return m_Text;
}
//--------------------------------------------- text -------------------------------------------------------------------
QString CMaskedLineEdit::getMixText () const
{return QLineEdit::text();
}
//--------------------------------------------- setText -------------------------------------------------------------------
void CMaskedLineEdit::setText ( const QString & text)
{QString result;
 m_Text      = text;
 int mixCurs = cursorPosition();
 DoMixedString(result);
 setMixText(result);
 m_CursPos   = CursPosInMixedString_To_PosInText(mixCurs);
 QLineEdit::setCursorPosition (mixCurs);
}

//--------------------------------------------- getMixMode -------------------------------------------------------------------
int     CMaskedLineEdit::getMixMode ( )const
{ return       m_MixMode;
}

//--------------------------------------------- setMixMode -------------------------------------------------------------------
void    CMaskedLineEdit::setMixMode (const int &mixmode )
{m_MixMode =  mixmode;
}

//--------------------------------------------- getCarMask -------------------------------------------------------------------
uint     CMaskedLineEdit::getCarMask ( )const
{ return       m_CarMask;
}

//--------------------------------------------- setCarMask -------------------------------------------------------------------
void    CMaskedLineEdit::setCarMask (const uint &carMask )
{m_CarMask =  carMask;
}

//--------------------------------------------- getNumKeyboardMap -------------------------------------------------------------------
QString    CMaskedLineEdit::getNumKeyboardMap () const
{return       m_NumKeyboardMap;
}

//--------------------------------------------- setNumKeyboardMap -------------------------------------------------------------------
void    CMaskedLineEdit::setNumKeyboardMap ( const QString &numKeyboardMap )
{m_NumKeyboardMap = numKeyboardMap;
}


///////////////////////////////////////////////////// COMMON ///////////////////////////////////////////////////////////////

//--------------------------------------------- event -------------------------------------------------------------------
// ACTION: surcharge de la fonction clavier de QLineEdit, afin de manager les évènements pour adapter
//         la saisie avec les chaines template et valid

bool CMaskedLineEdit::event ( QEvent * ev )
{QKeyEvent   *k = (QKeyEvent *)ev;
 //............ pour avoir toutes les touchs (F1 etc) faut graber le clavier ..............
 //             dés reception du focus
 if ( ev->type() == QEvent::FocusIn)
    {QLineEdit::setCursorPosition (CursPosInText_To_PosInMixedString(GetLastNotMaskCarFromEnd(m_Text)));
     //QLineEdit::setCursorPosition (0);
     emit Sig_FocusIn();
    }
 //............ liberer le clavier des qu'on a plus le focus ..............................
 else if ( ev->type() == QEvent::FocusOut)
    {//releaseKeyboard ();
     //QLineEdit::focusOutEvent ( (QFocusEvent *)ev );
     QLineEdit::setCursorPosition (0);
     emit Sig_FocusOut();
    }
 //............... si relachement d'une touche verifier si ce n'est pas ....................
 //                une touche de controle (CTRL ALT SHIFT) pour annuler flag
 else if ( ev->type() == QEvent::KeyRelease )
    {

     switch (k->key())
         {case (Qt::Key_Control):
               m_ControlIsDown &= ~CMaskedLineEdit::Ctrl_Key;
               break;
          case (Qt::Key_Shift):
               m_ControlIsDown &= ~CMaskedLineEdit::Shift_Key;
               break;
          case (Qt::Key_Alt):
               m_ControlIsDown &= ~CMaskedLineEdit::Alt_Key;
               break;
         } // end switch (key()) c = IsCarValid(m_Valid[m_CursPos].latin1(), k->text()[0].latin1());
    }
 //............... si appui d'une touche ...................................................
 else if ( ev->type() == QEvent::KeyPress )
    {
     int      state = k->state();
     int        car = k->ascii();
     int        key = 0;
     //qDebug(QString ("QEvent::KeyPress  : ") + QString::number(k->text()[0].unicode ()) );
     //.............. ===> si touche de Controle ............................................
     //                    positionner flag correspondant
     switch (k->key())
         {case (Qt::Key_Control):
               m_ControlIsDown |= CMaskedLineEdit::Ctrl_Key;
               break;
          case (Qt::Key_Shift):
               m_ControlIsDown |= CMaskedLineEdit::Shift_Key;
               break;
          case (Qt::Key_Alt):
               m_ControlIsDown |= CMaskedLineEdit::Alt_Key;
               break;
          default:
               //.............. ===> Autres touches ..................................................
               //                si utilisé par le gestionnaire de touches de controle
               //                alors bloquer cette touche pour QLineEdit (QEvent::None)
               if ( OnControlKey(m_ControlIsDown, k->key()))
                  {QKeyEvent keyEvent (QEvent::None, k->key(), car, state, k->text(),  k->isAutoRepeat(), k->count() );
                   if ( k->isAccepted ()) keyEvent.accept();
                   else                   keyEvent.ignore();
                   return QLineEdit::event (&keyEvent);
                  }
               //.............. sinon poursuivre chemin normal vers QLineEdit ........................
               else
                 {if ((key = OnChar(ev)) != -1)  // -1 signifie: touche evaluée par nous, à ne pas retourner à QLineEdit
                     {if (key == 0) key = k->key();   // 0  signifie: prendre touche d'origine si pas de retour special
                      QKeyEvent keyEvent (ev->type(), key, car, state, k->text(),  k->isAutoRepeat(), k->count() );
                      if ( k->isAccepted ()) keyEvent.accept();
                      else                   keyEvent.ignore();
                      return QLineEdit::event (&keyEvent);
                     }
                  else
                     {QKeyEvent keyEvent (QEvent::None, k->key(), car, state, k->text(),  k->isAutoRepeat(), k->count() );
                      if ( k->isAccepted ()) keyEvent.accept();
                      else                   keyEvent.ignore();
                      return QLineEdit::event (&keyEvent);
                     }
                }
         } // end switch (key())

    }  //endif ( ev->type() == QEvent::KeyPress )
 else if (ev->type() == QEvent::ContextMenu)
    {m_IsSel = getSelection ( &m_debMix, &m_endMix );
     createPopupMenu ( QCursor::pos() );
     QEvent event (QEvent::None);
     return QLineEdit::event (&event);
    }
 //.............................. click souris .....................................................
 else if (ev->type() == QEvent::MouseButtonRelease)
    {QLineEdit::event (ev);
     m_IsSel = getSelection ( &m_debMix, &m_endMix );
     if (!hasSelectedText ())
        {int mixPos  = cursorPosition();
         m_CursPos   = CursPosInMixedString_To_PosInText(mixPos);
         QLineEdit::setCursorPosition (mixPos);
        }
    }
 return QLineEdit::event (ev);
}

//--------------------------- OnChar -------------------------------------------------------------------
// ACTION: traitement des touches clavier autres que celles associées à une touche de controle  (insertion deletion deplacement)
// RETOUR:  0  la Touche est non traitée dans cette fonction (une touche non traitée devrait etre retournée à QLineEdit)
//         -1  La touche est traitée dans cette fonction et ne doit pas etre traitée par QLineEdit.
//         autre valeur correspondant a un code de touche qui devrait etre retourné et traité par QLineEdit

int CMaskedLineEdit::OnChar(QEvent * ev)
{    QKeyEvent   *k = (QKeyEvent *)ev;
     //int      state = k->state();
     //int        car = k->ascii();
     QString    result;
     int        mixCurs;
     int        debMix, endMix;
     int        key = k->key();
     switch (key)
         {case  Qt::Key_Down:
                emit Sig_KeyDown();
                return -1;

          case  Qt::Key_Up:
                emit Sig_KeyUp();
                return -1;

          case  Qt::Key_Tab:
          case  Qt::Key_Backtab:
                return 0;
          case  Qt::Key_Home:
                if (m_ControlIsDown==CMaskedLineEdit::NoCtrlKey)
                   {if (m_CursPos>0)
                       {m_CursPos=0;
                        QLineEdit::setCursorPosition (CursPosInText_To_PosInMixedString(m_CursPos));
                       }
                    return -1;
                   }
                 break;

          case  Qt::Key_End:
                if (m_ControlIsDown==CMaskedLineEdit::NoCtrlKey)
                   {if (m_CursPos < GetLastNotMaskCarFromEnd(m_Text))
                       {m_CursPos = GetLastNotMaskCarFromEnd(m_Text);
                        QLineEdit::setCursorPosition (CursPosInText_To_PosInMixedString(m_CursPos));
                       }
                    return -1;
                   }
                 break;

          case  Qt::Key_Left:
                if (m_ControlIsDown==CMaskedLineEdit::NoCtrlKey)
                   {if (m_CursPos>0)
                       {m_CursPos--;
                        QLineEdit::setCursorPosition (CursPosInText_To_PosInMixedString(m_CursPos));
                       }
                    return -1;
                   }
                break;

          case  Qt::Key_Right:
                if (m_ControlIsDown==CMaskedLineEdit::NoCtrlKey)
                   {if (m_CursPos < GetLastNotMaskCarFromEnd(m_Text))
                       {m_CursPos++;
                        QLineEdit::setCursorPosition (CursPosInText_To_PosInMixedString(m_CursPos));
                       }
                    return -1;
                   }
                 break;
          case  Qt::Key_Enter:
          case  Qt::Key_Return:
                emit returnPressed();
                return -1;
          case  Qt::Key_Backspace:
                if (m_CursPos>0 && m_Text != "")
                   {m_CursPos--;
                   }
          case  Qt::Key_Delete:
                if (m_Text != "")
                   {if (!QLineEdit::getSelection ( &debMix, &endMix ))
                       {m_Text  = m_Text.left(m_CursPos) + m_Text.mid(m_CursPos + 1);
                        mixCurs = DoMixedString(result);
                        setMixText(result);
                        QLineEdit::setCursorPosition (mixCurs);
                       }
                    else
                       {int deb = CursPosInMixedString_To_PosInText(debMix);
                        int end = CursPosInMixedString_To_PosInText(endMix);
                        if (deb>end){int tmp = deb; deb = end; end = tmp;}
                        if (deb != end)
                           {m_Text    = m_Text.left(deb) + m_Text.mid(end);
                            m_CursPos = deb;
                            mixCurs   = DoMixedString(result);
                            setMixText(result);
                            QLineEdit::setCursorPosition (mixCurs);
                           }
                       }
                   }
                emit Sig_CarEvent(ev);
                return -1;

          default:
            if (k->text().length()>0) // si touche avec contenu affichable
               {QChar c;
                //c = IsCarValid(m_Valid[m_CursPos].latin1(), k->text()[0].latin1());
                //if (c)
                   {if (QLineEdit::getSelection ( &debMix, &endMix ))
                       {int deb = CursPosInMixedString_To_PosInText(debMix);
                        int end = CursPosInMixedString_To_PosInText(endMix);
                        if (deb>end){int tmp = deb; deb = end; end = tmp;}
                        if (deb != end)
                           {m_Text    = m_Text.left(deb) + m_Text.mid(end);
                            m_CursPos = deb;
                            mixCurs   = DoMixedString(result);
                            setMixText(result);
                            QLineEdit::setCursorPosition (mixCurs);
                           }
                       }
                    //qDebug(QString(" Text 0 :") + QString::number((k->text().at(0).unicode ())));      // k->text().latin1()
                    c = IsCarValid(m_Valid.at(m_CursPos), k->text().at(0));

                    if (m_CursPos < (int)m_Valid.length() && ! c.isNull())
                       {m_Text.insert ( m_CursPos, c);
                        m_Text    = m_Text.left(m_Valid.length());
                        m_CursPos = Min(m_CursPos+1, (int)m_Valid.length());
                        m_LastCarToInsert = c;
                        mixCurs   = DoMixedString(result);
                        setMixText(result);
                        m_LastCarToInsert = 0;
                        QLineEdit::setCursorPosition (mixCurs);
                       } //endif (m_CursPos < m_Valid.length() && c)
                   }//endif (c)
                emit Sig_CarEvent(ev);
                return -1;
               }//endif (k->text().length()>0)

         } // end switch (key())
 return 0;
}
//-------------------------- IsCarValid ---------------------------------------
// ACTION: verifie et transforme eventuellement le caractere passe en entree
//         en fonction du critere de validite. si caractere non valide il est
//         retourne un zero, sinon le caractere eventuellement modifie.
// ENTREE: char valid: critère de validite en fait un caractere de la chaine
//                     de validite etendue comme vu plus haut.
//         char c: caractere à tester.
// RETOUR: char:   caractere eventuellement modifie(exemple mis en majuscule)
//                 ou zero si caractere non valide.

QChar  CMaskedLineEdit::IsCarValid(const QChar &valid, QChar c)
{QChar       qChar = c;
 static QString v  = tr(".+-*/^()[]{}");
 static QString rn = tr("1234567890");
 static QString v1 = tr(" _!@#$%^&()+-=~\'`;\",<>|[]{}");
 switch ( valid )    // .unicode ()
    {case 'G':
     case 'g':
             if (m_NumKeyboardMap.length()!=0)
                {int pos = m_NumKeyboardMap.find(c);
                 if ( pos != -1 )                                           return   rn.at( pos );
                }
             if (c >= '0' && c <= '9')          return (c);
             else if (c=='.')                                               return (c);
             else if (c==';')                                               return ('.');
             break;
    case 'c':
    case 'C':

            if ( v.find(c) != -1 )                                         return (c);
            else if (c== ',')                                              return ( '.');

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
            if (c>=  '0' && c<=valid)                        return (c);
            break;

    case 'h':
            c = qChar.lower();
            if ((c>= '0' && c<= '9') ||
                (c>= 'a' && c<= 'f'))          return (c);
            break;

    case 'H':
            c = qChar.upper();
            if ((c>= '0' && c<= '9') ||
                (c>= 'A' && c<= 'F'))          return (c);
            break;
    case 'n':
    case 'N':
            if (c>= '0' && c<= '9')            return (c);

    case 'A':
            c = qChar.upper();
            if ((c>= 'A' && c<= 'Z') ||
                 c== ' ')                                    return (c);
            break;

    case 'a':
            c = qChar.lower();
            if ((c>= 'a' && c<= 'z') ||
                 c== ' ')                                    return (c);
            break;

    case 'P':
            if (c== '\?' || c== '*')           return (c);
    case 'p':
            if (c== '.' || c== ':'   ||
                c== '\\')                                    return (c);
    case 'F':
            if ((c>= 'A' && c<= 'Z') ||
                (c>= 'a' && c<= 'z') ||
                (c>= '0' && c<= '9') ||
                v1.find(c) != -1)                 return (c);
            break;

    case 'U':
            c = qChar.upper();
            //if (c >= ' ' && c<=' ')                                           return (c);
            if (c >= ' ' )                                                     return (c);
            break;

    case 'u':
            c = qChar.lower();
            //if (c>=' ' && c<=127)                                           return (c);
           if (c>=' ')                                                        return (c);
            break;

    case 'V':
            c = qChar.upper();
            if (c>=' ')                                                     return (c);
            break;
    case 'v':
            c = qChar.upper();
            if (c>=' ')                                                     return (c);
            break;
    case 'w':
    case 'W':
    case 'y':
    case 'Y':
            if (c>=' ')                                                    return (c);
            break;
    case 0:                                                                return (c);

    case 'M':
            c = qChar.upper();                                             return (c);
    case 'm':
            c = qChar.lower();                                             return (c);
    case 'X':
    default:                                                               return (c);
    }
    return ( '\0');
}

//-----------------------------------CursPosInText_To_PosInMixedString -------------------------------------------
int CMaskedLineEdit::CursPosInText_To_PosInMixedString(int curspos)
{int pos_tpl     = 0;
 int pos_txt     = 0;
 int txt_len     = GetLastNotMaskCarFromEnd(m_Text);   // m_Text.length();
 int tpl_len     = m_Template.length();
 int curposMixed = 0;

 if (txt_len==0)  return m_Template.find('_');
 do {
     if (m_Template[pos_tpl]=='_' && pos_txt<=txt_len)
        { QChar   c = IsCarValid(m_Valid.at(pos_txt), m_Text.at(pos_txt));
          if (pos_txt <= curspos) curposMixed = pos_tpl;
          if (c.isNull())  pos_tpl--;
          pos_txt++;
        }
     pos_tpl++;
    }while( pos_tpl<tpl_len);
 if (curspos==(int)m_Valid.length())curposMixed++;
 return curposMixed;
 // return Min(curposMixed, GetLastNotMaskCarFromEnd(m_Text));
}

//-----------------------------------CursPosInMixedString_To_PosInText -------------------------------------------
// ACTION: retourne la position du curseur pointant le texte non mixé à partir d'une position dans le texte mixé
//         la position du curseur dans le texte mixé est ajustée sur le caractère editable le plus proche.

int CMaskedLineEdit::CursPosInMixedString_To_PosInText(int &cursposMixed)
{
 int pos_txt             =  0;
 int txt_len             =  GetLastNotMaskCarFromEnd(m_Text);   //m_Text.length();
 int tpl_len             =  m_Template.length();
 int lastValidText       =  pos_txt;
 int fstPosCarInMixedStr =  m_Template.find('_');
 int pos_tpl             =  fstPosCarInMixedStr;
 int lastValidMixpos     =  fstPosCarInMixedStr;

 if (txt_len==0)
    {cursposMixed = m_Template.find('_');
     return 0;
    }
 do {QChar c(1);
     if (m_Template.at(pos_tpl)=='_' && pos_txt<txt_len)
        { c               = IsCarValid(m_Valid.at(pos_txt), m_Text.at(pos_txt));
          lastValidMixpos = pos_tpl;
          lastValidText   = pos_txt;
          pos_txt++;
        }
     if (pos_tpl >= cursposMixed) break;
     if ( ! c.isNull() ) pos_tpl++;
    }while( pos_tpl<tpl_len);

 if (m_Template[pos_tpl]=='_' &&  cursposMixed <= lastValidMixpos)
    {cursposMixed  = lastValidMixpos;
    }
 else
    {cursposMixed  = lastValidMixpos+1;
     lastValidText = Min(txt_len, lastValidText+1);
    }
 return lastValidText;

}

//--------------------------- GetLastNotMaskCarFromEnd -------------------------------------------------------------------
//  rechercher le dernier caractère affichable

int CMaskedLineEdit::GetLastNotMaskCarFromEnd(const QString &txt)
{int i = txt.length();
 if (i<=0) return 0;
 QChar car(m_CarMask);
 while ( txt[--i] == car);
 return ++i;
}

//--------------------------- getEndMixText -------------------------------------------------------------------
//  rechercher le dernier caractère affichable
int CMaskedLineEdit::getEndMixText()
{int pos_lastCarPossible  = m_Template.findRev('_');
 int len_Template         = m_Template.length();
 int len_text             = GetLastNotMaskCarFromEnd(m_Text);

 if (pos_lastCarPossible == len_Template - 1)      // => cas de "MACHIN__TRUC__Bidule__" ou "_______"
    {int pos = len_Template;
     int nbs = 0;
     while(pos>0 && m_Template[pos-1]=='_') pos--; // repérer le dernier caractère possible
     if (pos==0)          return len_text;         // => cas de "__________"
     pos_lastCarPossible = pos;                    // pointe sur premier '_' de la fin
     while(pos>0)                                  // compter le nombre de caractères '_' possibles encadrés par un mot
          {if (m_Template[pos-1]=='_') nbs++;
           pos--;
          }
     nbs = len_text - nbs;                         // calculer le nb de car placés sur les derniers '__'
     if (nbs>0)            return pos_lastCarPossible + nbs;
     else                  return pos_lastCarPossible;
    }
 else                                              // => cas de "MACHIN__TRUC__Bidule__N" ou "_______N"
    {return len_Template;                          // le dernier affichable est le dernier de la chaine template
    }
}

//--------------------------- DoMixedString -------------------------------------------------------------------
int CMaskedLineEdit::DoMixedString(QString &result)
{result                 = "";
 int pos_tpl            = 0;
 int pos_txt            = 0;
 int pos_valid          = 0;
 int txt_len            = GetLastNotMaskCarFromEnd(m_Text);   // m_Text.length();
 int tpl_len            = m_Template.length();
 int der_pos_tpl        = -1;
 int mixCursPos         = -1;
 int last_mix_placed    = -1;
 if   (m_LastCarToInsert == " ") txt_len++; // faut pouvoir rajouter un espace
 if (txt_len==0)
    {result = m_Template;
     result.replace('_', m_CarMask);
     return m_Template.find('_');
    }
 do {QChar   c(1) ;
     if (m_Template[pos_tpl]=='_' )
        {  if ( pos_txt <= m_CursPos ) mixCursPos = pos_tpl;
           if ( pos_txt < txt_len )
              { c = IsCarValid(m_Valid.at(pos_valid), m_Text.at(pos_txt));
                if (c )
                   {result         += c;
                    ++pos_valid;
                   }
                pos_txt++;
              }
           else
              {result += m_CarMask;
               if (last_mix_placed == -1)  last_mix_placed = pos_tpl;
              }
           der_pos_tpl = pos_tpl;
        }
     else
        {result     += m_Template[pos_tpl];
        }
     if ( ! c.isNull () )   pos_tpl++;
    }while( pos_tpl<tpl_len);
 //......... ajustements divers ( un peu le bordel ) ..............
 //          à simplifier

 int lenMax = GetLastNotMaskCarFromEnd(result);
 if   (m_LastCarToInsert != " ")      // faut pouvoir rajouter un espace
      {result.truncate(lenMax);
      }
 else ++lenMax;
 if (m_CursPos >= (int)m_Valid.length()) mixCursPos = der_pos_tpl + 1;
 else if (m_CursPos > (int)txt_len) m_CursPos = txt_len;
 if (last_mix_placed!= -1) mixCursPos = Min(last_mix_placed, mixCursPos );
 return  Min(mixCursPos, lenMax);
}


//---------------------------------------------------- OnControlKey ----------------------------------------------
int CMaskedLineEdit::OnControlKey(int controlState, int key)
{int isControlKeyUsed = 0;

 //....................... tester touches associées à la touche de controle ..........................
 if (controlState & CMaskedLineEdit::Ctrl_Key &&
     controlState & CMaskedLineEdit::Alt_Key  &&
     controlState & CMaskedLineEdit::Shift_Key
    )
    {/*switch (key)
       {
        case (Qt::Key_F1):
           QLineEdit::setText("CTRL ALT SHIFT F1 ");     isControlKeyUsed = 1;
           break;
        case (Qt::Key_F2):
           QLineEdit::setText("CTRL ALT SHIFT F2 ");     isControlKeyUsed = 1;
           break;
        case (Qt::Key_V):
           QLineEdit::setText("CTRL ALT SHIFT V ");      isControlKeyUsed = 1;
           break;
        case (Qt::Key_C):
           QLineEdit::setText("CTRL ALT SHIFT C ");      isControlKeyUsed = 1;
           break;
           
       }*/ //endswitch (key())
    }
 else if (controlState & CMaskedLineEdit::Ctrl_Key &&
          controlState & CMaskedLineEdit::Alt_Key
         )
    {/*switch (key)
       {
        case (Qt::Key_F1):
           QLineEdit::setText("CTRL ALT  F1 ");         isControlKeyUsed = 1;
           break;
        case (Qt::Key_F2):
           QLineEdit::setText("CTRL ALT  F2 ");         isControlKeyUsed = 1;
           break;
        case (Qt::Key_V):
           QLineEdit::setText("CTRL ALT  V ");          isControlKeyUsed = 1;
           break;
        case (Qt::Key_C):
           QLineEdit::setText("CTRL ALT  C ");          isControlKeyUsed = 1;
           break;
           
       }*/ //endswitch (key())
    }
 else if (controlState & CMaskedLineEdit::Ctrl_Key &&
          controlState & CMaskedLineEdit::Shift_Key
         )
    {switch (key)
       {
        case (Qt::Key_X):
           cutAll();
                                                        isControlKeyUsed = 1;
           break;
        case (Qt::Key_C):
          {if (QLineEdit::hasSelectedText ())
              {(QApplication::clipboard())->setText( QLineEdit::selectedText ());
              }
                                                        isControlKeyUsed = 1;
          }
           break;
       } //endswitch (key())
    }
 else if (controlState & CMaskedLineEdit::Ctrl_Key
    )
    {switch (key)
       {case (Qt::Key_Delete):
           clear();
           break;
        case (Qt::Key_X):
           m_IsSel = getSelection ( &m_debMix, &m_endMix );
           cut();                                       isControlKeyUsed = 1;
           break;
        case (Qt::Key_V):
           m_IsSel = getSelection ( &m_debMix, &m_endMix );
           paste();                                     isControlKeyUsed = 1;
           break;
        case (Qt::Key_C):
           m_IsSel = getSelection ( &m_debMix, &m_endMix );
           copy();                                      isControlKeyUsed = 1;
           break;
        case (Qt::Key_A):
           QLineEdit::selectAll ();                     isControlKeyUsed = 1;
           break;
        case (Qt::Key_Z):
           undo();                                      isControlKeyUsed = 1;
           break;
       } //endswitch (key())
    }
 else if (controlState & CMaskedLineEdit::Alt_Key
    )
    {/*switch (key)
       {
        case (Qt::Key_F1):
           QLineEdit::setText(" ALT  F1 ");             isControlKeyUsed = 1;
           break;
        case (Qt::Key_F2):
           QLineEdit::setText(" ALT  F2 ");             isControlKeyUsed = 1;
           break;
        case (Qt::Key_V):
           QLineEdit::setText(" ALT  V ");              isControlKeyUsed = 1;
           break;
        case (Qt::Key_C):
           QLineEdit::setText(" ALT  C ");              isControlKeyUsed = 1;
           break;
           
       } *///endswitch (key())
    }
 else if (controlState & CMaskedLineEdit::Shift_Key
    )
    {/*switch (key)
       {
        
        case (Qt::Key_F2):
           QLineEdit::setText(" SHIFT F2 ");            isControlKeyUsed = 1;
           break;
        case (Qt::Key_V):
           QLineEdit::setText(" SHIFT V ");             isControlKeyUsed = 1;
           break;
        case (Qt::Key_C):
           QLineEdit::setText(" SHIFT C ");             isControlKeyUsed = 1;
           break;
           
       } *///endswitch (key())
    }
 return isControlKeyUsed;
}

//------------------------------------- copy ------------------------------------------------------------------------------
void CMaskedLineEdit::copy()
{int debMix,endMix;
 if (! m_IsSel)  m_IsSel = getSelection ( &m_debMix, &m_endMix );
 if (  m_IsSel)
 //if (GetSelection ( &debMix, &endMix )  == TRUE)
    {debMix  = m_debMix;
     endMix  = m_endMix;
     int deb = CursPosInMixedString_To_PosInText(debMix);
     int end = CursPosInMixedString_To_PosInText(endMix);
     if (deb>end){int tmp = deb; deb = end; end = tmp;}
     if (deb != end)
        {(QApplication::clipboard())->setText( m_Text.mid(deb, end-deb));
        }
     QLineEdit::setSelection (m_debMix, m_endMix-m_debMix );
     }
}
//------------------------------------- extendedCopy ------------------------------------------------------------------------------
void CMaskedLineEdit::extendedCopy()
{int debMix,endMix;
 if (! m_IsSel)  m_IsSel = getSelection ( &m_debMix, &m_endMix );
 if (  m_IsSel)
 //if (GetSelection ( &debMix, &endMix )  == TRUE)
    {debMix  = m_debMix;
     endMix  = m_endMix;
     if (debMix>endMix){int tmp = debMix; debMix = endMix; endMix = tmp;}
     if (debMix != endMix)
        { QString txt(QLineEdit::text());
         (QApplication::clipboard())->setText( txt.mid(debMix, endMix-debMix));
        }
     QLineEdit::setSelection (m_debMix, m_endMix-m_debMix );
     }
}
//------------------------------------- clear ------------------------------------------------------------------------------
void CMaskedLineEdit::clear()
{QString result;
 int mixCurs;
 m_Text    = "";
 m_CursPos = 0;
 mixCurs   = DoMixedString(result);
 setMixText(result);
 QLineEdit::setCursorPosition (mixCurs);
 m_IsSel = FALSE;
}

//------------------------------------- setMixText ------------------------------------------------------------------------------
void CMaskedLineEdit::setMixText(const QString& result)
{if (m_MixMode==0) QLineEdit::setText(result);
 else
    {QLineEdit::setText(result.left( getEndMixText()));
    }
}

//------------------------------------- cutAll ------------------------------------------------------------------------------
void CMaskedLineEdit::cutAll()
{QString result;
 int debMix,endMix,mixCurs;
 if (! m_IsSel)  m_IsSel = getSelection ( &m_debMix, &m_endMix );
 if (  m_IsSel)
 //if (GetSelection ( &debMix, &endMix )  == TRUE)
    {debMix  = m_debMix;
     endMix  = m_endMix;
     int deb = CursPosInMixedString_To_PosInText(debMix);
     int end = CursPosInMixedString_To_PosInText(endMix);
     if (debMix>endMix){int tmp = debMix; debMix = endMix; endMix = tmp;}
     if (deb>end)      {int tmp = deb; deb = end; end = tmp;}
     if (debMix != endMix)
        {QString txt(QLineEdit::text());
         (QApplication::clipboard())->setText( txt.mid(debMix, endMix-debMix));
         m_Text    = m_Text.left(deb) + m_Text.mid(end);
         m_CursPos = deb;
         mixCurs   = DoMixedString(result);
         setMixText(result);
         QLineEdit::setCursorPosition (mixCurs);
        }
     }
 ClearSelection ();
}

//------------------------------------- cut ------------------------------------------------------------------------------
void CMaskedLineEdit::cut()
{QString result;
 int debMix,endMix,mixCurs;
 //m_IsSel = getSelection ( &m_debMix, &m_endMix );
 if (! m_IsSel)  m_IsSel = getSelection ( &m_debMix, &m_endMix );
 if (  m_IsSel)
 //if (GetSelection ( &debMix, &endMix )  == TRUE)
    {debMix  = m_debMix;
     endMix  = m_endMix;
     int deb = CursPosInMixedString_To_PosInText(debMix);
     int end = CursPosInMixedString_To_PosInText(endMix);
     if (deb>end){int tmp = deb; deb = end; end = tmp;}
     if (deb != end)
        {(QApplication::clipboard())->setText( m_Text.mid(deb, end-deb));
         m_Text    = m_Text.left(deb) + m_Text.mid(end);
         m_CursPos = deb;
         mixCurs   = DoMixedString(result);
         setMixText(result);
         QLineEdit::setCursorPosition (mixCurs);
        }
     }
 ClearSelection ();
}

//------------------------------------- paste ------------------------------------------------------------------------------
void CMaskedLineEdit::paste()
{
 QString txt((QApplication::clipboard())->text());
 if (txt.isNull())   return;
 if (txt.isEmpty())  return;
 if (txt=="")        return;
 int debMix, endMix, mixCurs;
 QString result;
 if (! m_IsSel)  m_IsSel = getSelection ( &m_debMix, &m_endMix );
 if (  m_IsSel)
 //if (GetSelection ( &debMix, &endMix )  == TRUE)
    {debMix  = m_debMix;
     endMix  = m_endMix;
     int deb = CursPosInMixedString_To_PosInText(debMix);
     int end = CursPosInMixedString_To_PosInText(endMix);
     if (deb>end){int tmp = deb; deb = end; end = tmp;}
     if (deb != end)
        {m_Text    = m_Text.left(deb) + m_Text.mid(end);
         m_CursPos = deb;
         mixCurs   = DoMixedString(result);
         setMixText(result);
         QLineEdit::setCursorPosition (mixCurs);
         }
     }
 if (m_CursPos < (int)m_Valid.length())
    {m_Text.insert ( m_CursPos, txt);
     m_Text    = m_Text.left(m_Valid.length());
     m_CursPos = Min(m_CursPos + (int)txt.length(), (int)m_Valid.length());
     mixCurs   = DoMixedString(result);
     setMixText(result);
     QLineEdit::setCursorPosition (mixCurs);
    } //endif (m_CursPos < m_Valid.length() && c)
 ClearSelection ();
}

//----------------------------------------- GetSelection -----------------------------------------------------------------
bool CMaskedLineEdit::GetSelection ( int *debMix, int *endMix  )
{*debMix = m_debMix;
 *endMix = m_endMix;
  return m_IsSel;
}

//----------------------------------------- ClearSelection -----------------------------------------------------------------
void CMaskedLineEdit::ClearSelection ()
{m_IsSel = 0;
}
//----------------------------------------- setCursorPosition -----------------------------------------------------------------
void CMaskedLineEdit::setCursorPosition (int cursPos)
{if (cursPos==-1) cursPos = GetLastNotMaskCarFromEnd(m_Text);
 m_CursPos = cursPos;
 QLineEdit::setCursorPosition (CursPosInText_To_PosInMixedString(cursPos));
}

//----------------------------------------- createPopupMenu -----------------------------------------------------------------
// ACTION: recherche lors de l'appuis du bouton droit si il y a une zone de liste (bleue #0000ff)
//         et auquel cas rajoute les items de cette liste au popup menu classique.

QPopupMenu * CMaskedLineEdit::createPopupMenu ( const QPoint & pos )
{QPopupMenu *pQPopupMenu = new QPopupMenu(0, "MyCustomPopupMenu" );
 if (pQPopupMenu==0)    return 0;
 QPoint qpoint(pos);
 m_IsSel = getSelection ( &m_debMix, &m_endMix );
 pQPopupMenu->insertItem( tr("Annu&ler       \t Ctrl+Z"),          this, SLOT( undo()),   CTRL+Key_Z );
 pQPopupMenu->insertItem( tr("&Refaire       \t Ctrl+Y"),          this, SLOT( redo()),   CTRL+Key_Y );
 pQPopupMenu->insertSeparator ( ) ;
 pQPopupMenu->insertItem( tr("&Couper        \tCtrl+X"),          this, SLOT( cut()),    CTRL+Key_X );
 pQPopupMenu->insertItem( tr("C&opier       \tCtrl+C"),          this, SLOT( copy()),   CTRL+Key_C );
 pQPopupMenu->insertItem( tr("Cop&ie étendue   \tShift+Ctrl+C"),    this, SLOT( extendedCopy()),SHIFT+CTRL+Key_Y );
 pQPopupMenu->insertItem( tr("C&oller      \tCtrl+V"),          this, SLOT( paste()),  CTRL+Key_P );
 //pQPopupMenu->insertItem( tr("E&ffacer      \tCtrl+Del"),        this, SLOT( clear()),  CTRL+Key_R );

 pQPopupMenu->insertSeparator ( ) ;
 pQPopupMenu->insertItem( tr("&Tout sélectionner \tCtrl+A"),          this, SLOT( selectAll()),  CTRL+Key_A );


 pQPopupMenu->exec(QCursor::pos());
 delete  pQPopupMenu;
 return  pQPopupMenu;
}

