#include "C_Utils_Html.h"
#include <QNetworkCookie>
#include <QByteArray>
#include <QTextStream>
#include <QList>
#include <QTextEdit>
#include <QTextCursor>
#include <QApplication>
#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QProgressBar>
#include <QStringList>
#include <QMap>

#include "C_Utils_Log.h"
#include "CGestIni.h"

char C_Utils_Html::Utf8_Insecable[3]           = {194,160,0};   // espace insecable

  //===================================================== CookieJar ===================================================================================
  //---------------------------------------  CookieJar ---------------------------------------------------------
  CookieJar::CookieJar (QString path, QObject *parent /* = 0*/, QTextEdit *pQTextEditLog /*=0 */) : QNetworkCookieJar(parent), C_Log(pQTextEditLog)  //on preds en argument le nom du fichier ou l'on doit sauver/charger les cookies
  {   m_file      = path; //on stocke le nom de ce fichier
      QFile cookieFile(m_file);
      if (cookieFile.exists() && cookieFile.open(QIODevice::ReadOnly) )  //si on peut l'ouvrir...
      {
          outMessage( QObject::tr("Start -------- CookieJar List -------------"));
          QList<QNetworkCookie> list;  //on cree une liste vide de cookies
          QByteArray line; //on cree une ligne vide ( ca serviera de memoire-tampon)
          while(!(line = cookieFile.readLine()).isNull()) //tant qu'on a pas tout lu dans notre fichier...
          {   QString str = line; str.replace('\r','\n'); if (str.endsWith("\n")) str.truncate(str.length()-1);  // virer le dernier retour chariot
              outMessage( ".   CookieJar <b>" +  str +  "</b>");
              list.append(QNetworkCookie::parseCookies(line)); //on parse la ligne en cours pour en faire une cookie qu'on ajoute a notre liste
          }
          outMessage( QObject::tr("End -------- CookieJar List -------------"));
          setAllCookies(list); //et on charge notre liste <IMG src="http://s1.wp.com/wp-includes/images/smilies/icon_wink.gif?m=1235253170g" alt=";-)" class="wp-smiley">
      }
      else  //si on ne paut pas ouvrir le fichier...
      {
          outMessage( QObject::tr("WARNING : CookieJar:: can't open  <b>%1</b> for reading cookies !").arg(m_file));
      }

  }

  //---------------------------------------  ~CookieJar ---------------------------------------------------------
  CookieJar::~CookieJar () //destructeur; sauvegarde les cookies
  {
          QList <QNetworkCookie> cookieList;  //   on cree une liste de cookies...
          cookieList = allCookies();          //...et on la remplit de toutes nos cookies
          QFile file(m_file);

          if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
          {
                  outMessage( QObject::tr("WARNING : CookieJar:: can't open  <b>%1</b> for writing cookies !").arg(m_file));
                  return;
          }

          QTextStream out(&file); //sinon, on cree un flux a partir de notre fichier...

          for(int i = 0 ; i < cookieList.size() ; i++) //... et pour chaque cookie qu'on possede...
          {
                  QByteArray line =  cookieList.at(i).toRawForm(QNetworkCookie::Full); //...on recupere les donnees brutes de la cookie...
                  out << line << "\n"; //...et on les met dans le fichier !
                  // NB: ne pas oublier le \n pour sauter une ligne entre les cookies: sinon ca marche pas ! /
          }

          file.close(); //on ferme le fichier
  }

  //---------------------------------------  CookieJar ---------------------------------------------------------
  QList<QNetworkCookie> CookieJar::cookiesForUrl ( const QUrl & url ) const //est utilise par le QNetworkManager
  {
      return QNetworkCookieJar::cookiesForUrl(url); //on ne touche pas
  }

  //---------------------------------------  CookieJar ---------------------------------------------------------
  bool   CookieJar::setCookiesFromUrl ( const QList<QNetworkCookie> & cookieList, const QUrl & url ) //est utilise par le QNetworkManager
  {
      return QNetworkCookieJar::setCookiesFromUrl(cookieList, url); //on ne touche pas
  }


//================================================================ C_Utils_Html ==========================================================
//---------------------------------------  C_Utils_Html ---------------------------------------------------------
C_Utils_Html::C_Utils_Html(QObject *parent /* =0 */, QNetworkAccessManager *pQNetworkAccessManager /* =0 */, QTextEdit *pQTextEditLog /*=0 */ )
        : QObject(parent), C_Log(pQTextEditLog)
{m_LastError               = "";
 m_Wait_HttpProcess        = 0;
 m_position                = 0;
 m_pQProgressBar           = 0;
 //m_EndWaitHttpProcess      = 100000000;
 m_pQNetworkAccessManager  = pQNetworkAccessManager;
 m_QByteArray.clear();
}

//---------------------------------------  makeLinkList ---------------------------------------------------------
/*! \brief add link chapiter list to html text with structure
    as 1        for chapiter one
    as 1.1      for sub chapiter one
    as 1.1.1    for sub sub chapiter one  an so
 *  \param const QString txt input html text
 *  \param const QString prefix caracters before chapiter number line
 *  \param const QString endfix caracters after chapiter number line
 *  \return QString result whith anchor, links, and chapiters list link
 */

QString  C_Utils_Html::makeLinkList(const QString &txt, const QString prefix, const QString endfix, const QString idAnchor /* = "" */)
{   QStringList chapSegmt;
    int   i          =  0;
    int oldPos       =  0;
    int pos          =  0;
    int end          = -1;
    QString anchor   = "";
    QString line     = "";
    QString numChap  = "";
    QString linkList = "";
    QString result   = "";

    while ( (pos     = txt.indexOf(prefix , pos, Qt::CaseInsensitive)) != -1 )
    {i     = 0;
     pos  += prefix.length();
     if ( endfix=="EOL" )
        {              end = txt.indexOf('\n' ,     pos, Qt::CaseInsensitive);
          if (end==-1) end = txt.indexOf('\r' ,     pos, Qt::CaseInsensitive);
          if (end==-1) end = txt.indexOf("<br />" , pos, Qt::CaseInsensitive);
          if (end==-1) end = txt.indexOf("<br/>" ,  pos, Qt::CaseInsensitive);
          if (end==-1) end = txt.indexOf("<br>" ,   pos, Qt::CaseInsensitive);
        }
     else
        { end   = txt.indexOf(endfix , pos, Qt::CaseInsensitive);
        }
     if (end==-1)                                            continue;
     line = txt.mid(pos, end-pos).remove('\r').remove('\n').trimmed();
     if ( !((line[i]>='0' && line[i]<='9')|| line[i]=='*'))  continue;            // si premier car n'est pas un chiffre reboucler
     if ( (i = line.indexOf(' '))==-1)                       continue;            // avancer jusqu'au prochain espace et si pas trouve reboucler

     numChap = line.left(i);
     if (! (numChap.contains('.')|| numChap.contains('*')))  continue;            // si pas au moins 1 point on reboucle
     for (i=0;i<numChap.length();++i)                            // verifier si ne contient que . et 0 <--> 9
         {if ( ! (line[i]=='.' || (line[i]>='0' && line[i]<='9') || line[i]=='*') ) break;
         }
     if (i<numChap.length())                continue;            // si autre caractere que . et 0 <--> 9 reboucler

     //<a name="ancrage"></A>Point d'ancrage<br><br>
     //<a href="#ancrage">Vers le point d'ancre ci-dessus</A>
     chapSegmt  = numChap.split ('.', QString::SkipEmptyParts);
     result    += txt.mid(oldPos, pos-oldPos);
     anchor     = QString("<h%3 class=titre_resume><a name=\"%4_%1\">%2</a></h%3>\n").arg(line,line,QString::number(chapSegmt.count()),idAnchor);
     if (chapSegmt.count()==1) anchor = anchor.prepend("<b>").append("</b>");
     if (chapSegmt.count()==2) anchor = anchor.prepend("<b>").append("</b>");
     result    += anchor;
     linkList.append(QString("<a href=\"#%3_%1\">%2</a><br>").arg(line,line,idAnchor));      // <a name="smr0"></a>
     oldPos = end; while( oldPos<txt.length() && (txt[oldPos]=='\n'||txt[oldPos]=='\r') )++oldPos;       // on se place apres le titre
    }

    if(linkList.length())
      {result += txt.mid(oldPos);
       result  = result.prepend(linkList+"<hr>");
       return result;
      }
    else
      {return txt;
      }
}

//---------------------------------------  removeHtmlTag ---------------------------------------------------------
//QString  C_Utils_Html::removeHtmlTag(const QString &text){return C_Utils_Log::removeHtmlTag(text);}

//------------------------------------ saveImagesFromTag -------------------------------
/*! \brief download alls distants datas from tag and attribut tag where attributs datas represents url
 *  \param const QString& html :     page to analyse where find tags and attributs
 *  \param const QString& url :      base page url
 *  \param const QString &imageDir : directory where datas must be downloaded
 *  \param const QString &tag :      tag to find
 *  \param const QString &attribut : attribut of the tag where is data url
 *  \return   number of data found
 */
int C_Utils_Html::saveImagesFromTag(const QString& html, const QString& url_in, const QString &imageDir, const QString &tag, const QString &attribut)
{   int                   i = 0;
    QStringList      imgList;
    QByteArray            ba;
    QString          fileName = "";
    QString       distantFile = "";
    QString              url  = url_in; if ( (i=url.lastIndexOf('/'))!= -1) url.truncate(i);  // normaliser l'url au repertoire (cas ou elle donne avec nom de fichier)

    getImagesList( html, tag,attribut, imgList);
    for (i=0; i<imgList.count(); ++i)
        {//outMessage( TR("For tag :'%1' image : '%2'").arg("td",imgList[i]) );
         fileName = imageDir + QFileInfo(imgList[i]).fileName();
         if (!QFile::exists(fileName))
            {distantFile = url+"/"+imgList[i];
             if      (distantFile.startsWith("http://"))  distantFile = QString("http://")  + QDir::cleanPath(distantFile.mid(7));
             else if (distantFile.startsWith("https://")) distantFile = QString("https://") + QDir::cleanPath(distantFile.mid(8));
             QStringList rawHeadersList;
             ba = getDataFromUrl(distantFile, rawHeadersList);
              //outMessage( tr(".    Images distantes: %1").arg(url+"/"+imgList[i]));
              if (!ba.isEmpty())
                 {CGestIni::Param_UpdateToDisk(fileName, ba);
                  //outMessage( tr(".    Images locales: %1").arg(fileName));
                 }
            }
        }
 return i;
}
//---------------------------------------  postDataFromUrl ---------------------------------------------------------
/*! \brief uploag and post distants datas from url
 *  \param const QString &url :                                url where post data
 *  \param const QString &postData :                           data to post
 *  \param QNetworkAccessManager *pQNetworkAccessManager :     manager
 *  \return   data in a QByteArray
 */
bool C_Utils_Html::postDataFromUrl(const QString &url, const QString &postData,  const QStringList& rawHeaders, QNetworkAccessManager *pQNetworkAccessManager /* = 0*/)
{if (pQNetworkAccessManager) m_pQNetworkAccessManager = pQNetworkAccessManager;
 if (m_pQNetworkAccessManager == 0 ){outMessage( tr("ERREUR : postDataFromUrl() net work manager not initialized ")); return false;}
 beginPost_FromUrl(url, postData, rawHeaders);
 while (m_Wait_HttpProcess){qApp->processEvents(); /*--m_Wait_HttpProcess;*/}
 return true;
}
//--------------------------- beginPost_FromUrl -----------------------------------------------------------
void C_Utils_Html::beginPost_FromUrl(const QString &url, const QString &postData, const QStringList& rawHeaders)
{   m_Wait_HttpProcess = m_EndWaitHttpProcess;
    m_LastError   = "";
    if (m_pQProgressBar)   {m_position = 0;  m_pQProgressBar->setRange(0,0);  }
    disconnect(m_pQNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Slot_DataLoadFinished(QNetworkReply*)));
    connect(m_pQNetworkAccessManager,    SIGNAL(finished(QNetworkReply*)), this, SLOT(Slot_DataPostFinished(QNetworkReply*)));
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    for (int i=0; i<rawHeaders.count(); ++i)
        {QString     rawH = rawHeaders[i];
         QStringList list = rawH.split('|');
         request.setRawHeader(list[0].toLatin1(), list[1].toLatin1());
        }
    if (rawHeaders.count()==0) request.setRawHeader("User-Agent", "Free-for-all");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    m_pQNetworkAccessManager->post(request, postData.toUtf8());
}
//--------------------------- Slot_DataPostFinished -----------------------------------------------------------
void   C_Utils_Html::Slot_DataPostFinished(QNetworkReply *pQNetworkReply) //QNetworkReply::NoError
{if (pQNetworkReply->error()!= QNetworkReply::NoError) m_LastError = pQNetworkReply->errorString();
 if (m_LastError.length()) outMessage( tr("ERREUR : postDataFromUrl() <br> %1 ").arg(m_LastError));
 disconnect(m_pQNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Slot_DataPostFinished(QNetworkReply*)));
 disconnect(m_pQNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Slot_DataLoadFinished(QNetworkReply*)));
 m_Wait_HttpProcess = 0;
 if (m_pQProgressBar)  {m_pQProgressBar->setRange(0,100); m_pQProgressBar->setValue(0); m_pQProgressBar->reset();}
}

//---------------------------------------  getDataFromUrl ---------------------------------------------------------
/*! \brief download distants datas from url
 *  \param const QString &url :                                url where get data
 *  \param QNetworkAccessManager *pQNetworkAccessManager :     manager
 *  \return   data in a QByteArray
 */
QByteArray  C_Utils_Html::getDataFromUrl(const QString &url, const QStringList& rawHeaders, QNetworkAccessManager *pQNetworkAccessManager /* = 0*/)
{ m_QByteArray.clear();
  if (pQNetworkAccessManager) m_pQNetworkAccessManager = pQNetworkAccessManager;
  if (m_pQNetworkAccessManager == 0 ){outMessage( tr("ERREUR : getDataFromUrl() net work manager not initialized ")); return m_QByteArray;}
  beginGet_FromUrl(url, rawHeaders);
  while (m_Wait_HttpProcess){qApp->processEvents(); /*--m_Wait_HttpProcess;*/}
  return m_QByteArray;
}
//---------------------------------------  beginGet_FromUrl ---------------------------------------------------------
void  C_Utils_Html::beginGet_FromUrl(const QString &url, const QStringList& rawHeaders)
{ m_Wait_HttpProcess = m_EndWaitHttpProcess;
  m_LastError   = "";
  if (m_pQProgressBar)   {m_position = 0;  m_pQProgressBar->setRange(0,0);  }
  m_QByteArray.clear();
  disconnect(m_pQNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Slot_DataPostFinished(QNetworkReply*)));
  connect(m_pQNetworkAccessManager,    SIGNAL(finished(QNetworkReply*)), this, SLOT(Slot_DataLoadFinished(QNetworkReply*)));
  QNetworkRequest request;
  request.setUrl(QUrl(url));
  for (int i=0; i<rawHeaders.count(); ++i)
      {QString     rawH = rawHeaders[i];
       QStringList list = rawH.split('|');
       request.setRawHeader(list[0].toLatin1(), list[1].toLatin1());
      }
  if (rawHeaders.count()==0) request.setRawHeader("User-Agent", "Free-for-all");
  m_pQNetworkAccessManager->get(request);
}
//--------------------------- Slot_DataLoadFinished -----------------------------------------------------------
void   C_Utils_Html::Slot_DataLoadFinished(QNetworkReply *pQNetworkReply)
{ if (pQNetworkReply->open(QIODevice::ReadOnly))
  if (pQNetworkReply->error()!= QNetworkReply::NoError) m_LastError = pQNetworkReply->errorString();
  if (m_LastError.length()) outMessage( tr("ERREUR : getDataFromUrl() <br> %1 ").arg(m_LastError));
  if (pQNetworkReply->open(QIODevice::ReadOnly))
     {m_QByteArray  = pQNetworkReply->readAll();
      pQNetworkReply->close();
      //CGestIni::Param_UpdateToDisk("/home/ro/array.html", m_QByteArray);   // ok encodage utf8 correct
     }
  disconnect(m_pQNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Slot_DataPostFinished(QNetworkReply*)));
  disconnect(m_pQNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Slot_DataLoadFinished(QNetworkReply*)));
  m_Wait_HttpProcess = 0;
  if (m_pQProgressBar)  {m_pQProgressBar->setRange(0,100); m_pQProgressBar->setValue(0); m_pQProgressBar->reset();}
}

//---------------------------------- getImagesList ---------------------------------------------------------------------------
/*! \brief extract alls images tags from a thml text
 *  \param QString &txt:           text to examine
 *  \param const QString &tag      tag to find
 *  \param const QString attribut  attribut where image name is (to change origin string before image.name )
 *  \param QStringList &imgList    list to fill
 *  \param long from:              position to begin
 *  \return   if negative error or not image tag, if not negative, next position (after tag)
 */

long C_Utils_Html::getImagesList( const QString &txt, const QString &tag, const QString attribut, QStringList &imgList, long from /* = 0 */)
{//<img align=BOTTOM border=0 height=48 name=Image1 src=ccam_view_icn.png width=48 >

 QString fileName;
 long      i   = 0;
 long deb_tag;
 while (from>=0 && from<(long)txt.length())
    {deb_tag     = isThisTagImage(txt, tag, attribut, from , &fileName ); // en entree nameFind = "" car on recherche toutes les ancres
     if (deb_tag>=0)
        {fileName.replace("&gt;",">");
         fileName.replace("&lt;","<");
         fileName.replace("../","_._._/"); fileName.remove("./");fileName.replace("_._._/","../");
         imgList.append(fileName);
         ++i;
        }
    } //end while (from>=0 && from<txt.length())     essai2.html
 imgList.removeDuplicates();
 return i;
}
//---------------------------------- getImagesList ---------------------------------------------------------------------------
/*! \brief extract alls images tags from a html text and make a list with the content of attribut.\
 * tags list as QMap<QString,QString> where key is image name and value is formed as '4122;563'
 * where width is before caracter ';' and height is after caracter ';'.
 *  \param QString &txt:    text to examine
 *  \return  tags list .
 */
QMap<QString,QString> C_Utils_Html::getImagesList( const QString &txt)
{    QMap<QString,QString> imageMap;
     QString fileName = "";
     long deb_tag     = -1;
     long    from     = 0;
     int         w    = 0;
     int         h    = 0;
     while (from>=0 && from<(long)txt.length())
        { deb_tag     = isThisTagImage(txt, "img", "src", from , &fileName, &w, &h ); // en entree nameFind = "" car on recherche toutes les ancres
          if (deb_tag>=0)
             { // imageMap.insertMulti(fileName.trimmed(), QString::number(w)+';'+QString::number(h));
               imageMap.insert(fileName.trimmed(), QString::number(w)+';'+QString::number(h));
             }
        } //end while (from>=0 && from<txt.length())     essai2.html
    return imageMap;
}
//------------------------------------------ isThisTagImage ------------------------------------------------------
/*! \brief analyse a tag and if a image tag return it's url in (QString *fileName if not zero)
 *  \param QString &txt:           text to examine
 *  \param const QString &tag      tag to find
 *  \param const QString attribut  attribut where image name is (to change origin string before image.name )
 *  \param long from:              position to begin
 *  \param QString *fileName:      if not zero, image url is returned in the QString pointed
 *  \param int *w:                 if not zero, image width is returned in the int pointed
 *  \param int *h:                 if not zero, image height is returned in the int pointed
 *  \return   if negative: error or not image tag, if not negative: next position (after tag)
 */
// <img align=BOTTOM border=0 height=48 name=Image1 src=ccam_view_icn.png width=48 >
long C_Utils_Html::isThisTagImage(const QString &txt, const QString &tag, const QString attribut, long &from , QString *fileName /* = 0 */ , int *w /* = 0 */, int *h /* = 0 */)
{long deb_file;
 long deb_tag;
 long end_tag;
 if (w) *w = 0;    // si les indications de taille ne sont pas presentes dans le html on retourne 0
 if (h) *h = 0;    // si les indications de taille ne sont pas presentes dans le html on retourne 0
 //char *pt_deb;
 //char *pt_end;
 //........... rechercher debut du tag .........................
 from     = txt.indexOf(QString("<")+tag+" " , from, Qt::CaseInsensitive);   // false pour case insensitive
 if (from==-1)        {from = txt.length();        return -1;}
 deb_tag  = from;
 //........... rechercher fin du tag ...........................
 from    += tag.length()+2;    // passer < et le tag et l'espace
 end_tag  = txt.indexOf('>', from);
 if (end_tag==-1)                                  return -3;                    // erreur  -3 de syntaxe fin de tag '>' non trouvee

 int pos;
 //........... rechercher src ..................................
 if (fileName && (pos = txt.indexOf(attribut, from, Qt::CaseInsensitive)) != -1 && pos<end_tag)
    { //........... extraire nom du fichier image ...................
      pos += attribut.length();
      while (pos<end_tag && (txt.at(pos)==' ' || txt.at(pos)=='\t' || txt.at(pos)=='='))  ++pos;
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
 if (w && (pos = txt.indexOf("width", from, Qt::CaseInsensitive)) != -1 && pos<end_tag)
    { //........... extraire nom du fichier image ...................
      pos += 5;
      while (pos<end_tag && (txt.at(from)==' ' || txt.at(from)=='\t'|| txt.at(pos)=='\"' || txt.at(pos)=='='))  ++pos;
      deb_file = pos;
      while (pos<end_tag && txt.at(pos)!=' ' && txt.at(pos)!='\t' && txt.at(pos)!='\"' && txt.at(pos)!=';')     ++pos;
      *w = txt.mid(deb_file, pos - deb_file).toInt();
    }
 //........... rechercher height ..................................
 if (h && (pos = txt.indexOf("height", from, Qt::CaseInsensitive)) != -1 && pos<end_tag)
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

//---------------------------------- setTagAttributeValue ---------------------------------------------------------------------------
/*! \brief extract alls tags from a html text
 *  \param QString &txt:                        text to examine
 *  \param const QString &tag                   tag to find
 *  \param const QString attribut               attribut where is data value to set
 *  \param const QString value                  value to place
 *  \param C_Utils_Html::AttributChange flag    if zero the entire data of attribut will be replaced by contents of value else just to change origin string before image.name)
 *  \param long  from:                          position to begin
 *  \return   if negative error or not image tag, if not negative, next position (after tag)
 */
long C_Utils_Html::setTagAttributeValue( QString &txt,
                                         const QString &tag,
                                         const QString &attribut,
                                         const QString &value,
                                         C_Utils_Html::AttributChange flag /* = 0 */,
                                         long from /* = 0 */
                                       )
{return getTagList(txt,tag,0, from , value ,attribut , flag);
}

//---------------------------------- getTagList ---------------------------------------------------------------------------
/*! \brief extract alls tags from a html text
 *  \param QString &txt:               text to examine
 *  \param const QString &tag          tag to find
 *  \param QStringList &tagList        list to fill
 *  \param long from:                  position to begin
 *  \param const QString value         is just for img tag (to change origin string before image.name )
 *  \param const QString attribut      attribut where image name is (to change origin string before image.name )
 *  \param C_Utils_Html::AttributChange flag   if one the entire data of attribut will be replaced by contents of value else just to change origin string before image.name)
 *  \return   if negative error or not image tag, if not negative, next position (after tag)
 */
long C_Utils_Html::getTagList( QString &txt,
                               const QString &tag,
                               QStringList *tagList,
                               long from /* = 0 */,
                               const QString &value /*=""*/,
                               const QString &attribut /*=""*/ ,
                               C_Utils_Html::AttributChange flag /* = 0 */)
{//<img align=BOTTOM border=0 height=48 name=Image1 src=ccam_view_icn.png width=48 >

 QString tagData;
 long      i   = 0;
 long deb_tag;
 while (from>=0 && from<(long)txt.length())
    {deb_tag     = indexOfTag(txt, tag, from , value, attribut, flag);
     if (deb_tag>=0)
        {tagData = txt.mid(deb_tag,from-deb_tag);
         tagData.replace("&gt;",">");
         tagData.replace("&lt;","<");
         if (tagList) tagList->append(tagData);
         ++i;
        }
    } //end while (from>=0 && from<txt.length())     essai2.html
 if (tagList) tagList->removeDuplicates();
 return i;
}
//---------------------------------- nextTagContentImage [static] ---------------------------------------------------------------------------
/*! \brief find image tag as '<img src="image_url" width="120" height="180" />'      \
 *         and return positions start and end of tag content
 *  if no tag find -1 is returned otherwise end of first tag content is returned (ready for next find)
 *  if image height or image with attributs don't exists they will be added.
 *  \param html:             html datas where find tag image
 *  \param deb_t             position of first tag content caracter will be returned exple <img
 *  \param end_t             position just after last tag. At first time this value must be zero,\
 *                           after it is position just after tag content, so you can loop while -1 with it.
 *  \return   position just after tag content (ready for next find), otherwise  -1 if no tag found.
 */
int C_Utils_Html::nextTagContentImage(const QString &html,int &deb_t, int &end_t)
{ deb_t = html.indexOf("<img ",end_t);
  if (deb_t==-1)               return -1;
  int end_1 = html.indexOf(">", deb_t+5);   // il faut evaluer les deux notations
  int end_2 = html.indexOf("/>",deb_t+5);   // il faut evaluer les deux notations
  if (end_1==-1 && end_2==-1 ) return -1;
  if      (end_1==-1) end_t =  end_2;
  else if (end_2==-1) end_t =  end_1;
  else                end_t =  qMin(end_1,end_2);
  return                       end_t;
}
//---------------------------------- changeImageNameAndSize [static] ---------------------------------------------------------------------------
/*! \brief find image tag as <img src="image_url" width="120" height="180" /> whith a image Url attribut giving in imput \
 *  and replace image Url information by a new url, image with by new width, image height by a new height \
 *  if image height or image with attributs don't exists they will be added.
 *  \param html:              html datas where doing changes
 *  \param image_url          image Url to find.
 *  \param new_Url            new url to place.
 *  \param new_width          new width to place
 *  \param new_height         new height to place
 *  \return   nb of image Url attribut giving in imput found   0 otherwise
 */
int C_Utils_Html::changeImageNameAndSize(QString &html, const QString &image_url, const QString &new_Url, int new_width, int new_height )
{
    return changeImageNameAndSize(html, image_url, new_Url, QString::number(new_width), QString::number(new_height) );
}

//---------------------------------- changeImageNameAndSize [static] ---------------------------------------------------------------------------
/*! \brief find image tag as <img src="image_url" width="120" height="180" /> whith a image Url attribut giving in imput \
 *  and replace image Url information by a new url, image with by new width, image height by a new height \
 *  if image height or image with attributs don't exists they will be added.
 *  \param html:              html datas where doing changes
 *  \param image_url          image Url to find.
 *  \param new_Url            new url to place.
 *  \param new_width          new width to place
 *  \param new_height         new height to place
 *  \return   nb of image Url attribut giving in imput found   0 otherwise
 */
int C_Utils_Html::changeImageNameAndSize(QString &html, const QString &image_url, const QString &new_Url, const QString &new_width, const QString &new_height )
{  int deb_t = -1;
   int end_t = 0;
   // <img src="wcf/icon/loginS.png" alt="" id="loginButtonImage" />
   int nb    = 0;
  while ( nextTagContentImage(html, deb_t, end_t) != -1)   // si end_t != -1  pointe a la fin du contenu donc pret pour le tag suivant
   {   int deb = deb_t;   // relever les positions de debut avant de passer au suivant
       int end = end_t;   // relever les positions de fin   avant de passer au suivant
       /*
       //.................. METHODE NE RESPECTANT LE TAG D'ORIGINE .................................
       //                   efface les attributs autres que 'src'  'width'  'height'
       //............. verifier le nom ........................
       QString tag_interior  = html.mid(deb+1, end-deb-1);
       int deb_value_name    = -1;
       int end_value_name    = -1;
       QString value_name    = "";
       long pos_attribut     = 4;    // passer '<img '
            pos_attribut     = indexOfAttribut( tag_interior,"src",   pos_attribut, tag_interior.length() , &deb_value_name, &end_value_name, &value_name);
       if (pos_attribut==-1)                                continue;
       if (image_url != value_name.remove('\"').trimmed() ) continue;

       int ini_interior_len = end-deb;
       tag_interior = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"").arg(new_Url,new_width,new_height);
       if (html[end_t] == '>') tag_interior+='/';   // si '>'    on rajoute '/' pour faire '/>'
       html                 = html.remove(deb, ini_interior_len);      // on vire l'ancien
       html                 = html.insert(deb, tag_interior);          // on met le nouveau et on se casse
       */


       //.................. METHODE RESPECTANT LE TAG D'ORIGINE .................................
       //                   conserve les attributs autres que 'src'  'width'  'height'
       //........ recherche d'un tag image avec le meme nom 'image_url'.........................................
       QString tag_interior  = html.mid(deb+1, end-deb-1);
       int tag_interior_len  = tag_interior.length();
       int ini_interior_len  = tag_interior_len;
       int deb_value_name    = -1;
       int end_value_name    = -1;
       QString value_name    = "";
       long pos_attribut     = 4;    // passer '<img '
            pos_attribut     = indexOfAttribut( tag_interior,"src",   pos_attribut, tag_interior_len , &deb_value_name, &end_value_name,&value_name);
       if (pos_attribut==-1)                                continue;
       if (image_url != value_name.remove('\"').trimmed() ) continue;

       //........ si trouve completer les recherches avec les eventuel attributs de largeur et hauteur.........................................
       int deb_value_width   = -1;
       int end_value_width   = -1;
       QString value_width   = "";
       pos_attribut          = 4;    // passer '<img '
       pos_attribut          = indexOfAttribut( tag_interior,"width",  pos_attribut, tag_interior_len , &deb_value_width, &end_value_width,&value_width);
       int deb_value_height  = -1;
       int end_value_height  = -1;
       QString value_height  = "";
       pos_attribut          = 4;    // passer '<img '
       pos_attribut          = indexOfAttribut( tag_interior,"height", pos_attribut, tag_interior_len , &deb_value_height, &end_value_height,&value_height);
       //........ si trouve remplacer dans le texte d'origine les valeurs.........................................
       int delta             = 0;
       tag_interior          = tag_interior.remove(deb_value_name,end_value_name-deb_value_name);
       tag_interior          = tag_interior.insert(deb_value_name,"\""+new_Url+"\"");  // on encadre avec les guillemets (permet des espaces)
       delta                 = tag_interior.length()-tag_interior_len;
       end_t                += delta;
       //.......... eventuellement reajuster les positions des pointeurs ..........................................................
       if ( value_width.length() )
          { if (deb_value_width > deb_value_name) deb_value_width += delta;    // si apres faut appliquer le changement de position
            if (end_value_width > deb_value_name) end_value_width += delta;    // si apres faut appliquer le changement de position
          }
       if ( value_height.length() )
          { if (deb_value_height > deb_value_name) deb_value_height += delta;    // si apres faut appliquer le changement de position
            if (end_value_height > deb_value_name) end_value_height += delta;    // si apres faut appliquer le changement de position
          }
       tag_interior_len      = tag_interior.length();
       //.......... traiter l'attribut width ......................................................................................
       if ( value_width.length() )
          { tag_interior     = tag_interior.remove(deb_value_width,end_value_width-deb_value_width);
            tag_interior     = tag_interior.insert(deb_value_width,"\""+new_width+"\"");  // on encadre avec les guillemets (permet des espaces)
            delta            = tag_interior.length()-tag_interior_len;                    // calculer le delta pour ajuster les positions situees apres
            end_t           += delta;
            tag_interior_len = tag_interior.length();                                     // reactualiser la longueur
            if ( value_height.length() )
               { if (deb_value_height > deb_value_name) deb_value_height += delta;    // si apres faut appliquer le changement de position
                 if (end_value_height > deb_value_name) end_value_height += delta;    // si apres faut appliquer le changement de position
               }
          }
       else
          { tag_interior     += "width=\""+new_width+"\"";    // la on rajoute a la fin donc pas de reajustement des pointeurs de position
            tag_interior_len  = tag_interior.length();        // reactualiser la longueur
          }
       //.......... traiter l'attribut height .......................................................................
       if ( value_height.length() )
          { tag_interior     = tag_interior.remove(deb_value_height,end_value_height-deb_value_height);
            tag_interior     = tag_interior.insert(deb_value_height,"\""+new_height+"\"");  // on encadre avec les guillemets (permet des espaces)
            delta            = tag_interior.length()-tag_interior_len;                      // calculer le delta pour ajuster les positions situees apres
            end_t           += delta;
            tag_interior_len = tag_interior.length();                                       // reactualiser la longueur
            if ( value_width.length() )
               { if (deb_value_width > deb_value_name) deb_value_width += delta;    // si apres faut appliquer le changement de position
                 if (end_value_width > deb_value_name) end_value_width += delta;    // si apres faut appliquer le changement de position
               }
          }
       else
          { tag_interior     += "height=\""+new_height+"\"";    // la on rajoute a la fin donc pas de reajustement des pointeurs de position
            tag_interior_len  = tag_interior.length();          // reactualiser la longueur
          }
       //........ a ce stade tag_interior devrait etre pret a remplacer l'ancien.........................................
       html = html.remove(deb+1, ini_interior_len);      // on vire l'ancien
       html = html.insert(deb+1, tag_interior);          // on met le nouveau et on se casse
      ++ nb;

   } // end while ( (deb_t=html.indexOf("<img ",end_t))!=-1 && (end_t=html.indexOf(">",deb_t+5))!=-1)
   return nb;
}

//------------------------------------------ indexOfTag ------------------------------------------------------
/*! \brief analyse a tag and if a image tag return it's url in (QString *fileName if not zero)
 *  \param QString &txt:               text to examine
 *  \param const QString &tag          tag to find
 *  \param long from:                  position to begin (changed in position after tag at function end)
 *  \param const QString value         is just for img tag (to change origin string before image.name )
 *  \param const QString attribut      attribut where image name is (to change origin string before image.name )
 *  \param C_Utils_Html::AttributChange flag          if one the entire data of attribut will be replaced by contents of value else just to change origin string before image.name)
 *  \return   if negative: error or not tag, if not negative: start position of the tag.
 */
// <img align=BOTTOM border=0 height=48 name=Image1 src=ccam_view_icn.png width=48 >
long C_Utils_Html::indexOfTag(QString &txt,
                              const QString &tag,
                              long &from ,
                              const QString &value /*=""*/,
                              const QString &attribut  /*=""*/,
                              C_Utils_Html::AttributChange flag         /*=0 */)
{long deb_file;
 long deb_tag;
 long end_tag;
 QString strTag   = "";
 //........... rechercher debut du tag .........................
 from     = txt.indexOf(QString("<")+tag+" " , from, Qt::CaseInsensitive);   // false pour case insensitive
 if (from==-1)        {from = txt.length();        return -1;}
 deb_tag  = from;
 //........... rechercher fin du tag ...........................
 from    += tag.length()+1;    // passer < et le tag et l'espace
 end_tag  = txt.indexOf('>', from);
 if (end_tag==-1)    {outMessage(tr("ERREUR in <b>'indexOfTag'<b> end symbol <b>'&gt;'</b> not found"), __FILE__, __LINE__);  return -3;}                    // erreur  -3 de syntaxe fin de tag '>' non trouvee
 ++end_tag;
 //................... filtrer selon tag (faut pas chipoter quand on trie) ................
 //                    traiter le cas particulier du changement du path de l'image

 if (value.length()==0)   return deb_tag;        // rien a relocaliser
 int           pos = -1;
 QString  fileName = "";
 QString  new_Name = "";
 int         delta = 0;
 strTag            = txt.mid(deb_tag, end_tag-deb_tag);
 if (    (pos = indexOfAttribut(txt, attribut, from, end_tag))    != -1 && pos<end_tag  )
    { //........... extraire nom du fichier image ...................
      pos += attribut.length();
      // outMessage(tr(" tag : ")+ strTag, __FILE__, __LINE__);
      while (pos<end_tag && (txt.at(pos)==' ' || txt.at(pos)=='\t' || txt.at(pos)=='='))  ++pos;
      if (txt.at(pos)=='\"')
         {deb_file = pos+1;
          pos      = C_Utils_Log::jumpEnclosedsSymbols(txt, pos, end_tag, "\"" )-1;   //-1 car faut pointer avant le guillemet
         }
      else
         { deb_file = pos;
           while (pos<end_tag && txt.at(pos)!=' ' && txt.at(pos)!='\t' && txt.at(pos)!='\"' && txt.at(pos)!=';')     ++pos;
         }
      fileName = txt.mid(deb_file, pos - deb_file);                  // recuperer l'ancien nom
      txt.remove(deb_file, pos - deb_file);                           // virer le nom

      if (flag==C_Utils_Html::SetAllJustDir)   new_Name = QFileInfo(fileName).fileName().prepend(value);  // calculer le nouveau nom de fichier
      else                                     new_Name = value;                                          // calculer la nouvelle valeur
      txt.insert(deb_file,new_Name);                                 // le mettre a la place de l'autre
      delta    = new_Name.length()-fileName.length();                // calcule du delta du nil
      // outMessage(tr(".        change : %1 in %2 ").arg(fileName, new_Name));
    }
 else
    { // outMessage(tr("Attribut <b>'%1'</b> not found in tag %3").arg(attribut,strTag), __FILE__, __LINE__);  return -4;
    }
 from = end_tag + delta;
 return deb_tag;
}

//------------------------------------------ indexOfAttribut ------------------------------------------------------
/*! \brief find the next attribut whith specific name in a tag
 *  \param QString &txt:             text to examine
 *  \param const QString &attribut   attribut to find
 *  \param long from:                position to begin
 *  \param long end:                 position to ended
 *  \param deb_value:                if not zero it will be returned value position after symbol '='
 *  \param end_value:                if not zero it will be returned end value position (after symbol '=' if enclosed by)
 *  \param pValue:       QString     if not zero it will be returned value '='
 *  \return   if negative: error or not tag, if not negative: start position of the attribut.
 */
// <img align=BOTTOM border=0 height=48 name=Image1 src=ccam_view_icn.png width="48" >
long C_Utils_Html::indexOfAttribut(QString &txt,
                                   const QString &attribut,
                                   long &from,
                                   long   end,
                                   int *deb_value  /*=0*/,
                                   int *end_value  /*=0*/,
                                   QString *pValue /*="0""*/)
{while(from<end)
    {QString toTest  = attribut.toUpper();
     long lenToTest  = toTest.length();
     if (txt.at(from)=='\"')
        {   from = C_Utils_Log::jumpEnclosedsSymbols(txt, from, end, "\"");
        }
     else if (txt.at(from)=='\\')
        {   from +=2;
        }
     else if ( txt.mid(from,lenToTest).toUpper()==toTest )
        {  //............ verifier si signe egal apres l'attribut .................
           int  deb_egal  = CGestIni::gotoNextNotBlank(txt, from+lenToTest);
           if (txt.at(deb_egal)=='=')
              { int deb_val = CGestIni::gotoNextNotBlank(txt, deb_egal+1);
                if (deb_value) *deb_value = deb_val;
                if (end_value||pValue)
                   { int end_val = -1;
                     if (txt.at(deb_val)=='\"')
                        { end_val     = C_Utils_Log::jumpEnclosedsSymbols(txt, deb_val, end, "\"" );   //pointe apres le guillemet
                        }
                     else
                        { end_val = deb_val;
                          while ( end_val<end  && txt.at(end_val)!=' ' && txt.at(end_val)!='\t' && txt.at(end_val)!='\"' && txt.at(end_val)!=';' && txt.at(end_val)!='>'&& txt.at(end_val)!='<')  ++end_val;
                        }
                     if (end_value) *end_value  = end_val;
                     if (pValue)    *pValue = txt.mid(deb_val, end_val-deb_val );
                   }
                return from;
              }
        }
     ++from;
    }
 return -1;
}
//----------------------------- font_size_average [static] -----------------------------------
/*! \brief parse all the html and return font size average
 *  \param QString &html:             html to examine
 *  \return   double font size average 0.0 if no information about.
 */
double C_Utils_Html::font_size_average(const QString &html)
{   // "font-size:6.92pt;"

  int         pos    = 0;
  int         end    = html.length() -10;
  double   average   = 0.0;
  int           nb   = 0;
  QString attribut   = "";
  QString     truc   = "";
  while ( pos<end )
  {   pos = html.indexOf("<span ",pos);
      if   ( pos==-1 )
           { if (nb) return average/ (double) nb;
             else    return 0.0;
           }
      else {  pos += 6;
              while ( pos<end && html[pos]!='>' )
                    { truc = html.mid(pos, 15);
                      if ( html[pos]=='f' && html[pos+1]=='o' && (attribut = html.mid(pos,10))=="font-size:" )
                         {  QString value  = "";
                            pos           += 10;
                            while ( pos<end && (   html[pos]==' ' || html[pos]==160   || html[pos]=='\t') ) ++pos;
                            while ( pos<end && ( ( html[pos]>='0' && html[pos]<='9' ) || html[pos]=='.' ) )
                                  { value += html[pos];
                                    ++pos;
                                  }
                            if ( value.length() )
                               { ++ nb;
                                 average += value.toDouble();
                               }
                         } // if ( html[pos]=='f' && html[pos+1]=='o' )
                      else
                         {  ++pos;
                         }
                    }  // while ( pos<end && html[pos]!='>' )
           }
  } //  while ( pos<end )
  if (nb) return average/ (double) nb;
  else    return 0.0;
}

//----------------------------- removeHtmlTagsHeader [static] -----------------------------------
//  lors de l'edition des menus avec contenu html
//  un <p> de debut html et </p> de fin html parasitent
//  et creent un retour ligne non desire ==> le virer
/*! \brief   remove HtmlTags Header
 *  \param html where remove <html><head><meta name="qrichtext" content="1" /></head><body style="font-size:10pt;font-family:Sans Serif"><p>
 *                           </p></body></html>
 * \return modified html
*/
QString C_Utils_Html::removeHtmlTagsHeader( QString &html )
{   //...... on recupere que ce qui est entre <body xxxx>  et </body>
    //       ce qui revient a virer '<html><head><meta name="qrichtext" content="1" /></head><body xxx>'  et
    //                              '</body></html>' )
    int deb = html.indexOf("<body",0,Qt::CaseInsensitive);
    if (deb==-1)                return html;
    deb = (html.indexOf(">",deb+1));
    if (deb==-1)                return html;
    ++deb;
    int end = html.indexOf("</body>",0,Qt::CaseInsensitive);
    if (end==-1)                return html;
    html = html.mid(deb,end-deb);
    //...... on cherche un premier <p> ou <p xxxx> .........
    //       qui se situe avant tous les autres tags html
    //       ( c'est lui le parasite a tuer )
    int p = 0;
    while (p<html.length() && html[p] != '<') ++p;         // recherchr premier tag html
    if (p==html.length())       return html;
    deb = -1;
    QString tag = html.mid(p, 3).toLower() ;
    if ( tag == "<p>" )                                    // <p>
       { deb = p+3;
       }
    else if ( tag == "<p ")
       { while (p<html.length() && html[p] != '>') ++p;    // <p style=" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">
         if (p==html.length())  return html;
         deb = p+1;
       }
    if (deb==-1)                return html;              // cassos car ce premier tag html n'est pas un <p> ou <p xxx>
    //...... on cherche le </p> de fin correspondant.........
    int stack        = 1;
    end              = deb;    // on est apres le premier <p> ou <p xxxxxxx>
    while (stack && end < html.length())
    {   if ( html[end]!='<') {++end; continue;}
        tag = html.mid(p, 3).toLower() ;
        if (tag == "<p>" || tag == "<p " )
            {  ++   stack;
               if (html[end+2]!='>') while (end<html.length() && html[end] != '>') ++end; //  <p style=" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">
               else end += 3;                                                             //  <p>
            }
        else if (html.mid(end, 4).toLower() == "</p>")
            {  --   stack;
               end += 4;
            }
        else
            {  ++end;
            }
    }
    p    = end;
    end -= 4;    // - fermant_len pour pointer avant le fermant
    //...... on verifie que le </p> de fin correspondant.........
    //       est en fin d'html (y en a pas d'autres apres)
    stack        = 0;
    while (p < html.length())
    {   if ( html[p]!='<') {++p; continue;}
        QString tag = html.mid(p, 3).toLower() ;
        if ( tag == "<p>" || tag == "<p " )
            {  ++ stack;  // y a en un autre apres
               break;
            }
        else
            {  ++p;
            }
    }
    if (stack==0) html = html.mid(deb,end-deb);  // si </p> est bien le dernier alors on coupe
    return html;
}
//------------------------ htmlFind ------------------------------------------
/*! \brief find a text patern in a html document
 *  \param text :      const QString & html document where find text patern
 *  \param patern :    QString & text patern to find. If text patern == "FinDeLigne" or "EOL" the first end line tag is searched
 *  \param pos :       long &pos position where begin action
 *  \param debMotif :  long * pointer on long in which (if not egal zero) position after end patern will be returned (-1 if not found)
 *  \return -1 if not found, patern position if found.
*/
long C_Utils_Html::htmlFind(const QString &text, const QString &_patern, int pos, int *debMotif /*=0*/, C_Utils_Html::flags flags /* = C_Utils_Html::withoutSpaces */)
{long len = text.length();
 if (len==0)        return -1;
 if (debMotif) *debMotif = -1;
 QString patern          = _patern;
 // QString truc            = "";
 //.............. si il faut juste chercher la fin de la ligne ..................................
 if (patern.toUpper()=="$TOEND")
    { pos = text.length();
      if (debMotif) *debMotif = pos;
      return pos;
    }
 else if (patern.toUpper()=="$FROMSTART")
    { pos = 0;
      if (debMotif) *debMotif = pos;
      return pos;
    }
 else if (patern=="FinDeLigne"||patern=="EOL")
    { while (pos<len)
         { // truc = text.mid(pos, 10);
           if (text[pos] != '<' ) {++pos; continue;}
           QChar c = text[pos+1].toLower();  // premier caractere apres < (comme mis en majuscule et teste souvent on le fait ici)
           if ( c == 'b' )
              {   int l = 0;
                  if (text.mid(pos+2,2).toLower()=="r>")           // <br>
                     { l = 4;
                     }
                  else if (text.mid(pos+2,3).toLower()=="r/>")     // <br/>
                     { l = 5;
                     }
                  else if (text.mid(pos+2,4).toLower()=="r />")    // <br />
                     { l = 6;
                     }
                  else            {++pos; continue;}
                  if (debMotif) *debMotif = pos;
                  return         pos+l;
               }
           else if ( c=='p' )               // <p>   ou <p xxxxx>
               {   int l = 0;
                   if (text[pos+2]=='>')           // <p>
                      { l = 3;
                      }
                    else if (text[pos+2]==' ')     // <p xxxx>
                      { int end = text.indexOf(">",pos+2);
                        if (end==-1) {++pos; continue;}    // '>' de fin non trouve (erreur dans le htmls>
                        ++end;           //1 pour passer le '>'
                        l = end - pos;
                      }
                   else              {++pos; continue;}
                   if (debMotif) *debMotif = pos;
                   return         pos+l;
               }
            ++pos;
           }  // end while (pos<len)
     if (debMotif) *debMotif = pos;
     return pos;         // si pas de <br> ni <p> alors la fin de ligne est forcemeent la fin du texte
    }

 //.............. nettoyer le motif a comparer de tous les trucs non signifiant .................
 C_Utils_Html::htmlToAscii(patern);
 if (flags==C_Utils_Html::withoutSpaces)
    { patern =  patern.remove(' ');
      patern = patern.remove(C_Utils_Html::Utf8_Insecable);
    }
 patern.remove('\t');
 patern.remove('\r');
 patern.remove('\n');
 int   carToCompareInMotif = 0;
 int   len_motif           =  patern.length();
 while (pos<len)
   { // truc    = text.mid(pos, 10);
     if (text[pos]=='<')
        { int l = 0;
          QString tag = text.mid(pos,6).toLower();
          if (tag.left(4)=="<br>")
             {  l = 4;  // variante  <br>
             }
          else if (tag.left(5)=="<br/>")
             {  l = 5;  // variante  <br/>
             }
          else if (tag =="<br />")
             {  l = 6;  // variante  <br>
             }
          else if (tag =="<p>")
             {  l = 3;  // variante  <br>
             }
          else if (tag.left(3) =="<p ")
             { int end = text.indexOf(">",pos+2);
               if (end==-1) {++pos; continue;}    // '>' de fin non trouve (erreur dans le htmls>
               ++end;           //1 pour passer le '>'
               l = end - pos;
             }
         //.......... si tag  ou  la comparaison s'arrete car plus valide ...............................
         //           CAD on accepte des tags (exple couleur, fonte etc.) au milieu du texte a comparer
         //           ces tag doivent etre sautes sauf ceux de fin de ligne
         if (l)
            { pos                    += l;
              carToCompareInMotif     = 0;
              if (debMotif) *debMotif = -1;
            }
         else
         //.............. le tag est a sauter ...........................................................
            { pos = ifHtmlTagJumpIt(text,pos);          // sauter tag html
            }
        }
    else
        {QChar car  = text.at(pos);
         QChar ins  = QChar(196,160);
         if (car=='\r'||car=='\n'||car=='\t')                                      // sauter caracteres non significatifs || car=='\t'
            { ++pos;
            }
         else if ( flags & C_Utils_Html::withoutSpaces && car== 160)
            { ++pos;
            }
         else if ( flags & C_Utils_Html::withoutSpaces && car== ins)
            { ++pos;
            }
         else if ( flags & C_Utils_Html::withoutSpaces && car==' ')                      // sauter les caracteres non significatifs (ici les espaces ne sont pas evalues)
            { ++pos;
            }
         else //................... la on peut comparer ...............................................
            {
                 if ( debMotif && carToCompareInMotif==0 ) *debMotif = pos;
                 if ( car=='&' )   //............. convertir les caracteres codes .........................
                    { if      (text.at(pos+1)=='g'&&text.at(pos+2)=='t'&&text.at(pos+3)==';')                                              {car = '>';          pos += 4;}
                      else if (text.at(pos+1)=='l'&&text.at(pos+2)=='t'&&text.at(pos+3)==';')                                              {car = '<';          pos += 4;}
                      else if (text.at(pos+1)=='a'&&text.at(pos+2)=='m'&&text.at(pos+3)=='p'&&text.at(pos+4)==';')                         {car = '&';          pos += 5;}
                      else if (text.at(pos+1)=='n'&&text.at(pos+2)=='b'&&text.at(pos+3)=='s'&&text.at(pos+4)=='p'&& text.at(pos+5)==';')   {car = ' ';          pos += 6;}
                      else if (text.at(pos+1)=='q'&&text.at(pos+2)=='u'&&text.at(pos+3)=='o'&&text.at(pos+4)=='t'&& text.at(pos+5)==';')   {car = '\"';         pos += 6;}
                      else                                                                                                                 {car = text.at(pos); pos += 1;}
                    }
                 else
                    { ++pos;
                    }
                 if ( (car==160||car==' ') && (patern[carToCompareInMotif]==160||patern[carToCompareInMotif]==' '))
                    { ++carToCompareInMotif;
                      if (carToCompareInMotif >= len_motif) return pos;
                    }
                 else if ( car == patern[carToCompareInMotif] )
                    { ++carToCompareInMotif;
                      if (carToCompareInMotif >= len_motif) return pos;
                    }
                 else
                    { carToCompareInMotif     = 0;
                      if (debMotif) *debMotif = -1;
                    }
                }
        }
    }// end while (*pt && carToCompareInMotif < len_motif)
 return -1;
}

//------------------------------------------ ifHtmlTagJumpIt ------------------------------------------------------
/*! \brief jump a html tag in a html document
 *  \param text :      const QString & html document
 *  \param pos :       long &pos position where begin action must be just before the < start symbol tag
 *  \return position just after > end tag (if not found return position after last caracter of the text).
*/
long C_Utils_Html::ifHtmlTagJumpIt(const QString &text, int pos)
{
 if (text.at(pos)=='<')
    { pos = text.indexOf(">", pos+1);
      if (pos != -1) return pos+1;
    }
 return text.length();
}
//----------------------------- setZoom [static]-----------------------------------
/*! \brief multiply all values xx from 'font-size: xx pt' paterns by a factor zoom
 *  \param  text : where found and multiply paterns
 *  \param  zoom : multiplication factor
*/
void  C_Utils_Html::setZoom( QString &text, double zoom)
{ // style="font-size:
  int end         = 0;
  int pos         = 0;
  double fpt      = 0;
  int len         = 0;
  QString s_zoom  = "";
  while ( (pos = text.indexOf("font-size:",end)) != -1)
        {  pos += 10;
           end  = text.indexOf("pt",pos);
           if ( end == -1 )  continue;
           len = end-pos;
           fpt = text.mid(pos,len).toDouble();
           fpt = qMax(4.0,fpt*zoom);
           text.remove(pos,len);
           s_zoom.setNum(fpt,'f',2);
           text.insert(pos, s_zoom);
        }
}
//----------------------------- zoom_All_ImageTags [static]-----------------------------------
/*! \brief multiply all values xx from 'font-size: xx pt' paterns by a factor zoom
 *  \param  html : where found and multiply image tags
 *  \param  zoom : multiplication factor
 *  \param  maxWidth  : if superior, width is adjusted to maximum width
 *  \param  maxHeight : if superior, height is adjusted to maximum height
 *  \param imageCache : image cache where find size informations if they are not present in tag
 *  \param imagePath : image path where find size informations if they are not present in tag
*/
void  C_Utils_Html::zoom_All_ImagesTags(QString &html, double zoom, double maxWidth, double maxHeight, const QMap<QString, QPixmap> *imageCache /* = 0 */ , const QString &pathImage /* = ""*/)
{
    QMap<QString,QString> imgMap = C_Utils_Html::getImagesList( html );
    QMapIterator<QString,QString> it(imgMap);
    while (it.hasNext())
      { it.next();

        QString imgName   = it.key() ;
        QString sizeInfo  = it.value() ;
        int p             = sizeInfo.indexOf(';');
        if (p==-1) continue;
        double width      = sizeInfo.left(p).toDouble();
        double height     = sizeInfo.mid(p+1).toDouble();
        if ( width==0 || height==0 )
           {
             if ( imageCache &&  imageCache->contains( imgName ) )
                { QPixmap pm = imageCache->value( imgName );
                  width      = pm.width();
                  height     = pm.height();
                }
             else if (QFile::exists(pathImage+imgName))
                { QPixmap pm = QPixmap(pathImage+imgName);
                  width      = pm.width();
                  height     = pm.height();
                }
             else
                { width      = maxWidth/3;
                  height     = maxHeight/3;
                }
           }
        //........... on ajuste proportionnellement .............
        if ( width > maxWidth && height > maxHeight )
           { double divRatio = height/maxHeight;
             if (width/maxWidth > divRatio) divRatio = width/maxWidth;
             width  = width/divRatio;
             height = height/divRatio;
           }
        else if ( width > maxWidth )
          { double divRatio = width/maxWidth;
            width  = width/divRatio;
            height = height/divRatio;
          }
        else if ( height > maxHeight )
          { double divRatio = height/maxWidth;
            width  = width/divRatio;
            height = height/divRatio;
          }

        width  = width*zoom;
        height = height*zoom;
        C_Utils_Html::changeImageNameAndSize(html, imgName, imgName, QString::number(width,'f',2), QString::number(height,'f',2) );
      } //endwhile (it.hasNext())
}
//-------------------------------------------------- htmltoLatin1 -----------------------------------------------------------------
/*! \brief html text to text without html tag
 *  \param text :      const QString & html document
 *  \param mode :      conversion mode  StrictASCII  C_Utils_Html::KeepBR('\\n\\n' --> '<p>' and '\\n' --> '<br>')
 *  \return text modified.
*/
QString C_Utils_Html::htmlToAscii(QString &text, C_Utils_Html::flags mode /* = C_Utils_Html::NoMode */)
{
 text.replace("<br>","\n");
 text.replace("<br />","\n");
 text.replace("&nbsp;"," ");
 text.replace("<br />","\n");
 text.replace("</p>","\n\n");
 text.replace("<p>","\n\n");
 text.replace("<li>","\t");
 text.replace("</li>","\n");
 text.replace("</ul>","\n");
 text.replace("</div>","\n");
 text.replace("</div>","\n");
 text.replace("&quot;","\"");
 text.replace("&amp;","&");

 int posDeb = 0;
 int posEnd = 0;
 do {if ( (posDeb = text.indexOf("<", posDeb)) != -1)
        {if ( (posEnd =  text.indexOf(">", posDeb+1)) != -1) text.remove(posDeb, posEnd-posDeb+1);
         else return QString (QObject::tr("CHtmlTools::HtmltoLatin1() Erreur de syntaxe tag html non ferme") );
        }
     else break;
    } while (1);
 text.replace("&lt;","<");
 text.replace("&gt;",">");
 if (mode & C_Utils_Html::KeepBR) text.replace("\n\n","<p>");
 if (mode & C_Utils_Html::KeepBR) text.replace("\n",  "<br>");
 return    text;
}
//-------------------------------------------------- unProtectSymbolByAntiSlash -----------------------------------------------------------------
/*! \brief unprotect a caracter by antislash in a text (if already protected nothing append)
 *  \param text :      const QString & html document
 *  \return text modified
*/
QString C_Utils_Html::unProtectSymbolByAntiSlash(const QString &text)
{   QString result = text;
    result     = result.replace("\\\\","-*@*-");
    result.remove("\\");
    result     = result.replace("-*@*-","\\\\");
    return result;
}



//-------------------------------------------------- protectSymbolByAntiSlash -----------------------------------------------------------------
/*! \brief protect a caracter by antislash in a text (if already protected nothing append)
 *  \param text :      const QString & html document
 *  \param symbol :    symbol to protect
 *  \return text modified
*/
QString C_Utils_Html::protectSymbolByAntiSlash(const QString &text, const QChar &symbol /*=','*/)
{long       pos = 0;
 long       len = text.length();
 QString result = "";

 while (pos < len)
     {QChar car = text.at(pos);
      if (car=='\\')                                  // something already protected, jump it
         {result += '\\' + text.at(pos+1);
          ++pos ;                                     // que +1 car ++pos de fin;
         }
      else if (car == symbol)
         { result += QString("\\") + car;
         }
      else
         { result += car;
         }
      ++pos;
     }
 return result;
}

//------------------------------------------ toNum -----------------------------------------
/*! \brief ne retient d'une chaine de caracteres que les valeurs numeriques et le point (au passage les virgules seront transformees en point).
 *  \param text           const QString & texte a convertir en numerique
 *  \param cutFirstStr_in const QString & mode de conversion numerique :
 SI   contient $keepSign     la conversion tient compte du signe.
 SI   contient $stopIfNotNum la conversion s'arrete au premier caractere non numerique.
 SI   contient $allString tous les caracteres numeriques de la chaine seront retenus.
 SI   contient $toInt la conversion ne conservera que la partie entiere.
 SI   toute autre valeur, la conversion s'arretera a la premiere occurence de cette valeur.
 *  \param int *pos si different de zero alors la position dans la chaine apres la valeur numerique
 *  \return une QString resultat du traitement
 */
QString C_Utils_Html::toNum(const QString &text, const QString &cutFirstStr_in /* = ""*/, int *next_pos /* = 0*/)
{if (next_pos)  *next_pos  = -1;
 QString str               = text.trimmed();
 QString cutFirstStr       = cutFirstStr_in;
 bool    cutFirstNonNum    = true;
 bool    keepSign          = (cutFirstStr.indexOf("$keepSign") != -1);
 if (keepSign) cutFirstStr =  cutFirstStr.remove("$keepSign");
 bool    toInt             = (cutFirstStr.indexOf("$toInt")    != -1);
 if (toInt) cutFirstStr    =  cutFirstStr.remove("$toInt");

 if (cutFirstStr.length())
    {
     if      (cutFirstStr.indexOf("$stopIfNotNum") != -1)    cutFirstNonNum = true;
     else if (cutFirstStr.indexOf("$allString")    != -1)    cutFirstNonNum = false;
     else {int pos = str.indexOf(cutFirstStr);
           if (pos != -1) str.truncate(pos);
          }
    }
 int     end = str.length();
 QString ret = "";
 int       i = 0;
 while (i<end)
     {if (str.at(i)>='0' && str[i]<='9')                      {ret += str.at(i);}
      else if (str.at(i)=='-' || str[i]=='+')                 { if (keepSign) ret += str.at(i);}
      else if (str.at(i)=='.')                                {ret += ".";   }
      else if (str.at(i)==',')                                {ret += ".";   }
      else if (cutFirstNonNum)                                {if (next_pos) *next_pos = i;   i = end;   }
      ++i;
     }
 if (next_pos && *next_pos==-1) *next_pos = i;
 if (toInt)
    {int pos  =     ret.indexOf(".");
     if (pos != -1) ret.truncate(pos);
    }
 return ret;
}
//------------------------------------- insertHtml --------------------------------------------------------------------------
/*! \brief insert text in QText edit.
 *  \param dstEdit     QTextEdit where insert text
 *  \param htmlStr     html text to insert
 *  \param noInsertIfJustTag     noInsertIfEmptytTags  1/no text insert if no text after removing html tags 0/insert text
 */
 void C_Utils_Html::insertHtml(QTextEdit* dstEdit, const QString &htmlStr, int  noInsertIfEmptytTags /* = 1 */)
 { if (htmlStr.length()==0)                                      return;
   if (noInsertIfEmptytTags && removeHtmlTag( htmlStr).length()==0) return;  // si texte significatif null ne pas inserer
   dstEdit->textCursor().insertHtml(htmlStr);
 }













