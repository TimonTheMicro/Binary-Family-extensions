
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
			if ( strCompare(d_sNamei, p1) )
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
			   if ( strCompare(d_sNamei, p1) )
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
		   if ( strCompare(d_sNamei, p1) )
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
	
	if ( p1 < 0 )
		return -1;

		return 1;
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
			auto it = std::search( d_vData.begin()+dist, d_vData.end(), value, value + sizeof(char)*p2 );
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
			auto it = std::search( d_vData.begin()+dist, d_vData.end(), p1, p1 + strlen(p1) );			
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
			   if ( strCompare(d_sNamei, p1) )
			{
				long dist = 0;
				long counter = 0;
				while ( true )
				{
					auto it = std::search( d_vData.begin()+dist, d_vData.end(), d_vDatai.begin(), d_vDatai.end() );			
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
			auto it = std::search( d_vData.begin()+dist+1, d_vData.end(), value, value + sizeof(char)*p2 );
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
	/* Params */		(4, EXPPARAM_STRING, "String", /*EXPPARAM_NUMBER, "Case sensitive", */EXPPARAM_NUMBER, "Occurrence",EXPPARAM_NUMBER,"Begins with NULL (0 for false, 1 for true)",EXPPARAM_NUMBER,"Ends with NULL (0 for false, 1 for true)")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);
	long p2 = ExParam(TYPE_INT);
	bool p3 = ExParam(TYPE_INT);
	bool p4 = ExParam(TYPE_INT);

	if ( numBoards && p2 )
	{
		long dist = 0;
		while ( p2 && dist != d_vData.size() )
		{
			auto it = std::search( d_vData.begin()+dist+1, d_vData.end(), p1, p1 + strlen(p1) );			
			dist = it - d_vData.begin();						
			if ( !p3 || (p3 && !((char)d_vData.at(dist-1))))
				if ( !p4 || (p4 && ((dist+strlen(p1)) < d_vData.size()) && !((char)d_vData.at(dist+strlen(p1)))))
					p2--;
			if ( dist == d_vData.size() )
				return -1;
			if (p2)
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
			   if ( strCompare(d_sNamei, p1) )
			{
				long dist = 0;
				while ( true )
				{
					auto it = std::search( d_vData.begin()+dist, d_vData.end(), d_vDatai.begin(), d_vDatai.end() );
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
	float p1 = *reinterpret_cast<const float*>(&p1temp);

	ReturnFloat(p1);
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

EXPRESSION(
	/* ID */			27,
	/* Name */			"bit(",
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, "Offset", EXPPARAM_NUMBER, "bit")
	) {
	off_t p1 = ExParam(TYPE_INT);
	off_t bit = ExParam(TYPE_INT);

	if (numBoards && p1 < d_vData.size())
	{
		if (!rdPtr->bEndianness)
			return ((char)d_vData.at(p1) >> bit) & 1;
		else
			return ((char)d_vData.at(p1) >> (7-bit)) & 1;
	}

	return 0;
}

EXPRESSION(
	/* ID */			28,
	/* Name */			"ntsl(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, "Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);
	size_t p2 = 1024^2;

	if ( numBoards && p2 )
	{
		if ( p2>d_vData.size() )
			p2 = d_vData.size();
		string str( d_vData.begin()+p1, p1+p2>d_vData.size() ? d_vData.end():d_vData.begin()+p1+p2 );
		const char* output = str.c_str();
		return strlen(output);
	}

	return 0;
}
//EXPRESSION(
//	/* ID */			28,
//	/* Name */			"undefined(",
//	/* Flags */			0,
//	/* Params */		(2, EXPPARAM_NUMBER, "bits", EXPPARAM_NUMBER, "Offset")
//	) {
//	off_t p1 = ExParam(TYPE_INT);
//
//	if ( numBoards && p1 < d_vData.size() )
//		return (char)d_vData.at(p1);
//	
//	return 0;
//}

/* Date conversion */
EXPRESSION(
	/* ID */			29,
	/* Name */			"getYear(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, "Hexadecimal 64bit value")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
    istringstream iss(p1);
    iss >> hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return (1900+time.tm_year);
}
EXPRESSION(
	/* ID */			30,
	/* Name */			"getMonth(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, "Hexadecimal 64bit value")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
    istringstream iss(p1);
    iss >> hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_mon;
}
EXPRESSION(
	/* ID */			31,
	/* Name */			"getDay(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, "Hexadecimal 64bit value")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
    istringstream iss(p1);
    iss >> hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_mday;
}
EXPRESSION(
	/* ID */			32,
	/* Name */			"getHour(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, "Hexadecimal 64bit value")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
    istringstream iss(p1);
    iss >> hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_hour;
}
EXPRESSION(
	/* ID */			33,
	/* Name */			"getMinute(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, "Hexadecimal 64bit value")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
    istringstream iss(p1);
    iss >> hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_min;
}
EXPRESSION(
	/* ID */			34,
	/* Name */			"getSecond(",
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, "Hexadecimal 64bit value")
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
    istringstream iss(p1);
    iss >> hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_sec;
}

EXPRESSION(
	/* ID */			35,
	/* Name */			"IntToStr$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer", EXPPARAM_NUMBER, "Size" )
	) {
	long p1 = ExParam(TYPE_INT);
	long p2 = ExParam(TYPE_INT);

	stringstream ss;
	ss << std::setw(p2) << std::setfill('0') << p1;
	string str = ss.str();

	ReturnStringSafe(str.c_str());
}


EXPRESSION(
	/* ID */			36,
	/* Name */			"IntToHex$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer", EXPPARAM_NUMBER, "Size" )
	) {
	long p1 = ExParam(TYPE_INT);
	long p2 = ExParam(TYPE_INT);

	stringstream ss;
	ss << std::setfill('0') << std::setw(p2) << std::right << std::hex << p1;
	string hex = ss.str();
	std::transform(hex.begin(), hex.end(),hex.begin(), ::toupper);
	hex = "0x"+hex;

	ReturnStringSafe(hex.c_str());
}


EXPRESSION(
	/* ID */			37,
	/* Name */			"IntToBin$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_NUMBER, "Integer", EXPPARAM_NUMBER, "Size" )
	) {
	long p1 = ExParam(TYPE_INT);
	long p2 = ExParam(TYPE_INT);

	stringstream ss;
/*
switch(p2) {
  case 1:
    ss << int_to_bitset<char>(p1);
    break;
  case 2:
    ss << int_to_bitset<short>(p1);
    break;
  case 4:
    ss << int_to_bitset<int>(p1);
    break;
  case 8:
    ss << int_to_bitset<long>(p1);
    break;

  default:
    ss << int_to_bitset<int>(p1);
}
*/
	string bin = ss.str();

	ReturnString(bin.c_str());
}

EXPRESSION(
	/* ID */			38,
	/* Name */			"longlong$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_NUMBER, "Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);
	long long output(0);

	if ( numBoards && p1+sizeof(long) <= d_vData.size() )
	{
		if ( !rdPtr->bEndianness )
			output = *reinterpret_cast<const long long*>(&d_vData.at(p1));
		else
			output = _byteswap_ulong(*reinterpret_cast<const long long*>(&d_vData.at(p1)));
	}
	
	stringstream ss;
	ss << std::right << std::hex << output; // << std::endl
	string hex = ss.str();
	std::transform(hex.begin(), hex.end(),hex.begin(), ::toupper);
	hex = "0x"+hex;
	ReturnStringSafe( hex.c_str());
}



//MATRICES
#include <rotm2quat.h>

EXPRESSION(
	/* ID */			39,
	/* Name */			"m4x4getrW(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "Matrix Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(float) <= d_vData.size() )
	{
		ReturnFloat(
			mRot2Quat(0,
			*reinterpret_cast<const float *>(&d_vData.at(p1)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+4)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+8)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+16)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+20)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+24)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+32)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+36)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+40))
			)
		);
	}
	
	return 0;
}

EXPRESSION(
	/* ID */			40,
	/* Name */			"m4x4getrX(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "Matrix Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(float) <= d_vData.size() )
	{
		ReturnFloat(
			mRot2Quat(1,
			*reinterpret_cast<const float *>(&d_vData.at(p1)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+4)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+8)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+16)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+20)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+24)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+32)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+36)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+40))
			)
		);
	}
	
	return 0;
}

EXPRESSION(
	/* ID */			41,
	/* Name */			"m4x4getrY(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "Matrix Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(float) <= d_vData.size() )
	{
		ReturnFloat(
			mRot2Quat(2,
			*reinterpret_cast<const float *>(&d_vData.at(p1)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+4)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+8)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+16)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+20)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+24)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+32)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+36)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+40))
			)
		);
	}
	
	return 0;
}

EXPRESSION(
	/* ID */			42,
	/* Name */			"m4x4getrZ(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "4x4 Matrix Offset")
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(float) <= d_vData.size() )
	{
		ReturnFloat(
			mRot2Quat(3,
			*reinterpret_cast<const float *>(&d_vData.at(p1)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+4)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+8)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+16)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+20)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+24)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+32)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+36)),
			*reinterpret_cast<const float *>(&d_vData.at(p1+40))
			)
		);
	}
	
	return 0;
}


EXPRESSION(
	/* ID */			43,
	/* Name */			"FltToStr$(",
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, "Floating-Point", EXPPARAM_NUMBER, "Width", EXPPARAM_NUMBER, "Precision" )
	) {
	float p1 = ExParam(TYPE_FLOAT);
	long p2 = ExParam(TYPE_INT);
	long p3 = ExParam(TYPE_INT);

	stringstream ss;
	ss << std::internal << std::showpos << std::setw(p2) << std::setprecision(p3) << std::setfill('0') << p1;
	string str = ss.str();

	ReturnStringSafe(str.c_str());
}

//Alien Math
EXPRESSION(
	/* ID */			44,
	/* Name */			"cosh(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "Floating-Point value")
){	
	long input = ExParam(TYPE_FLOAT);
	float output = coshf(*reinterpret_cast<const float *>(&input));

	ReturnFloat(output);
}

EXPRESSION(
	/* ID */			45,
	/* Name */			"sinh(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, "Floating-Point value")
){	
	long input = ExParam(TYPE_FLOAT);
	float output = sinhf(*reinterpret_cast<const float *>(&input));

	ReturnFloat(output);
}
