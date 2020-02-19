cmake_minimum_required(VERSION 3.16)

project(LMAGIE LANGUAGES C CXX)

#TODO: extract common code and move it to cmake submodule
if(MSVC AND DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project(LMAGI
  VERSION 0.0.1
  DESCRIPTION "LMAGI Engine"
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(LMAGIE_STANDALONE ON)
endif()

if(LMAGIE_STANDALONE)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
  set(CMAKE_DISABLE_SOURCE_CHANGES ON)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  set(CMAKE_DEBUG_POSTFIX "d")

  if(NOT WIN32)
    set(CMAKE_LINK_WHAT_YOU_USE ON)
  endif()

  if(NOT MSVC)
    find_program(LMAGIE_CCACHE_PROGRAM ccache)
    if(LMAGIE_CCACHE_PROGRAM)
      set(CMAKE_C_COMPILER_LAUNCHER "${LMAGIE_CCACHE_PROGRAM}")
      set(CMAKE_CXX_COMPILER_LAUNCHER "${LMAGIE_CCACHE_PROGRAM}")
      message(STATUS "Using ccache: ${LMAGIE_CCACHE_PROGRAM}")
    endif()
  endif()

endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(BuildType)
include(CompilerType)
include(GetRepoInfo)

set(LMAGIE_ENABLE_TESTS ${LMAGI_STANDALONE} CACHE STRING "Enable unit tests")

if(LMAGIE_ENABLE_TESTS)
	message(STATUS "Tests are not integrated yet")
#	include(CTest)
# 	enable_testing()
#	add_subdirectory( tests )
endif()

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

set(LMAGIE_PUBLIC_HEADERS
  include/gui/app.h)
  
set(LMAGIE_ALL_SOURCES
  ${LMAGIE_PUBLIC_HEADERS}
  source/gui/app.cpp)
  
add_library(LMAGIE STATIC ${LMAGIE_ALL_SOURCES})

target_include_directories(LMAGIE
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  PRIVATE source)
  
target_include_directories(LMAGIE SYSTEM PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/include)

set(CMAKEINSTINCLUDROOT ${CMAKE_CURRENT_SOURCE_DIR}/install)
 
install(TARGETS LMAGIE
  COMPONENT SDK
  EXPORT LMAGIEConfig
  ARCHIVE DESTINATION ${CMAKEINSTINCLUDROOT}/lib
  LIBRARY DESTINATION ${CMAKEINSTINCLUDROOT}/lib
  RUNTIME DESTINATION ${CMAKEINSTINCLUDROOT}/lib)
  
install(FILES ${LMAGIE_PUBLIC_HEADERS}
  COMPONENT SDK
  DESTINATION ${CMAKEINSTINCLUDROOT}/include)
  
install(EXPORT LMAGIEConfig
  COMPONENT SDK
  DESTINATION ${CMAKEINSTINCLUDROOT}/cmake/LMAGIE)
export(TARGETS LMAGIE FILE LMAGIEConfig.cmake)