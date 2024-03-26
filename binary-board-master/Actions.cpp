#include "common.h"

// ============================================================================
//
// ACTIONS
// 
// ============================================================================

/*BOARD*/

ACTION(
	/* ID */            0,
	/* Name */          _T("%o: Create a board %0, capacity %1 Bytes"),
	/* Flags */         EXPFLAG_STRING,
	/* Params */        (2, PARAM_STRING,_T("Board name"), PARAM_NUMBER,_T("Capacity (in Bytes)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t p2 = GetInt();

	for(unsigned int i=0; i<numBoards; i++) //check if board already exists
		if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			return 0;	
	rdPtr->BOARDS.push_back(Board());
	unsigned int selected = numBoards-1;
	rdPtr->BOARDS[selected].bLock = false;
	rdPtr->BOARDS[selected].sName = p1;
	if(p2)
	{
		rdPtr->BOARDS[selected].vData.reserve(p2);
	}
	if(rdPtr->bAutoSelect)
	{
		if (signed int(rdPtr->SelBoard+1)) rdPtr->lastBoard = d_sName; //history
		rdPtr->SelBoard = selected;
	}

	rdPtr->rRd->PushEvent(1);  //Create event at the end of the loop, safer
}

ACTION(
	/* ID */            1,
	/* Name */          _T("%o: Select board %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_STRING,_T("Board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards)
	{
		for(unsigned int i=0; i<numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			{
				if (signed int(rdPtr->SelBoard+1)) rdPtr->lastBoard = d_sName; //history
				rdPtr->SelBoard = i;
				return 1;
			}
	}
	return 0;
}

ACTION(
	/* ID */            2,
	/* Name */          _T("%o: Select back previous board"),
	/* Flags */         0,
	/* Params */        (0)
) {
	if(numBoards > 1)
	{
		for(unsigned int i=0; i<numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, rdPtr->lastBoard, rdPtr->SelBoard))
			{
				rdPtr->SelBoard = i;
				return 1;
			}
	}
	return 0;
}


ACTION(
	/* ID */            3,
	/* Name */          _T("%o: Rename current board to %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_STRING,_T("Board new name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bLock)
	{
		for(unsigned int i=0; i<numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
				return 0; // a board with such name already exists, exiting
		d_sName = p1; //if there will be no dublicated names
	}
	return 1;
}

ACTION(
	/* ID */            4,
	/* Name */          _T("%o: Rename board %0 to %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("Target board name"), PARAM_STRING,_T("Board new name"))
) {
	string p1 = string(GetStr());
	string p2 = string(GetStr());
	if(!p1.length() || !p2.length()) return 0;

	for(unsigned int i=0; i<numBoards; i++) // check if new board already exists
		if(strCompare(d_sNamei, p2, rdPtr->bCaseSensitive))
			return 0; // name exists, prevent dublicate, exiting
					  // it doesn't, moving forward
	for(unsigned int i=0; i<numBoards; i++) // check if old board already exists
	{
		if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive)) // it does
		{ 
			d_sNamei = p2; // it does, change current old name to new
			return 1;
		}
	}
	return 0;
}

ACTION(
	/* ID */            5,
	/* Name */          _T("%o: Set current board lock flag to %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_NUMBER,_T("Content locked (0: No, 1: Yes)"))

) {
	bool p1 = GetInt();

	if(numBoards)
		d_bLock = p1;
	return 0;
}

ACTION(
	/* ID */            6,
	/* Name */          _T("%o: Set board %0 lock flag to %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("Target board name"), PARAM_NUMBER,_T("Content locked (0: No, 1: Yes)"))
) {
	string p1 = string(GetStr());
	bool p2 = GetInt();

	if(!p1.length()) return 0;

	for(unsigned int i=0; i<numBoards; i++) //check if board already exists
		if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
		{
			rdPtr->BOARDS[i].bLock = p2;
			return 1;
		}
	return 0;
}

ACTION(
	/* ID */            7,
	/* Name */          _T("%o: Resize current container dimensions to %0 Bytes, option %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Size (in Bytes)"), PARAM_NUMBER,_T("Which? (0: Capacity, 1: Size, 2: Both)"))
) {
	size_t p2 = GetInt();
	unsigned char p3 = GetInt();

	if(numBoards)
	{
		if(!d_bLock) //is not locked flag
			switch (p3)
			{
				case 1: d_vData.resize(p2); break;
				case 2: d_vData.resize(p2);
				case 0:
				{
					if(p2 > d_vData.capacity())
						d_vData.reserve(p2);
					else if(p2 < d_vData.capacity())
						d_vData.shrink_to_fit();
					//vector<char>(d_vData).swap(d_vData); // trick
				}
			}
			return 1;
	}
	return 0;
}

ACTION(
	/* ID */            8,
	/* Name */          _T("%o: Resize container dimensions of board %0 to %1 Bytes, option %2"),
	/* Flags */         0,
	/* Params */        (3, PARAM_STRING,_T("Board name"), PARAM_NUMBER,_T("Size (in Bytes)"), PARAM_NUMBER,_T("Which? (0: Capacity, 1: Size, 2: Both)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t p2 = GetInt();
	int p3 = GetInt();

	for(unsigned int i=0; i<numBoards; i++) //check if board already exists
	{
		if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
		{
			if(!d_bLocki)
			{
				switch (p3)
				{
					case 1: d_vDatai.resize(p2); break;
					case 2: d_vDatai.resize(p2);
					case 0:
					{
						if(p2 > d_vDatai.capacity())
							d_vDatai.reserve(p2);
						else if(p2 < d_vDatai.capacity())
							d_vDatai.shrink_to_fit();
						//vector<char>(d_vData).swap(d_vData); // trick
					}
				}
			return 1;
			}
		}
	}
	return 0;
}

ACTION(
	/* ID */            9,
	/* Name */          _T("%o: Delete current board"),
	/* Flags */         0,
	/* Params */        (0)
) {
	if(numBoards && !d_bLock)
	{
		unsigned int i = rdPtr->SelBoard;
		{
			rdPtr->BOARDS[i].sName.clear();
			rdPtr->BOARDS[i].vData.clear();
			rdPtr->BOARDS[i].vData.shrink_to_fit();
			rdPtr->BOARDS.erase(rdPtr->BOARDS.begin()+i, rdPtr->BOARDS.begin()+i+1);
			if(rdPtr->SelBoard == numBoards) //if selected board number is equal to boards number, decrease it to not overflow.
				rdPtr->SelBoard--;
		}
	}
	return 0;
}

ACTION(
	/* ID */            10,
	/* Name */          _T("%o: Delete board %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_STRING,_T("Target board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	for(unsigned int i=0; i<numBoards; i++) //check if board already exists
		if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
		{
			if(!d_bLocki)
			{
				rdPtr->BOARDS[i].sName.clear();
				rdPtr->BOARDS[i].vData.clear();
				rdPtr->BOARDS[i].vData.shrink_to_fit();
				rdPtr->BOARDS.erase(rdPtr->BOARDS.begin()+i, rdPtr->BOARDS.begin()+i+1);
				if (rdPtr->SelBoard == numBoards) //if selected board number is equal to boards number, decrease it to not overflow.
					rdPtr->SelBoard--;
			}
			return 1;
		}
	return 0;
}


ACTION(
	/* ID */            11,
	/* Name */          _T("%o: Clear Everything"),
	/* Flags */         0,
	/* Params */        (1, PARAM_NUMBER,_T("Ignore lock flag? (0: No, 1: Yes)"))
) {
	bool ignorelock = GetInt();
	//rdPtr->BOARDS.clear();
	for (unsigned int i=0; i<numBoards; i++) //check if board already exists			
		{
		if (ignorelock || !d_bLock)
			// current make sure section
			d_bLocki = false;
			d_sNamei.clear();
			d_vDatai.clear();
			d_vDatai.shrink_to_fit(); 
			// this should do current work
			rdPtr->BOARDS.erase(rdPtr->BOARDS.begin()+i, rdPtr->BOARDS.begin()+i+1);
			rdPtr->SelBoard = 0;
			//rdPtr->BOARDS.pop_back(); // trick
			return 1;
		}
	return 0;
}

/*CONTENT*/

ACTION(
	/* ID */			12,
	/* Name */			_T("%o: Load file %0 beginning at %1, size %2"),
	/* Flags */			0,
	/* Params */		(3, PARAM_FILENAME2,_T("File"), PARAM_NUMBER,_T("Begin"), PARAM_NUMBER,_T("Size (-1: End of File)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t p2 = GetInt();
	off_t p3 = GetInt();

	if(numBoards && !d_bLock && p3)
	{
		std::ifstream file(p1,std::ios::binary);
		if(file.good())
		{
			rdPtr->lastPath = p1;
			file.unsetf(std::ios::skipws);
			if(p3 == -1)
			{
				file.seekg(0,std::ios::end);
				std::streampos fileSize = file.tellg();
				file.seekg(0,std::ios::beg);
				p3 = fileSize;
			}
			d_vData.resize(p3);
			file.seekg(p2);
			file.read((char*)&d_vData[0],p3);

			file.close();
		}
	}
	return 0;
}
// testing results: good, but possibly small memory leak if saving file is looped. taking 5 MB because of caching system. Normal behavior.
ACTION(
	/* ID */			13,
	/* Name */			_T("%o: Save board to file %0"),
	/* Flags */			0,
	/* Params */		(2, PARAM_FILENAME,_T("File"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards)
	{
		rdPtr->lastPath = p1;
		std::ofstream fout(p1,std::ios::out | std::ios::binary);
		fout.write((char*)&(d_vData)[0],d_vData.size());
		fout.close();
	}
	return 0;
}
// might be taken as false positive to dumb Windows 10 Defender
ACTION(
	/* ID */			14,
	/* Name */			_T("%o: Load resource (PE) from file %0 to current board, type %1, find %2, index %3"),
	/* Flags */			0,
	/* Params */		(4, PARAM_FILENAME,_T("File"), PARAM_NUMBER,_T("Type (0x00000020: LOAD_LIBRARY_AS_IMAGE_RESOURCE)"), PARAM_NUMBER,_T("Type (3: RT_ICON)"), PARAM_NUMBER,_T("Index"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	int resType = GetInt();
	int resFind = GetInt();
	int resIndex = GetInt();

	if(numBoards && !d_bLock && p1.size())
	{
		std::ifstream file(p1,std::ios::binary);
		if(file.good())
		{
			rdPtr->lastPath = p1;
			d_vData.clear();
			file.unsetf(std::ios::skipws);
			file.seekg(0,std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0,std::ios::beg);
			std::vector<char> resContainer(fileSize); //prepare a vector of Bytes for current binary file
			if(file.read(resContainer.data(),fileSize)) //if file loaded into current vector
			{
				file.close();
				// load resources
				HMODULE hModule = LoadLibraryEx(p1.c_str(), NULL, resType);
				HRSRC hResource;	
				switch(resFind)
				{
					case 13: resFind = DIFFERENCE; break;
					case 14: resFind = (ULONG_PTR)(RT_CURSOR)+DIFFERENCE; break;
					case 15: resFind = (ULONG_PTR)(RT_ICON)+DIFFERENCE; break;
				};
				//that thing wanted a constant value as a parameter. C++ in a nutshell, had to case current argument
				hResource = FindResource(hModule, MAKEINTRESOURCE(resIndex), (MAKEINTRESOURCE(resFind)));
				HGLOBAL hMemory = LoadResource(hModule,hResource);
				DWORD dwSize = SizeofResource(hModule,hResource);
				LPVOID lpAddress = LockResource(hMemory);
				copy(reinterpret_cast<char*>(lpAddress),reinterpret_cast<char*>(lpAddress)+dwSize,back_inserter(d_vData));
				// free after loading data
				UnlockResource(hMemory);
				FreeResource(hResource);
				FreeLibrary(hModule);
			}
			resContainer.clear();
			resContainer.shrink_to_fit();
			d_vData.shrink_to_fit();
		}
	}
	return 0;
}

/* WORKSPACE */
#pragma pack(push, 1)
#pragma pack(1)
struct WorkspaceHeader // Workspace header structure
{
	char magic[4];
	FLOAT ver;
	DWORD count;
	DWORD fSize; //reserved
} WorkspaceHeader;

struct BoardHeader // Board header structure
{
	DWORD index;
	DWORD bSize;
	DWORD nSize;
	DWORD attr;
} BoardHeader;
#pragma pack(pop)
// serialization and saving
ACTION(
	/* ID */			15,
	/* Name */			_T("%o: Save workspace to file %0"),
	/* Flags */			0,
	/* Params */		(1, PARAM_FILENAME,_T("File"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t pos = 0;

	if(numBoards)
	{
		rdPtr->lastPath = p1;
		strcpy(WorkspaceHeader.magic, "BINB"); // store MAGIC
		WorkspaceHeader.ver = 2.0; // store version
		WorkspaceHeader.count = numBoards;
		std::ofstream fout(p1,std::ios::out | std::ios::binary);
		fout.write((char*)&WorkspaceHeader, sizeof(WorkspaceHeader)); // store workspace magic
		for(unsigned int i=0; i<numBoards; i++)
		{
			BoardHeader.index = i;
			BoardHeader.bSize = d_vDatai.size();
			BoardHeader.nSize = d_sNamei.size();
			BoardHeader.attr = d_bLocki;
			fout.write((char*)&BoardHeader,sizeof(BoardHeader)); // store workspace magic
			fout.write((char*)d_sNamei.c_str(), d_sNamei.size()*sizeof(wchar_t)); // store board name
			fout.write((char*)&(d_vDatai)[0], d_vDatai.size()); // store board data
			writePadding(fout);	
		}
		const size_t fSize = fout.tellp();
		fout.seekp(12);
		fout.write((char*)&fSize, 4); // store workspace data size
		fout.close();
	}	
	return 0;
}
// deserialization and loading
ACTION(
	/* ID */			16,
	/* Name */			_T("%o: Load workspace from file: %0"),
	/* Flags */			0,
	/* Params */		(1, PARAM_FILENAME,_T("File"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	std::ifstream file(p1, std::ios::binary); // open the file
	file.unsetf(std::ios::skipws); // stop eating new lines in binary mode
	file.seekg(0, std::ios::beg);
	file.read((char*)&WorkspaceHeader, sizeof(WorkspaceHeader));
	std::string str(WorkspaceHeader.magic);

	if(file.good())
	{
		rdPtr->lastPath = p1;
	if((str == "BINB") && (WorkspaceHeader.ver == 2.0)) // compare with the header
		for (int i=0; i<WorkspaceHeader.count; i++) // read board index
		{
			file.read((char*)&BoardHeader, sizeof(BoardHeader)); // read current board header
			vector <char> buffName;
			buffName.resize(BoardHeader.nSize*sizeof(wchar_t));
#ifdef UNICODE //UTF 16LE
			file.read((char*)&buffName[0], buffName.size()); // read current board name
			string bName = reinterpret_cast<TCHAR*>(&buffName.at(0));
#else
			file.read((char*)&buffName[0], BoardHeader.nSize); // read current board name
			string bName(buffName.begin(), buffName.end());
#endif
			bName.resize(BoardHeader.nSize);
			buffName.clear();
			buffName.shrink_to_fit();
			unsigned int j;
			rdPtr->SelBoard = 0;
			bool exists = 0;
			for(j = 0; j < numBoards; j++) //check if board already exists
			{
				if(strCompare(d_sNamej, bName, rdPtr->bCaseSensitive))
				{
					rdPtr->SelBoard = j; // select it
					exists = true;
					break;
				}
			}
			if(!exists)
			{ 
				rdPtr->BOARDS.push_back(Board()); // create a new board
				rdPtr->SelBoard = numBoards - 1;
			}
			if(!d_bLock)
			{
				d_bLock = BoardHeader.attr;
				d_sName = bName;
				d_vData.resize(BoardHeader.bSize);
				d_vData.reserve(BoardHeader.bSize);
				file.read((char*)&d_vData[0], BoardHeader.bSize);
			}
			else
				file.seekg(BoardHeader.bSize + file.tellg());
			skipPadding(file);
			if(file.peek() == EOF) break;
		}
	file.close();
	}
	return 0;
}

ACTION(
	/* ID */            17,
	/* Name */          _T("%o: Swap current board's content with board's %0 content"),
	/* Flags */         0,
	/* Params */        (1, PARAM_STRING,_T("Board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bLock)
	{
		bool exists = false;
		long slot = 0;
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		for(unsigned int i=0; i<numBoards; i++) //check if given board exists
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			{
				if(!d_bLocki)
					d_vData.swap(d_vDatai);
				break;
			}
	}
	return 0;
}

ACTION(
	/* ID */            18,
	/* Name */          _T("%o: Copy %0 Bytes at %1 to board %2"),
	/* Flags */         0,
	/* Params */        (3, PARAM_NUMBER,_T("Size (-1: Full)"), PARAM_NUMBER,_T("Offset"), PARAM_STRING,_T("Board name"))
) {
	size_t p1 = GetInt();
	size_t p2 = GetInt();
	string p3 = string(GetStr());
	if(!p3[0]) return 0;

	if(rdPtr->BOARDS.size())
	{
		bool exists = false;
		unsigned int i;
		if(numBoards && p3.length())
		{
			for(unsigned int i=0; i<numBoards; i++) //check if board already exists
				if(strCompare(d_sNamei, p3, rdPtr->bCaseSensitive))
				{
					exists = true;
					d_vDatai.clear();
					copy(d_vData.begin()+p2, p1<d_vData.size()?d_vData.begin()+p2+p1:d_vData.end(), back_inserter(d_vDatai));
					d_vDatai.shrink_to_fit();
				}
		}
	}
	return 0;
}

ACTION(
	/* ID */            19,
	/* Name */          _T("%o: Crop %0 Bytes at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Size, (-1: Full)"), PARAM_NUMBER,_T("Offset"))
) {
	off_t p1 = GetInt();
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p1<=d_vData.size()) && (p2<d_vData.size()))
	{
		d_vData.erase(d_vData.begin(),d_vData.begin() + p2);
		d_vData.erase(p1==-1 ? d_vData.end():d_vData.begin()+p1, d_vData.end());
		//d_vData.shrink_to_fit(); //slow. Use resize with parameter 0 instead or swap a board with itself.
	}
	return 0;
}

ACTION(
	/* ID */            20,
	/* Name */          _T("%o: Remove %0 Bytes at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Size (in Bytes)"), PARAM_NUMBER,_T("Offset"))
) {
	size_t p1 = GetInt();
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p1<=d_vData.size()) && (p2<d_vData.size()))
	{
		d_vData.erase(d_vData.begin()+p2, d_vData.begin()+p1+p2);
		//d_vData.shrink_to_fit(); //slow. Use resize with parameter 0 instead or swap a board with itself.
	}
	return 0;
}


ACTION(
	/* ID */            21,
	/* Name */          _T("%o: Fill current content with %0, size %1, offset %2"),
	/* Flags */         0,
	/* Params */        (3, PARAM_NUMBER,_T("Fill by char (0-255)"), PARAM_NUMBER, _T("Size (-1: Full)"), PARAM_NUMBER, _T("Offset"))
) {
	long p1 = GetInt();
	size_t p2 = GetInt();
	size_t p3 = GetInt();

	if(numBoards && !d_bLock && ((p3+p2)<d_vData.size()))
		fill(d_vData.begin()+p3, d_vData.begin()+p3+p2, p1);
	return 0;
}

/* COMPRESSION */
// Zlib (Mark Adler) */
ACTION(
	/* ID */            22,
	/* Name */          _T("%o: Compress current content with Zlib, level %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_NUMBER,_T("Compression level (0: No compression, -1: Default, 1: Best speed, 9: Best compression)"))
) {
	char p1 = GetInt();

	if(numBoards)
	{
		vector <char> buffer;
		const size_t BUFSIZE = 128 * 1024;
		unsigned char temp_buffer[BUFSIZE];
		z_stream strm;
		strm.zalloc = 0;
		strm.zfree = 0;
		strm.next_in = reinterpret_cast<unsigned char*>(d_vData.data());
		strm.avail_in = d_vData.size();
		strm.next_out = temp_buffer;
		strm.avail_out = BUFSIZE;
		deflateInit(&strm,p1);
		while(strm.avail_in)
		{
			int res = deflate(&strm,Z_NO_FLUSH);
			assert(res == Z_OK);
			if(!strm.avail_out)
			{
				buffer.insert(buffer.end(),temp_buffer,temp_buffer + BUFSIZE);
				strm.next_out = temp_buffer;
				strm.avail_out = BUFSIZE;
			}
		}
		int deflate_res = Z_OK;
		while(deflate_res == Z_OK)
		{
			deflate_res = deflate(&strm,Z_FINISH);
		}
		assert(deflate_res == Z_STREAM_END);
		buffer.insert(buffer.end(),temp_buffer,temp_buffer + BUFSIZE - strm.avail_out); // rest
		deflateEnd(&strm);
		d_vData.swap(buffer);
		buffer.clear();
	}
	return 0;
}

ACTION(
	/* ID */            23,
	/* Name */          _T("%o: Decompress current content with Zlib"),
	/* Flags */         0,
	/* Params */        (0)
) {
	if(numBoards)
	{
		vector <char> buffer;
		const size_t BUFSIZE = 128 * 1024;
		unsigned char temp_buffer[BUFSIZE];
		z_stream strm;
		strm.zalloc = 0;
		strm.zfree = 0;
		strm.next_in = reinterpret_cast<unsigned char*>(d_vData.data());
		strm.avail_in = d_vData.size();
		strm.next_out = temp_buffer;
		strm.avail_out = BUFSIZE;
		if(inflateInit(&strm) == Z_OK)
		{
			while(strm.avail_in)
			{
				int res = inflate(&strm,Z_NO_FLUSH);
				assert(res == Z_OK);
				if(!strm.avail_out)
				{
					buffer.insert(buffer.end(),temp_buffer,temp_buffer + BUFSIZE);
					strm.next_out = temp_buffer;
					strm.avail_out = BUFSIZE;
				}
			}
			int inflate_res = Z_OK;
			while(inflate_res == Z_OK)
			{
				inflate_res = inflate(&strm,Z_FINISH);
			}
			assert(inflate_res == Z_STREAM_END);
			buffer.insert(buffer.end(),temp_buffer,temp_buffer + BUFSIZE - strm.avail_out);
			inflateEnd(&strm);
			d_vData.swap(buffer);
			buffer.clear();
		}
	}
	return 0;
}
/* ENCRYPTION */
// Blowfish
ACTION(
	/* ID */            24,
	/* Name */          _T("%o: Encrypt current content with Blowfish, key %0, method %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("Set key string (length from 4 to 56 characters)"), PARAM_NUMBER,_T("Method (0: Simple, 1: Strict)"))
) {
	string p1 = string(GetStr());
	bool p2 = GetInt();
	
	if(numBoards && p1.size() >= 4 && d_vData.size() >= 8)
	{
		BLOWFISH_CTX ctx;

		Blowfish_Init(&ctx, (unsigned char*)p1.c_str(), min(p1.size(), 56));

		if(p2)
			for(size_t i = 0; i < d_vData.size() + 1 - 8; i++)
				Blowfish_Encrypt(&ctx,(unsigned long*)(d_vData.data() + i),(unsigned long*)(d_vData.data() + i + 4));
		else
			for(size_t i = 0; i < d_vData.size() / 8; i++)
				Blowfish_Encrypt(&ctx,(unsigned long*)&d_vData.at(i * 4),(unsigned long*)&d_vData.at((i + 1) * 4));
	}
	return 0;
}

ACTION(
	/* ID */            25,
	/* Name */          _T("%o: Decrypt current content with Blowfish, key %0, method %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("Set key string (length from 4 to 56 characters)"), PARAM_NUMBER,_T("Strict mode (0: No, 1: Yes)"))
) {
	string p1 = string(GetStr());
	bool p2 = GetInt();
	if(numBoards && p1.size() >= 4 && d_vData.size() >= 8)
	{
		BLOWFISH_CTX ctx;

		Blowfish_Init(&ctx, (unsigned char*)p1.c_str(), min(p1.size(), 56));

		if(p2)
			for(int i = d_vData.size() - 8; i >= 0; i--)
				Blowfish_Decrypt(&ctx,(unsigned long*)(d_vData.data() + i),(unsigned long*)(d_vData.data() + i + 4));
		else
			for(size_t i = 0; i < d_vData.size() / 8; i++)
				Blowfish_Decrypt(&ctx,(unsigned long*)&d_vData.at(i * 4),(unsigned long*)&d_vData.at((i + 1) * 4));
	}
	return 0;
}
/* ENCODING */
//Base64
ACTION(
	/* ID */            26,
	/* Name */          _T("%o: Encode current content with Base64"),
	/* Flags */         0,
	/* Params */        (0)
) {
	if(numBoards)
	{
		vector <char> buf;
		buf.resize(Base64encode_len(d_vData.size()));
		Base64encode(buf.data(),d_vData.data(),d_vData.size());
		buf.swap(d_vData);
		buf.clear();
	}
	return 0;
}

ACTION(
	/* ID */            27,
	/* Name */          _T("%o: Decode current content with Base64"),
	/* Flags */         0,
	/* Params */        (0)
) {

	if(numBoards)
	{
		Base64decode(d_vData.data(), d_vData.data());
	}
	return 0;
}

/* VALUE */

ACTION(
	/* ID */            28,
	/* Name */          _T("%o: Set bit %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Boolean value"), PARAM_NUMBER,_T("Offset"))
) {
	int p1 = GetInt();
	const off_t p2 = GetInt();
	const off_t offset = p2 / 8;
	const off_t bit = p2 % 8;
	BYTE output = (char)d_vData.at(offset);
	if(!rdPtr->bEndianness)
	{
		if(p1) output |= 1 << bit;
		else output &= ~(1 << bit);
	}
	else
	{
		if(p1) output |= 1 << (7 - bit);
		else output &= ~(1 << (7 - bit));
	}
	if(numBoards && !d_bLock && offset < d_vData.size()) //I don't want errors in my extensions
		d_vData.at(offset) = output;
	return 0;
}

ACTION(
	/* ID */            29,
	/* Name */          _T("%o: Set char %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Integer value"), PARAM_NUMBER,_T("Offset"))
) {
	long p1 = GetInt();
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && p2 < d_vData.size()) //I don't want errors in my extensions
		d_vData.at(p2) = p1;
	return 0;
}

ACTION(
	/* ID */            30,
	/* Name */          _T("%o: Set short int %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Integer value"), PARAM_NUMBER,_T("Offset"))
) {
	long p1 = GetInt();
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2+sizeof(short)) <= d_vData.size())
		*(short*)(&d_vData.at(p2)) = p1;
	return 0;
}

ACTION(
	/* ID */            31,
	/* Name */          _T("%o: Set long int %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Integer value"), PARAM_NUMBER,_T("Offset"))
) {
	long p1 = GetInt();
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2+sizeof(long)) <= d_vData.size())
		*(long*)(&d_vData.at(p2)) = p1;
	return 0;
}

ACTION(
	/* ID */            32,
	/* Name */          _T("%o: Set float %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Single precision floating-point value"), PARAM_NUMBER,_T("Offset"))
) {
	long p1 = CNC_GetFloatParameter(rdPtr);
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2+sizeof(float)) <= d_vData.size())
	{
		*(long*)(&d_vData.at(p2)) = p1;
	}
	return 0;
}

#ifdef UNICODE
ACTION(
	/* ID */            33,
	/* Name */          _T("%o: Set string %0 at %1, Unicode flag: %2"),
	/* Flags */         0,
	/* Params */        (3, PARAM_STRING,_T("String"), PARAM_NUMBER,_T("Offset"), PARAM_NUMBER,_T("Unicode? (0: No, 1: Yes)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t p2 = GetInt();
	bool p3 = GetInt();

	if(numBoards && !d_bLock)
	{
		p2 = p2<d_vData.size()?p2:d_vData.size()-1; // Q?Y:N overflow security for the argument

		if(p3) //Unicode
		{ //UTF-16 LE
			const char *pointer = reinterpret_cast<const char*>(&p1[0]);
			size_t size = p1.size()*sizeof(p1.front());
			if(p2+size>d_vData.size())
				d_vData.resize(size+p2);
			copy(pointer, pointer+size, d_vData.begin()+p2);
		}
		else
		{
			if(p2+p1.size()>d_vData.size())
				d_vData.resize(p1.size()+p2);
			copy(p1.begin(),p1.end(),d_vData.begin()+p2);
		}
	}
	return 0;
}
#else
ACTION(
	/* ID */            33,
	/* Name */          _T("%o: Set string %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("String"), PARAM_NUMBER,_T("Offset"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t p2 = GetInt();
	std::string wstos(p1.begin(), p1.end()); // quick and dirty conversion
	const char* p1cstr = wstos.c_str();

	if(numBoards && !d_bLock)
		p2 = p2<d_vData.size()?p2:d_vData.size()-1; // Q?Y:N overflow security for the argument
		if(p2+p1.size()>d_vData.size())
			d_vData.resize(p1.size()+p2);
		copy(p1.begin(),p1.end(),d_vData.begin()+p2);
	return 0;
}
#endif


ACTION(
	/* ID */            34,
	/* Name */          _T("%o: Set content from board %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("Board name"), PARAM_NUMBER,_T("Offset"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	size_t p2 = GetInt();

	if(numBoards && !d_bLock)
	{
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		for(unsigned int i=0; i<numBoards; i++) //check if board already exists
		{
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			{
				copy(d_vDatai.begin(), d_vDatai.size()+p2 > d_vData.size() ? d_vDatai.end()-(d_vDatai.size()-d_vData.size())-p2 : d_vDatai.end(), d_vData.begin()+p2);
				break;
			}
		}
	}
	return 0;
}

ACTION(
	/* ID */			35,
	/* Name */			_T("%o: Set current content from file %0 at %1"),
	/* Flags */			0,
	/* Params */		(2, PARAM_FILENAME,_T("File"), PARAM_NUMBER,_T("Offset"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t p2 = GetInt();

	if(numBoards && !d_bLock)
	{
		std::ifstream file(p1,std::ios::binary); // open current file
		if(file.good())
		{
			file.unsetf(std::ios::skipws);

			file.seekg(0,std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0,std::ios::beg);

			if(p2 + fileSize <= d_vData.size())
				file.read((char*)&d_vData[p2],fileSize);
			file.close();
			d_vData.resize(d_vData.size() + fileSize);
		}
	}
	return 0;
}

/* APPEND */

ACTION(
	/* ID */            36,
	/* Name */          _T("%o: Append char %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_NUMBER,_T("Integer value"))
) {
	const char p1 = GetInt();
	if(numBoards && !d_bLock)
		d_vData.push_back(p1);
	return 0;
}

ACTION(
	/* ID */            37,
	/* Name */          _T("%o: Append short %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_NUMBER,_T("Integer value"))
) {
	long p1 = GetInt();

	if(numBoards && !d_bLock)
	{
		d_vData.push_back(p1 & 0xFF);
		d_vData.push_back((p1 >> 8) & 0xFF);
	}
	return 0;
}

ACTION(
	/* ID */            38,
	/* Name */          _T("%o: Append long %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_NUMBER,_T("Integer value"))
) {
	long p1 = GetInt();

	if(numBoards && !d_bLock)
	{
		const char* chars = reinterpret_cast<char*>(&p1);
		copy(chars,chars + sizeof(long),back_inserter(d_vData));
	}
	return 0;
}

ACTION(
	/* ID */            39,
	/* Name */          _T("%o: Append float %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_NUMBER,_T("Floating-point value"))
) {
	long p1 = CNC_GetFloatParameter(rdPtr);
	long p2 = GetInt();

	if(numBoards && !d_bLock)
	{
		const char* chars = reinterpret_cast<char*>(&p1);
		copy(chars,chars + sizeof(float),back_inserter(d_vData));
	}
	return 0;
}

#ifdef UNICODE
ACTION(
	/* ID */            40,
	/* Name */          _T("%o: Append string %0, Unicode flag: %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("String"), PARAM_NUMBER,_T("Unicode? (0: No, 1: Yes)"))
) {
	string p1 = string(GetStr());
	bool p2 = GetInt();
	if(!p1.length()) return 0;
	if(numBoards && !d_bLock)
		if(p2)  //UTF 16LE
		{
			const char *pointer = reinterpret_cast<const char*>(&p1[0]);
			size_t size = p1.size()*sizeof(p1.front());
			copy(pointer, pointer+size, back_inserter(d_vData));
		}
		else
		{
			copy(p1.begin(), p1.end(), back_inserter(d_vData));
		}
	return 0;
}
#else
ACTION(
	/* ID */            40,
	/* Name */          _T("%o: Append string %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_STRING,_T("String"))

) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	const char * cstr = p1.c_str();
	if(numBoards && !d_bLock)
		copy(p1.begin(), p1.end(), back_inserter(d_vData));
	return 0;
}
#endif


ACTION(
	/* ID */            41,
	/* Name */          _T("%o: Append content from board %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_STRING,_T("Board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bLock)
	{
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		for(unsigned int i=0; i<numBoards; i++) //check if board already exists
		{
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			{
				d_vData.reserve(d_vData.size() + d_vDatai.size());
				copy(d_vDatai.begin(),d_vDatai.end(),back_inserter(d_vData));
				break;
			}
		}
	}
	return 0;
}

ACTION(
	/* ID */			42,
	/* Name */			_T("%o: Append content from file %0"),
	/* Flags */			0,
	/* Params */		(1, PARAM_FILENAME,_T("File"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bLock)
	{
		std::ifstream file(p1,std::ios::binary);
		if(file.good())
		{
			file.unsetf(std::ios::skipws);

			file.seekg(0,std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0,std::ios::beg);

			size_t tempSize = d_vData.size();
			d_vData.resize(d_vData.size()+fileSize);
			file.read((char*)&d_vData[d_vData.size()-fileSize], fileSize);
			file.close();
		}
	}
	return 0;
}

/* INSERT */

ACTION(
	/* ID */            43,
	/* Name */          _T("%o: Insert char %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Value"), PARAM_NUMBER,_T("Offset"))
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2<=d_vData.size()))
		d_vData.emplace(d_vData.begin() + p2, p1);
	return 0;
}

ACTION(
	/* ID */            44,
	/* Name */          _T("%o: Insert short %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Value"), PARAM_NUMBER,_T("Offset"))
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2<=d_vData.size()))
	{
		const char* chars = reinterpret_cast<char*>(&p1);
		//if(p2+sizeof(short)<d_vData.size())
			d_vData.insert(d_vData.begin()+p2, chars, chars+sizeof(short));
		//else
			//copy(chars, chars+sizeof(short), back_inserter(d_vData)); // append instead
	}
	return 0;
}

ACTION(
	/* ID */            45,
	/* Name */          _T("%o: Insert long %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Value"), PARAM_NUMBER,_T("Offset"))
) {
	long p1 = GetInt();
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2<=d_vData.size()))
	{
		const char* chars = reinterpret_cast<char*>(&p1);
		//if(p2+sizeof(long)<d_vData.size())
			d_vData.insert(d_vData.begin()+p2, chars, chars+sizeof(long));
		//else
			//copy(chars, chars+sizeof(long), back_inserter(d_vData));
	}
	return 0;
}

ACTION(
	/* ID */            46,
	/* Name */          _T("%o: Insert float %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Value"), PARAM_NUMBER,_T("Offset"))
) {
	long p1 = CNC_GetFloatParameter(rdPtr);
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2<=d_vData.size()))
	{
		const char* chars = reinterpret_cast<char*>(&p1);
		//if(p2+sizeof(long)<d_vData.size())
			d_vData.insert(d_vData.begin()+p2, chars, chars+sizeof(float));
		//else
			//copy(chars, chars+sizeof(float), back_inserter(d_vData)); // append instead
	}
	return 0;
}

#ifdef UNICODE
ACTION(
	/* ID */            47,
	/* Name */          _T("%o: Insert string %0 at %1, Unicode flag: %2"),
	/* Flags */         0,
	/* Params */        (3, PARAM_STRING,_T("String"), PARAM_NUMBER,_T("Offset"), PARAM_NUMBER,_T("Unicode? (0: No, 1: Yes)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t p2 = GetInt();
	bool p3 = GetInt();

	if(numBoards && !d_bLock && (p2<=d_vData.size()))
		if (p3)
		{ //UTF-16 LE
			const char *pointer = reinterpret_cast<const char*>(&p1[0]);
			size_t size = p1.size()*sizeof(p1.front());

			//if(p2+p1.size()<d_vData.size())
				d_vData.insert(d_vData.begin()+p2, pointer, pointer+size);
			//else
				//copy(pointer, pointer+size, back_inserter(d_vData)); // append instead
		}
		else
		{
			int diff = 0;
			if(p2+p1.length()>d_vData.size())
				diff = (p2+p1.length())-d_vData.size();
			//if(p2+p1.size()<d_vData.size())
				d_vData.insert(d_vData.begin()+p2, p1.begin(), p1.end()-diff);
			//else
				//copy(p1.begin(), p1.end(), back_inserter(d_vData)); // append instead
		}
	return 0;
}
#else
ACTION(
	/* ID */            47,
	/* Name */          _T("%o: Insert string %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("String"), PARAM_NUMBER,_T("Offset"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	off_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2<=d_vData.size()))
	{
		//if(p2+p1.size()<d_vData.size())
			d_vData.insert(d_vData.begin()+p2, p1.begin(), p1.end());
		//else
			//copy(p1.begin(), p1.end(), back_inserter(d_vData)); // append instead
	}
	return 0;
}
#endif


ACTION(
	/* ID */            48,
	/* Name */          _T("%o: Insert content from board %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("Board name"), PARAM_NUMBER,_T("Offset"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2<=d_vData.size()))
	{
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		for(unsigned int i=0; i<numBoards; i++) //check if current board exists
		{
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			{
				d_vData.reserve(d_vData.size() + d_vDatai.size());
				//if (p2<d_vData.size())
					d_vData.insert(d_vData.begin()+p2, d_vDatai.begin(), d_vDatai.end());
				//else
					//copy(d_vDatai.begin(), d_vDatai.end(), back_inserter(d_vData)); //append
				break;
			}
		}
	}
	return 0;
}

ACTION(
	/* ID */			49,
	/* Name */			_T("%o: Insert content from file %0 at %1"),
	/* Flags */			0,
	/* Params */		(2, PARAM_FILENAME,_T("File"), PARAM_NUMBER,_T("Offset (-1: End)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2<=d_vData.size()))
	{
		std::ifstream file(p1,std::ios::binary);
		if(file.good())
		{
			file.unsetf(std::ios::skipws);

			file.seekg(0,std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0,std::ios::beg);

			if(p2 > d_vData.size())
				p2 = d_vData.size();

			d_vData.resize(d_vData.size()+fileSize);
			//if (p2<d_vData.size())
				d_vData.insert(d_vData.begin()+p2, d_vData.begin(), d_vData.begin() + fileSize);
			//else
				//file.read((char*)&d_vData[d_vData.size()-fileSize], fileSize); // append instead

			file.read((char*)&d_vData[p2],fileSize);
			file.close();
		}
	}
	return 0;
}


ACTION(
	/* ID */            50,
	/* Name */          _T("%o: Swap row of Bytes at %0 with order %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Offset"), PARAM_STRING,_T("(example of swapping Bytes: 0,3,1,2"))
) {
	size_t p1 = GetInt();
	string p2 = string(GetStr());
	if(!p2.length()) return 0;

	if(numBoards && !d_bLock && ((p1+sizeof(short)) <= d_vData.size()))
	{
		vector<char> integer;
		stringstream ss(p2);
		for (int i; ss >> i;)
		{
			integer.push_back(i);    
			if (ss.peek() == ',')
				ss.ignore();
		}

		for (unsigned char i=0; i<integer.size(); i++) // fill current buffer
		{
			int buff = d_vData[p1+integer[i]];
			integer[i] = buff;
		}

		for (unsigned char i=0; i<integer.size(); i++) //rewrite current data in new order
			d_vData[p1+i] = integer[i];

	}
	return 0;
}

ACTION(
	/* ID */            51,
	/* Name */          _T("%o: Flip integer value at %0"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Offset"), PARAM_NUMBER,_T("(0: short int, 1: long int)"))
) {
	size_t p1 = GetInt();
	bool p2 = GetInt();

	if(numBoards && !d_bLock && p1 + sizeof(long) <= d_vData.size())
	{
		if (!p2)
			*(short*)(&d_vData.at(p1)) = _byteswap_ushort(*reinterpret_cast<const short*>(&d_vData.at(p1)));
		else
			*(long*)(&d_vData.at(p1)) = _byteswap_ulong(*reinterpret_cast<const long*>(&d_vData.at(p1)));
	return 1;
	}
	return 0;
}

/* OCCURRENCE */
/* REPLACE */
ACTION(
	/* ID */            52,
	/* Name */          _T("%o: Replace occurences of integer %0 by %2"),
	/* Flags */         0,
	/* Params */        (4, PARAM_NUMBER,_T("Old integer value"), PARAM_NUMBER,_T("Size"), PARAM_NUMBER,_T("New integer value"), PARAM_NUMBER,_T("Size"))
) {
	long p1 = GetInt();
	unsigned char p2 = GetInt();
	long p3 = GetInt();
	unsigned char p4 = GetInt();

	if(numBoards && !d_bLock && p2)
	{
		if(p2 == 1 && p4 == 1)
			replace(d_vData.begin(),d_vData.end(),(signed char)p1,(signed char)p3);
		else
		{
			const char* value = reinterpret_cast<char*>(&p1);
			const char* value2 = reinterpret_cast<char*>(&p3);

			long pos = 0;
			while(true)
			{
				auto it = search(d_vData.begin()+pos,d_vData.end(), value, value+p2);
				pos = it-d_vData.begin();

				if(pos != d_vData.size())
				{
					if(p4 > p2)
						d_vData.insert(d_vData.begin()+pos, value2, value2+(p4-p2));
					copy(value2, value2+p4, d_vData.begin()+pos);
					if(p4 < p2)
						d_vData.erase(d_vData.begin()+pos+p4, d_vData.begin()+pos+p2);
					// Replacing by value sized 0 works same as removing
					pos += p4;
				}
				else
					break;

			}

		}
	}
	return 0;
}
//HARD
#ifdef UNICODE
ACTION(
	/* ID */            53,
	/* Name */          _T("%o: Replace occurences of string %0 by %1, Unicode flag: %2"),
	/* Flags */         0,
	/* Params */        (3, PARAM_STRING,_T("Old string"), PARAM_STRING,_T("New string"), PARAM_NUMBER,_T("Unicode? (0: No, 1: Yes)"))
) {
	string p1 = string(GetStr()); // str old
	string p4 = string(GetStr()); // str new
	bool p5 = GetInt(); // unicode
	if(!p1.length()) return 0;

	if(numBoards && !d_bLock)
	{
		size_t pos = 0;
		if(p5)
		{
			// get old string from data
			const char *pointerA = reinterpret_cast<const char*>(&p1[0]);
			size_t sizeA = p1.size()*sizeof(p1.front());
			// get new string
			const char *pointerB = reinterpret_cast<const char*>(&p4[0]);
			size_t sizeB = p1.size()*sizeof(p1.front());			
			// search
			const auto it = std::search(d_vData.begin()+pos, d_vData.end(), pointerA, pointerA+sizeA);
			pos = it-d_vData.begin();
			if(pos == d_vData.size())
				return 1;
			// replace
			if (p4.size())
			{
				if(p4.size() > p1.size()) d_vData.insert(d_vData.begin()+pos, p4.begin(), p4.begin()+(sizeB-sizeA));
				copy(pointerB, pointerB+sizeB, d_vData.begin()+pos);
				if(p4.size() < p1.size()) d_vData.erase(d_vData.begin()+pos+sizeB, d_vData.begin()+pos+sizeA);
			}
			else
				d_vData.erase(d_vData.begin()+pos, d_vData.begin()+pos+sizeA); // replacing with blank works the same as removing
			pos++;
		}
		else
		{
			while(true)
			{	
				//get old string from data
				const auto it = std::search(d_vData.begin()+pos, d_vData.end(), p1.begin(), p1.end());
				pos = it-d_vData.begin();
				if(pos == d_vData.size())
					return 1;
				if (p4.size())
				{
					if(p4.size() > p1.size()) d_vData.insert(d_vData.begin()+pos, p4.begin(), p4.begin()+(p4.size()-p1.size()));
					copy(p4.begin(), p4.end(), d_vData.begin()+pos);
					if(p4.size() < p1.size()) d_vData.erase(d_vData.begin()+pos+p4.size(), d_vData.begin()+pos+p1.size());
				}
				else
					d_vData.erase(d_vData.begin()+pos, d_vData.begin()+pos+p1.size()); // replacing with blank works the same as removing
				pos++;
			}
		}
	}
	return 0;
}
#else
ACTION(
	/* ID */            53,
	/* Name */          _T("%o: Replace occurences of string %0 by %1"),
	/* Flags */         0,
	/* Params */        (3, PARAM_STRING,_T("Old string"), PARAM_STRING,_T("New string"))
) {
	string p1 = string(GetStr());
	string p4 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bLock)
	{
		size_t pos = 0;
		while(true)
		{	
			//get old string from data
			const auto it = std::search(d_vData.begin()+pos, d_vData.end(), p1.begin(), p1.end());
			pos = it-d_vData.begin();
			if(pos == d_vData.size())
				return 1;
			if (p4.size())
			{
				if(p4.size() > p1.size()) d_vData.insert(d_vData.begin()+pos, p4.begin(), p4.begin()+(p4.size()-p1.size()));
				copy(p4.begin(), p4.end(), d_vData.begin()+pos);
				if(p4.size() < p1.size()) d_vData.erase(d_vData.begin()+pos+p4.size(), d_vData.begin()+pos+p1.size());
			}
			else
				d_vData.erase(d_vData.begin()+pos, d_vData.begin()+pos+p1.size());
			pos++;
		}
	}
	return 0;
}
#endif

ACTION(
	/* ID */            54,
	/* Name */          _T("%o: Replace occurences of content from board %0 by %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("Old board name"), PARAM_STRING,_T("New board name"))
) {
	string p1 = string(GetStr());
	string p2 = string(GetStr());
	if(!p1.length() || !p2.length()) return 0;

	if(numBoards && !d_bLock)
	{
		size_t pos = 0;
		for(unsigned int i=0; i<numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			{
				for(long j = 0; j < numBoards; j++) //check if board already exists
					if(d_sNamej[0] == p2[0] && d_sNamej == p2)
					{
						if (!d_vDataj.size())
							return 0;
						while(true)
						{
							auto it = search(d_vData.begin()+pos, d_vData.end(), d_vDatai.begin(), d_vDatai.end());
							pos = it-d_vData.begin();
							if(pos == d_vData.size())
								return 1;
							if (d_vDatai.size())
							{
								if(d_vDataj.size() > d_vDatai.size())
									d_vData.insert(d_vData.begin()+pos, d_vDataj.begin(), d_vDataj.begin()+(d_vDataj.size()-d_vDatai.size()));
								copy(d_vDataj.begin(), d_vDataj.end(), d_vData.begin()+pos);
								if(d_vDataj.size() < d_vDatai.size())
									d_vData.erase(d_vData.begin()+pos+d_vDataj.size(), d_vData.begin()+pos+d_vDatai.size());
							}
							else
								d_vData.erase(d_vData.begin()+pos, d_vData.begin()+pos+d_vDatai.size()); // Replacing by blank works same as removing							
							pos += d_vDataj.size();
						}
						break;
					}
				break;
			}
	}
	return 0;
}

/* OCCURRENCES */
/* REMOVE */
ACTION(
	/* ID */            55,
	/* Name */          _T("%o: Remove occurences of integer %0"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Integer value"), PARAM_NUMBER,_T("Size (0: Auto)"))
) {
	int p1 = GetInt();
	int p2 = GetInt();

	if(numBoards && !d_bLock)
	{
		if (!p2)
			p2 = SizeOfValue3(p1);
		{
			const char* value = reinterpret_cast<char*>(&p1);
			size_t pos = 0;
			while(true)
			{
				auto it = search(d_vData.begin(), d_vData.end(), value, value+p2);
				pos = it-d_vData.begin();
				if (pos == d_vData.size())
					break;
				d_vData.erase(d_vData.begin()+pos, d_vData.begin()+pos+p2);
			}

		}
	}
	return 0;
}

#ifdef UNICODE
ACTION(
	/* ID */            56,
	/* Name */          _T("%o: Remove occurences of string %0, Unicode flag: %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_STRING,_T("String"), PARAM_NUMBER,_T("Unicode? (0: No, 1: Yes)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	bool p3 = GetInt();

	if(numBoards && !d_bLock)
	{
		size_t pos = 0;
		if (p3) //Unicode
		{
			while(true)
			{
				const char *pointer = reinterpret_cast<const char*>(&p1[0]);
				size_t size = p1.size()*sizeof(p1.front());
				const auto it = std::search(d_vData.begin()+pos, d_vData.end(), pointer, pointer+size);
				pos = it-d_vData.begin();
				if(pos == d_vData.size())
					return 1;
				d_vData.erase(d_vData.begin()+pos, d_vData.begin()+pos+size);
			}
		}
		else
		{
			while(true)
			{
				const auto it = std::search(d_vData.begin()+pos, d_vData.end(), p1.begin(), p1.end());
				pos = it-d_vData.begin();
				if(pos == d_vData.size())
					return 1;
				d_vData.erase(d_vData.begin()+pos, d_vData.begin()+pos+p1.size());
			}
		}
	}
	return 0;
}
#else
ACTION(
	/* ID */            56,
	/* Name */          _T("%o: Remove occurences of string %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_STRING,_T("String"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bLock)
	{
		size_t pos = 0;
		while(true)
		{
			const auto it = std::search(d_vData.begin()+pos, d_vData.end(), p1.begin(), p1.end());
			pos = it-d_vData.begin();
			if(pos == d_vData.size())
				return 1;
			d_vData.erase(d_vData.begin()+pos, d_vData.begin()+pos+p1.size());
		}
	}
	return 0;
}
#endif

ACTION(
	/* ID */            57,
	/* Name */          _T("%o: Remove occurences of content from board %0"),
	/* Flags */         0,
	/* Params */        (1, PARAM_STRING,_T("Board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bLock)
	{
		size_t pos = 0;
		for(unsigned int i=0; i<numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1, rdPtr->bCaseSensitive))
			{
				if (!d_vDatai.size())
					return 0;
				while(true)
				{
					auto it = search(d_vData.begin()+pos, d_vData.end(), d_vDatai.begin(), d_vDatai.end());
					pos = it-d_vData.begin();
					if(pos == d_vData.size())
						return 1;
					d_vData.erase(d_vData.begin()+pos, d_vData.begin()+pos+d_vDatai.size());
				}
			}
	}
	return 0;
}

/* ENDIANNESS */

ACTION(
	/* ID */            58,
	/* Name */          _T("%o: Set little-endian"),
	/* Flags */         0,
	/* Params */        (0)
) {
	rdPtr->bEndianness = false;
	return 0;
}

ACTION(
	/* ID */            59,
	/* Name */          _T("%o: Set big-endian"),
	/* Flags */         0,
	/* Params */        (0)
) {
	rdPtr->bEndianness = true;
	return 0;
}


ACTION(
	/* ID */            60,
	/* Name */          _T("%o: Insert blank space sized %0 at %1"),
	/* Flags */         0,
	/* Params */        (2, PARAM_NUMBER,_T("Size"), PARAM_NUMBER,_T("Offset"))
) {
	size_t p1 = GetInt();
	size_t p2 = GetInt();

	if(numBoards && !d_bLock && (p2<=d_vData.size()))
	{
		std::vector<char> blank;
		blank.resize(p1);
		d_vData.reserve(d_vData.size()+p1);
		d_vData.insert(d_vData.begin()+p2, blank.begin(), blank.end());
	}
	return 0;
}

