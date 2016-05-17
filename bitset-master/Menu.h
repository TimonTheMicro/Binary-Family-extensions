// --------------------------------
// Condition menu
// --------------------------------

#ifdef CONDITION_MENU


#endif


// --------------------------------
// Action menu
// --------------------------------

#ifdef ACTION_MENU

	SEPARATOR
	SUB_START("Set value at address")
		ITEM(0,"Integer")
		ITEM(1,"String")
	SUB_END
	SEPARATOR

#endif


// --------------------------------
// Expression menu
// --------------------------------

#ifdef EXPRESSION_MENU

	SEPARATOR
	SUB_START("Get")
		ITEM(2,"Sign")
		SEPARATOR
		ITEM(3,"Char")
		ITEM(4,"Unsigned Char")
		ITEM(5,"Short Int")
		ITEM(6,"Unsigned Short Int")
		SEPARATOR
		ITEM(18,"LOBYTE")
		ITEM(19,"HIBYTE")
		ITEM(20,"LOWORD")
		ITEM(21,"HIWORD")
	SUB_END
	SUB_START("Conversions")
		ITEM(8,"Binary to Floating-Point")
		ITEM(15,"Floating-Point to Binary")
		SEPARATOR
		ITEM(7,"Binary to Half Floating-Point")
		ITEM(14,"Half Floating-Point to Binary")
	SUB_END
	SEPARATOR
	SUB_START("Bitwise operations")
		ITEM(0,"Shift right (>>)")
		ITEM(1,"Shift left (<<)")
	SUB_END
	SUB_START("Flags")
		ITEM(9,"Get Flag value")
		SEPARATOR
		ITEM(10,"Set Flag ON")
		ITEM(11,"Set Flag OFF")
		ITEM(12,"Toggle Flag")
		SEPARATOR
		ITEM(13,"Count active Flags")
		ITEM(17,"Count Flags")
		SEPARATOR
		ITEM(16,"Expand bits as pattern")
	SUB_END
	SEPARATOR

#endif