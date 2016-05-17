// --------------------
// Object information
// --------------------

// Basic object details
#define	ObjectName			"Checksum object"
#define	ObjectAuthor		"Timon"
#define	ObjectCopyright		"Copyright � 2016"
#define	ObjectComment		"Allows user to generate CRC16, CRC32, MD2, MD4, MD5, SHA1, SHA2 and HMAC hash of string or data at memory address"
#define	ObjectURL			""
#define	ObjectHelp			"Help\\Checksum.chm"

// If you register your object with Clickteam, change this to the ID you were given
#define ObjectRegID			REGID_PRIVATE

// Change N,O,N,E to 4 unique characters (MMF currently still uses this to keep track)
#define	IDENTIFIER			MAKEID(C,S,U,M)

// --------------------
// Version information
// --------------------

// PRODUCT_VERSION_TGF or PRODUCT_VERSION_DEVELOPER
#define ForVersion			PRODUCT_VERSION_STANDARD

// Set this to the latest MMF build out when you build the object
#define	MinimumBuild		251

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
