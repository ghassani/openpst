#ifndef _HELPER_SAHARA_HELPER_H
#define _HELPER_SAHARA_HELPER_H

#include "definitions.h"

namespace SaharaHelper
{
    const char* getNamedMode(uint32_t mode);
    const char* getNamedClientCommand(uint32_t command);
    const char* getNamedErrorStatus(uint32_t status);
    const char* getNamedRequestedImage(uint32_t imageId);
    const char* getNamedRequestedImageAlt(uint32_t imageId);
}

#endif // _HELPER_SAHARA_HELPER_H
