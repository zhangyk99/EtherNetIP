//
// Created by 79933 on 2024/11/25.
//

#ifndef SERVERDEVICE_ETHERNET_ETHERNETIPTYPES_H
#define SERVERDEVICE_ETHERNET_ETHERNETIPTYPES_H

#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <random>
#include <iostream>
#include "EtherNetIPIPEnum.h"
#include "../tools/byte_decoder/Signal.h"

namespace EtherNetIP {
    struct CIPObject {
        uint16_t classID;
        uint16_t instanceID;
        uint16_t attributeID;
    };

    struct signal {
        uint16_t signalType;
        std::string name;
        std::string varName;
        uint16_t byteOrder;
        uint16_t dataType;
        uint16_t startBit;
        uint16_t numberOfBits;
        double gain;
        double offset;
        double maxValue;
        double minValue;
        double defaultValue;
        std::string description;
    };

    struct RemoteDevice {
        std::string IPAddress;
        uint16_t tcpPort;
        uint16_t udpPort;
        uint32_t cycleTime;
        uint32_t timeoutTicks;
        std::string name;
        std::string description;
        std::string watchdog;
        uint16_t type;
        std::string typeVar;
        CIPObject EIPConfig;
        CIPObject EIPO2T;
        CIPObject EIPT2O;
        uint16_t configBytes;
        uint16_t O2TBytes;
        uint16_t T2OBytes;
        std::vector<signal> signals;
    };

    struct DeviceConfig {
        std::string IPAddress;
        uint32_t udpPort;
        std::vector<RemoteDevice> remoteDevice;
    };

    bool EIPCIPObjectIsNull(CIPObject object) {
        return !(object.classID) & !(object.instanceID) & !(object.attributeID);
    };

    static void transformU8(uint16_t num, std::vector<uint8_t> &result) {
        if (result.size() != 2)
            return;
        result[0] = static_cast<uint8_t>(num & 0xff);
        result[1] = static_cast<uint8_t>((num >> 8) & 0xff);
    }

    static void transformU8(uint32_t num, std::vector<uint8_t> &result) {
        if (result.size() != 4)
            return;
        result[0] = static_cast<uint8_t>(num & 0xff);
        result[1] = static_cast<uint8_t>((num >> 8) & 0xff);
        result[2] = static_cast<uint8_t>((num >> 16) & 0xff);
        result[3] = static_cast<uint8_t>((num >> 24) & 0xff);
    }

    static void inTransformU8(uint16_t *num, const std::vector<uint8_t> &result) {
        if (result.size() != 2)
            return;
        *num = result[0] + (result[1] << 8);
    }

    static void inTransformU8(uint32_t *num, const std::vector<uint8_t> &result) {
        if (result.size() != 4)
            return;
        *num = result[0] + (result[1] << 8) + (result[2] << 16) + (result[3] << 24);
    }

    DataType convertDataType(uint16_t dataType = 0) {
        switch (dataType) {
            case 0:
                return DataType::Default;
            case 1:
                return DataType::Bool;
            case 3:
            case 5:
            case 7:
            case 9:
                return DataType::UnSigned;
            case 2:
            case 4:
            case 6:
            case 8:
                return DataType::Signed;
            case 10:
                return DataType::Float;
            case 11:
                return DataType::Double;
            default:
                return DataType::Default;
        }
    }

    Signal convertSignals(const signal &sin) {
        Signal sign;
        sign.name = sin.varName;
        sign.byteOrder = static_cast<ByteOrder>(sin.byteOrder);
        sign.dataType = convertDataType(sin.dataType);
        sign.startBit = sin.startBit;
        sign.numOfBits = sin.numberOfBits;
        sign.gain = sin.gain;
        sign.offset = sin.offset;
        sign.maxValue = sin.maxValue;
        sign.minValue = sin.minValue;
        sign.defaultValue = sin.defaultValue;
        return sign;
    }

    class EIPPath {
    public:
        static std::vector<uint8_t> addDataSegment(std::vector<uint8_t> &extendPath, std::vector<uint8_t> &data) {
            std::vector<uint8_t> path = extendPath;
            path.resize(extendPath.size() + 2);
            path[extendPath.size()] = 0x80;
            path[extendPath.size() + 1] = data.size() / 2 + data.size() % 2;
            path.insert(path.end(), data.begin(), data.end());
            return path;
        }

        static std::vector<uint8_t> getPath(uint16_t class_, uint16_t instance, uint16_t attribut, bool isConnectionPoint = false) {
//            if(!class_ && !attribut)
//                return {};
            std::vector<uint8_t> path{};
            int32_t offset = 0;
            if (class_)
                fit(path, &offset, class_, 0x20);

            if (isConnectionPoint)
                fit(path, &offset, instance, 0x2c);
            else
                fit(path, &offset, instance, 0x24);

            if (attribut)
                fit(path, &offset, attribut, 0x30);

            return path;
        }

        static std::vector<uint8_t> getPathString(const std::string &str) {
            std::vector<uint16_t> result;
            std::istringstream stream(str);
            std::string token;

            while (std::getline(stream, token, '.')) {
                try {
                    unsigned long value = std::stoul(token);
                    if (value > UINT16_MAX)
                        throw std::out_of_range("Value exceeds uint16_t range");
                    result.push_back(static_cast<uint16_t>(value));
                } catch (const std::exception &e) {
                    std::cerr << "Conversion error: " << e.what() << std::endl;
                    return {};
                }
            }

            if (result.size() == 2)
                return getPath(result[1], result[2], 0);

            if (result.size() == 3)
                return getPath(result[1], result[2], result[3]);

            return {};
        }

        static std::string getPathString(const std::vector<uint8_t> &path) {
            std::string str{};
            int i = 0;

            do {
                if ((path[i] & 3) == 1) {
                    str += std::to_string(path[i + 2]) + std::to_string(path[i + 3]) + '.';
                    i += 4;
                } else {
                    str += std::to_string(path[i + 1]) + '.';
                }
            } while (i < str.size());

            if (str.back() == '.')
                str.pop_back();

            return str;
        }

        static std::vector<uint8_t> grtExtendedPath(const std::string &iPendPoint, const std::vector<int32_t> &logicalSeg) {
            std::vector<uint8_t> path(iPendPoint.size() + iPendPoint.size() % 2 + logicalSeg.size() + 2, 0);
            path[0] = 0x15;
            path[1] = iPendPoint.size();

            std::vector<uint8_t> value(iPendPoint.size());
            int i = 0;
            for (auto ch: iPendPoint) {
                value[i] = static_cast<int>(ch);
                i++;
            }
            std::copy(value.begin(), value.end(), path.begin() + 2);
            std::copy(logicalSeg.begin(), logicalSeg.end(), path.begin() + 2 + static_cast<int>(iPendPoint.size() + iPendPoint.size() % 2));

            return path;
        }

    private:
        static void fit(std::vector<uint8_t> &path, int32_t *offset, uint16_t value, uint8_t code) {
            if (value > 255) {
                path.resize(*offset + 4);
                path[*offset] = code | 0x1;
                path[*offset + 2] = value & 0xff;
                path[*offset + 3] = value & 0xff00;
                *offset += 4;
            } else {
                path.resize(*offset + 2);
                path[*offset] = code;
                path[*offset + 1] = value & 0xff;
                *offset += 2;
            }
        }

    };

    class EncapsulationPacket {
    public:
        EncapsulationCommands commands = EncapsulationCommands::Nop;
        uint16_t length = 0;
        uint32_t sessionHandle = 0;
        EncapsulationStatus status = EncapsulationStatus::Success;
        std::vector<uint8_t> senderContext{};
        uint32_t options = 0;
        std::vector<uint8_t> encapsulateData;

        bool isOk() const {
            return status == EncapsulationStatus::Success;
        }

        std::vector<uint8_t> toByteArray() {
            std::vector<uint8_t> array(length + 24, 0);
            std::vector<uint8_t> u16(2, 0);
            std::vector<uint8_t> u32(4, 0);

            transformU8(static_cast<uint16_t>(commands), u16);
            std::copy(u16.begin(), u16.end(), array.begin());

            transformU8(length, u16);
            std::copy(u16.begin(), u16.end(), array.begin() + 2);

            transformU8(sessionHandle, u32);
            std::copy(u32.begin(), u32.end(), array.begin() + 4);

            transformU8(static_cast<uint32_t>(EncapsulationStatus::Success), u32);
            std::copy(u32.begin(), u32.end(), array.begin() + 8);

            std::copy(senderContext.begin(), senderContext.end(), array.begin() + 12);

            transformU8(options, u32);
            std::copy(u32.begin(), u32.end(), array.begin() + 20);

            std::copy(encapsulateData.begin(), encapsulateData.end(), array.begin() + 24);

            return array;
        }

        void newEncapsulationPacketByteArray(const std::vector<uint8_t> &packet, int32_t *offset, const int32_t length_) {
            std::vector<uint8_t> u16(2, 0);
            std::vector<uint8_t> u32(4, 0);
            uint16_t num16;
            uint32_t num32;

            std::copy(packet.begin() + *offset, packet.begin() + *offset + 2, u16.begin());
            inTransformU8(&num16, u16);
            if (!isEncapsulationCommand(num16)) {
                status = EncapsulationStatus::UnsupportedCommand;
                return;
            }
            commands = static_cast<EncapsulationCommands>(num16);

            *offset += 2;
            std::copy(packet.begin() + *offset, packet.begin() + *offset + 2, u16.begin());
            inTransformU8(&num16, u16);
            if (length_ < num16 + 24) {
                status = EncapsulationStatus::InvalidLength;
                return;
            }
            length = num16;

            *offset += 2;
            std::copy(packet.begin() + *offset, packet.begin() + *offset + 4, u32.begin());
            inTransformU8(&num32, u32);
            sessionHandle = num32;
            *offset += 4;

            std::copy(packet.begin() + *offset, packet.begin() + *offset + 4, u32.begin());
            inTransformU8(&num32, u32);
            status = static_cast<EncapsulationStatus>(num32);
            *offset += 4;

            senderContext.resize(8);
            std::copy(packet.begin() + *offset, packet.begin() + *offset + 8, senderContext.begin());
            *offset += 8;

            std::copy(packet.begin() + *offset, packet.begin() + *offset + 4, u32.begin());
            inTransformU8(&num32, u32);
            options = num32;
            *offset += 4;
        }

        void newEncapsulationPacket(uint16_t commands_, uint32_t sessionHandle_, std::vector<uint8_t> &data) {
            commands = static_cast<EncapsulationCommands>(commands_);
            length = data.size();
            sessionHandle = sessionHandle_;
            encapsulateData.resize(data.size());
            std::copy(data.begin(), data.end(), encapsulateData.begin());
        }

        static bool isEncapsulation(std::vector<uint8_t> &receivedArray) {
            std::vector<uint8_t> u16(2, 0);
            uint16_t num16;
            std::copy(receivedArray.begin(), receivedArray.begin() + 2, u16.begin());
            inTransformU8(&num16, u16);
            return isEncapsulationCommand(num16);
        }

    private:
        static bool isEncapsulationCommand(uint16_t command) {
            bool isCommand = false;
            for (auto &i: encapsulationCommands)
                if (command == i)
                    isCommand = true;
            return isCommand;
        }
    };

    class ForwardOpenConfig {
    public:
        bool isO2T = false;
        bool isT2O = false;
        bool O2TExculsive = false;
        bool T2OExculsive = false;
        bool O2TP2P = false;
        bool T2OP2P = false;
        uint8_t O2TPriority = 0;
        uint8_t T2OPriority = 0;
        uint16_t O2TDataSize = 0;
        uint16_t T2ODataSize = 0;
        uint32_t O2TRPI = 0;
        uint32_t T2ORPI = 0;

        void newForwardOpenConfig(uint16_t o2tDatasize, uint16_t t2oDatasize, uint32_t cycleTime, bool t2oP2P = true) {
            isO2T = static_cast<bool>(o2tDatasize);
            O2TDataSize = o2tDatasize;
            O2TRPI = cycleTime;
            O2TP2P = true;

            isT2O = static_cast<bool>(t2oDatasize);
            T2ODataSize = t2oDatasize;
            T2ORPI = cycleTime;
            T2OP2P = t2oP2P;
        }
    };

    class ForwardPacket {
    public:
        bool isLargeForwardOpen = false;
        uint8_t priorityTimeTick = 10;
        uint8_t timeoutTicks = 10;
        uint32_t connectionID = 0;
        uint32_t O2TConnectionID = 0;
        uint32_t T2OConnectionID = 0;
        uint16_t connectionSN = 0;
        uint16_t originatorVendorID = 0xFADA;
        uint32_t originatorSN = 0x8BADF00D;
        uint8_t connectionTimeoutMultiplier = 0;
        std::vector<uint8_t> reserved{0, 0, 0};
        uint32_t O2TRPI = 0;
        uint32_t O2TConnectionParameters = 0;
        uint32_t T2ORPI = 0;
        uint32_t T2OConnectionParameters = 0;
        uint8_t transportTrigger = 1;
        uint8_t connectionPathSize = 0;
        std::vector<uint8_t> connectionPath{};

        std::vector<uint8_t> forwardClosePacketToByteArray() {
            std::vector<uint8_t> output(12 + 2 * connectionPathSize);
            std::vector<uint8_t> num16(2, 0);
            std::vector<uint8_t> num32(4, 0);

            output[0] = priorityTimeTick;
            output[1] = timeoutTicks;

            transformU8(connectionSN, num16);
            std::copy(num16.begin(), num16.end(), output.begin() + 2);

            transformU8(originatorVendorID, num16);
            std::copy(num16.begin(), num16.end(), output.begin() + 4);

            transformU8(originatorSN, num32);
            std::copy(num32.begin(), num32.end(), output.begin() + 6);

            output[10] = connectionPathSize;
            output[11] = 0;

            std::copy(connectionPath.begin(), connectionPath.end(), output.begin() + 12);

            return output;
        }

        std::vector<uint8_t> forwardOpenPacketToByteArray() {
            std::vector<uint8_t> input;
            if (isLargeForwardOpen) {
                input.resize((connectionPath.size() / 2 + connectionPath.size() % 2) * 2 + 40);
            } else {
                input.resize((connectionPath.size() / 2 + connectionPath.size() % 2) * 2 + 36);
            }
            std::vector<uint8_t> num16(2, 0);
            std::vector<uint8_t> num32(4, 0);

            input[0] = priorityTimeTick;
            input[1] = timeoutTicks;

            transformU8(O2TConnectionID, num32);
            std::copy(num32.begin(), num32.end(), input.begin() + 2);

            transformU8(T2OConnectionID, num32);
            std::copy(num32.begin(), num32.end(), input.begin() + 6);

            transformU8(connectionSN, num16);
            std::copy(num16.begin(), num16.end(), input.begin() + 10);

            transformU8(originatorVendorID, num16);
            std::copy(num16.begin(), num16.end(), input.begin() + 12);

            transformU8(originatorSN, num32);
            std::copy(num32.begin(), num32.end(), input.begin() + 14);

            input[18] = connectionTimeoutMultiplier;

            std::copy(reserved.begin(), reserved.begin() + 2, input.begin() + 19);

            transformU8(O2TRPI, num32);
            std::copy(num32.begin(), num32.end(), input.begin() + 22);

            if (isLargeForwardOpen) {
                transformU8(O2TConnectionParameters, num32);
                std::copy(num32.begin(), num32.end(), input.begin() + 26);

                transformU8(T2ORPI, num32);
                std::copy(num32.begin(), num32.end(), input.begin() + 30);

                transformU8(T2OConnectionParameters, num32);
                std::copy(num32.begin(), num32.end(), input.begin() + 34);

                input[38] = transportTrigger;
                input[39] = connectionPathSize;

                std::copy(connectionPath.begin(), connectionPath.end(), input.begin() + 40);
            } else {
                transformU8(static_cast<uint16_t>(O2TConnectionParameters), num16);
                std::copy(num16.begin(), num16.end(), input.begin() + 26);

                transformU8(T2ORPI, num32);
                std::copy(num32.begin(), num32.end(), input.begin() + 28);

                transformU8(static_cast<uint16_t>(T2OConnectionParameters), num16);
                std::copy(num16.begin(), num16.end(), input.begin() + 32);

                input[34] = transportTrigger;
                input[35] = connectionPathSize;

                std::copy(connectionPath.begin(), connectionPath.end(), input.begin() + 36);
            }
            return input;
        }

        void newForwardPacket(uint8_t timeout, std::vector<uint8_t> &cPath, ForwardOpenConfig config, uint32_t cID) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);
            timeoutTicks = timeout;
            connectionPathSize = cPath.size() / 2 + cPath.size() % 2;
            isLargeForwardOpen = (config.O2TDataSize > (511 - 2) || config.T2ODataSize > (511 - 6));
            connectionPath.resize(cPath.size());
            std::copy(cPath.begin(), cPath.end(), connectionPath.begin());

            cID = cID == 0 ? static_cast<uint32_t>(dist(gen)) : cID;
            O2TConnectionID = config.isO2T ? cID : 0;
            T2OConnectionID = config.isT2O ? cID + 1 : 0;
            connectionID = cID;

            if (config.isO2T) {
//                uint32_t num0 = ((static_cast<uint32_t>(config.O2TPriority) & 3) << 10) & (config.O2TP2P ? 0x4000 : 0x2000);
//                uint32_t num1 = config.O2TExculsive ? num0 & 0x8000 : num0;
                uint32_t num0 = config.O2TP2P ? 0x4600 : 0x2600;
                O2TConnectionParameters = isLargeForwardOpen ? (config.O2TDataSize != 0 ? (num0 << 16) + config.O2TDataSize + 2 + 4 : (num0 << 16) + 2) : (config.O2TDataSize != 0 ? num0 +
                                                                                                                                                                                     config.O2TDataSize +
                                                                                                                                                                                     2 + 4 : num0 + 2);
                O2TRPI = config.O2TRPI;
            }

            if (config.isT2O) {
                uint32_t num0 = ((static_cast<uint32_t>(config.T2OPriority) & 3) << 10) | (config.T2OP2P ? 0x4000 : 0x2000);
                uint32_t num1 = config.T2OExculsive ? num0 & 0x8000 : num0;
                T2OConnectionParameters = isLargeForwardOpen ? (num1 << 16) + config.T2ODataSize + 2 : num1 + config.T2ODataSize + 2;

                T2ORPI = config.T2ORPI;
            } else {
                T2ORPI = 0;
                T2OConnectionParameters = 0;
            }

            connectionSN = dist(gen) * 0xffff;
        }

        void setTriggerType(uint8_t transportClassTriggerAttribute) {
            transportTrigger = (transportTrigger | 0x8f) & transportClassTriggerAttribute;
        }
    };

    class SequencedAddressItem {
    public:
        uint16_t typeID = 0;
        uint16_t length = 8;
        uint32_t connectionID = 0;
        uint32_t sequenceNumber = 0;
        uint16_t typeID2 = 0;
        uint16_t length2 = 8;
        uint16_t sequenceCount = 0;
        std::vector<uint8_t> data{};

        std::vector<uint8_t> sequenceToByteArray() {
            std::vector<uint8_t> array;
            std::vector<uint8_t> num16(2, 0);
            std::vector<uint8_t> num32(4, 0);
            uint16_t num{};

            if (data.empty()) {
                array.resize(18, 0);
            } else {
                array.resize(data.size() + 24, 0);
                num = static_cast<uint16_t>(data.size() + 6);
            }

            array[0] = 2;
            array[2] = 2;
            array[3] = 128;
            array[4] = 8;
            array[14] = 177;

            transformU8(connectionID, num32);
            std::copy(num32.begin(), num32.end(), array.begin() + 6);

            transformU8(sequenceNumber, num32);
            std::copy(num32.begin(), num32.end(), array.begin() + 10);

            transformU8(num, num16);
            std::copy(num16.begin(), num16.end(), array.begin() + 16);

            if (static_cast<bool>(num)) {
                transformU8(sequenceNumber, num32);
                std::copy(num32.begin(), num32.begin() + 1, array.begin() + 18);

                array[20] = 1;

                std::copy(data.begin(), data.end(), array.begin() + 24);

                return array;
            } else {
                return array;
            }
        }

        void newSequencedAddressItemByArray(std::vector<uint8_t> &array, int32_t *offset, int32_t length_) {
            std::vector<uint8_t> num16(2, 0);
            std::vector<uint8_t> num32(4, 0);
            uint16_t num0{};
            uint32_t num1{};

            *offset += 2;
            std::copy(array.begin() + *offset, array.begin() + *offset + 2, num16.begin());
            inTransformU8(&num0, num16);
            uint16_t num = num0;

            if (num == static_cast<uint16_t>(CommonPacketItemIdNumbers::SequencedAddressItem)) {
                *offset += 4;
                std::copy(array.begin() + *offset, array.begin() + *offset + 4, num32.begin());
                inTransformU8(&num1, num32);
                connectionID = num1;

                *offset += 4;
                std::copy(array.begin() + *offset, array.begin() + *offset + 4, num32.begin());
                inTransformU8(&num1, num32);
                sequenceNumber = num1;

                *offset += 4;
                std::copy(array.begin() + *offset, array.begin() + *offset + 2, num16.begin());
                inTransformU8(&num0, num16);
                typeID2 = num0;

                if ((typeID2 != static_cast<uint16_t>(CommonPacketItemIdNumbers::UnconnectedDataItem) && (typeID2 != static_cast<uint16_t>(CommonPacketItemIdNumbers::ConnectedDataItem)))) {
                    typeID = num;
                } else {
                    *offset += 2;
                    std::copy(array.begin() + *offset, array.begin() + *offset + 2, num16.begin());
                    inTransformU8(&num0, num16);
                    *offset += 2;

                    typeID = num0 + *offset == length_ ? num : 0;
                    length2 = num0;

                    if (num0) {
                        std::copy(array.begin() + *offset, array.begin() + *offset + 2, num16.begin());
                        inTransformU8(&num0, num16);
                        sequenceCount = num0;
                        *offset += 2;
                    }
                }
            } else {
                typeID = num;
            }
        }

        void newSequencedAddressItem(uint32_t cID, uint32_t sNumber = 0, std::vector<uint8_t> data_ = {}) {
            connectionID = cID;
            sequenceNumber = sNumber;
            data.assign(data_.begin(), data_.end());
        }

        bool sequencedIsOk() const {
            return (typeID == 0x8002) & (typeID2 == 0xb1);
        }
    };

    class UCMMRRPacket {
    public:
        uint16_t itemCount = 2;
        CommonPacketItemIdNumbers itemID = CommonPacketItemIdNumbers::UnconnectedDataItem;
        uint16_t dataLength = 0;
        uint8_t service = 0;
        CIPGeneralStatusCode generalStatus = CIPGeneralStatusCode::Success;
        uint8_t additionalStatusSize = 0;
        std::vector<uint16_t> additionalStatus{};
        std::vector<uint8_t> path{};
        std::vector<uint8_t> data{};

        bool isQuery() const {
            return service < 0x80;
        }

        bool isResponse() const {
            return service > 0x80;
        }

        bool isService(uint32_t serviceCode) const {
            if (static_cast<uint8_t>(serviceCode) == (service & 0x7f)) {
                return true;
            } else {
                return (static_cast<CIPServiceCodes>(serviceCode) == CIPServiceCodes::UnconnectedSend) && (service > 0x80);
            }
        }

        bool isOk() const {
            return generalStatus == CIPGeneralStatusCode::Success;
        }

        void newPacket(uint32_t CIPServiceCodes, bool isRequest, std::vector<uint8_t> &path_, std::vector<uint8_t> &data_) {
            service = isRequest ? CIPServiceCodes : CIPServiceCodes | 0x80;
            path.resize(path_.size());
            data.resize(data_.size());
            std::copy(path_.begin(), path_.end(), path.begin());
            std::copy(data_.begin(), data_.end(), data.begin());
        }

        void newPacketByteData(int32_t length, int32_t *offset, std::vector<uint8_t> &dataArray) {
            std::vector<uint8_t> num16(2, 0);
            uint16_t num{};
            *offset += 16;

            service = dataArray[*offset];
            additionalStatusSize = dataArray[*offset + 3];
            *offset += 4;
            generalStatus = (*offset + 2 * additionalStatusSize) > length ? CIPGeneralStatusCode::NotEnoughData : static_cast<CIPGeneralStatusCode>(dataArray[*offset - 2]);

            if (additionalStatusSize > 0)
                for (int i = 0; i < additionalStatusSize; i++) {
                    std::copy(dataArray.begin(), dataArray.begin() + 1, num16.begin());
                    inTransformU8(&num, num16);
                    additionalStatus.push_back(num);
                    *offset += 2;
                }
        }

        std::vector<uint8_t> packetToByteArray() {
            if (path.empty() || (path.size() % 2))
                return {};

            std::vector<uint8_t> num16(2, 0);
            std::vector<uint8_t> num32(2, 0);
            std::vector<uint8_t> byteArray(path.size() + (data.empty() ? 0 : data.size()) + 18, 0);

            transformU8(itemCount, num16);
            std::copy(num16.begin(), num16.end(), byteArray.begin() + 6);

            transformU8(static_cast<uint16_t>(itemID), num16);
            std::copy(num16.begin(), num16.end(), byteArray.begin() + 12);

            transformU8(static_cast<uint16_t>(2 + path.size() + (data.empty() ? 0 : data.size())), num16);
            std::copy(num16.begin(), num16.end(), byteArray.begin() + 14);

            byteArray[16] = service;
            byteArray[17] = path.size() >> 1;

            std::copy(path.begin(), path.end(), byteArray.begin() + 18);
            if (!data.empty())
                std::copy(data.begin(), data.end(), byteArray.begin() + 18 + static_cast<int>(path.size()));

            return byteArray;
        }
    };

}

#endif //SERVERDEVICE_ETHERNET_ETHERNETIPTYPES_H
