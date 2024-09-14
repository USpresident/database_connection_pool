#include "test_fun.h"

int main()
{
    // MysqlConn_fun();

    // OneThreadWithoutConnPool(0, 10);
    OneThreadWithConnPool(0, 10);

    // MultiThreadWithoutConnPool(0, 10);
    // MultiThreadWithConnPool(0, 10);

    return 0;
}

// g++ ../src/Mysql_pool_demo.cpp ../src/MysqlConn.cpp -g -o conn_pool -I../include -I../include_json -I/usr/include/mysql -L../lib -ljsoncpp -lmysqlclient