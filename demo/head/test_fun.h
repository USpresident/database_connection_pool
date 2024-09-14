#ifndef TEST_FUN_H
#define TEST_FUN_H

// 验证 MySqlConn 是否正常连接数据库 
void MysqlConn_fun();

// 单线程，非连接池
void OneThreadWithoutConnPool(int start, int end);

// 单线程，连接池
void OneThreadWithConnPool(int start, int end);

// 多线程，非连接池
void MultiThreadWithoutConnPool(int start, int end);

// 多线程，连接池
void MultiThreadWithConnPool(int start, int end);

#endif