// --------------------------------
// Condition menu
// --------------------------------

#ifdef CONDITION_MENU


#endif


// --------------------------------
// Action menu
// --------------------------------

#ifdef ACTION_MENU


#endif


// --------------------------------
// Expression menu
// --------------------------------

#ifdef EXPRESSION_MENU

	SEPARATOR
	SUB_START("Compute hash of")
		SUB_START("Binary data")
			ITEM(0,"CRC16")
			ITEM(1,"CRC32")
			ITEM(2,"MD2")
			ITEM(3,"MD4")
			ITEM(4,"MD5")
			//ITEM(20,"temp")
			ITEM(5,"SHA1")
			SUB_START("SHA2")
				ITEM(6,"SHA224")
				ITEM(7,"SHA256")
				ITEM(8,"SHA384")
				ITEM(9,"SHA512")
			SUB_END
		SUB_END
		SUB_START("String")
			ITEM(10,"CRC16")
			ITEM(11,"CRC32")
			ITEM(12,"MD2")
			ITEM(13,"MD4")
			ITEM(14,"MD5")
			//ITEM(21,"temp")
			ITEM(15,"SHA1")
			SUB_START("SHA2")
				ITEM(16,"SHA224")
				ITEM(17,"SHA256")
				ITEM(18,"SHA384")
				ITEM(19,"SHA512")
			SUB_END
		SUB_END
	SUB_END
	SEPARATOR

#endif