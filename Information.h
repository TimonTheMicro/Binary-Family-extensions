// --------------------
// Object information
// --------------------

// Basic object details
#define	ObjectName			"Binary Board object"
#define	ObjectAuthor		"Timon"
#define	ObjectCopyright		"Copyright © 2024"
#define	ObjectComment		"Load, save and manage binary data (Unicode Edition)"
#define	ObjectURL			""
#define	ObjectHelp			_T("Help\\BinaryBoard.chm")

// If you register your object with Clickteam, change this to the ID you were given
#define ObjectRegID			REGID_PRIVATE

// Change N,O,N,E to 4 unique characters (MMF currently still uses this to keep track)
#define	IDENTIFIER			MAKEID(B,I,N,B)

#define	ObjectVersion		1

// --------------------
// Version information
// --------------------

// PRODUCT_VERSION_TGF or PRODUCT_VERSION_DEVELOPER
#define ForVersion			PRODUCT_VERSION_STANDARD

// Set this to the latest MMF build out when you build the object
#define	MinimumBuild		257

// --------------------
// Beta information
// --------------------

// #define BETA
// #define POPUP_ON_DROP
// #define POPUP_ON_EXE
// #define POPUP_ON_BUILD
// #define POPUP_MESSAGE	"This is a beta extension- use with caution!"

// --------------------
// Handling priority
// --------------------
// If this extension will handle windows messages, specify the priority
// of the handling procedure, 0 = low and 255 = very high

#define	WINDOWPROC_PRIORITY 100
