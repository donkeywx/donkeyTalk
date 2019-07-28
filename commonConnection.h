#ifndef __COMMON_CONNECTION_H__
#define __COMMON_CONNECTION_H__

#include <unordered_map>
#include "utils.h"
#include "osType.h"

class CommonConnection: public NonCopyable
{
public:
    CommonConnection();
    virtual ~CommonConnection();

    virtual void onConnect(HANDLE _handle);
    virtual void onConfirm();
    virtual void onRead();
    virtual void onWrite();
    virtual void send(void* data, int len);

    static void commonCallback(void* calllbackData, MESSAGE msg, HANDLE handle, void* param); 

protected:

    HANDLE handle;
    std::string remoteIp;
    uint16_t remotePort;

    SimpleDynamicString inBuffer;
    SimpleDynamicString outBuffer;

protected:
    static const int MAX_RECV_BUFFER_SIZE = 1024;
    static const int MAX_SEND_BUFFER_SIZE = 1024;
};
#endif 