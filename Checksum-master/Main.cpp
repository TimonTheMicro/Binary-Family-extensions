// ============================================================================
//
// This file contains the actions, conditions and expressions your object uses
// 
// ============================================================================
#define LITTLE_ENDIAN

#include <string>

#include "common.h"
#include "crc16.h"
#include "crc32.h"
#include "md2.h"
#include "md4.h"
#include "md5.h"
#include "sha1.h"
#include "sha2.h"
#include "HMAC.h"


// ============================================================================
//
// CONDITIONS
// 
// ============================================================================


// ============================================================================
//
// ACTIONS
// 
// ============================================================================


// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================


EXPRESSION(
	/* ID */			0,
	/* Name */			"CRC16data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_NUMBER,"Memory location", EXPPARAM_NUMBER,"Size")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);
	
	if ( !rdPtr->crcTable[0] )
		make_crc_table(rdPtr->crcTable);

	unsigned short int value = crc16( (unsigned char*)p1, p2 );

	ReturnStringSafe( CRC16hexdigest(value).c_str() );
}

EXPRESSION(
	/* ID */			1,
	/* Name */			"CRC32data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_NUMBER,"Memory location", EXPPARAM_NUMBER,"Size")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);

	const unsigned char hashLen = 4;
	
	if ( !rdPtr->crcTable[0] )
		make_crc_table(rdPtr->crcTable);

	unsigned long int value = crc32( (unsigned char*)p1, p2, rdPtr->crcTable );

	char chars[2*hashLen+1];
	ltoa(value, chars, 16);

	ReturnStringSafe( string("0x" + std::string(chars)).c_str() );
}

EXPRESSION(
	/* ID */			2,
	/* Name */			"MD2data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, "Memory location", EXPPARAM_NUMBER,"Size", EXPPARAM_STRING, "HMAC key (optional)")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);
	LPSTR p3 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	MD2 md2;
	if ( strlen(p3) )
	{
		HMAC hmac(&md2, (unsigned char*)p2, strlen(p3));
		hmac.update((unsigned char*)p1, p2);
		hmac.finalize(output);
	}
	else
		md2.computeHash(output, (unsigned char*)p1, p2 );

	char buf[2*hashLen+1];
    for (int i=0; i<md2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[md2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			3,
	/* Name */			"MD4data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, "Memory location", EXPPARAM_NUMBER,"Size", EXPPARAM_STRING, "HMAC key (optional)")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);
	LPSTR p3 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	MD4 md4;
	if ( strlen(p3) )
	{
		HMAC hmac(&md4, (unsigned char*)p2, strlen(p3));
		hmac.update((unsigned char*)p1, p2);
		hmac.finalize(output);
	}
	else
		md4.computeHash(output, (unsigned char*)p1, p2 );

	char buf[2*hashLen+1];
    for (int i=0; i<md4.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[md4.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			4,
	/* Name */			"MD5data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, "Memory location", EXPPARAM_NUMBER,"Size", EXPPARAM_STRING, "HMAC key (optional)")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);
	LPSTR p3 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	MD5 md5;
	if ( strlen(p3) )
	{
		HMAC hmac(&md5, (unsigned char*)p2, strlen(p3));
		hmac.update((unsigned char*)p1, p2);
		hmac.finalize(output);
	}
	else
		md5.computeHash(output, (unsigned char*)p1, p2 );

	char buf[2*hashLen+1];
    for (int i=0; i<md5.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[md5.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			5,
	/* Name */			"SHA1data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, "Memory location", EXPPARAM_NUMBER,"Size", EXPPARAM_STRING, "HMAC key (optional)")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);
	LPSTR p3 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	SHA1 sha1;
	if ( strlen(p3) )
	{
		HMAC hmac(&sha1, (unsigned char*)p2, strlen(p3));
		hmac.update((unsigned char*)p1, p2);
		hmac.finalize(output);
	}
	else
		sha1.computeHash(output, (unsigned char*)p1, p2 );

	char buf[2*hashLen+1];
    for (int i=0; i<sha1.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha1.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			6,
	/* Name */			"SHA224data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, "Memory location", EXPPARAM_NUMBER,"Size", EXPPARAM_STRING, "HMAC key (optional)")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);
	LPSTR p3 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 32;
	unsigned char output[hashLen+1];

	SHA224 sha2;
	if ( strlen(p3) )
	{
		HMAC hmac(&sha2, (unsigned char*)p2, strlen(p3));
		hmac.update((unsigned char*)p1, p2);
		hmac.finalize(output);
	}
	else
		sha2.computeHash(output, (unsigned char*)p1, p2 );

	char buf[2*hashLen+1];
    for (int i=0; i<sha2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			7,
	/* Name */			"SHA256data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, "Memory location", EXPPARAM_NUMBER,"Size", EXPPARAM_STRING, "HMAC key (optional)")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);
	LPSTR p3 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 32;
	unsigned char output[hashLen+1];

	SHA256 sha2;
	if ( strlen(p3) )
	{
		HMAC hmac(&sha2, (unsigned char*)p2, strlen(p3));
		hmac.update((unsigned char*)p1, p2);
		hmac.finalize(output);
	}
	else
		sha2.computeHash(output, (unsigned char*)p1, p2 );

	char buf[2*hashLen+1];
    for (int i=0; i<sha2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			8,
	/* Name */			"SHA384data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, "Memory location", EXPPARAM_NUMBER,"Size", EXPPARAM_STRING, "HMAC key (optional)")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);
	LPSTR p3 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	SHA384 sha2;
	if ( strlen(p3) )
	{
		HMAC hmac(&sha2, (unsigned char*)p2, strlen(p3));
		hmac.update((unsigned char*)p1, p2);
		hmac.finalize(output);
	}
	else
		sha2.computeHash(output, (unsigned char*)p1, p2 );

	char buf[2*hashLen+1];
    for (int i=0; i<sha2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			9,
	/* Name */			"SHA512data$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, "Memory location", EXPPARAM_NUMBER,"Size", EXPPARAM_STRING, "HMAC key (optional)")
) {
	long p1=ExParam(TYPE_INT);
	long p2=ExParam(TYPE_INT);
	LPSTR p3 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	SHA512 sha2;
	if ( strlen(p3) )
	{
		HMAC hmac(&sha2, (unsigned char*)p2, strlen(p3));
		hmac.update((unsigned char*)p1, p2);
		hmac.finalize(output);
	}
	else
		sha2.computeHash(output, (unsigned char*)p1, p2 );

	char buf[2*hashLen+1];
    for (int i=0; i<sha2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			10,
	/* Name */			"CRC16str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_STRING, "String")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	
	if ( !rdPtr->crcTable[0] )
		make_crc_table(rdPtr->crcTable);

	unsigned long int value = crc16( (unsigned char*)p1, strlen(p1) );

	ReturnStringSafe( CRC16hexdigest(value).c_str() );
}

EXPRESSION(
	/* ID */			11,
	/* Name */			"CRC32str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_STRING, "String")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	
	const unsigned char hashLen = 4;

	if ( !rdPtr->crcTable[0] )
		make_crc_table(rdPtr->crcTable);

	unsigned long int value = crc32( (unsigned char*)p1, strlen(p1), rdPtr->crcTable );

	char chars[2*hashLen+1];
	ltoa(value, chars, 16);

	ReturnStringSafe( string("0x" + std::string(chars)).c_str() );
}

EXPRESSION(
	/* ID */			12,
	/* Name */			"MD2str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_STRING, "String", EXPPARAM_STRING, "HMAC key (optional)")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	LPSTR p2 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	MD2 md2;
	if ( strlen(p2) )
	{
		HMAC hmac(&md2, (unsigned char*)p2, strlen(p2));
		hmac.update((unsigned char*)p1, strlen(p1));
		hmac.finalize(output);
	}
	else
		md2.computeHash(output, (unsigned char*)p1, strlen(p1));

	char buf[2*hashLen+1];
    for (int i=0; i<md2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[md2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			13,
	/* Name */			"MD4str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_STRING, "String", EXPPARAM_STRING, "HMAC key (optional)")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	LPSTR p2 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	MD4 md4;
	if ( strlen(p2) )
	{
		HMAC hmac(&md4, (unsigned char*)p2, strlen(p2));
		hmac.update((unsigned char*)p1, strlen(p1));
		hmac.finalize(output);
	}
	else
		md4.computeHash(output, (unsigned char*)p1, strlen(p1));

	char buf[2*hashLen+1];
    for (int i=0; i<md4.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[md4.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			14,
	/* Name */			"MD5str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_STRING, "String", EXPPARAM_STRING, "HMAC key (optional)")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	LPSTR p2 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	MD5 md5;
	if ( strlen(p2) )
	{
		HMAC hmac(&md5, (unsigned char*)p2, strlen(p2));
		hmac.update((unsigned char*)p1, strlen(p1));
		hmac.finalize(output);
	}
	else
		md5.computeHash(output, (unsigned char*)p1, strlen(p1));

	char buf[2*hashLen+1];
    for (int i=0; i<md5.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[md5.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			15,
	/* Name */			"SHA1str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_STRING, "String", EXPPARAM_STRING, "HMAC key (optional)")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	LPSTR p2 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	SHA1 sha1;
	if ( strlen(p2) )
	{
		HMAC hmac(&sha1, (unsigned char*)p2, strlen(p2));
		hmac.update((unsigned char*)p1, strlen(p1));
		hmac.finalize(output);
	}
	else
		sha1.computeHash(output, (unsigned char*)p1, strlen(p1));

	char buf[2*hashLen+1];
    for (int i=0; i<sha1.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha1.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			16,
	/* Name */			"SHA224str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_STRING, "String", EXPPARAM_STRING, "HMAC key (optional)")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	LPSTR p2 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 32;
	unsigned char output[hashLen+1];

	SHA384 sha2;
	if ( strlen(p2) )
	{
		HMAC hmac(&sha2, (unsigned char*)p2, strlen(p2));
		hmac.update((unsigned char*)p1, strlen(p1));
		hmac.finalize(output);
	}
	else
		sha2.computeHash(output, (unsigned char*)p1, strlen(p1));

	char buf[2*hashLen+1];
    for (int i=0; i<sha2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			17,
	/* Name */			"SHA256str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_STRING, "String", EXPPARAM_STRING, "HMAC key (optional)")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	LPSTR p2 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 32;
	unsigned char output[hashLen+1];

	SHA256 sha2;
	if ( strlen(p2) )
	{
		HMAC hmac(&sha2, (unsigned char*)p2, strlen(p2));
		hmac.update((unsigned char*)p1, strlen(p1));
		hmac.finalize(output);
	}
	else
		sha2.computeHash(output, (unsigned char*)p1, strlen(p1));

	char buf[2*hashLen+1];
    for (int i=0; i<sha2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			18,
	/* Name */			"SHA384str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_STRING, "String", EXPPARAM_STRING, "HMAC key (optional)")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	LPSTR p2 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	SHA384 sha2;
	if ( strlen(p2) )
	{
		HMAC hmac(&sha2, (unsigned char*)p2, strlen(p2));
		hmac.update((unsigned char*)p1, strlen(p1));
		hmac.finalize(output);
	}
	else
		sha2.computeHash(output, (unsigned char*)p1, strlen(p1));

	char buf[2*hashLen+1];
    for (int i=0; i<sha2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}

EXPRESSION(
	/* ID */			19,
	/* Name */			"SHA512str$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_STRING, "String", EXPPARAM_STRING, "HMAC key (optional)")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	LPSTR p2 = (LPSTR)ExParam(TYPE_STRING);
	const unsigned char hashLen = 64;
	unsigned char output[hashLen+1];

	SHA512 sha2;
	if ( strlen(p2) )
	{
		HMAC hmac(&sha2, (unsigned char*)p2, strlen(p2));
		hmac.update((unsigned char*)p1, strlen(p1));
		hmac.finalize(output);
	}
	else
		sha2.computeHash(output, (unsigned char*)p1, strlen(p1));

	char buf[2*hashLen+1];
    for (int i=0; i<sha2.outputSize(); i++)
        sprintf(buf+i*2, "%02x", output[i]);
	buf[sha2.outputSize()*2] = NULL;

	ReturnStringSafe( (char*)buf );
}
