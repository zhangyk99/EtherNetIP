#pragma once

#include <cstdint>

struct lvtime
{
    uint64_t time_us = 0;
    uint64_t time_s = 0;
};

enum vartype : uint8_t
{
    vartype_dbl = 0,
    vartype_str = 1,
    vartype_wave = 2
};

typedef struct {
    vartype type;
    lvtime time;
    char name[64];
    double value_dbl;
    double *value_wave;
    int32_t wave_len;
    char *value_str;
    int32_t str_len;
    int32_t source;                 //Indicates the writer, typically used to determine the source of the event trigger
} event_data_t;