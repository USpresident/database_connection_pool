include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include_json)

# aux_source_directory(. SUB_SRC)
set(SUB_SRC MysqlConn.cpp ConnectionPool.cpp)

# 依赖了静态库，不能构建SHARED
add_library(dbCconnPool ${SUB_SRC})

# 添加依赖库目录 libjsoncpp.a，最好不使用 link_directories
target_link_directories(dbCconnPool PUBLIC ${PROJECT_SOURCE_DIR}/lib_json)

target_link_libraries(dbCconnPool jsoncpp)
target_link_libraries(dbCconnPool mysqlclient)
