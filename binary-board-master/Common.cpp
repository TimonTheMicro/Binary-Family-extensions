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

void printLog(int value,string path)
{
	std::ofstream outfile;
	outfile.open(path,std::ios_base::app); // append instead of overwrite
	outfile << std::endl << std::to_string(value);
	outfile.close();
}