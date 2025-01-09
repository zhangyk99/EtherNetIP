#ifndef SERVER_LOGGER_EXT_H
#define SERVER_LOGGER_EXT_H

#include <string>
#include "server_logger.h"

#ifndef LOG_FROM
#define LOG_FROM ""
#endif

#define ADD_MESSAGE_DEVICE_NAME(n,m) (n.empty() ? m : "\"" + n + "\" : " + m).c_str()

inline void msg_trace(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(trace, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_debug(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(debug, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_info(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(info, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_warn(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(warning, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_error(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(error, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_fatal(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(fatal, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}


inline void log_trace(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_log(trace, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void log_debug(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_log(debug, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void log_info(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_log(info, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void log_warn(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_log(warning, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void log_error(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_log(error, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void log_fatal(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_log(fatal, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_log_trace(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(trace, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
    server_log(trace, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_log_debug(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(debug, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
    server_log(debug, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_log_info(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(info, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
    server_log(info, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_log_warn(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(warning, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
    server_log(warning, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_log_error(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(error, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
    server_log(error, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

inline void msg_log_fatal(const std::string &message, const std::string &device_name = "", int error_code = 0) {
    server_message(fatal, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
    server_log(fatal, error_code, LOG_FROM, ADD_MESSAGE_DEVICE_NAME(device_name, message));
}

#endif //SERVER_LOGGER_EXT_H
