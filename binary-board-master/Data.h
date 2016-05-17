class rRundata;
typedef rRundata * LPRRDATA;

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// If you want to store anything between actions/conditions/expressions
// you should store it here

#ifndef DATA_H
#define DATA_H

#include <vector>

struct Board
{
	bool bProtected = false;
	string sName;
	vector <char> vData;
};

#endif /* DATA_H */

typedef struct tagRDATA
{
	#include "MagicRDATA.h"

	bool bEndianness;
	bool bAutoSelect;
	bool bStrCmp;

	unsigned int iSelBoard;
	vector <Board> vBoards;

} RUNDATA;
typedef	RUNDATA	* LPRDATA;

// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
// These values let you store data in your extension that will be saved in the MFA
// You should use these with properties

typedef struct tagEDATA_V1
{
	extHeader		eHeader;
//	short			swidth;
//	short			sheight;
	bool			bEndianness;
	bool			bAutoSelect;
	bool			bStrCmp;

} EDITDATA;
typedef EDITDATA * LPEDATA;

