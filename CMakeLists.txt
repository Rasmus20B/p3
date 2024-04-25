cmake_minimum_required(VERSION 3.13)
project(p3)
set(CMAKE_CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED YES)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")

# find_package(raylib 5.0 REQUIRED) # Requires at least version 3.0

add_compile_options(
-Wall
-pedantic
)

add_subdirectory(deps/raylib/)
add_subdirectory(deps/jcast/)
add_subdirectory(deps/ecstasy/)

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(${PROJECT_NAME} engine raylib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
