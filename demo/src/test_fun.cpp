#include "test_fun.h"
#include "ConnectionPool.h"
#include <iostream>

void MysqlConn_fun()
{
    MySqlConn conn;
    if (!conn.Connect("langzhong", "Liuchao@1234", "school", "192.168.152.136")) {
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

void OptWithoutConnPool(int start, int end)
{
    for (int i = start; i < end; ++i) {
        MySqlConn conn;
        if (!conn.Connect("langzhong", "Liuchao@1234", "school", "192.168.152.136")) {
            std::cout << "Connect db fail ... " << std::endl;
            return;
        }

        char sql[1024] = {0};
        sprintf(sql, "INSERT INTO db_conn_pool VALUES (%d, %d);", i, i * 3);
        if (!conn.Update(sql)) {
            std::cout << "Insert data fail ..." << std::endl;
            return;
        }
    }
}

void OptWithConnPool(int start, int end)
{
    for (int i = start; i < end; ++i) {
        std::shared_ptr<MySqlConn> conn = ConnectionPool::GetInstance().GetConnetion();

        char sql[1024] = {0};
        sprintf(sql, "INSERT INTO db_conn_pool VALUES (%d, %d);", i, i * 3);
        if (!conn->Update(sql)) {
            std::cout << "Insert data fail ..." << std::endl;
            return;
        }
    }
}

void OneThreadWithoutConnPool(int start, int end)
{
    using namespace std::chrono;
    steady_clock::time_point before = steady_clock::now();
    OptWithoutConnPool(start, end);
    steady_clock::time_point after = steady_clock::now();

    std::cout << "One thread without conn pool: ";
    std::cout << (after - before).count() << "ns, "
        << (after - before).count() / 1000000 << "ms ." << std::endl;
}

void OneThreadWithConnPool(int start, int end)
{
    using namespace std::chrono;
    steady_clock::time_point before = steady_clock::now();
    OptWithConnPool(start, end);
    steady_clock::time_point after = steady_clock::now();

    auto len = (after - before);

    std::cout << "One thread with conn pool: ";
    std::cout << len.count() << "ns, "
        << len.count() / 1000000 << "ms ." << std::endl;
}

void MultiThreadWithoutConnPool(int start, int end)
{

}

void MultiThreadWithConnPool(int start, int end)
{

}
