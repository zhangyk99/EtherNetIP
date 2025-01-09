//
// Created by 79933 on 2024/12/3.
//

#include <iostream>
#include <chrono>
#include <thread>
#include "../mtestserver_device_etherNetIP.cpp"
#include "../tools/varmanager/varmanager.h"
#include "gtest/gtest.h"

//TEST(UDPTest, Test){
//    std::string localip = "127.0.0.2";
//    std::shared_ptr<UDPChannel> udpPtr = std::make_shared<UDPChannel>(localip, 8080);
//    udpPtr->createConnection();
//    int times = 10;
//    std::vector<uint8_t> sentData{'a', 'b', 'c', 'd'};
//    std::string ip = "127.0.0.1";
//    while(times--){
//        std::cout << udpPtr->sendMessage(ip, 8080, sentData, 100) << std::endl;
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
//    std::vector<uint8_t> receivedData;
//    udpPtr->freeConnection();
//}
//
//TEST(UDPTest2, Test){
//    std::string localip = "127.0.0.2";
//    std::shared_ptr<UDPChannel> udpPtr = std::make_shared<UDPChannel>(localip, 8080);
//    udpPtr->createConnection();
//    int times = 10;
//    std::string ip = "127.0.0.1";
//    std::vector<uint8_t> receivedData;
//    uint16_t port;
//    while (times--){
//        if(!udpPtr->receiveMessage(receivedData, ip, &port)){
//            for(auto rData : receivedData)
//                std::cout << rData;
//            std::cout << std::endl;
//        }else{
//            std::cout << "times " << 10 - times <<" is null." << std::endl;
//        }
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
//    udpPtr->freeConnection();
//}

//TEST(TCPTest, Test){
//    std::string localip = "127.0.0.1";
//    std::shared_ptr<TCPChannel> tcpPtr = std::make_shared<TCPChannel>();
//    tcpPtr->createSocket();
//    tcpPtr->createConnection(localip, 8080);
//    std::vector<uint8_t> sentData{'h', 'e', 'l', 'l', 'o'};
//    std::vector<uint8_t> receivedData;
////    std::this_thread::sleep_for(std::chrono::seconds(5));
//    tcpPtr->sendAndReceive(sentData, 100, receivedData);
//    for(auto &i : receivedData)
//        std::cout << i;
//    std::cout << std::endl;
//    tcpPtr->freeConnect();
//}


TEST(MTest, Test){
    void *con{};
    create_container(&con);

    std::string name = "device";
    std::string config = "{\n"
                         "  \"DeviceInfo\": {\n"
                         "    \"DeviceName\": \"EtherNetIP\",\n"
                         "    \"DeviceID\": 0,\n"
                         "    \"DeviceType\": \"EtherNetIP\",\n"
                         "    \"DeviceDes\": \"\",\n"
                         "    \"Version\": 0,\n"
                         "    \"Author\": \"\",\n"
                         "    \"CreateTime\": \"2024-11-25 15:24:34.567\",\n"
                         "    \"Reviser\": \"\",\n"
                         "    \"ModifyTime\": \"2024-12-24 15:49:55.159\"\n"
                         "  },\n"
                         "  \"DeviceConfig\": {\n"
                         //windows "192.168.1.10"
                         //linux   "172.23.155.230"
                         "    \"IPAddress\": \"192.168.1.10\",\n"
                         "    \"UdpPort\": 2222,\n"
                         "    \"RemoteDevices\": [\n"
                         "      {\n"
                         "        \"IPAddress\": \"192.168.1.6\",\n"
                         "        \"TcpPort\": 44818,\n"
                         "        \"UdpPort\": 2222,\n"
                         "        \"CycleTime\": 10,\n"
                         "        \"TimeoutTicks\": 50,\n"
                         "        \"Name\": \"WAGO 363 8DI 8DO\",\n"
                         "        \"Description\": \"WAGO 363 8DI 8DO\",\n"
                         "        \"Watchdog\": \"Var_9\",\n"
                         "        \"Type\": 0,\n"
                         "        \"TypeVar\": \"Var_8\",\n"
                         "        \"EIPConfig\": {\n"
                         "          \"ClassID\": 4,\n"
                         "          \"InstanceID\": 0,\n"
                         "          \"AttributeID\": 3\n"
                         "        },\n"
                         "        \"EIPO2T\": {\n"
                         "          \"ClassID\": 4,\n"
                         "          \"InstanceID\": 101,\n"
                         "          \"AttributeID\": 3\n"
                         "        },\n"
                         "        \"EIPT2O\": {\n"
                         "          \"ClassID\": 4,\n"
                         "          \"InstanceID\": 104,\n"
                         "          \"AttributeID\": 3\n"
                         "        },\n"
                         "        \"ConfigBytes\": 2,\n"
                         "        \"O2TBytes\": 1,\n"
                         "        \"T2OBytes\": 2,\n"
                         "        \"Signals\": [\n"
                         "          {\n"
                         "            \"SignalType\": 1,\n"
                         "            \"Name\": \"DI0\",\n"
                         "            \"VarName\": \"Var_0\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 0,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 1,\n"
                         "            \"Name\": \"DI1\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 1,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 1,\n"
                         "            \"Name\": \"DI2\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 2,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 1,\n"
                         "            \"Name\": \"DI3\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 3,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 1,\n"
                         "            \"Name\": \"DI4\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 4,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 1,\n"
                         "            \"Name\": \"DI5\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 5,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 1,\n"
                         "            \"Name\": \"DI6\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 6,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 1,\n"
                         "            \"Name\": \"DI7\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 7,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 2,\n"
                         "            \"Name\": \"DO0\",\n"
                         "            \"VarName\": \"Var\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 0,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 2,\n"
                         "            \"Name\": \"DO1\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 1,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 2,\n"
                         "            \"Name\": \"DO2\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 2,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 2,\n"
                         "            \"Name\": \"DO3\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 3,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 2,\n"
                         "            \"Name\": \"DO4\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 4,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 2,\n"
                         "            \"Name\": \"DO5\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 5,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 2,\n"
                         "            \"Name\": \"DO6\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 6,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          },\n"
                         "          {\n"
                         "            \"SignalType\": 2,\n"
                         "            \"Name\": \"DO7\",\n"
                         "            \"VarName\": \"\",\n"
                         "            \"ByteOrder\": 1,\n"
                         "            \"DataType\": 1,\n"
                         "            \"StartBit\": 7,\n"
                         "            \"NumberOfBits\": 1,\n"
                         "            \"Gain\": 1.0,\n"
                         "            \"Offset\": 0.0,\n"
                         "            \"MaxValue\": 0.0,\n"
                         "            \"MinValue\": 0.0,\n"
                         "            \"DefaultValue\": 0.0,\n"
                         "            \"Description\": \"\"\n"
                         "          }\n"
                         "        ]\n"
                         "      }\n"
                         "    ]\n"
                         "  }\n"
                         "}";

    void *dvc{};
    double value{};
    char Name[100]{'\0'};
    char type[10]{'\0'};
    char description[100]{'\0'};
    int32_t max_len[100]{100, 100, '\0'};
    new_device(name.c_str(), nullptr, config.c_str(), con, nullptr, &dvc);

    get_all_variables(dvc, Name, type, description, max_len);
    std::cout << Name <<std::endl;
    std::cout << description << std::endl;


    for(int i = 0; i < 1000; i++) {
        do_step(dvc);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    close_device(dvc);
}


TEST(EtherNetIPTest,test){
    std::string localIP{"192.168.1.10"};
    std::string targetIP{"192.168.1.6"};
    uint16_t tcpPort = 44818;
    uint16_t udpPort = 2222;

    auto tcpPtr = std::make_shared<TCPChannel>();
    tcpPtr->createSocket();
    tcpPtr->createConnection(targetIP, tcpPort);

    std::vector<uint8_t> sendSessionHandle{0x65, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
    std::vector<uint8_t> recvSessionHandle{};

    std::vector<uint8_t> sendConfig{0x6f, 0x00, 0x18, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x08, 0x00, 0x0e, 0x03, 0x20, 0x04, 0x24, 0x00, 0x30, 0x03};
    std::vector<uint8_t> recvConfig{};

    std::vector<uint8_t> sendO2T{0x6f, 0x00, 0x18, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x08, 0x00, 0x0e, 0x03, 0x20, 0x04, 0x24, 0x65, 0x30, 0x03};
    std::vector<uint8_t> recvO2T{};

    std::vector<uint8_t> sendT2O{0x6f, 0x00, 0x18, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x08, 0x00, 0x0e, 0x03, 0x20, 0x04, 0x24, 0x68, 0x30, 0x03};
    std::vector<uint8_t> recvT2O{};

    std::vector<uint8_t> sendOpen{0x6f, 0x00, 0x42, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x32, 0x00, 0x54, 0x02, 0x20, 0x06, 0x24, 0x01, 0x0a, 0x32,
                                  0x1d, 0x69, 0xbf, 0x0c, 0x1e, 0x69, 0xbf, 0x0c, 0xdd, 0x36, 0xda, 0xfa, 0x0d, 0xf0, 0xad, 0x8b,
                                  0x00, 0x00, 0x00, 0x00, 0xa0, 0x86, 0x01, 0x00, 0x07, 0x46, 0xa0, 0x86, 0x01, 0x00, 0x04, 0x40,
                                  0x01, 0x04, 0x20, 0x04, 0x24, 0x00, 0x2c, 0x65, 0x2c, 0x68};
    std::vector<uint8_t> recvOpen{};

    std::vector<uint8_t> sendClose{0x6f, 0x00, 0x2a, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x1a, 0x00, 0x4e, 0x02, 0x20, 0x06, 0x24, 0x01, 0x0a, 0x32,
                                   0xdd, 0x36, 0xda, 0xfa, 0x0d, 0xf0, 0xad, 0x8b, 0x04, 0x00, 0x20, 0x04, 0x24, 0x00, 0x2c, 0x65,
                                   0x2c, 0x68};
    std::vector<uint8_t> recvClose{};

    std::vector<uint8_t> sendUDP{0x02, 0x00, 0x02, 0x80, 0x08, 0x00, 0x01, 0x00, 0x49, 0x80, 0x00, 0x00, 0x00, 0x00, 0xb1, 0x00,
                                 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
    std::vector<uint8_t> recvUDP{};

    tcpPtr->sendAndReceive(sendSessionHandle, 100, recvSessionHandle);
    tcpPtr->sendAndReceive(sendConfig, 100, recvConfig);
    tcpPtr->sendAndReceive(sendO2T, 100, recvO2T);
    tcpPtr->sendAndReceive(sendT2O, 100, recvT2O);
    tcpPtr->sendAndReceive(sendOpen, 100, recvOpen);

    auto udpPtr = std::make_shared<UDPChannel>(localIP, udpPort);
    udpPtr->createConnection();


    std::vector<uint8_t> recvMessage;
    std::string recvIP{};
    uint16_t recvPort;
    for(int i = 0; i < 10000; i++){
        sendUDP[10] = i;
        sendUDP[18] = i;
//        udpPtr->sendMessage(targetIP, udpPort, sendUDP);
//        udpPtr->receiveMessage(recvMessage, recvIP, &recvPort);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    tcpPtr->sendAndReceive(sendClose, 100, recvClose);


    tcpPtr->freeConnect();
    udpPtr->freeConnection();
}
