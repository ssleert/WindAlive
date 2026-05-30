cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

if(USE_STDCXX)
  if(UNIX AND NOT APPLE)
    set(LIBCXX /lib/libc++.a /lib/libc++abi.a)
    set(LIBCXX_FLAGS -stdlib=libc++ -fexperimental-library)
  endif()
endif()

if(USE_ANGLE_FROM_CHROMIUM)
  set(_GLFW_EGL_LIBRARY libEGL.so)
  set(_GLFW_GLESV2_LIBRARY libGLESv2.so)
  set(LIBANGLE /usr/lib/chromium/libEGL.so /usr/lib/chromium/libGLESv2.so)
endif()

project(windalive LANGUAGES CXX)

add_compile_options(
  -ffast-math 
  -march=native 
  -mtune=native 
  -fno-strict-aliasing
  -Wno-nan-infinity-disabled
  ${LIBCXX_FLAGS}
)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(-fsanitize=address,undefined)
  add_link_options(-fsanitize=address,undefined)
endif()

add_executable(windalive src/windalive.cpp)

file(GLOB_RECURSE MODULES src/*.cpp)
list(FILTER MODULES EXCLUDE REGEX windalive.cpp)

target_sources(windalive PRIVATE FILE_SET CXX_MODULES FILES ${MODULES})

set(GRAPHICS GRAPHICS_API_OPENGL_ES3)
add_subdirectory(libs/raylib)

add_subdirectory(libs/spdlog)
add_subdirectory(libs/mimalloc)

target_include_directories(windalive PRIVATE 
  src 
  libs/thread-pool/include
)

target_link_libraries(windalive PRIVATE
  raylib
  spdlog
  ${LIBCXX}
  ${LIBANGLE}
)
