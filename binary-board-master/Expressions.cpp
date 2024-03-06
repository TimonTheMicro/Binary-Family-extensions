
// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================

#include "common.h"
#include "floats.hpp" //half floating conversions

/* GET BOARD */

EXPRESSION(
	/* ID */			0,
	/* Name */			_T("CurBoardName$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(0)
) {
	if(numBoards) //what the actual ***k?! There is no boards but it crashes. EDIT: Added token for one code preventing crash.
	{ //One line under braces but it worked differently than without. Weird.
		ReturnString((d_sName.c_str())); 
	}

	ReturnString(L"");
}

EXPRESSION(
	/* ID */			1,
	/* Name */			_T("curBoardId("),
	/* Flags */			0,
	/* Params */		(0)
) {
	if (numBoards)
	{ 
		return rdPtr->iSelBoard;
	}

	return -1;
}

EXPRESSION(
	/* ID */			2,
	/* Name */			_T("NameFromId$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Board number"))
) {
	unsigned long i = ExParam(TYPE_INT);

	if ( i < numBoards )
	{
		ReturnString(d_sNamei.c_str());
	}

	ReturnString("");
}

EXPRESSION(
	/* ID */			3,
	/* Name */			_T("BoardMemLoc("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,_T("Board name"))
) {
	string p1(GetStr2());

	if ( numBoards ) //memory address shouldn't point to an empty space!
	{	
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			if ( strCompare(d_sNamei, p1) )
				if ( d_vDatai.size() >= 16 ) //minimum will be size of 16B
					return (int)d_vDatai.data();
	}

	return 0;			
}

EXPRESSION(
	/* ID */			4,
	/* Name */			_T("BoardContSize("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,_T("Board name"))
) {
	string p1(GetStr2());

	if ( numBoards )
	{		
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			   if ( strCompare(d_sNamei, p1) )
				return d_vDatai.size();
	}

	return 0;
}

EXPRESSION(
	/* ID */			5,
	/* Name */			_T("BoardLockFlag("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("Board name"))
)
{
	string p1(GetStr2());

	for (unsigned int i=0; i<numBoards; i++) //check if board already exists
		if ( strCompare(d_sNamei, p1) )
			return d_bProtectedi;

	return 0;
}


EXPRESSION(
	/* ID */			6,
	/* Name */			_T("BoardCount("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return numBoards;
}

/* GET VALUE */

EXPRESSION(
	/* ID */			7,
	/* Name */			_T("IntSize("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Integer value"))
) {
	long p1 = ExParam(TYPE_INT);

	if ( p1 < 0 || p1 > MAXWORD )
		return sizeof(long);

	if ( p1 <= MAXBYTE )
		return sizeof(char);

	return sizeof(short);
}

EXPRESSION(
	/* ID */			8,
	/* Name */			_T("IntSign("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Value"))
	) {
	off_t p1tmp = ExParam(TYPE_FLOAT);
	float p1 = *reinterpret_cast<const float *>(&p1tmp);
	
	if(p1 < 0)
	{
		return -1;
	}
		
		return 1;
}

EXPRESSION(
	/* ID */			9,
	/* Name */			_T("bitOfChar("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER,_T("Offset"), EXPPARAM_NUMBER,_T("bit"))
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
	/* ID */			10,
	/* Name */			_T("char("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Offset"))
	) {
	off_t p1 = ExParam(TYPE_INT);

	if(numBoards && p1 < d_vData.size())
	{
		return (char)d_vData.at(p1);
	}
		
	
	return 0;
}

EXPRESSION(
	/* ID */			11,
	/* Name */			_T("uchar("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Offset"))
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1 < d_vData.size() )
		return (unsigned char)d_vData.at(p1);
	
	return 0;
}

EXPRESSION(
	/* ID */			12,
	/* Name */			_T("short("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Offset"))
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
	/* ID */			13,
	/* Name */			_T("ushort("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Offset"))
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
	/* ID */			14,
	/* Name */			_T("long("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Offset"))
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
	/* ID */			15,
	/* Name */			_T("float("),
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Offset"))
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(float) <= d_vData.size() )
	{
		ReturnFloat(*reinterpret_cast<const float *>(&d_vData.at(p1)));
	}
	return 0;
}

EXPRESSION(
	/* ID */			16,
	/* Name */			_T("string$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_NUMBER,_T("Offset"), EXPPARAM_NUMBER,_T("Size"))
	) {
	off_t p1 = ExParam(TYPE_INT);
	size_t p2 = ExParam(TYPE_INT);

	if ( numBoards && p2 )
	{
		if ( p2>d_vData.size() )
			p2 = d_vData.size();
		string output( d_vData.begin()+p1, p1+p2>d_vData.size() ? d_vData.end():d_vData.begin()+p1+p2 );
		ReturnString(output.c_str());
	}
	ReturnString("");
}


EXPRESSION(
	/* ID */			17,
	/* Name */			_T("double$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Offset"))
) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(float) <= d_vData.size() )
	{
		ReturnString(*reinterpret_cast<const double *>(&d_vData.at(p1)));
	}

	ReturnString("");
}

EXPRESSION(
	/* ID */			18,
	/* Name */			_T("longlong$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Offset"))
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

	std::stringstream ss;
	ss << std::right << std::hex << output; // << std::endl
	std::string hex = ss.str();
	std::transform(hex.begin(), hex.end(),hex.begin(), ::toupper);
	hex = "0x"+hex;
	ReturnString( hex.c_str());
}

EXPRESSION(
	/* ID */			19,
	/* Name */			_T("LOBYTE("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Integer value"))
) {
	short input = ExParam(TYPE_INT);
	return LOBYTE(input);
}

EXPRESSION(
	/* ID */			20,
	/* Name */			_T("HIBYTE("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Integer value"))
) {
	short input = ExParam(TYPE_INT);
	return HIBYTE(input);
}

EXPRESSION(
	/* ID */			21,
	/* Name */			_T("LOWORD("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Integer value"))
) {
	long input = ExParam(TYPE_INT);
	return LOWORD(input);
}

EXPRESSION(
	/* ID */			22,
	/* Name */			_T("HIWORD("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Integer value"))
) {
	long input = ExParam(TYPE_INT);
	return HIWORD(input);
}

/* GET FILE */

EXPRESSION(
	/* ID */			23,
	/* Name */			_T("fileSize("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("File"))
) {
	string p1(GetStr2());
	if(!p1[0]) return 0;

	std::ifstream file( p1,std::ios::binary | std::ios::ate);

	if ( file.is_open() )
	{
		const long size = file.tellg();
		file.close();
		return size;
	}	
	
	return 0;
}

EXPRESSION(
	/* ID */			24,
	/* Name */			_T("lastPath$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(0)
) {
		ReturnStringSafe(rdPtr->lastPath.c_str());
}

/* GET DATE/TIME OF */

EXPRESSION(
	/* ID */			25,
	/* Name */			_T("getYear("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("Hexadecimal 64bit value"))
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
	std::istringstream iss(p1);
	iss >> std::hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return (1900+time.tm_year);
}

EXPRESSION(
	/* ID */			26,
	/* Name */			_T("getMonth("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("Hexadecimal 64bit value"))
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
	std::istringstream iss(p1);
	iss >> std::hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_mon;
}
EXPRESSION(
	/* ID */			27,
	/* Name */			_T("getDay("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("Hexadecimal 64bit value"))
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
	std::istringstream iss(p1);
	iss >> std::hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_mday;
}
EXPRESSION(
	/* ID */			28,
	/* Name */			_T("getHour("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("Hexadecimal 64bit value"))
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
	std::istringstream iss(p1);
	iss >> std::hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_hour;
}
EXPRESSION(
	/* ID */			29,
	/* Name */			_T("getMinute("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("Hexadecimal 64bit value"))
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
	std::istringstream iss(p1);
	iss >> std::hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_min;
}
EXPRESSION(
	/* ID */			30,
	/* Name */			_T("getSecond("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("Hexadecimal 64bit value"))
) {
	LPSTR p1 = (LPSTR)ExParam(TYPE_STRING);

	long long value;
	std::istringstream iss(p1);
	iss >> std::hex >> value;

	time_t timestamp = value;
	struct tm time;
	char date_time[256];
	localtime_s(&time, &timestamp);
	//asctime_s(date_time, sizeof(date_time), &time);

	return time.tm_sec;
}

/* OCCURENCES */
// count
EXPRESSION(
	/* ID */			31,
	/* Name */			_T("countInt("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER,_T("Integer value"), EXPPARAM_NUMBER,_T("Value size"))
) {
	long p1 = ExParam(TYPE_INT);
	char p2 = ExParam(TYPE_INT);

	if(numBoards)
	{
		const char * value = reinterpret_cast<char *>(&p1);
		unsigned int pos = 0, count = 0;
		while(true)
		{
			const auto it = std::search(d_vData.begin()+pos, d_vData.end(),value, value+sizeof(char)*p2);
			pos = it-d_vData.begin();
			if(pos == d_vData.size())
				return count;
			pos++;
			count++;
		}
	}
	return 0;
}
#ifdef UNICODE
EXPRESSION(
	/* ID */			32,
	/* Name */			_T("countStr("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_STRING,_T("String"), EXPPARAM_NUMBER,_T("Unicode? (0: No, 1: Yes"))
) {
	string p1(GetStr2());
	if(!p1.length()) return 0;
	bool p2 = ExParam(TYPE_INT);
	if(numBoards)
	{
		unsigned int pos = 0, count = 0;
		while(true)
		{
			if(p2) //Unicode
			{ //UTF-16 LE
				const char *pointer = reinterpret_cast<const char*>(&p1[0]);
				size_t size = p1.size()*sizeof(p1.front());
				const auto it = std::search(d_vData.begin()+pos, d_vData.end(),pointer,pointer+size);
				pos = it-d_vData.begin();
				if(pos == d_vData.size())
					return count;
				pos++;
				count++;
			}
			else
			{
				const auto it = std::search(d_vData.begin()+pos, d_vData.end(),p1.begin(),p1.end());
				pos = it-d_vData.begin();
				if(pos == d_vData.size())
					return count;
				pos++;
				count++;
			}
		}
	}
	return 0;
}
#else
EXPRESSION(
	/* ID */			32,
	/* Name */			_T("countStr("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("String"))
) {
	string p1(GetStr2());
	if(!p1.length()) return 0;

	if(numBoards)
	{
		unsigned int pos = 0, count = 0;
		while(true)
		{
			const auto it = std::search(d_vData.begin()+pos, d_vData.end(),p1.begin(),p1.end());
			pos = it-d_vData.begin();
			if(pos == d_vData.size())
				return count;
			pos++;
			count++;
		}
	}
	return 0;
}
#endif

EXPRESSION(
	/* ID */			33,
	/* Name */			_T("countCont("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING,_T("Board name"))
) {
	string p1(GetStr2());

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
// FIND
EXPRESSION(
	/* ID */			34,
	/* Name */			_T("findInt("),
	/* Flags */			0,
	/* Params */		(3, EXPPARAM_NUMBER,_T("Integer value"), EXPPARAM_NUMBER,_T("Value size"), EXPPARAM_NUMBER,_T("Occurrence"))
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

EXPRESSION( //to modify. Find since beginning of offset. Then give offset of found. In that way you can enlist them.
	/* ID */			35,
	/* Name */			_T("findStr("),
	/* Flags */			0,
	/* Params */		(4, EXPPARAM_STRING,_T("String"), /*EXPPARAM_NUMBER,_T("Case sensitive"), */EXPPARAM_NUMBER,_T("Occurrence"),EXPPARAM_NUMBER,_T("Begins with NULL (0 for false, 1 for true)"),EXPPARAM_NUMBER,_T("Ends with NULL (0 for false, 1 for true)"))
) {
	string p1(GetStr2());
	long p2 = ExParam(TYPE_INT);
	bool p3 = ExParam(TYPE_INT);
	bool p4 = ExParam(TYPE_INT);

	if ( numBoards && p2 )
	{
		long dist = 0;
		while ( p2 && dist != d_vData.size() )
		{
			/*
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
				*/
		}
	}
	
	return -1;
}

EXPRESSION(
	/* ID */			36,
	/* Name */			_T("findCont("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_STRING,_T("Board name"), EXPPARAM_NUMBER,_T("Occurrence"))
) {
	string p1(GetStr2());
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

/* CONVERSIONS */

EXPRESSION(
	/* ID */			37,
	/* Name */			_T("IntToFlt("),
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER,_T("Integer value"))
){	
	long p1temp = ExParam(TYPE_INT);
	float p1 = *reinterpret_cast<const float*>(&p1temp);

	ReturnFloat(p1);
}

EXPRESSION(
	/* ID */			38,
	/* Name */			_T("FltToInt("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Floating-Point value"))
){	
	long p1temp = ExParam(TYPE_FLOAT);

	return p1temp;
}


EXPRESSION(
	/* ID */			39,
	/* Name */			_T("IntToHflt("),
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Integer value"))
) {
	short input = ExParam(TYPE_INT);
	float output;

	FP32 float32;
	FP16 float16;
	float16.u = input;
	float32 = half_to_float(float16);
	output = float32.f;

	ReturnFloat(output);
}


EXPRESSION(
	/* ID */			40,
	/* Name */			_T("HfltToInt("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Floating-Point value"))
){	
	int p1temp = ExParam(TYPE_FLOAT);
	float value = *(float*)&p1temp;

	FP32 float32;
	float32.f = value;
	FP16 float16;
	float16 = float_to_half_full(float32);
	return (WORD)float16.u;
}


EXPRESSION(
	/* ID */			41,
	/* Name */			_T("precision$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3,EXPPARAM_STRING,_T("Floating-Point"),EXPPARAM_NUMBER,_T("Width"),EXPPARAM_NUMBER,_T("Precision"))
) {
	string p1(GetStr2());
	if(!p1.length()) return 0;
	long p2 = ExParam(TYPE_INT);
	long p3 = ExParam(TYPE_INT);
	std::stringstream ss;
	std::string input(p1.begin(), p1.end()); //to string
	long double fvalue = stold(input);
	ss << std::internal << std::setw(p2) << std::setprecision(p3) << std::setfill('0') << fvalue;
	std::string cstr = ss.str();
	string output(cstr.begin(),cstr.end());

	ReturnStringSafe(output.c_str());
}

/* BITWISE OPERATIONS */

EXPRESSION(
	/* ID */			42,
	/* Name */			_T("shl("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, _T("Integer value"), EXPPARAM_NUMBER, _T("shift left by"))
) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);

	return p1 << p2;
}

EXPRESSION(
	/* ID */			43,
	/* Name */			_T("shr("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("Integer value"),EXPPARAM_NUMBER,_T("shift right by"))
) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);

	return p1 >> p2;
}
