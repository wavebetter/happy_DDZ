cmake_minimum_required(VERSION 3.5)
project(control)

# 搜索当前目录下的所有源文件
aux_source_directory(./ SRC)

# 将当前目录下的源文件制作成静态库
add_library(control ${SRC}
    datamanager.h datamanager.cpp)

target_link_libraries(control PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Multimedia
)
