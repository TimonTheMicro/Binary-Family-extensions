
//:-------------------------------
// Condition menu
//:-------------------------------

#ifdef CONDITION_MENU

SEPARATOR
SUB_START("Board")
ITEM(0, "Exists")
ITEM(1, "Protected")
SUB_END
SEPARATOR

#endif

//:-------------------------------
// Action menu
//:-------------------------------

#ifdef ACTION_MENU

SEPARATOR
SUB_START("Board")
ITEM(0, "Create")
ITEM(1, "Select")
ITEM(2, "Rename")
ITEM(3, "Resize")
ITEM(54, "Resize...")
ITEM(4, "Delete")
ITEM(53, "Delete...")
ITEM(57, "Clear Everything")
SEPARATOR
ITEM(5, "Protected mode")
SUB_END
SUB_START("Data")
ITEM(6, "Load from file")
ITEM(7, "Save to file")
SEPARATOR
ITEM(58,"Load PE resource from file")
SEPARATOR
SUB_START("Zlib compression")
ITEM(46, "Compress")
ITEM(47, "Decompress")
SUB_END
SUB_START("Blowfish encryption")
ITEM(48, "Encrypt")
ITEM(49, "Decrypt")
SUB_END
SUB_START("Base64 encoding")
ITEM(50, "Encode")
ITEM(51, "Decode")
SUB_END
SEPARATOR
ITEM(8, "Copy to another")
ITEM(9, "Swap with another")
SEPARATOR
ITEM(10, "Crop at offset")
ITEM(11, "Remove at offset")
SEPARATOR
ITEM(12, "Fill with Char")
SUB_END
SUB_START("Set")
ITEM(52, "bit")
//ITEM(53,"undefined")
ITEM(13, "Char")
ITEM(14, "Short Int")
ITEM(15, "Long Int")
ITEM(16, "Float")
ITEM(17, "String")
SEPARATOR
ITEM(18, "Board")
ITEM(19, "File")
SUB_END
SUB_START("Append")
ITEM(20, "Char")
ITEM(21, "Short Int")
ITEM(22, "Long Int")
ITEM(23, "Float")
ITEM(24, "String")
SEPARATOR
ITEM(25, "Board")
ITEM(26, "File")
SUB_END
SUB_START("Insert")
ITEM(27, "Char")
ITEM(28, "Short Int")
ITEM(29, "Long Int")
ITEM(30, "Float")
ITEM(31, "String")
SEPARATOR
ITEM(32, "Board")
ITEM(33, "File")
SUB_END
SUB_START("Occurrences")
SUB_START("Search and replace")
ITEM(34, "Integer by Integer")
ITEM(35, "String by String")
ITEM(36, "Board by Board")
SUB_END
SUB_START("Search and remove")
ITEM(37, "Integer")
ITEM(38, "String")
ITEM(39, "Board")
SUB_END
SUB_END
SUB_START("Endianness")
ITEM(40, "Little Endian")
ITEM(41, "Big Endian")
SEPARATOR
ITEM(42, "Flip Short Int")
ITEM(43, "Flip Long Int")
SUB_END
SEPARATOR
SUB_START("Workspace")
ITEM(44, "Save to file")
ITEM(45, "Load from file")
SUB_END
SEPARATOR

#endif


//:-------------------------------
// Expression menu
//:-------------------------------

#ifdef EXPRESSION_MENU

SEPARATOR
SUB_START("Board")
SUB_START("Current")
ITEM(0, "Name")
ITEM(1, "Identifier")
SUB_END
ITEM(2, "Memory location")
ITEM(3, "Size")
ITEM(4, "Protected mode")
SEPARATOR
ITEM(5, "Count")
ITEM(6, "Name from identifier")
SUB_END
SUB_START("Get")
ITEM(7, "sign")
ITEM(27, "bit")
//ITEM(28,"undefined")
ITEM(8, "Char")
ITEM(9, "Unsigned Char")
ITEM(10, "Short Int")
ITEM(11, "Unsigned Short Int")
ITEM(12, "Long Int")
ITEM(13, "Floating-Point")
ITEM(14, "String")
SEPARATOR
ITEM(38, "Long Long Int")
ITEM(28, "null-terminated String length")
ITEM(15, "Value size")
ITEM(16, "File size")
SEPARATOR
SUB_START("Binary date")
ITEM(29, "Year")
ITEM(30, "Month")
ITEM(31, "Day")
ITEM(32, "Hour")
ITEM(33, "Minute")
ITEM(34, "Second")
SUB_END
SUB_END
SUB_START("Occurrences")
SUB_START("Count")
ITEM(17, "Integer")
ITEM(18, "String")
ITEM(19, "Board")
SUB_END
SUB_START("Find")
ITEM(20, "Integer")
ITEM(21, "String")
ITEM(22, "Board")
SUB_END
SUB_END
SUB_START("Conversions")
ITEM(23, "Binary to Floating-Point")
ITEM(24, "Floating-Point to Binary")
SEPARATOR
ITEM(25, "Flip Short Int endianness")
ITEM(26, "Flip Long Int endianness")
SEPARATOR
ITEM(35, "Integer to String")
ITEM(36, "Integer to Hexadecimal String")
ITEM(37, "Integer to Binary String")
ITEM(43, "Floating-Point to String")
SEPARATOR
SUB_START("4x4 Matrix")
SUB_START("Rotation To Quaternions")
ITEM(39, "Get rX")
ITEM(40, "Get rY")
ITEM(41, "Get rZ")
ITEM(42, "Get rW")
SUB_END
SUB_START("Math")
ITEM(44, "hyperbolic cosine")
ITEM(45, "hyperbolic sine")
SUB_END
SUB_END
SUB_END
SEPARATOR

#endif
