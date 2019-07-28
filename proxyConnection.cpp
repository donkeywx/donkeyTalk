#include "proxyConnection.h"
#include "netlib.h"

std::unordered_map<HANDLE, ProxyConnection*> ProxyConnection::proxyConnMap;
ProxyConnection::ProxyConnection()
{

}
ProxyConnection::~ProxyConnection()
{

}

void ProxyConnection::onConnect(HANDLE _handle)
{
    handle = _handle;
    proxyConnMap.insert(std::make_pair(handle, this));

    Netlib::socketOperationInterface(handle, NETLIB_OPT_SET_CALLBACK, (void*)CommonConnection::commonCallback);
    Netlib::socketOperationInterface(handle, NETLIB_OPT_SET_CALLBACK_DATA, (void*)&proxyConnMap);
    Netlib::socketOperationInterface(handle, NETLIB_OPT_GET_REMOTE_IP, &remoteIp);
    Netlib::socketOperationInterface(handle, NETLIB_OPT_GET_REMOTE_PORT, &remotePort);
}
void ProxyConnection::onRead()
{
    for (;;)
    {
        inBuffer.makeRoomFor(MAX_RECV_BUFFER_SIZE);

        int result = Netlib::recv(handle, inBuffer.getBuffer() + inBuffer.getWriteOffset(), MAX_RECV_BUFFER_SIZE);
        if (result < 0)
        {
            break;
        }

        inBuffer.incWritOffeset(result);
    }

    printf("receive a message:%s", inBuffer.getBuffer());

    send(inBuffer.getBuffer(), inBuffer.getStringLength());
}