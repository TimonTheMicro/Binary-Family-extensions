
// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================

#include "common.h"

/* GET BOARD */

EXPRESSION(
	/* ID */			0,
	/* Name */			_T("bName$("),
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
	/* Name */			_T("bId("),
	/* Flags */			0,
	/* Params */		(0)
) {
	if (numBoards)
	{ 
		return rdPtr->SelBoard;
	}

	return -1;
}

EXPRESSION(
	/* ID */			2,
	/* Name */			_T("bId2Name$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Board number"))
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
	/* Name */			_T("bMemLoc("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Board name"))
) {
	string p1(GetStr2());

	if(numBoards) //memory address shouldn't point to an empty space!
	{	
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			if ( strCompare(d_sNamei, p1, rdPtr->bCaseSensitive) )
				if ( d_vDatai.size() >= 16 ) //minimum will be size of 16B
					return (int)d_vDatai.data();
	}

	return 0;			
}

EXPRESSION(
	/* ID */			4,
	/* Name */			_T("bDimen("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_STRING, _T("Board name"), EXPPARAM_NUMBER, _T("Which? (0: Capacity, 1: Size)"))
) {
	string p1(GetStr2());
	bool p2 = ExParam(TYPE_INT);
	if(numBoards)
	{
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			if ( strCompare(d_sNamei, p1, rdPtr->bCaseSensitive) )
			{
				if (!p2)
				{
					return d_vDatai.capacity();
				}
				else
				{
					return d_vDatai.size();
				}
			}
	}

	return 0;
}

EXPRESSION(
	/* ID */			5,
	/* Name */			_T("bLock("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Board name"))
)
{
	string p1(GetStr2());

	for (unsigned int i=0; i<numBoards; i++) //check if board already exists
		if ( strCompare(d_sNamei, p1, rdPtr->bCaseSensitive) )
			return d_bLocki;

	return 0;
}


EXPRESSION(
	/* ID */			6,
	/* Name */			_T("bCount("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return numBoards;
}

/* GET VALUE */

EXPRESSION(
	/* ID */			7,
	/* Name */			_T("vSize("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Integer value"))
) {
	long p1 = ExParam(TYPE_INT);

	return SizeOfValue3(p1);
}

EXPRESSION(
	/* ID */			8,
	/* Name */			_T("vSign("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Value"))
	) {
	int p1 = ExParam(TYPE_FLOAT);
	float p1b = *reinterpret_cast<const float *>(&p1);
	
	if(p1b < 0)
	{
		return -1;
	}
		
		return 1;
}

EXPRESSION(
	/* ID */			9,
	/* Name */			_T("bitOfByte("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, _T("Offset"), EXPPARAM_NUMBER, _T("bit"))
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
	/* Params */		(1, EXPPARAM_NUMBER, _T("Offset"))
	) {
	off_t p1 = ExParam(TYPE_INT);

	if(numBoards && p1 < d_vData.size())
	{
		if ( !rdPtr->bEndianness )
			return (const signed char)d_vData.at(p1);
		else
			return (const unsigned char)d_vData.at(p1);	
	}
		
	return 0;
}

EXPRESSION(
	/* ID */			11,
	/* Name */			_T("uchar("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Offset"))
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1 < d_vData.size() )
		return (const unsigned char)d_vData.at(p1);
	
	return 0;
}

EXPRESSION(
	/* ID */			12,
	/* Name */			_T("short("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Offset"))
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(short) <= d_vData.size() )
	{
		if ( !rdPtr->bEndianness )
			return *reinterpret_cast<const short int*>(&d_vData.at(p1));
		else
			return _byteswap_ushort(*reinterpret_cast<const short int*>(&d_vData.at(p1)));
	}
	
	return 0;
}

EXPRESSION(
	/* ID */			13,
	/* Name */			_T("ushort("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Offset"))
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(short) <= d_vData.size() )
	{
		if ( !rdPtr->bEndianness )
			return *reinterpret_cast<const unsigned short int*>(&d_vData.at(p1));
		else
			return _byteswap_ushort(*reinterpret_cast<const unsigned short int*>(&d_vData.at(p1)));
	}
	return 0;
}

EXPRESSION(
	/* ID */			14,
	/* Name */			_T("long("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Offset"))
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(long) <= d_vData.size() )
	{
		if ( !rdPtr->bEndianness )
			return *reinterpret_cast<const long int*>(&d_vData.at(p1));
		else
			return _byteswap_ulong(*reinterpret_cast<const long int*>(&d_vData.at(p1)));
	}
	return 0;
}

EXPRESSION(
	/* ID */			15,
	/* Name */			_T("float("),
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Offset"))
	) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && p1+sizeof(float) <= d_vData.size() )
	{
		ReturnFloat(*reinterpret_cast<const float *>(&d_vData.at(p1)));
	}
	return 0;
}
#ifdef UNICODE //Checked!
EXPRESSION(
	/* ID */			16,
	/* Name */			_T("string$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_NUMBER, _T("Offset"), EXPPARAM_NUMBER, _T("Unicode? (0: No, 1: Yes)"), EXPPARAM_NUMBER, _T("Size (0: Auto)"))
) {
	size_t p1 = ExParam(TYPE_INT);
	bool p2 = ExParam(TYPE_INT);
	size_t p3 = ExParam(TYPE_INT); // 0: null-terminated

	if(numBoards)
	{
		p1 = p1<d_vData.size()?p1:d_vData.size()-1; // Q?Y:N overflow security for the argument
		if (p2)
		{
			string output = reinterpret_cast<TCHAR*>(&d_vData.at(p1));
			if (p3) output.resize(min(p3,d_vData.size())*sizeof(wchar_t));
			ReturnStringSafe(output.c_str());
		}
		else
		{
			if (!p3)
				p3 = d_vData.size()-p1;
			string output( d_vData.begin()+p1, p1+p3<d_vData.size()?d_vData.begin()+p1+p3:d_vData.end());
			ReturnStringSafe(output.c_str());
		}
	}
	ReturnString(L"");
}
#else
EXPRESSION(
	/* ID */			16,
	/* Name */			_T("string$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2, EXPPARAM_NUMBER, _T("Offset"), EXPPARAM_NUMBER, _T("Size (0: Auto)"))
) {
	size_t p1 = ExParam(TYPE_INT);
	off_t p3 = ExParam(TYPE_INT); // 0: null-terminated
	p1 = p1<d_vData.size()?p1:d_vData.size()-1; // Q?Y:N overflow security for the argument
	if(numBoards)
	{
		if (!p3)
			p3 = d_vData.size()-p1; //		IF TRUE ?			YES					NO
		string output( d_vData.begin()+p1, p1+p3<d_vData.size()?d_vData.begin()+p1+p3:d_vData.end());
		ReturnStringSafe(output.c_str());
	}
	ReturnString("");
}
#endif

EXPRESSION(
	/* ID */			17,
	/* Name */			_T("double$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Offset"))
) {
	off_t p1 = ExParam(TYPE_INT);

	if ( numBoards && (p1+sizeof(double) <= d_vData.size()) )
	{
		double output = *reinterpret_cast<const double *>(&d_vData.at(p1));
		std::ostringstream sstream;
		sstream << output;
		std::string str = sstream.str();
		string strb(str.begin(), str.end());
		ReturnStringSafe(strb.c_str());
	}

	ReturnString("");
}

EXPRESSION(
	/* ID */			18,
	/* Name */			_T("longlong$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Offset"))
) {
	size_t p1 = ExParam(TYPE_INT);
	long long output = 0;

	if (numBoards && (p1+sizeof(long long) <= d_vData.size()))
	{
		if ( !rdPtr->bEndianness )
			output = *reinterpret_cast<const long long*>(&d_vData.at(p1));
		else
			output = _byteswap_ulong(*reinterpret_cast<const long long*>(&d_vData.at(p1)));
	}

	stringstream ss;
	ss << output;
	string dec = ss.str();
	ReturnStringSafe(dec.c_str());
}

/* GET FILE */

EXPRESSION(
	/* ID */			19,
	/* Name */			_T("fileSize("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("File"))
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
	/* ID */			20,
	/* Name */			_T("lastPath$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(0)
) {
		ReturnStringSafe(rdPtr->lastPath.c_str());
}

/* GET DATE/TIME OF */

EXPRESSION(
	/* ID */			21,
	/* Name */			_T("getYear("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Hexadecimal 64bit value"))
) {
	string p1(GetStr2());
	if(!p1[0]) return 0;
	return GetDateTime(p1, 0);
}

EXPRESSION(
	/* ID */			22,
	/* Name */			_T("getMonth("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Hexadecimal 64bit value"))
) {
	string p1(GetStr2());
	if(!p1[0]) return 0;
	return GetDateTime(p1, 1);
}
EXPRESSION(
	/* ID */			23,
	/* Name */			_T("getDay("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Hexadecimal 64bit value"))
) {
	string p1(GetStr2());
	if(!p1[0]) return 0;
	return GetDateTime(p1, 2);
}
EXPRESSION(
	/* ID */			24,
	/* Name */			_T("getHour("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Hexadecimal 64bit value"))
) {
	string p1(GetStr2());
	if(!p1[0]) return 0;
	return GetDateTime(p1, 3);
}

EXPRESSION(
	/* ID */			25,
	/* Name */			_T("getMinute("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Hexadecimal 64bit value"))
) {
	string p1(GetStr2());
	if(!p1[0]) return 0;
	return GetDateTime(p1, 4);
}

EXPRESSION(
	/* ID */			26,
	/* Name */			_T("getSecond("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Hexadecimal 64bit value"))
) {
	string p1(GetStr2());
	if(!p1[0]) return 0;
	return GetDateTime(p1, 5);
}

/* OCCURENCES */
// count
EXPRESSION(
	/* ID */			27,
	/* Name */			_T("countInt("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, _T("Integer value"), EXPPARAM_NUMBER, _T("Value size"))
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
	/* ID */			28,
	/* Name */			_T("countStr("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_STRING, _T("String"), EXPPARAM_NUMBER, _T("Unicode? (0: No, 1: Yes)"))
) {
	string p1(GetStr2());
	if(!p1.length()) return 0;
	bool p3 = ExParam(TYPE_INT);
	if(numBoards)
	{
		unsigned int pos = 0, count = 0;
		if(p3) //Unicode
		{ //UTF-16 LE
			while(true)
			{
				const char *pointer = reinterpret_cast<const char*>(&p1[0]);
				size_t size = p1.size()*sizeof(p1.front());
				const auto it = std::search(d_vData.begin()+pos, d_vData.end(), pointer, pointer+size);
				pos = it-d_vData.begin();
				if(pos == d_vData.size())
					return count;
				count++;
				pos++;
			}
		}
		else
		{
			while(true)
			{
				const auto it = std::search(d_vData.begin()+pos, d_vData.end(), p1.begin(), p1.end());
				pos = it-d_vData.begin();
				if(pos == d_vData.size())
					return count;
				count++;
				pos++;
			}
		}
	}
	return 0;
}
#else
EXPRESSION(
	/* ID */			28,
	/* Name */			_T("countStr("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("String"))
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
			count++;
			pos++;
		}
	}
	return 0;
}
#endif

EXPRESSION(
	/* ID */			29,
	/* Name */			_T("countCont("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Board name"))
) {
	string p1(GetStr2());

	if(numBoards)
	{	
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			{
				long pos = 0;
				long counter = 0;
				while ( true )
				{
					auto it = std::search( d_vData.begin()+pos, d_vData.end(), d_vDatai.begin(), d_vDatai.end() );			
					pos = it - d_vData.begin();	
					if ( pos == d_vData.size() )
						break;
					counter++;
					pos += d_vDatai.size();				
				}
				return counter;
			}
	}

	return 0;
}
// FIND
// Find at beginning of given offset. In that way you can enlist them.
EXPRESSION(
	/* ID */			30,
	/* Name */			_T("findInt("),
	/* Flags */			0,
	/* Params */		(3, EXPPARAM_NUMBER, _T("Integer value"), EXPPARAM_NUMBER, _T("Value size (0: Auto)"), EXPPARAM_NUMBER, _T("Begin offset"))
) {
	long p1 = ExParam(TYPE_INT);
	unsigned char p2 = ExParam(TYPE_INT);
	size_t p3 = ExParam(TYPE_INT);

	if (numBoards)
	{
		if (!p2)
			p2 = SizeOfValue3(p1);
		const char* value = reinterpret_cast<char*>(&p1);
		int pos = 0;
		auto it = search(d_vData.begin(), d_vData.end(), value, value+p2);
		return it-d_vData.begin();
	}
	return d_vData.size();
}

#ifdef UNICODE
EXPRESSION(
	/* ID */			31,
	/* Name */			_T("findStr("),
	/* Flags */			0,
	/* Params */		(3, EXPPARAM_STRING, _T("String"), EXPPARAM_NUMBER, _T("Begin offset"), EXPPARAM_NUMBER, _T("Unicode? (0: No, 1: Yes)"))
) {
	string p1(GetStr2());
	if(!p1.length()) return 0;
	size_t p3 = ExParam(TYPE_INT);
	bool p4 = ExParam(TYPE_INT);

	if(numBoards)
	{
		size_t pos = p3;
		if(p4) //Unicode
		{ //UTF-16 LE
			const char *pointer = reinterpret_cast<const char*>(&p1[0]);
			size_t size = p1.size()*sizeof(p1.front());
			const auto it = std::search(d_vData.begin()+pos, d_vData.end(), pointer, pointer+size);
			return it-d_vData.begin();
		}
		else
		{
			const auto it = std::search(d_vData.begin()+pos, d_vData.end(), p1.begin(), p1.end());
			return it-d_vData.begin();
		}
	}
	return d_vData.size();
}
#else
EXPRESSION(
	/* ID */			31,
	/* Name */			_T("findStr("),
	/* Flags */			0,
	/* Params */		(4, EXPPARAM_STRING, _T("String"), EXPPARAM_NUMBER, _T("Begin offset"))
) {
	string p1(GetStr2());
	if(!p1.length()) return 0;
	size_t p3 = ExParam(TYPE_INT);
	if(numBoards)
	{
		size_t pos = p3;
		const auto it = std::search(d_vData.begin()+pos, d_vData.end(), p1.begin(), p1.end());
		return it-d_vData.begin();
	}
	return d_vData.size();
}
#endif
EXPRESSION(
	/* ID */			32,
	/* Name */			_T("findCont("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_STRING, _T("Board name"), EXPPARAM_NUMBER, _T("Begin offset"))
) {
	string p1(GetStr2());
	size_t p2 = ExParam(TYPE_INT);

	if(numBoards)
	{		
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			{
				long pos = p2;
				auto it = std::search( d_vData.begin()+pos, d_vData.end(), d_vDatai.begin(), d_vDatai.end() );
				return it-d_vData.begin();
			}
	}
	return d_vData.size();
}

/* CONVERSIONS */

EXPRESSION(
	/* ID */			33,
	/* Name */			_T("IntToFlt("),
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Integer value"))
){	
	long p1temp = ExParam(TYPE_INT);
	float p1 = *reinterpret_cast<const float*>(&p1temp);

	ReturnFloat(p1);
}

EXPRESSION(
	/* ID */			34,
	/* Name */			_T("FltToInt("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Floating-Point value"))
){	
	long p1temp = ExParam(TYPE_FLOAT);

	return p1temp;
}


EXPRESSION(
	/* ID */			35,
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
	/* ID */			36,
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
	/* ID */			37,
	/* Name */			_T("precision$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(3, EXPPARAM_STRING, _T("Floating-Point"), EXPPARAM_NUMBER, _T("Width"), EXPPARAM_NUMBER, _T("Precision"))
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

EXPRESSION(
	/* ID */			38,
	/* Name */			_T("LOBYTE("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Integer value"))
) {
	short input = ExParam(TYPE_INT);
	return LOBYTE(input);
}

EXPRESSION(
	/* ID */			39,
	/* Name */			_T("HIBYTE("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Integer value"))
) {
	short input = ExParam(TYPE_INT);
	return HIBYTE(input);
}

EXPRESSION(
	/* ID */			40,
	/* Name */			_T("LOWORD("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Integer value"))
) {
	long input = ExParam(TYPE_INT);
	return LOWORD(input);
}

EXPRESSION(
	/* ID */			41,
	/* Name */			_T("HIWORD("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Integer value"))
) {
	long input = ExParam(TYPE_INT);
	return HIWORD(input);
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
	/* Params */		(2, EXPPARAM_NUMBER, _T("Integer value"), EXPPARAM_NUMBER, _T("shift right by"))
) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);

	return p1 >> p2;
}


EXPRESSION(
	/* ID */			44,
	/* Name */			_T("Pattern("),
	/* Flags */			0,
	/* Params */		(2, EXPPARAM_NUMBER, _T("Integer value"), EXPPARAM_NUMBER, _T("Pattern size"))
) {
	int p1 = ExParam(TYPE_INT);
	int p2 = ExParam(TYPE_INT);

	int bitsCount = NumberOfBits(p1);
	int output = 0;

	if (bitsCount)
	{
		output = ( p1 << (p2-bitsCount) );
		for ( int i=0; i<p2/bitsCount; i++ )
			output |= p1 << (p2 - bitsCount*(i+1));
		if ( p2%bitsCount )
			output |= p1 >> bitsCount - p2%bitsCount;
	}
	return output;
}

EXPRESSION(
	/* ID */			45,
	/* Name */			_T("bPrevName$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(0)
) {
	if(numBoards) //EDIT: Added token for one code preventing crash. Weird.
	{
		string output = rdPtr->lastBoard;
		ReturnString(output.c_str()); 
	}

	ReturnString(L"");
}

EXPRESSION(
	/* ID */			46,
	/* Name */			_T("ascii2int("),
	/* Flags */			0,
	/* Params */		(1, EXPPARAM_STRING, _T("Character"))
) {
	string p1(GetStr2());
	if(!p1.length()) return 0;

	return static_cast<int>(p1[0]);
}

EXPRESSION(
	/* ID */			47,
	/* Name */			_T("int2ascii$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1, EXPPARAM_NUMBER, _T("Unsigned char"))
) {
	unsigned char p1 = ExParam(TYPE_INT);
	string str(1, char(p1));
	ReturnStringSafe(str.c_str());
}
