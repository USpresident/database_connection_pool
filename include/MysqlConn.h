#ifndef MYSQL_CONN_H
#define MYSQL_CONN_H

#include <mysql/mysql.h>
#include <string>
#include <chrono> // 时间对应的头文件

class MySqlConn {

public:
    MySqlConn();
    ~MySqlConn();

    bool Connect(std::string user, std::string pwd,
        std::string dbName, std::string ip, unsigned short port = 3306);

    bool Update(std::string sql); // insert, update, delete
    bool Query(std::string sql); // query

    bool Next(); // 遍历结果集
    std::string Value(int index); // 获取结果集中字段值

    bool Transaction(); // 事务操作
    bool Commit(); // 提交事务
    bool Rollback(); // s事务回滚

    void RefreshAvailTime();
    long long GetAvailTime();

private:
    void FreeResult();

private:
    MYSQL *m_conn{nullptr};
    MYSQL_RES *m_result{nullptr};
    MYSQL_ROW m_row{nullptr};

    int m_rowCount;

    std::chrono::steady_clock::time_point m_availTime;
};

#endif