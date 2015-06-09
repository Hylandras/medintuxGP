#ifndef C_UTILS_LOG_H
#define C_UTILS_LOG_H

#include <QObject>
class QTextEdit;
class QSqlQuery;
class QApplication;



//================================= C_Utils_Log ===================================
/*! \class C_Utils_Log
 *  \brief utils .
 *  - this Class contains various generals statics fonctions
 *    to manage strings parsing and so.
 */

class C_Utils_Log
{

public:

    explicit             C_Utils_Log();

    //-----------------------------------------------------  findNext -------------------------------------------
    /*! \brief  go to the next separator (const QString &sep) and jump alls enclosed strings (by const QChar enclosedSymbol).
     *  \param  QString       &text                 text to process
     *  \param  int            pos                  position where begin process
     *  \param  const QString &sep                  separator
     *  \param  const QString &enclosedSymbol       string enclosed symbols wich must be jumped
     *  \return the next separator position.
    */
    static int           findNextSep(const QString & text, int pos, const QString &sep  = "\n" , const QChar enclosedSymbol  = '\'' );

    //-----------------------------------------------------  jumpEnclosedsSymbols -------------------------------------------
    /*! \brief  jump alls enclosed strings (by const QChar enclosedSymbol).
     *  \param  QString       &text                 text to process
     *  \param  long           pos                  position where begin  process, must be just before first enclosedSymbol
     *  \param  long           end                  position where finish process
     *  \param  const QString &enclosedSymbol       string enclosed symbols wich must be jumped
     *  \return the position just after enclosed symbol.
    */

    static long          jumpEnclosedsSymbols(const QString & text, long pos, long end, const QString &enclosedSymbol = "\"" );
    //-------------------------------------------------- outSQL_error -----------------------------------------------
    /*! \brief  display et retrieve last QSqlQuery error
     *  \param  QTextEdit *logWidget                QTextEdit message display widget
     *  \param  const QSqlQuery &cur                QSqlQuery from wich error must be retrieve
     *  \param  const QString &messFunc             title of the message
     *  \param  const QString &requete              QSqlQuery requete
     *  \param  const QString   &file               source file
     *  \param  const int       &line               source line number
     *  \return the message if error occured, empty if not.
    */
    static               QString outSQL_error(QTextEdit *logWidget, const QSqlQuery &cur, const QString &messFunc ="", const QString &requete ="" , const QString &file ="", const int line = -1);

    //----------------------------------------------- outMessage ---------------------------------------------------------------------
    /*! \brief  append a message in a log QTextEdit
     *  \param  QTextEdit *logWidget                QTextEdit message display widget
     *  \param  const QString &mess                 message to append
     *  \param  const QString   &file               source file
     *  \param  const int       &line               source line number
    */
    static               void    outMessage(QTextEdit *logWidget, const QString &mess, const QString &file ="", const int line = -1);

    //----------------------------------------------- removeHtmlTag ---------------------------------------------------------------------
    /*! \brief  remove html tags from a string
     *  \param  const QString &txt   text to compute
     *  \return the string without html tags.
    */
    static               QString removeHtmlTag( const QString &txt);
};


//================================= C_Log ===================================
/*! \class C_Log
 *  \brief utils .
 *  - this Class contains various generals fonctions
 *    to manage strings log and so.
 */

class C_Log : public C_Utils_Log
{
public:
    //----------------------------------------------- C_Log ---------------------------------------------------------------------
    /*! \brief  constructor initialise class on a QTextEdit logwidget
     *  \param  QObject *parent        parent of the class
     *  \param  QTextEdit *logWidget   pointer on the logwidget
    */

    explicit             C_Log(QTextEdit *logWidget=0);

    //-------------------------------------------------- outSQL_error -----------------------------------------------
    /*! \brief  display et retrieve last QSqlQuery error
     *  \param  QTextEdit *logWidget                QTextEdit message display widget
     *  \param  const QSqlQuery &cur                QSqlQuery from wich error must be retrieve
     *  \param  const QString &messFunc             title of the message
     *  \param  const QString &requete              QSqlQuery requete
     *  \param  const QString   &file               source file
     *  \param  const int       &line               source line number
     *  \return the message if error occured, empty if not.
    */
    QString     outSQL_error( const QSqlQuery &cur, const QString &messFunc ="", const QString &requete ="" , const QString &file ="", const int line = -1);

    //----------------------------------------------- outMessage ---------------------------------------------------------------------
    /*! \brief  append a message in a display logwidget QTextEdit
     *  \param  const QString &mess                 message to append
     *  \param  const QString   &file               source file
     *  \param  const int       &line               source line number
    */
    void        outMessage( const QString &mess, const QString &file ="", const int line = -1);
    //----------------------------------------------- setLogWidget ---------------------------------------------------------------------
    /*! \brief  initialise class on a QTextEdit display logwidget
     *  \param  QTextEdit *logWidget                pointer on the logwidget
    */
    void        setLogWidget(QTextEdit *logWidget);
    //----------------------------------------------- logWidget ---------------------------------------------------------------------
    /*! \brief  return the QTextEdit display logwidget
     *  \param  QTextEdit *logWidget                pointer on the logwidget
    */
    QTextEdit  *logWidget();

protected:
    QTextEdit  *m_logWidget;    // display widget
};

#endif // C_UTILS_LOG_H
