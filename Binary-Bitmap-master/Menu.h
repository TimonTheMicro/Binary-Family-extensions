#define ITEM_MULTIPLEIMG	ITEM(-1,"(Enable multiple images)")

//:-------------------------------
// Condition menu
//:-------------------------------

#ifdef CONDITION_MENU

#endif

//:-------------------------------
// Action menu
//:-------------------------------

#ifdef ACTION_MENU

	SUB_START("Display settings")
		ITEM(1,"Force redraw")
		ITEM(2,"Skip redraw")
		SEPARATOR
		ITEM(3,"Set transparent")
		SEPARATOR
		ITEM(4,"Set scale")
		ITEM(5,"Set X scale")
		ITEM(6,"Set Y scale")
		ITEM(29,"Set angle")
	SUB_END
	SUB_START("Settings")
		ITEM(7,"Set transparent color")
		SEPARATOR
		ITEM(8,"Set hot spot")
		ITEM(9,"Set hot spot (percent)")
	SUB_END
	SUB_START("Input / Output")
		ITEM(10,"Load image from file")
		ITEM(11,"Save image to file")
		SEPARATOR
		ITEM(12,"Load image from clipboard")
		ITEM(13,"Copy image to clipboard")
		SEPARATOR
		ITEM(16,"Load raw pixels from memory location")
		ITEM(28,"Load bitmap image from memory location")
	SUB_END
	SUB_START("Alpha channel")
		ITEM(17,"Create")
		ITEM(18,"Remove")
		SEPARATOR
		ITEM(19,"Clear")
		ITEM(20,"Set pixel")
	SUB_END
	SUB_START("Transformations")
		ITEM(21,"Resize image")
		ITEM(22,"Resize canvas")
		SEPARATOR
		ITEM(23,"Flip image horizontally")
		ITEM(24,"Flip image vertically")
	SUB_END
	SUB_START("Draw with color")
		ITEM(25,"Clear")
		ITEM(26,"Set pixel")
		ITEM(27,"Flood fill")		
	SUB_END
#endif


//:-------------------------------
// Expression menu
//:-------------------------------

#ifdef EXPRESSION_MENU
	
	SUB_START("Image")
		ITEM(0,"Width")
		ITEM(1,"Height")
	SUB_END
	SUB_START("Scale / Angle")
		ITEM(2,"X scale")
		ITEM(3,"Y scale")
		ITEM(4,"Angle")
	SUB_END

#endif