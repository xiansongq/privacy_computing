cmake_minimum_required(VERSION 3.16)
project(private)

# On some machines, the path of OpenSSL is  /usr/local/lib64/openssl/libcrypto.a
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    SET(OPENSSL_LIBRARIES /usr/local/lib64/libcrypto.a /usr/local/lib64/libssl.a)
    SET(OPENSSL_INCLUDE_DIR /usr/local/include)
    SET(OPENSSL_DIR /usr/local/lib64)
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    SET(OPENSSL_LIBRARIES /usr/local/lib/libcrypto.a /usr/local/lib/libssl.a)
    SET(OPENSSL_INCLUDE_DIR /usr/local/include)
    SET(OPENSSL_DIR /usr/local/lib)
ENDIF()
FIND_PACKAGE(OpenMP REQUIRED)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fpic -msse4.2 -Wno-deprecated-declarations -Wno-unused-result -Xpreprocessor -O3")

INCLUDE_DIRECTORIES(
        ${OPENSSL_INCLUDE_DIR}
        "/usr/local/include"
)

LINK_DIRECTORIES(
        /usr/local/lib
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

include(CMakePrintHelpers)

foreach(key
        IN ITEMS
        NUMBER_OF_LOGICAL_CORES
        NUMBER_OF_PHYSICAL_CORES
        IS_64BIT
        HAS_SSE2
        OS_NAME
        OS_RELEASE
        )
    # query the item ${key} and save its value in the variable _${key}
    cmake_host_system_information(RESULT _${key} QUERY ${key})
    cmake_print_variables(_${key})
endforeach()

# let the preprocessor know about the system name
if(CMAKE_SYSTEM_NAME  STREQUAL "Linux")
    SET(IS_LINUX 1)
endif()
if(CMAKE_SYSTEM_NAME  STREQUAL "Darwin")
    SET(IS_MACOS 1)
endif()
if(CMAKE_SYSTEM_NAME  STREQUAL "Windows")
    SET(IS_WINDOWS 1)
endif()

configure_file(${PROJECT_SOURCE_DIR}/src/config/config.h.in ${PROJECT_SOURCE_DIR}/src/config/config.h @ONLY)



set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Concurrent
        REQUIRED)


# 添加可执行文件
file(GLOB_RECURSE QTSOURCES
        "src/frontend/*.cpp"
        "src/frontend/*.h"
        "src/frontend/*.ui"
        )
add_executable(private
        src/frontend/include/mainwindow.h
        src/frontend/cpp/mainwindow.cpp
        src/frontend/cpp/mainwindow.ui
#        src/frontend/include/page1.h
#        src/frontend/cpp/page1.cpp
#        src/frontend/cpp/page1.ui
#        src/frontend/include/page2.h
#        src/frontend/cpp/page2.cpp
#        src/frontend/cpp/page2.ui
#        src/frontend/include/page3.h
#        src/frontend/cpp/page3.cpp
#        src/frontend/cpp/page3.ui
#        src/frontend/include/page4.h
#        src/frontend/cpp/page4.cpp
#        src/frontend/cpp/page4.ui
#        src/frontend/include/page5.h
#        src/frontend/cpp/page5.cpp
#        src/frontend/cpp/page5.ui
        src/frontend/include/pages.h
        src/frontend/cpp/pages.cpp
        src/frontend/main.cpp
        src/frontend/include/loading.h
        src/frontend/cpp/loading.cpp
#        src/frontend/include/WorkThread.h
        )

#add_library(psi_cm20 STATIC  src/qtapi/psi_cm20.cpp  src/qtapi/messge.hpp)
#target_link_libraries(psi_cm20 ${OPENSSL_LIBRARIES} OpenMP::OpenMP_CXX ${CMAKE_DL_LIBS})


#[[!!!有时会遇到 seal/seal.h 找不到的错误]]
find_package(SEAL 4.0  REQUIRED)

if(NOT SEAL_FOUND)
    message(FATAL_ERROR "Microsoft SEAL: not found")
else()
    message(STATUS "Microsoft SEAL: ${SEAL_DIR}")
endif()


add_library(sealpir src/mpc/pir/pir.hpp src/mpc/pir/pir.cpp src/mpc/pir/pir_client.hpp src/mpc/pir/pir_client.cpp src/mpc/pir/pir_server.hpp
        src/mpc/pir/pir_server.cpp  )

target_link_libraries(sealpir SEAL::seal)
target_link_libraries(private
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Concurrent
        sealpir
        ${OPENSSL_LIBRARIES} OpenMP::OpenMP_CXX ${CMAKE_DL_LIBS}
        )