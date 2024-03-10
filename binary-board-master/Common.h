// Include guard
#ifndef COMMON_H
#define COMMON_H

// Suppress the deprecated warnings for VC2005
#define _CRT_SECURE_NO_WARNINGS
#define OPTIMIZED_ACTION
#define _CFCFILTERMGR_UNICODE_DEFS

//#define	CUSTOM_BACKSAVE
#define FORCE_FALSE 2

// General includes
#include	"TemplateInc.h"
#include	<bitset>
#include	<algorithm>
#include	<iterator>
#include	<vector>
#include	<map>
#include	<string>
#include	<fstream>
#include	<sstream>
#include	<iostream>
#include	<iomanip>
#include	<assert.h>
#include	<limits.h>
#include	<time.h>
#include	<cctype>    // std::tolower

using std::vector;
using std::map;
using std::cin;
using std::cout;
using std::pair;
using std::swap;

#ifdef UNICODE
using std::wstring;
typedef wstring string;

using std::wofstream;
typedef wofstream ofstream;

using std::wstringstream;
typedef wstringstream stringstream;

using std::wifstream;
typedef wifstream ifstream;
#else
using std::string;
using std::ofstream;
using std::ifstream;
using std::stringstream;
#endif

// Specific to this extension
#include	"Resource.h"
#include	"FlagsPrefs.h"
#include	"Information.h"
#include	"CfcFile.h"
#include	"Data.h"
#include	"zlib.h" // zlib
#include	"blowfish.h" // blowfish
#include	"base64.h" // base64
#include	"floats.h" //half floating conversions


#define GetInt() CNC_GetIntParameter(rdPtr)
#define GetParam() CNC_GetParameter(rdPtr)
#define GetStr() (TCHAR*)CNC_GetStringParameter(rdPtr)
#define GetXPos() CNC_GetIntParameter(rdPtr)-(rdPtr->useAbs?(rdPtr->rHo.hoRect.left+rdPtr->rHo.hoAdRunHeader->rhWindowX):0)
#define GetYPos() CNC_GetIntParameter(rdPtr)-(rdPtr->useAbs?(rdPtr->rHo.hoRect.top+rdPtr->rHo.hoAdRunHeader->rhWindowY):0)
#define GetCol() Param(TYPE_GENERIC)//GetColorInline(rdPtr)//GetColorInline(rdPtr)
#define LoadFloat(foo) 	{ long tmp = CNC_GetFloatParameter(rdPtr); foo = *reinterpret_cast<float*>(&tmp); }
#define GetInt2() ExParam(TYPE_INT)
#define GetXPos2() ExParam(TYPE_INT)-(rdPtr->useAbs?(rdPtr->rHo.hoRect.left+rdPtr->rHo.hoAdRunHeader->rhWindowX):0)
#define GetYPos2() ExParam(TYPE_INT)-(rdPtr->useAbs?(rdPtr->rHo.hoRect.top+rdPtr->rHo.hoAdRunHeader->rhWindowY):0)
#define GetStr2() (TCHAR*)ExParam(TYPE_STRING)


// Binary Board object
#define d_vData rdPtr->BOARDS[rdPtr->SelBoard].vData
#define d_vDatai rdPtr->BOARDS[i].vData
#define d_vDataj rdPtr->BOARDS[j].vData
#define d_sName rdPtr->BOARDS[rdPtr->SelBoard].sName
#define d_sNamei rdPtr->BOARDS[i].sName
#define d_sNamej rdPtr->BOARDS[j].sName
#define d_bLock rdPtr->BOARDS[rdPtr->SelBoard].bLock
#define d_bLocki rdPtr->BOARDS[i].bLock
#define d_bLockj rdPtr->BOARDS[j].bLock
#define numBoards rdPtr->BOARDS.size()


/* BINARY */

inline int SizeOfValue3(int p1)
{
	if ( p1 < 0 || p1 > 0xFFFF )
		return 4;
	if ( p1 < 0x100 )
		return 1;
	return p1;
}

inline int NumberOfBits(int input)
{
	int output = 0;
	for (int i=0; i<sizeof(input)*CHAR_BIT; i++)
	{
		if ((input >> i) & 1)
			output = i+1;
	}
	return output;
}

inline void skipPadding(std::ifstream &stream)
{
	unsigned char padSize = 16;
	off_t pos = (padSize-(stream.tellg() % padSize)) % padSize;
	stream.seekg(pos+stream.tellg(),std::ios::beg);
}

inline void writePadding(std::ofstream &stream)
{
	unsigned char padSize = 16;
	off_t pos = stream.tellp();
	char padChar = 0;
	for(int i=0; i< (padSize-(pos % padSize))%padSize ; i++)
		stream.write((char*)&padChar,sizeof(char));
}

/* STRINGS */
inline size_t count(std::string const &haystack,std::string const &needle, unsigned int size)
{
	auto occurrences = 0;
	auto len = size;
	auto pos = 0;

	while(std::string::npos != (pos = haystack.find(needle,pos)))
	{
		++occurrences;
		pos += len;
	}
	return occurrences;
}

inline string strLow(string in)
{
	for(unsigned int i=0; i<i; ++i)
		in[i] = tolower(in[i]);
	return in;
}

inline bool strCompare(string ina, string inb, bool csensitive)
{
	unsigned int size = ina.size();

	if(inb.size() != size) //first phase, the size
		return false;
	if (!csensitive) //second phase, the case sensitivity
	{
		for(unsigned int i=0; i < size; ++i)
			if(tolower(ina[i]) != tolower(inb[i]))
				return false;
	}
	else
	{
		for(unsigned int i=0; i < size; ++i)
			if(ina[i] != inb[i])
				return false;
	}
	return true;
}

/* DEBUGGER */
inline void printLogInt(int value, string path)
{
	ofstream outfile;
	outfile.open(path,std::ios_base::app); // append instead of overwrite
#ifdef UNICODE
	outfile << std::endl << std::to_wstring(value);
#else
	outfile << std::endl << std::to_string(value);
#endif
	outfile.close();
}

inline void printLogString(string value, string path)
{
	ofstream outfile;
	outfile.open(path,std::ios_base::app); // append instead of overwrite
	outfile << std::endl << value;
	outfile.close();
}



#define USE_AA (rdPtr->rs.rsEffect&EFFECTFLAG_ANTIALIAS?TRUE:FALSE)

#define PARAM_OPERATOR _T("Operator (+  -  *  /  **  %%  <  >  &  |  ^  <<  >>  =)")
#define PARAM_OPERATOR_OPTIONAL _T("Operator (Empty: None, +  -  *  /  **  %%  <  >  &  |  ^  <<  >>  =)")

// rTemplate include
#include	"rTemplate.h"

// Globals and prototypes
extern HINSTANCE hInstLib;
extern short * conditionsInfos;
extern short * actionsInfos;
extern short * expressionsInfos;
extern long (WINAPI ** ConditionJumps)(LPRDATA rdPtr,long param1,long param2);
extern short (WINAPI ** ActionJumps)(LPRDATA rdPtr,long param1,long param2);
extern long (WINAPI ** ExpressionJumps)(LPRDATA rdPtr,long param);
extern PropData Properties[];
extern WORD DebugTree[];

// End include guard
#endif
