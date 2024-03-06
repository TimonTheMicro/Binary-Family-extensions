
//:-------------------------------
// Condition menu
//:-------------------------------

#ifdef CONDITION_MENU

SEPARATOR
	ITEM(0,"Board exists?")
SEPARATOR

#endif

//:-------------------------------
// Action menu
//:-------------------------------

#ifdef ACTION_MENU

SEPARATOR
SUB_START("Board")
	ITEM(0,"Create")
	ITEM(1,"Select")
	ITEM(2,"Select previous")
	SEPARATOR
	ITEM(3,"Rename")
	ITEM(4,"Rename...")
	ITEM(5,"Lock")
	ITEM(6,"Lock...")
	ITEM(7,"Resize")
	ITEM(8,"Resize...")
	ITEM(9,"Delete")
	ITEM(10,"Delete...")
	SEPARATOR
	ITEM(11,"Clear everything")
SUB_END
SUB_START("Content")
	ITEM(12,"Load from file")
	ITEM(13,"Save to file")
	ITEM(14,"Load PE resource from file")
	SEPARATOR
	SUB_START("Workspace")
		ITEM(15,"Save to file")
		ITEM(16,"Load from file")
	SUB_END
	SEPARATOR
	ITEM(17,"Swap with another")
	ITEM(18,"Copy to another")
	SEPARATOR
	ITEM(19,"Crop at offset")
	ITEM(20,"Remove at offset")
	SEPARATOR
	ITEM(21,"Fill with character")
	SEPARATOR
	SUB_START("Compress with Zlib")
		ITEM(22,"Compress")
		ITEM(23,"Decompress")
	SUB_END
	SUB_START("Encrypt with Blowfish")
		ITEM(24,"Encrypt")
		ITEM(25,"Decrypt")
	SUB_END
	SUB_START("Encode with Base64")
		ITEM(26,"Encode")
		ITEM(27,"Decode")
	SUB_END
SUB_END
SUB_START("Value")
	SUB_START("Set")
		ITEM(28,"bit")
		ITEM(29,"Char")
		ITEM(30,"Short Int")
		ITEM(31,"Long Int")
		ITEM(32,"Float")
		ITEM(33,"String")
		SEPARATOR
		ITEM(34,"Content")
		ITEM(35,"File")
	SUB_END
	SUB_START("Append")
		ITEM(36,"Char")
		ITEM(37,"Short Int")
		ITEM(38,"Long Int")
		ITEM(39,"Float")
		ITEM(40,"String")
		SEPARATOR
		ITEM(41,"Content")
		ITEM(42,"File")
	SUB_END
	SUB_START("Insert")
		ITEM(43,"Char")
		ITEM(44,"Short Int")
		ITEM(45,"Long Int")
		ITEM(46,"Float")
		ITEM(47,"String")
		SEPARATOR
		ITEM(48,"Content")
		ITEM(49,"File")
	SUB_END
	SEPARATOR
	ITEM(50,"Swap")
	ITEM(51,"Flip")
SUB_END
SUB_START("Occurrence")
	SUB_START("Replace every")
		ITEM(52,"Integer by Integer")
		ITEM(53,"String by String")
		ITEM(54,"Content by Content")
	SUB_END
	SUB_START("Remove every")
		ITEM(55,"Integer")
		ITEM(56,"String")
		ITEM(57,"Content")
	SUB_END
SUB_END
SUB_START("Endianness")
	ITEM(58,"Little-endian")
	ITEM(59,"Big-endian")
SUB_END
SEPARATOR

#endif


//:-------------------------------
// Expression menu
//:-------------------------------

#ifdef EXPRESSION_MENU

SEPARATOR
SUB_START("Get board")
	SUB_START("Current")
		ITEM(0,"Name")
		ITEM(1,"Identifier")
		ITEM(2,"Name from identifier")
	SUB_END
	ITEM(3,"Memory location")
	ITEM(4,"Content size")
	ITEM(5,"Lock flag")
	SEPARATOR
	ITEM(6,"Count")
SUB_END
SUB_START("Get value")
	ITEM(7,"size")
	ITEM(8,"sign")
	ITEM(9,"bit")
	SEPARATOR
	ITEM(10,"Char")
	ITEM(11,"Unsigned char")
	ITEM(12,"Short int")
	ITEM(13,"Unsigned short int")
	ITEM(14,"Long int")
	ITEM(15,"Float")
	ITEM(16,"String")
	SEPARATOR
	ITEM(17,"Double")
	ITEM(18,"Long Long Int")
SUB_END
SUB_START("Get file")
	ITEM(23,"Size")
	ITEM(24,"Last used path")
SUB_END
SUB_START("Get Date/Time of")
	ITEM(25,"Year")
	ITEM(26,"Month")
	ITEM(27,"Day")
	ITEM(28,"Hour")
	ITEM(29,"Minute")
	ITEM(30,"Second")
SUB_END
SEPARATOR
SUB_START("Occurences")
	SUB_START("Count")
		ITEM(31,"Integer")
		ITEM(32,"String")
		ITEM(33,"Content")
	SUB_END
	SUB_START("Find")
		ITEM(34,"Integer")
		ITEM(35,"String")
		ITEM(36,"Content")
	SUB_END
SUB_END
SUB_START("Conversions")
	ITEM(37,"Long int to single")
	ITEM(38,"Single to long int")
	ITEM(39,"Short int to half float")
	ITEM(30,"Half float to short int")
	ITEM(41,"Precision adjustment")
	SEPARATOR
	ITEM(19,"LOBYTE")
	ITEM(20,"HIBYTE")
	ITEM(21,"LOWORD")
	ITEM(22,"HIWORD")
SUB_END
SUB_START("Bitwise operations")
	ITEM(42,"Shift left <<")
	ITEM(43,"Shift right >>")
SUB_END
SEPARATOR

#endif
