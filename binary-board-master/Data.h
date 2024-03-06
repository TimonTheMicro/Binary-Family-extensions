

enum //FillType
{
	FILL_NONE,

	//Inbuilt
	FILL_COLOR,
	FILL_GRADIENT,
	FILL_MOSAIC,

	//Custom
	FILL_USER,
	FILL_RADIAL,

	//Image flag
	FILL_IMAGE = 0xf0000000
};

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// If you want to store anything between actions/conditions/expressions
// you should store it here

//#pragma pack(push, 1)
struct Board
{
	bool bProtected;
	string sName;
	vector <char> vData;
};
//#pragma pack(pop)

class rRundata;
typedef rRundata * LPRRDATA;

typedef struct tagRDATA
{
#include "MagicRDATA.h"

	bool bEndianness;
	bool bAutoSelect;
	bool bStrCmp;

	unsigned int iSelBoard;
	string lastBoard; //new (boards can change their ids because their vector position is not constant, so it is string)
	string lastPath; //new
	vector <Board> vBoards; //vector of structs

} RUNDATA;
typedef	RUNDATA	* LPRDATA;



// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
// These values let you store data in your extension that will be saved in the MFA
// You should use these with properties

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
	bool			bStrCmp;
} EDITDATAA;

typedef struct tagEDATAW_V1
{
	extHeader		eHeader;
	//	short			swidth;
	//	short			sheight;
	bool			bEndianness;
	bool			bAutoSelect;
	bool			bStrCmp;
} EDITDATAW;

#ifdef _UNICODE
#define EDITDATA	EDITDATAW
#define LPEDATA		LPEDATAW
#else
#define EDITDATA	EDITDATAA
#define LPEDATA		LPEDATAA
#endif

typedef EDITDATA * LPEDATA;

