cmake_minimum_required(VERSION 3.28)
project(NBody)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(NBody
        src/glad.c
        include/imgui/imgui.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/backends/imgui_impl_glfw.cpp
        include/imgui/backends/imgui_impl_opengl3.cpp
        src/main.cpp)

target_include_directories(NBody PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(NBody PRIVATE ${CMAKE_SOURCE_DIR}/include/imgui)
target_include_directories(NBody PRIVATE ${CMAKE_SOURCE_DIR}/include/imgui/backends)

target_link_libraries(NBody PRIVATE OpenGL::GL glfw)