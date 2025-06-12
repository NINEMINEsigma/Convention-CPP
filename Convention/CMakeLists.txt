message("Convention: --- ----- ----- ----- ----- --")

# 设置CMake版本要求和项目名称
cmake_minimum_required(VERSION 3.17)
project(ConventionLibrary VERSION 0.1.1)

# 引入GNUInstallDirs以获取标准安装路径
include(GNUInstallDirs)

add_subdirectory(src)

message("Convention: ----- ----- ----- ----- -----")