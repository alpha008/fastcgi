#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fcgi_stdio.h"
//
int main(int argc, char *argv[])
{
	int count = 0;
	
	//阻塞等待并监听某个端口，等待Nginx将数据发过来
	while( FCGI_Accept() >= 0 ) 
	{

		char* method = getenv("REQUEST_METHOD");

        printf("Content-type: text/html\r\n\r\n");
		printf("method:%s\n", method);
		//如果想得到数据，需要从stdin去读，实际上从Nginx上去读
		//如果想上传数据，需要往stdout写，实际上是给Nginx写数据
		char *contentLength = getenv("CONTENT_LENGTH");
        int len;
        char token[128] = {0};
        if( contentLength == NULL )
        {
            len = 0;
        }
        else
        {
            len = atoi(contentLength); //字符串转整型
        }

        if (len <= 0)//没有登陆用户信息
        {
            printf("No data from standard input.<p>\n");
            printf( "len = 0, No data from standard input\n");
        }
        else //获取登陆用户信息
        {
			char buf[4*1024] = {0};
            int ret = 0;
            ret = fread(buf, 1, len, stdin); //从标准输入(web服务器)读取内容
			if(ret == 0)
            {
                printf("fread(buf, 1, len, stdin) err\n");
                continue;
            }
            printf("buf = %s\n", buf);
		}
#if 0
		printf("len = %s\n",len);
		printf("Content-type: text/html\r\n");
		printf("\r\n");
		printf("<title>Fast CGI Hello!</title>");
		printf("<h1>Fast CGI Hello!</h1>\n");

		printf("SERVER_NAME:%s <br>\n", getenv("SERVER_NAME"));
		printf(" <i>SERVER_PORT:%s</i>\n", getenv("SERVER_PORT"));
		printf(" <i>QUERY_STRING:%s</i>\n", getenv("QUERY_STRING"));
		//SERVER_NAME：得到server的host名称
		printf("Request number %d running on host <i>%s</i>\n", ++count, getenv("SERVER_NAME"));
#endif
	}
	
	return 0;
}