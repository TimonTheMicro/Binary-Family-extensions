// Include guard
#ifndef COMMON_H
#define COMMON_H

// Suppress the deprecated warnings for VC2005
#define _CRT_SECURE_NO_WARNINGS
#define OPTIMIZED_ACTION
#define _CFCFILTERMGR_UNICODE_DEFS

#define	MAX_IMAGES	16
#define DEF_WIDTH	320
#define DEF_HEIGHT	240

//#define	CUSTOM_BACKSAVE
#define FORCE_FALSE 2


// General includes
#include	"TemplateInc.h"
#include	<string>
#include	<vector>
#include	<map>
#include	<fstream>
#include	<iostream>
#include	<cstdlib>

using std::vector;
using std::map;
using std::cin;
using std::cout;
using std::pair;
using std::swap;
#include	<bitset>
#include	<algorithm>
#include	<iterator>
#include	<set>
#include	<string>
#include	<vector>
#include	<map>
#include	<fstream>
#include	<iostream>
#include	<sstream>
#include	<iomanip>
#include	<stdlib.h>  
#include	<assert.h>
#include	<stdio.h>
#include	<limits.h>
#include	<time.h>
#include	<comdef.h>  // you will need this (unicode convertors)

#ifdef UNICODE
using std::wstring;
typedef wstring string;
#else
using std::string;
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
#include    <locale>
#include    <codecvt>

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
#define d_vData rdPtr->vBoards[rdPtr->iSelBoard].vData
#define d_vDatai rdPtr->vBoards[i].vData
#define d_vDataj rdPtr->vBoards[j].vData
#define d_sName rdPtr->vBoards[rdPtr->iSelBoard].sName
#define d_sNamei rdPtr->vBoards[i].sName
#define d_sNamej rdPtr->vBoards[j].sName
#define d_bProtected rdPtr->vBoards[rdPtr->iSelBoard].bProtected
#define d_bProtectedi rdPtr->vBoards[i].bProtected
#define d_bProtectedj rdPtr->vBoards[j].bProtected
#define numBoards rdPtr->vBoards.size()

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


inline bool strCompare(string a, string b)
{
	unsigned int sz = a.size();

	if(b.size() != sz)
		return false;

	for(unsigned int i=0; i < sz; ++i)
		if(tolower(a[i]) != tolower(b[i]))
			return false;

	return true;
}

inline void printLog(int value,string path)
{
	std::ofstream outfile;
	outfile.open(path,std::ios_base::app); // append instead of overwrite
	outfile << std::endl << std::to_string(value);
	outfile.close();
}

inline void printLogWStr(wstring value,wstring path)
{
	std::wofstream outfile;
	outfile.open(path,std::ios_base::app); // append instead of overwrite
	outfile << std::endl << value;
	outfile.close();
}


inline unsigned short rowSize(unsigned int width,unsigned char bpp)
{
	return ((bpp * width + 31) / 32) * 4;
}

/*
inline std::string wstrToStr(string input)
{
//setup converter
using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type,wchar_t> converter;

//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
std::string converted_str = converter.to_bytes(input);
return converted_str;
}
*/
inline int swap(int color)
{
	char * bytes = (char *)&color;

	bytes[3] = bytes[0];
	bytes[0] = bytes[2];
	bytes[2] = bytes[3];

	return color;
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
