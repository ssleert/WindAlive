cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(USE_ANGLE_FROM_CHROMIUM)
  add_definitions(-D_GLFW_EGL_LIBRARY="libEGL.so")
  add_definitions(-D_GLFW_GLESV2_LIBRARY="libGLESv2.so")
  set(LIBANGLE /usr/lib/chromium/libEGL.so /usr/lib/chromium/libGLESv2.so)
endif()

project(windalive LANGUAGES CXX C)

if(USE_STDCXX)
  add_compile_options(
    -stdlib=libc++
  )
  link_libraries(
    c++.a
    c++abi.a
  )
endif()

add_compile_options(
  -fstrict-aliasing
  -Wno-nan-infinity-disabled
)

add_link_options(-Wl,--build-id=none)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  add_compile_options(
    -fno-stack-protector
    -ffast-math 
    -march=native 
    -mtune=native 
  )
  add_definitions(
    -DNDEBUG -DNTRACE
  )
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  if(NOT DEBUGGER)
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
  endif()

  add_compile_options(-g3)
  add_link_options(-g3)
endif()

if(USE_MIMALLOC)
  add_subdirectory(libs/mimalloc)
  link_libraries(
    mimalloc-static
  )
endif()

set(GRAPHICS GRAPHICS_API_OPENGL_ES3)
set(OPENGL_VERSION "ES 3.0")
add_definitions(-DOPENGL_VERSION="ES 3.0")
#add_definitions(-DRL_DEFAULT_SHADER_VERTEX_PRECISION_HIGHP)
#add_definitions(-DRL_DEFAULT_SHADER_FRAGMENT_PRECISION_HIGHP)
add_subdirectory(libs/raylib)

add_subdirectory(libs/spdlog)
add_subdirectory(libs/boost)

add_executable(windalive src/windalive.cpp 
  libs/rlImGui/rlImGui.cpp
  libs/imgui/imgui.cpp
  libs/imgui/imgui_demo.cpp
  libs/imgui/imgui_draw.cpp
  libs/imgui/imgui_tables.cpp
  libs/imgui/imgui_widgets.cpp
)

file(GLOB_RECURSE MODULES src/*.cpp)
list(FILTER MODULES EXCLUDE REGEX windalive.cpp)

target_sources(windalive PRIVATE FILE_SET CXX_MODULES FILES ${MODULES})

target_include_directories(windalive PRIVATE 
  src 
  libs/thread-pool/include
  libs/rlImGui
  libs/imgui
)

target_compile_options(windalive PRIVATE
  -Wall -Wextra -Wpedantic
  -Wformat=2 -Wno-unused-parameter
  -Wwrite-strings -Wstrict-prototypes -Wold-style-definition
  -Wredundant-decls -Wnested-externs -Wmissing-include-dirs
  -Wno-format-nonliteral -Wno-missing-field-initializers
)

target_link_libraries(windalive PRIVATE
  raylib
  spdlog
  Boost::container
  ${LIBANGLE}
)

