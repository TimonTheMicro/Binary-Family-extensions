# Binary-Family-extensions

To create or edit binary data.

Binary Family contains:

Binary Board object:
The main object. Powerfull binary manipulator, which allows user to edit files and data on memory,
compress it (zlib) and protect (blowfish).
It's somewhat like refreshed old and known extension called Binary object.


Binary Bitmap object:
Based on Surface Object, (not as powerfull as Surface), allows to load almost all types of uncompressed
bitmaps. From 32 bit to 1 bit. Works well with Binary Board object.


Bitset object:
Bits (or flags) manipulator, allows user to treat every integer value as a set of 32 flags.
The pattern bits option allows to expand value, for eg. bits 11111 are expanded to 11111111 or 10101 to 10101101 patterns.


Checksum object:
Hash manager, allows to calculate hash of data on memory or string of
MD2, MD4, MD5, CRC16, CRC32, SHA1, SHA2 checksums with additional HMAC key.
