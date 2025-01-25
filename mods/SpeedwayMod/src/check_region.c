#include <common.h>

int CheckRegion(void)
{
    char* ptr_to_text = 0x80010d28;
    
    if (ptr_to_text[0] == 'T')
    {
        return NTSC;
    }
    else
    {
        return PAL;
    }
}