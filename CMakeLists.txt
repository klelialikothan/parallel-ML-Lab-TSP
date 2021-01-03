cmake_minimum_required(VERSION 3.13)  # CMake version check
project(parallel-ML-Lab-tsp)          # Create project "parallel-ML-Lab-tsp"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

# Add appropriate tsp.cpp file of project root directory as source file
#set(SOURCE_FILES ./q1/tsp.cpp)
#set(SOURCE_FILES ./q2/tsp.cpp)
#set(SOURCE_FILES ./q3/tsp.cpp)
#set(SOURCE_FILES ./q4/tsp.cpp)
set(SOURCE_FILES ./q5/tsp.cpp)
#set(SOURCE_FILES ./q5alt/tsp.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(parallel-ML-Lab-tsp ${SOURCE_FILES})

# Add linker flag for math library
#target_link_libraries(parallel-ML-Lab-tsp m)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(parallel-ML-Lab-tsp PUBLIC m PUBLIC OpenMP::OpenMP_CXX)
endif()