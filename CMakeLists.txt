cmake_minimum_required(VERSION 2.8.9)
project(vmp)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Threads)
find_package(ALSA)
find_package(SoundIo)

add_definitions(-std=c++11 -O3 -g)
file(GLOB SOURCES "*.cpp")

add_executable(vmp ${SOURCES})
target_link_libraries(vmp ${ALSA_LIBRARIES} ${SOUNDIO_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

