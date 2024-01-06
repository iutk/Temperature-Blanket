cmake_minimum_required(VERSION 3.26)
project(TemperatureBlanket)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TOOLCHAIN_FILE "[path-to]/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(CURL REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem regex)

add_executable(TemperatureBlanket main.cpp
        TemperatureBlanket.h)

target_link_libraries(TemperatureBlanket PRIVATE ${CURL_LIBRARIES} Boost::filesystem Boost::regex)


