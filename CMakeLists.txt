cmake_minimum_required(VERSION 3.15)
project(modern_coroutine_lib VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准为C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加编译选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 添加库目标
add_library(modern_coroutine INTERFACE)
target_include_directories(modern_coroutine INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 添加示例目标
add_subdirectory(examples)

# 安装配置
include(GNUInstallDirs)
install(TARGETS modern_coroutine
    EXPORT modern_coroutine-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

install(EXPORT modern_coroutine-targets
    FILE modern_coroutine-targets.cmake
    NAMESPACE modern_coroutine::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/modern_coroutine
)