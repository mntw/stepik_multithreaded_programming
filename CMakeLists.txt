cmake_minimum_required(VERSION 3.9)
project(multithread_stepik)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wshadow -Wpointer-arith -Wcast-align -std=c++17 -fPIC -finline-functions -O0 -g -ggdb")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
include_directories(src)

add_executable(pthreadCreate src/pthreadCreate.cpp)
add_executable(mutex src/mutex.cpp)
add_executable(condVar src/condVar.cpp)
add_executable(threads_std src/threads_std_cpp11.cpp)
add_executable(pipe src/pipeline.cpp)
add_executable(namedPipe src/namedPipe.cpp)

TARGET_LINK_LIBRARIES(pthreadCreate pthread)
TARGET_LINK_LIBRARIES(mutex pthread)
TARGET_LINK_LIBRARIES(condVar pthread)
TARGET_LINK_LIBRARIES(threads_std pthread)
