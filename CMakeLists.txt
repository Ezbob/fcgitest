CMAKE_MINIMUM_REQUIRED(VERSION 3.1.3)

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

# tests for compiler compliance and sets the C++ standard to C++11
SET(CMAKE_CXX_STANDARD 11)

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
    ${SOURCES_PREFIX}/fcgisrv_shims/asio_scheduler.cpp
    ${SOURCES_PREFIX}/main.cpp
)

SET(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LIB_DIR}/asio/asio/include
)

# libraries and their cmake files
ADD_SUBDIRECTORY(${LIB_DIR}/fcgisrv EXCLUDE_FROM_ALL)
ADD_SUBDIRECTORY(${LIB_DIR}/json11 EXCLUDE_FROM_ALL)

# binary using the main file
SET(EXEC_NAME ${PROJECT_NAME})
ADD_EXECUTABLE(${EXEC_NAME} "")
TARGET_SOURCES(${EXEC_NAME} PRIVATE ${SOURCES})
TARGET_INCLUDE_DIRECTORIES(${EXEC_NAME} PRIVATE ${INCLUDES})
TARGET_LINK_LIBRARIES(${EXEC_NAME} PRIVATE json11 fcgisrv)

# sockets for win and other
if(WIN32)
    TARGET_LINK_LIBRARIES(${EXEC_NAME} PRIVATE ws2_32)
else(WIN32)
    FIND_PACKAGE(Threads)
    TARGET_LINK_LIBRARIES(${EXEC_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
endif(WIN32)
