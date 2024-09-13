#include "MysqlConn.h"

MySqlConn::MySqlConn()
{
    m_conn = mysql_init(nullptr);
    mysql_set_character_set(m_conn, "utf8");
}

MySqlConn::~MySqlConn()
{
    if (m_conn != nullptr) {
        mysql_close(m_conn);
        m_conn = nullptr;
    }

    FreeResult();
}

bool MySqlConn::Connect(std::string user, std::string pwd,
    std::string dbName, std::string ip, unsigned short port = 3306)
{
    return (mysql_real_connect(m_conn, ip.c_str(), user.c_str(), pwd.c_str(),
        dbName.c_str(), port, nullptr, 0) != nullptr);
}

bool MySqlConn::Update(std::string sql)
{
    return (mysql_query(m_conn, sql.c_str()) == 0);
}

bool MySqlConn::Query(std::string sql)
{
    FreeResult();

    if (!Update(sql)) {
        return false;
    }

    m_result = mysql_store_result(m_conn); // null 可返回 false
    return true;
}

bool MySqlConn::Next()
{
    if (m_result == nullptr) {
        return false;
    }

    m_row = mysql_fetch_row(m_result);
    return true;
}

std::string MySqlConn::Value(int index)
{
    int rowCount = mysql_num_fields(m_result);
    if ((index < 0) || (index >= rowCount)) {
        return std::string();
    }

    char *val = m_row[index];
    unsigned long len = mysql_fetch_lengths(m_result)[index]; // 获取字段值的长度
    return std::string(val, len); // 遇到 \0 可跳过
}

bool MySqlConn::Transaction()
{
    return (mysql_autocommit(m_conn, false) == 0); // false手动提交，true自动提交
}

bool MySqlConn::Commit()
{
    return (mysql_commit(m_conn) == 0);
}

bool MySqlConn::Rollback()
{
    return (mysql_rollback(m_conn) == 0);
}

void MySqlConn::FreeResult()
{
    if (m_result != 0) {
        mysql_free_result(m_result);
        m_result = nullptr;
    }
}

void MySqlConn::RefreshAvailTime()
{
    m_availTime = std::chrono::steady_clock::now();
}

long long MySqlConn::GetAvailTime()
{
    using namespace std::chrono;
    nanoseconds res = steady_clock::now() - m_availTime;
    milliseconds milsc = duration_cast<milliseconds>(res);
    return milsc.count();
}
