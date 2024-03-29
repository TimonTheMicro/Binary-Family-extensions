/// =====================================================================================
//
// The following routines are used internally by MMF, and should not need to be modified
// 
// =====================================================================================

#include "common.h"

HINSTANCE hInstLib;
EXT_INIT()

// ============================================================================
//
// LIBRARY ENTRY & QUIT POINTS
// 
// ============================================================================

// -----------------
// Entry points
// -----------------
// Usually you do not need to do any initialization here: it is preferable to
// do it in "Initialize" found in Edittime.cpp

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	conditionsInfos = getConditionInfos();
	actionsInfos = getActionInfos();
	expressionsInfos = getExpressionInfos();
	
	ConditionJumps = getConditions();
	ActionJumps = getActions();
	ExpressionJumps = getExpressions();
	
	switch (dwReason)
	{
		// DLL is attaching to the address space of the current process.
		case DLL_PROCESS_ATTACH:
			
			hInstLib = hDLL; // Store HINSTANCE
			break;

		// A new thread is being created in the current process.
		case DLL_THREAD_ATTACH:
			break;

		// A thread is exiting cleanly.
		case DLL_THREAD_DETACH:
			break;

		// The calling process is detaching the DLL from its address space.
	    case DLL_PROCESS_DETACH:
			break;
	}
	
	return TRUE;
}

// -----------------
// Initialize
// -----------------
// Where you want to do COLD-START initialization.
// Called when the extension is loaded into memory.
//
extern "C" int WINAPI DLLExport Initialize(mv _far *mV, int quiet)
{
	// No error
	return 0;
}

// -----------------
// Free
// -----------------
// Where you want to kill and initialized data opened in the above routine
// Called just before freeing the DLL.
// 
extern "C" int WINAPI DLLExport Free(mv _far *mV)
{
	// No error
	return 0;
}

// ============================================================================
//
// GENERAL INFO
// 
// ============================================================================

// -----------------
// Get Infos
// -----------------
// 
extern "C" 
{
	DWORD WINAPI DLLExport GetInfos(int info)
	{
		switch (info)
		{
			case KGI_VERSION:
				return EXT_VERSION2;
			case KGI_PLUGIN:
				return EXT_PLUGIN_VERSION1;
			case KGI_PRODUCT:
				return ForVersion;
			case KGI_BUILD:
				return MinimumBuild;
			case KGI_UNICODE:
				#ifdef _UNICODE
					return TRUE;
				#else
					return FALSE;
				#endif
			default:
				return 0;
		}
	}
}

// ----------------------------------------------------------
// GetRunObjectInfos
// ----------------------------------------------------------
// Fills an information structure that tells MMF2 everything
// about the object, its actions, conditions and expressions
// 

short WINAPI DLLExport GetRunObjectInfos(mv _far *mV, fpKpxRunInfos infoPtr)
{
	infoPtr->conditions = (LPBYTE)ConditionJumps;
	infoPtr->actions = (LPBYTE)ActionJumps;
	infoPtr->expressions = (LPBYTE)ExpressionJumps;

	infoPtr->numOfConditions = Conditions.size();
	infoPtr->numOfActions = Actions.size();
	infoPtr->numOfExpressions = Expressions.size();

	infoPtr->editDataSize = sizeof(EDITDATA);
	
	MagicFlags(infoPtr->editFlags);

	infoPtr->windowProcPriority = WINDOWPROC_PRIORITY;

	MagicPrefs(infoPtr->editPrefs);

	infoPtr->identifier = IDENTIFIER;
	infoPtr->version = ObjectVersion;
	
	return TRUE;
}

// ----------------------------------------------------------
// GetDependencies
// ----------------------------------------------------------
// Returns the name of the external modules that you wish MMF to include
// with stand-alone applications (these modules must be in the MMF
// Data\Runtime folder).
//

LPCSTR* WINAPI DLLExport GetDependencies()
{
	// Do some rSDK stuff
	#include "rGetDependencies.h"
	
	//LPCSTR szDep[] = {
	//	"MyDll.dll",
	//	NULL
	//};

	return NULL; // szDep;
}

// -----------------
// LoadObject
// -----------------
// Routine called for each object when the object is read from the MFA file (edit time)
// or from the CCN or EXE file (run time).
// You can load data here, reserve memory etc...
//
int	WINAPI DLLExport LoadObject(mv _far *mV, LPCSTR fileName, LPEDATA edPtr, int reserved)
{
	return 0;
}

// -----------------
// UnloadObject
// -----------------
// The counterpart of the above routine: called just before the object is
// deleted from the frame.
//
void WINAPI DLLExport UnloadObject(mv _far *mV, LPEDATA edPtr, int reserved)
{
}

// --------------------
// UpdateEditStructure
// --------------------
// For you to update your object structure to newer versions
// Called at both edit time and run time
// 

HGLOBAL WINAPI DLLExport UpdateEditStructure(mv __far *mV, void __far * OldEdPtr)
{
#ifdef UNICODE

	// Update ANSI to Unicode EDITDATA
	if (!mvIsUnicodeApp(mV, mV->mvEditApp))
	{
		if(HGLOBAL hgNew = GlobalAlloc(GPTR, sizeof(EDITDATAW)))
		{
			EDITDATAA* ansiPtr = (EDITDATAA*)OldEdPtr;
			EDITDATAW* edPtr = (EDITDATA*)GlobalLock(hgNew);
			memcpy(&edPtr->eHeader, &ansiPtr->eHeader, sizeof(extHeader));
			edPtr->eHeader.extVersion = ObjectVersion;
			edPtr->eHeader.extSize = sizeof(EDITDATAW);
	
			// Copy non-text stuff
			//memcpy(&edPtr->width, &ansiPtr->width, (size_t)&ansiPtr->textFont.lfFaceName[0] - (size_t)&ansiPtr->width);
			//edPtr->textColor = ansiPtr->textColor;
			//edPtr->textFlags = ansiPtr->textFlags;

			// Convert font face
			//MultiByteToWideChar(mvGetAppCodePage(mV, mV->mvEditApp), 0, ansiPtr->textFont.lfFaceName, -1, edPtr->textFont.lfFaceName, LF_FACESIZE);

			GlobalUnlock(hgNew);
			return (HGLOBAL)hgNew;
		}
	}
#endif

	return 0;
}

// --------------------
// UpdateFileNames
// --------------------
// If you store file names in your datazone, they have to be relocated when the
// application is moved to a different directory: this routine does it.
// Called at edit time and run time.
//
// Call lpfnUpdate to update your file pathname (refer to the documentation)
// 
void WINAPI DLLExport UpdateFileNames(mv _far *mV, LPSTR appName, LPEDATA edPtr, void (WINAPI * lpfnUpdate)(LPSTR, LPSTR))
{
}

// ---------------------
// EnumElts
// ---------------------
//
// Uncomment this function if you need to store an image in the image bank.
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.
//
