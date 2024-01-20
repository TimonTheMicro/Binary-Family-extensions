#include "Common.h"

bool strCompare( string a, string b )
{
	unsigned int sz = a.size();

	if (b.size() != sz)
		return false;
	
		for (unsigned int i=0; i < sz; ++i)
			if ( tolower(a[i]) != tolower(b[i]) )
				return false;
	
	return true;
}
