
 /******************************** C_Zlib.h **********************************
 *   adapted for MedinTux by Sevin Roland    medintux@medintux.org           *
 *   from original code : http://www.zlib.net/                               *
 *                                                                           *
 *   for change  grep : #~#                                                  *
 *   i have just put the code in a C_Zlib class for C++ usage                *
 *                                                                           *
 * ================== original notice folows:  ============================= *
 *                                                                           *
  Copyright notice:

 (C) 1995-2004 Jean-loup Gailly and Mark Adler

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Jean-loup Gailly        Mark Adler
  jloup@gzip.org          madler@alumni.caltech.edu

 * ================== End of original notice folows:  ====================== *
 *                                                                           *
 *****************************************************************************/
#ifndef  C_MED_ZLIB_H
#define  C_MED_ZLIB_H
#include <qstring.h>

//===================================== C_Zlib =============================================================================
class C_Zlib
{
 //............................... METHODES .........................................................................
 public:
     C_Zlib();
    ~C_Zlib(){};
     int Compresse(QString src, QString dst, const QString &ext = ".zip");
     int Decompresse(QString src, QString dst, const QString &ext = ".zip");
};

#endif
