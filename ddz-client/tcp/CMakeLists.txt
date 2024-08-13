cmake_minimum_required(VERSION 3.5)
project(tcp)

# 搜索当前目录下的所有源文件
aux_source_directory(./ SRC)
file(GLOB HEAD ./*.h)

# 将当前目录下的源文件制作成静态库
add_library(tcp ${SRC} ${HEAD})
target_link_libraries(tcp PRIVATE Qt${QT_VERSION_MAJOR}::Core Ws2_32)
