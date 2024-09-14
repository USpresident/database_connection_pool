#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

// ubuntu环境的mysql dev库变更，头文件删除了，本地暂时不支持运行
int main() {
    try {
        sql::mysql::MySQL_Driver *driver;      // MySQL 驱动
        sql::Connection *con;                   // 连接对象
        sql::Statement *stmt;                   // 语句对象
        sql::ResultSet *res;                    // 查询结果集

        // 创建驱动
        driver = sql::mysql::get_mysql_driver_instance();

        // 建立连接（请替换为你的数据库用户名、密码、数据库名称）
        // 在服务器上创建用户：CREATE USER langzhong@'192.168.152.129' identified by 'Liuchao@1234';
        con = driver->connect("192.168.152.136:3306", "langzhong", "Liuchao@1234");

        // 连接到指定数据库
        // 授权：GRANT ALL ON school.* TO 'langzhong'@'192.168.152.129';
        con->setSchema("school");

        // 创建语句对象
        stmt = con->createStatement();

        // 执行查询
        res = stmt->executeQuery("SELECT * FROM class_01;");

        // 输出结果
        while (res->next()) {
            std::cout << res->getString("no") << " | "
                << res->getString("name") << " | "
                << res->getString("score") << " | "
                << res->getString("hobby") << " | "
                << std::endl;
        }

        // 释放资源
        delete res;
        delete stmt;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "MySQL 连接错误：" << e.what() << std::endl;
        exit(1);
    }

    return 0;
}


// build目录下： g++ -o mysql_connect ../src/Mysql_conn_demo.cpp -I/usr/include -L/usr/lib/mysql -lmysqlcppconn