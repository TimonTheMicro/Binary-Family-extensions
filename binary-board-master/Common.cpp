#include "Common.h"

bool strCompare( string a, string b, bool caseSensitive )
{
	unsigned int sz = a.size();

	if (b.size() != sz)
		return false;
	
	if ( caseSensitive && a != b )
		return false;

	for (unsigned int i=0; i < sz; ++i)
		if ( tolower(a[i]) != tolower(b[i]) )
			return false;
	
	return true;
}
