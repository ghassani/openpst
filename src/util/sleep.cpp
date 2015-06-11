/**
* LICENSE PLACEHOLDER
*/

#include "sleep.h"

void sleep(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}
