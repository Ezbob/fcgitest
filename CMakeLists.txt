CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# Name and description are injected from makefile
PROJECT(${PROJECT_NAME}
    VERSION 0.1
    DESCRIPTION "${PROJECT_DESCRIPTION}"
    LANGUAGES CXX C
)

# CMAKE module path adding "cmake" directory to the path
SET(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Adds the cmake c++ standard tests in cxx_standards.cmake file
INCLUDE(cxx_standards)

# tests for compiler compliance and sets the C++ standard to C++11
USE_CXX11_STANDARD()

# SOURCES_PREFIX refers to the source folder and is useful when stating
# the source files depedencies of a target
SET(SOURCES_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/src")

# A set of compilation flags that is always on when compiling C++
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Dc_plusplus")

# Compilation flags that depends on build type
# A symbol _DEBUG is exposed to enable conditional compilation through preprocessor directives
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Linker flags. This just makes the c and c++ stdlibs statically linked for higher portability
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

SET(EXEC_NAME "${PROJECT_NAME}")

SET(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Next steps:
SET(SOURCES
    "${SOURCES_PREFIX}/main.cpp"
)

SET(INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${LIB_DIR}/asio/asio/include"
)

ADD_SUBDIRECTORY("${LIB_DIR}/fcgi-2.4.0")

# Adds an executable
ADD_EXECUTABLE(${EXEC_NAME} "")

# Target_Sources works best when sources has been set to "" in lib/exe (this then becomes a legacy parameter)
# Target_Sources helps with transitive scoping (i.e. PRIVATE, PUBLIC or INTERFACE)
# all TARGET_* needs to defined after the target (since it has to be there first)

TARGET_SOURCES(${EXEC_NAME} PRIVATE ${SOURCES})

TARGET_INCLUDE_DIRECTORIES(${EXEC_NAME} PRIVATE ${INCLUDES})

TARGET_LINK_LIBRARIES(${EXEC_NAME} PRIVATE fcgi-lib)

if(WIN32)
    TARGET_LINK_LIBRARIES(${EXEC_NAME} PRIVATE ws2_32)
endif(WIN32)
