#ifndef C_WORDGENREPAIR_H
#define C_WORDGENREPAIR_H
#include <QString>

//=================== C_WordGenrePair ===================================
/*! \class C_WordGenrePair
 *  \brief Class to set plural and genre of a word.
 */

class C_WordGenrePair
{
  public:
            C_WordGenrePair(){}
            C_WordGenrePair(const QString &key_word,
                            const QString singular_word,
                            const QString &plural_word,
                            const char &sexe
                            )
  {  m_key_word            =  key_word;
     m_singular_word       =  singular_word;
     m_plural_plural       =  plural_word;
     m_sexe                =  sexe;
  }
  QString key_word ()       {return m_key_word       ;   }
  QString singular_word ()  {return m_singular_word  ;   }
  QString plural_word ()    {return m_plural_plural  ;   }
  char    sexe  ()          {return m_sexe           ;   }    // f ou m

private:
  QString m_key_word          ;
  QString m_singular_word     ;
  QString m_plural_plural     ;
  char    m_sexe              ;
};
#endif // C_WORDGENREPAIR_H
