cmake_minimum_required (VERSION 3.0)
project (Collecto)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(libs)

set(SRC "${Collecto_SOURCE_DIR}/")

file(GLOB_RECURSE SOURCE
  "${SRC}source/*.cpp"
  "${SRC}include/*.hpp"
)

include_directories(
  libs/GuiCode/include
  libs/GuiCode/libs
  include/
)

add_executable(Collecto
  ${SOURCE}
)

source_group(TREE ${SRC} FILES ${SOURCE})

target_precompile_headers(Collecto PUBLIC
  "${SRC}include/pch.hpp"
)

target_link_libraries(Collecto
  GuiCode
)