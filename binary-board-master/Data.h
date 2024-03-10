class rRundata;
typedef rRundata * LPRRDATA;

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// If you want to store anything between actions/conditions/expressions
// you should store it here


struct Board // a board structure
{
	bool bLock = false; // lock flag
	string sName; // name
	vector <char> vData; // container
};


typedef struct tagRDATA
{
#include "MagicRDATA.h"

	bool bEndianness;
	bool bAutoSelect;
	bool bCaseSensitive;

	unsigned int SelBoard;
	string lastBoard; //new (boards can change their ids because their vector position is not constant, so it is string)
	string lastPath; //new
	vector <Board> BOARDS; // main board initialization (vector of structs)

} RUNDATA;
typedef	RUNDATA	* LPRDATA;



// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
// These values let you store data in your extension that will be saved in the MFA
// You should use these with properties


typedef struct tagEDATAA_V1
{
	extHeader		eHeader;
	//	short			swidth;
	//	short			sheight;
	bool			bEndianness;
	bool			bAutoSelect;
	bool			bCaseSensitive;
} EDITDATAA;

typedef struct tagEDATAW_V1
{
	extHeader		eHeader;
	//	short			swidth;
	//	short			sheight;
	bool			bEndianness;
	bool			bAutoSelect;
	bool			bCaseSensitive;
} EDITDATAW;

#ifdef _UNICODE
#define EDITDATA	EDITDATAW
#define LPEDATA		LPEDATAW
#else
#define EDITDATA	EDITDATAA
#define LPEDATA		LPEDATAA
#endif

typedef EDITDATA * LPEDATA;

