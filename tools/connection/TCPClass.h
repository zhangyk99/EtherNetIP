//
// Created by 79933 on 2024/11/19.
//

#ifndef TCP_IP_TCPCLASS_H
#define TCP_IP_TCPCLASS_H

#if defined(_WIN32) || defined(_WIN64)
#define TYPE_WINDOWS 1
#elif defined(__linux__)
#define TYPE_WINDOWS 0
#endif

#define RECEIVE_BUFFER_SIZE 512
#define RECEIVE_TIMEOUT 10

#if TYPE_WINDOWS == 1

#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <cstdint>
#include <cctype>
#include <string>
#include <regex>
#include <iostream>
#include <chrono>

class TCPChannel {
public:
    int createSocket() {
        WSADATA wsaData;
        tcpSocket = INVALID_SOCKET;

        // 初始化 Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            return 1;
        }

        // 创建 TCP 套接字
        tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (tcpSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }
        return 0;
    }

    int createConnection(std::string &serverIP, uint16_t serverPort) const {
        std::string command = "ping -n 1 " + serverIP;

        STARTUPINFO si = { sizeof(STARTUPINFO) };
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        PROCESS_INFORMATION pi;

        if (!CreateProcess(
                NULL,
                const_cast<char*>(command.c_str()),
                NULL, NULL, FALSE,
                CREATE_NO_WINDOW, // 无窗口模式
                NULL, NULL,
                &si, &pi)) {
            std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
            return -1;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD result;
        GetExitCodeProcess(pi.hProcess, &result);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if(result){
            closesocket(tcpSocket);
            WSACleanup();
            return 1;
        }

        if (tcpSocket == INVALID_SOCKET)
            return 1;
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());

        if (connect(tcpSocket, (sockaddr *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(tcpSocket);
            WSACleanup();
            return 1;
        }

        return 0;
    }

    int freeConnect() {
        if (tcpSocket == INVALID_SOCKET)
            return 1;
        closesocket(tcpSocket);
        WSACleanup();
        return 0;
    }

    int sendAndReceive(std::vector<uint8_t> &sentArray, int32_t timeout, std::vector<uint8_t> &receivedArray) const {
        int sendTimeout = timeout;
        setsockopt(tcpSocket, SOL_SOCKET, SO_SNDTIMEO, (const char *) &sendTimeout, sizeof(sendTimeout));
        int bytesSent = send(tcpSocket, reinterpret_cast<const char *>(sentArray.data()), static_cast<int>(sentArray.size()), 0);
        if (bytesSent == SOCKET_ERROR)
            return WSAGetLastError();

        char recvBuffer[RECEIVE_BUFFER_SIZE] = {0};
        int totalBytesReceived = 0;
        int recvTimeout = RECEIVE_TIMEOUT;
        if (setsockopt(tcpSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &recvTimeout, sizeof(recvTimeout)) == SOCKET_ERROR)
            std::cout << "setSockopt error" << std::endl;

        int bytesReceived = recv(tcpSocket, recvBuffer, sizeof(recvBuffer), 0);

        if (bytesReceived > 0) {
            totalBytesReceived += bytesReceived;
        } else if (bytesReceived == 0) {
            return 1;
        } else {
            int error = WSAGetLastError();
            if (error != WSAETIMEDOUT)
                return error;
        }

        receivedArray.resize(totalBytesReceived);
        std::copy(recvBuffer, recvBuffer + totalBytesReceived, receivedArray.begin());

        return 0;
    }

private:
    SOCKET tcpSocket;
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
#include <chrono>
#include <thread>

class TCPChannel {
public:
    int createSocket() {
        tcpSocket = -1;
        sockaddr_in serverAddr{};

        tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (tcpSocket < 0) {
            std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
            return 1;
        }
        return 0;
    }

    int createConnection(std::string &clientIP, uint16_t clientPort) {
        if (tcpSocket < 0)
            return 1;

        sockaddr_in clientAddr{};
        clientAddr.sin_family = AF_INET;
        clientAddr.sin_port = htons(clientPort);
        if (inet_pton(AF_INET, clientIP.c_str(), &clientAddr.sin_addr) <= 0) {
            std::cerr << "Invalid IP address format" << std::endl;
            return 1;
        }

        if (connect(tcpSocket, reinterpret_cast<sockaddr *>(&clientAddr), sizeof(clientAddr)) < 0) {
            std::cerr << "Connect failed: " << strerror(errno) << std::endl;
            close(tcpSocket);
            return errno;
        }

        return 0;
    }

    int freeConnect() {
        if (tcpSocket < 0)
            return 1;

        close(tcpSocket);
        return 0;
    }

    int sendAndReceive(std::vector<uint8_t> &sentArray, int32_t timeout, std::vector<uint8_t> &receivedArray) {
        // 设置发送超时
        int sendBuffer = sentArray.size();
        struct timeval sendTimeout;
        sendTimeout.tv_sec = timeout / 1000;
        sendTimeout.tv_usec = (timeout % 1000) * 1000;
        setsockopt(tcpSocket, SOL_SOCKET, SO_SNDTIMEO, &sendTimeout, sizeof(sendTimeout));

        // 发送数据
        ssize_t bytesSent = send(tcpSocket, sentArray.data(), sentArray.size(), 0);
        if (bytesSent < 0) {
            std::cerr << "Send failed: " << strerror(errno) << std::endl;
            return errno;
        }

        char recvBuffer[RECEIVE_BUFFER_SIZE] = {0};
        int bytesToRead = RECEIVE_BUFFER_SIZE;
        int totalBytesReceived = 0;
        struct timeval recvTimeout;
        recvTimeout.tv_sec = RECEIVE_TIMEOUT / 1000;
        recvTimeout.tv_usec = (RECEIVE_TIMEOUT % 1000) * 1000;
        setsockopt(tcpSocket, SOL_SOCKET, SO_RCVTIMEO, &recvTimeout, sizeof(recvTimeout));

        ssize_t bytesReceived = recv(tcpSocket, recvBuffer + totalBytesReceived, bytesToRead - totalBytesReceived, 0);

        if (bytesReceived > 0) {
            totalBytesReceived += static_cast<int>(bytesReceived);
        } else if (bytesReceived == 0) {
            std::cerr << "Connection closed by peer" << std::endl;
            return 1;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cerr << "Receive timeout" << std::endl;
            } else {
                std::cerr << "Receive error: " << strerror(errno) << std::endl;
            }
            return errno;
        }

        receivedArray.resize(totalBytesReceived);
        std::copy(recvBuffer, recvBuffer + totalBytesReceived, receivedArray.begin());

        return 0;
    }

private:
    int tcpSocket;
};

#endif

#endif //TCP_IP_TCPCLASS_H
