#include "ConnectionPool.h"
#include "json.h"
#include <fstream>
#include <thread>

#include <iostream>

ConnectionPool &ConnectionPool::GetInstance()
{
    // instance访问范围是当前函数，生命周期到应用程序结束
    static ConnectionPool instance; // 不管调用多少次，都和第一次的内存一样
    return instance;
}

ConnectionPool::ConnectionPool()
{
    // 加载配置文件
    if (!ParseJsonFile()) {
        return;
    }

    for (int i = 0; i < m_minSize; ++i) {
        AddConn();
    }

    // 检测连接池的连接是否够用，不够用创建新连接
    std::thread producer(&ConnectionPool::ProduceConn, this);

    // 连接是否空闲太多，多的话销毁
    std::thread recycler(&ConnectionPool::RecycleConn, this);

    producer.detach(); // 为了不阻塞主线程
    recycler.detach();
}

ConnectionPool::~ConnectionPool()
{
    while (!m_connQ.empty()) {
        MySqlConn *conn = m_connQ.front();
        m_connQ.pop();
        delete conn;
    }
}

std::shared_ptr<MySqlConn> ConnectionPool::GetConnetion()
{
    std::unique_lock<std::mutex> lock(m_mtxQ);
    while (m_connQ.empty()) {
        // 阻塞超时，队列仍然为空
        if (m_cond.wait_for(lock, std::chrono::milliseconds(m_timeout)) ==
            std::cv_status::timeout) {
            if (m_connQ.empty()) {
                continue; // 仍要阻塞
                // return nullptr;
            }
        }
    }

    // 操作完数据库要还回给队列, 通过智能指针实现地址回收，而不是释放
    std::shared_ptr<MySqlConn> sharePtr(m_connQ.front(),
        [this](MySqlConn *conn) {
            // std::unique_lock<std::mutex> lock(m_mtxQ);
            std::lock_guard<std::mutex> lock(m_mtxQ);
            conn->RefreshAvailTime();
            m_connQ.push(conn);
        }
    );

    m_connQ.pop();
    m_cond.notify_all();
    return sharePtr;
}

bool ConnectionPool::ParseJsonFile()
{
    std::ifstream inFile("/code/database_connection_pool/data/dbconf.json");
    if (!inFile.is_open()) {
        return false;
    }

    Json::Reader rd;
    Json::Value root;
    rd.parse(inFile, root);
    inFile.close();

    if (root.isObject()) {
        m_ip          = root["ip"].asString();
        m_user        = root["userName"].asString();
        m_pwd         = root["password"].asString();
        m_dbName      = root["dbName"].asString();
        m_port        = root["port"].asInt();
        m_minSize     = root["minSize"].asInt();
        m_maxSize     = root["maxSize"].asInt();
        m_timeout     = root["timeout"].asInt();
        m_maxIdleTime = root["maxIdleTime"].asInt();
        return true;
    }

    return false;
}

void ConnectionPool::AddConn()
{
    MySqlConn *conn = new MySqlConn();
    if (!conn->Connect(m_user, m_pwd, m_dbName, m_ip, m_port)){
        delete conn;
        return;
    }

    conn->RefreshAvailTime();
    m_connQ.push(conn);
}

void ConnectionPool::ProduceConn()
{
    while (true) {
        std::unique_lock<std::mutex> lock(m_mtxQ);
        while (m_connQ.size() >= m_minSize) {
            std::cout << "ProduceConn wait ........." << std::endl;
            m_cond.wait(lock); // 阻塞不消耗CPU资源
        }

        AddConn();
        m_cond.notify_all();
    }
}

void ConnectionPool::RecycleConn()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        // std::unique_lock<std::mutex> lock(m_mtxQ);
        std::lock_guard<std::mutex> lock(m_mtxQ);
        while (m_connQ.size() > m_minSize) {
            MySqlConn *conn = m_connQ.front();
            if (conn->GetAvailTime() <= m_maxIdleTime) { // 入队后等待时间没有超过最大空闲时间
                break;
            }

            m_connQ.pop();
            delete conn;
        }
    }
}