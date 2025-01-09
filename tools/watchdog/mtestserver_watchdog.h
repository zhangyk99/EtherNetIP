/**
 * @file mtestserver_watchdog.h
 * @author Julles
 * @brief  watchdog api for mtest
 * @version 1.0
 * @date 2024-10-28
 */


#ifndef MTESTSERVER_WATCHDOG_H
#define MTESTSERVER_WATCHDOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef _WIN32
#   ifdef MTESTSERVER_WATCHDOG_EXPORT
#       define WATCHDOG_API __declspec(dllexport)
#   else
#       define WATCHDOG_API __declspec(dllimport)
#   endif
#else
#   define WATCHDOG_API __attribute__ ((visibility("default")))
#endif

//watchdog
WATCHDOG_API int32_t wd_create(const char* name, int32_t accuracy, void** wdv);

WATCHDOG_API int32_t wd_remove(void* wdv);

WATCHDOG_API int32_t wd_read_time(void *wdv, uint32_t *timeCount);

WATCHDOG_API int32_t wd_reset(void *wdv, uint8_t reset);

WATCHDOG_API int32_t wd_read_name(void *wdv, char *name);

WATCHDOG_API int32_t wd_read_accuracy(void *wdv, uint32_t *accuracy);

//watchdogVarDaemon
WATCHDOG_API int32_t wd_init(void *con);

WATCHDOG_API int32_t wd_free();

WATCHDOG_API int32_t wd_write_var();

WATCHDOG_API int32_t wd_get_size(uint32_t *size);

WATCHDOG_API void wd_say_hello();

#ifdef  __cplusplus
}
#endif

#endif //MTESTSERVER_WATCHDOG_H
