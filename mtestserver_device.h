/**
 * @file mtestserver_device.h
 * @author Julles
 * @brief Define API for mtestserver device.
 * @version 1.0
 * @date 2024-08-01
 *
 * @copyright ITIMotor Copyright (c) 2024
 *
 * @History
 * @author Julles
 * @date 2024-08-08
 * @version 1.1
 * @Update Add set_name and get_name function
 *          change new_device function add lic and function
 *
 * @author Julles
 * @date 2024-08-20
 * @version 1.2
 * @Update Add get_write_variables function, remove get_error and set_parameter function,add comment
 */

#ifndef MTESTSERVER_DEVICE_H
#define MTESTSERVER_DEVICE_H

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

#define MAX_VERSION_LEN 64

//API ERROR CODE
#define DEVICE_NO_ERROR 0
#define DEVICE_HANDLE_ERROR (-1)
#define DEVICE_LACK_MEMORY (-2)

//Check null pointer and returns a handle error
#define CHECK_HANDLE(x) if(x == nullptr) return DEVICE_HANDLE_ERROR

/**
 * Create a new device based on parameters such as configuration strings, variables, container pointers, provided by the caller
 * @param name the device name, which is often used to inform the device status
 * @param lic an activation code, usually a JSON string, is used to decide whether or not to allow the use of this device or some of the features in this device
 * @param config a configuration string, usually a JSON string, indicates all the configuration information required to createChannel a new device
 * @param con a pointer to a variable container, through which variables are read and written
 * @param func a function container pointer that allows you to call other device functions
 * @param device device pointer, return new device pointer
 * @return error code
 */
DEVICE_API int32_t new_device(const char *name, const char *lic, const char *config, void *con, void *func, void **device);

/**
 * In the execution step, the caller usually calls the get_timing_loop_info to obtain the timing information and calls this function in a loop according to the time interval
 * @param device device pointer
 * @return error code
 */
DEVICE_API int32_t do_step(void *device);

/**
 * Perform the device method
 * @param device device pointer
 * @param name the name of the method
 * @param data method data
 * @param timeout timeout milliseconds
 * @param result the data returned by the method
 * @param max_len the maximum length of data returned by the method
 * @return error code
 */
DEVICE_API int32_t do_function(void *device, const char *name, const char *data, int32_t timeout, char *result, int32_t *max_len);

/**
 * This function returns timing information and is used to call do_step function at regular intervals
 * @param device device pointer
 * @param freq timing frequency
 * @param processor Processor, -2 for automatic, -1 for use of non-strict latency, 0-255 for CPU cores
 * @param priority priority 1-65535, higher values have higher priority
 * @return error code
 */
DEVICE_API int32_t get_timing_loop_info(void *device, double *freq, int32_t *processor, int32_t *priority);

/**
 * Get the names of all the variables needed for this device
 * @param device device pointer
 * @param name all variable names separated by commas
 * @param type the types of all variables separated by commas
 * @param description descriptions of all variables separated by commas
 * @param max_len the maximum length of the string
 * @return error code
 */
DEVICE_API int32_t get_all_variables(void *device, char *name, char *type, char *description, int32_t *max_len);

/**
 * Getting the names of all variables written by this device is typically used to check for possible anomalies where multiple devices are writing the same variable at the same time
 * @param device device pointer
 * @param name all write variable names separated by commas
 * @param type all types of variables are written, separated by commas
 * @param description descriptions of all written variables, separated by commas
 * @param max_len the maximum length of the string
 * @return error code
 */
DEVICE_API int32_t get_write_variables(void *device, char *name, char *type, char *description, int32_t *max_len);

/**
 * Set the device name
 * @param device device pointer
 * @param name the name of the device
 * @return error code
 */
DEVICE_API int32_t set_device_name(void *device, const char *name);

/**
 * Get the device name
 * @param device device pointer
 * @param name the name of the device
 * @param max_len the maximum length of the device name
 * @return error code
 */
DEVICE_API int32_t get_device_name(void *device, char *name, int32_t *max_len);

/**
 * Get the device version
 * @param version device version string such like V1.0.0
 * @return error code
 */
DEVICE_API int32_t get_device_version(char *version);

/**
 * Close the device to release resources
 * @param device device pointer
 * @return error code
 */
DEVICE_API int32_t close_device(void *device);

#ifdef __cplusplus
}
#endif

#endif //MTESTSERVER_DEVICE_H