#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include "MysqlConn.h"
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>

class ConnectionPool {
public:
    static ConnectionPool &GetInstance(); // 编译器不支持c++11，静态局部遍历构建单例有线程安全问题？
    ConnectionPool(const ConnectionPool &) = delete;
    ConnectionPool &operator=(const ConnectionPool &) = delete;
    ~ConnectionPool();

    std::shared_ptr<MySqlConn> GetConnetion();

private:
    ConnectionPool();
    bool ParseJsonFile();

    void AddConn();
    void ProduceConn();
    void RecycleConn();

private:
    std::queue<MySqlConn *> m_connQ{}; // 多线程同时访问，共享资源
    std::mutex m_mtxQ;
    std::condition_variable m_cond;

    std::string m_ip; // 服务器ip
    std::string m_user; // 服务器用户名
    std::string m_pwd; // 服务器登录密码
    std::string m_dbName; // 数据库名称
    unsigned short m_port; // 数据库绑定的端口 uint16_t

    int m_minSize; // 队列的上下限
    int m_maxSize;

    int m_timeout; // 线程等待取连接的超时时长，超时再尝试/报错
    int m_maxIdleTime; // 最大空闲时长，超过则删除该连接
};

#endif