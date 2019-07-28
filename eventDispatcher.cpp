#include <sys/epoll.h>
#include <stdio.h>
#include <errno.h>
#include "eventDispatcher.h"
#include "socket.h"

Mutex* EventDispatcher::mutex = new Mutex();
EventDispatcher* EventDispatcher::eventDispatcher = nullptr;

EventDispatcher::EventDispatcher(uint32_t _eventSize)
    :eventSize(_eventSize)
{
    epollFd = epoll_create(eventSize);
    if (EPOLL_ERROR == epollFd)
    {
        printf("epoll_create failed, error code:%d", errno);
    }
}

EventDispatcher::~EventDispatcher()
{

}

EventDispatcher* EventDispatcher::getInstance()
{
    if (nullptr == eventDispatcher)
    {
        MutexGuard guard(mutex);

        if (nullptr == eventDispatcher)
        {
            eventDispatcher = new EventDispatcher(1024);
        }
    }
    
    return eventDispatcher;
}

void EventDispatcher::addEvent(int fd, uint8_t event)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLPRI | EPOLLHUP;

    if (EPOLL_ERROR == epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev))
    {
        printf("epoll_ctl failed, error code:%d\n", errno);
    }
}

void EventDispatcher::startDispatch(uint32_t waitTimeOut)
{
    struct epoll_event events[1024];
    int eventNum = 0;

    if (running)
    {
        return;
    }
    running = true;

    while(running)
    {
        eventNum = epoll_wait(epollFd, events, eventSize, waitTimeOut);
        for (int i = 0; i < eventNum; i++)
        {
            int eventFd = events[i].data.fd;
            Socket* socket = Socket::findSocketByFd(eventFd);
            if (nullptr == socket)
            {
                continue;
            }

            if  (EPOLLIN & events[i].events)
            {
                socket->onRead();
            }
            if (EPOLLOUT & events[i].events)
            {
                socket->onWrite();
            }
        }
    }
}
