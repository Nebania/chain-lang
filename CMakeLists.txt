cmake_minimum_required(VERSION 3.16)
project(chain_webview CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    # Windows x86_64
    add_compile_definitions(WEBVIEW_WINAPI)

    set(PLATFORM_LIBS ole32 comctl32 shlwapi version)
else()
    # Linux (x86_64, aarch64, i686, riscv64)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.0)
    
    include_directories(${WEBKIT_INCLUDE_DIRS})
    link_directories(${WEBKIT_LIBRARY_DIRS})
    add_definitions(${WEBKIT_CFLAGS_OTHER})
    
    set(PLATFORM_LIBS ${WEBKIT_LIBRARIES} stdc++fs)
endif()

add_library(link_webview STATIC link_webview.cpp)
target_link_libraries(link_webview PRIVATE ${PLATFORM_LIBS})
