# CMakeList.txt : CMake project for GearCalcListen, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("GearCalcListen")

# Add source to this project's executable.
add_executable (GearCalcListen "GearCalcListen.c" "GearCalcListen.h")

# TODO: Add tests and install targets if needed.
include(FileEmbed.cmake)

FileEmbedSetup()
FileEmbedAdd(${CMAKE_SOURCE_DIR}/blank.html)

target_link_libraries(GearCalcListen file_embed)
