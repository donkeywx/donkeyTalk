#ifndef __NETLIB_H__
#define __NETLIB_H__


#include <stdint.h>
#include <string>
#include "osType.h"
#include "utils.h"
#include "nonCopyable.h"

class Netlib: public NonCopyable
{
public:

    Netlib();
    virtual ~Netlib();

    static void listenCallback(void* callbackData, MESSAGE msg, HANDLE handle, void* param);
    static int listen(std::string serverIp, uint16_t serverPort);
    static int connect(std::string serverIp, uint16_t serverPort, callback_t callback, void* calbackData);
    static int send(FILEDES fd, char* buffer, int len);
    static int recv(FILEDES fd, char* buffer, int len);

    static int socketOperationInterface(FILEDES _handle, OPERATION operation, void* operationVal);
    static void startEventLoop(uint32_t waitTimeout);
};

#endif
