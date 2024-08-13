cmake_minimum_required(VERSION 3.5)

project(ddz-client VERSION 0.1 LANGUAGES CXX)
# 自动处理.UI文件, 最后生成一个 ui_xxx.h 文件
set(CMAKE_AUTOUIC ON)
# 自动处理元对象相关的宏 Q_OBJECT
set(CMAKE_AUTOMOC ON)
# 自动处理 qrc 文件, 会将其生成为相应的c++代码, 最后嵌入到可执行程序中
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
# 让编译器严格按照上面指定的标准进行程序的构建, 如果编译器不支持上面的标准, cmake会报错并停止构建
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 搜索Qt相关的组件(QT 模块)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
)

# 添加头文件目录 - 要写到添加子目录指令的上面
include_directories(
    ${PROJECT_SOURCE_DIR}/card
    ${PROJECT_SOURCE_DIR}/windows
    ${PROJECT_SOURCE_DIR}/control
    ${PROJECT_SOURCE_DIR}/player
    ${PROJECT_SOURCE_DIR}/strategy
    ${PROJECT_SOURCE_DIR}/thread
    ${PROJECT_SOURCE_DIR}/tcp
    ${PROJECT_SOURCE_DIR}/serialize
    ${PROJECT_SOURCE_DIR}/crypto
    "C:/OpenSSL-Win64/include"
    "C:/Protobuf/include"
)

link_directories("C:\\Protobuf\\lib")

# 添加子目录
add_subdirectory(card)
add_subdirectory(windows)
add_subdirectory(control)
add_subdirectory(player)
add_subdirectory(strategy)
add_subdirectory(thread)
add_subdirectory(tcp)
add_subdirectory(serialize)
add_subdirectory(crypto)

# 添加资源文件
qt5_add_resources(RES res.qrc)

# 添加图标资源文件
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set(ICON_RES ${CMAKE_CURRENT_SOURCE_DIR}/ddz.rc)
endif()

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ddz-client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ddz-client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ddz-client SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ddz-client
            ${PROJECT_SOURCES}
            ${RES}
            ${ICON_RES}
        )
    endif()
endif()

target_link_libraries(ddz-client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    card windows control player strategy thread crypto serialize tcp
)

set_target_properties(ddz-client PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ddz-client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ddz-client)
endif()
