//
// Created by 79933 on 2024/12/23.
//

#ifndef SERVERDEVICE_ETHERNET_MTESTSERVER_ERROR_CODE_H
#define SERVERDEVICE_ETHERNET_MTESTSERVER_ERROR_CODE_H

#define MTESTSERVER_OK 0

//–8999至–8000、5000至9999或500000至599999 范围内的自定义错误代码

//其他内置错误码

//通用错误码 500000~500499
#define DEVICE_CONFIG_PARSE_ERROR   500000  //设备配置解析错误！
#define DEVICE_CONFIG_ERROR         500001  //设备配置错误！
#define DEVICE_NOT_FOUND            500002  //设备没有找到指定资源！
#define DEVICE_NOT_SUPPORT          500003  //设备不支持指定操作!
#define DEVICE_NOT_IMPLEMENTED      500004  //设备没有实现指定操作！
#define DEVICE_NOT_INITIALIZED      500005  //设备没有初始化！
#define DEVICE_NOT_CONNECTED        500006  //设备没有连接！
#define DEVICE_NOT_READY            500007  //设备没有准备好！
#define DEVICE_CONNECT_ERROR        500008  //设备连接错误！
#define DEVICE_CONFIG_ERROR_FREQ    500009  //设备配置错误，指定的执行频率无效！
#define FUNC_PARAM_ERROR            500010  //函数参数错误！
#define DEVICE_CONFIG_ERROR_FREQ_1  500011  //设备配置错误，指定的执行频率无效！频率必须大于0且小于100！
#define DEVICE_CONFIG_ERROR_FREQ_2  500012  //设备配置错误，指定的执行频率无效！频率必须大于0且小于1000！
#define DEVICE_EMPTY_DATA           500013  //设备配置错误，通道或数据为空！
#define DEVICE_TYPE_ERROR           500014  //设备配置错误，未知设备类型！
#define DEVICE_EMPTY_VAR            500015  //设备配置错误，存在空变量！变量必须被正确链接！

//变量 500500~500999
#define VAR_NOT_FOUND               501001  //变量没有找到！
#define VAR_REGISTER_ERROR          501002  //变量注册失败！
#define VAR_CREATE_ERROR            501003  //变量创建失败！
#define VAR_UNREGISTER_ERROR        501004  //变量取消注册失败！
#define VAR_DUPLICATE_NAME          501005  //变量名重复！

//脚本 501000~501499

//EtherCAT+EtherNetIP 501500~501999
#define EtherCat                    501500  
#define EIP_IP_ADDRESS_ERROR        501501  //IP地址有误
#define EIP_CONFIG_READ_FAILED      501502  //配置读取失败
#define EIP_CONFIG_BYTE_ERROR       501503  //配置字节错误
#define EIP_OUTPUT_READ_FAILED      501504  //输出读取失败
#define EIP_OUTPUT_BYTE_ERROR       501505  //输出字节错误
#define EIP_OUTPUT_BYTE_NULL        501506  //输出字节为空
#define EIP_INPUT_READ_FAILED       501507  //输入读取失败
#define EIP_INPUT_BYTE_ERROR        501508  //输入字节错误
#define EIP_CONNECT_ERROR           501509  //连接失败

//Recorder+PML+ChannelList 502000~502499

//Limit+报警监控相关 502500~502999
#define LIMITS_VALUE_SORT_ERROR     502501  //报警值错误，必须从小到大排列！
#define LIMITS_AUTO_RESET_THRESHOLD 502502  //自动复位时，阈值必须大于0！
#define LIMITS_VAR_EMPTY            502503  //报警变量名为空或无效！
#define LIMITS_VAR_NOT_FOUND        502504  //报警变量没有找到！
#define LIMITS_IGNORE_TIME_ERROR    502505  //报警忽略时间必须大于等于0！
#define LIMITS_RECOVER_ACTION_ERROR 502506  //报警恢复动作错误，不能为停机！
#define LIMITS_ID_OUT_OF_RANGE      502507  //报警ID数量超出范围！
#define LIMITS_CONFIG_ERROR_FREQ    502508  //执行频率超出范围，允许取值范围0.1hz-1000hz
#define LIMITS_ACTION_NOT_FOUND     502509  //报警动作错误，没有找到指定的报警动作！

//Test 工况相关 503000~503499

//CAN/LIN 503500~503999
#define CAN_NOT_FOUND               800001  //没有找到匹配序列号的设备，请检查是否插入设备，或拔插设备后重试。
#define CAN_OPEN_DEVICE_ERROR       800002  //打开设备失败。
#define CAN_GET_MESSAGE_ERROR       800003  //获取消息失败。
#define CAN_SEND_MESSAGE_ERROR      800004  //发送消息失败。
#define CAN_SET_BITRATE_ERROR       800005  //设置波特率失败。
#define CAN_GET_BITRATE_ERROR       800006  //获取波特率失败。
#define CAN_SET_FD_BITRATE_ERROR    800007  //设置FD波特率失败。
#define CAN_GET_FD_BITRATE_ERROR    800008  //获取FD波特率失败。
#define CAN_CLOSE_DEVICE_ERROR      800009  //关闭设备失败。
#define CAN_TERM_NOT_SUPPORT        800010  //该设备不支持设置终端电阻。
#define CAN_APPLY_CONFIG_ERROR      800011  //写入配置失败。其他通道在使用中时有几率写入失败，可能需要重新加载项目。
#define CAN_READ_CONFIG_ERROR       800012  //读取配置失败。
#define CAN_SET_TERM_ERROR          800013  //启用终端电阻失败。
#define CAN_LIB_NOT_FOUND           800014  //没有找到库文件。
#define CAN_NOT_SUPPORT_VERSION     800015  //该版本不支持该函数。
#define CAN_UNKNOWN_ERROR           800016  //未知错误。
#define CAN_HW_SEND_NOT_SUPPORT     800017  //该设备不支持硬件定时发送模式。

//CANOpen 504000~504499

//XCP 504500~504999

//Edcon 505000~505499
#define EDCON_DYNO_POWER_ON_TIMEOUT 800101  //测功机上电失败！
#define EDCON_DYNO_POWER_OFF        800102  //测功机异常断电！
#define EDCON_IO_CONFIG_ERROR       800103  //IO配置错误！
#define EDCON_START_TIMEOUT         800104  //启动失败！
#define EDCON_CLOSE_LOOP_WARING     800105  //非实时系统，不支持闭环转速模式和闭环扭矩模式！
#define EDCON_NOT_READY             800106  //测功机没有上电！
#define EDCON_SAFE_SPEED_LIMIT      800107  //安全转速超限！
#define EDCON_SPEED_LIMIT           800108  //转速超限！
#define EDCON_TORQUE_LIMIT          800109  //扭矩超限！
#define EDCON_POWER_LIMIT           800110  //功率超限！
#define EDCON_DYNO_WARNING          800111  //测功机发生警告！
#define EDCON_DYNO_ERROR            800112  //测功机发生错误！
#define EDCON_DYNO_RUNNING_ERROR    800113  //测功机运行异常！

//Filter 505500~505749
#define FILTER_CUTOFF_ERROR         505501  //截止频率参数错误，截止频率必须大于0且小于采样频率的一半！
#define FILTER_CUTOFF_ORDER_ERROR   505502  //截止频率参数错误，低截止频率必须小于高截止频率！
#define FILTER_ORDER_ERROR_1        505503  //滤波阶数参数错误，阶数必须大于2且为2的倍数！
#define FILTER_ORDER_ERROR_2        505504  //滤波阶数参数错误，阶数必须大于4且为4的倍数！
#define FILTER_LAG_FACTOR_ERROR     505505  //一阶滞后滤波系数错误，滤波系数必须在0-1之间！

//Map505750~505999
#define MAP_FINDUP_SLOPE            505750  //设备配置错误，在斜坡模式下，不允许使用向上查找！
#define MAP_TWO_INPUT_SLOPE         505751  //设备配置错误，在斜坡模式下，不允许超过两个输入！
#define MAP_FILE_READ_ERROR         505752  //MAP文件读取错误！
#define MAP_UNKNOWN_TYPE            505753  //设备配置错误，未知Map类型！
#define MAP_FILE_NOT_FOUND          505754  //设备配置错误，Map文件为空或不存在！


//506000~506499

//506500~506999

//507000~507499

//507500~507999

//508000~508499

//508500~508999

//509000~509499

//509500~509999

//510000~510499

//510500~510999

//511000~511499

//511500~511999

#endif //SERVERDEVICE_ETHERNET_MTESTSERVER_ERROR_CODE_H
