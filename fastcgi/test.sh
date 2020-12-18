#!/bin/sh
ps -ef | grep fcgi_test | grep -v grep | awk '{print $2}' | xargs kill -9
spawn-fcgi -a 127.0.0.1 -p 9002 -f  /home/ubuntu/fastcgi/fastcgi/fcgi_test
#ps -ef | grep fcgi_test | grep -v fcgi_test | awk '{print $2}' | xargs kill -9
#http://182.254.181.144:9000/fcgi_test.cgi  


#使用该网址模拟post方法
#https://getman.cn/?s=749801132f968ca578ea3a6bfdd3d299