
// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================

#include "common.h"

/* BOARD */

/* CURRENT */

EXPRESSION(
	/* ID */			0,
	/* Name */			"CurBoard$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(0)
) {
	if ( numBoards )
	{
		ReturnString(d_sName.c_str());
	}
	
	ReturnString("");
}

EXPRESSION(
	/* ID */			1,
	/* Name */			"curBoardNum(",
	/* Flags */			0,
	/* Params */		(0)
) {
	if ( numBoards )
		return rdPtr->iSelBoard;

	return -1;
}

EXPRESSION(
	/* ID */			2,
	/* Name */			"boardAddr(",
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,"Board name")
) {
	string p1( (LPSTR)ExParam(TYPE_STRING) );

	if ( numBoards )
	{	
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			if ( strCompare(d_sNamei, p1, rdPtr->bStrCmp) )
				return (int)d_vDatai.data();
	}

	return 0;			
}

EXPRESSION(
	/* ID */			3,
	/* Name */			"boardSize(",
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,"Board name")
) {
	string p1( (LPSTR)ExParam(TYPE_STRING) );

	if ( numBoards )
	{		
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			   if ( strCompare(d_sNamei, p1, rdPtr->bStrCmp) )
				return d_vDatai.size();
	}

	return 0;
}

EXPRESSION(
	/* ID */			4,
	/* Name */			"Protected(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, "Board name")
)
{
	string p1( (LPSTR)ExParam(TYPE_STRING) );

	for (unsigned int i=0; i<numBoards; i++) //check if board already exists
		   if ( strCompare(d_sNamei, p1, rdPtr->bStrCmp) )
			return d_bProtectedi;
	return 0;
}


EXPRESSION(
	/* ID */			5,
	/* Name */			"NBoards(",
	/* Flags */			0,
	/* Params */		(0)
) {
	return numBoards;
}


EXPRESSION(
	/* ID */			6,
	/* Name */			("NameByNum$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_NUMBER, "Board number")
) {
	unsigned long i = ExParam(TYPE_INT);
	
	if ( numBoards && i < numBoards )
	{
		ReturnString(d_sNamei.c_str());
	}
	
	ReturnString("");
}

/* GET */

EXPRESSION(
	/* ID */			7,
	/* Name */			"sign(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Value")
	) {
	off_t p1tmp = ExParam(TYPE_FLOAT);
	float p1 = *reinterpret_cast<const float *>(&p1tmp);
	
	if ( p1 >= 0 )
		return 1;
	else
		return -1;
}

EXPRESSION(
	/* ID */			8,
	/* Name */			"char(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1 < d_vData.size() )
		return (char)d_vData.at(p1);
	
	return 0;
}

EXPRESSION(
	/* ID */			9,
	/* Name */			"uchar(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1 < d_vData.size() )
		return (unsigned char)d_vData.at(p1);
	
	return 0;
}

EXPRESSION(
	/* ID */			10,
	/* Name */			"short(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(short) <= d_vData.size() )
	{
		if ( !rdPtr->bEndianness )
			return *reinterpret_cast<const short *>(&d_vData.at(p1));
		else
			return _byteswap_ushort(*reinterpret_cast<const short *>(&d_vData.at(p1)));
	}
	
	return 0;
}

EXPRESSION(
	/* ID */			11,
	/* Name */			"ushort(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(short) <= d_vData.size() )
	{
		if ( !rdPtr->bEndianness )
			return *reinterpret_cast<const unsigned short *>(&d_vData.at(p1));
		else
			return _byteswap_ushort(*reinterpret_cast<const unsigned short *>(&d_vData.at(p1)));
	}
	
	return 0;
}

EXPRESSION(
	/* ID */			12,
	/* Name */			"long(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(long) <= d_vData.size() )
	{
		if ( !rdPtr->bEndianness )
			return *reinterpret_cast<const long *>(&d_vData.at(p1));
		else
			return _byteswap_ulong(*reinterpret_cast<const long *>(&d_vData.at(p1)));
	}
	
	return 0;
}

EXPRESSION(
	/* ID */			13,
	/* Name */			"float(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(float) <= d_vData.size() )
	{
		ReturnFloat(*reinterpret_cast<const float *>(&d_vData.at(p1)));
	}
	
	return 0;
}

EXPRESSION(
	/* ID */			14,
	/* Name */			"string$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_NUMBER, "Offset", EXPPARAM_NUMBER, "Size")
	) {
	off_t p1 = ExParam(TYPE_INT);
	size_t p2 = ExParam(TYPE_INT);

	if ( numBoards && p2 )
	{
		if ( p2>d_vData.size() )
			p2 = d_vData.size();
		string output( d_vData.begin()+p1, p1+p2>d_vData.size() ? d_vData.end():d_vData.begin()+p1+p2 );
		ReturnStringSafe(output.c_str());
	}

	ReturnString("");
}

EXPRESSION(
	/* ID */			15,
	/* Name */			"valueSize(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	long p1 = ExParam(TYPE_INT);

	if ( p1 < 0 || p1 > MAXWORD )
		return sizeof(long);

	if ( p1 <= MAXBYTE )
		return sizeof(char);

	return sizeof(short);
}

EXPRESSION(
	/* ID */			16,
	/* Name */			"fileSize(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,"File")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	ifstream file( p1, ios::binary | ios::ate);

	if ( file.is_open() )
	{
		const long size = file.tellg();
		file.close();
		return size;
	}	
	
	return 0;
}

/* OCCURENCES */
/* COUNT */

EXPRESSION(
	/* ID */			17,
	/* Name */			"countInt(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer value", EXPPARAM_NUMBER, "Value size")
) {
	long p1 = ExParam(TYPE_INT);
	char p2 = ExParam(TYPE_INT);

	if ( numBoards )
	{
		const char * value = reinterpret_cast<char *>(&p1);

		if ( p2 == 1 )
			return count(d_vData.begin(), d_vData.end(), (signed char)p1);

		long dist = 0;
		long counter = 0;
		while ( true )
		{
			auto it = search( d_vData.begin()+dist, d_vData.end(), value, value + sizeof(char)*p2 );
			dist = it - d_vData.begin();
			if ( dist == d_vData.size() )
				break;
			counter++;
			dist += sizeof(char)*p2;
		}
		return counter;
	}

	return 0;
}

EXPRESSION(
	/* ID */			18,
	/* Name */			"countStr(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, "String")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	
	if ( numBoards )
	{
		long counter = 0;
		long dist = 0;
		while ( true )
		{
			auto it = search( d_vData.begin()+dist, d_vData.end(), p1, p1 + strlen(p1) );			
			dist = it - d_vData.begin();	
			if ( dist == d_vData.size() )
				break;
			counter++;
			dist += strlen(p1);				
		}
		return counter;
	}
	
	return 0;
}

EXPRESSION(
	/* ID */			19,
	/* Name */			"countBoard(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, "Board name")
) {
	string p1( (LPSTR)ExParam(TYPE_STRING) );

	if ( numBoards )
	{	
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			   if ( strCompare(d_sNamei, p1, rdPtr->bStrCmp) )
			{
				long dist = 0;
				long counter = 0;
				while ( true )
				{
					auto it = search( d_vData.begin()+dist, d_vData.end(), d_vDatai.begin(), d_vDatai.end() );			
					dist = it - d_vData.begin();	
					if ( dist == d_vData.size() )
						break;
					counter++;
					dist += d_vDatai.size();				
				}
				return counter;
			}
	}

	return 0;
}

/* FIND */

EXPRESSION(
	/* ID */			20,
	/* Name */			"findInt(",
	/* Flags */			0,
	/* Params */		(3, EXPPARAM_NUMBER, "Integer value", EXPPARAM_NUMBER, "Value size", EXPPARAM_NUMBER, "Occurrence")
) {
	long p1 = ExParam(TYPE_INT);
	char p2 = ExParam(TYPE_INT);
	long p3 = ExParam(TYPE_INT);

	if ( numBoards && p3 )
	{
		const char * value = reinterpret_cast<char *>(&p1);

		if ( p2 == 1 )
			return count(d_vData.begin()+p3, d_vData.end(), (signed char)p1);

		long dist = 0;
		while ( true )
		{
			auto it = search( d_vData.begin()+dist+1, d_vData.end(), value, value + sizeof(char)*p2 );
			dist = it - d_vData.begin();
			p3--;
			if ( dist == d_vData.size() )
				return -1;
			if (p3)
				dist += sizeof(char)*p2;
			else
				return dist;

		}
	}
	
	return -1;
}

EXPRESSION(
	/* ID */			21,
	/* Name */			"findStr(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_STRING, "String", /*EXPPARAM_NUMBER, "Case sensitive", */EXPPARAM_NUMBER, "Occurrence")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	long p3 = ExParam(TYPE_INT);

	if ( numBoards && p3 )
	{
		long dist = 0;
		while ( p3 && dist != d_vData.size() )
		{
			auto it = search( d_vData.begin()+dist+1, d_vData.end(), p1, p1 + strlen(p1) );			
			dist = it - d_vData.begin();						
			p3--;
			if ( dist == d_vData.size() )
				return -1;
			if (p3)
				dist += strlen(p1);
			else
				return dist;
		}

	}
	
	return -1;
}

EXPRESSION(
	/* ID */			22,
	/* Name */			"findBoard(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_STRING, "Board name", EXPPARAM_NUMBER, "Occurrence")
) {
	string p1( (LPSTR)ExParam(TYPE_STRING) );
	long p2 = ExParam(TYPE_INT);

	if ( numBoards )
	{		
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			   if ( strCompare(d_sNamei, p1, rdPtr->bStrCmp) )
			{
				long dist = 0;
				while ( true )
				{
					auto it = search( d_vData.begin()+dist, d_vData.end(), d_vDatai.begin(), d_vDatai.end() );
					dist = it - d_vData.begin();
					p2--;
					if ( dist == d_vData.size() )
						return -1;
					if (p2)
						dist += d_vDatai.size();
					else
						return dist;
				}
			}
	}

	return -1;
}

/* Data conversions */

EXPRESSION(
	/* ID */			23,
	/* Name */			"BinToFlt(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
){	
	long p1temp = ExParam(TYPE_INT);
	float p1 = *reinterpret_cast<float *>(&p1temp);

	return p1;
}

EXPRESSION(
	/* ID */			24,
	/* Name */			"FltToBin(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Floating-Point value")
){	
	long p1temp = ExParam(TYPE_FLOAT);

	return p1temp;
}


EXPRESSION(
	/* ID */			25,
	/* Name */			"flipShort(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	short p1 = ExParam(TYPE_INT);

	return _byteswap_ushort(p1);
}

EXPRESSION(
	/* ID */			26,
	/* Name */			"flipLong(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Integer value")
	) {
	long p1 = ExParam(TYPE_INT);

	return _byteswap_ulong(p1);
}
