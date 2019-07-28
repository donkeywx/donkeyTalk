#ifndef __EVENT_DISPATCHER_H__
#define __EVETN_DISPATCHER_H__

#include <stdint.h>
#include "mutexGuard.h"
#include "osType.h"


class EventDispatcher : public NonCopyable
{
public:
    EventDispatcher(uint32_t _eventSize);
    
    virtual ~EventDispatcher();

    static EventDispatcher* getInstance();

    void addEvent(int fd, uint8_t event);

    void startDispatch(uint32_t waitTimeOut);
private:

    static EventDispatcher* eventDispatcher;

    static Mutex* mutex;

    int eventSize;

    FILEDES epollFd;

    bool running;
};

#endif
