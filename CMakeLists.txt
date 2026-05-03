cmake_minimum_required(VERSION 3.24)

project(GameEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENGINE_ENABLE_ASSERTS "Enable engine assertions" ON)

add_subdirectory(external/glfw)
add_subdirectory(engine)
add_subdirectory(external/entt)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/box2d)
add_subdirectory(sandbox)