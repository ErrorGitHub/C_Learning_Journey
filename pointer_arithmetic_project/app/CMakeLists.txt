cmake_minimum_required(VERSION 3.20)

project(
  app
  VERSION 1.0
  LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
set(CMAKE_C_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(${PROJECT_NAME} src/main.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE -Wall
          -Wextra
          -Wpedantic
          -Wshadow
          -Wconversion
          -Wsign-conversion
          -Wnull-dereference
          -Wdouble-promotion
          -Wformat=2
          -Wimplicit-fallthrough
          -Wunused
          -Wstrict-prototypes
          -Wmissing-prototypes
          -Wold-style-definition
          -Wundef
          -Wcast-qual
          -Wwrite-strings
          -Wredundant-decls
          -Wmissing-declarations
          -Wstrict-overflow=5
          $<$<CONFIG:Debug>:-O0
          -g3>
          $<$<CONFIG:Release>:-O3>)

option(ENABLE_WERROR "Treat warnings as errors" ON)
if(ENABLE_WERROR)
  target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
endif()

option(ENABLE_SANITIZERS "Enable sanitizers" OFF)
if(ENABLE_SANITIZERS)
  target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
  target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
endif()

option(ENABLE_STATIC "Build static binary" ON)
if(ENABLE_STATIC)
  target_link_options(${PROJECT_NAME} PRIVATE -static -static-libgcc
                      -static-libstdc++)
endif()

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES C_STANDARD 17
             C_STANDARD_REQUIRED YES
             C_EXTENSIONS NO)
