#include "commonConnection.h"
#include "netlib.h"

CommonConnection::CommonConnection()
{

}
CommonConnection::~CommonConnection()
{

}
void CommonConnection::onConnect(HANDLE _hanlde)
{

}
void CommonConnection::onConfirm()
{

}
void CommonConnection::onRead()
{

}
void CommonConnection::onWrite()
{

}
void CommonConnection::send(void* data, int len)
{
    int offset = 0;
    int remain = len;

    while (remain > 0)
    {
        int sendSize = remain;
        if (sendSize > MAX_SEND_BUFFER_SIZE)
        {
            sendSize = MAX_SEND_BUFFER_SIZE;
        }

        int result = Netlib::send(handle, (char*)data+offset, sendSize);
        if (result <= 0)
        {
            result = 0;
            break;
        }

        offset += result;
        remain -= result;
    }
    printf("send a massage:%s", (char*)data);
    if (remain > 0)
    {
        // todo
    }   
}

void CommonConnection::commonCallback(void* callbackData, MESSAGE msg, HANDLE _handle, void* param)
{
    if (nullptr == callbackData)
    {
        return;
    }
    std::unordered_map<HANDLE, CommonConnection*>* connMap = 
        (std::unordered_map<HANDLE, CommonConnection*>*) callbackData;
    std::unordered_map<HANDLE, CommonConnection*>::iterator iter = (*connMap).find(_handle);

    if ( (*connMap).end() == iter )
    {
        return;
    }

    CommonConnection* conn = iter->second;
    if (nullptr == conn)
    {
        return;
    }

    switch(msg)
    {
        case NETLIB_MSG_CONFIRM:
            conn->onConfirm();
        break;
        case NETLIB_MSG_READ:
            conn->onRead();
        break;
        case NETLIB_MSG_WRITE:
            conn->onWrite();
        break;
        default:
            printf("CommonConnection::commonCallback error msg:%d\n", msg);
        break;
    }
}