
 /******************************** C_Huffman.h *******************************
 *   adapted for MedinTux by Sevin Roland    medintux@medintux.org           *
 *   from original code : http://sourceforge.net/projects/huffman/           *
 *                                                                           *
 *               original notice folows:                                     *
 *                                                                           *
 *               huffcode - Encode/Decode files using Huffman encoding.      *
 *               http://huffman.sourceforge.net                              *
 *               Copyright (c) 2007, Douglas Ryan Richardson,                *
 *               Gauss Interprise, Inc All rights reserved.                  *
 *                                                                           *
 *               Redistribution and use in source and binary forms, with or  *
 *               without modification, are permitted provided that the       *
 *               following conditions are met:                               *
 *                                                                           *
 *               Redistributions of source code must retain the above        *
 *               copyright notice, this list of conditions and the following *
 *               disclaimer.                                                 *
 *               Redistributions in binary form must reproduce the above     *
 *               copyright notice, this list of conditions and the following *
 *               disclaimer in the documentation and/or other materials      *
 *               provided with the distribution.                             *
 *               Neither the name of Gauss Interprise, Inc nor the names of  *
 *               its contributors may be used to endorse or promote products *
 *               derived from this software without specific prior written   *
 *               permission.                                                 *
 *                                                                           *
 *               THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND      *
 *               CONTRIBUTORS  "AS IS" AND ANY EXPRESS OR IMPLIED            *
 *               WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED      *
 *               WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR  *
 *               PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT     *
 *               OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,   *
 *               INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    *
 *               (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE   *
 *               GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR        *
 *               BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF  *
 *               LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT   *
 *               (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  *
 *               OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE         *
 *               POSSIBILITY OF SUCH DAMAGE.                                 *
 *                                                                           *
 *****************************************************************************/
#ifndef  C_HUFFMAN_H
#define  C_HUFFMAN_H


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#ifdef Q_WS_WIN
   //#include  <sys/socket.h>
   #include <WinSock2.h>
   #include <malloc.h>

   extern int getopt(int, char**, char*);
   extern char* optarg;
#else
   #include <netinet/in.h>
   #include <unistd.h>
#endif

#define CACHE_SIZE 2048
#define MAX_SYMBOLS 512


//===================================== huffman_node_tag =============================================================================
typedef struct huffman_node_tag
{
        unsigned char isLeaf;
        unsigned long count;
        struct huffman_node_tag *parent;

        union
        {
                struct
                {
                        struct huffman_node_tag *zero, *one;
                };
                unsigned char symbol;
        };
} huffman_node;

//===================================== huffman_code_tag =============================================================================
typedef struct huffman_code_tag
{
        /* The length of this code in bits. */
        unsigned long numbits;

        /* The bits that make up this code. The first
           bit is at position 0 in bits[0]. The second
           bit is at position 1 in bits[0]. The eighth
           bit is at position 7 in bits[0]. The ninth
           bit is at position 0 in bits[1]. */
        unsigned char *bits;
} huffman_code;

//===================================== buf_cache_tag =============================================================================
typedef struct buf_cache_tag
{
        unsigned char *cache;
        unsigned int cache_len;
        unsigned int cache_cur;
        unsigned char **pbufout;
        unsigned int *pbufoutlen;
} buf_cache;

typedef huffman_node* SymbolFrequencies[MAX_SYMBOLS];
typedef huffman_code* SymbolEncoder[MAX_SYMBOLS];

//===================================== C_Huffman =============================================================================
class C_Huffman
{
 //............................... METHODES .........................................................................
 public:
     C_Huffman(int argc, char** argv);
     C_Huffman(){};
    ~C_Huffman(){};

void version(FILE *out);

void usage(FILE* out);

int main_entry (int argc, char** argv);

int            memory_encode_file(FILE *in, FILE *out);
int            memory_decode_file(FILE *in, FILE *out);

int            huffman_encode_file(FILE *in, FILE *out);
int            huffman_decode_file(FILE *in, FILE *out);

int            huffman_encode_memory(const unsigned char *bufin,  unsigned int bufinlen, unsigned char **pbufout, unsigned int *pbufoutlen);
int            huffman_decode_memory(const unsigned char *bufin,  unsigned int bufinlen, unsigned char **pbufout, unsigned int *pbufoutlen);

protected:
unsigned long  numbytes_from_numbits(unsigned long numbits);
unsigned char  get_bit(unsigned char* bits, unsigned long i);
void           reverse_bits(unsigned char* bits, unsigned long numbits);
huffman_code*  new_code(const huffman_node* leaf);
huffman_node*  new_leaf_node(unsigned char symbol);
huffman_node*  new_nonleaf_node(unsigned long count, huffman_node *zero, huffman_node *one);
void           free_huffman_tree(huffman_node *subtree);
void           free_code(huffman_code* p);
void           free_encoder(SymbolEncoder *pSE);
void           init_frequencies(SymbolFrequencies *pSF);
int            init_cache(buf_cache* pc,   unsigned int cache_size,  unsigned char **pbufout,  unsigned int *pbufoutlen);
void           free_cache(buf_cache* pc);
int            flush_cache(buf_cache* pc);
int            write_cache(buf_cache* pc,  const void *to_write,  unsigned int to_write_len);
unsigned int   get_symbol_frequencies(SymbolFrequencies *pSF, FILE *in);
unsigned int   get_symbol_frequencies_from_memory(SymbolFrequencies *pSF, const unsigned char *bufin, unsigned int bufinlen);
static int     SFComp(const void *p1, const void *p2);
void           build_symbol_encoder(huffman_node *subtree, SymbolEncoder *pSF);
SymbolEncoder* calculate_huffman_codes(SymbolFrequencies * pSF);
int            write_code_table(FILE* out, SymbolEncoder *se, unsigned int symbol_count);
int            write_code_table_to_memory(buf_cache *pc, SymbolEncoder *se, unsigned int symbol_count);
huffman_node*  read_code_table(FILE* in, unsigned int *pDataBytes);
int            memread(const unsigned char* buf, unsigned int buflen, unsigned int *pindex, void* bufout, unsigned int readlen);
huffman_node*  read_code_table_from_memory(const unsigned char* bufin, unsigned int bufinlen, unsigned int *pindex, unsigned int *pDataBytes);
int            do_file_encode(FILE* in, FILE* out, SymbolEncoder *se);
int            do_memory_encode(buf_cache *pc, const unsigned char* bufin, unsigned int bufinlen, SymbolEncoder *se);

};

#endif
