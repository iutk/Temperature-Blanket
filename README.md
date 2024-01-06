Project Title: TemperatureBlanket
Description:

TemperatureBlanket is a C++ application designed to create a visual representation of temperature trends
over time for a specific location. Using data from the World Weather Online API, it maps temperature
ranges to colors, allowing users to visualize temperature changes.

Author:
Ilhan Utku Kargaci

Written and maintained by Ilhan Utku Kargaci.
Contact: iutkukargaci@gmail.com
GitHub: https://github.com/iutk

Prerequisites:

C++ Compiler (GCC, Clang, MSVC, etc.)
CMake (Version 3.26 or higher)
vcpkg (for dependency management)

Installation:
Setting up vcpkg

1. Install vcpkg:
git clone https://github.com/Microsoft/vcpkg.git

2. Activate vcpkg:
.\vcpkg\bootstrap-vcpkg.bat # For Windows

3. Install requirements in vcpkg.json:
./vcpkg/vcpkg install

Building the Project:

1. Clone the repository:
git clone https://github.com/iutk/Temperature-Blanket.git

2. Access the project:
cd TemperatureBlanket

3. Create a build directory and navigate into it:
mkdir build && cd build

4. Run CMake and build the project:
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build .