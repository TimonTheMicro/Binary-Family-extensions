#include "common.h"

// ============================================================================
//
// ACTIONS
// 
// ============================================================================

/*BOARD*/

ACTION(
	/* ID */            0,
	/* Name */          _T("%o: Create a board %0, size %1 Bytes"),
	/* Flags */         EXPFLAG_STRING,
	/* Params */        (2,PARAM_STRING,_T("Board name"),PARAM_NUMBER,_T("*Size (in Bytes)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	size_t p2 = GetInt();

	if(p1.length()) //length
	{
		bool exists = false;
		for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1))
			{
				exists = true;
				break;
			}

		if(!exists)
		{
			rdPtr->vBoards.push_back(Board());
			unsigned int selected = numBoards - 1;
			rdPtr->vBoards[selected].sName = p1;
			rdPtr->vBoards[selected].bProtected = false;
			if(p2)
				rdPtr->vBoards[selected].vData.resize(p2);
			if(rdPtr->bAutoSelect)
				rdPtr->iSelBoard = selected;
		}
	}
	return 0;
}

ACTION(
	/* ID */            1,
	/* Name */          _T("%o: Select board %0"),
	/* Flags */         0,
	/* Params */        (1,PARAM_STRING,_T("Board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards)
	{
		for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1))
			{
				rdPtr->lastBoard = d_sName; //history
				rdPtr->iSelBoard = i;
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
	if(numBoards)
	{
		for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, rdPtr->lastBoard))
			{
				rdPtr->iSelBoard = i;
				return 1;
			}
	}
	return 0;
}


ACTION(
	/* ID */            3,
	/* Name */          _T("%o: Rename current board to %0"),
	/* Flags */         0,
	/* Params */        (1,PARAM_STRING,_T("Board new name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bProtected)
	{
		for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1))
				return 0; // a board with new name already exists, exiting

		d_sName = p1; //if there will be no dublicated names
	}

	return 0;
}

ACTION(
	/* ID */            4,
	/* Name */          _T("%o: Rename board %0 to %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("Board old name"),PARAM_STRING,_T("Board new name"))
) {
	string p1 = string(GetStr());
	string p2 = string(GetStr());
	if(!p1.length() || !p2.length()) return 0;

	for(unsigned int i=0; i<numBoards; i++) // check if new board already exists
		if(strCompare(d_sNamei, p2))
			return 0; // name exists, prevent dublicate, exiting
					  // it doesn't, moving forward
	for(unsigned int i=0; i<numBoards; i++) // check if old board already exists
	{
		if(strCompare(d_sNamei, p1)) // it does
		{ 
			d_sNamei = p2; // it does, change the old name to new
			break;
		}
	}

	return 0;
}

ACTION(
	/* ID */            5,
	/* Name */          _T("%o: Set current board content lock flag to %0"),
	/* Flags */         0,
	/* Params */        (1,PARAM_NUMBER,_T("Content locked (0: no, 1: yes)"))

) {
	bool p1 = GetInt();

	if(numBoards)
		d_bProtected = p1;
	return 0;
}

ACTION(
	/* ID */            6,
	/* Name */          _T("%o: Set board %0 content lock flag to %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("Board name"), PARAM_NUMBER,_T("Content locked (0: no, 1: yes)"))
) {
	string p1 = string(GetStr());
	bool p2 = GetInt();
	if(!p1.length()) return 0;

	for(unsigned int i=0; i<numBoards; i++) //check if board already exists
		if(strCompare(d_sNamei, p1))
		{
			rdPtr->vBoards[i].bProtected = p2;
			break;
		}

	return 0;
}

ACTION(
	/* ID */            7,
	/* Name */          _T("%o: Resize current board container to %0 Bytes"),
	/* Flags */         0,
	/* Params */        (1,PARAM_NUMBER,_T("Size (in Bytes)"))
) {
	size_t p1 = GetInt();

	if(numBoards && !d_bProtected)
		if(p1 != -1)
		{
			if(p1 <= d_vData.size())
				d_vData.reserve(p1);

			d_vData.resize(p1);
			d_vData.shrink_to_fit();
		}
	return 0;
}

ACTION(
	/* ID */            8,
	/* Name */          _T("%o: Resize board container %0 to %1 Bytes"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("Board name"),PARAM_NUMBER,_T("Size (in Bytes)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	size_t p2 = GetInt();

	for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
		if(strCompare(d_sNamei, p1))
		{
			if(!d_bProtectedi)
				if(p2 != -1)
				{
					if(p2 > d_vData.size())
						d_vData.reserve(p2);

					rdPtr->vBoards[i].vData.resize(p2);
					rdPtr->vBoards[i].vData.shrink_to_fit();
					break;
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

	if(numBoards && !d_bProtected)
	{
		unsigned int i = rdPtr->iSelBoard;
		{
			rdPtr->vBoards.erase(rdPtr->vBoards.begin()+i, rdPtr->vBoards.begin()+i+1);
			rdPtr->vBoards.shrink_to_fit();
			if(rdPtr->iSelBoard == numBoards) //if selected board number is equal to boards number, decrease it to not overflow.
				rdPtr->iSelBoard--;
		}
	}
	return 0;
}

ACTION(
	/* ID */            10,
	/* Name */          _T("%o: Delete board %0"),
	/* Flags */         0,
	/* Params */        (1,PARAM_STRING,_T("Board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	for(unsigned int i=0; i<numBoards; i++) //check if board already exists
		if(strCompare(d_sNamei, p1))
		{
			if(!d_bProtectedi)
			{
				rdPtr->vBoards.erase(rdPtr->vBoards.begin()+i, rdPtr->vBoards.begin()+i+1);
				rdPtr->vBoards.shrink_to_fit();
				if(rdPtr->iSelBoard >= numBoards)
					rdPtr->iSelBoard--;
			}
			break;
		}
	return 0;
}


ACTION(
	/* ID */            11,
	/* Name */          _T("%o: Clear Everything"),
	/* Flags */         0,
	/* Params */        (0)
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	size_t p2 = GetInt();

	rdPtr->vBoards.clear();
	rdPtr->vBoards.shrink_to_fit();

	return 0;
}

/*CONTENT*/

ACTION(
	/* ID */			12,
	/* Name */			_T("%o: Load file %0 from offset %2, size %1 Bytes to current board"),
	/* Flags */			0,
	/* Params */		(3, PARAM_FILENAME2,_T("File"), PARAM_NUMBER,_T("Begin (in Bytes)"), PARAM_NUMBER,_T("Size (in Bytes, -1: End of File)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	off_t p2 = GetInt();
	off_t p3 = GetInt();

	if(numBoards && !d_bProtected && p3)
	{

		std::ifstream file(p1,std::ios::binary);
		if(file.good())
		{
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

ACTION(
	/* ID */			13,
	/* Name */			_T("%o: Save board to file %0"),
	/* Flags */			0,
	/* Params */		(2,PARAM_FILENAME,_T("File"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards)
	{
		std::ofstream fout(p1,std::ios::out | std::ios::binary);
		fout.write((char*)&(d_vData)[0],d_vData.size());
		fout.close();
	}
	return 0;
}
// might be taken as false positive for dumb Windows 10 antivirus
ACTION(
	/* ID */			14,
	/* Name */			_T("%o: Load resource (PE) from file %0 to current board, type %1, find %2,  index %3"),
	/* Flags */			0,
	/* Params */		(4,PARAM_FILENAME,_T("File"),PARAM_NUMBER,_T("Type (example: 0x00000020: LOAD_LIBRARY_AS_IMAGE_RESOURCE)"),PARAM_NUMBER,_T("Type (example: 3: RT_ICON)"),PARAM_NUMBER,_T("Index"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	int resType = GetInt();
	int resFind = GetInt();
	int resIndex = GetInt();

	if(numBoards && !d_bProtected && p1.size())
	{
		std::ifstream file(p1,std::ios::binary);
		if(file.good())
		{
			d_vData.clear();
			file.unsetf(std::ios::skipws);
			file.seekg(0,std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0,std::ios::beg);
			std::vector<char> resContainer(fileSize); //prepare a vector of Bytes for the binary file
			if(file.read(resContainer.data(),fileSize)) //if file loaded into the vector
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
				//that thing wanted a constant value as a parameter. C++ in a nutshell, had to case the argument
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
	/* Params */		(1,PARAM_FILENAME,_T("File"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	off_t pos = 0;

	if(numBoards)
	{
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
			BoardHeader.attr = d_bProtectedi;
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
	/* Params */		(1,PARAM_FILENAME,_T("File"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	std::ifstream file(p1, std::ios::binary); // open the file
	file.unsetf(std::ios::skipws); // stop eating new lines in binary mode
	file.seekg(0, std::ios::beg);
	file.read((char*)&WorkspaceHeader, sizeof(WorkspaceHeader));
	std::string str(WorkspaceHeader.magic);
	if((str == "BINB") && (WorkspaceHeader.ver == 2.0)) // compare with the header
		for (int i=0; i<WorkspaceHeader.count; i++) // read board index
		{
			file.read((char*)&BoardHeader, sizeof(BoardHeader)); // read the board header

			char buffer[256]; // maximum length is 256 characters for CF2.5 argument
#ifdef UNICODE
			file.read((char*)&buffer[0], BoardHeader.nSize*sizeof(wchar_t)); // read the board name
			std::wstring bName = reinterpret_cast<TCHAR*>(buffer);
#else // ANSI
			file.read((char*)&buffer[0], BoardHeader.nSize); // read the board name
			std::string bName = buffer;
#endif
			bName.resize(BoardHeader.nSize);

			unsigned int j;
			rdPtr->iSelBoard = 0;
			bool exists = 0;
			for(j = 0; j < numBoards; j++) //check if board already exists
			{
				if(strCompare(d_sNamej, bName))
				{
					rdPtr->iSelBoard = j; // select it
					exists = true;
					break;
				}
			}
			if(!exists)
			{ 
				rdPtr->vBoards.push_back(Board()); // create a new board
				rdPtr->iSelBoard = numBoards - 1;
			}
			if(!d_bProtected)
			{
				d_bProtected = BoardHeader.attr;
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
	return 0;
}

ACTION(
	/* ID */            17,
	/* Name */          _T("%o: Swap data of current board with %0"),
	/* Flags */         0,
	/* Params */        (1,PARAM_STRING,_T("Board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bProtected)
	{
		bool exists = false;
		long slot = 0;
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1))
			{
				if(!d_bProtectedi)
					d_vData.swap(d_vDatai);
				break;
			}
	}
	return 0;
}

ACTION(
	/* ID */            18,
	/* Name */          _T("%o: Copy %0 Bytes at %1 to board (%2)"),
	/* Flags */         0,
	/* Params */        (3,PARAM_NUMBER,_T("size (in Bytes), -1: Size"),PARAM_NUMBER,_T("at offset"),PARAM_STRING,_T("Board name"))
) {
	size_t p1 = GetInt();
	off_t p2 = GetInt();
	string p3 = string(GetStr());
	if(!p3[0]) return 0;

	if(rdPtr->vBoards.size())
	{
		bool exists = false;
		unsigned int i;
		if(numBoards && p3.length())
		{
			for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
				if(strCompare(d_sNamei,p3))
				{
					exists = true;
					d_vDatai.clear();
					copy(d_vData.begin() + p2,d_vData.size() > p1 ? d_vData.begin() + p2 + p1 : d_vData.end(),back_inserter(d_vDatai));
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
	/* Params */        (2,PARAM_NUMBER,_T("Size (in Bytes), -1: Size"),PARAM_NUMBER,_T("Offset"))
) {
	size_t p1 = GetInt();
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected)
	{
		d_vData.erase(d_vData.begin(),d_vData.begin() + p2);
		d_vData.erase(p1 == -1 ? d_vData.end() : d_vData.begin() + p1,d_vData.end());
		d_vData.shrink_to_fit();
	}
	return 0;
}

ACTION(
	/* ID */            20,
	/* Name */          _T("%o: Remove %0 Bytes at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,_T("Size (in Bytes)"),PARAM_NUMBER,_T("Offset"))
) {
	size_t p1 = GetInt();
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected && p2 < p1)
	{
		d_vData.erase(d_vData.begin() + p2,d_vData.begin() + p1 + p2);
		d_vData.shrink_to_fit();
	}
	return 0;
}


ACTION(
	/* ID */            21,
	/* Name */          _T("%o: Fill data by char %0 in current board"),
	/* Flags */         0,
	/* Params */        (1,PARAM_NUMBER,_T("Fill by"))
) {
	long p1 = GetInt();

	if(numBoards && !d_bProtected)
		fill(d_vData.begin(),d_vData.end(),p1);
	return 0;
}

/*Compress*/
//Zlib (Mark Adler) */
ACTION(
	/* ID */            22,
	/* Name */          _T("%o: Compress current data using zlib, level %0"),
	/* Flags */         0,
	/* Params */        (1,PARAM_NUMBER,_T("Compression level (0: No compression, -1: Default, 1: Best speed, 9: Best compression)"))
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
	/* Name */          _T("%o: Decompress current data using zlib"),
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
/* Encrypt */
// Blowfish
ACTION(
	/* ID */            24,
	/* Name */          _T("%o: Encrypt current data using blowfish, key %0, method %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("Set key string (length from 4 to 56 characters)"),PARAM_NUMBER,_T("Method (0: Simple, 1: Strict)"))
) {
	string p1 = string(GetStr());
	if(!p1[0]) return 0;
	bool p2 = GetInt();
	/*
	if(numBoards && wcslen(p1) >= 4 && d_vData.size() >= 8)
	{
		BLOWFISH_CTX ctx;

		Blowfish_Init(&ctx,(unsigned char*)p1,min(wcslen(p1),56));

		if(p2)
			for(size_t i = 0; i < d_vData.size() + 1 - 8; i++)
				Blowfish_Encrypt(&ctx,(unsigned long*)(d_vData.data() + i),(unsigned long*)(d_vData.data() + i + 4));
		else
			for(size_t i = 0; i < d_vData.size() / 8; i++)
				Blowfish_Encrypt(&ctx,(unsigned long*)&d_vData.at(i * 4),(unsigned long*)&d_vData.at((i + 1) * 4));
	}*/
	return 0;
}

ACTION(
	/* ID */            25,
	/* Name */          _T("%o: Decrypt current data using blowfish, key %0, method %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("Set key string (length from 4 to 56 characters)"),PARAM_NUMBER,_T("Strict mode (0: No, 1: Yes)"))
) {/*
	TCHAR* p1 = GetStr();
	bool p2 = GetInt();
	if(numBoards && wcslen(p1) >= 4 && d_vData.size() >= 8)
	{
		BLOWFISH_CTX ctx;

		Blowfish_Init(&ctx,(unsigned char*)p1,min(wcslen(p1),56));

		if(p2)
			for(int i = d_vData.size() - 8; i >= 0; i--)
				Blowfish_Decrypt(&ctx,(unsigned long*)(d_vData.data() + i),(unsigned long*)(d_vData.data() + i + 4));
		else
			for(size_t i = 0; i < d_vData.size() / 8; i++)
				Blowfish_Decrypt(&ctx,(unsigned long*)&d_vData.at(i * 4),(unsigned long*)&d_vData.at((i + 1) * 4));
	}*/
	return 0;
}
/* ENCODE */
//Base64
ACTION(
	/* ID */            26,
	/* Name */          _T("%o: Encode current data using base64"),
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
	/* Name */          _T("%o: Decode current data using base64"),
	/* Flags */         0,
	/* Params */        (0)
) {

	if(numBoards)
	{
		Base64decode(d_vData.data(),d_vData.data());
	}
	return 0;
}

/* VALUE */

ACTION(
	/* ID */            28,
	/* Name */          _T("%o: Set bit %0 at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,_T("Boolean value"),PARAM_NUMBER,_T("Offset"))
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
	if(numBoards && !d_bProtected && offset < d_vData.size()) //I don't want errors in my extensions
		d_vData.at(offset) = output;
	return 0;
}

ACTION(
	/* ID */            29,
	/* Name */          _T("%o: Set char %0 at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,_T("Integer value"),PARAM_NUMBER,_T("Offset"))
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected && p2 < d_vData.size()) //I don't want errors in my extensions
		d_vData.at(p2) = p1;
	return 0;
}

ACTION(
	/* ID */            30,
	/* Name */          _T("%o: Set short int %0 at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,_T("Integer value"),PARAM_NUMBER,_T("Offset"))
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected && p2 + sizeof(short) <= d_vData.size())
		*(short*)(&d_vData.at(p2)) = p1;
	return 0;
}

ACTION(
	/* ID */            31,
	/* Name */          _T("%o: Set long int %0 at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,_T("Integer value"),PARAM_NUMBER,_T("Offset"))
) {
	long p1 = GetInt();
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected && p2 + sizeof(long) <= d_vData.size())
		*(long*)(&d_vData.at(p2)) = p1;
	return 0;
}

ACTION(
	/* ID */            32,
	/* Name */          _T("%o: Set float %0 at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,_T("Single precision floating-point value"),PARAM_NUMBER,_T("Offset"))
) {
	long p1 = CNC_GetFloatParameter(rdPtr);
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected && p2 + sizeof(float) <= d_vData.size())
	{
		*(long*)(&d_vData.at(p2)) = p1;
	}
	return 0;
}

#ifdef UNICODE
ACTION(
	/* ID */            33,
	/* Name */          _T("%o: Set String %0 at %1, Unicode flag: %2"),
	/* Flags */         0,
	/* Params */        (3,PARAM_STRING,_T("String"), PARAM_NUMBER,_T("Offset"), PARAM_NUMBER,_T("Unicode? (0: no, 1: yes)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	off_t p2 = GetInt();
	bool p3 = GetInt();

	if(numBoards && !d_bProtected)
		if(p3) //Unicode
		{ //UTF-16 LE
			const char *pointer = reinterpret_cast<const char*>(&p1[0]);
			size_t size = p1.size()*sizeof(p1.front());
			if(p2+size>d_vData.size())
				d_vData.resize(p2+size);
			copy(pointer, pointer+size, d_vData.begin()+p2);
		}
		else
		{ //ANSI
			//std::string p1s(p1.begin(),p1.end()); // quick and dirty conversion
			if(p2+p1.size()>d_vData.size())
				d_vData.resize(p2+p1.size());
			copy(p1.begin(),p1.end(),d_vData.begin()+p2);
		}
	return 0;
}
#else
ACTION(
	/* ID */            33,
	/* Name */          _T("%o: Set String %0 at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("String"), PARAM_NUMBER,_T("Offset"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	off_t p2 = GetInt();
	std::string wstos(p1.begin(), p1.end()); // quick and dirty conversion
	const char* p1cstr = wstos.c_str();

	if(numBoards && !d_bProtected)
		copy(p1cstr, std::strlen(p1cstr)+p2 > d_vData.size() ? p1cstr+std::strlen(p1cstr)-(std::strlen(p1cstr) - d_vData.size()) - p2 : p1cstr + std::strlen(p1cstr),d_vData.begin() + p2);
	return 0;
}
#endif


ACTION(
	/* ID */            34,
	/* Name */          _T("%o: Set content from %0 at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("Board name"),PARAM_NUMBER,_T("Offset"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	off_t p2 = GetInt();

	if(numBoards && !d_bProtected)
	{
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
		{
			if(strCompare(d_sNamei, p1))
			{
				copy(d_vDatai.begin(),d_vDatai.size() + p2 > d_vData.size() ? d_vDatai.end() - (d_vDatai.size() - d_vData.size()) - p2 : d_vDatai.end(),d_vData.begin() + p2);
				break;
			}
		}
	}
	return 0;
}

ACTION(
	/* ID */			35,
	/* Name */			_T("%o: Set content from %0 at %1 in current board"),
	/* Flags */			0,
	/* Params */		(2,PARAM_FILENAME,_T("File"),PARAM_NUMBER,_T("Offset (-1: End)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	long p2 = GetInt();

	if(numBoards && !d_bProtected)
	{
		std::ifstream file(p1,std::ios::binary); // open the file
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
	/* Params */        (1,PARAM_NUMBER,_T("Integer value"))
) {
	const char p1 = GetInt();
	if(numBoards && !d_bProtected)
		d_vData.push_back(p1);
	return 0;
}

ACTION(
	/* ID */            37,
	/* Name */          _T("%o: Append short %0"),
	/* Flags */         0,
	/* Params */        (1,PARAM_NUMBER,_T("Integer value"))
) {
	long p1 = GetInt();

	if(numBoards && !d_bProtected)
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
	/* Params */        (1,PARAM_NUMBER,_T("Integer value"))
) {
	long p1 = GetInt();

	if(numBoards && !d_bProtected)
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
	/* Params */        (1,PARAM_NUMBER,_T("Floating-point value"))
) {
	long p1 = CNC_GetFloatParameter(rdPtr);
	long p2 = GetInt();

	if(numBoards && !d_bProtected)
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
	/* Params */        (2,PARAM_STRING,_T("String"),PARAM_NUMBER,_T("Unicode? (0: No, 1: Yes)"))
) {
	string p1 = string(GetStr());
	bool p2 = GetInt();
	if(!p1.length()) return 0;
	if(numBoards && !d_bProtected)
		if(p2) //Unicode
		{
			const char *pointer = reinterpret_cast<const char*>(&p1[0]);
			size_t size = p1.size()*sizeof(p1.front());
			copy(p1.begin(), p1.end(), back_inserter(d_vData));
		}
		else //Ansi
		{
			//std::string p1s(p1.begin(),p1.end()); // quick and dirty conversion
			copy(p1.begin(),p1.end(),back_inserter(d_vData));
		}
	return 0;
}
#else
ACTION(
	/* ID */            40,
	/* Name */          _T("%o: Append string %0"),
	/* Flags */         0,
	/* Params */        (1,PARAM_STRING,_T("String"))

) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	const char * cstr = p1.c_str();
	if(numBoards && !d_bProtected)
		copy(p1.begin(), p1.end(), back_inserter(d_vData));
	return 0;
}
#endif


ACTION(
	/* ID */            41,
	/* Name */          _T("%o: Append content from board %0"),
	/* Flags */         0,
	/* Params */        (1,PARAM_STRING,_T("Board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bProtected)
	{
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
		{
			if(strCompare(d_sNamei, p1))
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
	/* Params */		(1,PARAM_FILENAME,_T("File"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bProtected)
	{
		std::ifstream file(p1,std::ios::binary);
		if(file.good())
		{
			file.unsetf(std::ios::skipws);

			file.seekg(0,std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0,std::ios::beg);

			size_t tempSize = d_vData.size();
			d_vData.resize(d_vData.size() + fileSize);
			file.read((char*)&d_vData[d_vData.size() - fileSize],fileSize);
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

	if(numBoards && !d_bProtected)
		d_vData.emplace(d_vData.begin() + p2,p1);
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

	if(numBoards && !d_bProtected)
	{
		const char* chars = reinterpret_cast<char*>(&p1);
		d_vData.insert(d_vData.begin() + p2,chars,chars + sizeof(short));
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
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected)
	{
		const char* chars = reinterpret_cast<char*>(&p1);
		d_vData.insert(d_vData.begin()+p2, chars, chars+sizeof(long));
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
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected)
	{
		const char* chars = reinterpret_cast<char*>(&p1);
		d_vData.insert(d_vData.begin()+p2, chars, chars+sizeof(float));
	}
	return 0;
}

#ifdef UNICODE
ACTION(
	/* ID */            47,
	/* Name */          _T("%o: Insert string %0 at %1, Unicode flag: %2"),
	/* Flags */         0,
	/* Params */        (3,PARAM_STRING,_T("String"),PARAM_NUMBER,_T("Offset"),PARAM_NUMBER,_T("Unicode? (0: No, 1: Yes)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	off_t p2 = GetInt();
	bool p3 = GetInt();

	if(numBoards && !d_bProtected)
		if (p3)
		{ //UTF-16 LE
			const char *pointer = reinterpret_cast<const char*>(&p1[0]);
			size_t size = p1.size()*sizeof(p1.front());
			if(p2+size>d_vData.size())
				d_vData.resize(p2+size);
			d_vData.insert(d_vData.begin()+p2, p1.begin(), p1.end());
		}
		else
		{ //ANSI
		  //std::string p1s(p1.begin(),p1.end()); // quick and dirty conversion
			if(p2+p1.size()>d_vData.size())
				d_vData.resize(p2+p1.size());
			d_vData.insert(d_vData.begin()+p2, p1.begin(), p1.end());
		}
	return 0;
}
#else
ACTION(
	/* ID */            47,
	/* Name */          _T("%o: Insert string %0 at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("String"),PARAM_NUMBER,_T("Offset"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected)
		d_vData.insert(d_vData.begin()+p2, p1.begin(), p1.end());
	return 0;
}
#endif


ACTION(
	/* ID */            48,
	/* Name */          _T("%o: Insert content from board %0 at %1"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("Board name"),PARAM_NUMBER,_T("Offset"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	off_t p2 = GetInt();

	if(numBoards && !d_bProtected)
	{
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		for(unsigned int i = 0; i < numBoards; i++) //check if the board exists
		{
			if(strCompare(d_sNamei, p1))
			{
				d_vData.reserve(d_vData.size() + d_vDatai.size());
				d_vData.insert(d_vData.begin() + p2,d_vDatai.begin(),d_vDatai.end());
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
	/* Params */		(2,PARAM_FILENAME,_T("File"),PARAM_NUMBER,_T("Offset (-1: End)"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	off_t p2 = GetInt();

	if(numBoards && !d_bProtected)
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

			d_vData.insert(d_vData.begin() + p2,d_vData.begin(),d_vData.begin() + fileSize);
			file.read((char*)&d_vData[p2],fileSize);
			file.close();
		}
	}
	return 0;
}


ACTION(
	/* ID */            50,
	/* Name */          _T("%o: Flip row of Bytes at %0"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,_T("Offset"),PARAM_STRING,_T("(example of swapping Bytes: 0,3,1,2"))
) {
	off_t p1 = GetInt();
	string p2 = string(GetStr());
	if(!p2.length()) return 0;

	if(numBoards && !d_bProtected && p1 + sizeof(short) <= d_vData.size())
	{
		std::vector<char> integer;
		stringstream ss(p2);

		for (int i; ss >> i;)
		{
			integer.push_back(i);    
			if (ss.peek() == ',')
				ss.ignore();
		}
		for (std::size_t i=0; i<integer.size(); i++) // fill the buffer
		{
			int buff = d_vData[p1+integer[i]];
			integer[i] = buff;
		}
		for (std::size_t i=0; i<integer.size(); i++) //rewrite the data in new order
			d_vData[p1+i] = integer[integer.size()-i];

	}
	return 0;
}

/* OCCURRENCE */
/* REPLACE */
ACTION(
	/* ID */            51,
	/* Name */          _T("%o: Replace every integer %0 size %1 by %2 size %3 in current board"),
	/* Flags */         0,
	/* Params */        (4,PARAM_NUMBER,_T("Old integer value"),PARAM_NUMBER,_T("Size"),PARAM_NUMBER,_T("New integer value"),PARAM_NUMBER,_T("Size"))
) {
	long p1 = GetInt();
	char p2 = GetInt();
	long p3 = GetInt();
	char p4 = GetInt();

	if(numBoards && !d_bProtected)
	{
		if(p2 == 1 && p4 == 1)
			replace(d_vData.begin(),d_vData.end(),(signed char)p1,(signed char)p3);
		else
		{
			const char* value = reinterpret_cast<char*>(&p1);
			const char* value2 = reinterpret_cast<char*>(&p3);

			long dist = 0;
			while(true)
			{
				auto it = search(d_vData.begin() + dist,d_vData.end(),value,value + sizeof(char) * p2);
				dist = it - d_vData.begin();

				if(dist != d_vData.size())
				{
					if(p4 > p2)
						d_vData.insert(d_vData.begin() + dist,value2,value2 + sizeof(char) * (p4 - p2));
					copy(value2,value2 + sizeof(char) * p4,d_vData.begin() + dist);
					if(p4 < p2)
						d_vData.erase(d_vData.begin() + dist + p4,d_vData.begin() + dist + p2);

					dist += sizeof(char) * p4;
				}
				else
					break;

			}

		}
	}
	return 0;
}

ACTION(
	/* ID */            52,
	/* Name */          _T("%o: Replace every string %0 by %1 in current board"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("Old string"),PARAM_STRING,_T("New string"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	string p2 = string(GetStr());
	if(!p2.length()) return 0;

	if(numBoards && !d_bProtected)
	{
		long dist = 0;
		while(true)
		{/*
			auto it = search(d_vData.begin() + dist,d_vData.end(),p1,p1 + strlen(p1));
			dist = it - d_vData.begin();

			if(dist != d_vData.size())
			{
				if(wcslen(p2) > wcslen(p1))
					d_vData.insert(d_vData.begin() + dist,p2,p2 + (wcslen(p2) - wcslen(p1)));
				copy(p2,p2 + wcslen(p2),d_vData.begin() + dist);
				if(wcslen(p2) < wcslen(p1))
					d_vData.erase(d_vData.begin() + dist + wcslen(p2),d_vData.begin() + dist + wcslen(p1));

				dist += wcslen(p2);
			}
			else
				break;*/
		}
	}
	return 0;
}

ACTION(
	/* ID */            53,
	/* Name */          _T("%o: Replace every content of board %0 by %1 in current board"),
	/* Flags */         0,
	/* Params */        (2,PARAM_STRING,_T("Old board name"),PARAM_STRING,_T("New board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;
	string p2 = string(GetStr());
	if(!p2.length()) return 0;

	if(numBoards && !d_bProtected)
	{
		long dist = 0;
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		transform(p2.begin(),p2.end(),p2.begin(),::tolower);
		for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1))
			{
				for(long j = 0; j < numBoards; j++) //check if board already exists
					if(d_sNamej[0] == p2[0] && d_sNamej == p2)
					{
						while(true)
						{
							auto it = search(d_vData.begin() + dist,d_vData.end(),d_vDatai.begin(),d_vDatai.end());
							dist = it - d_vData.begin();
							if(dist == d_vData.size())
								break;
							if(d_vDataj.size() > d_vDatai.size())
								d_vData.insert(d_vData.begin() + dist,d_vDataj.begin(),d_vDataj.begin() + (d_vDataj.size() - d_vDatai.size()));
							copy(d_vDataj.begin(),d_vDataj.end(),d_vData.begin() + dist);
							if(d_vDataj.size() < d_vDatai.size())
								d_vData.erase(d_vData.begin() + dist + d_vDataj.size(),d_vData.begin() + dist + d_vDatai.size());
							dist += d_vDataj.size();
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
	/* ID */            54,
	/* Name */          _T("%o: Remove every integer %0, size %1 from current board"),
	/* Flags */         0,
	/* Params */        (2,PARAM_NUMBER,_T("Integer value"),PARAM_NUMBER,_T("Size"))
) {
	long p1 = GetInt();
	char p2 = GetInt();

	if(numBoards && !d_bProtected)
	{
		if(p2 == 1)
			d_vData.erase(remove(d_vData.begin(),d_vData.end(),(signed char)p1),d_vData.end());
		else
		{
			const char* value = reinterpret_cast<char*>(&p1);

			long buff = -1,dist = -1;
			while(dist != d_vData.size())
			{
				if(buff != dist)
				{
					d_vData.erase(d_vData.begin() + dist,d_vData.begin() + dist + sizeof(char) * p2);
					buff = dist;
				}
				auto it = search(d_vData.begin() + dist + 1,d_vData.end(),value,value + sizeof(char) * p2);
				dist = it - d_vData.begin();
			}

		}
	}
	return 0;
}

ACTION(
	/* ID */            55,
	/* Name */          _T("%o: Remove every string %0 from current board"),
	/* Flags */         0,
	/* Params */        (1,PARAM_STRING,_T("String"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bProtected)
	{
		long dist = 0;
		while(true)
		{/*
			auto it = search(d_vData.begin() + dist,d_vData.end(),p1,p1 + wcslen(p1));
			dist = it - d_vData.begin();
			if(dist != d_vData.size())
				d_vData.erase(d_vData.begin() + dist,d_vData.begin() + dist + wcslen(p1));
			else
				break;*/
		}
	}
	return 0;
}

ACTION(
	/* ID */            56,
	/* Name */          _T("%o: Remove every content of board %0 from current board"),
	/* Flags */         0,
	/* Params */        (1,PARAM_STRING,_T("Board name"))
) {
	string p1 = string(GetStr());
	if(!p1.length()) return 0;

	if(numBoards && !d_bProtected)
	{
		long dist = 0;
		transform(p1.begin(),p1.end(),p1.begin(),::tolower);
		for(unsigned int i = 0; i < numBoards; i++) //check if board already exists
			if(strCompare(d_sNamei, p1))
			{
				while(true)
				{
					auto it = search(d_vData.begin() + dist,d_vData.end(),d_vDatai.begin(),d_vDatai.end());
					dist = it - d_vData.begin();

					if(dist != d_vData.size())
						d_vData.erase(d_vData.begin() + dist,d_vData.begin() + dist + d_vDatai.size());
					else
						break;
				}
				break;
			}
	}
	return 0;
}

/* ENDIANNESS */

ACTION(
	/* ID */            57,
	/* Name */          _T("%o: Set little-endian"),
	/* Flags */         0,
	/* Params */        (0)
) {
	rdPtr->bEndianness = 0;
	return 0;
}

ACTION(
	/* ID */            58,
	/* Name */          _T("%o: Set big-endian"),
	/* Flags */         0,
	/* Params */        (0)
) {
	rdPtr->bEndianness = 1;
	return 0;
}
