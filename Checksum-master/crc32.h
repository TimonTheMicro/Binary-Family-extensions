/* This is the basic CRC algorithm with no optimizations. It follows the
logic circuit as closely as possible. */


void make_crc_table(unsigned long crcTable[])
{
    unsigned long POLYNOMIAL = 0xEDB88320;
    unsigned long remainder;
    unsigned char b = 0;
    do
	{
        // Start with the data byte
        remainder = b;
        for (unsigned long bit = 8; bit > 0; --bit)
        {
            if (remainder & 1)
                remainder = (remainder >> 1) ^ POLYNOMIAL;
            else
                remainder = (remainder >> 1);
        }
        crcTable[(size_t)b] = remainder;
    } while(0 != ++b);
}

unsigned long crc32(unsigned char *p, size_t n, unsigned long crcTable[])
{
unsigned long crc = 0xfffffffful;
size_t i;
    for(i = 0; i < n; i++)
        crc = crcTable[*p++ ^ (crc&0xff)] ^ (crc>>8);
    return(~crc);
}
