cmake_minimum_required(VERSION 3.0.0)
project(GTools VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(GTools src/CmdArgsParser.cpp)
#set_property(TARGET GTools PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_property(TARGET GTools PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")

target_include_directories(GTools PUBLIC include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
