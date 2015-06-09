 /******************************* C_Zlib.cpp ********************************
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
 ****************************************************************************/


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "C_Zlib.h"
#include "zlib.h"
#define CHUNK 16384

int _deflate(FILE *source, FILE *dest, int level);
int _inflate(FILE *source, FILE *dest);
void zerr(int ret);


//===================================== _deflate =============================================================================
/* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
int _deflate(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    char in[CHUNK];
    char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = (Bytef*)in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = (Bytef*)out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

//===================================== _inflate =============================================================================
/* Decompress from file source to file dest until stream ends or EOF.
   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_DATA_ERROR if the deflate data is
   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
   the version of the library linked do not match, or Z_ERRNO if there
   is an error reading or writing the files. */
int _inflate(FILE *source, FILE *dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    char in[CHUNK];
    char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = (Bytef*)in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = (Bytef*)out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

//===================================== zerr =============================================================================
/* report a zlib or i/o error */
void zerr(int ret)
{
    fputs("zpipe: ", stderr);
    switch (ret) {
    case Z_ERRNO:
        if (ferror(stdin))
            fputs("error reading stdin\n", stderr);
        if (ferror(stdout))
            fputs("error writing stdout\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}

//------------------------------- C_Zlib -----------------------------------------------------------
C_Zlib::C_Zlib()
{
}

//------------------------------- Decompresse -----------------------------------------------------------
int C_Zlib::Decompresse(QString src, QString dst, const QString &ext /*= ".zip"*/)
{   FILE *in  = stdin;
    FILE *out = stdout;

    //............................... decompresser le fichier ........................................
    src += ext;
    in = fopen(src, "rb");

    if (!in) {QString err  = "C_Zlib::Inflate()  erreur ouverture fichier d'entree : ";
                      err += src;
              fputs(err, stderr);
              return -1;
             }

    out = fopen(dst , "wb");
    if (!out) {QString err  = "C_Zlib::Inflate()  erreur ouverture fichier de sortie : ";
                       err += dst;
               fputs(err, stderr);
               return -1;
              }

    int ret  = _inflate(in, out);
    if (ret != Z_OK)  zerr(ret);

    fclose(out);
    fclose(in);
    return ret;
}

//------------------------------- Compresse -----------------------------------------------------------
int C_Zlib::Compresse(QString src, QString dst, const QString &ext /*= ".zip"*/)
{   FILE *in  = stdin;
    FILE *out = stdout;

    //............................... compresser le fichier ........................................
    in = fopen(src , "rb");
    if (!in) {fputs("C_Zlib::Deflate()  erreur ouverture fichier d'entree\n", stderr);  return -1;}

    out = fopen(dst + ext, "wb");
    if (!out) {fputs("C_Zlib::Deflate() erreur ouverture fichier de sortie\n", stderr); return -1;}
    int ret  = _deflate(in, out, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)  zerr(ret);
    fclose(out);
    fclose(in);
    return ret;
}

