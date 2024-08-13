cmake_minimum_required(VERSION 3.5)
project(player)

# 搜索当前目录下的所有源文件
aux_source_directory(./ SRC)

# 将当前目录下的源文件制作成静态库
add_library(player ${SRC})
target_link_libraries(player PRIVATE Qt${QT_VERSION_MAJOR}::Core)
