#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "vartype.h"

#ifdef _WIN32
#    ifdef VAR_EXPORT
#        define VAR_API __declspec(dllexport)
#    else
#        define VAR_API __declspec(dllimport)
#    endif
#else
#   define VAR_API __attribute__((visibility("default")))
#endif

#ifndef VARMANAGER_DYNAMICLOAD

// variable

/**
 * \brief Get variable name
 * \param[in] con use variable's container to determine whether the variable has been deleted,to prevent a large number of find operations.
 * \param[in] var variable
 * \param[out] name variable name return,max 64byte
 * \return libtype, non-zero is error
 */
VAR_API int32_t get_value_name(void *con, void *var, char *name);

/**
 * \brief Get variable type
 * \param[in] con use variable's container to determine whether the variable has been deleted
 * \param[in] var variable
 * \param[out] type variable type return dbl,str,wave...
 * \return libtype, non-zero is error
 */
VAR_API int32_t get_value_type(void *con, void *var, vartype *type);

/**
 * \brief Get variable value
 * @param con use variable's container to determine whether the variable has been deleted
 * @param var variable
 * @param value double value of variable
 * @return libtype, non-zero is error
 */
VAR_API int32_t get_value_dbl(void *con, void *var, double *value);

VAR_API int32_t get_value_str(void *con, void *var, char *value);

VAR_API int32_t get_value_wave(void *con, void *var, double *value, int32_t *value_max_len);

VAR_API int32_t get_value_time(void *con, void *var, lvtime *time);

VAR_API int32_t set_value_dbl(void *con, void *var, double value, lvtime *time, int8_t *changed, int32_t source);

VAR_API int32_t set_value_str(void *con, void *var, const char *value, lvtime *time, int8_t *changed, int32_t source);

VAR_API int32_t set_value_wave(void *con, void *var, const double *value, int32_t value_len, lvtime *time, int8_t *changed, int32_t source);

VAR_API bool is_empty_var(void *var);

VAR_API bool is_valid_var(void *var);

// container

VAR_API int32_t create_container(void **container);

VAR_API int32_t create_sub_container(void **sub_container, void *con, const char *names);

VAR_API int32_t free_container(void *con);

VAR_API int32_t get_var(void *con, const char *name, vartype type, uint8_t create, void **variable);

VAR_API int32_t remove_var(void *con, const char *name);

VAR_API int32_t con_get_value_type(void *con, const char *name, vartype *type);

VAR_API int32_t con_get_value_dbl(void *con, const char *name, double *value);

VAR_API int32_t con_get_value_str(void *con, const char *name, char *value);

VAR_API int32_t con_get_value_wave(void *con, const char *name, double *value, int32_t *value_max_len);

VAR_API int32_t con_get_value_time(void *con, const char *name, lvtime *time);

VAR_API int32_t con_set_value_dbl(void *con, const char *name, double value, lvtime *time, int8_t *changed, int32_t source);

VAR_API int32_t con_set_value_str(void *con, const char *name, const char *value, lvtime *time, int8_t *changed, int32_t source);

VAR_API int32_t con_set_value_wave(void *con, const char *name, const double *value, int32_t value_len, lvtime *time, int8_t *changed, int32_t source);

// event

VAR_API int32_t con_add_event(void *con, const char *names, uint8_t allow_empty, int32_t *id);

VAR_API int32_t con_remove_event(void *con, int32_t id);

VAR_API int32_t add_event(void *con, void *var, int32_t *id);

VAR_API int32_t remove_event(void *con, void *var, int32_t id);

VAR_API int32_t con_add_callback(void *con, const char *names, uint8_t allow_empty, int32_t *id, void(*p)(event_data_t, void *data), void *data);

VAR_API int32_t con_remove_callback(void *con, int32_t id);

VAR_API int32_t add_callback(void *con, void *var, int32_t *id, void(*p)(event_data_t, void *data), void *data);

VAR_API int32_t remove_callback(void *con, void *var, int32_t id);

VAR_API int32_t event_add_names(int32_t id, const char *names);

VAR_API int32_t event_remove_names(int32_t id, const char *names);

VAR_API int32_t wait_event(int32_t id, int32_t timeout_ms, char *name, vartype *type, lvtime *time, double *value_dbl, char *value_str, double *value_wave, int *wave_len, int32_t *source);

VAR_API int32_t try_get_event(int32_t id, char *name, vartype *type, lvtime *time, double *value_dbl, char *value_str, double *value_wave, int *wave_len, int32_t *source);

VAR_API int32_t clear_event(int32_t id);

VAR_API int32_t get_event_size(int32_t id, uint32_t *size);

VAR_API int32_t close_event(int32_t id);

// ramp

VAR_API int32_t start_ramp(void *con, const char *name, double value, double time, double freq);

VAR_API int32_t start_ramps(void *con, const char *names, double *value, int len, double time, double freq);

VAR_API int32_t ramp(void *con, const char *name, double value, double time, double freq);

VAR_API int32_t ramps(void *con, const char *names, double *value, int len, double time, double freq);

VAR_API int32_t stop_ramp(void *con, const char *name);

VAR_API int32_t ramp_status(void *con, const char *name, int8_t *running);

// debug
VAR_API int32_t get_version();

VAR_API int32_t get_time(lvtime *time);

VAR_API int32_t say_hello();

#else

#endif

#ifdef __cplusplus
}
#endif