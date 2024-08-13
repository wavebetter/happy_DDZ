# Install script for directory: E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ddz-client")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "E:/QT5.15/Tools/mingw810_64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/ddz-client.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ddz-client.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ddz-client.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "E:/QT5.15/Tools/mingw810_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/ddz-client.exe")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/card/cmake_install.cmake")
  include("E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/windows/cmake_install.cmake")
  include("E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/control/cmake_install.cmake")
  include("E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/player/cmake_install.cmake")
  include("E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/strategy/cmake_install.cmake")
  include("E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/thread/cmake_install.cmake")
  include("E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/tcp/cmake_install.cmake")
  include("E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/serialize/cmake_install.cmake")
  include("E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/crypto/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "E:/serverProject/DDZ-NetWork/0814/ddz-client/ddz-client/build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
