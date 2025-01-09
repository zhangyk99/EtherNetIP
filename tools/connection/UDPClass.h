//
// Created by 79933 on 2024/11/19.
//

#ifndef TCP_IP_UDPCLASS_H
#define TCP_IP_UDPCLASS_H

#if defined(_WIN32) || defined(_WIN64)
#define TYPE_WINDOWS 1
#elif defined(__linux__)
#define TYPE_WINDOWS 0
#endif

//接收缓冲区大小设置为512
#define BUFFER_SIZE 512
//接收超时时间设置为1ms
#define RECV_TIMEOUT  1

#if TYPE_WINDOWS == 1
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <sstream>
    #include <cstdint>
    #include <cctype>
    #include <string>
    #include <regex>
    #include <thread>
    #include <chrono>

class UDPChannel {
public:
    //传入client IP地址和端口
    explicit UDPChannel(std::string &ip, uint16_t port) : ip(ip), port(port){};

    //建立连接
    int createConnection() {
        WSADATA udpCon;
        int result = WSAStartup(MAKEWORD(2, 2), &udpCon);
        if (result != 0)
            return result;
        //创建套接字
        udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (udpSocket == INVALID_SOCKET) {
            WSACleanup();
            return 1;
        }
        //写入IP地址和端口
        sockaddr_in clientAddr{};
        clientAddr.sin_family = AF_INET;
        clientAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &clientAddr.sin_addr);
        //绑定IP地址和端口
        result = bind(udpSocket, reinterpret_cast<sockaddr *>(&clientAddr), sizeof(clientAddr));
        if (result == SOCKET_ERROR) {
            closesocket(udpSocket);
            WSACleanup();
            return 1;
        }
        //设置接收超时时间
        struct timeval timeout{};
        timeout.tv_sec = RECV_TIMEOUT / 1000;
        timeout.tv_usec = (RECV_TIMEOUT % 1000) * 1000;
        if (setsockopt(udpSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &timeout, sizeof(timeout)) == SOCKET_ERROR) {
            closesocket(udpSocket);
            WSACleanup();
            return 1;
        }
        return result;
    }

    //释放UDP占有资源
    void freeConnection() {
        closesocket(udpSocket);
        WSACleanup();
    }

    int sendMessage (sockaddr_in targetAddr, std::vector<uint8_t> &sendArray){
        //发送I/O消息
        int result = sendto(udpSocket, reinterpret_cast<const char *>(sendArray.data()), static_cast<int>(sendArray.size()), 0, (sockaddr *) &targetAddr, sizeof(targetAddr));
        if (result == SOCKET_ERROR)
            return result;
        return result;
    }

    //接收I/O消息
    int receiveMessage(std::vector<uint8_t> &receivedArray, std::string &clientIP, uint16_t *clientPort) const {
        sockaddr_in clientAddr{};
        //创建接收缓冲区
        char buffer[BUFFER_SIZE];
        int clientAddrLen = sizeof(clientAddr);
        //接收I/O消息
        int receivedBytes = recvfrom(udpSocket, buffer, BUFFER_SIZE, 0, (sockaddr *) &clientAddr, &clientAddrLen);
        //若报错不为超时，则返回1
        if (receivedBytes == SOCKET_ERROR) {
            int error = WSAGetLastError();
            //If no data is received, return 0
            //For other errors, return 1
            if (error == WSAEWOULDBLOCK) {
                receivedArray.clear();
                return 0;
            } else {
                return 1;
            }
        }
        //返回接收消息的IP地址和端口号
        char *cip = inet_ntoa(clientAddr.sin_addr);
        clientIP = cip;
        *clientPort = ntohs(clientAddr.sin_port);
        //返回I/O消息
        receivedArray.assign(buffer, buffer + receivedBytes);
        return 0;
    }

    //获取套接字
    SOCKET getSocket() const { return udpSocket; }

private:
    std::string ip;
    uint16_t port;
    SOCKET udpSocket{};
};
#elif TYPE_WINDOWS == 0
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cstring>
    #include <iostream>
    #include <vector>
    #include <cstdint>

class UDPChannel {
public:
    //传入client IP地址和端口
    UDPChannel(std::string &ip, uint16_t port) : ip(ip), port(port) {}

    //建立UDP连接
    int createConnection() {
        //创建套接字
        udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (udpSocket < 0) {
            return 1;
        }
        //写入IP地址和端口
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
        //绑定IP地址和端口
        if (bind(udpSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
            close(udpSocket);
            return 1;
        }
        //设置接收超时时间
        struct timeval timeout;
        timeout.tv_sec = RECV_TIMEOUT / 1000;
        timeout.tv_usec = (RECV_TIMEOUT % 1000) * 1000;
        if (setsockopt(udpSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            close(udpSocket);
            return 1;
        }
        return 0;
    }

    //释放UDP占有资源
    void freeConnection() {
        close(udpSocket);
    }

    //发送I/O消息
    int sendMessage(sockaddr_in targetAddr, std::vector<uint8_t>& sendArray) {
        //发送I/O消息
        ssize_t result = sendto(udpSocket, sendArray.data(), sendArray.size(), 0,
                                reinterpret_cast<sockaddr*>(&targetAddr), sizeof(targetAddr));
        if (result < 0)
            return 1;
        return static_cast<int>(result);
    }

    //接收I/O消息
    int receiveMessage(std::vector<uint8_t>& receivedArray, std::string &clientIP, uint16_t* clientPort) {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);
        //创建接收缓冲区
        char buffer[BUFFER_SIZE];
        //接收I/O消息
        ssize_t receivedBytes = recvfrom(udpSocket, buffer, BUFFER_SIZE, 0,
                                         reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
        //若报错不为超时，则返回1
        if (receivedBytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                receivedArray.clear();
                return 0;
            } else {
                return 1;
            }
        }
        //返回接收消息的IP地址和端口号
        clientIP = inet_ntoa(clientAddr.sin_addr);
        *clientPort = ntohs(clientAddr.sin_port);
        //返回I/O消息
        receivedArray.assign(buffer, buffer + receivedBytes);
        return 0;
    }

    //获取套接字
    int getSocket() const { return udpSocket; }

private:
    std::string ip;
    uint16_t port;
    int udpSocket; // Replace SOCKET with int
};

#endif

#endif //TCP_IP_UDPCLASS_H
