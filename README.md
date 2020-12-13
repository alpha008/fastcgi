# fastcgi
nginx -- webserver -- cgi 


1. cgi
通用网关接口（Common Gateway Interface/CGI） 描述了客户端和服务器程序之间传输数据
的一种标准， 可以让一个客户端， 从网页浏览器向执行在网络服务器上的程序请求数据。
CGI 独立于任何语言的， CGI 程序可以用任何脚本语言或者是完全独立编程语言实现， 只
要这个语言可以在这个系统上运行。 Unix shell script, Python, Ruby, PHP, perl, Tcl, C/C++, 和
Visual Basic 都可以用来编写 CGI 程序

2.公共网关接口
Common Gateway Interface， 简称 CGI
在物理层面上是一段程序， 运行在服务器上， 提供同客户端 HTML 页面的接口

3.浏览器请求流程
web browser  -- http protocol -- httpserver --- cgi program -- database

处理步骤：
1) 通过 Internet 把用户请求送到 Web 服务器
2) Web 服务器接收到用户请求并交给 CGI 程序
3) CGI 程序把处理结果传送给 Web 服务器
4) Web 服务器把结果送回到用户
nginx 结合cgi处理请求
1) web 服务器(ngxin) , 收到一个请求
2) web 服务器 fork 一个子进程
3) 每处理一请求, 都会创建一个子进程
4) 数据处理完成之后, 该 cgi 进程会被 web 服务器杀死
CGI 弊端
缺点：
需要频繁的创建和销毁进程
web 服务器效率低

1. fastcgi
快速通用网关接口（Fast Common Gateway Interface／FastCGI） 是通用网关接口（CGI） 的改
进， 描述了客户端和服务器程序之间传输数据的一种标准。 FastCGI 致力于减少 Web 服务器
与 CGI 程式之间互动的开销， 从而使服务器可以同时处理更多的 Web 请求。 与为每个请求
创建一个新的进程不同， FastCGI 使用持续的进程来处理一连串的请求。 这些进程由 FastCGI
进程管理器管理， 而不是 web 服务器
FastCGI 是语言无关的、 可伸缩架构的 CGI 开放扩展
其主要行为是将 CGI 解释器进程保持在内存中进行管理调度因此获得较高的性能

2.工作流程
1) Web Server 启动时载入 FastCGI 进程管理器
2) FastCGI 进程管理器自身初始化， 启动多个 CGI 解释器进程 并等待来自 Web Server 的连接
3) 当客户端请求到达 Web Server 时， FastCGI 进程管理器选择并连接到一个 CGI 解释器
4) FastCGI 子进程完成处理后将标准输出和错误信息从同一连接返回 Web Server

3.FastCGI 协议
在 FastCGI 中， 每一个 HTTP 请求（或者响应） 消息都分为若干个记录（Record） 进行传递，
每个 Record 又由头部（Header） 和数据（Body） 组成。
使用 Record 进行消息传递的好处：
• 多个请求的数据可以复用同一个连接进行传输，这样应用的实现就可以采用事件驱动的编
程模型或者多线程编程模型以提升效率；
• 同一个请求中的多个数据流的数据可以通过封装成不同记录的形式在同一个连接上传输，
例如 STDOUT 和 STDERR 两个输出流的数据可以通过同一个连接返回给 Web 服务器， 而
不是不得不使用 2 个连接。

FastCGI 协议类型
FastCGI 是二进制连续传递的， 定义了一个统一结构的消息头， 用来读取每个消息的消息体，
方便消息包的切割。 一般情况下，
最先发送的是 FCGI_BEGIN_REQUEST 类型的消息， 然后是 FCGI_PARAMS 和 FCGI_STDIN 类型
的消息，
当 FastCGI 响应处理完后， 将发送 FCGI_STDOUT 和 FCGI_STDERR 类型的消息， 最后以
FCGI_END_REQUEST 表示请求的结束。
FCGI_BEGIN_REQUEST 和 FCGI_END_REQUEST 分别表示请求的开始和结束， 与整个协议相关。

Web 服务器发送 FastCGI 请求时： 依次发送了 3 类 Record， 类型分别为
BEGIN_REQUEST、 PARAMS 和 STDIN
FastCGI 进程返回 FastCGI 响应时： 依次返回了 3 类 Record， 类型分别为
STDOUT、 STDERR、 END_REQUEST
FastCGI 数据包格式
FastCGI 数据包两部分, 头部(header), 包体(body), 每个数据包都必须包含 header, body 可以
没有。 header 为 8 个字节, body 必须为 8 的整数倍, 不是的话需要填充。

FastCGI 配置参数和环境变量
Nginx 基于模块 ngx_http_fastcgi_module 实现通过 fastcgi 协议将指定的客户端请求转发至spawn-fcgi 处理

FastCGI 进程管理器
fastcgi 可使用 spawn-fcgi 或者 php-fpm 来管理 (fastcgi 进程管理器， 有很多不同类型的)
nginx 下 fastcgi 与服务器是分离的

spawn-fcgi 作用？
• 相当一个代理工具
• 角色完成 nginx 和 fastcgi 之间的进程间通信

 nginx 处理不了的指令, 交给 fastcgi 处理
• 数据需要转发
• 数据需要发送到指定的端口
• 处理一个指令 test
• url: http://192.168.52.139/test


 spawn-fcgi 的使用
○ 编写一个 fcgi 程序
编译出来的程序名 test
○ spawn-fcgi -a IP -p 端口 -f fastcgi 程序
参数说明：
-a IP: 服务器 IP 地址
-p port: 服务器将数据发送到的端口
-f cgi 程序: spawn-fcgi 启动的可执行 fastcgi 程序

fastCGI 协议、 Spawn-fcgi、 Nginx 三者关系
FastCGI 协议、 Spawn-fcgi、 Nginx 三者关系
Nginx 是 web 服务器， 只提供 HTTP 协议的输入和输出。
spawn-fcgi 服务器， 只支持 Fastcgi 协议的输入和输出。
它们 2 者直接由 Nginx 将 HTTP 协议转换为 Fastcgi 协议传输给 fastCGI 进程处理

nginx 服务支持 FastCGI 模式， 能够快速高效地处理动态请求。
而 nginx 对应的 FastCGI 模块为 ngx_http_fastcgi_module。
ngx_http_fastcgi_module 模块允许将请求传递给 FastCGI 服务器。
将 http 协议转为 fastcgi 协议


参考地址： https://mp.weixin.qq.com/s?__biz=MzUzMjkwMTU5Mw==&mid=2247483999&idx=1&sn=d3e1a162d5c775e860ecbdc12e437ac2&chksm=faad79e9cddaf0ff479db22eece66aa03a676b24fab1f3bf3e9bfd752d3e579e2d20c54597e2&mpshare=1&scene=23&srcid=1213z0TVe3vs7OXgmzHkrMoi&sharer_sharetime=1607843537664&sharer_shareid=bbb5a11cf10a2b2fffe1d71d4b15e4ad#rd






















