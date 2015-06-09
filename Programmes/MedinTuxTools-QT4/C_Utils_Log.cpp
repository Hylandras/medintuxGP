#include "C_Utils_Log.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QScrollBar>
#include <QTextEdit>
#include <QApplication>
#include <QDateTime>
#include <QDebug>

//========================================= C_Utils_Log ======================================================
//-----------------------------------------------------  findNext -------------------------------------------
C_Utils_Log::C_Utils_Log()
{
}


//-----------------------------------------------------  findNext -------------------------------------------
/*! \brief  go to the next separator (const QString &sep) and jump alls enclosed strings (by const QChar enclosedSymbol).
 *  \param  QString       &text                 text to process
 *  \param  int            pos                  position where begin process
 *  \param  const QString &sep                  separator
 *  \param  const QString &enclosedSymbol       string enclosed symbols wich must be jumped
 *  \return the next separator position.
*/

int  C_Utils_Log::findNextSep(const QString & text, int pos, const QString &sep /* = "\n"*/ , const QChar enclosedSymbol /* = \' */)
{int textLen = text.length();
 int  inside = false;
 int  lenSep = sep.length();

 while (pos<textLen)
    {if (text[pos]=='\\')
        {pos += 2;
        }
     else if (text[pos]==enclosedSymbol)
        {inside ^= 1;
        }
     else if (!inside                         &&   // ne pas etre inside chaine
               text[pos]            == sep[0] &&   // en testant prem car on gagne un peu de temps, sur un text.mid(pos,lenSep) direct
               text.mid(pos,lenSep) == sep
             ) return pos;
     ++pos;
    }
 return -1;
}

//-----------------------------------------------------  jumpEnclosedsSymbols -------------------------------------------
/*! \brief  jump alls enclosed strings (by const QChar enclosedSymbol).
 *  \param  QString       &text                 text to process
 *  \param  long           pos                  position where begin  process, must be just before first enclosedSymbol
 *  \param  long           end                  position where finish process
 *  \param  const QString &enclosedSymbol       string enclosed symbols wich must be jumped
 *  \return the position just after enclosed symbol.
*/

long  C_Utils_Log::jumpEnclosedsSymbols(const QString & text, long pos, long end, const QString &enclosedSymbol /* = \"\"" */)
{int  inside = false;
 long len    =  enclosedSymbol.length();
 while (pos<end)
    {if (text.at(pos)=='\\')
        {pos += 2;
        }
     else if (text.mid(pos,len)==enclosedSymbol)
        {pos    += len-1;  // -1 car ++pos
         inside ^= 1;
        }
     if (!inside ) return pos+1;
     ++pos;
    }
 return -1;
}

//-------------------------------------------------- outSQL_error -----------------------------------------------
// __FILE__ : affiche le nom du fichier source.
// __LINE__ : affiche le numero de la ligne atteinte
// __DATE__ : affiche la date de compilation du code source
// __TIME__ : affiche l'heure de compilation de la source
/*! \brief  display et retrieve last QSqlQuery error
 *  \param  QTextEdit *logWidget                QTextEdit message display widget
 *  \param  const QSqlQuery &cur                QSqlQuery from wich error must be retrieve
 *  \param  const QString &messFunc             title of the message
 *  \param  const QString &requete              QSqlQuery requete
 *  \return the message if error occured, empty if not.
*/
QString C_Utils_Log::outSQL_error( QTextEdit *logWidget, const QSqlQuery &cur, const QString &messFunc /*=""*/, const QString &requete /*=""*/, const QString &file /*=""*/, const int line /*= -1*/)
{   QSqlError error       = cur.lastError();
    QString lastError     = error.databaseText();
    if (lastError.length())
       { lastError        =  lastError.prepend(QString(messFunc) + "\n") + "\n" + requete;
                  qDebug() << removeHtmlTag(lastError).toLatin1();
          outMessage(logWidget, lastError, file, line);
       }
    return lastError;
}

//----------------------------------------------- outMessage ---------------------------------------------------------------------
/*! \brief  append a message in a log QTextEdit
 *  \param  QTextEdit *logWidget                QTextEdit message display widget
 *  \param  const QString &mess                 message to append
*/
void  C_Utils_Log::outMessage(QTextEdit *logWidget, const QString &mess, const QString &file /*=""*/, const int line /*= -1*/)
{
    QString str =  "";                                  // += v.message.leftJustified(100,' ');;
    if (file.length())
       {str = QObject::tr ("------- %1\t at line : %2 \t in file : %3 ---------\n").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss:zzz").leftJustified(24),QString::number(line),file);
       }
    else
       {str = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss:zzz").leftJustified(24);
       }
 str += mess;
 if (  mess.startsWith(QObject::tr("ERR")) &&
     ( mess.startsWith (QObject::tr("ERREUR")) || mess.startsWith (QObject::tr("ERROR")) )
    )
    {str = str.prepend("<font color=#ff0000>")+"</font>";
    }
 //qDebug(removeHtmlTag(str).toLatin1());
 if (logWidget==0)  return;
 logWidget->append( str );
 //.......... scrollToBottom ................
 QScrollBar *sb = logWidget->verticalScrollBar();
 sb->setValue(sb->maximum());
 if (qApp) {qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();qApp->processEvents();}
}

//---------------------------------- removeHtmlTag ---------------------------------------------------------------------------
/*! \brief  remove html tags from a string
 *  \param  const QString &txt   text to compute
 *  \return the string without html tags.
*/
QString C_Utils_Log::removeHtmlTag( const QString &txt)
{int  posDeb   = -1;
 int  posEnd   =  0;
 int  debSign  =  0;
 int  endSign  =  0;
 bool htmlFind = false;
 QString strRet;
 while ( (posDeb  =  txt.indexOf("<", posDeb + 1)) != -1  )
       {  posEnd  =  txt.indexOf(">", posDeb + 1);
          endSign =  posDeb;                          //       fin des caracteres significatifs avant '<'
          if (posEnd != -1)
            {QString tag = txt.mid(posDeb, posEnd-posDeb);
             if (tag == "<br /" || tag=="<br")
                 {strRet +=  "\n";
                 }
             else if (tag == "<p /" || tag=="<p")
                 {strRet +=  "\n\n";
                 }
             else if (endSign>debSign)
                 {while( (txt[debSign]=='\r' || txt[debSign]=='\n') && debSign<endSign) ++debSign;
                  strRet  +=  txt.mid(debSign, endSign-debSign);
                  htmlFind = true;
                 }
             debSign = posEnd + 1;                    //       debut des caracteres significatifs apres '>'
             posDeb  = posEnd;
            }
       }
 if (!htmlFind) return txt;
 if (posEnd != -1) strRet += txt.mid(posEnd+1);    // rapatrier les reliquats
 return  strRet;
}

//=============================================== C_Log ================================================
C_Log::C_Log(QTextEdit *logWidget /*=0 */)
{   m_logWidget = 0;
    setLogWidget(logWidget);
}

//-------------------------------------------------- outSQL_error -----------------------------------------------
/*! \brief  display et retrieve last QSqlQuery error
 *  \param  const QSqlQuery &cur                QSqlQuery from wich error must be retrieve
 *  \param  const QString &messFunc             title of the message
 *  \param  const QString &requete              QSqlQuery requete
 *  \param  const QString   &file               source file
 *  \param  const int       &line               source line number
 *  \return the message if error occured, empty if not.
*/
QString C_Log::outSQL_error( const QSqlQuery &cur, const QString &messFunc /* ="" */ , const QString &requete /* ="" */ , const QString &file /* ="" */, const int line /* = -1*/ )
{   return  C_Utils_Log::outSQL_error(m_logWidget, cur, messFunc , requete , file , line );
}

//----------------------------------------------- outMessage ---------------------------------------------------------------------
/*! \brief  append a message in a log widget QTextEdit
 *  \param  const QString &mess                 message to append
 *  \param  const QString   &file               source file
 *  \param  const int       &line               source line number
*/
void     C_Log::outMessage( const QString &mess, const QString &file /* = "" */, const int line /* = -1*/)
{        C_Utils_Log::outMessage(m_logWidget, mess, file, line );
}

//----------------------------------------------- setLogWidget ---------------------------------------------------------------------
/*! \brief  initialise class on a QTextEdit logwidget
 *  \param  QTextEdit *logWidget                pointer on the logwidget
*/
void     C_Log::setLogWidget(QTextEdit *logWidget)  { m_logWidget = logWidget; }

//----------------------------------------------- logWidget ---------------------------------------------------------------------
/*! \brief  return the QTextEdit logwidget
 *  \param  QTextEdit *logWidget                pointer on the logwidget
*/
QTextEdit  *C_Log::logWidget(){ return m_logWidget; }
