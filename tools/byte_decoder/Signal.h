#pragma once

#include <string>

enum class ByteOrder
{
    BigEndian = 0,
    LittleEndian = 1
};

enum class DataType
{
    Default = 0,
    Bool = 1,
    Signed = 2,
    UnSigned = 3,
    Float = 4,
    Double = 5
};

enum class SignalType
{
    Static = 0,
    Multiplexer = 1,
    Multiplexed = 2
};

struct Signal
{
    std::string name;
    ByteOrder byteOrder = ByteOrder::LittleEndian;
    DataType dataType = DataType::Default;
    int startBit = 0;
    int numOfBits = 0;
    double gain = 1;
    double offset = 0;
    double maxValue = 1;
    double minValue = 0;
    double defaultValue = 0;
    SignalType signalType = SignalType::Static;
    int modeValue = -1;
};
