PROJECT (qomxwidget)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

set(CMAKE_DEBUG_POSTFIX "d")

OPTION ( ENABLE_WIDGET "Enable widget" ON )

find_package(QDBusDaemon REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
#add_subdirectory(sample)
