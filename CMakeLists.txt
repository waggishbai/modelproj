cmake_minimum_required(VERSION 3.20)

project(model)

add_executable(model main.cpp)

add_subdirectory(glad)

add_subdirectory(glfw)
add_subdirectory(glm)

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/objloader)
target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)

target_link_libraries(model glfw glad glm)


