
// ============================================================================
//
// ACTIONS
// 
// ============================================================================

#include "common.h"

/* BOARD */

ACTION(
	/* ID */            0,
	/* Name */          ("%o: Create a board %0, size %1 Bytes"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_STRING,("Board name"), PARAM_NUMBER,("Size (in Bytes)") )
)
{
	string p1 = GetStr();
    size_t p2 = GetInt();

	if ( p1.length() ) //length
	{
		bool exists = false;
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			if ( strCompare(d_sNamei, p1) )
			{
				exists = true;
				break;
			}

			if ( !exists )
			{
				rdPtr->vBoards.push_back(Board());
				rdPtr->vBoards[numBoards-1].sName = p1;
				if (p2)
					rdPtr->vBoards[numBoards-1].vData.resize(p2);
				if (rdPtr->bAutoSelect)
					rdPtr->iSelBoard = numBoards-1;
			}
	}
}


ACTION(
	/* ID */            1,
	/* Name */          ("%o: Select board %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_STRING,("Board name") )
) {
	string p1(GetStr());

	if ( numBoards && p1.length() )
	{		
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			if ( strCompare(d_sNamei, p1) )
			{
				rdPtr->iSelBoard = i;
					
				break; //i = numBoards;
			}
	}
}

ACTION(
	/* ID */            2,
	/* Name */          ("%o: Rename current board to %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_STRING,("Board new name") )
) {
	string p1(GetStr());
	
	if ( numBoards && !d_bProtected && !p1.empty() )
	{
		
		bool exists = false;
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			   if ( strCompare(d_sNamei, p1) )
			{
				exists = true;
				break;
			}
		
		if ( !exists )
			d_sName = p1;
	}
}

ACTION(
	/* ID */            3,
	/* Name */          ("%o: Resize current board to %0 Bytes"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Size (in Bytes)") )
) {
	size_t p1 = GetInt();

	if ( numBoards && !d_bProtected )
		if (p1 != -1)
		{
				if ( p1 <= d_vData.size() )
					d_vData.reserve(p1);

				d_vData.resize(p1);
				d_vData.shrink_to_fit();
		}
}

ACTION(
	/* ID */            4,
	/* Name */          ("%o: Delete current board"),
	/* Flags */         0,
	/* Params */        ( 0 )
) {
	if ( numBoards && !d_bProtected )
	{
		d_sName = "";
		//rdPtr->vBoards.erase( rdPtr->vBoards.begin()+rdPtr->iSelBoard, rdPtr->vBoards.begin()+rdPtr->iSelBoard+1 );
		d_vData.clear();
		d_vData.shrink_to_fit();

		if (rdPtr->iSelBoard == numBoards)
			rdPtr->iSelBoard--;
	}
}

ACTION(
	/* ID */            5,
	/* Name */          ("%o: Set current board protected flag to %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Protected mode (0: No, 1: Yes)") )

) {
	bool p1 = GetInt();

	if ( numBoards )
		d_bProtected = p1;
}

/* DATA */


ACTION(
	/* ID */			6,
	/* Name */			("%o: Load file %0, size %1 Bytes, offset %2 to current board"),
	/* Flags */			0,
	/* Params */		(3, PARAM_FILENAME, ("File"), PARAM_NUMBER, ("Size (in Bytes), -1: End of File"), PARAM_NUMBER, ("Offset"))
	) {
	char *p1 = GetStr();
	size_t p2 = GetInt();
	off_t p3 = GetInt();

	if (numBoards && !d_bProtected && strlen(p1) && p2)
	{

		ifstream file(p1, ios::binary);
		if (file.good())
		{
			file.unsetf(ios::skipws);
			if (p2 == -1)
			{
				file.seekg(0, ios::end);
				streampos fileSize = file.tellg();
				file.seekg(0, ios::beg);
				p2 = fileSize;
			}
			d_vData.resize(p2);
			file.seekg(p3);
			file.read((char *)&d_vData[0], p2);

			file.close();
		}
	}
}
ACTION(
	/* ID */			7,
	/* Name */			("%o: Save board to file %0"),
	/* Flags */			0,
	/* Params */		( 2, PARAM_FILENAME,("File") )
) {
	char *p1 = GetStr();

	if ( numBoards && strlen(p1) )
	{
		ofstream fout(p1, ios::out | ios::binary);
		fout.write( (char *)&(d_vData)[0], d_vData.size() );
		fout.close();
	}
}

ACTION(
	/* ID */            8,
	/* Name */          ("%o: Copy %0 Bytes at %1 to board (%2)"),
	/* Flags */         0,
	/* Params */        ( 3, PARAM_NUMBER,("size (in Bytes), -1: Size"), PARAM_NUMBER,("at offset"), PARAM_STRING,("Board name") )
) {
	size_t p1 = GetInt();
	off_t p2 = GetInt();
	string p3(GetStr());

	if ( rdPtr->vBoards.size() && !p3.empty() )
	{
		bool exists = false;
		unsigned int i;
		if (numBoards && p3.length())
		{
		for (unsigned int i = 0; i<numBoards; i++) //check if board already exists
			if (strCompare(d_sNamei, p3))
			{
				exists = true;
				d_vDatai.clear();
				copy( d_vData.begin()+p2, d_vData.size()>p1 ? d_vData.begin()+p2+p1:d_vData.end(), back_inserter(d_vDatai) );
				d_vDatai.shrink_to_fit();
			}
		}
	}
}

ACTION(
	/* ID */            9,
	/* Name */          ("%o: Swap data of current board with %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_STRING,("Board name") )
) {
	string p1(GetStr());

	if ( numBoards && !p1.empty() && !d_bProtected )
	{
		bool exists = false;
		long slot = 0;
		transform(p1.begin(), p1.end(), p1.begin(), ::tolower);
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			   if ( strCompare(d_sNamei, p1) )
			{	
				if ( !d_bProtectedi )
					d_vData.swap(d_vDatai);
				break;
			} //you could just swap boards id's and names
	}
}

ACTION(
	/* ID */            10,
	/* Name */          ("%o: Crop %0 Bytes at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_NUMBER,("Size (in Bytes), -1: Size"), PARAM_NUMBER,("Offset") )
) {
	size_t p1 = GetInt();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected )
	{
		d_vData.erase( d_vData.begin(), d_vData.begin()+p2 );
		d_vData.erase( p1==-1 ? d_vData.end():d_vData.begin()+p1, d_vData.end() );
		d_vData.shrink_to_fit();
	}
}

ACTION(
	/* ID */            11,
	/* Name */          ("%o: Remove %0 Bytes at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,("Size (in Bytes)"), PARAM_NUMBER,("Offset") )
) {
	size_t p1 = GetInt();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected && p2 < p1 )
	{
		d_vData.erase( d_vData.begin()+p2, d_vData.begin()+p1+p2 );
		d_vData.shrink_to_fit();
	}
}

ACTION(
	/* ID */            12,
	/* Name */          ("%o: Fill data by char %0 in current board"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Fill by") )
) {
	long p1 = GetInt();

	if ( numBoards && !d_bProtected )
		fill( d_vData.begin(), d_vData.end(), p1 );
}

/* SET */

ACTION(
	/* ID */            13,
	/* Name */          ("%o: Set char %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_NUMBER,("Integer value"), PARAM_NUMBER,("Offset") )
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected && p2 < d_vData.size() ) //I don't want errors in my extensions
		d_vData.at(p2) = p1;
}

ACTION(
	/* ID */            14,
	/* Name */          ("%o: Set short %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2,PARAM_NUMBER,("Integer value"), PARAM_NUMBER,("Offset") )
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected && p2+sizeof(short) <= d_vData.size() )
		*(short *)(&d_vData.at(p2)) = p1;
}

ACTION(
	/* ID */            15,
	/* Name */          ("%o: Set long %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_NUMBER,("Integer value"), PARAM_NUMBER,("Offset") )
) {
	long p1 = GetInt();
	off_t p2 = GetInt();
	
	if ( numBoards && !d_bProtected && p2+sizeof(long) <= d_vData.size() )
		*(long *)(&d_vData.at(p2)) = p1;
}

ACTION(
	/* ID */            16,
	/* Name */          ("%o: Set float %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_NUMBER,("Floating-point value"), PARAM_NUMBER,("Offset") )
) {
	long p1 = CNC_GetFloatParameter(rdPtr);
	off_t p2 = GetInt();
	
	if ( numBoards && !d_bProtected && p2+sizeof(float) <= d_vData.size() )
	{
		*(long *)(&d_vData.at(p2)) = p1;
	}
}

ACTION(
	/* ID */            17,
	/* Name */          ("%o: Set string %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_STRING,("String"), PARAM_NUMBER,("Offset") )
) {
	const char * p1 = GetStr();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected )
		copy( p1, strlen(p1)+p2>d_vData.size() ? p1+strlen(p1)-(strlen(p1)-d_vData.size())-p2:p1+strlen(p1), d_vData.begin()+p2 );
}

ACTION(
	/* ID */            18,
	/* Name */          ("%o: Set board %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_STRING,("Board name"), PARAM_NUMBER,("Offset") )
) {
	string p1(GetStr());
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected && !p1.empty() )
	{
		transform(p1.begin(), p1.end(), p1.begin(), ::tolower);
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
		{
			   if ( strCompare(d_sNamei, p1) )
			{			
				copy( d_vDatai.begin(), d_vDatai.size()+p2>d_vData.size() ? d_vDatai.end()-(d_vDatai.size()-d_vData.size())-p2:d_vDatai.end(), d_vData.begin()+p2 );
				break;
			}
		}
	}
}

ACTION(
	/* ID */			19,
	/* Name */			("%o: Set file %0 at %1 to current board"),
	/* Flags */			0,
	/* Params */		( 2, PARAM_FILENAME,("File"), PARAM_NUMBER,("Offset (-1: End)") )
) {
	char *p1 = GetStr();
	long p2 = GetInt();

	if ( numBoards && !d_bProtected && strlen(p1) )
	{
		ifstream file(p1, ios::binary);
		if ( file.good() )
		{
			file.unsetf(ios::skipws);

			file.seekg(0, ios::end);
			streampos fileSize = file.tellg();
			file.seekg(0, ios::beg);		

			if ( p2+fileSize <= d_vData.size() )			
				file.read( (char *)&d_vData[p2], fileSize );			
			file.close();
			d_vData.resize(d_vData.size() + fileSize);
		}
	}
}

/* APPEND */

ACTION(
	/* ID */            20,
	/* Name */          ("%o: Append char %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Integer value") )
) {
	const char p1 = GetInt();
	if ( numBoards && !d_bProtected )
		d_vData.push_back(p1);
}

ACTION(
	/* ID */            21,
	/* Name */          ("%o: Append short %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Integer value") )
) {
	long p1 = GetInt();

	if ( numBoards && !d_bProtected )
	{
		d_vData.push_back(p1 & 0xFF);
		d_vData.push_back((p1 >> 8) & 0xFF);
	}
}

ACTION(
	/* ID */            22,
	/* Name */          ("%o: Append long %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Integer value") )
) {
	long p1 = GetInt();

	if ( numBoards && !d_bProtected )
	{
		const char * chars = reinterpret_cast<char *>(&p1);
		copy( chars, chars+sizeof(long), back_inserter(d_vData) );
	}
}

ACTION(
	/* ID */            23,
	/* Name */          ("%o: Append float %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Floating-point value") )
) {
	long p1 = CNC_GetFloatParameter(rdPtr);
	long p2 = GetInt();
	
	if ( numBoards && !d_bProtected )
	{
		const char * chars = reinterpret_cast<char *>(&p1);
		copy( chars, chars+sizeof(float), back_inserter(d_vData) );
	}
}

ACTION(
	/* ID */            24,
	/* Name */          ("%o: Append string %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_STRING,("String") )
) {
	const char * p1 = GetStr();

	if ( numBoards && !d_bProtected && strlen(p1) )
		copy( p1, p1+strlen(p1), back_inserter(d_vData) );
}

ACTION(
	/* ID */            25,
	/* Name */          ("%o: Append board %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_STRING,("Board name") )
) {
	string p1(GetStr());

	if ( numBoards && !d_bProtected && !p1.empty() )
	{
		transform(p1.begin(), p1.end(), p1.begin(), ::tolower);
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
		{
			   if ( strCompare(d_sNamei, p1) )
			{
				d_vData.reserve(d_vData.size()+d_vDatai.size());
				copy( d_vDatai.begin(), d_vDatai.end(), back_inserter(d_vData) );
				break;
			}
		}
	}
}

ACTION(
	/* ID */			26,
	/* Name */			("%o: Append file %0 to current board"),
	/* Flags */			0,
	/* Params */		( 1, PARAM_FILENAME,("File") )
) {
	char *p1 = GetStr();

	if ( numBoards && !d_bProtected && strlen(p1) )
	{
		ifstream file(p1, ios::binary);
		if ( file.good() )
		{
			file.unsetf(ios::skipws);

			file.seekg(0, ios::end);
			streampos fileSize = file.tellg();
			file.seekg(0, ios::beg);		

			size_t tempSize = d_vData.size();
			d_vData.resize(d_vData.size()+fileSize);
			file.read( (char *)&d_vData[d_vData.size()-fileSize], fileSize );
			file.close();
		}
	}
}

/* INSERT */

ACTION(
	/* ID */            27,
	/* Name */          ("%o: Insert char %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_NUMBER,("Value"),PARAM_NUMBER,("Offset") )
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected )
		d_vData.emplace( d_vData.begin()+p2, p1 );
}

ACTION(
	/* ID */            28,
	/* Name */          ("%o: Insert short %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_NUMBER,("Value"),PARAM_NUMBER,("Offset") )
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected )
	{
		const char * chars = reinterpret_cast<char *>(&p1);
		d_vData.insert( d_vData.begin()+p2, chars, chars+sizeof(short) );
	}
}

ACTION(
	/* ID */            29,
	/* Name */          ("%o: Insert long %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_NUMBER,("Value"),PARAM_NUMBER,("Offset") )
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected )
	{
		const char * chars = reinterpret_cast<char *>(&p1);
		d_vData.insert( d_vData.begin()+p2, chars, chars+sizeof(long) );
	}
}

ACTION(
	/* ID */            30,
	/* Name */          ("%o: Insert float %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_NUMBER,("Value"), PARAM_NUMBER,("Offset") )
) {
	long p1 = CNC_GetFloatParameter(rdPtr);
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected )
	{
		const char * chars = reinterpret_cast<char *>(&p1);
		d_vData.insert( d_vData.begin()+p2, chars, chars+sizeof(float) );
	}
}

ACTION(
	/* ID */            31,
	/* Name */          ("%o: Insert string %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_STRING,("String"), PARAM_NUMBER,("Offset") )
) {
	const char * p1 = GetStr();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected && strlen(p1) )
		d_vData.insert( d_vData.begin()+p2, p1, p1+strlen(p1) );
}

ACTION(
	/* ID */            32,
	/* Name */          ("%o: Insert board %0 at %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_STRING,("Board name"), PARAM_NUMBER,("Offset") )
) {
	string p1(GetStr());
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected && !p1.empty() )
	{
		transform(p1.begin(), p1.end(), p1.begin(), ::tolower);
		for (unsigned int i=0; i<numBoards; i++) //check if the board exists
		{
			   if ( strCompare(d_sNamei, p1) )
			{				
				d_vData.reserve( d_vData.size()+d_vDatai.size() );
				d_vData.insert( d_vData.begin()+p2, d_vDatai.begin(), d_vDatai.end() );
				break;
			}
		}
	}
}


ACTION(
	/* ID */			33,
	/* Name */			("%o: Insert file %0 at %1 to current board"),
	/* Flags */			0,
	/* Params */		( 2, PARAM_FILENAME,("File"), PARAM_NUMBER,("Offset (-1: End)") )
) {
	char *p1 = GetStr();
	off_t p2 = GetInt();

	if ( numBoards && !d_bProtected && strlen(p1) )
	{
		ifstream file(p1, ios::binary);
		if ( file.good() )
		{
			file.unsetf(ios::skipws);

			file.seekg(0, ios::end);
			streampos fileSize = file.tellg();
			file.seekg(0, ios::beg);		

			if ( p2 > d_vData.size() )
				p2 = d_vData.size();

			d_vData.insert( d_vData.begin()+p2, d_vData.begin(), d_vData.begin()+fileSize );
			file.read( (char *)&d_vData[p2], fileSize );			
			file.close();
		}
	}
}

/* OCCURRENCESS */
/* REPLACE */

ACTION(
	/* ID */            34,
	/* Name */          ("%o: Replace every integer %0 size %1 by %2 size %3 in current board"),
	/* Flags */         0,
	/* Params */        ( 4, PARAM_NUMBER,("Old integer value"), PARAM_NUMBER,("Size"), PARAM_NUMBER,("New integer value"), PARAM_NUMBER,("Size") )
) {
	long p1 = GetInt();
	char p2 = GetInt();
	long p3 = GetInt();
	char p4 = GetInt();

	if ( numBoards && !d_bProtected )
	{
		if ( p2 == 1 && p4 == 1 )
			replace( d_vData.begin(), d_vData.end(), (signed char)p1, (signed char)p3 );
		else
		{
			const char * value = reinterpret_cast<char *>(&p1);
			const char * value2 = reinterpret_cast<char *>(&p3);

			long dist = 0;
			while ( true )
			{
				auto it = search( d_vData.begin()+dist, d_vData.end(), value, value + sizeof(char)*p2 );			
				dist = it - d_vData.begin();	

				if ( dist != d_vData.size() )
				{
					if ( p4 > p2 )
						d_vData.insert( d_vData.begin()+dist, value2, value2 + sizeof(char)*(p4-p2) );
					copy( value2, value2 + sizeof(char)*p4, d_vData.begin()+dist );
					if ( p4 < p2 )
						d_vData.erase( d_vData.begin()+dist+p4, d_vData.begin()+dist+p2 );

					dist += sizeof(char)*p4;
				}
				else
					break;
					
			}

		}
	}
}

ACTION(
	/* ID */            35,
	/* Name */          ("%o: Replace every string %0 by %1 in current board"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_STRING,("Old string"), PARAM_STRING,("New string") )
) {
	char *p1 = GetStr();
	char *p2 = GetStr();

	if ( numBoards && !d_bProtected )
	{			
		long dist = 0;
		while ( true )
		{
			auto it = search( d_vData.begin()+dist, d_vData.end(), p1, p1 + strlen(p1) );
			dist = it - d_vData.begin();

			if ( dist != d_vData.size() )
			{
				if ( strlen(p2) > strlen(p1) )
					d_vData.insert( d_vData.begin()+dist, p2, p2 + (strlen(p2)-strlen(p1)) );
				copy( p2, p2 + strlen(p2), d_vData.begin()+dist );
				if ( strlen(p2) < strlen(p1) )
					d_vData.erase( d_vData.begin()+dist+strlen(p2), d_vData.begin()+dist+strlen(p1) );

				dist += strlen(p2);
			}
			else
				break;
		}
	}
}

ACTION(
	/* ID */            36,
	/* Name */          ("%o: Replace every board %0 by %1 in current board"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_STRING,("Old board name"), PARAM_STRING,("New board name") )
) {
	string p1( (LPSTR)Param(TYPE_STRING) );
	string p2( (LPSTR)Param(TYPE_STRING) );

	if ( numBoards && !d_bProtected )
	{			
		long dist = 0;
		transform(p1.begin(), p1.end(), p1.begin(), ::tolower);
		transform(p2.begin(), p2.end(), p2.begin(), ::tolower);
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			   if ( strCompare(d_sNamei, p1) )
			{
				for (long j=0; j<numBoards; j++) //check if board already exists
					if ( d_sNamej[0] == p2[0] && d_sNamej == p2 )
					{
						while ( true )
						{
							auto it = search( d_vData.begin()+dist, d_vData.end(), d_vDatai.begin(), d_vDatai.end() );
							dist = it - d_vData.begin();
							if ( dist == d_vData.size() )
								break;
							if ( d_vDataj.size() > d_vDatai.size() )
								d_vData.insert( d_vData.begin()+dist, d_vDataj.begin(), d_vDataj.begin() + (d_vDataj.size()-d_vDatai.size()) );
							copy( d_vDataj.begin(), d_vDataj.end(), d_vData.begin()+dist );
							if ( d_vDataj.size() < d_vDatai.size() )
								d_vData.erase( d_vData.begin()+dist+d_vDataj.size(), d_vData.begin()+dist+d_vDatai.size() );
							dist += d_vDataj.size();	
						}
						break;
					}
				break;
			}
			

	}
}

/* OCCURRENCES */
/* REMOVE */

ACTION(
	/* ID */            37,
	/* Name */          ("%o: Remove every integer %0, size %1 in current board"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_NUMBER,("Integer value"), PARAM_NUMBER,("Size") )
) {
	long p1 = GetInt();
	char p2 = GetInt();

	if ( numBoards && !d_bProtected )
	{
		if ( p2 == 1 )
			d_vData.erase( remove(d_vData.begin(), d_vData.end(), (signed char)p1), d_vData.end() );
		else
		{
			const char * value = reinterpret_cast<char *>(&p1);

			long buff = -1, dist = -1;
				while ( dist != d_vData.size() )
				{
					if ( buff != dist )
					{
						d_vData.erase( d_vData.begin()+dist, d_vData.begin()+dist + sizeof(char)*p2 );
						buff = dist;
					}
					auto it = search( d_vData.begin()+dist+1, d_vData.end(), value, value + sizeof(char)*p2 );			
					dist = it - d_vData.begin();						
				}

		}
	}
}

ACTION(
	/* ID */            38,
	/* Name */          ("%o: Remove every string %0 in current board"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_STRING,("String") )
) {
	char *p1 = GetStr();

	if ( numBoards && !d_bProtected )
	{
		long dist = 0;
			while ( true )
			{
				auto it = search( d_vData.begin()+dist, d_vData.end(), p1, p1 + strlen(p1) );			
				dist = it - d_vData.begin();	
				if ( dist != d_vData.size() )
					d_vData.erase( d_vData.begin()+dist, d_vData.begin()+dist + strlen(p1) );
				else
					break;					
			}		
	}
}

ACTION(
	/* ID */            39,
	/* Name */          ("%o: Remove every board %0 in current board"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_STRING,("Board name") )
) {
	string p1( (LPSTR)Param(TYPE_STRING) );

	if ( numBoards && !d_bProtected )
	{			
		long dist = 0;
		transform(p1.begin(), p1.end(), p1.begin(), ::tolower);
		for (unsigned int i=0; i<numBoards; i++) //check if board already exists
			   if ( strCompare(d_sNamei, p1) )			
			{
				while ( true )
				{
					auto it = search( d_vData.begin()+dist, d_vData.end(), d_vDatai.begin(), d_vDatai.end() );
					dist = it - d_vData.begin();

					if ( dist != d_vData.size() )
						d_vData.erase( d_vData.begin()+dist, d_vData.begin()+dist+d_vDatai.size() );
					else
						break;
				}
				break;
			}

	}
}

/* ENDIANNESS */

ACTION(
	/* ID */            40,
	/* Name */          ("%o: Set little endian"),
	/* Flags */         0,
	/* Params */        ( 0 )
) {
	rdPtr->bEndianness = 0;
}

ACTION(
	/* ID */            41,
	/* Name */          ("%o: Set big endian"),
	/* Flags */         0,
	/* Params */        ( 0 )
) {
	rdPtr->bEndianness = 1;
}

ACTION(
	/* ID */            42,
	/* Name */          ("%o: Flip short at %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Offset") )
) {
	off_t p1 = GetInt();

	if ( numBoards && !d_bProtected && p1+sizeof(short) <= d_vData.size() )
	{
		const char buff = d_vData[p1];
		d_vData.at(p1) = d_vData[p1+1];
		d_vData.at(p1+1) = buff;
	}
}

ACTION(
	/* ID */            43,
	/* Name */          ("%o: Flip integer at %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Offset") )
) {
	off_t p1 = GetInt();
	long buffer;

	if ( numBoards && !d_bProtected && p1+sizeof(long) <= d_vData.size() )
		*(long *)(&d_vData.at(p1)) = _byteswap_ulong(*reinterpret_cast<const long *>(&d_vData.at(p1)));
}

/* WORKSPACE */

struct BINBheader
{
	string magic = "BINB";
	float version = 1.0;
	char padding[8];
} BINBheader;

ACTION(
	/* ID */			44,
	/* Name */			("%o: Save workspace to file %0"),
	/* Flags */			0,
	/* Params */		( 1, PARAM_FILENAME,("File") )
) {
	char *p1 = GetStr();

	if ( numBoards && strlen(p1) )
	{
		ofstream fout(p1, ios::out | ios::binary);
		fout.write( BINBheader.magic.c_str(), BINBheader.magic.size() );
		fout.write( (char *)&BINBheader.version, sizeof(float) );
		fout.write( (char *)&BINBheader.padding, sizeof(BINBheader.padding) );
		for ( unsigned int i=0; i<numBoards; i++ )
		{
			const unsigned char size = d_sNamei.size();
			fout.write( (char *)&size, sizeof(size) );
			fout.write( d_sNamei.c_str(), d_sNamei.size() );
			const long size2 = d_vDatai.size();
			fout.write( (char *)&size2, sizeof(size2) );
			fout.write( (char *)&(d_vDatai)[0], d_vDatai.size() );
			fout.write( (char *)&d_bProtectedi, sizeof(d_bProtectedi) );
		}
		fout.close();
	}
}

ACTION(
	/* ID */			45,
	/* Name */			("%o: Load workspace from file %0, merge Flag %1"),
	/* Flags */			0,
	/* Params */		( 2, PARAM_FILENAME,("File"), PARAM_NUMBER,("Merge with current? (0: No, 1: Yes)") )
) {
	LPSTR p1 = GetStr();
	bool p2 = GetInt();

	if ( strlen(p1) )
	{
		string magic;		
		float version;
		ifstream file(p1, ios::binary);
		file.unsetf(ios::skipws);
		file.seekg(0, ios::beg);

		magic.resize( BINBheader.magic.size() );
		file.read( &magic[0], magic.size() );
		file.read( (char *)&version, sizeof(float) );
		if ( magic == BINBheader.magic && version == BINBheader.version )
		{	
			file.seekg( 0x10 );
			rdPtr->iSelBoard = 0;
			if ( !p2 )
			{
				rdPtr->vBoards.clear();
				rdPtr->vBoards.shrink_to_fit();						
				while ( true )
				{							
					unsigned char vnamelen = 0;
					file.read( (char *)&vnamelen, sizeof(unsigned char) );
					rdPtr->vBoards.push_back(Board());				
					d_sName.resize(vnamelen);
					file.read( &d_sName[0], d_sName.size() );				
					long vecsize = 0;
					file.read( (char *)&vecsize, sizeof(long) );	
					d_vData.resize(vecsize);
					file.read( (char *)&d_vData[0], vecsize );				
					file.read( (char *)&d_bProtected, sizeof(char) );				
					if ( file.peek() == EOF )
						break;
					rdPtr->iSelBoard++;
				}
			}
			else
				while ( true )
				{		
					bool exists = false;
					unsigned char vnamelen = 0;
					file.read( (char *)&vnamelen, sizeof(unsigned char) );				
					string name;
					name.resize(vnamelen);
					file.read( &name[0], vnamelen );	

					for (unsigned int i=0; i<numBoards; i++) //check if board already exists
						if ( d_sNamei[0] == name[0] && d_sNamei == name )						
							exists = true;
						
					long vecsize = 0;
					file.read( (char *)&vecsize, sizeof(long) );

					if ( !exists )
					{
						rdPtr->iSelBoard = numBoards;
						rdPtr->vBoards.push_back(Board());
						d_sName = name;
						d_vData.resize(vecsize);
						file.read( (char *)&d_vData[0], vecsize );				
						file.read( (char *)&d_bProtected, sizeof(char) );	
					}
					else
					{
						off_t pos = file.tellg();
						file.seekg( pos + vecsize + 1 );
					}
					
					if ( file.peek() == EOF )
						break;
				}
		}	
		file.close();
	}
}

//ACTION(
//	/* ID */            46,
//	/* Name */          ("%o: Select board %0"),
//	/* Flags */         0,
//	/* Params */        ( 1, PARAM_NUMBER,("Board unique number") )
//) {
//	long p1 = GetInt();
//
//	if ( numBoards && p1 < numBoards )
//		rdPtr->iSelBoard = p1;
//}

/* Zlib (Mark Adler) */

ACTION(
	/* ID */            46,
	/* Name */          ("%o: Compress current data using zlib, level %0"),
	/* Flags */         0,
	/* Params */        ( 1, PARAM_NUMBER,("Compression level (0: No compression, -1: Default, 1: Best speed, 9: Best compression)") )
) {
	char p1 = GetInt();

	if (numBoards)
	{
		vector <char> buffer;
		
		const size_t BUFSIZE = 128 * 1024;
		unsigned char temp_buffer[BUFSIZE];
		
		z_stream strm;
		strm.zalloc = 0;
		strm.zfree = 0;
		strm.next_in = reinterpret_cast<unsigned char *>(d_vData.data());
		strm.avail_in = d_vData.size();
		strm.next_out = temp_buffer;
		strm.avail_out = BUFSIZE;
		
		deflateInit(&strm, p1);
		
		while (strm.avail_in)
		{
			int res = deflate(&strm, Z_NO_FLUSH);
			assert(res == Z_OK);
			if (!strm.avail_out)
			{
				buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
				strm.next_out = temp_buffer;
				strm.avail_out = BUFSIZE;
			}
		}
		
		int deflate_res = Z_OK;
		while (deflate_res == Z_OK)
		{
			deflate_res = deflate(&strm, Z_FINISH);
		}
		
		assert(deflate_res == Z_STREAM_END);
		buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out); // rest
		deflateEnd(&strm);
		
		d_vData.swap(buffer);
		buffer.clear();
	}
}

ACTION(
	/* ID */            47,
	/* Name */          ("%o: Decompress current data using zlib"),
	/* Flags */         0,
	/* Params */        (0)
) {
	if (numBoards)
	{
		vector <char> buffer;
		
		const size_t BUFSIZE = 128 * 1024;
		unsigned char temp_buffer[BUFSIZE];
		
		z_stream strm;
		strm.zalloc = 0;
		strm.zfree = 0;
		strm.next_in = reinterpret_cast<unsigned char *>(d_vData.data());
		strm.avail_in = d_vData.size();
		strm.next_out = temp_buffer;
		strm.avail_out = BUFSIZE;
		
		if ( inflateInit(&strm) == Z_OK )
		{
			while (strm.avail_in)
			{
				int res = inflate(&strm, Z_NO_FLUSH);
				assert(res == Z_OK);
				if (!strm.avail_out)
				{
					buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
					strm.next_out = temp_buffer;
					strm.avail_out = BUFSIZE;
				}
			}
			
			int inflate_res = Z_OK;
			while (inflate_res == Z_OK)
			{
				inflate_res = inflate(&strm, Z_FINISH);
			}
			
			assert(inflate_res == Z_STREAM_END);
			buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
			inflateEnd(&strm);
			
			d_vData.swap(buffer);
			buffer.clear();
		}
	}
}

/* Blowfish */

ACTION(
	/* ID */            48,
	/* Name */          ("%o: Encrypt current data using blowfish, key %0, method %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_STRING,("Set key string (length from 4 to 56 characters)"), PARAM_NUMBER,("Method (0: Simple, 1: Strict)") )
) {
	LPSTR p1 = GetStr();
	bool p2 = GetInt();

	if ( numBoards && strlen(p1) >= 4 && d_vData.size() >= 8 )
	{
		BLOWFISH_CTX ctx;	

		Blowfish_Init(&ctx, (unsigned char *)p1, min(strlen(p1), 56));
		
		if (p2)
			for(size_t i=0; i<d_vData.size()+1 - 8; i++)
				Blowfish_Encrypt( &ctx, (unsigned long *)(d_vData.data()+i), (unsigned long *)(d_vData.data()+i+4) );
		else
			for(size_t i=0; i<d_vData.size()/8; i++)
				Blowfish_Encrypt( &ctx, (unsigned long *)&d_vData.at(i*4), (unsigned long *)&d_vData.at((i+1)*4) );
	}
}

ACTION(
	/* ID */            49,
	/* Name */          ("%o: Decrypt current data using blowfish, key %0, method %1"),
	/* Flags */         0,
	/* Params */        ( 2, PARAM_STRING,("Set key string (length from 4 to 56 characters)"), PARAM_NUMBER,("Strict mode (0: No, 1: Yes)") )
) {
	LPSTR p1 = GetStr();
	bool p2 = GetInt();

	if ( numBoards && strlen(p1) >= 4 && d_vData.size() >= 8 )
	{
		BLOWFISH_CTX ctx;	

		Blowfish_Init(&ctx, (unsigned char *)p1, min(strlen(p1), 56));
		
		if (p2)
			for(int i=d_vData.size() - 8; i>=0; i--)
				Blowfish_Decrypt( &ctx, (unsigned long *)(d_vData.data()+i), (unsigned long *)(d_vData.data()+i+4) );
		else
			for(size_t i=0; i<d_vData.size()/8; i++)
				Blowfish_Decrypt( &ctx, (unsigned long *)&d_vData.at(i*4), (unsigned long *)&d_vData.at((i+1)*4) );
	}
}

/* Base64 */

ACTION(
	/* ID */            50,
	/* Name */          ("%o: Encode current data using base64"),
	/* Flags */         0,
	/* Params */        (0)
) {

	if ( numBoards )
	{
		vector <char> buf;
		buf.resize(Base64encode_len(d_vData.size()));
		Base64encode(buf.data(), d_vData.data(), d_vData.size());
		buf.swap(d_vData);
		buf.clear();
	}
}

ACTION(
	/* ID */            51,
	/* Name */          ("%o: Decode current data using base64"),
	/* Flags */         0,
	/* Params */        (0)
) {

	if ( numBoards )
	{
		Base64decode(d_vData.data(), d_vData.data());
	}
}


ACTION(
	/* ID */            52,
	/* Name */          ("%o: Set bit %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER, ("Boolean value"), PARAM_NUMBER, ("Offset"))
	) {
	int p1 = GetInt();
	const off_t p2 = GetInt();
	const off_t offset = p2 / 8;
	const off_t bit = p2 % 8;

	BYTE output = (char)d_vData.at(offset);

	if (!rdPtr->bEndianness)
	{
		if (p1) output |= 1 << bit;
		else output &= ~(1 << bit);
	}
	else
	{
		if (p1) output |= 1 << (7 - bit);
		else output &= ~(1 << (7 - bit));
	}

	if (numBoards && !d_bProtected && offset < d_vData.size()) //I don't want errors in my extensions
		d_vData.at(offset) = output;
}


//ACTION(
//	/* ID */            53,
//	/* Name */          ("%o: Set undefined %0 at %1"),
//	/* Flags */         0,
//	/* Params */        ( 3, PARAM_NUMBER,("Integer value"), PARAM_NUMBER,("reserved bits (1 - 8)"), PARAM_NUMBER,("Offset") )
//) {
//	long p1 = GetInt();
//	unsigned char p2 = GetInt();
//	off_t p3 = GetInt();
//
//	unsigned char p1b = p1 >> (8-p2);
//
//	off_t p3b = (GetInt()%8)*p2;
//	BYTE output = (char)d_vData.at(p2);
//
//	if ( numBoards && !d_bProtected && p2 < d_vData.size() ) //I don't want errors in my extensions
//		d_vData.at(p2) = output;
//}

ACTION(
	/* ID */            53,
	/* Name */          ("%o: Delete board %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_STRING, ("Board name"))
	) {
	string p1(GetStr());

	if (numBoards && p1.length())
	{
		for (unsigned int i = 0; i<numBoards; i++) //check if board already exists
			if (strCompare(d_sNamei, p1))
			{

				rdPtr->vBoards[i].sName = "";
				rdPtr->vBoards.erase(rdPtr->vBoards.begin() + i, rdPtr->vBoards.begin() + i + 1);
				//rdPtr->vBoards[i].vData.clear();
				//rdPtr->vBoards[i].vData.resize(0); //we have resize board for that
				rdPtr->vBoards[i].vData.shrink_to_fit(); //slow !

			if (rdPtr->iSelBoard >= numBoards)
				rdPtr->iSelBoard--;
				break;
			}
	}
}

ACTION(
	/* ID */            54,
	/* Name */          ("%o: Resize board %0 to %1 Bytes"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING, ("Board name"), PARAM_NUMBER, ("Size (in Bytes)"))
	)
{
	string p1(GetStr());
	size_t p2 = GetInt();
	
	for (unsigned int i = 0; i<numBoards; i++) //check if board already exists
		if (strCompare(d_sNamei, p1))
		{
			if (numBoards && !d_bProtected)
				if (p2 != -1)
				{
					if ( p2 > d_vData.size() )
						d_vData.reserve(p2);

					rdPtr->vBoards[i].vData.resize(p2);
					rdPtr->vBoards[i].vData.shrink_to_fit();
					break;
				}
		}
}


ACTION(
	/* ID */            55,
	/* Name */          ("%o: Add space to current board to %0 Bytes"),
	/* Flags */         0,
	/* Params */        (1, PARAM_NUMBER, ("Size (in Bytes)"))
	) {
	size_t p1 = GetInt();

	if (numBoards && !d_bProtected)
	{
		if (p1)
		{
			d_vData.resize(d_vData.size() + p1);
			d_vData.shrink_to_fit();
		}
	}
}

ACTION(
	/* ID */            56,
	/* Name */          ("%o: Add space to board %0 to %1 Bytes"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING, ("Board name"), PARAM_NUMBER, ("Size (in Bytes)"))
	)
{
	string p1(GetStr());
	size_t p2 = GetInt();

	for (unsigned int i = 0; i<numBoards; i++) //check if board already exists
		if (strCompare(d_sNamei, p1))
		{
			if (numBoards && !d_bProtected)
			{
				if (p2)
				{
					rdPtr->vBoards[i].vData.resize(rdPtr->vBoards[i].vData.size() + p2);
					d_vData.shrink_to_fit();
				}
			}
			break;
		}
}

ACTION(
	/* ID */            57,
	/* Name */          ("%o: Clear Everything"),
	/* Flags */         0,
	/* Params */        (0)
	) {
	unsigned int i = 0;

	string p1(GetStr());
	size_t p2 = GetInt();
	
	for (unsigned int i = 0; i<numBoards; i++) //check if board already exists	
		if (numBoards && !d_bProtected)
		{
			rdPtr->vBoards[i].sName = "";
			d_vDatai.clear();
			d_vDatai.shrink_to_fit();
		}
	
}