#ifndef MYSQL_CONN_H
#define MYSQL_CONN_H

#include <string>

class MySqlConn {

public:
    MySqlConn();
    ~MySqlConn();

    bool Connect(std::string user, std::string pwd,
        std::string dbName, std::string ip, unsigned short port = 3306);

    bool Update(std::string sql);
    bool Query(std::string sql);

    bool Next(); // 遍历结果集
    std::string Value(int index); // 获取结果集中字段值

    bool Transaction(); // 事务操作
    bool Commit(); // 提交事务
    bool Rollback(); // s事务回滚

private:
    
};

#endif