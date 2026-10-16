cmake_minimum_required(VERSION 3.10)

project(CarSalesManager C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(car_sales PROJECT.c)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(car_sales PRIVATE -Wall -Wextra)
endif()

# The program reads and writes cars.csv in the working directory,
# so copy it next to the executable in the build directory.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cars.csv
               ${CMAKE_CURRENT_BINARY_DIR}/cars.csv COPYONLY)
