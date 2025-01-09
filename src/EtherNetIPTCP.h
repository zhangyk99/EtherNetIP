//
// Created by 79933 on 2024/12/5.
//

#ifndef SERVERDEVICE_ETHERNET_ETHERNETIPTCP_H
#define SERVERDEVICE_ETHERNET_ETHERNETIPTCP_H

#include <cstdint>
#include <memory>

#include "EtherNetIPTypes.h"

#include "../tools/connection/TCPClass.h"

namespace EtherNetIP{

    class EtherNetIPTCP{
    public:
        explicit EtherNetIPTCP(int32_t timeout) : timeout(timeout){
            tcpPtr = std::make_shared<TCPChannel>();
            errorCode = tcpPtr->createSocket();
            if(!errorCode)
                isCreate = true;
        }

        ~EtherNetIPTCP(){
            tcpPtr->freeConnect();
        }

        bool is_connected() const{ return isConnected; }

        int connect(std::string &ip, uint16_t port){
            if(isConnected)
                return 1;
            if(!isCreate) {
                isCreate = true;
                tcpPtr->createSocket();
            }
            if(tcpPtr->createConnection(ip, port)){
                return 1;
            }
            isConnected = true;
            return 0;
        }

        int disconnect(){
            isCreate = false;
            isConnected = false;
            return tcpPtr->freeConnect();
        }

        int sendRcv(EncapsulationPacket *sent, int32_t *offset, EncapsulationPacket *received, int32_t *length, std::vector<uint8_t> &packet)  {
            if(tcpPtr == nullptr)
                return 1;
            std::vector<uint8_t> sentArray = sent->toByteArray();
            std::vector<uint8_t> receivedArray;

            int result = tcpPtr->sendAndReceive(sentArray, timeout, receivedArray);

            *length = static_cast<int32_t>(receivedArray.size());
            packet.assign(receivedArray.begin(), receivedArray.end());

            if(!receivedArray.empty())
                received->newEncapsulationPacketByteArray(receivedArray, offset, *length);
            return result;
        }

    private:
        int32_t timeout;
        std::shared_ptr<TCPChannel> tcpPtr;
        int32_t errorCode;
        bool isCreate;
        bool isConnected = false;
    };
}

#endif //SERVERDEVICE_ETHERNET_ETHERNETIPTCP_H
