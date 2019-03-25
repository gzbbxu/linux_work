### 命令：netstat -na | grep 8001 netstat -na | grep tcp
###实验 
1,启动服务器，启动客户端。
2，在服务器中ctrl+c 再重启，会起不来。（客户端不关）这个时候需要地址复用。
### socket api 中的地址复用。
1,服务器端尽可能使用SO_REUSEADDR
2,在绑定之前尽可能调用setsockopt 来设置 SO_REUSEADDR 套接字选项。
3,使用SO_REUSEADDR 选项可以使得不必等待TIME_WAIT 状态消失就可重连服务器。
### 服务器支持多并发
1,
