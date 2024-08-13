cmake_minimum_required(VERSION 3.5)
project(crypto)

# 搜索当前目录下的所有源文件
aux_source_directory(./ SRC)
file(GLOB HEAD ./*.h)

# 将当前目录下的源文件制作成静态库
add_library(crypto ${SRC} ${HEAD}
    rsacrypto.h rsacrypto.cpp
    base64.h base64.cpp)
target_link_libraries(crypto PRIVATE Qt${QT_VERSION_MAJOR}::Core
    # "C:\\OpenSSL-Win64\\lib\\libcrypto.lib")
    "C:/OpenSSL-Win64/lib/libcrypto.lib")
