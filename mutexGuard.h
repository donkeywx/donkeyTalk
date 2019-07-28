#ifndef __MUTEX_GUARD_H__
#define __MUTEX_GUARD_H__

#include "mutex.h"


class MutexGuard: public NonCopyable
{
public:
    MutexGuard(Mutex* _mutex)
    {
        mutex = _mutex;
        mutex->lock();
    }

    ~MutexGuard()
    {
        mutex->unlock();
    }

private:
    Mutex* mutex;
};

#endif