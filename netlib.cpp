#include "netlib.h"
#include "socket.h"
#include "proxyConnection.h"
#include "eventDispatcher.h"

void Netlib::listenCallback(void* callbackData, MESSAGE msg, HANDLE handle, void* param)
{
    
    if (NETLIB_MSG_CONNECT == msg)
    {
        ProxyConnection* proxyConn = new ProxyConnection();
        proxyConn->onConnect(handle);
    }
    else
    {
        printf("listenCallback receive error message:%d\n", msg);
    }
    
}
int Netlib::listen(std::string serverIp, uint16_t serverPort)
{
    Socket* socket = new Socket();
    if (nullptr == socket)
    {
        return NETLIB_STATE_ERROR;
    }

    int result = socket->listen(serverIp, serverPort, Netlib::listenCallback, nullptr);
    if (NETLIB_STATE_ERROR == result)
    {
        delete socket;
        socket = nullptr;
    }

    return result;
}
int Netlib::connect(std::string serverIp, uint16_t serverPort, callback_t callback, void* calbackData)
{

}
int Netlib::send(FILEDES fd, char* buffer, int len)
{
    Socket* socket = Socket::findSocketByFd(fd);
    if (!socket)
    {
        printf("netlib send: cannot find socket by fd");
        return NETLIB_STATE_ERROR;
    }

    int result = socket->send(buffer, len);
    return result;
}
int Netlib::recv(FILEDES fd, char* buffer, int len)
{
    Socket* socket = Socket::findSocketByFd(fd);
    if (!socket)
    {
        printf("netlib send: cannot find socket by fd");
        return NETLIB_STATE_ERROR;
    }

    int result = socket->recv(buffer, len);
    return result;
}
int Netlib::socketOperationInterface(FILEDES fd, OPERATION operation, void* operationVal)
{
    Socket* socket = Socket::findSocketByFd(fd);
    if (!socket)
    {
        printf("netlib send: cannot find socket by fd");
        return NETLIB_STATE_ERROR;
    }
    switch(operation)
    {
        case NETLIB_OPT_SET_CALLBACK:
            socket->setCallback((callback_t)operationVal);
        break;
        case NETLIB_OPT_SET_CALLBACK_DATA:
            socket->setCallbackData(operationVal);
        break;
        case NETLIB_OPT_GET_REMOTE_IP:
            *(std::string*)operationVal = socket->getRemoteIp();
        break;
        case NETLIB_OPT_GET_REMOTE_PORT:
            *(uint16_t*)operationVal = socket->getRemotePort();
        break;
        default:
        break;
    }
}
void Netlib::startEventLoop(uint32_t waitTimeout)
{
    EventDispatcher::getInstance()->startDispatch(waitTimeout);
}