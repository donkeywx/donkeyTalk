#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include "socket.h"
#include "eventDispatcher.h"

std::unordered_map<int, Socket*> Socket::socketMap;
Socket::Socket()
{
    socketFd = SOCKET_INVALID;
    socketState = SOCKET_STATE_IDLE;
}

Socket::~Socket()
{

}

int Socket::listen(std::string serverIp, uint16_t serverPort, callback_t _callback, void* _callbackData)
{
    localIp = serverIp;
    localPort = serverPort;
    callback = _callback;
    callbackData = _callbackData;

    socketFd = socket(AF_INET, SOCK_STREAM, 0); // #include <netinet/in.h>
    if (SOCKET_ERROR == socketFd)
    {
        printf("socket() failed, error code:%d\n", errno);
        return NETLIB_STATE_ERROR;
    }

    setNonblock(socketFd);
    sockaddr_in serverAddr;
    setAddress(serverIp, serverPort, &serverAddr);

    int result = ::bind(socketFd, (const sockaddr*)&serverAddr, sizeof(sockaddr_in));
    if (SOCKET_ERROR == result)
    {
        printf("bind() failed, error code:%d\n", errno);
        return NETLIB_STATE_ERROR;
    }

    result = ::listen(socketFd, 64);
    if (SOCKET_ERROR == result)
    {
        printf("listen() failed, error code:%d\n", errno);
        return NETLIB_STATE_ERROR;
    }

    socketState = SOCKET_STATE_LISTENING;

    printf("server listen on %s:%d\n", serverIp.c_str(), serverPort);

    addSocket2Map(this);

    EventDispatcher::getInstance()->addEvent(socketFd, 0);
    return NETLIB_STATR_OK;
}

int Socket::recv(char* buffer, int len)
{
    return ::recv(socketFd, buffer, len, 0);
}

int Socket::send(char* buffer, int len)
{
    if (SOCKET_STATE_CONNECTED != socketState)
    {
        return NETLIB_STATE_ERROR;
    }
    int result = ::send(socketFd, buffer, len, 0);
    if (SOCKET_ERROR == result)
    {
        if (isBlock(errno))
        {
            result = 0;
        }
        else
        {
            printf("send failed, error code:%d\n", errno);
        }
    }
    return result;
}
int Socket::accept()
{
    FILEDES clientFd = 0;
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(sockaddr_in);
    char clientIpString[64];

    while ( SOCKET_INVALID != (clientFd = ::accept(socketFd, (sockaddr*)&clientAddr, &clientAddrLen)) )
    {
        Socket* clientSocket = new Socket();
        uint32_t clientIp = ntohl(clientAddr.sin_addr.s_addr);
        uint16_t clientPort = ntohs(clientAddr.sin_port);

        snprintf(clientIpString, sizeof(clientIpString), "%d.%d.%d.%d",
                clientIp >> 24, (clientIp >> 16) & 0xff,  (clientIp >> 8) & 0xff,  clientIp & 0xff);

        printf("accept a new connection:%s:%d\n", clientIpString, clientPort);
        clientSocket->setSocketFd(clientFd);
        clientSocket->setSocketState(SOCKET_STATE_CONNECTED);
        clientSocket->setCallback(callback);
        clientSocket->setCallbackData(callbackData);
        clientSocket->setRemoteIp(clientIpString);
        clientSocket->setRemotePort(clientPort);

        setNonblock(clientFd);
        
        addSocket2Map(clientSocket);

        EventDispatcher::getInstance()->addEvent(clientFd, 0);

        callback(callbackData, NETLIB_MSG_CONNECT, clientFd, nullptr);
    }
    
}
void Socket::onRead()
{
    if (SOCKET_STATE_LISTENING == socketState)
    {
        accept();
    }
    else
    {
        callback(callbackData, NETLIB_MSG_READ, socketFd, nullptr);
    }
    
}
void Socket::onWrite()
{
    // todo 
    printf("Socket::onWrite() trigger a epoll event\n");
}
bool Socket::isBlock(int errorCode)
{
    return ( (EINPROGRESS == errorCode) || (EWOULDBLOCK == errorCode) );
}
void Socket::addSocket2Map(Socket* socket)
{
    socketMap.insert(std::make_pair(socket->getSocketFd(), socket));
}
Socket* Socket::findSocketByFd(int socketFd)
{
    Socket* socket = nullptr;
    std::unordered_map<int, Socket*>::iterator iter = socketMap.find(socketFd);
    if (socketMap.end() != iter)
    {
        socket = iter->second;
    }

    return socket;
}
void Socket::setNonblock(FILEDES _socketFd)
{
    int result = fcntl(_socketFd, F_SETFL, O_NONBLOCK | fcntl(_socketFd, F_GETFL));

    if (SOCKET_ERROR == result)
    {
        printf("fcntl failed, error code:%d\n", errno);
    }
}
void Socket::setAddress(std::string ip, uint16_t port, sockaddr_in* addr)
{
    memset(addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = inet_addr(ip.c_str());  // arps/inet

    // todo  判断失败的情况
}
void Socket::setSocketFd(FILEDES _socketFd)
{
    socketFd = _socketFd;
}
FILEDES Socket::getSocketFd()
{
    return socketFd;
}
void Socket::setSocketState(uint8_t _socketState)
{
    socketState = _socketState;
}
uint8_t Socket::getSocketState()
{
    return socketState;
}
void Socket::setCallback(callback_t _callback)
{
    callback = _callback;
}
void Socket::setCallbackData(void* _callbackData)
{
    callbackData = _callbackData;
}
void Socket::setRemoteIp(std::string _remoteIp)
{
    remoteIp = _remoteIp;
}
std::string Socket::getRemoteIp()
{
    return remoteIp;
}
void Socket::setRemotePort(uint16_t _remotePort)
{
    remotePort = _remotePort;
}
uint16_t Socket::getRemotePort()
{
    return remotePort;
}