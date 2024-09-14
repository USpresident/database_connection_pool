#include "ConnectionPool.h"
#include <iostream>


void MysqlConn_fun()
{
    MySqlConn conn;
    if (!conn.Connect("langzhong", "Liuchao@1234", "school", "192.168.152.136", 3306)) {
        std::cout << "Connect db fail ... " << std::endl;
        return;
    }

    if (!conn.Update("INSERT INTO stu_base_info (name) VALUES ('lyy');")) {
        std::cout << "Insert data fail ..." << std::endl;
        return;
    }

    if (!conn.Query("SELECT * FROM stu_base_info;")) {
        std::cout << "Query fail ..." << std::endl;
        return;
    }

    // conn.Next() 获取一整行， conn.Value() 获取一行的字段
    while (conn.Next()) {
        std::cout << conn.Value(0) << ", "
                  << conn.Value(1) << ", "
                  << conn.Value(2) << ", "
                  << conn.Value(3) << std::endl;
    }
}

int main()
{
    MysqlConn_fun();

    return 0;
}

// g++ ../src/Mysql_pool_demo.cpp ../src/MysqlConn.cpp -g -o conn_pool -I../include -I../include_json -I/usr/include/mysql -L../lib -ljsoncpp -lmysqlclient