// Include guard
#ifndef COMMON_H
#define COMMON_H

using namespace std;

// Suppress the deprecated warnings for VC2005
#define _CRT_SECURE_NO_WARNINGS

// General includes
#include	"TemplateInc.h"

// Specific to this extension
#include	"Resource.h"
#include	"FlagsPrefs.h"
#include	"Information.h"
#include	"Data.h"

// rTemplate include
#include	"rTemplate.h"

// Binary Board object
#include	<assert.h>
#include	<stdio.h>

#include	<algorithm>
#include	<fstream>
#include	<string>

// zlib
#include	"zlib.h"
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

// blowfish
#include	"blowfish.h"

// base64
#include	"base64.h"

// Globals and prototypes
extern HINSTANCE hInstLib;
extern short * conditionsInfos;
extern short * actionsInfos;
extern short * expressionsInfos;
extern long (WINAPI ** ConditionJumps)(LPRDATA rdPtr, long param1, long param2);
extern short (WINAPI ** ActionJumps)(LPRDATA rdPtr, long param1, long param2);
extern long (WINAPI ** ExpressionJumps)(LPRDATA rdPtr, long param);
extern PropData Properties[];
extern WORD DebugTree[];

#define GetInt() CNC_GetIntParameter(rdPtr)
#define GetParam() CNC_GetParameter(rdPtr)
#define GetStr() (TCHAR*)CNC_GetStringParameter(rdPtr)
#define LoadFloat(foo) 	{ long tmp = CNC_GetFloatParameter(rdPtr); foo = *reinterpret_cast<float*>(&tmp); }

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

using namespace std;

bool strCompare( string a, string b, bool caseSensitive );
// End include guard
#endif


