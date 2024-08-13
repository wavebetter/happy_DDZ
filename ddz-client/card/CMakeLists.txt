cmake_minimum_required(VERSION 3.5)
project(card)

# 搜索当前目录下的所有源文件
aux_source_directory(./ SRC)

# 将当前目录下的源文件制作成静态库
add_library(card ${SRC})

target_link_libraries(card PRIVATE Qt${QT_VERSION_MAJOR}::Core)
