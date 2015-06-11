/**
* LICENSE PLACEHOLDER
*/

#ifndef _UTIL_SLEEP_H
#define _UTIL_SLEEP_H
#ifdef _WIN32

#include <windows.h>
#else
#include <unistd.h>
#endif

void sleep(int milliseconds);

#endif // _UTIL_SLEEP_H
