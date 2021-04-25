#include <ultra64.h>
#include <debug.h>

extern u8 _customSegmentNoloadStart[];
extern u8 _customSegmentNoloadEnd[];

typedef u32 uint32_t;

/**
 * xtou32
 * Take a hex string and convert it to a 32bit number (max 8 hex digits).
 * The string must only contain digits and valid hex characters (and optional 0x or 0X prefix)
 * Modified from https://stackoverflow.com/a/57112610
 */
uint32_t xtou32(const char *str, int *err)
{
    uint32_t res = 0;
    char c;
    
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
        str += 2;

    while ((c = *str++)) {
        char v;
        if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
        {
            v = ((c & 0xF) + (c >> 6)) | ((c >> 3) & 0x8);
            res = (res << 4) | (uint32_t) v;
        }
        else
        {
            *err = 1;
            return 0;
        }
    }

    *err = 0;
    return res;
} 

char outputBuf[0x20];

char *readword(void)
{
    int size = debug_sizecommand();
    char addrStr[9];
    int addrParseErr;
    u32 addr;
    if (size == 0 || size > 10)
        return "Bad argument ADDR";
    debug_parsecommand(addrStr);
    addrStr[size] = '\0';

    addr = xtou32(addrStr, &addrParseErr);

    if (addrParseErr)
        return "Invalid hex string ADDR";
    
    if (addr < 0x80000000 || addr > (0x80800000 - 4))
        return "Out out range ADDR";

    if (addr & 0x3)
        return "Not word aligned ADDR";
    
    sprintf(outputBuf, "0x%08X", *(u32*)addr);

    return outputBuf;
}

void custom_init(void)
{
    bzero(_customSegmentNoloadStart, _customSegmentNoloadEnd - _customSegmentNoloadStart);
    debug_initialize();
    debug_printf(
        "Paper Mario Precomp\n"
        "Build Date: " __DATE__ ":" __TIME__ "\n");
    debug_addcommand("read ADDR", "Reads word at given address (hex)", readword);
}