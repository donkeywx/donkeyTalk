#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <string>
#include <netinet/in.h>
#include <unordered_map>
#include "nonCopyable.h"
#include "osType.h"

class Socket: public NonCopyable
{
public:
    Socket();
    virtual ~Socket();

    int listen(std::string serverIp, uint16_t serverPort, callback_t _callback, void* _callbackData);

    int recv(char* buffer, int len);

    int send(char* buffer, int len);

    int accept();
public:
    void onRead();
    void onWrite();

    static void addSocket2Map(Socket* socket);
    static Socket* findSocketByFd(int socketFd);
public:
    void setSocketFd(FILEDES _socketFd);
    FILEDES getSocketFd();
    void setSocketState(uint8_t _socketState);
    uint8_t getSocketState();
    void setCallback(callback_t _callback);
    void setCallbackData(void* _callbackData);
    void setRemoteIp(std::string _remoteIp);
    std::string getRemoteIp();
    void setRemotePort(uint16_t _remotePort);
    uint16_t getRemotePort();
private:
    void setNonblock(FILEDES _socketFd);
    void setAddress(std::string ip, uint16_t port, sockaddr_in* addr);
    bool isBlock(int errorCode);
private:

    FILEDES socketFd;
    uint8_t socketState;

    std::string localIp;
    uint16_t localPort;

    std::string remoteIp;
    uint32_t remotePort;

    callback_t callback;
    void* callbackData;

    static std::unordered_map<int, Socket*> socketMap;
};

#endif
