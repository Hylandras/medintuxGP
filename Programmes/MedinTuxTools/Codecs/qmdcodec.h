/*
   Copyright (C) 2000-2001 Dawit Alemayehu <adawit@kde.org>
   Copyright (C) 2001 Rik Hemsley (rikkus) <rik@kde.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License (LGPL)
   version 2 as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   RFC 1321 "MD5 Message-Digest Algorithm" Copyright (C) 1991-1992.
   RSA Data Security, Inc. Created 1991. All rights reserved.

   The KMD5 class is based on a C++ implementation of
   "RSA Data Security, Inc. MD5 Message-Digest Algorithm" by
   Mordechai T. Abzug,	Copyright (c) 1995.  This implementation
   passes the test-suite as defined in RFC 1321.

   The encoding and decoding utilities in KCodecs with the exception of
   quoted-printable are based on the java implementation in HTTPClient
   package by Ronald Tschalär Copyright (C) 1996-1999.

   The quoted-printable codec as described in RFC 2045, section 6.7. is by
   Rik Hemsley (C) 2001.
*/

#ifndef _QMDHHHCODEC_H
#define _QMDHHHCODEC_H

#define QBase64 QCodecs

#include <qglobal.h>
#include <qstring.h>
#include <qcstring.h>
#include <qiodevice.h>

/**
 * A wrapper class for the most commonly used encoding and
 * decoding algorithms.  Currently there is support for encoding
 * and decoding input using base64, uu and the quoted-printable
 * specifications.
 *
 * @sect Usage:
 *
 * <PRE>
 * QCString input = "Aladdin:open sesame";
 * QCString result = QCodecs::base64Encode(input);
 * cout << "Result: " << result.data() << endl;
 *
 * Output should be
 * Result: QWxhZGRpbjpvcGVuIHNlc2FtZQ==
 * </PRE>
 *
 * The above example makes use of the convenience functions
 * (ones that accept/return null-terminated strings) to encode/decode
 * a string.  If what you need is to encode or decode binary data, then
 * it is highly recommended that you use the functions that take an input
 * and output QByteArray as arguments.  These functions are specifically
 * tailored for encoding and decoding binary data.
 *
 * @short A collection of commonly used encoding and decoding algorithms.
 * @author Dawit Alemayehu <adawit@kde.org>
 * @author Rik Hemsley <rik@kde.org>
 */
class QCodecs
{
public:

  /**
   * Encodes the given data using the base64 algorithm.
   *
   * The boolean argument determines if the encoded data is
   * going to be restricted to 76 characters or less per line
   * as specified by RFC 2045.  If @p insertLFs is true, then
   * there will be 76 characters or less per line.
   *
   * @param in         the data to be encoded.
   * @param insertLFs  limit the number of characters per line.
   *
   * @return           a base64 encoded string.
   */
  static QCString base64Encode( const QByteArray& in, bool insertLFs = false);

  /**
   * @overload
   *
   * Same as the above functions except it accepts
   * a null terminated string instead an array.
   *
   * @param str       the string to be encoded.
   * @param insertLFs limit the number of characters per line.
   * @return          the decoded string.
   */
  static QCString base64Encode( const QCString& str, bool insertLFs = false );

  /**
   * Encodes the given data using the base64 algorithm.
   *
   * Use this function if you want the result of the encoding
   * to be placed in another array which cuts down the number
   * of copy operation that have to be performed in the process.
   * This is also the preferred method for encoding binary data.
   *
   * The boolean argument determines if the encoded data is going
   * to be restricted to 76 characters or less per line as specified
   * by RFC 2045.  If @p insertLFs is true, then there will be 76
   * characters or less per line.
   *
   * NOTE: the output array is first reset and then resized
   * appropriately before use, hence, all data stored in the
   * output array will be lost.
   *
   * @param in        the data to be encoded using base64.
   * @param out       the container for the encoded data.
   * @param insertLFs limit the number of characters per line.
   */
  static void base64Encode( const QByteArray& in, QByteArray& out,   bool insertLFs = false );

  /**
   * Decodes the given data that was encoded using the
   * base64 algorithm.
   *
   * @param in   the base64-encoded data to be decoded.
   * @return     the decoded data.
   */
  static QCString base64Decode( const QByteArray& in );

  /**
   * @overload
   *
   * Same as the above functions except it accepts
   * a null terminated string instead an array.
   *
   * @param str  the base64-encoded string.
   * @return     the decoded string.
   */
  static QCString base64Decode( const QCString& str );

  /**
   * Decodes the given data that was encoded with the base64
   * algorithm.
   *
   * Use this function if you want the result of the decoding
   * to be placed in another array which cuts down the number
   * of copy operation that have to be performed in the process.
   * This is also the preferred method for decoding an encoded
   * binary data.
   *
   * NOTE: the output array is first reset and then resized
   * appropriately before use, hence, all data stored in the
   * output array will be lost.
   *
   * @param in   the encoded data to be decoded.
   * @param out  the container for the decoded data.
   */
  static void base64Decode( const QByteArray& in, QByteArray& out );


private:
  QCodecs();

private:
  static const char Base64EncMap[64];
  static const char Base64DecMap[128];
  static const unsigned int maxQPLineLength;
};
#endif
