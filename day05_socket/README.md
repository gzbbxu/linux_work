### socket 基本概念
socket 可以看成是用户进程与内核网络协议栈的编程接口
### ipv4 国际套接字接口
 ~~~
 	struct  socketaddr_in{
		uint8_t sin_len; // 4 字节
		sa_familt_t sin_family;// 4字节
		in_port sin_port;// 2字节
		struct in_addr_sin_addr;//4字节
		char size_zero[8];//8 字节
	}
 ~~~
1, sin_len 有的协议里面没有了。man 7 ip查看就没有 
2, sin_family 指定该地址家族，这里必须设定为AF_INET;
3,sin_port 是端口
4,size_zero 暂不使用，一半将其设置为0
### 通用地址结构 一般很少用，用上面的强转
 ~~~
 	struct sockaddr{
		uint8_t sin_len;
		sa_family_t sin_family;
		char sa_data[14] // 这里的14 等于上面的sin_port ，sin_addr,sin_zero	加起来
	}
 ~~~
 1, sin_len 整个结构体长度
 2, sin_family 指定该地址家族
 3, sa_data 由sin_family 决定它的形式

 以上两个结构是通用的。
### socketaddr_in 和 sockaddr 是通用的。
 
### 网络字节
 1，字节序 
  大端字节序，和 小端字节序 。不同的主机有不同的字节序，x86 为小端，motorola大端
  arm 字节序是可配置的。
 2,网络字节序 规定为大端字节序。

### 字节序转换函数
 uint32 htonl(uint32_t hostlong);
 uint16_t hons(uint16_t hostshort);
 uint32_t ntohl(uint32_t netlogn);
 uint16_t ntohs(uint16_t netshort);
 上述函数中，h代表host  n代表network  s代表shot l代表logn; man 2 htonl;
	
### 地址转换函数

1,在tcp/ip 协议里，iPv4 是32bit. === > 192.168.6.222 怎么把  它 转换成32 位数？
int inet_aton(const char * cp ,struct in_adr * inp)		
in_addr_t inet_addr(const char * cp);
char * inet_ntoa(struct in_adrr_in);

### 套接字类型
1,流式套接字（SOCK_SREAM）
提供面向连接的，可靠的数据传输服务，数据无差错，无重复的发送，且按照顺序接受
2，数据报式套接字（SOCK_DGRAM）
提供五连接服务。不提供无措保证，数据可能丢失，或重复，并且接受顺序混乱
3,原始套接字（SOCK_RAW）
### listen  
1，一般来说，listen 函数应该在socket和bind 之后，调用函数accept 之前调用。
2,对于给定的监听套接口，内核要维护两个队列。
	1),已由客户发出并到达服务器，服务器正在等待完成相应的tcp 三路握手过程。
	2),已完成连接队列。
	3) ，这连个队列的和不超过backlog.
### TCP连接创建过程
1,服务器收到客户端SYN包,发送SYN+ACK包后,在内存创建一个状态为SYN_RCVD
 的连接,放入未完成队列,这个队列的大小可通过/proc/sys/net/ipv4/tcp_max_syn_backlog设置.
2,服务器收到客户端的ACK包后,该连接的状态由SYN_RCVD改为ESTABLISHED,并移到已完成队列.
服务器程序调用accept后,该连接移除已完成队列, 由内核交给程序控制
### accept 函数
 功能：从已完成连接队列中返回第一个连接，如果已完成连接队列为空，则阻塞。

