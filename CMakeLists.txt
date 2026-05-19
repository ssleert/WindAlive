cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

project(windalive LANGUAGES CXX)
add_compile_options(-ffast-math -march=native -mtune=native)

add_executable(windalive src/windalive.cpp)

file(GLOB_RECURSE MODULES src/*.cpp)
list(FILTER MODULES EXCLUDE REGEX windalive.cpp)

target_sources(windalive PRIVATE FILE_SET CXX_MODULES FILES ${MODULES})

add_subdirectory(libs/raylib)
add_subdirectory(libs/spdlog)

target_include_directories(windalive PRIVATE 
  src 
  libs/thread-pool/include
)

target_link_libraries(windalive PRIVATE
  raylib
  spdlog
)
