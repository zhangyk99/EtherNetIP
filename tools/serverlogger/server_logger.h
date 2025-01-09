/**
 * @file server_logger.h
 * @author Julles
 * @brief Define API for logger.
 * @version 1.0
 * @date 2024-08-20
 *
 * @copyright ITIMotor Copyright (c) 2024
 *
 * @History
 * @author Julles
 * @date 2024-10-20
 * @version 1.1
 * @Update Change get_message support get log_time
 */

#ifndef MTESTSERVER_LOGGER_H
#define MTESTSERVER_LOGGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

#ifdef _WIN32
#    ifdef DEVICE_EXPORT
#        define DEVICE_API __declspec(dllexport)
#    else
#        define DEVICE_API __declspec(dllimport)
#    endif
#else
#   define DEVICE_API __attribute__((visibility("default")))

#endif

#define MAX_FROM_LEN 64
#define MAX_MESSAGE_LEN 1024

enum log_type : uint8_t {
    trace,
    debug,
    info,
    warning,
    error,
    fatal,
    off
};

struct log_time {
    uint64_t time_us;
    uint64_t time_s;
};

struct log_data {
    uint8_t type;
    int32_t error_code;
    log_time time;
    char from[MAX_FROM_LEN];
    char message[MAX_MESSAGE_LEN];
};

///log
DEVICE_API void server_log(log_type type, int32_t error_code, const char *from, const char *message);

//DEVICE_API void get_server_log(log_type *type, int32_t *error_code, const char *from, char *message, int32_t size);

DEVICE_API void set_log_level(log_type type, const char *from);

DEVICE_API log_type get_log_level(const char *from);

DEVICE_API int32_t add_log_callback(void(*p)(log_data, void *), void* user_data);

DEVICE_API void remove_log_callback(int32_t id);

DEVICE_API void set_log_flush_Time(int32_t time_s);

DEVICE_API void flush_log();

///msg
DEVICE_API void server_message(log_type type, int32_t error_code, const char *from, const char *message);

DEVICE_API void get_server_message(log_type *type, int32_t *error_code, log_time *time, char *from, char *message);

DEVICE_API void wait_server_message(log_type *type, int32_t *error_code, log_time *time, char *from, char *message);

DEVICE_API void wait_server_message_timed(log_type *type, int32_t *error_code, log_time *time, char *from, char *message, int32_t timeout);

DEVICE_API uint32_t get_message_size();

DEVICE_API void set_message_level(log_type type, const char *from);

DEVICE_API log_type get_message_level(const char *from);

DEVICE_API int32_t add_message_callback(void(*p)(log_data, void *), void* user_data);

DEVICE_API void remove_message_callback(int32_t id);

///Init&Unit
DEVICE_API void init_logger(const char *logger_name, const char *filename, int32_t hour, int32_t minute);

DEVICE_API void free_logger(const char *logger_name);

#ifdef __cplusplus
}
#endif

#endif //MTESTSERVER_LOGGER_H