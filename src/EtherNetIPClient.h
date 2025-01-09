//
// Created by 79933 on 2024/12/5.
//

#ifndef SERVERDEVICE_ETHERNET_ETHERNETIPCLIENT_H
#define SERVERDEVICE_ETHERNET_ETHERNETIPCLIENT_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <future>
#include <thread>
#include <chrono>
#include <utility>
#include <functional>

#include "EtherNetIPTypes.h"

#include "../tools/connection/UDPClass.h"
#include "../tools/tools.h"

#define REFRESH_INTERVAL_MS 1

namespace EtherNetIP {
    struct ItemMessage {
        std::vector<uint8_t> msg;
        SequencedAddressItem item;
        int32_t offset{};
        int32_t msg_length{};
        std::string IP{};
        uint16_t port{};
    };

    struct EncapMessage {
        std::vector<uint8_t> msg;
        EncapsulationPacket packet;
        int32_t offset{};
        int32_t msg_length{};
        std::string IP{};
        uint16_t port{};
    };

    class EtherNetIPClient {
    public:
        explicit EtherNetIPClient(std::string &IPAddress, uint16_t udpPort = 0x8AE) {
            //创建UDP连接
            udpPtr = std::make_shared<UDPChannel>(IPAddress, udpPort);
            udpPtr->createConnection();
        }

        void freeClient() {
            //释放TCP连接
            udpPtr->freeConnection();
        }

        //将functional函数指针依次写入对应容器
        int addReceiveFunction(const std::function<void(std::shared_ptr<ItemMessage>)>& receiveFunction) {
            if (receiveFunction == nullptr)
                return 1;
            receiveItemFunctions.push_back(receiveFunction);
            return 0;
        }

        //Encap部分应当属于TCP，Labview代码中包含此函数，但从未被调用，故标注
//        int addEncapReceiveFunction(std::function<void(std::shared_ptr<EncapMessage>)> receiveFunction){
//            if(receiveFunction == nullptr)
//                return 1;
//            receiveEncapFunctions.push_back(receiveFunction);
//            return 0;
//        }

        //获取UDP套接字
        uint64_t getRawSocket() { return udpPtr->getSocket(); }


        //用于Linux环境发送I/O消息
        int UDPSpendItem(sockaddr_in addr, SequencedAddressItem *sent) {
            std::vector<uint8_t> sentArray = sent->sequenceToByteArray();
            return udpPtr->sendMessage(addr, sentArray);
        }

        //Encap部分应当属于TCP，Labview代码中包含此函数，但从未被调用，故标注
//        int UDPSendPacket(std::string &encapIP, uint16_t encapPort, EncapsulationPacket *sent) {
//            std::vector<uint8_t> sentArray = sent->toByteArray();
//            return udpPtr->sendMessage(encapIP, encapPort, sentArray);
//        }

        //用于开启接收I/O消息接收线程
        void startRcvDaemon() {
            std::thread clientRcv(&EtherNetIPClient::rcvDaemon, this);
            clientRcv.detach();
        }

    private:
        std::shared_ptr<UDPChannel> udpPtr;                                                                 //udp指针
        std::shared_ptr<EncapMessage> encapPtr;                                                             //encap结构体指针，属于TCP发送类型，类内没有使用到
        std::shared_ptr<ItemMessage> itemPtr;                                                               //item结构体指针
        std::vector<std::function<void(std::shared_ptr<ItemMessage>)>> receiveItemFunctions;                //接收I/O消息的函数指针
//        std::vector<std::function<void(std::shared_ptr<EncapMessage>)>> receiveEncapFunctions;            //接收encap包的函数指针，属于TCP发送类型，类内没有使用到

        //接收I/O消息的函数，需要使用线程分离调用
        int rcvDaemon() {
            std::vector<uint8_t> receivedArray;
            std::string clientIP{};
            uint16_t clientPort{};
            int32_t offset = 0;

            while(true){
                offset = 0;
                //使用udp接收I/O消息，如果失败返回1
                if (udpPtr->receiveMessage(receivedArray, clientIP, &clientPort))
                    return 1;
                if (!receivedArray.empty()) {
                    //Encap包似乎是由TCP收发的，Labview代码有如下函数，但没有执行
                    if (EncapsulationPacket::isEncapsulation(receivedArray)) {
                        encapPtr = std::make_shared<EncapMessage>();
                        encapPtr->packet.newEncapsulationPacketByteArray(receivedArray, &offset, static_cast<int32_t>(receivedArray.size()));
                        encapPtr->msg.assign(receivedArray.begin(), receivedArray.end());
                        encapPtr->offset = offset;
                        encapPtr->msg_length = static_cast<int32_t>(receivedArray.size());
                        encapPtr->IP = clientIP;
                        encapPtr->port = clientPort;
//                        for(auto &dvc : receiveEncapFunctions)
//                            dvc(encapPtr);
                    } else {
                        //依次调用functional，接收Item包
                        itemPtr = std::make_shared<ItemMessage>();
                        itemPtr->item.newSequencedAddressItemByArray(receivedArray, &offset, static_cast<int32_t>(receivedArray.size()));
                        itemPtr->msg.assign(receivedArray.begin(), receivedArray.end());
                        itemPtr->offset = offset;
                        itemPtr->msg_length = static_cast<int32_t>(receivedArray.size());
                        itemPtr->IP = clientIP;
                        itemPtr->port = clientPort;

                        for (auto &receive: receiveItemFunctions) {
                            receive(itemPtr);
                        }
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_INTERVAL_MS));
                }
            }
            return 0;
        }
    };
}

#endif //SERVERDEVICE_ETHERNET_ETHERNETIPCLIENT_H
