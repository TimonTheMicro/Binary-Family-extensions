
// ============================================================================
//
// CONDITIONS
// 
// ============================================================================

#include "common.h"


CONDITION(
	/* ID */			0,
	/* Name */			("%o: Board  %0  exists"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		( 1, PARAM_STRING,("Board name") )
)
{
	string p1(GetStr());

	for (unsigned int i=0; i<numBoards; i++) //check if board already exists
		if ( strCompare(d_sNamei, p1, rdPtr->bStrCmp) )
			return 1;
	return 0;
}

CONDITION(
	/* ID */			1,
	/* Name */			("%o: Board  %0  protected"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		( 1,PARAM_STRING,("Board name") )
)
{
	string p1(GetStr());

	for (unsigned int i=0; i<numBoards; i++) //check if board already exists
		   if ( strCompare(d_sNamei, p1, rdPtr->bStrCmp) )
			return d_bProtectedi;
	return 0;
}
