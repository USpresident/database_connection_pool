#include "test_fun.h"

int main()
{
    // MysqlConn_fun();

    OneThreadWithoutConnPool(0, 1000);   // time_cost: 67710851886ns, 67710ms
    OneThreadWithConnPool(0, 1000);      // time_cost: 2313295485ns, 2313ms

    MultiThreadWithoutConnPool(0, 1000); // time_cost: 28945063455ns, 28945ms
    MultiThreadWithConnPool(0, 1000);    // time_cost: 1241058639ns, 1241ms

    return 0;
}

// g++ ../src/Mysql_pool_demo.cpp ../src/MysqlConn.cpp -g -o conn_pool -I../include -I../include_json -I/usr/include/mysql -L../lib -ljsoncpp -lmysqlclient