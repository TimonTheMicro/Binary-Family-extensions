
// ============================================================================
//
// CONDITIONS
// 
// ============================================================================

#include "common.h"


CONDITION(
	/* ID */			0,
	/* Name */			_T("%o: Board  %0  exists"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(1, PARAM_STRING,_T("Board name"))
)
{
	string p1 = string(GetStr());
	if(!p1[0]) return 0;

	for(unsigned int i=0; i<numBoards; i++) //check if board already exists
		if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			return 1;
	return 0;
}
