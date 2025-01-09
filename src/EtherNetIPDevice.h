//
// Created by 79933 on 2024/11/25.
//

#ifndef SERVERDEVICE_ETHERNET_ETHERNETIPDEVICE_H
#define SERVERDEVICE_ETHERNET_ETHERNETIPDEVICE_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "EtherNetIPRemoteDevice.h"
#include "EtherNetIPTypeSupport.h"

#include "serverlogger/logger.h"
#include "byte_decoder/byte_decoder.h"
#include "watchdog/mtestserver_watchdog.h"
#include "tools.h"

#define BUFFER_SIZE 512
namespace EtherNetIP {
    class EtherNetIPDevice {
    public:
        explicit EtherNetIPDevice(std::string name, const std::shared_ptr<DeviceConfig> &deviceConfig, void *con) : deviceName(std::move(name)), deviceConfig(deviceConfig) {
            //检查输入client的IP地址和端口
            if (!isValidIPv4(deviceConfig->IPAddress)) {
                errorCode = EIP_IP_ADDRESS_ERROR;
                msg_log_error("EtherNetIP Client Loaded failed!");
                msg_log_error("Local IP address error: " + deviceConfig->IPAddress, deviceName, errorCode);
                return;
            }
            //创建client指针
            clientPtr = std::make_shared<EtherNetIPClient>(deviceConfig->IPAddress, deviceConfig->udpPort);
            if (clientPtr == nullptr) {
                errorCode = DEVICE_CONFIG_PARSE_ERROR;
                return;
            }
            //用于给remoteDevice对象编号，从1开始，用于serverLog报错输出
            int rdvcWdNum = 1;
            //用于记录每个remoteDevice对象的循环时间
            std::vector<int> cycles;
            //遍历每个remoteDevice对象
            for (auto &dvc: deviceConfig->remoteDevice) {
                //检测remoteDevice对象的IP地址和端口
                if (!isValidIPv4(dvc.IPAddress)) {
                    errorCode = EIP_IP_ADDRESS_ERROR;
                    msg_log_error("EtherNetIP RemoteDevice: " + dvc.name + "loaded failed!");
                    msg_log_error("RemoteDevice " + dvc.name + "IP address error: " + dvc.IPAddress, deviceName, errorCode);
                    return;
                }
                //创建watchdog指针
                void *watchdog;
                wd_create(dvc.watchdog.data(), 10, &watchdog);
                //创建remoteDevice对象指针
                auto ptr = std::make_shared<EtherNetIPRemoteDevice>(watchdog, clientPtr, dvc.IPAddress, dvc.tcpPort, dvc.udpPort);
                //指针不为空，否则报错
                if (ptr == nullptr) {
                    errorCode = DEVICE_CONFIG_PARSE_ERROR;
                    msg_log_error("EtherNetIP RemoteDevice: " + dvc.name + "loaded failed!");
                    return;
                }
                //使用functional创建函数指针接收I/O消息
                auto receiveFunction = [ptr](const std::shared_ptr<ItemMessage>& arg) { return ptr->receiveFunction(std::forward<decltype(arg)>(arg)); };
                //将functional指针依次存入client指针，便于后续调用
                errorCode = clientPtr->addReceiveFunction(receiveFunction);
                if (errorCode != 0)
                    return;
                //将remoteDevice对象指针依次存入其指针容器
                remoteDevicesPtr.push_back(ptr);
                //配置设备
                EIPNetworkStatus status;
                std::vector<uint8_t> rawData;
                //配置config
                if (dvc.configBytes != 0) {
                    ptr->getAttributeData(&dvc.EIPConfig, rawData, &status);
                    if (status == EIPNetworkStatus::OnLine) {
                        if (rawData.size() != dvc.configBytes) {
                            errorCode = EIP_CONFIG_BYTE_ERROR;
                            msg_log_error("EtherNetIP RemoteDevice: " + dvc.name + "loaded failed!");
                            msg_log_error("The number of config bytes is inconsistent", deviceName, errorCode);
                            return;
                        }
                    } else {
                        errorCode = EIP_CONFIG_READ_FAILED;
                        msg_log_error("EtherNetIP RemoteDevice: " + dvc.name + "loaded failed!");
                        msg_log_error("Config read failed", deviceName, errorCode);
                        return;
                    }
                }
                //配置O2T
                if (dvc.O2TBytes != 0) {
                    ptr->getAttributeData(&dvc.EIPO2T, rawData, &status);
                    if (status == EIPNetworkStatus::OnLine) {
                        if (rawData.size() != dvc.O2TBytes) {
                            errorCode = EIP_OUTPUT_BYTE_ERROR;
                            msg_log_error("EtherNetIP RemoteDevice: " + dvc.name + "loaded failed!");
                            msg_log_error("The number of output bytes is inconsistent, the number of bytes in the device: " + std::to_string(rawData.size()), deviceName, errorCode);
                            return;
                        }
                    } else {
                        errorCode = EIP_OUTPUT_READ_FAILED;
                        msg_log_error("EtherNetIP RemoteDevice: " + dvc.name + " loaded failed!");
                        msg_log_error("Output read Failed", deviceName, errorCode);
                        return;
                    }
                } else {
                    errorCode = EIP_OUTPUT_BYTE_NULL;
                    msg_log_error("EtherNetIP RemoteDevice: " + dvc.name + " loaded failed!");
                    msg_log_error("The output byte count is empty", deviceName, errorCode);
                    return;
                }
                //配置T2O
                if (dvc.T2OBytes != 0) {
                    ptr->getAttributeData(&dvc.EIPT2O, rawData, &status);
                    if (status == EIPNetworkStatus::OnLine) {
                        if (rawData.size() != dvc.T2OBytes) {
                            errorCode = EIP_INPUT_BYTE_ERROR;
                            msg_log_error("EtherNetIP RemoteDevice: " + dvc.name + " loaded failed!");
                            msg_log_error("The number of input bytes is inconsistent, the number of bytes in the device: " + std::to_string(rawData.size()), deviceName, errorCode);
                            return;
                        }
                    } else {
                        errorCode = EIP_INPUT_READ_FAILED;
                        msg_log_error("EtherNetIP RemoteDevice: " + dvc.name + " loaded failed!");
                        msg_log_error("Input read Failed", deviceName, errorCode);
                        return;
                    }
                }
                //创建forwardOpen
                ForwardOpenConfig fConfig{};
                fConfig.newForwardOpenConfig(dvc.O2TBytes, dvc.T2OBytes, dvc.cycleTime * 1000, true);
                //依次将配置信号分类，并将其转换成Signal类型，存入对应容器中
                void *configDecoder{};
                void *T2ODecoder{};
                void *O2TDecoder{};
                std::string configStr;
                std::string T2OStr;
                std::string O2TStr;
                for (auto &sin: dvc.signals) {
                    if (sin.signalType == 0 && dvc.configBytes != 0) {
                        configSignal.push_back(convertSignals(sin));
                    }
                    if (sin.signalType == 1) {
                        T2OSignal.push_back(convertSignals(sin));
                    }
                    if (sin.signalType == 2) {
                        O2TSignal.push_back(convertSignals(sin));
                    }
                }
                //处理config类型的信号
                uint8_t data[BUFFER_SIZE]{};
                if (!configSignal.empty()) {
                    configDecoder = create_signal_decoder(con, signalVectorToJSON(configSignal).c_str(), 0, 0);
                    signal_decoder(configDecoder, data, BUFFER_SIZE);
                    free_signal_decoder(configDecoder);
                    std::vector<uint8_t> configRawData(data, data + BUFFER_SIZE);
                }
                //处理T2O类型的信号
                if (!T2OSignal.empty()) {
                    T2ODecoder = create_signal_decoder(con, signalVectorToJSON(T2OSignal).c_str(), 0, 0);
                }
                //处理O2T类型的信号
                if (!O2TSignal.empty()) {
                    O2TDecoder = create_signal_decoder(con, signalVectorToJSON(O2TSignal).c_str(), 0, 0);
                }

                //发送forwardOpen TCP消息
                uint32_t O2TID;
                uint32_t T2OID;
                std::vector<uint8_t> data_u8{};
                errorCode = ptr->forwardOpenWithRcvSend(&fConfig, data_u8, &dvc.EIPConfig, &dvc.EIPO2T, &dvc.EIPT2O, O2TDecoder, T2ODecoder, &T2OID, &O2TID, &status, dvc.timeoutTicks);
                if (errorCode)
                    return;
                //依次记录每个remoteDevice对象的循环时间
                cycles.push_back(static_cast<int>(dvc.cycleTime));
                //记录每个remoteDevice对象的编号
                rdvcWdNum++;
                //没有错误，则返回配置设置成功
                msg_log_info("EtherNetIP RemoteDevice: " + dvc.name + " loaded success!");
            }
            //根据每个设备的循环时间，使用最大公因数调整写入data的时间
            rSize = static_cast<int>(remoteDevicesPtr.size());
            cycle = cycles[0];
            for (int n = 1; n < cycles.size(); n++)
                cycle = EuclideanAlgorithm(cycle, cycles[n]);
            times.resize(remoteDevicesPtr.size());
            for (int m = 0; m < cycles.size(); m++)
                times[m] = cycles[m] / cycle;
            //开启UDP接收线程
            clientPtr->startRcvDaemon();
        }

        explicit EtherNetIPDevice(const std::string &name, const std::string &config, void *con) : EtherNetIPDevice(name, parseConfig(config, errorCode), con) {}

        ~EtherNetIPDevice() {
            //释放client和所有remoteDevice指针使用的资源
            clientPtr->freeClient();
            for (auto &dvc: remoteDevicesPtr)
                dvc->freeRemoteDevice();
        }

        void DoStep() {
            for (int i = 0; i < rSize; i++) {
                //按照最大公因数，设置循环时间，调用UDP发送函数
                if (doStepTimes % times[i] == 0)
                    remoteDevicesPtr[i]->rcvSendIOMessage();
            }
            doStepTimes++;
        }

        bool DoFunction(const std::string &name, const std::string &data, std::string &result) const { return true; }

        void GetAllVariables(std::string &vars, std::string &des) {
            int i = 1, j = 1;
            for (auto &dvc: deviceConfig->remoteDevice) {
                if (!dvc.typeVar.empty()) {
                    vars += dvc.typeVar + ",";
                    des += dvc.typeVar + ",";
                }
                if (!dvc.watchdog.empty()) {
                    vars += dvc.watchdog + ",";
                    des += dvc.watchdog + ",";
                }
                for (auto &sin: dvc.signals) {
                    if (!sin.varName.empty()) {
                        vars += sin.varName + ",";
                        des += sin.varName + ",";
                    }
                    j++;
                }
                i++;
            }
            if (vars.back() == ',')vars.pop_back();
            if (des.back() == ',')des.pop_back();
        }

        void GetWrittenVariables(std::string &vars, std::string &des) {
            int i = 1, j = 1;
            for (auto &dvc: deviceConfig->remoteDevice) {
                for (auto &sin: dvc.signals) {
                    if (!sin.varName.empty()) {
                        vars += sin.varName + ",";
                        des += sin.varName + ",";
                    }
                    j++;
                }
                i++;
            }
            if (vars.back() == ',')vars.pop_back();
            if (des.back() == ',')des.pop_back();
        }

        void set_name(const std::string &name) { deviceName = name; }

        std::string get_name() const { return deviceName; }

        int get_error_code() const { return errorCode; }

        void get_timing_info(double &freq, int &processor, int &priority) const {
            freq = static_cast<double>(cycle);
            processor = -1;
            priority = 100;
        }

        static std::string get_version() { return "V1.0.0"; }

    private:
        std::string deviceName;                                                           //设备名称
        int errorCode = 0;                                                                //错误码
        std::shared_ptr<DeviceConfig> deviceConfig;                                       //设备配置结构体指针
        std::shared_ptr<EtherNetIPClient> clientPtr;                                      //客户端指针
        std::vector<std::shared_ptr<EtherNetIPRemoteDevice>> remoteDevicesPtr;            //远程设备指针
        std::vector<Signal> configSignal;                                                 //设备信号列表
        std::vector<Signal> O2TSignal;                                                    //输出信号列表
        std::vector<Signal> T2OSignal;                                                    //输入信号列表
        int rSize;                                                                        //远程设备个数
        int cycle;                                                                        //各远程设备循环时间的最大公因数
        std::vector<int> times;                                                           //各远程设备循环时间与最大公因数的商
        int doStepTimes = 0;                                                              //doStep运行次数
    };
}

#endif //SERVERDEVICE_ETHERNET_ETHERNETIPDEVICE_H
