#ifndef __PROXY_CONNECTION_H__
#define __PROXY_CONNECTION_H__

#include <unordered_map>
#include "commonConnection.h"

class ProxyConnection: public CommonConnection
{
public:
    ProxyConnection();
    ~ProxyConnection();

public:
    virtual void onConnect(HANDLE _handle);
    virtual void onRead();

private:
    static std::unordered_map<HANDLE, ProxyConnection*> proxyConnMap;
};

#endif 