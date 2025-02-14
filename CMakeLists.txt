cmake_minimum_required(VERSION 3.10) # 设置CMake最低版本

project(ThreadPool VERSION 1.0) # 项目名称和版本

set(CMAKE_CXX_STANDARD 11) # 设置C++标准

# 添加头文件包含路径
include_directories(".")

# 创建可执行文件
add_executable(example example.cpp)
target_link_libraries(example pthread) # 链接pthread库
