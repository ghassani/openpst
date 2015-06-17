
#ifndef _INCLUDE_DEFINITIONS_H
#define _INCLUDE_DEFINITIONS_H


#if defined(_WIN32) && !defined(__MINGW32__)
	#include "win_stdint.h"
	#include "win_inttypes.h"

	#define PACKED( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )

#else
	#include <stdint.h>
	#define PACKED( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif


#endif //_INCLUDE_DEFINITIONS_H
