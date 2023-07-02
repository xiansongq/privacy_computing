cmake_minimum_required(VERSION 3.16)
project(privacy_computing)

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

set(Qt5_DIR /usr/lib/x86_64-linux-gnu)  # 将路径替换为你Qt库的实际路径

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Concurrent
        REQUIRED)
# 添加Qt静态库
set(QT_STATIC_LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Concurrent
        )

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

# 管理资源文件
qt5_add_resources (img src/frontend/cpp/icon.qrc)

add_executable(private
        src/frontend/include/mainwindow.h
        src/frontend/cpp/mainwindow.cpp
        src/frontend/cpp/mainwindow.ui
        src/frontend/include/pages.h
        src/frontend/cpp/pages.cpp
        src/frontend/main.cpp
        src/frontend/include/loading.h
        src/frontend/cpp/loading.cpp
        ${img}
        )

#target_link_libraries(private
#        Qt5::Core
#        Qt5::Gui
#        Qt5::Widgets
#        Qt5::Concurrent
#        sealpir
#        ${OPENSSL_LIBRARIES} OpenMP::OpenMP_CXX ${CMAKE_DL_LIBS}
#        )
target_link_libraries(private
        ${QT_STATIC_LIBRARIES}
        sealpir
        ${OPENSSL_LIBRARIES}
        OpenMP::OpenMP_CXX
        ${CMAKE_DL_LIBS}
        )

#add_executable(ecdh_psi src/test/tes_ecdh_pis.cpp)
#target_link_libraries(ecdh_psi  ${OPENSSL_LIBRARIES} OpenMP::OpenMP_CXX ${CMAKE_DL_LIBS})

# 这一步 不一定需要
# 设置生成的安装包名字
set(CPACK_PACKAGE_NAME "privacy_computing")
# 设置支持指定安装目录的控制为 ON
set(CPACK_SET_DESTDIR ON)
# 设置安装到的目录路径
set(CPACK_INSTALL_PREFIX "/software")
# 这是生成的安装的版本号信息
set(CPACK_PACKAGE_VERSION "1.0.0")
# 设置 group 名字
set(CPACK_RPM_PACKAGE_GROUP "xiansong")
# 设置 vendor 名字
set(CPACK_PACKAGE_VENDOR "xiansong")
# 设置 license 信息
set(CPACK_RPM_PACKAGE_LICENSE "No License")
# 指定输出文件类型为ZIP和TGZ
set(CPACK_GENERATOR "ZIP;TGZ")

install(TARGETS private DESTINATION bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/README.md DESTINATION . )


install(DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}/testfile/ DESTINATION testfile)


include(CPack)
