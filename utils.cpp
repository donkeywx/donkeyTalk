#include <stdlib.h>
#include "utils.h"

SimpleDynamicString::SimpleDynamicString()
    : mallocSize(0), writeOffeset(0),
        buffer(nullptr)
{

}
SimpleDynamicString::~SimpleDynamicString()
{

}
uint32_t SimpleDynamicString::getMallocSize()
{
    return mallocSize;
}
uint32_t SimpleDynamicString::getWriteOffset()
{
    return writeOffeset;
}
void SimpleDynamicString::incWritOffeset(uint32_t offset)
{
    writeOffeset += offset;
}
uint32_t SimpleDynamicString::getStringLength()
{
    return writeOffeset;
}
char* SimpleDynamicString::getBuffer()
{
    return buffer;
}
void SimpleDynamicString::makeRoomFor(uint32_t length)
{
    uint32_t freeLen = getFreeLen();
    if (freeLen >= length)
    {
        return;
    }
    extend(length);
}
uint32_t SimpleDynamicString::getFreeLen()
{
    return mallocSize - writeOffeset;
}
void SimpleDynamicString::extend(uint32_t length)
{
    mallocSize += length;
    if (mallocSize < MAX_PRE_MALLOC_SIZE)
    {
        mallocSize *= 2;
    }
    else
    {
        mallocSize += MAX_PRE_MALLOC_SIZE;
    }

    char* newBuffer = (char*)realloc(buffer, mallocSize);
    buffer = newBuffer;
}