EtherNet/IP c++代码 按照Labview代码改写，函数名称和功能基本保持不变。

在UDP接收I/O消息部分，使用std::function代替Labview的队列接收。

EtherNetIP是基于Ether的CIP通讯协议

通讯中有两种消息：
> 显示消息（TCP）
> I/O消息（UDP）

消息帧结构参考：https://blog.csdn.net/m0_46577050/article/details/120898683

一般通讯流程：
1、Register Session 注册会话 执行1次（使用显示消息）
2、Assembly - Get Attribute Single 配置config、O2T、T2O信号  执行3次 (使用显示消息)
3、Forward Open 打开Forward 执行一次（使用显示消息）
4、发送、接收I/O消息
5、Forward Close 关闭Forward 执行一次（使用显示消息）




