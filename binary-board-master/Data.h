

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
	wstring sName;
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


//Callback
class CFillUser : public CFillData
{
public:
	RUNDATA*	rdPtr;
	TCHAR*	fillName;

public:
	CFillUser(RUNDATA* __rdPtr,const TCHAR* name);
	~CFillUser();
	virtual BOOL Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack=FALSE);
};

//Radial gradient
class CFillRadial : public CFillData
{
public:
	COLORREF	m_crFrom;
	COLORREF	m_crTo;

public:
	CFillRadial(COLORREF crFrom, COLORREF crTo);
	void SetColors(COLORREF crFrom, COLORREF crTo);
	virtual BOOL Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack=FALSE);
};

//Linear gradient
class CFillDirEx : public CFillData
{
public:
	// Constructors
	CFillDirEx(COLORREF crFrom, COLORREF crTo, BOOL Vertical);

public:
	int m_xOrg;
	int m_yOrg;
	// Attributes
	COLORREF	m_crFrom;
	COLORREF	m_crTo;
	BOOL		m_vert;

public:
	void SetColors(COLORREF crFrom, COLORREF crTo);
	void SetDir(BOOL vert);
	// Implementation
	virtual BOOL Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack=FALSE);
};