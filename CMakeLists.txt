cmake_minimum_required(VERSION 3.26)
project(TemperatureBlanket)

set(CMAKE_CXX_STANDARD 17)

# Set the path to the vcpkg toolchain (adjust if necessary)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/utkuk/OneDrive/Desktop/TemperatureBlanket/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Find required packages
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem) # Add the required Boost components here

add_executable(TemperatureBlanket main.cpp)

# Link libraries
target_link_libraries(TemperatureBlanket PRIVATE ${CURL_LIBRARIES} nlohmann_json pugixml::shared pugixml::pugixml Boost::system Boost::filesystem)
