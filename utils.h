#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include "nonCopyable.h"

class SimpleDynamicString: public NonCopyable
{
public:
    SimpleDynamicString();
    ~SimpleDynamicString();

    uint32_t getMallocSize();
    uint32_t getStringLength();
    uint32_t getWriteOffset();
    void incWritOffeset(uint32_t offset);
    char* getBuffer();
    void makeRoomFor(uint32_t length);

private:
    uint32_t getFreeLen();
    void extend(uint32_t length);
private:
    uint32_t mallocSize;
    uint32_t writeOffeset;
    char*   buffer;
    static const uint32_t  MAX_PRE_MALLOC_SIZE = 1024;
};

#endif