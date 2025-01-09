//
// Created by 79933 on 2024/11/25.
//

#ifndef SERVERDEVICE_ETHERNET_ETHERNETIPTYPESUPPORT_H
#define SERVERDEVICE_ETHERNET_ETHERNETIPTYPESUPPORT_H

#include <memory>
#include <vector>
#include "EtherNetIPTypes.h"
#include "../tools/byte_decoder/Signal.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <rapidjson/error/en.h>

#define get_json_string(x) x.IsNull() ? "" : x.GetString()

namespace EtherNetIP {
    std::shared_ptr<DeviceConfig> parseConfig(const std::string &config) {
        rapidjson::Document doc1;
        doc1.Parse<rapidjson::kParseNanAndInfFlag>(config.c_str());
        if (doc1.HasParseError())
            return nullptr;
        const rapidjson::Value &doc = doc1["DeviceConfig"];
        auto cfg = std::make_shared<DeviceConfig>();

        cfg->IPAddress = get_json_string(doc["IPAddress"]);
        cfg->udpPort = doc["UdpPort"].GetInt();
        const rapidjson::Value &remoteDevice = doc["RemoteDevices"];
        for (rapidjson::SizeType i = 0; i < remoteDevice.Size(); i++) {
            EtherNetIP::RemoteDevice dvc;
            dvc.IPAddress = get_json_string(remoteDevice[i]["IPAddress"]);
            dvc.tcpPort = remoteDevice[i]["TcpPort"].GetInt();
            dvc.udpPort = remoteDevice[i]["UdpPort"].GetInt();
            dvc.cycleTime = remoteDevice[i]["CycleTime"].GetInt();
            dvc.timeoutTicks = remoteDevice[i]["TimeoutTicks"].GetInt();
            dvc.name = get_json_string(remoteDevice[i]["Name"]);
            dvc.description = get_json_string(remoteDevice[i]["Description"]);
            dvc.watchdog = get_json_string(remoteDevice[i]["Watchdog"]);
            dvc.type = remoteDevice[i]["Type"].GetInt();
            dvc.typeVar = get_json_string(remoteDevice[i]["TypeVar"]);
            const rapidjson::Value &EIPConfig = remoteDevice[i]["EIPConfig"];
            dvc.EIPConfig.classID = EIPConfig["ClassID"].GetInt();
            dvc.EIPConfig.instanceID = EIPConfig["InstanceID"].GetInt();
            dvc.EIPConfig.attributeID = EIPConfig["AttributeID"].GetInt();
            const rapidjson::Value &EIPO2T = remoteDevice[i]["EIPO2T"];
            dvc.EIPO2T.classID = EIPO2T["ClassID"].GetInt();
            dvc.EIPO2T.instanceID = EIPO2T["InstanceID"].GetInt();
            dvc.EIPO2T.attributeID = EIPO2T["AttributeID"].GetInt();
            const rapidjson::Value &EIPT2O = remoteDevice[i]["EIPT2O"];
            dvc.EIPT2O.classID = EIPT2O["ClassID"].GetInt();
            dvc.EIPT2O.instanceID = EIPT2O["InstanceID"].GetInt();
            dvc.EIPT2O.attributeID = EIPT2O["AttributeID"].GetInt();
            dvc.configBytes = remoteDevice[i]["ConfigBytes"].GetInt();
            dvc.O2TBytes = remoteDevice[i]["O2TBytes"].GetInt();
            dvc.T2OBytes = remoteDevice[i]["T2OBytes"].GetInt();
            const rapidjson::Value &signal = remoteDevice[i]["Signals"];
            for (rapidjson::SizeType j = 0; j < signal.Size(); j++) {
                EtherNetIP::signal sin;
                sin.signalType = signal[j]["SignalType"].GetInt();
                sin.name = get_json_string(signal[j]["Name"]);
                sin.varName = get_json_string(signal[j]["VarName"]);
                sin.byteOrder = signal[j]["ByteOrder"].GetInt();
                sin.dataType = signal[j]["DataType"].GetInt();
                sin.startBit = signal[j]["StartBit"].GetInt();
                sin.numberOfBits = signal[j]["NumberOfBits"].GetInt();
                sin.gain = signal[j]["Gain"].GetDouble();
                sin.offset = signal[j]["Offset"].GetDouble();
                sin.maxValue = signal[j]["MaxValue"].GetDouble();
                sin.minValue = signal[j]["MaxValue"].GetDouble();
                sin.defaultValue = signal[j]["DefaultValue"].GetDouble();
                sin.description = get_json_string(signal[j]["Description"]);
                dvc.signals.push_back(sin);
            }
            cfg->remoteDevice.push_back(dvc);
        }
        return cfg;
    }

    std::shared_ptr<DeviceConfig> parseConfig(const std::string &config, int &errorCode) {
        try {
            auto cfg = parseConfig(config);
            if (cfg) return cfg;
            errorCode = DEVICE_CONFIG_PARSE_ERROR;
            return std::make_shared<EtherNetIP::DeviceConfig>();
        } catch (...) {
            errorCode = DEVICE_CONFIG_PARSE_ERROR;
            return std::make_shared<EtherNetIP::DeviceConfig>();
        }
    }

    std::string byteOrderToString(ByteOrder byteOrder){
        return (byteOrder == ByteOrder::BigEndian) ? "BigEndian" : "LittleEndian";
    }

    std::string dataTypeToString(DataType dataType){
        switch(dataType){
            case DataType::Default : return "Default";
            case DataType::Bool : return "Bool";
            case DataType::Signed : return "Signed";
            case DataType::UnSigned : return "UnSigned";
            case DataType::Float : return "Float";
            case DataType::Double : return "Double";
            default : return "Default";
        }
    }

    std::string signalTypeToString(SignalType signalType){
        switch(signalType){
            case SignalType::Static : return "Static";
            case SignalType::Multiplexer : return "Multiplexer";
            case SignalType::Multiplexed : return "Multiplexed";
            default : return "Unknown";
        }
    }

    std::string signalVectorToJSON(const std::vector<Signal> &signals){
        rapidjson::Document doc;
        doc.SetArray();
        rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();
        for (const auto &signal : signals) {
            rapidjson::Value signalObj(rapidjson::kObjectType);
            signalObj.AddMember("name", rapidjson::Value().SetString(signal.name.c_str(), allocator), allocator);
            signalObj.AddMember("byteOrder", rapidjson::Value().SetInt(static_cast<int>(signal.byteOrder)), allocator);
            signalObj.AddMember("dataType", rapidjson::Value().SetInt(static_cast<int>(signal.dataType)), allocator);
            signalObj.AddMember("startBit", rapidjson::Value().SetInt(signal.startBit), allocator);
            signalObj.AddMember("numOfBits", rapidjson::Value().SetInt(signal.numOfBits), allocator);
            signalObj.AddMember("gain", rapidjson::Value().SetDouble(signal.gain), allocator);
            signalObj.AddMember("offset", rapidjson::Value().SetDouble(signal.offset), allocator);
            signalObj.AddMember("maxValue", rapidjson::Value().SetDouble(signal.maxValue), allocator);
            signalObj.AddMember("minValue", rapidjson::Value().SetDouble(signal.minValue), allocator);
            signalObj.AddMember("defaultValue", rapidjson::Value().SetDouble(signal.defaultValue), allocator);
            signalObj.AddMember("signalType", rapidjson::Value().SetInt(static_cast<int>(signal.signalType)), allocator);
            signalObj.AddMember("modeValue", rapidjson::Value().SetInt(signal.modeValue), allocator);
            doc.PushBack(signalObj, allocator);
        }
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        return buffer.GetString();
    }
}

#endif //SERVERDEVICE_ETHERNET_ETHERNETIPTYPESUPPORT_H
