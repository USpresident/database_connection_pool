#include "test_fun.h"
#include "ConnectionPool.h"
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> GenerateArray(int start, int end, int size)
{
    std::vector<int> vec(size + 1);
    int step = (end - start) / size;
    vec[0] = start;
    for (int i = 1; i < size; ++i) {
        vec[i] = vec[i - 1] + step;
    }
    vec[size] = end;

    return vec;
}

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

    std::cout << __FUNCTION__ << " time_cost: " << (after - before).count() << "ns, "
        << (after - before).count() / 1000000 << "ms ." << std::endl;
}

void OneThreadWithConnPool(int start, int end)
{
    using namespace std::chrono;
    steady_clock::time_point before = steady_clock::now();
    OptWithConnPool(start, end);
    steady_clock::time_point after = steady_clock::now();

    auto len = (after - before);

    std::cout << __FUNCTION__ << " time_cost: " << len.count() << "ns, "
        << len.count() / 1000000 << "ms ." << std::endl;
}

void MultiThreadWithoutConnPool(int start, int end)
{
    // 这段不需要数据也可以存完全部数据，没有丢失数据
    // MySqlConn conn;
    // if (!conn.Connect("langzhong", "Liuchao@1234", "school", "192.168.152.136")) {
    //     std::cout << "Connect db fail ... " << std::endl;
    //     return;
    // }

    using namespace std::chrono;
    steady_clock::time_point before = steady_clock::now();
    int threadNum = 5;
    std::vector<int> arr = GenerateArray(start, end, threadNum);
    std::thread t1(OptWithoutConnPool, arr[0], arr[1]);
    std::thread t2(OptWithoutConnPool, arr[1], arr[2]);
    std::thread t3(OptWithoutConnPool, arr[2], arr[3]);
    std::thread t4(OptWithoutConnPool, arr[3], arr[4]);
    std::thread t5(OptWithoutConnPool, arr[4], arr[5]);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    steady_clock::time_point after = steady_clock::now();

    std::cout << __FUNCTION__ << " time_cost: " << (after - before).count() << "ns, "
        << (after - before).count() / 1000000 << "ms ." << std::endl;
}

void MultiThreadWithConnPool(int start, int end)
{
    using namespace std::chrono;
    steady_clock::time_point before = steady_clock::now();
    int threadNum = 5;
    std::vector<int> arr = GenerateArray(start, end, threadNum);
    std::thread t1(OptWithConnPool, arr[0], arr[1]);
    std::thread t2(OptWithConnPool, arr[1], arr[2]);
    std::thread t3(OptWithConnPool, arr[2], arr[3]);
    std::thread t4(OptWithConnPool, arr[3], arr[4]);
    std::thread t5(OptWithConnPool, arr[4], arr[5]);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    steady_clock::time_point after = steady_clock::now();

    auto len = (after - before);

    std::cout << __FUNCTION__ << " time_cost: " << len.count() << "ns, "
        << len.count() / 1000000 << "ms ." << std::endl;
}
