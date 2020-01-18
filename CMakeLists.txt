CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# Name and description are injected from makefile
PROJECT(${PROJECT_NAME}
    VERSION 0.1
    DESCRIPTION "${PROJECT_DESCRIPTION}"
    LANGUAGES CXX C
)

# Some paths
SET(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
SET(SOURCES_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Adds the cmake c++ standard tests in cxx_standards.cmake file
INCLUDE(cxx_standards)

# tests for compiler compliance and sets the C++ standard to C++11
USE_CXX11_STANDARD()

# A set of compilation flags that is always on when compiling C++
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Dc_plusplus -DASIO_STANDALONE")

# Compilation flags that depends on build type
# A symbol _DEBUG is exposed to enable conditional compilation through preprocessor directives
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Linker flags. This just makes the c and c++ stdlibs statically linked for higher portability
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

# Sources sets
SET(SOURCES
    ${SOURCES_PREFIX}/fcgipp/default_dispatcher.cpp
    ${SOURCES_PREFIX}/fcgipp/default_authenticator.cpp
    ${SOURCES_PREFIX}/fcgipp/fcgi_acceptor.cpp
    ${SOURCES_PREFIX}/fcgipp/fcgi_server_request_response.cpp

    ${SOURCES_PREFIX}/fcgipp/http/method.cpp
    ${SOURCES_PREFIX}/fcgipp/http/response.cpp
    ${SOURCES_PREFIX}/fcgipp/http/default_http_internal_server_error_handler.cpp
    ${SOURCES_PREFIX}/fcgipp/http/default_http_method_not_allowed_handler.cpp
    ${SOURCES_PREFIX}/fcgipp/http/default_http_not_found_handler.cpp
    ${SOURCES_PREFIX}/fcgipp/http/default_http_unauthorized_handler.cpp

    ${SOURCES_PREFIX}/fcgipp_shims/asio_multiplexer.cpp
)

SET(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LIB_DIR}/asio/asio/include
)

SET(MAIN_SOURCE
    ${SOURCES_PREFIX}/main.cpp
)

SET(MAINLESS_LIB
    fcgipp-lib
)

# libraries and their cmake files
ADD_SUBDIRECTORY(${LIB_DIR}/fcgi-2.4.0 EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${LIB_DIR}/json11 EXCLUDE_FROM_ALL)

# lib without the main file
ADD_LIBRARY(${MAINLESS_LIB} STATIC "")
TARGET_SOURCES(${MAINLESS_LIB} PUBLIC ${SOURCES})
TARGET_INCLUDE_DIRECTORIES(${MAINLESS_LIB} PUBLIC ${INCLUDES})
TARGET_LINK_LIBRARIES(${MAINLESS_LIB} PUBLIC fcgi-lib json11)

# binary using the main file
SET(EXEC_NAME ${PROJECT_NAME})
ADD_EXECUTABLE(${EXEC_NAME} "")
TARGET_SOURCES(${EXEC_NAME} PRIVATE ${MAIN_SOURCE})
TARGET_LINK_LIBRARIES(${EXEC_NAME} PRIVATE ${MAINLESS_LIB})

# sockets for win and other
if(WIN32)
    TARGET_LINK_LIBRARIES(${MAINLESS_LIB} PRIVATE ws2_32)
else(WIN32)
    FIND_PACKAGE(Threads)
    TARGET_LINK_LIBRARIES(${MAINLESS_LIB} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
endif(WIN32)
