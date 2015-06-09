
 /******************************** C_Zip.h *******************************
 *   adapted for MedinTux by Sevin Roland    medintux@medintux.org           *
 *   from original code :            *
 *                                                                           *
 *               original notice folows:                                     *
 *                                                                           *

 *****************************************************************************/
#ifndef  C_ZIP_H
#define  C_ZIP_H


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#ifdef WIN32
   #include  <sys/socket.h>
   #include <malloc.h>
#else
   #include <netinet/in.h>
   #include <unistd.h>
#endif

#define CACHE_SIZE 2048
#define MAX_SYMBOLS 512




//===================================== CGestIni =============================================================================
class C_Zip
{
 //............................... METHODES .........................................................................
 public:
     C_Zip(int argc, char** argv);
     C_Zip(){};
    ~C_Zip(){};

};

#endif
