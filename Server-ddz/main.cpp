#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "TcpServer.h"

#include "MyTest.h"
#include "Log.h"

int main(int argc, char* argv[])
{
#if 0
    if (argc < 3)
    {
        printf("./a.out port path\n");
        return -1;
    }
    unsigned short port = atoi(argv[1]);
    // 切换服务器的工作路径
    chdir(argv[2]);
#else
    unsigned short port = 8888;
    chdir(" ");
#endif
    // 启动服务器
    TcpServer* server = new TcpServer(port, 4);
    server->run();

//    MyTest t;
//    t.test();

    return 0;
}