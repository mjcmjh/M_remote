cmake_minimum_required(VERSION 3.10)
project(EigenTest)

# 查找 Eigen3
find_package(Eigen3 REQUIRED)

# 添加可执行文件
add_executable(test_eigen test_eigen.cpp)

# 链接 Eigen3
target_link_libraries(test_eigen Eigen3::Eigen)