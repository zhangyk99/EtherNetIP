#pragma once

#include <cstdint>
#include <vector>

#include "Signal.h"

struct Frame{
    uint32_t identifier;    //帧ID
    bool extended;          //扩展帧
    int dataLength;         //帧长度
    uint8_t type;           //帧类型
    std::vector<Signal> signals;    //帧信号
};
