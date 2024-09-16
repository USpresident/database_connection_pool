include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/demo/head)

#指定c++ 20
set(CMAKE_CXX_STANDARD 20)

#生成ycm可识别的json文件，让源文件找到头文件
#SET(CMAKE_EXPORT_COMPILE_COMMANDS ON )

#可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ../../bin_exe)

#调试, 非必须的
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(SUB_SRC Mysql_pool_demo.cpp test_fun.cpp)

add_executable(conn_pool ${SUB_SRC})

# 加依赖库, 去头去尾 libjsoncpp.a --> jsoncpp
target_link_libraries(conn_pool dbCconnPool)