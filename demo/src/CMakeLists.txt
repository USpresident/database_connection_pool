include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/demo/head)

set(SUB_SRC Mysql_pool_demo.cpp test_fun.cpp)

add_executable(conn_pool ${SUB_SRC})

# 加依赖库, 去头去尾 libjsoncpp.a --> jsoncpp
target_link_libraries(conn_pool dbCconnPool)