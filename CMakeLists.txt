cmake_minimum_required(VERSION 2.8.9)
project(vmp)

find_package(Threads)
find_package(ALSA)
find_package(PulseAudio)

add_definitions(-std=c++11 -O3 -g)
file(GLOB SOURCES "*.cpp")

add_executable(vmp ${SOURCES})
target_link_libraries(vmp ${CMAKE_THREAD_LIBS_INIT} ${ALSA_LIBRARIES} ${PULSEAUDIO_LIBRARY} ${CMAKE_SOURCE_DIR}/libsoundio.a)

