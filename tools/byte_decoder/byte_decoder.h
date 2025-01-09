#pragma once

#ifdef _WIN32
#    ifdef DECODER_EXPORT
#        define DECODER_API __declspec(dllexport)
#    else
#        define DECODER_API __declspec(dllimport)
#    endif
#else
#   define DECODER_API
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

DECODER_API int32_t get_version();

//signal

DECODER_API void *create_signal_decoder(void *con, const char *signals, uint8_t useDefault, uint8_t useMaxMin);

DECODER_API void free_signal_decoder(void *dec);

/**
 * @brief 对信号进行解码
 * @param[in] dec 解码器指针
 * @param[in] rawData 信号数据
 * @param[in] dataLength 数据长度
 */
DECODER_API void signal_decoder(void *dec, uint8_t *rawData, int32_t dataLength);

/**
 * @brief 对信号进行编码
 * @param[in] dec 解码器指针
 * @param[in,out] rawData 输入输出缓存，要求缓存区中数据初始化为0
 * @param[in,out] dataLength 输入缓存区大小，输出写入的缓存区大小
 */
DECODER_API void signal_encoder(void *dec, uint8_t *rawData, int32_t dataLength);

DECODER_API int32_t signal_get_raw_length(void *dec);


//frame

DECODER_API void *create_frame_decoder(void *con, const char *crc_path, const char *name, const char *tx_frames, const char *rx_frames, uint8_t useDefault, uint8_t useMaxMin);

DECODER_API void free_frame_decoder(void *dec);


/**
 * @brief 对指定ID进行解码
 * @param[in] dec 解码器指针
 * @param[in] id 当前帧ID
 * @param[in] rawData 当前帧的数据
 * @param[in] dataLength 当前帧数据的数量
 * @return 是否已解码
 */
DECODER_API uint8_t frame_decoder(void *dec, uint32_t id, uint8_t *rawData, int32_t dataLength);

/**
 * @brief 对指定ID的帧进行编码
 * @param[in] dec 解码器指针
 * @param[in] id 当前帧ID
 * @param[out] ext 返回是否是远程帧
 * @param[out] type 返回帧类型
 * @param[in,out] rawData 输入输出缓存，要求缓存中数据初始化为0
 * @param[in,out] dataLength 输入缓存区大小，输出写入的缓存区大小
 * @return 是否已编码
 */
DECODER_API uint8_t frame_encoder(void *dec, uint32_t id, uint8_t *ext, uint8_t *type, uint8_t *rawData, int32_t *dataLength);

DECODER_API void frame_get_tx_ids(void *dec, uint32_t *id, int32_t *idLen);

DECODER_API void frame_get_rx_ids(void *dec, uint32_t *id, int32_t *idLen);

DECODER_API int32_t frame_get_data_len(void *dec, uint32_t id);

DECODER_API uint8_t frame_use_crc(void *dec);

#ifdef __cplusplus
}
#endif // __cplusplus