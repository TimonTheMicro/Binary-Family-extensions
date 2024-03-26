
//:-------------------------------
// Condition menu
//:-------------------------------

#ifdef CONDITION_MENU

SEPARATOR
SUB_START("Board")
	ITEM(0,"Exists")
	ITEM(1,"On create")
SUB_END
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
		ITEM(60,"Space")
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
		ITEM(52,"Previous Name")
		ITEM(1,"Identifier")
		ITEM(2,"Name from identifier")
	SUB_END
	ITEM(3,"Memory location")
	ITEM(4,"Content dimensions")
	ITEM(5,"Lock flag")
	SEPARATOR
	ITEM(6,"Count")
SUB_END
SUB_START("Get value")
	ITEM(7,"Size")
	ITEM(8,"Sign")
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
	ITEM(18,"Long long int")
SUB_END
SUB_START("Get file")
	ITEM(19,"Size")
	ITEM(20,"Last used path")
SUB_END
SUB_START("Get Date/Time of")
	ITEM(21,"Year")
	ITEM(22,"Month")
	ITEM(23,"Day")
	ITEM(24,"Hour")
	ITEM(25,"Minute")
	ITEM(26,"Second")
SUB_END
SEPARATOR
SUB_START("Occurences")
	SUB_START("Count")
		ITEM(27,"Integer")
		ITEM(28,"String")
		ITEM(29,"Content")
	SUB_END
	SUB_START("Find")
		ITEM(30,"Integer")
		ITEM(31,"String")
		ITEM(32,"Content")
	SUB_END
SUB_END
SUB_START("Conversions")
	ITEM(33,"Long int to float")
	ITEM(34,"Float to long int")
	ITEM(35,"Read half float")
	ITEM(36,"Store half float")
	ITEM(37,"Precision adjustment")
	SEPARATOR
	ITEM(38,"LOBYTE")
	ITEM(39,"HIBYTE")
	ITEM(40,"LOWORD")
	ITEM(41,"HIWORD")
	SEPARATOR
	ITEM(42,"ASCII character to char")
	ITEM(43,"Char to ASCII character")
SUB_END
SUB_START("Bitwise operations")
	ITEM(44,"Shift left")
	ITEM(45,"Shift right")
	SEPARATOR
	SUB_START("On integer value")
		ITEM(46,"Inflate bits")
		ITEM(47,"Crop bits")
		SEPARATOR
		ITEM(48,"Get bit")
		ITEM(49,"Set bit")
		ITEM(50,"Unset bit")
		ITEM(51,"Toggle bit")
	SUB_END
SUB_END
SEPARATOR

#endif
