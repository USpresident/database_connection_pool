include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include_json)
include_directories(/usr/include/mysql)

# aux_source_directory(. SUB_SRC)
set(SUB_SRC MysqlConn.cpp ConnectionPool.cpp Mysql_pool_demo.cpp)

add_executable(conn_pool ${SUB_SRC})

# 添加依赖库目录 libjsoncpp.a，最好不使用 link_directories
target_link_directories(conn_pool PUBLIC ${PROJECT_SOURCE_DIR}/lib)

# 加依赖库, 去头去尾 libjsoncpp.a --> jsoncpp
target_link_libraries(conn_pool jsoncpp)
target_link_libraries(conn_pool mysqlclient)
