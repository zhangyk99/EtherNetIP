//
// Created by 79933 on 2024/12/5.
//

#ifndef SERVERDEVICE_ETHERNET_ETHERNETIPREMOTEDEVICE_H
#define SERVERDEVICE_ETHERNET_ETHERNETIPREMOTEDEVICE_H

#include <mutex>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <functional>

#include "EtherNetIPTCP.h"
#include "EtherNetIPClient.h"
#include "EtherNetIPTypes.h"

#include "mtestserver_error_code.h"
#include "../tools/watchdog/mtestserver_watchdog.h"
#include "../tools/serverlogger/logger.h"
#include "../tools/byte_decoder/byte_decoder.h"
#include "../tools/tools.h"

namespace EtherNetIP {
    class EtherNetIPRemoteDevice {
    public:

        explicit EtherNetIPRemoteDevice(void *watchdog, std::shared_ptr<EtherNetIPClient> &clientPtr, std::string &IPAddress, uint16_t tcpPort = 0xAF12, uint16_t udpPort = 0x8AE,
                                        int32_t tcpTimeout = 100) :
                watchdog(watchdog), clientPtr(clientPtr), IPAddress(IPAddress), tcpPort(tcpPort) {
            tcpPtr = std::make_shared<EtherNetIPTCP>(tcpTimeout);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(udpPort);
            addr.sin_addr.s_addr = inet_addr(IPAddress.c_str());
        }

        //释放remoteDevice占用资源
        void freeRemoteDevice() {
            forwardClose();
            wd_remove(watchdog);
#if TYPE_WINDOWS == 1
            stopSendO2T();
#endif
            free_signal_decoder(rDecO2T);
            free_signal_decoder(rDecT2O);
        }

        int
        setClassInstanceAttributeData(uint32_t cip, std::vector<uint8_t> &dataPath, std::vector<uint8_t> &data, EIPNetworkStatus *status, std::vector<uint8_t> &pkt, int32_t *offset, int32_t *length) {
            return sendUCMM_RR_Packet(cip, dataPath, data, status, pkt, offset, length);
        }

        int getClassInstanceAttributeData(uint32_t cip, std::vector<uint8_t> &classDataPath, EIPNetworkStatus *status, std::vector<uint8_t> &pkt, int32_t *offset, int32_t *length) {
            std::vector<uint8_t> data{};
            return sendUCMM_RR_Packet(cip, classDataPath, data, status, pkt, offset, length);
        }

        int setAttributeData(std::vector<uint8_t> &data, CIPObject *obj, EIPNetworkStatus *status) {
            std::vector<uint8_t> dataPath = EIPPath::getPath(obj->classID, obj->instanceID, obj->attributeID);
            std::vector<uint8_t> pkt;
            int32_t offset = 0;
            int32_t length;
            return setClassInstanceAttributeData(static_cast<uint32_t>(CIPServiceCodes::SetAttributeSingle), dataPath, data, status, pkt, &offset, &length);
        }

        //用于配置不同种类的信号
        int getAttributeData(CIPObject *obj, std::vector<uint8_t> &rawData, EIPNetworkStatus *status = {}) {
            std::vector<uint8_t> classDataPath = EIPPath::getPath(obj->classID, obj->instanceID, obj->attributeID);
            std::vector<uint8_t> pkt;
            int32_t offset = 0;
            int32_t length;
            int result = getClassInstanceAttributeData(static_cast<uint32_t>(CIPServiceCodes::GetAttributeSingle), classDataPath, status, pkt, &offset, &length);
            if (*status == EIPNetworkStatus::OnLine) {
                rawData.assign(pkt.begin() + offset, pkt.begin() + length);
            } else {
                rawData.clear();
            }
            return result;
        }

        //读取会话句柄
        uint32_t readSessionHandle() const { return sessionHandle; }

        //写入会话句柄
        void writeSessionHandle(uint32_t sHandle) { sessionHandle = sHandle; }

        //使用TCP发送显示消息，注册会话
        uint32_t registerSession() {
            if (autoConnect)
                connect();
            if (isConnected() && (readSessionHandle() == 0)) {
                EncapsulationPacket packet;
                std::vector<uint8_t> array{1, 0, 0, 0};
                packet.newEncapsulationPacket(static_cast<uint16_t>(EncapsulationCommands::RegisterSession), 0, array);
                int32_t offset = 0;
                EncapsulationPacket received;
                int32_t length;
                std::vector<uint8_t> pkt;
                tcpPtr->sendRcv(&packet, &offset, &received, &length, pkt);
                if ((length == 28) && (received.isOk())) {
                    writeSessionHandle(received.sessionHandle);
                    return received.sessionHandle;
                }
            }
            return readSessionHandle();
        }

        //检查TCP是否连接
        bool isConnected() { return tcpPtr->is_connected(); }

        //建立TCP连接
        int connect() {
            if (tcpPtr->connect(IPAddress, tcpPort))
                return 1;
            writeSessionHandle(0);
            return 0;
        }

        //断开TCP连接
        int disconnect() {
            writeSessionHandle(0);
            return tcpPtr->disconnect();
        }

        //调用UDP发送I/O消息
        int class1SendO2T(SequencedAddressItem *item) {
            return clientPtr->UDPSpendItem(addr, item);
        }

        //关闭forwardOpen
        EIPNetworkStatus forwardClose() {
            if (forwardRunning) {
                std::vector<uint8_t> dataPath = EIPPath::getPath(6, 1, 0);
                std::vector<uint8_t> data = forwardPacket->forwardClosePacketToByteArray();
                EIPNetworkStatus status;
                std::vector<uint8_t> pkt;
                int32_t offset = 0;
                int32_t length;
                sendUCMM_RR_Packet(static_cast<uint32_t>(CIPServiceCodes::ForwardClose), dataPath, data, &status, pkt, &offset, &length);
                forwardRunning = false;
                return status;
            }
            return EIPNetworkStatus::OnLine;
        }

        void forwardOpen(uint8_t timeoutTicks, std::vector<uint8_t> &configRawData, CIPObject *config, CIPObject *O2T, CIPObject *T2O, ForwardOpenConfig *forwardOpenConfig,
                         EIPNetworkStatus *status = {}, uint32_t *T2OID = {}, uint32_t *O2TID = {}) {
            std::vector<uint8_t> path = getForwardOpenPath(config, O2T, T2O);
            if (configRawData.empty() && EIPCIPObjectIsNull(*config))
                EIPPath::addDataSegment(path, configRawData);
            forwardPacket = std::make_shared<ForwardPacket>();
            forwardPacket->newForwardPacket(timeoutTicks, path, *forwardOpenConfig, 0);
            std::vector<uint8_t> pkt;
            int32_t offset = 0;
            int32_t length;
            std::vector<uint8_t> data_path = EIPPath::getPath(6, 1, 0);
            std::vector<uint8_t> data = forwardPacket->forwardOpenPacketToByteArray();
            sendUCMM_RR_Packet(forwardPacket->isLargeForwardOpen ? static_cast<uint32_t>(CIPServiceCodes::LargeForwardOpen) : static_cast<uint32_t>(CIPServiceCodes::ForwardOpen),
                               data_path, data, status, pkt, &offset, &length);
            forwardRunning = *status == EIPNetworkStatus::OnLine;
            if (forwardRunning) {
                std::vector<uint8_t> u32(4, 0);
                if (!EIPCIPObjectIsNull(*O2T)) {
                    std::copy(pkt.begin() + offset, pkt.begin() + offset + 4, u32.begin());
                    inTransformU8(O2TID, u32);
                }

                if (!EIPCIPObjectIsNull(*T2O)) {
                    std::copy(pkt.begin() + offset + 4, pkt.begin() + offset + 8, u32.begin());
                    inTransformU8(T2OID, u32);
                }
            }
        }

        int forwardOpenWithRcvSend(ForwardOpenConfig *fConfig, std::vector<uint8_t> &rawData, CIPObject *config, CIPObject *O2T, CIPObject *T2O, void *decO2T, void *decT2O,
                                   uint32_t *T2OID = {}, uint32_t *O2TID = {}, EIPNetworkStatus *status = {}, uint8_t timeoutTick = 50) {
            forwardOpen(timeoutTick, rawData, config, O2T, T2O, fConfig, status, T2OID, O2TID);
            if (*status == EIPNetworkStatus::OnLine)
                return setRcvSendDaemon(fConfig->O2TDataSize, fConfig->O2TRPI / 1000, *O2TID, *T2OID, decO2T, decT2O);
            return 1;
        }

        void rcvSendIOMessage() {
#if TYPE_WINDOWS == 1
            std::vector<uint8_t> sentData(rO2TDataSize, 0);
            signal_encoder(rDecO2T, sentData.data(), rO2TDataSize);
            writeData(sentData);
#elif TYPE_WINDOWS == 0
            std::vector<uint8_t> sentData(rO2TDataSize);
            signal_decoder(rDecO2T, sentData.data(), rO2TDataSize);
            rItem.sequenceNumber = doTimes;
            rItem.data.assign(sentData.begin(), sentData.begin() + rO2TDataSize);
            class1SendO2T(&rItem);
            doTimes++;
#endif
        }

        void receiveFunction(const std::shared_ptr<ItemMessage>& item) {
            if (!forwardRunning)
                return;
            if ((item->item.connectionID == rT2OID) && ((item->msg_length - item->offset) > 0)) {
                //watchdog未配置，本地运行时需要标注以下行
                wd_reset(watchdog, 1);
                std::vector<uint8_t> receive(item->msg.begin() + item->offset, item->msg.begin() + item->msg_length);
                signal_decoder(rDecT2O, receive.data(), item->msg_length - item->offset);
            }
        }

//        std::function<void(std::shared_ptr<ItemMessage>)> getReceiveFunction(){
//            return [this](auto&& arg) { return this->receiveFuction(std::forward<decltype(arg)>(arg)); };
//        }

    private:
        void *watchdog;                                             //看门狗指针
        std::shared_ptr<EtherNetIPClient> clientPtr;                //远程设备指针
        std::string IPAddress;                                      //目标IP地址
        uint16_t tcpPort;                                           //目标TCP端口号
        std::shared_ptr<EtherNetIPTCP> tcpPtr;                      //TCP指针
        std::shared_ptr<ForwardPacket> forwardPacket;               //forward结构体指针
        uint32_t sessionHandle = 0;                                 //会话句柄
        bool forwardRunning = false;                                //forward运行状态
        bool autoConnect = true;                                    //自动连接开关
        bool autoRegisterSession = true;                            //自动开启会话开关
        int32_t rO2TDataSize{};                                       //输出数据长度
        uint32_t rT2OID{};                                            //输出连接ID
        void *rDecO2T{};                                              //输出decoder指针
        void *rDecT2O{};                                              //输入decoder指针
        SequencedAddressItem rItem;                                 //item结构体对象
        sockaddr_in addr{};                                           //I/O消息发送目标地址
        unsigned int tmrThread{};                                     //I/O消息发送线程指针
        uint32_t doTimes = 0;                                       //I/O消息发送编号

#if TYPE_WINDOWS == 1
#include <winsock2.h>
#include <mmsystem.h>

        //发送I/O消息
        int32_t class1SendO2T(){
            rItem.sequenceNumber = doTimes;
            int32_t result = clientPtr->UDPSpendItem(addr, &rItem);
            doTimes++;
            return result;

        }

        //回调函数
        static void CALLBACK RTthread(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2){
            auto pThis = reinterpret_cast<EtherNetIPRemoteDevice*>(dwUser);
            pThis->class1SendO2T();
        }

        //配置并启用发送线程
        int32_t startSendO2T(UINT uDelay, uint32_t connectionID, uint32_t O2TDataSize){
            std::vector<uint8_t> data(O2TDataSize, 0);
            rItem.newSequencedAddressItem(connectionID, 0, data);
            tmrThread = timeSetEvent(uDelay, 0, RTthread, (DWORD_PTR)this, TIME_PERIODIC | TIME_KILL_SYNCHRONOUS);
            if(tmrThread == 0)
                return -2;
            return 0;
        }

        //终止线程
        void stopSendO2T() const{
            timeKillEvent(tmrThread);
        }

        //写入I/O消息
        void writeData(std::vector<uint8_t> &data){
            std::copy(data.begin(), data.end(), rItem.data.begin());
        }
#endif
        //配置并发送显示消息，用于建立连接
        int sendUCMM_RR_Packet(uint32_t cip, std::vector<uint8_t> &dataPath, std::vector<uint8_t> &data, EIPNetworkStatus *status, std::vector<uint8_t> &pkt, int32_t *offset, int32_t *length) {
            uint32_t isTrue = 0;
            if (autoRegisterSession) {
                isTrue = registerSession();
            } else {
                isTrue = readSessionHandle();
            }
            if (isTrue == 0) {
                *status = EIPNetworkStatus::OffLine;
                msg_log_error("SessionHandleError", "", EIP_CONNECT_ERROR);
                return EIP_CONNECT_ERROR;
            }

            UCMMRRPacket packet;
            packet.newPacket(cip, true, dataPath, data);
            std::vector<uint8_t> array = packet.packetToByteArray();
            EncapsulationPacket ePacket;
            ePacket.newEncapsulationPacket(static_cast<uint32_t>(EncapsulationCommands::SendRRDate), isTrue, array);
            EncapsulationPacket received;
            tcpPtr->sendRcv(&ePacket, offset, &received, length, pkt);

            if (*length > 24) {
                if ((received.commands == EncapsulationCommands::SendRRDate) && (received.status == EncapsulationStatus::Success)) {
                    UCMMRRPacket packet1;
                    packet1.newPacketByteData(*length, offset, pkt);
                    if (packet1.isOk() && packet1.isService(cip)) {
                        *status = EIPNetworkStatus::OnLine;
                        return 0;
                    } else {
                        *status = cip == static_cast<uint32_t>(CIPServiceCodes::SetAttributeSingle) ? EIPNetworkStatus::OnLineWriteRejected : EIPNetworkStatus::OnLineReadRejected;
                        msg_log_error("UCMMStatusError: " + std::to_string(static_cast<int32_t>(packet1.generalStatus)), "remoteDevice", EIP_CONNECT_ERROR);
                    }
                } else {
                    *status = cip == static_cast<uint32_t>(CIPServiceCodes::SetAttributeSingle) ? EIPNetworkStatus::OnLineWriteRejected : EIPNetworkStatus::OnLineReadRejected;
                    msg_log_error("EncapsulationRespondError: " + std::to_string(static_cast<int32_t>(*status)), "remoteDevice", EIP_CONNECT_ERROR);
                }
            } else {
                *status = EIPNetworkStatus::OffLine;
            }
            return EIP_CONNECT_ERROR;
        }

        std::vector<uint8_t> getForwardOpenPath(CIPObject *config, CIPObject *O2T, CIPObject *T2O) {
            uint16_t attribute = 0;
            if (config->classID == 4 && config->attributeID == 3)
                attribute = 0;
            else
                attribute = config->attributeID;
            std::vector<uint8_t> configData = EtherNetIP::EIPPath::getPath(config->classID, config->instanceID, attribute, false);

            if (O2T->classID == 4 && O2T->attributeID == 3)
                attribute = 0;
            else
                attribute = O2T->attributeID;
            std::vector<uint8_t> OData = EtherNetIP::EIPPath::getPath(config->classID == O2T->classID ? 0 : O2T->classID, O2T->instanceID, attribute, true);


            if (T2O->classID == 4 && T2O->attributeID == 3)
                attribute = 0;
            else
                attribute = T2O->attributeID;
            std::vector<uint8_t> TData = EtherNetIP::EIPPath::getPath(O2T->classID == T2O->classID ? 0 : T2O->classID, T2O->instanceID, attribute, true);
            std::vector<uint8_t> path(configData.size() + OData.size() + TData.size());
            int offset = 0;
            std::copy(configData.begin(), configData.end(), path.begin());
            offset += static_cast<int>(configData.size());
            std::copy(OData.begin(), OData.end(), path.begin() + offset);
            offset += static_cast<int>(OData.size());
            std::copy(TData.begin(), TData.end(), path.begin() + offset);
            return path;
        }

        //配置收发线程
        int setRcvSendDaemon(int32_t O2TDataSize, int64_t cycle, uint32_t connectionID, uint32_t T2OID, void *decO2T, void *decT2O) {
            rO2TDataSize = O2TDataSize;
            rT2OID = T2OID;
            rDecT2O = decT2O;
            rDecO2T = decO2T;
            int32_t result = 0;
            //返回结果，0为正常
            //预处理
            if(cycle < 1)
                result = -2;
            if(O2TDataSize <= 0)
                result = -3;
            if(result != 0) return result;
#if TYPE_WINDOWS == 1
            startSendO2T(static_cast<UINT>(cycle), connectionID, static_cast<uint32_t>(O2TDataSize));
#elif TYPE_WINDOWS == 0
            rItem.newSequencedAddressItem(connectionID);
#endif
            return result;
        }
    };
}
#endif //SERVERDEVICE_ETHERNET_ETHERNETIPREMOTEDEVICE_H
